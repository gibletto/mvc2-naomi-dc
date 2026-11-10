/* libsnd: the NAOMI library's sound driver calls, in its Sep 1999 version. Loads the sound CPU's
   program and the sound packs into the AICA's memory, and sends it requests through a mailbox in
   that memory. */
#include "nlsnd.h"

/* a port's pack in the sound memory: its id, where it starts and how long it is */
typedef struct SNDSLOT {
	Sint32	id;		/* -1 when free */
	Sint32	start;
	Sint32	size;
} SNDSLOT;

/* a stream the driver plays: its state, its format and where its buffers and registers are */
typedef struct SNDSTREAM {
	Sint8	state;		/* -1 when there is none, 1 when open */
	Sint8	playing;
	Sint16	bits;
	Uint32	buf[2];		/* the buffers as the main CPU sees them */
	Uint32	freq;
	volatile Uint32 *pos;
	volatile Uint32 *ch;
} SNDSTREAM;

/* what the main CPU shares with the driver of a stream */
typedef struct SNDSTREAMBOX {
	Uint32	unk_00;
	Uint32	ch[2];		/* the addresses of the registers of its channels */
	Uint32	buf[2];		/* where its buffers are in the sound memory */
	Uint32	pos;
	Uint32	unk_18[2];
	Uint32	hd[sizeof( SNDSTREAMHD ) / 4];	/* its head, word by word */
} SNDSTREAMBOX;

/* the driver's table of its state in the sound memory: where each part is (from NLSND_RAM), and
   the status of its last sequence */
typedef struct SNDINFO {
	Sint32	pack;		/* the packs' settings, 8 words each */
	Sint32	midich;		/* each port's MIDI channels, 16 words each */
	Sint32	ch;		/* the channels' settings, 16 words each */
	Sint32	song;		/* the songs, 12 words each */
	Sint32	seq_status;
} SNDINFO;

/* the word of a stream's head that holds its format and, above it, its channels (chs) */
#define SND_HD_FORMAT_WORD	( 8 / 4 )

static volatile Sint32 snd_busy;
static volatile Uint32 *snd_ring;
static volatile SNDINFO *snd_info;
static Sint32 snd_base;
static volatile Sint32 *snd_unk_b8;
static Sint32 snd_ram_size;
static Sint32 snd_used;
static Sint32 snd_stream_size;
static Sint32 snd_state;
static Sint32 snd_load_chunk;
static Sint32 snd_load_left;
static Sint32 snd_load_after;
static Uint32 *snd_load_src;
static volatile Uint32 *snd_load_dst;
static Sint32 snd_g2_mode;
static Uint32 snd_driver_size;
static SNDSTREAM snd_stream[SND_STREAMS];
static SNDSLOT snd_slot[SND_PORTS];
static SNDG2HANDLE snd_g2_handle[SND_G2_HANDLES];

const char nlsnd_version[] = "libsnd0.93-19990920";

/* the uncached view of memory */
#define P2_AREA		0xA0000000

/* the sound chip's registers: the sound CPU's reset, the memory mode and the interrupts */
#define SND_CTRL	( *( volatile Uint32 * )0xA0702C00 )
#define SND_MODE	( *( volatile Uint32 * )0xA0702800 )
#define SND_INT		( *( volatile Uint32 * )0xA070289C )

/* the mailbox words */
#define SND_RAM_MODE	( *( volatile Uint32 * )0xA0800050 )
#define SND_READY	( *( volatile Uint32 * )0xA080005C )
#define SND_PORT_ID( port )	( ( ( volatile Sint32 * )0xA0800060 )[port] )
#define SND_MEM_SIZE	( *( volatile Sint32 * )0xA0800088 )
#define SND_MOVE_DST	( *( volatile Uint32 * )0xA0800090 )
#define SND_MOVE_SRC	( *( volatile Uint32 * )0xA0800094 )
#define SND_MOVE_SIZE	( *( volatile Uint32 * )0xA0800098 )
#define SND_LOAD_GO	( *( volatile Uint32 * )0xA08000B0 )
#define SND_LOAD_ACK	( *( volatile Uint32 * )0xA08000B4 )
#define SND_STATUS	( *( volatile Sint32 * )0xA08000E8 )
#define SND_REQ_COUNT	( *( volatile Sint32 * )0xA08000EC )

/* the streams, the mailbox's block for each, and the handles of the copies */
#define SND_STREAM_BOX	( ( SNDSTREAMBOX * )0xA0800100 )
#define SND_STREAM_MONO	( *( volatile Uint32 * )0xA08000A0 )
#define SND_STREAM_STEREO	( *( volatile Uint32 * )0xA08000A4 )
#define SND_STREAM_UNIT	( *( volatile Sint32 * )0xA08000A8 )
#define SND_STREAM_MEM	( *( volatile Uint32 * )0xA080008C )
#define SND_STREAM_MAX_SIZE	0x4000

/* the requests of a stream: its number in the bits above the code */
#define SND_REQ_STREAM( s, code )	( ( ( Sint16 )( s ) << 12 ) + ( code ) )
#define SND_REQ_STREAM_START	0x1a1
#define SND_REQ_STREAM_STOP	0x2a1

#define SND_RING_BASE	0xA0800400
#define SND_RING_END	0xA0800500

/* how many polls of the mailbox before giving up (a stop polls its channels fewer times) */
#define SND_TIMEOUT	0x400000
#define SND_KEY_TIMEOUT	0x40000
#define SND_MOVE_TIMEOUT	0x1000000

#define SND_ERR_TIMEOUT	-256

/* the states of a load or free by slices: the step in the low byte, what is done in the next */
#define SND_STATE_LOAD		0x001
#define SND_STATE_FREE		0x101
#define SND_STATE_WRITE_OPEN	0x1001
#define SND_STATE_WRITE		0x1004
#define SND_MODE_FREE		0x100

/* the smallest driver the library takes */
#define SND_DRIVER_MIN	0x6e0

/* the requests: a bit that marks one, the status, and the one that makes the driver take a pack */
#define SND_REQ_VALID	0x80
#define SND_REQ_STATUS	0x2000a0
#define SND_REQ_PACK	0x18a0

