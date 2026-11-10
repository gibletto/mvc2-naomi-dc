/* sg_chain.h: Shinobi's interrupt chain, as the libraries call it. */
#ifndef SG_CHAIN_H
#define SG_CHAIN_H

#include "types.h"

#define SYD_CHAIN_EVENT_IML6_VBLANK	0x0900
#define SYD_CHAIN_EVENT_IML4_MPDMAE	0x0A00
#define SYD_CHAIN_EVENT_IML4_MPVOVR	0x0A20

typedef Sint16 SY_CHAIN_EVENT;
typedef Uint32 SYCHAIN;

extern SYCHAIN syChainAddHandler( SY_CHAIN_EVENT event, void ( *usrIsr )( void * ), Uint32 priority, void *arg );
extern void syChainDeleteHandler( SYCHAIN chain );

#endif
