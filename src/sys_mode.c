/* Task 1, the attract: the copyright, the title, the demos and the coin messages, until a player
   starts (task 0 is task_init.c, task 2 game.c). */
#include "player.h"
#include "naomi.h"

static Sint8 coin_blink ;	/* the coin messages blink: 1 shown */
static Sint8 coin_timer ;	/* and the frames to the next blink */

extern unsigned long strlen( const char *s ) ;
extern void entry_init( PLAYER_WORK *pl_w ) ;
extern void total_vol_set( int vol ) ;
extern void total_vol_init( void ) ;
extern void ex_sound_load( void ) ;
extern void ex_se_req( Uint8 no ) ;
extern Sint8 credit_enough_ck( int side, int cont ) ;
extern void demo_init( void ) ;
extern void attract_opening_main( void ) ;
extern void at03_ef4_set( void ) ;
extern void at03_ef1_set( void ) ;
extern void at03_ef5_set( void ) ;
extern void disp_front_info( void ) ;
extern void at03_ef2_set( int no ) ;
extern void at03_ef7_set( void ) ;
extern void at03_ef6_set( void ) ;
extern void at03_ef0_set( int no ) ;
extern void at03_ef3_set( int no ) ;
extern void demo_main( void ) ;
extern void fight_load_start( int no ) ;
extern void disp_stage_obj( void ) ;
extern void demo_pl_set( void ) ;
extern void at01_ef_set_all( void ) ;
extern void sys_mode_ef0_set( void ) ;
extern void at03_ef8_set( void ) ;
extern void load_init( void ) ;

static void attract_init( void ) ;
static void attract_logo( void ) ;
static void attract_opening( void ) ;
static void attract_title( void ) ;
static void attract_copyright( void ) ;
static void attract_demo( void ) ;
static void attract_ranking( void ) ;
static void attract_coin( void ) ;
static void attract_08( void ) ;
static void attract_09( void ) ;
static void attract_10( void ) ;
static void attract_copyright_init( void ) ;
static void attract_copyright_move( void ) ;
static void attract_title_init( void ) ;
static void attract_title_move( void ) ;
static void attract_demo_load( void ) ;
static void attract_scr_set( void ) ;
static void attract_logo_init( void ) ;
static void attract_logo_move( void ) ;
static void attract_coin_init( void ) ;
static void attract_coin_move( void ) ;
static void start_ck( void ) ;
static void credit_disp( void ) ;
static void coin_disp( void ) ;

/* the light's direction for the attract's scenes */
static VEC3 light_vec[] = {
	{ 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f },
} ;

/****************************************************************/
/*			Task 1: the attract, until a coin goes in or, in	*/
/*			free play, a player starts							*/
/****************************************************************/
void task_attract( void )
{
	static void ( *attract_jmp[] )( void ) = {
		attract_init,
		attract_logo,
		attract_opening,
		attract_title,
		attract_copyright,
		attract_demo,
		attract_ranking,
		attract_coin,
		attract_08,
		attract_09,
		attract_10,
	} ;

	Sys->cpu_level_max = 0 ;
	attract_jmp[Sys->task_step]() ;
	nlRand() ;
	Rnd() ;
	test_sw_ck() ;
	if( Sys->play_mode != 64 ) return ;
	if( credit_conf->CoinFlag != NLA_FREEPLAY ){
		if( ( credit_info.Player[0].Credit != 0 ) | ( credit_info.Player[0].Remain != 0 )
		 | ( credit_info.Player[1].Credit != 0 ) | ( credit_info.Player[1].Remain != 0 ) ){
			if( Sys->task_step != 7 ){
				Sys->task_step = 7 ;
				Sys->scene_no = 0 ;
				stream_stop() ;
				sound_all_off() ;
				se_all_off() ;
				total_vol_set( 127 ) ;
				se_vol_up( 0, 1 ) ;
				se_vol_up( 1, 1 ) ;
				s3l_fade_in_set( 1 ) ;
			}
		}
		return ;
	}
	start_ck() ;
	if( ( sw_work[0].on & KEY_START ) || ( sw_work[1].on & KEY_START ) ){
		stream_stop() ;
		sound_all_off() ;
		se_all_off() ;
		total_vol_set( 127 ) ;
		se_vol_up( 0, 1 ) ;
		se_vol_up( 1, 1 ) ;
		s3l_fade_in_set( 1 ) ;
		work_init() ;
		back_color_set( 0 ) ;
	}
}

