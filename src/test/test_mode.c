/* The test program's menus ("GAME TEST MODE"): the input test, the game data, the game
   configuration and the ROM board test. */
#include "player.h"
#include "book.h"
#include "naomi.h"
#include "shinobi.h"
#include "test.h"

/* SHC's built-ins, as its smachine.h names them */
#define set_imask( mask )	_builtin_set_imask( mask )
#define get_imask()	_builtin_get_imask()

#define TEST_SW_TEST	0x10	/* test_sw: the test switch */
#define TEST_SW_SERVICE	0x20	/* and the service switch */

#define COL_WHITE	0xffffffff
#define COL_RED		0xffff0000

/* a line of a menu; a list of them ends with one whose x is below 0 */
typedef struct TEST_MES {
	Sint32	x, y ;		/* the place, in characters */
	Uint32	col ;
	char	*str ;
} TEST_MES;

/* a ROM's sums: of its bytes and of its words */
typedef struct ROM_SUM {
	Uint16	b_sum ;
	Uint16	w_sum ;
} ROM_SUM;

extern const SYS_SET sys_set_tbl[8] ;
extern void sys_set_get( SYS_SET *set ) ;
extern Sint32 VBlankInIrq( void ) ;
extern void book_count_clear( void ) ;
extern void nlaUpdateEEPROMBootDefault( void ) ;

extern TEST_MES menu_mes[] ;
extern TEST_MES input_mes[] ;
extern TEST_MES data_mes[] ;
extern TEST_MES config_mes[] ;
extern TEST_MES help_mes[] ;
extern TEST_MES rom_mes[] ;

/* the menu's last line, EXIT: the M1 board's menu has its ROM board test before it */
#if ROM_BOARD == 2
#define MENU_EXIT	3
#else
#define MENU_EXIT	4
#endif

static int set_change ;		/* the settings are not the country's own */
static SYS_SET set_work ;	/* the settings while the menu changes them */
static SW_WORK test_sw ;	/* the board's switches, with the I/O board's test and service */
static int cmd_end ;		/* a key command is done: 1 entered, -1 missed */
static int cmd_cnt ;		/* the keys of it entered */
#if ROM_BOARD != 2	/* the M1 board's ROM test */
static Uint16 rom_sum_byte[16] ;	/* each ROM's sums */
static Uint16 rom_sum_word[16] ;
static Uint8 rom_ck_flag[16] ;		/* and whether it has been summed */
#endif
static int unk_0C0E3B64[2] ;		/* nothing uses it */

static void test_init( void ) ;
static void test_menu( void ) ;
static void menu_main( void ) ;
static void input_test( void ) ;
static void jamma_disp( void ) ;
static void vm_disp( void ) ;
static void pad_socket_disp( void ) ;
static void pad_disp( void ) ;
static void jamma_sw_disp( int x, int y, int side ) ;
static void pad_sw_disp( int x, int y, int side ) ;
static void exit_ck( void ) ;
static void next_ck( void ) ;
static void game_data( void ) ;
static void sound_mode_set( void ) ;
static void demo_sound_set( void ) ;
static void continue_set( void ) ;
static void voice_type_set( void ) ;
static void difficulty_set( void ) ;
static void damage_level_set( void ) ;
static void timer_speed_set( void ) ;
static void vm_kit_set( void ) ;
static void vm_write_set( void ) ;
static void game_speed_set( void ) ;
static void event_set( void ) ;
static void join_in_set( void ) ;
static void default_set( void ) ;
static int save_exit( void ) ;
static void key_cmd_ck( int no, const Uint16 *tbl ) ;
static void game_config( void ) ;
static void level_disp( int lv, int now, int max ) ;
static void test_exit( void ) ;
static void sys_set_read( SYS_SET *set ) ;
static void sys_set_write( SYS_SET *set ) ;
#if ROM_BOARD != 2
static void rom_sum_disp( void ) ;
static void rom_test( void ) ;
#endif

/* the settings, copied a member at a time */
#define SYS_SET_COPY( dst, src )	\
	( dst )->id = ( src )->id ; ( dst )->country = ( src )->country ;	\
	( dst )->stereo = ( src )->stereo ; ( dst )->demo_sound_half = ( src )->demo_sound_half ;	\
	( dst )->cont_enable = ( src )->cont_enable ; ( dst )->voice_type = ( src )->voice_type ;	\
	( dst )->difficulty = ( src )->difficulty ; ( dst )->damage_level = ( src )->damage_level ;	\
	( dst )->time_speed = ( src )->time_speed ; ( dst )->vms_mode = ( src )->vms_mode ;	\
	( dst )->vm_write = ( src )->vm_write ; ( dst )->speed_fix = ( src )->speed_fix ;	\
	( dst )->event_flag = ( src )->event_flag ; ( dst )->no_challenge_flag = ( src )->no_challenge_flag ;	\
	( dst )->open_lv = ( src )->open_lv ;

/* the key commands of the game configuration: six keys each, for Japan and for the rest */
static const Uint16 key_cmd_1[2][6] = {
	{ KEY_LK, KEY_HP, KEY_A2, KEY_A1, KEY_HK, KEY_LP },
	{ KEY_A2, KEY_A1, KEY_HK, KEY_HP, KEY_LK, KEY_LP },
} ;
static const Uint16 key_cmd_2[2][6] = {
	{ KEY_HK, KEY_HP, KEY_A1, KEY_A2, KEY_LP, KEY_LK },
	{ KEY_HK, KEY_A2, KEY_A1, KEY_HK, KEY_HP, KEY_HK },
} ;
static const Uint16 key_cmd_3[2][6] = {
	{ KEY_LP, KEY_HP, KEY_HK, KEY_A2, KEY_A1, KEY_LK },
	{ KEY_LP, KEY_HP, KEY_LP, KEY_A1, KEY_LK, KEY_HP },
} ;
static const Uint16 key_cmd_4[2][6] = {
	{ KEY_HP, KEY_LK, KEY_A1, KEY_HK, KEY_A2, KEY_LP },
	{ KEY_HP, KEY_LK, KEY_A1, KEY_LP, KEY_A2, KEY_LP },
} ;

#if ROM_BOARD != 2
/* the sums of the ROM board's ROMs, IC17 to IC32 */
static const ROM_SUM rom_sum_tbl[16] = {
	{ 0x363a, 0xa972 }, { 0x77d4, 0x7bdf }, { 0xa662, 0x1791 }, { 0x862a, 0xdafd },
	{ 0x1a1f, 0x6c5c }, { 0x1f1d, 0x822e }, { 0x203e, 0xefc2 }, { 0x62c4, 0xa4e4 },
	{ 0x448f, 0x94a9 }, { 0x1c8e, 0xdf93 }, { 0x7404, 0x5519 }, { 0x0a49, 0xe1f3 },
	{ 0xbc98, 0x5bc3 }, { 0x60e5, 0xc4cd }, { 0xe629, 0xefb2 }, { 0x7c8d, 0x0d62 },
} ;
#endif

