/* The players' buttons for the frame. The home game's module also has what its pads bring: the
   pause, the button config and the training mode's menu. */
#include "player.h"
#ifdef TARGET_DC
#include "shinobi.h"
#include "adx.h"

extern ADXT adxt ;		/* system.c: the music's player */
extern void pl_sw_config( void ) ;
extern Uint16 decide_sw_get( Sint8 side ) ;
extern void soft_reset( int trans ) ;
extern void vib_req( int pl_no, int no ) ;
extern void vib_stop( void ) ;
extern void fight_load_start( int no ) ;
extern void load_stop( void ) ;

/* the training menu's objects */
extern void pause_yn_win_set( void ) ;
extern void pause_yn_cursor_set( void ) ;
extern void pause_sub_win_set( void ) ;
extern void pause_sub_cursor_set( void ) ;
extern void pause_opt_set( void ) ;
extern void pause_cfg_set( void ) ;
extern void pause_bg_set( void ) ;
extern void pause_win_set( void ) ;
extern void pause_cursor_set( void ) ;

static void pause_end( PAUSE_WORK *w ) ;
static void pause_menu( PAUSE_WORK *w, int side ) ;
static void pause_quit( PAUSE_WORK *w, int side ) ;
static void pause_menu_training( PAUSE_WORK *w, int side ) ;
static void btn_config_keep( void ) ;
static void btn_config_back( void ) ;
static void training_restart( void ) ;

/* a line of the pause's menus: where it is printed, and where its cursor ends */
typedef struct {
	Uint8	x, y ;
	Uint8	x_end ;
	Sint8	col ;
	const char	*mes ;
} PAUSE_MES ;

static const PAUSE_MES pause_mes[] = {
	/* the pause */
	{ 68, 56, 92, 34, "CONTINUE" },
	{ 60, 64, 99, 35, "BUTTON CONFIG." },
	{ 66, 72, 93, 36, "MODE MENU" },
	{ 65, 56, 94, 37, "QUIT GAME?" },
	{ 75, 64, 85, 38, "YES" },
	{ 76, 72, 84, 39, "NO" },
	/* the training's pause */
	{ 68, 54, 92, 34, "CONTINUE" },
	{ 60, 60, 99, 50, "TRAINING MENU" },
	{ 60, 66, 99, 35, "BUTTON CONFIG." },
	{ 66, 72, 93, 36, "MODE MENU" },
	/* the button config's lines */
	{ 30, 44, 78, 40, "X  <           >" },
	{ 30, 48, 78, 41, "Y  <           >" },
	{ 30, 52, 78, 42, "A  <           >" },
	{ 30, 56, 78, 43, "B  <           >" },
	{ 30, 68, 78, 46, "VIBRATE  <     >" },
	{ 30, 72, 78, 47, "COMMAND <      >" },
	{ 30, 76, 78, 48, "DEFAULT SETTINGS" },
	{ 45, 80, 57, 49, "EXIT" },
	{ 30, 60, 78, 44, "L  <           >" },
	{ 30, 64, 78, 45, "R  <           >" },
	/* what a button does */
	{ 48, 44, 69, 40, "L.PUNCH" },
	{ 48, 44, 69, 40, "H.PUNCH" },
	{ 50, 44, 68, 40, "L.KICK" },
	{ 50, 44, 68, 40, "H.KICK" },
	{ 45, 44, 72, 40, "PARTNER-A" },
	{ 45, 44, 72, 40, "PARTNER-B" },
	{ 48, 44, 69, 40, "L.P+H.P" },
	{ 48, 44, 69, 40, "L.K+H.K" },
	{ 48, 44, 69, 40, "L.P+L.K" },
	{ 48, 44, 69, 40, "H.P+H.K" },
	{ 42, 44, 75, 40, "PARTNER-A+B" },
	{ 51, 44, 66, 40, "TAUNT" },
	/* the vibration and the command settings */
	{ 63, 68, 72, 46, "OFF" },
	{ 65, 68, 71, 46, "ON" },
	{ 57, 72, 75, 47, "NORMAL" },
	{ 60, 72, 72, 47, "LONG" },
	/* the fifth and sixth lines of a pad with Z and C buttons */
	{ 30, 60, 78, 44, "Z  <           >" },
	{ 30, 64, 78, 45, "C  <           >" },
} ;

/* the pause's boxes: the dark screen, the window's frame and inside, the cursor */
static NLVERTEX00 pause_vtx[4][4] = {
	{
		{ 0.0f, 0.0f, 0.99f, 0xff000000 },
		{ 640.0f, 0.0f, 0.99f, 0xff000000 },
		{ 0.0f, 480.0f, 0.99f, 0xff000000 },
		{ 640.0f, 480.0f, 0.99f, 0xff000000 },
	}, {
		{ 320.0f, 240.0f, 0.98f, 0xffffffff },
		{ 320.0f, 240.0f, 0.98f, 0xffffffff },
		{ 320.0f, 240.0f, 0.98f, 0xffffffff },
		{ 320.0f, 240.0f, 0.98f, 0xffffffff },
	}, {
		{ 320.0f, 240.0f, 0.97f, 0xff0000ee },
		{ 320.0f, 240.0f, 0.97f, 0xff0000ee },
		{ 320.0f, 240.0f, 0.97f, 0xff00b4ff },
		{ 320.0f, 240.0f, 0.97f, 0xff00b4ff },
	}, {
		{ 320.0f, 240.0f, 0.96f, 0xff00ff15 },
		{ 320.0f, 240.0f, 0.96f, 0xff00ff15 },
		{ 320.0f, 240.0f, 0.96f, 0xff00ff15 },
		{ 320.0f, 240.0f, 0.96f, 0xff00ff15 },
	},
} ;
#endif

/****************************************************************/
/*			Give each player work its side's buttons for the	*/
/*			frame												*/
/****************************************************************/
#ifndef TARGET_DC
void pl_sw_copy( void )
{
	int i ;

	for( i = 0 ; i < 6 ; i ++ ) pl_work[i].side_sw = sw_work[i & 1] ;
	for( i = 0 ; i < 6 ; i ++ ) pl_work[i].pad_sw = sw_work[i & 1] ;
}
#else
/* the home game's: the button config makes pad_sw, and the training's dummy is given its lever */
void pl_sw_copy( void )
{
	PLAYER_WORK *pl_w ;
	Uint16 sw ;
	int i ;

	for( i = 0 ; i < 6 ; i ++ ) pl_work[i].side_sw = sw_work[i & 1] ;
	pl_sw_config() ;
	if( training_work.side > 0 ){
		pl_w = &pl_work[2 - training_work.side] ;
		switch( training_work.stance ){
		case 0 :
			sw = 0 ;
			break ;
		case 1 :
			sw = KEY_DOWN ;
			break ;
		case 2 :
			pl_w->sjump_w.step = 0 ;
			pl_w->sjump_w.timer = 0 ;
			sw = KEY_UP ;
			break ;
		case 3 :
			sw = 0 ;
			break ;
		}
		pl_w->pad_sw.old = pl_w->pad_sw.now ;
		pl_w->pad_sw.now = sw ;
		pl_w->pad_sw.on = ~pl_w->pad_sw.old & pl_w->pad_sw.now ;
		pl_w->pad_sw.off = ~pl_w->pad_sw.now & pl_w->pad_sw.old ;
		pl_work[4 - training_work.side].pad_sw = pl_w->pad_sw ;
		pl_work[6 - training_work.side].pad_sw = pl_w->pad_sw ;
	}
}
#endif

