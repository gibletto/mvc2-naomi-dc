/* libam: the NAOMI board's system layer. Maple commands and the JAMMA I/O requests, the ROM board's
   DMA, the boot service, the credit and backup memory, and the real-time clock. */
#include "nlam.h"
#include "nlahard.h"

/* the boot ROM's copyright text, each character raised by its place in a group of eight */
#define BOOT_ROM_TEXT	"CPR\\VNMOT!*F-XKNA!GQXJXWRJUHW1R[D/\0024=>>'Amn#vnmott\"uixkyvff#f~&ZEHC#ISZLRQTLWJY3LUF1\004\004\005\006\377\000\001\002\003\004\005\006NBQPM%HVOU\"USR\006\007"

/* a frame's descriptor: the last one of the transfer */
#define MAPLE_DESC_LAST	0x80000000

/* the program the 315-6149 I/O controller runs (Sega's binary, which tools/sdkdata.py writes
   from the SDK's libam), the byte that ends it, and the ID the controller answers with */
static char nla6149Program[] = {
#include "nla6149.h"
};
static char nla6149ProgramEnd[] = { 0 };
static char nla6149ID[60] = "\203\0 \007315-6149    COPYRIGHT SEGA E\203\0 \005NTERPRISES CO,LTD.  1998";

int nlaTestMode;
int nlaROMType;
unsigned char nlaInput6149[16];
char nlaOutput6149PortD;
char nlaOutput6149PortE;
char nlaOutput6149PortF;
char nlaOutput6149DirC;

static char mapleRecvArea[0x800];
static char mapleSendArea[0x800];
static unsigned int mapleRecvAddr;
static unsigned int mapleSendAddr;
static volatile unsigned int *mapleRecvBuf;
static volatile unsigned int *mapleSendBuf;
static volatile unsigned int *mapleWrite;

/* The ROM wait set-up for the fastest speed. */
static void romWaitFast( void )
{
	SB_GDAPRO = 0x8843007f;
	SB_G1RRC = 0x600;
	SB_G1RWC = 0x600;
	SB_G1FRC = 0x200;
	SB_G1FWC = 0x200;
	SB_G1CRC = 0x811;
	SB_G1CWC = 0x811;
	SB_G1GDWC = 0x1009;
	SB_G1GDRC = 0x1009;
}

/* Sets the ROM wait states for a bus speed (80 to 210 in steps of 10). */
void nlaROMWaitInit( int speed )
{
	SB_GDAPRO = 0x8843007f;
	SB_G1RRC = 0x600;
	SB_G1RWC = 0x600;
	SB_G1FRC = 0x200;
	SB_G1FWC = 0x200;
	switch ( speed ) {
	case 80:
	case 90:
		SB_G1CRC = 0x211;
		SB_G1CWC = 0x211;
		SB_G1GDWC = 0x1003;
		SB_G1GDRC = 0x1003;
		break;
	case 100:
	case 110:
		SB_G1CRC = 0x311;
		SB_G1CWC = 0x311;
		SB_G1GDWC = 0x1004;
		SB_G1GDRC = 0x1004;
		break;
	case 120:
	case 130:
		SB_G1CRC = 0x411;
		SB_G1CWC = 0x411;
		SB_G1GDWC = 0x1005;
		SB_G1GDRC = 0x1005;
		break;
	case 160:
	case 170:
		SB_G1CRC = 0x611;
		SB_G1CWC = 0x611;
		SB_G1GDWC = 0x1007;
		SB_G1GDRC = 0x1007;
		break;
	case 180:
	case 190:
		SB_G1CRC = 0x711;
		SB_G1CWC = 0x711;
		SB_G1GDWC = 0x1008;
		SB_G1GDRC = 0x1008;
		break;
	case 200:
	case 210:
		SB_G1CRC = 0x811;
		SB_G1CWC = 0x811;
		SB_G1GDWC = 0x1009;
		SB_G1GDRC = 0x1009;
		break;
	case 140:
	case 150:
	default:
		SB_G1CRC = 0x511;
		SB_G1CWC = 0x511;
		SB_G1GDWC = 0x1006;
		SB_G1GDRC = 0x1006;
		break;
	}
}

/* Waits until the G1 DMA has ended. */
void nlaWaitROMDMA( void )
{
	while ( SB_GDST )
		;
}

/* The CRC of a block, that the backup RAM's frames and the EEPROM's records carry: the
   polynomial 0x1021 in the top half, over the bytes and then one zero byte. */
static unsigned short crc16( const unsigned char *data, unsigned int size )
{
	unsigned int crc, i;
	int n;
	unsigned int poly;

	poly = 0x10210000;
	crc = 0xdebdeb00;
	for ( i = 0 ; i < size ; i++ ) {
		crc = ( crc & 0xffffff00 ) + *data++;
		for ( n = 0 ; n < 8 ; n++ ) {
			if ( crc >= 0x80000000 ) {
				crc += crc;
				crc += poly;
			} else
				crc += crc;
		}
	}
	for ( n = 0 ; n < 8 ; n++ ) {
		if ( crc >= 0x80000000 ) {
			crc += crc;
			crc += poly;
		} else
			crc += crc;
	}
	return crc >> 16;
}

/* Sends a frame with data words to the unit and waits for its answer, which it returns. */
volatile unsigned int *nlaMapleCommand( int command, int sender, int address, unsigned int *data, int words )
{
	int i;

	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	*mapleWrite++ = ( sender << 10 ) | MAPLE_DESC_LAST;
	*mapleWrite++ = mapleRecvAddr;
	*mapleWrite++ = ( sender << 16 ) | ( address << 8 ) | command;
	for ( i = 0 ; i < words ; i++ )
		*mapleWrite++ = *data++;
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3;
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24;
	while ( SB_MDST )
		;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
	while ( SB_MDST )
		;
	return mapleRecvBuf;
}

/* The same, without waiting for the answer. */
void nlaMapleCommandNoWait( int command, int sender, int address, unsigned int *data, int words )
{
	int i;

	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	*mapleWrite++ = ( sender << 10 ) | MAPLE_DESC_LAST;
	*mapleWrite++ = mapleRecvAddr;
	*mapleWrite++ = ( sender << 16 ) | ( address << 8 ) | command;
	for ( i = 0 ; i < words ; i++ )
		*mapleWrite++ = *data++;
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3;
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
}

/* A command to the main unit (address 0x20) with no data. */
volatile unsigned int *nlaMapleCommand0( int command )
{
	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	*mapleWrite++ = MAPLE_DESC_LAST;
	*mapleWrite++ = mapleRecvAddr;
	*mapleWrite++ = command | 0x2000;
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3;
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24;
	while ( SB_MDST )
		;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
	while ( SB_MDST )
		;
	return mapleRecvBuf;
}

/* Resets the main unit. */
volatile unsigned int *nlaMapleCommand0Reset( void )
{
	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	*mapleWrite++ = MAPLE_DESC_LAST | 0x300;
	*mapleWrite++ = mapleRecvAddr;
	*mapleWrite++ = 0x2001;
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3;
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24;
	while ( SB_MDST )
		;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
	while ( SB_MDST )
		;
	return mapleRecvBuf;
}

/* Resets a unit. */
volatile unsigned int *nlaMapleResetCommand( int sender, int address )
{
	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	*mapleWrite++ = ( sender << 10 ) | MAPLE_DESC_LAST | 0x300;
	*mapleWrite++ = mapleRecvAddr;
	*mapleWrite++ = ( sender << 16 ) | ( address << 8 ) | 1;
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3;
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24;
	while ( SB_MDST )
		;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
	while ( SB_MDST )
		;
	return mapleRecvBuf;
}

/* Puts the Maple buffers on a 32 byte line and takes their uncached addresses. */
static void nlaMapleBufferInit( void )
{
	mapleRecvAddr = ( ( unsigned int )mapleRecvArea & ~31 ) + 32;
	mapleSendAddr = ( ( unsigned int )mapleSendArea & ~31 ) + 32;
	mapleRecvBuf = ( volatile unsigned int * )( ( mapleRecvAddr & 0x0fffffff ) | P2_AREA );
	mapleSendBuf = ( volatile unsigned int * )( ( mapleSendAddr & 0x0fffffff ) | P2_AREA );
	mapleWrite = mapleSendBuf;
}

/* Nothing to do; the system start calls it after the buffers are set. */
static void nlaMapleDummy( int unused )
{
}

/* Asks the I/O controller for its ID; 1 if it is not the expected one, else 0 and its two
   halves of text in info. */
static int nlaMapleCheck6149( char *info )
{
	volatile unsigned int *res;
	char *id, *p, *q;
	int n;

	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	*mapleWrite++ = MAPLE_DESC_LAST;
	*mapleWrite++ = mapleRecvAddr;
	*mapleWrite++ = 0x2082;
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3;
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24;
	while ( SB_MDST )
		;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
	while ( SB_MDST )
		;

	res = mapleRecvBuf;
	id = ( char * )res;
	if ( *res != -1 ) {
		for ( n = 0 ; n < 56 ; n++ ) {
			if ( id[n] != nla6149ID[n] )
				return 1;
		}
	}
	p = id + 4;
	q = info;
	for ( n = 0 ; n < 28 ; n++ )
		*q++ = *p++;
	p += 4;
	for ( n = 0 ; n < 28 ; n++ )
		*q++ = *p++;
	return 0;
}

/* The system's data in the backup RAM: after a header of 8 bytes come two frames (each a CRC,
   a flag and data), of which one is current and the other a backup. */
