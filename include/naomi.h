/* The NAOMI library's (nlam) system, credit, backup and JAMMA I/O types and calls that the game
   uses, as the NAOMI SDK declares them, and the game's copies of its settings. */
#ifndef NAOMI_H
#define NAOMI_H

#include "types.h"

/* the system flags, set in the board's test mode */
typedef enum { NLA_COUNTRY_JAPAN, NLA_COUNTRY_USA, NLA_COUNTRY_EXPORT, NLA_COUNTRY_KOREA, NLA_COUNTRY_AUSTRALIA } NLA_COUNTRY ;
typedef enum { NLA_ADVSND_OFF, NLA_ADVSND_ON } NLA_ADVSND ;
typedef enum { NLA_MONITOR_HORIZONTAL, NLA_MONITOR_VERTICAL } NLA_MONITOR_TYPE ;
typedef enum { NLA_DISPLAY_15K, NLA_DISPLAY_31K } NLA_DISPLAY_MODE ;

typedef struct NLA_SYSTEM_FLAG {
	NLA_COUNTRY	Country;
	NLA_ADVSND	SoundFlag;	/* 004: the attract's sound */
	NLA_MONITOR_TYPE	MonitorType;
	NLA_DISPLAY_MODE	DisplayMode;	/* 00c */
	Uint8	GameID[4];
} NLA_SYSTEM_FLAG;

/* the coins and credits */
#define NLA_CREDIT_SEQUENCE	8

typedef enum { NLA_CHUTE_1, NLA_CHUTE_2, NLA_CHUTE_3, NLA_CHUTE_4, NLA_CHUTE_MAX } NLA_CHUTE_NUM ;
typedef enum { NLA_CHUTE_COMMON, NLA_CHUTE_INDIVIDUAL } NLA_CHUTE_TYPE ;
typedef enum { NLA_COINOPE, NLA_FREEPLAY } NLA_COIN_FLAG ;
typedef enum { NLA_CREDIT_ZERO, NLA_CREDIT_MORE, NLA_CREDIT_ENOUGH } NLA_CREDIT_FLAG ;
typedef enum { NLA_SEQUENCE_START, NLA_SEQUENCE_CONTINUE } NLA_SEQUENCE ;

/* a side's credits */
typedef struct NLA_CREDIT_DATA {
	Uint32	Credit;
	Uint32	Remain;		/* 004: the coins toward the next credit */
	Uint32	BaseRate;	/* 008: the coins a credit takes */
	NLA_CREDIT_FLAG	Status;	/* 00c: enough to start, more needed, or none */
} NLA_CREDIT_DATA;

typedef struct NLA_CREDIT_INFO {
	NLA_CREDIT_DATA	Player[NLA_CHUTE_MAX];
	NLA_COIN_FLAG	CoinFlag;
	NLA_CHUTE_TYPE	ChuteType;
	Uint32	Chute;
	Uint32	Sequence[NLA_CREDIT_SEQUENCE];	/* 04c: the credits a start or a continue takes */
} NLA_CREDIT_INFO;

/* the coin settings */
typedef struct NLA_CREDIT_CONF {
	Uint32	MaxCredit;
	NLA_CHUTE_NUM	CabinetType;
	NLA_CHUTE_TYPE	ChuteType;	/* 008: individual, each side keeps its own credits */
	NLA_COIN_FLAG	CoinFlag;	/* 00c: free play */
	Uint32	CoinSetting;
	Uint32	Sequence[NLA_CREDIT_SEQUENCE];
	Uint32	CoinRate[2];
	Uint32	BonusAdd;
	Uint32	CreditRate;
} NLA_CREDIT_CONF;

/* the backup memory */
typedef struct NLA_BACKUP_CREDIT {
	Uint32	Credit[4];
	Uint32	CreditAll[4];
	Uint32	CoinChute[4];
	Uint32	TotalCoin;
	Uint32	CoinCredit;
	Uint32	ServiceCredit;
	Uint32	TotalCredit;
	Uint32	SequenceCount[4][8];
	Uint32	Reserved[8];
} NLA_BACKUP_CREDIT;