/****************************************************************/
/*			The buttons a player holds							*/
/****************************************************************/
int pl_button_ck( PLAYER_WORK *pl_w )
{
	int sw ;

	sw = pl_w->pad_sw.now & KEY_BUTTON ;
	return sw ;
}

#ifdef TARGET_DC
/****************************************************************/
/*			Hold the music, or let it play on					*/
/****************************************************************/
void adx_pause( int flag )
{
	Sys->adx_pause_flag = flag ;
	ADXT_Pause( adxt, flag ) ;
}

/*==============================================================*/
/*			What a side asks of the pause: its start button		*/
/*			opens and ends it, and so does its pad pulled out	*/
/*			and put back										*/
/*==============================================================*/
static void pause_sw_ck( PAUSE_WORK *w, int side )
{
	SW_WORK *sw ;

	if( Sys->pause_off ) return ;
	if( !( w->side & ( 1 << side ) ) ) return ;
	if( training_work.menu_on ) return ;
	if( port_peri[side]->info->type ){
		if( w->pause_flag & 4 ){
			if( w->pad_on[side] ) return ;
			if( w->pause_flag & ( side + 1 ) ) w->req[side] = 2 ;
			else w->req[side] = 0 ;
		} else {
			sw = &sw_work[side] ;
			if( ( sw->now & ( KEY_START | KEY_ATCK ) ) == ( KEY_START | KEY_ATCK ) ) return ;	/* the reset keys */
			if( sw->on & KEY_START ){
				if( w->pause_flag & ( side + 1 ) ){
					if( w->config_flag ) return ;
					w->req[side] = 0 ;
				} else {
					if( w->req[1 - side] == 2 ) return ;
					if( pl_work[side].pad_sw.now & KEY_LK ) return ;
					w->req[side] = 2 ;
				}
			}
		}
	} else {
		if( w->pad_on[side] ) w->req[side] = 1 ;
	}
}

/*==============================================================*/
/*			A pad was pulled out, or (end) is back				*/
/*==============================================================*/
static void pause_out_set( PAUSE_WORK *w, int end )
{
	w->dark = 0 ;
	w->unk_03 = 0 ;
	w->open_flag = 0 ;
	w->config_open = 0 ;
	w->dark_timer = 18000 ;
	w->open_timer = 10 ;
	if( end ){
		w->win_w = 80.0f ;
		w->win_h = 80.0f ;
		if( w->config_on ){
			w->win_w = 200.0f ;
			w->win_h = 100.0f ;
		}
		if( Sys->adx_pause_flag && !w->pause_flag ) adx_pause( 0 ) ;
	} else {
		w->win_w = 192.0f ;
		w->win_h = 32.0f ;
		vib_stop() ;
		if( !Sys->adx_pause_flag ){
			adx_pause( 1 ) ;
			se_all_off() ;
		}
		na_req( 57 ) ;
	}
}

/*==============================================================*/
/*			A side's start button opens the pause				*/
/*==============================================================*/
static void pause_start( PAUSE_WORK *w )
{
	w->dark = 0 ;
	w->unk_03 = 0 ;
	w->open_flag = 0 ;
	w->page = 0 ;
	w->cursor = 0 ;
	w->cursor_mes = 0 ;
	w->config_flag = 0 ;
	w->config_on = 0 ;
	w->config_open = 0 ;
	if( !Sys->adx_pause_flag ){
		adx_pause( 1 ) ;
		se_all_off() ;
	}
	na_req( 57 ) ;
	w->dark_timer = 18000 ;
	w->open_timer = 10 ;
	w->win_w = 80.0f ;
	w->win_h = 80.0f ;
	vib_stop() ;
	if( Sys->home_mode == HOME_TRAINING ) w->page = 2 ;
	switch( Sys->home_mode ){
	case 0 :
		break ;
	case 1 :
	case 5 :
		break ;
	case 2 :
		break ;
	case 7 :
		break ;
	case 3 :
	case 4 :
	case 6 :
	case 8 :
		break ;
	}
}

/*==============================================================*/
/*			The fight goes on									*/
/*==============================================================*/
static void pause_end( PAUSE_WORK *w )
{
	w->pause_flag = 0 ;
	w->req[0] = 0 ;
	w->req[1] = 0 ;
	Sys->join_flag = 0 ;
	if( Sys->adx_pause_flag ) adx_pause( 0 ) ;
}

/****************************************************************/
/*			Each frame of the fight: a side's start button, or	*/
/*			its pad pulled out, holds it						*/
/****************************************************************/
void pause_ck( void )
{
	PAUSE_WORK *w = &pause_work ;

	pause_sw_ck( w, 0 ) ;
	pause_sw_ck( w, 1 ) ;
	if( !w->pause_flag ){
		if( w->req[0] == 1 || w->req[1] == 1 ){
			w->pause_flag |= 4 ;
			pause_out_set( w, 0 ) ;
		} else {
			if( w->req[0] == 2 ){
				w->pause_flag = 1 ;
				w->req[0] = 2 ;
				w->req[1] = 0 ;
				pause_start( w ) ;
			}
			if( w->req[1] == 2 ){
				w->pause_flag = 2 ;
				w->req[0] = 0 ;
				w->req[1] = 2 ;
				pause_start( w ) ;
			}
		}
	} else if( w->pause_flag & 4 ){
		if( w->req[0] != 1 && w->req[1] != 1 ){
			w->pause_flag &= 3 ;
			Sys->join_flag = 0 ;
			pause_out_set( w, 1 ) ;
		}
	} else {
		if( w->req[0] == 1 || w->req[1] == 1 ){
			w->pause_flag |= 4 ;
			pause_out_set( w, 0 ) ;
		} else if( ( w->pause_flag == 1 && !w->req[0] ) || ( w->pause_flag == 2 && !w->req[1] ) ){
			pause_end( w ) ;
		}
	}
	w->pad_on[0] = port_peri[0]->info->type ? 1 : 0 ;
	w->pad_on[1] = port_peri[1]->info->type ? 1 : 0 ;
}

