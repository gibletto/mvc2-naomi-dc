/* The home game's SECRET FACTOR, the system's task 3 in mode 6: its menu and the shop that sells
   characters, their second colours and the extras for points. */
#include "player.h"
#include "shinobi.h"
#include "dm.h"
#include "secret.h"

typedef struct VS_SHOP_W {
	PLAYER_WORK	*cursor;	/* 00: the cursor, whose work2 is its shadow */
	PLAYER_WORK	*msg;		/* 04: the message, by its sub_no */
	PLAYER_WORK	*subwin;	/* 08: the window asking whether to buy */
	PLAYER_WORK	*shop;		/* 0c: the shop's frame */
	Sint16	flag;		/* 10: the first good is a colour, not an extra */
	Sint16	cpos;		/* 12: the cursor: the three goods, 3 EXIT */
	Sint16	shop_goods[3];	/* 14: a character's number, an extra's, or -1 when sold */
	Sint16	shop_pts[3][3];	/* 1a: each good's price */
	Sint16	unk_2c;
	Sint16	empty;		/* 2e: the goods sold out */
} VS_SHOP_W;

void vs_shop_cursor( VS_SHOP_W *shop ) ;
void shop_msg_mv( VS_SHOP_W *shop ) ;

static VS_SHOP_W vs_shop_w ;
static Sint32 vs_shop_unused ;

static void ( *secret_factor_jmp[] )() = {
	secret_factor_0, secret_factor_1, secret_factor_2, secret_factor_3
} ;
static Float shop_cursor_x[3] = { -7.5f, 0.0f, 7.5f } ;
static Sint32 shop_rank_tbl[10] = { 2, 0, 1, 0, 1, 2, 0, 1, 0, 1 } ;	/* the goods by vm_level % 10 */
static Uint8 shop_char_tbl[32] = {	/* the characters the shop sells, in groups by level */
	0x07, 0x2d, 0x30, 0x2a, 0x1c, 0x1d, 0x38, 0x1b, 0x1f, 0x1e, 0x37, 0x03, 0x28, 0x2f, 0x29, 0x2c,
	0x32, 0x35, 0x2b, 0x31, 0x27, 0x21, 0x25, 0x26, 0x2e, 0x33, 0x36, 0x34, 0x23, 0x24, 0x20, 0x3a
} ;

