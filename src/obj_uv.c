/* The model helpers that follow wall_dust.c: a cursor over the vertices of a
   model (obj_uv_begin and its next, get and put) that the texture scrolls use on two models at
   once, and routines on a model's materials (their count, their headers, their colour). */
#include "player.h"

/* a model's vertex: where it stands, its normal and where it reads its texture */
typedef struct OBJ_VTX {
	VEC3	pos;
	VEC3	nrm;
	Float	u, v;
} OBJ_VTX;

/* a material of a model: a polygon header, then its vertices */
typedef struct OBJ_MAT {
	Uint32	ctrl;		/* 00 */
	Uint32	isp;		/* 04 */
	Uint32	tsp;		/* 08: the blend modes are its top six bits */
	Uint32	tcw;		/* 0c */
	Sint32	unk_10[7];
	Uint32	col[4];		/* 2c: its alpha, red, green and blue (float words) */
	Sint32	unk_3c[4];
	Sint32	size;		/* 4c: of the vertices after the header */
	Uint8	data[4];	/* 50 */
} OBJ_MAT;

#define OBJ_MAT_NEXT( m )	( ( OBJ_MAT * )( ( m )->data + ( m )->size ) )

static int *uv_src ;		/* the vertex it stands on in the model it reads */
static int *uv_src_top ;	/* and the next one */
static int *uv_dst ;		/* the same in the model it writes */
static int *uv_dst_top ;
static Sint32 uv_in ;		/* inside a material */
static Sint32 uv_cnt ;		/* the vertex of it */
static Sint32 uv_num ;		/* how many it has */
static Uint8 unk_0C2FB5D0[64] ;

/****************************************************************/
int obj_uv_begin( int *src, int *obj )
{
	uv_src = src ;
	uv_src += 6 ;
	uv_dst = obj ;
	uv_dst += 6 ;
	uv_src_top = uv_src ;
	uv_dst_top = uv_dst ;
	uv_in = 0 ;
	return 0 ;
}

/****************************************************************/
/*			Steps to the next vertex of both models; -1 at		*/
/*			the end of the model								*/
/****************************************************************/
int obj_uv_next( void )
{
	uv_src = uv_src_top ;
	uv_dst = uv_dst_top ;
	for(;;){
		if( uv_in == 0 ){
			if( !uv_src[0] ) return -1 ;
			if( uv_src[0] < 0 ){
				uv_src += 20 ;
				uv_dst += 20 ;
			}
			if( *uv_src ++ & 0x10 ){
				uv_num = *uv_src ;
				uv_src ++ ;
			} else {
				uv_num = uv_src[0] * 3 ;
				uv_src ++ ;
			}
			uv_dst += 2 ;
			uv_in = 1 ;
			uv_cnt = 0 ;
		}
		if( uv_src[0] & 1 ){
			uv_src_top = uv_src ;
			uv_dst_top = uv_dst ;
			uv_src_top += 8 ;
			uv_dst_top += 8 ;
			uv_cnt ++ ;
			if( uv_cnt >= uv_num ) uv_in = 0 ;
			return 0 ;
		} else {
			uv_src += 2 ;
			uv_dst += 2 ;
			uv_src_top = uv_src ;
			uv_dst_top = uv_dst ;
			uv_cnt ++ ;
			if( uv_cnt >= uv_num ) uv_in = 0 ;
		}
	}
}

/****************************************************************/
int obj_uv_pos_get( VEC3 *v )
{
	*v = ( ( OBJ_VTX * )uv_src )->pos ;
	return 0 ;
}

/****************************************************************/
int obj_uv_nrm_get( VEC3 *v )
{
	*v = ( ( OBJ_VTX * )uv_src )->nrm ;
	return 0 ;
}

/****************************************************************/
int obj_uv_get( Float *x, Float *y )
{
	*x = *( Float * )( uv_src + 6 ) ;
	*y = *( Float * )( uv_src + 7 ) ;
	return 0 ;
}

/****************************************************************/
int obj_uv_col_get( Sint32 *col )
{
	*col = uv_src[4] ;
	return 0 ;
}

/****************************************************************/
int obj_uv_pos_put( VEC3 *v )
{
	( ( OBJ_VTX * )uv_dst )->pos = *v ;
	uv_dst[0] |= 1 ;
	return 0 ;
}

/****************************************************************/
int obj_uv_nrm_put( VEC3 *v )
{
	( ( OBJ_VTX * )uv_dst )->nrm = *v ;
	return 0 ;
}

/****************************************************************/
int obj_uv_put( Float *x, Float *y )
{
	*( Float * )( uv_dst + 6 ) = *x ;
	*( Float * )( uv_dst + 7 ) = *y ;
	uv_dst[7] |= 1 ;
	return 0 ;
}

