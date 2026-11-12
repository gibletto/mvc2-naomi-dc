/* The rankings: the screen after a game that shows the player's place, the attract mode's
   ranking with the top team, and the tables' defaults. */
#include "player.h"

#define SKIP_SW	0x360	/* the buttons that end the screen early */
#define NAME_SPACE	42	/* the name entry's letter for a space */
#define RANK_MAX	21	/* the places in each ranking */
#define RANK_NONE	20	/* a player's place when not in the ranking */

#define TEAM_PL( no, side )	( pl_work + ( no ) * 2 )[side]	/* a side's first, second or third player */

extern void disp_front_stage2( void );	/* the scene's display */
extern void rank_tex_set( int a, int b );
extern void rank_bg_set( int no );
extern void rank_frame_set( int no );
extern void rank_page_set( int no, int a );
extern void rank_mark_set( int no, int a );
#ifdef TARGET_DC
extern void fight_load_start( int no );	/* am_load.c: asks the disc for a scene's files */
extern void adx_pause( int flag );	/* pl_sw.c: holds the music */
#endif

static void rank_disp_set( void );
static void ranking_init( void );
static void ranking_move( void );
static void ranking_end( void );
static void rank_demo_init( void );
static void rank_demo_move( void );
static void rank_demo_end( void );
static Uint8 rank_chr_get( Uint8 c );
static void rank_name_def( RANK_DATA *rd );

/* the defaults, with the names in ASCII */
static const RANK_DATA rank_def[RANK_MAX] = {
	{  1, { 0x17, 0x13, 0x00 }, { 0, 1, 2 }, 0, 10, 10,  0,  0, "ONI ", 0x00000000, 2 },
	{  2, { 0x16, 0x14, 0x02 }, { 0, 1, 2 }, 0,  9, 10, 10,  0, "KAD ", 0x90000000, 1 },
	{  3, { 0x0f, 0x15, 0x01 }, { 0, 1, 2 }, 0,  8, 10, 20,  0, "TLD ", 0x80000000, 1 },
	{  4, { 0x09, 0x12, 0x05 }, { 0, 1, 2 }, 0,  7, 10, 30,  0, "HAC ", 0x70000000, 1 },
	{  5, { 0x08, 0x11, 0x22 }, { 0, 1, 2 }, 0,  6, 10, 40,  0, "TOM ", 0x60000000, 1 },
	{  6, { 0x0a, 0x10, 0x04 }, { 0, 1, 2 }, 0,  5, 10, 50,  0, "MNR ", 0x50000000, 1 },
	{  7, { 0x07, 0x12, 0x02 }, { 0, 1, 2 }, 0,  4, 11,  0,  0, "YOU ", 0x40000000, 1 },
	{  8, { 0x0c, 0x14, 0x05 }, { 0, 1, 2 }, 0,  3, 12,  0,  0, "MOT ", 0x30000000, 1 },
	{  9, { 0x0e, 0x15, 0x04 }, { 0, 1, 2 }, 0,  2, 13,  0,  0, "YUK ", 0x20000000, 1 },
	{ 10, { 0x0d, 0x15, 0x01 }, { 0, 1, 2 }, 0,  2, 14,  0,  0, "SPP ", 0x10000000, 1 },
	{ 11, { 0x17, 0x16, 0x06 }, { 0, 1, 2 }, 0,  2, 15,  0,  0, "KRA ", 0x00000000, 1 },
	{ 12, { 0x11, 0x10, 0x13 }, { 0, 1, 2 }, 0,  2, 16,  0,  0, "DAI ", 0x90000000, 0 },
	{ 13, { 0x0b, 0x08, 0x0a }, { 0, 1, 2 }, 0,  2, 17,  0,  0, "YOK ", 0x80000000, 0 },
	{ 14, { 0x14, 0x15, 0x12 }, { 0, 1, 2 }, 0,  2, 18,  0,  0, "KOZ ", 0x70000000, 0 },
	{ 15, { 0x0f, 0x07, 0x09 }, { 0, 1, 2 }, 0,  2, 19,  0,  0, "NIS ", 0x60000000, 0 },
	{ 16, { 0x22, 0x05, 0x02 }, { 0, 1, 2 }, 0,  2, 20,  0,  0, "ITT ", 0x50000000, 0 },
	{ 17, { 0x0c, 0x0d, 0x0e }, { 0, 1, 2 }, 0,  2, 21,  0,  0, "SOG ", 0x40000000, 0 },
	{ 18, { 0x00, 0x01, 0x04 }, { 0, 1, 2 }, 0,  2, 22,  0,  0, "WAN ", 0x30000000, 0 },
	{ 19, { 0x17, 0x16, 0x0f }, { 0, 1, 2 }, 0,  2, 23,  0,  0, "D50 ", 0x20000000, 0 },
	{ 20, { 0x00, 0x01, 0x02 }, { 0, 1, 2 }, 0,  2, 24,  0,  0, "DEF ", 0x10000000, 0 },
	{ 21, { 0x13, 0x15, 0x14 }, { 0, 1, 2 }, 0,  0, 99,  0,  0, "CAP ", 0x00000000, 0 },
} ;