/****************************************************************/
/*			SECRET FACTOR, each frame							*/
/****************************************************************/
void secret_factor( void )
{
	Uint16 sw ;

	sw = menu_sw_get( 3 ) ;
	switch( Sys->routine_0 ){
	case 0 :
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 18 ) ;
		Sys->routine_0 ++ ;
		Sys->routine_1 = 0 ;
	case 1 :
		Sys->routine_0 ++ ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		work_init() ;
		dm0c_wall_paper( 4 ) ;
		headline_set( 9 ) ;
		dm0f_menu_set() ;
		dm0f_part1_set() ;
		dm0f_part2_set() ;
		dm0f_part3_set() ;
		dm0f_point_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 2 :
		if( !fade_cnt() ) Sys->routine_0 ++ ;
		break ;
	case 3 :
		Sys->routine_1 = ud_select_loop( sw, Sys->routine_1, 4 ) ;
		if( decide_ck() ){
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			if( Sys->routine_1 == 4 ){
				Sys->routine_0 ++ ;
				return ;
			}
			Sys->routine_0 = 5 ;
			return ;
		}
		if( cancel_ck() ){
			Sys->routine_0 ++ ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
		}
		break ;
	case 4 :
		if( !fade_cnt() ){
			Sys->routine_0 = 7 ;
			Sys->routine_3 = Sys->exit_flag = 0 ;
		}
		break ;
	case 5 :
		if( !fade_cnt() ) Sys->routine_0 ++ ;
		break ;
	case 6 :
		secret_factor_jmp[Sys->routine_1]( sw, 0 ) ;
		break ;
	case 7 :
		switch( Sys->routine_3 ){
		case 0 :
			if( vms_work.file_flag != 1 ){
				Sys->task_step = Sys->scene_no = Sys->routine_0 = Sys->routine_1 = Sys->routine_2 = Sys->routine_3 = Sys->exit_flag = 0 ;
				if( vms_work.d_point ) d_point_get() ;
				if( vms_work.n_point ) n_point_get() ;
				break ;
			}
			Sys->routine_3 ++ ;
			Sys->exit_flag = 0 ;
			bright_init() ;
			fog_init() ;
			Palette_Init() ;
			Texture_Init() ;
			TexLoad( 16 ) ;
			work_init() ;
			dm0c_wall_paper( 9 ) ;
			Sys->timer0 = 120 ;
			vms_work.card_task_step[0][0] = vms_work.card_task_step[0][1] = 0 ;
			vms_work.card_task_step[1][0] = vms_work.card_task_step[1][1] = 0 ;
			if( vms_work.d_point && vms_work.n_point ) vms_work.vm_msg[0] = 3 ;
			else if( vms_work.d_point ){
				vms_work.vm_msg[0] = 1 ;
				if( 0 ) vms_work.vm_msg[0] = 1 ;	/* left out of the build */
			} else if( !vms_work.n_point ) goto none ;
			else vms_work.vm_msg[0] = 2 ;
			dm0c_vm_msg_set() ;
			vms_work.vm_msg[1] = 1 ;
			dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
			fade_set( 0xff000000, 10, FADE_IN ) ;
			break ;
		none :
			Sys->timer0 = 1 ;
			fade_set( 0xff000000, 10, FADE_IN ) ;
			break ;
		case 1 :
			if( !fade_cnt() ) Sys->routine_3 ++ ;
			break ;
		case 2 :
			if( cancel_ck() ) Sys->timer0 = 1 ;
			if( Sys->timer0 -- == 0 ){
				Sys->routine_3 ++ ;
				vms_work.vm_msg[0] = 4 ;
				dm0c_vm_msg_set() ;
				vms_work.vm_msg[1] = 2 ;
				dm0c_vm_win_set( vms_work.vm_msg[1] ) ;
				if( vms_work.d_point ) d_point_get() ;
				if( vms_work.n_point ) n_point_get() ;
			}
			return ;
		case 3 :
			{
				int ret ;

				ret = vms_task_save_all( vms_work.file_port, 0 ) ;
				if( ret == 2 ) return ;
				if( ret == 1 ){
					Sys->routine_3 = 4 ;
					Sys->timer0 = 60 ;
					vms_work.vm_msg[0] = 5 ;
				} else if( ret == 0 ){
					Sys->routine_3 = 5 ;
					vms_work.vm_msg[0] = 6 ;
				} else {
					if( ret != 3 ) break ;
					Sys->routine_3 = 5 ;
					vms_work.vm_msg[0] = 7 ;
				}
				dm0c_vm_msg_set() ;
				return ;
			}
		case 4 :
			if( decide_cancel_ck() ) Sys->timer0 = 1 ;
			if( Sys->timer0 -- == 0 ){
				Sys->routine_3 = 6 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 5 :
			if( decide_cancel_ck() ){
				vms_work.file_flag = 0 ;
				Sys->routine_3 = 6 ;
				fade_set( 0xff000000, 10, FADE_OUT ) ;
			}
			break ;
		case 6 :
			if( fade_cnt() ) break ;
			else {
				work_init() ;
				Sys->task_step = 1 ;
				Sys->scene_no = Sys->routine_0 = Sys->routine_1 = Sys->routine_2 = Sys->routine_3 = Sys->exit_flag = 0 ;
				home_menu_set() ;
				dm0c_wall_paper( 0 ) ;
			}
			fade_set( 0xff000000, 10, FADE_IN ) ;
			break ;
		}
		break ;
	}
}

/****************************************************************/
/*			The shop's cursor: on a good, or on EXIT			*/
/****************************************************************/
void vs_shop_cursor( VS_SHOP_W *shop )
{
	PLAYER_WORK *shadow ;

	shadow = (PLAYER_WORK *)shop->cursor->work2 ;
	if( shop->cpos == 3 ){
		shop->cursor->obj = obj_dm0f->obj[188] ;
		shop->cursor->pos.x = 0.0f ;
		shop->cursor->pos.y = -2.75f ;
		shop->cursor->pos.z = -13.05f ;
		shop->cursor->scale.x = 1.0f ;
		shop->cursor->scale.y = 0.3f ;
		shadow->disp_flag = 0 ;
		shop->shop->fade.r = shop->shop->fade.g = shop->shop->fade.b = 1.0f ;
	} else {
		shop->cursor->obj = obj_dm0f->obj[166] ;
		shop->cursor->pos.x = shop_cursor_x[shop->cpos] ;
		shop->cursor->pos.y = 1.0f ;
		shop->cursor->pos.z = -12.89f ;
		shop->cursor->scale.x = shop->cursor->scale.y = 1.0f ;
		if( shop->empty < 2 ){
			shadow->disp_flag = 1 ;
			shadow->pos = shop->cursor->pos ;
			shadow->pos.y = 0.0f ;
		} else shadow->disp_flag = 0 ;
		shop->shop->fade.r = shop->shop->fade.g = shop->shop->fade.b = 0.8f ;
	}
}

