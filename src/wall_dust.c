/* The dust where a fighter is thrown against the screen's edge (kind 7 works):
   a puff that stands at the wall and dims, and a flat piece that steps through seven pictures. */
#include "player.h"

static void wall_dust_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			The puff at the wall the player is against,			*/
/*			level with his middle								*/
/*==============================================================*/
static void wall_dust_a_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = wall_dust_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->obj = obj_efky->obj[85] ;
		switch( pl_w->wall_flag ){
		case 1:
			ew->pos.x = scr_work.scr_right ;
			break ;
		case 2:
			ew->pos.x = scr_work.scr_left ;
			break ;
		}
		ew->pos.y = pl_w->pos.y + ( pl_w->size_top / 2 ) * pl_w->scale.y * 2.142857f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

static void wall_dust_wait( PLAYER_WORK *ew ) ;
static void wall_dust_dim( PLAYER_WORK *ew ) ;
static void wall_dust_hide( PLAYER_WORK *ew ) ;
static void wall_dust_end( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void wall_dust_main( PLAYER_WORK *ew )
{
	static void ( *wall_dust_jmp[] )( PLAYER_WORK *ew ) = {
		wall_dust_wait, wall_dust_dim, wall_dust_hide, wall_dust_end,
	} ;

	wall_dust_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void wall_dust_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	wall_dust_dim( ew ) ;
}

/*==============================================================*/
/*			It dims a sixth a frame for seven frames			*/
/*==============================================================*/
static void wall_dust_dim( PLAYER_WORK *ew )
{
	ew->fade.r -= 0.16666667f ;
	ew->fade.g -= 0.16666667f ;
	ew->fade.b -= 0.16666667f ;
	if( ++ ew->routine_1 > 6 ) ew->routine_0 ++ ;
}

static void wall_dust_star_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			The flat piece that follows, by the side of the		*/
/*			wall												*/
/*==============================================================*/
static void wall_dust_b_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = wall_dust_star_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_FADE ;
		switch( pl_w->wall_flag ){
		case 1:
			ew->pos.x = scr_work.scr_right ;
			break ;
		case 2:
			ew->pos.x = scr_work.scr_left ;
			break ;
		}
		ew->pos.y = pl_w->pos.y + ( pl_w->size_top / 2 ) * pl_w->scale.y * 2.142857f ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
		ew->rot[1] = pl_w->disp_dir ? 0x4000 : 0xc001 ;
		ew->work1 = 3 ;
	}
}

static void wall_dust_star_wait( PLAYER_WORK *ew ) ;
static void wall_dust_star_step( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void wall_dust_star_main( PLAYER_WORK *ew )
{
	static void ( *wall_dust2_jmp[] )( PLAYER_WORK *ew ) = {
		wall_dust_star_wait, wall_dust_star_step, wall_dust_hide, wall_dust_end,
	} ;

	wall_dust2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void wall_dust_star_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->routine_0 ++ ;
	wall_dust_star_step( ew ) ;
}

/*==============================================================*/
/*			A picture of the sheet a frame, each with its		*/
/*			alpha												*/
/*==============================================================*/
static void wall_dust_star_step( PLAYER_WORK *ew )
{
	const Float *fade ;
	/* the last step reads past the table: the first colour of stage.c's stage_back_tbl, 0x7F7F7F,
	   as a float (about 1e-38) */
	static const Float wall_dust2_fade_tbl[6] = { 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.2f, } ;

	fade = &wall_dust2_fade_tbl[ew->routine_1] ;
	ew->obj = obj_efky->obj[ew->routine_1 + 88] ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	if( ++ ew->routine_1 > 6 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void wall_dust_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void wall_dust_end( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/****************************************************************/
/*			The dust at a wall hit: both pieces, and the		*/
/*			side noted for the pair								*/
/****************************************************************/
void wall_dust_set( PLAYER_WORK *pl_w )
{
	wall_dust_a_set( pl_w ) ;
	wall_dust_b_set( pl_w ) ;
	if( pl_w->fly_screen_flag && pl_w->wall_flag ) vs_sys_w->wall_flag = pl_w->wall_flag ;
}

/*==============================================================*/
/*			These two are the same for a place and not for a	*/
/*			player (nothing calls them)							*/
/*==============================================================*/
static void wall_dust_c_set( VEC3 *pos, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = wall_dust_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		ew->obj = obj_efky->obj[85] ;
		ew->pos = *pos ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
	}
}

/*==============================================================*/
static void wall_dust_d_set( VEC3 *pos, Uint8 dir )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = wall_dust_star_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_FADE ;
		ew->pos = *pos ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
		ew->rot[1] = dir ? 0x4000 : 0xc001 ;
		ew->work1 = 3 ;
	}
}

/****************************************************************/
void wall_dust_pos_set( VEC3 *pos, Uint8 dir )
{
	wall_dust_c_set( pos, dir ) ;
	wall_dust_d_set( pos, dir ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "wall_dust.c" End of File	*/
