/* DM0D, a selector: three boxes in a row with their labels, which rise in, follow the cursor and
   sink away again once one is chosen or the choice is cancelled. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern int decide_ck( void ) ;
extern int cancel_ck( void ) ;
extern int lr_select_loop( Uint16 sw, int no, int max ) ;
extern Uint16 menu_sw_get( Uint8 side_bit ) ;

void sel_move( PLAYER_WORK *w ) ;
void sel_box( PLAYER_WORK *w ) ;
void sel_box_init( PLAYER_WORK *w ) ;
void sel_box_main( PLAYER_WORK *w ) ;
void sel_label( PLAYER_WORK *w ) ;
void sel_label_init( PLAYER_WORK *w ) ;
void sel_label_main( PLAYER_WORK *w ) ;
void sel_ctrl( PLAYER_WORK *w ) ;
void sel_ctrl_init( PLAYER_WORK *w ) ;
void sel_ctrl_main( PLAYER_WORK *w ) ;
void sel_off( PLAYER_WORK *w ) ;
void sel_free( PLAYER_WORK *w ) ;

static Float sel_x[4] = { -7.0f, 0.0f, 7.0f, 7.0f } ;

/****************************************************************/
/*			The selector: the work that runs it, and its six	*/
/*			pieces behind it									*/
/****************************************************************/
#define SEL_ONE	0	/* a build setting: each row's pieces all its first */
PLAYER_WORK *dm0d_sel_set( void )
{
	PLAYER_WORK *mw, *w ;
	int i ;

	mw = 0 ;
	if( work_rest_cnt >= 7 ){
		unk_8C2143C2 = -1 ;
		unk_8C2143C4 = 0 ;
		mw = work_get( 0, W_SYS, LIST_END ) ;
		mw->be_flag = 1 ;
		mw->work_func = sel_move ;
		mw->type = 2 ;
		mw->sub_type = 0 ;
		for( i = 0 ; i < 6 ; i ++ ){
			w = work_get( mw, W_SYS, LIST_BEHIND ) ;
			w->work_func = sel_move ;
			w->master_w = mw ;
			w->type = i / 3 ;
			if( SEL_ONE ) w->sub_type = 0 ;	/* left out of the build */
			w->sub_type = i % 3 ;
		}
	}
	return mw ;
}

void sel_move( PLAYER_WORK *w )
{
	static void ( *sel_jmp[3] )( PLAYER_WORK *w ) = { sel_box, sel_label, sel_ctrl } ;

	sel_jmp[w->type]( w ) ;
}

/****************************************************************/
/*			A box												*/
/****************************************************************/
void sel_box( PLAYER_WORK *w )
{
	static void ( *sel_box_jmp[4] )( PLAYER_WORK *w ) = { sel_box_init, sel_box_main, sel_off, sel_free } ;

	if( w->master_w->routine_0 >= 2 && w->routine_0 < 2 ) w->routine_0 = 2 ;
	sel_box_jmp[w->routine_0]( w ) ;
}

void sel_box_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos.x = sel_x[w->sub_type] ;
	w->pos.y = -7.54f ;
	w->obj = obj_dm0d->obj[235] ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->obj_flag = OBJ_POS | OBJ_FADE ;
	w->dir = 0xff ;
	sel_box_main( w ) ;
}

/* the box the cursor is on pulses; the others are dimmer and further back the further away they are */
void sel_box_main( PLAYER_WORK *w )
{
	static VEC3 sel_box_col[3] = { { 0.6f, 1.0f, 1.0f }, { 0.4f, 0.8f, 0.9f }, { 0.2f, 0.6f, 0.8f } } ;
	static Float sel_box_z[3] = { -11.0f, -11.01f, -11.02f } ;
	int d ;

	d = w->master_w->dir - w->sub_type ;
	if( d < 0 ) d = -d ;
	if( w->dir != w->master_w->dir ){
		w->dir = w->master_w->dir ;
		w->pos.z = sel_box_z[d] ;
		*( VEC3 * )&w->fade.r = sel_box_col[d] ;
		w->timer1 = 0 ;
	}
	if( !d ){
		if( w->master_w->sub_no == 2 ) w->timer1 += 20 ;
		w->timer1 += 8 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.r = 0.8f - nlSin( DEG_ANG( w->timer1 ) ) * 0.2f ;
	} else if( w->master_w->sub_no == 2 ){
		if( w->pos.y > -10.54f ) w->pos.y -= 0.4f ;
		w->pos.z = sel_box_z[d] - 0.1f ;
	}
}

