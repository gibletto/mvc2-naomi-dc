/* Sound_Heart: the scene that waits for the sound bank to load, with its own music and
   announcer, run by the scene mains while Sys->join_side is set. */
#include "player.h"
#include "naomi.h"

#define SKIP_SW	0x360	/* the buttons that end the scene early */

#ifdef TARGET_DC
extern void load_stop( void );
extern void load_side_set( Sint8 side );
extern void fight_load_start( int no );	/* am_load.c: asks the disc for a scene's files */
#else
extern void load_init( void );
#endif
extern void sound_heart_ef0_set( void );
extern void sound_heart_ef1_set( int no );

static void heart_init( void );
static void heart_wait( void );
static void heart_move( void );
static void heart_end( void );

/****************************************************************/
/*			Each frame, by Sys->heart_step						*/
/****************************************************************/
void Sound_Heart( void )
{
	static void ( *heart_jmp[4] )( void ) = {
		heart_init, heart_wait, heart_move, heart_end,
	} ;

	heart_jmp[Sys->heart_step]() ;
}

/*==============================================================*/
/*			The music stops, and the scene starts once the		*/
/*			bank's DMA is done									*/
/*==============================================================*/
static void heart_init( void )
{
#ifdef TARGET_DC
	load_stop() ;
#else
	load_init() ;
#endif
	Sys->heart_step ++ ;
	Sys->timer0 = 300 ;
	Sys->timer3 = 5 ;
#ifndef TARGET_DC
	stream_stop() ;
	while( !nlaCheckWaitROMDMA() ) ;
#endif
	sound_all_off() ;
	se_all_off() ;
	work_list_kill( W_SYS ) ;
	TexLoad( 8 ) ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	scene_song_req( 3, 24 ) ;
	na_req( 47 ) ;
	sound_heart_ef0_set() ;
	sound_heart_ef1_set( 0 ) ;
	sound_heart_ef1_set( 1 ) ;
	sound_heart_ef1_set( 2 ) ;
	sound_heart_ef1_set( 3 ) ;
	sound_heart_ef1_set( 4 ) ;
	sound_heart_ef1_set( 5 ) ;
	sound_heart_ef1_set( 6 ) ;
	sound_heart_ef1_set( 7 ) ;
	work_list_move( W_MARK ) ;
#ifdef TARGET_DC	/* the fight's files are read while the scene runs */
	fight_load_start( 11 ) ;
	fight_load_start( 12 ) ;
#endif
}

/*==============================================================*/
static void heart_wait( void )
{
	if( Sys->timer3 ){
		if( Sys->timer3 > 2 ) work_list_move( W_MARK ) ;
		Sys->timer3 -- ;
		return ;
	}
	work_list_move( W_SYS ) ;
	Sys->timer0 -- ;
	if( Sys->timer0 > 180 ) return ;
	Sys->heart_step ++ ;
	vs_sys_work.game_stop = 0 ;
}

/*==============================================================*/
/*			Until the time runs out or a button is pressed		*/
/*==============================================================*/
static void heart_move( void )
{
	work_list_move( W_SYS ) ;
	Sys->timer0 -- ;
	if( ( Sys->play_side & 1 && sw_work[0].now & SKIP_SW )
	 || ( Sys->play_side & 2 && sw_work[1].now & SKIP_SW ) || !Sys->timer0 ){
#ifdef TARGET_DC	/* it waits here until the fight's files are read */
		Sys->timer0 = 1 ;
		if( load_ck() ) Sys->heart_step ++ ;
#else
		Sys->heart_step ++ ;
#endif
	}
}

/*==============================================================*/
static void heart_end( void )
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
	back_color_set( 0xff000000 ) ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
#ifdef TARGET_DC
	load_side_set( Sys->play_side ) ;
#endif
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sound_heart.c" End of File	*/
