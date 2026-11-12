/* sj_mem: the stream joint over data already in memory (SJ 5.50). Its data line is the data not
   yet got, and nothing can be put into it. */
#include "sj.h"

#define SJMEM_MAX_OBJ	32

typedef struct {
	SJ_IF	*vtbl ;
	Sint32	used ;
	UUID	*uuid ;
	Sint32	num ;		/* the data not yet got */
	Sint32	pos ;		/* where it starts in the buffer */
	Sint8	*buf ;
	Sint32	bsize ;
	SJ_ERRFN	err_func ;
	void	*err_obj ;
} SJMEM_OBJ ;

typedef SJMEM_OBJ *SJMEM ;

void SJMEM_Destroy( SJ sj ) ;
UUID *SJMEM_GetUuid( SJ sj ) ;
void SJMEM_Reset( SJ sj ) ;
void SJMEM_GetChunk( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck ) ;
void SJMEM_UngetChunk( SJ sj, Sint32 id, SJCK *ck ) ;
void SJMEM_PutChunk( SJ sj, Sint32 id, SJCK *ck ) ;
Sint32 SJMEM_GetNumData( SJ sj, Sint32 id ) ;
Sint32 SJMEM_IsGetChunk( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte ) ;
void SJMEM_EntryErrFunc( SJ sj, SJ_ERRFN func, void *obj ) ;

SJ_IF sjmem_vtbl = {
	NULL, NULL, NULL,
	SJMEM_Destroy,
	SJMEM_GetUuid,
	SJMEM_Reset,
	SJMEM_GetChunk,
	SJMEM_UngetChunk,
	SJMEM_PutChunk,
	SJMEM_GetNumData,
	SJMEM_IsGetChunk,
	SJMEM_EntryErrFunc,
} ;

static const UUID sjmem_uuid = { 0xdd9eee41, 0x1679, 0x11d2, { 0x93, 0x6c, 0x00, 0x60, 0x08, 0x94, 0x48, 0xbc } } ;

Sint32 sjmem_init_cnt = 0 ;
SJMEM_OBJ sjmem_obj[SJMEM_MAX_OBJ] ;

/* the interrupts held off while a joint changes; gives the mask to put back */
Sint32 SJMEM_Lock( SJ sj )
{
	Sint32 im ;

	im = get_imask() ;
	set_imask( 15 ) ;
	return im ;
}

void SJMEM_Unlock( SJ sj, Sint32 im )
{
	set_imask( im ) ;
}

/* the error routine a joint starts with: it stops there */
void SJMEM_Error( void *obj, Sint32 ecode )
{
	for ( ; ; ) ;
}

void SJMEM_Init( void )
{
	if ( sjmem_init_cnt > 0 ) {
		return ;
	}
	sjmem_init_cnt++ ;
	memset( sjmem_obj, 0, sizeof( sjmem_obj ) ) ;
}

void SJMEM_Finish( void )
{
	sjmem_init_cnt-- ;
	if ( sjmem_init_cnt > 0 ) {
		return ;
	}
	/* the last user gone: the objects need nothing done */
}

SJ SJMEM_Create( Sint8 *data, Sint32 bsize )
{
	SJMEM sjmem ;
	Sint32 i ;

	for ( i = 0 ; i < SJMEM_MAX_OBJ ; i++ ) {
		if ( sjmem_obj[i].used == 0 ) {
			break ;
		}
	}
	if ( i == SJMEM_MAX_OBJ ) {
		return NULL ;
	}
	sjmem = &sjmem_obj[i] ;
	sjmem->used = 1 ;
	sjmem->vtbl = &sjmem_vtbl ;
	sjmem->buf = data ;
	sjmem->bsize = bsize ;
	sjmem->uuid = ( UUID * )&sjmem_uuid ;
	sjmem->err_func = SJMEM_Error ;
	sjmem->err_obj = sjmem ;
	SJMEM_Reset( ( SJ )sjmem ) ;
	return ( SJ )sjmem ;
}

void SJMEM_Destroy( SJ sj )
{
	SJMEM sjmem ;

	sjmem = ( SJMEM )sj ;
	if ( sjmem != NULL ) {
		memset( sjmem, 0, sizeof( SJMEM_OBJ ) ) ;
		sjmem->used = 0 ;
	}
}

UUID *SJMEM_GetUuid( SJ sj )
{
	return ( ( SJMEM )sj )->uuid ;
}

