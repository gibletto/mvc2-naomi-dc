/* libjamma's JAMMA I/O: finding the boards, checking their functions, and reading their coins and
   service switches through libam's requests and acknowledges. */
#include "nlajamma.h"

int nlaJAMMANodes;
int nlaJAMMAPrimaryNode;
NLA_JAMMA_INFO nlaJAMMAInfo[NLA_MAX_JAMMA];
int nlaJAMMAErrorAcknowledge;
int nlaJAMMAErrorReport;
int nlaJAMMAAcknowError[NLA_MAX_JAMMA][8];
int nlaJAMMAStatusError[NLA_MAX_JAMMA][8];
int nlaJAMMAAutoCommandBytes[NLA_MAX_JAMMA];
char nlaJAMMAAutoCommand[NLA_MAX_JAMMA][NLA_MAX_JAMMA_AUTO_COMMAND];
int nlaJAMMAAddCommandBytes[NLA_MAX_JAMMA];
char nlaJAMMAAddCommand[NLA_MAX_JAMMA][NLA_MAX_JAMMA_AUTO_COMMAND];
char nlaJAMMAInput[NLA_MAX_JAMMA][NLA_MAX_JAMMA_INPUT];
char *nlaJAMMAAcknowledgeIndex[NLA_MAX_JAMMA][NLA_MAX_JAMMA_COMMAND];
char nlaJAMMARecentAutoCommand[NLA_MAX_JAMMA][NLA_MAX_JAMMA_AUTO_COMMAND * 2];
int nlaJAMMACoinOld[NLA_MAX_JAMMA][NLA_CHUTES];
int nlaJAMMACoinFlg[NLA_MAX_JAMMA][NLA_CHUTES];
int nlaSystemBackupUpdateFlag;

extern int ( *nlaJAMMAAckFunctionTable[] )();

/* Index where each report of the acknowledge starts, by the command it answers. */
int nlaJAMMAMakeAcknowledgeIndex( int node, int byte, char *ack, char *com )
{
	int funcno;
	int flag;
	int offset;
	NLAJ_ACK_INFO info;

	if ( *com == 0 )
		return NLAJ_FAIL;

	info.ackbytes = byte;
	info.ack = ( unsigned char * )ack;
	info.com = ( unsigned char * )com;

	while ( info.ackbytes > 0 ) {
		funcno = ( int )( ( unsigned char )*( info.com ) );
		offset = ( int )( ( char * )( info.ack ) - ( char * )ack );
		nlaJAMMAAcknowledgeIndex[node][funcno] = ( char * )NLAJ_ACK_NO_OFFSET;
		flag = ( nlaJAMMAAckFunctionTable[funcno] )( &info );
		if ( flag != 1 ) {
			nlaJAMMAErrorReport++;
			return NLAJ_FAIL;
		}
		nlaJAMMAAcknowledgeIndex[node][funcno] = ( char * )offset;
	}
	return NLAJ_SUCCESS;
}

/* The commands sent last: the node's own, then the extra ones. */
void nlaJAMMAKeepCommandTail( int node, int bytes, char *com )
{
	char *dst, *src;
	int i;

	dst = &( nlaJAMMARecentAutoCommand[node][0] );
	src = &( nlaJAMMAAutoCommand[node][0] );
	for ( i = 0 ; i < nlaJAMMAAutoCommandBytes[node] ; i++ )
		*dst++ = *src++;
	src = com;
	for ( i = 0 ; i < bytes ; i++ )
		*dst++ = *src++;
}

/* The commands sent last: the extra ones, then the node's own. */
void nlaJAMMAKeepCommandHead( int node, int bytes, char *com )
{
	char *dst, *src;
	int i;

	dst = &( nlaJAMMARecentAutoCommand[node][0] );
	src = com;
	for ( i = 0 ; i < bytes ; i++ )
		*dst++ = *src++;
	src = &( nlaJAMMAAutoCommand[node][0] );
	for ( i = 0 ; i < nlaJAMMAAutoCommandBytes[node] ; i++ )
		*dst++ = *src++;
}

