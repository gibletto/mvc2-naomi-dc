/* Task 0, the start. */
#include "player.h"

extern void rnd_init( void ) ;

static void task_init_rnd( void ) ;
static void task_init_end( void ) ;

/****************************************************************/
/*			Task 0: the start									*/
/****************************************************************/
void task_init( void )
{
	static void ( *task_00_jmp[] )( void ) = {
		task_init_rnd,
		task_init_end,
	} ;

	task_00_jmp[Sys->task_step]() ;
}

/*==============================================================*/
static void task_init_rnd( void )
{
	Sys->task_step ++ ;
	rnd_init() ;
}

/*==============================================================*/
static void task_init_end( void )
{
	Sys->play_mode = 64 ;
	Sys->task_no = 1 ;
	Sys->task_step = 0 ;
	Sys->scene_no = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "task_init.c" End of File	*/