/****************************************************************/
/*			A character's price, by the level					*/
/****************************************************************/
void shop_pts_set( VS_SHOP_W *shop, int no, int char_no )
{
	int lv, pts ;

	if( no == 1 ){
		if( CS_Secret_Work.vm_level >= 50 ) lv = char_no / 4 ;
		else if( CS_Secret_Work.vm_level % 2 ) lv = char_no / 4 ;
		else lv = char_no / 4 + 2 ;
	} else if( CS_Secret_Work.vm_level >= 50 ) lv = char_no / 4 + 4 ;
	else if( shop_rank_tbl[CS_Secret_Work.vm_level % 10] == 2 ) lv = char_no / 4 + 6 ;
	else if( shop_rank_tbl[CS_Secret_Work.vm_level % 10] == 0 ) lv = 4 ;
	else lv = 5 ;
	pts = 0 ;
	switch( lv ){
	case 0 :
	case 2 :
		pts = 500 + nlRand() % 501 ;
		break ;
	case 1 :
	case 3 :
		pts = 1000 + nlRand() % 1001 ;
		break ;
	case 4 :
	case 5 :
		pts = 2000 + nlRand() % 1001 ;
		break ;
	case 6 :
	case 7 :
		pts = 3000 + nlRand() % 1001 ;
		break ;
	}
	shop->shop_pts[no][0] = pts ;
	shop->shop_pts[no][1] = 0 ;
	shop->shop_pts[no][2] = 0 ;
}

/****************************************************************/
/*			A good: the second colour of a character opened,	*/
/*			from a random one on								*/
/****************************************************************/
void shop_color_select( VS_SHOP_W *shop, int no )
{
	Uint32 *open, *all ;
	int i, c, cnt ;

	shop->flag = 1 ;
	cnt = 0 ;
	c = nlRand() % 59 ;
	for( i = 0 ; i < 59 ; i++ ){
		if( c >= 24 && c <= 26 ){
			c ++ ;
			continue ;
		}
		if( c >= 32 ){
			open = &Sys->char_open1 ;
			all = &Sys->char_open_all1 ;
		} else {
			open = &Sys->char_open0 ;
			all = &Sys->char_open_all0 ;
		}
		if( ( *open & ( 1 << c ) ) && !( *all & ( 1 << c ) ) ) break ;
		if( ++ c >= 59 ) c = 0 ;
	}
	if( i != 59 ){
		shop->shop_goods[no] = c ;
		open = &Sys->char_open0 ;
		all = &Sys->char_open_all0 ;
		for( c = 0 ; c <= 58 ; c++ ){
			if( c == 32 ){
				open ++ ;
				all ++ ;
			}
			if( c >= 24 && c <= 26 ) continue ;
			if( ( *open & ( 1 << c ) ) && ( *all & ( 1 << c ) ) ) cnt ++ ;
		}
		shop->shop_pts[no][0] = ( cnt + 1 ) * 100 ;
	} else shop->shop_goods[no] = -1 ;
}

/****************************************************************/
/*			A good: the next extra								*/
/****************************************************************/
void shop_extra_select( VS_SHOP_W *shop, int no )
{
	shop->flag = 0 ;
	if( vms_work.stage_ex_cnt >= 7 ) shop->shop_goods[no] = -1 ;
	else {
		shop->shop_goods[no] = vms_work.stage_ex_cnt + 1 ;
		shop->shop_pts[no][0] = vms_work.stage_ex_cnt * 500 + 1000 ;
	}
}

