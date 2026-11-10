/* The sparks of a guard (kind 7 works): a flash that fades, a ring that swells and a shower of
   chips that fly off to the side the guard faces. */
#include "player.h"

/* a vector the code does not use; each effect's turn and the chips' speed, by the side faced */
static VEC3 guard_unit = { 1.0f, 1.0f, 1.0f } ;
static Sint32 guard_rot_tbl[2] = { 0xa000, 0xe001 } ;
static VEC3 guard_spd_tbl[2] = {
	{ 0.35350001f, 0.0f, -0.35350001f }, { -0.35350001f, 0.0f, -0.35350001f },
} ;

/* the flash's fade, the ring's size (its other table is not used) and fade, the chips' size and fade */
static MOTION guard_flash_fade_key[] = {
	{ 0, 1.0f }, { 6, 0.0f },
} ;
static MOTION guard_ring_size_key[] = {
	{ 0, 1.0f }, { 12, 2.5f },
} ;
static MOTION guard_ring_ofs_key[] = {
	{ 0, 0.0f }, { 20, -100.0f },
} ;
static MOTION guard_ring_fade_key[] = {
	{ 0, 0.0f }, { 3, 1.0f }, { 12, 0.0f },
} ;
static MOTION guard_chip_size_key[] = {
	{ 0, 1.0f }, { 20, 2.5f },
} ;
static MOTION guard_chip_ofs_key[] = {
	{ 0, 0.0f }, { 20, -100.0f },
} ;
static MOTION guard_chip_fade_key[] = {
	{ 0, 0.0f }, { 3, 1.0f }, { 12, 0.0f },
} ;

/*==============================================================*/
static void guard_flash_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 6 ){
		work_free( ew ) ;
		return ;
	}
	ew->fade.r = get_motion( guard_flash_fade_key, &ew->routine_0, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_flash_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_flash_main ;
		ew->obj = obj_efky->obj[32] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_rot_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/*==============================================================*/
static void guard_ring_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 12 ){
		work_free( ew ) ;
		return ;
	}
	ew->scale.x = 1.0f ;
	ew->scale.y = get_motion( guard_ring_size_key, &ew->routine_0, ew->timer0 ) ;
	ew->scale.z = ew->scale.y ;
	ew->fade.r = get_motion( guard_ring_fade_key, &ew->routine_2, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_ring_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_ring_main ;
		ew->obj = obj_efky->obj[33] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_rot_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/*==============================================================*/
static void guard_chip_main( PLAYER_WORK *ew )
{
	if( ew->timer0 >= 12 ){
		work_free( ew ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->pos.z += ew->spd.z ;
	ew->scale.x = get_motion( guard_chip_size_key, &ew->routine_0, ew->timer0 ) ;
	ew->scale.y = ew->scale.x ;
	ew->scale.z = ew->scale.x ;
	ew->fade.r = get_motion( guard_chip_fade_key, &ew->routine_2, ew->timer0 ) ;
	ew->fade.g = ew->fade.r ;
	ew->fade.b = ew->fade.r ;
	ew->timer0 ++ ;
}

/*==============================================================*/
static void guard_chip_set( VEC3 *pos, int dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = guard_chip_main ;
		ew->obj = obj_efky->obj[40] ;
		ew->pos = *pos ;
		ew->rot[1] = guard_rot_tbl[dir] ;
		ew->spd = guard_spd_tbl[dir] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FACE_Y | OBJ_FADE ;
	}
}

/****************************************************************/
void guard_mark_ef_set( VEC3 *pos, int dir )
{
	guard_flash_set( pos, dir ) ;
	guard_ring_set( pos, dir ) ;
	guard_chip_set( pos, dir ) ;
}

/****************************************************************/
void guard_mark_ef_set2( VEC3 *pos, int dir )
{
	guard_flash_set( pos, dir ) ;
	guard_ring_set( pos, dir ) ;
	guard_chip_set( pos, dir ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "guard_ef.c" End of File	*/
