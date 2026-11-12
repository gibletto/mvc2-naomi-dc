/* syChain_: Shinobi's interrupt chain, as the disc has it (syChain 1.50 of May 1999, built with the
   library's own options): each hardware event keeps a list of handlers in priority order, and one
   master handler per event runs the list. */
#include "types.h"
#include "syreg.h"

#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()
#define get_vbr()	_builtin_get_vbr()

/* the handler slot of an event in the vector table */
#define USER_VECT( ev )	( *(Uint32 *)( (Uint32)get_vbr() + 0x200 + ( ( ( ev ) - 0x200 ) >> 3 ) ) )

#define SYD_CHAIN_MAX_EVENT	( 0x0C00 >> 4 )
#define SYD_CHAIN_MAX_ID	256
#define SYD_CHAIN_VALID		0x80000000

#define SYD_CHAIN_EVENT_IML6_NRM	0x0900
#define SYD_CHAIN_EVENT_IML6_VBLANK	0x0900
#define SYD_CHAIN_EVENT_IML6_HBLANK	0x0920
#define SYD_CHAIN_EVENT_IML4_NRM	0x0A00
#define SYD_CHAIN_EVENT_IML4_EXT	0x0B00
#define SYD_INT_EVENT_IRL4		0x0360

/* the system bus's interrupt registers, as words from the first */
#define INTBASE		( (Uint32 *)SB_ISTNRM )
#define ISTNRM		( INTBASE + 0 )
#define ISTEXT		( INTBASE + 1 )
#define IML4NRM		( INTBASE + 8 )
#define IML4EXT		( INTBASE + 9 )

typedef Sint16 SY_CHAIN_EVENT;
typedef Uint32 SYCHAIN;

typedef struct SYS_CHAIN_ISR {
	void ( *usrIsr )( void * );
	struct SYS_CHAIN_ISR *next;
	void *arg;
	Uint32 priority;
} SYS_CHAIN_ISR;

typedef struct {
	Sint8	major;
	Sint8	minor;
	Sint8	debug;
	Sint8	rsv;
} SY_VERSION;

extern void kmSetVSyncCallback( void ( *callback )( void * ), void *arg );
extern void kmSetHSyncCallback( void ( *callback )( void * ), void *arg );
extern void syIntBrkRteRte( void );

const char * const sygChainBuild = "\nsyChain Ver 1.50 Build:" "May 31 1999" " " "16:35:21" "\n";
const SY_VERSION sygChainVer = { 1, 5, 0, 0 };

SYS_CHAIN_ISR sygChainIsrPool[SYD_CHAIN_MAX_ID];
SYS_CHAIN_ISR *sygChainKeyTbl[SYD_CHAIN_MAX_EVENT];

void syChainMasterHandler( SY_CHAIN_EVENT event );
void syChainMasterHandlerPVR_V( void *arg );
void syChainMasterHandlerPVR_H( void *arg );
void syChainClx1Handler( SY_CHAIN_EVENT event );

void syChainInit( void )
{
	Uint32 i;
	Uint16 imask;

	imask = get_imask();
	set_imask( 15 );
	for( i = 0 ; i < SYD_CHAIN_MAX_ID ; i++ ) {
		sygChainIsrPool[i].priority = 0;
	}
	for( i = 0 ; i < SYD_CHAIN_MAX_EVENT ; i++ ) {
		sygChainKeyTbl[i] = (void *)0;
	}
	set_imask( imask );
}

SYCHAIN syChainAddHandler( SY_CHAIN_EVENT event, void ( *usrIsr )( void * ), Uint32 priority, void *arg )
{
	SYS_CHAIN_ISR *ptr, *oldptr;
	SYCHAIN ret;
	Uint32 i;
	Uint16 imask;

	imask = get_imask();
	set_imask( 15 );
	for( ptr = sygChainKeyTbl[event >> 4], oldptr = ptr ;
	     ( ptr != 0 ) && ( ptr->priority < ( priority | SYD_CHAIN_VALID ) ) ;
	     oldptr = ptr, ptr = ptr->next ) {
	}
	for( i = 1 ; i < SYD_CHAIN_MAX_ID ; i++ ) {
		if( ( sygChainIsrPool[i].priority & SYD_CHAIN_VALID ) == 0 ) {
			break;
		}
	}
	if( i < SYD_CHAIN_MAX_ID ) {
		if( ( ptr == 0 ) && ( oldptr == 0 ) ) {
			if( event < SYD_CHAIN_EVENT_IML6_NRM ) {
				USER_VECT( event ) = (Uint32)syChainMasterHandler;
			} else if( event == SYD_CHAIN_EVENT_IML6_VBLANK ) {
				kmSetVSyncCallback( syChainMasterHandlerPVR_V, 0 );
			} else if( event == SYD_CHAIN_EVENT_IML6_HBLANK ) {
				kmSetHSyncCallback( syChainMasterHandlerPVR_H, 0 );
			} else {
				USER_VECT( SYD_INT_EVENT_IRL4 ) = (Uint32)syChainClx1Handler;
			}
			sygChainKeyTbl[event >> 4] = &sygChainIsrPool[i];
			sygChainIsrPool[i].next = 0;
			sygChainIsrPool[i].usrIsr = usrIsr;
			sygChainIsrPool[i].arg = arg;
			sygChainIsrPool[i].priority = priority | SYD_CHAIN_VALID;
			if( ( event >> 8 ) == ( SYD_CHAIN_EVENT_IML4_NRM >> 8 ) ) {
				*IML4NRM |= ( 1 << ( ( ( event & 0x00FF ) >> 5 ) + 12 ) );
			} else if( ( event >> 8 ) == ( SYD_CHAIN_EVENT_IML4_EXT >> 8 ) ) {
				*IML4EXT |= ( 1 << ( ( event & 0x00FF ) >> 5 ) );
			}
		} else if( ptr == sygChainKeyTbl[event >> 4] ) {
			sygChainIsrPool[i].next = ptr;
			sygChainKeyTbl[event >> 4] = &sygChainIsrPool[i];
			sygChainIsrPool[i].usrIsr = usrIsr;
			sygChainIsrPool[i].arg = arg;
			sygChainIsrPool[i].priority = priority | SYD_CHAIN_VALID;
		} else {
			oldptr->next = &sygChainIsrPool[i];
			sygChainIsrPool[i].next = ptr;
			sygChainIsrPool[i].usrIsr = usrIsr;
			sygChainIsrPool[i].arg = arg;
			sygChainIsrPool[i].priority = priority | SYD_CHAIN_VALID;
		}
		ret = i;
	} else {
		ret = 0;
	}
	set_imask( imask );
	return ret;
}

