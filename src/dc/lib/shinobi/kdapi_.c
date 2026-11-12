/* kdapi_: Shinobi's Maple library (kaede) API, as the disc has it (kd 1.48 of July 1999, before
   the arcade's application hooks): the interrupt handlers that drive the Maple driver, the device
   and command calls, and the light gun support. */
#define KD_DC	/* the console's kd */

#include "types.h"
#include "syreg.h"
#include "kaede.h"
#include "mpdrv.h"
#include "sg_chain.h"

#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()
#define tas( p )	_builtin_tas( p )

#define TRUE	1

extern void *memset( void *s, int c, unsigned long n );

#define ISTNRM	( *(volatile Uint32 *)SB_ISTNRM )
#define ISTERR	( *(volatile Uint32 *)SB_ISTERR )
#define IML4NRM	( *(volatile Uint32 *)SB_IML4NRM )

#define SCANLINE	( ( *(volatile Uint32 *)PVR_SPG_STATUS ) & 0x3FF )
#define VBINLINE	( ( *(volatile Uint32 *)PVR_SPG_VBLANK ) & 0x3FF )

#define VO_BORDER	( *(volatile Uint32 *)PVR_VO_BORDER_COL )
#define VO_CTRL		( *(volatile Uint32 *)PVR_VO_CONTROL )

#define SPG_TRIGGER_H	( ( *(volatile Sint32 *)PVR_SPG_TRIGGER_POS ) & 0x3FF )
#define SPG_TRIGGER_V	( ( *(volatile Sint32 *)PVR_SPG_TRIGGER_POS >> 16 ) & 0x3FF )

/* a receive interrupt that came while a command was being set is served now */
#define CHKIRQ()	if( _mp->DmaEndIrqOccured ) kdRecvFrame()

/* take and give back the driver's semaphore (a test-and-set byte) */
#define GETSMP() \
	if( !tas( &*_mp_SemaphoreAddress ) ) { \
		mpdrv_dpf( "***FATAL ERROR:Cannot get semaphore!\n" ); \
		return -2; \
	}
#define RELSMP()	( *_mp_SemaphoreAddress = 0 )

typedef struct {
	Sint32	dev;
	Sint32	seqno;
	KD_CALLBACK	func;
} CBTBL;

SYCHAIN kdgChID[4];
const Uint32 kdgChLevel[] = {
	0x38,	/* vblank in */
	0x01,	/* Maple DMA end */
	0x40,	/* Maple DMA end, clearing the cause */
	0x01,	/* vblank over */
};

Uint32 kdgHandler[32];

const char *kdgBuild = "\nkd Ver 1.48 Build:" "Jul 30 1999" " " "14:59:52" "\n";

KDS_DEVICE kdgDevice[MP_DEV_MAX];
KDS_SYSTEMINFO kdgSystemInfo;

void ( *kdgErrFunc )( Sint32 code, Sint32 err, Sint32 frame, Sint32 param );
Sint32 ( *kdgGunCallback )( void );

CBTBL kdgCbTbl[MP_DEV_MAX * 2];

Uint32 kdgInitialized;
Uint32 kdgSurface;
Uint32 kdgBorder;
Uint32 kdgFlashFlag;
Uint32 kdgGunReadPort;
Sint32 kdgGunLatchedPort;

const Sint32 kdgGunModeTimeout = 300;

Sint32 kdgGunReqPort;
Sint32 kdgGunPosition[2];
Uint32 kdgGunFlashTime[2];
Uint32 kdgGunFlashCount;
Uint32 kdgDmaType;
Uint32 kdgSDCKBCount;

Uint8 kdgSDCKBEnableWork[32 + 32];

Uint8 kdgTriggerOld[4];
Uint8 kdgTriggerNew[4];

void kdExecGun( void );

void kdVbInHandler( void *param )
{
	kdgSystemInfo.vbin_count++;
	kdSendFrame();
}

void kdDmaEndHandler0( void *param )
{
	Sint32 i;
	void ( *func )( void );

	set_imask( 15 );
	if( kdgDmaType == 0 ) {
		kdgSystemInfo.dmaend_count++;
		kdRecvFrame();
		kdExecGun();
		for( i = 0 ; i < 32 ; i++ ) {
			if( kdgHandler[i] ) {
				func = (void ( * )( void ))( kdgHandler[i] );
				func();
			}
		}
	} else {
		kdgDmaType = 0;
		kdgSDCKBCount++;
	}
}