/****************************************************************/
/*			The shop's three goods								*/
/****************************************************************/
void shop_goods_select( VS_SHOP_W *shop )
{
	Uint8 *tbl ;
	Uint32 *pl_flag ;
	SECRET_WORK *sw ;
	int no, i, n, k, c ;

	sw = &CS_Secret_Work ;

	no = 0 ;
	shop->shop_pts[0][0] = no ;
	shop->shop_pts[0][2] = no ;
	shop->shop_pts[0][1] = no ;
	if( sw->vm_level < 50 ){
		if( sw->vm_level % 5 ) shop_color_select( shop, 0 ) ;
		else shop_extra_select( shop, 0 ) ;
	} else {
		i = nlRand() & 1 ;
		if( i ){
			shop_color_select( shop, 0 ) ;
			if( shop->shop_goods[0] != -1 ) goto next ;
			shop_extra_select( shop, 0 ) ;
		} else {
			shop_extra_select( shop, 0 ) ;
			if( shop->shop_goods[0] != -1 ) goto next ;
			shop_color_select( shop, 0 ) ;
		}
	}
	if( shop->shop_goods[0] == -1 ) shop->empty ++ ;
next :
	no ++ ;

	if( sw->vm_level >= 50 ){
		tbl = shop_char_tbl ;
		n = 16 ;
	} else {
		if( sw->vm_level % 2 == 0 ) tbl = shop_char_tbl + 8 ;
		else tbl = shop_char_tbl ;
		n = 8 ;
	}
	k = nlRand() % n ;
	for( i = 0 ; i < n ; i++ ){
		c = tbl[k] ;
		if( c >= 32 ) pl_flag = &Sys->char_open1 ;
		else pl_flag = &Sys->char_open0 ;
		if( !( *pl_flag & ( 1 << c ) ) ) break ;
		k = ( k + 1 ) & ( n - 1 ) ;
	}
	if( i == n ) shop->shop_goods[no] = -1 ;
	else {
		shop->shop_goods[no] = c ;
		shop_pts_set( shop, no, k ) ;
	}
	if( shop->shop_goods[no] == -1 ) shop->empty ++ ;
	no ++ ;

	if( sw->vm_level >= 50 ){
		tbl = shop_char_tbl + 16 ;
		n = 16 ;
	} else if( shop_rank_tbl[sw->vm_level % 10] == 2 ){
		tbl = shop_char_tbl + 24 ;
		n = 8 ;
	} else {
		if( shop_rank_tbl[sw->vm_level % 10] == 0 ) tbl = shop_char_tbl + 16 ;
		else tbl = shop_char_tbl + 20 ;
		n = 4 ;
	}
	k = nlRand() % n ;
	for( i = 0 ; i < n ; i++ ){
		c = tbl[k] ;
		if( c >= 32 ) pl_flag = &Sys->char_open1 ;
		else pl_flag = &Sys->char_open0 ;
		if( !( *pl_flag & ( 1 << c ) ) ) break ;
		k = ( k + 1 ) & ( n - 1 ) ;
	}
	if( i == n ) shop->shop_goods[no] = -1 ;
	else {
		shop->shop_goods[no] = c ;
		shop_pts_set( shop, no, k ) ;
	}
	if( shop->shop_goods[no] == -1 ) shop->empty ++ ;
}

/****************************************************************/
/*			The message for the cursor's place					*/
/****************************************************************/
void shop_msg_mv( VS_SHOP_W *shop )
{
	switch( shop->cpos ){
	case 0 :
		if( shop->flag ) shop->msg->sub_no = 1 ;
		else shop->msg->sub_no = 2 ;
		break ;
	case 1 :
	case 2 :
		shop->msg->sub_no = 0 ;
		break ;
	case 3 :
		if( shop->shop_goods[0] == -1 && shop->shop_goods[1] == -1 && shop->shop_goods[2] == -1 ) shop->msg->sub_no = 6 ;
		else shop->msg->sub_no = 7 ;
		break ;
	}
	vs_shop_cursor( shop ) ;
}

/****************************************************************/
/*			The cursor moved: 0 right, 1 left, 2 down to EXIT,	*/
/*			3 up from it										*/
/****************************************************************/
void shop_cursor_mv( VS_SHOP_W *shop, int flag )
{
	Sint16 pos ;

	if( flag < 2 ){
		if( shop->empty == 3 ){
			shop->cpos = 3 ;
			shop_msg_mv( shop ) ;
			return ;
		} else {
			pos = shop->cpos ;
			while( pos != 3 ){
				if( flag == 0 ){
					pos ++ ;
					if( pos == 3 ) pos = 0 ;
					if( shop->shop_goods[pos] != -1 ) break ;
					if( pos == shop->cpos ) break ;
				} else {
					pos -- ;
					if( pos < 0 ) pos = 2 ;
					if( shop->shop_goods[pos] != -1 ) break ;
					if( pos == shop->cpos ) break ;
				}
			}
			if( pos != shop->cpos && Sys->routine_2 == 2 ) goto sound ;
		}
	} else if( flag == 2 ){
		pos = 3 ;
		goto sound ;
	} else if( flag == 3 ){
		if( shop->shop_goods[1] != -1 ) pos = 1 ;
		else if( shop->shop_goods[0] != -1 ) pos = 0 ;
		else if( shop->shop_goods[2] != -1 ) pos = 2 ;
		else pos = 3 ;
		if( pos != shop->cpos ){
		sound :
			shop->cursor->fade.trnsl = 0.99f ;
			shop->cursor->timer1 = -9 ;
			na_req( 1 ) ;
		}
	} else return ;
	shop->cpos = pos ;
	shop_msg_mv( shop ) ;
}

