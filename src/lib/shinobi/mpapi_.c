/* mpapi_: Shinobi's Maple driver API: start-up, the frame cycle (send, receive) and the settings
   and status calls. */
#include "mpdrv.h"
#include "syreg.h"

#define tas( p )	_builtin_tas( p )

#define ADRMASK		0x0000001F
#define ALIGN_32BYTE( adr )	\
	if ( ( (long)( adr ) ) & ADRMASK ) {	\
		( (long)( adr ) ) += ADRMASK + 1;	\
		( (long)( adr ) ) &= ~ADRMASK;	\
	}

static const char smp_error[] = "***FATAL ERROR:Cannot get semaphore!\n";

#define GETSMP()	\
	if ( !tas( &*_mp_SemaphoreAddress ) ) {	\
		mpdrv_dpf( smp_error );	\
		return -2;	\
	}
#define RELSMP()	*_mp_SemaphoreAddress = 0

typedef char *va_list;
#define va_start( ap, last )	( ( ap ) = ( ( (long)&( last ) + 4 ) & 3 ) ? (char *)&( last ) + 8 : (char *)&( last ) + 4 )
#define va_end( ap )
extern int vsprintf( char *buf, const char *fmt, va_list ap );
extern unsigned long syTmrGetCount( void );
extern long syTmrDiffCount( unsigned long from, unsigned long to );
extern long kdgGunModeTimeout;

long mpdrv_init( void *cmdbuf, long cmdsize, void *readbuf, long readsize )
{
	long port;

	_mp = &_mp_Context;
	_mp->SendDataTop = cmdbuf;
	_mp->RecvDataTop = readbuf;
	ALIGN_32BYTE( _mp->SendDataTop );
	ALIGN_32BYTE( _mp->RecvDataTop );
	(long)_mp->SendDataTop |= P2_AREA;
	(long)_mp->RecvDataTop |= P2_AREA;
	mpdrv_init_globals();
	for ( port = 0; port < 4; port++ ) {
		if ( _mp->DevEnable[_mp_RootDeviceNo[port]] ) {
			mpdrv_set_dreq_ns( _mp_RootDeviceNo[port] );
		}
	}
	mpdrv_set_end_ns();
	_mp->DriverState = MP_STATE_RECEIVED;
	REG_RAMPRT = 0x6155404F;
	REG_DMATRG = 0x00000000;
	REG_SYSCTL = _mp->TimeOut | _mp->Speed;
	REG_DMAEN = 0x00000001;
	return 0;
}

long mpdrv_init_ex( long *devmap, void **recvbuf, void *sendbuf, long num )
{
	long i, dev, port;
	char *buf;

	_mp = &_mp_Context;
	_mp->SendDataTop = sendbuf;
	_mp->RecvDataTop = (long *)recvbuf;
	ALIGN_32BYTE( _mp->SendDataTop );
	ALIGN_32BYTE( _mp->RecvDataTop );
	(long)_mp->SendDataTop |= P2_AREA;
	(long)_mp->RecvDataTop |= P2_AREA;
	mpdrv_init_globals();
	for ( i = 0; i < MP_DEV_MAX; i++ ) {
		_mp->RecvAddr[0][i] = 0;
		_mp->RecvAddr[1][i] = 0;
	}
	for ( i = 0; i < num; i++ ) {
		buf = (char *)recvbuf[i];
		ALIGN_32BYTE( buf );
		(long)buf |= P2_AREA;
		dev = devmap[i];
		_mp->RecvAddr[0][dev] = (long *)buf;
		_mp->RecvAddr[1][dev] = (long *)( buf + 0x400 );
	}
	_mp->SendAddr[0] = _mp->SendDataTop;
	_mp->SendAddr[1] = _mp->SendDataTop + 0x100 * num;
	_mp->SendData = _mp->SendAddr[0];
	for ( i = 0; i < MP_DEV_MAX; i++ ) {
		_mp->DevEnable[i] = 0;
	}
	for ( i = 0; i < num; i++ ) {
		dev = devmap[i];
		switch ( dev ) {
		case MP_DEV_A0:
		case MP_DEV_B0:
		case MP_DEV_C0:
		case MP_DEV_D0:
			_mp->DevEnable[dev] = 'r';
			break;
		default:
			_mp->DevEnable[dev] = 'e';
			break;
		}
	}
	for ( port = 0; port < 4; port++ ) {
		if ( _mp->DevEnable[_mp_RootDeviceNo[port]] ) {
			mpdrv_set_dreq_ns( _mp_RootDeviceNo[port] );
		}
	}
	mpdrv_set_end_ns();
	_mp->DriverState = MP_STATE_RECEIVED;
	REG_RAMPRT = 0x6155404F;
	REG_DMATRG = 0x00000000;
	REG_SYSCTL = _mp->TimeOut | _mp->Speed;
	REG_DMAEN = 0x00000001;
	return 0;
}