/* the commands of the system, MIDI channel, ICS, FX and GD controls */
#define SND_TOTAL_VOLUME	0x01a0
#define SND_SONG_VOLUME		0x04a0
#define SND_FADE_IN		0x09a0
#define SND_FADE_OUT		0x0aa0
#define SND_SE_VOLUME		0x10a0
#define SND_SYS_05A0		0x05a0
#define SND_SYS_06A0		0x06a0
#define SND_SYS_07A0		0x07a0
#define SND_SYS_11A0		0x11a0
#define SND_SYS_1BA0		0x1ba0
#define SND_SYS_30A0		0x30a0
#define SND_SYS_28A0		0x28a0
#define SND_SYS_29A0		0x29a0
#define SND_SYS_02A0		0x02a0
#define SND_SYS_03A0		0x03a0
#define SND_MIDICH_01B0		0x01b0
#define SND_MIDICH_02B0		0x02b0
#define SND_MIDICH_07B0		0x07b0
#define SND_MIDICH_VOLUME	0x27b0
#define SND_MIDICH_0AB0		0x0ab0
#define SND_MIDICH_PAN		0x2ab0
#define SND_ICS_00A5		0x00a5
#define SND_ICS_10A5		0x10a5
#define SND_ICS_20A5		0x20a5
#define SND_ICS_30A5		0x30a5
#define SND_ICS_40A5		0x40a5
#define SND_ICS_00A7		0x00a7
#define SND_ICS_10A7		0x10a7
#define SND_ICS_50A5		0x50a5
#define SND_ICS_00A6		0x00a6
#define SND_ICS_70A5		0x70a5
#define SND_FX_00A4		0x00a4
#define SND_FX_10A4		0x10a4
#define SND_FX_20A4		0x20a4
#define SND_FX_70A4		0x70a4
#define SND_GD_00A3		0x00a3

/* The wait for a mailbox word: goes on at label when cond is true, and gives up at timeout. */
#define SND_WAIT( cond, label ) \
	for ( n = 0 ; n < SND_TIMEOUT ; n++ ) \
		if ( cond ) \
			goto label; \
	goto timeout; \
label:

/* The same wait, for a block move: it may take longer. */
#define SND_WAIT_MOVE( cond, label ) \
	for ( n = 0 ; n < SND_MOVE_TIMEOUT ; n++ ) \
		if ( cond ) \
			goto label; \
	goto timeout; \
label:

/* Moves a block of the AICA's memory by the driver: the main CPU's copy of the block move. */
Sint32 nlsndBlockMove( Uint32 dst, Uint32 src, Uint32 size )
{
	Sint32 i;
	Sint32 j;

	SND_MOVE_DST = dst + NLSND_G2_BASE;
	SND_MOVE_SRC = src + NLSND_G2_BASE;
	SND_MOVE_SIZE = size;
	for ( i = 0 ; i < SND_TIMEOUT ; i++ )
		if ( SND_MOVE_SIZE )
			goto ready;
	return SND_ERR_TIMEOUT;
ready:
	for ( j = 0 ; j < SND_TIMEOUT ; j++ )
		if ( nlsndSeqRequest( SND_REQ_STATUS ) == 0 )
			goto done;
	return SND_ERR_TIMEOUT;
done:
	return 0;
}

/* Whether the driver has taken the block move. */
Sint32 nlsndBlockMoveStatus( void )
{
	if ( SND_MOVE_SIZE )
		return 1;
	return 0;
}

/* Checks a pack and its port before it is loaded: the pack's tag, the port's number and use, the
   room in the sound memory and a load in progress. */
Sint32 nlsndCheckPackLoadError( const SNDPACKHD *pack, Sint32 port )
{
	const char *tag;
	const char *magic;
	Sint32 i;
	Sint32 size;
	Sint32 mem;

	if ( port < 0 || port >= SND_PORTS )
		return -1;
	magic = pack->magic;
	tag = "DTPK";
	for ( i = 0 ; i < 4 ; i++ )
		if ( *magic++ != *tag++ )
			return -2;
	if ( snd_slot[port].id != -1 )
		return -3;
	size = pack->size;
	mem = SND_MEM_SIZE;
	if ( size + snd_used > mem )
		return -4;
	if ( snd_state )
		return -5;
	return 0;
}

/* Puts a pack into its port's slot: after the packs before it, and moves the packs behind it up
   by its length. Gives back where it starts and, in *after, how long the packs behind it are. */
Sint32 nlsndPackLoadInit( Sint32 port, Sint32 id, Sint32 size, Sint32 *after )
{
	Sint32 i;
	Sint32 start;
	Sint32 behind;

	snd_used += size;
	snd_slot[port].id = id;
	snd_slot[port].size = size;
	SND_PORT_ID( port ) = id;
	start = behind = 0;
	for ( i = port - 1 ; i >= 0 ; i-- ) {
		if ( snd_slot[i].id != -1 ) {
			start = snd_slot[i].start + snd_slot[i].size;
			break;
		}
	}
	if ( start == 0 )
		start = snd_base;
	snd_slot[port].start = start;
	for ( i = port + 1 ; i < SND_PORTS ; i++ ) {
		if ( snd_slot[i].id != -1 ) {
			behind += snd_slot[i].size;
			snd_slot[i].start += size;
		}
	}
	*after = behind;
	return start;
}

/* Checks a port before its pack is freed: the port's number, that it holds a pack and a load in
   progress. */
Sint32 nlsndCheckPackFreeError( Sint32 port )
{
	if ( port < 0 || port >= SND_PORTS )
		return -1;
	if ( snd_slot[port].id == -1 )
		return -2;
	if ( snd_state )
		return -3;
	return 0;
}

/* Takes a pack out of its port's slot and moves the packs behind it down by its length. Gives
   back where the first of them started and, in *after, how long they are. */
Sint32 nlsndPackFreeInit( Sint32 port, Sint32 size, Sint32 *after )
{
	Sint32 i;
	Sint32 first;
	Sint32 behind;

	snd_used -= size;
	snd_slot[port].id = -1;
	SND_PORT_ID( port ) = -1;
	first = behind = 0;
	for ( i = port + 1 ; i < SND_PORTS ; i++ ) {
		if ( snd_slot[i].id != -1 ) {
			if ( first == 0 )
				first = snd_slot[i].start;
			behind += snd_slot[i].size;
			snd_slot[i].start -= size;
		}
	}
	*after = behind;
	return first;
}