/* Checks an acknowledge, and copies its reports to the node's input. */
int nlaJAMMAAcknowledgeCheckSub( int node, NLA_JAMMA_INFO *inf, NLA_JAMMA_ACKNOWLEDGE *ack )
{
	char *src, *dst;
	int i;

	if ( ack == 0 ) {
		inf->AcknowledgeError = 1;
		inf->ReportError = 1;
		nlaJAMMAErrorAcknowledge++;
		return NLAJ_FAIL;
	}

	if ( ( ack->AckStatus != NLA_JAMMA_ACK_OK ) ||
	     ( ack->Status != NLA_JAMMA_STATUS_OK ) ||
	     ( ack->PacketByte == 0 ) ) {
		inf->AcknowledgeError = 1;
		inf->ReportError = 1;
		nlaJAMMAErrorAcknowledge++;
		if ( ack->AckStatus != NLA_JAMMA_ACK_OK )
			( nlaJAMMAAcknowError[node][ack->AckStatus] )++;
		if ( ack->Status != NLA_JAMMA_STATUS_OK )
			( nlaJAMMAStatusError[node][ack->Status] )++;
		return NLAJ_FAIL;
	}

	if ( node != ack->AckNode )
		return NLAJ_FAIL;

	src = ( char * )( &( ack->Report ) );
	dst = &( nlaJAMMAInput[node][0] );
	for ( i = 0 ; i < ack->PacketByte ; i++ )
		*dst++ = *src++;

	i = nlaJAMMAMakeAcknowledgeIndex( node, ack->Byte - 2, &( nlaJAMMAInput[node][0] ),
					  &( nlaJAMMARecentAutoCommand[node][0] ) );
	if ( i != NLAJ_SUCCESS ) {
		inf->ReportError = 1;
		return NLAJ_FAIL;
	}
	return NLAJ_SUCCESS;
}

/* The same check, without copying the reports. */
int nlaJAMMAAcknowledgeCheckSub2( int node, NLA_JAMMA_INFO *inf, NLA_JAMMA_ACKNOWLEDGE *ack )
{
	if ( ack == 0 ) {
		inf->AcknowledgeError = 1;
		inf->ReportError = 1;
		nlaJAMMAErrorAcknowledge++;
		return NLAJ_FAIL;
	}

	if ( ( ack->AckStatus != NLA_JAMMA_ACK_OK ) ||
	     ( ack->Status != NLA_JAMMA_STATUS_OK ) ||
	     ( ack->PacketByte == 0 ) ) {
		inf->AcknowledgeError = 1;
		inf->ReportError = 1;
		nlaJAMMAErrorAcknowledge++;
		if ( ack->AckStatus != NLA_JAMMA_ACK_OK )
			( nlaJAMMAAcknowError[node][ack->AckStatus] )++;
		if ( ack->Status != NLA_JAMMA_STATUS_OK )
			( nlaJAMMAStatusError[node][ack->Status] )++;
		return NLAJ_FAIL;
	}

	if ( node != ack->AckNode )
		return NLAJ_FAIL;
	return NLAJ_SUCCESS;
}

#define COIN_DEC_LIMIT	0x1000

