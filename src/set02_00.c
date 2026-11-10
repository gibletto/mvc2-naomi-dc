/* Stage 02's first object (kind 5 works; the stub that the stage's table names is the last routine
   of this module): a machine of nested parts, each riding on the one that made it by `base_mtx` and
   running through tables of key poses. Parts are named by their model (object 1 to 6 and 25 and 26
   of the stage's file); a part passes `unk_d4`/`unk_d8` between itself and the part it rides on. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* Kamui */
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *u, Float *v ) ;
extern void obj_uv_put( Float *u, Float *v ) ;
extern void set02_set( PLAYER_WORK *pl_w ) ;

/* a table of key poses is a run of these: the count (cnt), the frames each move takes (cnt - 1
   of them), then the pose each move starts from and ends at (cnt of them) */
typedef union SET02_00_KEY {
	Sint16	w ;
	Uint8	cnt ;
} SET02_00_KEY ;

/* part 5 and part 6, which rides on part 5 (part 5's turn is its set-up's, as is the second set's) */
static const VEC3 set02_00_p5_pos = { -6.616f, -1053.71f, 8.665f } ;
static const VEC3 set02_00_p6_pos[2] = { { 6.404f, 1094.598f, -789.669f }, { 6.404f, 1094.598f, -789.669f } } ;
static const VEC3 set02_00_p6_rot = { -19.4806f, 0.0f, 0.0f } ;
static const Float set02_00_p6_val[] = { 50.0f, 19.0f } ;
static const SET02_00_KEY set02_00_p6_key[] = {
	7, 600, 600, 600, 600, 600, 600, 1, 1, 0, 0, 0, 1, 1,
	7, 600, 600, 600, 600, 600, 600, 1, 1, 0, 0, 0, 1, 1,
	7, 300, 300, 300, 300, 300, 300, 1, 1, 0, 0, 0, 1, 1,
	7, 300, 300, 300, 300, 300, 300, 1, 1, 0, 0, 0, 1, 1,
	5, 400, 400, 400, 400, 1, 1, 2, 2, 1,
	7, 400, 400, 400, 400, 400, 400, 1, 1, 0, 0, 0, 1, 1,
} ;
/* part 1 */
static const VEC3 set02_00_p1_pos = { 0.0f, 193.767f, -970.999f } ;
static const VEC3 set02_00_p1_rot = { 0.0f, -38.0412f, 0.0f } ;
static const Float set02_00_p1_val[] = { -60.0f, -20.0f, 20.0f } ;
static const SET02_00_KEY set02_00_p1_key[] = {
	7, 600, 600, 600, 600, 600, 600, 2, 0, 0, 0, 0, 0, 2,
	7, 600, 600, 600, 600, 600, 600, 2, 0, 0, 0, 0, 0, 2,
	7, 300, 300, 300, 300, 300, 300, 2, 1, 1, 1, 1, 1, 2,
	7, 300, 300, 300, 300, 300, 300, 2, 1, 1, 1, 1, 1, 2,
	5, 400, 400, 400, 400, 2, 1, 1, 2, 2,
	7, 400, 400, 400, 400, 400, 400, 2, 1, 1, 0, 1, 2, 2,
} ;
/* part 3 */
static const VEC3 set02_00_p3_pos = { -1.5f, 128.513f, -116.196f } ;
static const VEC3 set02_00_p5_rot = { -19.4806f, 0.0f, 0.0f } ;
static const Float set02_00_p3_val[] = { -50.0f, -19.0f, 0.0f } ;
static const SET02_00_KEY set02_00_p3_key[] = {
	7, 600, 600, 600, 600, 600, 600, 1, 1, 0, 0, 0, 1, 1,
	7, 600, 600, 600, 600, 600, 600, 1, 1, 0, 0, 0, 1, 1,
	7, 300, 300, 300, 300, 300, 300, 1, 1, 0, 0, 0, 1, 1,
	7, 300, 300, 300, 300, 300, 300, 1, 1, 0, 0, 0, 1, 1,
	5, 400, 400, 400, 400, 1, 1, 2, 2, 1,
	7, 400, 400, 400, 400, 400, 400, 1, 1, 0, 0, 0, 1, 1,
} ;
/* part 2 and part 4, which share a table */
static const VEC3 set02_00_p2_pos = { 0.0f, -42.767f, 69.98f } ;
static const VEC3 set02_00_unused_p2_rot = { 0.0f, 0.0f, 0.0f } ;
static const VEC3 set02_00_p4_pos = { 127.394f, 1059.771f, -681.895f } ;
static const VEC3 set02_00_unused_p4_rot = { 0.0f, 0.0f, 0.0f } ;
static const Float set02_00_p24_val[] = { 0.0f, -1.0f, 1.0f } ;
static const SET02_00_KEY set02_00_p24_key[] = {
	7, 600, 600, 600, 600, 600, 600, 0, 0, 1, 2, 0, 0, 0,
	7, 600, 600, 600, 600, 600, 600, 0, 0, 1, 2, 0, 0, 0,
	7, 300, 300, 300, 300, 300, 300, 0, 0, 1, 2, 0, 0, 0,
	7, 300, 300, 300, 300, 300, 300, 0, 0, 1, 2, 0, 0, 0,
	5, 400, 400, 400, 400, 0, 0, 0, 0, 0,
	7, 400, 400, 400, 400, 400, 400, 0, 0, 0, 0, 0, 0, 0,
} ;