/****************************************************************/
/*			Which sides may pause, by the mode					*/
/****************************************************************/
void pause_init( void )
{
	PAUSE_WORK *w = &pause_work ;

	w->pause_flag = 0 ;
	w->side = 0 ;
	w->dark = 0 ;
	w->unk_03 = 0 ;
	w->open_flag = 0 ;
	w->page = 0 ;
	w->cursor_mes = 0 ;
	w->cursor = 0 ;
	w->config_flag = 0 ;
	w->config_on = 0 ;
	w->config_open = 0 ;
	adx_pause( 0 ) ;
	w->dark_timer = 18000 ;
	w->open_timer = 10 ;
	w->win_w = 80.0f ;
	w->win_h = 80.0f ;
	w->pad_on[0] = 0 ;
	w->pad_on[1] = 0 ;
	w->req[0] = 0 ;
	w->req[1] = 0 ;
	if( Sys->play_mode != 96 ) return ;
	switch( Sys->home_mode ){
	case 0 :
		w->side = Sys->play_side ;
		break ;
	case 1 :
	case 5 :
		w->side = 3 ;
		break ;
	case 2 :
		w->page = 2 ;
		w->side = Sys->entry_side ;
		training_work.unk_1c = 0 ;
		break ;
	case 7 :
		w->side = Sys->entry_side ;
		break ;
	case 3 :
	case 4 :
	case 6 :
	case 8 :
		w->side = 0 ;
		break ;
	}
	if( w->side & 1 ) w->pad_on[0] = 1 ;
	if( w->side & 2 ) w->pad_on[1] = 1 ;
}

/*==============================================================*/
/*			The pause's window: the screen behind goes dark,	*/
/*			the window opens, and the cursor shows the line		*/
/*==============================================================*/
static void pause_disp( void )
{
	PAUSE_WORK *w = &pause_work ;
	NLVERTEX00 *vt ;
	NLVERTEX00 *v ;
	const PAUSE_MES *mes ;
	Uint32 col ;
	Float z ;
	Float rate ;

	if( !w->pause_flag ) return ;
	vt = pause_vtx[0] ;
	col = ( w->dark + 96 ) << 24 ;
	z = w->dark ? 0.8f : 0.99f ;
dark :
	v = vt ;
	do {
		v->col = col ;
		v->z = z ;
		v ++ ;
	} while( v < vt + 4 ) ;
	nlDispStripTrnsl( 4, vt ) ;

	vt += 4 ;
	rate = ( Float )( 11 - w->open_timer ) / 10.0f ;
	vt[0].x = vt[2].x = -( w->win_w * rate ) + option_w.adjust_x + 316.0f ;
	vt[1].x = vt[3].x = option_w.adjust_x + w->win_w * rate + 324.0f ;
	vt[0].y = vt[1].y = -( w->win_h * rate ) + option_w.adjust_y + 236.0f ;
	vt[2].y = vt[3].y = option_w.adjust_y + w->win_h * rate + 244.0f ;
	nlDispStripTrnsl( 4, vt ) ;

	vt += 4 ;
	vt[0].x = vt[2].x = -( w->win_w * rate ) + option_w.adjust_x + 320.0f ;
	vt[1].x = vt[3].x = option_w.adjust_x + w->win_w * rate + 320.0f ;
	vt[0].y = vt[1].y = -( w->win_h * rate ) + option_w.adjust_y + 240.0f ;
	vt[2].y = vt[3].y = option_w.adjust_y + w->win_h * rate + 240.0f ;
	nlDispStripTrnsl( 4, vt ) ;

	if( w->open_timer ) return ;
	if( !w->open_flag ) return ;
	vt += 4 ;
	mes = &pause_mes[w->cursor_mes] ;
	vt[0].x = vt[2].x = option_w.adjust_x + mes->x * 4.0f + -4.0f ;
	vt[1].x = vt[3].x = option_w.adjust_x + mes->x_end * 4.0f + 8.0f ;
	vt[0].y = vt[1].y = option_w.adjust_y + mes->y * 4.0f + -2.0f ;
	vt[2].y = vt[3].y = option_w.adjust_y + mes->y * 4.0f + 17.0f ;
	nlDispStripTrnsl( 4, vt ) ;
}

/*==============================================================*/
/*			A pad was pulled out: the window says so			*/
/*==============================================================*/
static void pause_out_disp( void )
{
	pause_disp() ;
	if( !pause_work.open_flag ) return ;
	spr_mes_put( 13, Sys->net_pause + 13 ) ;
}

/*==============================================================*/
static void pause_out_move( PAUSE_WORK *w )
{
	Sys->join_flag = 1 ;
	if( !w->open_flag ){
		if( w->open_timer ) w->open_timer -- ;
		else {
			w->open_flag ++ ;
			w->open_timer = 1 ;
		}
	} else {
		if( w->dark_timer ) w->dark_timer -- ;
		else if( w->dark != 96 ) w->dark ++ ;
	}
	SysTransSet( pause_out_disp ) ;
}

/*==============================================================*/
/*			The pause's menu									*/
/*==============================================================*/
static void pause_menu( PAUSE_WORK *w, int side )
{
	const PAUSE_MES *mes ;
	Sint8 old ;
	int i ;

	mes = &pause_mes[0] ;
	for( i = 0 ; i < 3 ; i ++ ){
		STrPrint( mes->x, mes->y, mes->col, mes->mes ) ;
		mes ++ ;
	}
	old = w->cursor ;
	if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) w->cursor -- ;
	if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) w->cursor ++ ;
	w->cursor = ( w->cursor + 3 ) % 3 ;
	w->cursor_mes = w->cursor ;
	if( old != w->cursor ) na_req( 1 ) ;
	if( sw_work[side].on & decide_sw_get( side ) ){
		switch( w->cursor ){
		case 0 :
			pause_end( w ) ;
			break ;
		case 1 :
			w->dark = 0 ;
			w->unk_03 = 0 ;
			w->open_flag = 0 ;
			w->dark_timer = 18000 ;
			w->open_timer = 10 ;
			w->win_w = 200.0f ;
			w->win_h = 100.0f ;
			w->config_on = 1 ;
			w->config_open = 0 ;
			btn_config_back() ;
			break ;
		case 2 :
			w->page ++ ;
			w->cursor = 1 ;
			break ;
		}
		na_req( 2 ) ;
	}
}