typedef struct {
	unsigned int crc;
	unsigned int flag;
	unsigned char data[0xf0];
} SHM_FRAME;

/* the header: the running time, and the running time when the program booted */
typedef struct {
	unsigned int total;
	unsigned int bootTotal;
} SHM_HEAD;

#define SHM_FRAME_SIZE	0xf8
#define SHM_WORD( n )	( *( volatile unsigned int * )( SHM_BASE + ( n ) * 4 ) )
/* a frame's data, behind its CRC and flag */
#define SHM_DATA( f )	( ( unsigned char * )( &( f )->flag + 1 ) )

static int shmCount;
static unsigned char *shmTop;
static SHM_FRAME *shmCur;
static SHM_FRAME *shmBack;
static SHM_HEAD *shmBase;
static unsigned char *flashData;
static unsigned char *flashMark;

/* a record of the I/O controller's EEPROM: 16 bytes under a CRC. There are two, the second a
   copy of the first. */
typedef union {
	struct {
		unsigned short crc;
		unsigned char data[16];
	} f;
	unsigned char b[18];
} FLASH_REC;

static FLASH_REC flashRec0;
static FLASH_REC flashRec1;

/* The user backup in the EEPROM, behind its two records: a header
   (CRC, size, padded size) for each of the two copies, then the copies. */
typedef struct {
	unsigned short crc;
	unsigned char size;
	unsigned char padded;
} EEPROM_BACKUP_HEAD;

static union {
	struct {
		FLASH_REC rec0;
		FLASH_REC rec1;
		EEPROM_BACKUP_HEAD head0;
		EEPROM_BACKUP_HEAD head1;
		unsigned char data[0x200 - 44];
	} f;
	unsigned char b[0x200];
	unsigned int w[0x80];
} flashBuf;

#define MAPLE_PUT( v )	( *mapleWrite++ = ( v ) )

/* Closes the frame in the send buffer: its length, and the sender's check byte. */
#define MAPLE_CLOSE()	\
	*mapleSendBuf |= ( mapleWrite - mapleSendBuf ) - 3; \
	mapleSendBuf[2] |= ( *mapleSendBuf & 0xff ) << 24

/* Sends the program image to the controller, 24 bytes a frame, each with a check sum of its
   bytes and address; 0 when done, 3 when the controller does not answer, 4 when a sum is wrong. */
static int upload6149Program( void )
{
	volatile int wait;
	unsigned int *src;
	const char *sum_p;
	int left, addr, i, sum;

	left = nla6149ProgramEnd - nla6149Program;
	addr = 0x8010;
	src = ( unsigned int * )nla6149Program;
	sum_p = nla6149Program;
	for ( ; left > 0 ; left -= 24 ) {
		wait = 10000;
		while ( wait-- )
			;
		while ( SB_MDST )
			;
		*mapleRecvBuf = -1;
		mapleWrite = mapleSendBuf;
		MAPLE_PUT( MAPLE_DESC_LAST );
		MAPLE_PUT( mapleRecvAddr );
		MAPLE_PUT( 0x2080 );
		MAPLE_PUT( ( ( addr >> 8 & 0xff ) << 16 ) + ( ( addr & 0xff ) << 24 ) );
		MAPLE_PUT( *src++ );
		MAPLE_PUT( *src++ );
		MAPLE_PUT( *src++ );
		MAPLE_PUT( *src++ );
		MAPLE_PUT( *src++ );
		MAPLE_PUT( *src++ );
		MAPLE_CLOSE();
		while ( SB_MDST )
			;
		SB_MDSTAR = mapleSendAddr;
		SB_MDEN = 1;
		SB_MDST = 1;
		while ( SB_MDST )
			;
		sum = 0;
		for ( i = 0 ; i < 24 ; i++ )
			sum += *sum_p++;
		sum += addr & 0xff;
		sum += ( addr & 0xff00 ) >> 8;
		addr += 24;
		if ( *mapleRecvBuf == -1 )
			return 3;
		if ( ( unsigned char )mapleRecvBuf[1] != ( unsigned char )sum )
			return 4;
	}
	while ( SB_MDST )
		;
	*mapleRecvBuf = -1;
	mapleWrite = mapleSendBuf;
	MAPLE_PUT( MAPLE_DESC_LAST );
	MAPLE_PUT( mapleRecvAddr );
	MAPLE_PUT( 0x2080 );
	MAPLE_PUT( -1 );
	MAPLE_CLOSE();
	while ( SB_MDST )
		;
	SB_MDSTAR = mapleSendAddr;
	SB_MDEN = 1;
	SB_MDST = 1;
	while ( SB_MDST )
		;
	return 0;
}

/* Clears the 126 words of the system's part of the backup RAM and starts both frames empty,
   with the flag kept. */
static void shmResetShort( void )
{
	SHM_FRAME *cur, *back;
	unsigned int *p;
	int n, flag;
	unsigned short crc;
	int size;

	flag = shmCur->flag & 1;
	p = ( unsigned int * )SHM_BASE;
	n = 126;
	while ( n-- )
		*p++ = 0;
	cur = shmCur;
	back = shmBack;
	size = SHM_FRAME_SIZE - ( SHM_DATA( cur ) - shmTop );
	crc = crc16( SHM_DATA( cur ), size );
	cur->crc = crc;
	back->crc = crc;
	cur->flag = flag;
	back->flag = flag;
}

/* The same, clearing 0x2000 words. */
static void shmResetLong( void )
{
	SHM_FRAME *cur, *back;
	unsigned int *p;
	int n, flag;
	unsigned short crc;
	int size;

	flag = shmCur->flag & 1;
	n = 0x2000;
	p = ( unsigned int * )SHM_BASE;
	while ( n-- )
		*p++ = 0;
	cur = shmCur;
	back = shmBack;
	size = SHM_FRAME_SIZE - ( SHM_DATA( cur ) - shmTop );
	crc = crc16( SHM_DATA( cur ), size );
	cur->crc = crc;
	back->crc = crc;
	cur->flag = flag;
	back->flag = flag;
}

/* Copies a frame's data and CRC into another. */
static void shmCopyFrame( SHM_FRAME *from, SHM_FRAME *to )
{
	unsigned int *dst, *src;
	int n;

	src = ( unsigned int * )from->data;
	dst = ( unsigned int * )to->data;
	for ( n = 56 ; n-- ; )
		*dst++ = *src++;
	to->crc = from->crc;
}

/* Seals the current frame with its CRC and copies it over the backup. */
static void shmSaveToBack( void )
{
	SHM_FRAME *cur, *back;
	unsigned short crc;
	int size;

	cur = shmCur;
	back = shmBack;
	size = SHM_FRAME_SIZE - ( SHM_DATA( cur ) - shmTop );
	crc = crc16( SHM_DATA( cur ), size );
	cur->crc = crc;
	shmCopyFrame( cur, back );
}

/* Seals the backup the same way and copies it over the current frame. */
static void shmRestoreFromBack( void )
{
	SHM_FRAME *cur, *back;
	unsigned short crc;
	int size;

	cur = shmCur;
	back = shmBack;
	size = SHM_FRAME_SIZE - ( SHM_DATA( cur ) - shmTop );
	crc = crc16( SHM_DATA( cur ), size );
	back->crc = crc;
	shmCopyFrame( back, cur );
}

/* Sets the backup RAM's pointers up and checks its two frames against their CRCs: 0 when both are
   right, 1 when the backup is not, 2 when the current one is not, 3 when neither is (and both
   are cleared). */
static int shmCheck( void )
{
	SHM_FRAME *cur, *back;
	unsigned short crc, crcBack, oldCrc, oldCrcBack;
	int size, result;

	shmCount = 0;
	shmBase = ( SHM_HEAD * )SHM_BASE;
	shmTop = ( unsigned char * )( SHM_BASE + 8 );
	shmCur = ( SHM_FRAME * )( SHM_BASE + 8 );
	shmBack = ( SHM_FRAME * )( SHM_BASE + 0x100 );
	cur = shmCur;
	back = shmBack;
	oldCrc = cur->crc;
	oldCrcBack = back->crc;
	size = SHM_FRAME_SIZE - ( SHM_DATA( cur ) - shmTop );
	crc = crc16( SHM_DATA( cur ), size );
	crcBack = crc16( SHM_DATA( back ), size );
	if ( crc == oldCrc ) {
		if ( crcBack == oldCrcBack ) {
			result = 0;
		} else {
			shmSaveToBack();
			result = 1;
		}
	} else {
		if ( crcBack == oldCrcBack ) {
			shmRestoreFromBack();
			result = 2;
		} else {
			shmResetLong();
			result = 3;
		}
	}
	return result;
}

/* Counts up, but not from -1. */
static int counterNext( unsigned int *counter )
{
	if ( *counter == -1 )
		return 0;
	*counter += 1;
	return *counter;
}

/* 1 when the frame's flag is set. */
static int shmFlagSet( void )
{
	SHM_FRAME *frame;
	int a, b;

	frame = ( SHM_FRAME * )( SHM_BASE + 8 );
	b = frame->flag;
	a = frame->flag;
	if ( a == 0 && b == 0 )
		return 0;
	if ( a == 1 && b == 1 )
		return 1;
	return 0;
}

/* Copies one 18 byte record. */
static void flashCopyRecord( const unsigned char *src, unsigned char *to )
{
	unsigned char *dst = to;
	int n;

	for ( n = 18 ; n-- ; )
		*dst++ = *src++;
}

