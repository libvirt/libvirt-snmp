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
#include <sys/types.h>
#include <sys/poll.h>
#include <pthread.h>
#include <signal.h>

#include "libvirtSnmp.h"
#include "libvirtGuestTable.h"      /* include our MIB structures*/
#include "libvirtNotifications.h"
#ifdef LIBVIRT_OLD
# include "event_poll.h"
#endif

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

static void
showError(virConnectPtr conn)
{
    int ret;
    virErrorPtr err;

    err = malloc(sizeof(*err));
    if (NULL == err) {
        printf("Could not allocate memory for error data\n");
        goto out;
    }

    ret = virConnCopyLastError(conn, err);

    switch (ret) {
    case 0:
        snmp_log(LOG_ERR, "No error found\n");
        break;

    case -1:
    	snmp_log(LOG_ERR, "Parameter error when attempting to get last error\n");
        break;

    default:
    	snmp_log(LOG_ERR, "libvirt reported: \"%s\"\n", err->message);
        break;
    }

    virResetError(err);
    free(err);

out:
    return;
}

/*
 * Populate libvirtGuestTable into given container.
 */
int
libvirtSnmpLoadGuests(netsnmp_container *container)
{
    int ret = 0, i, numIds, numActiveDomains;
    int *idList = NULL;
    virDomainPtr domain = NULL;
    virDomainInfo info;
    libvirtGuestTable_rowreq_ctx *row_ctx = NULL;
    const char *name = NULL;

    numActiveDomains = virConnectNumOfDomains(conn);
    if (-1 == numActiveDomains) {
        ret = -1;
        printf("Failed to get number of active domains\n");
        showError(conn);
        goto out;
    }

    idList = malloc(sizeof(*idList) * numActiveDomains);

    if (NULL == idList) {
        ret = -1;
        printf("Could not allocate memory for list of active domains\n");
        goto out;
    }

    numIds = virConnectListDomains(conn,
				     idList,
				     numActiveDomains);

    if (-1 == numIds) {
        ret = -1;
        printf("Could not get list of defined domains from hypervisor\n");
        showError(conn);
        goto out;
    }

    for (i = 0 ; i < numIds ; i++) {
    	unsigned char uuid[16];

        domain = virDomainLookupByID(conn, *(idList + i));
        if (NULL == domain) {
            printf("Failed to lookup domain\n");
            showError(conn);
           	ret = -1;
            goto out;
        }

        if (-1 == virDomainGetInfo(domain, &info)) {
            printf("Failed to get domain info\n");
            showError(conn);
            virDomainFree(domain);
           	ret = -1;
            goto out;
        }

        /* create new row in the container */
        row_ctx = libvirtGuestTable_allocate_rowreq_ctx(NULL);
        if (!row_ctx) {
        	virDomainFree(domain);
        	snmp_log(LOG_ERR, "Error creating row");
           	ret = -1;
        	goto out;
        }
        /* set the index of the row */
        ret = virDomainGetUUID(domain, uuid);
        if (ret) {
        	virDomainFree(domain);
           	snmp_log(LOG_ERR, "Cannot get UUID");
           	libvirtGuestTable_release_rowreq_ctx(row_ctx);
           	ret = -1;
           	goto out;
    	}
        if (MFD_SUCCESS != libvirtGuestTable_indexes_set(row_ctx, (char*) uuid,
        		sizeof(uuid))) {
        	virDomainFree(domain);
        	snmp_log(LOG_ERR, "Error setting row index");
        	libvirtGuestTable_release_rowreq_ctx(row_ctx);
           	ret = -1;
        	goto out;
        }

        /* set the data */
        name = virDomainGetName(domain);
        if (name)
        	row_ctx->data.libvirtGuestName = strdup(name);
        else
        	row_ctx->data.libvirtGuestName = strdup("");
        if (!row_ctx->data.libvirtGuestName) {
           	snmp_log(LOG_ERR, "Not enough memory for domain name '%s'", name);
           	libvirtGuestTable_release_rowreq_ctx(row_ctx);
           	ret = -1;
           	goto out;
        }

        row_ctx->data.libvirtGuestState = info.state;
        row_ctx->data.libvirtGuestCpuCount = info.nrVirtCpu;
        /* convert the memory to MiB */
        row_ctx->data.libvirtGuestMemoryCurrent = info.memory / 1024;
        row_ctx->data.libvirtGuestMemoryLimit = info.maxMem / 1024;
        row_ctx->data.libvirtGuestCpuTime.high = info.cpuTime >> 32;
        row_ctx->data.libvirtGuestCpuTime.low = info.cpuTime & 0xFFFFFFFF;

        row_ctx->data.libvirtGuestRowStatus = ROWSTATUS_ACTIVE;
        virDomainFree(domain);

        ret = CONTAINER_INSERT(container, row_ctx);
        if (ret) {
           	snmp_log(LOG_ERR, "Cannot insert domain '%s' to container", name);
           	libvirtGuestTable_release_rowreq_ctx(row_ctx);
           	ret = -1;
           	goto out;
    	}

    }

out:
    free(idList);
    return ret;
}