/* The coins the board counted since the last read; a count too high to hold is taken off it. */
int nlaJAMMACheckCoinCount( int node, NLA_JAMMA_INFO *inf, unsigned int *coinin )
{
	char *offset;
	unsigned char *src;
	int coindata;
	int coinerror;
	int i, j, max;

	offset = nlaJAMMAAcknowledgeIndex[node][NLA_JAMMA_COMMAND_COIN_READ];
	src = ( unsigned char * )( ( int )&( nlaJAMMAInput[node][0] ) + ( int )offset );
	if ( ( int )offset == NLAJ_ACK_NO_OFFSET )
		return 0;
	if ( *src != NLA_JAMMA_REPORT_OK )
		return 0;
	src++;

	max = inf->Coin;
	if ( max > NLA_CHUTES )
		max = NLA_CHUTES;

	for ( i = 0 ; i < max ; i++ ) {
		coindata = ( ( *( src + 0 ) << 8 ) | *( src + 1 ) << 0 );
		coinerror = ( ( coindata & ( NLAJ_JAMMA_COIN_STUFF | NLAJ_JAMMA_COIN_COUNTER_BREAK |
					     NLAJ_JAMMA_COIN_BUSY ) ) );
		src += 2;
		if ( coinerror != 0 ) {
			inf->CoinError |= coinerror;
			continue;
		}
		if ( coindata == 0 ) {
			nlaJAMMACoinFlg[node][i] = 0;
			continue;
		}
		if ( coindata > COIN_DEC_LIMIT ) {
			if ( nlaJAMMACoinFlg[node][i] == 0 ) {
				coinin[i] = coindata - nlaJAMMACoinOld[node][i];
				nlaJAMMACoinOld[node][i] = 0;
			}
			j = nlaJAMMAAddCommandBytes[node];
			nlaJAMMAAddCommand[node][j + 0] = NLA_JAMMA_COMMAND_COIN_DECREMENT;
			nlaJAMMAAddCommand[node][j + 1] = i + 1;
			nlaJAMMAAddCommand[node][j + 2] = ( coindata >> 8 ) & 0xff;
			nlaJAMMAAddCommand[node][j + 3] = ( coindata >> 0 ) & 0xff;
			nlaJAMMAAddCommandBytes[node] += 4;
			nlaJAMMACoinFlg[node][i] = 1;
		} else if ( nlaJAMMACoinOld[node][i] != coindata ) {
			coinin[i] = coindata - nlaJAMMACoinOld[node][i];
			nlaJAMMACoinOld[node][i] = coindata;
		}
	}
	return 1;
}

/* The service switch of each side. */
int nlaJAMMACheckServiceButton( int node, NLA_JAMMA_INFO *inf, int *servsw )
{
	char *src;
	char *offset;
	int swbytes;
	int i;
	int max;

	offset = nlaJAMMAAcknowledgeIndex[node][NLA_JAMMA_COMMAND_SWITCH_READ];
	src = ( char * )( ( int )&( nlaJAMMAInput[node][0] ) + ( int )offset );
	if ( ( int )offset != NLAJ_ACK_NO_OFFSET ) {
		if ( *src == NLA_JAMMA_REPORT_OK ) {
			*servsw = 0;
			swbytes = inf->Switch[NLA_JAMMA_SWITCH_BYTES];
			max = inf->Switch[NLA_JAMMA_SWITCH_PLAYERS];
			if ( max > NLA_CHUTES )
				max = NLA_CHUTES;
			src += 2;
			for ( i = 0 ; i < max ; i++ ) {
				if ( ( *src & ( 1 << NLAJ_SW_IN_SERVICE ) ) != 0 ) {
					if ( i < NLA_CHUTES )
						*servsw |= ( NLAC_SW_SERVICE1 << i );
				}
				src += swbytes;
			}
		}
	}
}

void nlaJAMMARead( int node, int *servsw, unsigned int *coinin )
{
	NLA_JAMMA_ACKNOWLEDGE *ack;
	NLA_JAMMA_INFO *inf;
	int flag;

	ack = nlaJAMMAAcknowledge( 0 );
	inf = &( nlaJAMMAInfo[node] );
	flag = nlaJAMMAAcknowledgeCheckSub( node, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		nlaJAMMAKeepCommandHead( node, nlaJAMMAAddCommandBytes[node],
					 &( nlaJAMMAAddCommand[node][0] ) );
		nlaJAMMAAutoCommandHeadRequest( 0, node, nlaJAMMAAddCommandBytes[node],
						&( nlaJAMMAAddCommand[node][0] ) );
		nlaJAMMAAddCommandBytes[node] = 0;
		return;
	}

	nlaJAMMACheckCoinCount( node, inf, coinin );
	nlaJAMMACheckServiceButton( node, inf, servsw );

	nlaJAMMAKeepCommandHead( node, nlaJAMMAAddCommandBytes[node],
				 &( nlaJAMMAAddCommand[node][0] ) );
	nlaJAMMAAutoCommandHeadRequest( 0, node, nlaJAMMAAddCommandBytes[node],
					&( nlaJAMMAAddCommand[node][0] ) );
	nlaJAMMAAddCommandBytes[node] = 0;
}

