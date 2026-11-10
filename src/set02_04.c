/* Stage 02's object 4: two models (objects 39 and 40 of the stage's file) that swing between two
   sets of angles, each with a second model riding on it. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* where each stands, and the two sets of angles it swings between */
static const VEC3 set02_04_pos_tbl[2] = {
	{ -857.164f, -697.184f, -4949.01f },
	{ -455.164f, -697.184f, -5492.01f },
} ;
static const VEC3 set02_04_rot_tbl[2] = {
	{ 0.0f, 51.0f, 0.0f },
	{ -53.0f, 51.0f, 0.0f },
} ;
static const VEC3 set02_04_sub_pos = { 12.823f, -22.467f, 1533.9f } ;
static const VEC3 set02_04_sub_rot_tbl[2] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 53.0f, 0.0f, 0.0f },
} ;

/* how far each swings in a frame, on each axis */
static VEC3 set02_04_step[2] ;
static VEC3 set02_04_sub_step[2] ;

static void set02_04_init( PLAYER_WORK *ew ) ;
static void set02_04_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_04_main( PLAYER_WORK *ew )
{
	static void ( *set02_04_jmp[] )( PLAYER_WORK *ew ) = {
		set02_04_init, set02_04_move,
	} ;

	set02_04_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_04_init( PLAYER_WORK *ew )
{
	const VEC3 *tbl = set02_04_rot_tbl ;

	ew->spd = tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = tbl[ew->routine_1] ;
	set02_04_step[ew->sub_no].x = ( ew->acc.x - ew->spd.x ) / 100.0f ;
	set02_04_step[ew->sub_no].y = ( ew->acc.y - ew->spd.y ) / 100.0f ;
	set02_04_step[ew->sub_no].z = ( ew->acc.z - ew->spd.z ) / 100.0f ;
	ew->routine_0 = 1 ;
	set02_04_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_04_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 100 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_04_step[ew->sub_no].x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_04_step[ew->sub_no].y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_04_step[ew->sub_no].z * ew->timer0 ) ;
}

static void set02_04_sub_set( PLAYER_WORK *pl_w ) ;
static void set02_04_sub_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_04_make()
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 2 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
		ew->disp_flag = 1 ;
		ew->work_func = set02_04_main ;
		ew->obj = obj_stg->obj[39] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_04_pos_tbl[i] ;
		ew->rot[0] = DEG_ANG( set02_04_rot_tbl[i].x ) ;
		ew->rot[1] = DEG_ANG( set02_04_rot_tbl[i].y ) ;
		ew->rot[2] = DEG_ANG( set02_04_rot_tbl[i].z ) ;
		ew->sub_no = i ;
		ew->routine_1 = i ;
		set02_04_sub_set( ew ) ;
	}
}

static void set02_04_sub_init( PLAYER_WORK *ew ) ;
static void set02_04_sub_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_04_sub_main( PLAYER_WORK *ew )
{
	static void ( *set02_04_sub_jmp[] )( PLAYER_WORK *ew ) = {
		set02_04_sub_init, set02_04_sub_move,
	} ;

	set02_04_sub_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_04_sub_init( PLAYER_WORK *ew )
{
	const VEC3 *tbl = set02_04_sub_rot_tbl ;

	ew->spd = tbl[ew->routine_1] ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc = tbl[ew->routine_1] ;
	set02_04_sub_step[ew->sub_no].x = ( ew->acc.x - ew->spd.x ) / 100.0f ;
	set02_04_sub_step[ew->sub_no].y = ( ew->acc.y - ew->spd.y ) / 100.0f ;
	set02_04_sub_step[ew->sub_no].z = ( ew->acc.z - ew->spd.z ) / 100.0f ;
	ew->routine_0 = 1 ;
	set02_04_sub_move( ew ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void set02_04_sub_move( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 100 ){
		ew->routine_0 = 0 ;
		ew->timer0 = 0 ;
		return ;
	}
	ew->rot[0] = DEG_ANG( ew->spd.x ) ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[2] = DEG_ANG( ew->spd.z ) ;
	ew->rot[0] += DEG_ANG( set02_04_sub_step[ew->sub_no].x * ew->timer0 ) ;
	ew->rot[1] += DEG_ANG( set02_04_sub_step[ew->sub_no].y * ew->timer0 ) ;
	ew->rot[2] += DEG_ANG( set02_04_sub_step[ew->sub_no].z * ew->timer0 ) ;
}

/*==============================================================*/
static void set02_04_sub_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_04_sub_main ;
		ew->obj = obj_stg->obj[40] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_BRIGHT ;
		ew->pos = set02_04_sub_pos ;
		ew->rot[0] = DEG_ANG( set02_04_sub_rot_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( set02_04_sub_rot_tbl[0].y ) ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->routine_1 = pl_w->routine_1 ;
		ew->sub_no = pl_w->sub_no ;
	}
}

/****************************************************************/
void set02_04_set( void )
{
	set02_04_make( 39 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_04.c" End of File	*/
