/* The training menu's settings page on the pause's list, shown while training_work.page is 2:
   eight lines, each with its label and its setting, and the cursor. */
#include "player.h"
#include "nl.h"
#include "dm.h"

void pause_cfg_win_move( PLAYER_WORK *w ) ;
void pause_cfg_cursor_move( PLAYER_WORK *w ) ;
void pause_cfg_label_move( PLAYER_WORK *w ) ;
void pause_cfg_value_move( PLAYER_WORK *w ) ;
void pause_cfg_end_move( PLAYER_WORK *w ) ;

static PAUSE_PART pause_cfg_win[3] = {
	{ 1, 0, 50, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 19, { 0.0f, 0.0f, 0.0f } },
	{ -1, 0, 0, { 0.0f, 0.0f, 0.0f } }
} ;
static VEC3 pause_cfg_scale[1] = { { 1.0f, 1.0f, 1.0f } } ;
static FADE_COL pause_cfg_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;		/* the line the cursor is on */
static FADE_COL pause_cfg_fade_off = { 1.0f, 0.7f, 0.7f, 0.7f } ;	/* the other lines */
static FADE_COL pause_cfg_fade_value = { 1.0f, 0.0f, 0.7f, 0.7f } ;	/* the settings */
static VEC3 pause_cfg_pos[1] = { { 0.0f, 0.0f, 0.0f } } ;
static VEC3 pause_cfg_end_pos[1] = { { -1.0f, -7.0f, -12.98f } } ;	/* the last line */
static Sint8 pause_cfg_label_model[8] = { 9, 15, 16, 18, 22, 23, 24, 25 } ;
static VEC3 pause_cfg_cursor_pos[7] = {
	{ -3.28f, 4.9f, -13.02f }, { -3.76f, 3.3f, -13.02f }, { -4.68f, 1.54f, -13.02f }, { -6.36f, -0.22f, -13.02f },
	{ -7.0f, -1.94f, -13.02f }, { -6.6f, -3.78f, -13.02f }, { -3.72f, -5.26f, -13.02f }
} ;
static VEC3 pause_cfg_cursor_scale[7] = {
	{ 2.2f, 0.7f, 1.0f }, { 2.5f, 0.7f, 1.0f }, { 2.4f, 0.7f, 1.0f }, { 2.0f, 0.7f, 1.0f },
	{ 1.93f, 0.7f, 1.0f }, { 1.93f, 0.7f, 1.0f }, { 3.3f, 0.7f, 1.0f }
} ;

/****************************************************************/
/*			The page, line by line								*/
/****************************************************************/
void pause_cfg_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_win_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade ;
	w->work2 = ( Sint32 )pause_cfg_win ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_cursor_move ;
	w->pos = pause_cfg_cursor_pos[training_work.cursor2] ;
	w->scale = pause_cfg_cursor_scale[training_work.cursor2] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade ;
	w->obj = obj_stg->obj[49] ;
	w->work2 = 0 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[9] ;
	w->work2 = 0 ;
	w->type = 0 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[15] ;
	w->work2 = 0 ;
	w->type = 1 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[16] ;
	w->work2 = 0 ;
	w->type = 2 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[18] ;
	w->work2 = 0 ;
	w->type = 3 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[22] ;
	w->work2 = 0 ;
	w->type = 4 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[23] ;
	w->work2 = 0 ;
	w->type = 5 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[24] ;
	w->work2 = 0 ;
	w->type = 6 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_label_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[25] ;
	w->work2 = 0 ;
	w->type = 7 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[17] ;
	w->work2 = 0 ;
	w->type = 0 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[59] ;
	w->work2 = 0 ;
	w->type = 1 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[31] ;
	w->work2 = 0 ;
	w->type = 2 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[51] ;
	w->work2 = 0 ;
	w->type = 3 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[26] ;
	w->work2 = 0 ;
	w->type = 4 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[67] ;
	w->work2 = 0 ;
	w->type = 5 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_value_move ;
	w->pos = pause_cfg_end_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[17] ;
	w->work2 = 0 ;
	w->type = 7 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cfg_end_move ;
	w->pos = pause_cfg_end_pos[0] ;
	w->scale = pause_cfg_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_cfg_fade_off ;
	w->obj = obj_stg->obj[48] ;
	w->work2 = 0 ;
	w->type = 7 ;
}

