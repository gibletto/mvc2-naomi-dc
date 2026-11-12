/* DM11, the memory card messages at boot: message no's lines, with the card's port drawn after
   two of them and one line blinking. */
#include "player.h"

void dm11_msg_move( PLAYER_WORK *w ) ;
void dm11_msg_blink( PLAYER_WORK *w ) ;
PLAYER_WORK *dm11_msg_sub_set( void ) ;

/* each message's lines, models of obj_dm11, -1 at the end */
static int dm11_msg_0[3] = { 0, 1, -1 } ;
static int dm11_msg_1[4] = { 0, 1, 2, -1 } ;
static int dm11_msg_2[3] = { 0, 9, -1 } ;
static int dm11_msg_3[7] = { 0, 10, 11, 12, 13, 26, -1 } ;
static int dm11_msg_4[5] = { 0, 14, 15, 2, -1 } ;
static int dm11_msg_5[3] = { 0, 16, -1 } ;
static int dm11_msg_6[6] = { 0, 17, 18, 19, 13, -1 } ;
static int dm11_msg_7[8] = { 0, 20, 10, 11, 21, 22, 13, -1 } ;
static int dm11_msg_8[8] = { 0, 23, 10, 11, 21, 22, 13, -1 } ;
static int dm11_msg_9[6] = { 0, 24, 18, 25, 13, -1 } ;

/* and their heights, by the number of lines */
static Float dm11_msg_y2[2] = { 0.0f, 5.0f } ;
static Float dm11_msg_y3[3] = { 0.0f, 5.0f, -6.0f } ;
static Float dm11_msg_y6[6] = { 0.0f, 5.0f, 3.0f, 1.0f, -2.0f, -6.0f } ;
static Float dm11_msg_y4[4] = { 0.0f, 5.0f, 3.0f, -6.0f } ;
static Float dm11_msg_y5[5] = { 0.0f, 5.0f, 3.0f, 1.0f, -6.0f } ;
static Float dm11_msg_y7[7] = { 0.0f, 5.0f, 3.0f, 1.0f, -1.0f, -3.0f, -6.0f } ;

static int *dm11_msg_tbl[10] = {
	dm11_msg_0, dm11_msg_1, dm11_msg_2, dm11_msg_3, dm11_msg_4,
	dm11_msg_5, dm11_msg_6, dm11_msg_7, dm11_msg_8, dm11_msg_9
} ;
static Float *dm11_msg_y[10] = {
	dm11_msg_y2, dm11_msg_y3, dm11_msg_y2, dm11_msg_y6, dm11_msg_y4,
	dm11_msg_y2, dm11_msg_y5, dm11_msg_y7, dm11_msg_y7, dm11_msg_y5
} ;
static int dm11_msg_port[16] = {	/* each port's letter and number */
	3, 7, 3, 8, 4, 7, 4, 8, 5, 7, 5, 8, 6, 7, 6, 8
} ;

void dm11_msg_move( PLAYER_WORK *w )
{
	w->disp_flag = 1 ;
}

/* on for 60 frames, off for 30 */
void dm11_msg_blink( PLAYER_WORK *w )
{
	w->timer0 ++ ;
	if( w->disp_flag ){
		if( w->timer0 == 60 ){
			w->disp_flag = 0 ;
			w->timer0 = 0 ;
		}
	} else if( w->timer0 == 30 ){
		w->disp_flag = 1 ;
		w->timer0 = 0 ;
	}
}

PLAYER_WORK *dm11_msg_sub_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm11_msg_move ;
		w->obj_flag = OBJ_POS ;
	}
	return w ;
}

/****************************************************************/
/*			Message no, for the card in port					*/
/****************************************************************/
void dm11_msg_set( int no, int port )
{
	PLAYER_WORK *w ;
	int *p ;
	Float *y ;
	int obj ;

	work_list_kill( W_SYS ) ;
	p = dm11_msg_tbl[no] ;
	y = dm11_msg_y[no] ;
	while( ( obj = *p ) != -1 ){
		if( ( w = dm11_msg_sub_set() ) ){
			w->obj = obj_dm11->obj[obj] ;
			w->type = obj ;
			w->pos.x = 0.0f ;
			w->pos.y = *y ;
			w->pos.z = -13.0f ;
			if( obj == 2 || obj == 26 ){
				if( ( w = dm11_msg_sub_set() ) ){
					w->obj = obj_dm11->obj[dm11_msg_port[port * 2]] ;
					if( no == 3 ){
						w->pos.x = 1.73f ;
						w->pos.y = -6.0f ;
						w->pos.z = -13.0f ;
					} else {
						w->pos.x = 4.932f ;
						w->pos.y = -6.0f ;
						w->pos.z = -13.0f ;
					}
					w->type = 3 ;
				}
				if( ( w = dm11_msg_sub_set() ) ){
					w->obj = obj_dm11->obj[dm11_msg_port[port * 2 + 1]] ;
					if( no == 3 ){
						w->pos.x = 2.55f ;
						w->pos.y = -6.0f ;
						w->pos.z = -13.0f ;
					} else {
						w->pos.x = 5.752f ;
						w->pos.y = -6.0f ;
						w->pos.z = -13.0f ;
					}
					w->type = 3 ;
				}
			} else if( obj == 13 ){
				w->pos.z += 0.1f ;
				w->work_func = dm11_msg_blink ;
			}
		}
		p ++ ;
		y ++ ;
	}
}