/*==============================================================*/
/*			The attract's set-up								*/
/*==============================================================*/
static void attract_init( void )
{
	Sys->task_step ++ ;
	Sys->play_mode = 64 ;
	Sys->task_stop = 0 ;
	Sys->join_flag = 0 ;
	if( Sys->attract_step ){
		Sys->task_step = Sys->attract_step ;
		Sys->attract_step = 0 ;
	}
	coin_blink = 1 ;
	coin_timer = 20 ;
	nlaChangeCreditSequence( 0, 0 ) ;
	nlaChangeCreditSequence( 1, 0 ) ;
	Palette_Init() ;
	Texture_Init() ;
	memset( &sys_work.game_timer, 0, SYS_HALF ) ;
	memset( &vs_sys_work, 0, sizeof( vs_sys_work ) ) ;
	work_init() ;
	total_vol_init() ;
	rank_name_chk() ;
}

/*==============================================================*/
/*			The copyright										*/
/*==============================================================*/
static void attract_copyright( void )
{
	static void ( *attract_copyright_jmp[] )( void ) = {
		attract_copyright_init,
		attract_copyright_move,
	} ;

	attract_copyright_jmp[Sys->scene_no]() ;
}

/*==============================================================*/
static void attract_copyright_init( void )
{
	Sys->scene_no ++ ;
	Sys->timer0 = 180 ;
	Palette_Init() ;
	Texture_Init() ;
	work_init() ;
	scr_work_init() ;
	bright_init() ;
	fog_init() ;
	light_dir_set( &light_vec[0] ) ;
	light_amb_set( 1.0f ) ;
	back_color_set( 0 ) ;
	fade_set( -1, 20, FADE_IN ) ;
}

/* the copyright notices: the first for cpu_level_type 0, the second for 1 */
const MES_WORK copyright_mes_0[] = {
	{ 0x10, 0x1c, 0x12, "MARVEL: ^E & ^Z 2000 MARVEL CHARACTERS, INC." },
	{ 0x10, 0x24, 0x12, "ALL RIGHTS RESERVED." },
	{ 0x10, 0x2c, 0x12, "^Z CAPCOM CO., LTD. 2000 ALL RIGHTS RESERVED." },
	{ 0x10, 0x34, 0x12, "STRIDER: ^Z MOTO KIKAKU.  ^Z CAPCOM CO., LTD." },
	{ 0x10, 0x3c, 0x12, "2000 ALL RIGHTS RESERVED." },
	{ 0x10, 0x44, 0x12, "This video game has been produced" },
	{ 0x10, 0x4c, 0x12, "under the license from Marvel Characters," },
	{ 0x10, 0x54, 0x12, "Inc." },
	{ 0xff, 0x00, 0x00, "" },
} ;

const MES_WORK copyright_mes_1[] = {
	{ 0x10, 0x1c, 0x12, "MARVEL: ^E & ^Z 2000 MARVEL CHARACTERS, INC." },
	{ 0x10, 0x24, 0x12, " ALL RIGHTS RESERVED." },
	{ 0x10, 0x2c, 0x12, "^Z CAPCOM CO., LTD. 2000," },
	{ 0x10, 0x34, 0x12, "^Z CAPCOM U.S.A., INC. 2000 ALL RIGHTS RESERVED." },
	{ 0x10, 0x3c, 0x12, "STRIDER: ^Z MOTO KIKAKU. ^Z CAPCOM CO., LTD. 2000," },
	{ 0x10, 0x44, 0x12, "^Z CAPCOM U.S.A., INC. 2000 ALL RIGHTS RESERVED." },
	{ 0x10, 0x4c, 0x12, "This video game has been produced" },
	{ 0x10, 0x54, 0x12, "under the license from Marvel Characters," },
	{ 0x10, 0x5c, 0x12, "Inc." },
	{ 0xff, 0x00, 0x00, "" },
} ;