/*==============================================================*/
/*			QUIT GAME?											*/
/*==============================================================*/
static void pause_quit( PAUSE_WORK *w, int side )
{
	const PAUSE_MES *mes ;
	Sint8 old ;
	int i ;

	mes = &pause_mes[3] ;
	for( i = 0 ; i < 3 ; i ++ ){
		STrPrint( mes->x, mes->y, mes->col, mes->mes ) ;
		mes ++ ;
	}
	old = w->cursor ;
	if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) w->cursor -- ;
	if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) w->cursor ++ ;
	w->cursor = ( w->cursor + 2 ) % 2 ;
	w->cursor_mes = w->cursor + 4 ;
	if( old != w->cursor ) na_req( 1 ) ;
	if( sw_work[side].on & decide_sw_get( side ) ){
		if( !w->cursor ){
			Sys->unk_50 = 1 ;
			soft_reset( 0 ) ;
			Sys->ended_mode = Sys->home_mode ;
		} else {
			w->page = 0 ;
			w->cursor = 2 ;
			if( Sys->home_mode == HOME_TRAINING ){
				w->page = 2 ;
				w->cursor = 3 ;
			}
		}
		na_req( 2 ) ;
	}
}

/*==============================================================*/
/*			The training's pause menu							*/
/*==============================================================*/
static void pause_menu_training( PAUSE_WORK *w, int side )
{
	const PAUSE_MES *mes ;
	Sint8 old ;
	int i ;

	mes = &pause_mes[6] ;
	for( i = 0 ; i < 4 ; i ++ ){
		STrPrint( mes->x, mes->y, mes->col, mes->mes ) ;
		mes ++ ;
	}
	old = w->cursor ;
	if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) w->cursor -- ;
	if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) w->cursor ++ ;
	w->cursor = ( w->cursor + 4 ) % 4 ;
	w->cursor_mes = w->cursor + 6 ;
	if( old != w->cursor ) na_req( 1 ) ;
	if( sw_work[side].on & decide_sw_get( side ) ){
		switch( w->cursor ){
		case 0 :
			pause_end( w ) ;
			break ;
		case 1 :
			training_work.menu_on = 1 ;
			training_work.page = 0 ;
			training_work.cursor = 0 ;
			training_work.cursor2 = 0 ;
			w->stop_keep = vs_sys_work.game_stop ;
			vs_sys_work.game_stop = 0 ;
			break ;
		case 2 :
			w->dark = 0 ;
			w->unk_03 = 0 ;
			w->open_flag = 0 ;
			w->dark_timer = 18000 ;
			w->open_timer = 10 ;
			w->win_w = 200.0f ;
			w->win_h = 100.0f ;
			w->config_on = 1 ;
			w->config_open = 0 ;
			btn_config_back() ;
			break ;
		case 3 :
			w->page = 1 ;
			w->cursor = 1 ;
			break ;
		}
		na_req( 2 ) ;
	}
}

/*==============================================================*/
/*			The pause, each frame: its window opens, then the	*/
/*			page's menu runs									*/
/*==============================================================*/
static void pause_move( PAUSE_WORK *w )
{
	static void ( *pause_move_jmp[] )( PAUSE_WORK *w, int side ) = {
		pause_menu,
		pause_quit,
		pause_menu_training,
	} ;
	int side ;

	Sys->join_flag = 1 ;
	w->config_flag = 0 ;
	if( !w->open_flag ){
		if( w->open_timer ) w->open_timer -- ;
		else w->open_flag ++ ;
	} else {
		side = 0 ;
		if( w->pause_flag != 1 ) side = 1 ;
		STrPrint( 68, 44, side + 32, "%dP PAUSE", side + 1 ) ;
		pause_move_jmp[w->page]( w, side ) ;
		if( sw_work[side].now ){
			w->dark = 0 ;
			w->dark_timer = 18000 ;
		}
		if( w->dark_timer ) w->dark_timer -- ;
		else if( w->dark != 96 ) w->dark ++ ;
	}
	SysTransSet( pause_disp ) ;
}

/*==============================================================*/
/*			The button config: its settings to the options		*/
/*==============================================================*/
static void btn_config_keep( void )
{
	BTN_WORK *b ;
	int side ;

	option_w.vib_bit = 0 ;
	for( side = 0 ; side < 2 ; side ++ ){
		b = &btn_work[side] ;
		option_w.vib_bit |= b->vib << side ;
		option_w.com_time[side] = b->com_time ;
		option_w.key_config[side][0] = b->key[0] ;
		option_w.key_config[side][1] = b->key[1] ;
		option_w.key_config[side][2] = b->key[2] ;
		option_w.key_config[side][3] = b->key[3] ;
		option_w.key_config[side][4] = b->key[4] ;
		option_w.key_config[side][5] = b->key[5] ;
	}
}

/*==============================================================*/
/*			And the options to its settings, as it opens		*/
/*==============================================================*/
static void btn_config_back( void )
{
	BTN_WORK *b ;
	int side ;
	OPTION_WORK *ow = &option_w ;

	for( side = 0 ; side < 2 ; side ++ ){
		b = &btn_work[side] ;
		b->cursor = 0 ;
		b->side = side ;
		b->vib = ( ow->vib_bit >> side ) & 1 ;
		b->com_time = ow->com_time[side] ;
		b->key[0] = ow->key_config[side][0] ;
		b->key[1] = ow->key_config[side][1] ;
		b->key[2] = ow->key_config[side][2] ;
		b->key[3] = ow->key_config[side][3] ;
		b->key[4] = ow->key_config[side][4] ;
		b->key[5] = ow->key_config[side][5] ;
	}
}

/*==============================================================*/
/*			The button config's window, with each side's cursor	*/
/*==============================================================*/
static void btn_config_disp( void )
{
	BTN_WORK *b ;
	NLVERTEX00 *vt ;
	const PAUSE_MES *mes ;
	int side ;

	pause_disp() ;
	if( pause_work.config_open ){
		vt = pause_vtx[3] ;
		for( side = 0 ; side < 2 ; side ++ ){
			b = &btn_work[side] ;
			mes = &pause_mes[b->cursor_mes] ;
			vt[0].x = vt[2].x = option_w.adjust_x + ( mes->x + side * 53 ) * 4.0f + -4.0f ;
			vt[1].x = vt[3].x = option_w.adjust_x + ( mes->x_end + side * 53 ) * 4.0f + 8.0f ;
			vt[0].y = vt[1].y = option_w.adjust_y + ( b->cursor + 11 ) * 16.0f + -2.0f ;
			vt[2].y = vt[3].y = option_w.adjust_y + ( b->cursor + 11 ) * 16.0f + 17.0f ;
			nlDispStripTrnsl( 4, vt ) ;
		}
	}
}

