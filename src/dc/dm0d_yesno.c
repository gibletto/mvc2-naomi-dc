/* DM0D, a yes/no bar: it opens out, its cursor slides to the answer in yesno_ans, and it goes
   when the answer is -1. unk_8C212F16 is set while it moves. */
#include "player.h"
#include "dm0d.h"

void yesno_move( PLAYER_WORK *w ) ;
void yesno_init( PLAYER_WORK *w ) ;
void yesno_main( PLAYER_WORK *w ) ;
void yesno_open( PLAYER_WORK *w ) ;
void yesno_nop( PLAYER_WORK *w ) ;
void yesno_parts_set( PLAYER_WORK *mw, Uint8 n, Uint16 obj ) ;
void yesno_parts_move( PLAYER_WORK *w ) ;
void yesno_parts_init( PLAYER_WORK *w ) ;
void yesno_parts_main( PLAYER_WORK *w ) ;
void yesno_cursor( PLAYER_WORK *w ) ;
void yesno_cursor_wait( PLAYER_WORK *w ) ;
void yesno_cursor_slide( PLAYER_WORK *w ) ;
void yesno_off( PLAYER_WORK *w ) ;
void yesno_free( PLAYER_WORK *w ) ;

/****************************************************************/
/*			The bar, and its yes, its no and its cursor			*/
/****************************************************************/
void dm0d_yesno_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = yesno_move ;
		w->obj = obj_dm0d->obj[230] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE ;
		w->scale.x = 0.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
		w->pos.z = -10.5f ;
	}
	yesno_parts_set( w, 0, 0x10a ) ;
	yesno_parts_set( w, 1, 0x133 ) ;
	yesno_parts_set( w, 2, 0xe7 ) ;
}

void yesno_move( PLAYER_WORK *w )
{
	static void ( *yesno_jmp[4] )( PLAYER_WORK *w ) = { yesno_init, yesno_main, yesno_off, yesno_free } ;

	if( yesno_ans < 0 ) w->routine_0 = 2 ;
	yesno_jmp[w->routine_0]( w ) ;
}

void yesno_init( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
	w->timer0 = 20 ;
}

void yesno_main( PLAYER_WORK *w )
{
	static void ( *yesno_main_jmp[2] )( PLAYER_WORK *w ) = { yesno_open, yesno_nop } ;

	yesno_main_jmp[w->routine_1]( w ) ;
}

/* the bar widens over twenty frames */
void yesno_open( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		w->scale.x += 0.05f ;
	}
	if( !w->timer0 ){
		unk_8C212F16 = 0 ;
		w->scale.x = 1.0f ;
		w->routine_1 ++ ;
	}
}

void yesno_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The bar's parts, shown once it is open				*/
/****************************************************************/
void yesno_parts_set( PLAYER_WORK *mw, Uint8 n, Uint16 obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = yesno_parts_move ;
		w->obj_flag = OBJ_POS ;
		w->obj = obj_dm0d->obj[obj] ;
		w->master_w = mw ;
		w->base_mtx = &mw->obj_mtx ;
		w->sub_no = n ;
	}
}

void yesno_parts_move( PLAYER_WORK *w )
{
	static void ( *yesno_parts_jmp[4] )( PLAYER_WORK *w ) = {
		yesno_parts_init, yesno_parts_main, yesno_off, yesno_free
	} ;

	if( w->master_w->routine_0 >= 2 ) w->routine_0 = 2 ;
	yesno_parts_jmp[w->routine_0]( w ) ;
}

void yesno_parts_init( PLAYER_WORK *w )
{
	switch( w->sub_no ){
	case 0 :
		w->pos.y = 1.5f ;
		w->pos.z = 13.3f ;
		break ;
	case 1 :
		w->type = yesno_ans ;
		w->pos.x = 4.7f ;
		w->pos.y = 1.5f ;
		w->pos.z = 13.3f ;
		break ;
	case 2 :
		w->pos.z = 10.5f ;
		break ;
	}
	w->routine_0 ++ ;
}

void yesno_parts_main( PLAYER_WORK *w )
{
	if( w->master_w->routine_1 == 1 ){
		w->disp_flag = 1 ;
		if( w->sub_no == 1 ) yesno_cursor( w ) ;
	}
}

/* part 1 is the cursor; type keeps the answer it is on */
void yesno_cursor( PLAYER_WORK *w )
{
	static void ( *yesno_cursor_jmp[2] )( PLAYER_WORK *w ) = { yesno_cursor_wait, yesno_cursor_slide } ;

	yesno_cursor_jmp[w->routine_1]( w ) ;
}

void yesno_cursor_wait( PLAYER_WORK *w )
{
	if( w->type != yesno_ans ){
		w->routine_1 ++ ;
		unk_8C212F16 = 1 ;
		w->timer0 = 10 ;
	}
	w->type = yesno_ans ;
}

/* ten frames to the other answer */
void yesno_cursor_slide( PLAYER_WORK *w )
{
	if( w->timer0 ){
		w->timer0 -- ;
		if( yesno_ans ) w->pos.x += 4.7f / 10 ;
		else w->pos.x -= 4.7f / 10 ;
	}
	if( !w->timer0 ){
		w->routine_1 = 0 ;
		if( yesno_ans ) w->pos.x = 4.7f ;
		else w->pos.x = 0.0f ;
		unk_8C212F16 = 0 ;
	}
}

void yesno_off( PLAYER_WORK *w )
{
	w->disp_flag = 0 ;
	w->routine_0 ++ ;
}

void yesno_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}