/*==============================================================*/
static void attract_copyright_move( void )
{
	const MES_WORK *mes ;

	Sys->timer0 -- ;
	if( !Sys->timer0 ){
		if( load_ck() ) Sys->timer0 ++ ;
		else {
			Sys->task_step ++ ;
			Sys->scene_no = 0 ;
		}
	} else {
		if( Sys->cpu_level_type == 1 ) mes = copyright_mes_1 ;
		else mes = copyright_mes_0 ;
		while( mes->x < 100 ){
			mes_font_disp( mes ) ;
			mes ++ ;
		}
	}
	if( Sys->timer0 == 155 ) demo_init() ;
}

/*==============================================================*/
static void attract_opening( void )
{
	attract_opening_main() ;
}

/*==============================================================*/
/*			The title											*/
/*==============================================================*/
static void attract_title( void )
{
	static void ( *attract_title_jmp[] )( void ) = {
		attract_title_init,
		attract_title_move,
	} ;

	attract_title_jmp[Sys->scene_no]() ;
}

/*==============================================================*/
static void attract_title_init( void )
{
	Sys->scene_no ++ ;
	Sys->timer0 = 840 ;
	Sys->timer1 = -90 ;
	Palette_Init() ;
	Texture_Init() ;
	total_vol_init() ;
	TexLoad( 11 ) ;
	work_init() ;
	scr_work_init() ;
	bright_init() ;
	fade_init() ;
	back_color_set( 0 ) ;
	light_dir_set( &light_vec[0] ) ;
	light_amb_set( 1.0f ) ;
	fog_init() ;
	fog_work.on = 1 ;
	fog_work.len = 91 ;
	fog_work.density = 1780.0f ;
	fog_work.r = 19 ;
	fog_work.g = 24 ;
	fog_work.b = 91 ;
	at03_ef4_set() ;
	at03_ef1_set() ;
	at03_ef5_set() ;
	fog_set() ;
	fade_set( -1, 60, FADE_IN ) ;
	attract_demo_load() ;
	SysTransSet( disp_front_info ) ;
}