static long frame_make( void )
{
	long i;
	MPDEVICE *dm;
	MPDEVICEINFO *info;

	dm = _mp->DeviceMgrTable;
	info = _mp->DeviceInfo;
	if ( _mp->OptimizeNopCount > 0 ) {
		mpdrv_insertnop( _mp->OptimizeNopCount );
	}
	for ( i = 0; i < MP_DEV_MAX; i++, dm++, info++ ) {
		if ( !_mp->DevEnable[i] ) continue;
		if ( dm->usercmd == 0 ) {
			switch ( dm->nextcmd ) {
			case MP_CMD_DEVICEREQUEST:
				mpdrv_set_dreq_ns( i );
				break;
			case MP_CMD_ALLSTATUSREQUEST:
				mpdrv_set_allstatreq_ns( i );
				break;
			case MP_CMD_DEVICERESET:
				mpdrv_set_dreset_ns( i );
				break;
			case MP_CMD_DEVICEKILL:
				mpdrv_set_dkill_ns( i );
				break;
			case MP_CMD_GETCONDITION:
				{
					long func;

					func = info->function;
					if ( func & MP_FUNC_CONTROLLER ) {
						func = MP_FUNC_CONTROLLER;
					}
					mpdrv_set_getcond_ns( i, func );
				}
				break;
			case MP_CMD_GETMEDIAINFO:
				break;
			case MP_CMD_BLOCKREAD:
				break;
			case MP_CMD_BLOCKWRITE:
				break;
			case MP_CMD_GETLASTERROR:
				break;
			case 0x00:
			case 0xFF:
				break;
			default:
				break;
			}
		}
	}
	mpdrv_set_end_ns();
	return 0;
}

long mpdrv_frame_send( void )
{
	long i;
	MPDEVICE *dm;

	if ( _mp->DriverState != MP_STATE_RECEIVED ) {
		return -3;
	}
	if ( REG_DMAST & 1 ) {
		return -1;
	}
	GETSMP();
	if ( _mp->GunModeFlag != _mp->GunModeRequest ) {
		_mp->GunModeFlag = _mp->GunModeRequest;
		if ( _mp->GunModeRequest != 0 ) {
			_mp->TimeOut = ( kdgGunModeTimeout * 1000 / 20 ) << 16;
			REG_DMAEN = 0x00000000;
			REG_SYSCTL = _mp->TimeOut | _mp->Speed;
			REG_DMAEN = 0x00000001;
		} else {
			_mp->TimeOut = ( 1000 * 1000 / 20 ) << 16;
			REG_DMAEN = 0x00000000;
			REG_SYSCTL = _mp->TimeOut | _mp->Speed;
			REG_DMAEN = 0x00000001;
		}
	}
	frame_make();
	_mp->LastSendAddr = _mp->SendAddr[_mp->CurrentSendBuffer];
	REG_DMACTA = (long)mpdrv_v2p( _mp->SendAddr[_mp->CurrentSendBuffer] );
	REG_DMAST = 0x00000001;
	_mp->LastCommandSize = 0;
	_mp->CurrentSendBuffer ^= 1;
	_mp->SendData = _mp->SendAddr[_mp->CurrentSendBuffer];
	_mp->CurrentRecvBuffer ^= 1;
	_mp->TotalCommandLength = 0;
	dm = _mp->DeviceMgrTable;
	for ( i = 0; i < MP_DEV_MAX; i++, dm++ ) {
		if ( _mp->DevEnable[i] ) {
			dm->execcmd = dm->usercmd;
			dm->usercmd = 0;
			dm->execute_seqno = dm->accept_seqno;
			dm->accept_seqno = 0;
		}
	}
	_mp->DriverState = MP_STATE_SENT;
	RELSMP();
	return 0;
}

long mpdrv_frame_recv_pre( void )
{
	long port, dev;

	if ( _mp->DriverState != MP_STATE_SENT ) {
		return -3;
	}
	if ( REG_DMAST & 1 ) {
		return -1;
	}
	GETSMP();
	return 0;
}

long mpdrv_frame_recv_post( void )
{
	_mp->DriverState = MP_STATE_RECEIVED;
	RELSMP();
	return 0;
}