/*==============================================================*/
/*			Lines of a side's column							*/
/*==============================================================*/
static void btn_config_line( BTN_WORK *b, int no, int cnt )
{
	const PAUSE_MES *mes ;
	int i ;

	mes = &pause_mes[no] ;
	for( i = 0 ; i < cnt ; i ++ ){
		STrPrint( mes->x + b->side * 53, mes->y, mes->col, mes->mes ) ;
		mes ++ ;
	}
}

/*==============================================================*/
/*			A side's column: its cursor picks a line, and left	*/
/*			and right change the line's setting					*/
/*==============================================================*/
static void btn_config_pl( PAUSE_WORK *w, BTN_WORK *b )
{
	const PAUSE_MES *mes ;
	Sint8 *item ;
	Sint8 old ;
	int max ;
	int i ;

	STrPrint( b->side * 53 + 30, 40, b->side + 32, "PLAYER%d", b->side + 1 ) ;
	if( port_peri[b->side]->support & ( PDD_DGT_TC | PDD_DGT_TZ ) ){
		btn_config_line( b, 10, 8 ) ;
		btn_config_line( b, 36, 2 ) ;
	} else {
		btn_config_line( b, 10, 10 ) ;
	}
	old = b->cursor ;
	if( ( sw_work[b->side].on | sw_work[b->side].rep ) & KEY_UP ) b->cursor -- ;
	if( ( sw_work[b->side].on | sw_work[b->side].rep ) & KEY_DOWN ) b->cursor ++ ;
	b->cursor = ( b->cursor + 10 ) % 10 ;
	if( old != b->cursor ) na_req( 1 ) ;
	switch( b->cursor ){
	case 0 :
	case 1 :
	case 2 :
	case 3 :
	case 4 :
	case 5 :
		max = 12 ;
		b->cursor_mes = 20 ;
		item = &b->key[b->cursor] ;
		break ;
	case 6 :
		b->cursor_mes = 32 ;
		max = 2 ;
		item = &b->vib ;
		break ;
	case 7 :
		b->cursor_mes = 34 ;
		max = 2 ;
		item = &b->com_time ;
		break ;
	case 8 :
		b->cursor_mes = 18 ;
		max = -1 ;
		if( sw_work[b->side].on & decide_sw_get( b->side ) ){
			b->vib = 0 ;
			b->com_time = 0 ;
			b->key[0] = 0 ;
			b->key[1] = 1 ;
			b->key[2] = 2 ;
			b->key[3] = 3 ;
			b->key[4] = 4 ;
			b->key[5] = 5 ;
			na_req( 2 ) ;
		}
		break ;
	case 9 :
		b->cursor_mes = 19 ;
		max = -1 ;
		if( sw_work[b->side].on & decide_sw_get( b->side ) ){
			w->config_on = 0 ;
			w->config_open = 0 ;
			na_req( 2 ) ;
		}
		break ;
	}
	if( max > 0 ){
		old = *item ;
		if( ( sw_work[b->side].on | sw_work[b->side].rep ) & KEY_LEFT ) ( *item ) -- ;
		if( ( sw_work[b->side].on | sw_work[b->side].rep ) & KEY_RIGHT ) ( *item ) ++ ;
		*item = ( *item + max ) % max ;
		if( old != *item ){
			btn_config_keep() ;
			na_req( 4 ) ;
			if( b->cursor == 6 && *item ) vib_req( b->side, 8 ) ;
		}
		b->cursor_mes += *item ;
	}
	for( i = 0 ; i < 6 ; i ++ ){
		mes = &pause_mes[b->key[i] + 20] ;
		STrPrint( mes->x + b->side * 53, mes->y + i * 4, mes->col + i, mes->mes ) ;
	}
	mes = &pause_mes[b->vib + 32] ;
	STrPrint( mes->x + b->side * 53, mes->y, mes->col, mes->mes ) ;
	mes = &pause_mes[b->com_time + 34] ;
	STrPrint( mes->x + b->side * 53, mes->y, mes->col, mes->mes ) ;
}

/*==============================================================*/
/*			The button config, each frame: both sides at once	*/
/*==============================================================*/
static void btn_config_move( PAUSE_WORK *w )
{
	Sys->join_flag = 1 ;
	w->config_flag = 1 ;
	if( !w->config_open ){
		if( w->open_timer ) w->open_timer -- ;
		else w->config_open ++ ;
	} else {
		STrPrint( 60, 34, 51, "BUTTON CONFIG." ) ;
		btn_config_pl( w, &btn_work[0] ) ;
		btn_config_pl( w, &btn_work[1] ) ;
		if( sw_work[0].now || sw_work[1].now ){
			w->dark = 0 ;
			w->dark_timer = 18000 ;
		}
		if( w->dark_timer ) w->dark_timer -- ;
		else if( w->dark != 96 ) w->dark ++ ;
	}
	SysTransSet( btn_config_disp ) ;
	if( !w->config_on ){
		w->dark = 0 ;
		w->unk_03 = 0 ;
		w->open_flag = 0 ;
		w->dark_timer = 18000 ;
		w->open_timer = 10 ;
		w->win_w = 80.0f ;
		w->win_h = 80.0f ;
		btn_config_keep() ;
	}
}

/*==============================================================*/
/*			The training menu's settings to the game's rules	*/
/*==============================================================*/
static void training_rule_set( TRAINING_WORK *tw )
{
	tw->menu_on = 0 ;
	Sys->dummy_guard = tw->dummy_guard ;
	Sys->dummy_air_recover = tw->dummy_air_recover ;
	Sys->dummy_change = tw->dummy_change ;
	Sys->auto_gauge = tw->auto_gauge ;
	Sys->cpu_renda_level = tw->renda_level ;
	Sys->damage_level = tw->damage_level ;
	Sys->game_speed = tw->game_speed ;
	Sys->damage_disp = tw->damage_disp ;
	Sys->delay_hyper_free = tw->delay_hyper_free ;
	vs_rule_set( &vs_sys_work ) ;
}

/****************************************************************/
/*			The training mode's rules, as a game starts			*/
/****************************************************************/
void training_init( void )
{
	TRAINING_WORK *tw = &training_work ;

	tw->menu_on = 0 ;
	tw->side = 0 ;
	tw->page = 0 ;
	tw->cursor = 0 ;
	tw->cursor2 = 0 ;
	tw->unk_03 = 0 ;
	tw->unk_04 = 0 ;
	tw->stance = 0 ;
	tw->dummy_guard = 0 ;
	tw->dummy_air_recover = 0 ;
	tw->dummy_change = 0 ;
	tw->damage_disp = 0 ;
	tw->delay_hyper_free = 0 ;
	tw->renda_level = 1 ;
	tw->damage_level = 1 ;
	tw->game_speed = 1 ;
	tw->auto_gauge = 0 ;
	training_rule_set( tw ) ;
	Sys->no_ko_flag = 0 ;
	if( Sys->home_mode != HOME_TRAINING ) return ;
	Sys->no_ko_flag = 1 ;
	tw->side = Sys->entry_side ;
}

