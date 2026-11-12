/* The Dreamcast library's (Shinobi) types and calls that the game uses, as its headers declare
   them: the controllers (sg_pad.h) for both programs, and for the Dreamcast's the keyboard, the
   disc, the console's settings and clock, start-up and the modem. */
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

/* what syCblCheck finds on the video cable */
#define SYE_CBL_NTSC	0
#define SYE_CBL_VGA		1
#define SYE_CBL_PAL		2

extern Sint32 syCblCheck( void ) ;

extern void pdInitPeripheral( Sint32 logic, void *recvbuf, void *sendbuf ) ;
extern PDS_PERIPHERAL *pdGetPeripheral( Uint32 port ) ;
extern void pdExecPeripheralServer( void ) ;
extern PDS_PERIPHERALINFO *pdGetPeripheralInfo( Uint32 port ) ;
extern Sint32 pdVibMxIsReady( Uint32 port ) ;
extern Sint32 pdVibMxStart( Uint32 port, PDS_VIBPARAM *param ) ;
extern Sint32 pdVibMxStop( Uint32 port ) ;
extern void pdExitPeripheral( void ) ;

/* the first socket of each of the console's four ports */
#define PDD_PORT_A0	0
#define PDD_PORT_B0	6
#define PDD_PORT_C0	12
#define PDD_PORT_D0	18

/* the keyboard (sg_pdkbd.h) */
#define PDD_DEVTYPE_KEYBOARD	0x40

typedef struct PDS_KEYBOARDINFO {
	Uint8	lang;
	Uint8	type;
	Uint8	led;
	Uint8	led_ctrl;
} PDS_KEYBOARDINFO;

typedef struct PDS_KEYBOARD {
	Uint8	ctrl;	/* 000: shift, control and the like */
	Uint8	led;
	Uint8	key[6];	/* 002: the codes of the keys held */
	PDS_KEYBOARDINFO	*info;
} PDS_KEYBOARD;

extern void pdKbdInit( void ) ;
extern void pdKbdExit( void ) ;
extern const PDS_KEYBOARD *pdKbdGetData( Uint32 port ) ;

/* the disc's files (gdFs, sg_gd.h) */
typedef struct GDS_FS_HANDLE *GDFS ;	/* an open file */
typedef void ( *GDFS_ERRFUNC )( void *obj, Sint32 err ) ;

#define GDD_SCT_SIZE	2048	/* a sector: files are read in whole sectors */

/* what a file is doing (gdFsGetStat) */
#define GDD_STAT_IDLE	0
#define GDD_STAT_COMPLETE	1
#define GDD_STAT_READ	2
#define GDD_STAT_SEEK	3
#define GDD_STAT_BUSY	4
#define GDD_STAT_ERR	5

/* the errors that mean the disc is gone */
#define GDD_ERR_OK	0
#define GDD_ERR_TRAYOPEND	( -23 )	/* the door was opened */
#define GDD_ERR_UNITATTENT	( -33 )	/* the disc was changed */

/* the drive (gdFsGetDrvStat) */
#define GDD_DRVSTAT_OPEN	6
#define GDD_DRVSTAT_NODISC	7

extern Sint32 gdFsInit( Sint32 max_open, void *work, Sint32 max_dirent, void *dirbuf ) ;
extern void gdFsFinish( void ) ;
extern GDFS gdFsOpen( const char *fname, void *dirrec ) ;
extern void gdFsClose( GDFS gdfs ) ;
extern Sint32 gdFsGetFileSize( GDFS gdfs, Sint32 *fsize ) ;
extern Sint32 gdFsRead( GDFS gdfs, Sint32 nsct, void *buf ) ;	/* waits for the sectors */
extern Sint32 gdFsReqRd32( GDFS gdfs, Sint32 nsct, void *buf ) ;	/* asks for them, to a buffer on a 32-byte boundary */
extern Sint32 gdFsGetStat( GDFS gdfs ) ;
extern GDFS gdFsGetSysHn( void ) ;	/* the library's own handle, which the drive requests use */
extern Sint32 gdFsGetDrvStat( void ) ;
extern Sint32 gdFsReqDrvStat( void ) ;
extern void gdFsEntryErrFuncAll( GDFS_ERRFUNC func, void *obj ) ;

/* the console's settings in its flash memory (sg_sycfg.h) */
#define SYD_CFG_STEREO	0
#define SYD_CFG_MONO	1
#define SYD_CFG_IID_SIZE	6	/* the bytes of a console's own number */

extern Sint32 syCfgInit( void *work ) ;
extern Sint32 syCfgExit( void ) ;
extern Sint32 syCfgSetSoundMode( Sint32 mode ) ;
extern Sint32 syCfgGetSoundMode( Sint32 *mode ) ;
extern Sint32 syCfgGetIndividualID( Sint8 *id ) ;

/* the clock (sg_syrtc.h) */
typedef struct SYS_RTC_DATE {
	Uint16	year;
	Uint8	month;
	Uint8	day;
	Uint8	hour;		/* 004 */
	Uint8	minute;
	Uint8	second;
	Uint8	dayofweek;
	Uint8	ageofmoon;
} SYS_RTC_DATE;

extern Sint32 syRtcInit( void ) ;
extern Sint32 syRtcGetDate( SYS_RTC_DATE *date ) ;

/* start-up and the way out, the heap, the boot ROM's character codes */
extern void *_ISTACK_END ;	/* the top of the interrupts' stack, from the start-up module */
extern void syStartGlobalConstructor( void ) ;
extern void syStartGlobalDestructor( void ) ;
extern void syHwInit( void ) ;
extern void syHwInit2( void ) ;
extern void syHwFinish( void ) ;
extern void syMallocInit( void *heap, Uint32 size ) ;
extern void syMallocFinish( void ) ;
extern void syBtExit( void ) ;	/* back to the console's own menu */
extern Sint16 syBtFntSjis2Jis( Sint16 sjis ) ;

/* the modem (mdmAPI.h) and the console's network settings (Nt_utl.h) */
typedef Uint32 MDM ;	/* an open modem */

#define NTD_ERR_NOINFO	( -3 )	/* no provider has been set up */

extern void mdmClose( MDM handle ) ;
extern void mdmFinish( void ) ;
extern Sint32 ntInfIsNetworkInfo( void ) ;

#endif