void kdExecGun( void )
{
	Uint32 wk;
	Uint32 *wkp2;
	Uint32 dmydev;	/* unused */
	Sint32 port;
	Uint32 data;

	if( kdgGunReadPort == 0 )
		return;
	port = kdgGunReadPort - 1;
	wk = ( (Uint32)kdgSDCKBEnableWork & 0x1FFFFFE0 ) + 0x20;
	wkp2 = (Uint32 *)( wk | P2_AREA );
	*wkp2 = 0x80000200 | ( port << 16 );
	kdgDmaType = 1;
	REG_DMACTA = (Uint32)wk;
	REG_DMAST = 0x00000001;
	data = MP_FUNC_CONTROLLER;
	mpdrv_setcmd_force( port * 6, MP_CMD_GETCONDITION, &data, 1, NULL, 0 );
	kdgGunReqPort = kdgGunReadPort;
}

void kdDmaEndHandler1( void *param )
{
	ISTNRM = 1 << 12;
}

void kdVbOverHandler( void *param )
{
	kdgSystemInfo.total_vover_count++;
	kdgSystemInfo.vover_count[kdgSurface]++;
	ISTNRM = 1 << 13;
}

static void copy_device( void )
{
	Sint32 i;
	KDS_DEVICE *k;
	MPDEVICE *m;

	k = kdgDevice;
	m = _mp->DeviceMgrTable;
	for( i = 0 ; i < MP_DEV_MAX ; i++, k++, m++ ) {
		k->on = m->on;
		k->old = m->old;
		k->connect = m->connect;
		k->remove = m->remove;
		k->recvdata = m->recvdata;
		k->response = m->response;
		k->dst_ap = m->dst_ap;
		k->src_ap = m->src_ap;
		k->datasize = m->datasize;
		k->errcode = m->errcode;
	}
}

static void init_sub( void )
{
	kdgChID[0] = syChainAddHandler( SYD_CHAIN_EVENT_IML6_VBLANK, kdVbInHandler, kdgChLevel[0], NULL );
	kdgChID[1] = syChainAddHandler( SYD_CHAIN_EVENT_IML4_MPDMAE, kdDmaEndHandler0, kdgChLevel[1], NULL );
	kdgChID[2] = syChainAddHandler( SYD_CHAIN_EVENT_IML4_MPDMAE, kdDmaEndHandler1, kdgChLevel[2], NULL );
	kdgChID[3] = syChainAddHandler( SYD_CHAIN_EVENT_IML4_MPVOVR, kdVbOverHandler, kdgChLevel[3], NULL );

	kdgErrFunc = NULL;
	kdgSurface = 0;
	kdgBorder = 0;
	kdgFlashFlag = 0;
	kdgGunReadPort = 0;
	kdgGunReqPort = 0;
	kdgGunLatchedPort = -1;
	kdgGunFlashCount = 0;
	kdgDmaType = 0;
	kdgSDCKBCount = 0;
	memset( &kdgSystemInfo, 0, sizeof( kdgSystemInfo ) );
	memset( kdgDevice, 0, sizeof( kdgDevice ) );
	memset( kdgCbTbl, 0, sizeof( kdgCbTbl ) );
	memset( kdgHandler, 0, sizeof( kdgHandler ) );
}

Sint32 kdInitSystem( void *recvbuf, void *sendbuf )
{
	Sint32 ret;
	Uint32 imask;

	imask = get_imask();
	set_imask( 15 );
	(Uint32)_mp_SemaphoreAddress |= P2_AREA;	/* TAS needs the semaphore uncached */
	init_sub();
	ret = mpdrv_init( sendbuf, 1024 * 24 * 2, recvbuf, 1024 * 24 * 2 );
	copy_device();
	kdgInitialized = TRUE;
	set_imask( imask );
	return ret;
}

Sint32 kdInitSystemEx( Sint32 *devmap, void **recvbuf, void *sendbuf, Sint32 num )
{
	Sint32 ret;
	Uint32 imask;

	imask = get_imask();
	set_imask( 15 );
	(Uint32)_mp_SemaphoreAddress |= P2_AREA;	/* TAS needs the semaphore uncached */
	init_sub();
	ret = mpdrv_init_ex( devmap, recvbuf, sendbuf, num );
	copy_device();
	kdgInitialized = TRUE;
	set_imask( imask );
	return ret;
}

