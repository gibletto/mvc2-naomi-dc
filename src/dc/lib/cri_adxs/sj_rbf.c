/* sj_rbf: the stream joint over a ring buffer (SJ 5.50). Its free and data lines go round the
   buffer; the extra area past its end mirrors the start, so a chunk never has to wrap. */
#include "sj.h"

#define SJRBF_MAX_OBJ	64

typedef struct {
	SJ_IF	*vtbl ;
	Sint32	used ;
	UUID	*uuid ;
	Sint32	datlen ;	/* the data line's length */
	Sint32	frelen ;	/* the free line's */
	Sint32	frepos ;	/* where the free line starts in the buffer */
	Sint32	datpos ;	/* and the data line */
	Sint8	*buf ;
	Sint32	bsize ;
	Sint32	xsize ;		/* the extra area after it */
	SJ_ERRFN	err_func ;
	void	*err_obj ;
} SJRBF_OBJ ;

typedef SJRBF_OBJ *SJRBF ;

void SJRBF_Destroy( SJ sj ) ;
UUID *SJRBF_GetUuid( SJ sj ) ;
void SJRBF_Reset( SJ sj ) ;
void SJRBF_GetChunk( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck ) ;
void SJRBF_UngetChunk( SJ sj, Sint32 id, SJCK *ck ) ;
void SJRBF_PutChunk( SJ sj, Sint32 id, SJCK *ck ) ;
Sint32 SJRBF_GetNumData( SJ sj, Sint32 id ) ;
Sint32 SJRBF_IsGetChunk( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte ) ;
void SJRBF_EntryErrFunc( SJ sj, SJ_ERRFN func, void *obj ) ;

char *sj_build = "\nSJ Ver.5.50 Build:Jun 10 1999 18:21:01\n" ;

SJ_IF sjrbf_vtbl = {
	NULL, NULL, NULL,
	SJRBF_Destroy,
	SJRBF_GetUuid,
	SJRBF_Reset,
	SJRBF_GetChunk,
	SJRBF_UngetChunk,
	SJRBF_PutChunk,
	SJRBF_GetNumData,
	SJRBF_IsGetChunk,
	SJRBF_EntryErrFunc,
} ;

static const UUID sjrbf_uuid = { 0x3b9a9e81, 0x0dbb, 0x11d2, { 0xa6, 0xbf, 0x44, 0x45, 0x53, 0x54, 0x00, 0x00 } } ;

Sint32 sjrbf_init_cnt = 0 ;
SJRBF_OBJ sjrbf_obj[SJRBF_MAX_OBJ] ;

/* the interrupts held off while a joint changes; gives the mask to put back */
Sint32 SJRBF_Lock( SJ sj )
{
	Sint32 im ;

	im = get_imask() ;
	set_imask( 15 ) ;
	return im ;
}

void SJRBF_Unlock( SJ sj, Sint32 im )
{
	set_imask( im ) ;
}

/* the error routine a joint starts with: it stops there */
void SJRBF_Error( void *obj, Sint32 ecode )
{
	for ( ; ; ) ;
}

void SJRBF_Init( void )
{
	if ( sjrbf_init_cnt > 0 ) {
		return ;
	}
	sjrbf_init_cnt++ ;
	memset( sjrbf_obj, 0, sizeof( sjrbf_obj ) ) ;
}

void SJRBF_Finish( void )
{
	sjrbf_init_cnt-- ;
	if ( sjrbf_init_cnt > 0 ) {
		return ;
	}
	/* the last user gone: the objects need nothing done */
}

