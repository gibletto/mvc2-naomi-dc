/* DM0F, the first page of SECRET FACTOR: the characters opened, an icon each, one picked from the
   list with its picture and number, and the lines of what was bought, with a cursor. */
#include "player.h"
#include "nl.h"
#include "key.h"
#include "dm.h"

#define BUFF_W( b )	( ( PLAYER_WORK * )( b ) )	/* a work of the pool */
#define SUB_W( w )	( ( PLAYER_WORK * )( w )->work2 )	/* an icon's shine, the cursor's arrow */

static Uint8 dm0f_page1_order[56] = {	/* the characters in the list's order */
	23, 22, 6, 11, 8, 10, 15, 57, 9, 12, 13, 14, 17, 16, 19, 20,
	21, 18, 34, 5, 2, 0, 1, 4, 47, 40, 41, 44, 45, 42, 7, 48,
	31, 30, 55, 3, 28, 29, 56, 27, 43, 53, 49, 50, 46, 51, 54, 52,
	39, 33, 37, 38, 35, 36, 32, 58
} ;
static Float dm0f_page1_y[7] = { -1.5f, -2.2f, -2.9f, -3.6f, -4.3f, -5.0f, -5.7f } ;	/* the cursor's lines */
static Sint32 dm0f_page1_obj[6] = { 167, 168, 58, 203, 37, 37 } ;
static Float dm0f_page1_pos1[6][3] = {	/* with the lines of what was bought */
	{ 6.0f, 1.8f, -13.0f }, { 0.0f, 0.0f, 0.0f }, { 4.7f, 4.5f, -12.88f },
	{ 6.0f, 3.3f, -12.8f }, { 7.3f, 4.2f, -12.8f }, { 6.5f, 4.2f, -12.8f }
} ;
static Float dm0f_page1_pos0[6][3] = {	/* without them */
	{ 6.0f, -1.2f, -13.0f }, { 0.0f, -3.0f, 0.0f }, { 4.7f, 1.5f, -12.88f },
	{ 6.0f, 0.3f, -12.8f }, { 7.3f, 1.3f, -12.8f }, { 6.5f, 1.3f, -12.8f }
} ;
static Sint32 dm0f_page1_part_obj[4] = { 3, 4, 169, 180 } ;
static Float dm0f_page1_part_pos[4][3] = {
	{ 6.05f, -7.65f, -13.1f }, { 6.0f, -7.6f, -13.0f }, { 5.2f, -6.46f, -11.0f }, { 0.0f, 0.0f, 0.0f }
} ;
static Float dm0f_page1_part_scale[4][2] = { { 1.0f, 0.3f }, { 1.0f, 0.3f }, { 1.2f, 1.2f }, { 1.0f, 1.0f } } ;
static Sint8 dm0f_page1_icon[8 * 8] = {	/* the icons' places, eight rows: a character, 0x80 hidden until opened; -1 none */
	-1, -96, 92, 120, 71, 109, -74, -1,
	-1, -93, 2, 21, 6, 57, -82, -1,
	-89, 95, 0, 20, 22, 8, 111, -85,
	-95, 94, 1, 19, 23, 9, 104, -75,
	-91, 119, 4, 18, 11, 12, 105, -79,
	-90, 67, 34, 16, 15, 14, 108, -78,
	-1, -92, 5, 17, 10, 13, -77, -1,
	-1, -70, 93, 91, 112, 106, -76, -1
} ;
static PLAYER_WORK *dm0f_page1_w[20] ;	/* the page's parts; [13] the cursor */
static PLAYER_WORK *dm0f_page1_icon_w[56] ;	/* each character's icon */

void dm0f_page1_nop( PLAYER_WORK *w )
{
}

