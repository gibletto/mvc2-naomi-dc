/* Jill's player effect 00 (character 11's player effect 00): the twenty pieces that ride on her,
   chosen by type. Each type has its own routines for init and move. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl11_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS11_00_LOCAL_W {
	Uint16	char_no;		/* 00: her animation when the piece began */
} PLS11_00_LOCAL_W;

static PL11_LOCAL_W *pls11_00_lw ;	/* Jill's tlocal, set each frame by the main routine */
static PLS11_00_LOCAL_W *pls11_00_ew_w ;	/* the effect's own work */

static void pls11_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls11_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls11_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x1100 ;
	}
	return ew ;
}

/****************************************************************/
/*			The second setter: a piece that rides on another	*/
/*			piece												*/
/****************************************************************/
PLAYER_WORK *pls11_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls11_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x1100 ;
	}
	return ew ;
}

static void pls11_00_shell_main( PLAYER_WORK *ew );
static void pls11_00_t1_main( PLAYER_WORK *ew );
static void pls11_00_flash_main( PLAYER_WORK *ew );
static void pls11_00_stars_main( PLAYER_WORK *ew );
static void pls11_00_trail_main( PLAYER_WORK *ew );
static void pls11_00_hstars_main( PLAYER_WORK *ew );
static void pls11_00_rocket_flash_main( PLAYER_WORK *ew );
static void pls11_00_rocket_trail_main( PLAYER_WORK *ew );
static void pls11_00_heal_main( PLAYER_WORK *ew );
static void pls11_00_appear_main( PLAYER_WORK *ew );
static void pls11_00_win_shot_main( PLAYER_WORK *ew );
static void pls11_00_win_main( PLAYER_WORK *ew );
static void pls11_00_tyrant_atck_main( PLAYER_WORK *ew );
static void pls11_00_charge_main( PLAYER_WORK *ew );
static void pls11_00_rocket_jump_main( PLAYER_WORK *ew );
static void pls11_00_backblast_main( PLAYER_WORK *ew );
static void pls11_00_zombie_fire_main( PLAYER_WORK *ew );
static void pls11_00_feather_main( PLAYER_WORK *ew );
static void pls11_00_t18_main( PLAYER_WORK *ew );
static void pls11_00_rocket_fall_main( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_shell_main, pls11_00_t1_main, pls11_00_flash_main, pls11_00_stars_main,
		pls11_00_trail_main, pls11_00_hstars_main, pls11_00_rocket_flash_main, pls11_00_rocket_trail_main,
		pls11_00_heal_main, pls11_00_appear_main, pls11_00_win_shot_main, pls11_00_win_main,
		pls11_00_tyrant_atck_main, pls11_00_charge_main, pls11_00_rocket_jump_main, pls11_00_backblast_main,
		pls11_00_zombie_fire_main, pls11_00_feather_main, pls11_00_t18_main, pls11_00_rocket_fall_main,
	} ;

	pls11_00_lw = ( PL11_LOCAL_W * )&ew->master_w->tlocal[0] ;
	pls11_00_ew_w = ( PLS11_00_LOCAL_W * )&ew->obj_flag ;
	pls11_00_jmp[ew->type]( ew ) ;
}

static void pls11_00_end( PLAYER_WORK *ew );
static void pls11_00_free( PLAYER_WORK *ew );

/* type 0: the spent case of the counter's shots, thrown behind her; it bounces and blinks out */
static void pls11_00_shell_init( PLAYER_WORK *ew );
static void pls11_00_shell_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_shell_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_shell_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_shell_init, pls11_00_shell_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_shell_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_shell_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 32.0f ) : -DOT_X( 32.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 80.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? -5.0f : 5.0f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	ef_char_set( ew, 27, 0 ) ;
}

