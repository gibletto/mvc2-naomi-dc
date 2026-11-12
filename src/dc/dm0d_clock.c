/* DM0D, the lamps that show how the line to the server is, the frame around the player's ID, and
   the clock of the time spent in the lobby. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

extern int sprintf( char *buf, const char *fmt, ... ) ;
extern void str_wide_set( char *src, char *dst ) ;

void clock_move( PLAYER_WORK *w ) ;
void clock_init( PLAYER_WORK *w ) ;
void clock_main( PLAYER_WORK *w ) ;
void clock_off( PLAYER_WORK *w ) ;
void clock_free( PLAYER_WORK *w ) ;
void clock_top_init( PLAYER_WORK *w ) ;
void clock_base_init( PLAYER_WORK *w ) ;
void clock_mark_init( PLAYER_WORK *w ) ;
void clock_sign_init( PLAYER_WORK *w ) ;
void clock_dot_init( PLAYER_WORK *w ) ;
void clock_model_init( PLAYER_WORK *w ) ;
void clock_id_put( PLAYER_WORK *w ) ;
void clock_top_main( PLAYER_WORK *w ) ;
void clock_base_main( PLAYER_WORK *w ) ;
void clock_mark_main( PLAYER_WORK *w ) ;
void clock_sign_main( PLAYER_WORK *w ) ;
void clock_dot_main( PLAYER_WORK *w ) ;
void clock_tip_main( PLAYER_WORK *w ) ;
void clock_model_main( PLAYER_WORK *w ) ;
void clock_put( PLAYER_WORK *w ) ;

static VEC3 clock_pos = { 0.0f, 0.0f, 0.0f } ;
static VEC3 clock_scale = { 1.0f, 1.0f, 1.0f } ;
static FADE_COL clock_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static Uint16 clock_sign_obj[4] = { 7, 0x33, 0x0a, 0 } ;
static Float clock_dot_x[4] = { 3.32f, 4.08f, 4.85f, 5.62f } ;
static Float clock_dot_x2[4] = { 2.07f, 2.86f, 3.63f, 4.4f } ;

/****************************************************************/
/*			A piece: sub_no is its kind							*/
/****************************************************************/
void clock_work_set( PLAYER_WORK *w, Sint8 no, Sint8 type, Sint8 sub_type )
{
	w->disp_flag = 1 ;
	w->work_func = clock_move ;
	w->sub_no = no ;
	w->type = type ;
	w->sub_type = sub_type ;
	w->scale = clock_scale ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->fade = clock_fade ;
}

/****************************************************************/
/*			A lamp, its two bases and its marks					*/
/****************************************************************/
PLAYER_WORK *dm0d_lamp_set( Sint8 type )
{
	PLAYER_WORK *mw, *w ;
	int i, j, n ;

	if( ( mw = work_get( 0, W_SYS, LIST_TOP ) ) ){
		clock_work_set( mw, 0, type, 0 ) ;
		mw->master_w = mw ;
		for( i = 0 ; i < 2 ; i ++ ){
			w = work_get( mw, W_SYS, LIST_BEHIND ) ;
			clock_work_set( w, 1, i, 0 ) ;
			w->master_w = mw ;
			w->base_mtx = &mw->obj_mtx ;
		}
		if( !type ){
			i = 0 ;
			n = 4 ;
		} else {
			i = 1 ;
			n = 3 ;
		}
		for( j = i ; j < n ; j ++ ){
			w = work_get( mw, W_SYS, LIST_BEHIND ) ;
			clock_work_set( w, 2, j, 0 ) ;
			w->master_w = mw ;
			w->base_mtx = &mw->obj_mtx ;
		}
	}
	return mw ;
}

