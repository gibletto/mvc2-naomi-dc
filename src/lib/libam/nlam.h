/* libam's declarations: what the library's own routines share. */
#ifndef NLAM_H
#define NLAM_H

extern int nlaTestMode;
extern int nlaROMType;
extern unsigned char nlaInput6149[16];
extern char nlaOutput6149PortD;
extern char nlaOutput6149PortE;
extern char nlaOutput6149PortF;
extern char nlaOutput6149DirC;

/* the library's version, and the revision of each of its source files */
typedef struct {
	float version;
	char name[32];
} NLAM_VERSION;

extern char nlamVersionInfo[60];
extern NLAM_VERSION nlamVersions[24];

extern volatile unsigned int *nlaMapleCommand( int command, int sender, int address, unsigned int *data, int words );
extern void nlaMapleCommandNoWait( int command, int sender, int address, unsigned int *data,
				   int words );

/* the system flags: the country, sound, monitor and display settings, and the game's ID */
enum { NLA_COUNTRY_JAPAN, NLA_COUNTRY_USA, NLA_COUNTRY_EXPORT, NLA_COUNTRY_KOREA, NLA_COUNTRY_AUSTRALIA, NLA_COUNTRY_MAX };
enum { NLA_ADVSND_OFF, NLA_ADVSND_ON, NLA_ADVSND_MAX };
enum { NLA_MONITOR_HORIZONTAL, NLA_MONITOR_VERTICAL, NLA_MONITOR_TYPE_MAX };
enum { NLA_DISPLAY_15K, NLA_DISPLAY_31K, NLA_DISPLAY_MODE_MAX };

typedef struct {
	int Country;
	int SoundFlag;
	int MonitorType;
	int DisplayMode;
	unsigned char GameID[4];
} NLA_SYSTEM_FLAG;

/* the credit service: coin chutes, the settings, and what a player has */
#define NLA_CREDIT_SEQUENCE	8
#define NLA_CHUTE_MAX		4

enum { NLA_COINOPE, NLA_FREEPLAY };
enum { NLA_CREDIT_ZERO, NLA_CREDIT_MORE, NLA_CREDIT_ENOUGH };

typedef struct {
	unsigned int Credit;
	unsigned int Remain;
	unsigned int BaseRate;
	unsigned int Status;
} NLA_CREDIT_DATA;

typedef struct {
	NLA_CREDIT_DATA Player[NLA_CHUTE_MAX];
	int CoinFlag;
	int ChuteType;
	unsigned int Chute;
	unsigned int Sequence[NLA_CREDIT_SEQUENCE];
} NLA_CREDIT_INFO;

typedef struct {
	unsigned int MaxCredit;
	int CabinetType;
	int ChuteType;
	int CoinFlag;
	unsigned int CoinSetting;
	unsigned int Sequence[NLA_CREDIT_SEQUENCE];
	unsigned int CoinRate[2];
	unsigned int BonusAdd;
	unsigned int CreditRate;
} NLA_CREDIT_CONF;

/* a player's credit, as the credit service keeps it in bytes */
typedef struct {
	unsigned char credit;
	unsigned char remain;
	unsigned char baseRate;
	unsigned char status;
} CREDIT_STATUS;

/* the credit service's own state, which the boot service works on */
typedef struct {
	unsigned int sequence[NLA_CHUTE_MAX];
	unsigned int credit[NLA_CHUTE_MAX];
	unsigned int unk_20[NLA_CHUTE_MAX];
	unsigned int unk_30[NLA_CHUTE_MAX];
	unsigned int unk_40[9];
	unsigned int coinMeter[NLA_CHUTE_MAX];
	unsigned int sound[16];
	unsigned int soundWrite;
	unsigned int soundRead;
	unsigned int unk_BC[8];
} CREDIT_STATE;

/* the credit data kept in the backup memory */
typedef struct {
	unsigned int Credit[4];
	unsigned int CreditAll[4];
	unsigned int CoinChute[4];
	unsigned int TotalCoin;
	unsigned int CoinCredit;
	unsigned int ServiceCredit;
	unsigned int TotalCredit;
	unsigned int SequenceCount[4][8];
	unsigned int Reserved[8];
} NLA_BACKUP_CREDIT;

/* the frame of the backup memory */
typedef struct {
	unsigned int crc;
	unsigned int dummy;
	NLA_BACKUP_CREDIT credit;
	unsigned int usercrc;
	unsigned int usersize;
	unsigned int *usersram;
	unsigned int *userdata;
} NLA_BACKUP_BANK;

/* what the boot ROM says about the game */
typedef struct {
	char BoardName[16];
	char VenderName[32];
	char GameTitle[8][32];
	short Year;
	char Month;
	char Day;
	char GameID[4];
	short RomMode;
	short RomWaitFlag;
	long RomWait[8];
	short RomID[22][3];
	char CoinFlag[8][16];
	char Sequence[8][32];
	long GameLoad[8][3];
	long TestLoad[8][3];
	long GamePC;
	long TestPC;
	char Country;
	char Cabinet;
	char Resolution;
	char Vertical;
	char SerialID;
	char Reserve[211];
} NLA_BOOTID;

/* the Maple start-up's result */
typedef struct {
	int InitialFlag;
	int OverFlowError;
	int ParityError;
	char IDCode[64];
} NLA_MAPLE_INFO;

typedef struct {
	unsigned short year;
	unsigned char month;
	unsigned char day;
	unsigned char hour;
	unsigned char minute;
	unsigned char second;
	unsigned char dayofweek;
	unsigned char ageofmoon;
} NLA_RTC_DATE;

/* the Kaede driver and the V-blank function, from other modules */
extern void kdBlockKaedeJob( int block );
extern int kdGetDriverStatus( void );
extern void kdSetAppVBFunction( void ( *function )( void ) );
extern void nlaVblankFunction( void );

/* the compiler's built-ins for the interrupt mask */
#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

#endif
