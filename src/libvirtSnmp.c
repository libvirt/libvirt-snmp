/*
 * libvirtSnmp.c: interface between libvirt and the agent
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Author: Michal Privoznik <mprivozn@redhat.com>
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

#include "libvirtSnmp.h"
#include "libvirtSnmpError.h"
#include "libvirtGuestTable.h"      /* include our MIB structures*/
#include "libvirtNotifications.h"

#define DEBUG0(fmt) if (verbose) printf("%s:%d :: " fmt "\n", \
        __func__, __LINE__)
#define DEBUG(fmt, ...) if (verbose) printf("%s:%d: " fmt "\n", \
        __func__, __LINE__, __VA_ARGS__)
#define STREQ(a,b) (strcmp(a,b) == 0)

#ifndef ATTRIBUTE_UNUSED
#define ATTRIBUTE_UNUSED __attribute__((__unused__))
#endif

int verbose = 0;
virConnectPtr conn;
int callbackRet = -1;
int run = 1;
pthread_t poll_thread;

static int
domainEventCallback(virConnectPtr conn ATTRIBUTE_UNUSED,
                    virDomainPtr dom, int event, int detail,
                    void *opaque ATTRIBUTE_UNUSED)
{
    DEBUG("%s EVENT: Domain %s(%d) %d %d\n", __func__,
          virDomainGetName(dom), virDomainGetID(dom), event, detail);

    send_libvirtGuestNotif_trap(dom);
    return 0;
}

static void
myFreeFunc(void *opaque)
{
    if (opaque)
        free(opaque);
}

/* Signal trap function */
static void
stop(int sig)
{
    run = 0;
}

static int
insertGuest(netsnmp_container *container, virDomainPtr domain)
{
    virDomainInfo info;
    libvirtGuestTable_rowreq_ctx *row_ctx = NULL;
    const char *name;
    unsigned char uuid[VIR_UUID_BUFLEN];

    if (virDomainGetInfo(domain, &info) < 0) {
        printLibvirtError("Failed to get domain info");
        goto error;
    }

    /* create new row in the container */
    if (!(row_ctx = libvirtGuestTable_allocate_rowreq_ctx(NULL))) {
        snmp_log(LOG_ERR, "Error creating row");
        goto error;
    }

    /* set the index of the row */
    if (virDomainGetUUID(domain, uuid) < 0) {
        printLibvirtError("Failed to get domain UUID");
        goto error;
    }

    if (libvirtGuestTable_indexes_set(row_ctx,
                                      (char*) uuid,
                                      sizeof(uuid)) != MFD_SUCCESS) {
        snmp_log(LOG_ERR, "Error setting row index");
        goto error;
    }

    /* set the data */
    if (!(name = virDomainGetName(domain))) {
        printLibvirtError("Failed to get domain name");
        goto error;
    }

    if (!(row_ctx->data.libvirtGuestName = strdup(name))) {
        snmp_log(LOG_ERR, "Not enough memory for domain name '%s'", name);
        goto error;
    }

    row_ctx->data.libvirtGuestState = info.state;
    row_ctx->data.libvirtGuestCpuCount = info.nrVirtCpu;
    /* convert the memory to MiB */
    row_ctx->data.libvirtGuestMemoryCurrent = info.memory / 1024;
    row_ctx->data.libvirtGuestMemoryLimit = info.maxMem / 1024;
    row_ctx->data.libvirtGuestCpuTime.high = info.cpuTime >> 32;
    row_ctx->data.libvirtGuestCpuTime.low = info.cpuTime & 0xFFFFFFFF;

    row_ctx->data.libvirtGuestRowStatus = ROWSTATUS_ACTIVE;

    if (CONTAINER_INSERT(container, row_ctx) < 0) {
        snmp_log(LOG_ERR, "Cannot insert domain '%s' to container", name);
        goto error;
    }

    return 0;

 error:
    if (row_ctx)
        libvirtGuestTable_release_rowreq_ctx(row_ctx);
    return -1;
}

/*
 * Populate libvirtGuestTable into given container.
 */
int
libvirtSnmpLoadGuests(netsnmp_container *container)
{
    int ret = -1;
    int i;
    int ndomains;
    virDomainPtr *domains = NULL;

    if ((ndomains = virConnectListAllDomains(conn, &domains, 0)) < 0) {
        printLibvirtError("Failed to list all domains");
        goto cleanup;
    }


    for (i = 0; i < ndomains; i++) {
        if (insertGuest(container, domains[i]) < 0)
            goto cleanup;
    }

    ret = 0;
 cleanup:
    if (domains) {
        for (i = 0; i < ndomains; i++)
            virDomainFree(domains[i]);
        free(domains);
    }
    return ret;
}

/* Polling thread function */
void *
pollingThreadFunc(void *foo)
{
    while (run) {
        if (virEventRunDefaultImpl() < 0) {
            printLibvirtError("Failed to run event loop");
            pthread_exit(NULL);
        }
    }
    return NULL;
}