void SJMEM_EntryErrFunc( SJ sj, SJ_ERRFN func, void *obj )
{
	SJMEM sjmem ;

	sjmem = ( SJMEM )sj ;
	sjmem->err_func = func ;
	sjmem->err_obj = obj ;
}

void SJMEM_Reset( SJ sj )
{
	SJMEM sjmem ;

	sjmem = ( SJMEM )sj ;
	sjmem->num = sjmem->bsize ;
	sjmem->pos = 0 ;
}

Sint32 SJMEM_GetNumData( SJ sj, Sint32 id )
{
	SJMEM sjmem ;

	sjmem = ( SJMEM )sj ;
	if ( id == SJ_LIN_DATA ) {
		return sjmem->num ;
	}
	if ( id == SJ_LIN_FREE ) {
		return 0 ;
	}
	if ( sjmem->err_func != NULL ) {
		sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
	}
	return 0 ;
}

void SJMEM_GetChunk( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck )
{
	SJMEM sjmem ;
	Sint32 im ;

	sjmem = ( SJMEM )sj ;
	im = SJMEM_Lock( ( SJ )sjmem ) ;
	if ( id == SJ_LIN_FREE ) {
		ck->len = 0 ;
		ck->data = NULL ;
	} else if ( id == SJ_LIN_DATA ) {
		ck->len = SJ_MIN( sjmem->num, nbyte ) ;
		ck->data = sjmem->buf + sjmem->pos ;
		sjmem->pos = sjmem->pos + ck->len ;
		sjmem->num -= ck->len ;
	} else {
		ck->len = 0 ;
		ck->data = NULL ;
		if ( sjmem->err_func != NULL ) {
			sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
		}
	}
	SJMEM_Unlock( ( SJ )sjmem, im ) ;
}

void SJMEM_PutChunk( SJ sj, Sint32 id, SJCK *ck )
{
	SJMEM sjmem ;
	Sint32 im ;

	sjmem = ( SJMEM )sj ;
	im = SJMEM_Lock( ( SJ )sjmem ) ;
	if ( id == SJ_LIN_FREE ) {
		/* the data line can't grow */
	} else if ( id == SJ_LIN_DATA ) {
		/* nor the free one */
	} else {
		ck->len = 0 ;
		ck->data = NULL ;
		if ( sjmem->err_func != NULL ) {
			sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
		}
	}
	SJMEM_Unlock( ( SJ )sjmem, im ) ;
}

void SJMEM_UngetChunk( SJ sj, Sint32 id, SJCK *ck )
{
	SJMEM sjmem ;
	Sint32 im ;
	Sint32 pos, ofs ;

	sjmem = ( SJMEM )sj ;
	im = SJMEM_Lock( ( SJ )sjmem ) ;
	if ( id == SJ_LIN_FREE ) {
		if ( sjmem->err_func != NULL ) {
			sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
		}
	} else if ( id == SJ_LIN_DATA ) {
		pos = sjmem->pos - ck->len ;
		pos = ( pos > 0 ) ? pos : 0 ;
		sjmem->pos = pos ;
		sjmem->num = SJ_MIN( sjmem->bsize, sjmem->num + ck->len ) ;
		ofs = ck->data - sjmem->buf ;
		if ( pos != ofs ) {
			if ( sjmem->err_func != NULL ) {
				sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
			}
		}
	} else {
		ck->len = 0 ;
		ck->data = NULL ;
		if ( sjmem->err_func != NULL ) {
			sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
		}
	}
	SJMEM_Unlock( ( SJ )sjmem, im ) ;
}

Sint32 SJMEM_IsGetChunk( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte )
{
	SJMEM sjmem ;
	Sint32 im ;
	Sint32 n ;

	sjmem = ( SJMEM )sj ;
	im = SJMEM_Lock( ( SJ )sjmem ) ;
	if ( id == SJ_LIN_FREE ) {
		n = 0 ;
	} else if ( id == SJ_LIN_DATA ) {
		n = SJ_MIN( sjmem->num, nbyte ) ;
	} else {
		n = 0 ;
		if ( sjmem->err_func != NULL ) {
			sjmem->err_func( sjmem->err_obj, SJ_ERR_PRM ) ;
		}
	}
	*rbyte = n ;
	SJMEM_Unlock( ( SJ )sjmem, im ) ;
	return n == nbyte ;
}

Sint8 *SJMEM_GetBufPtr( SJ sj )
{
	return ( ( SJMEM )sj )->buf ;
}

Sint32 SJMEM_GetBufSize( SJ sj )
{
	return ( ( SJMEM )sj )->bsize ;
}