/*==============================================================*/
static void attract_title_move( void )
{
	Sys->timer0 -- ;
	if( !Sys->timer0 ){
		Sys->task_step ++ ;
		Sys->scene_no = 0 ;
	} else {
		coin_disp() ;
		credit_disp() ;
	}
	Sys->timer1 ++ ;
	switch( Sys->timer1 ){
		case 0xf0 : at03_ef2_set( 0 ) ; break ;
		case 0xfa : at03_ef2_set( 1 ) ; break ;
		case 0x104 : at03_ef2_set( 2 ) ; break ;
		case 0x10e : at03_ef2_set( 3 ) ; break ;
		case 0x118 : at03_ef2_set( 4 ) ; break ;
		case 0x122 : at03_ef2_set( 5 ) ; break ;
		case 0xbc : at03_ef2_set( 6 ) ; break ;
		case 0xc6 : at03_ef2_set( 7 ) ; break ;
		case 0xd0 : at03_ef2_set( 8 ) ; break ;
		case 0xda : at03_ef2_set( 9 ) ; break ;
		case 0xe4 : at03_ef2_set( 10 ) ; break ;
		case 0xee : at03_ef2_set( 11 ) ; break ;
	}
	if( Sys->timer1 == 0x177 ) at03_ef7_set() ;
	if( Sys->timer1 == 0x17c ) at03_ef6_set() ;
	if( Sys->timer1 == 0x15e ){
		at03_ef0_set( 0 ) ;
		at03_ef0_set( 1 ) ;
		at03_ef0_set( 2 ) ;
	}
	switch( Sys->timer1 ){
		case 0x19a : at03_ef3_set( 0 ) ; break ;
		case 0x1a4 : at03_ef3_set( 1 ) ; break ;
		case 0x1c2 : at03_ef3_set( 2 ) ; break ;
		case 0x1cc : at03_ef3_set( 3 ) ; break ;
		case 0x1ea : at03_ef3_set( 4 ) ; break ;
		case 0x1f4 : at03_ef3_set( 5 ) ; break ;
	}
	if( Sys->timer1 == 0x190 ){
		at03_ef0_set( 3 ) ;
		at03_ef0_set( 4 ) ;
	}
	if( Sys->timer1 == 0x15e ) ex_se_req( 2 ) ;
	if( Sys->timer1 < 0x140 ) fog_work.density += 4.625f ;
	work_list_move( W_SYS ) ;
	fog_set() ;
	if( Sys->timer1 == 0x15e ) fade_set( -1, 20, FADE_IN ) ;
	if( Sys->timer1 == 840 ){
		stream_stop() ;
		fade_set( -1, 60, FADE_OUT ) ;
	}
	SysTransSet( disp_front_info ) ;
}

/*==============================================================*/
static void attract_demo( void )
{
	demo_main() ;
	coin_disp() ;
	credit_disp() ;
}

/*==============================================================*/
static void attract_demo_load( void )
{
	fight_load_start( 0 ) ;
}

/*==============================================================*/
static void attract_ranking( void )
{
	RankDemoMain() ;
}

/*==============================================================*/
/*			The logo											*/
/*==============================================================*/
static void attract_logo( void )
{
	static void ( *attract_logo_jmp[] )( void ) = {
		attract_logo_init,
		attract_logo_move,
	} ;

	attract_logo_jmp[Sys->scene_no]() ;
	work_list_move( W_SET ) ;
	scr_move() ;
	fog_set() ;
	SysTransSet( disp_stage_obj ) ;
}

/*==============================================================*/
static void attract_scr_set( void )
{
	scr_work.scr_type = 1 ;
	scr_work.scr_routine[0] = 0 ;
	scr_work.scr_routine[1] = 0 ;
}

/*==============================================================*/
static void attract_logo_init( void )
{
	Sys->scene_no ++ ;
	Sys->timer0 = 480 ;
	Palette_Init() ;
	Texture_Init() ;
	TexLoad( 13 ) ;
	work_init() ;
	scr_work_init() ;
	attract_scr_set() ;
	bright_init() ;
	fog_init() ;
	fade_init() ;
	back_color_set( 0 ) ;
	light_dir_set( &light_vec[1] ) ;
	light_amb_set( 0.3f ) ;
	fade_set( -1, 60, FADE_IN ) ;
	at01_ef_set_all() ;
	stream_req( 13 ) ;
	s3l_fade_in_set( 1 ) ;
	stream_start() ;
	demo_pl_set() ;
}

/*==============================================================*/
static void attract_logo_move( void )
{
	Sys->timer0 -- ;
	if( !Sys->timer0 ){
		Sys->task_step ++ ;
		Sys->scene_no = 0 ;
		stream_stop() ;
	} else {	/* empty */
	}
	if( Sys->timer0 == 0x14f ) fade_set( -1, 60, FADE_OUT ) ;
	if( Sys->timer0 == 0xeb ){
		fade_set( -1, 60, FADE_IN ) ;
		sys_mode_ef0_set() ;
	}
}