/****************************************************************/
/*			The frame around the ID								*/
/****************************************************************/
void dm0d_id_set( void )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return ;
	clock_work_set( w, 6, 0, 0 ) ;
	w->ef_no = 60 ;
	w->pos.z = -13.3f ;
	w = work_get( 0, W_SYS, LIST_TOP ) ;
	clock_work_set( w, 6, 0, 0 ) ;
	w->ef_no = 59 ;
	w->pos.z = -13.2f ;
	w = work_get( 0, W_SYS, LIST_TOP ) ;
	clock_work_set( w, 6, 0, 0 ) ;
	w->ef_no = 61 ;
	w->pos.x = -13.0f ;
	w = work_get( 0, W_SYS, LIST_TOP ) ;
	clock_work_set( w, 6, 0, 0 ) ;
	w->ef_no = 0x89 ;
	w = work_get( 0, W_SYS, LIST_TOP ) ;
	clock_work_set( w, 6, 0, 0 ) ;
	w->ef_no = 0x8a ;
	w = work_get( 0, W_SYS, LIST_TOP ) ;
	clock_work_set( w, 6, 0, 0 ) ;
	w->ef_no = 0x81 ;
	w->pos.z = -185.0f ;
	w = work_get( 0, W_SYS, LIST_TOP ) ;
	clock_work_set( w, 7, 0, 0 ) ;
}

/****************************************************************/
/*			Each kind's first frame								*/
/****************************************************************/
void clock_top_init( PLAYER_WORK *w )
{
	PLAYER_WORK *cw ;
	int i ;

	w->routine_0 ++ ;
	w->disp_flag = 0 ;
	w->dir = 0 ;
	cw = work_get( w, W_SYS, LIST_BEHIND ) ;
	clock_work_set( cw, 3, w->type, 0 ) ;
	cw->master_w = w ;
	for( i = 0 ; i < 4 ; i ++ ){
		cw = work_get( w, W_SYS, LIST_BEHIND ) ;
		clock_work_set( cw, 4, i, 0 ) ;
		cw->master_w = w ;
	}
	if( !w->type ){
		cw = work_get( w, W_SYS, LIST_BEHIND ) ;
		clock_work_set( cw, 5, w->type, 0 ) ;
		cw->master_w = w ;
		cw->ef_no = 78 ;
		cw->fade.trnsl = 0.0f ;
		cw->timer0 = 16 ;
		cw->timer1 = 0 ;
	}
}

void clock_base_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->pos.x = 0.0f ;
	w->pos.y = -6.73f ;
	w->pos.z = -14.0f ;
	if( w->type ) w->pos.z -= 0.1f ;
	w->scale.x = 1.12f ;
	w->scale.y = 1.12f ;
	w->obj = obj_dm0d->obj[w->type + 12] ;
}

void clock_mark_init( PLAYER_WORK *w )
{
	int n ;

	w->routine_0 ++ ;
	n = w->type ;
	if( n != 3 ) n += 2 ;
	else n = 6 ;
	w->obj = obj_dm0d->obj[n] ;
}

void clock_sign_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->obj = obj_dm0d->obj[clock_sign_obj[w->type]] ;
}

void clock_dot_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->obj = obj_dm0d->obj[52] ;
	if( w->master_w->type == 2 ){
		w->pos.x = clock_dot_x2[w->type] ;
	} else {
		w->pos.x = clock_dot_x[w->type] ;
	}
	w->pos.y = 1.1f ;
	w->pos.z = -12.53f ;
	w->timer0 = ( w->type + 1 ) * 16 ;
	w->timer1 = ( 5 - w->type ) * 16 ;
	w->fade.trnsl = 0.0f ;
	w->dir = 0 ;
}

void clock_model_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->obj = obj_dm0d->obj[w->ef_no] ;
}

/* the player's ID, in wide letters */
void clock_id_put( PLAYER_WORK *w )
{
	char buf[32] ;

	w->disp_flag = 0 ;
	str_wide_set( ( char * )net_file_key, buf ) ;
	str_put( 98.0f, 205.0f, 12.8f, 0xff000000, buf ) ;
}

void clock_init( PLAYER_WORK *w )
{
	static void ( *clock_init_jmp[8] )( PLAYER_WORK *w ) = {
		clock_top_init, clock_base_init, clock_mark_init, clock_sign_init,
		clock_dot_init, clock_model_init, clock_model_init, clock_id_put
	} ;

	clock_init_jmp[w->sub_no]( w ) ;
}

/****************************************************************/
/*			Each kind every frame								*/
/****************************************************************/
void clock_top_main( PLAYER_WORK *w )
{
}

void clock_base_main( PLAYER_WORK *w )
{
}

void clock_mark_main( PLAYER_WORK *w )
{
	if( !unk_8C212F20 && w->master_w->type == 1 ) w->disp_flag = 0 ;
}

