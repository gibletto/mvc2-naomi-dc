/* The Dreamcast library's (Shinobi) controller types that the game uses, as its sg_pad.h declares
   them. */
#ifndef SHINOBI_H
#define SHINOBI_H

#include "types.h"

typedef struct PDS_PERIPHERALINFO {
	Uint32	type;
	Uint32	reserved[3];
	Uint8	is_root;
	Uint8	area_code;
	Uint8	connector_dir[2];
	char	product_name[32];
	char	license[64];
	Uint16	stdby_pow;
	Uint16	max_pow;
} PDS_PERIPHERALINFO;

typedef struct PDS_PERIPHERAL {
	Uint32	id;
	Uint32	support;
	Uint32	on;
	Uint32	off;
	Uint32	press;
	Uint32	release;
	Uint16	r;
	Uint16	l;
	Sint16	x1;
	Sint16	y1;
	Sint16	x2;
	Sint16	y2;
	char	*name;
	void	*extend;
	Uint32	old;
	PDS_PERIPHERALINFO	*info;
} PDS_PERIPHERAL;

typedef struct PDS_VIBPARAM {
	Uint8	unit;
	Uint8	flag;
	Sint8	power;
	Uint8	freq;
	Uint8	inc;
	Uint8	reserved[3];
} PDS_VIBPARAM;

#define PDD_DEVTYPE_CONTROLLER	1
#define PDD_DEVTYPE_VIBRATION	0x100

#define PDD_DGT_TC	( 1 << 0 )
#define PDD_DGT_TB	( 1 << 1 )
#define PDD_DGT_TA	( 1 << 2 )
#define PDD_DGT_ST	( 1 << 3 )
#define PDD_DGT_KU	( 1 << 4 )
#define PDD_DGT_KD	( 1 << 5 )
#define PDD_DGT_KL	( 1 << 6 )
#define PDD_DGT_KR	( 1 << 7 )
#define PDD_DGT_TZ	( 1 << 8 )
#define PDD_DGT_TY	( 1 << 9 )
#define PDD_DGT_TX	( 1 << 10 )
#define PDD_DGT_TR	( 1 << 16 )
#define PDD_DGT_TL	( 1 << 17 )

extern void pdInitPeripheral( Sint32 logic, void *recvbuf, void *sendbuf ) ;
extern PDS_PERIPHERAL *pdGetPeripheral( Uint32 port ) ;
extern void pdExecPeripheralServer( void ) ;
extern PDS_PERIPHERALINFO *pdGetPeripheralInfo( Uint32 port ) ;
extern Sint32 pdVibMxIsReady( Uint32 port ) ;
extern Sint32 pdVibMxStart( Uint32 port, PDS_VIBPARAM *param ) ;
extern Sint32 pdVibMxStop( Uint32 port ) ;

#endif
