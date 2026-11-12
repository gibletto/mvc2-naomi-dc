/* The training menu's yes/no window and its cursor on the pause's list, shown while
   training_work.unk_1c is 5. */
#include "player.h"
#include "nl.h"
#include "dm.h"

void pause_yn_win_move( PLAYER_WORK *w ) ;
void pause_yn_cursor_move( PLAYER_WORK *w ) ;

static PAUSE_PART pause_yn_part[4] = {
	{ 1, 0, 71, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 76, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 77, { 0.0f, 0.0f, 0.0f } },
	{ -1, 0, 79, { 0.0f, 0.0f, 0.0f } }
} ;
static VEC3 pause_yn_scale[1] = { { 1.0f, 1.0f, 1.0f } } ;
static FADE_COL pause_yn_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 pause_yn_pos[2] = { { 0.026f, 0.968f, -12.99f }, { 0.026f, -0.952f, -12.99f } } ;	/* on yes and no */

void pause_yn_win_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_yn_win_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_yn_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	w->fade = pause_yn_fade ;
	w->work2 = ( Sint32 )pause_yn_part ;
}

void pause_yn_cursor_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_yn_cursor_move ;
	w->pos = pause_yn_pos[training_work.cursor2] ;
	w->scale = pause_yn_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	w->fade = pause_yn_fade ;
	w->obj = obj_stg->obj[78] ;
	w->work2 = 0 ;
}

void pause_yn_win_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 5 ) work_free( w ) ;
}

/* it follows the answer and glows */
void pause_yn_cursor_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 5 ){
		work_free( w ) ;
		return ;
	}
	w->pos = pause_yn_pos[training_work.cursor2] ;
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}