/****************************************************************/
void training_exit( void )
{
}

/*==============================================================*/
/*			The fight again from its start, with the menu's		*/
/*			settings											*/
/*==============================================================*/
static void training_restart( void )
{
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->join_flag = 0 ;
	Sys->heart_step = 0 ;
	Sys->entry_stop = 1 ;
	work_init() ;
	training_rule_set( &training_work ) ;
	pause_end( &pause_work ) ;
	back_color_set( 0xff000000 ) ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
	load_stop() ;
	fight_load_start( 12 ) ;
}

/*==============================================================*/
/*			The training menu's first page						*/
/*==============================================================*/
static void training_menu_0( TRAINING_WORK *tw, int side )
{
	Sint8 old ;

	switch( tw->step[0] ){
	case 0 :
		tw->step[0] ++ ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ) tw->step[0] ++ ;
		break ;
	case 2 :
		old = tw->cursor ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) tw->cursor -- ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) tw->cursor ++ ;
		tw->cursor = ( tw->cursor + 5 ) % 5 ;
		if( old != tw->cursor ) na_req( 1 ) ;
		if( sw_work[side].on & decide_sw_get( side ) ){
			tw->step[0] ++ ;
			tw->step[1] = tw->cursor ;
			na_req( 2 ) ;
			if( tw->step[1] ) fade_set( 0xff000000, 10, 1 ) ;
		} else if( sw_work[side].on & KEY_HK ){
			tw->unk_1c = 0 ;
			training_rule_set( tw ) ;
			tw->step[0] = tw->step[1] = 0 ;
			na_req( 3 ) ;
		}
		break ;
	case 3 :
		if( fade_cnt() ) break ;
		switch( tw->step[1] ){
		case 0 :
			tw->unk_1c = 0 ;
			training_rule_set( tw ) ;
			tw->step[0] = tw->step[1] = 0 ;
			vs_sys_work.game_stop = pause_work.stop_keep ;
			break ;
		case 1 :
			tw->unk_1c = 3 ;
			tw->page = tw->cursor ;
			tw->cursor2 = tw->unk_04 ;
			tw->unk_03 = 0 ;
			pause_sub_win_set() ;
			pause_sub_cursor_set() ;
			break ;
		case 2 :
			pause_cfg_set() ;
			tw->unk_1c = 3 ;
			tw->page = tw->cursor ;
			tw->unk_03 = tw->cursor2 = 0 ;
			break ;
		case 3 :
			Sys->play_side = 0 ;
			Sys->new_side = Sys->entry_side ;
			Sys->join_side = Sys->entry_side ;
			tw->unk_1c = 0 ;
			training_restart() ;
			break ;
		case 4 :
			tw->page = 3 ;
			tw->cursor2 = 1 ;
			tw->unk_03 = 0 ;
			tw->unk_1c = 5 ;
			pause_yn_win_set() ;
			pause_yn_cursor_set() ;
			break ;
		}
		tw->step[0] = tw->step[1] = 0 ;
		break ;
	}
}

/*==============================================================*/
/*			Its second: who plays the dummy						*/
/*==============================================================*/
static void training_menu_1( TRAINING_WORK *tw, int side )
{
	Sint8 old ;

	switch( tw->step[0] ){
	case 0 :
		tw->step[0] ++ ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ) tw->step[0] ++ ;
		break ;
	case 2 :
		old = tw->cursor2 ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) tw->cursor2 -- ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) tw->cursor2 ++ ;
		tw->cursor2 = ( tw->cursor2 + 4 ) % 4 ;
		if( old != tw->cursor2 ) na_req( 1 ) ;
		if( sw_work[side].on & decide_sw_get( side ) ){
			tw->step[0] ++ ;
			tw->step[1] = tw->cursor2 ;
			fade_set( 0xff000000, 10, 1 ) ;
			na_req( 2 ) ;
		} else if( sw_work[side].on & KEY_HK ){
			tw->step[0] ++ ;
			tw->step[1] = 3 ;
			fade_set( 0xff000000, 10, 1 ) ;
			na_req( 3 ) ;
		}
		break ;
	case 3 :
		if( fade_cnt() ) break ;
		switch( tw->step[1] ){
		case 0 :
			if( tw->unk_04 ){
				tw->unk_04 = 0 ;
				tw->side = 1 << side ;
				tw->stance = 0 ;
				tw->dummy_guard = 0 ;
				tw->dummy_air_recover = 0 ;
				tw->dummy_change = 0 ;
				Sys->entry_side = 1 << side ;
				Sys->play_side = 0 ;
				Sys->new_side = Sys->entry_side ;
				Sys->join_side = Sys->entry_side ;
				training_restart() ;
			} else {
				tw->unk_1c = 4 ;
				tw->page = 4 ;
				pause_opt_set() ;
			}
			break ;
		case 1 :
			tw->unk_04 = 1 ;
			tw->side = -1 ;
			tw->stance = 0 ;
			tw->dummy_guard = 0 ;
			tw->dummy_air_recover = 0 ;
			tw->dummy_change = 0 ;
			Sys->entry_side = 1 << side ;
			Sys->play_side = 0 ;
			Sys->new_side = Sys->entry_side ;
			Sys->join_side = Sys->entry_side ;
			training_restart() ;
			break ;
		case 2 :
			tw->unk_04 = 2 ;
			tw->side = 0 ;
			tw->stance = 0 ;
			tw->dummy_guard = 0 ;
			tw->dummy_air_recover = 0 ;
			tw->dummy_change = 0 ;
			Sys->entry_side = 3 ;
			Sys->play_side = 0 ;
			Sys->new_side = Sys->entry_side ;
			Sys->join_side = Sys->entry_side ;
			training_restart() ;
			break ;
		case 3 :
			tw->page = 0 ;
			tw->unk_1c = 1 ;
			break ;
		}
		tw->step[0] = tw->step[1] = 0 ;
		break ;
	}
}