/****************************************************************/
/*			An icon: it grows and shrinks while it is picked	*/
/****************************************************************/
void dm0f_page1_icon_zoom( PLAYER_WORK *w )
{
	PLAYER_WORK *w2 ;

	if( w->routine_0 ){
		w2 = SUB_W( w ) ;
		if( !w->routine_1 ){
			w->scale.x += 0.3f / 30.0f ;
			w->scale.y += 0.3f / 30.0f ;
			if( w2 ){
				w2->scale.x += 0.4f / 30.0f ;
				w2->scale.y += 0.4f / 30.0f ;
			}
			if( w->scale.x >= 1.3f ){
				w->scale.x = w->scale.y = 1.3f ;
				if( w2 ) w2->scale.x = w2->scale.y = 1.4f ;
				w->routine_1 = 1 ;
			}
		} else {
			w->scale.x -= 0.3f / 30.0f ;
			w->scale.y -= 0.3f / 30.0f ;
			if( w2 ){
				w2->scale.x -= 0.4f / 30.0f ;
				w2->scale.y -= 0.4f / 30.0f ;
			}
			if( w->scale.x <= 1.01f ){
				w->scale.x = w->scale.y = 1.01f ;
				if( w2 ) w2->scale.x = w2->scale.y = 1.01f ;
				w->routine_1 = 0 ;
			}
		}
	}
}

/****************************************************************/
/*			The list: on to the next character opened			*/
/****************************************************************/
void dm0f_page1_step( PLAYER_WORK *w, int dir )
{
	Uint32 *open ;
	int i, n ;

	for( i = 0 ; i < 56 ; i ++ ){
		if( dir ){
			w->routine_1 -- ;
			if( w->routine_1 == 0xff ) w->routine_1 = 55 ;
		} else {
			w->routine_1 ++ ;
			if( w->routine_1 == 56 ) w->routine_1 = 0 ;
		}
		n = dm0f_page1_order[w->routine_1] ;
		if( n >= 32 ){
			n -= 32 ;
			open = &Sys->char_open1 ;
		} else open = &Sys->char_open0 ;
		if( *open & ( 1 << n ) ) return ;
	}
	w->routine_1 = w->routine_2 ;
}

/****************************************************************/
/*			The list: left and right through the characters		*/
/*			opened, ten at a time with light kick held			*/
/****************************************************************/
void dm0f_page1_main( PLAYER_WORK *w )
{
	PLAYER_WORK *cw, *w2 ;
	Uint32 *open ;
	Uint16 sw ;
	int i, cnt ;
	int no[2] ;

	sw = menu_sw_get( 3 ) ;
	cw = dm0f_page1_w[13] ;
	w2 = SUB_W( dm0f_page1_w[13] ) ;
	if( cw->sub_no ){
		if( cw->routine_1 ){
			w2->disp_flag = 0 ;
			return ;
		}
	} else if( cw->routine_1 == 7 ){
		w2->disp_flag = 0 ;
		return ;
	}
	w2->disp_flag = 1 ;
	if( !w->routine_0 ){
		w->routine_0 = 1 ;
		w->routine_1 = 0 ;
		w->routine_2 = 1 ;
	} else {
		w->routine_2 = w->routine_1 ;
		if( sw & KEY_LK ) cnt = 10 ;
		else cnt = 1 ;
		if( sw & KEY_RIGHT ){
			for( i = 0 ; i < cnt ; i ++ ) dm0f_page1_step( w, 0 ) ;
			na_req( 1 ) ;
		}
		if( sw & KEY_LEFT ){
			for( i = 0 ; i < cnt ; i ++ ) dm0f_page1_step( w, 1 ) ;
			na_req( 1 ) ;
		}
	}
	if( w->routine_1 == w->routine_2 ) return ;
	no[0] = dm0f_page1_order[w->routine_1] ;
	if( no[0] >= 24 ) no[0] -= 3 ;
	no[1] = dm0f_page1_order[w->routine_2] ;
	if( no[1] >= 24 ) no[1] -= 3 ;
	w2 = dm0f_page1_icon_w[no[0]] ;
	w2->routine_0 = w2->routine_1 = 1 ;
	w2->fade.r = w2->fade.g = w2->fade.b = 1.0f ;
	w2->pos.z = -12.85f ;
	w2->scale.x = w2->scale.y = 1.3f ;
	w2 = SUB_W( dm0f_page1_icon_w[no[0]] ) ;
	if( w2 ){
		w2->pos.z = -12.845f ;
		w2->scale.x = w2->scale.y = 1.4f ;
	}
	w2 = dm0f_page1_icon_w[no[1]] ;
	w2->routine_0 = 0 ;
	w2->fade.r = w2->fade.g = w2->fade.b = 0.8f ;
	w2->pos.z = -12.88f ;
	w2->scale.x = w2->scale.y = 1.0f ;
	w2 = SUB_W( dm0f_page1_icon_w[no[1]] ) ;
	if( w2 ){
		w2->pos.z = -12.86f ;
		w2->scale.x = w2->scale.y = 1.0f ;
	}
	dm0f_page1_w[2]->obj = obj_dm0f->obj[no[0] + 58] ;
	dm0f_page1_w[3]->obj = obj_dm0f->obj[no[0] + 203] ;
	dm0f_page1_w[4]->obj = obj_dm0f->obj[( w->routine_1 + 1 ) % 10 + 37] ;
	dm0f_page1_w[5]->obj = obj_dm0f->obj[( w->routine_1 + 1 ) / 10 + 37] ;
	if( no[0] >= 24 ) no[0] += 3 ;
	if( no[0] >= 32 ){
		no[0] -= 32 ;
		open = &Sys->char_open_all1 ;
	} else open = &Sys->char_open_all0 ;
	if( *open & ( 1 << no[0] ) ) dm0f_page1_w[1]->disp_flag = 1 ;
	else dm0f_page1_w[1]->disp_flag = 0 ;
}