static void pls11_00_shell_fall( PLAYER_WORK *ew );
static void pls11_00_shell_lie( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls11_00_shell_move( PLAYER_WORK *ew )
{
	static void ( *pls11_00_shell_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_shell_fall, pls11_00_shell_lie,
	} ;

	pls11_00_shell_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls11_00_shell_fall( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->spd.y = -( ew->spd.y / 2.0f ) ;
		ew->spd.x = ew->spd.x / 2.0f ;
		ew->timer0 = 48 ;
	}
}

/*--------------------------------------------------------------*/
static void pls11_00_shell_lie( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 ){
		ew->disp_flag = ( ew->timer0 & 4 ) ? 1 : 0 ;
		if( ew->pos.y <= ew->master_w->ground_y ){
			ew->pos.y = ew->master_w->ground_y ;
			ew->spd.y = -( ew->spd.y / 2.0f ) ;
		}
		return ;
	}
	pls11_00_end( ew ) ;
}

/* type 1: the same, thrown forward */
static void pls11_00_t1_init( PLAYER_WORK *ew );
static void pls11_00_t1_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_t1_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_t1_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_t1_init, pls11_00_t1_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_t1_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_t1_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 32.0f ) : -DOT_X( 32.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 68.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	ew->spd.y = -DOT_Y( 3.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ef_char_set( ew, 27, 1 ) ;
}

static void pls11_00_t1_fall( PLAYER_WORK *ew );
static void pls11_00_t1_lie( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls11_00_t1_move( PLAYER_WORK *ew )
{
	static void ( *pls11_00_t1_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_t1_fall, pls11_00_t1_lie,
	} ;

	pls11_00_t1_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls11_00_t1_fall( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		ew->spd.y = -( ew->spd.y / 2.0f ) ;
		ew->timer0 = 48 ;
	}
}

/*--------------------------------------------------------------*/
static void pls11_00_t1_lie( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 ){
		ew->disp_flag = ( ew->timer0 & 4 ) ? 1 : 0 ;
		if( ew->pos.y <= ew->master_w->ground_y ){
			ew->pos.y = ew->master_w->ground_y ;
			ew->spd.y = -( ew->spd.y / 2.0f ) ;
		}
		return ;
	}
	pls11_00_end( ew ) ;
}

/* type 2: the gun's flash in front of her, with each of the counter's shots */
static void pls11_00_flash_init( PLAYER_WORK *ew );
static void pls11_00_flash_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_flash_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_flash_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_flash_init, pls11_00_flash_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_flash_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_flash_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? 85.0f : -85.0f ) ;
	ew->pos.y = ew->master_w->pos.y + 165.0f ;
	ef_char_set( ew, 27, 2 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_flash_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* type 3: a piece that stays on her while Charging Stars rushes */
static void pls11_00_stars_init( PLAYER_WORK *ew );
static void pls11_00_stars_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_stars_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_stars_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_stars_init, pls11_00_stars_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_stars_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_stars_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls11_00_ew_w->char_no = ew->master_w->char_no.w ;
	if( ew->sub_type ) pl_se_req( ew, 75 ) ;
	ew->alpha = 0.75f ;
	ef_char_set( ew, 27, ew->sub_type + 5 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_stars_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != pl11_sp_stars || !pl_w->cg_type ){
		pls11_00_end( ew ) ;
		return ;
	}
	ef_char_move( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

/* type 4: a puff of the trail, left where the grenade or the win pose's shot was */
static void pls11_00_trail_init( PLAYER_WORK *ew );
static void pls11_00_trail_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_trail_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_trail_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_trail_init, pls11_00_trail_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_trail_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_trail_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 11 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ef_char_set( ew, 27, 9 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_trail_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* type 5: Hyper Charging Stars' piece, on her until her animation changes, then it plays its second part */
static void pls11_00_hstars_init( PLAYER_WORK *ew );
static void pls11_00_hstars_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_hstars_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_hstars_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_hstars_init, pls11_00_hstars_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_hstars_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_hstars_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	pls11_00_ew_w->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, ew->sub_type + 10 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_hstars_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( !ew->routine_1 ){
		ef_char_move( ew ) ;
		if( pls11_00_ew_w->char_no == ew->master_w->char_no.w ) return ;
		ew->routine_1 ++ ;
		ef_char_set( ew, 27, ew->sub_type + 12 ) ;
	} else if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* where the piece is put out from her: x, and y above her feet */
typedef struct PLS11_00_ROCKET_FLASH_DATA {
	Float	x, y ;
} PLS11_00_ROCKET_FLASH_DATA ;

/* type 6: a rocket's flash as it is fired, at a place ahead of her chosen by no */
static void pls11_00_rocket_flash_init( PLAYER_WORK *ew );
static void pls11_00_rocket_flash_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_rocket_flash_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_rocket_flash_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_rocket_flash_init, pls11_00_rocket_flash_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_rocket_flash_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_rocket_flash_init( PLAYER_WORK *ew )
{
	static PLS11_00_ROCKET_FLASH_DATA pls11_00_rocket_flash_tbl[] = {
		{ DOT_X( 56.0f ), DOT_Y( 86.0f ) }, { 110.0f, DOT_Y( 73.0f ) },
		{ 110.0f, 180.0f }, { 95.0f, DOT_Y( 74.0f ) },
	} ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->spd.x = ew->disp_dir ? pls11_00_rocket_flash_tbl[ew->sub_type].x : -pls11_00_rocket_flash_tbl[ew->sub_type].x ;
	ew->spd.y = pls11_00_rocket_flash_tbl[ew->sub_type].y ;
	ew->pos.x = ew->master_w->pos.x + ew->spd.x ;
	ew->pos.y = ew->master_w->pos.y + ew->spd.y ;
	ef_char_set( ew, 27, 14 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_rocket_flash_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* type 7: a puff of the trail, left where a rocket was */
static void pls11_00_rocket_trail_init( PLAYER_WORK *ew );
static void pls11_00_rocket_trail_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_rocket_trail_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_rocket_trail_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_rocket_trail_init, pls11_00_rocket_trail_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_rocket_trail_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_rocket_trail_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ef_char_set( ew, 27, 9 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_rocket_trail_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* type 8: the heal's piece out in front of her, with her voice */
static void pls11_00_heal_init( PLAYER_WORK *ew );
static void pls11_00_heal_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_heal_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_heal_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_heal_init, pls11_00_heal_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_heal_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_heal_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_dir = 0 ;
	ew->prio = 11 ;
	pl_w = ew->master_w ;
	ew->pos.x = pl_w->pos.x + ( pl_w->disp_dir ? DOT_X( 64.0f ) : -DOT_X( 64.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 88.0f ) ;
	pl_voice_req( ew, 32 ) ;
	ef_char_set( ew, 27, ew->sub_type + 16 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_heal_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* type 9: a piece of her entrance that falls behind her, bounces once and blinks out */
static void pls11_00_appear_init( PLAYER_WORK *ew );
static void pls11_00_appear_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_appear_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_appear_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_appear_init, pls11_00_appear_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_appear_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_appear_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? 20.0f : -20.0f ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 119.0f ) ;
	ew->spd.x = ew->disp_dir ? -DOT_X( 2.0f ) : DOT_X( 2.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ef_char_set( ew, 27, ew->sub_type + 3 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_appear_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->routine_1 ){
		ef_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y <= pl_w->ground_y ){
			ew->routine_1 ++ ;
			ew->pos.y = pl_w->ground_y ;
			ew->spd.y = -ew->spd.y / 2.0f ;
			ew->timer0 = 32 ;
		}
	} else {
		ef_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y <= ew->master_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.y = -ew->spd.y / 2.0f ;
		}
		if( !-- ew->timer0 ){
			pls11_00_end( ew ) ;
			return ;
		}
		ew->disp_flag = Sys->game_timer0 & 1 ;
	}
}

/* type 10: the win pose's shot, which flies up in front of her and leaves a trail (type 4) */
static void pls11_00_win_shot_init( PLAYER_WORK *ew );
static void pls11_00_win_shot_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_win_shot_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_win_shot_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_win_shot_init, pls11_00_win_shot_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_win_shot_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_win_shot_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 7 ;
	ew->size_top = 48 ;
	ew->size_bottom = 48 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 62.0f ) : -DOT_X( 62.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 30.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = 15.0f ;
	ew->timer0 = 2 ;
	ef_char_set( ew, 27, 18 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_win_shot_move( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->timer0 = 2 ;
		pls11_00_set2( ew, 4, 0 ) ;
	}
	if( !scr_in_y_ck( ew ) ) pls11_00_end( ew ) ;
}

/* type 11: a piece on her that goes when the win pose is over */
static void pls11_00_win_init( PLAYER_WORK *ew );
static void pls11_00_win_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_win_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_win_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_win_init, pls11_00_win_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_win_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_win_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 7 ;
	ew->size_top = 48 ;
	ew->size_bottom = 48 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 19 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_win_move( PLAYER_WORK *ew )
{
	if( ew->master_w->act_st != N_WIN ) pls11_00_end( ew ) ;
}

/* type 12: the mark of a blow on the held opponent, which keeps with the hand it came from */
static void pls11_00_tyrant_atck_init( PLAYER_WORK *ew );
static void pls11_00_tyrant_atck_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_tyrant_atck_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_tyrant_atck_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_tyrant_atck_init, pls11_00_tyrant_atck_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_tyrant_atck_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_tyrant_atck_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 11 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ef_char_set( ew, 27, ew->sub_type + 20 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_tyrant_atck_move( PLAYER_WORK *ew )
{
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* where the charge's piece sits by her stance: x ahead of her and y above her feet */
typedef struct PLS11_00_CHARGE_DATA {
	Float	x, y ;
} PLS11_00_CHARGE_DATA ;

/* type 13: the charge, a piece at a place by her stance until it is let go or Charging Stars starts */
static void pls11_00_charge_init( PLAYER_WORK *ew );
static void pls11_00_charge_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_charge_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_charge_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_charge_init, pls11_00_charge_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_charge_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_charge_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->sub_prio = -1 ;
	ew->disp_flag = 0 ;
	pl_voice_req( ew, 33 ) ;
	ef_char_set( ew, 27, 22 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_charge_move( PLAYER_WORK *ew )
{
	static PLS11_00_CHARGE_DATA pls11_00_charge_tbl[] = {
		{ 0.0f, DOT_Y( 64.0f ) }, { DOT_X( 10.0f ), DOT_Y( 39.0f ) },
		{ 5.0f, DOT_Y( 72.0f ) }, { -25.0f, DOT_Y( 8.0f ) },
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	int no ;

	ew->prio = pl_w->prio ;
	switch( pl_w->pl_stance ){
	case STANCE_STAND:
		no = 0 ;
		break ;
	case STANCE_CROUCH:
		no = 1 ;
		break ;
	case STANCE_AIR:
		no = 2 ;
		break ;
	case STANCE_DOWN:
	default:
		no = 3 ;
		break ;
	}
	ew->disp_flag = 1 ;
	ew->pos.x = pl_w->pos.x + ( pl_w->pl_dir ? pls11_00_charge_tbl[no].x : -pls11_00_charge_tbl[no].x ) ;
	ew->pos.y = pl_w->pos.y + pls11_00_charge_tbl[no].y ;
	ef_char_move( ew ) ;
	if( !pls11_00_lw->charge_flag || ( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type == pl11_sp_stars ) ){
		pl_voice_req( ew, 39 ) ;
		pls11_00_end( ew ) ;
	}
}

/* type 14: a piece at her place as the last rocket throws her back */
static void pls11_00_rocket_jump_init( PLAYER_WORK *ew );
static void pls11_00_rocket_jump_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_rocket_jump_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_rocket_jump_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_rocket_jump_init, pls11_00_rocket_jump_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_rocket_jump_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_rocket_jump_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_set( ew, 27, 23 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_rocket_jump_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* type 15: a rocket's blast behind her as it is fired, at a height chosen by no */
static void pls11_00_backblast_init( PLAYER_WORK *ew );
static void pls11_00_backblast_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_backblast_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_backblast_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_backblast_init, pls11_00_backblast_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_backblast_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_backblast_init( PLAYER_WORK *ew )
{
	static Float pls11_00_backblast_tbl[] = {
		DOT_Y( 86.0f ), DOT_Y( 73.0f ), 180.0f, DOT_Y( 74.0f ),
	} ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 12 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? -DOT_X( 85.0f ) : DOT_X( 85.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + pls11_00_backblast_tbl[ew->sub_type] ;
	ef_char_set( ew, 27, 15 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_backblast_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls11_00_end( ew ) ;
}

/* where the fire's pieces sit by the frame of the zombie they ride: x ahead, y above */
typedef struct PLS11_00_ZOMBIE_FIRE_DATA {
	Float	x, y ;
} PLS11_00_ZOMBIE_FIRE_DATA ;

/* type 16: the three fires that ride on the zombie and fade once it has landed; nothing sets it */
static void pls11_00_zombie_fire_init( PLAYER_WORK *ew );
static void pls11_00_zombie_fire_move( PLAYER_WORK *ew );
static void pls11_00_zombie_fire_pos( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_zombie_fire_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_zombie_fire_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_zombie_fire_init, pls11_00_zombie_fire_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_zombie_fire_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_zombie_fire_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 12 ;
	ef_char_set( ew, 27, 24 ) ;
	pls11_00_zombie_fire_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_zombie_fire_move( PLAYER_WORK *ew )
{
	ef_char_move( ew ) ;
	pls11_00_zombie_fire_pos( ew ) ;
	if( pls11_00_lw->zombie_shot_flag ){
		if( !ew->routine_1 ){
			if( pls11_00_lw->zombie_land_flag ){
				ew->routine_1 ++ ;
				ew->alpha = 1.0f ;
			}
		} else if( ( ew->alpha -= 0.05f ) <= 0.0f ) pls11_00_end( ew ) ;
	} else pls11_00_end( ew ) ;
}

/*==============================================================*/
static void pls11_00_zombie_fire_pos( PLAYER_WORK *ew )
{
	static PLS11_00_ZOMBIE_FIRE_DATA pls11_00_zombie_fire_tbl[] = {
		{ -20.0f, DOT_Y( 85.0f ) }, { DOT_X( 7.0f ), DOT_Y( 54.0f ) }, { DOT_X( 22.0f ), DOT_Y( 81.0f ) },
		{ -DOT_X( 10.0f ), DOT_Y( 89.0f ) }, { DOT_X( 7.0f ), DOT_Y( 54.0f ) }, { DOT_X( 22.0f ), DOT_Y( 81.0f ) },
		{ -DOT_X( 14.0f ), DOT_Y( 81.0f ) }, { DOT_X( 7.0f ), DOT_Y( 54.0f ) }, { DOT_X( 22.0f ), DOT_Y( 81.0f ) },
		{ DOT_X( 4.0f ), DOT_Y( 83.0f ) }, { 0.0f, DOT_Y( 47.0f ) }, { DOT_X( 31.0f ), DOT_Y( 71.0f ) },
		{ -DOT_X( 25.0f ), DOT_Y( 72.0f ) }, { DOT_X( 7.0f ), DOT_Y( 41.0f ) }, { DOT_X( 10.0f ), DOT_Y( 74.0f ) },
		{ -60.0f, DOT_Y( 47.0f ) }, { DOT_X( 7.0f ), DOT_Y( 25.0f ) }, { -DOT_X( 7.0f ), DOT_Y( 55.0f ) },
		{ -60.0f, 30.0f }, { DOT_X( 16.0f ), DOT_Y( 8.0f ) }, { -15.0f, DOT_Y( 19.0f ) },
		{ -DOT_X( 38.0f ), 15.0f }, { DOT_X( 16.0f ), DOT_Y( 17.0f ) }, { -DOT_X( 14.0f ), -DOT_Y( 4.0f ) },
		{ -65.0f, 0.0f }, { DOT_X( 22.0f ), DOT_Y( 6.0f ) }, { -20.0f, -DOT_Y( 6.0f ) },
	} ;

	Float x ;

	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	x = pls11_00_zombie_fire_tbl[ew->ef_w->cg_flag1 * 3 + ew->sub_type].x ;
	ew->pos.x += ew->disp_dir ? x : -x ;
	ew->pos.y += pls11_00_zombie_fire_tbl[ew->ef_w->cg_flag1 * 3 + ew->sub_type].y ;
}

/* type 17: a feather that waits a few frames, shows near the crow, falls and fades */
static void pls11_00_feather_init( PLAYER_WORK *ew );
static void pls11_00_feather_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_feather_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_feather_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_feather_init, pls11_00_feather_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_feather_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_feather_init( PLAYER_WORK *ew )
{
	int x, y ;

	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		CHAR_PART_COPY( ew, ew->master_w ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = ew->master_w->pl_no ;
		ew->pl_type = ew->master_w->pl_type ;
		ew->scale.x = ew->master_w->scale.x ;
		ew->scale.y = ew->master_w->scale.y ;
		ew->atck_level = ew->master_w->atck_level ;
		ew->work_no = ew->master_w->work_no ;
		ew->col_cnt = ew->master_w->col_cnt ;
		ew->scale = ew->master_w->scale ;
		ew->prio = ew->master_w->prio ;
		ew->disp_flag = 0 ;
		ew->prio = 11 ;
		ew->timer0 = Rnd() & 15 ;
	} else if( -- ew->timer0 < 0 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 1 ;
		ew->pos.x = ew->ef_w->pos.x ;
		ew->pos.y = ew->ef_w->pos.y + DOT_Y( 32.0f ) ;
		x = Rnd() & 31 ;
		if( x & 16 ) x = -15 ;
		ew->pos.x += DOT_X( x ) ;
		y = Rnd() & 31 ;
		if( y & 16 ) y = -15 ;
		ew->pos.y += DOT_Y( y ) ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->spd.y = -DOT_Y( 0.75f ) ;
		ef_char_set2( ew, 27, 25, Rnd() & 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls11_00_feather_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		ef_char_move( ew ) ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y <= ew->master_w->ground_y ){
			ew->routine_1 ++ ;
			ew->pos.y = ew->master_w->ground_y ;
			ew->alpha = 1.0f ;
		}
	} else if( ( ew->alpha -= 0.05f ) <= 0.0f ) pls11_00_end( ew ) ;
}

/* type 18: a blinking piece on Tyrant Escape's reach, two kinds by no; its animation changes once the reach has caught */
static void pls11_00_t18_init( PLAYER_WORK *ew );
static void pls11_00_t18_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_t18_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_t18_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_t18_init, pls11_00_t18_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_t18_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_t18_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 12 ;
	ew->sub_prio = -4 - ew->pl_no ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	if( ew->sub_type ){
		ew->pos.y += DOT_Y( 288.0f ) ;
		ew->scale.y = 18.0f ;
		ef_char_set( ew, 27, 27 ) ;
	} else ef_char_set( ew, 27, 26 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_t18_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 && pls11_00_lw->tyrant_reach_st > 0 ){
		ew->routine_1 ++ ;
		if( ew->sub_type ) ef_char_set( ew, 27, 29 ) ;
		else ef_char_set( ew, 27, 28 ) ;
	}
	if( ef_char_move( ew ) < 0 ){
		pls11_00_end( ew ) ;
		return ;
	}
	ew->disp_flag = Sys->game_timer0 & 1 ;
}

/* type 19: a piece that flies off behind her as she falls back, and goes at the edge of the screen or the ground */
static void pls11_00_rocket_fall_init( PLAYER_WORK *ew );
static void pls11_00_rocket_fall_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls11_00_rocket_fall_main( PLAYER_WORK *ew )
{
	static void ( *pls11_00_rocket_fall_jmp[] )( PLAYER_WORK *ew ) = {
		pls11_00_rocket_fall_init, pls11_00_rocket_fall_move, pls11_00_end, pls11_00_free,
	} ;

	pls11_00_rocket_fall_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls11_00_rocket_fall_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 12 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? -DOT_X( 115.0f ) : DOT_X( 115.0f ) ) ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 17.0f ) ;
	ew->spd.x = ew->disp_dir ? -20.0f : 20.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 4.5f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	ef_char_set( ew, 27, 30 ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_rocket_fall_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ef_char_move( ew ) ;
	if( !scr_in_ck( ew ) ){
		pls11_00_end( ew ) ;
		return ;
	}
	if( ew->pos.y > pl_w->ground_y ) return ;
	pls11_00_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	pls11_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls11_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls11_00.c" End of File	*/