/*==============================================================*/
/*			Its third: the game's settings						*/
/*==============================================================*/
static void training_menu_2( TRAINING_WORK *tw, int side )
{
	Sint8 *item ;
	Sint8 old ;
	int max ;

	switch( tw->step[0] ){
	case 0 :
		tw->step[0] ++ ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ) tw->step[0] ++ ;
		break ;
	case 2 :
		old = tw->cursor2 ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) tw->cursor2 -- ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) tw->cursor2 ++ ;
		tw->cursor2 = ( tw->cursor2 + 8 ) % 8 ;
		if( old != tw->cursor2 ) na_req( 1 ) ;
		if( sw_work[side].on & KEY_HK ){
			tw->step[0] ++ ;
			fade_set( 0xff000000, 10, 1 ) ;
			na_req( 3 ) ;
			break ;
		}
		switch( tw->cursor2 ){
		case 0 :
			max = 2 ;
			item = &tw->auto_gauge ;
			break ;
		case 1 :
			max = 2 ;
			item = &tw->delay_hyper_free ;
			break ;
		case 2 :
			max = 2 ;
			item = &tw->damage_disp ;
			break ;
		case 3 :
			max = 8 ;
			item = &tw->renda_level ;
			break ;
		case 4 :
			max = 4 ;
			item = &tw->damage_level ;
			break ;
		case 5 :
			max = 3 ;
			item = &tw->game_speed ;
			break ;
		case 6 :
			max = -1 ;
			if( sw_work[side].on & decide_sw_get( side ) ){
				tw->renda_level = 1 ;
				tw->damage_level = 1 ;
				tw->game_speed = 1 ;
				tw->auto_gauge = 0 ;
				tw->damage_disp = 0 ;
				tw->delay_hyper_free = 0 ;
				na_req( 2 ) ;
			}
			break ;
		case 7 :
			max = -1 ;
			if( sw_work[side].on & decide_sw_get( side ) ){
				tw->step[0] ++ ;
				fade_set( 0xff000000, 10, 1 ) ;
				na_req( 2 ) ;
			}
			break ;
		}
		if( max > 0 ){
			old = *item ;
			if( ( sw_work[side].on | sw_work[side].rep ) & KEY_LEFT ) ( *item ) -- ;
			if( ( sw_work[side].on | sw_work[side].rep ) & KEY_RIGHT ) ( *item ) ++ ;
			*item = ( *item + max ) % max ;
			if( old != *item ) na_req( 4 ) ;
		}
		break ;
	case 3 :
		if( !fade_cnt() ){
			tw->page = 0 ;
			tw->unk_1c = 1 ;
			tw->step[0] = tw->step[1] = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
/*			Its fourth: QUIT TRAINING?							*/
/*==============================================================*/
static void training_menu_3( TRAINING_WORK *tw, int side )
{
	Sint8 old ;

	switch( tw->step[0] ){
	case 0 :
		tw->step[0] ++ ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ) tw->step[0] ++ ;
		break ;
	case 2 :
		old = tw->cursor2 ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) tw->cursor2 -- ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) tw->cursor2 ++ ;
		tw->cursor2 = ( tw->cursor2 + 2 ) % 2 ;
		if( old != tw->cursor2 ) na_req( 1 ) ;
		if( sw_work[side].on & decide_sw_get( side ) ){
			tw->step[0] ++ ;
			tw->step[1] = tw->cursor2 ;
			na_req( 2 ) ;
			fade_set( 0xff000000, 10, 1 ) ;
		} else if( sw_work[side].on & KEY_HK ){
			tw->step[0] ++ ;
			tw->step[1] = 1 ;
			fade_set( 0xff000000, 10, 1 ) ;
			na_req( 3 ) ;
		}
		break ;
	case 3 :
		if( !fade_cnt() ){
			if( !tw->step[1] ){
				Sys->unk_50 = 1 ;
				soft_reset( 0 ) ;
				Sys->ended_mode = Sys->home_mode ;
			} else {
				tw->page = 0 ;
				tw->unk_1c = 1 ;
			}
			tw->step[0] = tw->step[1] = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
/*			Its fifth: what the dummy does						*/
/*==============================================================*/
static void training_menu_4( TRAINING_WORK *tw, int side )
{
	Sint8 *item ;
	Sint8 old ;
	int max ;

	switch( tw->step[0] ){
	case 0 :
		tw->step[0] ++ ;
		fade_set( 0xff000000, 10, FADE_IN ) ;
		break ;
	case 1 :
		if( !fade_cnt() ) tw->step[0] ++ ;
		break ;
	case 2 :
		old = tw->unk_03 ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_UP ) tw->unk_03 -- ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_DOWN ) tw->unk_03 ++ ;
		tw->unk_03 = ( tw->unk_03 + 6 ) % 6 ;
		if( old != tw->unk_03 ) na_req( 1 ) ;
		if( sw_work[side].on & KEY_HK ){
			tw->step[0] ++ ;
			fade_set( 0xff000000, 10, 1 ) ;
			na_req( 3 ) ;
			return ;
		}
		switch( tw->unk_03 ){
		case 0 :
			max = 4 ;
			item = &tw->stance ;
			break ;
		case 1 :
			max = 3 ;
			item = &tw->dummy_guard ;
			break ;
		case 2 :
			max = 2 ;
			item = &tw->dummy_air_recover ;
			break ;
		case 3 :
			max = 2 ;
			item = &tw->dummy_change ;
			break ;
		case 4 :
			max = -1 ;
			if( sw_work[side].on & decide_sw_get( side ) ){
				tw->stance = 0 ;
				tw->dummy_guard = 0 ;
				tw->dummy_air_recover = 0 ;
				tw->dummy_change = 0 ;
				na_req( 2 ) ;
			}
			break ;
		case 5 :
			max = -1 ;
			if( sw_work[side].on & decide_sw_get( side ) ){
				tw->step[0] ++ ;
				fade_set( 0xff000000, 10, 1 ) ;
				na_req( 2 ) ;
			}
		}
		break ;
	case 3 :
		if( !fade_cnt() ){
			tw->unk_1c = 3 ;
			tw->step[0] = tw->step[1] = 0 ;
			tw->page = 1 ;
			tw->cursor2 = tw->unk_04 ;
			tw->unk_03 = 0 ;
			pause_sub_win_set() ;
			pause_sub_cursor_set() ;
		}
		break ;
	}
	if( max > 0 ){
		old = *item ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_LEFT ) ( *item ) -- ;
		if( ( sw_work[side].on | sw_work[side].rep ) & KEY_RIGHT ) ( *item ) ++ ;
		*item = ( *item + max ) % max ;
		if( old != *item ) na_req( 4 ) ;
	}
}

/*==============================================================*/
/*			The training menu, each frame: its page				*/
/*==============================================================*/
static void training_menu( PAUSE_WORK *w, TRAINING_WORK *tw )
{
	static void ( *training_menu_jmp[] )( TRAINING_WORK *tw, int side ) = {
		training_menu_0,
		training_menu_1,
		training_menu_2,
		training_menu_3,
		training_menu_4,
	} ;
	int side ;

	Sys->join_flag = 1 ;
	w->config_flag = 1 ;
	side = 0 ;
	if( w->pause_flag != 1 ) side = 1 ;
	training_menu_jmp[tw->page]( tw, side ) ;
	SysTransSet( training_exit ) ;
}