/****************************************************************/
/*			The shine on the icon of a character opened twice	*/
/****************************************************************/
void dm0f_page1_move( PLAYER_WORK *w )
{
	Float u, v ;

	w->timer0 ++ ;
	if( w->timer0 >= 50 ) w->timer0 = 0 ;
	obj_uv_begin( obj_dm0f->obj[201], w->obj ) ;
	while( !obj_uv_next() ){
		obj_uv_get( &u, &v ) ;
		u += w->timer0 * 0.02f ;
		v += w->timer0 * 0.02f ;
		obj_uv_put( &u, &v ) ;
	}
}

/****************************************************************/
/*			The cursor: up and down the lines of what was		*/
/*			bought, or onto the way back; it glows				*/
/****************************************************************/
void dm0f_page1_cursor( PLAYER_WORK *w )
{
	WORK_BUFF *lw ;	/* the seven lines' works, made one after another */
	Uint16 sw ;
	int i ;

	sw = menu_sw_get( 3 ) ;
	if( !w->sub_no ){
		w->pos.x = 2.86f ;
		w->pos.z = -12.87f ;
		if( !w->routine_0 ){
			w->routine_1 = 0 ;
			w->routine_0 = 1 ;
		} else {
			if( ( sw & KEY_UP ) && w->routine_1 ){
				w->routine_1 -- ;
				w->timer1 = -9 ;
				na_req( 1 ) ;
			}
			if( ( sw & KEY_DOWN ) && w->routine_1 != 7 ){
				w->routine_1 ++ ;
				w->timer1 = -9 ;
				na_req( 1 ) ;
			}
		}
		if( w->routine_1 == 7 ){
			w->disp_flag = 1 ;
			w->pos.x = 6.02f ;
			w->pos.y = -7.62f ;
			w->pos.z = -13.05f ;
			w->scale.x = 1.0f ;
			w->scale.y = 0.3f ;
			dm0f_page1_w[16]->disp_flag = 0 ;
			dm0f_page1_w[0]->routine_3 = 0 ;
			dm0f_page1_w[19]->fade.r = dm0f_page1_w[19]->fade.g = dm0f_page1_w[19]->fade.b = 1.0f ;
		} else {
			w->disp_flag = 1 ;
			w->pos.y = dm0f_page1_y[w->routine_1] ;
			w->scale.x = 0.7f ;
			w->scale.y = 0.2f ;
			dm0f_page1_w[15]->obj = obj_dm0f->obj[w->routine_1 + 191] ;
			if( vms_work.stage_ex_cnt >= w->routine_1 + 1 ) dm0f_page1_w[15]->disp_flag = 1 ;
			else dm0f_page1_w[15]->disp_flag = 0 ;
			dm0f_page1_w[16]->disp_flag = 1 ;
			dm0f_page1_w[16]->pos.x = w->pos.x + -1.58f ;
			dm0f_page1_w[16]->pos.y = w->pos.y ;
			dm0f_page1_w[16]->pos.z = w->pos.z + 0.1f ;
			dm0f_page1_w[19]->fade.r = dm0f_page1_w[19]->fade.g = dm0f_page1_w[19]->fade.b = 0.8f ;
		}
		lw = ( WORK_BUFF * )dm0f_page1_w[6] ;
		for( i = 0 ; i < 7 ; lw ++, i ++ ){
			BUFF_W( lw )->obj_flag = OBJ_FADE ;
			if( i == w->routine_1 ){
				BUFF_W( lw )->fade.r = 1.0f ;
				BUFF_W( lw )->fade.g = 0.8f ;
				BUFF_W( lw )->fade.b = 0.2f ;
			} else if( i + 1 <= vms_work.stage_ex_cnt ){
				BUFF_W( lw )->fade.r = 0.4f ;
				BUFF_W( lw )->fade.g = BUFF_W( lw )->fade.b = 1.0f ;
			} else {
				BUFF_W( lw )->fade.r = 0.4f ;
				BUFF_W( lw )->fade.g = 0.2f ;
				BUFF_W( lw )->fade.b = 0.5f ;
			}
		}
	} else {
		if( !w->routine_0 ){
			w->routine_1 = 0 ;
			w->routine_0 = 1 ;
		} else {
			if( ( sw & KEY_UP ) && w->routine_1 ){
				w->routine_1 -- ;
				w->timer1 = -9 ;
				na_req( 1 ) ;
			}
			if( ( sw & KEY_DOWN ) && w->routine_1 != 1 ){
				w->routine_1 ++ ;
				w->timer1 = -9 ;
				na_req( 1 ) ;
			}
		}
		if( w->routine_1 ){
			w->disp_flag = 1 ;
			w->pos.x = 6.02f ;
			w->pos.y = -7.62f ;
			w->pos.z = -13.05f ;
			w->scale.x = 1.0f ;
			w->scale.y = 0.3f ;
			dm0f_page1_w[0]->routine_3 = 0 ;
			dm0f_page1_w[19]->fade.r = dm0f_page1_w[19]->fade.g = dm0f_page1_w[19]->fade.b = 1.0f ;
		} else {
			w->disp_flag = 0 ;
			dm0f_page1_w[19]->fade.r = dm0f_page1_w[19]->fade.g = dm0f_page1_w[19]->fade.b = 0.8f ;
		}
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

/****************************************************************/
/*			An arrow beside the list, glowing					*/
/****************************************************************/
void dm0f_page1_arrow( PLAYER_WORK *w )
{
	PLAYER_WORK *aw ;

	if( w->sub_no ){
		w->timer1 += 3 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.3f ;
		aw = SUB_W( dm0f_page1_w[13] ) ;
		w->disp_flag = aw->disp_flag ;
	} else {
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.trnsl = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
	}
}

/****************************************************************/
/*			The page's parts; flag shows the lines of what was	*/
/*			bought. It returns the cursor						*/
/****************************************************************/
PLAYER_WORK *dm0f_page1_set( int flag )
{
	PLAYER_WORK *w ;
	Float *p, *s ;
	int i, j, n ;

	if( flag ) p = dm0f_page1_pos1[0] ;
	else p = dm0f_page1_pos0[0] ;
	for( i = 0 ; i < 6 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return w ;
		dm0f_page1_w[i] = w ;
		n = dm0f_page1_obj[i] ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_page1_nop ;
		w->obj = obj_dm0f->obj[n] ;
		w->type = n ;
		w->obj_flag = OBJ_POS ;
		w->pos.x = *p ++ ;
		w->pos.y = *p ++ ;
		w->pos.z = *p ++ ;
	}
	if( flag ){
		for( j = 181 ; j < 190 ; j ++, i ++ ){
			if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return w ;
			dm0f_page1_w[i] = w ;
			w->disp_flag = 1 ;
			w->work_func = dm0f_page1_nop ;
			w->obj = obj_dm0f->obj[j] ;
			w->type = j ;
			w->obj_flag = 0 ;
			if( j >= 181 && j <= 187 ){
				w->obj_flag = OBJ_FADE ;
				if( j - 180 <= vms_work.stage_ex_cnt ){
					w->fade.r = 0.4f ;
					w->fade.g = w->fade.b = 1.0f ;
				} else {
					w->fade.r = 0.4f ;
					w->fade.g = 0.2f ;
					w->fade.b = 0.5f ;
				}
			}
		}
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return w ;
		dm0f_page1_w[i] = w ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_page1_nop ;
		w->obj = obj_dm0f->obj[191] ;
		w->type = j ;
		w->obj_flag = 0 ;
		dm0f_page1_w[13]->work_func = dm0f_page1_cursor ;
		dm0f_page1_w[13]->sub_no = 0 ;
		dm0f_page1_w[13]->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		dm0f_page1_w[13]->scale.x = 0.7f ;
		dm0f_page1_w[13]->scale.y = 0.2f ;
		dm0f_page1_w[13]->fade.trnsl = 0.0f ;
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return w ;
		dm0f_page1_w[16] = w ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_page1_nop ;
		w->obj = obj_dm0f->obj[36] ;
		w->type = j ;
		w->obj_flag = OBJ_POS | OBJ_ROT_Z | OBJ_SCALE ;
		w->rot[2] = DEG_ANG( 270 ) ;
		w->scale.x = w->scale.y = 0.4f ;
	} else {
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return w ;
		dm0f_page1_w[13] = w ;
		w->disp_flag = 1 ;
		w->obj = obj_dm0f->obj[188] ;
		w->work_func = dm0f_page1_cursor ;
		w->sub_no = 1 ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
		w->scale.x = 0.7f ;
		w->scale.y = 0.2f ;
		w->fade.trnsl = 0.0f ;
	}
	p = dm0f_page1_part_pos[0] ;
	s = dm0f_page1_part_scale[0] ;
	for( i = 0 ; i < 4 ; i ++ ){
		if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return w ;
		n = dm0f_page1_part_obj[i] ;
		w->disp_flag = 1 ;
		w->work_func = dm0f_page1_nop ;
		w->obj = obj_dm0f->obj[n] ;
		w->type = n ;
		w->obj_flag = OBJ_POS | OBJ_SCALE ;
		w->pos.x = *p ++ ;
		w->pos.y = *p ++ ;
		w->pos.z = *p ++ ;
		w->scale.x = *s ++ ;
		w->scale.y = *s ++ ;
		if( i == 2 ){
			w->obj_flag |= OBJ_FADE ;
			dm0f_page1_w[19] = w ;
		}
	}
	dm0f_page1_w[0]->work_func = dm0f_page1_main ;
	dm0f_page1_w[2]->obj_flag = OBJ_POS | OBJ_SCALE ;
	dm0f_page1_w[2]->scale.x = dm0f_page1_w[2]->scale.y = 1.0f ;
	dm0f_page1_w[4]->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	dm0f_page1_w[4]->scale.x = dm0f_page1_w[4]->scale.y = 0.8f ;
	dm0f_page1_w[4]->fade.r = dm0f_page1_w[4]->fade.g = 1.0f ;
	dm0f_page1_w[4]->fade.b = 0.0f ;
	dm0f_page1_w[5]->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	dm0f_page1_w[5]->scale.x = dm0f_page1_w[5]->scale.y = 0.8f ;
	dm0f_page1_w[5]->fade.r = dm0f_page1_w[5]->fade.g = 1.0f ;
	dm0f_page1_w[5]->fade.b = 0.0f ;
	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 1 ;
		w->sub_no = 0 ;
		w->work_func = dm0f_page1_arrow ;
		w->obj = obj_dm0f->obj[179] ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL ;
		w->pos.y = flag ? 1.8f : 1.8f - 3.0f ;
		w->pos.x = 6.0f ;
		w->pos.z = -13.0f ;
		w->fade.trnsl = 0.0f ;
		dm0f_page1_w[13]->work2 = ( Sint32 )w ;
	}
	if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
		w->disp_flag = 0 ;
		w->sub_no = 1 ;
		w->work_func = dm0f_page1_arrow ;
		w->obj = obj_dm0f->obj[166] ;
		w->obj_flag = OBJ_POS | OBJ_TRNSL ;
		w->pos.y = flag ? 2.8f : 2.8f - 3.0f ;
		w->pos.x = 6.0f ;
		w->pos.z = -12.89f ;
	}
	if( 0 ){	/* a check left out of the build */
		if( Sys ) if( Sys ) ;
	}
	return dm0f_page1_w[13] ;
}

