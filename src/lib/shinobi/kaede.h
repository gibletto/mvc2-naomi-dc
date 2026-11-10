/* kaede.h: Shinobi's Maple library (kd*): devices, their function blocks and the driver's modes. */
#ifndef KAEDE_H
#define KAEDE_H

#include "types.h"

#define KDD_DEV_A0	0
#define KDD_DEV_B0	6
#define KDD_DEV_C0	12
#define KDD_DEV_D0	18

/* device functions */
#define KDD_FUNC_CONTROLLER	0x00000001
#define KDD_FUNC_STORAGE	0x00000002
#define KDD_FUNC_LCD		0x00000004
#define KDD_FUNC_TIMER		0x00000008
#define KDD_FUNC_SOUNDINPUT	0x00000010
#define KDD_FUNC_ARGUN		0x00000020
#define KDD_FUNC_KEYBOARD	0x00000040
#define KDD_FUNC_LIGHTGUN	0x00000080
#define KDD_FUNC_VIBRATION	0x00000100

#define KDD_RES_DATATRANSFER	0x08

/* driver modes of kdSetDriverMode */
#define KDD_MODE_OPTIMIZE	0
#define KDD_MODE_NOPCOUNT	1
#define KDD_MODE_TIMEOUT	2
#define KDD_MODE_SPEED		3
#define KDD_MODE_DREQCOUNT	4
#define KDD_MODE_DEVENDIAN	5
#define KDD_MODE_DEBUG		6
#define KDD_MODE_ROOTPLAY	7
#define KDD_MODE_GUNMODE	8
#define KDD_MODE_AUTOFLASH	9

typedef struct {
	Uint8	lmon;
	Uint8	lmold;
	Uint8	lmcon;
	Uint8	lmremv;
	Uint32	playcount;
} KDS_PORT;

typedef struct {
	Uint8	on;
	Uint8	old;
	Uint8	connect;
	Uint8	remove;
	Uint8	response;
	Uint8	dst_ap;
	Uint8	src_ap;
	Uint8	datasize;
	void	*recvdata;
	Sint32	errcode;
	Uint32	reserved[2];
} KDS_DEVICE;

typedef struct {
	Uint32	function;
	Uint8	fd[3][4];
	Uint8	is_root;
	Uint8	area_code;
	Uint8	connector_dir[2];
	char	product_name[32];
	char	license[64];
	Uint16	stdby_pow;
	Uint16	max_pow;
} KDS_DEVICEINFO;

typedef struct {
	Uint32	button;
} KDS_CONTROLLERINFO;

typedef struct {
	Uint16	partition;
	Uint16	blocksize;
	Uint16	writecount;
	Uint8	direction;
	Uint8	normally;
} KDS_LCDINFO;

typedef struct {
	Uint16	partition;
	Uint16	blocksize;
	Uint16	writecount;
	Uint16	readcount;
	Uint16	removable;
	Uint16	reserved;
} KDS_STORAGEINFO;

typedef struct {
	Uint8	write_timer;
	Uint8	read_timer;
	Uint8	button;
	Uint8	alarm;
} KDS_TIMERINFO;

typedef struct {
	Uint32	function;
} KDS_SIPINFO;

typedef struct {
	Uint32	button;
} KDS_GUNINFO;

typedef struct {
	Uint32	vn;
	Uint32	se;
} KDS_VIBRATIONINFO;

typedef struct {
	Uint32	send_count;
	Uint32	recv_count;
	Uint32	vbin_count;
	Uint32	dmaend_count;
	Uint32	cannot_send_count;
	Uint32	blocked_count;
	Uint32	total_vover_count;
	Uint32	error_count[4];
	Uint16	send_line[2];
	Uint16	recv_line[2];
	Uint16	vover_count[2];
	Uint32	job_blocked_count;
	Uint32	reserved;
} KDS_SYSTEMINFO;

typedef void ( *KD_CALLBACK )( Sint32 device, Sint32 seqno, KDS_DEVICE *dev, KDS_DEVICEINFO *info );

extern Sint32 kdInitSystem( void *recvbuf, void *sendbuf );
extern Sint32 kdInitSystemEx( Sint32 *devmap, void **recvbuf, void *sendbuf, Sint32 num );
extern void kdExitSystem( void );
extern const KDS_SYSTEMINFO *kdGetSystemInfo( void );
extern Sint32 kdSendFrame( void );
extern Sint32 kdRecvFrame( void );
extern Sint32 kdSetDriverMode( Sint32 type, Sint32 mode );
extern Sint32 kdGetDriverMode( Sint32 type );