long mpdrv_frame_recv_main( void )
{
	long port, dev;
	long ret;

	for ( port = 0; port < 4; port++ ) {
		if ( !_mp->DevEnable[_mp_RootDeviceNo[port]] ) continue;
		mpdrv_check_port( port );
		for ( dev = 0; dev < 6; dev++ ) {
			if ( _mp->DevEnable[port * 6 + dev] ) {
				mpdrv_make_devicedata( port * 6 + dev, port );
			}
		}
	}
	return 0;
}

long mpdrv_exit( void )
{
	unsigned long count;

	count = syTmrGetCount();
	while ( REG_DMAST & 1 ) {
		if ( syTmrDiffCount( count, syTmrGetCount() ) > 100 * 1000 ) break;
	}
	REG_DMAST = 0x00000000;
	REG_DMAEN = 0x00000000;
	REG_DMATRG = 0x00000000;
	REG_RAMPRT = 0x6155404F;
	REG_SYSCTL = 0xC3500000;
	REG_DMACTA = 0x0CFF0000;
	count = syTmrGetCount();
	while ( REG_DMAST & 1 ) {
		if ( syTmrDiffCount( count, syTmrGetCount() ) > 17 * 1000 ) break;
	}
	return 0;
}

void ( *_mpdrv_dbg_out )( const char *str ) = 0;

void mpdrv_dpf( const char *fmt, ... )
{
	if ( _mp->Debug ) {
		static char buffer[256];
		va_list va;

		va_start( va, fmt );
		vsprintf( buffer, fmt, va );
		va_end( va );
		if ( _mpdrv_dbg_out ) _mpdrv_dbg_out( buffer );
	}
}

const MPDEVICE *mpdrv_get_device( long device )
{
	return &_mp->DeviceMgrTable[device];
}

const MPDEVICEINFO *mpdrv_get_deviceinfo( long device )
{
	return &_mp->DeviceInfo[device];
}

const MPPORT *mpdrv_get_portstatus( long port )
{
	return &_mp->PortMgrTable[port];
}

long mpdrv_stat( long device, long seq )
{
	const MPDEVICE *dev;

	dev = mpdrv_get_device( device );
	if ( dev->complete_seqno == seq ) {
		return 1;
	} else {
		return 0;
	}
}

long mpdrv_ischanged( long device )
{
	const MPDEVICE *dev;

	dev = mpdrv_get_device( device );
	return dev->change;
}

void mpdrv_recognize( long device )
{
	MPDEVICE *dev;

	dev = (MPDEVICE *)mpdrv_get_device( device );
	dev->recognize = 1;
}

void mpdrv_timeout( long count )
{
	_mp->TimeOut = count << 16;
}

long mpdrv_get_timeout( void )
{
	return ( _mp->TimeOut >> 16 ) & 0xFFFF;
}

void mpdrv_swapmode( long type )
{
	_mp->DeviceEndian = type & 1;
}

long mpdrv_get_swapmode( void )
{
	return _mp->DeviceEndian;
}

void mpdrv_nopcount( long count )
{
	_mp->NopCount = count;
}

long mpdrv_get_nopcount( void )
{
	return _mp->NopCount;
}

void mpdrv_speed( long speed )
{
	_mp->Speed = speed;
}

long mpdrv_get_speed( void )
{
	return _mp->Speed;
}

void mpdrv_get_version( long *major, long *minor )
{
	if ( major ) *major = _mp_MajorVersion;
	if ( minor ) *minor = _mp_MinorVersion;
}

void mpdrv_debugmode( long mode )
{
	_mp->Debug = mode;
}

long mpdrv_get_debugmode( void )
{
	return _mp->Debug;
}

void mpdrv_dreqcount( long count )
{
	_mp->ExDevReqCount = count;
}

long mpdrv_get_dreqcount( void )
{
	return _mp->ExDevReqCount;
}

void mpdrv_optimize( long count )
{
	_mp->OptimizeMode = count;
	_mp->OptimizeNopCount = count;
}

long mpdrv_get_optimize( void )
{
	return _mp->OptimizeNopCount;
}

void *mpdrv_get_lastsendaddr( void )
{
	return (void *)_mp->LastSendAddr;
}

void mpdrv_rootplay( long count )
{
	_mp->RootDevPlayCount = count;
}

long mpdrv_get_rootplay( void )
{
	return _mp->RootDevPlayCount;
}

void mpdrv_gunmode( long mode )
{
	_mp->GunModeRequest = mode;
}

long mpdrv_get_gunmode( void )
{
	return _mp->GunModeFlag;
}

void mpdrv_autoflash( long color )
{
	_mp->GunModeFlashColor = color;
}

long mpdrv_get_autoflash( void )
{
	return _mp->GunModeFlashColor;
}