/* Function to register domain lifecycle events collection */
int
libvirtRegisterEvents(virConnectPtr conn) {
    struct sigaction action_stop;
    int ret = -1;

    memset(&action_stop, 0, sizeof action_stop);

    action_stop.sa_handler = stop;

    sigaction(SIGTERM, &action_stop, NULL);
    sigaction(SIGINT, &action_stop, NULL);

    DEBUG0("Registering domain event callback");

    callbackRet = virConnectDomainEventRegisterAny(conn, NULL,
                                                   VIR_DOMAIN_EVENT_ID_LIFECYCLE,
                                                   VIR_DOMAIN_EVENT_CALLBACK
                                                   (domainEventCallback),
                                                   NULL, myFreeFunc);

    if (callbackRet == -1) {
        printLibvirtError("Failed to register libvirt event handler");
        goto cleanup;
    }

    /* we need a thread to poll for events */
    if (pthread_create(&poll_thread, NULL, pollingThreadFunc, NULL))
        goto cleanup;

    ret = 0;
 cleanup:
    return ret;
}

/* Unregister domain events collection */
int
libvirtUnregisterEvents(virConnectPtr conn)
{
    void *status;

    virConnectDomainEventDeregisterAny(conn, callbackRet);
    callbackRet = -1;
    return 0;
}

int libvirtSnmpInit(void)
{
    char *verbose_env = getenv("LIBVIRT_SNMP_VERBOSE");

    verbose = verbose_env != NULL;

    /* if we don't already have registered callback */
    if (callbackRet == -1)
        virEventRegisterDefaultImpl();

    /* TODO: configure the URI */
    /* Use libvirt env variable LIBVIRT_DEFAULT_URI by default*/
    if (!(conn = virConnectOpenAuth(NULL, virConnectAuthPtrDefault, 0))) {
        printLibvirtError("No connection to hypervisor");
        return -1;
    }

    if ((callbackRet == -1) && libvirtRegisterEvents(conn)) {
        printf("Unable to register domain events\n");
        return -1;
    }

    return 0;
}

static void
libvirtDeinitTimer(int timer ATTRIBUTE_UNUSED, void *opaque ATTRIBUTE_UNUSED)
{
    /* nothing to be done here */
}

void libvirtSnmpShutdown(void)
{
    int timer;
    int rc;

    /* in case server is being stopped, run is still 1, so let's
     * shutdown the thread in a clean way */
    run = 0;

    /* HACK: Add a dummy timeout to break event loop */
    timer = virEventAddTimeout(0, libvirtDeinitTimer, NULL, NULL);
    pthread_join(poll_thread, NULL);

    if (timer != -1)
        virEventRemoveTimeout(timer);

    if (libvirtUnregisterEvents(conn)) {
        printf("Failed to unregister domain events\n");
    }

    if ((rc = virConnectClose(conn))) {
        printLibvirtError("Failed to disconnect from hypervisor. "
                          "Leaked references: %d\n", rc);
    }
}

int
libvirtSnmpCheckDomainExists(unsigned char *uuid)
{
    virDomainPtr d = virDomainLookupByUUID(conn, uuid);
    if (d == NULL)
        return -1;

    virDomainFree(d);
    return 0;
}

int
libvirtSnmpCreate(unsigned char *uuid, int state)
{
    virDomainPtr dom;
    int ret = -1;
    unsigned int flags = 0;

    switch(state) {
    case VIR_DOMAIN_RUNNING:
        break;
    case VIR_DOMAIN_PAUSED:
        flags = VIR_DOMAIN_START_PAUSED;
        break;
    default:
        printf("Can't create domain with state %d\n", flags);
        return -1;
    }

    if (!(dom = virDomainLookupByUUID(conn, uuid))) {
        printf("Cannot find domain to create\n");
        return -1;
    }

    if (virDomainCreateWithFlags(dom, flags) < 0) {
        printLibvirtError("Failed to create domain: %s", virDomainGetName(dom));
        goto cleanup;
    }

    ret = 0;
 cleanup:
    virDomainFree(dom);
    return ret;
}

int
libvirtSnmpDestroy(unsigned char *uuid)
{
    virDomainPtr dom;
    int ret = -1;

    if (!(dom = virDomainLookupByUUID(conn, uuid))) {
        printf("Cannot find domain to destroy\n");
        return -1;
    }

    if (virDomainDestroy(dom) < 0) {
        printLibvirtError("Failed to destroy domain %s", virDomainGetName(dom));
        goto cleanup;
    }

    ret = 0;
 cleanup:
    virDomainFree(dom);
    return 0;
}

int
libvirtSnmpChangeState(unsigned char *uuid, int newstate, int oldstate)
{
    virDomainPtr dom;
    int ret = -1;

    if (!(dom = virDomainLookupByUUID(conn, uuid))) {
        printf("Cannot find domain to change\n");
        return 1;
    }

    if (oldstate == VIR_DOMAIN_RUNNING && newstate == VIR_DOMAIN_PAUSED)
        ret = virDomainSuspend(dom);
    else if (oldstate == VIR_DOMAIN_PAUSED && newstate == VIR_DOMAIN_RUNNING)
        ret = virDomainResume(dom);
    else if (newstate == VIR_DOMAIN_SHUTDOWN)
        ret = virDomainShutdown(dom);
    else {
        printf("Wrong state transition from %d to %d\n", oldstate, newstate);
        goto out;
    }

    if (ret < 0)
        printLibvirtError("Failed to change state of %s", virDomainGetName(dom));
 out:
    virDomainFree(dom);
    return ret;
}
