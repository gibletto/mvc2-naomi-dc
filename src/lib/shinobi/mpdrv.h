/* mpdrv.h: the Maple driver's tables and the calls of its modules (mpapi_, mpdrv_, mpgbl_). */
#ifndef MPDRV_H
#define MPDRV_H

#ifndef NULL
#define NULL	0
#endif

#define MP_DEV_MAX	24
#define MP_PORT_MAX	4

#define MP_DEV_A0	0
#define MP_DEV_B0	6
#define MP_DEV_C0	12
#define MP_DEV_D0	18

#define MP_FUNC_CONTROLLER	0x00000001

#define MP_CMD_DEVICEREQUEST	0x01
#define MP_CMD_ALLSTATUSREQUEST	0x02
#define MP_CMD_DEVICERESET	0x03
#define MP_CMD_DEVICEKILL	0x04
#define MP_CMD_GETCONDITION	0x09
#define MP_CMD_GETMEDIAINFO	0x0A
#define MP_CMD_BLOCKREAD	0x0B
#define MP_CMD_BLOCKWRITE	0x0C
#define MP_CMD_GETLASTERROR	0x0D

#define MP_STATE_SENT		0
#define MP_STATE_RECEIVED	1

typedef struct {
	unsigned char	lmon;
	unsigned char	lmold;
	unsigned char	lmcon;
	unsigned char	lmremv;
	unsigned long	playcount;
} MPPORT;

typedef struct {
	unsigned char	on;
	unsigned char	old;
	unsigned char	connect;
	unsigned char	remove;
	unsigned char	nextcmd;
	unsigned char	oldcmd;
	unsigned char	usercmd;
	unsigned char	execcmd;
	unsigned char	response;
	unsigned char	dst_ap;
	unsigned char	src_ap;
	unsigned char	datasize;
	unsigned char	change;
	unsigned char	recognize;
	unsigned char	reserved[2];
	void		*recvdata;
	long		errcode;
	long		accept_seqno;
	long		execute_seqno;
	long		complete_seqno;
	unsigned long	dreqcount;
	void		(*func)( long, long );
} MPDEVICE;

typedef struct {
	unsigned long	function;
	unsigned char	fd[3][4];
	unsigned char	is_root;
	unsigned char	area_code;
	unsigned char	connector_dir[2];
	char		product_name[32];
	char		license[64];
	unsigned short	stdby_pow;
	unsigned short	max_pow;
} MPDEVICEINFO;

typedef struct {
	char		magic0[4];
	MPPORT		PortMgrTable[MP_PORT_MAX];
	char		magic1[4];
	MPDEVICE	DeviceMgrTable[MP_DEV_MAX];
	char		magic2[4];
	MPDEVICEINFO	DeviceInfo[MP_DEV_MAX];
	char		magic3[4];
	unsigned char	DevEnable[MP_DEV_MAX];
	char		magic4[4];
	long		GunModeRequest;
	long		GunModeFlag;
	long		GunModeCurrentPort;
	long		GunModeLastPort;
	long		GunModeFlashColor;
	long		DriverState;
	long		*SendDataTop;
	long		*RecvDataTop;
	long		*SendData;
	long		*RecvData;
	long		*SendBackup;
	long		LastCommandSize;
	long		TotalCommandLength;
	long		SequenceNo;
	char		magic5[4];
	long		*RecvAddr[2][MP_DEV_MAX];
	long		*SendAddr[2];
	long		*LastSendAddr;
	long		CurrentRecvBuffer;
	long		CurrentSendBuffer;
	char		magic6[4];
	long		TimeOut;
	long		DeviceEndian;
	long		Debug;
	long		NopCount;
	long		Speed;
	char		magic7[4];
	long		ExDevReqCount;
	long		RootDevPlayCount;
	long		OptimizeMode;
	long		OptimizeNopCount;
	long		DmaEndIrqOccured;
	long		FifoError;
	char		magic8[4];
	volatile long	*Regs;
	char		magic9[4];
} MPCONTEXT;

extern MPCONTEXT _mp_Context;
extern MPCONTEXT *_mp;
extern const long _mp_RootDeviceNo[];
extern const long _mp_MajorVersion;
extern const long _mp_MinorVersion;
extern char *_mp_SemaphoreAddress;