SJ SJRBF_Create( Sint8 *buf, Sint32 bsize, Sint32 xsize )
{
	SJRBF sjrbf ;
	Sint32 i ;

	for ( i = 0 ; i < SJRBF_MAX_OBJ ; i++ ) {
		if ( sjrbf_obj[i].used == 0 ) {
			break ;
		}
	}
	if ( i == SJRBF_MAX_OBJ ) {
		return NULL ;
	}
	sjrbf = &sjrbf_obj[i] ;
	sjrbf->used = 1 ;
	sjrbf->vtbl = &sjrbf_vtbl ;
	sjrbf->buf = buf ;
	sjrbf->bsize = bsize ;
	sjrbf->xsize = xsize ;
	sjrbf->uuid = ( UUID * )&sjrbf_uuid ;
	sjrbf->err_func = SJRBF_Error ;
	sjrbf->err_obj = sjrbf ;
	SJRBF_Reset( ( SJ )sjrbf ) ;
	return ( SJ )sjrbf ;
}

void SJRBF_Destroy( SJ sj )
{
	SJRBF sjrbf ;

	sjrbf = ( SJRBF )sj ;
	if ( sjrbf != NULL ) {
		memset( sjrbf, 0, sizeof( SJRBF_OBJ ) ) ;
		sjrbf->used = 0 ;
	}
}

UUID *SJRBF_GetUuid( SJ sj )
{
	return ( ( SJRBF )sj )->uuid ;
}

void SJRBF_EntryErrFunc( SJ sj, SJ_ERRFN func, void *obj )
{
	SJRBF sjrbf ;

	sjrbf = ( SJRBF )sj ;
	sjrbf->err_func = func ;
	sjrbf->err_obj = obj ;
}

void SJRBF_Reset( SJ sj )
{
	SJRBF sjrbf ;

	sjrbf = ( SJRBF )sj ;
	sjrbf->datlen = 0 ;
	sjrbf->frelen = sjrbf->bsize ;
	sjrbf->frepos = 0 ;
	sjrbf->datpos = 0 ;
}

Sint32 SJRBF_GetNumData( SJ sj, Sint32 id )
{
	SJRBF sjrbf ;

	sjrbf = ( SJRBF )sj ;
	if ( id == SJ_LIN_DATA ) {
		return sjrbf->datlen ;
	}
	if ( id == SJ_LIN_FREE ) {
		return sjrbf->frelen ;
	}
	if ( sjrbf->err_func != NULL ) {
		sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
	}
	return 0 ;
}

void SJRBF_GetChunk( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck )
{
	SJRBF sjrbf ;
	Sint32 im ;

	sjrbf = ( SJRBF )sj ;
	im = SJRBF_Lock( ( SJ )sjrbf ) ;
	if ( id == SJ_LIN_FREE ) {
		ck->len = SJ_MIN( sjrbf->frelen, sjrbf->bsize - sjrbf->frepos + sjrbf->xsize ) ;
		ck->len = SJ_MIN( ck->len, nbyte ) ;
		ck->data = sjrbf->buf + sjrbf->frepos ;
		sjrbf->frepos = ( sjrbf->frepos + ck->len ) % sjrbf->bsize ;
		sjrbf->frelen -= ck->len ;
	} else if ( id == SJ_LIN_DATA ) {
		ck->len = SJ_MIN( sjrbf->datlen, sjrbf->bsize - sjrbf->datpos + sjrbf->xsize ) ;
		ck->len = SJ_MIN( ck->len, nbyte ) ;
		ck->data = sjrbf->buf + sjrbf->datpos ;
		sjrbf->datpos = ( sjrbf->datpos + ck->len ) % sjrbf->bsize ;
		sjrbf->datlen -= ck->len ;
	} else {
		ck->len = 0 ;
		ck->data = NULL ;
		if ( sjrbf->err_func != NULL ) {
			sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
		}
	}
	SJRBF_Unlock( ( SJ )sjrbf, im ) ;
}

/* data put into the data line is copied between the buffer's start and its extra area, so that
   either side of the wrap can be read in one piece */