static char addcmd[16];

/* Joins two nodes' extra commands into one request. */
int nlaJAMMAMakeDoubleCommand( int node1, int node2 )
{
	char *dst;
	int i;

	dst = &( addcmd[0] );

	nlaJAMMAKeepCommandHead( node1, nlaJAMMAAddCommandBytes[node1],
				 &( nlaJAMMAAddCommand[node1][0] ) );
	*dst++ = node1;
	*dst++ = nlaJAMMAAddCommandBytes[node1];
	for ( i = 0 ; i < nlaJAMMAAddCommandBytes[node1] ; i++ )
		*dst++ = nlaJAMMAAddCommand[node1][i];

	nlaJAMMAKeepCommandHead( node2, nlaJAMMAAddCommandBytes[node2],
				 &( nlaJAMMAAddCommand[node2][0] ) );
	*dst++ = node2;
	*dst++ = nlaJAMMAAddCommandBytes[node2];
	for ( i = 0 ; i < nlaJAMMAAddCommandBytes[node2] ; i++ )
		*dst++ = nlaJAMMAAddCommand[node2][i];

	return ( int )( dst - addcmd );
}

void nlaJAMMAReadDouble( int node1, int *servsw1, unsigned int *coinin1, int node2, int *servsw2,
			 unsigned int *coinin2 )
{
	NLA_JAMMA_ACKNOWLEDGE *ack;
	NLA_JAMMA_INFO *inf;
	int flag;
	int bytes;

	ack = nlaJAMMAAcknowledge( 0 );
	inf = &( nlaJAMMAInfo[node1] );
	flag = nlaJAMMAAcknowledgeCheckSub( node1, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		bytes = nlaJAMMAMakeDoubleCommand( node1, node2 );
		nlaJAMMARequestDoubleAuto( 0, 2, bytes, addcmd );
		nlaJAMMAAddCommandBytes[node1] = 0;
		nlaJAMMAAddCommandBytes[node2] = 0;
		return;
	}
	nlaJAMMACheckCoinCount( node1, inf, coinin1 );
	nlaJAMMACheckServiceButton( node1, inf, servsw1 );

	/* the second node's report follows the first's packet */
	ack = ( NLA_JAMMA_ACKNOWLEDGE * )( ( unsigned char * )ack + ack->PacketByte + 3 );
	inf = &( nlaJAMMAInfo[node2] );
	flag = nlaJAMMAAcknowledgeCheckSub( node2, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		bytes = nlaJAMMAMakeDoubleCommand( node1, node2 );
		nlaJAMMARequestDoubleAuto( 0, 2, bytes, addcmd );
		nlaJAMMAAddCommandBytes[node1] = 0;
		nlaJAMMAAddCommandBytes[node2] = 0;
		return;
	}
	nlaJAMMACheckCoinCount( node2, inf, coinin2 );
	nlaJAMMACheckServiceButton( node2, inf, servsw2 );

	bytes = nlaJAMMAMakeDoubleCommand( node1, node2 );
	nlaJAMMARequestDoubleAuto( 0, 2, bytes, addcmd );
	nlaJAMMAAddCommandBytes[node1] = 0;
	nlaJAMMAAddCommandBytes[node2] = 0;
}

void nlaJAMMASetPrimaryNode( int node )
{
	nlaJAMMAPrimaryNode = node;
}

/* The board needs time between accesses; the count must not be optimised away. */
static void jammaAccessWait( void )
{
	volatile int timer;

	timer = 1000000;
	while ( timer-- )
		;
}

/* Gives each board on the chain an address in turn until one's sense line drops to 0 V.
   The number of boards, 0 if there is none, -1 or -2 if a board answers wrongly. */
