/* A model of the third attract demo that flies in and glows (kind 11 works):
   the demo's model 0 moves, turns and grows to its last place in 270 frames, brightens once the
   count passes 380, and makes the two glows that ride on its matrix (at03_ef4_glow0, _glow1). */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* seven keys of its place, its turn (in degrees) and its size, of which the first and last are used */
static const VEC3 at03_ef4_pos_tbl[7] = {
	{ 0.0f, -0.6f, -524.0f },
	{ 0.0f, 53.0f, -273.0f },
	{ 0.0f, 58.8f, -249.2f },
	{ 0.0f, 64.6f, -225.4f },
	{ 0.0f, 55.9f, -202.2f },
	{ 0.0f, 33.4f, -178.4f },
	{ 0.0f, -0.6f, -154.0f },
} ;
static const VEC3 at03_ef4_rot_tbl[7] = {
	{ -110.0f, 0.0f, 0.0f },
	{ -86.4f, 0.0f, 0.0f },
	{ -77.65f, 0.0f, 0.0f },
	{ -53.98f, 0.0f, 0.0f },
	{ -32.6f, 0.0f, 0.0f },
	{ -12.49f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
} ;
static const VEC3 at03_ef4_scale_tbl[7] = {
	{ 1.0f, 1.0f, 3.0f },
	{ 1.0f, 1.0f, 3.0f },
	{ 1.0f, 1.0f, 3.0f },
	{ 1.0f, 1.0f, 3.0f },
	{ 1.0f, 1.0f, 2.3f },
	{ 1.0f, 1.0f, 1.5f },
	{ 1.0f, 1.0f, 1.0f },
} ;

/* the turn on all three axes is the x key */
static void at03_ef4_main( PLAYER_WORK *ew ) ;
static void at03_ef4_glow0( PLAYER_WORK *pl_w ) ;
static void at03_ef4_glow1( PLAYER_WORK *pl_w ) ;

/****************************************************************/
void at03_ef4_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef4_main ;
		ew->obj = obj_dm07->obj[0] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
		ew->pos = at03_ef4_pos_tbl[0] ;
		ew->rot[0] = DEG_ANG( at03_ef4_rot_tbl[0].x ) ;
		ew->rot[1] = DEG_ANG( at03_ef4_rot_tbl[0].y ) ;
		ew->rot[2] = DEG_ANG( at03_ef4_rot_tbl[0].z ) ;
		ew->scale = at03_ef4_scale_tbl[0] ;
		ew->fade.r = 0.3f ;
		ew->fade.g = 0.3f ;
		ew->fade.b = 0.3f ;
		at03_ef4_glow0( ew ) ;
		at03_ef4_glow1( ew ) ;
	}
}

/*==============================================================*/
/* the turn on all three axes is the x key */
static void at03_ef4_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		ew->pos.x = at03_ef4_pos_tbl[0].x + ( ( at03_ef4_pos_tbl[6].x - at03_ef4_pos_tbl[0].x ) / 270.0f ) * ew->timer0 ;
		ew->pos.y = at03_ef4_pos_tbl[0].y + ( ( at03_ef4_pos_tbl[6].y - at03_ef4_pos_tbl[0].y ) / 270.0f ) * ew->timer0 ;
		ew->pos.z = at03_ef4_pos_tbl[0].z + ( ( at03_ef4_pos_tbl[6].z - at03_ef4_pos_tbl[0].z ) / 270.0f ) * ew->timer0 ;
		ew->rot[0] = DEG_ANG( at03_ef4_rot_tbl[0].x + ( ( at03_ef4_rot_tbl[6].x - at03_ef4_rot_tbl[0].x ) / 270.0f ) * ew->timer0 ) ;
		ew->rot[1] = DEG_ANG( at03_ef4_rot_tbl[0].x + ( ( at03_ef4_rot_tbl[6].x - at03_ef4_rot_tbl[0].x ) / 270.0f ) * ew->timer0 ) ;
		ew->rot[2] = DEG_ANG( at03_ef4_rot_tbl[0].x + ( ( at03_ef4_rot_tbl[6].x - at03_ef4_rot_tbl[0].x ) / 270.0f ) * ew->timer0 ) ;
		ew->scale.x = at03_ef4_scale_tbl[0].x + ( ( at03_ef4_scale_tbl[6].x - at03_ef4_scale_tbl[0].x ) / 270.0f ) * ew->timer0 ;
		ew->scale.y = at03_ef4_scale_tbl[0].y + ( ( at03_ef4_scale_tbl[6].y - at03_ef4_scale_tbl[0].y ) / 270.0f ) * ew->timer0 ;
		ew->scale.z = at03_ef4_scale_tbl[0].z + ( ( at03_ef4_scale_tbl[6].z - at03_ef4_scale_tbl[0].z ) / 270.0f ) * ew->timer0 ;
		if( ++ ew->timer0 >= 270 ){
			ew->routine_0 ++ ;
			ew->pos = at03_ef4_pos_tbl[6] ;
			ew->rot[0] = DEG_ANG( at03_ef4_rot_tbl[6].x ) ;
			ew->rot[1] = DEG_ANG( at03_ef4_rot_tbl[6].y ) ;
			ew->rot[2] = DEG_ANG( at03_ef4_rot_tbl[6].z ) ;
			ew->scale = at03_ef4_scale_tbl[6] ;
		}
		break ;
	case 1:
		break ;
	}
	switch( ew->routine_1 ){
	case 0:
		if( Sys->timer1 >= 380 ) ew->routine_1 ++ ;
		break ;
	case 1:
		ew->fade.r += 0.1f ;
		ew->fade.g += 0.1f ;
		ew->fade.b += 0.1f ;
		if( ew->fade.r >= 1.0f ){
			ew->routine_1 ++ ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 1.0f ;
			ew->fade.b = 1.0f ;
		}
		break ;
	case 2:
		break ;
	}
}

/* a glow on the model's matrix, which dims away once the count passes 400 */
static void at03_ef4_glow_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void at03_ef4_glow0( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef4_glow_main ;
		ew->obj = obj_dm07->obj[32] ;
		ew->obj_flag = OBJ_FADE | OBJ_BRIGHT | OBJ_ENV ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void at03_ef4_glow_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		if( Sys->timer1 >= 400 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->fade.r -= 0.01f ;
		ew->fade.g -= 0.01f ;
		ew->fade.b -= 0.01f ;
		if( ew->fade.r <= 0.0f ){
			ew->routine_0 ++ ;
			ew->fade.r = 0.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
		}
		break ;
	case 2:
		work_free( ew ) ;
		break ;
	}
}

static void at03_ef4_glow1_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void at03_ef4_glow1( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = at03_ef4_glow1_main ;
		ew->obj = obj_dm07->obj[33] ;
		ew->obj_flag = OBJ_FADE | OBJ_BRIGHT ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void at03_ef4_glow1_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0:
		if( Sys->timer1 >= 400 ) ew->routine_0 ++ ;
		break ;
	case 1:
		ew->fade.r -= 0.01f ;
		ew->fade.g -= 0.01f ;
		ew->fade.b -= 0.01f ;
		if( ew->fade.r <= 0.0f ){
			ew->routine_0 ++ ;
			ew->fade.r = 0.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
		}
		break ;
	case 2:
		work_free( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at03_ef4.c" End of File	*/
