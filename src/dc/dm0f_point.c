/* DM0F, the top screen's numbers: the card's level, the time played and the points. */
#include "player.h"
#include "nl.h"

void dm0f_num_set( int num, int keta, VEC3 *pos, Float dx, Float r, Float g, Float b, int type ) ;
void dm0f_num_move( PLAYER_WORK *w ) ;

static VEC3 dm0f_num_scale = { 0.5f, 0.65f, 1.0f } ;
static VEC3 dm0f_level_pos = { 3.6f, 5.2f, -12.69f } ;
static VEC3 dm0f_hour_pos = { 1.6f, 3.11f, -12.69f } ;
static VEC3 dm0f_min_pos = { 3.0f, 3.11f, -12.69f } ;
static VEC3 dm0f_sec_pos = { 4.4f, 3.11f, -12.69f } ;
static VEC3 dm0f_pts_pos = { 3.6f, 1.1f, -12.69f } ;
static VEC3 dm0f_line_pos[5] = {	/* five lines down from the points; not used */
	{ 3.6f, 1.1f, -12.69f }, { 4.0f, 0.0f, -12.69f }, { 4.4f, -1.0f, -12.69f }, { 4.8f, -2.05f, -12.69f },
	{ 5.2f, -3.15f, -12.69f }
} ;

/****************************************************************/
/*			The top screen's numbers							*/
/****************************************************************/
void dm0f_point_set( void )
{
	dm0f_num_set( CS_Secret_Work.vm_level, 2, &dm0f_level_pos, 0.5f, 0.0f, 1.0f, 1.0f, 0 ) ;
	dm0f_num_set( CS_Secret_Work.play_hour, 2, &dm0f_hour_pos, 0.4f, 0.0f, 1.0f, 1.0f, 1 ) ;
	dm0f_num_set( CS_Secret_Work.play_min, 2, &dm0f_min_pos, 0.4f, 0.0f, 1.0f, 1.0f, 2 ) ;
	dm0f_num_set( CS_Secret_Work.play_sec, 2, &dm0f_sec_pos, 0.4f, 0.0f, 1.0f, 1.0f, 3 ) ;
	dm0f_num_set( CS_Secret_Work.d_pts, 4, &dm0f_pts_pos, 0.7f, 1.0f, 0.0f, 0.0f, 0 ) ;
}

/****************************************************************/
/*			A number of keta digits from pos, dx apart, in the	*/
/*			colour r, g, b; a time (type 1 to 3) keeps its		*/
/*			first 0s and goes on counting						*/
/****************************************************************/
void dm0f_num_set( int num, int keta, VEC3 *pos, Float dx, Float r, Float g, Float b, int type )
{
	PLAYER_WORK *w ;
	int i, n, f ;

	f = 0 ;
	for( i = 0 ; i < keta ; i ++ ){
		n = num / ( int )nlPow( 10.0f, ( Float )( keta - i - 1 ) ) ;
		if( !( type || n || f || i == keta - 1 ) ) continue ;
		f = 1 ;
		n %= 10 ;
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) break ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_num_move ;
		w->obj = obj_dm0f->obj[n + 37] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		w->type = type ;
		w->sub_type = i ;
		w->pos = *pos ;
		w->pos.x += i * dx ;
		w->scale = dm0f_num_scale ;
		w->fade.r = r ;
		w->fade.g = g ;
		w->fade.b = b ;
	}
}

/****************************************************************/
/*			A digit of the time, as the time goes on			*/
/****************************************************************/
void dm0f_num_move( PLAYER_WORK *w )
{
	SECRET_WORK *sw = &CS_Secret_Work ;
	int n ;

	if( w->type ){
		switch( w->type ){
		case 1 :
			n = sw->play_hour / ( int )nlPow( 10.0f, ( Float )( 1 - w->sub_type ) ) ;
			break ;
		case 2 :
			n = sw->play_min / ( int )nlPow( 10.0f, ( Float )( 1 - w->sub_type ) ) ;
			break ;
		case 3 :
			n = sw->play_sec / ( int )nlPow( 10.0f, ( Float )( 1 - w->sub_type ) ) ;
			break ;
		}
		n %= 10 ;
		w->obj = obj_dm0f->obj[n + 37] ;
	}
}