/* Reads the two records the controller keeps in its EEPROM. */
static void flashReadRecords( void )
{
	register volatile unsigned int *res;
	register unsigned char *src;
	unsigned char *dst;
	register int n;
	unsigned char *src0;
	register unsigned char *dst0;
	register int n0;
	register unsigned char *src1;
	unsigned char *dst1;
	register int n1;
	unsigned int cmd;

	do {
		res = nlaMapleCommand( 1, 0, 32, 0, 0 );
	} while ( *res == -1 );
	cmd = 1;
	nlaMapleCommand( 0x86, 0, 32, &cmd, 1 );
	do {
		res = nlaMapleCommand( 1, 0, 32, 0, 0 );
	} while ( *res == -1 );
	cmd = 3;
	res = nlaMapleCommand( 0x86, 0, 32, &cmd, 1 );
	src = ( unsigned char * )res + 4;
	dst = flashBuf.b;
	n = 0x80;
	do
		*dst++ = *src++;
	while ( --n );
	src0 = flashBuf.b;
	dst0 = flashRec0.b;
	n0 = 18;
	while ( n0-- )
		*dst0++ = *src0++;
	src1 = flashBuf.f.rec1.b;
	dst1 = flashRec1.b;
	n1 = 18;
	while ( n1-- )
		*dst1++ = *src1++;
}

/* Writes the two records to the controller's EEPROM, 16 bytes a frame, and waits for each. */
static void flashWriteRecords( void )
{
	volatile int wait;
	unsigned char frame[32];
	volatile unsigned int *res;
	unsigned char *p, *src, *q;
	int i, n;

	q = flashRec0.b;
	p = flashBuf.b;
	for ( n = 18 ; n-- ; )
		*p++ = *q++;
	q = flashRec1.b;
	p = flashBuf.f.rec1.b;
	for ( n = 18 ; n-- ; )
		*p++ = *q++;
	wait = 100000;
	while ( wait-- )
		;
	src = flashBuf.b;
	for ( i = 0 ; i < 8 ; i++ ) {
		p = &frame[0];
		*p++ = 11;
		*p++ = i * 16;
		*p++ = 16;
		*p++ = 0;
		n = 16;
		do
			*p++ = *src++;
		while ( --n );
		do {
			res = nlaMapleCommand( 1, 0, 32, 0, 0 );
		} while ( *res == -1 );
		nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )frame, 5 );
	}
}

/* Clears both records and seals the first with its CRC. */
static void flashClearRecords( void )
{
	FLASH_REC *from = &flashRec0;
	unsigned char *to = flashRec1.b;
	unsigned char *p;
	int n;
	unsigned short crc;

	p = flashBuf.b;
	n = 36;
	while ( n-- )
		*p++ = 0;
	p = from->b;
	n = 18;
	while ( n-- )
		*p++ = 0;
	crc = crc16( from->f.data, 16 );
	from->f.crc = crc;
	flashCopyRecord( from->b, to );
}

/* Seals the first record with its CRC, copies it to the second and writes both. */
static void flashSaveFirst( void )
{
	FLASH_REC *from = &flashRec0;
	unsigned char *to = flashRec1.b;
	unsigned short crc;

	crc = crc16( from->f.data, 16 );
	from->f.crc = crc;
	flashCopyRecord( from->b, to );
	flashWriteRecords();
}

/* The same from the second record to the first. */
static void flashSaveSecond( void )
{
	unsigned char *to = flashRec0.b;
	FLASH_REC *from = &flashRec1;
	unsigned short crc;

	crc = crc16( from->f.data, 16 );
	from->f.crc = crc;
	flashCopyRecord( from->b, to );
	flashWriteRecords();
}

/* Reads the EEPROM's records and checks their CRCs the way shmCheck does the frames. */
static int flashCheck( void )
{
	FLASH_REC *rec0, *rec1;
	unsigned short crc0, crc1, old1;
	volatile unsigned short old0;
	int result;
	unsigned int *p, *end;

	p = flashBuf.w;
	end = &flashBuf.w[0x80];
	do {
		*p = 0;
		p++;
	} while ( p < end );
	flashReadRecords();
	rec0 = &flashRec0;
	rec1 = &flashRec1;
	flashData = rec0->f.data;
	flashMark = rec0->f.data + 5;
	old0 = rec0->f.crc;
	old1 = rec1->f.crc;
	crc0 = crc16( rec0->f.data, 16 );
	crc1 = crc16( rec1->f.data, 16 );
	if ( crc0 == old0 ) {
		if ( crc1 == old1 ) {
			result = 0;
		} else {
			flashSaveFirst();
			result = 1;
		}
	} else {
		if ( crc1 == old1 ) {
			flashSaveSecond();
			result = 2;
		} else {
			flashClearRecords();
			result = 3;
		}
	}
	return result;
}

/* Writes the first record out twice. */
static void flashWriteFirstTwice( void )
{
	flashSaveFirst();
	flashWriteRecords();
}

/* A pause of about a million loops. */
void pauseLong( void )
{
	volatile int wait;

	wait = 1000000;
	while ( wait-- )
		;
}

/* the request frame the I/O controller takes: a header of the output ports, then the request's
   ID, node and bytes, then the bytes */
static char jammaFrame[0x100];
char nlaOldJAMMAIOID[0x80];