/****************************************************************/
int obj_uv_col_put( Uint32 *col )
{
	uv_dst[4] = *col ;
	return 0 ;
}

/****************************************************************/
void obj_uv_clear( int *obj )
{
	OBJ_MAT *mat, *next ;

	if( obj[0] >= 0 && obj[6] ){
		next = ( OBJ_MAT * )&obj[6] ;
		for( ;; ){
			mat = next ;
			mat->tsp &= 0xfffe7fff ;
			next = OBJ_MAT_NEXT( mat ) ;
			if( !next->ctrl ) break ;
		}
	}
}

/****************************************************************/
/*			The number of vertices of a model					*/
/****************************************************************/
void obj_vtx_count( int *obj, Sint32 *num )
{
	*num = 0 ;
	obj_uv_begin( obj, obj ) ;
	while( obj_uv_next() == 0 ) ( *num ) ++ ;
}

/****************************************************************/
/*			A translucent model's blend modes					*/
/****************************************************************/
void obj_blend_set( int *obj, Sint32 src, Sint32 dst )
{
	OBJ_MAT *mat ;
	int ctrl ;

	if( obj[0] >= 0 && obj[6] ){
		obj += 6 ;
		do {
			mat = ( OBJ_MAT * )obj ;
			obj = ( int * )mat->data ;
			if( ( mat->ctrl >> 24 & 7 ) == 2 ){
				mat->tsp &= 0x3ffffff ;
				mat->tsp |= ( src << 29 ) + ( dst << 26 ) ;
			}
			obj = ( int * )( ( int )obj + mat->size ) ;
			ctrl = *obj ;
		} while( ctrl ) ;
	}
}

/****************************************************************/
/*			The number of materials of a model					*/
/****************************************************************/
void obj_mat_count( int *obj, Sint32 *num )
{
	OBJ_MAT *mat ;
	int ctrl ;

	if( obj[0] >= 0 && obj[6] ){
		obj += 6 ;
		*num = 0 ;
		for( ;; ){
			mat = ( OBJ_MAT * )obj ;
			obj = ( int * )mat->data ;
			( *num ) ++ ;
			obj = ( int * )( ( int )obj + mat->size ) ;
			ctrl = *obj ;
			if( !ctrl ) break ;
		}
	}
}

/****************************************************************/
/*			Keep the headers of a model's materials, four		*/
/*			words each, and put them back						*/
/****************************************************************/
void obj_mat_save( int *obj, Sint32 *buf )
{
	OBJ_MAT *mat, *next ;
	Uint32 ctrl ;

	if( obj[0] >= 0 && obj[6] ){
		next = ( OBJ_MAT * )&obj[6] ;
		for( ;; ){
			mat = next ;
			*buf ++ = mat->ctrl ;
			*buf ++ = mat->isp ;
			*buf ++ = mat->tsp ;
			*buf ++ = mat->tcw ;
			next = ( OBJ_MAT * )next->data ;
			next = ( OBJ_MAT * )( ( int )next + mat->size ) ;
			ctrl = next->ctrl ;
			if( !ctrl ) break ;
		}
	}
}

/****************************************************************/
void obj_mat_load( int *obj, Sint32 *buf )
{
	OBJ_MAT *mat, *next ;
	Uint32 ctrl ;

	if( obj[0] >= 0 && obj[6] ){
		next = ( OBJ_MAT * )&obj[6] ;
		for( ;; ){
			mat = next ;
			mat->ctrl = *buf ++ ;
			mat->isp = *buf ++ ;
			mat->tsp = *buf ++ ;
			mat->tcw = *buf ++ ;
			next = ( OBJ_MAT * )next->data ;
			next = ( OBJ_MAT * )( ( int )next + mat->size ) ;
			ctrl = next->ctrl ;
			if( !ctrl ) break ;
		}
	}
}

/****************************************************************/
/*			The colour of a model's materials (the floats		*/
/*			are copied as words, through memory)				*/
/****************************************************************/
void obj_argb_set( int *obj, Float a, Float r, Float g, Float b )
{
	OBJ_MAT *mat, *next ;

	if( obj[0] >= 0 && obj[6] ){
		next = ( OBJ_MAT * )&obj[6] ;
		for( ;; ){
			mat = next ;
			mat->col[0] = *( Uint32 * )&a ;
			mat->col[1] = *( Uint32 * )&r ;
			mat->col[2] = *( Uint32 * )&g ;
			mat->col[3] = *( Uint32 * )&b ;
			next = OBJ_MAT_NEXT( mat ) ;
			if( !next->ctrl ) break ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "obj_uv.c" End of File	*/
