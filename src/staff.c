/* The staff roll after the ending: the credits rise up the screen a letter at a time, with
   pictures fading in and out beside them, then THANK YOU FOR PLAYING, and the game goes back to
   its demo. */
#include "player.h"

/* the credits' codes: a line of the roll ends with one */
#define STAFF_HEAD	0xfe	/* the next line is a heading */
#define STAFF_NAME	0xfd	/* the next line is a name */
#define STAFF_BIG	0xfc	/* the letters from here are double size */
#define STAFF_END	0xff	/* the end of the credits */
#define STAFF_CODE	0xf0	/* the codes start here */

/* the home game's roll has its own music file, credits the network's staff and shows two
   pictures fewer */
#ifdef TARGET_DC
#define STAFF_STREAM	20
#define STAFF_SIZE	0x5d0
#define STAFF_PIC_MAX	14
#else
#define STAFF_STREAM	12
#define STAFF_SIZE	0x588
#define STAFF_PIC_MAX	16
#endif

extern const MES_WORK copyright_mes_0[];	/* sys_mode.c: lines of text, ended by an x of 100 or more */
extern const MES_WORK copyright_mes_1[];	/* the same for cpu_level_type 1 */

extern void disp_front_spr( void );	/* the scene's display */
extern void at03_ef1_set( void );
extern void at03_ef5_set( void );

static void staff_init( void );
static void staff_move( void );
static void staff_exit( void );
static void staff_fade_in( void );
static void staff_back_set( void );
static void staff_back_in( void );
static void staff_roll( void );
static void staff_roll_out( void );
static void staff_copyright( void );
static void staff_thanks( void );
static void staff_move_7( void );
static void staff_skip( void );
static void staff_chr_set( Uint8 c, Float x, Float spd );
static void staff_chr_move( PLAYER_WORK *ew );
static void staff_pic_set( Sint32 tile, Uint8 type );
static void staff_pic_move( PLAYER_WORK *ew );
static void pic_stay( PLAYER_WORK *ew );
static void pic_stay_init( PLAYER_WORK *ew );
static void pic_stay_in( PLAYER_WORK *ew );
static void pic_stay_wait( PLAYER_WORK *ew );
static void pic_stay_out( PLAYER_WORK *ew );
static void pic_shake( PLAYER_WORK *ew );
static void pic_shake_init( PLAYER_WORK *ew );
static void pic_shake_in( PLAYER_WORK *ew );
static void pic_shake_wait( PLAYER_WORK *ew );
static void pic_shake_out( PLAYER_WORK *ew );
static void pic_left( PLAYER_WORK *ew );
static void pic_left_init( PLAYER_WORK *ew );
static void pic_left_move( PLAYER_WORK *ew );
static void pic_right( PLAYER_WORK *ew );
static void pic_right_init( PLAYER_WORK *ew );
static void pic_right_move( PLAYER_WORK *ew );
static void pic_down( PLAYER_WORK *ew );
static void pic_down_init( PLAYER_WORK *ew );
static void pic_down_move( PLAYER_WORK *ew );
static void pic_up( PLAYER_WORK *ew );
static void pic_up_init( PLAYER_WORK *ew );
static void pic_up_move( PLAYER_WORK *ew );
static void pic_wait( PLAYER_WORK *ew );
static void pic_wait2( PLAYER_WORK *ew );
static void pic_in( PLAYER_WORK *ew );
static void pic_out( PLAYER_WORK *ew );
static void logo_set( void );
static void logo_move( PLAYER_WORK *ew );
static void logo_flash_set( void );
static void logo_flash_move( PLAYER_WORK *ew );
void logo3_set( void );
static void logo3_move( PLAYER_WORK *ew );