/* Starts the sound CPU with its program and the memory mode (0 for 2M, 0x200 for 8M). */
Sint32 nlsndInit( const SNDDRIVER *driver, Sint32 mode )
{
	volatile Uint32 *dst;
	const Uint32 *src;
	Uint32 i;
	Uint32 words;
	Uint32 n;
	Uint32 val;
	register Sint32 p;
	volatile Sint32 *port_id;

	snd_driver_size = driver->unk_c4;
	if ( snd_driver_size < SND_DRIVER_MIN )
		return -1;
	if ( mode == 0 )
		snd_ram_size = 0x200000;
	else if ( mode == 0x200 )
		snd_ram_size = 0x800000;
	else
		return -2;
	val = SND_CTRL;
	val &= 0xfffe;
	val |= 1;
	SND_CTRL = val;
	SND_MODE = mode;
	val = SND_CTRL;
	val &= 0xfffe;
	val |= 1;
	SND_CTRL = val;
	dst = ( volatile Uint32 * )NLSND_RAM;
	src = ( const Uint32 * )driver;
	words = *( const volatile Uint32 * )( ( Uint32 )src + 84 ) >> 2;	/* size */
	for ( i = 0 ; i < words ; i++ )
		*dst++ = *src++;
	if ( 0 )
		*dst = i;
	SND_RAM_MODE = mode;
	port_id = &SND_PORT_ID( 0 );
	for ( p = SND_PORTS ; p > 0 ; p-- )
		*port_id++ = -1;
	val = SND_CTRL & 0xfffe;
	SND_CTRL = val;
	for ( i = 0 ; i < SND_PORTS ; i++ ) {
		snd_slot[i].id = -1;
		snd_slot[i].start = 0;
		snd_slot[i].size = 0;
	}
	for ( i = 0 ; i < SND_STREAMS ; i++ ) {
		snd_stream[i].state = -1;
		snd_stream[i].playing = 0;
		snd_stream[i].buf[0] = snd_stream[i].buf[1] = -1;
	}
	snd_base = NLSND_RAM + driver->base_ofs;
	snd_unk_b8 = ( volatile Sint32 * )( NLSND_RAM + 0x10 + driver->unk_80 );
	snd_info = ( volatile SNDINFO * )( NLSND_RAM + *( const Uint32 * )( ( Uint32 )driver + 0xc8 ) );	/* info_ofs */
	snd_ring = ( volatile Uint32 * )SND_RING_BASE;
	snd_used = 0;
	snd_state = 0;
	snd_busy = 0;
	for ( n = 0 ; n < SND_TIMEOUT ; n++ )
		if ( SND_READY )
			goto ready;
	return SND_ERR_TIMEOUT;
ready:
	SND_INT &= 0x7f7;
	return 0;
}

/* Puts the pack whole into the sound memory, and waits for the driver to take it. */
Sint32 nlsndPackLoad( const SNDPACKHD *pack, Sint32 port )
{
	Sint32 error;
	register Sint32 n;
	Sint32 after;
	Uint32 start;
	Uint32 size;
	register volatile Uint32 *dst;
	register const Uint32 *src;
	Uint32 words;
	Uint32 go;

	if ( ( error = nlsndCheckPackLoadError( pack, port ) ) != 0 )
		return error;
	size = pack->size;
	start = nlsndPackLoadInit( port, pack->id, size, &after );
	for ( n = 0 ; n < SND_TIMEOUT ; n++ )
		if ( nlsndSeqRequest( SND_REQ_PACK ) == 0 )
			goto requested;
	return SND_ERR_TIMEOUT;
requested:
	for ( n = 0 ; n < SND_TIMEOUT ; n++ )
		if ( SND_LOAD_ACK )
			goto ready;
	return SND_ERR_TIMEOUT;
ready:
	if ( after ) {
		if ( ( error = nlsndBlockMove( start, start + size, after ) ) != 0 )
			return error;
		for ( n = 0 ; n < SND_MOVE_TIMEOUT ; n++ )
			if ( nlsndBlockMoveStatus() == 0 )
				goto moved;
		return SND_ERR_TIMEOUT;
moved:
		;
	}
	src = ( const Uint32 * )pack;
	dst = ( volatile Uint32 * )start;
	for ( n = 0, words = size >> 2 ; n < words ; n++ )
		*dst++ = *src++;
	SND_LOAD_GO = go = 1;
	if ( n = 0, go )
		for ( ; n < SND_TIMEOUT ; n++ )
			if ( SND_LOAD_ACK == 0 )
				goto loaded;
	return SND_ERR_TIMEOUT;
loaded:
	SND_LOAD_GO = 0;
	return 0;
}

/* Starts a load by slices: Status moves the pack words at a time, SND_LOAD_CHUNK words each call. */
Sint32 nlsndPackLoad_Open( const SNDPACKHD *pack, Sint32 port, Sint32 chunk )
{
	Sint32 error;
	Sint32 size;
	Sint32 after;

	if ( ( error = nlsndCheckPackLoadError( pack, port ) ) != 0 )
		return error;
	size = pack->size;
	snd_load_dst = ( volatile Uint32 * )nlsndPackLoadInit( port, pack->id, size, &after );
	snd_load_src = ( Uint32 * )pack;
	snd_state = SND_STATE_LOAD;
	snd_load_chunk = chunk;
	snd_load_left = size;
	snd_load_after = after;
	return 0;
}

/* The same for a pack that the caller sends with nlsndPackLoad_Write. */
Sint32 nlsndPackLoadLock_Open( const SNDPACKHD *pack, Sint32 port, Sint32 chunk )
{
	Sint32 error;
	Sint32 size;
	Sint32 after;

	if ( ( error = nlsndCheckPackLoadError( pack, port ) ) != 0 )
		return error;
	size = pack->size;
	snd_load_dst = ( volatile Uint32 * )nlsndPackLoadInit( port, pack->id, size, &after );
	snd_state = SND_STATE_WRITE_OPEN;
	snd_load_chunk = chunk;
	snd_load_left = size;
	snd_load_after = after;
	return 0;
}

/* Takes a slice of the pack. Gives back 1 while more is due, 0 when the pack is whole. */
Sint32 nlsndPackLoad_Write( const void *data )
{
	const Uint32 *src;
	Sint32 i;

	if ( snd_state != SND_STATE_WRITE )
		return -1;
	src = data;
	for ( i = 0 ; i < snd_load_chunk ; i++ ) {
		*snd_load_dst++ = *src++;
		if ( ( snd_load_left -= 4 ) == 0 ) {
			SND_LOAD_GO = 1;
			snd_state++;
			return 0;
		}
	}
	return 1;
}

