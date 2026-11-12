/* DM0F, the shop (dm0f_30): the goods on sale, their prices and pictures, the points left, the
   cursor and the yes/no window. */
#include "player.h"
#include "nl.h"
#include "key.h"
#include "dm.h"

extern int sprintf( char *s, const char *fmt, ... ) ;
extern int decide_ck_nose( void ) ;
extern int cancel_ck_nose( void ) ;

void dm0f_30_goods_off( int no ) ;

static Float dm0f_30_num_x[6] = { -9.15f, -1.8199999f, 5.69f, -6.95f, -6.45f, -5.95f } ;
static Float dm0f_30_num_col[3 * 3] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.7f, 1.0f } ;
static Float dm0f_30_num_scale[2 * 2] = { 0.8f, 0.6f, 0.8f, 0.6f } ;
static Sint32 dm0f_30_obj[5] = { 3, 4, 169, 160, 178 } ;
static Float dm0f_30_pos[5][3] = {
	{ 0.0f, -2.75f, -13.1f }, { 0.0f, -2.75f, -13.0f }, { 0.01f, -2.37f, -11.0f }, { 0.0f, 0.0f, 0.0f },
	{ -0.225f, 0.0f, 0.0f }
} ;
static Uint8 dm0f_30_order[56] = {	/* the characters in the list's order, as the first page's */
	23, 22, 6, 11, 8, 10, 15, 57, 9, 12, 13, 14, 17, 16, 19, 20,
	21, 18, 34, 5, 2, 0, 1, 4, 47, 40, 41, 44, 45, 42, 7, 48,
	31, 30, 55, 3, 28, 29, 56, 27, 43, 53, 49, 50, 46, 51, 54, 52,
	39, 33, 37, 38, 35, 36, 32, 58
} ;
static Float dm0f_30_char_x[3 * 2] = { -8.9f, -7.5f, -1.4f, 0.0f, 6.1f, 7.5f } ;
static Float dm0f_30_no_x[3] = { -6.9f, 0.59999996f, 8.099999f } ;
static Float dm0f_30_frame_x[3] = { -7.5f, 0.0f, 7.5f } ;
static Float dm0f_30_subwin_pos[4][3] = {
	{ 0.0f, 0.9f, -12.5f }, { 0.0f, -0.9f, -12.5f }, { 0.0f, -0.9f, -12.6f }, { 0.0f, 0.0f, -12.7f }
} ;
static PLAYER_WORK *dm0f_30_w[4][20] ;	/* the three goods' works, then the points' and the window's */

void dm0f_30_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			The message: its model picked by sub_no				*/
/****************************************************************/
void dm0f_30_msg_move( PLAYER_WORK *w )
{
	w->obj = obj_dm0f->obj[w->sub_no + 170] ;
}

/****************************************************************/
/*			The yes/no window: it opens, takes up and down		*/
/*			and the answer (work3), and closes					*/
/****************************************************************/
void dm0f_30_subwin( PLAYER_WORK *w )
{
	Uint16 sw ;

	sw = menu_sw_get( 3 ) ;
	switch( w->routine_0 ){
	case 0 :
		break ;
	case 1 :
		w->disp_flag = 1 ;
		w->scale.x += 0.2f ;
		if( w->scale.x >= 1.0f ){
			w->scale.x = 1.0f ;
			dm0f_30_w[3][12]->disp_flag = 1 ;
			dm0f_30_w[3][13]->disp_flag = 1 ;
			dm0f_30_w[3][14]->disp_flag = 1 ;
			w->routine_0 ++ ;
			w->routine_1 = 1 ;
			dm0f_30_w[3][14]->pos.y = -0.9f ;
			return ;
		}
		break ;
	case 2 :
		if( ( sw & KEY_DOWN ) && !w->routine_1 ){
			w->routine_1 = 1 ;
			dm0f_30_w[3][14]->pos.y = -0.9f ;
			dm0f_30_w[3][14]->fade.trnsl = 1.0f ;
			dm0f_30_w[3][14]->timer1 = -9 ;
			na_req( 1 ) ;
		}
		if( ( sw & KEY_UP ) && w->routine_1 ){
			w->routine_1 = 0 ;
			dm0f_30_w[3][14]->pos.y = 0.9f ;
			dm0f_30_w[3][14]->fade.trnsl = 1.0f ;
			dm0f_30_w[3][14]->timer1 = -9 ;
			na_req( 1 ) ;
		}
		if( decide_ck_nose() ){
			w->work3 = 1 ;
			goto close ;
		}
		if( cancel_ck_nose() ){
			w->work3 = 0 ;
			w->routine_1 = 1 ;
			goto close ;
		}
		break ;
	case 3 :
		w->scale.x -= 0.2f ;
		if( w->scale.x <= 0.0f ){
			w->scale.x = 0.0f ;
			w->routine_0 = 0 ;
		}
		break ;
	close :
		dm0f_30_w[3][12]->disp_flag = 0 ;
		dm0f_30_w[3][13]->disp_flag = 0 ;
		dm0f_30_w[3][14]->disp_flag = 0 ;
		w->routine_0 ++ ;
		break ;
	}
}

