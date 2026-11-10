/* libjamma's acknowledge walkers: one per JAMMA command, each stepping over the command's bytes
   and its report to the next, and the table that picks the walker by command number. */
#include "nlajamma.h"

static int jammaResetFunc( NLAJ_ACK_INFO *info )
{
	info->com++;
	info->combytes--;
	info->com++;
	info->combytes--;
	return 1;
}

static int jammaMainIDFunc( NLAJ_ACK_INFO *info )
{
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	while ( *info->com != 0 ) {
		info->com++;
		info->combytes--;
	}
	return rep;
}

static int jamma1Req0AckFunc( NLAJ_ACK_INFO *info )
{
	info->com++;
	info->combytes--;
	return 1;
}

static int jamma1ReqReportOnlyAckFunc( NLAJ_ACK_INFO *info )
{
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	return rep;
}

static int jammaAnyReqReportOnlyAckFunc( NLAJ_ACK_INFO *info )
{
	int i, length;
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	length = *info->com;
	info->com++;
	info->combytes--;
	for ( i = 0 ; i < length ; i++ ) {
		info->com++;
		info->combytes--;
	}
	return rep;
}

static int jamma1Req1DataAckFunc( NLAJ_ACK_INFO *info )
{
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	if ( rep != NLA_JAMMA_REPORT_PARAMETER_ERROR1 ) {
		info->ack++;
		info->ackbytes--;
	}
	return rep;
}

static int jammaStringAckFunc( NLAJ_ACK_INFO *info )
{
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	if ( rep != NLA_JAMMA_REPORT_PARAMETER_ERROR1 ) {
		while ( *info->ack != 0 ) {
			info->ack++;
			info->ackbytes--;
		}
	}
	return rep;
}

static int jammaPlayerSwitchAckFunc( NLAJ_ACK_INFO *info )
{
	int players, bytes;
	int i, j;
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	players = *info->com;
	info->com++;
	info->combytes--;
	bytes = *info->com;
	info->com++;
	info->combytes--;
	if ( rep != NLA_JAMMA_REPORT_PARAMETER_ERROR1 ) {
		info->ack++;
		info->ackbytes--;
		for ( i = 0 ; i < players ; i++ ) {
			for ( j = 0 ; j < bytes ; j++ ) {
				info->ack++;
			}
		}
		info->ackbytes -= players * bytes;
	}
	return rep;
}

static int jammaChannelAckFunc( NLAJ_ACK_INFO *info )
{
	int slots;
	int i;
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	slots = *info->com;
	info->com++;
	info->combytes--;
	if ( rep != NLA_JAMMA_REPORT_PARAMETER_ERROR1 ) {
		for ( i = 0 ; i < slots * 2 ; i++ ) {
			info->ack++;
		}
		info->ackbytes -= slots * 2;
	}
	return rep;
}

static int jammaScreenAckFunc( NLAJ_ACK_INFO *info )
{
	int i;
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	info->com++;
	info->combytes--;
	if ( rep != NLA_JAMMA_REPORT_PARAMETER_ERROR1 ) {
		for ( i = 0 ; i < 4 ; i++ ) {
			info->ack++;
		}
		info->ackbytes -= 4;
	}
	return rep;
}

static int jammaHopperAckFunc( NLAJ_ACK_INFO *info )
{
	int i;
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	info->com++;
	info->combytes--;
	if ( rep != NLA_JAMMA_REPORT_PARAMETER_ERROR1 ) {
		for ( i = 0 ; i < 4 ; i++ ) {
			info->ack++;
		}
		info->ackbytes -= 4;
	}
	return rep;
}

static int jammaCoinSlotFunc( NLAJ_ACK_INFO *info )
{
	int rep;

	rep = *info->ack;
	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	info->com += 3;
	info->combytes -= 3;
	return rep;
}

static int jammaAnalogOutFunc( NLAJ_ACK_INFO *info )
{
	int channel;

	info->ack++;
	info->ackbytes--;
	info->com++;
	info->combytes--;
	channel = *info->com;
	info->com += channel * 2;
	info->combytes -= channel * 2;
	return 1;
}

int ( *nlaJAMMAAckFunctionTable[NLA_MAX_JAMMA_COMMAND] )( NLAJ_ACK_INFO *info ) = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x00-0x0f */
	jammaStringAckFunc,	/* 0x10 */
	jamma1Req1DataAckFunc,	/* 0x11 */
	jamma1Req1DataAckFunc,	/* 0x12 */
	jamma1Req1DataAckFunc,	/* 0x13 */
	jammaStringAckFunc,	/* 0x14 */
	jammaMainIDFunc,	/* 0x15 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x16-0x1f */
	jammaPlayerSwitchAckFunc,	/* 0x20 */
	jammaChannelAckFunc,	/* 0x21 */
	jammaChannelAckFunc,	/* 0x22 */
	jammaChannelAckFunc,	/* 0x23 */
	jamma1Req1DataAckFunc,	/* 0x24 */
	jammaScreenAckFunc,	/* 0x25 */
	0, 0, 0, 0, 0, 0, 0, 0,	/* 0x26-0x2d */
	jammaHopperAckFunc,	/* 0x2e */
	0,	/* 0x2f */
	jammaCoinSlotFunc,	/* 0x30 */
	jammaCoinSlotFunc,	/* 0x31 */
	jammaAnyReqReportOnlyAckFunc,	/* 0x32 */
	jammaAnalogOutFunc,	/* 0x33 */
	jammaAnyReqReportOnlyAckFunc,	/* 0x34 */
	jammaCoinSlotFunc,	/* 0x35 */
	jammaCoinSlotFunc,	/* 0x36 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x37-0x46 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x47-0x56 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x57-0x66 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x67-0x76 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x77-0x86 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x87-0x96 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0x97-0xa6 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xa7-0xb6 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xb7-0xc6 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xc7-0xd6 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xd7-0xe6 */
	0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xe7-0xef */
	jammaResetFunc,	/* 0xf0 */
	jamma1ReqReportOnlyAckFunc,	/* 0xf1 */
	jamma1Req0AckFunc,	/* 0xf2 */
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 0xf3-0xff */
};