/* Resets the JAMMA I/O boards on the chain. */
int nlaJAMMAResetRequest( void )
{
	unsigned char *p;
	volatile unsigned int *res;

	p = jammaFrame;
	*p++ = 0x17;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = 0;
	*p++ = 0xff;
	*p++ = 2;
	*p++ = 0xf0;
	*p++ = 0xf0 - 0x17;
	*p++ = 0;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, 3 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* Sends a request of up to 12 bytes to a node and waits for the acknowledge. */
int nlaJAMMARequest( int id, int node, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	if ( bytes < 0 || bytes > 12 )
		return -1;

	p = jammaFrame;
	*p++ = 0x17;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	*p++ = bytes;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* The same, but an answer that did not come is not looked for. */
int nlaJAMMARequestNoWait( int id, int node, int bytes, char *command )
{
	char *p;
	int i;

	if ( bytes < 0 || bytes > 12 )
		return -1;

	p = jammaFrame;
	*p++ = 0x17;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	*p++ = bytes;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
}

/* The same, for a request of up to 20 bytes that goes to both of a pair of nodes. */
int nlaJAMMARequestDouble( int id, int node, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	if ( bytes < 0 || bytes > 20 )
		return -1;

	p = jammaFrame;
	*p++ = 0x25;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* The same with the controller answering by itself. */
int nlaJAMMARequestDoubleAuto( int id, int node, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	if ( bytes < 0 || bytes > 20 )
		return -1;

	p = jammaFrame;
	*p++ = 0x27;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* The same code again under another name. */
int nlaJAMMARequestDoubleAuto2( int id, int node, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	if ( bytes < 0 || bytes > 20 )
		return -1;

	p = jammaFrame;
	*p++ = 0x27;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* Acknowledges a node's input; keeps the 16 bytes of input the controller answers with and
   returns the end of its answer, or 0 when it does not answer. */
unsigned char *nlaJAMMAAcknowledge( int node )
{
	char *frame, *p;
	unsigned char *src;
	int i;
	volatile unsigned int *res;

	p = frame = jammaFrame;
	*p++ = 21;
	*p++ = node;
	*p++ = 0;
	*p = 0;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )frame, 1 );
	if ( *res == -1 )
		return 0;
	src = ( unsigned char * )res + 5;
	for ( i = 0 ; i < 16 ; i++ )
		nlaInput6149[i] = *src++;
	return ( unsigned char * )res + 21;
}

/* Asks the controller for its status byte. */
int nlaJAMMAStatus( void )
{
	char *p;
	volatile unsigned int *res;

	p = jammaFrame;
	*p++ = 17;
	*p++ = 0;
	*p++ = 0;
	*p = 0;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* Starts an automatic command: its ID and bytes, after a cleared frame. */
int nlaJAMMAAutoCommandEntry( int id, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	p = jammaFrame;
	for ( i = 0 ; i < 32 ; i++ )
		*p++ = 0;
	p = jammaFrame;
	*p++ = 19;
	*p++ = id;
	*p++ = bytes;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 2 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* A request of up to 25 bytes to one of the controller's extra commands. */
int nlaJAMMAExtraCommandEntry( unsigned int id, unsigned int bytes, char *command )
{
	char *p;
	unsigned int i;
	volatile unsigned int *res;

	if ( bytes > 25 )
		return -1;
	if ( id > 0xff )
		return -1;

	p = jammaFrame;
	*p++ = 55;
	*p++ = id;
	*p++ = bytes;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 2 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* The end of an automatic command: a request of up to 12 bytes. */
int nlaJAMMAAutoCommandTailRequest( int id, int node, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	if ( bytes < 0 || bytes > 12 )
		return -1;

	p = jammaFrame;
	*p++ = 25;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	*p++ = bytes;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
	if ( *res == -1 )
		return -1;
	return ( ( unsigned char * )res )[5];
}

/* The start of one; it does not look whether the controller answered. */
int nlaJAMMAAutoCommandHeadRequest( int id, int node, int bytes, char *command )
{
	char *p;
	int i;
	volatile unsigned int *res;

	if ( bytes < 0 || bytes > 12 )
		return -1;

	p = jammaFrame;
	*p++ = 33;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = id;
	*p++ = node;
	*p++ = bytes;
	for ( i = 0 ; i < bytes ; i++ )
		*p++ = *command++;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, ( bytes + 7 ) / 4 + 1 );
	return ( ( unsigned char * )res )[5];
}

/* Writes the ROM board's port, which is active low. */
static void nlaROMBoardPortSet( unsigned int bits )
{
	ROMB_PORT = ~bits;
}

/* The access of the first JAMMA boards: asks for the 16 bytes of input and keeps them. */
int nlaOldJAMMAAccess( void )
{
	char *p;
	unsigned char *src;
	int i;
	volatile unsigned int *res;

	p = jammaFrame;
	*p++ = 49;
	*p++ = nlaOutput6149DirC;
	*p++ = nlaOutput6149PortD;
	*p++ = nlaOutput6149PortE;
	*p++ = nlaOutput6149PortF;
	*p++ = 0;
	*p++ = 0;
	*p++ = 0;

	res = nlaMapleCommand( 0x86, 0, 32, ( unsigned int * )jammaFrame, 2 );
	if ( *res == -1 )
		return 0;
	src = ( unsigned char * )res + 5;
	for ( i = 0 ; i < 16 ; i++ )
		nlaInput6149[i] = *src++;
	return 1;
}

/* Starts the I/O controller: clears its outputs, asks for its input, and keeps its ID text. */
static void nla6149Init( void )
{
	char *dst, *top, *end;
	char info[0x90];
	char *src;

	nlaOutput6149PortD = 0;
	nlaOutput6149PortE = 0;
	nlaOutput6149PortF = 0;
	nlaOutput6149DirC = 119;
	pauseLong();
	nlaOldJAMMAAccess();
	pauseLong();
	nlaMapleCheck6149( info );
	dst = nlaOldJAMMAIOID;
	end = nlaOldJAMMAIOID + sizeof nlaOldJAMMAIOID;
	do {
		*dst++ = 0;
	} while ( dst < end );
	dst = nlaOldJAMMAIOID;
	top = nlaOldJAMMAIOID;
	end = top + sizeof nlaOldJAMMAIOID;
	src = info;
	while ( dst < end ) {
		*dst = *src;
		if ( *src == 0 )
			break;
		dst++;
		src++;
	}
}

/* The ROM board's DMA: the request for the data of the ROM at an address, and the wait for it. */
int nlaROMTypeSet( int type, int mode )
{
	switch ( type ) {
	case 0:
		nlaROMType = 0;
		break;
	case 1:
		nlaROMType = 0x2000;
		break;
	default:
		return 0;
	}
	switch ( mode ) {
	case 0:
		break;
	case 1:
		nlaROMType |= 0x1000;
		break;
	default:
		return 0;
	}
	ROMB_OFSH = nlaROMType;
	return 1;
}

/* Reads bytes from the ROM through the board's data window. */
void nlaReadROM( unsigned int addr, unsigned short *buf, unsigned int bytes )
{
	ROMB_OFSL = ( unsigned short )addr;
	ROMB_OFSH = ( ( addr & 0xffff0000 ) >> 16 ) | nlaROMType | 0x8000;
	bytes >>= 1;
	while ( bytes-- )
		*buf++ = ROMB_DATA;
}

/* Starts the DMA of ROM data to memory, and waits for it unless nowait is set. */
void nlaReadROMDMA( unsigned int addr, void *dest, unsigned int bytes, int nowait )
{
	volatile unsigned int status;

	addr &= ~31;
	if ( bytes == 0 )
		return;
	while ( SB_GDST )
		;
	ROMB_OFSL = 0;
	ROMB_OFSH = nlaROMType;
	ROMB_DMALEN = bytes >> 5;
	ROMB_DMAL = ( unsigned short )addr;
	ROMB_DMAH = ( ( addr & 0xffff0000 ) >> 16 ) | nlaROMType | 0xa000;
	SB_GDSTAR = ( unsigned int )dest;
	SB_GDLEN = bytes;
	SB_GDDIR = 1;
	SB_GDEN = 1;
	SB_GDST = 1;
	if ( !nowait ) {
		while ( ( status = SB_GDST ) != 0 )
			;
		ROMB_DMAH = 0xc000;
	}
}

/* 1 when the DMA is done, and the board is put back to its normal mode. */
int nlaCheckWaitROMDMA( void )
{
	if ( SB_GDST )
		return 0;
	ROMB_DMAH = 0xc000;
	return 1;
}

static void crcInit( unsigned int *crc )
{
	*crc = 0xdebdeb00;
}

/* Adds a block to a CRC kept in memory, as crc16 works it out. */
static void crcUpdate( unsigned int *crc, const char *data, unsigned int size )
{
	unsigned int i;
	int n;
	unsigned int poly;

	poly = 0x10210000;
	for ( i = 0 ; i < size ; i++ ) {
		*crc = ( *crc & 0xffffff00 ) + *data++;
		for ( n = 0 ; n < 8 ; n++ ) {
			if ( *crc >= 0x80000000 ) {
				*crc += *crc;
				*crc += poly;
			} else
				*crc += *crc;
		}
	}
	for ( n = 0 ; n < 8 ; n++ ) {
		if ( *crc >= 0x80000000 ) {
			*crc += *crc;
			*crc += poly;
		} else
			*crc += *crc;
	}
}

/* The CRC of size bytes of the ROM from addr, read through the buffer buf of bufsize bytes. */
unsigned int nlaGetROMCRC( unsigned int addr, unsigned int size, char *buf, unsigned int bufsize )
{
	unsigned int crc, end, n;

	crcInit( &crc );
	end = addr + size;
	while ( addr < end ) {
		n = end - addr;
		if ( n < bufsize ) {
			nlaReadROMDMA( addr, buf, n, 1 );
			crcUpdate( &crc, buf, n );
			addr += n;
		} else {
			nlaReadROMDMA( addr, buf, bufsize, 1 );
			crcUpdate( &crc, buf, bufsize );
			addr += bufsize;
		}
	}
	return crc >> 16;
}

/* The user backup in the backup RAM: a header for each of its two copies, then the copies.
   The data is padded to words. */
typedef struct {
	unsigned int crc;
	unsigned int size;
	unsigned int padded;
	unsigned int reserved;
} SHM_BACKUP_HEAD;

#define SHM_BACKUP_HEAD0	( ( SHM_BACKUP_HEAD * )( SHM_BASE + 0x1f8 ) )
#define SHM_BACKUP_HEAD1	( ( SHM_BACKUP_HEAD * )( SHM_BASE + 0x208 ) )
#define SHM_BACKUP_DATA		( ( unsigned char * )( SHM_BASE + 0x218 ) )

static int shmBackupOpen;
static SHM_BACKUP_HEAD shmBackupHead0;
static SHM_BACKUP_HEAD shmBackupHead1;
static unsigned char *shmBackupData;
static unsigned int shmBackupSize;
static int shmBackupPadded;

static void clearBytes( int n, unsigned char *p )
{
	while ( n-- )
		*p++ = 0;
}

/* Takes the two headers out of the backup RAM. */
static void shmBackupHeadsRead( void )
{
	unsigned int *dst, *src;
	int n;

	src = ( unsigned int * )SHM_BACKUP_HEAD0;
	dst = ( unsigned int * )&shmBackupHead0;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
	src = ( unsigned int * )SHM_BACKUP_HEAD1;
	dst = ( unsigned int * )&shmBackupHead1;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
}

static void shmBackupHead0Write( void )
{
	unsigned int *dst, *src;
	int n;

	src = ( unsigned int * )&shmBackupHead0;
	dst = ( unsigned int * )SHM_BACKUP_HEAD0;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
}

static void shmBackupHead1Write( void )
{
	unsigned int *dst, *src;
	int n;

	src = ( unsigned int * )&shmBackupHead1;
	dst = ( unsigned int * )SHM_BACKUP_HEAD1;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
}

/* Writes the user's data to the first copy, and to the second. */
static void shmBackupData0Write( void )
{
	unsigned int *dst, *src;
	unsigned int n;

	src = ( unsigned int * )shmBackupData;
	dst = ( unsigned int * )SHM_BACKUP_DATA;
	for ( n = ( unsigned int )shmBackupPadded >> 2 ; n-- ; )
		*dst++ = *src++;
}

static void shmBackupData1Write( void )
{
	unsigned int *dst, *src;
	unsigned int n;

	src = ( unsigned int * )shmBackupData;
	dst = ( unsigned int * )( SHM_BACKUP_DATA + shmBackupPadded );
	for ( n = ( unsigned int )shmBackupPadded >> 2 ; n-- ; )
		*dst++ = *src++;
}

/* The same for a part of the data. */
static void shmBackupData0PartWrite( int offset, int bytes )
{
	unsigned char *dst, *src;
	int n;

	n = bytes;
	src = shmBackupData + offset;
	dst = SHM_BACKUP_DATA + offset;
	while ( n-- )
		*dst++ = *src++;
}

static void shmBackupData1PartWrite( int offset, int bytes )
{
	unsigned char *dst, *src;
	int n;

	n = bytes;
	src = shmBackupData + offset;
	dst = SHM_BACKUP_DATA + shmBackupPadded + offset;
	while ( n-- )
		*dst++ = *src++;
}

/* Seals both headers with the data's CRC and writes everything. */
static void shmBackupWriteAll( void )
{
	unsigned short crc;

	crc = crc16( shmBackupData, shmBackupPadded );
	shmBackupHead0.crc = crc;
	shmBackupHead0.size = shmBackupSize;
	shmBackupHead0.padded = shmBackupPadded;
	shmBackupHead0.reserved = 0;
	shmBackupHead1.crc = crc;
	shmBackupHead1.size = shmBackupSize;
	shmBackupHead1.padded = shmBackupPadded;
	shmBackupHead1.reserved = 0;
	shmBackupHead0Write();
	shmBackupData0Write();
	shmBackupHead1Write();
	shmBackupData1Write();
}

/* The same for a part of the data. */
static void shmBackupWritePart( int offset, int bytes )
{
	unsigned short crc;

	crc = crc16( shmBackupData, shmBackupPadded );
	shmBackupHead0.crc = crc;
	shmBackupHead0.size = shmBackupSize;
	shmBackupHead0.padded = shmBackupPadded;
	shmBackupHead0.reserved = 0;
	shmBackupHead1.crc = crc;
	shmBackupHead1.size = shmBackupSize;
	shmBackupHead1.padded = shmBackupPadded;
	shmBackupHead1.reserved = 0;
	shmBackupHead0Write();
	shmBackupData0PartWrite( offset, bytes );
	shmBackupHead1Write();
	shmBackupData1PartWrite( offset, bytes );
}

/* Reads the user's data back out of the first copy, and out of the second. */
static void shmBackupData0Read( unsigned int bytes, unsigned int *dst )
{
	unsigned int *src;
	bytes >>= 2;
	src = ( unsigned int * )SHM_BACKUP_DATA;
	while ( bytes-- ) {
		*dst = *src;
		dst++;
		src++;
	}
}

static void shmBackupData1Read( unsigned int bytes, unsigned int *dst )
{
	unsigned int *src;
	src = ( unsigned int * )( SHM_BACKUP_DATA + bytes );
	bytes >>= 2;
	while ( bytes-- )
		*dst++ = *src++;
}

/* The most data the backup RAM takes. */
static int shmBackupMaxSize( void )
{
	return 0x3ef4;
}

/* Opens the user backup for size bytes at data: reads it from the backup RAM when a copy
   of it is whole, else clears it. 0 when both copies are right, 1 or 2 when only the first or
   the second is, 3 when neither (data is cleared); -1 when the size does not fit. */
static int shmBackupInit( unsigned int size, unsigned char *data )
{
	unsigned int padded;
	int result;
	unsigned short crc0, crc1, crc;

	shmBackupOpen = 0;
	padded = size;
	if ( size & 3 )
		padded = ( ( size >> 2 ) + 1 ) << 2;
	if ( padded * 2 + 0x218 > 0x8000 || size == 0 )
		return -1;
	shmBackupOpen = 1;
	shmBackupData = data;
	shmBackupSize = size;
	shmBackupPadded = padded;
	shmBackupHeadsRead();
	crc0 = shmBackupHead0.crc;
	crc1 = shmBackupHead1.crc;
	if ( shmBackupHead0.size == size && shmBackupHead1.size == size
	  && shmBackupHead0.padded == padded && shmBackupHead1.padded == padded ) {
		shmBackupData0Read( padded, ( unsigned int * )data );
		crc = crc16( data, padded );
		if ( crc == crc0 ) {
			shmBackupData1Read( padded, ( unsigned int * )data );
			crc = crc16( data, padded );
			if ( crc == crc1 )
				result = 0;
			else
				result = 1;
		} else {
			shmBackupData1Read( padded, ( unsigned int * )data );
			crc = crc16( data, padded );
			if ( crc == crc1 )
				result = 2;
			else
				result = 3;
		}
	} else {
		result = 3;
	}
	switch ( result ) {
	case 0:
		break;
	case 1:
		shmBackupData0Read( padded, ( unsigned int * )data );
		shmBackupWriteAll();
		break;
	case 2:
		shmBackupData1Read( padded, ( unsigned int * )data );
		shmBackupWriteAll();
		break;
	case 3:
		clearBytes( padded, data );
		shmBackupWriteAll();
		break;
	}
	return result;
}

#define EEPROM_BACKUP_DATA	( flashBuf.f.data )
/* the two headers' place in the buffer, behind the records */
#define EEPROM_BACKUP_HEADS	( ( unsigned char * )&flashBuf + 2 * sizeof( FLASH_REC ) )

static int eepromBackupOpen;
static EEPROM_BACKUP_HEAD eepromBackupHead0;
static EEPROM_BACKUP_HEAD eepromBackupHead1;
static unsigned char *eepromBackupData;
static unsigned int eepromBackupSize;
static unsigned int eepromBackupPadded;

static void clearBytes2( int n, unsigned char *p )
{
	while ( n-- )
		*p++ = 0;
}

static void eepromBackupHeadsRead( void )
{
	unsigned char *dst, *src;
	int n;

	src = ( unsigned char * )&flashBuf.f.head0;
	dst = ( unsigned char * )&eepromBackupHead0;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
	src = ( unsigned char * )&flashBuf.f.head1;
	dst = ( unsigned char * )&eepromBackupHead1;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
}

static void eepromBackupHeadsWrite( void )
{
	unsigned char *dst, *src;
	int n;

	src = ( unsigned char * )&eepromBackupHead0;
	dst = EEPROM_BACKUP_HEADS;
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
	src = ( unsigned char * )&eepromBackupHead1;
	dst = EEPROM_BACKUP_HEADS + sizeof( EEPROM_BACKUP_HEAD );
	for ( n = 4 ; n-- ; )
		*dst++ = *src++;
}

static void eepromBackupDataWrite( void )
{
	unsigned char *dst, *src;
	unsigned int n;

	src = eepromBackupData;
	dst = flashBuf.b + 44;
	for ( n = eepromBackupPadded ; n-- ; )
		*dst++ = *src++;
	src = eepromBackupData;
	dst = flashBuf.b + eepromBackupPadded + 44;
	for ( n = eepromBackupPadded ; n-- ; )
		*dst++ = *src++;
}

static void eepromBackupWriteAll( void )
{
	unsigned short crc;

	crc = crc16( eepromBackupData, eepromBackupPadded );
	eepromBackupHead0.crc = crc;
	eepromBackupHead0.size = eepromBackupSize;
	eepromBackupHead0.padded = eepromBackupPadded;
	eepromBackupHead1.crc = crc;
	eepromBackupHead1.size = eepromBackupSize;
	eepromBackupHead1.padded = eepromBackupPadded;
	eepromBackupHeadsWrite();
	eepromBackupDataWrite();
}

static void eepromBackupDataRead( int bytes, unsigned char *dest )
{
	unsigned char *src, *dst;

	src = EEPROM_BACKUP_DATA;
	dst = dest;
	while ( bytes-- )
		*dst++ = *src++;
}

static void eepromBackupData1Read( int bytes, unsigned char *dest )
{
	unsigned char *src, *dst;

	src = flashBuf.b + bytes + 44;
	dst = dest;
	while ( bytes-- )
		*dst++ = *src++;
}

/* The most data the EEPROM takes. */
static int eepromBackupMaxSize( void )
{
	return 42;
}

/* Opens the user backup in the EEPROM, as shmBackupInit does in the backup RAM. */
static int eepromBackupInit( unsigned int size, unsigned char *data )
{
	unsigned int padded;
	int result;
	unsigned short crc0, crc1, crc;

	eepromBackupOpen = 0;
	padded = size;
	if ( size & 3 )
		padded = ( ( size >> 2 ) + 1 ) << 2;
	if ( padded * 2 + 44 > 0x80 || size == 0 )
		return -1;
	eepromBackupOpen = 1;
	eepromBackupData = data;
	eepromBackupSize = size;
	eepromBackupPadded = padded;
	eepromBackupHeadsRead();
	crc0 = eepromBackupHead0.crc;
	crc1 = eepromBackupHead1.crc;
	if ( eepromBackupHead0.size == size && eepromBackupHead1.size == size
	  && eepromBackupHead0.padded == padded && eepromBackupHead1.padded == padded ) {
		eepromBackupDataRead( padded, data );
		crc = crc16( data, padded );
		if ( crc == crc0 ) {
			eepromBackupData1Read( padded, data );
			crc = crc16( data, padded );
			if ( crc == crc1 )
				result = 0;
			else
				result = 1;
		} else {
			eepromBackupData1Read( padded, data );
			crc = crc16( data, padded );
			if ( crc == crc1 )
				result = 2;
			else
				result = 3;
		}
	} else {
		result = 3;
	}
	switch ( result ) {
	case 0:
		break;
	case 1:
		eepromBackupDataRead( padded, data );
		eepromBackupWriteAll();
		break;
	case 2:
		eepromBackupData1Read( padded, data );
		eepromBackupWriteAll();
		break;
	case 3:
		clearBytes2( padded, data );
		eepromBackupWriteAll();
		break;
	}
	return result;
}

/* The boot service: a table of entries that the boot ROM's code is entered through, which
   the program copies to RAM. Words that are offsets are taken from the uncached area. */
typedef void ( *BOOT_FUNC )();
typedef int ( *CREDIT_PROCESS )( NLA_CREDIT_CONF *, CREDIT_STATE *, CREDIT_STATUS *, unsigned int,
				 unsigned int *, NLA_BACKUP_CREDIT * );

static unsigned int *bootTable;

/* Takes the boot service from the boot ROM's area to RAM, when it is whole: each of the first
   eight words must be a jump into it. */
void nlaLoadBootService( void )
{
	unsigned int *src, *dst, word;
	int i, n;
	unsigned int key;

	bootTable = BOOT_SERVICE_RAM;
	src = BOOT_SERVICE_ROM;
	key = 0x0c010000;
	for ( i = 0 ; i < 8 ; i++ ) {
		word = *src++;
		word &= 0x0fff0000;
		if ( word != key )
			return;
	}
	n = BOOT_SERVICE_SIZE;
	src = BOOT_SERVICE_ROM;
	dst = BOOT_SERVICE_RAM;
	while ( n-- )
		*dst++ = *src++;
}

/* The same from a copy in memory. */
void nlaLoadBootServiceDebug( unsigned int *src )
{
	unsigned int *dst;
	int n;

	n = BOOT_SERVICE_SIZE;
	bootTable = BOOT_SERVICE_RAM;
	dst = BOOT_SERVICE_RAM;
	while ( n-- )
		*dst++ = *src++;
}

int nlaBootCall4()
{
	int ( *service )();

	service = ( int ( * )() )bootTable[4];
	return service();
}

unsigned char *nlaBootCall5()
{
	unsigned char *( *service )();

	service = ( unsigned char *( * )() )bootTable[5];
	return service();
}

void nlaInitCreditData()
{
	BOOT_FUNC service;

	service = ( BOOT_FUNC )bootTable[6];
	service();
}

int nlaCredirServiceModeChange()
{
	int ( *service )();

	service = ( int ( * )() )bootTable[13];
	if ( service == 0 )
		return 0;
	return service();
}

int nlaBootCall7()
{
	int ( *service )();

	service = ( int ( * )() )bootTable[7];
	return service();
}

void nlaBootCall8()
{
	BOOT_FUNC service;

	service = ( BOOT_FUNC )bootTable[8];
	service();
}

int nlaCreditProcess( NLA_CREDIT_CONF *conf, CREDIT_STATE *state, CREDIT_STATUS *status,
		      unsigned int coinsw, unsigned int *coinin, NLA_BACKUP_CREDIT *backup )
{
	CREDIT_PROCESS service;

	service = ( CREDIT_PROCESS )bootTable[9];
	return service( conf, state, status, coinsw, coinin, backup );
}

void nlaBootCall10()
{
	BOOT_FUNC service;

	service = ( BOOT_FUNC )bootTable[10];
	service();
}

void nlaBootCall11()
{
	BOOT_FUNC service;

	service = ( BOOT_FUNC )bootTable[11];
	service();
}

void nlaGetTestModeCount( void )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[2] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry();
}

void nlaSetTestModeCount( int count )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[3] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry( count );
}

void nlaJumpGameMode( void )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[16] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry();
}

void nlaJumpTestMode( void )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[17] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry();
}

void nlaJumpGameTestMode( void )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[18] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry();
}