Sint32 kdSendFrame( void )
{
	Sint32 ret;

	if( !kdgInitialized )
		return 0;
	kdgGunLatchedPort = kdgGunReqPort - 1;
	kdgGunReqPort = 0;
	if( kdgGunReadPort ) {
		kdgGunPosition[0] = SPG_TRIGGER_H;
		kdgGunPosition[1] = SPG_TRIGGER_V;
	}
	kdgGunReadPort = 0;
	if( kdgFlashFlag ) {
		VO_BORDER = kdgBorder;
		VO_CTRL &= ~( 1 << 3 );
		kdgFlashFlag = 0;
	}
	if( ISTERR & 0x00000F00 ) {
		if( ISTERR & 0x00000800 ) {
			kdgSystemInfo.error_count[0]++;
			if( kdgErrFunc )
				kdgErrFunc( 1, -2, kdgSystemInfo.vbin_count, ISTERR );
		}
		if( ISTERR & 0x00000400 ) {
			kdgSystemInfo.error_count[1]++;
			if( kdgErrFunc )
				kdgErrFunc( 1, -3, kdgSystemInfo.vbin_count, ISTERR );
		}
		if( ISTERR & 0x00000200 ) {
			kdgSystemInfo.error_count[2]++;
			if( kdgErrFunc )
				kdgErrFunc( 1, -4, kdgSystemInfo.vbin_count, ISTERR );
		}
		if( ISTERR & 0x00000100 ) {
			kdgSystemInfo.error_count[3]++;
			if( kdgErrFunc )
				kdgErrFunc( 1, -5, kdgSystemInfo.vbin_count, ISTERR );
		}
		ISTERR = 0x00000F00;
		kdRecvFrame();
		ret = mpdrv_frame_send();
		if( ret >= 0 ) {
			copy_device();
		} else {
			if( kdgErrFunc )
				kdgErrFunc( 1, -6, kdgSystemInfo.vbin_count, ret );
		}
	} else {
		ret = mpdrv_frame_send();
		if( ret >= 0 ) {
			kdgDmaType = 0;
			kdgSystemInfo.vover_count[kdgSurface] = 0;
			kdgSystemInfo.send_line[kdgSurface] = SCANLINE;
			copy_device();
			kdgSystemInfo.send_count++;
		} else {
			kdRecvFrame();
			kdgSystemInfo.cannot_send_count++;
			if( kdgErrFunc )
				kdgErrFunc( 1, -7, kdgSystemInfo.vbin_count, ret );
		}
	}
	return ret;
}

/* the port (+1) of a light gun whose trigger has just been pulled, or 0 */
static Sint32 trigger( void )
{
	const KDS_DEVICE *dm;
	const KDS_DEVICEINFO *info;
	Sint32 i;
	Uint8 trg, xor;
	Uint8 *p;
	Uint8 bit;
	static const Uint8 devno[] = {
		KDD_DEV_A0, KDD_DEV_B0, KDD_DEV_C0, KDD_DEV_D0,
	};

	bit = _mp->GunModeFlag;
	for( i = 0 ; i < 4 ; i++ ) {
		if( !( bit & ( 1 << i ) ) )
			continue;
		dm = kdGetDevice( devno[i] );
		info = kdGetDeviceInfo( devno[i] );
		kdgTriggerOld[i] = kdgTriggerNew[i];
		if( ( info->function & KDD_FUNC_LIGHTGUN ) && !dm->connect && dm->response == KDD_RES_DATATRANSFER ) {
			p = dm->recvdata;
			p += 8;
			trg = *p & ( 1 << 2 );
			kdgTriggerNew[i] = trg;
			xor = trg ^ kdgTriggerOld[i];
			if( ( trg == 0 ) && xor )
				return i + 1;
		}
	}
	return 0;
}