void syChainMasterHandler( SY_CHAIN_EVENT event )
{
	SYS_CHAIN_ISR *ptr;

	for( ptr = sygChainKeyTbl[event >> 4] ; ptr != 0 ; ptr = ptr->next ) {
		ptr->usrIsr( ptr->arg );
	}
}

void syChainMasterHandlerPVR_V( void *arg )
{
	SYS_CHAIN_ISR *ptr;
	SY_CHAIN_EVENT event;

	set_imask( 6 );
	event = SYD_CHAIN_EVENT_IML6_VBLANK;
	for( ptr = sygChainKeyTbl[event >> 4] ; ptr != 0 ; ptr = ptr->next ) {
		ptr->usrIsr( ptr->arg );
	}
}

void syChainMasterHandlerPVR_H( void *arg )
{
	SYS_CHAIN_ISR *ptr;
	SY_CHAIN_EVENT event;

	set_imask( 6 );
	event = SYD_CHAIN_EVENT_IML6_HBLANK;
	for( ptr = sygChainKeyTbl[event >> 4] ; ptr != 0 ; ptr = ptr->next ) {
		ptr->usrIsr( ptr->arg );
	}
}

void syChainClx1Handler( SY_CHAIN_EVENT event )
{
	Uint32 ist;
	Uint32 i;

	switch( event >> 4 ) {
	case 0x32:
		break;
	case 0x36:
		set_imask( 4 );
		ist = ( *ISTNRM & *IML4NRM ) >> 12;
		if( ist != 0 ) {
			for( i = 0 ; ( 1 << i ) != 0x80 ; i++ ) {
				if( ist & ( 1 << i ) ) {
					syChainMasterHandler( SYD_CHAIN_EVENT_IML4_NRM + ( i << 5 ) );
				}
			}
		} else {
			ist = *ISTEXT & *IML4EXT;
			for( i = 0 ; ( 1 << i ) != 0x10 ; i++ ) {
				if( ist & ( 1 << i ) ) {
					syChainMasterHandler( SYD_CHAIN_EVENT_IML4_EXT + ( i << 5 ) );
				}
			}
		}
		break;
	case 0x3A:
		break;
	}
}

void syChainDeleteHandler( SYCHAIN chainId )
{
	Uint32 i;
	Uint16 imask;

	imask = get_imask();
	set_imask( 15 );
	for( i = 1 ; i < SYD_CHAIN_MAX_ID ; i++ ) {
		if( ( sygChainIsrPool[i].next == &sygChainIsrPool[chainId] ) &&
		    ( sygChainIsrPool[i].priority & SYD_CHAIN_VALID ) ) {
			break;
		}
	}
	if( i != SYD_CHAIN_MAX_ID ) {
		sygChainIsrPool[i].next = sygChainIsrPool[chainId].next;
		sygChainIsrPool[chainId].priority = 0;
	} else {
		for( i = 1 ; i < SYD_CHAIN_MAX_EVENT ; i++ ) {
			if( sygChainKeyTbl[i] == &sygChainIsrPool[chainId] ) {
				sygChainKeyTbl[i] = sygChainIsrPool[chainId].next;
				sygChainIsrPool[chainId].priority = 0;
				if( sygChainKeyTbl[i] == 0 ) {
					if( i == ( SYD_CHAIN_EVENT_IML6_VBLANK >> 4 ) ) {
						kmSetVSyncCallback( 0, 0 );
					} else if( i == ( SYD_CHAIN_EVENT_IML6_HBLANK >> 4 ) ) {
						kmSetHSyncCallback( 0, 0 );
					} else {
						SY_CHAIN_EVENT event;

						event = i << 4;
						if( ( event >> 8 ) == ( SYD_CHAIN_EVENT_IML4_NRM >> 8 ) ) {
							*IML4NRM &= ~( 1 << ( ( ( event & 0x00FF ) >> 5 ) + 12 ) );
						} else if( ( event >> 8 ) == ( SYD_CHAIN_EVENT_IML4_EXT >> 8 ) ) {
							*IML4EXT &= ~( 1 << ( ( event & 0x00FF ) >> 5 ) );
						}
						USER_VECT( i << 4 ) = (Uint32)&syIntBrkRteRte;
					}
				}
				break;
			}
		}
		if( i == SYD_CHAIN_MAX_EVENT ) {
			/* unknown handler: the debug build reports it here */
		}
	}
	set_imask( imask );
}