/* Goes on with the load or free that was opened. Gives back -1 when there is none, 1 while it
   is going on and 0 when it is done. */
Sint32 nlsndPackLoadStatus( void )
{
	Sint32 n;
	Sint32 i;
	Uint32 after;

	switch ( snd_state & 0xff ) {
	case 0:
		return -1;
		break;
	case 1:
		for ( n = 0 ; n < SND_TIMEOUT ; n++ )
			if ( nlsndSeqRequest( SND_REQ_PACK ) == 0 )
				goto requested;
		return 1;
	requested:
		snd_state++;
		return 1;
		break;
	case 2:
		if ( SND_LOAD_ACK == 0 ) {
			return 1;
			break;
		}
		after = snd_load_after;
		if ( after )
			nlsndBlockMove( ( Uint32 )snd_load_dst, ( Uint32 )snd_load_dst + snd_load_left, after );
		snd_state++;
		return 1;
		break;
	case 3:
		if ( nlsndBlockMoveStatus() ) {
			return 1;
			break;
		}
		snd_state++;
	case 4:
		if ( ( snd_state & 0xff00 ) == 0 ) {
			for ( i = 0 ; i < snd_load_chunk ; i++ ) {
				*snd_load_dst++ = *snd_load_src++;
				if ( ( snd_load_left -= 4 ) == 0 ) {
					SND_LOAD_GO = 1;
					snd_state++;
					return 1;
				}
			}
			return 1;
		} else if ( ( snd_state & 0xff00 ) == SND_MODE_FREE ) {
			SND_LOAD_GO = 1;
			snd_state += 1;
			return 1;
		}
		return 0;
		break;
	case 5:
		if ( SND_LOAD_ACK )
			return 1;
		SND_LOAD_GO = 0;
		snd_state = 0;
		return 0;
		break;
	}
}

/* Takes the pack of a port out of the sound memory and waits for the driver. */
Sint32 nlsndPackFree( Sint32 port )
{
	Sint32 error;
	Sint32 n;
	Sint32 after;
	Sint32 size;
	Sint32 first;
	Uint32 go;

	if ( ( error = nlsndCheckPackFreeError( port ) ) != 0 )
		return error;
	size = snd_slot[port].size;
	first = nlsndPackFreeInit( port, size, &after );
	SND_WAIT( nlsndSeqRequest( SND_REQ_PACK ) == 0, requested );
	SND_WAIT( SND_LOAD_ACK, ready );
	if ( first ) {
		if ( ( error = nlsndBlockMove( first, first - size, after ) ) != 0 )
			return error;
		SND_WAIT_MOVE( nlsndBlockMoveStatus() == 0, moved );
	}
	SND_LOAD_GO = go = 1;
	if ( n = 0, go )
		for ( ; n < SND_TIMEOUT ; n++ )
			if ( SND_LOAD_ACK == 0 )
				goto freed;
timeout:
	return SND_ERR_TIMEOUT;
freed:
	return SND_LOAD_GO = 0;
}

/* Starts a free that nlsndPackLoadStatus goes on with. */
Sint32 nlsndPackFreeBG( Sint32 port )
{
	Sint32 error;
	Sint32 size;
	Sint32 after;

	if ( ( error = nlsndCheckPackFreeError( port ) ) != 0 )
		return error;
	size = snd_slot[port].size;
	snd_load_dst = ( volatile Uint32 * )nlsndPackFreeInit( port, size, &after );
	snd_state = SND_STATE_FREE;
	snd_load_left = -size;
	snd_load_after = after;
	return 0;
}

/* Sends one of the system's commands for a pack. The parameter is added to the request in the
   place the command wants it. */
Sint32 nlsndSystemControl( Uint16 pack, Uint32 command, Sint32 param )
{
	Uint32 request;

	request = ( ( pack & 15 ) << 24 ) + command;
	if ( command & 0xff00 ) {
		switch ( command ) {
		case SND_TOTAL_VOLUME:
		case SND_SONG_VOLUME:
		case SND_FADE_IN:
		case SND_FADE_OUT:
		case SND_SE_VOLUME:
			request += ( param & 127 ) << 16;
			break;
		case SND_SYS_05A0:
		case SND_SYS_06A0:
		case SND_SYS_07A0:
		case SND_SYS_11A0:
			request += ( ( param + 64 ) & 127 ) << 16;
			break;
		case SND_SYS_1BA0:
			request += ( param & 15 ) << 16;
			break;
		case SND_SYS_30A0:
		case SND_SYS_28A0:
		case SND_SYS_29A0:
			request += ( ( param - 1 ) & 15 ) << 16;
			break;
		case SND_SYS_02A0:
		case SND_SYS_03A0:
			break;
		default:
			return -2;
		}
	}
	return nlsndSeqRequest( request );
}

/* The same for a MIDI channel of the pack. */
Sint32 nlsndControlChange( Uint32 pack, Uint32 midich, Uint32 command, Sint32 param )
{
	Uint32 request;

	request = ( ( pack & 15 ) << 24 ) + ( ( midich - 1 ) & 15 ) + command;
	switch ( command ) {
	case SND_MIDICH_01B0:
	case SND_MIDICH_02B0:
	case SND_MIDICH_07B0:
	case SND_MIDICH_VOLUME:
		request += ( param & 127 ) << 16;
		break;
	case SND_MIDICH_0AB0:
	case SND_MIDICH_PAN:
		request += ( ( param + 64 ) & 127 ) << 16;
		break;
	default:
		return -2;
	}
	return nlsndSeqRequest( request );
}

/* The same for the pack's ICS. */
Sint32 nlsndIcsControl( Uint32 pack, Uint32 command, Sint32 param )
{
	Uint32 base;
	Uint32 level;
	Uint32 request;
	Uint32 limit;
	Sint32 error;

	base = pack & 15;
	base <<= 8;
	request = base;
	request += command;
	level = ( param & 127 ) << 16;
	switch ( command ) {
	case SND_ICS_00A5:
		limit = ( ( param & 0x7f00 ) << 8 ) + base + SND_ICS_70A5;
		if ( ( error = nlsndSeqRequest( limit ) ) != 0 )
			return error;
		request = base + level + SND_ICS_00A5;
		break;
	case SND_ICS_10A5:
		request += level;
		break;
	case SND_ICS_20A5:
	case SND_ICS_30A5:
	case SND_ICS_40A5:
	case SND_ICS_00A7:
	case SND_ICS_10A7:
		request += ( ( param + 64 ) & 127 ) << 16;
		break;
	case SND_ICS_50A5:
		request += ( param & 15 ) << 16;
		break;
	case SND_ICS_00A6:
		level += ( param & 0x80 ) << 5;
		request += level;
		break;
	default:
		return -2;
	}
	return nlsndSeqRequest( request );
}

