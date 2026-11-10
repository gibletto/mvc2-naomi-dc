/* Stage 2's moving parts: three works of kind 5 that ride on the stage's object, two turning about
   y and z and one sliding up and down, each running through a table of key poses. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* the stage's models */

/* a table of key poses is a run of these: the count (cnt), the frames each move takes (cnt - 1
   of them), then the pose each move starts from and ends at (cnt of them), as numbers into a table of values */
typedef union SET02_KEY {
	Sint16	w ;
	Uint8	cnt ;
} SET02_KEY ;

/* where each work starts: spin_y 0, (1 not used), spin_z 2, rise 3 */
static const VEC3 set02_pos[] = {
	{ 0.663f, -0.846f, -0.309f },
	{ 10.769f, 0.4f, 2.865f },
	{ 395.22f, 299.156f, -10.514f },
	{ 2382.973f, 606.555f, -5371.06f },
} ;

static const Float set02_rise_val[] = { -2000.0f, 606.555f } ;
static const SET02_KEY set02_rise_key[] = {
	6, 600, 300, 300, 300, 600, 0, 1, 1, 1, 1, 0,
	4, 600, 300, 600, 0, 1, 1, 0,
	5, 600, 300, 300, 600, 0, 1, 1, 1, 0,
} ;
static const VEC3 set02_rise_tbl[] = {
	{ 2382.973f, 606.555f, -5371.06f },
	{ 1918.974f, 606.555f, -2657.06f },
} ;

static const Float set02_spin_y_val[] = { -120.0f, 0.0f, 100.0f } ;
static const SET02_KEY set02_spin_y_key[] = {
	6, 600, 300, 300, 300, 600, 0, 0, 1, 1, 1, 0,
	4, 600, 300, 600, 1, 1, 0, 0,
	5, 600, 300, 300, 600, 1, 1, 2, 2, 2,
} ;
static const VEC3 set02_spin_y_tbl[] = {
	{ 0.663f, -0.846f, -0.309f },
	{ 0.663f, -1.286f, -0.309f },
} ;

static const Float set02_spin_z_val[] = { 10.0f, 38.0f } ;
static const SET02_KEY set02_spin_z_key[] = {
	8, 600, 300, 100, 200, 100, 200, 600, 0, 0, 0, 1, 1, 0, 0, 0,
	4, 600, 300, 600, 0, 0, 1, 0,
	5, 600, 300, 300, 600, 0, 0, 0, 1, 1,
} ;
static const VEC3 set02_spin_z_tbl[] = {
	{ 395.22f, 299.156f, -10.514f },
	{ 395.22f, 299.155f, -10.514f },
} ;

static void set02_spin_z_main( PLAYER_WORK *ew );
static void set02_spin_z_init( PLAYER_WORK *ew );
static void set02_spin_z_move( PLAYER_WORK *ew );
static void set02_spin_z_turn( PLAYER_WORK *ew );
static Uint8 set02_spin_z_step( PLAYER_WORK *ew );
static void set02_wait( PLAYER_WORK *ew );