/* the credits: lines of letters, each ended by a code */
static const Uint8 staff_data[STAFF_SIZE] =
	"STAFF CREDIT\376\376\376"
	"PLANNER\375"
	" TATSUYA NAKAE\375"
	" SPP IORYA\375"
	" BURUMA\375"
	" ONI-SUZUKI(M.P)\375\375"
	" OBATA SHINICHIRO\375"
	" NEO-G(ISHIZAWA)\375\376\376"
	"PROGRAMMER\375"
	" MOTSU\375"
	" TERUAKI HIROKADO\375"
	" KAW.TLD\375"
	" SILVER KADONTZ\375"
	" YOU!\375"
	" MINOMIYA\375"
	" YUKO KAWAMURA\375"
	" AKIHIRO YOKOYAMA\375"
	" HYPER-SHINCHAN-\375"
	" NISHI\375"
	" REIKO TOH\375"
	" ITTETSU\375"
	" DICE-K\375"
	" SOJI SETA\375"
	" CHAM\375"
	" HISASHI KURAMOTO\375"
	" KOMUU\375"
#ifndef TARGET_DC
	" SHIN\375"
#endif
	" MIKKY\375\376\376"
	"TITLE DESIGN\375"
	" SHOEI\376\376\376"
	"ILLUSTRATIONS\375"
	" MF\376\376\376"
	"INSTRUCTION\376"
	"      CARD DESIGN\375"
#ifdef TARGET_DC
	" SAKOMIZU\375"
	" Y.UCHIDA\376\376\376"
#else
	" SAKOMIZU\376\376\376"
#endif
	"OBJECT DESIGN\375"
	" MAKOTO ISHII\375"
	" AKEMI KURIHARA\375"
	" F\375"
	" MIZUPYON\375"
	" MIZUHO\375"
	" IGARASHI\375"
	" NISHIMURA MASARU\375"
	" HIDEYA TAKADA\375"
	" RIN BOKU\375"
	" TAGIRIN\375"
	" MASATSUGU SATO\375"
	" HIRANO DAICHI\375"
	" IMAMURA\375"
	" HIROKI SHIBAKI\375"
	" TOMOHIKO OHSUMI\375"
	" YANO\375"
	" AKIZUKI MICHIRU\375"
	" IZUMI NODA\375"
	" MINOBE HIROAKI\375"
	" KIMO KIMO\375"
	" BALL BOY\375"
	" A IWASAKI\375"
	" MIWA SAKAGUCHI\375"
	" Y YAMAMOTO\375"
	" KAERU\375"
	" HIRO\375"
	" YOSHIHIKO AKITA\375\376\376"
	"EFFECT DESIGN\375"
	" SAGATA\375"
	" TAKEP\375"
	" R\375\376\376"
	"SCROLL DESIGN\375"
	" SAWATCH\375"
	" TAKASHI FUJIWARA\375"
	" YUKI KYOUTANI\375"
	" YOKO FUKUMOTO\375"
	" YUSUKE SAIWAI\375"
	" MAKOTO TANAKA\375"
	" CHIKA IWAI\375"
	" KENJI KUSHIRO\375\376\376"
	"MUSIC COMPOSE\375"
	" TETSUYA SHIBATA\375"
	" MITSUHIKO TAKANO\375\376\376"
	"SOUND DESIGN\375"
	" SATOSHI ISE\375"
	" MASAYUKI ENDO\375\376\376"
	"VOICE ACTOR\375"
	" SECRET\375\376\376"
	"RECORDING ENGINEER\375"
	" KAZUYA TAKIMOTO\376\376\376"
	"VOICE DIRECTOR\375"
	" ERIK(ESX)SUZUKI\376\376\376"
#ifdef TARGET_DC
	"NETWORK\375"
	" SHIN\375"
	" NET MAN\375"
	" ICHIRO YUNDE\375"
	" MINEYUKI NODA\375"
	" HIDEAKI ITSUNO\376\376\376"
#endif
	"SPECIAL THANKS\375"
	" SHIRAIWA TAKUYA\375"
	" TAKANO MIKI\375"
	" STEVE LEE\375\375"
	" TEN LITTLE KOBUN\375\375"
	" BAS(M.P)\375"
	" DAISUKE AMEMIYA\375"
	" H.MIYASHITA\375"
	" OZZY\375"
	" TOMOYA MAEKAWA\375\376\376"
	"PRODUCER\375"
