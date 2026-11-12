/* DM0F, the second page of SECRET FACTOR: its parts, the cursor that slides down its five lines,
   the arrows lit while it can move, and the scrolled back. */
#include "player.h"
#include "nl.h"
#include "key.h"
#include "dm.h"

#define SLIDE_SPD( d )	( ( d ) + ( d ) / 30.0f * 1.5f )

void dm0f_page2_nop( PLAYER_WORK *w ) ;
void dm0f_page2_glow( PLAYER_WORK *w ) ;
void dm0f_page2_fade( int flag ) ;
void dm0f_page2_main( PLAYER_WORK *w ) ;
void dm0f_page2_wall( PLAYER_WORK *w ) ;

static Float dm0f_page2_y[5] = { 2.0f, 5.8f, 10.6f, 18.1f, 25.8f } ;	/* the cursor's lines */
static Sint32 dm0f_page2_obj[14] = {
	268, 263, 269, 264, 262, 266, 267, 267, 265, 260, 2, 6, 3, 4
} ;
static VEC3 dm0f_page2_pos[14] = {
	{ 0.05f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f }, { 1.5f, -7.2f, -11.7f }, { 0.0f, 4.55f, -12.7f }, { 0.0f, -7.76f, -12.7f },
	{ 9.7f, -6.0f, -12.7f }, { 0.0f, 0.0f, -13.13f }, { 0.0f, 0.0f, 0.015f }, { 0.0f, 0.0f, 0.033f },
	{ -8.95f, 6.5f, -13.01f }, { -8.95f, 6.5f, -13.0f }
} ;
static PLAYER_WORK *dm0f_page2_w[3] ;	/* the up and down arrows, and the part that fades in on line 2 */

void dm0f_page2_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			An arrow: it glows while routine_1 is set			*/
/****************************************************************/
void dm0f_page2_glow( PLAYER_WORK *w )
{
	if( !w->routine_1 ){
		w->timer1 = 0 ;
		w->fade.trnsl -= 0.1f ;
		if( w->fade.trnsl < 0.0f ) w->fade.trnsl = 0.0f ;
	} else {
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	}
}

/****************************************************************/
/*			The part shown while the cursor is on line 2		*/
/****************************************************************/
void dm0f_page2_fade( int flag )
{
	if( flag ){
		if( dm0f_page2_w[2]->fade.trnsl < 1.0f ) dm0f_page2_w[2]->fade.trnsl += 0.1f ;
	} else {
		if( dm0f_page2_w[2]->fade.trnsl > 0.0f ) dm0f_page2_w[2]->fade.trnsl -= 0.1f ;
	}
}

