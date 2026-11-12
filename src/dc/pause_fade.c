/* A pause menu piece fading in or out by acc.x a frame; 1 once it is solid or gone. Nothing calls
   them. */
#include "player.h"

int pause_fade_in( PLAYER_WORK *w )
{
	w->fade.trnsl += w->acc.x ;
	if( -- w->timer0 < 0 ){
		w->fade.trnsl = 1.0f ;
		return 1 ;
	}
	return 0 ;
}

int pause_fade_out( PLAYER_WORK *w )
{
	w->fade.trnsl -= w->acc.x ;
	if( -- w->timer0 < 0 ){
		w->fade.trnsl = 0.0f ;
		return 1 ;
	}
	return 0 ;
}
