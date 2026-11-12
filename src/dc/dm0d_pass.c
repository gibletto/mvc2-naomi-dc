/* DM0D, the panel that asks for the lobby's ID and password: its frame, with the ID written out and
   the password as stars. */
#include "player.h"
#include "dm0d.h"

extern unsigned long strlen( const char *s ) ;

void pass_parts_set( Uint8 n, Uint16 obj ) ;
void pass_move( PLAYER_WORK *w ) ;
void pass_init( PLAYER_WORK *w ) ;
void pass_main( PLAYER_WORK *w ) ;
void pass_put( void ) ;
void pass_off( PLAYER_WORK *w ) ;
void pass_free( PLAYER_WORK *w ) ;

/****************************************************************/
/*			The panel's fourteen parts							*/
/****************************************************************/
void dm0d_pass_set( void )
{
	pass_parts_set( 0, 63 ) ;
	pass_parts_set( 1, 64 ) ;
	pass_parts_set( 2, 65 ) ;
	pass_parts_set( 3, 66 ) ;
	pass_parts_set( 4, 16 ) ;
	pass_parts_set( 5, 16 ) ;
	pass_parts_set( 6, 16 ) ;
	pass_parts_set( 7, 61 ) ;
	pass_parts_set( 8, 60 ) ;
	pass_parts_set( 9, 59 ) ;
	pass_parts_set( 10, 68 ) ;
	pass_parts_set( 11, 210 ) ;
	pass_parts_set( 12, 62 ) ;
	pass_parts_set( 13, 62 ) ;
}

void pass_parts_set( Uint8 n, Uint16 obj )
{
	PLAYER_WORK *w ;

	if( ( w = work_get( 0, W_SYS, LIST_TOP ) ) ){
		w->disp_flag = 0 ;
		w->work_func = pass_move ;
		w->obj = obj_dm0d->obj[obj] ;
		w->obj_flag = OBJ_POS | OBJ_SCALE ;
		w->sub_no = n ;
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
	}
}

void pass_move( PLAYER_WORK *w )
{
	static void ( *pass_jmp[4] )( PLAYER_WORK *w ) = { pass_init, pass_main, pass_off, pass_free } ;

	pass_jmp[w->routine_0]( w ) ;
}

/* each part's place and size; part 0 writes the text */
void pass_init( PLAYER_WORK *w )
{
	if( !w->sub_no ) pass_put() ;
	switch( w->sub_no ){
	case 0 :
	case 1 :
	case 2 :
	case 3 :
	case 11 :
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
		w->pos.x = 0.0f ;
		w->pos.y = 0.0f ;
		w->pos.z = 0.0f ;
		break ;
	case 7 :
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
		w->pos.z = -12.9f ;
		break ;
	case 8 :
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
		w->pos.z = -13.3f ;
		break ;
	case 9 :
		w->scale.x = 1.0f ;
		w->scale.y = 1.0f ;
		w->scale.z = 1.0f ;
		w->pos.z = -13.1f ;
		break ;
	case 4 :
		w->scale.x = 0.75f ;
		w->scale.y = 1.2f ;
		w->scale.z = 1.0f ;
		w->pos.x = -7.33f ;
		w->pos.y = 1.41f ;
		w->pos.z = -12.8f ;
		break ;
	case 5 :
		w->scale.x = 0.75f ;
		w->scale.y = 1.2f ;
		w->scale.z = 1.0f ;
		w->pos.x = -7.33f ;
		w->pos.y = -0.83f ;
		w->pos.z = -12.8f ;
		break ;
	case 6 :
		w->scale.x = 0.35f ;
		w->scale.y = 1.2f ;
		w->scale.z = 1.0f ;
		w->pos.x = -7.53f ;
		w->pos.y = -2.96f ;
		w->pos.z = -12.8f ;
		break ;
	case 10 :
		w->scale.x = 1.9f ;
		w->scale.y = 1.1f ;
		w->scale.z = 1.0f ;
		w->pos.x = 6.68f ;
		w->pos.y = 4.59f ;
		w->pos.z = -0.14f ;
		break ;
	case 12 :
		w->scale.x = 0.5f ;
		w->scale.y = 0.65f ;
		w->scale.z = 1.0f ;
		w->pos.x = 3.14f ;
		w->pos.y = 1.52f ;
		w->pos.z = -12.9f ;
		break ;
	case 13 :
		w->scale.x = 0.5f ;
		w->scale.y = 0.65f ;
		w->scale.z = 1.0f ;
		w->pos.x = 3.14f ;
		w->pos.y = -0.79f ;
		w->pos.z = -12.9f ;
		break ;
	}
	w->disp_flag = 1 ;
	w->routine_0 ++ ;
}

/* part 10 is the cursor, on the ID or the password */
void pass_main( PLAYER_WORK *w )
{
	if( !w->sub_no ) pass_put() ;
	if( w->sub_no == 10 ){
		switch( net_cursor_no ){
		case 0 :
			w->scale.x = 1.9f ;
			w->scale.y = 1.1f ;
			w->scale.z = 1.0f ;
			w->pos.x = 6.68f ;
			w->pos.y = 2.32f ;
			w->pos.z = -0.14f ;
			break ;
		case 1 :
			w->scale.x = 0.9f ;
			w->scale.y = 1.1f ;
			w->scale.z = 1.0f ;
			w->pos.x = -0.96f ;
			w->pos.y = 0.19f ;
			w->pos.z = -0.14f ;
			break ;
		}
	}
}

void pass_put( void )
{
	Sint8 len, i ;
	char buf[64] ;

	str_put( 238.0f, 192.0f, 12.0f, 0xff000000, net_user_id ) ;
	len = strlen( net_password ) ;
	memset( buf, 0, 64 ) ;
	for( i = 0 ; i < len ; i ++ ) buf[i] = '*' ;
	str_put( 238.0f, 250.0f, 12.0f, 0xff000000, buf ) ;
}

void pass_off( PLAYER_WORK *w )
{
	w->disp_flag = 0 ;
	w->routine_0 ++ ;
}

void pass_free( PLAYER_WORK *w )
{
	work_free( w ) ;
}
