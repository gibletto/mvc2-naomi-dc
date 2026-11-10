/* The marks beside the fight's gauges (work kind 11, a sequel to gauge.c):
   one for each of the six players, set up with the stage. It turns over and moves to its new place
   when the player's place in the team changes, dims while the vital's red part waits, and fades
   when the fight is over. */
#include "player.h"

/* the object of each player's mark, in obj_dm00 */
static Sint32 name_obj_tbl[6] = { 0x43, 0x4c, 0x44, 0x4d, 0x45, 0x4e } ;

/* where each is, by the place in the team times two, plus the player's side */
static VEC3 name_pos_tbl[6] = {
	{ -82.0f, 73.5f, -126.0f },
	{ 82.0f, 73.5f, -126.0f },
	{ -118.0f, 86.5f, -170.0f },
	{ 118.0f, 86.5f, -170.0f },
	{ -118.0f, 75.0f, -170.0f },
	{ 118.0f, 75.0f, -170.0f },
} ;

static VEC3 name_zero_vec = { 0.0f, 0.0f, 0.0f } ;
static VEC3 name_unit_vec = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL name_white_col = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static FADE_COL name_dim_col = { 1.0f, 0.5f, 0.5f, 0.5f } ;
static Uint8 name_team_idx_tbl[6] = { 0, 2, 4, 1, 3, 5 } ;

static void name_set( int no ) ;
static void name_main( PLAYER_WORK *ew ) ;

/****************************************************************/
/*			The six players' marks								*/
/****************************************************************/
void name_init( void )
{
	int i ;

	for( i = 0 ; i < 6 ; i ++ ) name_set( i ) ;
}

/*==============================================================*/
static void name_set( int no )
{
	PLAYER_WORK *ew, *pl_w ;
	int i ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = name_main ;
		ew->obj = obj_dm00->obj[name_obj_tbl[no]] ;
		pl_w = &pl_work[no] ;
		i = name_team_idx_tbl[pl_w->pl_no * 3 + pl_w->team_no] ;
		ew->pos = name_pos_tbl[i] ;
		ew->obj_flag = 0x10c31 ;
		ew->scale = name_unit_vec ;
		ew->type = no ;
		ew->sub_type = pl_work[no].team_no ;
		ew->fade = name_white_col ;
		ew->master_w = pl_w ;
	}
}

/*==============================================================*/
static void name_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VEC3 *p ;
	int team = pl_w->team_no ;

	if( ew->sub_type != team ){
		p = &name_pos_tbl[team * 2 + pl_w->pl_no] ;
		ew->spd = *p ;
		ew->sub_type = team ;
		ew->timer0 = 32 ;
		ew->timer1 = 0 ;
	}
	if( ew->timer0 ){
		if( ew->timer0 == 16 ){
			ew->timer1 += 0x8000 ;
			ew->pos = ew->spd ;
		}
		ew->scale.x = nlCos( ew->timer1 ) ;
		ew->timer0 -- ;
		ew->timer1 += 0x400 ;
	}
	ew->fade.r = name_white_col.r ;
	ew->fade.g = name_white_col.g ;
	ew->fade.b = name_white_col.b ;
	if( pl_w->vital_red_wait ){
		ew->fade.r = name_dim_col.r ;
		ew->fade.g = name_dim_col.g ;
		ew->fade.b = name_dim_col.b ;
	}
	if( !pl_w->ko_flag && pl_w->vital_new <= 0 ) ew->disp_flag = 0 ;
	if( ew->disp_flag ){
		if( pl_w->ko_flag && !pl_w->be_flag ){
			if( vs_sys_work.ko_cnt[pl_w->pl_no] >= 3 && !pl_w->team_no ){
				ew->fade.trnsl = 1.0f ;
			} else {
				ew->fade.trnsl -= 0.03125f ;
				if( ew->fade.trnsl < 0.0f ){
					ew->disp_flag = 0 ;
					ew->fade.trnsl = 0.0f ;
				}
			}
		}
	} else if( !pl_w->ko_flag && pl_w->vital_new > 0 ){
		ew->disp_flag = 1 ;
		ew->fade.trnsl = 1.0f ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "gauge_name.c" End of File	*/