/****************************************************************/
/*			The cursor: it glows								*/
/****************************************************************/
void dm0f_30_cursor( PLAYER_WORK *w )
{
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	if( w->fade.trnsl >= 1.0f ) w->fade.trnsl = 0.99f ;
}

/****************************************************************/
/*			A number of up to four digits, right-aligned at		*/
/*			num_x[pos], in colour col and size scl				*/
/****************************************************************/
void dm0f_30_num_set( PLAYER_WORK **wp, int pos, int num, int col, Float y, int scl )
{
	PLAYER_WORK *w ;
	Float x, sx, sy ;
	Float *c ;
	char buf[12], *s ;
	int i, n, d ;

	x = dm0f_30_num_x[pos] ;
	c = &dm0f_30_num_col[col * 3] ;
	sx = dm0f_30_num_scale[scl * 2] ;
	sy = dm0f_30_num_scale[scl * 2 + 1] ;
	i = sprintf( buf, "%d", num ) ;
	n = 4 - i ;
	if( n < 0 ){
		s = buf - n ;
		n = 0 ;
	} else s = buf ;
	x += n * 0.7f ;
	for( i = n ; i < 4 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		d = *s ++ - '0' ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_30_nop ;
		w->obj = obj_dm0f->obj[d + 37] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		w->pos.x = x ;
		w->pos.y = y ;
		w->pos.z = -12.8f ;
		w->fade.r = c[0] ;
		w->fade.g = c[1] ;
		w->fade.b = c[2] ;
		w->scale.x = sx ;
		w->scale.y = sy ;
		*wp ++ = w ;
		x += 0.7f ;
	}
}

/****************************************************************/
/*			The shop's frame and the points left; it returns	*/
/*			the part that fades									*/
/****************************************************************/
PLAYER_WORK *dm0f_30_set( void )
{
	PLAYER_WORK *w, *fw ;
	Float *p ;
	int i ;

	p = dm0f_30_pos[0] ;
	for( i = 0 ; i < 5 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_30_nop ;
		w->obj = obj_dm0f->obj[dm0f_30_obj[i]] ;
		w->obj_flag = OBJ_POS ;
		w->pos.x = *p ++ ;
		w->pos.y = *p ++ ;
		w->pos.z = *p ++ ;
		if( i <= 1 ){
			w->obj_flag |= OBJ_SCALE ;
			w->scale.x = 1.0f ;
			w->scale.y = 0.3f ;
		}
		if( i == 2 ){
			w->obj_flag |= OBJ_FADE ;
			fw = w ;
		}
	}
	dm0f_30_num_set( dm0f_30_w[3], 4, CS_Secret_Work.d_pts, 0, -5.99f, 1 ) ;
	return fw ;
}

/****************************************************************/
/*			The points left, again after a purchase				*/
/****************************************************************/
void dm0f_30_pts_set( void )
{
	int i ;

	for( i = 0 ; i < 12 ; i ++ ){
		if( dm0f_30_w[3][i] ) work_free( dm0f_30_w[3][i] ) ;
	}
	dm0f_30_num_set( dm0f_30_w[3], 4, CS_Secret_Work.d_pts, 0, -5.99f, 1 ) ;
}

