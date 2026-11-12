/* The network screens' helpers: a screen's starting state, set from the entry screen. */
#include "player.h"

void net_scene_init( void ) ;
void net_scene_clear( void ) ;

/****************************************************************/
/*			A network screen's state, by its number				*/
/****************************************************************/
void net_scene_set( int no )
{
	switch( no ){
	case 0 :
		net_scene_init() ;
		break ;
	case 1 :
		unk_8C2136FC = 15 ;
		unk_8C2136FD = 0 ;
		unk_8C2136FE = 0 ;
		unk_8C2136FF = 0 ;
		unk_8C213700 = 0 ;
		unk_8C213705 = -9 ;
		break ;
	case 2 :
		unk_8C2136FD = 9 ;
		unk_8C2136FE = 0 ;
		unk_8C2136FF = 2 ;
		unk_8C213700 = 0 ;
		break ;
	case 3 :
		net_scene_clear() ;
		break ;
	}
}

/****************************************************************/
/*			The network screens' start: sound off, their		*/
/*			textures, the scene from its first step				*/
/****************************************************************/
void net_scene_init( void )
{
	sound_all_off() ;
	se_all_off() ;
	stream_stop() ;
	Palette_Init() ;
	Texture_Init() ;
	work_init() ;
	TexLoad( 16 ) ;
	Sys->scene_no = 0 ;
	Sys->routine_0 = 0 ;
	unk_8C212F0C = unk_8C212F0D = unk_8C212F0E = unk_8C212F0F = 0 ;
}

/****************************************************************/
/*			Clear the four bytes								*/
/****************************************************************/
void net_scene_clear( void )
{
	unk_8C212F0C = unk_8C212F0D = unk_8C212F0E = unk_8C212F0F = 0 ;
}