/*==============================================================*/
/*			The demo fight										*/
/*==============================================================*/
static void attract_coin( void )
{
	static void ( *attract_coin_jmp[] )( void ) = {
		attract_coin_init,
		attract_coin_move,
	} ;

	coin_disp() ;
	credit_disp() ;
	attract_coin_jmp[Sys->scene_no]() ;
	fog_set() ;
	SysTransSet( disp_front_info ) ;
	start_ck() ;
}

/*==============================================================*/
static void attract_coin_init( void )
{
	Sys->scene_no ++ ;
	memset( &sys_work.game_timer, 0, SYS_HALF ) ;
	memset( &vs_sys_work, 0, sizeof( vs_sys_work ) ) ;
	Palette_Init() ;
	Texture_Init() ;
	ex_sound_load() ;
	TexLoad( 11 ) ;
	work_init() ;
	bright_init() ;
	scr_work_init() ;
	fade_init() ;
	back_color_set( 0 ) ;
	light_dir_set( &light_vec[0] ) ;
	light_amb_set( 1.0f ) ;
	fog_init() ;
	fog_work.on = 1 ;
	fog_work.len = 91 ;
	fog_work.density = 1780.0f ;
	fog_work.r = 19 ;
	fog_work.g = 24 ;
	fog_work.b = 91 ;
	at03_ef8_set() ;
	at03_ef1_set() ;
	at03_ef5_set() ;
	at03_ef7_set() ;
	at03_ef0_set( 0 ) ;
	at03_ef0_set( 1 ) ;
	at03_ef0_set( 2 ) ;
	at03_ef0_set( 3 ) ;
	at03_ef0_set( 4 ) ;
	fade_set( 0, 60, FADE_IN ) ;
	stream_stop() ;
	ex_se_req( 1 ) ;
	load_init() ;
}

/*==============================================================*/
static void attract_coin_move( void )
{
	work_list_move( W_SYS ) ;
}

/*==============================================================*/
static void attract_08( void )
{
	Sys->task_step ++ ;
}

/*==============================================================*/
static void attract_10( void )
{
	Sys->task_step = 0 ;
}

/*==============================================================*/
static void attract_09( void )
{
	Sys->task_step = 0 ;
	Sys->scene_no = 0 ;
}

/*==============================================================*/
/*			A player pressed start with credits: set up the		*/
/*			sides and start the game							*/
/*==============================================================*/
static void start_ck( void )
{
	PLAYER_WORK *pl_w ;
	Sint8 side = 0 ;
	Sint8 i ;

	if( sw_work[0].on & KEY_START ) side += credit_enough_ck( 0, 0 ) ;
	if( side && Sys->no_challenge_flag ) goto skip ;
	if( sw_work[1].on & KEY_START ) side += credit_enough_ck( 1, 0 ) ;
skip:
	if( !side ) return ;
	else {
		if( Sys->event_flag ) side = 3 ;
		Sys->entry_side = side ;
		Sys->new_side = side ;
		Sys->play_mode = 96 ;
		for( i = 0 ; i < 6 ; i ++ ){
			pl_w = &pl_work[i] ;
			memset( pl_w, 0, sizeof( PLAYER_WORK ) ) ;
			pl_w->pl_index = i ;
			entry_init( pl_w ) ;
		}
		if( Sys->entry_side & 1 ) pl_work[0].entry_routine_1 = ENTRY_PLAY ;
		if( Sys->entry_side & 2 ) pl_work[1].entry_routine_1 = ENTRY_PLAY ;
		pl_work[0].side_bit = 1 ;
		pl_work[1].side_bit = 2 ;
		Sys->task_no = 2 ;
		Sys->task_step = 0 ;
		Sys->scene_no = 0 ;
		Sys->routine_0 = 0 ;
		Sys->routine_1 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->timer0 = 0 ;
		Sys->timer1 = 0 ;
		Sys->timer2 = 0 ;
		Sys->timer3 = 0 ;
		stream_stop() ;
	}
}

