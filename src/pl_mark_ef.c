/* The marks that follow the six players of a fight (kind 7 works): a mark
   under the point character of each side with a second that turns to the side he faces, and a
   mark over each partner while he fights in a team move. pl_init makes them all. */
#include "player.h"

extern int scr_bottom_ck( PLAYER_WORK *pl_w ) ;
extern void obj_func_set( PLAYER_WORK *w, void ( *func )( PLAYER_WORK *w ) ) ;

#define MARK_ANG( t )	( ( int )( ( t ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

static void mark_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void mark_a_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void mark_a_main( PLAYER_WORK *ew )
{
	static void ( *mark_a_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		mark_a_wait, mark_a_turn,
	} ;
	PLAYER_WORK *pl_w = vs_sys_work.team_w[0][0] ;	/* each side's point character, for the turning mark */

	ew->disp_flag = 0 ;
	if( !ew->ef_w->disp_flag ){
		ew->timer0 = pl_w->disp_dir ? 0 : -180 ;
		ew->rot[2] = MARK_ANG( ew->timer0 ) ;
		return ;
	}
	mark_a_jmp[ew->routine_0]( ew, pl_w ) ;
}

static Uint8 mark_dir_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void mark_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	if( mark_dir_ck( ew, pl_w ) ){
		ew->routine_0 ++ ;
		mark_a_turn( ew, pl_w ) ;
		return ;
	}
	ew->timer0 = pl_w->disp_dir ? 0 : -180 ;
	ew->rot[2] = MARK_ANG( ew->timer0 ) ;
}

/*--------------------------------------------------------------*/
static void mark_a_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float step ;

	ew->disp_flag = 1 ;
	mark_dir_ck( ew, pl_w ) ;
	step = 10.0f ;
	if( ew->routine_1 ){
		ew->timer0 += step ;
		if( ew->timer0 >= 0 ){
			ew->timer0 = 0 ;
			ew->routine_0 = 0 ;
		}
	} else {
		ew->timer0 -= step ;
		if( ew->timer0 <= -180 ){
			ew->timer0 = -180 ;
			ew->routine_0 = 0 ;
		}
	}
	ew->rot[2] = MARK_ANG( ew->timer0 ) ;
}

/*==============================================================*/
/*			Whether the player turned since the last frame:		*/
/*			routine_1 keeps the side he faces					*/
/*==============================================================*/
static Uint8 mark_dir_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->disp_dir != ew->routine_3 ){
		ew->routine_1 = pl_w->disp_dir ;
		ew->routine_3 = pl_w->disp_dir ;
		return 1 ;
	}
	ew->routine_3 = pl_w->disp_dir ;
	return 0 ;
}

/*==============================================================*/
static void mark_a_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_a_main ;
		ew->obj = obj_efky->obj[16] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/*==============================================================*/
static void mark_point_a_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = vs_sys_work.team_w[0][0] ;

	ew->disp_flag = 0 ;
	if( vs_sys_work.vs_st > VS_READY ){
		if( !scr_bottom_ck( pl_w ) ) ew->disp_flag = 1 ;
	}
	ew->pos = pl_w->pos ;
	ew->pos.y = scr_work.eye[0].y + -50.0f ;
	ew->pos.z += 4.0f ;
}

/*==============================================================*/
static void mark_point_a_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_point_a_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE ;
		ew->obj = obj_efky->obj[17] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		mark_a_set( ew ) ;
	}
}

static void mark_b_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void mark_b_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void mark_b_main( PLAYER_WORK *ew )
{
	static void ( *mark_b_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		mark_b_wait, mark_b_turn,
	} ;
	PLAYER_WORK *pl_w = vs_sys_work.team_w[1][0] ;

	ew->disp_flag = 0 ;
	if( !ew->ef_w->disp_flag ){
		ew->timer0 = pl_w->disp_dir ? 0 : -180 ;
		ew->rot[2] = MARK_ANG( ew->timer0 ) ;
		return ;
	}
	mark_b_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void mark_b_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	if( mark_dir_ck( ew, pl_w ) ){
		ew->routine_0 ++ ;
		mark_b_turn( ew, pl_w ) ;
		return ;
	}
	ew->timer0 = pl_w->disp_dir ? 0 : -180 ;
	ew->rot[2] = MARK_ANG( ew->timer0 ) ;
}

/*--------------------------------------------------------------*/
static void mark_b_turn( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float step ;

	ew->disp_flag = 1 ;
	mark_dir_ck( ew, pl_w ) ;
	step = 10.0f ;
	if( ew->routine_1 ){
		ew->timer0 += step ;
		if( ew->timer0 >= 0 ){
			ew->timer0 = 0 ;
			ew->routine_0 = 0 ;
		}
	} else {
		ew->timer0 -= step ;
		if( ew->timer0 <= -180 ){
			ew->timer0 = -180 ;
			ew->routine_0 = 0 ;
		}
	}
	ew->rot[2] = MARK_ANG( ew->timer0 ) ;
}