/* The same for the pack's FX. */
Sint32 nlsndFxControl( Uint32 pack, Uint32 command, Sint32 param )
{
	Uint32 request;
	Uint32 limit;
	Sint32 error;

	request = ( ( pack & 15 ) << 8 ) + command;
	switch ( command ) {
	case SND_FX_00A4:
		limit = ( ( param & 0x7f00 ) << 8 ) + SND_FX_70A4;
		if ( ( error = nlsndSeqRequest( limit ) ) != 0 )
			return error;
		request = ( ( param & 127 ) << 16 ) + SND_FX_00A4;
		break;
	case SND_FX_10A4:
		request += ( param & 15 ) << 16;
		break;
	case SND_FX_20A4:
		request += ( ( param + 64 ) & 127 ) << 16;
		break;
	default:
		return -2;
	}
	return nlsndSeqRequest( request );
}

/* The same for the GD-ROM's volume: sends the request and then its second half. */
Sint32 nlsndGdControl( Uint32 command, Sint32 param )
{
	Uint32 request;
	Sint32 error;

	request = command;
	switch ( command ) {
	case SND_GD_00A3:
		request += ( param & 15 ) << 16;
		if ( ( error = nlsndSeqRequest( request ) ) != 0 )
			return error;
		request += 0x100;
		return nlsndSeqRequest( request );
	default:
		return -2;
	}
}

/* Puts a request in the driver's ring of requests. Gives back -1 when the ring is full, -2 when
   it isn't a request and -3 while another call is in the ring. */
Sint32 nlsndSeqRequest( Uint32 request )
{
	Sint32 result;

	result = 0;
	if ( snd_busy )
		return -3;
	snd_busy = -1;
	if ( !( request & SND_REQ_VALID ) ) {
		result = -2;
		goto done;
	}
	if ( *snd_ring ) {
		result = -1;
		goto done;
	}
	*snd_ring++ = request;
	if ( snd_ring == ( volatile Uint32 * )SND_RING_END )
		snd_ring = ( volatile Uint32 * )SND_RING_BASE;
done:
	snd_busy = 0;
	return result;
}

/* A pack's setting: its song volume (SND_SONG_VOLUME) or its SE volume (SND_SE_VOLUME). */
Sint32 nlsndGetPackInfo( Uint32 port, Uint32 command )
{
	volatile Sint32 *info;

	if ( port >= SND_PORTS )
		return -128;
	info = ( volatile Sint32 * )( NLSND_RAM + snd_info->pack );
	info += port * 8 + 5;
	switch ( command ) {
	case SND_SONG_VOLUME:
		return *info & 127;
	case SND_SE_VOLUME:
		return ( *info & 0x7f00 ) >> 8;
	default:
		return -128;
	}
}

/* A head as a stream starts out: 8 bits, one channel, 22050 Hz and full volume. */
static const Uint8 snd_hd_default[64] = {
	'S', 'P', 'S', 'D', 0x00, 0x01, 0x00, 0x04, 0x01, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00,
	0xf7, 0x1f, 0xf7, 0x1f, 0xf7, 0x1f, 0xf7, 0x1f, 0xf7, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x04, 0x01,
	0x00, 0x00, 0x00, 0x78, 0xdf, 0x07, 0x1f, 0x3c, 0x00, 0x00, 0x22, 0x56, 0x00, 0x00, 0x00, 0x00,
	0x0f, 0x7f, 0x00, 0x00, 0x0f, 0x7f, 0x00, 0x00, 0x0f, 0x7f, 0x00, 0x00, 0x0f, 0x7f, 0x00, 0x00
};

/* A MIDI channel's setting: the volume or pan, as set by nlsndControlChange. */
Sint32 nlsndGetMidiChInfo( Uint32 port, Sint32 midich, Uint32 command )
{
	static const Sint8 pan_tbl[32] = {
		0, 4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 52, 56, 60,
		0, -4, -8, -12, -16, -20, -24, -28, -32, -36, -40, -44, -48, -52, -56, -60
	};
	volatile Sint32 *ch;
	volatile Sint32 *info;
	Sint8 pan;
	Sint32 word;

	if ( port >= SND_PORTS )
		return -128;
	midich--;
	if ( midich < 0 || midich >= 16 )
		return -128;
	ch = ( volatile Sint32 * )( NLSND_RAM + snd_info->midich );
	ch += port * 16 + midich;
	word = *ch & 0xff;
	if ( word == 0xff )
		return -129;
	info = ( volatile Sint32 * )( NLSND_RAM + snd_info->ch );
	info += ( word & 63 ) * 16;
	switch ( command ) {
	case SND_MIDICH_01B0:
		return info[5] & 127;
	case SND_MIDICH_02B0:
		return ( info[5] & 0x7f00 ) >> 8;
	case SND_MIDICH_VOLUME:
		return ( Sint16 )( ( info[2] & 0x7f0000 ) >> 16 );
	case SND_MIDICH_PAN:
		pan = ( info[2] & 0x1f000000 ) >> 24;
		return pan_tbl[pan];
	default:
		return -128;
	}
}

/* Takes the driver's last sequence status, and clears it. */
Sint32 nlsndDriverSeqStatus( void )
{
	volatile Sint32 *status;
	Sint32 value;

	status = &snd_info->seq_status;
	value = *status;
	if ( value )
		*status = 0;
	return value;
}

/* The status of a song: 0x100 playing, 0x101 paused, 0 when it isn't there. */
Sint32 nlsndDriverSongStatus( Sint32 song )
{
	volatile Sint32 *entry;
	Sint32 i;
	Sint32 status;
	Sint32 result;

	entry = ( volatile Sint32 * )( NLSND_RAM + snd_info->song );
	for ( i = 0 ; i < 16 ; i++ ) {
		status = *entry & 0xff;
		if ( status & 0x80 ) {
			if ( ( Sint16 )( ( *entry & 0xff0000 ) >> 16 ) == song ) {
				result = 0x100;
				if ( status & 1 )
					result = 0x101;
				return result;
			}
		}
		entry += 12;
	}
	return 0;
}

