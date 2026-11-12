/* The training menu's sub-menu (shown while training_work.unk_1c is 3) and its settings page
   (while it is 4): six lines, four with a setting drawn beside them, and the cursor. */
#include "player.h"
#include "nl.h"
#include "dm.h"

void pause_sub_win_move( PLAYER_WORK *w ) ;
void pause_sub_cursor_move( PLAYER_WORK *w ) ;
void pause_opt_win_move( PLAYER_WORK *w ) ;
void pause_opt_label_move( PLAYER_WORK *w ) ;
void pause_opt_value_move( PLAYER_WORK *w ) ;
void pause_opt_cursor_move( PLAYER_WORK *w ) ;

static PAUSE_PART pause_sub_win[3] = {
	{ 1, 0, 50, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 20, { 0.0f, -0.0f, 0.0f } },
	{ -1, 0, 15, { 0.0f, 0.0f, 0.0f } }
} ;
static PAUSE_PART pause_opt_win[3] = {
	{ 1, 0, 50, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 47, { 0.0f, -0.0f, 0.0f } },
	{ -1, 0, 0, { 0.0f, 0.0f, 0.0f } }
} ;
/* each line's label, and the setting beside it by its value */
static PAUSE_PART pause_opt_label_0[2] = { { 1, 0, 4, { 0.0f, 0.0f, 0.0f } }, { -1, 0, 0, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_0[4] = {
	{ 1, 0, 35, { 0.0f, 0.0f, 0.0f } }, { 1, 0, 36, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 37, { 0.0f, 0.0f, 0.0f } }, { 1, 0, 38, { 0.0f, 0.0f, 0.0f } }
} ;
static PAUSE_PART pause_opt_label_1[2] = { { 1, 0, 5, { 0.0f, 0.0f, 0.0f } }, { -1, 0, 0, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_1[3] = {
	{ 1, 0, 40, { 0.0f, 0.0f, 0.0f } }, { 1, 0, 41, { 0.0f, 0.0f, 0.0f } }, { 1, 0, 42, { 0.0f, 0.0f, 0.0f } }
} ;
static PAUSE_PART pause_opt_label_2[2] = { { 1, 0, 10, { 0.0f, 0.0f, 0.0f } }, { -1, 0, 0, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_2[1] = { { 1, 0, 44, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_label_3[2] = { { 1, 0, 11, { 0.0f, 0.0f, 0.0f } }, { -1, 0, 0, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_3[1] = { { 1, 0, 13, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_label_4[2] = { { 1, 0, 14, { 0.0f, 0.0f, 0.0f } }, { -1, 0, 0, { 0.0f, 0.0f, 0.0f } } } ;
static PAUSE_PART pause_opt_label_5[3] = {
	{ 1, 0, 12, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 17, { -2.48f, -7.68f, -12.98f } },
	{ -1, 0, 0, { 0.0f, 0.0f, 0.0f } }
} ;
static PAUSE_PART pause_opt_end[1] = { { 1, 0, 48, { -2.48f, -7.68f, -12.99f } } } ;	/* the cursor on the last line */
static Sint8 pause_sub_cursor_scale[4] = { 3, 4, 5, 6 } ;
static FADE_COL pause_opt_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 pause_sub_cursor_pos[4] = {
	{ 0.092f, 3.64f, -12.99f }, { 0.092f, 0.168f, -12.99f }, { 0.092f, -3.28f, -12.99f }, { -5.904f, -7.016f, -12.99f }
} ;
static VEC3 pause_opt_cursor_pos[6] = {
	{ -5.2f, 3.164f, -13.02f }, { -5.2f, 1.412f, -13.02f }, { -4.324f, -0.236f, -13.02f },
	{ -4.116f, -2.104f, -13.02f }, { -2.0f, -3.86f, -13.02f }, { -0.24f, -5.06f, -13.02f }
} ;
static Sint8 pause_opt_cursor_scale[6] = { 14, 14, 15, 16, 17, 17 } ;

/****************************************************************/
/*			The sub-menu's window and cursor					*/
/****************************************************************/
void pause_sub_win_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_sub_win_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_sub_win ;
}

void pause_sub_cursor_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_sub_cursor_move ;
	w->pos = pause_sub_cursor_pos[training_work.cursor2] ;
	w->scale = pause_scale[pause_sub_cursor_scale[training_work.cursor2]] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	w->fade = pause_opt_fade ;
	w->obj = obj_stg->obj[8] ;
	w->work2 = 0 ;
}

/****************************************************************/
/*			The settings page: the cursor, the window, then		*/
/*			each line's label and setting						*/
/****************************************************************/
void pause_opt_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_cursor_move ;
	w->pos = pause_opt_cursor_pos[training_work.unk_03] ;
	w->scale = pause_scale[pause_opt_cursor_scale[training_work.unk_03]] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = 0 ;
	w->obj = obj_stg->obj[49] ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_win_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_win ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 0 ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_label_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_label_0 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 0 ;
	w->disp_flag = 1 ;
	w->sub_no = 0 ;
	w->work_func = pause_opt_value_move ;
	w->pos.x = pause_opt_0[0].pos.x ;
	w->pos.y = pause_opt_0[0].pos.y ;
	w->pos.z = pause_opt_0[0].pos.z ;
	w->scale = pause_scale[pause_opt_0[0].scale] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = 0 ;
	w->obj = obj_stg->obj[35] ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 1 ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_label_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_label_1 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 1 ;
	w->disp_flag = 1 ;
	w->sub_no = 0 ;
	w->work_func = pause_opt_value_move ;
	w->pos.x = pause_opt_1[0].pos.x ;
	w->pos.y = pause_opt_1[0].pos.y ;
	w->pos.z = pause_opt_1[0].pos.z ;
	w->scale = pause_scale[pause_opt_1[0].scale] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = 0 ;
	w->obj = obj_stg->obj[pause_opt_1[0].model] ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 2 ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_label_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_label_2 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 2 ;
	w->disp_flag = 1 ;
	w->sub_no = 0 ;
	w->work_func = pause_opt_value_move ;
	w->pos.x = pause_opt_2[0].pos.x ;
	w->pos.y = pause_opt_2[0].pos.y ;
	w->pos.z = pause_opt_2[0].pos.z ;
	w->scale = pause_scale[pause_opt_2[0].scale] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = 0 ;
	w->obj = obj_stg->obj[pause_opt_2[0].model] ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 3 ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_label_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_label_3 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 3 ;
	w->disp_flag = 1 ;
	w->sub_no = 0 ;
	w->work_func = pause_opt_value_move ;
	w->pos.x = pause_opt_3[0].pos.x ;
	w->pos.y = pause_opt_3[0].pos.y ;
	w->pos.z = pause_opt_3[0].pos.z ;
	w->scale = pause_scale[pause_opt_3[0].scale] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = 0 ;
	w->obj = obj_stg->obj[pause_opt_3[0].model] ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 4 ;
	w->disp_flag = 1 ;
	w->work_func = pause_opt_label_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->sub_no = 0 ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_label_4 ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->sub_type = 5 ;
	w->disp_flag = 1 ;
	w->sub_no = 0 ;
	w->work_func = pause_opt_label_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = pause_opt_fade ;
	w->work2 = ( Sint32 )pause_opt_label_5 ;
}

void pause_sub_win_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 3 ) work_free( w ) ;
}

void pause_sub_cursor_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 3 ){
		work_free( w ) ;
		return ;
	}
	w->pos = pause_sub_cursor_pos[training_work.cursor2] ;
	w->scale = pause_scale[pause_sub_cursor_scale[training_work.cursor2]] ;
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

void pause_opt_win_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 4 ) work_free( w ) ;
}

/* a line's label: white on the cursor's line, grey on the others */
void pause_opt_label_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 4 ){
		work_free( w ) ;
		return ;
	}
	w->fade.r = w->fade.g = w->fade.b = training_work.unk_03 != w->sub_type ? 0.7f : 1.0f ;
}

/* a line's setting, its model by the setting's value */
void pause_opt_value_move( PLAYER_WORK *w )
{
	Sint8 obj ;

	if( training_work.unk_1c != 4 ){
		work_free( w ) ;
		return ;
	}
	if( training_work.unk_03 != w->sub_type ){
		w->fade.g = w->fade.b = 0.7f ;
		w->fade.r = 0.0f ;
	} else w->fade.r = w->fade.g = w->fade.b = 1.0f ;
	switch( w->sub_type ){
	case 0 :
		obj = training_work.stance + 35 ;
		break ;
	case 1 :
		obj = training_work.dummy_guard + 40 ;
		break ;
	case 2 :
		obj = 45 - training_work.dummy_air_recover ;
		break ;
	case 3 :
		if( training_work.dummy_change ) obj = 21 ;
		else obj = 13 ;
		break ;
	default :
		obj = 0 ;
		break ;
	}
	w->obj = obj_stg->obj[obj] ;
}

/* the cursor glows, on the line or on the last one */
void pause_opt_cursor_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 4 ){
		work_free( w ) ;
		return ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	if( training_work.unk_03 != 5 ){
		w->obj = obj_stg->obj[49] ;
		w->pos = pause_opt_cursor_pos[training_work.unk_03] ;
		w->scale = pause_scale[pause_opt_cursor_scale[training_work.unk_03]] ;
	} else {
		w->obj = obj_stg->obj[48] ;
		w->pos.x = pause_opt_end[0].pos.x ;
		w->pos.y = pause_opt_end[0].pos.y ;
		w->pos.z = pause_opt_end[0].pos.z ;
		w->scale = pause_scale[pause_opt_end[0].scale] ;
	}
}