/****************************************************************/
/*			The pause, each frame the fight is held				*/
/****************************************************************/
void pause_main( void )
{
	PAUSE_WORK *w = &pause_work ;
	TRAINING_WORK *tw = &training_work ;

	if( w->pause_flag & 4 ){
		pause_out_move( w ) ;
		return ;
	}
	if( w->config_on ){
		btn_config_move( w ) ;
		return ;
	}
	if( tw->menu_on ){
		switch( tw->unk_1c ){
		case 0 :
			tw->unk_1c = 1 ;
			pause_bg_set() ;
			tw->step[0] = tw->step[1] = 0 ;
			break ;
		case 1 :
			tw->unk_1c = 2 ;
			pause_win_set() ;
			pause_cursor_set() ;
			break ;
		}
		training_menu( w, tw ) ;
		work_list_move( 13 ) ;
		return ;
	}
	if( w->pause_flag & 3 ) pause_move( w ) ;
}

/****************************************************************/
/*			A network game's pause: the other console's			*/
/****************************************************************/
void net_pause_set( int flag )
{
	PAUSE_WORK *w ;

	Sys->net_pause = flag ;
	w = &pause_work ;
	w->pause_flag = 4 ;
	pause_out_set( w, 0 ) ;
	vib_stop() ;
	if( Sys->play_mode == 96 ) rand_pic_set() ;
}

/****************************************************************/
void net_pause_move( void )
{
	PAUSE_WORK *w ;

	if( Sys->home_mode != HOME_NETWORK ) return ;
	w = &pause_work ;
	if( Sys->net_pause ){
		Sys->join_flag = 1 ;
		if( !w->open_flag ){
			if( w->open_timer ) w->open_timer -- ;
			else {
				w->open_flag ++ ;
				w->open_timer = 1 ;
				w->dark_timer = 240 ;
			}
		} else {
			if( w->dark_timer ) w->dark_timer -- ;
			else Sys->net_pause = 0 ;
		}
	}
	SysTransSet( pause_out_disp ) ;
}

/****************************************************************/
/*			The network game's rules into the game's			*/
/****************************************************************/
void net_rule_set( void )
{
	NET_WORK *nw = &net_w ;

	Sys->damage_level = nw->rule[0] ;
	Sys->time_speed = nw->rule[1] ;
	Sys->round_time_no = nw->rule[2] ;
	Sys->speed_fix = nw->rule[3] ;
	Sys->auto_gauge = nw->rule[4] ;
	Sys->gauge_double = nw->rule[5] ;
	Sys->red_vital_keep = nw->rule[6] ;
	Sys->vital_drain = nw->rule[7] ;
	Sys->red_vital_rule = nw->rule[8] ;
	Sys->guard_dm_rule = nw->rule[9] ;
	Sys->share_vital = nw->rule[10] ;
	Sys->one_char = nw->rule[11] ;
	Sys->delay_hyper_free = nw->rule[12] ;
	Sys->armor_all = nw->rule[13] ;
	Sys->unk_b4 = nw->rule[14] ;
	Sys->partner_ng = 0 ;
	Sys->partner_free = 0 ;
	if( nw->rule[15] == 1 ) Sys->partner_ng = 1 ;
	if( nw->rule[15] == 2 ) Sys->partner_free = 1 ;
	Sys->one_char = 0 ;
	Sys->unk_b7 = 0 ;
	if( nw->rule[11] & 1 ) Sys->one_char = 1 ;
	if( nw->rule[11] & 2 ) Sys->unk_b7 = 1 ;
	if( !nw->open_or[0] && !nw->open_or[1] ){
		nw->open_or[0] = 0xffff77 ;
		nw->open_or[1] = 0x2000004 ;
	}
	nw->char_open[0] = ( vms_work.card_open[0] | nw->unk_70[0] | nw->open_or[0] ) & nw->open_and[0] ;
	nw->char_open[1] = ( vms_work.card_open[1] | nw->unk_70[1] | nw->open_or[1] ) & nw->open_and[1] ;
	nw->char_ex[0] = ( vms_work.card_ex[0] | nw->unk_78[0] | nw->ex_or[0] ) & nw->ex_and[0] ;
	nw->char_ex[1] = ( vms_work.card_ex[1] | nw->unk_78[1] | nw->ex_or[1] ) & nw->ex_and[1] ;
}

/****************************************************************/
/*			And their defaults									*/
/****************************************************************/
void net_rule_init( void )
{
	NET_WORK *nw = &net_w ;

	nw->rule[0] = 1 ;
	nw->rule[1] = 1 ;
	nw->rule[2] = 2 ;
	nw->rule[3] = 1 ;
	nw->rule[4] = 0 ;
	nw->rule[5] = 0 ;
	nw->rule[6] = 0 ;
	nw->rule[7] = 0 ;
	nw->rule[8] = 0 ;
	nw->rule[9] = 0 ;
	nw->rule[10] = 0 ;
	nw->rule[11] = 0 ;
	nw->rule[12] = 0 ;
	nw->rule[13] = 0 ;
	nw->rule[14] = 0 ;
	nw->rule[15] = 0 ;
	nw->open_and[0] = -1 ;
	nw->open_and[1] = -1 ;
	nw->ex_and[0] = -1 ;
	nw->ex_and[1] = -1 ;
	nw->open_or[0] = 0xffff77 ;
	nw->open_or[1] = 0x2000004 ;
	nw->ex_or[0] = 0 ;
	nw->ex_or[1] = 0 ;
	nw->char_open[0] = vms_work.card_open[0] | nw->unk_70[0] | nw->open_or[0] ;
	nw->char_open[1] = vms_work.card_open[1] | nw->unk_70[1] | nw->open_or[1] ;
	nw->char_ex[0] = vms_work.card_ex[0] | nw->unk_78[0] | nw->ex_or[0] ;
	nw->char_ex[1] = vms_work.card_ex[1] | nw->unk_78[1] | nw->ex_or[1] ;
	nw->def_team_bit[0][0] = 0x00080000 ;
	nw->def_team_bit[0][1] = 0x00100000 ;
	nw->def_team_bit[0][2] = 0x00200000 ;
	nw->def_team_bit[1][0] = 0x00800000 ;
	nw->def_team_bit[1][1] = 0x00400000 ;
	nw->def_team_bit[1][2] = 0x00000040 ;
	nw->def_team_tbl[0][0] = 0x13 ;
	nw->def_team_tbl[0][1] = 0x14 ;
	nw->def_team_tbl[0][2] = 0x15 ;
	nw->def_team_tbl[1][0] = 0x17 ;
	nw->def_team_tbl[1][1] = 0x16 ;
	nw->def_team_tbl[1][2] = 0x06 ;
}
#endif

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_sw.c" End of File	*/