/* the sign shows whether the line is busy or broken */
void clock_sign_main( PLAYER_WORK *w )
{
	switch( w->routine_1 ){
	case 0 :
		if( !unk_8C212F20 ){
			if( w->type == 1 ){
				w->obj = obj_dm0d->obj[53] ;
			} else {
				w->obj = obj_dm0d->obj[8] ;
			}
			w->routine_1 = 2 ;
		}
		if( unk_8C213705 < 0 ){
			if( w->type == 1 ){
				w->obj = obj_dm0d->obj[51] ;
				w->timer0 = 60 ;
				w->routine_1 = 1 ;
			} else {
				w->obj = obj_dm0d->obj[9] ;
				w->routine_1 = 2 ;
			}
		}
		break ;
	case 1 :
		if( -- w->timer0 < 0 && w->type ) w->obj = obj_dm0d->obj[52] ;
		break ;
	}
}

/* the dots light and go out in turn while the server is asked */
void clock_dot_main( PLAYER_WORK *w )
{
	if( unk_8C212F20 && unk_8C213705 >= 0 ){
		if( !w->dir ){
			if( -- w->timer0 < 0 ){
				w->timer0 = ( w->type + 1 ) * 16 ;
				w->dir ^= 1 ;
				w->fade.trnsl = 1.0f ;
			}
		} else {
			if( -- w->timer1 < 0 ){
				w->timer1 = ( 5 - w->type ) * 16 ;
				w->dir ^= 1 ;
				w->fade.trnsl = 0.0f ;
			}
		}
	} else {
		w->routine_0 = 2 ;
	}
}

/* a tip, one after another every five seconds */
void clock_tip_main( PLAYER_WORK *w )
{
	static Uint16 clock_tip_obj[5] = { 0x4e, 0x4d, 0x50, 0x15, 0x4f } ;

	if( w->timer1 < 5 ){
		if( -- w->timer0 < 0 ){
			w->fade.trnsl = 1.0f ;
			w->timer0 = 300 ;
			w->obj = obj_dm0d->obj[clock_tip_obj[w->timer1]] ;
			w->timer1 ++ ;
		}
	}
}

void clock_model_main( PLAYER_WORK *w )
{
}

void clock_main( PLAYER_WORK *w )
{
	static void ( *clock_main_jmp[8] )( PLAYER_WORK *w ) = {
		clock_top_main, clock_base_main, clock_mark_main, clock_sign_main,
		clock_dot_main, clock_tip_main, clock_model_main, clock_id_put
	} ;

	clock_main_jmp[w->sub_no]( w ) ;
}

void clock_off( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 0 ;
}

void clock_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}

/* the pieces go when their lamp goes */
void clock_move( PLAYER_WORK *w )
{
	static void ( *clock_jmp[4] )( PLAYER_WORK *w ) = { clock_init, clock_main, clock_off, clock_free } ;

	if( w->sub_no < 6 ){
		if( w->master_w->routine_0 >= 2 ) w->routine_0 = 2 ;
	}
	clock_jmp[w->routine_0]( w ) ;
}

/****************************************************************/
/*			The clock of the time in the lobby					*/
/****************************************************************/
void dm0d_clock_set( int type )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 1 ;
		w->work_func = clock_put ;
		w->sub_no = 0 ;
		w->type = type ;
		w->sub_type = 0 ;
		w->scale = clock_scale ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
		w->fade = clock_fade ;
		w->obj = obj_dm0d->obj[228] ;
	}
}

#define CLOCK_TEST	0	/* a build setting: the clock is set from its own hours */
void clock_put( PLAYER_WORK *w )
{
	char buf[16] ;
	int h, m, s ;

	h = lobby_time / ( 60 * 60 * 60 ) ;
	if( h > 99 ) h = 99 ;
	if( CLOCK_TEST ) lobby_time = h ;	/* left out of the build */
	m = lobby_time % ( 60 * 60 * 60 ) / ( 60 * 60 ) ;
	s = lobby_time % ( 60 * 60 ) / 60 ;
	sprintf( buf, "%02d:%02d:%02d", h, m, s ) ;
	if( !w->type ){
		str_put( 21.0f, 431.0f, 11.04f, 0xffffffff, buf ) ;
	} else {
		w->pos.y = 11.2f ;
		str_put( 21.0f, 107.0f, 11.04f, 0xffffffff, buf ) ;
	}
}