/****************************************************************/
/*			A character on sale: its picture, its name and its	*/
/*			number in the list									*/
/****************************************************************/
void dm0f_30_char_set( PLAYER_WORK **wp, int pos, int c )
{
	PLAYER_WORK *w ;
	Float *p ;
	Float x ;
	int i, n, chr ;

	p = &dm0f_30_char_x[pos * 2] ;
	x = dm0f_30_no_x[pos] ;
	chr = c ;
	if( c >= 24 ) c -= 3 ;
	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_30_nop ;
	w->obj = obj_dm0f->obj[c + 58] ;
	w->obj_flag = OBJ_POS ;
	w->pos.x = *p ++ ;
	w->pos.y = 2.7f ;
	w->pos.z = -12.88f ;
	*wp ++ = w ;
	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_30_nop ;
	w->obj = obj_dm0f->obj[c + 203] ;
	w->obj_flag = OBJ_POS ;
	w->pos.x = *p ;
	w->pos.y = 1.5f ;
	w->pos.z = -12.8f ;
	*wp ++ = w ;
	for( i = 0 ; i < 56 ; i ++ ){
		c = dm0f_30_order[i] ;
		if( c == chr ) break ;
	}
	n = i + 1 ;
	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_30_nop ;
	w->obj = obj_dm0f->obj[n / 10 + 37] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	w->pos.x = x ;
	w->pos.y = 2.5f ;
	w->pos.z = -12.8f ;
	w->scale.x = w->scale.y = 0.8f ;
	w->fade.r = w->fade.g = 1.0f ;
	w->fade.b = 0.0f ;
	*wp ++ = w ;
	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 1 ;
	w->work_func = dm0f_30_nop ;
	w->obj = obj_dm0f->obj[n % 10 + 37] ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	x += 0.8f ;
	w->pos.x = x ;
	w->pos.y = 2.5f ;
	w->pos.z = -12.8f ;
	w->scale.x = w->scale.y = 0.8f ;
	w->fade.r = w->fade.g = 1.0f ;
	w->fade.b = 0.0f ;
	*wp = w ;
}

/****************************************************************/
/*			A good's frame, hidden until it is sold out			*/
/****************************************************************/
void dm0f_30_frame_set( PLAYER_WORK **wp, int pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 0 ;
	w->work_func = dm0f_30_nop ;
	w->obj = obj_dm0f->obj[164] ;
	w->obj_flag = OBJ_POS ;
	w->pos.x = dm0f_30_frame_x[pos] ;
	w->pos.y = 0.0f ;
	w->pos.z = -13.0f ;
	*wp ++ = w ;
	if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
	w->disp_flag = 0 ;
	w->work_func = dm0f_30_nop ;
	w->obj = obj_dm0f->obj[165] ;
	w->type = pos ;
	w->obj_flag = OBJ_POS ;
	w->pos.x = dm0f_30_frame_x[pos] ;
	w->pos.y = 1.0f ;
	w->pos.z = -12.88f ;
	*wp = w ;
}

/****************************************************************/
/*			The first good: a character (flag) or an extra		*/
/****************************************************************/
void dm0f_30_shop1_set( int flag, int goods, Sint16 pts )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_30_nop ;
		w->obj = obj_dm0f->obj[1 - flag + 162] ;
		w->type = flag ;
		w->obj_flag = OBJ_POS ;
		w->pos.x = -7.5f ;
		w->pos.y = 0.0f ;
		w->pos.z = -13.0f ;
		dm0f_30_w[0][0] = w ;
	} else dm0f_30_w[0][0] = NULL ;
	if( !flag ){
		if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
			w->disp_flag = 1 ;
			w->work_func = dm0f_30_nop ;
			w->obj = obj_dm0f->obj[goods + 37] ;
			w->type = flag ;
			w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
			w->scale.x = w->scale.y = 0.9f ;
			w->pos.x = -7.6f ;
			w->pos.y = 0.8f ;
			w->pos.z = -12.8f ;
			w->fade.r = w->fade.g = 1.0f ;
			w->fade.b = 0.0f ;
			dm0f_30_w[0][1] = w ;
		} else dm0f_30_w[0][1] = NULL ;
		dm0f_30_num_set( &dm0f_30_w[0][2], 0, pts, 0, -0.5f, 0 ) ;
	} else {
		dm0f_30_num_set( &dm0f_30_w[0][2], 0, pts, 0, -0.8f, 0 ) ;
		dm0f_30_char_set( &dm0f_30_w[0][6], 0, goods ) ;
	}
	dm0f_30_frame_set( &dm0f_30_w[0][18], 0 ) ;
	if( goods == -1 ) dm0f_30_goods_off( 0 ) ;
}

