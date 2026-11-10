/* libjamma's shared declarations: the JAMMA I/O nodes, the credit service and the V-blank calls
   (the libam routines they use are declared here too). */
#ifndef NLAJAMMA_H
#define NLAJAMMA_H

#define NLA_MAX_JAMMA		31
#define NLA_MAX_JAMMA_COMMAND	0x100
#define NLA_MAX_JAMMA_INPUT	512
#define NLA_MAX_JAMMA_AUTO_COMMAND	0x20
#define NLA_CHUTES		4

#define NLAJ_FAIL		0
#define NLAJ_SUCCESS		1
#define NLAJ_ACK_NO_OFFSET	-1

/* the credit service's switch bits */
#define NLAC_SW_SERVICE1	0x10

/* the commands of the JAMMA I/O protocol */
#define NLA_JAMMA_COMMAND_IO_ID			0x10
#define NLA_JAMMA_COMMAND_COMMAND_REV		0x11
#define NLA_JAMMA_COMMAND_JV_REV		0x12
#define NLA_JAMMA_COMMAND_COMMUNICATION_REV	0x13
#define NLA_JAMMA_COMMAND_FUNCTION_CHECK	0x14
#define NLA_JAMMA_COMMAND_SWITCH_READ		0x20
#define NLA_JAMMA_COMMAND_COIN_READ		0x21
#define NLA_JAMMA_COMMAND_ANALOG_READ		0x22
#define NLA_JAMMA_COMMAND_ROTARY_READ		0x23
#define NLA_JAMMA_COMMAND_KEY_CODE_READ		0x24
#define NLA_JAMMA_COMMAND_SCREEN_READ		0x25
#define NLA_JAMMA_COMMAND_HOPPER_READ		0x2e
#define NLA_JAMMA_COMMAND_COIN_DECREMENT	0x30
#define NLA_JAMMA_COMMAND_ADDRESS_SET		0xf1

#define NLA_JAMMA_ACK_OK	0
#define NLA_JAMMA_STATUS_OK	1
#define NLA_JAMMA_REPORT_OK	1
#define NLA_JAMMA_REPORT_PARAMETER_ERROR1	2
#define NLA_JAMMA_SENSE_00V	0x8e

#define NLAJ_JAMMA_COIN_STUFF		0x4000
#define NLAJ_JAMMA_COIN_COUNTER_BREAK	0x8000
#define NLAJ_JAMMA_COIN_BUSY		( NLAJ_JAMMA_COIN_STUFF | NLAJ_JAMMA_COIN_COUNTER_BREAK )

#define NLAJ_SW_IN_SERVICE	6

#define NLA_PER_BUSY		1

/* SHC's built-ins, as its smachine.h names them */
#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

/* a node's description, from its function check */
enum { NLA_JAMMA_SWITCH_PLAYERS, NLA_JAMMA_SWITCH_SWITCHES, NLA_JAMMA_SWITCH_BYTES };

typedef struct {
	int	node;
	char	IDCode[128];
	char	CommandFormatVersion;
	char	VideoStandardVersion;
	char	CommunicationVersion;
	int	AcknowledgeError;
	int	ReportError;
	int	CoinError;
	unsigned char	Switch[3];
	unsigned char	Coin;
	unsigned char	Analog;
	unsigned char	Rotary;
	unsigned char	Keycode;
	unsigned char	Screen[3];
	unsigned char	Card;
	unsigned char	Hopper;
	unsigned char	Driver;
	unsigned char	AnalogOut;
	unsigned char	Character[2];
	unsigned char	Backup;
	unsigned int	( *ReadFunction )();
} NLA_JAMMA_INFO;

/* the acknowledge: the board's header, then the packet */
typedef struct {
	char	RequestID;
	char	SenseStatus;
	char	AckNode;
	char	AckStatus;
	char	PacketByte;
	char	SyncCode;
	char	Node;
	char	Byte;
	char	Status;
	char	Report;
	char	Data;
} NLA_JAMMA_ACKNOWLEDGE;

/* the walk through a node's acknowledge, one report at a time */
typedef struct {
	int	ackbytes;
	int	combytes;
	unsigned char	*com;
	unsigned char	*ack;
} NLAJ_ACK_INFO;

extern int nlaVblankOff;
extern int nlaJAMMANodes;
extern int nlaJAMMAPrimaryNode;
extern int nlaTestMode;
extern char nlaOutput6149PortD;

extern NLA_JAMMA_INFO nlaJAMMAInfo[NLA_MAX_JAMMA];
extern char nlaJAMMAInput[NLA_MAX_JAMMA][NLA_MAX_JAMMA_INPUT];
extern char *nlaJAMMAAcknowledgeIndex[NLA_MAX_JAMMA][NLA_MAX_JAMMA_COMMAND];

/* libam */
extern int nlaCheckPerStatus( void );
extern void nlaVblankFunctionEnd( void );
extern unsigned int *nlaGetCoinMeterCount( void );
extern int nlaCreditServiceRoutine( int sw, unsigned int *coin );
extern int nlaCheckWaitROMDMA( void );
extern void nlaUpdateCreditData( void );
extern void nlaUpdateTotalTime( void );
extern int nlaJAMMARequest( int id, int node, int bytes, char *command );
extern int nlaJAMMAAutoCommandHeadRequest( int id, int node, int bytes, char *command );
extern int nlaJAMMAAutoCommandEntry( int node, int bytes, char *command );
extern int nlaJAMMARequestDoubleAuto( int id, int nodes, int bytes, char *command );
extern NLA_JAMMA_ACKNOWLEDGE *nlaJAMMAAcknowledge( int id );
extern int nlaJAMMAResetRequest( void );
extern void nlaInitOldJAMMA( void );
extern void nlaJAMMAOldRead( int node, int *sw, unsigned int *coin );

/* libjamma */
extern void nlaJAMMARead( int node, int *sw, unsigned int *coin );
extern void nlaJAMMAReadDouble( int node1, int *sw1, unsigned int *coin1,
				int node2, int *sw2, unsigned int *coin2 );
extern void nlaJAMMASetPrimaryNode( int node );
extern int nlaJAMMAFunctionCheck( int node );
extern void nlaJAMMAInit( void );

#endif
