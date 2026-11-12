/* The start demo: the screen before a fight that shows the two teams, where holding buttons
   changes who goes first. */
#include "player.h"

#define SKIP_SW	0x360	/* the buttons that end the demo early */

extern void disp_front_stage( void );	/* the scene's display */
#ifdef TARGET_DC
extern void disp_st_demo_end( void );
#endif
extern void fight_load_start( int a );
extern void st_demo_ef0_set( void );
extern void st_demo_ef1_set( int no );
extern void st_demo_ef2_set( int no, int side );

static void st_demo_init( void );
static void st_demo_move( void );
static void st_demo_out( void );
static void st_demo_end( void );
static void team_order_set( void );

/****************************************************************/
/*			Each frame, by Sys->routine_1						*/
/****************************************************************/
void StartDemoMain( void )
{
	static void ( *st_demo_jmp[4] )( void ) = {
		st_demo_init, st_demo_move, st_demo_out, st_demo_end,
	} ;

	if( Sys->join_side ) Sound_Heart() ;
	else {
		Sys->st_demo_flag = 1 ;
		st_demo_jmp[Sys->routine_1]() ;
		work_list_move( W_SET ) ;
		work_list_move( W_SYS ) ;
	}
	SysTransSet( disp_front_stage ) ;
}

/*==============================================================*/
/*			The teams' models, the camera, and the music and	*/
/*			the announcer										*/
/*==============================================================*/
static void st_demo_init( void )
{
#ifdef TARGET_DC	/* the scene's own files must be read first */
	if( !load_ck() ) return ;
#endif
	Sys->routine_1 ++ ;
	Sys->timer0 = 300 ;
	Sys->timer3 = 0 ;
	Sys->entry_stop = Sys->play_side ? 0 : 1 ;
	if( pl_work[0].load_type == 24 && pl_work[2].load_type == 24 && pl_work[4].load_type == 24 ){
		pl_work[2].load_type = 25 ;
		pl_work[4].load_type = 26 ;
	}
	if( pl_work[1].load_type == 24 && pl_work[3].load_type == 24 && pl_work[5].load_type == 24 ){
		pl_work[3].load_type = 25 ;
		pl_work[5].load_type = 26 ;
	}
	work_init() ;
	light_init() ;
	bright_init() ;
	Palette_Init() ;
	Texture_Init() ;
	TexLoad( 6 ) ;
	song_req( 2 ) ;
	na_req( 68 ) ;
#ifdef TARGET_DC	/* the fight's files are read from the disc while the scene runs */
	fight_load_start( 10 ) ;
	fight_load_start( 3 ) ;
	fight_load_start( 9 ) ;
#else
	fight_load_start( 0 ) ;
#endif
	scr_work_init() ;
	scr_work.eye[0].x = 0.0f ;
	scr_work.eye[0].y = 160.0f ;
	scr_work.eye[0].z = 900.0f ;
	scr_work.target[0].x = 0.0f ;
	scr_work.target[0].y = 160.0f ;
	scr_work.target[0].z = 0.0f ;
	scr_work.shift_y = 0.0f ;
	st_demo_ef0_set() ;
	st_demo_ef1_set( 0 ) ;
	st_demo_ef1_set( 1 ) ;
	st_demo_ef1_set( 2 ) ;
	st_demo_ef1_set( 3 ) ;
	st_demo_ef1_set( 4 ) ;
	st_demo_ef1_set( 5 ) ;
	st_demo_ef2_set( 0, 0 ) ;
	st_demo_ef2_set( 1, 0 ) ;
	st_demo_ef2_set( 2, 0 ) ;
	st_demo_ef2_set( 3, 0 ) ;
	st_demo_ef2_set( 4, 0 ) ;
	st_demo_ef2_set( 5, 0 ) ;
	st_demo_ef2_set( 0, 1 ) ;
	st_demo_ef2_set( 1, 1 ) ;
	st_demo_ef2_set( 2, 1 ) ;
	st_demo_ef2_set( 3, 1 ) ;
	st_demo_ef2_set( 4, 1 ) ;
	st_demo_ef2_set( 5, 1 ) ;
	bright_init() ;
	fade_set( -1, 30, FADE_IN ) ;
	fog_init() ;
	fog_work.on = 1 ;
	fog_work.len = 42 ;
	fog_work.density = 7000.0f ;
	fog_work.r = 0xff ;
	fog_work.g = 0xff ;
	fog_work.b = 0x80 ;
	fog_set() ;
}