/*==============================================================*/
static void mark_b_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_b_main ;
		ew->obj = obj_efky->obj[18] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE ;
		ew->ef_w = pl_w ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/*==============================================================*/
static void mark_point_b_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = vs_sys_work.team_w[1][0] ;

	ew->disp_flag = 0 ;
	if( vs_sys_work.vs_st > VS_READY ){
		if( !scr_bottom_ck( pl_w ) ) ew->disp_flag = 1 ;
	}
	ew->pos = pl_w->pos ;
	ew->pos.y = scr_work.eye[0].y + -50.0f ;
	ew->pos.z += 5.0f ;
}

/*==============================================================*/
static void mark_point_b_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_point_b_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE ;
		ew->obj = obj_efky->obj[19] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
		mark_b_set( ew ) ;
	}
}

/*==============================================================*/
static void mark_partner_b1_main( PLAYER_WORK *ew )
{
	VS_SYS_WORK *vs_w = &vs_sys_work ;
	PLAYER_WORK *pl_w = vs_w->team_w[1][1] ;

	ew->disp_flag = 0 ;
	if( pl_w->be_flag && pl_w->team_type == 8 && vs_w->vs_st == VS_FIGHT && pl_w->disp_flag ) ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	ew->pos.y += pl_w->size_top * pl_w->scale.y * DOT_Y( 1.0f ) ;
	ew->pos.y += 100.0f ;
	ew->pos.z += 1.0f ;
	if( ew->pos.y <= scr_work.scr_bottom + 100.0f ) ew->pos.y = scr_work.scr_bottom + 100.0f ;
}

/*==============================================================*/
static void mark_partner_b1_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_partner_b1_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE ;
		ew->obj = obj_efky->obj[21] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/*==============================================================*/
static void mark_partner_a1_main( PLAYER_WORK *ew )
{
	VS_SYS_WORK *vs_w = &vs_sys_work ;
	PLAYER_WORK *pl_w = vs_w->team_w[0][1] ;

	ew->disp_flag = 0 ;
	if( pl_w->be_flag && pl_w->team_type == 8 && vs_w->vs_st == VS_FIGHT && pl_w->disp_flag ) ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	ew->pos.y += pl_w->size_top * pl_w->scale.y * DOT_Y( 1.0f ) ;
	ew->pos.y += 100.0f ;
	ew->pos.z += 0.0f ;
	if( ew->pos.y <= scr_work.scr_bottom + 100.0f ) ew->pos.y = scr_work.scr_bottom + 100.0f ;
}

/*==============================================================*/
static void mark_partner_a1_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_partner_a1_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE ;
		ew->obj = obj_efky->obj[20] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/*==============================================================*/
static void mark_partner_b2_main( PLAYER_WORK *ew )
{
	VS_SYS_WORK *vs_w = &vs_sys_work ;
	PLAYER_WORK *pl_w = vs_w->team_w[1][2] ;

	ew->disp_flag = 0 ;
	if( pl_w->be_flag && pl_w->team_type == 8 && vs_w->vs_st == VS_FIGHT && pl_w->disp_flag ) ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	ew->pos.y += pl_w->size_top * pl_w->scale.y * DOT_Y( 1.0f ) ;
	ew->pos.y += 100.0f ;
	ew->pos.z += 3.0f ;
	if( ew->pos.y <= scr_work.scr_bottom + 100.0f ) ew->pos.y = scr_work.scr_bottom + 100.0f ;
}

/*==============================================================*/
static void mark_partner_b2_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_partner_b2_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE ;
		ew->obj = obj_efky->obj[21] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/*==============================================================*/
static void mark_partner_a2_main( PLAYER_WORK *ew )
{
	VS_SYS_WORK *vs_w = &vs_sys_work ;
	PLAYER_WORK *pl_w = vs_w->team_w[0][2] ;

	ew->disp_flag = 0 ;
	if( pl_w->be_flag && pl_w->team_type == 8 && vs_w->vs_st == VS_FIGHT && pl_w->disp_flag ) ew->disp_flag = 1 ;
	ew->pos = pl_w->pos ;
	ew->pos.y += pl_w->size_top * pl_w->scale.y * DOT_Y( 1.0f ) ;
	ew->pos.y += 100.0f ;
	ew->pos.z += 2.0f ;
	if( ew->pos.y <= scr_work.scr_bottom + 100.0f ) ew->pos.y = scr_work.scr_bottom + 100.0f ;
}

/*==============================================================*/
static void mark_partner_a2_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = mark_partner_a2_main ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->obj = obj_efky->obj[20] ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->scale.z = 1.0f ;
	}
}

/****************************************************************/
void pl_mark_init( void )
{
	mark_point_a_set() ;
	mark_point_b_set() ;
	mark_partner_a1_set() ;
	mark_partner_b1_set() ;
	mark_partner_a2_set() ;
	mark_partner_b2_set() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_mark_ef.c" End of File	*/