/* How many requests the driver has got. */
Sint32 nlsndDriverMidiReqNum( void )
{
	return SND_REQ_COUNT;
}

/* The driver's status word. */
Sint32 nlsndDriverStatus( void )
{
	return SND_STATUS;
}

/* How much of the sound memory is left. */
Sint32 nlsndRemainMem( void )
{
	return SND_MEM_SIZE - snd_used;
}

/* The library's version, as a text. */
const char *nlsndVersion( void )
{
	return nlsnd_version;
}

/* Sets up the streams: how many of one buffer and of two, and how the sound memory left over is
   shared out between their buffers. The stereo loop steps pointers to the box's fields. */
Sint32 nlsndStreamInit( Sint16 mono, Sint16 stereo, Sint16 mode, Sint16 count )
{
	volatile SNDSTREAMBOX *box;
	volatile Uint32 *ch;
	volatile Uint32 *pos;
	volatile Uint32 *buf;
	Sint32 unit;
	Sint32 start;
	Uint32 size;
	Uint32 mem;
	Sint16 i;
	Sint16 j;

	if ( mode )
		return -2;
	count = mono + stereo * 2;
	if ( count > SND_STREAMS )
		return -1;
	for ( j = 0 ; j < SND_STREAMS ; j++ )
		if ( snd_stream[j].state == 1 )
			return -3;
	SND_STREAM_MONO = mono;
	SND_STREAM_STEREO = stereo;
	size = SND_STREAM_MEM;
	if ( count ) {
		if ( ( snd_stream_size = ( size - count * 64 ) / count & 0x7fffffe0 ) > SND_STREAM_MAX_SIZE )
			snd_stream_size = SND_STREAM_MAX_SIZE;
		unit = snd_stream_size + 64;
		start = *snd_unk_b8;
		if ( start & 31 )
			start = ( start & -32 ) + 32;
		mem = snd_ram_size - unit - start;
		box = SND_STREAM_BOX;
	} else {
		unit = 0;
		snd_stream_size = 0;
	}
	SND_STREAM_UNIT = unit;
	for ( i = 0 ; i < mono ; i++ ) {
		snd_stream[i].state = snd_stream[i].playing = 0;
		snd_stream[i].buf[0] = NLSND_RAM + mem;
		box->buf[0] = mem;
		mem -= unit;
		snd_stream[i].buf[1] = 0;
		box->buf[1] = 0;
		snd_stream[i].pos = &box->pos;
		snd_stream[i].ch = &box->ch[0];
		box++;
	}
	ch = &box->ch[0];
	pos = &box->pos;
	buf = ( volatile Uint32 * )box;
	buf = buf + 3;	/* its buf[0] */
	for ( ; i < mono + stereo ; i++ ) {
		snd_stream[i].state = snd_stream[i].playing = 0;
		snd_stream[i].buf[1] = NLSND_RAM + mem;
		box->buf[1] = mem;
		mem -= unit;
		snd_stream[i].buf[0] = NLSND_RAM + mem;
		*buf = mem;
		mem -= unit;
		snd_stream[i].pos = pos;
		snd_stream[i].ch = ch;
		box++;
		ch += sizeof( SNDSTREAMBOX ) / 4;
		pos += sizeof( SNDSTREAMBOX ) / 4;
		buf += sizeof( SNDSTREAMBOX ) / 4;
	}
	for ( ; i < SND_STREAMS ; i++ ) {
		snd_stream[i].state = -1;
		snd_stream[i].playing = 0;
		snd_stream[i].buf[0] = snd_stream[i].buf[1] = -1;
	}
	nlsndSeqRequest( 0xa1 );
	return 0;
}

/* Gives back the head of a stream and, in *size, how big its buffers are. */
SNDSTREAMHD *nlsndStreamGetHdInfo( SNDSTREAMHD *hd, Sint32 *size )
{
	*size = hd->unk_07 << 4;
	return hd;
}

/* Gives the stream its head: the format of its data and what to play on each channel. */
void nlsndStreamSetHdInfo( Sint16 stream, const SNDSTREAMHD *hd )
{
	volatile Uint32 *dst;
	const Uint32 *src;
	Sint16 i;
	Sint16 fmt;

	if ( snd_stream[stream].state != 1 )
		return;
	if ( snd_stream[stream].playing != 0 )
		return;
	src = ( const Uint32 * )hd;
	dst = SND_STREAM_BOX[stream].hd;
	for ( i = 0 ; i < sizeof( SNDSTREAMHD ) / 4 ; i++ )
		*dst++ = *src++;
	snd_stream[stream].freq = hd->freq;
	fmt = hd->format;
	switch ( fmt ) {
	case 0:
		snd_stream[stream].bits = 16;
		break ;
	case 1:
		snd_stream[stream].bits = 8;
		break ;
	case 2:
	case 3:
		snd_stream[stream].bits = 4;
		break ;
	default:
		return;
	}
}

/* Gives back the start of the stream's buffer on one side and, in *size, how big it is. */
Uint32 nlsndStreamGetBufInfo( Sint16 stream, Sint16 side, Sint32 *size )
{
	if ( snd_stream[stream].state != 1 )
		return 0;
	*size = snd_stream_size;
	return snd_stream[stream].buf[side];
}

/* The frequency the stream plays at. */
Sint32 nlsndStreamGetFreq( Sint16 stream )
{
	if ( snd_stream[stream].state != 1 )
		return 0;
	return snd_stream[stream].freq;
}

/* How many bits each sample of the stream has. */
Sint32 nlsndStreamGetBitSize( Sint16 stream )
{
	if ( snd_stream[stream].state != 1 )
		return 0;
	return snd_stream[stream].bits;
}

/* Where the driver is in the stream's buffer. */
Uint32 nlsndStreamGetPlayPos( Sint16 stream )
{
	volatile Uint32 *pos;

	if ( snd_stream[stream].state != 1 )
		return 0;
	if ( snd_stream[stream].playing == 0 )
		return 0;
	pos = snd_stream[stream].pos;
	return *pos;
}

/* Takes a stream that is free: type 1 is one with a buffer, type 2 one with two. Gives back its
   number, or -1 when there is none. */