/*==============================================================*/
/*			The credits line									*/
/*==============================================================*/
static void credit_disp( void )
{
	if( credit_conf->CoinFlag != NLA_FREEPLAY ){
		STrPrint( 4, 108, 11, "%s", credit_str1 ) ;
		if( credit_conf->ChuteType ){
			STrPrint( ( 70 - strlen( credit_str2 ) ) * 2, 108, 11, "%s", credit_str2 ) ;
		}
	}
}

/*==============================================================*/
/*			The coin messages of each side, blinking			*/
/*==============================================================*/
static void coin_disp( void )
{
	coin_timer -- ;
	if( coin_timer == 0 ){
		coin_blink = 1 - coin_blink ;
		if( coin_blink == 0 ) coin_timer = 15 ;
		else coin_timer = 20 ;
	}
	if( credit_conf->CoinFlag == NLA_FREEPLAY ){
		if( coin_blink == 1 ) STrPrint( 64, 98, 11, "FREE PLAY" ) ;
		return ;
	} else if( !credit_conf->ChuteType ){
		if( credit_info.Player[0].Status == NLA_CREDIT_ENOUGH ){
			if( coin_blink == 1 ) STrPrint( 52, 98, 11, "PRESS START BUTTON" ) ;
		} else if( credit_info.Player[0].Status == NLA_CREDIT_MORE ){
			if( coin_blink == 1 ) STrPrint( 52, 98, 11, "INSERT MORE COIN(S)" ) ;
			else if( Sys->vms_mode != 0 ) STrPrint( 70, 98, 22, "^N" ) ;
			if( credit_info.Player[0].Credit ){
				if( coin_blink == 1 ) STrPrint( 52, 102, 11, "%d CREDITS TO START", credit_info.Sequence[0] ) ;
			}
		} else {
			if( coin_blink == 1 ) STrPrint( 56, 98, 11, "INSERT COIN(S)" ) ;
			else if( Sys->vms_mode != 0 ) STrPrint( 70, 98, 22, "^N" ) ;
		}
	} else {
		if( credit_info.Player[0].Status == NLA_CREDIT_ENOUGH ){
			if( coin_blink == 1 ) STrPrint( 4, 98, 11, "PRESS START BUTTON" ) ;
		} else if( credit_info.Player[0].Status == NLA_CREDIT_MORE ){
			if( coin_blink == 1 ) STrPrint( 4, 98, 11, "INSERT MORE COIN(S)" ) ;
			else if( Sys->vms_mode != 0 ) STrPrint( 18, 98, 22, "^N" ) ;
			if( credit_info.Player[0].Credit ){
				if( coin_blink == 1 ) STrPrint( 4, 102, 11, "%d CREDITS TO START", credit_info.Sequence[0] ) ;
			}
		} else {
			if( coin_blink == 1 ) STrPrint( 4, 98, 11, "INSERT COIN(S)" ) ;
			else if( Sys->vms_mode != 0 ) STrPrint( 14, 98, 22, "^N" ) ;
		}
		if( credit_info.Player[1].Status == NLA_CREDIT_ENOUGH ){
			if( coin_blink == 1 ) STrPrint( 102, 98, 11, "PRESS START BUTTON" ) ;
		} else if( credit_info.Player[1].Status == NLA_CREDIT_MORE ){
			if( coin_blink == 1 ) STrPrint( 100, 98, 11, "INSERT MORE COIN(S)" ) ;
			else if( Sys->vms_mode != 0 ) STrPrint( 122, 98, 22, "^N" ) ;
			if( credit_info.Player[1].Credit ){
				if( coin_blink == 1 ) STrPrint( 100, 102, 11, "%d CREDITS TO START", credit_info.Sequence[0] ) ;
			}
		} else {
			if( coin_blink == 1 ) STrPrint( 114, 98, 11, "INSERT COIN(S)" ) ;
			else if( Sys->vms_mode != 0 ) STrPrint( 126, 98, 22, "^N" ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sys_mode.c" End of File	*/
