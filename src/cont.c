/* The continue scene: the count while a side may continue, and where the game goes after it
  . */
#include "player.h"

extern void disp_stage_front( void ) ;
extern void cont_ef0_set( void ) ;
extern void cont_ef1_set( void ) ;

static void cont_start( void ) ;
static void cont_count( void ) ;
static void cont_over( void ) ;
static void cont_restart( void ) ;

/****************************************************************/
/*			The continue scene, run each frame					*/
/****************************************************************/
void cont_main( void )
{
	static void ( *cont_jmp[] )( void ) = {
		cont_start, cont_count, cont_over, cont_restart,
	} ;

	cont_jmp[Sys->routine_0]() ;
	work_list_move( W_SET ) ;
	work_list_move( W_SYS ) ;
	if( !Sys->exit_flag ) SysTransSet( disp_stage_front ) ;
}

/*==============================================================*/
/*			The continue's start, if a continue is allowed		*/
/*==============================================================*/
static void cont_start( void )
{
	SYSTEM_WORK *sys ;

	light_init() ;
	bright_init() ;
	TexLoad( 15 ) ;
	back_color_set2( 0xff000000, 0xff000000 ) ;
	sys = Sys ;
	if( sys->event_flag ) goto skip ;
	if( sys->cont_enable ){
		sys->routine_0 ++ ;
		Sys->entry_stop = 0 ;
		Sys->timer2 = 9 ;
		Sys->timer3 = 120 ;
		se_vol_up( 0, 1 ) ;
		se_vol_up( 1, 1 ) ;
		song_req( 4 ) ;
		song_req( 4 ) ;
		na_req( 60 ) ;
	} else {
skip:
		sys->routine_0 = 2 ;
		return ;
	}
	cont_ef0_set() ;
	cont_ef1_set() ;
	back_color_set( 0xffffffff ) ;
}

/*==============================================================*/
/*			The count, from the side still in its continue		*/
/*==============================================================*/
static void cont_count( void )
{
	int side ;

	light_dummy() ;
	if( Sys->new_side ){
		Sys->routine_0 = 3 ;
		Sys->timer1 = 180 ;
		fade_out_set( 0, 20 ) ;
		na_req( 65 ) ;
		Sys->exit_flag = 2 ;
		return ;
	}
	if( !Sys->cont_flag ){
		Sys->routine_0 ++ ;
		Sys->timer2 = 0 ;
		Sys->timer3 = 0 ;
	} else {
		if( Sys->cont_flag == 3 ){
			if( pl_work[0].entry_timer > pl_work[1].entry_timer ) side = 0 ;
			else side = 1 ;
		} else side = Sys->cont_flag - 1 ;
		Sys->timer2 = pl_work[side].entry_timer - 1 ;
		Sys->timer3 = pl_work[side].entry_timer2 - 1 ;
		if( Sys->timer2 < 0 ) Sys->timer2 = 0 ;
	}
	if( !Sys->timer2 && !Sys->routine_1 ){
		Sys->routine_1 ++ ;
		na_req( 62 ) ;
	}
}

/*==============================================================*/
/*			No one continued: on to the game over				*/
/*==============================================================*/
static void cont_over( void )
{
	Sys->scene_no = 5 ;
	Sys->routine_0 = 0 ;
	Sys->entry_stop = 1 ;
	sound_all_off() ;
	se_all_off() ;
}

/*==============================================================*/
/*			A side continued: wait for the fade, then start		*/
/*			again												*/
/*==============================================================*/
static void cont_restart( void )
{
	if( Sys->exit_flag ) Sys->exit_flag -- ;
	if( !Sys->timer1 -- ){
		Sys->scene_no = 0 ;
		Sys->routine_0 = 0 ;
		Sys->routine_1 = 0 ;
		Sys->routine_0 = 0 ;
		Sys->routine_2 = 0 ;
		Sys->routine_3 = 0 ;
		Sys->exit_flag = 0 ;
		Sys->entry_stop = 1 ;
		work_init() ;
		back_color_set( 0xff000000 ) ;
		sound_all_off() ;
		se_all_off() ;
		stream_stop() ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "cont.c" End of File	*/