Sint32 kdRecvFrame( void )
{
	Sint32 i, ret;
	KD_CALLBACK func;
	MPDEVICE *dev;	/* unused */
	CBTBL *tbl;
	Uint32 imask;

	imask = get_imask();
	set_imask( 15 );
	ret = mpdrv_frame_recv_pre();
	if( ret == -2 ) {
		_mp->DmaEndIrqOccured = 1;
		set_imask( imask );
		return ret;
	}
	if( ret >= 0 ) {
		mpdrv_frame_recv_main();
		kdgSystemInfo.recv_line[kdgSurface] = SCANLINE + kdgSystemInfo.vover_count[kdgSurface] * VBINLINE;
		copy_device();
		mpdrv_frame_recv_post();
		if( mpdrv_get_gunmode() != 0 ) {
			if( kdgGunCallback ) {
				if( kdgGunCallback() != 0 )
					kdgGunReadPort = trigger();
			} else {
				kdgGunReadPort = trigger();
			}
			if( _mp->GunModeFlashColor ) {
				if( kdgGunReadPort ) {
					kdgBorder = VO_BORDER;
					VO_BORDER = _mp->GunModeFlashColor;
					VO_CTRL |= 1 << 3;
					kdgFlashFlag = 1;
					kdgGunFlashCount++;
				}
			}
		}
		tbl = kdgCbTbl;
		for( i = 0 ; i < MP_DEV_MAX * 2 ; i++, tbl++ ) {
			if( tbl->func ) {
				if( kdGetStatus( tbl->dev, tbl->seqno ) ) {
					func = tbl->func;
					tbl->func = NULL;
					func( tbl->dev, tbl->seqno, (KDS_DEVICE *)kdGetDevice( tbl->dev ),
					      (KDS_DEVICEINFO *)kdGetDeviceInfo( tbl->dev ) );
				}
			}
		}
		_mp->DmaEndIrqOccured = 0;
		kdgSurface ^= 1;
		kdgSystemInfo.recv_count++;
	} else {
		if( kdgErrFunc )
			kdgErrFunc( 2, -8, kdgSystemInfo.vbin_count, ret );
	}
	set_imask( imask );
	return ret;
}

void kdExitSystem( void )
{
	Uint32 imask;

	imask = get_imask();
	set_imask( 15 );
	IML4NRM &= 0xFFFFCFFF;
	syChainDeleteHandler( kdgChID[3] );
	syChainDeleteHandler( kdgChID[4] );
	syChainDeleteHandler( kdgChID[2] );
	syChainDeleteHandler( kdgChID[1] );
	mpdrv_exit();
	memset( kdgDevice, 0, sizeof( kdgDevice ) );
	set_imask( imask );
}

const KDS_DEVICE *kdGetDevice( Sint32 device )
{
	return &kdgDevice[device];
}

const KDS_DEVICEINFO *kdGetDeviceInfo( Sint32 device )
{
	const MPDEVICEINFO *info;

	info = mpdrv_get_deviceinfo( device );
	return (KDS_DEVICEINFO *)info;
}

const KDS_PORT *kdGetPortStatus( Sint32 port )
{
	const MPPORT *ps;

	ps = mpdrv_get_portstatus( port );
	return (KDS_PORT *)ps;
}

Sint32 kdGetStatus( Sint32 device, Sint32 seq )
{
	return mpdrv_stat( device, seq );
}

Sint32 kdSetDeviceRequest( Sint32 device )
{
	Sint32 ret;

	ret = mpdrv_set_dreq( device );
	CHKIRQ();
	return ret;
}

Sint32 kdSetAllStatusRequest( Sint32 device )
{
	Sint32 ret;

	ret = mpdrv_set_allstatreq( device );
	CHKIRQ();
	return ret;
}

Sint32 kdSetDeviceReset( Sint32 device )
{
	Sint32 ret;

	ret = mpdrv_set_dreset( device );
	CHKIRQ();
	return ret;
}

Sint32 kdSetDeviceKill( Sint32 device )
{
	Sint32 ret;

	ret = mpdrv_set_dkill( device );
	CHKIRQ();
	return ret;
}

Sint32 kdSetGetCondition( Sint32 device, Sint32 func )
{
	Sint32 ret;

	ret = mpdrv_set_getcond( device, func );
	CHKIRQ();
	return ret;
}

Sint32 kdSetGetMediaInfo( Sint32 device, Sint32 func, Sint32 pt )
{
	Sint32 ret;

	ret = mpdrv_set_getminfo( device, func, pt );
	CHKIRQ();
	return ret;
}

Sint32 kdSetBlockRead( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block )
{
	Sint32 ret;

	ret = mpdrv_set_bread( device, func, pt, phase, block );
	CHKIRQ();
	return ret;
}

Sint32 kdSetBlockWrite( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block, const void *src,
			Sint32 size )
{
	Sint32 ret;

	ret = mpdrv_set_bwrite( device, func, pt, phase, block, src, size );
	CHKIRQ();
	return ret;
}

Sint32 kdSetGetLastError( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block )
{
	Sint32 ret;

	ret = mpdrv_set_getlerr( device, func, pt, phase, block );
	CHKIRQ();
	return ret;
}

Sint32 kdSetARControl( Sint32 device, const void *buf )
{
	Sint32 ret;

	ret = mpdrv_set_arctrl( device, buf );
	CHKIRQ();
	return ret;
}

