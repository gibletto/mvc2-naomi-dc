/* DM0D, the board of the two players' records: their names, wins and losses, with pieces that
   pulse, scroll and follow whose record is shown. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a piece of a board: its kind (the jump below), what it shows, its model and its place */
typedef struct BOARD_PARTS {
	Uint16	type;
	Uint16	sub_type;
	Uint16	obj;
	Uint16	pos;
} BOARD_PARTS;

PLAYER_WORK *board_parts_set( int type, int sub_type, int obj, VEC3 *pos ) ;
void board_move( PLAYER_WORK *w ) ;
void board_main( PLAYER_WORK *w ) ;
void board_cursor( PLAYER_WORK *w ) ;
void board_scroll( PLAYER_WORK *w ) ;
void board_num_put( int num, int max, int flag, VEC3 *pos, FADE_COL *fade ) ;

static VEC3 board_scale = { 1.0f, 1.0f, 1.0f } ;
static VEC3 board_scale_big = { 1.12f, 1.12f, 1.0f } ;
static VEC3 board_scale_face = { 0.73f, 0.73f, 1.0f } ;
static VEC3 board_scale_unk = { 0.6f, 0.6f, 1.0f } ;
static VEC3 board_scale_unk2 = { 0.8f, 0.8f, 1.0f } ;
static VEC3 board_scale_num = { 0.8f, 0.8f, 1.0f } ;
static VEC3 board_scale_wide = { 1.3f, 1.25f, 1.0f } ;
static FADE_COL board_fade = { 1.0f, 1.0f, 1.0f, 1.0f } ;
static FADE_COL board_fade_dark = { 1.0f, 0.1f, 0.1f, 0.1f } ;
static FADE_COL board_fade_rec = { 1.0f, 0.0f, 0.7f, 0.0f } ;
static FADE_COL board_fade_cnt = { 1.0f, 0.0f, 0.7f, 1.0f } ;
static FADE_COL board_fade_pt = { 1.0f, 0.0f, 0.65f, 1.0f } ;
static BOARD_PARTS board_list0[] = {
	{ 0, 0, 0x5d, 0 }, { 2, 0, 0x5e, 0 }, { 0, 2, 0x61, 0 }, { 0, 4, 0x74, 0 },
	{ 0, 0x10, 0x73, 1 }, { 0, 0x11, 0x73, 2 }, { 0, 0x20, 0x5f, 0 }, { 0, 0x21, 0x60, 0 },
	{ 0, 0x50, 0x62, 7 }, { 0, 0x51, 0x62, 7 }, { 0, 0x70, 0x65, 8 }, { 0, 0x71, 0x65, 9 },
	{ 0, 0x81, 0x0d, 0x0c }, { 0, 0x82, 0xb5, 0x0d }, { 0, 0x83, 0xbd, 0x0e }, { 0, 0x84, 0xbf, 0x0f },
	{ 0xff, 0xff, 0xffff, 0xffff },
} ;
static BOARD_PARTS board_list1[] = {
	{ 0, 0, 0x5d, 0 }, { 2, 0, 0x5e, 0 }, { 0, 2, 0x61, 0 }, { 0, 3, 0x64, 0x12 },
	{ 0, 4, 0x74, 0 }, { 0, 0x10, 0x73, 1 }, { 0, 0x11, 0x73, 2 }, { 0, 0x20, 0x5f, 0 },
	{ 0, 0x21, 0x60, 0 }, { 0, 0x30, 0x68, 5 }, { 0, 0x31, 0x68, 0x13 }, { 0, 0x32, 0x69, 5 },
	{ 0, 0x33, 0x69, 0x14 }, { 0, 0x40, 0x66, 6 }, { 0, 0x41, 0x66, 6 }, { 0, 0x42, 0x67, 6 },
	{ 0, 0x43, 0x67, 6 }, { 0, 0x50, 0x62, 7 }, { 0, 0x51, 0x62, 7 }, { 0, 0x60, 0x63, 0 },
	{ 0, 0x70, 0x65, 8 }, { 0, 0x71, 0x65, 9 }, { 0, 0x81, 0x0d, 0x0c }, { 0, 0xc0, 0x102, 0 },
	{ 1, 0, 0x6a, 0x0a }, { 1, 1, 0x6a, 0x0a },
	{ 0xff, 0xff, 0xffff, 0xffff },
} ;
static BOARD_PARTS *board_list[2] = { board_list0, board_list1 } ;
static VEC3 board_pos[21] = {
	{ 0.0f, 0.0f, 0.0f }, { -9.35f, 1.01f, -12.59f }, { 5.03f, 1.01f, -12.59f },
	{ -7.45f, 3.78f, -13.0f }, { 7.43f, 3.78f, -13.0f }, { 0.0f, -2.14f, -13.0f },
	{ 0.0f, 0.0f, -12.45f }, { 0.0f, 3.31f, -12.52f }, { -6.01f, 0.84f, -12.52f },
	{ 8.28f, 0.84f, -12.52f }, { 0.0f, -2.14f, -12.91f }, { 0.0f, -6.73f, -14.0f },
	{ 0.0f, -6.73f, -14.1f }, { 0.0f, -6.81f, -13.9f }, { 0.0f, -5.68f, -13.9f },
	{ 0.0f, -7.89f, -13.9f }, { 0.0f, -0.24f, -11.88f }, { 0.93f, -7.15f, -12.59f },
	{ 0.0f, 3.26f, -12.3f }, { 4.82f, -2.14f, -13.0f }, { 10.11f, -2.14f, -13.0f },
} ;
static Float board_char_x[4] = { -9.26f, 4.82f, -4.02f, 10.11f } ;
static Float board_face_x[4] = { -8.98f, 4.62f, -4.35f, 9.29f } ;
static Float board_face_y[2] = { -2.1f, -2.08f } ;
static Float board_rec_x[2] = { 9.41f, -4.92f } ;
static Float board_cursor_x[3][2] = { { -9.22f, 4.79f }, { -3.98f, 10.07f }, { 1.33f, -1.77f } } ;
static Float board_name_x[2] = { 37.0f, 415.0f } ;
static VEC3 board_rec_pos0[10] = {
	{ -8.32f, 4.36f, -12.52f }, { -7.48f, 4.36f, -12.52f }, { -6.63f, 4.36f, -12.52f }, { -5.79f, 4.36f, -12.52f },
	{ -7.48f, 3.34f, -12.52f }, { -6.63f, 3.34f, -12.52f }, { -5.79f, 3.34f, -12.52f },
	{ -7.48f, 2.34f, -12.52f }, { -6.63f, 2.34f, -12.52f }, { -5.79f, 2.34f, -12.52f },
} ;
static VEC3 board_rec_pos1[10] = {
	{ 6.04f, 4.36f, -12.52f }, { 6.88f, 4.36f, -12.52f }, { 7.73f, 4.36f, -12.52f }, { 8.57f, 4.36f, -12.52f },
	{ 6.88f, 3.34f, -12.52f }, { 7.73f, 3.34f, -12.52f }, { 8.57f, 3.34f, -12.52f },
	{ 6.88f, 2.34f, -12.52f }, { 7.73f, 2.34f, -12.52f }, { 8.57f, 2.34f, -12.52f },
} ;
static VEC3 *board_rec_pos[2] = { board_rec_pos0, board_rec_pos1 } ;
static VEC3 board_cnt_pos[2] = { { -0.55f, 3.83f, -12.28f }, { 0.51f, 3.83f, -12.28f } } ;
static VEC3 board_pt_pos[3] = { { -6.35f, -6.02f, -12.52f }, { -5.51f, -6.02f, -12.52f }, { -4.66f, -6.02f, -12.52f } } ;

