/* libsnd's declarations: what the sound driver calls share. */
#ifndef NLSND_H
#define NLSND_H

#include "types.h"

/* the sound CPU's mailbox, in the AICA's RAM as the main CPU sees it */
#define NLSND_RAM		0xA0800000
#define NLSND_G2_BASE		0x5F800000

/* how many ports hold a pack */
#define SND_PORTS	8

/* how many streams and handles of copies */
#define SND_STREAMS	8
#define SND_G2_HANDLES	16

/* the head of a sound pack: its tag, its id and its length */
typedef struct SNDPACKHD {
	char	magic[4];
	Sint32	id;
	Sint32	size;
} SNDPACKHD;

/* the head of a stream: its tag, its format and what is played on each of its channels */
typedef struct SNDSTREAMHD {
	char	magic[4];	/* "SPSD" */
	Uint8	unk_04[3];
	Uint8	unk_07;
	Uint8	format;		/* 08: 0 is 16 bits, 1 is 8 bits and 2 or 3 is 4 bits */
	char	chs;		/* 09: 0 is 1 channel, 1 is 2 and 2 is 4 */
	Uint8	unk_0a[2];
	Uint32	size;		/* 0C */
	Uint8	unk_10[6];
	Uint16	unk_16;
	Uint8	unk_18[6];
	Uint8	unk_1e;
	Uint8	unk_1f;
	Uint16	unk_20;
	Uint16	octfns;		/* 22 */
	Uint8	unk_24[6];
	Uint16	freq;		/* 2A */
	Uint8	unk_2c[4];
	struct {
		Uint8	gain;
		Uint8	vol;
		Uint8	pan;
		Uint8	fx;
	} ch[4];		/* 30 */
} SNDSTREAMHD;

/* a handle of a copy into the sound memory */
typedef struct SNDG2HANDLE {
	Sint32	state;		/* 1 when open */
	Sint32	unk_04[3];
} SNDG2HANDLE;

/* the sound CPU's program: its length and the places it names in the memory */
typedef struct SNDDRIVER {
	Uint32	unk_00[21];
	Uint32	size;		/* 054 */
	Uint32	unk_58[10];
	Uint32	unk_80;
	Uint32	base_ofs;	/* 084 */
	Uint32	unk_88[15];
	Uint32	unk_c4;
	Uint32	info_ofs;	/* 0C8: its table of its state */
} SNDDRIVER;