/* The word at an index of the boot service's table. */
unsigned int nlaGetBootFuncitonAddress( int index )
{
	index <<= 2;
	return *( unsigned int * )( ( char * )bootTable + index );
}

void nlaJumpMultiGame( void )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[19] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry();
}

void nlaLoadProgramMultiSlave( void )
{
	unsigned int addr;
	BOOT_FUNC entry;

	addr = bootTable[12] + P2_AREA;
	entry = ( BOOT_FUNC )addr;
	entry();
}

/* The two serial numbers of the boot ROM, 16 characters each. */
void nlaGetSerialID( char *id1, char *id2 )
{
	const char *src;
	char *dst;
	int n;

	src = BOOT_SERIAL1;
	dst = id1;
	for ( n = 0 ; n < 16 ; n++ )
		*dst++ = *src++;
	*dst = 0;
	src = BOOT_SERIAL2;
	dst = id2;
	for ( n = 0 ; n < 16 ; n++ )
		*dst++ = *src++;
	*dst = 0;
}

/* The version of the library. */
float nlaVersionInfo( void )
{
	return nlamVersions[0].version;
}

/* The system flags and the credit settings are kept in the I/O controller's EEPROM: five bytes
   for the flags (the screen and sound settings in the nibbles of the first) and eleven for the
   credit settings. */
