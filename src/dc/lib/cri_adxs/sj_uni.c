/* sj_uni: the stream joint over chains of chunks (SJ 5.50). Each of its lines is a list of chunks
   that live elsewhere; the nodes come from a pool in the work area the caller gives it. */
#include "sj.h"

#define SJUNI_MAX_OBJ	64
#define SJUNI_NLINE	4

#define SJUNI_MODE_SEPA	0	/* each chunk kept as it was put */
#define SJUNI_MODE_JOIN	1	/* chunks that follow on in memory joined into one */

/* a node of a line's chain */
typedef struct _sjuni_ck {
	struct _sjuni_ck	*next ;
	Sint32	rsv ;
	SJCK	ck ;
} SJUNI_CK ;

typedef struct {
	SJ_IF	*vtbl ;
	Sint8	used ;
	Sint8	mode ;
	UUID	*uuid ;
	SJUNI_CK	*work ;		/* the nodes */
	Sint32	nck ;		/* how many */
	SJUNI_CK	*pool ;		/* the chain of free ones */
	SJUNI_CK	*line[SJUNI_NLINE] ;
	SJ_ERRFN	err_func ;
	void	*err_obj ;
} SJUNI_OBJ ;

typedef SJUNI_OBJ *SJUNI ;

extern void SJ_SplitChunk( SJCK *ck, Sint32 nbyte, SJCK *ck1, SJCK *ck2 ) ;

void SJUNI_Destroy( SJ sj ) ;
UUID *SJUNI_GetUuid( SJ sj ) ;
void SJUNI_Reset( SJ sj ) ;
void SJUNI_GetChunk( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck ) ;
void SJUNI_UngetChunk( SJ sj, Sint32 id, SJCK *ck ) ;
void SJUNI_PutChunk( SJ sj, Sint32 id, SJCK *ck ) ;
Sint32 SJUNI_GetNumData( SJ sj, Sint32 id ) ;
Sint32 SJUNI_IsGetChunk( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte ) ;
void SJUNI_EntryErrFunc( SJ sj, SJ_ERRFN func, void *obj ) ;

SJ_IF sjuni_vtbl = {
	NULL, NULL, NULL,
	SJUNI_Destroy,
	SJUNI_GetUuid,
	SJUNI_Reset,
	SJUNI_GetChunk,
	SJUNI_UngetChunk,
	SJUNI_PutChunk,
	SJUNI_GetNumData,
	SJUNI_IsGetChunk,
	SJUNI_EntryErrFunc,
} ;

static const UUID sjuni_uuid = { 0x2e534fa3, 0xaf97, 0x11d2, { 0xa5, 0x27, 0x00, 0x60, 0x08, 0x94, 0x48, 0xbc } } ;

Sint32 sjuni_init_cnt = 0 ;
SJUNI_OBJ sjuni_obj[SJUNI_MAX_OBJ] ;

/* the interrupts held off while a joint changes; gives the mask to put back */
Sint32 SJUNI_Lock( SJ sj )
{
	Sint32 im ;

	im = get_imask() ;
	set_imask( 15 ) ;
	return im ;
}

void SJUNI_Unlock( SJ sj, Sint32 im )
{
	set_imask( im ) ;
}

/* the error routine a joint starts with: it stops there */
void SJUNI_Error( void *obj, Sint32 ecode )
{
	for ( ; ; ) ;
}

void SJUNI_Init( void )
{
	if ( sjuni_init_cnt > 0 ) {
		return ;
	}
	sjuni_init_cnt++ ;
	memset( sjuni_obj, 0, sizeof( sjuni_obj ) ) ;
}

void SJUNI_Finish( void )
{
	sjuni_init_cnt-- ;
	if ( sjuni_init_cnt > 0 ) {
		return ;
	}
	/* the last user gone: the objects need nothing done */
}