#ifdef TARGET_DC
	" YOSHIHIRO SUDO\375\376\376"
	"GENERAL PRODUCER\375"
	" NORITAKA FUNAMIZU\376\376\376"
	"EXECUTIVE PRODUCER\375"
	" YOSHIKI OKAMOTO\376\376\376\376\376\375"
#else
	" YOSHIHIRO SUDO\375\376\376\376"
	"GENERAL PRODUCER\375"
	" NORITAKA FUNAMIZU\376\376\376\376"
	"EXECUTIVE PRODUCER\375"
	" YOSHIKI OKAMOTO\376\376\376\376\376\376\375"
#endif
	"ALL CAPCOM STAFF\375\375"
	"AND YOU\376\376\376\376\376\376\376\376\376\376\376\376\376\376\376\374"
	"   FIN\376\376\376\376\376\376\376\376\376\377";

/* the pictures: seconds until the next, a flag for the background, the picture and its kind */
static Uint8 staff_pic_tbl[( STAFF_PIC_MAX + 1 ) * 4] = {
	10, 0, 0, 1,
	6, 1, 1, 2,
	6, 2, 0, 4,
	6, 3, 1, 5,
	6, 4, 0, 2,
	6, 5, 1, 3,
	6, 6, 0, 2,
	6, 7, 1, 3,
	6, 8, 0, 2,
	6, 9, 1, 3,
	6, 10, 0, 2,
#ifdef TARGET_DC
	6, 11, 1, 5,
	6, 12, 0, 2,
	30, 13, 1, 0,
#else
	6, 11, 1, 3,
	6, 12, 0, 5,
	6, 13, 1, 3,
	6, 14, 0, 2,
	30, 15, 1, 0,
#endif
	0xff, 0xff, 0xff, 0xff,
} ;

Uint8 *staff_ptr ;	/* the next line of the credits */
Uint8 *staff_pic_ptr ;	/* the next picture */
Uint8 staff_head ;	/* the line is a heading, drawn in the other colour */
Uint8 staff_stop ;	/* the credits are over, and stop rising */
Uint8 staff_out ;	/* the scene is over: everything fades out */
Uint8 staff_big ;	/* the letters are double size */

/****************************************************************/
/*			Each frame, by Sys->routine_1: the set-up, the		*/
/*			roll and the end									*/
/****************************************************************/
void staff_main( void )
{
	static void ( *staff_jmp[3] )( void ) = {
		staff_init, staff_move, staff_exit,
	} ;

	staff_jmp[Sys->routine_1]() ;
	work_list_move( W_SYS ) ;
	work_list_move( W_BACK ) ;
	SysTransSet( disp_front_spr ) ;
}

/*==============================================================*/
static void staff_init( void )
{
	static VEC3 light_dir = { 0.0f, 1.0f, 1.0f } ;

	if( !Sys->routine_3 ){
		Sys->routine_2 = 0 ;
		Sys->routine_3 ++ ;
		Sys->entry_stop = 1 ;
		Sys->timer0 = 6000 ;
		Sys->timer1 = 0 ;
		Sys->timer2 = 1 ;
		Sys->timer3 = 60 ;
		staff_ptr = ( Uint8 * )staff_data ;
		staff_pic_ptr = staff_pic_tbl ;
		staff_head = 1 ;
		staff_big = 0 ;
		staff_out = 0 ;
		staff_stop = 0 ;
		staff_sound_load() ;
		work_init() ;
		light_init() ;
		light_dir_set( &light_dir ) ;
		light_amb_set( 1.0f ) ;
		Palette_Init() ;
		Texture_Init() ;
		TexLoad( 9 ) ;
		scr_work_init() ;
		back_color_set( 0xff000000 ) ;
		bright_init() ;
		fade_set( -1, 60, FADE_IN ) ;
		fog_init() ;
		fog_work.on = 0 ;
		fog_work.len = 0x80 ;
		fog_work.density = 9000.0f ;
		fog_work.r = 0 ;
		fog_work.g = 0xb0 ;
		fog_work.b = 0xe0 ;
		fog_set() ;
	}
	else {
		Sys->routine_1 ++ ;
		Sys->routine_3 = 0 ;
		stream_req( STAFF_STREAM ) ;
		s3l_fade_in_set( 1 ) ;
		stream_start() ;
	}
}