/* the second part, then the first: where each stands and how it is turned */
static const VEC3 set02_00_sway_pos = { 1.723f, 331.168f, 221.627f } ;
static const VEC3 set02_00_sway_rot = { 19.4858f, -1.2994f, -0.4597f } ;
static const VEC3 set02_00_bob_pos = { 5.996f, 1154.418f, 10.158f } ;
static const VEC3 set02_00_bob_rot = { 19.4806f, -0.003f, -0.0011f } ;
/* the poses, six runs of them (a pose is whether the part moves) */
static const SET02_00_KEY set02_00_key[] = {
	7, 600, 600, 600, 600, 600, 600, 0, 0, 1, 1, 0, 0, 0,
	7, 600, 600, 600, 600, 600, 600, 0, 0, 1, 1, 0, 0, 0,
	7, 300, 300, 300, 300, 300, 300, 0, 0, 1, 2, 0, 0, 0,
	7, 300, 300, 300, 300, 300, 300, 0, 0, 1, 2, 0, 0, 0,
	5, 400, 400, 400, 400, 0, 0, 0, 0, 0,
	7, 400, 400, 400, 400, 400, 400, 0, 0, 0, 0, 0, 0, 0,
} ;

static void set02_00_bob_init( PLAYER_WORK *ew ) ;
static void set02_00_bob_next( PLAYER_WORK *ew ) ;
static void set02_00_bob_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_bob_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_bob_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_bob_init, set02_00_bob_next, set02_00_bob_move,
	} ;

	set02_00_bob_jmp[ew->routine_1]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_00_bob_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	ew->routine_1 = 1 ;
	set02_00_bob_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_bob_next( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		k = set02_00_key ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->work2 = k->w ;
		ew->routine_1 = 2 ;
		set02_00_bob_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_bob_init( ew ) ;
	return ;
}