/****************************************************************/
/*			The cursor: up and down the five lines, sliding		*/
/*			half a sine wave from one to the next				*/
/****************************************************************/
void dm0f_page2_main( PLAYER_WORK *w )
{
	Uint16 sw ;
	Float d ;

	sw = menu_sw_get( 3 ) ;
	switch( w->routine_0 ){
	case 0 :
		w->pos.x = -10.5f ;
		w->pos.y = dm0f_page2_y[w->sub_no] ;
		w->pos.z = -13.12f ;
		w->routine_0 = 1 ;
		w->timer0 = 0 ;
		dm0f_page2_w[0]->routine_1 = 0 ;
		dm0f_page2_w[1]->routine_1 = 1 ;
		break ;
	case 1 :
		if( ( sw & KEY_DOWN ) && w->sub_no < 4 ){
			if( w->sub_no == 2 ) w->timer0 = 0 ;
			w->dir = 0 ;
			w->spd.y = SLIDE_SPD( dm0f_page2_y[w->sub_no + 1] - dm0f_page2_y[w->sub_no] ) ;
			goto start ;
		}
		if( ( sw & KEY_UP ) && w->sub_no ){
			if( w->sub_no == 2 ) w->timer0 = 0 ;
			w->dir = 1 ;
			w->spd.y = SLIDE_SPD( dm0f_page2_y[w->sub_no - 1] - dm0f_page2_y[w->sub_no] ) ;
			goto start ;
		}
		break ;
	case 2 :
		if( w->spd.x > 180.0f ){
			if( w->dir == 0 ) w->sub_no ++ ;
			else w->sub_no -- ;
			w->pos.y = dm0f_page2_y[w->sub_no] ;
			w->routine_0 = 1 ;
			if( w->sub_no == 0 ){
				dm0f_page2_w[0]->routine_1 = 0 ;
				dm0f_page2_w[1]->routine_1 = 1 ;
			} else if( w->sub_no == 4 ){
				dm0f_page2_w[0]->routine_1 = 1 ;
				dm0f_page2_w[1]->routine_1 = 0 ;
			} else {
				dm0f_page2_w[0]->routine_1 = 1 ;
				dm0f_page2_w[1]->routine_1 = 1 ;
			}
			if( w->sub_no == 2 ) w->timer0 = 1 ;
		} else {
			d = w->spd.y / 30.0f ;
			d *= nlSin( DEG_ANG( w->spd.x ) ) ;
			w->pos.y += d ;
			w->spd.x += 4.0f ;
		}
		break ;
	default :
		break ;
	start :
		na_req( 1 ) ;
		w->routine_0 = 2 ;
		w->spd.x = 0.0f ;
		dm0f_page2_w[0]->routine_1 = 0 ;
		dm0f_page2_w[1]->routine_1 = 0 ;
		break ;
	}
	dm0f_page2_fade( w->timer0 ) ;
}

/****************************************************************/
/*			The back, its texture scrolled						*/
/****************************************************************/
void dm0f_page2_wall( PLAYER_WORK *w )
{
	Float u, v ;

	w->timer0 ++ ;
	if( w->timer0 >= 1000 ) w->timer0 = 0 ;
	w->timer1 ++ ;
	if( w->timer1 >= 1000 ) w->timer1 = 0 ;
	obj_uv_begin( obj_dm0f->obj[261], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		u += w->timer0 * 0.001f ;
		v += w->timer1 * 0.001f ;
		obj_uv_put( &u, &v ) ;
	}
}

/****************************************************************/
/*			A part of the second page							*/
/****************************************************************/
void dm0f_page2_set( int n )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_page2_nop ;
	w->obj = obj_dm0f->obj[dm0f_page2_obj[n]] ;
	w->type = n ;
	w->obj_flag = OBJ_POS ;
	w->pos.x = dm0f_page2_pos[n].x ;
	w->pos.y = dm0f_page2_pos[n].y ;
	w->pos.z = dm0f_page2_pos[n].z ;
	switch( n ){
	case 2 :
		w->work_func = dm0f_page2_main ;
		break ;
	case 6 :
		EF_WK( w )->work[0] |= OBJ_TRNSL ;	/* obj_flag, through the work's own view (the ROM's code) */
		w->work_func = dm0f_page2_glow ;
		w->routine_1 = 0 ;
		dm0f_page2_w[0] = w ;
		break ;
	case 7 :
		w->obj_flag = w->obj_flag | OBJ_TRNSL | OBJ_ROT_Z ;
		w->rot[2] = DEG_ANG( 180 ) ;
		w->work_func = dm0f_page2_glow ;
		w->routine_1 = 1 ;
		dm0f_page2_w[1] = w ;
		break ;
	case 8 :
		w->obj_flag |= OBJ_TRNSL ;
		w->fade.trnsl = 0.0f ;
		dm0f_page2_w[2] = w ;
		break ;
	case 9 :
		w->work_func = dm0f_page2_wall ;
		break ;
	case 10 :
	case 11 :
		w->obj_flag = w->obj_flag | OBJ_FADE ;
		w->fade.r = 1.0f ;
		w->fade.g = 0.6f ;
		w->fade.b = 0.0f ;
		break ;
	case 12 :
	case 13 :
		w->obj_flag = w->obj_flag | OBJ_ROT_Z ;
		w->rot[2] = DEG_ANG( 15 ) ;
		break ;
	}
}