Sint32 kdSetFT4Control( Sint32 device, const void *buf )
{
	Sint32 ret;

	ret = mpdrv_set_ft4ctrl( device, buf );
	CHKIRQ();
	return ret;
}

Sint32 kdSetSetCondition( Sint32 device, Sint32 func, const void *buf, Sint32 size )
{
	Sint32 ret;

	ret = mpdrv_set_setcond( device, func, buf, size );
	CHKIRQ();
	return ret;
}

Sint32 kdSetTransmitAgain( Sint32 device )
{
	Sint32 ret;

	ret = mpdrv_set_transagain( device );
	CHKIRQ();
	return ret;
}

Sint32 kdSetResetPattern( Sint32 port )
{
	Sint32 ret;

	ret = mpdrv_set_hreset( port );
	CHKIRQ();
	return ret;
}

void kdGetVersion( Sint32 *major, Sint32 *minor )
{
	mpdrv_get_version( major, minor );
}

Sint32 kdIsChanged( Sint32 device )
{
	return mpdrv_ischanged( device );
}

void kdRecognizeChanged( Sint32 device )
{
	mpdrv_recognize( device );
}

const Uint8 *kdDevGetFD( const KDS_DEVICEINFO *info, Uint32 func )
{
	Uint32 found, bit, function;
	Sint32 i;
	const Uint8 *fd;

	found = 0;
	fd = info->fd[found];
	function = info->function;
	for( i = 0, bit = 0x80000000 ; i < 32 ; i++, bit >>= 1 ) {
		if( function & bit ) {
			if( func == bit )
				return fd;
			found++;
			fd = info->fd[found];
		}
	}
	return NULL;
}

Sint32 kdDevGetControllerInfo( KDS_CONTROLLERINFO *sinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_CONTROLLER );
	if( !fd )
		return -1;
	sinfo->button = ( fd[0] << 24 ) | ( fd[1] << 16 ) | ( fd[2] << 8 ) | fd[3];
	return 0;
}

Sint32 kdDevGetStorageInfo( KDS_STORAGEINFO *sinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_STORAGE );
	if( !fd )
		return -1;
	sinfo->partition = fd[0] + 1;
	sinfo->blocksize = ( fd[1] + 1 ) * 32;
	sinfo->writecount = ( fd[2] >> 4 ) & 0xF;
	sinfo->readcount = fd[2] & 0xF;
	sinfo->removable = ( fd[3] >> 7 ) & 1;
	return 0;
}

Sint32 kdDevGetSIPInfo( KDS_SIPINFO *sinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_SOUNDINPUT );
	if( !fd )
		return -1;
	sinfo->function = ( fd[0] << 24 ) | ( fd[1] << 16 ) | ( fd[2] << 8 ) | fd[3];
	return 0;
}

Sint32 kdDevGetLCDInfo( KDS_LCDINFO *sinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_LCD );
	if( !fd )
		return -1;
	sinfo->partition = fd[0] + 1;
	sinfo->blocksize = ( fd[1] + 1 ) * 32;
	sinfo->writecount = ( fd[2] >> 4 ) & 0xF;
	sinfo->direction = ( fd[3] >> 6 ) & 3;
	sinfo->normally = ( fd[3] >> 5 ) & 1;
	return 0;
}

Sint32 kdDevGetTimerInfo( KDS_TIMERINFO *tinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_TIMER );
	if( !fd )
		return -1;
	tinfo->write_timer = fd[0];
	tinfo->read_timer = fd[1];
	tinfo->button = fd[2];
	tinfo->alarm = fd[3];
	return 0;
}

Sint32 kdDevGetGunInfo( KDS_GUNINFO *sinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_ARGUN );
	if( !fd )
		return -1;
	sinfo->button = ( fd[0] << 24 ) | ( fd[1] << 16 ) | ( fd[2] << 8 ) | fd[3];
	return 0;
}

Sint32 kdDevGetVibrationInfo( KDS_VIBRATIONINFO *sinfo, const KDS_DEVICEINFO *info )
{
	const Uint8 *fd;

	fd = kdDevGetFD( info, KDD_FUNC_VIBRATION );
	if( !fd )
		return -1;
	sinfo->vn = fd[0];
	sinfo->se = fd[1];
	return 0;
}