int nlaBootRomMode;
unsigned char nlaDIPSwitch;

static NLA_SYSTEM_FLAG sysFlag;
static NLA_CREDIT_CONF creditConf;
static CREDIT_STATE creditState;
static CREDIT_STATUS creditStatus[5];
static unsigned int creditTemp[4];
static int creditTempCount;
static int creditUnk;
static NLA_BACKUP_CREDIT creditBackup;
static int shmUsed;
NLA_MAPLE_INFO nlaMapleInfo;
static int bootTime;

/* Puts the flags back to their defaults if one is out of range; the number of bad ones. */
static int sysFlagCheck( NLA_SYSTEM_FLAG *flag )
{
	int bad;

	bad = 0;
	if ( flag->SoundFlag >= NLA_ADVSND_MAX )
		bad++;
	if ( flag->MonitorType >= NLA_MONITOR_TYPE_MAX )
		bad++;
	if ( bad ) {
		flag->Country = NLA_COUNTRY_JAPAN;
		flag->SoundFlag = NLA_ADVSND_ON;
		flag->MonitorType = NLA_MONITOR_HORIZONTAL;
		flag->DisplayMode = NLA_DISPLAY_31K;
	}
	return bad;
}

/* Packs the credit settings into the EEPROM's bytes. */
static void creditConfPack( const NLA_CREDIT_CONF *conf, unsigned char *dst )
{
	int i;

	dst[1] = ( conf->CabinetType << 4 ) + conf->ChuteType;
	dst[0] = conf->MaxCredit;
	dst[2] = conf->CoinSetting;
	dst[3] = conf->CreditRate;
	dst[4] = conf->CoinRate[0];
	dst[5] = conf->CoinRate[1];
	dst[6] = conf->BonusAdd;
	for ( i = 0 ; i < 4 ; i++ )
		dst[7 + i] = ( conf->Sequence[i * 2] << 4 ) + conf->Sequence[i * 2 + 1];
}

/* The opposite; the boot service sets right what is out of range, and says how much. */
static int creditConfUnpack( const unsigned char *src, NLA_CREDIT_CONF *conf, int dip )
{
	int seq[8];
	int tbl[8] = { 9, 24, 9, 9, 9, 9, 9, 9 };
	const unsigned char *p;
	int i;

	conf->CabinetType = ( src[1] >> 4 ) & 15;
	conf->ChuteType = src[1] & 15;
	conf->MaxCredit = tbl[dip];
	conf->CoinSetting = src[2];
	conf->CreditRate = src[3];
	conf->CoinRate[0] = src[4];
	conf->CoinRate[1] = src[5];
	conf->BonusAdd = src[6];
	p = src + 7;
	for ( i = 0 ; i < 8 ; i += 2 ) {
		seq[i] = ( *p >> 4 ) & 15;
		seq[i + 1] = *p & 15;
		p++;
	}
	for ( i = 0 ; i < 8 ; i++ )
		conf->Sequence[i] = seq[i];
	return nlaBootCall4( conf );
}

/* Unpacks the flags; the bad ones are counted. */
static int sysFlagUnpack( const unsigned char *src, NLA_SYSTEM_FLAG *flag, int country )
{
	int bad;

	flag->Country = country;
	flag->DisplayMode = 1;
	if ( !( nlaDIPSwitch & 1 ) )
		flag->DisplayMode = 0;
	flag->SoundFlag = ( src[0] >> 4 ) & 15;
	flag->MonitorType = src[0] & 15;
	flag->GameID[0] = src[1];
	flag->GameID[1] = src[2];
	flag->GameID[2] = src[3];
	flag->GameID[3] = src[4];
	bad = sysFlagCheck( flag );
	return bad;
}

static void sysFlagPack( const NLA_SYSTEM_FLAG *flag, unsigned char *dst )
{
	dst[0] = flag->SoundFlag * 16 + flag->MonitorType;
	dst[1] = flag->GameID[0];
	dst[2] = flag->GameID[1];
	dst[3] = flag->GameID[2];
	dst[4] = flag->GameID[3];
}

/* Sets the credit settings by hand (num 27 takes the rates as given) and writes them to the
   EEPROM; 0 when they are right. */
int nlaCreditConfForce( int max, int cabi, int type, int num, int coinrate1, int coinrate2,
			int bonusadd, int creditrate, int start, int conti )
{
	unsigned char *rate;
	int bad, i;

	creditConf.MaxCredit = max;
	creditConf.CabinetType = cabi;
	creditConf.ChuteType = type;
	creditConf.CoinSetting = num;
	creditConf.Sequence[0] = start;
	creditConf.Sequence[1] = conti;
	for ( i = 2 ; i < 8 ; i++ )
		creditConf.Sequence[i] = 1;
	if ( num != 27 ) {
		rate = nlaBootCall5( num );
		creditConf.CreditRate = *rate++;
		creditConf.CoinRate[0] = *rate++;
		creditConf.CoinRate[1] = *rate++;
		creditConf.BonusAdd = *rate;
	} else {
		creditConf.CreditRate = creditrate;
		creditConf.CoinRate[0] = coinrate1;
		creditConf.CoinRate[1] = coinrate2;
		creditConf.BonusAdd = bonusadd;
	}
	bad = nlaBootCall4( &creditConf );
	if ( bad )
		return bad;
	creditConf.MaxCredit = max;
	if ( creditConf.MaxCredit > 999 || creditConf.MaxCredit < 1 )
		return 1;
	creditConfPack( &creditConf, flashMark );
	flashWriteFirstTwice();
	nlaInitCreditData( &creditConf, &creditState, &creditBackup, creditStatus );
	nlaCredirServiceModeChange( shmUsed );
	return 0;
}

/* Sets the flags by hand; 0 when they are right. */
int nlaSetSystemFlagForce( int country, int soundflag, int monitor, int display )
{
	NLA_SYSTEM_FLAG flag;
	int bad;

	flag.Country = country;
	flag.SoundFlag = soundflag;
	flag.MonitorType = monitor;
	flag.DisplayMode = display;
	bad = sysFlagCheck( &flag );
	if ( bad == 0 ) {
		sysFlag.Country = country;
		sysFlag.SoundFlag = soundflag;
		sysFlag.MonitorType = monitor;
		sysFlag.DisplayMode = display;
		return 0;
	}
	return bad;
}

