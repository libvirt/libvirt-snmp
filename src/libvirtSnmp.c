/* This file contains trivial example code to connect to the running
 * hypervisor and gather a few bits of information.  */

/* compile with:

gcc -lvirt activeguests.c

*/

#include <stdio.h>
#include <stdlib.h>

#include "libvirtSnmp.h"
/* include our MIB structures*/
#include "libvirtGuestTable.h"

virConnectPtr conn;

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

int libvirtSnmpInit(void)
{
    /* virConnectOpenAuth is called here with all default parameters,
     * except, possibly, the URI of the hypervisor. */
    /* TODO: configure the URI */
    conn = virConnectOpenAuth("qemu:///system", virConnectAuthPtrDefault, 0);

    if (NULL == conn) {
        printf("No connection to hypervisor\n");
        showError(conn);
        return -1;
    }
    return 0;
}

void libvirtSnmpShutdown(void)
{
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