SJ SJUNI_Create( Sint32 mode, Sint8 *work, Sint32 wksize )
{
	SJUNI sjuni ;
	Sint32 i ;

	for ( i = 0 ; i < SJUNI_MAX_OBJ ; i++ ) {
		if ( sjuni_obj[i].used == 0 ) {
			break ;
		}
	}
	if ( i == SJUNI_MAX_OBJ ) {
		return NULL ;
	}
	sjuni = &sjuni_obj[i] ;
	sjuni->used = 1 ;
	sjuni->mode = mode ;
	sjuni->vtbl = &sjuni_vtbl ;
	sjuni->mode = mode ;
	sjuni->uuid = ( UUID * )&sjuni_uuid ;
	sjuni->work = ( SJUNI_CK * )work ;
	sjuni->nck = wksize / sizeof( SJUNI_CK ) ;
	sjuni->err_func = SJUNI_Error ;
	sjuni->err_obj = sjuni ;
	SJUNI_Reset( ( SJ )sjuni ) ;
	return ( SJ )sjuni ;
}

void SJUNI_Destroy( SJ sj )
{
	SJUNI sjuni ;

	sjuni = ( SJUNI )sj ;
	if ( sjuni != NULL ) {
		memset( sjuni, 0, sizeof( SJUNI_OBJ ) ) ;
		sjuni->used = 0 ;
	}
}

UUID *SJUNI_GetUuid( SJ sj )
{
	return ( ( SJUNI )sj )->uuid ;
}

void SJUNI_EntryErrFunc( SJ sj, SJ_ERRFN func, void *obj )
{
	SJUNI sjuni ;

	sjuni = ( SJUNI )sj ;
	sjuni->err_func = func ;
	sjuni->err_obj = obj ;
}

/* every node back in the pool, and the lines empty */
void SJUNI_Reset( SJ sj )
{
	SJUNI sjuni ;
	SJUNI_CK *work ;
	Sint32 i ;

	sjuni = ( SJUNI )sj ;
	work = sjuni->work ;
	sjuni->pool = work ;
	for ( i = 0 ; i < sjuni->nck - 1 ; i++ ) {
		work[i].next = &work[i + 1] ;
		work[i].ck.data = NULL ;
		work[i].ck.len = 0 ;
	}
	work[i].next = NULL ;
	work[i].ck.data = NULL ;
	work[i].ck.len = 0 ;
	for ( i = 0 ; i < SJUNI_NLINE ; i++ ) {
		sjuni->line[i] = NULL ;
	}
}

Sint32 SJUNI_GetNumData( SJ sj, Sint32 id )
{
	SJUNI sjuni ;
	SJUNI_CK *node ;
	Sint32 n ;

	sjuni = ( SJUNI )sj ;
	if ( id < 0 || id >= SJUNI_NLINE ) {
		if ( sjuni->err_func != NULL ) {
			sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
		}
		return 0 ;
	}
	n = 0 ;
	for ( node = sjuni->line[id] ; node != NULL ; node = node->next ) {
		n += node->ck.len ;
	}
	return n ;
}

/* the first chunk of a line, or as much of it as was asked for where chunks are joined */
void SJUNI_GetChunk( SJ sj, Sint32 id, Sint32 nbyte, SJCK *ck )
{
	SJUNI sjuni ;
	Sint32 im ;
	SJUNI_CK *node ;
	SJCK ck1, ck2 ;

	sjuni = ( SJUNI )sj ;
	if ( id < 0 || id >= SJUNI_NLINE ) {
		if ( sjuni->err_func != NULL ) {
			sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
		}
		ck->data = NULL ;
		ck->len = 0 ;
		return ;
	}
	im = SJUNI_Lock( ( SJ )sjuni ) ;
	node = sjuni->line[id] ;
	if ( node != NULL ) {
		ck1 = node->ck ;
		if ( ck1.len <= nbyte ) {
			*ck = ck1 ;
			sjuni->line[id] = node->next ;
			node->next = sjuni->pool ;
			sjuni->pool = node ;
		} else if ( sjuni->mode == SJUNI_MODE_JOIN ) {
			SJ_SplitChunk( &ck1, nbyte, &ck1, &ck2 ) ;
			*ck = ck1 ;
			node->ck = ck2 ;
		} else {
			ck->data = NULL ;
			ck->len = 0 ;
		}
	} else {
		ck->data = NULL ;
		ck->len = 0 ;
	}
	SJUNI_Unlock( ( SJ )sjuni, im ) ;
}