NLA_SYSTEM_FLAG *nlaGetSystemFlag( void )
{
	return &sysFlag;
}

/* Finds out whether the boot ROM is the final one, by its copyright text. */
static void bootRomCheck( void )
{
	static char bootText[] = BOOT_ROM_TEXT;
	const char *text;
	const char *rom;
	int i;

	nlaBootRomMode = 0;
	text = bootText;
	rom = BOOT_ROM_COPYRIGHT;
	for ( i = 0 ; i < 112 ; i++, text++, rom++ ) {
		if ( *text - i % 8 != *rom )
			return;
	}
	nlaBootRomMode = 1;
}

int nlaCheckReleaseBootROM( void )
{
	return nlaBootRomMode;
}

/* The running time in seconds, which the backup RAM keeps. */
unsigned int nlaGetTotalTime( void )
{
	return shmBase->total;
}

/* Starts the I/O controller: waits for the Maple bus to answer, checks the controller's ID and
   sends it its program; 0 when it runs, else the step that failed. */
static int maple6149Start( void )
{
	volatile int wait;
	volatile int retry;
	char info[256];
	volatile unsigned int *res;
	int code;

	nlaMapleInfo.InitialFlag = 0;
	nlaMapleInfo.OverFlowError = 0;
	nlaMapleInfo.ParityError = 0;
	nlaMapleBufferInit();
	nlaMapleDummy( 0 );
	for ( wait = 10000 ; wait-- ; )
		;
	for ( retry = 40000 ; retry-- ; ) {
		for ( wait = 10000 ; wait-- ; )
			;
		res = nlaMapleCommand0( 3 );
		if ( *res != -1 )
			break;
	}
	if ( *res == -1 ) {
		code = 2;
		goto error;
	}
	for ( retry = 40000 ; retry-- ; ) {
		for ( wait = 10000 ; wait-- ; )
			;
		res = nlaMapleCommand0( 1 );
		if ( *res != -1 )
			break;
	}
	if ( *res == -1 ) {
		code = 2;
		goto error;
	}
	code = nlaMapleCheck6149( info );
	if ( code ) {
		code = 4;
		goto error;
	} else {
		code = upload6149Program();
		if ( code ) {
			code = 8;
error:
			nlaMapleInfo.InitialFlag = code;
			return code;
		}
	}
	for ( retry = 40000 ; retry-- ; ) {
		for ( wait = 10000 ; wait-- ; )
			;
		res = nlaMapleCommand0( 1 );
		if ( *res != -1 )
			break;
	}
	return 0;
}

/* Puts the credit data in the backup RAM, and takes it from there. */
static void creditBackupSave( void )
{
	unsigned int *dst, *src;
	int n;

	src = ( unsigned int * )&creditBackup;
	dst = ( unsigned int * )( shmTop + 8 );
	for ( n = 0 ; n < sizeof( creditBackup ) / 4 ; n++ )
		*dst++ = *src++;
}

static void creditBackupLoad( void )
{
	unsigned int *dst, *src;
	int n;

	src = ( unsigned int * )( shmTop + 8 );
	dst = ( unsigned int * )&creditBackup;
	for ( n = 0 ; n < sizeof( creditBackup ) / 4 ; n++ )
		*dst++ = *src++;
}

/* Starts the board's system: reads the flags and the credit settings from the EEPROM, where a
   bad one is put right and written back, and starts the credit service and the V-blank function.
   dip is what the DIP switches give, or the country when the final boot ROM is there. */
void nlaInitSystemBoot( int dip, int romwait )
{
	int i, changed;
	int ipl, bad;

	ipl = get_imask();
	nlaLoadBootService();
	nlaTestMode = 0;
	nlaDIPSwitch = 0;
	creditUnk = 0x04444444;
	bootRomCheck();
	if ( DMAOR != DMAOR_SET )
		DMAOR = DMAOR_SET;
	DMAOR = DMAOR_SET;
	if ( romwait == 1 )
		romWaitFast();
	creditTempCount = 0;
	for ( i = 0 ; i < 4 ; i++ )
		creditTemp[i] = 0;
	set_imask( 0 );
	kdBlockKaedeJob( 0 );
	while ( !kdGetDriverStatus() )
		;
	set_imask( 14 );
	maple6149Start();
	nla6149Init();
	nlaDIPSwitch = nlaInput6149[5] & 15;
	if ( !nlaBootRomMode ) {
		nlaROMWaitInit( 0 );
		dip = ( ~nlaDIPSwitch & 15 ) >> 1;
	}
	shmBackupOpen = 0;
	eepromBackupOpen = 0;
	shmCheck();
	shmUsed = shmFlagSet();
	bad = flashCheck();
	if ( bad == 3 )
		*flashData = 16;
	else
		bad = 0;
	bad += sysFlagUnpack( flashData, &sysFlag, dip );
	bad += creditConfUnpack( flashMark, &creditConf, dip );
	creditBackupLoad();
	changed = nlaBootCall7( creditConf.MaxCredit, &creditBackup );
	if ( changed ) {
		while ( SB_GDST )
			;
		creditBackupSave();
		shmSaveToBack();
	}
	nlaInitCreditData( &creditConf, &creditState, &creditBackup, creditStatus );
	nlaCredirServiceModeChange( shmUsed );
	nlaBootCall8( &creditConf, &creditState, creditStatus );
	if ( bad ) {
		sysFlagPack( &sysFlag, flashData );
		creditConfPack( &creditConf, flashMark );
		flashWriteFirstTwice();
	}
	kdSetAppVBFunction( nlaVblankFunction );
	kdBlockKaedeJob( 1 );
	SB_ISTNRM = IST_MAPLE_DMA;
	set_imask( ipl );
}

/* The same without the V-blank function. */
void nlaInitSystemSimpleBoot( int dip, int romwait )
{
	int i, changed;
	int bad;

	nlaLoadBootService();
	nlaTestMode = 0;
	nlaDIPSwitch = 0;
	creditUnk = 0x04444444;
	bootRomCheck();
	if ( DMAOR != DMAOR_SET )
		DMAOR = DMAOR_SET;
	DMAOR = DMAOR_SET;
	if ( romwait == 1 )
		romWaitFast();
	creditTempCount = 0;
	for ( i = 0 ; i < 4 ; i++ )
		creditTemp[i] = 0;
	maple6149Start();
	nla6149Init();
	nlaDIPSwitch = nlaInput6149[5] & 15;
	if ( !nlaBootRomMode ) {
		nlaROMWaitInit( 0 );
		dip = ( ~nlaDIPSwitch & 15 ) >> 1;
	}
	shmBackupOpen = 0;
	eepromBackupOpen = 0;
	shmCheck();
	shmUsed = shmFlagSet();
	bad = flashCheck();
	if ( bad == 3 )
		*flashData = 16;
	else
		bad = 0;
	bad += sysFlagUnpack( flashData, &sysFlag, dip );
	bad += creditConfUnpack( flashMark, &creditConf, dip );
	creditBackupLoad();
	changed = nlaBootCall7( creditConf.MaxCredit, &creditBackup );
	if ( changed ) {
		while ( SB_GDST )
			;
		creditBackupSave();
		shmSaveToBack();
	}
	nlaInitCreditData( &creditConf, &creditState, &creditBackup, creditStatus );
	nlaCredirServiceModeChange( shmUsed );
	nlaBootCall8( &creditConf, &creditState, creditStatus );
	if ( bad ) {
		sysFlagPack( &sysFlag, flashData );
		creditConfPack( &creditConf, flashMark );
		flashWriteFirstTwice();
	}
}

/* The start of the board's system, with the boot ROM's service. */
void nlaInitSystem( void )
{
	bootTime = BOOT_TIME;
	*( unsigned int * )( SHM_BASE + 4 ) = *( unsigned int * )SHM_BASE;
	nlaLoadBootService();
	nlaBootCall10( &sysFlag );
	nlaBootCall11( &creditConf );
	nlaInitSystemBoot( sysFlag.Country, 1 );
}

void nlaInitSystemNoBoot( void )
{
	nlaBootCall10( &sysFlag );
	nlaBootCall11( &creditConf );
	nlaInitSystemBoot( sysFlag.Country, 1 );
}

/* Gets the flags and the settings from the boot ROM. */
void nlaGetBootFlag( void )
{
	bootRomCheck();
	nlaLoadBootService();
	nlaBootCall10( &sysFlag );
	nlaBootCall11( &creditConf );
}

void nlaPutLED( unsigned char bits )
{
	nlaROMBoardPortSet( bits );
}

NLA_CREDIT_CONF *nlaGetCreditConf( void )
{
	return &creditConf;
}

/* The credit state, a player at a time. */
void nlaGetCreditData( NLA_CREDIT_INFO *info )
{
	unsigned char *p;
	int i;

	p = &creditStatus[0].credit;
	for ( i = 0 ; i < NLA_CHUTE_MAX ; i++ ) {
		info->Player[i].Credit = *p++;
		info->Player[i].Remain = *p++;
		info->Player[i].BaseRate = *p++;
		info->Player[i].Status = *p++;
	}
	info->CoinFlag = creditConf.CoinFlag;
	info->ChuteType = creditConf.ChuteType;
	info->Chute = creditConf.CabinetType;
	for ( i = 0 ; i < NLA_CREDIT_SEQUENCE ; i++ )
		info->Sequence[i] = creditConf.Sequence[i];
}