/* Polling thread function */
void *
pollingThreadFunc(void *foo) {
    while (run) {
#ifdef LIBVIRT_OLD
        if (virEventPollRunOnce() < 0) {
#else
        if (virEventRunDefaultImpl() < 0) {
#endif
            showError(conn);
            pthread_exit(NULL);
        }
    }
    return NULL;
}

/* Function to register domain lifecycle events collection */
int
libvirtRegisterEvents(virConnectPtr conn) {
    struct sigaction action_stop;
    pthread_attr_t thread_attr;

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

    if (callbackRet == -1)
        return -1;

    /* we need a thread to poll for events */
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);

    if (pthread_create
        (&poll_thread, &thread_attr, pollingThreadFunc, NULL))
        return -1;

    pthread_attr_destroy(&thread_attr);

    return 0;
}

/* Unregister domain events collection */
int
libvirtUnregisterEvents(virConnectPtr conn)
{
    void *status;

    pthread_join(poll_thread, &status);
    virConnectDomainEventDeregisterAny(conn, callbackRet);
    callbackRet = -1;
    return 0;
}

int libvirtSnmpInit(void)
{
    char *verbose_env = getenv("LIBVIRT_SNMP_VERBOSE");

    verbose = verbose_env != NULL;

    /* if we don't already have registered callback */
    if (callbackRet == -1) {
#ifdef LIBVIRT_OLD
        if (virEventPollInit() < 0)
            return -1;

        virEventRegisterImpl(
            virEventPollAddHandle,
            virEventPollUpdateHandle,
            virEventPollRemoveHandle,
            virEventPollAddTimeout,
            virEventPollUpdateTimeout,
            virEventPollRemoveTimeout);
#else
        virEventRegisterDefaultImpl();
#endif
    }

    /* TODO: configure the URI */
    /* Use libvirt env variable LIBVIRT_DEFAULT_URI by default*/
    conn = virConnectOpenAuth(NULL, virConnectAuthPtrDefault, 0);

    if (NULL == conn) {
        printf("No connection to hypervisor\n");
        showError(conn);
        return -1;
    }

    if ((callbackRet == -1) && libvirtRegisterEvents(conn)) {
        printf("Unable to register domain events\n");
        return -1;
    }

    return 0;
}

void libvirtSnmpShutdown(void)
{
    if (libvirtUnregisterEvents(conn)) {
        printf("Failed to unregister domain events\n");
    }

    if (0 != virConnectClose(conn)) {
        printf("Failed to disconnect from hypervisor\n");
        showError(conn);
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
	int ret;
	unsigned int flags = 0;

	dom = virDomainLookupByUUID(conn, uuid);
	if (dom == NULL) {
		printf("Cannot find domain to create\n");
		return -1;
	}

	switch(state) {
	case VIR_DOMAIN_RUNNING:
		flags = 0;
		break;
	case VIR_DOMAIN_PAUSED:
		flags = VIR_DOMAIN_START_PAUSED;
		break;
	default:
		printf("Can't create domain with state %d\n", flags);
		return -1;
	}

	ret = virDomainCreateWithFlags(dom, flags);
	if (ret) {
		showError(conn);
	}
	virDomainFree(dom);
	return ret;
}

int
libvirtSnmpDestroy(unsigned char *uuid)
{
	virDomainPtr dom;
	int ret;

	dom = virDomainLookupByUUID(conn, uuid);
	if (dom == NULL) {
		printf("Cannot find domain to destroy\n");
		return -1;
	}

	ret = virDomainDestroy(dom);
	if (ret) {
		showError(conn);
	}
	virDomainFree(dom);
	return ret;
}

int
libvirtSnmpChangeState(unsigned char *uuid, int newstate, int oldstate)
{
	virDomainPtr dom;
	int ret = 0;

	dom = virDomainLookupByUUID(conn, uuid);
	if (dom == NULL) {
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
		ret = -1;
		goto out;
	}

	if (ret != 0) {
		showError(conn);
	}
out:
	virDomainFree(dom);
	return ret;
}