Sint16 nlsndStreamOpen( Sint16 type )
{
	Sint16 i;
	Sint16 found;

	if ( 0 ) {
		snd_stream[i].state = 1;
		return i;
	}
	found = 0;
	if ( type == 1 ) {
		for ( i = 0 ; i < SND_STREAMS ; i++ ) {
			if ( snd_stream[i].state == 0 && snd_stream[i].buf[1] == 0 ) {
				found = 1;
				break;
			}
		}
	} else if ( type == 2 ) {
		for ( i = 0 ; i < SND_STREAMS ; i++ ) {
			if ( snd_stream[i].state == 0 && snd_stream[i].buf[1] != 0 ) {
				found = 1;
				break;
			}
		}
	} else
		return -1;
	if ( found == 0 )
		return -1;
	snd_stream[i].state = 1;
	return i;
}

/* Gives the stream back. */
void nlsndStreamClose( Sint16 stream )
{
	if ( snd_stream[stream].state != 1 )
		return;
	if ( snd_stream[stream].playing != 0 )
		return;
	else
		snd_stream[stream].state = 0;
}

/* Starts the stream playing. */
void nlsndStreamStart( Sint16 stream )
{
	Uint32 request;

	if ( snd_stream[stream].state == 1 ) {
		if ( !snd_stream[stream].playing ) {
			request = SND_REQ_STREAM( stream, SND_REQ_STREAM_START );
			nlsndSeqRequest( request );
			snd_stream[stream].playing = 1;
		}
	}
}

/* Stops the stream: waits for the driver to give its channels, and has them keyed off. */
void nlsndStreamStop( Sint16 stream )
{
	volatile Uint32 *ch;
	volatile Uint32 *format;
	Uint32 request;
	register Uint32 reg;
	register Uint32 reg2;
	register Uint32 val;
	register Uint32 val2;
	Uint32 n;

	if ( snd_stream[stream].state != 1 )
		return;
	if ( snd_stream[stream].playing ) {
		format = ( volatile Uint32 * )&SND_STREAM_BOX->hd[SND_HD_FORMAT_WORD] + stream * ( sizeof( SNDSTREAMBOX ) / 4 );
		request = SND_REQ_STREAM( stream, SND_REQ_STREAM_STOP );
		ch = snd_stream[stream].ch;
		for ( n = 0 ; n < SND_KEY_TIMEOUT ; n++ )
			if ( *ch )
				goto first;
		goto stop;
first:
		reg = *ch;
		reg = reg | P2_AREA;
		val = *( volatile Uint32 * )reg;
		val = val & 0x7ff;
		*( volatile Uint32 * )reg = val;
		if ( ( *format & 0x300 ) == 0x100 ) {	/* two channels */
			ch = snd_stream[stream].ch + 1;
			for ( n = 0 ; n < SND_KEY_TIMEOUT ; n++ )
				if ( *ch )
					goto second;
			goto stop;
second:
			reg2 = *ch;
			reg2 = reg2 | P2_AREA;
			val2 = *( volatile Uint32 * )reg2;
			val2 = val2 & 0x7ff;
			*( volatile Uint32 * )reg2 = val2;
		}
		val = val | 0x8000;
		*( volatile Uint32 * )reg = val;
stop:
		nlsndSeqRequest( request );
		snd_stream[stream].playing = 0;
	}
}

void nlsndStreamPause( Sint16 stream )
{
}

void nlsndStreamContinue( Sint16 stream )
{
}

void nlsndStreamGenerateHdInfo( SNDSTREAMHD *hd )
{
	const Uint8 *src;
	Uint8 *dst;
	Sint32 i;

	src = snd_hd_default;
	dst = ( Uint8 * )hd;
	for ( i = 64 ; i > 0 ; i-- )
		*dst++ = *src++;
}

/* Sets the octave and the frequency number the stream is played at. */
void nlsndStreamSetHdOctFns( SNDSTREAMHD *hd, Sint16 oct, Uint16 fns )
{
	Uint16 *p = &hd->octfns;

	*p = ( oct << 11 ) | fns;
}

/* Sets how many channels the stream has: 1, 2 or 4. */
void nlsndStreamSetHdChs( SNDSTREAMHD *hd, Sint16 chs )
{
	char *p;

	p = &hd->chs;
	switch ( chs ) {
	case 1:
		*p &= 0x80;
		break;
	case 2:
		*p = ( *p & 0x80 ) | 1;
		break;
	case 4:
		*p = ( *p & 0x80 ) | 2;
		break;
	default:
		return;
	}
}

/* the head as bytes: a channel is 4 of them from 0x30 (gain, volume, pan, fx) */
void nlsndStreamSetHdVol( Uint8 *hd, Sint16 ch, Sint32 vol )
{
	*( hd + ch * 4 + 49 ) = vol;
}

void nlsndStreamSetHdPan( Uint8 *hd, Sint16 ch, Sint32 pan )
{
	*( hd + ch * 4 + 50 ) = pan;
}

/* Sets how many bits each sample has: 4, 8 or 16, in the head's byte 8 (the format). */
void nlsndStreamSetHdBitSize( Uint8 *hd, Sint16 bits )
{
	Uint8 *p;

	p = hd;
	switch ( bits ) {
	case 4:
		p[8] = 3;
		break;
	case 8:
		p[8] = 1;
		break;
	case 16:
		p[8] = 0;
		break;
	}
	return;
}

void nlsndStreamSetHdFx( SNDSTREAMHD *hd, Sint16 send, Sint16 level )
{
	Uint8 *p = &hd->ch[0].fx;
	Sint32 v = ( send << 4 ) | level;
	Sint32 i;

	for ( i = 0 ; i < 4 ; i++ ) {
		*p = v;
		p += 4;
	}
}

void nlsndStreamSetHdGain( SNDSTREAMHD *hd, Sint32 gain )
{
	Uint8 *p = &hd->ch[0].gain;
	Sint32 i;

	for ( i = 0 ; i < 4 ; i++ ) {
		*p = gain;
		p += 4;
	}
}

/* Reads one of a stream head's values: 0 the frequency, 1 the bits of a sample, 2 the samples in
   a channel, 3 the size, 4 the channels, 5 and 6 the volume and pan of a channel, 7 and 8 the fx
   send and level, 9 and 10 the loop. The head is read as bytes and words. */