int nlaJAMMASearchNodes( void )
{
	NLA_JAMMA_ACKNOWLEDGE *ack;
	char req[4];
	int adr;

	jammaAccessWait();
	nlaJAMMAResetRequest();
	jammaAccessWait();
	nlaJAMMAResetRequest();

	for ( adr = 0 ; adr < 10 ; adr++ ) {
		jammaAccessWait();
		jammaAccessWait();
		jammaAccessWait();
		jammaAccessWait();
		jammaAccessWait();
		jammaAccessWait();
		jammaAccessWait();
		jammaAccessWait();
	}

	for ( adr = 1 ; adr <= NLA_MAX_JAMMA ; adr++ ) {
		req[0] = NLA_JAMMA_COMMAND_ADDRESS_SET;
		req[1] = adr;
		jammaAccessWait();
		nlaJAMMARequest( 0, 0xff, 2, req );
		jammaAccessWait();
		ack = nlaJAMMAAcknowledge( 0 );
		if ( ack == 0 ) {
			return -2;
		} else if ( ( ack->AckStatus != NLA_JAMMA_ACK_OK ) ||
			    ( ack->Status != NLA_JAMMA_STATUS_OK ) ||
			    ( ack->PacketByte == 0 ) ) {
			return -1;
		}
		if ( ack->SenseStatus == ( char )NLA_JAMMA_SENSE_00V )
			break;
	}
	if ( adr == 32 )
		return 0;
	return adr;
}

/* Asks a board for its ID, its revisions and its functions, and sets up the commands that read
   them every frame. */
