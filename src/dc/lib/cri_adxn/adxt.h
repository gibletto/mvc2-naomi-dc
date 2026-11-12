/* The ADX player's object (CRI ADXT 5.66), as far as the modules here use it: what the ADXT
   handle of adx.h's calls points to. */
#ifndef ADXT_H
#define ADXT_H

#include "adx.h"

#define ADXT_MAX_OBJ		8
#define ADXT_MAX_DATASIZE	0x40000000

#define ADXT_PLY_MEMIDX		2	/* pmode: playing from data in memory */

typedef struct _adx_talk {
	Sint8	used ;
	Sint8	stat ;
	Sint8	pmode ;
	Sint8	maxnch ;
	void	*sjd ;
	void	*stm ;		/* the stream it reads from */
	void	*rna ;
	void	*sjf ;		/* the stream joint the file's data comes in by */
	Uint8	unk_14[0x78] ;
} ADX_TALK ;

extern void ADXERR_CallErrFunc( char *format, ... ) ;
extern void ADXCRS_Lock( void ) ;
extern void ADXCRS_Unlock( void ) ;

#endif