/*==============================================================*/
/*			Print a list of lines								*/
/*==============================================================*/
static void mes_disp( TEST_MES *mes )
{
	while( 1 ){
		if( mes->x < 0 ) break ;
		nlLocate( mes->x, mes->y ) ;
		nlPrintColor( mes->col ) ;
		nlPuts( mes->str ) ;
		mes ++ ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			The cursor, left of a line							*/
/*==============================================================*/
static void cursor_disp( TEST_MES *mes )
{
	nlLocate( mes->x - 3, mes->y ) ;
	nlPrintf( "->" ) ;
}

/*==============================================================*/
/*			Do the settings differ from the country's own?		*/
/*==============================================================*/
static int sys_set_ck( SYS_SET *set )
{
	const SYS_SET *tbl = &sys_set_tbl[sys_flag->Country] ;
	int ret = 0 ;

	if( tbl->stereo != set->stereo ) ret = 1 ;
	if( tbl->demo_sound_half != set->demo_sound_half ) ret = 1 ;
	if( tbl->cont_enable != set->cont_enable ) ret = 1 ;
	if( tbl->voice_type != set->voice_type ) ret = 1 ;
	if( tbl->difficulty != set->difficulty ) ret = 1 ;
	if( tbl->damage_level != set->damage_level ) ret = 1 ;
	if( tbl->time_speed != set->time_speed ) ret = 1 ;
	if( tbl->vms_mode != set->vms_mode ) ret = 1 ;
	if( tbl->vm_write != set->vm_write ) ret = 1 ;
	if( tbl->speed_fix != set->speed_fix ) ret = 1 ;
	if( tbl->event_flag != set->event_flag ) ret = 1 ;
	if( tbl->no_challenge_flag != set->no_challenge_flag ) ret = 1 ;
	return ret ;
}

/*==============================================================*/
/*			The board's switches, with the I/O board's test		*/
/*			and service switches								*/
/*==============================================================*/
static void test_sw_get( SW_WORK *sw )
{
	sw->old = board_sw.old ;
	sw->now = board_sw.now ;
	sw->on = board_sw.on ;
	sw->off = board_sw.off ;
	if( sys_sw.old & 0x80 ) sw->old |= TEST_SW_TEST ;
	if( sys_sw.now & 0x80 ) sw->now |= TEST_SW_TEST ;
	if( sys_sw.on & 0x80 ) sw->on |= TEST_SW_TEST ;
	if( sys_sw.off & 0x80 ) sw->off |= TEST_SW_TEST ;
	if( sw_work[0].old & 0x4000 ) sw->old |= TEST_SW_SERVICE ;
	if( sw_work[0].now & 0x4000 ) sw->now |= TEST_SW_SERVICE ;
	if( sw_work[0].on & 0x4000 ) sw->on |= TEST_SW_SERVICE ;
	if( sw_work[0].off & 0x4000 ) sw->off |= TEST_SW_SERVICE ;
}

/****************************************************************/
/*			The test menus, each frame							*/
/****************************************************************/
void test_main( void )
{
	test_sw_get( &test_sw ) ;
	switch( Sys->task_no ){
	case 0 :
		test_init() ;
		break ;
	case 1 :
		test_menu() ;
		break ;
	}
}

/*==============================================================*/
static void test_init( void )
{
	nlSetFontSize( 16, 16 ) ;
	Sys->task_no = 1 ;
	Sys->task_step = 0 ;
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->timer0 = 0 ;
	Sys->timer1 = 0 ;
	Sys->timer2 = 0 ;
	Sys->timer3 = 0 ;
	set_change = 0 ;
	if( sys_set_ck( &sys_set ) ){
		menu_mes[3].col = COL_RED ;
		config_mes[13].col = COL_RED ;
		menu_mes[3].col = COL_RED ;
	} else {
		menu_mes[3].col = COL_WHITE ;
		config_mes[13].col = COL_WHITE ;
		menu_mes[3].col = COL_WHITE ;
	}
}

/*==============================================================*/
static void test_menu( void )
{
	switch( Sys->task_step ){
	case 0 :
		menu_main() ;
		break ;
	case 1 :
		switch( Sys->timer1 ){
		case 0 :
			input_test() ;
			break ;
		case 1 :
			game_data() ;
			break ;
		case 2 :
			game_config() ;
			break ;
#if ROM_BOARD != 2
		case 3 :
			rom_test() ;
			break ;
#endif
		case MENU_EXIT :
			test_exit() ;
			break ;
		}
		break ;
	}
}

/*==============================================================*/
/*			The menu: pick a test								*/
/*==============================================================*/
static void menu_main( void )
{
	if( sw_work[0].on & KEY_UP ){
		Sys->timer1 -- ;
		if( Sys->timer1 < 0 ) Sys->timer1 = MENU_EXIT ;
	}
	if( sw_work[0].on & KEY_DOWN ){
		Sys->timer1 ++ ;
		if( Sys->timer1 > MENU_EXIT ) Sys->timer1 = 0 ;
	}
	if( test_sw.on & TEST_SW_SERVICE ){
		Sys->timer1 ++ ;
		if( Sys->timer1 > MENU_EXIT ) Sys->timer1 = 0 ;
	}
	if( ( sw_work[0].on & KEY_LP ) || ( test_sw.on & TEST_SW_TEST ) ){
		Sys->task_step = 1 ;
		cmd_end = 0 ;
		cmd_cnt = 0 ;
		sys_set_read( &set_work ) ;
	}
	switch( Sys->timer1 ){
	case 0 :
		mes_disp( &help_mes[11] ) ;
		break ;
	case 1 :
		mes_disp( &help_mes[11] ) ;
		break ;
	case 2 :
		mes_disp( &help_mes[11] ) ;
		break ;
#if ROM_BOARD != 2
	case 3 :
		mes_disp( &help_mes[11] ) ;
		break ;
#endif
	case MENU_EXIT :
		mes_disp( &help_mes[14] ) ;
		break ;
	}
	mes_disp( menu_mes ) ;
	cursor_disp( menu_mes + 1 + Sys->timer1 ) ;
}

/*==============================================================*/
/*			The input test: the I/O board's switches, then		*/
/*			the memory cards and the controllers of the kit		*/
/*==============================================================*/
static void input_test( void )
{
	mes_disp( input_mes ) ;
	switch( sys_set.vms_mode ){
	case 0 :
		jamma_disp() ;
		exit_ck() ;
		break ;
	case 1 :
		if( !Sys->scene_no ){
			nlLocate( 12, 4 ) ;
			nlPrintf( "1/2 JAMMA TEST" ) ;
			jamma_disp() ;
			next_ck() ;
		} else {
			vm_disp() ;
			exit_ck() ;
		}
		break ;
	case 2 :
		switch( Sys->scene_no ){
		case 0 :
			nlLocate( 12, 4 ) ;
			nlPrintf( "1/3 JAMMA TEST" ) ;
			jamma_disp() ;
			next_ck() ;
			break ;
		case 1 :
			pad_socket_disp() ;
			next_ck() ;
			break ;
		case 2 :
			pad_disp() ;
			exit_ck() ;
			break ;
		}
		break ;
	}
}

/*==============================================================*/
static void jamma_disp( void )
{
	nlLocate( 4, 7 ) ;
	nlPrintf( "TEST        " ) ;
	if( test_sw.now & TEST_SW_TEST ) nlPrintf( "1" ) ;
	else nlPrintf( "0" ) ;
	nlLocate( 4, 10 ) ;
	nlPrintf( "SERVICE\n" ) ;
	nlPrintf( "START\n\n" ) ;
	nlPrintf( "LEVER\n\n\n\n" ) ;
	nlPrintf( "SHOT1\n" ) ;
	nlPrintf( "SHOT2\n" ) ;
	nlPrintf( "SHOT3\n" ) ;
	nlPrintf( "SHOT4\n" ) ;
	nlPrintf( "SHOT5\n" ) ;
	nlPrintf( "SHOT6\n" ) ;
	jamma_sw_disp( 14, 9, 0 ) ;
	jamma_sw_disp( 24, 9, 1 ) ;
}

/*==============================================================*/
static void vm_disp( void )
{
	nlLocate( 10, 4 ) ;
	nlPrintf( "2/2 MEMORY CARD TEST" ) ;
	nlLocate( 4, 8 ) ;
	nlPrintf( "1P VM  SOCKET\n\n\n" ) ;
	nlPrintf( "2P VM  SOCKET\n\n\n" ) ;
	nlLocate( 30, 8 ) ;
	if( peri_bit & 4 ) nlPrintf( "ON\n\n\n" ) ;
	else nlPrintf( "OFF\n\n\n" ) ;
	if( peri_bit & 8 ) nlPrintf( "ON\n\n\n" ) ;
	else nlPrintf( "OFF\n\n\n" ) ;
}

/*==============================================================*/
static void pad_socket_disp( void )
{
	nlLocate( 10, 4 ) ;
	nlPrintf( "2/3 MEMORY CARD TEST" ) ;
	nlLocate( 4, 8 ) ;
	nlPrintf( "1P CONTROLLER SOCKET1\n\n\n" ) ;
	nlPrintf( "2P CONTROLLER SOCKET1\n\n\n" ) ;
	nlLocate( 4, 9 ) ;
	nlPrintf( "              SOCKET2\n\n\n" ) ;
	nlPrintf( "              SOCKET2\n\n\n" ) ;
	nlLocate( 30, 8 ) ;
	if( peri_bit & 4 ) nlPrintf( "ON\n" ) ;
	else nlPrintf( "OFF\n" ) ;
	if( peri_bit & 0x10 ) nlPrintf( "ON\n\n" ) ;
	else nlPrintf( "OFF\n\n" ) ;
	if( peri_bit & 8 ) nlPrintf( "ON\n" ) ;
	else nlPrintf( "OFF\n" ) ;
	if( peri_bit & 0x20 ) nlPrintf( "ON\n\n" ) ;
	else nlPrintf( "OFF\n\n" ) ;
}

/*==============================================================*/
static void pad_disp( void )
{
	nlLocate( 10, 4 ) ;
	nlPrintf( "3/3 CONTROLLER TEST" ) ;
	nlLocate( 4, 11 ) ;
	nlPrintf( "START\n\n" ) ;
	nlPrintf( "LEVER\n\n\n\n" ) ;
	nlPrintf( "SHOTXY\n" ) ;
	nlPrintf( "SHOTAB\n" ) ;
	nlPrintf( "TRIGGER L\n" ) ;
	nlPrintf( "TRIGGER R\n" ) ;
	nlPrintf( "ANALOG X\n" ) ;
	nlPrintf( "ANALOG Y\n" ) ;
	pad_sw_disp( 14, 7, 0 ) ;
	pad_sw_disp( 25, 7, 1 ) ;
}

/*==============================================================*/
/*			A side's switches of the I/O board					*/
/*==============================================================*/
static void jamma_sw_disp( int x, int y, int side )
{
	nlLocate( x, y ) ;
	nlPrintf( "%dP\n", side + 1 ) ;
	if( sw_work[side].now & 0x4000 ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	if( sw_work[side].now & KEY_START ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	nlPrintf( "\n" ) ;
	switch( sw_work[side].now & KEY_LEVER ){
	case KEY_UP | KEY_LEFT :
		nlPrintf( "100\n0 0\n000\n" ) ;
		break ;
	case KEY_UP :
		nlPrintf( "010\n0 0\n000\n" ) ;
		break ;
	case KEY_UP | KEY_RIGHT :
		nlPrintf( "001\n0 0\n000\n" ) ;
		break ;
	case KEY_LEFT :
		nlPrintf( "000\n1 0\n000\n" ) ;
		break ;
	case KEY_RIGHT :
		nlPrintf( "000\n0 1\n000\n" ) ;
		break ;
	case KEY_DOWN | KEY_LEFT :
		nlPrintf( "000\n0 0\n100\n" ) ;
		break ;
	case KEY_DOWN :
		nlPrintf( "000\n0 0\n010\n" ) ;
		break ;
	case KEY_DOWN | KEY_RIGHT :
		nlPrintf( "000\n0 0\n001\n" ) ;
		break ;
	default :
		nlPrintf( "000\n0 0\n000\n" ) ;
		break ;
	}
	nlPrintf( "\n" ) ;
	if( sw_work[side].now & KEY_LP ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	if( sw_work[side].now & KEY_HP ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	if( sw_work[side].now & KEY_A1 ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	if( sw_work[side].now & KEY_LK ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	if( sw_work[side].now & KEY_HK ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
	if( sw_work[side].now & KEY_A2 ) nlPrintf( "  1\n" ) ;
	else nlPrintf( "  0\n" ) ;
}

/*==============================================================*/
/*			A side's controller									*/
/*==============================================================*/
static void pad_sw_disp( int x, int y, int side )
{
	nlLocate( x, y ) ;
	nlPrintf( "%dP\n", side + 1 ) ;
	nlLocate( x - 2, y + 1 ) ;
	nlPrintf( "CONTROLLER\n" ) ;
	nlLocate( x, y + 3 ) ;
	if( peri_bit & ( 1 << side ) ) nlPrintf( "ON\n" ) ;
	else nlPrintf( "OFF\n" ) ;
	if( pad_peri[side]->on & PDD_DGT_ST ) nlPrintf( "1\n\n" ) ;
	else nlPrintf( "0\n\n" ) ;
	switch( pad_peri[side]->on & ( PDD_DGT_KU | PDD_DGT_KD | PDD_DGT_KL | PDD_DGT_KR ) ){
	case PDD_DGT_KU | PDD_DGT_KL :
		nlPrintf( "100\n0 0\n000\n" ) ;
		break ;
	case PDD_DGT_KU :
		nlPrintf( "010\n0 0\n000\n" ) ;
		break ;
	case PDD_DGT_KU | PDD_DGT_KR :
		nlPrintf( "001\n0 0\n000\n" ) ;
		break ;
	case PDD_DGT_KL :
		nlPrintf( "000\n1 0\n000\n" ) ;
		break ;
	case PDD_DGT_KR :
		nlPrintf( "000\n0 1\n000\n" ) ;
		break ;
	case PDD_DGT_KD | PDD_DGT_KL :
		nlPrintf( "000\n0 0\n100\n" ) ;
		break ;
	case PDD_DGT_KD :
		nlPrintf( "000\n0 0\n010\n" ) ;
		break ;
	case PDD_DGT_KD | PDD_DGT_KR :
		nlPrintf( "000\n0 0\n001\n" ) ;
		break ;
	default :
		nlPrintf( "000\n0 0\n000\n" ) ;
		break ;
	}
	nlPrintf( "\n" ) ;
	if( pad_peri[side]->on & PDD_DGT_TX ) nlPrintf( "1" ) ;
	else nlPrintf( "0" ) ;
	if( pad_peri[side]->on & PDD_DGT_TY ) nlPrintf( " 1\n" ) ;
	else nlPrintf( " 0\n" ) ;
	if( pad_peri[side]->on & PDD_DGT_TA ) nlPrintf( "1" ) ;
	else nlPrintf( "0" ) ;
	if( pad_peri[side]->on & PDD_DGT_TB ) nlPrintf( " 1\n" ) ;
	else nlPrintf( " 0\n" ) ;
	if( pad_peri[side]->on & PDD_DGT_TL ) nlPrintf( "1\n" ) ;
	else nlPrintf( "0\n" ) ;
	if( pad_peri[side]->on & PDD_DGT_TR ) nlPrintf( "1\n" ) ;
	else nlPrintf( "0\n" ) ;
	nlPrintf( "%d\n", pad_peri[side]->x1 ) ;
	nlPrintf( "%d\n", pad_peri[side]->y1 ) ;
}

/*==============================================================*/
/*			Back to the menu with the first side's start and	*/
/*			first button, or service and test					*/
/*==============================================================*/
static void exit_ck( void )
{
	nlLocate( 8, 25 ) ;
	nlPrintf( "EXIT = 1P START & SHOT1" ) ;
	if( ( ( sw_work[0].now & KEY_START ) && ( sw_work[0].on & KEY_LP ) )
	 || ( ( test_sw.now & TEST_SW_SERVICE ) && ( test_sw.on & TEST_SW_TEST ) ) ){
		Sys->task_step = 0 ;
		Sys->scene_no = 0 ;
	}
}

/*==============================================================*/
/*			And on to the next page								*/
/*==============================================================*/
static void next_ck( void )
{
	nlLocate( 8, 25 ) ;
	nlPrintf( "NEXT = 1P START & SHOT1" ) ;
	if( ( ( sw_work[0].now & KEY_START ) && ( sw_work[0].on & KEY_LP ) )
	 || ( ( test_sw.now & TEST_SW_SERVICE ) && ( test_sw.on & TEST_SW_TEST ) ) ){
		Sys->scene_no ++ ;
	}
}

/*==============================================================*/
/*			The game data: the book-keeping, and its clear		*/
/*==============================================================*/
static void game_data( void )
{
	BOOK_WORK *bk ;

	if( sw_work[0].on & KEY_UP ) Sys->timer2 = 1 - Sys->timer2 ;
	if( sw_work[0].on & KEY_DOWN ) Sys->timer2 = 1 - Sys->timer2 ;
	if( test_sw.on & TEST_SW_SERVICE ) Sys->timer2 = 1 - Sys->timer2 ;
	if( ( ( test_sw.on & TEST_SW_TEST ) || ( sw_work[0].on & KEY_LP ) ) && Sys->timer2 == 1 ) book_count_clear() ;
	bk = &book ;
	nlLocate( 27, 6 ) ;
	nlPrintf( " %d\n", bk->coin_all ) ;
	nlPrintf( " %d\n", bk->service_all ) ;
	nlPrintf( " %d\n", bk->play_all ) ;
	nlLocate( 27, 13 ) ;
	nlPrintf( " %d\n", bk->coin ) ;
	nlPrintf( " %d\n", bk->service ) ;
	nlPrintf( " %d\n", bk->play ) ;
	mes_disp( data_mes ) ;
	if( Sys->timer2 == 0 ) cursor_disp( &data_mes[11] ) ;
	else cursor_disp( &data_mes[10] ) ;
	if( ( ( sw_work[0].now & KEY_START ) && ( sw_work[0].on & KEY_LP ) )
	 || ( ( test_sw.now & TEST_SW_SERVICE ) && ( test_sw.on & TEST_SW_TEST ) ) ){
		Sys->task_step = 0 ;
		Sys->timer2 = 0 ;
	}
}

/* a setting's keys: one step up or down, around its range */
#define SET_UP( m, max )	{ set->m ++ ; if( set->m > max ) set->m = 0 ; }
#define SET_DOWN( m, max )	{ set->m -- ; if( set->m < 0 ) set->m = max ; }

/*==============================================================*/
/*			The game configuration's lines: SOUND MODE			*/
/*==============================================================*/
static void sound_mode_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 0 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_UP( stereo, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( stereo, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_DOWN( stereo, 1 )
		if( sw_work[0].on & KEY_LP ) SET_UP( stereo, 1 )
		if( sw_work[0].on & KEY_HP ) SET_DOWN( stereo, 1 )
		if( !sys_flag->Country ) key_cmd_ck( 1, key_cmd_1[0] ) ;
	}
	nlLocate( 20, config_mes[1].y ) ;
	if( set->stereo != now->stereo ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->stereo ){
	case 0 :
		nlPrintf( "MONAURAL" ) ;
		break ;
	case 1 :
		nlPrintf( "STEREO" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			DEMO SOUND HALF										*/
/*==============================================================*/
static void demo_sound_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 1 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_UP( demo_sound_half, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( demo_sound_half, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_DOWN( demo_sound_half, 1 )
		if( sw_work[0].on & KEY_LP ) SET_UP( demo_sound_half, 1 )
		if( sw_work[0].on & KEY_HP ) SET_DOWN( demo_sound_half, 1 )
		if( sys_flag->Country ) key_cmd_ck( 1, key_cmd_1[0] ) ;
	}
	nlLocate( 20, config_mes[2].y ) ;
	if( set->demo_sound_half != now->demo_sound_half ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->demo_sound_half ){
	case 0 :
		nlPrintf( "OFF" ) ;
		break ;
	case 1 :
		nlPrintf( "ON" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			CONTINUE: off in an event							*/
/*==============================================================*/
static void continue_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 2 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_UP( cont_enable, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( cont_enable, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_DOWN( cont_enable, 1 )
		if( sw_work[0].on & KEY_LP ) SET_UP( cont_enable, 1 )
		if( sw_work[0].on & KEY_HP ) SET_DOWN( cont_enable, 1 )
		if( sys_flag->Country ) key_cmd_ck( 3, key_cmd_3[0] ) ;
	}
	if( set_work.event_flag ) set->cont_enable = 0 ;
	nlLocate( 20, config_mes[3].y ) ;
	if( set->cont_enable != now->cont_enable ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->cont_enable ){
	case 0 :
		nlPrintf( "OFF" ) ;
		break ;
	case 1 :
		nlPrintf( "ON" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			VOICE TYPE											*/
/*==============================================================*/
static void voice_type_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 3 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_UP( voice_type, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( voice_type, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_DOWN( voice_type, 1 )
		if( sw_work[0].on & KEY_LP ) SET_UP( voice_type, 1 )
		if( sw_work[0].on & KEY_HP ) SET_DOWN( voice_type, 1 )
	}
	nlLocate( 20, config_mes[4].y ) ;
	if( set->voice_type != now->voice_type ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->voice_type ){
	case 0 :
		nlPrintf( "OFF" ) ;
		break ;
	case 1 :
		nlPrintf( "ON" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			DIFFICULTY											*/
/*==============================================================*/
static void difficulty_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 4 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( difficulty, 7 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( difficulty, 7 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( difficulty, 7 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( difficulty, 7 )
		if( sw_work[0].on & KEY_HP ) SET_UP( difficulty, 7 )
		if( !sys_flag->Country ) key_cmd_ck( 2, key_cmd_2[0] ) ;
		else key_cmd_ck( 4, key_cmd_4[0] ) ;
	}
	nlLocate( 20, config_mes[5].y ) ;
	nlPrintf( "EASY[" ) ;
	level_disp( set->difficulty, now->difficulty, 7 ) ;
	nlPrintf( "]HARD" ) ;
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			DAMAGE LEVEL										*/
/*==============================================================*/
static void damage_level_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 5 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( damage_level, 3 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( damage_level, 3 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( damage_level, 3 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( damage_level, 3 )
		if( sw_work[0].on & KEY_HP ) SET_UP( damage_level, 3 )
		if( !sys_flag->Country ) key_cmd_ck( 3, key_cmd_3[0] ) ;
		else key_cmd_ck( 2, key_cmd_2[0] ) ;
	}
	nlLocate( 20, config_mes[6].y ) ;
	nlPrintf( "LOW [" ) ;
	level_disp( set->damage_level, now->damage_level, 3 ) ;
	nlPrintf( "]HIGH" ) ;
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			TIMER SPEED											*/
/*==============================================================*/
static void timer_speed_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 6 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( time_speed, 3 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( time_speed, 3 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( time_speed, 3 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( time_speed, 3 )
		if( sw_work[0].on & KEY_HP ) SET_UP( time_speed, 3 )
		if( !sys_flag->Country ) key_cmd_ck( 4, key_cmd_4[0] ) ;
	}
	nlLocate( 20, config_mes[7].y ) ;
	nlPrintf( "LOW [" ) ;
	level_disp( set->time_speed, now->time_speed, 3 ) ;
	nlPrintf( "]HIGH" ) ;
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			VM-KIT TYPE											*/
/*==============================================================*/
static void vm_kit_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 7 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( vms_mode, 2 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( vms_mode, 2 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( vms_mode, 2 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( vms_mode, 2 )
		if( sw_work[0].on & KEY_HP ) SET_UP( vms_mode, 2 )
	}
	nlLocate( 20, config_mes[8].y ) ;
	if( set->vms_mode != now->vms_mode ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->vms_mode ){
	case 0 :
		nlPrintf( "NO USE" ) ;
		break ;
	case 1 :
		nlPrintf( "SEGA  " ) ;
		break ;
	case 2 :
		nlPrintf( "CAPCOM" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			VM WRITE											*/
/*==============================================================*/
static void vm_write_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 8 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( vm_write, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( vm_write, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( vm_write, 1 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( vm_write, 1 )
		if( sw_work[0].on & KEY_HP ) SET_UP( vm_write, 1 )
	}
	nlLocate( 20, config_mes[9].y ) ;
	if( set->vm_write != now->vm_write ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->vm_write ){
	case 0 :
		nlPrintf( "OFF" ) ;
		break ;
	case 1 :
		nlPrintf( "ON " ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			GAME SPEED											*/
/*==============================================================*/
static void game_speed_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 9 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( speed_fix, 2 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( speed_fix, 2 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( speed_fix, 2 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( speed_fix, 2 )
		if( sw_work[0].on & KEY_HP ) SET_UP( speed_fix, 2 )
	}
	nlLocate( 20, config_mes[10].y ) ;
	if( set->speed_fix != now->speed_fix ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->speed_fix ){
	case 0 :
		nlPrintf( "FREE SELECT" ) ;
		break ;
	case 1 :
		nlPrintf( "NORMAL" ) ;
		break ;
	case 2 :
		nlPrintf( "TURBO" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			EVENT												*/
/*==============================================================*/
static void event_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 10 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( event_flag, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( event_flag, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( event_flag, 1 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( event_flag, 1 )
		if( sw_work[0].on & KEY_HP ) SET_UP( event_flag, 1 )
	}
	nlLocate( 20, config_mes[11].y ) ;
	if( set->event_flag != now->event_flag ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->event_flag ){
	case 0 :
		nlPrintf( "OFF" ) ;
		break ;
	case 1 :
		nlPrintf( "ON " ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			JOIN-IN												*/
/*==============================================================*/
static void join_in_set( void )
{
	SYS_SET *set = &set_work ;
	SYS_SET *now = &sys_set ;

	if( Sys->timer2 == 11 ){
		mes_disp( &help_mes[0] ) ;
		if( sw_work[0].on & KEY_LEFT ) SET_DOWN( no_challenge_flag, 1 )
		if( test_sw.on & TEST_SW_TEST ) SET_UP( no_challenge_flag, 1 )
		if( sw_work[0].on & KEY_RIGHT ) SET_UP( no_challenge_flag, 1 )
		if( sw_work[0].on & KEY_LP ) SET_DOWN( no_challenge_flag, 1 )
		if( sw_work[0].on & KEY_HP ) SET_UP( no_challenge_flag, 1 )
	}
	nlLocate( 20, config_mes[12].y ) ;
	if( set->no_challenge_flag != now->no_challenge_flag ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	switch( set->no_challenge_flag ){
	case 0 :
		nlPrintf( "ON " ) ;
		break ;
	case 1 :
		nlPrintf( "OFF" ) ;
		break ;
	}
	nlPrintColor( COL_WHITE ) ;
}

/*==============================================================*/
/*			DEFAULT: the country's own settings					*/
/*==============================================================*/
static void default_set( void )
{
	SYS_SET *set = &set_work ;

	if( Sys->timer2 == 12 ){
		mes_disp( &help_mes[8] ) ;
		if( ( ( sw_work[0].now & KEY_LP ) && ( sw_work[0].now & KEY_HP ) ) || ( test_sw.on & TEST_SW_TEST ) ){
			sys_set_get( set ) ;
			set_change = 1 ;
		}
	}
}

/*==============================================================*/
/*			SAVE & EXIT: write the settings to the EEPROM,		*/
/*			and the boot ROM's too if DEFAULT was taken			*/
/*==============================================================*/
static int save_exit( void )
{
	volatile int i ;
	int mask ;
	SYS_SET *set = &set_work ;

	if( Sys->timer2 == 13 ){
		if( ( sw_work[0].on & KEY_LP ) || ( test_sw.on & TEST_SW_TEST ) ){
			nlLocate( 5, 10 ) ;
			nlPrintColor( COL_WHITE ) ;
			nlPrintf( "SAVING NEW CONFIGURATION\n\n" ) ;
			nlPrintf( "                  IN EEPROM ...\n" ) ;
			nlWaitVSync() ;
			nlWaitVSync() ;
			sys_set_write( set ) ;
			nlSetIntrFunction( 1, 0 ) ;
			if( set_change ){
				mask = get_imask() ;
				set_imask( 14 ) ;
				nlaUpdateEEPROMBootDefault() ;
				i = 20000000 ;
				while( i -- ) ;
				set_imask( mask ) ;
				set_change = 0 ;
				mask = get_imask() ;
				set_imask( 14 ) ;
				nlaUpdateUserEEPROM() ;
				i = 20000000 ;
				while( i -- ) ;
				set_imask( mask ) ;
			} else {
				mask = get_imask() ;
				set_imask( 14 ) ;
				nlaUpdateUserEEPROM() ;
				i = 20000000 ;
				while( i -- ) ;
				set_imask( mask ) ;
			}
			nlSetIntrFunction( 1, VBlankInIrq ) ;
			Sys->task_step = 0 ;
			Sys->timer2 = 0 ;
			return 1 ;
		}
		mes_disp( &help_mes[4] ) ;
	}
	return 0 ;
}

/*==============================================================*/
/*			A key command: the keys of its table in turn, on	*/
/*			its own line, open the next of the hidden settings	*/
/*==============================================================*/
static void key_cmd_ck( int no, const Uint16 *tbl )
{
	SYS_SET *set = &set_work ;
	int ofs ;

	if( cmd_end ) return ;
	if( no != set->open_lv + 1 ) return ;
	if( !( sw_work[0].on & ( KEY_START | KEY_LEFT | KEY_RIGHT | KEY_BUTTON ) ) ) return ;
	ofs = !sys_flag->Country ? 0 : 6 ;
	if( sw_work[0].on != *( tbl + cmd_cnt + ofs ) ){
		cmd_end = -1 ;
		return ;
	}
	cmd_cnt ++ ;
	if( cmd_cnt != 6 ) return ;
	set->open_lv ++ ;
	cmd_end = 1 ;
}

/*==============================================================*/
/*			The game configuration								*/
/*==============================================================*/
static void game_config( void )
{
	SYS_SET *set = &set_work ;

	if( sw_work[0].on & KEY_UP ){
		Sys->timer2 -- ;
		if( Sys->timer2 < 0 ) Sys->timer2 = 13 ;
	}
	if( sw_work[0].on & KEY_DOWN ){
		Sys->timer2 ++ ;
		if( Sys->timer2 > 13 ) Sys->timer2 = 0 ;
	}
	if( test_sw.on & TEST_SW_SERVICE ){
		Sys->timer2 ++ ;
		if( Sys->timer2 > 13 ) Sys->timer2 = 0 ;
	}
	if( sys_set_ck( set ) ){
		menu_mes[3].col = COL_RED ;
		config_mes[13].col = COL_RED ;
		menu_mes[3].col = COL_RED ;
	} else {
		menu_mes[3].col = COL_WHITE ;
		config_mes[13].col = COL_WHITE ;
		menu_mes[3].col = COL_WHITE ;
	}
	if( sys_flag->Country ){
		config_mes[8].col = 0 ;
		config_mes[9].col = 0 ;
		if( Sys->timer2 == 7 ){
			if( sw_work[0].on & KEY_UP ) Sys->timer2 = 6 ;
			if( ( sw_work[0].on & KEY_DOWN ) || ( test_sw.on & TEST_SW_SERVICE ) ) Sys->timer2 = 9 ;
		}
		if( Sys->timer2 == 8 ){
			if( sw_work[0].on & KEY_UP ) Sys->timer2 = 6 ;
			if( ( sw_work[0].on & KEY_DOWN ) || ( test_sw.on & TEST_SW_SERVICE ) ) Sys->timer2 = 9 ;
		}
	}
	if( set_work.event_flag ){
		if( Sys->timer2 == 2 ){
			if( sw_work[0].on & KEY_UP ) Sys->timer2 = 1 ;
			if( ( sw_work[0].on & KEY_DOWN ) || ( test_sw.on & TEST_SW_SERVICE ) ) Sys->timer2 = 3 ;
		}
	}
	if( sys_flag->Country != NLA_COUNTRY_KOREA ){
		config_mes[4].col = 0 ;
		if( Sys->timer2 == 3 ){
			if( sw_work[0].on & KEY_UP ){
				if( set_work.event_flag ) Sys->timer2 = 1 ;
				else Sys->timer2 = 2 ;
			}
			if( ( sw_work[0].on & KEY_DOWN ) || ( test_sw.on & TEST_SW_SERVICE ) ) Sys->timer2 = 4 ;
		}
	}
	if( save_exit() ) return ;
	sound_mode_set() ;
	demo_sound_set() ;
	continue_set() ;
	if( sys_flag->Country == NLA_COUNTRY_KOREA ) voice_type_set() ;
	difficulty_set() ;
	damage_level_set() ;
	timer_speed_set() ;
	if( !sys_flag->Country ){
		vm_kit_set() ;
		vm_write_set() ;
	}
	game_speed_set() ;
	event_set() ;
	join_in_set() ;
	default_set() ;
	mes_disp( config_mes ) ;
	cursor_disp( config_mes + 1 + Sys->timer2 ) ;
}

/*==============================================================*/
/*			A level as a row of stars with its number in		*/
/*			its place											*/
/*==============================================================*/
static void level_disp( int lv, int now, int max )
{
	int i ;

	i = 0 ;
	while( i ++ != lv ) nlPrintf( "*" ) ;
	if( now != lv ) nlPrintColor( COL_RED ) ;
	else nlPrintColor( COL_WHITE ) ;
	nlPrintf( "%d", lv + 1 ) ;
	nlPrintColor( COL_WHITE ) ;
	while( i ++ <= max ) nlPrintf( "*" ) ;
}

/*==============================================================*/
/*			EXIT: back to the boot ROM's test menu				*/
/*==============================================================*/
static void test_exit( void )
{
	volatile int i ;

	nlSetFontSize( 16, 16 ) ;
	nlLocate( 8, 10 ) ;
	nlPrintf( "LOADING TEST MODE NOW...\n" ) ;
	nlWaitVSync() ;
	nlWaitVSync() ;
	nlInitInterrupt() ;
	nlSetIntrFunction( 1, 0 ) ;
	i = 16500000 ;
	while( i -- ) ;
	nlaJumpTestMode() ;
}

/*==============================================================*/
/*			The settings in use, for the menu to change			*/
/*==============================================================*/
static void sys_set_read( SYS_SET *set )
{
	SYS_SET_COPY( set, &sys_set )
}

/*==============================================================*/
/*			And the menu's, taken into use						*/
/*==============================================================*/
static void sys_set_write( SYS_SET *set )
{
	SYS_SET_COPY( &sys_set, set )
}

#if ROM_BOARD != 2
/*==============================================================*/
/*			The ROM board test's list: each ROM's sums, once	*/
/*			it has been read, and whether they are right		*/
/*==============================================================*/
static void rom_sum_disp( void )
{
	const ROM_SUM *tbl = rom_sum_tbl ;
	int i ;

	mes_disp( rom_mes ) ;
	for( i = 0 ; i < 16 ; i ++ ){
		if( rom_ck_flag[i] ){
			nlLocate( 24, i + 6 ) ;
			nlPrintf( "%04X %04X", rom_sum_byte[i], rom_sum_word[i] ) ;
			nlLocate( 17, i + 6 ) ;
			if( rom_sum_byte[i] == tbl[i].b_sum && rom_sum_word[i] == tbl[i].w_sum ) nlPrintf( "GOOD" ) ;
			else nlPrintf( "BAD" ) ;
		}
	}
}

#define ROM_BUFF	0x0d000000	/* where a ROM is read to */

/*==============================================================*/
/*			The ROM board test: read the ROMs, 8 megabytes		*/
/*			at a time, and sum them								*/
/*==============================================================*/
static void rom_test( void )
{
	Uint8 *bp ;
	Uint16 *wp ;
	int no ;
	int cnt ;
	int adrs ;
	int n ;

	switch( Sys->scene_no ){
	case 0 :
		Sys->scene_no += 1 ;
		for( no = 0 ; no < 16 ; no ++ ) rom_ck_flag[no] = 0 ;
		mes_disp( rom_mes ) ;
		nlWaitVSync() ;
		n = 2000000 ;
		while( n -- ) ;
	case 1 :
		Sys->scene_no += 1 ;
		for( cnt = 0, no = 0, adrs = 1 ; cnt < 14 ; cnt ++ ){
			cache_inv( ( void * )ROM_BUFF, 0x800000 ) ;
			nlaReadROMDMA( adrs * 0x800000, ( char * )ROM_BUFF, 0x800000, 0 ) ;
			cache_purge( ( void * )ROM_BUFF, 0x800000 ) ;
			rom_sum_byte[no] = 0 ;
			rom_sum_word[no] = 0 ;
			bp = ( Uint8 * )ROM_BUFF ;
			wp = ( Uint16 * )ROM_BUFF ;
			for( n = 0 ; n < 0x400000 ; n ++ ){
				rom_sum_byte[no] += *bp ++ ;
				rom_sum_byte[no] += *bp ++ ;
				rom_sum_word[no] += *wp ++ ;
			}
			rom_ck_flag[no] = 1 ;
			adrs ++ ;
			no ++ ;
			rom_sum_disp() ;
			nlWaitVSync() ;
			n = 2000000 ;
			while( n -- ) ;
		}
		adrs ++ ;
		cache_inv( ( void * )ROM_BUFF, 0x800000 ) ;
		nlaReadROMDMA( 0x08000000, ( char * )ROM_BUFF, 0x800000, 0 ) ;
		cache_purge( ( void * )ROM_BUFF, 0x800000 ) ;
		rom_sum_byte[no] = 0 ;
		rom_sum_byte[no + 1] = 0 ;
		rom_sum_word[no] = 0 ;
		rom_sum_word[no + 1] = 0 ;
		bp = ( Uint8 * )ROM_BUFF ;
		wp = ( Uint16 * )ROM_BUFF ;
		for( n = 0 ; n < 0x200000 ; n ++ ){
			rom_sum_byte[no] += *bp ++ ;
			rom_sum_byte[no] += *bp ++ ;
			rom_sum_byte[no + 1] += *bp ++ ;
			rom_sum_byte[no + 1] += *bp ++ ;
			rom_sum_word[no] += *wp ++ ;
			rom_sum_word[no + 1] += *wp ++ ;
		}
		rom_ck_flag[no] = 1 ;
		rom_ck_flag[no + 1] = 1 ;
		rom_sum_disp() ;
		nlWaitVSync() ;
		n = 2000000 ;
		while( n -- ) ;
		break ;
	case 2 :
		rom_sum_disp() ;
		exit_ck() ;
		break ;
	}
}
#endif

TEST_MES menu_mes[] = {
	{ 13, 2, COL_WHITE, "GAME TEST MODE" },
	{ 15, 6, COL_WHITE, "INPUT TEST" },
	{ 15, 8, COL_WHITE, "GAME DATA" },
	{ 15, 10, COL_WHITE, "GAME CONFIGURATION" },
#if ROM_BOARD == 2
	{ 15, 12, COL_WHITE, "EXIT" },
#else
	{ 15, 12, COL_WHITE, "ROM BOARD TEST" },
	{ 15, 14, COL_WHITE, "EXIT" },
#endif
	{ -1 },
} ;

TEST_MES input_mes[] = {
	{ 10, 2, COL_WHITE, "I N P U T   T E S T" },
	{ -1 },
} ;

TEST_MES data_mes[] = {
	{ 11, 2, COL_WHITE, "G A M E   D A T A" },
	{ 11, 4, COL_WHITE, "- ACCUMULATAED DATA -" },
	{ 11, 6, COL_WHITE, "COIN COUNTER" },
	{ 11, 7, COL_WHITE, "SERVICE COUNTER" },
	{ 11, 8, COL_WHITE, "FREEPLAY COUNTER" },
	{ 11, 11, COL_WHITE, "- RESETABLE DATA -" },
	{ 11, 13, COL_WHITE, "COIN COUNTER" },
	{ 11, 14, COL_WHITE, "SERVICE COUNTER" },
	{ 11, 15, COL_WHITE, "FREEPLAY COUNTER" },
	{ 10, 17, COL_WHITE, "RESETABLE DATA CLEAR" },
	{ 19, 19, COL_WHITE, "YES" },
	{ 19, 20, COL_WHITE, "NO" },
	{ 8, 23, COL_WHITE, "SELECT WITH 1P UP or DOWN" },
	{ 8, 24, COL_WHITE, "     AND PRESS SHOT1" },
	{ 8, 26, COL_WHITE, "EXIT  = 1P START & SHOT1" },
	{ -1 },
} ;

TEST_MES config_mes[] = {
	{ 11, 2, COL_WHITE, "GAME CONFIGRATION" },
	{ 4, 5, COL_WHITE, "SOUND MODE" },
	{ 4, 6, COL_WHITE, "DEMO SOUND HALF" },
	{ 4, 7, COL_WHITE, "CONTINUE" },
	{ 4, 8, COL_WHITE, "VOICE TYPE" },
	{ 4, 9, COL_WHITE, "DIFFICULTY" },
	{ 4, 10, COL_WHITE, "DAMAGE LEVEL" },
	{ 4, 11, COL_WHITE, "TIMER SPEED" },
	{ 4, 12, COL_WHITE, "VM-KIT TYPE" },
	{ 4, 13, COL_WHITE, "VM WRITE" },
	{ 4, 14, COL_WHITE, "GAME SPEED" },
	{ 4, 15, COL_WHITE, "EVENT" },
	{ 4, 16, COL_WHITE, "JOIN-IN" },
	{ 4, 18, COL_WHITE, "DEFAULT" },
	{ 4, 19, COL_WHITE, "SAVE & EXIT" },
	{ -1 },
} ;

/* the keys' lines: of a setting, of SAVE & EXIT, of DEFAULT, of the menu's tests and of its EXIT */
TEST_MES help_mes[] = {
	{ 4, 25, COL_WHITE, "SELECT OPTION  = 1P UP or DOWN" },
	{ 4, 26, COL_WHITE, "MODIFY SETTING = 1P LEFT or RIGHT" },
	{ 4, 27, COL_WHITE, "               = 1P SHOT1 or SHOT2" },
	{ -1 },
	{ 4, 25, COL_WHITE, "SELECT OPTION  = 1P UP or DOWN" },
	{ 4, 26, COL_WHITE, "RETURN TO GAME TEST MODE" },
	{ 4, 27, COL_WHITE, "               = 1P SHOT1" },
	{ -1 },
	{ 4, 25, COL_WHITE, "SELECT OPTION  = 1P UP or DOWN" },
	{ 4, 26, COL_WHITE, "RESET          = 1P SHOT1 & SHOT2" },
	{ -1 },
	{ 4, 25, COL_WHITE, "SELECT OPTION  = 1P UP or DOWN" },
	{ 4, 26, COL_WHITE, "START          = 1P SHOT1" },
	{ -1 },
	{ 4, 25, COL_WHITE, "SELECT OPTION  = 1P UP or DOWN" },
	{ 4, 26, COL_WHITE, "RETURN TO SYSTEM MODE" },
	{ 4, 27, COL_WHITE, "               = SHOT1" },
	{ -1 },
} ;

#if ROM_BOARD != 2
TEST_MES rom_mes[] = {
	{ 7, 2, COL_WHITE, "R O M  B O R A D  T E S T" },
	{ 7, 5, COL_WHITE, "NO.  TYPE RESULT BYTE WORD" },
	{ 7, 6, COL_WHITE, "IC17 64M" },
	{ 7, 7, COL_WHITE, "IC18 64M" },
	{ 7, 8, COL_WHITE, "IC19 64M" },
	{ 7, 9, COL_WHITE, "IC20 64M" },
	{ 7, 10, COL_WHITE, "IC21 64M" },
	{ 7, 11, COL_WHITE, "IC22 64M" },
	{ 7, 12, COL_WHITE, "IC23 64M" },
	{ 7, 13, COL_WHITE, "IC24 64M" },
	{ 7, 14, COL_WHITE, "IC25 64M" },
	{ 7, 15, COL_WHITE, "IC26 64M" },
	{ 7, 16, COL_WHITE, "IC27 64M" },
	{ 7, 17, COL_WHITE, "IC28 64M" },
	{ 7, 18, COL_WHITE, "IC29 64M" },
	{ 7, 19, COL_WHITE, "IC30 64M" },
	{ 7, 20, COL_WHITE, "IC31 64M" },
	{ 7, 21, COL_WHITE, "IC32 64M" },
	{ -1 },
} ;
#endif