extern const KDS_DEVICE *kdGetDevice( Sint32 device );
extern const KDS_DEVICEINFO *kdGetDeviceInfo( Sint32 device );
extern const KDS_PORT *kdGetPortStatus( Sint32 port );
extern Sint32 kdGetStatus( Sint32 device, Sint32 seq );
extern Sint32 kdIsChanged( Sint32 device );
extern void kdRecognizeChanged( Sint32 device );

extern Sint32 kdSetDeviceRequest( Sint32 device );
extern Sint32 kdSetAllStatusRequest( Sint32 device );
extern Sint32 kdSetDeviceReset( Sint32 device );
extern Sint32 kdSetDeviceKill( Sint32 device );
extern Sint32 kdSetGetCondition( Sint32 device, Sint32 func );
extern Sint32 kdSetGetMediaInfo( Sint32 device, Sint32 func, Sint32 pt );
extern Sint32 kdSetBlockRead( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block );
extern Sint32 kdSetBlockWrite( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block,
			       const void *src, Sint32 size );
extern Sint32 kdSetGetLastError( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block );
extern Sint32 kdSetARControl( Sint32 device, const void *buf );
extern Sint32 kdSetFT4Control( Sint32 device, const void *buf );
extern Sint32 kdSetSetCondition( Sint32 device, Sint32 func, const void *buf, Sint32 size );
extern Sint32 kdSetTransmitAgain( Sint32 device );
extern Sint32 kdSetResetPattern( Sint32 port );

extern const Uint8 *kdDevGetFD( const KDS_DEVICEINFO *info, Uint32 func );
extern Sint32 kdDevGetControllerInfo( KDS_CONTROLLERINFO *sinfo, const KDS_DEVICEINFO *info );
extern Sint32 kdDevGetStorageInfo( KDS_STORAGEINFO *sinfo, const KDS_DEVICEINFO *info );
extern Sint32 kdDevGetSIPInfo( KDS_SIPINFO *sinfo, const KDS_DEVICEINFO *info );
extern Sint32 kdDevGetLCDInfo( KDS_LCDINFO *sinfo, const KDS_DEVICEINFO *info );
extern Sint32 kdDevGetTimerInfo( KDS_TIMERINFO *tinfo, const KDS_DEVICEINFO *info );
extern Sint32 kdDevGetGunInfo( KDS_GUNINFO *sinfo, const KDS_DEVICEINFO *info );
extern Sint32 kdDevGetVibrationInfo( KDS_VIBRATIONINFO *sinfo, const KDS_DEVICEINFO *info );

extern void kdGetGunPosition( Sint32 *x, Sint32 *y );
extern Sint32 kdGetGunLatchedPort( void );
extern void kdSetGunTrigger( Uint32 port );
extern void kdSetGunCallback( Sint32 ( *func )( void ) );
extern void kdGetVersion( Sint32 *major, Sint32 *minor );
extern void kdSetHandler( Uint32 num, void ( *func )( void ) );

extern Sint32 kdSetCallback( Sint32 device, KD_CALLBACK func, Sint32 seqno );
extern Sint32 kdSetDeviceRequestEx( Sint32 device, KD_CALLBACK cbfunc );
extern Sint32 kdSetAllStatusRequestEx( Sint32 device, KD_CALLBACK cbfunc );
extern Sint32 kdSetDeviceResetEx( Sint32 device, KD_CALLBACK cbfunc );
extern Sint32 kdSetDeviceKillEx( Sint32 device, KD_CALLBACK cbfunc );
extern Sint32 kdSetGetConditionEx( Sint32 device, Sint32 func, KD_CALLBACK cbfunc );
extern Sint32 kdSetGetMediaInfoEx( Sint32 device, Sint32 func, Sint32 pt, KD_CALLBACK cbfunc );
extern Sint32 kdSetBlockReadEx( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block,
				KD_CALLBACK cbfunc );
extern Sint32 kdSetBlockWriteEx( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block,
				 const void *src, Sint32 size, KD_CALLBACK cbfunc );
extern Sint32 kdSetGetLastErrorEx( Sint32 device, Sint32 func, Sint32 pt, Sint32 phase, Sint32 block,
				   KD_CALLBACK cbfunc );
extern Sint32 kdSetARControlEx( Sint32 device, const void *buf, KD_CALLBACK cbfunc );
extern Sint32 kdSetFT4ControlEx( Sint32 device, const void *buf, KD_CALLBACK cbfunc );
extern Sint32 kdSetSetConditionEx( Sint32 device, Sint32 func, const void *buf, Sint32 size,
				   KD_CALLBACK cbfunc );

extern Sint32 kdGetDriverStatus( void );
extern void kdBlockKaedeJob( Sint32 mode );
extern void kdSetAppVBFunction( void ( *func )( void ) );

#endif