/* the second ranking's names */
static const Uint8 rank_name[RANK_MAX][4] = {
	"ONI ", "DAI ", "MOT ", "HAC ", "KRA ", "MNR ", "YOU ", "SPP ", "TLD ", "KOZ ", "KAD ",
	"WAN ", "YUK ", "SOG ", "NIS ", "HKD ", "YOK ", "ITT ", "D50 ", "DEF ", "CAP ",
} ;

/* the home game's third ranking's; the board doesn't use them */
static const Uint8 rank_sa_name[RANK_MAX][4] = {
	"ONI ", "SPP ", "DAI ", "KOZ ", "REI ", "MNR ", "WAN ", "YUK ", "HKD ", "MOT ", "TLD ",
	"KAD ", "YOU ", "YOK ", "ITT ", "KRA ", "NIS ", "HAC ", "D50 ", "DEF ", "CAP ",
} ;

/* the name entry's letter for each ASCII character from the space */
static const Uint8 rank_chr_tbl[0x60] = {
	0x29, 0x26, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x1b, 0x2a, 0x1a, 0x2a,
	0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x27,
	0x28, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
	0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x2a, 0x2a, 0x2a, 0x2a, 0x2b,
	0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
	0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a, 0x2a,
} ;

/*==============================================================*/
/*			The scene's models, camera, fog and the				*/
/*			ranking's pages										*/
/*==============================================================*/
static void rank_disp_set( void )
{
	static VEC3 light_dir = { 0.0f, 1.0f, 1.0f } ;
	int rank ;
	int no ;
	Sint8 page ;

	Sys->routine_1 ++ ;
	Sys->routine_2 = 20 ;
	Sys->timer0 = 540 ;
	Sys->timer1 = 7 ;
	Sys->timer2 = 0 ;
	Sys->timer3 = 0 ;
	work_init() ;
	light_init() ;
	light_dir_set( &light_dir ) ;
	light_amb_set( 1.0f ) ;
	bright_init() ;
	Palette_Init() ;
	Texture_Init() ;
	TexLoad( 10 ) ;
	scr_work_init() ;
	scr_work.scr_type = 1 ;
	scr_work.scr_routine[1] = 3 ;
	scr_work.fov_x = 90.0f ;
	scr_work.eye[0].x = 0.0f ;
	scr_work.eye[0].y = 0.0f ;
	scr_work.eye[0].z = 0.0f ;
	scr_work.target[0].x = 0.0f ;
	scr_work.target[0].y = 0.0f ;
	scr_work.target[0].z = -1.0f ;
	scr_work.roll = 0 ;
	scr_work.shift_y = 0.0f ;
	rank_bg_set( 0 ) ;
	rank_bg_set( 2 ) ;
	rank_frame_set( 0 ) ;
	rank_frame_set( 1 ) ;
	rank_frame_set( 2 ) ;
	if( Sys->play_mode == 96 && Sys->event_flag ){
		page = vs_sys_work.winner_id + 1 ;
		rank_page_set( page + 5, page + 10 ) ;
	}
	else {
#ifdef TARGET_DC
		/* the score attack's ranking has the first's pages */
		switch( Sys->rank_disp ){
		case 0 :
			rank_page_set( 0, 0 ) ;
			rank_page_set( 1, 1 ) ;
			rank_page_set( 2, 2 ) ;
			rank_page_set( 3, 3 ) ;
			rank_page_set( 4, 4 ) ;
			rank = pl_work[Sys->rank_side].score_rank ;
			break ;
		case 1 :
			rank_page_set( 0, 5 ) ;
			rank_page_set( 1, 6 ) ;
			rank_page_set( 2, 7 ) ;
			rank_page_set( 3, 8 ) ;
			rank = pl_work[Sys->rank_side].win_rank ;
			break ;
		case 2 :
			rank_page_set( 0, 0 ) ;
			rank_page_set( 1, 1 ) ;
			rank_page_set( 2, 2 ) ;
			rank_page_set( 3, 3 ) ;
			rank_page_set( 4, 4 ) ;
			rank = pl_work[Sys->rank_side].score_rank ;
			break ;
		}
#else
		if( Sys->vs_flag ){
			rank_page_set( 0, 5 ) ;
			rank_page_set( 1, 6 ) ;
			rank_page_set( 2, 7 ) ;
			rank_page_set( 3, 8 ) ;
			rank = pl_work[Sys->rank_side].win_rank ;
		}
		else {
			rank_page_set( 0, 0 ) ;
			rank_page_set( 1, 1 ) ;
			rank_page_set( 2, 2 ) ;
			rank_page_set( 3, 3 ) ;
			rank_page_set( 4, 4 ) ;
			rank = pl_work[Sys->rank_side].score_rank ;
		}
#endif
		if( rank < RANK_NONE ){
			no = rank % 5 ;
			if( !rank ) rank_mark_set( 3, 0 ) ;
			else rank_mark_set( 0, no ) ;
			rank_mark_set( 1, no ) ;
		}
		else rank_mark_set( 2, 0 ) ;
	}
	vs_sys_work.game_stop = 0 ;
	back_color_set( -1 ) ;
	fog_init() ;
	fog_work.on = 1 ;
	fog_work.len = 0x80 ;
	fog_work.density = 9000.0f ;
	fog_work.r = 0xff ;
	fog_work.g = 0xff ;
	fog_work.b = 0xff ;
	fog_set() ;
	tex_req_flag = 1 ;
	tex_req_no = 11 ;
	rank_tex_set( -1, 0 ) ;
	rank_tex_set( -1, 1 ) ;
	rank_tex_set( -1, 2 ) ;
	rank_tex_set( -1, 3 ) ;
	rank_tex_set( -1, 4 ) ;
	rank_tex_set( -1, 5 ) ;
	rank_tex_set( -1, 6 ) ;
	rank_tex_set( -1, 7 ) ;
#ifdef TARGET_DC
	fight_load_start( 8 ) ;
#endif
}