/* The same code again under another name. */
void nlaGetCreditDataP( NLA_CREDIT_INFO *info )
{
	unsigned char *p;
	int i;

	p = &creditStatus[0].credit;
	for ( i = 0 ; i < NLA_CHUTE_MAX ; i++ ) {
		info->Player[i].Credit = *p++;
		info->Player[i].Remain = *p++;
		info->Player[i].BaseRate = *p++;
		info->Player[i].Status = *p++;
	}
	info->CoinFlag = creditConf.CoinFlag;
	info->ChuteType = creditConf.ChuteType;
	info->Chute = creditConf.CabinetType;
	for ( i = 0 ; i < NLA_CREDIT_SEQUENCE ; i++ )
		info->Sequence[i] = creditConf.Sequence[i];
}

/* The next sound the credit service has queued, or 0. */
int nlaGetCreditSound( void )
{
	int n;

	n = creditState.soundRead;
	if ( n == creditState.soundWrite )
		return 0;
	creditState.soundRead = ( n + 1 ) & 15;
	return creditState.sound[n];
}

void nlaChangeCreditSequence( int pl, int sequence )
{
	creditState.sequence[pl] = sequence;
	nlaBootCall8( &creditConf, &creditState, creditStatus );
}

/* Takes the credits a start or continue costs from a player's chute, 1 if there are enough. */
int nlaCheckCreditEnough( int pl, int sequence )
{
	int chute;

	creditState.sequence[pl] = sequence;
	nlaBootCall8( &creditConf, &creditState, creditStatus );
	if ( creditStatus[pl].status != NLA_CREDIT_ENOUGH )
		return 0;
	chute = creditConf.ChuteType == 0 ? 0 : pl;
	if ( creditConf.CoinFlag != NLA_FREEPLAY ) {
		if ( creditConf.ChuteType == 0 ) {
			creditState.unk_30[0] = creditState.unk_30[1] = 0;
		} else
			creditState.unk_30[chute] = 0;
		creditState.credit[chute] -= creditConf.Sequence[sequence];
	}
	nlaBootCall8( &creditConf, &creditState, creditStatus );
	counterNext( &creditBackup.SequenceCount[pl][sequence] );
	creditBackup.Credit[chute] = creditState.credit[chute];
	while ( SB_GDST )
		;
	creditBackupSave();
	shmSaveToBack();
	return 1;
}

/* Writes the flags and the settings to the EEPROM. */
void nlaUpdateEEPROM( void )
{
	sysFlagPack( &sysFlag, flashData );
	creditConfPack( &creditConf, flashMark );
	flashWriteFirstTwice();
}

/* Sets the flags and the settings to the boot ROM's defaults for the country, and writes them. */
void nlaUpdateEEPROMBootDefault( void )
{
	const char *entry;
	unsigned char *rate;
	int num, i;

	entry = BOOT_DEFAULTS + sysFlag.Country * 16;
	switch ( entry[1] ) {
	case 0:
		sysFlag.SoundFlag = 1;
		sysFlag.MonitorType = 0;
		break;
	case 1:
		sysFlag.SoundFlag = 1;
		sysFlag.MonitorType = 1;
		break;
	case 2:
		sysFlag.SoundFlag = 0;
		sysFlag.MonitorType = 0;
		break;
	case 3:
		sysFlag.SoundFlag = 0;
		sysFlag.MonitorType = 1;
		break;
	}
	num = entry[3] - 1;
	rate = nlaBootCall5( num );
	creditConf.ChuteType = entry[2];
	creditConf.CoinSetting = num;
	creditConf.CreditRate = *rate++;
	creditConf.CoinRate[0] = *rate++;
	creditConf.CoinRate[1] = *rate++;
	creditConf.BonusAdd = *rate;
	if ( num == 27 ) {
		creditConf.CoinRate[0] = entry[4];
		creditConf.CoinRate[1] = entry[5];
		creditConf.CreditRate = entry[6];
		creditConf.BonusAdd = entry[7];
	}
	for ( i = 0 ; i < 8 ; i++ )
		creditConf.Sequence[i] = entry[8 + i];
	sysFlagPack( &sysFlag, flashData );
	creditConfPack( &creditConf, flashMark );
	flashWriteFirstTwice();
}

/* The boot ROM's description of the game. */
NLA_BOOTID *nlaGetBootID( void )
{
	return ( NLA_BOOTID * )BOOT_ID;
}

/* Copies the backup RAM's current frame out. */
void nlaGetBackupData( NLA_BACKUP_BANK *buffer )
{
	unsigned int *dst, *src;
	int n;

	while ( SB_GDST )
		;
	src = ( unsigned int * )shmTop;
	dst = ( unsigned int * )buffer;
	for ( n = 62 ; n-- ; )
		*dst++ = *src++;
}

/* Clears the system's backup data (the credit data), or all of the backup RAM. */
void nlaBackupSystemDataClear( void )
{
	while ( SB_GDST )
		;
	shmResetShort();
	creditBackupLoad();
	nlaInitCreditData( &creditConf, &creditState, &creditBackup, creditStatus );
	nlaCredirServiceModeChange( shmUsed );
}

void nlaBackupClear( void )
{
	while ( SB_GDST )
		;
	shmResetLong();
	creditBackupLoad();
	nlaInitCreditData( &creditConf, &creditState, &creditBackup, creditStatus );
	nlaCredirServiceModeChange( shmUsed );
}

int nlaInitUserBackup( int size, unsigned char *sdram )
{
	while ( SB_GDST )
		;
	return shmBackupInit( size, sdram );
}

int nlaInitUserEEPROM( int size, unsigned char *sdram )
{
	return eepromBackupInit( size, sdram );
}

void nlaUpdateUserBackup( void )
{
	while ( SB_GDST )
		;
	shmBackupWriteAll();
}

void nlaUpdateUserEEPROM( void )
{
	eepromBackupWriteAll();
	flashWriteFirstTwice();
}

void nlaUpdateUserBackupPart( int offset, int size )
{
	shmBackupWritePart( offset, size );
}

int nlaCheckUserBackupArea( void )
{
	return shmBackupMaxSize();
}

int nlaCheckUserEEPROMArea( void )
{
	return eepromBackupMaxSize();
}

static const int monthStart[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };

/* The date and time from the real-time clock, which counts the seconds from 1950. */
void nlaGetRTC( NLA_RTC_DATE *date )
{
	int rtc, t, years, leap, doy, month, year, phase;

	rtc = ( RTC_HIGH << 16 ) | RTC_LOW;
	t = rtc;
	date->second = t % 60;
	t /= 60;
	date->minute = t % 60;
	t /= 60;
	date->hour = t % 24;
	t /= 24;
	years = t / 365;
	doy = t % 365;
	leap = ( years + 1 ) / 4;
	if ( leap <= doy ) {
		doy -= leap;
		date->year = 1950 + years;
	} else {
		date->year = 1949 + years;
		doy += 365 - leap;
		if ( date->year % 4 == 0 )
			doy++;
	}
	month = 1;
	if ( date->year % 4 == 0 ) {
		if ( doy >= monthStart[1] ) {
			for ( month = 2 ; month < 12 ; month++ ) {
				if ( doy < monthStart[month] + 1 )
					break;
			}
		}
	} else {
		for ( month = 1 ; month < 12 ; month++ ) {
			if ( doy < monthStart[month] )
				break;
		}
	}
	date->month = month;
	date->day = doy - monthStart[month - 1] + 1;
	if ( date->year % 4 == 0 && date->month > 2 )
		date->day--;
	month = date->month;
	year = date->year;
	if ( month <= 2 ) {
		year--;
		month += 12;
	}
	date->dayofweek = ( year + ( year >> 2 ) + ( 13 * month + 8 ) / 5 + date->day - 15 ) % 7;
	if ( rtc >= 0x5cb417d1 ) {
		phase = rtc - 0x5cb417d1;
		phase %= 0x26ee7f;
	} else {
		phase = 0x5cb417d1 - rtc;
		phase %= 0x26ee7f;
		phase = 0x26ee7f - phase;
	}
	date->ageofmoon = phase / 86400;
}

void nlaInTestMode( void )
{
	nlaTestMode = 1;
}

void nlaExitTestMode( void )
{
	nlaTestMode = 0;
}

/* The coin meter counts of the credit service. */
unsigned int *nlaGetCoinMeterCount( void )
{
	return creditState.coinMeter;
}

/* Runs the credit service for the coin switches, and the coins it took in. */
int nlaCreditServiceRoutine( unsigned int coinsw, unsigned int *coinin )
{
	return nlaCreditProcess( &creditConf, &creditState, creditStatus, coinsw, coinin, &creditBackup );
}

/* Updates the running time in the backup RAM. */
void nlaUpdateTotalTime( void )
{
	int now;

	now = ( RTC_HIGH_L & 0xffff ) << 16 | ( RTC_LOW_L & 0xffff );
	shmBase->total = now - bootTime + shmBase->bootTotal;
}

void nlaUpdateCreditData( void )
{
	creditBackupSave();
	shmSaveToBack();
}

/* Waits for the Kaede driver's job. */
void nlaWaitPerProcess( void )
{
	set_imask( 0 );
	kdBlockKaedeJob( 0 );
	while ( !kdGetDriverStatus() )
		;
	set_imask( 14 );
	kdBlockKaedeJob( 1 );
}

int nlaCheckPerStatus( void )
{
	switch ( kdGetDriverStatus() ) {
	case 0:
		return 1;
	default:
		return 0;
	}
}

void nlaVblankFunctionEnd( void )
{
	SB_ISTNRM = IST_MAPLE_DMA;
}
