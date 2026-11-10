/* The test program's main(): the set-up, then the test menus once each vertical blank. */
#include "player.h"
#include "test.h"

extern void system_init( int test ) ;
extern void book_coin_ck( void ) ;

/****************************************************************/
void main( void )
{
	system_init( 1 ) ;
	Sys = &sys_work ;
	memset( Sys, 0, SYS_HALF ) ;
	while( 1 ){
		book_coin_ck() ;
		if( !task_wait ) test_main() ;
		nlWaitVSync() ;
	}
}

/*==============================================================*/
/*			The vertical blank interrupt's routine				*/
/*==============================================================*/
Sint32 vblank_func( void )
{
	vsync_flag = 1 ;
	return 0 ;
}