#define REG_DMACTA	_mp->Regs[0x04 / 4]
#define REG_DMATRG	_mp->Regs[0x10 / 4]
#define REG_DMAEN	_mp->Regs[0x14 / 4]
#define REG_DMAST	_mp->Regs[0x18 / 4]
#define REG_SYSCTL	_mp->Regs[0x80 / 4]
#define REG_RAMPRT	_mp->Regs[0x8c / 4]

extern long mpdrv_check_port( long port );
extern long mpdrv_make_devicedata( long device, long port );
extern void mpdrv_init_globals( void );
extern long mpdrv_insertnop( long count );
extern void mpdrv_dpf( const char *fmt, ... );
extern void *mpdrv_v2p( void *adr );
extern long mpdrv_set_dreq_ns( long device );
extern long mpdrv_set_allstatreq_ns( long device );
extern long mpdrv_set_dreset_ns( long device );
extern long mpdrv_set_dkill_ns( long device );
extern long mpdrv_set_getcond_ns( long device, long func );
extern long mpdrv_set_end_ns( void );

/* the driver's other calls, as kaede uses them */
extern long mpdrv_init( void *cmdbuf, long cmdsize, void *readbuf, long readsize );
extern long mpdrv_init_ex( long *devmap, void **recvbuf, void *sendbuf, long num );
extern long mpdrv_exit( void );
extern long mpdrv_frame_send( void );
extern long mpdrv_frame_recv_pre( void );
extern long mpdrv_frame_recv_main( void );
extern long mpdrv_frame_recv_post( void );
extern long mpdrv_setcmd_force( long device, unsigned char cmd, const void *data1, long size1,
				  const void *data2, long size2 );
extern void mpdrv_timeout( long count );
extern void mpdrv_swapmode( long mode );
extern void mpdrv_nopcount( long count );
extern void mpdrv_speed( long speed );
extern void mpdrv_debugmode( long mode );
extern void mpdrv_dreqcount( long count );
extern void mpdrv_optimize( long count );
extern void mpdrv_rootplay( long mode );
extern void mpdrv_gunmode( long mode );
extern void mpdrv_autoflash( long color );
extern long mpdrv_get_timeout( void );
extern long mpdrv_get_swapmode( void );
extern long mpdrv_get_nopcount( void );
extern long mpdrv_get_speed( void );
extern long mpdrv_get_debugmode( void );
extern long mpdrv_get_dreqcount( void );
extern long mpdrv_get_optimize( void );
extern long mpdrv_get_rootplay( void );
extern long mpdrv_get_gunmode( void );
extern long mpdrv_get_autoflash( void );
extern const MPDEVICEINFO *mpdrv_get_deviceinfo( long device );
extern const MPPORT *mpdrv_get_portstatus( long port );
extern long mpdrv_stat( long device, long seq );
extern long mpdrv_ischanged( long device );
extern void mpdrv_recognize( long device );
extern void mpdrv_get_version( long *major, long *minor );
extern long mpdrv_set_dreq( long device );
extern long mpdrv_set_allstatreq( long device );
extern long mpdrv_set_dreset( long device );
extern long mpdrv_set_dkill( long device );
extern long mpdrv_set_getcond( long device, long func );
extern long mpdrv_set_getminfo( long device, long func, long pt );
extern long mpdrv_set_bread( long device, long func, long pt, long phase, long block );
extern long mpdrv_set_bwrite( long device, long func, long pt, long phase, long block,
				const void *src, long size );
extern long mpdrv_set_getlerr( long device, long func, long pt, long phase, long block );
extern long mpdrv_set_arctrl( long device, const void *buf );
extern long mpdrv_set_ft4ctrl( long device, const void *buf );
extern long mpdrv_set_setcond( long device, long func, const void *buf, long size );
extern long mpdrv_set_transagain( long device );
extern long mpdrv_set_hreset( long port );
extern long mpdrv_set_getminfo_ns( long device, long func, long pt );
extern long mpdrv_set_bread_ns( long device, long func, long pt, long phase, long block );
extern long mpdrv_set_bwrite_ns( long device, long func, long pt, long phase, long block,
				   const void *src, long size );
extern long mpdrv_set_getlerr_ns( long device, long func, long pt, long phase, long block );
extern long mpdrv_set_arctrl_ns( long device, const void *buf );
extern long mpdrv_set_ft4ctrl_ns( long device, const void *buf );
extern long mpdrv_set_setcond_ns( long device, long func, const void *buf, long size );

#endif
