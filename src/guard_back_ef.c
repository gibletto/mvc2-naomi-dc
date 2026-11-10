/* The waves of a guard that pushes the attacker back (kind 7 works, set by act 18 at the guard
   mark): two flashes that fly off sideways and two that spin and fade. */
#include "player.h"

/* the size of the second wave; each effect's turn and the two waves' speed, by the side faced */
static VEC3 guard_back_scale = { 1.0f, 1.0f, 1.0f } ;
static Sint32 guard_back_rot_tbl[2] = { 0x9555, 0xeaac } ;
static VEC3 guard_back_spd_tbl[2] = {
	{ -4.33f, 0.0f, 2.5f }, { 4.33f, 0.0f, 2.5f },
} ;

/* each effect's size or turn, its fade, and (a table the code does not use) its offset */
static MOTION guard_back_a_size_key[] = {
	{ 0, 1.0f }, { 20, 0.0f },
} ;
static MOTION guard_back_a_ofs_key[] = {
	{ 0, 0.0f }, { 20, 100.000000f },
} ;
static MOTION guard_back_a_fade_key[] = {
	{ 0, 1.0f }, { 8, 1.0f }, { 13, 0.0f },
} ;
static MOTION guard_back_b_size_key[] = {
	{ 0, 0.0f }, { 8, 2.0f }, { 20, 8.0f },
} ;
static MOTION guard_back_b_ofs_key[] = {
	{ 0, 0.0f }, { 20, 200.000000f },
} ;
static MOTION guard_back_b_fade_key[] = {
	{ 0, 1.0f }, { 8, 1.0f }, { 13, 0.0f },
} ;
static MOTION guard_back_c_turn_key[] = {
	{ 0, 0.0f }, { 20, 240.000000f },
} ;
static MOTION guard_back_c_fade_key[] = {
	{ 0, 1.0f }, { 8, 1.0f }, { 13, 0.0f },
} ;
static MOTION guard_back_d_turn_key[] = {
	{ 0, 0.0f }, { 20, -240.000000f },
} ;
static MOTION guard_back_d_fade_key[] = {
	{ 0, 1.0f }, { 8, 1.0f }, { 13, 0.0f },
} ;

/*==============================================================*/
static void guard_back_a_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 13 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = get_motion( guard_back_a_size_key, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->pos.x += ew->spd.x ;
	ew->pos.z += ew->spd.z ;
	ew->fade.r = get_motion( guard_back_a_fade_key, &ew->routine_2, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_back_a_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_back_a_main ;
		ew->obj = obj_efky->obj[12] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_back_rot_tbl[dir] ;
		ew->spd = guard_back_spd_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/*==============================================================*/
static void guard_back_b_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 13 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = 1.0f ;
	ew->scale.y = get_motion( guard_back_b_size_key, &ew->routine_0, ew->timer0 ) ;
	ew->scale.z = ew->scale.y ;
	ew->pos.x += ew->spd.x ;
	ew->pos.z += ew->spd.z ;
	ew->fade.r = get_motion( guard_back_b_fade_key, &ew->routine_2, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_back_b_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_back_b_main ;
		ew->obj = obj_efky->obj[13] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_back_rot_tbl[dir] ;
		ew->scale = guard_back_scale ;
		ew->spd = guard_back_spd_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/*==============================================================*/
static void guard_back_c_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 13 ){
		work_free( ew ) ;
		return ;
	}
	ew->rot[0] = ( int )( get_motion( guard_back_c_turn_key, &ew->routine_0, ew->timer0 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->fade.r = get_motion( guard_back_c_fade_key, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_back_c_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_back_c_main ;
		ew->obj = obj_efky->obj[14] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_back_rot_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/*==============================================================*/
static void guard_back_d_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 13 ){
		work_free( ew ) ;
		return ;
	}
	ew->rot[0] = ( int )( get_motion( guard_back_d_turn_key, &ew->routine_0, ew->timer0 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->fade.r = get_motion( guard_back_d_fade_key, &ew->routine_1, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_back_d_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_back_d_main ;
		ew->obj = obj_efky->obj[15] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_back_rot_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/****************************************************************/
void guard_wave_set( VEC3 *pos, int dir )
{
	guard_back_a_set( pos, dir ) ;
	guard_back_b_set( pos, dir ) ;
	guard_back_c_set( pos, dir ) ;
	guard_back_d_set( pos, dir ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "guard_back_ef.c" End of File	*/