/*==============================================================*/
static void set02_spin_z_main( PLAYER_WORK *ew )
{
	static void ( *set02_spin_z_jmp[] )( PLAYER_WORK *ew ) = {
		set02_spin_z_init, set02_spin_z_move, set02_spin_z_turn, set02_wait,
	} ;

	set02_spin_z_jmp[ew->routine_2]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_spin_z_init( PLAYER_WORK *ew )
{
	const Float *f = set02_spin_z_val ;
	const VEC3 *v = set02_spin_z_tbl ;
	const SET02_KEY *k = set02_spin_z_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	v += ew->routine_3 ;
	ew->sub_type = k->cnt ;
	k += ew->sub_type ;
	f += k->w ;
	ew->rot[2] = DEG_ANG( *f ) ;
	ew->pos.x = v->x ;
	ew->pos.y = v->y ;
	ew->pos.z = v->z ;
	ew->routine_1 = 1 ;
	ew->routine_2 = 1 ;
	set02_spin_z_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_spin_z_move( PLAYER_WORK *ew )
{
	const Float *f = set02_spin_z_val ;
	const SET02_KEY *k = set02_spin_z_key ;
	int i ;

	if( ew->sub_type > ew->routine_1 ){
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		ew->work1 = k[ew->routine_1].w ;
		k += ew->sub_type ;
		k += ew->routine_1 ;
		ew->acc.z = f[k[-1].w] ;
		f += k->w ;
		ew->spd.z = ( *f - ew->acc.z ) / ew->work1 ;
		ew->routine_2 = 2 ;
		set02_spin_z_turn( ew ) ;
	} else {
		ew->routine_1 = 0 ;
		ew->routine_2 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_spin_z_turn( PLAYER_WORK *ew )
{
	ew->rot[2] = DEG_ANG( ew->acc.z ) ;
	if( set02_spin_z_step( ew ) ){
		ew->routine_1 ++ ;
		ew->routine_2 = 1 ;
	}
}

/*==============================================================*/
static Uint8 set02_spin_z_step( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[2] += DEG_ANG( ew->spd.z * ew->timer0 ) ;
	if( ew->work1 <= ew->timer0 ){
		ew->timer0 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			After the last move, rest 300 frames and start		*/
/*			the next table of poses								*/
/*==============================================================*/
static void set02_wait( PLAYER_WORK *ew )
{
	if( ew->timer1 ++ >= 300 ){
		ew->timer1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_0 ++ ;
		ew->routine_0 %= 3 ;
		ew->routine_3 ++ ;
		ew->routine_3 &= 1 ;
	}
}

/****************************************************************/
void set02_spin_z_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_spin_z_main ;
		ew->obj = obj_stg->obj[23] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_BRIGHT ;
		ew->pos = set02_pos[2] ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->routine_0 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
	}
}

static void set02_spin_y_main( PLAYER_WORK *ew );
static void set02_spin_y_init( PLAYER_WORK *ew );
static void set02_spin_y_move( PLAYER_WORK *ew );
static void set02_spin_y_turn( PLAYER_WORK *ew );
static Uint8 set02_spin_y_step( PLAYER_WORK *ew );

/*==============================================================*/
static void set02_spin_y_main( PLAYER_WORK *ew )
{
	static void ( *set02_spin_y_jmp[] )( PLAYER_WORK *ew ) = {
		set02_spin_y_init, set02_spin_y_move, set02_spin_y_turn, set02_wait,
	} ;

	set02_spin_y_jmp[ew->routine_2]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_spin_y_init( PLAYER_WORK *ew )
{
	const Float *f = set02_spin_y_val ;
	const VEC3 *v = set02_spin_y_tbl ;
	const SET02_KEY *k = set02_spin_y_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	v += ew->routine_3 ;
	ew->sub_type = k->cnt ;
	k += ew->sub_type ;
	f += k->w ;
	ew->rot[1] = DEG_ANG( *f ) ;
	ew->pos.x = v->x ;
	ew->pos.y = v->y ;
	ew->pos.z = v->z ;
	ew->routine_1 = 1 ;
	ew->routine_2 = 1 ;
	set02_spin_y_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_spin_y_move( PLAYER_WORK *ew )
{
	const Float *f = set02_spin_y_val ;
	const SET02_KEY *k = set02_spin_y_key ;
	int i ;

	if( ew->routine_1 < ew->sub_type ){
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		ew->work1 = k[ew->routine_1].w ;
		k += ew->sub_type ;
		k += ew->routine_1 ;
		ew->acc.y = f[k[-1].w] ;
		f += k->w ;
		ew->spd.y = ( *f - ew->acc.y ) / ew->work1 ;
		ew->routine_2 = 2 ;
		set02_spin_y_turn( ew ) ;
	} else {
		ew->routine_1 = 0 ;
		ew->routine_2 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_spin_y_turn( PLAYER_WORK *ew )
{
	ew->rot[1] = DEG_ANG( ew->acc.y ) ;
	if( set02_spin_y_step( ew ) ){
		ew->routine_1 ++ ;
		ew->routine_2 = 1 ;
	}
}

/*==============================================================*/
static Uint8 set02_spin_y_step( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[1] += DEG_ANG( ew->spd.y * ew->timer0 ) ;
	if( ew->work1 <= ew->timer0 ){
		ew->timer0 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/****************************************************************/
void set02_spin_y_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_spin_y_main ;
		ew->obj = obj_stg->obj[21] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_pos[0] ;
		ew->base_mtx = &pl_w->obj_mtx ;
		set02_spin_z_set( ew ) ;
	}
}

static void set02_rise_main( PLAYER_WORK *ew );
static void set02_rise_init( PLAYER_WORK *ew );
static void set02_rise_move( PLAYER_WORK *ew );
static void set02_rise_slide( PLAYER_WORK *ew );
static Uint8 set02_rise_step( PLAYER_WORK *ew );
static void set02_rise_wait( PLAYER_WORK *ew );

/*==============================================================*/
static void set02_rise_main( PLAYER_WORK *ew )
{
	static void ( *set02_rise_jmp[] )( PLAYER_WORK *ew ) = {
		set02_rise_init, set02_rise_move, set02_rise_slide, set02_rise_wait,
	} ;

	set02_rise_jmp[ew->routine_2]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_rise_init( PLAYER_WORK *ew )
{
	const Float *f = set02_rise_val ;
	const VEC3 *v = set02_rise_tbl ;
	const SET02_KEY *k = set02_rise_key ;
	int i ;

	for( i = 0 ; i < ew->routine_0 ; i ++ ){
		k += k->w * 2 ;
	}
	v += ew->routine_3 ;
	ew->pos.x = v->x ;
	ew->pos.z = v->z ;
	ew->sub_type = k->cnt ;
	k += ew->sub_type ;
	f += k->w ;
	ew->pos.y = *f ;
	ew->routine_1 = 1 ;
	ew->routine_2 = 1 ;
	set02_rise_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_rise_move( PLAYER_WORK *ew )
{
	const Float *f = set02_rise_val ;
	const SET02_KEY *k = set02_rise_key ;
	int i ;

	if( ew->routine_1 < ew->sub_type ){
		for( i = 0 ; i < ew->routine_0 ; i ++ ){
			k += k->w * 2 ;
		}
		ew->work1 = k[ew->routine_1].w ;
		k += ew->sub_type ;
		k += ew->routine_1 ;
		ew->acc.y = f[k[-1].w] ;
		f += k->w ;
		ew->spd.y = ( *f - ew->acc.y ) / ew->work1 ;
		ew->routine_2 = 2 ;
		set02_rise_slide( ew ) ;
	} else {
		ew->routine_1 = 0 ;
		ew->routine_2 = 3 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_rise_slide( PLAYER_WORK *ew )
{
	if( set02_rise_step( ew ) ){
		ew->routine_1 ++ ;
		ew->routine_2 = 1 ;
	}
}

/*==============================================================*/
static Uint8 set02_rise_step( PLAYER_WORK *ew )
{
	ew->timer1 ++ ;
	ew->pos.y += ew->spd.y ;
	if( ew->work1 <= ew->timer1 ){
		ew->timer1 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The stage's object (unk_d8) can send the work		*/
/*			back to the first position							*/
/*==============================================================*/
static void set02_rise_wait( PLAYER_WORK *ew )
{
	if( ew->timer0 ++ >= 300 ){
		ew->timer0 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_0 ++ ;
		ew->routine_0 %= 3 ;
		ew->routine_3 ++ ;
		ew->routine_3 &= 1 ;
		if( ew->master_w->work3 ) ew->routine_3 = 0 ;
	}
}

/****************************************************************/
void set02_rise_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_rise_main ;
		ew->obj = obj_stg->obj[24] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->pos = set02_pos[3] ;
		ew->master_w = pl_w ;
		set02_spin_y_set( ew ) ;
	}
}

/****************************************************************/
void set02_set( PLAYER_WORK *pl_w )
{
	set02_rise_set( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "s_set02.c" End of File	*/