/*==============================================================*/
/*			It bobs while the move's pose is not 0				*/
/*==============================================================*/
static void set02_00_bob_move( PLAYER_WORK *ew )
{
	Float base_y ;

	ew->timer0 ++ ;
	if( ew->work2 ) ew->timer1 ++ ;
	if( ew->timer1 >= 360 ) ew->timer1 = 0 ;
	base_y = set02_00_bob_pos.y ;
	ew->pos.y = base_y + 10.0f * nlSin( DEG_ANG( ew->timer1 ) ) ;
	if( ew->timer0 > ew->work1 ){
		ew->timer0 = 0 ;
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/****************************************************************/
void set02_00_bob_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_bob_main ;
		ew->obj = obj_stg->obj[26] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set02_00_bob_pos ;
		ew->rot[0] = DEG_ANG( set02_00_bob_rot.x ) ;
		ew->rot[1] = DEG_ANG( set02_00_bob_rot.y ) ;
		ew->rot[2] = DEG_ANG( set02_00_bob_rot.z ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

static void set02_00_sway_init( PLAYER_WORK *ew ) ;
static void set02_00_sway_next( PLAYER_WORK *ew ) ;
static void set02_00_sway_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_sway_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_sway_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_sway_init, set02_00_sway_next, set02_00_sway_move,
	} ;

	set02_00_sway_jmp[ew->routine_1]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_00_sway_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	ew->routine_1 = 1 ;
	set02_00_sway_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_sway_next( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		k = set02_00_key ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->work2 = k->w ;
		ew->routine_1 = 2 ;
		set02_00_sway_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_sway_init( ew ) ;
	return ;
}

/*==============================================================*/
/*			It sways while the move's pose is not 0				*/
/*==============================================================*/
static void set02_00_sway_move( PLAYER_WORK *ew )
{
	Float base_x ;

	ew->timer0 ++ ;
	if( ew->work2 ) ew->timer1 ++ ;
	if( ew->timer1 >= 360 ) ew->timer1 = 0 ;
	base_x = set02_00_sway_pos.x ;
	ew->pos.x = base_x + 10.0f * nlSin( DEG_ANG( ew->timer1 ) ) ;
	if( ew->timer0 > ew->work1 ){
		ew->timer0 = 0 ;
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/****************************************************************/
void set02_00_sway_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_sway_main ;
		ew->obj = obj_stg->obj[25] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set02_00_sway_pos ;
		ew->rot[0] = DEG_ANG( set02_00_sway_rot.x ) ;
		ew->rot[1] = DEG_ANG( set02_00_sway_rot.y ) ;
		ew->rot[2] = DEG_ANG( set02_00_sway_rot.z ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/*==============================================================*/
/*			Part 5 (object 5) rides on part 6: while the		*/
/*			machine runs (unk_d8) it follows part 6's			*/
/*			matrix, and when the machine stops it takes its		*/
/*			place in the world									*/
/*==============================================================*/
static void set02_00_part5_main( PLAYER_WORK *ew )
{
	VEC3 org, pt ;

	if( ( ew->work3 = ew->ef_w->work3 ) ){
		switch( ew->work3 = ew->ef_w->work2 ){
		case 0:
			break ;
		case 1:
			ew->pos.y -- ;
			break ;
		case 2:
			ew->pos.y ++ ;
			break ;
		}
		if( !ew->base_mtx ){
			ew->base_mtx = &ew->ef_w->obj_mtx ;
			ew->pos = ew->spd ;
			ew->rot[1] = 0 ;
		}
		return ;
	}
	if( ew->base_mtx ){
		ew->base_mtx = ( MATRIX * )0 ;
		ew->spd = ew->pos ;
		nlPushMatrix( 0 ) ;
		org.x = 0.0f ;
		org.y = 0.0f ;
		org.z = 0.0f ;
		nlLoadMatrix( &ew->obj_mtx ) ;
		nlCalcPoint( &org, &pt ) ;
		ew->pos = pt ;
		nlPopMatrix( 1 ) ;
		ew->rot[1] = ew->ef_w->ef_w->ef_w->rot[1] ;
	}
}

/*==============================================================*/
static void set02_00_part5_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_part5_main ;
		ew->obj = obj_stg->obj[5] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set02_00_p5_pos ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->rot[0] = set02_00_p5_rot.x ;
		ew->rot[1] = set02_00_p5_rot.y ;
		ew->rot[2] = set02_00_p5_rot.z ;
		ew->work3 = ew->ef_w->work3 ;
	}
}

/* part 6 (object 6) turns about x, and slides its texture */
static void set02_00_part6_init( PLAYER_WORK *ew ) ;
static void set02_00_part6_next( PLAYER_WORK *ew ) ;
static void set02_00_part6_move( PLAYER_WORK *ew ) ;
static Uint8 set02_00_part6_turn( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_part6_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_part6_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_part6_init, set02_00_part6_next, set02_00_part6_move,
	} ;

	set02_00_part6_jmp[ew->routine_1]( ew ) ;
	ew->work3 = ew->ef_w->work3 ;
}

/*==============================================================*/
static void set02_00_part6_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_p6_key ;
	const Float *f = set02_00_p6_val ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	k += ew->routine_3 ;
	ew->rot[0] = DEG_ANG( f[k->w] ) ;
	ew->routine_1 ++ ;
	set02_00_part6_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part6_next( PLAYER_WORK *ew )
{
	const Float *f ;
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		k = set02_00_p6_key ;
		f = set02_00_p6_val ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->scale.x = f[k->w] ;
		k ++ ;
		ew->scale.y = ( f[k->w] - ew->scale.x ) / ew->work1 ;
		ew->routine_1 = 2 ;
		set02_00_part6_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_part6_init( ew ) ;
	return ;
}

/*==============================================================*/
/*			The model's texture slides as the machine runs		*/
/*			forward or back (unk_d4 1 or 2)						*/
/*==============================================================*/
static void set02_00_part6_move( PLAYER_WORK *ew )
{
	Float py, px ;
	Float unused[3] ;

	obj_uv_begin( obj_stg->obj[7], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += ew->timer1 * 0.02f ;
		obj_uv_put( &px, &py ) ;
	}
	switch( ew->work2 = ew->ef_w->work2 ){
	case 0:
		break ;
	case 1:
		ew->timer1 ++ ;
		ew->timer1 %= 50 ;
		break ;
	case 2:
		ew->timer1 -- ;
		if( ew->timer1 < 0 ) ew->timer1 = 49 ;
		break ;
	}
	ew->rot[0] = DEG_ANG( ew->scale.x ) ;
	if( set02_00_part6_turn( ew ) ){
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/*==============================================================*/
static Uint8 set02_00_part6_turn( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[0] += DEG_ANG( ew->scale.y * ew->timer0 ) ;
	if( ew->timer0 > ew->work1 ){
		ew->timer0 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void set02_00_part6_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_part6_main ;
		ew->obj = obj_stg->obj[6] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_00_p6_pos[0] ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->ef_w = pl_w ;
		ew->rot[0] = DEG_ANG( set02_00_p6_rot.x ) ;
		ew->rot[1] = DEG_ANG( set02_00_p6_rot.y ) ;
		ew->rot[2] = DEG_ANG( set02_00_p6_rot.z ) ;
		ew->work3 = ew->ef_w->work3 ;
		set02_00_part5_set( ew ) ;
	}
}

/* part 4 (object 4) turns about x, and tells the part it rides on what it is doing */
static void set02_00_part4_init( PLAYER_WORK *ew ) ;
static void set02_00_part4_next( PLAYER_WORK *ew ) ;
static void set02_00_part4_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_part4_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_part4_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_part4_init, set02_00_part4_next, set02_00_part4_move,
	} ;

	set02_00_part4_jmp[ew->routine_1]( ew ) ;
	ew->scale.z = ew->ef_w->scale.z ;
	ew->ef_w->work2 = ew->work2 ;
	ew->ef_w->work3 = ew->work3 ;
}

/*==============================================================*/
static void set02_00_part4_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_p24_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	ew->routine_1 = 1 ;
	set02_00_part4_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part4_next( PLAYER_WORK *ew )
{
	const Float *f ;
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		f = set02_00_p24_val ;
		k = set02_00_p24_key ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->work2 = k->w ;
		ew->scale.x = f[k->w] ;
		ew->routine_1 = 2 ;
		set02_00_part4_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_part4_init( ew ) ;
	return ;
}

/*==============================================================*/
/*			It turns by scale.x each frame; a backward move		*/
/*			flips unk_d8										*/
/*==============================================================*/
static void set02_00_part4_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[0] += DEG_ANG( ew->scale.x ) ;
	if( ew->timer0 > ew->work1 ){
		if( ew->scale.x == -1.0f ) ew->work3 ^= 1 ;
		ew->timer0 = 0 ;
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/*==============================================================*/
static void set02_00_part4_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_part4_main ;
		ew->obj = obj_stg->obj[4] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_00_p4_pos ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->work3 = 1 ;
		ew->scale.z = ew->ef_w->scale.z ;
		set02_set( ew ) ;
	}
}

/* part 2 (object 2) is part 4 without the flip, and sets nothing on the part it rides on */
static void set02_00_part2_init( PLAYER_WORK *ew ) ;
static void set02_00_part2_next( PLAYER_WORK *ew ) ;
static void set02_00_part2_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_part2_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_part2_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_part2_init, set02_00_part2_next, set02_00_part2_move,
	} ;

	set02_00_part2_jmp[ew->routine_1]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_00_part2_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_p24_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	ew->routine_1 = 1 ;
	set02_00_part2_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part2_next( PLAYER_WORK *ew )
{
	const Float *f ;
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		f = set02_00_p24_val ;
		k = set02_00_p24_key ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->work2 = k->w ;
		ew->scale.x = f[k->w] ;
		ew->routine_1 = 2 ;
		set02_00_part2_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_part2_init( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part2_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[0] += DEG_ANG( ew->scale.x ) ;
	if( ew->timer0 > ew->work1 ){
		ew->timer0 = 0 ;
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/*==============================================================*/
static void set02_00_part2_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_part2_main ;
		ew->obj = obj_stg->obj[2] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_00_p2_pos ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
	}
}

/* part 3 (object 3) swings about x between key angles; parts 4, 6, the bob and the sway ride on it */
static void set02_00_part3_init( PLAYER_WORK *ew ) ;
static void set02_00_part3_next( PLAYER_WORK *ew ) ;
static void set02_00_part3_move( PLAYER_WORK *ew ) ;
static Uint8 set02_00_part3_turn( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_part3_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_part3_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_part3_init, set02_00_part3_next, set02_00_part3_move,
	} ;

	set02_00_part3_jmp[ew->routine_1]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_00_part3_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_p3_key ;
	const Float *f = set02_00_p3_val ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	k += ew->routine_3 ;
	ew->rot[0] = DEG_ANG( f[k->w] ) ;
	ew->routine_1 = 1 ;
	set02_00_part3_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part3_next( PLAYER_WORK *ew )
{
	const Float *f ;
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		k = set02_00_p3_key ;
		f = set02_00_p3_val ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->scale.x = f[k->w] ;
		k ++ ;
		ew->scale.y = ( f[k->w] - ew->scale.x ) / ew->work1 ;
		ew->routine_1 = 2 ;
		set02_00_part3_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_part3_init( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part3_move( PLAYER_WORK *ew )
{
	ew->rot[0] = DEG_ANG( ew->scale.x ) ;
	if( set02_00_part3_turn( ew ) ){
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/*==============================================================*/
static Uint8 set02_00_part3_turn( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[0] += DEG_ANG( ew->scale.y * ew->timer0 ) ;
	if( ew->timer0 > ew->work1 ){
		ew->timer0 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void set02_00_part3_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_part3_main ;
		ew->obj = obj_stg->obj[3] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_00_p3_pos ;
		ew->rot[0] = DEG_ANG( set02_00_p5_rot.y ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		set02_00_part4_set( ew ) ;
		set02_00_sway_set( ew ) ;
		set02_00_bob_set( ew ) ;
		set02_00_part6_set( ew ) ;
	}
}

/* part 1 (object 1) swings about y; parts 2 and 3 ride on it */
static void set02_00_part1_init( PLAYER_WORK *ew ) ;
static void set02_00_part1_next( PLAYER_WORK *ew ) ;
static void set02_00_part1_move( PLAYER_WORK *ew ) ;
static Uint8 set02_00_part1_turn( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_00_part1_main( PLAYER_WORK *ew )
{
	static void ( *set02_00_part1_jmp[] )( PLAYER_WORK *ew ) = {
		set02_00_part1_init, set02_00_part1_next, set02_00_part1_move,
	} ;

	set02_00_part1_jmp[ew->routine_1]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_00_part1_init( PLAYER_WORK *ew )
{
	const SET02_00_KEY *k = set02_00_p1_key ;
	const Float *f = set02_00_p1_val ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	ew->routine_3 = k->cnt ;
	k += ew->routine_3 ;
	ew->rot[1] = DEG_ANG( f[k->w] ) ;
	ew->routine_1 = 1 ;
	set02_00_part1_next( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part1_next( PLAYER_WORK *ew )
{
	const Float *f ;
	const SET02_00_KEY *k ;
	int i ;

	if( ew->routine_2 < ew->routine_3 - 1 ){
		k = set02_00_p1_key ;
		f = set02_00_p1_val ;
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		k ++ ;
		ew->work1 = k[ew->routine_2].w ;
		k += ew->routine_3 - 1 ;
		k += ew->routine_2 ;
		ew->scale.x = f[k->w] ;
		k ++ ;
		ew->scale.y = ( f[k->w] - ew->scale.x ) / ew->work1 ;
		ew->routine_1 = 2 ;
		set02_00_part1_move( ew ) ;
		return ;
	}
	ew->routine_0 ++ ;
	ew->routine_0 %= 6 ;
	ew->routine_1 = 0 ;
	ew->routine_2 = 0 ;
	set02_00_part1_init( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_00_part1_move( PLAYER_WORK *ew )
{
	ew->rot[1] = DEG_ANG( ew->scale.x ) ;
	if( set02_00_part1_turn( ew ) ){
		ew->routine_1 = 1 ;
		ew->routine_2 ++ ;
	}
}

/*==============================================================*/
static Uint8 set02_00_part1_turn( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[1] += DEG_ANG( ew->scale.y * ew->timer0 ) ;
	if( ew->timer0 > ew->work1 ){
		ew->timer0 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void set02_00_part1_set( )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_00_part1_main ;
		ew->obj = obj_stg->obj[1] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_00_p1_pos ;
		ew->rot[1] = DEG_ANG( set02_00_p1_rot.y ) ;
		ew->type = 1 ;
		set02_00_part2_set( ew ) ;
		set02_00_part3_set( ew ) ;
	}
}

/****************************************************************/
void set02_00_set( void )
{
	set02_00_part1_set( 1 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_00.c" End of File	*/