/*--------------------------------------------------------------*/
static void staff_move( void )
{
	static void ( *staff_move_jmp[8] )( void ) = {
		staff_fade_in, staff_back_set, staff_back_in, staff_roll,
		staff_roll_out, staff_copyright, staff_thanks, staff_move_7,
	} ;

	staff_move_jmp[Sys->routine_2]() ;
}

/*==============================================================*/
/*			A short wait										*/
/*==============================================================*/
static void staff_fade_in( void )
{
	if( !--Sys->timer3 ){
		Sys->routine_2 ++ ;
		Sys->timer3 = 16 ;
	}
}

/*==============================================================*/
/*			The scene fades in									*/
/*==============================================================*/
static void staff_back_set( void )
{
	if( !--Sys->timer3 ){
		Sys->routine_2 ++ ;
		Sys->timer3 = 480 ;
		at03_ef1_set() ;
		at03_ef5_set() ;
		fade_set( 0xff000000, 480, FADE_IN ) ;
		staff_se_req( 0 ) ;
	}
	if( ( Sys->play_side & 1 && sw_work[0].now & KEY_START )
	 || ( Sys->play_side & 2 && sw_work[1].now & KEY_START ) ) staff_skip() ;
}

/*--------------------------------------------------------------*/
static void staff_back_in( void )
{
	if( !--Sys->timer3 ){
		Sys->routine_2 ++ ;
		Sys->timer3 = 24 ;
	}
	if( ( Sys->play_side & 1 && sw_work[0].now & KEY_START )
	 || ( Sys->play_side & 2 && sw_work[1].now & KEY_START ) ) staff_skip() ;
}

/*==============================================================*/
/*			The roll: a picture at its time, and a line of		*/
/*			the credits every 32 frames							*/
/*==============================================================*/
static void staff_roll( void )
{
	Float x ;

	if( Sys->timer0 -- == 0 ){
		Sys->routine_2 ++ ;
		Sys->timer0 = 32 ;
		staff_out = 1 ;
		fade_set( 0xff000000, 32, FADE_OUT ) ;
		return ;
	}
	if( Sys->timer0 == 220 ){
		sound_all_off() ;
		se_all_off() ;
		s3l_fade_out_set( 20 ) ;
	}
	fog_set() ;
	if( !--Sys->timer2 ){
		if( ( Sys->timer2 = *staff_pic_ptr ) != 0xff ){
			Sys->timer2 *= 60 ;
			if( staff_pic_ptr[1] ){
				tex_req_flag = 1 ;
				tex_req_no = staff_pic_ptr[1] + 13 ;
			}
			staff_pic_set( staff_pic_ptr[2] + 0x910, staff_pic_ptr[3] ) ;
			staff_pic_ptr += 4 ;
		}
	}
	if( *staff_ptr != STAFF_END ){
		if( !--Sys->timer3 ){
			Sys->timer3 = 32 ;
			x = 304.0f ;
			if( *staff_ptr == STAFF_HEAD ) staff_head = 1 ;
			if( *staff_ptr == STAFF_NAME ) staff_head = 0 ;
			if( *staff_ptr == STAFF_BIG ) staff_big = 1 ;
			while( *staff_ptr < STAFF_CODE ){
				if( *staff_ptr != ' ' ) staff_chr_set( *staff_ptr, x, -1.0f ) ;
				if( !staff_big ) x += 16.0f ;
				else x += 32.0f ;
				staff_ptr ++ ;
			}
			if( *staff_ptr == STAFF_HEAD ) staff_head = 1 ;
			if( *staff_ptr == STAFF_NAME ) staff_head = 0 ;
			if( *staff_ptr == STAFF_BIG ) staff_big = 1 ;
			staff_ptr ++ ;
		}
	}
	else staff_stop = 1 ;
	if( ( Sys->play_side & 1 && sw_work[0].now & KEY_START )
	 || ( Sys->play_side & 2 && sw_work[1].now & KEY_START ) ) staff_skip() ;
}