Sint32 kdSetDriverMode( Sint32 type, Sint32 mode )
{
	switch( type ) {
	case KDD_MODE_NOPCOUNT:
		mpdrv_nopcount( mode );
		break;
	case KDD_MODE_TIMEOUT:
		mpdrv_timeout( mode );
		break;
	case KDD_MODE_DREQCOUNT:
		mpdrv_dreqcount( mode );
		break;
	case KDD_MODE_SPEED:
		mpdrv_speed( mode );
		break;
	case KDD_MODE_DEBUG:
		mpdrv_debugmode( mode );
		break;
	case KDD_MODE_DEVENDIAN:
		mpdrv_swapmode( mode );
		break;
	case KDD_MODE_OPTIMIZE:
		mpdrv_optimize( mode );
		break;
	case KDD_MODE_ROOTPLAY:
		mpdrv_rootplay( mode );
		break;
	case KDD_MODE_GUNMODE:
		mpdrv_gunmode( mode );
		break;
	case KDD_MODE_AUTOFLASH:
		mpdrv_autoflash( mode );
		break;
	default:
		return -1;
	}
	return 0;
}

Sint32 kdGetDriverMode( Sint32 type )
{
	switch( type ) {
	case KDD_MODE_NOPCOUNT:
		return mpdrv_get_nopcount();
	case KDD_MODE_TIMEOUT:
		return mpdrv_get_timeout();
	case KDD_MODE_DREQCOUNT:
		return mpdrv_get_dreqcount();
	case KDD_MODE_SPEED:
		return mpdrv_get_speed();
	case KDD_MODE_DEBUG:
		return mpdrv_get_debugmode();
	case KDD_MODE_DEVENDIAN:
		return mpdrv_get_swapmode();
	case KDD_MODE_OPTIMIZE:
		return mpdrv_get_optimize();
	case KDD_MODE_ROOTPLAY:
		return mpdrv_get_rootplay();
	case KDD_MODE_GUNMODE:
		return mpdrv_get_gunmode();
	case KDD_MODE_AUTOFLASH:
		return mpdrv_get_autoflash();
	default:
		return -1;
	}
}

const KDS_SYSTEMINFO *kdGetSystemInfo( void )
{
	return &kdgSystemInfo;
}

void kdGetGunPosition( Sint32 *x, Sint32 *y )
{
	*x = kdgGunPosition[0];
	*y = kdgGunPosition[1];
}

Sint32 kdGetGunLatchedPort( void )
{
	return kdgGunLatchedPort;
}

void kdSetGunTrigger( Uint32 port )
{
	kdgGunReadPort = port + 1;
}

void kdSetGunCallback( Sint32 ( *func )( void ) )
{
	kdgGunCallback = func;
}

void kdSetHandler( Uint32 num, void ( *func )( void ) )
{
	if( num > 31 )
		return;
	kdgHandler[num] = (Uint32)func;
}

Sint32 kdSetCallback( Sint32 device, KD_CALLBACK func, Sint32 seqno )
{
	CBTBL *tbl;

	tbl = &kdgCbTbl[device * 2 + _mp->DriverState];
	tbl->func = func;
	tbl->seqno = seqno;
	tbl->dev = device;
	return 0;
}

Sint32 kdSetDeviceRequestEx( Sint32 device, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_dreq_ns( device );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetAllStatusRequestEx( Sint32 device, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_allstatreq_ns( device );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetDeviceResetEx( Sint32 device, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_dreset_ns( device );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetDeviceKillEx( Sint32 device, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_dkill_ns( device );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetGetConditionEx( Sint32 device, Sint32 func, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_getcond_ns( device, func );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetGetMediaInfoEx( Sint32 device, Sint32 func, Sint32 pt, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_getminfo_ns( device, func, pt );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetBlockReadEx( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_bread_ns( device, func, pt, phase, block );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetBlockWriteEx( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block, const void *src,
			  Sint32 size, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_bwrite_ns( device, func, pt, phase, block, src, size );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetGetLastErrorEx( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block,
			    KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_getlerr_ns( device, func, pt, phase, block );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetARControlEx( Sint32 device, const void *buf, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_arctrl_ns( device, buf );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetFT4ControlEx( Sint32 device, const void *buf, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_ft4ctrl_ns( device, buf );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}

Sint32 kdSetSetConditionEx( Sint32 device, Sint32 func, const void *buf, Sint32 size, KD_CALLBACK cbfunc )
{
	Sint32 ret;

	GETSMP();
	ret = mpdrv_set_setcond_ns( device, func, buf, size );
	if( ret > 0 )
		kdSetCallback( device, cbfunc, ret );
	RELSMP();
	CHKIRQ();
	return ret;
}