Sint32 nlsndStreamGetParam( const SNDSTREAMHD *hd, Sint32 command, Sint16 ch )
{
	Sint32 size;
	Sint32 chs;
	Sint32 bits;

	if ( 0 )
		if ( ch < 0 )
			return -1;
	size = hd->size;
	switch ( hd->chs & 3 ) {
	case 0:
		chs = 1;
		break;
	case 1:
		chs = 2;
		break;
	case 2:
		chs = 4;
		break;
	}
	switch ( ( Sint8 )hd->format ) {
	case 0:
		bits = 16;
		break;
	case 1:
		bits = 8;
		break;
	case 2:
	case 3:
		bits = 4;
		break;
	}
	switch ( command ) {
	case 0:
		return ( ( const Uint16 * )hd )[21];
	case 1:
		return bits;
	case 2:
		return size * 8 / bits / chs;
	case 3:
		return size;
	case 4:
		return chs;
	case 5:
		return *( ( const Uint8 * )hd + ch * 4 + 49 );
	case 6:
		return *( ( const Uint8 * )hd + ch * 4 + 50 );
	case 7:
		return ( ( const Uint8 * )hd )[51] & 15;
	case 8:
		return ( ( ( const Uint8 * )hd )[51] & 0xf0 ) >> 4;
	case 9:
		if ( ( ( const Uint8 * )hd )[31] )
			return 0;
		if ( ( size = hd->unk_16 >> 6 ) == 0 )
			return 1;
		return size;
	case 10:
		return ( ( const Uint8 * )hd )[30];
	default:
		return -1;
	}
}

/* Changes the volume of a channel of the playing stream. */
void nlsndStreamChgVol( Sint16 stream, Sint16 ch, Sint16 vol )
{
	Uint32 req;

	if ( snd_stream[stream].state == 1 ) {
		req = ( ( stream << 12 ) | ( vol << 16 ) | 0xa2 ) + ( ch << 11 );
		nlsndSeqRequest( req );
	}
}

void nlsndStreamChgVolBias( Sint16 stream, Sint16 bias )
{
	Uint32 req;

	if ( snd_stream[stream].state == 1 ) {
		req = ( stream << 12 ) | ( ( bias + 64 ) << 16 ) | 0xca1;
		nlsndSeqRequest( req );
	}
}

void nlsndStreamChgPan( Sint16 stream, Sint16 ch, Sint16 pan )
{
	Uint32 req;

	if ( snd_stream[stream].state == 1 ) {
		req = ( ( stream << 12 ) | ( pan << 16 ) | 0x1a2 ) + ( ch << 11 );
		nlsndSeqRequest( req );
	}
}

void nlsndStreamTranspose( Sint16 stream, Sint16 oct )
{
	Uint32 req;

	if ( snd_stream[stream].state == 1 ) {
		req = ( stream << 12 ) | ( ( oct + 64 ) << 16 ) | 0x8a1;
		nlsndSeqRequest( req );
	}
}

void nlsndStreamDetune( Sint16 stream, Sint16 fns )
{
	Uint32 req;

	if ( snd_stream[stream].state == 1 ) {
		req = ( stream << 12 ) | ( ( fns + 64 ) << 16 ) | 0x9a1;
		nlsndSeqRequest( req );
	}
}

/* Changes the fx the stream is sent to: the send and its level. A stream of two buffers does it
   for the second as well. */
void nlsndStreamChgFx( Sint16 stream, Sint16 send, Sint16 level )
{
	Uint32 level_req;
	Uint32 send_req;

	if ( snd_stream[stream].state == 1 ) {
		level_req = ( stream << 12 ) | ( level << 16 ) | 0x2a2;
		nlsndSeqRequest( level_req );
		send_req = ( send << 16 ) | ( stream << 12 ) | 0x3a2;
		nlsndSeqRequest( send_req );
		if ( snd_stream[stream].buf[1] != 0 ) {
			level_req += 0x800;
			nlsndSeqRequest( level_req );
			send_req += 0x800;
			nlsndSeqRequest( send_req );
		}
	}
}

void nlsndStreamChgFilter( Sint16 stream, Sint16 cutoff, Sint16 q )
{
	Uint32 req;

	if ( snd_stream[stream].state == 1 ) {
		req = ( stream << 12 ) | ( cutoff << 16 ) | 0xaa1;
		nlsndSeqRequest( req );
		req = ( q << 16 ) | ( stream << 12 ) | 0xba1;
		nlsndSeqRequest( req );
	}
}

/* Clears the handles of the copies and has them done by the CPU. */
void nlsndG2DmaInit( void )
{
	Sint32 i;
	SNDG2HANDLE *h = snd_g2_handle;

	for ( i = SND_G2_HANDLES ; i > 0 ; i-- ) {
		h->state = 0;
		h++;
	}
	snd_g2_mode = 0;
}

void nlsndSetG2TransMode( Sint32 mode )
{
	if ( mode == 0 )
		snd_g2_mode = mode;
}

Sint32 nlsndGetG2TransMode( void )
{
	return snd_g2_mode;
}

/* Takes a handle that is free, or gives back 0 when there is none. */
SNDG2HANDLE *nlsndG2DmaHandleOpen( void )
{
	register Sint32 i;
	SNDG2HANDLE *h = snd_g2_handle;

	for ( i = 0 ; i < SND_G2_HANDLES ; i++, h++ ) {
		if ( h->state == 0 ) {
			h->state = 1;
			return h;
		}
	}
	return 0;
}

Sint32 nlsndG2DmaHandleClose( SNDG2HANDLE *handle )
{
	if ( handle->state == 1 ) {
		handle->state = 0;
		return 0;
	}
	return -1;
}

/* Copies a block into the sound memory. All of it has to be on a 32 byte line. */
Sint32 nlsndG2DmaRequest( SNDG2HANDLE *handle, Uint32 *src, Uint32 *dst, Uint32 size )
{
	Uint32 i;
	register volatile Uint32 *to;

	if ( handle->state != 1 )
		return -1;
	if ( ( Uint32 )src & 31 )
		return -2;
	if ( ( Uint32 )dst & 31 )
		return -3;
	if ( size & 31 )
		return -4;
	if ( snd_g2_mode == 0 ) {
		to = dst;
		size >>= 2;
		for ( i = 0 ; i < size ; i++ )
			*to++ = *src++;
	} else if ( snd_g2_mode != 1 )
		return -5;
	return 0;
}

/* Whether the copy is done: 0x100 when the CPU did it. */
Sint32 nlsndG2DmaGetStatus( SNDG2HANDLE *handle )
{
	if ( handle->state != 1 )
		return -1;
	if ( snd_g2_mode == 0 )
		return 0x100;
	if ( snd_g2_mode != 1 )
		return -5;
	return 0;
}