/*==============================================================*/
/*			The roll fades out									*/
/*==============================================================*/
static void staff_roll_out( void )
{
	if( !--Sys->timer0 ){
		Sys->routine_2 ++ ;
		Sys->timer0 = 240 ;
		work_init() ;
		fade_set( 0xff000000, 32, FADE_IN ) ;
	}
}

/*==============================================================*/
/*			The logo, then the screen fades out					*/
/*==============================================================*/
static void staff_copyright( void )
{
	const MES_WORK *p ;

	if( !--Sys->timer0 ){
#ifdef TARGET_DC
		/* the home game shows a third logo first */
		Sys->routine_2 = 7 ;
		Sys->timer0 = 120 ;
		logo3_set() ;
#else
		Sys->routine_2 ++ ;
		Sys->timer0 = 240 ;
		logo_set() ;
		logo_flash_set() ;
		fade_set( 0xff000000, 2, FADE_IN ) ;
#endif
	}
	else {
		if( Sys->cpu_level_type == 1 ) p = copyright_mes_1 ;
		else p = copyright_mes_0 ;
		while( p->x < 100 ){
			mes_font_disp( p ) ;
			p ++ ;
		}
		if( Sys->timer0 == 40 ) fade_set( 0xff000000, 32, FADE_OUT ) ;
	}
	fog_set() ;
}

/*--------------------------------------------------------------*/
static void staff_thanks( void )
{
	if( !--Sys->timer0 ){
		Sys->routine_1 ++ ;
		Sys->routine_2 = 0 ;
	}
	else {
		STrPrint( 48, 36, 16, "THANK YOU FOR PLAYING" ) ;
		STrPrint( 60, 46, 16, "PRESENTED BY" ) ;
	}
	fog_set() ;
	if( ( Sys->play_side & 1 && sw_work[0].now & KEY_START )
	 || ( Sys->play_side & 2 && sw_work[1].now & KEY_START ) ) staff_exit() ;
}

/*==============================================================*/
/*			The skip's way back in: the logo, then THANK YOU	*/
/*			FOR PLAYING											*/
/*==============================================================*/
static void staff_move_7( void )
{
	if( !--Sys->timer0 ){
		Sys->routine_2 = 6 ;
		Sys->timer0 = 240 ;
		logo_set() ;
		logo_flash_set() ;
		fade_set( 0xff000000, 2, FADE_IN ) ;
	}
	fog_set() ;
}

/*==============================================================*/
/*			Back to the demo									*/
/*==============================================================*/
static void staff_exit( void )
{
	Sys->scene_no = 5 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->timer0 = 0 ;
	Sys->timer1 = 0 ;
	Sys->timer2 = 0 ;
	Sys->timer3 = 0 ;
	work_init() ;
	TexLoad( 15 ) ;
	back_color_set( 0xff000000 ) ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
}

/*==============================================================*/
/*			A start button skips the roll						*/
/*==============================================================*/
static void staff_skip( void )
{
	Sys->routine_2 = 5 ;
	Sys->timer0 = 240 ;
	fade_set( 0xff000000, 32, FADE_IN ) ;
	work_init() ;
	back_color_set( 0xff000000 ) ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
}

/*==============================================================*/
/*			A letter of the credits, rising from the bottom		*/
/*			of the screen										*/
/*==============================================================*/
static void staff_chr_set( Uint8 c, Float x, Float spd )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->type = c ;
		ew->work_func = staff_chr_move ;
		ew->tile_top = staff_head + 0x912 ;
		ew->disp_x = x ;
		ew->disp_y = 496.0f ;
#ifdef TARGET_DC
		ew->disp_x += option_w.adjust_x ;
		ew->disp_y += option_w.adjust_y ;