typedef struct NLA_BACKUP_BANK {
	Uint32	crc;
	Uint32	dummy;
	NLA_BACKUP_CREDIT	credit;
	Uint32	usercrc;
	Uint32	usersize;
	Uint32	*usersram;
	Uint32	*userdata;
} NLA_BACKUP_BANK;

/* a JAMMA I/O board */
#define NLA_JAMMA_COMMAND_SWITCH_READ	0x20
#define NLA_JAMMA_COMMAND_ANALOG_READ	0x22
#define NLA_JAMMA_REPORT_OK	1

typedef struct NLA_JAMMA_INFO {
	int	node;
	char	IDCode[128];
	char	CommandFormatVersion;
	char	VideoStandardVersion;
	char	CommunicationVersion;
	int	AcknowledgeError;
	int	ReportError;
	int	CoinError;
	Uint8	Switch[3];
	Uint8	Coin;
	Uint8	Analog;
	Uint8	Rotary;
	Uint8	Keycode;
	Uint8	Screen[3];
	Uint8	Card;
	Uint8	Hopper;
	Uint8	Driver;
	Uint8	AnalogOut;
	Uint8	Character[2];
	Uint8	Backup;
	Uint32	( *ReadFunction )();
} NLA_JAMMA_INFO;

/* the clock: the game's is 12 bytes, three more than the SDK's of August 2000 */
typedef struct NLA_RTC_DATE {
	Uint16	year;
	Uint8	month;
	Uint8	day;
	Uint8	hour;		/* 004 */
	Uint8	minute;
	Uint8	second;
	Uint8	dayofweek;
	Uint8	ageofmoon;
	Uint8	unk_09[3];
} NLA_RTC_DATE;

extern void nlaGetBootFlag( void ) ;
extern void nlaInitSystem( void ) ;
extern NLA_SYSTEM_FLAG *nlaGetSystemFlag( void ) ;
extern void nlaJAMMAInit( void ) ;
extern void nlaJAMMACreditInit( void ) ;
extern void nlaJAMMASetPrimaryNode( int node ) ;
extern int nlaJAMMANodes ;
extern NLA_JAMMA_INFO nlaJAMMAInfo[31] ;
extern char nlaJAMMAInput[31][512] ;
extern int nlaJAMMAAcknowledgeIndex[31][256] ;
extern int nlaJAMMAAcknowError[31][8] ;	/* each I/O board's error counts */
extern unsigned char nlaInput6149[16] ;
extern void nlaInTestMode( void ) ;
extern int nlaROMTypeSet( int type, int a ) ;
extern NLA_CREDIT_CONF *nlaGetCreditConf( void ) ;
extern void nlaGetCreditData( NLA_CREDIT_INFO *info ) ;
extern void nlaGetBackupData( NLA_BACKUP_BANK *buffer ) ;
extern int nlaInitUserBackup( int size, char *sdram ) ;
extern int nlaInitUserEEPROM( int size, char *sdram ) ;
extern int nlaCheckCreditEnough( int pl, int sequence ) ;
extern void nlaChangeCreditSequence( int pl, int sequence ) ;
extern void nlaUpdateUserBackup( void ) ;
extern void nlaUpdateUserEEPROM( void ) ;
extern void nlaUpdateSystemBackup( void ) ;
extern void nlaReadROMDMA( unsigned long src, char *dst, unsigned long size, int flag ) ;
extern void nlaNRSDecodeRead( unsigned long src, char *dst, unsigned long size, int flag ) ;
extern int nlaCheckWaitROMDMA( void ) ;
extern void nlaJumpTestMode( void ) ;
extern void nlaPutLED( unsigned char led ) ;
extern void nlaGetRTC( NLA_RTC_DATE *date ) ;

/* the game's copies, which system.c takes at start-up and each frame */
extern NLA_SYSTEM_FLAG *sys_flag ;
extern NLA_CREDIT_CONF *credit_conf ;
extern NLA_CREDIT_INFO credit_info ;

#endif