/****************************************************************/
/*			The second and third goods, both characters			*/
/****************************************************************/
void dm0f_30_shop2_set( int no, int goods, Sint16 *pts )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_30_nop ;
		w->obj = obj_dm0f->obj[161] ;
		w->type = no ;
		w->obj_flag = OBJ_POS ;
		w->pos.x = 0.0f ;
		w->pos.y = 0.0f ;
		w->pos.z = -13.0f ;
		if( no ) w->pos.x += 7.5f ;
		dm0f_30_w[no + 1][0] = w ;
	} else dm0f_30_w[no + 1][0] = NULL ;
	dm0f_30_char_set( &dm0f_30_w[no + 1][13], no + 1, goods ) ;
	dm0f_30_num_set( &dm0f_30_w[no + 1][1], no + 1, *pts, 0, -0.225f, 0 ) ;
	dm0f_30_frame_set( &dm0f_30_w[no + 1][18], no + 1 ) ;
	if( goods == -1 ) dm0f_30_goods_off( no + 1 ) ;
}

/****************************************************************/
/*			The yes/no window; it returns its back				*/
/****************************************************************/
PLAYER_WORK *dm0f_30_subwin_set( void )
{
	PLAYER_WORK *w ;
	Float *p ;
	int i ;

	p = dm0f_30_subwin_pos[0] ;
	for( i = 0 ; i < 4 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return NULL ;
		w->disp_flag = 0 ;
		w->obj = obj_dm0f->obj[i + 114] ;
		w->work_func = dm0f_30_nop ;
		w->obj_flag = OBJ_POS ;
		w->pos.x = *p ++ ;
		w->pos.y = *p ++ ;
		w->pos.z = *p ++ ;
		if( i == 3 ){
			w->obj_flag |= OBJ_SCALE ;
			w->scale.x = 0.0f ;
			w->scale.y = 1.0f ;
			w->work_func = dm0f_30_subwin ;
		}
		dm0f_30_w[3][i + 12] = w ;
	}
	EF_WK( dm0f_30_w[3][14] )->work[0] |= OBJ_TRNSL ;	/* obj_flag, through the work's own view (the ROM's code) */
	dm0f_30_w[3][14]->fade.trnsl = 1.0f ;
	return dm0f_30_w[3][15] ;
}

/****************************************************************/
/*			The cursor and the arrow it keeps in work2			*/
/****************************************************************/
PLAYER_WORK *dm0f_30_cursor_set( void )
{
	PLAYER_WORK *w, *w2 ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_30_cursor ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		w->fade.trnsl = 0.0f ;
		w->timer0 = 30 ;
		if( ( w2 = work_get( 0, W_SYS, LIST_END ) ) ){
			w2->disp_flag = 1 ;
			w2->obj = obj_dm0f->obj[179] ;
			w2->work_func = dm0f_30_cursor ;
			w2->obj_flag = OBJ_POS | OBJ_TRNSL ;
			w2->fade.trnsl = 0.0f ;
			w2->timer0 = 20 ;
		}
		w->work2 = ( Sint32 )w2 ;
	}
	return w ;
}

/****************************************************************/
/*			The message under the goods							*/
/****************************************************************/
PLAYER_WORK *dm0f_30_msg_set( void )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->work_func = dm0f_30_msg_move ;
		w->obj_flag = 0 ;
	}
	return w ;
}

/****************************************************************/
/*			A good sold out: its parts hidden, its frame shown	*/
/****************************************************************/
void dm0f_30_goods_off( int no )
{
	PLAYER_WORK *w ;
	int i ;

	for( i = 0 ; i < 18 ; i ++ ){
		w = dm0f_30_w[no][i] ;
		if( w ) w->disp_flag = 0 ;
	}
	for( ; i < 20 ; i ++ ){
		w = dm0f_30_w[no][i] ;
		if( w ) w->disp_flag = 1 ;
	}
}

void dm0f_30_shop_init( void )
{
	int i, j ;

	for( i = 0 ; i < 4 ; i ++ ){
		for( j = 0 ; j < 20 ; j ++ ) dm0f_30_w[i][j] = NULL ;
	}
}
