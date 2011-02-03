#ifndef __VIR_SNMP_H__
#define __VIR_SNMP_H__

/* standard libvirt includes */
#include <libvirt/libvirt.h>
#include <libvirt/virterror.h>

/* standard Net-SNMP includes */
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <net-snmp/agent/net-snmp-agent-includes.h>

/*
 * Populate libvirtGuestTable into given container.
 */
extern int
libvirtSnmpLoadGuests(netsnmp_container *container);

extern int
libvirtSnmpInit(void);

extern void
libvirtSnmpShutdown(void);

/**
 * Check that domain with given UUID exists.
 * Return 0 if so, -1 if not.
 */
extern int
libvirtSnmpCheckDomainExists(unsigned char *uuid);

extern int
libvirtSnmpCreate(unsigned char *uuid, int state);

extern int
libvirtSnmpDestroy(unsigned char *uuid);

extern int
libvirtSnmpChangeState(unsigned char *uuid, int newstate, int oldstate);

#endif /* __VIR_SNMP_H__ */