/****************************************************************/
/*			The shop											*/
/****************************************************************/
void secret_factor_0( Uint16 sw, Uint16 sw2 )
{
	VS_SHOP_W *shop ;
	Uint32 *open ;
	int dec, can, c ;

	shop = &vs_shop_w ;
	switch( Sys->routine_2 ){
	case 0 :
		work_list_kill( 11 ) ;
		dm0f_30_shop_init() ;
		dm0c_wall_paper( 6 ) ;
		headline_set( 12 ) ;
		memset( shop, 0, sizeof( VS_SHOP_W ) ) ;
		shop_goods_select( shop ) ;
		shop->shop = dm0f_30_set() ;
		dm0f_30_shop1_set( shop->flag, shop->shop_goods[0], shop->shop_pts[0][0] ) ;
		dm0f_30_shop2_set( 0, shop->shop_goods[1], shop->shop_pts[1] ) ;
		dm0f_30_shop2_set( 1, shop->shop_goods[2], shop->shop_pts[2] ) ;
		shop->msg = dm0f_30_msg_set() ;
		shop->cursor = dm0f_30_cursor_set() ;
		shop->cpos = 1 ;
		shop_cursor_mv( shop, 1 ) ;
		shop->subwin = dm0f_30_subwin_set() ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		Sys->routine_2 ++ ;
	case 1 :
		if( !fade_cnt() ) Sys->routine_2 ++ ;
		break ;
	case 2 :
		if( ( sw & KEY_RIGHT ) && shop->cpos != 3 ) shop_cursor_mv( shop, 0 ) ;
		if( ( sw & KEY_LEFT ) && shop->cpos != 3 ) shop_cursor_mv( shop, 1 ) ;
		if( ( sw & KEY_DOWN ) && shop->cpos != 3 ) shop_cursor_mv( shop, 2 ) ;
		if( ( sw & KEY_UP ) && shop->cpos == 3 ) shop_cursor_mv( shop, 3 ) ;
		dec = decide_ck_nose() ;
		can = cancel_ck_nose() ;
		if( can || ( dec && shop->cpos == 3 ) ){
			na_req( dec ? 2 : 3 ) ;
			fade_set( 0xff000000, 10, FADE_OUT ) ;
			Sys->routine_2 = 4 ;
		} else if( dec ){
			na_req( 2 ) ;
			shop->subwin->routine_0 = 1 ;
			shop->msg->sub_no = 3 ;
			Sys->routine_2 ++ ;
		}
		break ;
	case 3 :
		if( shop->subwin->routine_0 == 0 ){
			if( shop->subwin->routine_1 == 0 ){
				if( shop->shop_pts[shop->cpos][0] > CS_Secret_Work.d_pts ){
					shop->msg->sub_no = 5 ;
					na_req( 5 ) ;
				} else {
					com_se_req( 42 ) ;
					CS_Secret_Work.d_pts -= shop->shop_pts[shop->cpos][0] ;
					dm0f_30_pts_set() ;
					if( shop->cpos ){
						c = shop->shop_goods[shop->cpos] ;
						if( c >= 32 ) open = &Sys->char_open1 ;
						else open = &Sys->char_open0 ;
						*open |= 1 << c ;
					} else if( shop->flag ){
						c = shop->shop_goods[shop->cpos] ;
						if( c >= 32 ) open = &Sys->char_open_all1 ;
						else open = &Sys->char_open_all0 ;
						*open |= 1 << c ;
					} else if( vms_work.stage_ex_cnt < 7 ) vms_work.stage_ex_cnt ++ ;
					shop->shop_goods[shop->cpos] = -1 ;
					shop->empty ++ ;
					dm0f_30_goods_off( shop->cpos ) ;
					shop->msg->sub_no = 4 ;
					shop_cursor_mv( shop, 0 ) ;
				}
			} else {
				shop_msg_mv( shop ) ;
				if( shop->subwin->work3 ) na_req( 2 ) ;
				else na_req( 3 ) ;
			}
			Sys->routine_2 -- ;
		}
		break ;
	case 4 :
		if( !fade_cnt() ){
			Sys->routine_0 = 1 ;
			Sys->routine_2 = 0 ;
		}
		break ;
	}
}
