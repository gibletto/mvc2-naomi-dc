/* Stage 07's first object, second part: three models that swing about the path of
   set07_00.c with the sine of its angle, bob in size and shimmer in colour. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* Kamui */
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern int obj_uv_pos_get( VEC3 *v ) ;
extern int obj_uv_pos_put( VEC3 *v ) ;

/* a vertex's normal slot, which these models use for a colour */
typedef struct SET07_00B_NRM {
	Float	x;
	Uint32	col;
	Float	z;
} SET07_00B_NRM;
extern int obj_uv_nrm_get( SET07_00B_NRM *v ) ;
extern int obj_uv_nrm_put( SET07_00B_NRM *v ) ;

static VEC3 set07_00b_pos_tbl[3] = {
	{ -9321.09f, 17368.2f, -46053.2f },
	{ -10.978f, 17752.14f, -45790.0f },
	{ 6646.787f, 17524.19f, -49964.5f },
} ;
static VEC3 set07_00b_rot_tbl[3] = {
	{ -15.0f, -40.0f, -5.0f },
	{ -15.0f, 5.0f, -12.0f },
	{ -20.0f, 8.0f, -12.0f },
} ;
static Float set07_00b_amp_tbl[3] = { 1.0f, 0.75f, 0.5f } ;
static VEC3 set07_00b_scale = { 1.0f, 1.0f, 1.0f } ;

/* the work these models keep from obj_flag on: the flag, then three angles */
typedef struct SET07_00B_LOCAL_W {
	Sint32	flag;
	Sint32	ang0;
	Sint32	ang1;
	Sint32	ang2;
} SET07_00B_LOCAL_W;
#define SET07_00B_LW( w )	( ( SET07_00B_LOCAL_W * )&( w )->obj_flag )

static void set07_00b_make( PLAYER_WORK *pl_w, int no ) ;

/****************************************************************/
void set07_00b_set( PLAYER_WORK *pl_w )
{
	int i ;

	for( i = 0 ; i < 3 ; i ++ ) set07_00b_make( pl_w, i ) ;
}

static void set07_00b_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set07_00b_make( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set07_00b_main ;
		ew->obj = obj_stg->obj[no * 2 + 9] ;
		ew->pos = set07_00b_pos_tbl[no] ;
		ew->rot[0] = DEG_ANG( set07_00b_rot_tbl[no].x ) ;
		ew->rot[1] = DEG_ANG( set07_00b_rot_tbl[no].y ) ;
		ew->rot[2] = DEG_ANG( set07_00b_rot_tbl[no].z ) ;
		ew->scale = set07_00b_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_BRIGHT ;
		ew->type = no ;
		ew->timer0 = no * 60 ;
		ew->timer1 = no * 60 ;
		ew->work1 = no * 4096 ;
		ew->work2 = no * 12288 ;
		ew->work3 = no * 20480 ;
		ew->ef_w = pl_w ;
	}
}

static void set07_00b_move( PLAYER_WORK *ew ) ;
static void set07_00b_wave( PLAYER_WORK *ew ) ;
static void set07_00b_nop( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set07_00b_main( PLAYER_WORK *ew )
{
	set07_00b_move( ew ) ;
	set07_00b_wave( ew ) ;
	set07_00b_nop( ew ) ;
}

/*==============================================================*/
static void set07_00b_move( PLAYER_WORK *ew )
{
	ew->pos.x = set07_00b_pos_tbl[ew->type].x - nlSin( ew->ef_w->rot[1] ) * 16000.0f ;
}

/*==============================================================*/
/*			The colours of the model's vertices shimmer, and	*/
/*			its points sway										*/
/*==============================================================*/
static void set07_00b_wave( PLAYER_WORK *ew )
{
	VEC3 v ;
	SET07_00B_NRM nrm ;
	Sint32 ang0, ang1, ang2 ;
	Uint32 *col ;
	Uint8 r, g, b ;
	int n ;
	SET07_00B_LOCAL_W *lw ;

	ew->timer0 += ew->type + 1 ;
	if( ew->timer0 >= 1440 ) ew->timer0 %= 1440 ;
	if( ++ ew->timer1 >= 360 ) ew->timer1 = 0 ;
	ew->work1 = ew->work1 + 0x100 ;
	ew->work2 = ew->work2 + 0x80 ;
	ew->work3 = ew->work3 + 64 ;
	ew->work1 &= 0xffff ;
	ew->work2 &= 0xffff ;
	ew->work3 &= 0xffff ;
	if( 1 ) lw = SET07_00B_LW( ew ) ;
	else lw = ( SET07_00B_LOCAL_W * )ew ;
	ang0 = lw->ang0 ;
	ang1 = lw->ang1 ;
	ang2 = lw->ang2 ;
	obj_uv_begin( obj_stg->obj[ew->type * 2 + 9 + 1], ew->obj ) ;
	n = 0 ;
	while( obj_uv_next() == 0 ){
		obj_uv_pos_get( &v ) ;
		v.x += 2000.0f * ( set07_00b_amp_tbl[ew->type] * ( 1.0f + 0.5f * nlSin( DEG_ANG( ew->timer1 ) ) ) * nlSin( DEG_ANG( ew->timer0 / 4 + n ) ) ) ;
		n += 30 ;
		obj_uv_pos_put( &v ) ;
		ang0 += 0x800 ;
		ang1 += 0x800 ;
		ang2 += 0x800 ;
		obj_uv_nrm_get( &nrm ) ;
		col = &nrm.col ;
		r = nlSin( ang0 & 0xffff ) * 64.0f + 96.0f ;
		g = nlSin( ang1 & 0xffff ) * 64.0f + 96.0f ;
		b = nlSin( ang2 & 0xffff ) * 64.0f + 96.0f ;
		*col = ( r << 16 ) | ( g << 8 ) | b ;
		obj_uv_nrm_put( &nrm ) ;
	}
}

/*==============================================================*/
static void set07_00b_nop( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set07_00b.c" End of File	*/