/****************************************************************/
/*			The ranking after a game, each frame by				*/
/*			Sys->routine_1										*/
/****************************************************************/
void RankingMain( void )
{
	static void ( *ranking_jmp[3] )( void ) = {
		ranking_init, ranking_move, ranking_end,
	} ;

	if( Sys->join_side ){
		if( !Sys->heart_step ) work_list_move( W_SET ) ;
		Sound_Heart() ;
	}
	else {
		ranking_jmp[Sys->routine_1]() ;
		work_list_move( W_SET ) ;
		work_list_move( W_SYS ) ;
	}
	SysTransSet( disp_front_stage2 ) ;
}

/*==============================================================*/
static void ranking_init( void )
{
	Sys->rank_disp = Sys->vs_flag ;
	if( Sys->home_mode == HOME_SCORE ) Sys->rank_disp = 2 ;
	Sys->entry_stop = 1 ;
#ifdef TARGET_DC
	/* the music starts first, and is held while the disc is read */
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	scene_song_req( 8, 27 ) ;
	adx_pause( 1 ) ;
	rank_disp_set() ;
	adx_pause( 0 ) ;
#else
	rank_disp_set() ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	scene_song_req( 8, 27 ) ;
#endif
	na_req( ( Uint8 )Sys->vblank_cnt % 3 + 70 ) ;
}