/* a chunk put at the end of a line, joined to the last one where it follows on from it */
void SJUNI_PutChunk( SJ sj, Sint32 id, SJCK *ck )
{
	SJUNI sjuni ;
	Sint32 im ;
	SJUNI_CK **p, *last, *node ;

	sjuni = ( SJUNI )sj ;
	if ( id < 0 || id >= SJUNI_NLINE ) {
		if ( sjuni->err_func != NULL ) {
			sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
		}
		return ;
	}
	if ( ck->len <= 0 || ck->data == NULL ) {
		return ;
	}
	im = SJUNI_Lock( ( SJ )sjuni ) ;
	last = NULL ;
	for ( p = &sjuni->line[id] ; *p != NULL ; p = &( *p )->next ) {
		last = *p ;
	}
	if ( sjuni->mode == SJUNI_MODE_JOIN && last != NULL && last->ck.data + last->ck.len == ck->data ) {
		last->ck.len += ck->len ;
	} else {
		node = sjuni->pool ;
		if ( node == NULL ) {
			if ( sjuni->err_func != NULL ) {
				sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
			}
		} else {
			sjuni->pool = node->next ;
			node->next = NULL ;
			node->ck = *ck ;
			*p = node ;
		}
	}
	SJUNI_Unlock( ( SJ )sjuni, im ) ;
}

/* a chunk put back at the start of a line, joined to the first one where that follows on from it */
void SJUNI_UngetChunk( SJ sj, Sint32 id, SJCK *ck )
{
	SJUNI sjuni ;
	Sint32 im ;
	SJUNI_CK *first, *node ;

	sjuni = ( SJUNI )sj ;
	if ( id < 0 || id >= SJUNI_NLINE ) {
		if ( sjuni->err_func != NULL ) {
			sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
		}
		return ;
	}
	if ( ck->len <= 0 || ck->data == NULL ) {
		return ;
	}
	im = SJUNI_Lock( ( SJ )sjuni ) ;
	first = sjuni->line[id] ;
	if ( sjuni->mode == SJUNI_MODE_JOIN && first != NULL && ck->data + ck->len == first->ck.data ) {
		first->ck.data = ck->data ;
		first->ck.len += ck->len ;
	} else {
		node = sjuni->pool ;
		if ( node == NULL ) {
			if ( sjuni->err_func != NULL ) {
				sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
			}
		} else {
			sjuni->pool = node->next ;
			node->next = NULL ;
			node->ck = *ck ;
			node->next = sjuni->line[id] ;
			sjuni->line[id] = node ;
		}
	}
	SJUNI_Unlock( ( SJ )sjuni, im ) ;
}

Sint32 SJUNI_IsGetChunk( SJ sj, Sint32 id, Sint32 nbyte, Sint32 *rbyte )
{
	SJUNI sjuni ;
	SJUNI_CK *node ;
	SJCK ck ;

	sjuni = ( SJUNI )sj ;
	*rbyte = 0 ;
	if ( id < 0 || id >= SJUNI_NLINE ) {
		if ( sjuni->err_func != NULL ) {
			sjuni->err_func( sjuni->err_obj, SJ_ERR_PRM ) ;
		}
		return 0 ;
	}
	node = sjuni->line[id] ;
	if ( node == NULL ) {
		return 0 ;
	}
	ck = node->ck ;
	*rbyte = ck.len ;
	if ( sjuni->mode == SJUNI_MODE_JOIN ) {
		if ( ck.len >= nbyte ) {
			return 1 ;
		}
	} else if ( ck.len == nbyte ) {
		return 1 ;
	}
	return 0 ;
}

Sint32 SJUNI_GetNumChunk( SJ sj, Sint32 id )
{
	SJUNI sjuni ;
	SJUNI_CK *node ;
	Sint32 n ;

	sjuni = ( SJUNI )sj ;
	for ( node = sjuni->line[id], n = 0 ; node != NULL ; node = node->next ) {
		n++ ;
	}
	return n ;
}

Sint32 SJUNI_GetNumChainPool( SJ sj )
{
	SJUNI sjuni ;
	SJUNI_CK *node ;
	Sint32 n ;

	sjuni = ( SJUNI )sj ;
	for ( node = sjuni->pool, n = 0 ; node != NULL ; node = node->next ) {
		n++ ;
	}
	return n ;
}