int nlaJAMMAFunctionCheck( int node )
{
	NLA_JAMMA_ACKNOWLEDGE *ack;
	NLA_JAMMA_INFO *inf;
	int i, flag;
	int p0, p1, p2, p3;
	char jammareq[16];
	char *data;
	char *autocom;

	inf = &( nlaJAMMAInfo[node] );

	jammareq[0] = NLA_JAMMA_COMMAND_IO_ID;
	jammaAccessWait();
	nlaJAMMARequest( 0, node, 1, jammareq );
	jammaAccessWait();
	ack = ( NLA_JAMMA_ACKNOWLEDGE * )nlaJAMMAAcknowledge( 0 );
	flag = nlaJAMMAAcknowledgeCheckSub2( node, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		return flag;
	} else {
		data = &( ack->Data );
		for ( i = 0 ; i < ( ack->Byte - 3 ) ; i++ )
			nlaJAMMAInfo[node].IDCode[i] = *data++;
	}

	jammareq[0] = NLA_JAMMA_COMMAND_COMMAND_REV;
	jammaAccessWait();
	nlaJAMMARequest( 0, node, 1, jammareq );
	jammaAccessWait();
	ack = ( NLA_JAMMA_ACKNOWLEDGE * )nlaJAMMAAcknowledge( 0 );
	flag = nlaJAMMAAcknowledgeCheckSub2( node, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		return flag;
	} else {
		data = &( ack->Data );
		nlaJAMMAInfo[node].CommandFormatVersion = *data++;
	}

	jammareq[0] = NLA_JAMMA_COMMAND_JV_REV;
	jammaAccessWait();
	nlaJAMMARequest( 0, node, 1, jammareq );
	jammaAccessWait();
	ack = ( NLA_JAMMA_ACKNOWLEDGE * )nlaJAMMAAcknowledge( 0 );
	flag = nlaJAMMAAcknowledgeCheckSub2( node, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		return flag;
	} else {
		data = &( ack->Data );
		nlaJAMMAInfo[node].VideoStandardVersion = *data++;
	}

	jammareq[0] = NLA_JAMMA_COMMAND_COMMUNICATION_REV;
	jammaAccessWait();
	nlaJAMMARequest( 0, node, 1, jammareq );
	jammaAccessWait();
	ack = ( NLA_JAMMA_ACKNOWLEDGE * )nlaJAMMAAcknowledge( 0 );
	flag = nlaJAMMAAcknowledgeCheckSub2( node, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		return flag;
	} else {
		data = &( ack->Data );
		nlaJAMMAInfo[node].CommunicationVersion = *data++;
	}

	jammareq[0] = NLA_JAMMA_COMMAND_FUNCTION_CHECK;
	jammaAccessWait();
	nlaJAMMARequest( 0, node, 1, jammareq );
	jammaAccessWait();
	ack = ( NLA_JAMMA_ACKNOWLEDGE * )nlaJAMMAAcknowledge( 0 );
	flag = nlaJAMMAAcknowledgeCheckSub2( node, inf, ack );
	if ( flag == NLAJ_FAIL ) {
		return flag;
	} else {
		autocom = &( nlaJAMMAAutoCommand[node][0] );
		for ( i = 0 ; i < NLA_MAX_JAMMA_AUTO_COMMAND ; i++ )
			*autocom++ = 0;
		autocom = &( nlaJAMMAAutoCommand[node][0] );

		data = &( ack->Data );
		while ( ( int )*data != 0 ) {
			p0 = ( int )*data++;
			p1 = ( int )*data++;
			p2 = ( int )*data++;
			p3 = ( int )*data++;
			switch ( p0 ) {
			case 0x00:
				break;
			case 0x01:
				nlaJAMMAInfo[node].Switch[NLA_JAMMA_SWITCH_PLAYERS] = p1;
				nlaJAMMAInfo[node].Switch[NLA_JAMMA_SWITCH_SWITCHES] = p2;
				if ( p2 == 0 ) {
					p2 = 0;
				} else {
					p2 = ( ( p2 - 1 ) / 8 ) + 1;
				}
				nlaJAMMAInfo[node].Switch[NLA_JAMMA_SWITCH_BYTES] = p2;
				if ( p1 != 0 ) {
					*autocom++ = NLA_JAMMA_COMMAND_SWITCH_READ;
					*autocom++ = p1;
					*autocom++ = p2;
				} else {
					*autocom++ = NLA_JAMMA_COMMAND_SWITCH_READ;
					*autocom++ = 0;
					*autocom++ = 0;
				}
				break;
			case 0x02:
				nlaJAMMAInfo[node].Coin = p1;
				if ( p1 != 0 ) {
					*autocom++ = NLA_JAMMA_COMMAND_COIN_READ;
					*autocom++ = p1;
				}
				break;
			case 0x03:
				nlaJAMMAInfo[node].Analog = p1;
				if ( p1 != 0 ) {
					*autocom++ = NLA_JAMMA_COMMAND_ANALOG_READ;
					*autocom++ = p1;
				}
				break;
			case 0x04:
				nlaJAMMAInfo[node].Rotary = p1;
				if ( p1 != 0 ) {
					*autocom++ = NLA_JAMMA_COMMAND_ROTARY_READ;
					*autocom++ = p1;
				}
				break;
			case 0x05:
				nlaJAMMAInfo[node].Keycode = 1;
				*autocom++ = NLA_JAMMA_COMMAND_KEY_CODE_READ;
				break;
			case 0x06:
				nlaJAMMAInfo[node].Screen[0] = p1;
				nlaJAMMAInfo[node].Screen[1] = p2;
				nlaJAMMAInfo[node].Screen[2] = p3;
				if ( ( p1 != 0 ) && ( p2 != 0 ) && ( p3 != 0 ) ) {
					*autocom++ = NLA_JAMMA_COMMAND_SCREEN_READ;
					*autocom++ = 1;
				}
				break;
			case 0x10:
				nlaJAMMAInfo[node].Card = p1;
				break;
			case 0x11:
				nlaJAMMAInfo[node].Hopper = p1;
				if ( p1 != 0 ) {
					*autocom++ = NLA_JAMMA_COMMAND_HOPPER_READ;
					*autocom++ = p1;
				}
				break;
			case 0x12:
				nlaJAMMAInfo[node].Driver = p1;
				break;
			case 0x14:
				nlaJAMMAInfo[node].Character[0] = p1;
				nlaJAMMAInfo[node].Character[1] = p2;
				break;
			case 0x15:
				nlaJAMMAInfo[node].Backup = 1;
				break;
			default:
				break;
			}
		}
		nlaJAMMAAutoCommandBytes[node] = ( int )( autocom - &( nlaJAMMAAutoCommand[node][0] ) );
	}
	return NLAJ_SUCCESS;
}

