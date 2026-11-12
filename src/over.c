/* The game over scene: its music and its model, until its frames run out or a start button is
   pressed. */
#include "player.h"

extern void disp_stage_front( void ) ;
extern void over_ef0_set( void ) ;
extern void over_ef1_set( int no ) ;

static void over_init( void ) ;
static void over_wait( void ) ;
static void over_exit( void ) ;

/****************************************************************/
/*			The game over scene, run each frame					*/
/****************************************************************/
void over_main( void )
{
	static void ( *over_jmp[] )( void ) = {
		over_init, over_wait, over_exit,
	} ;

	over_jmp[Sys->routine_0]() ;
	work_list_move( W_SET ) ;
	work_list_move( W_SYS ) ;
	SysTransSet( disp_stage_front ) ;
}

/*==============================================================*/
/*			The game over's start: its music and its model		*/
/*==============================================================*/
static void over_init( void )
{
	Sys->routine_0 ++ ;
	Sys->entry_stop = 1 ;
	Sys->name_stop = 0 ;
	Sys->timer0 = 330 ;
	Sys->timer3 = 60 ;
	light_init() ;
	bright_init() ;
	se_vol_up( 0, 1 ) ;
	se_vol_up( 1, 1 ) ;
	scene_song_req( 5, 25 ) ;
	na_req( 61 ) ;
	over_ef0_set() ;
	over_ef1_set( 0 ) ;
	over_ef1_set( 1 ) ;
	over_ef1_set( 2 ) ;
	over_ef1_set( 3 ) ;
	over_ef1_set( 4 ) ;
	over_ef1_set( 5 ) ;
	over_ef1_set( 6 ) ;
	over_ef1_set( 7 ) ;
	over_ef1_set( 8 ) ;
	over_ef1_set( 9 ) ;
	over_ef1_set( 10 ) ;
	over_ef1_set( 11 ) ;
	over_ef1_set( 12 ) ;
	over_ef1_set( 13 ) ;
	fog_init() ;
	fog_work.on = 1 ;
	fog_work.len = 42 ;
	fog_work.density = 9000.0f ;
	fog_work.r = 0 ;
	fog_work.g = 0 ;
	fog_work.b = 0 ;
	fog_set() ;
}

/*==============================================================*/
/*			The model comes in over a second; it ends after		*/
/*			its frames or a start button						*/
/*==============================================================*/
static void over_wait( void )
{
	if( Sys->timer3 ){
		Sys->timer3 -- ;
		fog_work.len = Sys->timer3 * 32 / 60 + 10 ;
		fog_work.density = 8994.0f / ( 2 << ( 60 - Sys->timer3 ) ) + 6.0f ;
		fog_work.r -= 4 ;
		fog_work.g -= 4 ;
		fog_work.b -= 4 ;
		if( fog_work.density < 6.0f ){
			Sys->timer3 = 0 ;
			fog_work.len = 10 ;
			fog_work.density = 6.0f ;
			fog_work.r = 0 ;
			fog_work.g = 0 ;
			fog_work.b = 0 ;
		}
	}
	light_dummy() ;
	fog_set() ;
	if( Sys->timer0 ) Sys->timer0 -- ;
	if( Sys->event_flag || !Sys->entry_side ){
		if( ( sw_work[0].now & KEY_START ) || ( sw_work[1].now & KEY_START ) || !Sys->timer0 ) Sys->routine_0 ++ ;
	}
}

/*==============================================================*/
/*			On to the attract demo								*/
/*==============================================================*/
static void over_exit( void )
{
	Sys->scene_no = 6 ;
	Sys->routine_0 = 0 ;
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "over.c" End of File	*/
