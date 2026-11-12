/* CRI's ADX player (ADXT) calls that the Dreamcast game uses to stream its music from the disc,
   as the library's cri_adxt.h declares them. */
#ifndef ADX_H
#define ADX_H

#include "types.h"

typedef struct _adx_talk *ADXT ;	/* a player */

/* the work a player of two channels needs to stream one file at 44100 Hz */
#define ADXT_WORK_SIZE	0x208c4

/* what a player is doing (ADXT_GetStat) */
#define ADXT_STAT_STOP	0
#define ADXT_STAT_DECINFO	1	/* reading the file's header */
#define ADXT_STAT_PREP	2	/* getting ready to play */
#define ADXT_STAT_PLAYING	3
#define ADXT_STAT_DECEND	4	/* all decoded */
#define ADXT_STAT_PLAYEND	5

/* what a player does about a read error (ADXT_SetAutoRcvr) */
#define ADXT_RMODE_NOACT	0
#define ADXT_RMODE_STOP	1
#define ADXT_RMODE_REPLAY	2

extern void ADXT_Init( void ) ;
extern void ADXT_Finish( void ) ;
extern ADXT ADXT_Create( Sint32 maxnch, void *work, Sint32 worksize ) ;
extern void ADXT_SetAutoRcvr( ADXT adxt, Sint32 rmode ) ;
extern void ADXT_ExecServer( void ) ;	/* each vertical blank */
extern Sint32 ADXT_GetStat( ADXT adxt ) ;
extern Sint32 ADXT_IsIbufSafety( ADXT adxt ) ;	/* enough is read to play on */
extern void ADXT_Pause( ADXT adxt, Sint32 sw ) ;

/* volumes are in tenths of a decibel: 0 full, -960 silent */
extern void ADXT_StartFname( ADXT adxt, char *fname ) ;
extern void ADXT_Stop( ADXT adxt ) ;
extern void ADXT_SetOutVol( ADXT adxt, Sint32 vol ) ;
extern Sint32 ADXT_GetOutVol( ADXT adxt ) ;

#endif