/****************************************************************/
/*			A box's label										*/
/****************************************************************/
void sel_label( PLAYER_WORK *w )
{
	static void ( *sel_label_jmp[4] )( PLAYER_WORK *w ) = { sel_label_init, sel_label_main, sel_off, sel_free } ;

	if( w->master_w->routine_0 >= 2 && w->routine_0 < 2 ) w->routine_0 = 2 ;
	sel_label_jmp[w->routine_0]( w ) ;
}

void sel_label_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 1 ;
	w->pos.x = sel_x[w->sub_type] ;
	w->pos.y = -7.0f ;
	w->pos.z = -10.99f ;
	w->base_mtx = &w->master_w->obj_mtx ;
	w->obj = obj_dm0d->obj[w->sub_type + 236] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	w->dir = 0xff ;
	sel_label_main( w ) ;
}

/* the chosen label swells and shrinks */
void sel_label_main( PLAYER_WORK *w )
{
	static VEC3 sel_label_col[2] = { { 1.0f, 1.0f, 0.5f }, { 0.9f, 0.4f, 0.0f } } ;
	int i ;

	if( w->dir != w->master_w->dir ){
		w->dir = w->master_w->dir ;
		if( w->master_w->dir == w->sub_type ){
			i = 0 ;
			w->timer1 = 0 ;
		} else {
			i = 1 ;
			w->scale.x = w->scale.y = w->scale.z = 1.0f ;
		}
		*( VEC3 * )&w->fade.r = sel_label_col[i] ;
	}
	w->dir = w->master_w->dir ;
	if( w->sub_type == w->master_w->dir ){
		if( ( w->timer1 += 8 ) >= 360 ) w->timer1 = 0 ;
		w->scale.x = w->scale.y = w->scale.z = nlCos( DEG_ANG( w->timer1 ) ) * 0.1f + 1.1f ;
	} else if( w->master_w->sub_no == 2 ){
		if( w->pos.y > -10.0f ) w->pos.y -= 0.4f ;
		w->pos.z = -11.09f ;
	}
}

/****************************************************************/
/*			The work that runs the selector: dir is the box		*/
/*			the cursor is on, sub_no the step					*/
/****************************************************************/
void sel_ctrl( PLAYER_WORK *w )
{
	static void ( *sel_ctrl_jmp[4] )( PLAYER_WORK *w ) = { sel_ctrl_init, sel_ctrl_main, sel_off, sel_free } ;

	sel_ctrl_jmp[w->routine_0]( w ) ;
}

void sel_ctrl_init( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 0 ;
	w->pos.x = w->pos.y = w->pos.z = 0.0f ;
	w->pos.y = -3.0f ;
	w->obj = obj_dm0d->obj[235] ;
	w->master_w = w ;
	w->obj_flag = OBJ_POS ;
	w->master_w->dir = 0 ;
	w->master_w->sub_no = 0 ;
	sel_ctrl_main( w ) ;
	na_req( 57 ) ;
}

void sel_ctrl_main( PLAYER_WORK *w )
{
	switch( w->master_w->sub_no ){
	case 0 :
		w->pos.y += 0.4f ;
		if( w->pos.y >= 0.0f ){
			w->pos.y = 0.0f ;
			w->master_w->sub_no = 1 ;
		} else if( cancel_ck() || unk_8C21429C ){
			w->master_w->sub_no = 3 ;
		}
		break ;
	case 1 :
		if( cancel_ck() || unk_8C21429C ){
			w->master_w->sub_no = 3 ;
		} else if( decide_ck() ){
			w->timer0 = 30 ;
			w->master_w->sub_no = 2 ;
			unk_8C2143C4 = w->master_w->dir + 1 ;
		} else {
			w->master_w->dir = lr_select_loop( menu_sw_get( 3 ), w->master_w->dir, 2 ) ;
		}
		break ;
	case 2 :
		if( -- w->timer0 < 0 ) w->master_w->sub_no = 3 ;
		break ;
	case 3 :
		if( ( w->pos.y -= 0.4f ) <= -3.0f ){
			w->routine_0 ++ ;
			unk_8C2143C2 = 0 ;
		}
		break ;
	}
}

void sel_off( PLAYER_WORK *w )
{
	w->routine_0 ++ ;
	w->disp_flag = 0 ;
}

void sel_free( PLAYER_WORK *w )
{
	w->be_flag = 0 ;
	work_free( w ) ;
}