/*==============================================================*/
/*			The pages turn until the player's place is shown	*/
/*==============================================================*/
static void ranking_move( void )
{
	int sw ;
	int rank ;
	int no ;

	Sys->entry_stop = Sys->play_side ? 0 : 1 ;
	sw = Sys->play_side ;
	if( !sw ) sw = Sys->entry_side ;
	if( ( sw & 1 && sw_work[0].now & SKIP_SW && Sys->timer3 )
	 || ( sw & 2 && sw_work[1].now & SKIP_SW && Sys->timer3 ) || !--Sys->timer0 ) Sys->routine_1 ++ ;
	if( Sys->timer0 == 360 ) Sys->timer3 = 1 ;
	if( Sys->timer0 == 330 ) Sys->timer3 = 2 ;
	if( Sys->timer0 == 120 ) fade_out_set( 0, 20 ) ;
	if( Sys->timer3 == 2 ){
		if( Sys->vs_flag ) rank = pl_work[Sys->rank_side].win_rank ;
		else rank = pl_work[Sys->rank_side].score_rank ;
		if( rank == RANK_NONE ) rank = 0 ;
		Sys->timer2 ++ ;
		if( Sys->timer2 >= 8 ){
			Sys->timer2 = 0 ;
			Sys->timer1 ++ ;
			if( Sys->timer1 >= 8 ) Sys->timer1 = 0 ;
			Sys->routine_2 -- ;
			if( Sys->routine_2 == rank / 5 * 5 - 2 ){
				Sys->timer3 = 3 ;
				no = 41 ;
				if( vs_sys_work.winner_id >= 0 ){
					no = 46 ;
					if( Sys->play_side || Sys->event_flag ) no = 45 ;
				}
				na_req( no ) ;
			}
			tex_req_flag = 1 ;
			tex_req_no = 11 ;
			rank_tex_set( Sys->routine_2, 7 - Sys->timer1 ) ;
		}
	}
	scr_move() ;
	light_dummy() ;
	fog_set() ;
}

/*--------------------------------------------------------------*/
static void ranking_end( void )
{
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	if( Sys->play_side == 0 ) Sys->scene_no = 3 ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
}

/****************************************************************/
/*			The attract mode's ranking, each frame by			*/
/*			Sys->routine_1										*/
/****************************************************************/
void RankDemoMain( void )
{
	static void ( *rank_demo_jmp[3] )( void ) = {
		rank_demo_init, rank_demo_move, rank_demo_end,
	} ;

	rank_demo_jmp[Sys->routine_1]() ;
	work_list_move( W_SET ) ;
	work_list_move( W_SYS ) ;
	SysTransSet( disp_front_stage2 ) ;
}

/*==============================================================*/
/*			The rankings take turns (two on the board, three	*/
/*			at home); the top team stands on the first side		*/
/*==============================================================*/
static void rank_demo_init( void )
{
	RANK_DATA *rd ;

	Sys->routine_0 = 4 ;
	Sys->rank_side = 0 ;
#ifdef TARGET_DC
	Sys->rank_disp = ( Sys->demo_no + 3 ) % 3 ;
	Sys->vs_flag = Sys->rank_disp & 1 ;
#else
	Sys->vs_flag = Sys->demo_no & 1 ;
	Sys->rank_disp = Sys->vs_flag ;
#endif
	vs_sys_work.team_w[0][0] = &pl_work[0] ;
	vs_sys_work.team_w[0][1] = &pl_work[2] ;
	vs_sys_work.team_w[0][2] = &pl_work[4] ;
	vs_sys_work.team_w[1][0] = &pl_work[1] ;
	vs_sys_work.team_w[1][1] = &pl_work[3] ;
	vs_sys_work.team_w[1][2] = &pl_work[5] ;
#ifdef TARGET_DC
	switch( Sys->rank_disp ){
	case 0 :
		rd = rank_score ;
		break ;
	case 1 :
		rd = rank_win ;
		break ;
	case 2 :
		rd = rank_sa ;
		break ;
	}
#else
	if( Sys->vs_flag ) rd = rank_win ;
	else rd = rank_score ;
#endif
	pl_work[0].load_type = rd->load_type[0] ;
	pl_work[2].load_type = rd->load_type[1] ;
	pl_work[4].load_type = rd->load_type[2] ;
	pl_work[0].old_score_hi = rd->score_hi ;
	pl_work[0].old_score = rd->score ;
	pl_work[0].win_max = rd->win_max ;
	pl_work[0].score_rank = 0 ;
	pl_work[0].win_rank = 0 ;
	Sys->rank_char = rd->load_type[0] ;
	rank_disp_set() ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	scene_song_req( 6, 26 ) ;
	fade_set( 0xff000000, 20, FADE_IN ) ;
#ifdef TARGET_DC
	fight_load_start( 6 ) ;
#endif
}

