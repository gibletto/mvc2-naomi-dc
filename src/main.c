/* The program's main(): the set-up, then the frame loop that runs the game's tasks once each
   vertical blank. */
#include "player.h"

/* SHC's built-ins, as its umachine.h names them */
#define get_fpscr()	_builtin_get_fpscr()
#define set_fpscr( cr )	_builtin_set_fpscr( cr )


extern void system_init( int a );
extern void sys_data_load( void );
extern void warning_disp( void );
extern void jamma_err_init( void );
extern void System_Init( void );
extern void key_repeat( SW_WORK *sw );
extern void SysTransReset2( void );
extern void credit_str_set( void );
extern void book_coin_ck( void );
extern void jamma_err_ck( void );
extern void sound_vsync( void );
extern void System_Main( void );
extern void SysTrans( void );
extern void SysTrans2( void );
extern void vsync_wait( void );
extern void load_main( void );

/****************************************************************/
void main( void )
{
	int cr ;

	cr = get_fpscr() ;
	cr &= ~3 ;	/* round to nearest */
	set_fpscr( cr ) ;
	Sys = &sys_work ;
	system_init( 0 ) ;
	sys_data_load() ;
	warning_disp() ;
	jamma_err_init() ;
	System_Init() ;
	while( 1 ){
		nlWaitVSync() ;
		nlSysMetricsCpuPowerStart() ;
		key_repeat( &sw_work[0] ) ;
		key_repeat( &sw_work[1] ) ;
		SysTransReset2() ;
		credit_str_set() ;
		book_coin_ck() ;
		jamma_err_ck() ;
		sound_vsync() ;
		vsync_flag = 0 ;
		vsync_count = 0 ;
		System_Main() ;
		vms_main() ;
		SysTrans() ;
		SysTrans2() ;
		vsync_wait() ;
		load_main() ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "main.c" End of File	*/