/****************************************************************/
/*			A board, from its list of pieces					*/
/****************************************************************/
void dm0d_board_set( int no )
{
	BOARD_PARTS *p ;

	p = board_list[no] ;
	while( p->type != 0xff ){
		board_parts_set( p->type, p->sub_type, p->obj, p->pos + board_pos ) ;
		p ++ ;
	}
}

PLAYER_WORK *board_parts_set( int type, int sub_type, int obj, VEC3 *pos )
{
	PLAYER_WORK *w ;

	if( !( w = work_get( 0, W_SYS, LIST_TOP ) ) ) return 0 ;
	w->disp_flag = 1 ;
	w->work_func = board_move ;
	w->obj = obj_dm0d->obj[obj] ;
	w->pl_no = net_board_side ;
	w->type = type ;
	w->sub_type = sub_type ;
	w->timer0 = obj ;
	w->timer1 = 0 ;
	w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
	w->pos = *pos ;
	w->scale = board_scale ;
	w->fade = board_fade ;
	return w ;
}

void board_move( PLAYER_WORK *w )
{
	static void ( *board_jmp[3] )( PLAYER_WORK *w ) = { board_main, board_cursor, board_scroll } ;

	board_jmp[w->type]( w ) ;
}

/****************************************************************/
/*			A piece: the high digit of sub_type is what it		*/
/*			shows, the low one whose							*/
/****************************************************************/
void board_main( PLAYER_WORK *w )
{
	int n ;
	VEC3 *p ;

	n = w->sub_type & 0x0f ;
	switch( w->sub_type & 0xf0 ){
	case 0x10 :
		w->scale = board_scale_face ;
		if( w->pl_no == n ){
			w->obj = obj_dm0d->obj[w->timer0 - net_rank] ;
		} else {
			w->obj = obj_dm0d->obj[w->timer0 - net_vs_rank] ;
		}
		break ;
	case 0x20 :
		if( w->pl_no == n ){
			str_put( board_name_x[n], 89.0f, 12.01f, 0xff000000, ( char * )net_handle ) ;
		} else {
			str_put( board_name_x[n], 89.0f, 12.01f, 0xff000000, net_vs_handle ) ;
		}
		break ;
	case 0x30 :
		if( !w->routine_0 ){
			w->routine_0 ++ ;
			w->pos.x = board_char_x[n] ;
		}
		break ;
	case 0x40 :
		if( !w->routine_0 ){
			w->routine_0 ++ ;
			w->pos.x = board_face_x[n] ;
			w->pos.y = board_face_y[n & 1] ;
			w->obj = obj_dm0d->obj[w->timer0 + ( unk_8C213AEE > 0 ) * 15] ;
		}
		break ;
	case 0x50 :
		if( !w->routine_0 ){
			w->routine_0 ++ ;
			w->pos.x = board_rec_x[n & 0x0f] ;
			p = board_rec_pos[n] ;
			if( w->pl_no == n ){
				n = net_win_cnt + net_lose_cnt + net_win_add + net_lose_add ;
				board_num_put( n, 9999, 4, p, &board_fade_rec ) ;
				p += 4 ;
				board_num_put( net_win_cnt + net_win_add, 999, 3, p, &board_fade_rec ) ;
				p += 3 ;
				board_num_put( net_lose_cnt + net_lose_add, 999, 3, p, &board_fade_rec ) ;
			} else {
				n = net_vs_win_cnt + net_vs_lose_cnt + net_vs_win_add + net_vs_lose_add ;
				board_num_put( n, 9999, 4, p, &board_fade_rec ) ;
				p += 4 ;
				board_num_put( net_vs_win_cnt + net_vs_win_add, 999, 3, p, &board_fade_rec ) ;
				p += 3 ;
				board_num_put( net_vs_lose_cnt + net_vs_lose_add, 999, 3, p, &board_fade_rec ) ;
			}
		}
		break ;
	case 0x60 :
		if( !w->routine_0 ){
			w->routine_0 ++ ;
			board_num_put( unk_8C213AEE, 10, 0x82, board_cnt_pos, &board_fade_cnt ) ;
		}
		break ;
	case 0x70 :
		if( !unk_8C213AEE ) break ;
		w->disp_flag = 0 ;
		if( board_decide[n] == 1 ) w->disp_flag = 1 ;
		break ;
	case 0x80 :
		if( !n ) w->scale = board_scale_big ;
		if( n == 1 ){
			w->scale = board_scale_wide ;
			w->fade = board_fade_dark ;
		}
		break ;
	case 0xc0 :
		if( !w->routine_0 ){
			w->routine_0 ++ ;
			board_num_put( unk_8C215294, 999, 3, board_pt_pos, &board_fade_pt ) ;
		}
		str_put( 240.0f, 212.0f, -12.01f, 0xffffffff, ( char * )net_handle ) ;
		break ;
	}
}