void pause_cfg_win_move( PLAYER_WORK *w )
{
	if( training_work.page != 2 ) work_free( w ) ;
}

/* it follows the line and glows, and hides on the last line */
void pause_cfg_cursor_move( PLAYER_WORK *w )
{
	if( training_work.page != 2 ){
		work_free( w ) ;
		return ;
	}
	w->disp_flag = 0 ;
	if( training_work.cursor2 == 7 ) return ;
	w->disp_flag = 1 ;
	w->pos = pause_cfg_cursor_pos[training_work.cursor2] ;
	w->scale = pause_cfg_cursor_scale[training_work.cursor2] ;
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

void pause_cfg_label_move( PLAYER_WORK *w )
{
	if( training_work.page != 2 ){
		work_free( w ) ;
		return ;
	}
	w->fade = pause_cfg_fade_off ;
	if( training_work.cursor2 == w->type ) w->fade = pause_cfg_fade ;
}

/* each line's setting, by its value */
void pause_cfg_value_move( PLAYER_WORK *w )
{
	if( training_work.page != 2 ){
		work_free( w ) ;
		return ;
	}
	w->fade = pause_cfg_fade_value ;
	if( training_work.cursor2 == w->type ) w->fade = pause_cfg_fade ;
	switch( w->type ){
	case 0:
		w->obj = obj_stg->obj[34] ;
		if( training_work.auto_gauge ) w->obj = obj_stg->obj[33] ;
		break ;
	case 1:
		w->obj = obj_stg->obj[59] ;
		if( training_work.delay_hyper_free ) w->obj = obj_stg->obj[60] ;
		break ;
	case 2:
		w->obj = obj_stg->obj[32] ;
		if( training_work.damage_disp ) w->obj = obj_stg->obj[31] ;
		break ;
	case 3:
		w->fade = pause_cfg_fade_off ;
		if( training_work.cursor2 == w->type ) w->fade = pause_cfg_fade ;
		switch( training_work.renda_level ){
		case 0: w->obj = obj_stg->obj[51] ; break ;
		case 1: w->obj = obj_stg->obj[52] ; break ;
		case 2: w->obj = obj_stg->obj[53] ; break ;
		case 3: w->obj = obj_stg->obj[54] ; break ;
		case 4: w->obj = obj_stg->obj[55] ; break ;
		case 5: w->obj = obj_stg->obj[56] ; break ;
		case 6: w->obj = obj_stg->obj[57] ; break ;
		case 7: w->obj = obj_stg->obj[58] ; break ;
		}
		break ;
	case 4:
		w->fade = pause_cfg_fade_off ;
		if( training_work.cursor2 == w->type ) w->fade = pause_cfg_fade ;
		switch( training_work.damage_level ){
		case 0: w->obj = obj_stg->obj[26] ; break ;
		case 1: w->obj = obj_stg->obj[27] ; break ;
		case 2: w->obj = obj_stg->obj[28] ; break ;
		case 3: w->obj = obj_stg->obj[30] ; break ;
		}
		break ;
	case 5:
		switch( training_work.game_speed ){
		case 0: w->obj = obj_stg->obj[67] ; break ;
		case 1: w->obj = obj_stg->obj[68] ; break ;
		case 2: w->obj = obj_stg->obj[64] ; break ;
		}
		break ;
	case 6:
		break ;
	case 7:
		w->fade = pause_cfg_fade_off ;
		if( training_work.cursor2 == w->type ) w->fade = pause_cfg_fade ;
		break ;
	}
}

/* the cursor on the last line */
void pause_cfg_end_move( PLAYER_WORK *w )
{
	if( training_work.page != 2 ){
		work_free( w ) ;
		return ;
	}
	w->disp_flag = 0 ;
	w->fade = pause_cfg_fade_off ;
	if( training_work.cursor2 != w->type ) return ;
	w->disp_flag = 1 ;
	w->fade = pause_cfg_fade ;
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}