/*==============================================================*/
/*			The teams are shown, until the time runs out or		*/
/*			a button is pressed									*/
/*==============================================================*/
static void st_demo_move( void )
{
	Sys->timer0 -- ;
	sw_work[0].now &= 0xfc9f ;
	sw_work[1].now &= 0xfc9f ;
#ifdef TARGET_DC	/* no button ends it early, and it waits for the files */
	if( Sys->timer0 == 60 ){
		if( !load_ck() ) Sys->timer0 ++ ;
		else {
			Sys->routine_1 ++ ;
			Sys->timer0 = 60 ;
			Sys->timer3 = 2 ;
		}
	}
#else
	if( ( Sys->play_side & 1 && sw_work[0].now & SKIP_SW )
	 || ( Sys->play_side & 2 && sw_work[1].now & SKIP_SW ) || Sys->timer0 == 60 ){
		Sys->routine_1 ++ ;
		Sys->timer0 = 60 ;
		Sys->timer3 = 2 ;
	}
#endif
	if( Sys->timer0 == 150 ) Sys->timer3 = 1 ;
	scr_work.eye[1] = scr_work.eye[0] ;
	scr_work.target[1] = scr_work.target[0] ;
	light_dummy() ;
	fog_set() ;
}

/*==============================================================*/
/*			The sound and the fog close in						*/
/*==============================================================*/
static void st_demo_out( void )
{
	if( !--Sys->timer0 ) Sys->routine_1 ++ ;
	if( Sys->timer0 <= 30 ) se_vol_down( 1, 30 ) ;
	if( Sys->timer0 == 10 ) Sys->timer3 = 3 ;
	if( Sys->timer3 >= 2 ){
		fog_work.density -= 140.0f ;
		fog_work.b += 2 ;
		if( fog_work.density < 0.0f ){
			Sys->timer3 = 3 ;
			fog_work.density = 0.0f ;
			fog_work.b = 0xff ;
			back_color_set( -1 ) ;
		}
	}
	light_dummy() ;
	fog_set() ;
}

/*==============================================================*/
/*			On to the fight										*/
/*==============================================================*/
static void st_demo_end( void )
{
	Sys->routine_0 ++ ;
	Sys->routine_1 = 0 ;
	Sys->routine_2 = 0 ;
	Sys->routine_3 = 0 ;
	Sys->exit_flag = 0 ;
	Sys->entry_stop = 1 ;
#ifdef TARGET_DC	/* a network game keeps the order both sides have */
	if( Sys->home_mode != HOME_NETWORK ) team_order_set() ;
#else
	team_order_set() ;
#endif
	back_color_set( -1 ) ;
	sound_all_off() ;
	se_all_off() ;
#ifdef TARGET_DC
	SysTransSet( disp_st_demo_end ) ;
#endif
}

/*==============================================================*/
/*			The team order: a held button swaps two of the		*/
/*			side's characters									*/
/*==============================================================*/
static void team_order_set( void )
{
	Uint16 sw ;

	if( Sys->play_side & 1 ){
		int no ;

		sw = pl_work[0].pad_sw.now & KEY_ASSIST ;
		if( sw == KEY_ASSIST ){
			no = pl_work[2].team_order ;
			pl_work[2].team_order = pl_work[4].team_order ;
			pl_work[4].team_order = no ;
		}
		else if( sw == KEY_A1 ){
			no = pl_work[0].team_order ;
			pl_work[0].team_order = pl_work[2].team_order ;
			pl_work[2].team_order = no ;
		}
		else if( sw == KEY_A2 ){
swap_0_4 :
			no = pl_work[0].team_order ;
			pl_work[0].team_order = pl_work[4].team_order ;
			pl_work[4].team_order = no ;
		}
	}
	if( Sys->play_side & 2 ){
		Uint8 no ;

		sw = pl_work[1].pad_sw.now & KEY_ASSIST ;
		if( sw == KEY_ASSIST ){
			no = pl_work[3].team_order ;
			pl_work[3].team_order = pl_work[5].team_order ;
			pl_work[5].team_order = no ;
			return ;
		}
		if( sw == KEY_A1 ){
			no = pl_work[1].team_order ;
			pl_work[1].team_order = pl_work[3].team_order ;
			pl_work[3].team_order = no ;
			return ;
		}
		if( sw == KEY_A2 ){
			no = pl_work[1].team_order ;
			pl_work[1].team_order = pl_work[5].team_order ;
			pl_work[5].team_order = no ;
			return ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "start_demo.c" End of File	*/