/*--------------------------------------------------------------*/
static void rank_demo_move( void )
{
	if( !--Sys->timer0 ) Sys->routine_1 ++ ;
	if( Sys->timer0 == 360 ) Sys->timer3 = 1 ;
	if( Sys->timer0 == 330 ) Sys->timer3 = 2 ;
	if( Sys->timer0 == 60 ) fade_out_set( 0, 20 ) ;
	if( Sys->timer3 == 2 ){
		Sys->timer2 ++ ;
		if( Sys->timer2 >= 8 ){
			Sys->timer2 = 0 ;
			Sys->timer1 ++ ;
			if( Sys->timer1 >= 8 ) Sys->timer1 = 0 ;
			Sys->routine_2 -- ;
			if( Sys->routine_2 == -2 ) Sys->timer3 = 3 ;
			tex_req_flag = 1 ;
			tex_req_no = 11 ;
			rank_tex_set( Sys->routine_2, 7 - Sys->timer1 ) ;
		}
	}
	scr_move() ;
	light_dummy() ;
	fog_set() ;
}

/*--------------------------------------------------------------*/
static void rank_demo_end( void )
{
	Sys->task_step = 1 ;
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->demo_no ++ ;
	work_init() ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
}

/*==============================================================*/
static Uint8 rank_chr_get( Uint8 c )
{
	return rank_chr_tbl[c - ' '] ;
}

/****************************************************************/
/*			The rankings back to their defaults					*/
/****************************************************************/
void rank_data_init( void )
{
	const RANK_DATA *def ;
	const Uint8 *name ;
#ifdef TARGET_DC
	const Uint8 *sa_name ;
#endif
	Uint8 *p ;
	int i ;
	int j ;

	name = rank_name[0] ;
#ifdef TARGET_DC
	sa_name = rank_sa_name[0] ;
#endif
	def = rank_def ;
	for( i = 0 ; i < RANK_MAX ; i++ ){
		rank_score[i] = *def ;
		rank_win[i] = *def ;
#ifdef TARGET_DC
		rank_sa[i] = *def ;
		for( j = 0 ; j < 4 ; j++ ){
			rank_sa[i].name[j] = *sa_name ;
			sa_name ++ ;
		}
#endif
		for( j = 0 ; j < 4 ; j++ ){
			rank_win[i].name[j] = *name ;
			name ++ ;
		}
		def ++ ;
		p = rank_score[i].name ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p = rank_win[i].name ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
#ifdef TARGET_DC
		p = rank_sa[i].name ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
		p ++ ;
		*p = rank_chr_get( *p ) ;
#endif
	}
}

/*==============================================================*/
/*			A name left blank becomes CAP						*/
/*==============================================================*/
static void rank_name_def( RANK_DATA *rd )
{
	int i ;

	for( i = 0 ; i < RANK_MAX ; i++ ){
		if( rd[i].name[0] == NAME_SPACE && rd[i].name[1] == NAME_SPACE && rd[i].name[2] == NAME_SPACE ){
			rd[i].name[0] = 2 ;
			rd[i].name[1] = 0 ;
			rd[i].name[2] = 15 ;
		}
	}
}

/****************************************************************/
void rank_name_chk( void )
{
	rank_name_def( rank_score ) ;
	rank_name_def( rank_win ) ;
#ifdef TARGET_DC
	rank_name_def( rank_sa ) ;
#endif
}

