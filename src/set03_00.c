/* Stage 03's object 0: a model (object 1 of the stage's file) whose points move from one of its
   stored shapes to the next, in time with a table, and back round to the first. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern void *memcpy( void *dst, const void *src, Uint32 n ) ;

/* the shapes' file, in words: the count of points that move and a second word, the numbers of
   those points, and then every shape's points */
extern Sint32 unk_0D852038[] ;

static const VEC3 set03_00_pos = { -149.0f, 647.0f, -898.0f } ;
static const VEC3 set03_00_rot = { 0.0f, 5.0f, 0.0f } ;

/* the shapes it goes through, and how many frames each takes */
static Sint32 set03_00_shape_tbl[34] = {
	0, 2, 5, 1, 4, 1, 4, 1, 2, 2, 2, 5, 2, 5, 2, 5, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 6, 3, 1, 4, 1,
	4, 1,
} ;
static Sint32 set03_00_time_tbl[34] = {
	20, 23, 20, 30, 14, 27, 77, 116, 26, 35, 20, 21, 20, 28, 18, 26, 20, 20, 26, 12, 10, 10, 10, 10,
	10, 20, 20, 21, 25, 20, 20, 20, 20, 20,
} ;

static void set03_00_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set03_00_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set03_00_main ;
		ew->obj = obj_stg->obj[1] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set03_00_pos ;
		ew->rot[1] = DEG_ANG( set03_00_rot.y ) ;
	}
}

static void set03_00_morph( PLAYER_WORK *ew, Float t, int a, int b ) ;

/*==============================================================*/
static void set03_00_main( PLAYER_WORK *ew )
{
	Float t ;
	int a, b ;

	ew->timer0 ++ ;
	if( ew->timer0 >= set03_00_time_tbl[ew->timer1] ){
		ew->timer0 = 0 ;
		ew->timer1 ++ ;
		if( ew->timer1 >= sizeof( set03_00_shape_tbl ) / sizeof( set03_00_shape_tbl[0] ) ) ew->timer1 = 0 ;
	}
	t = 0.5f + 0.5f * nlSin( DEG_ANG( -90.0f + 180.0f * ( ( Float )ew->timer0 / set03_00_time_tbl[ew->timer1] ) ) ) ;
	a = set03_00_shape_tbl[ew->timer1] ;
	if( ew->timer1 + 1 >= sizeof( set03_00_shape_tbl ) / sizeof( set03_00_shape_tbl[0] ) ) b = set03_00_shape_tbl[0] ;
	else b = set03_00_shape_tbl[ew->timer1 + 1] ;
	set03_00_morph( ew, t, a, b ) ;
}

/*==============================================================*/
/*			Each point that moves, set t of the way from		*/
/*			shape a to shape b									*/
/*==============================================================*/
static void set03_00_morph( PLAYER_WORK *ew, Float t, int a, int b )
{
	Sint32 *file, *no ;
	int cnt ;
	VEC3 *shape ;
	VEC3 *pa, *pb ;
	VEC3 v ;
	int i ;

	file = no = unk_0D852038 ;
	cnt = *no ;
	no += 2 ;
	shape = ( VEC3 * )( file + cnt + 2 ) ;
	for( i = 0 ; i < cnt ; i ++ ){
		pa = &shape[cnt * a + i] ;
		pb = &shape[cnt * b + i] ;
		v.x = pa->x + ( pb->x - pa->x ) * t ;
		v.y = pa->y + ( pb->y - pa->y ) * t ;
		v.z = pa->z + ( pb->z - pa->z ) * t ;
		memcpy( ( Sint32 * )ew->obj + no[i], &v, 12 ) ;
		( ( Sint32 * )ew->obj )[no[i]] |= 1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set03_00.c" End of File	*/