extern Sint32 nlsndSeqRequest( Uint32 request );
extern Sint32 nlsndBlockMove( Uint32 dst, Uint32 src, Uint32 size );
extern Sint32 nlsndBlockMoveStatus( void );
extern Sint32 nlsndCheckPackLoadError( const SNDPACKHD *pack, Sint32 port );
extern Sint32 nlsndPackLoadInit( Sint32 port, Sint32 id, Sint32 size, Sint32 *after );
extern Sint32 nlsndCheckPackFreeError( Sint32 port );
extern Sint32 nlsndPackFreeInit( Sint32 port, Sint32 size, Sint32 *after );
extern Sint32 nlsndInit( const SNDDRIVER *driver, Sint32 mode );
extern Sint32 nlsndPackLoad( const SNDPACKHD *pack, Sint32 port );
extern Sint32 nlsndPackLoad_Open( const SNDPACKHD *pack, Sint32 port, Sint32 chunk );
extern Sint32 nlsndPackLoadLock_Open( const SNDPACKHD *pack, Sint32 port, Sint32 chunk );
extern Sint32 nlsndPackLoad_Write( const void *data );
extern Sint32 nlsndPackLoadStatus( void );
extern Sint32 nlsndPackFree( Sint32 port );
extern Sint32 nlsndPackFreeBG( Sint32 port );
extern Sint32 nlsndSystemControl( Uint16 pack, Uint32 command, Sint32 param );
extern Sint32 nlsndControlChange( Uint32 pack, Uint32 midich, Uint32 command, Sint32 param );
extern Sint32 nlsndIcsControl( Uint32 pack, Uint32 command, Sint32 param );
extern Sint32 nlsndFxControl( Uint32 pack, Uint32 command, Sint32 param );
extern Sint32 nlsndGdControl( Uint32 command, Sint32 param );
extern Sint32 nlsndGetPackInfo( Uint32 port, Uint32 command );
extern Sint32 nlsndGetMidiChInfo( Uint32 port, Sint32 midich, Uint32 command );
extern Sint32 nlsndDriverSeqStatus( void );
extern Sint32 nlsndDriverSongStatus( Sint32 song );
extern Sint32 nlsndDriverMidiReqNum( void );
extern Sint32 nlsndDriverStatus( void );
extern Sint32 nlsndRemainMem( void );
extern const char *nlsndVersion( void );
extern Sint32 nlsndStreamInit( Sint16 mono, Sint16 stereo, Sint16 mode, Sint16 unk );
extern SNDSTREAMHD *nlsndStreamGetHdInfo( SNDSTREAMHD *hd, Sint32 *size );
extern void nlsndStreamSetHdInfo( Sint16 stream, const SNDSTREAMHD *hd );
extern Uint32 nlsndStreamGetBufInfo( Sint16 stream, Sint16 side, Sint32 *size );
extern Sint32 nlsndStreamGetFreq( Sint16 stream );
extern Sint32 nlsndStreamGetBitSize( Sint16 stream );
extern Uint32 nlsndStreamGetPlayPos( Sint16 stream );
extern Sint16 nlsndStreamOpen( Sint16 type );
extern void nlsndStreamClose( Sint16 stream );
extern void nlsndStreamStart( Sint16 stream );
extern void nlsndStreamStop( Sint16 stream );
extern void nlsndStreamPause( Sint16 stream );
extern void nlsndStreamContinue( Sint16 stream );
extern void nlsndStreamGenerateHdInfo( SNDSTREAMHD *hd );
extern void nlsndStreamSetHdOctFns( SNDSTREAMHD *hd, Sint16 oct, Uint16 fns );
extern void nlsndStreamSetHdChs( SNDSTREAMHD *hd, Sint16 chs );
extern void nlsndStreamSetHdVol( Uint8 *hd, Sint16 ch, Sint32 vol );
extern void nlsndStreamSetHdPan( Uint8 *hd, Sint16 ch, Sint32 pan );
extern void nlsndStreamSetHdBitSize( Uint8 *hd, Sint16 bits );
extern void nlsndStreamSetHdFx( SNDSTREAMHD *hd, Sint16 send, Sint16 level );
extern void nlsndStreamSetHdGain( SNDSTREAMHD *hd, Sint32 gain );
extern Sint32 nlsndStreamGetParam( const SNDSTREAMHD *hd, Sint32 command, Sint16 ch );
extern void nlsndStreamChgVol( Sint16 stream, Sint16 ch, Sint16 vol );
extern void nlsndStreamChgVolBias( Sint16 stream, Sint16 bias );
extern void nlsndStreamChgPan( Sint16 stream, Sint16 ch, Sint16 pan );
extern void nlsndStreamTranspose( Sint16 stream, Sint16 oct );
extern void nlsndStreamDetune( Sint16 stream, Sint16 fns );
extern void nlsndStreamChgFx( Sint16 stream, Sint16 send, Sint16 level );
extern void nlsndStreamChgFilter( Sint16 stream, Sint16 cutoff, Sint16 q );
extern void nlsndG2DmaInit( void );
extern void nlsndSetG2TransMode( Sint32 mode );
extern Sint32 nlsndGetG2TransMode( void );
extern SNDG2HANDLE *nlsndG2DmaHandleOpen( void );
extern Sint32 nlsndG2DmaHandleClose( SNDG2HANDLE *handle );
extern Sint32 nlsndG2DmaRequest( SNDG2HANDLE *handle, Uint32 *src, Uint32 *dst, Uint32 size );
extern Sint32 nlsndG2DmaGetStatus( SNDG2HANDLE *handle );

#endif
