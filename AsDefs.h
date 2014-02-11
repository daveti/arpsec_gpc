#ifndef AsDefs_INCLUDED
#define AsDefs_INCLUDED

////////////////////////////////////////////////////////////////////////////////
//
//  File          : AsDefs.h
//  Description   : These are the basic data types for the arpsec deamon.
//
//  Author  : Patrick McDaniel
//  Created : Tue Mar 26 13:06:54 EDT 2013
//  Dev	    : daveti Aug 21 2013
//

// Includes
#include <time.h>
#include <stdlib.h>

//
// Defines
#define MAX_MEDADDR_LENGTH 128
#define MAX_NETADDR_LENGTH 128
#define HW_ADDR_ANY "HWANY"

// Below are moved from AsKrnRelay.h to
// 1. fix the header file interaction issue
// 2. avoid malloc within the askRelayMessage
// Feb 10, 2014
// daveti
#define ARPSEC_ETH_ALEN                 6  // Length of ethernet (hardware) address
#define ARPSEC_IPV4_ALEN                4  // Length of IPv4 address
#define ARPSEC_ARP_16BIT                2  // 16-bit used by ARP
// Linux Kernel ARP/RARP msg data structure
typedef struct _arpmsg {
	unsigned char   ar_hrd[ARPSEC_ARP_16BIT];	/* format of hardware address   */
	unsigned char   ar_pro[ARPSEC_ARP_16BIT];	/* format of protocol address   */
	unsigned char   ar_hln;				/* length of hardware address   */
	unsigned char   ar_pln;				/* length of protocol address   */
	unsigned char   ar_op[ARPSEC_ARP_16BIT];	/* ARP opcode (command)         */
	unsigned char	ar_sha[ARPSEC_ETH_ALEN];	/* sender hardware address      */
	unsigned char	ar_sip[ARPSEC_IPV4_ALEN];	/* sender IP address            */
	unsigned char	ar_tha[ARPSEC_ETH_ALEN];	/* target hardware address      */
	unsigned char	ar_tip[ARPSEC_IPV4_ALEN];	/* target IP address            */
} arpsec_arpmsg;
// arpsec kernel relay raw message structure
typedef struct _arpsec_rlmsg {
	arpsec_arpmsg   arpsec_arp_msg;
	void            *arpsec_dev_ptr;
} arpsec_rlmsg;


//
// Type definitions

typedef time_t	AsTime; 			// The time component of our logic (T)
typedef char *	AsSystem;			// The system set of the logic (S)
typedef char *  AsMediaAddress;		// The media (MAC) addresses
typedef	char *  AsNetworkAddress;	// The network (IP) addresses

// ARP Definitions
typedef enum {
    RFC_826_ARP_REQ = 0,	// ARP Request
    RFC_826_ARP_RES = 1,	// ARP Response
    RFC_903_ARP_RREQ = 2,	// ARP Reverse Request
    RFC_903_ARP_RRES = 3,	// ARP Reverse Response
} ArpOpcode;

// Ask Message Type
typedef struct {
    AsSystem		source;	// This is the (suspected) source system
    AsMediaAddress 	sndr;	// The sender of the message
    AsMediaAddress 	dest;	// The destination HW address of message
    AsNetworkAddress	sndr_net;	// daveti: save the sender's IP here
    AsNetworkAddress	dest_net;	// daveti: save the target's IP here
    unsigned		op;	// The message type (ArpOpcode)
    union {
	AsNetworkAddress    network;	// Network address to lookup
	AsMediaAddress	    media;	// Media address to reverse lookup
    } target;
    union {
	AsNetworkAddress    network;	// Network address to bind to
	AsMediaAddress	    media;	// Media address to bind to
    } binding;

    arpsec_rlmsg	rlmsg;		// daveti: Save the whole rlmsg to ease the future processing

} askRelayMessage;

// A random function
#define as_random(x) (int)((float)rand()/((float)RAND_MAX)*((float)x))

//
// Global data

extern char *askOpCodeStrings[4]; // Message type strings

#endif