#endif
		ew->disp_z = 1.1f ;
		ew->disp_zm_x = 0.125f ;
		ew->disp_zm_y = 0.25f ;
		ew->disp_ang = 0 ;
		ew->alpha = 1.0f ;
		ew->disp_list = 2 ;
		ew->disp_flip = 5 ;
		ew->spd.y = spd ;
	}
}

/*==============================================================*/
static void staff_chr_move( PLAYER_WORK *ew )
{
	Float u, v ;

	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 32 ;
		switch( ew->type ){
		case '&' :
			u = 0.25f ;
			v = 0.75f ;
			break ;
		case '-' :
			u = 0.375f ;
			v = 0.75f ;
			break ;
		case '.' :
			u = 0.5f ;
			v = 0.75f ;
			break ;
		case '!' :
			u = DOT_X( 0.375f ) ;
			v = 0.75f ;
			break ;
		case '(' :
			u = 0.75f ;
			v = 0.75f ;
			break ;
		case ')' :
			u = 0.875f ;
			v = 0.75f ;
			break ;
		default :
			u = ( Float )( ( ( ew->type - 'A' ) & 7 ) << 3 ) / 64.0f ;
			v = ( Float )( ( ( ew->type - 'A' ) >> 3 ) << 2 ) / 16.0f ;
			break ;
		}
		ew->disp_u0 = u ;
		ew->disp_u1 = u + 0.125f ;
		ew->disp_v0 = v ;
		ew->disp_v1 = v + 0.25f ;
		if( !staff_big ){
			ew->disp_zm_x = 0.125f ;
			ew->disp_zm_y = 0.25f ;
		}
		else {
			ew->disp_zm_x = 0.25f ;
			ew->disp_zm_y = 0.5f ;
		}
	}
	if( !staff_stop ) ew->disp_y += ew->spd.y ;
	if( staff_out ){
		ew->alpha -= 0.03125f ;
		if( --ew->timer0 < 0 ) work_free( ew ) ;
	}
	if( ew->disp_y < -48.0f ) work_free( ew ) ;
}

/*==============================================================*/
/*			A picture, of one of six kinds						*/
/*==============================================================*/
static void staff_pic_set( Sint32 tile, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->sub_type = type ;
		ew->work_func = staff_pic_move ;
		ew->tile_top = tile ;
		ew->disp_x = 32.0f ;
		ew->disp_y = 0.0f ;
#ifdef TARGET_DC
		ew->disp_x += option_w.adjust_x ;
		ew->disp_y += option_w.adjust_y ;
#endif
		ew->disp_z = 1.1f ;
		ew->disp_zm_x = 1.0f ;
		ew->disp_zm_y = 2.0f ;
		ew->disp_ang = 0 ;
		ew->disp_list = 2 ;
		ew->disp_flip = 0x1005 ;
	}
}

/*==============================================================*/
static void staff_pic_move( PLAYER_WORK *ew )
{
	static void ( *pic_jmp[6] )( PLAYER_WORK *ew ) = {
		pic_stay, pic_shake, pic_left, pic_right, pic_down, pic_up,
	} ;

	pic_jmp[ew->sub_type]( ew ) ;
}

/*==============================================================*/
/*			Fades in, and out when the scene ends				*/
/*==============================================================*/
static void pic_stay( PLAYER_WORK *ew )
{
	static void ( *pic_stay_jmp[4] )( PLAYER_WORK *ew ) = {
		pic_stay_init, pic_stay_in, pic_stay_wait, pic_stay_out,
	} ;

	pic_stay_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pic_stay_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 16 ;
	ew->timer1 = 240 ;
	ew->disp_u0 = 0.0f ;
	ew->disp_u1 = 0.9375f ;
	ew->disp_v0 = 0.0f ;
	ew->disp_v1 = 1.0f ;
	ew->alpha = 0.0f ;
}

