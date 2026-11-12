/* The training menu's window and cursor on the pause's list, shown while training_work.unk_1c is 2,
   and the routine that draws a work's part list (work2) for disp_pause. */
#include "player.h"
#include "nl.h"
#include "dm.h"

#define PART_LIST( w )	( ( PAUSE_PART * )( w )->work2 )

extern void scr_2d_matrix_set( void ) ;
extern int obj_scale_max() ;	/* handed the VEC3 itself */

void pause_win_move( PLAYER_WORK *w ) ;
void pause_cursor_move( PLAYER_WORK *w ) ;

static PAUSE_PART pause_win_part[3] = {
	{ 1, 0, 50, { 0.0f, 0.0f, 0.0f } },
	{ 1, 0, 3, { 0.0f, -0.0f, 0.0f } },
	{ -1, 0, 15, { 0.0f, 0.0f, 0.0f } }
} ;
VEC3 pause_scale[20] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0802f, 1.0f, 1.0f }, { 0.7225f, 1.0f, 1.0f }, { 1.2832f, 1.0f, 1.0f },
	{ 1.0728f, 1.0f, 1.0f }, { 1.2832f, 1.0f, 1.0f }, { 0.85f, 1.0f, 1.0f }, { 1.4685f, 1.0f, 1.0f },
	{ 1.5396f, 1.0f, 1.0f }, { 2.0551f, 1.0f, 1.0f }, { 2.0714f, 1.0f, 1.0f }, { 1.559f, 1.0f, 1.0f },
	{ 0.18f, 1.0f, 1.0f }, { 0.4078f, 1.0f, 1.0f }, { 1.93f, 0.7f, 1.0f }, { 2.79f, 0.7f, 1.0f },
	{ 3.97f, 0.7f, 1.0f }, { 3.2858f, 0.7f, 1.0f }, { 1.1666f, 1.0f, 1.0f }, { 0.85f, 1.0f, 1.0f }
} ;
static FADE_COL pause_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static VEC3 pause_cursor_pos[5] = {	/* on each line of the menu */
	{ 0.22f, 3.84f, -12.99f }, { 0.18f, 1.076f, -12.99f }, { 0.096f, -1.64f, -12.99f },
	{ -1.78f, -4.452f, -12.99f }, { -5.9f, -7.612f, -12.99f }
} ;
static Sint8 pause_cursor_scale[5] = { 1, 2, 0, 18, 19 } ;	/* and its width there, in pause_scale */

/****************************************************************/
/*			The window											*/
/****************************************************************/
void pause_win_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_win_move ;
	w->pos.x = 0.0f ;
	w->pos.y = 0.0f ;
	w->pos.z = 0.0f ;
	w->scale = pause_scale[0] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	w->fade = pause_fade ;
	w->work2 = ( Sint32 )pause_win_part ;
}

/****************************************************************/
/*			The cursor on the menu's line						*/
/****************************************************************/
void pause_cursor_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get2( 0, W_PAUSE, LIST_TOP ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = pause_cursor_move ;
	w->pos = pause_cursor_pos[training_work.cursor] ;
	w->scale = pause_scale[pause_cursor_scale[training_work.cursor]] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	w->fade = pause_fade ;
	w->obj = obj_stg->obj[8] ;
	w->work2 = 0 ;
}

void pause_win_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 2 ) work_free( w ) ;
}

/* it follows the line and glows */
void pause_cursor_move( PLAYER_WORK *w )
{
	if( training_work.unk_1c != 2 ){
		work_free( w ) ;
		return ;
	}
	w->pos = pause_cursor_pos[training_work.cursor] ;
	w->scale = pause_scale[pause_cursor_scale[training_work.cursor]] ;
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->timer1 = 0 ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

/****************************************************************/
/*			A work's part list, in its fade						*/
/****************************************************************/
void pause_obj_disp( PLAYER_WORK *w )
{
	PAUSE_PART *p ;

	if( !w->disp_flag ) return ;
	if( !( p = PART_LIST( w ) ) ) return ;
	scr_2d_matrix_set() ;
	nlPushMatrix( NULL ) ;
	while( p->kind >= 0 ){
		if( p->kind ){
			nlUnitMatrix() ;
			nlTranslate( p->pos.x, p->pos.y, p->pos.z ) ;
			if( p->scale ){
				nlScaleV( &pause_scale[p->scale] ) ;
				nlSetScaleFactor( obj_scale_max( pause_scale[p->scale] ) ) ;
			}
			nlObjPutSetFadeColor( w->fade.r, w->fade.g, w->fade.b ) ;
			nlObjPutTrnsl( obj_stg->obj[p->model], w->fade.trnsl ) ;
		}
		p ++ ;
	}
	nlPopMatrix( 1 ) ;
}