/****************************************************************/
/*			A player's game goes into the first ranking (at		*/
/*			home, the score attack's into the third) at his		*/
/*			place, and moves up past lower scores				*/
/****************************************************************/
void rank_entry( PLAYER_WORK *wk )
{
	RANK_DATA tmp ;
	RANK_DATA *rd ;
	RANK_DATA *up ;
	RANK_DATA *top ;
	int rank ;
	int no ;
	int other ;

#ifdef TARGET_DC
	if( Sys->home_mode != HOME_SCORE ) top = rank_score ;
	else top = rank_sa ;
#endif
	rank = wk->score_rank ;
	no = rank - 1 ;
	other = ( wk->pl_index ^ 1 ) & 1 ;
#ifndef TARGET_DC
	top = rank_score ;
#endif
	( rd = &top[rank] )->load_type[0] = TEAM_PL( 0, Sys->rank_side ).load_type ;
	rd->load_type[1] = TEAM_PL( 1, Sys->rank_side ).load_type ;
	rd->load_type[2] = TEAM_PL( 2, Sys->rank_side ).load_type ;
	rd->pl_personal[0] = TEAM_PL( 0, Sys->rank_side ).pl_personal ;
	rd->pl_personal[1] = TEAM_PL( 1, Sys->rank_side ).pl_personal ;
	rd->pl_personal[2] = TEAM_PL( 2, Sys->rank_side ).pl_personal ;
	rd->ending_flag = Sys->ending_flag ;
	rd->win_max = wk->win_max ;
	rd->play_min = wk->play_time / 3600 ;
	rd->play_sec = wk->play_time / 60 % 60 ;
	rd->play_csec = wk->play_time % 60 * 100 / 60 ;
	rd->name[0] = NAME_SPACE ;
	rd->name[1] = NAME_SPACE ;
	rd->name[2] = NAME_SPACE ;
	rd->name[3] = NAME_SPACE ;
	rd->score = wk->old_score ;
	rd->score_hi = wk->old_score_hi ;
	while( rank ){
		if( top[rank].score_hi < top[no].score_hi ) break ;
		if( top[rank].score_hi == top[no].score_hi && top[rank].score < top[no].score ) break ;
#ifndef TARGET_DC
		rd = &top[rank] ;
#endif
		up = &top[no] ;
		if( pl_work[other].score_rank == no ) pl_work[other].score_rank = rank ;
		tmp = *up ;
#ifdef TARGET_DC	/* the lower row is swapped in place */
		*up = top[rank] ;
		top[rank] = tmp ;
		top[rank].rank ++ ;
#else
		*up = *rd ;
		*rd = tmp ;
		rd->rank ++ ;
#endif
		up->rank -- ;
		rank -- ;
		no -- ;
	}
	wk->score_rank = rank ;
}

/****************************************************************/
/*			And into the second, which is ordered by win_max	*/
/****************************************************************/
void vs_rank_entry( PLAYER_WORK *wk )
{
	RANK_DATA tmp ;
	RANK_DATA *rd ;
	RANK_DATA *top ;
	int rank ;
	int no ;
	int side ;
	int other ;

	other = ( wk->pl_index ^ 1 ) & 1 ;
	rank = wk->win_rank ;
	no = rank - 1 ;
	side = wk->pl_index ;
	top = rank_win ;
	rd = &top[rank] ;
	rd->load_type[0] = TEAM_PL( 0, side ).load_type ;
	rd->load_type[1] = TEAM_PL( 1, wk->pl_index ).load_type ;
	rd->load_type[2] = TEAM_PL( 2, wk->pl_index ).load_type ;
	rd->pl_personal[0] = TEAM_PL( 0, wk->pl_index ).pl_personal ;
	rd->pl_personal[1] = TEAM_PL( 1, wk->pl_index ).pl_personal ;
	rd->pl_personal[2] = TEAM_PL( 2, wk->pl_index ).pl_personal ;
	rd->ending_flag = Sys->ending_flag ;
	rd->win_max = wk->win_max ;
	rd->play_min = wk->play_time / 3600 ;
	rd->play_sec = wk->play_time / 60 % 60 ;
	rd->play_csec = wk->play_time % 60 * 100 / 60 ;
	rd->name[0] = NAME_SPACE ;
	rd->name[1] = NAME_SPACE ;
	rd->name[2] = NAME_SPACE ;
	rd->name[3] = NAME_SPACE ;
	rd->score = wk->old_score ;
	rd->score_hi = wk->old_score_hi ;
	while( rank ){
		if( top[rank].win_max < top[no].win_max ) break ;
		if( pl_work[other].win_rank == no ) pl_work[other].win_rank = rank ;
		tmp = top[no] ;
		top[no] = top[rank] ;
		top[rank] = tmp ;
		top[rank].rank ++ ;
		top[no].rank -- ;
		rank -- ;
		no -- ;
	}
	wk->win_rank = rank ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ranking.c" End of File	*/