/****************************************************************/
/*			The icons, eight rows of eight: a picture for each	*/
/*			character opened, a shine on one opened twice		*/
/****************************************************************/
void dm0f_page1_icon_set( void )
{
	PLAYER_WORK *w ;
	Uint32 *open, *all ;
	Float x, y ;
	int i, j, c, no, bit, m ;

	y = 3.25f ;
	for( i = 0 ; i < 8 ; y -= 1.5f, i ++ ){
		x = -11.24f ;
		for( j = 0 ; j < 8 ; x += 1.5f, j ++ ){
			c = dm0f_page1_icon[i * 8 + j] ;
			if( c != -1 ){
				if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
				bit = no = c & 0x3f ;
				if( bit >= 32 ){
					bit -= 32 ;
					open = &Sys->char_open1 ;
					all = &Sys->char_open_all1 ;
				} else {
					open = &Sys->char_open0 ;
					all = &Sys->char_open_all0 ;
				}
				if( *open & ( 1 << bit ) ) m = 0 ;
				else m = 199 ;
				if( no >= 24 ) c -= 3 ;
				dm0f_page1_icon_w[c & 0x3f] = w ;
				w->type = i * 8 + j ;
				w->sub_no = c ;
				w->disp_flag = 1 ;
				w->work_func = dm0f_page1_icon_zoom ;
				if( !m ) m = ( c & 0x3f ) + 58 ;
				w->obj = obj_dm0f->obj[m] ;
				w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
				w->pos.x = x ;
				w->pos.y = y ;
				w->pos.z = -12.88f ;
				w->fade.r = w->fade.g = w->fade.b = 0.8f ;
				w->scale.x = w->scale.y = 1.0f ;
				if( m == 199 && ( c & 0x80 ) ) w->disp_flag = 0 ;
				if( m != 199 ){
					if( *all & ( 1 << bit ) ){
						if( !( w = work_get( 0, W_SYS, LIST_END ) ) ) return ;
						w->disp_flag = 1 ;
						w->work_func = dm0f_page1_move ;
						w->obj = obj_dm0f->obj[200] ;
						w->obj_flag = OBJ_POS | OBJ_SCALE ;
						w->pos.x = x ;
						w->pos.y = y ;
						w->pos.z = -12.86f ;
						w->scale.x = w->scale.y = 1.0f ;
						dm0f_page1_icon_w[c & 0x3f]->work2 = ( Sint32 )w ;
						obj_uv_clear( w->obj ) ;
					} else dm0f_page1_icon_w[c & 0x3f]->work2 = 0 ;
				}
			}
		}
	}
}