/* Finds the boards and starts them reading; with none, falls back to the old JAMMA. */
void nlaJAMMAInit( void )
{
	char *tmp;
	char code[256];
	int size;
	int i, j;
	int nodes;
	int servsw[2];
	unsigned int coinin[NLA_CHUTES][2];
	int flag;

	nlaVblankOff = 0;
	nlaSystemBackupUpdateFlag = 0;
	nlaJAMMAErrorAcknowledge = 0;
	nlaJAMMAErrorReport = 0;

	for ( i = 0 ; i < NLA_MAX_JAMMA ; i++ ) {
		for ( j = 0 ; j < 8 ; j++ ) {
			nlaJAMMAAcknowError[i][j] = 0;
			nlaJAMMAStatusError[i][j] = 0;
		}
	}

	for ( i = 0 ; i < NLA_MAX_JAMMA ; i++ ) {
		for ( j = 0 ; j < NLA_CHUTES ; j++ ) {
			nlaJAMMACoinOld[i][j] = 0;
			nlaJAMMACoinFlg[i][j] = -1;
		}
	}

	nlaJAMMAPrimaryNode = 0;
	for ( i = 0 ; i < NLA_MAX_JAMMA ; i++ ) {
		size = sizeof( NLA_JAMMA_INFO );
		tmp = ( char * )&( nlaJAMMAInfo[i] );
		while ( size-- )
			*tmp++ = 0;
		nlaJAMMAInfo[i].node = i;
		nlaJAMMAAutoCommandBytes[i] = 0;
		nlaJAMMAAddCommandBytes[i] = 0;
		for ( j = 0 ; j < NLA_MAX_JAMMA_COMMAND ; j++ )
			nlaJAMMAAcknowledgeIndex[i][j] = ( char * )-1;
	}

	for ( i = 0 ; i < NLA_MAX_JAMMA ; i++ ) {
		size = NLA_MAX_JAMMA_INPUT;
		tmp = ( char * )&( nlaJAMMAInput[i][0] );
		while ( size-- )
			*tmp++ = 0;
	}

	for ( i = 0 ; i < 3 ; i++ ) {
		nodes = nlaJAMMASearchNodes();
		if ( nodes < 1 )
			break;
	}
	if ( nodes < 1 )
		nlaJAMMANodes = 0;
	else
		nlaJAMMANodes = nodes;

	for ( i = 1 ; i <= nodes ; i++ ) {
		for ( j = 0 ; j < 10 ; j++ ) {
			flag = nlaJAMMAFunctionCheck( i );
			if ( flag == NLAJ_SUCCESS )
				break;
		}
	}

	for ( i = 1 ; i <= nodes ; i++ ) {
		if ( nlaJAMMAAutoCommandBytes[i] != 0 ) {
			jammaAccessWait();
			nlaJAMMAAutoCommandEntry( i, nlaJAMMAAutoCommandBytes[i],
						  &( nlaJAMMAAutoCommand[i][0] ) );
			jammaAccessWait();
			nlaJAMMAKeepCommandHead( i, 0, 0 );
			nlaJAMMAAutoCommandHeadRequest( 0, i, 0, 0 );
		}
	}

	if ( nodes < 1 ) {
		nlaInitOldJAMMA();
		jammaAccessWait();
		nlaJAMMAOldRead( 0, &( servsw[0] ), coinin[0] );
	} else if ( nodes == 1 ) {
		jammaAccessWait();
		nlaJAMMARead( 1, &( servsw[0] ), coinin[0] );
		jammaAccessWait();
		nlaJAMMARead( 1, &( servsw[0] ), coinin[0] );
		jammaAccessWait();
	} else if ( nodes == 2 ) {
		jammaAccessWait();
		nlaJAMMAReadDouble( 2, &( servsw[1] ), coinin[1], 1, &( servsw[0] ), coinin[0] );
		jammaAccessWait();
		nlaJAMMAReadDouble( 2, &( servsw[1] ), coinin[1], 1, &( servsw[0] ), coinin[0] );
		jammaAccessWait();
	}
}
