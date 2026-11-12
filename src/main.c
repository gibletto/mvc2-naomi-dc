/* The program's main(): the set-up, then the frame loop that runs the game's tasks once each
   vertical blank. */
#include "player.h"

/* SHC's built-ins, as its umachine.h names them */
#define get_fpscr()	_builtin_get_fpscr()
#define set_fpscr( cr )	_builtin_set_fpscr( cr )


#ifdef TARGET_DC
extern void system_init( void );
extern void gd_door_ck( void );
extern void net_vsync_wait( void );
extern void system_exit( void );
extern void vib_stop( void );
#else
extern void system_init( int a );
extern void warning_disp( void );
extern void jamma_err_init( void );
extern void credit_str_set( void );
extern void book_coin_ck( void );
extern void jamma_err_ck( void );
#endif
extern void sys_data_load( void );
extern void System_Init( void );
extern void key_repeat( SW_WORK *sw );
extern void SysTransReset2( void );
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
#ifdef TARGET_DC
	system_init() ;
	sys_data_load() ;
	nlWaitVSync() ;
#else
	system_init( 0 ) ;
	sys_data_load() ;
	warning_disp() ;
	jamma_err_init() ;
#endif
	System_Init() ;
	while( 1 ){
		nlWaitVSync() ;
		nlSysMetricsCpuPowerStart() ;
		key_repeat( &sw_work[0] ) ;
		key_repeat( &sw_work[1] ) ;
		SysTransReset2() ;
#ifdef TARGET_DC
		/* the console goes back to its menu when the door opens, once a card save is over */
		gd_door_ck() ;
		if( exit_req == 1 && vms_work.save_busy != 1 ){
			vib_stop() ;
			nlWaitVSync() ;
			net_vsync_wait() ;
			system_exit() ;
		}
#else
		credit_str_set() ;
		book_coin_ck() ;
		jamma_err_ck() ;
#endif
		sound_vsync() ;
		vsync_flag = 0 ;
		vsync_count = 0 ;
		System_Main() ;
		vms_check() ;
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