/*--------------------------------------------------------------*/
static void pic_stay_in( PLAYER_WORK *ew )
{
	ew->alpha += 0.0625f ;
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 1200 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_stay_wait( PLAYER_WORK *ew )
{
	if( staff_out ){
		ew->routine_0 ++ ;
		ew->timer0 = 32 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_stay_out( PLAYER_WORK *ew )
{
	ew->alpha -= 0.03125f ;
	if( !--ew->timer0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			Fades in and out shaking							*/
/*==============================================================*/
static void pic_shake( PLAYER_WORK *ew )
{
	static void ( *pic_shake_jmp[4] )( PLAYER_WORK *ew ) = {
		pic_shake_init, pic_shake_in, pic_shake_wait, pic_shake_out,
	} ;

	pic_shake_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pic_shake_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 32 ;
	ew->timer1 = 632 ;
	ew->disp_u0 = 0.0f ;
	ew->disp_u1 = 0.9375f ;
	ew->disp_v0 = 0.0f ;
	ew->disp_v1 = 1.0f ;
	ew->alpha = 0.0f ;
}

/*--------------------------------------------------------------*/
static void pic_shake_in( PLAYER_WORK *ew )
{
	ew->alpha += 0.03125f ;
	ew->disp_y = ( -- ew->timer1 & 2 ) ? 0.0f : -2.0f ;
#ifdef TARGET_DC
	ew->disp_y += option_w.adjust_y ;
#endif
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 584 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_shake_wait( PLAYER_WORK *ew )
{
	ew->disp_y = ( -- ew->timer1 & 2 ) ? 0.0f : -2.0f ;
#ifdef TARGET_DC
	ew->disp_y += option_w.adjust_y ;
#endif
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_shake_out( PLAYER_WORK *ew )
{
	ew->alpha -= 0.0625f ;
	ew->disp_y = ( -- ew->timer1 & 2 ) ? 0.0f : -2.0f ;
#ifdef TARGET_DC
	ew->disp_y += option_w.adjust_y ;
#endif
	if( !--ew->timer0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			Pans left											*/
/*==============================================================*/
static void pic_left( PLAYER_WORK *ew )
{
	static void ( *pic_left_jmp[6] )( PLAYER_WORK *ew ) = {
		pic_left_init, pic_in, pic_wait, pic_left_move, pic_wait2, pic_out,
	} ;

	pic_left_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pic_left_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 16 ;
	ew->timer1 = 360 ;
	ew->disp_u0 = 0.0625f ;
	ew->disp_u1 = 1.0f ;
	ew->disp_v0 = 0.0f ;
	ew->disp_v1 = 1.0f ;
	ew->alpha = 0.0f ;
}

/*--------------------------------------------------------------*/
static void pic_left_move( PLAYER_WORK *ew )
{
	ew->disp_u0 -= 0.00052083336f ;
	ew->disp_u1 -= 0.00052083336f ;
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 240 ;
	}
}

/*==============================================================*/
/*			Pans right											*/
/*==============================================================*/
static void pic_right( PLAYER_WORK *ew )
{
	static void ( *pic_right_jmp[6] )( PLAYER_WORK *ew ) = {
		pic_right_init, pic_in, pic_wait, pic_right_move, pic_wait2, pic_out,
	} ;

	pic_right_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pic_right_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 16 ;
	ew->timer1 = 360 ;
	ew->disp_u0 = 0.0f ;
	ew->disp_u1 = 0.9375f ;
	ew->disp_v0 = 0.0f ;
	ew->disp_v1 = 1.0f ;
	ew->alpha = 0.0f ;
}

/*--------------------------------------------------------------*/
static void pic_right_move( PLAYER_WORK *ew )
{
	ew->disp_u0 += 0.00052083336f ;
	ew->disp_u1 += 0.00052083336f ;
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 240 ;
	}
}

/*==============================================================*/
/*			Pans down											*/
/*==============================================================*/
static void pic_down( PLAYER_WORK *ew )
{
	static void ( *pic_down_jmp[6] )( PLAYER_WORK *ew ) = {
		pic_down_init, pic_in, pic_wait, pic_down_move, pic_wait2, pic_out,
	} ;

	pic_down_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pic_down_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 16 ;
	ew->timer1 = 360 ;
	ew->disp_y -= 32.0f ;
	ew->disp_u0 = 0.0f ;
	ew->disp_u1 = 0.9375f ;
	ew->disp_v0 = 0.0f ;
	ew->disp_v1 = 1.0f ;
	ew->alpha = 0.0f ;
}

/*--------------------------------------------------------------*/
static void pic_down_move( PLAYER_WORK *ew )
{
	ew->disp_y += 0.26666668f ;
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 240 ;
	}
}

/*==============================================================*/
/*			Pans up												*/
/*==============================================================*/
static void pic_up( PLAYER_WORK *ew )
{
	static void ( *pic_up_jmp[6] )( PLAYER_WORK *ew ) = {
		pic_up_init, pic_in, pic_wait, pic_up_move, pic_wait2, pic_out,
	} ;

	pic_up_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pic_up_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->timer0 = 16 ;
	ew->timer1 = 360 ;
	ew->disp_u0 = 0.0f ;
	ew->disp_u1 = 0.9375f ;
	ew->disp_v0 = 0.0f ;
	ew->disp_v1 = 1.0f ;
	ew->alpha = 0.0f ;
}

/*--------------------------------------------------------------*/
static void pic_up_move( PLAYER_WORK *ew )
{
	ew->disp_y -= 0.26666668f ;
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 240 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_wait( PLAYER_WORK *ew )
{
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 120 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_wait2( PLAYER_WORK *ew )
{
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 16 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_in( PLAYER_WORK *ew )
{
	ew->alpha += 0.0625f ;
	if( !--ew->timer0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 1 ;
	}
}

/*--------------------------------------------------------------*/
static void pic_out( PLAYER_WORK *ew )
{
	ew->alpha -= 0.0625f ;
	if( !--ew->timer0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			The logo's model, shrinking							*/
/*==============================================================*/
static void logo_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = logo_move ;
		ew->obj = obj_dm05->obj[23] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->pos.x = 0.0f ;
		ew->pos.y = 0.0f ;
		ew->pos.z = -0.5f ;
		ew->scale.x = 0.03f ;
		ew->scale.y = 0.03f ;
		ew->scale.z = 0.01f ;
	}
}

/*==============================================================*/
static void logo_move( PLAYER_WORK *ew )
{
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->timer0 = 100 ;
	}
	else if( !ew->routine_1 ){
		if( --ew->timer0 ){
			ew->scale.x -= 0.0002f ;
			ew->scale.y -= 0.0002f ;
		}
		else {
			ew->routine_1 ++ ;
			ew->timer0 = 480 ;
		}
	}
	else if( !--ew->timer0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			A flash over the logo								*/
/*==============================================================*/
static void logo_flash_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = logo_flash_move ;
		ew->tile_top = 2 ;
		ew->disp_x = 0.0f ;
		ew->disp_y = 0.0f ;
		ew->disp_z = 0.4f ;
		ew->disp_zm_x = 20.0f ;
		ew->disp_zm_y = 15.0f ;
		ew->disp_ang = 0 ;
		ew->disp_list = 2 ;
		ew->disp_flip = 0x1005 ;
		ew->alpha = 1.0f ;
	}
}

/*==============================================================*/
static void logo_flash_move( PLAYER_WORK *ew )
{
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 60 ;
	}
	else if( --ew->timer0 ) ew->alpha -= 0.016666668f ;
	else work_free( ew ) ;
}

/****************************************************************/
/*			Another model of the logo: the home game shows it	*/
/*			after the copyright, and on the board nothing sets	*/
/*			it													*/
/****************************************************************/
void logo3_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SYS, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = logo3_move ;
		ew->obj = obj_dm05->obj[24] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_BRIGHT ;
		ew->pos.x = 0.0f ;
		ew->pos.y = 0.0f ;
		ew->pos.z = -0.5f ;
		ew->scale.x = 0.1f ;
		ew->scale.y = 0.1f ;
		ew->scale.z = 0.01f ;
	}
}

/*==============================================================*/
static void logo3_move( PLAYER_WORK *ew )
{
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 120 ;
	}
	else {
		if( Sys->play_mode == 64 ) return ;
		if( !--ew->timer0 ) work_free( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "staff.c" End of File	*/
