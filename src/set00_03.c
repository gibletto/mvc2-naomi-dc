/* Stage 00's fourth object (kind 5 works): a model that waits out the first
   frames of a fight, then crosses the stage once, along one of six lines, fading in and out as a
   sine of how far along it is. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* where each line starts and where it ends */
typedef struct SET00_03_LINE {
	VEC3	from, to;
} SET00_03_LINE;

static SET00_03_LINE set00_03_line_tbl[6] = {
	{ { -1000.0f, 500.0f, 200.0f }, { -200.0f, 500.0f, -3000.0f } },
	{ { 900.0f, 600.0f, 200.0f }, { 1700.0f, 600.0f, -3000.0f } },
	{ { 1000.0f, 700.0f, 200.0f }, { 1800.0f, 700.0f, -3000.0f } },
	{ { -1000.0f, 600.0f, 200.0f }, { -200.0f, 600.0f, -3000.0f } },
	{ { -550.0f, 750.0f, 200.0f }, { 350.0f, 750.0f, -3000.0f } },
	{ { 1000.0f, 550.0f, 200.0f }, { 1800.0f, 550.0f, -3000.0f } },
} ;

static void set00_03_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set00_03_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = set00_03_main ;
		ew->obj_flag = 0xb21 ;
	}
}

/*==============================================================*/
static void set00_03_main( PLAYER_WORK *ew )
{
	Uint32 t ;

	switch( ew->routine_0 ){
	case 0:
		ew->disp_flag = 0 ;
		if( ++ ew->timer0 >= 60 ){
			ew->timer0 = 60 ;
			t = Sys->play_timer ;
			if( t > 500 && t < 1000 ) return ;
			if( t > 3500 && t < 4500 ) return ;
			ew->routine_0 ++ ;
			ew->type = ( Uint32 )nlRand() % 6 ;
			ew->timer0 = 0 ;
			ew->disp_flag = 1 ;
			ew->obj = obj_stg->obj[nlRand() % 2 + 9] ;
			ew->pos = set00_03_line_tbl[ew->type].from ;
			ew->rot[2] = nlRand() % 0x4000 ;
		}
		break ;
	case 1:
		ew->pos.x += ( set00_03_line_tbl[ew->type].to.x - set00_03_line_tbl[ew->type].from.x ) / 120.0f ;
		ew->pos.y += ( set00_03_line_tbl[ew->type].to.y - set00_03_line_tbl[ew->type].from.y ) / 120.0f ;
		ew->pos.z += ( set00_03_line_tbl[ew->type].to.z - set00_03_line_tbl[ew->type].from.z ) / 120.0f ;
		ew->fade.trnsl = nlSin( DEG_ANG( ew->timer0 / 120.0f * 180.0f ) ) ;
		if( ++ ew->timer0 >= 120 ){
			ew->routine_0 = 0 ;
			ew->timer0 = 0 ;
		}
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set00_03.c" End of File	*/