/****************************************************************/
/*			The cursor on the chosen character, pulsing			*/
/****************************************************************/
void board_cursor( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->timer1 += 9 ;
		if( w->timer1 >= 360 ) w->timer1 = 0 ;
		w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
		w->spd.x = w->pos.x ;
		w->pos.x = board_cursor_x[board_cursor_no[w->sub_type]][w->sub_type] ;
		w->obj = obj_dm0d->obj[w->timer0 + board_cursor_no[w->sub_type]] ;
		if( w->spd.x != w->pos.x ) w->timer1 = 0 ;
		if( board_decide[w->sub_type] ){
			w->routine_0 ++ ;
			w->disp_flag = 0 ;
		}
	} else {
		work_free( w ) ;
	}
}

/****************************************************************/
/*			A model whose texture scrolls						*/
/****************************************************************/
void board_scroll( PLAYER_WORK *w )
{
	Float u, v ;

	switch( w->routine_0 ){
	case 0 :
		w->routine_0 ++ ;
		w->timer0 = 0 ;
		w->timer1 = 0 ;
		obj_uv_clear( w->obj ) ;
		break ;
	case 1 :
		if( ++ w->timer0 >= 1000 ) w->timer0 = 0 ;
		if( ++ w->timer1 >= 1000 ) w->timer1 = 0 ;
		obj_uv_begin( obj_dm0d->obj[58], w->obj ) ;
		while( !obj_uv_next() ){
			obj_uv_get( &u, &v ) ;
			u -= 1.0f - w->timer0 * 0.001f ;
			v -= 1.0f - w->timer1 * 0.001f ;
			obj_uv_put( &u, &v ) ;
		}
		break ;
	}
}

/****************************************************************/
/*			A number, digits+0x80 to show its leading zeros		*/
/****************************************************************/
void board_num_put( int num, int max, int flag, VEC3 *pos, FADE_COL *fade )
{
	PLAYER_WORK *w ;
	int i, d, zero, digit ;

	if( num > max ) num = max ;
	digit = flag & ~0x80 ;
	zero = ( flag & 0x80 ) >> 7 ;
	for( i = 0 ; i < digit ; i ++, pos ++ ){
		d = num / ( int )nlPow( 10.0f, digit - i - 1 ) ;
		d %= 10 ;
		if( i == digit - 1 ) zero = 1 ;
		if( !zero && !d ) continue ;
		w = board_parts_set( 0, 0xd0, d + 0xf8, pos ) ;
		w->fade = *fade ;
		if( digit > 2 ) w->scale = board_scale_num ;
		zero = 1 ;
	}
}