void SJRBF_PutChunk( SJ sj, Sint32 id, SJCK *ck )
{
	register SJRBF sjrbf ;
	Sint32 im ;
	Sint32 ofs, n ;

	sjrbf = ( SJRBF )sj ;
	im = SJRBF_Lock( ( SJ )sjrbf ) ;
	if ( id == SJ_LIN_DATA ) {
		sjrbf->datlen += ck->len ;
		ofs = ck->data - sjrbf->buf ;
		if ( ofs < sjrbf->xsize ) {
			n = SJ_MIN( ck->len, sjrbf->xsize - ofs ) ;
			memcpy( &sjrbf->buf[sjrbf->bsize + ofs], ck->data, n ) ;
		}
		ofs = ck->data - sjrbf->buf + ck->len ;
		if ( ofs > sjrbf->bsize ) {
			n = SJ_MIN( ck->len, ofs - sjrbf->bsize ) ;
			memcpy( sjrbf->buf, &sjrbf->buf[ofs - n], n ) ;
		}
	} else if ( id == SJ_LIN_FREE ) {
		sjrbf->frelen += ck->len ;
	} else {
		ck->len = 0 ;
		ck->data = NULL ;
		if ( sjrbf->err_func != NULL ) {
			sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
		}
	}
	SJRBF_Unlock( ( SJ )sjrbf, im ) ;
}

void SJRBF_UngetChunk( SJ sj, Sint32 id, SJCK *ck )
{
	SJRBF sjrbf ;
	Sint32 im ;
	Sint32 pos, ofs ;

	sjrbf = ( SJRBF )sj ;
	im = SJRBF_Lock( ( SJ )sjrbf ) ;
	if ( id == SJ_LIN_FREE ) {
		pos = ( sjrbf->frepos + sjrbf->bsize - ck->len ) % sjrbf->bsize ;
		ofs = ( ck->data - sjrbf->buf ) % sjrbf->bsize ;
		if ( pos == ofs ) {
			sjrbf->frepos = pos ;
			sjrbf->frelen += ck->len ;
		} else if ( sjrbf->err_func != NULL ) {
			sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
		}
	} else if ( id == SJ_LIN_DATA ) {
		pos = ( sjrbf->datpos + sjrbf->bsize - ck->len ) % sjrbf->bsize ;
		ofs = ( ck->data - sjrbf->buf ) % sjrbf->bsize ;
		if ( pos == ofs ) {
			sjrbf->datpos = pos ;
			sjrbf->datlen += ck->len ;
		} else if ( sjrbf->err_func != NULL ) {
			sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
		}
	} else {
		ck->len = 0 ;
		ck->data = NULL ;
		if ( sjrbf->err_func != NULL ) {
			sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
		}
	}
	SJRBF_Unlock( ( SJ )sjrbf, im ) ;
}

Sint32 SJRBF_IsGetChunk( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte )
{
	SJRBF sjrbf ;
	Sint32 im ;
	Sint32 n ;

	sjrbf = ( SJRBF )sj ;
	im = SJRBF_Lock( ( SJ )sjrbf ) ;
	if ( id == SJ_LIN_FREE ) {
		n = SJ_MIN( sjrbf->frelen, sjrbf->bsize - sjrbf->frepos + sjrbf->xsize ) ;
		n = SJ_MIN( n, nbyte ) ;
	} else if ( id == SJ_LIN_DATA ) {
		n = SJ_MIN( sjrbf->datlen, sjrbf->bsize - sjrbf->datpos + sjrbf->xsize ) ;
		n = SJ_MIN( n, nbyte ) ;
	} else {
		n = 0 ;
		if ( sjrbf->err_func != NULL ) {
			sjrbf->err_func( sjrbf->err_obj, SJ_ERR_PRM ) ;
		}
	}
	*rbyte = n ;
	SJRBF_Unlock( ( SJ )sjrbf, im ) ;
	return n == nbyte ;
}

Sint8 *SJRBF_GetBufPtr( SJ sj )
{
	return ( ( SJRBF )sj )->buf ;
}

Sint32 SJRBF_GetBufSize( SJ sj )
{
	return ( ( SJRBF )sj )->bsize ;
}

Sint32 SJRBF_GetXtrSize( SJ sj )
{
	return ( ( SJRBF )sj )->xsize ;
}
