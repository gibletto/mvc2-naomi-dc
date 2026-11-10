/* The ring of seventeen pieces that turns behind the select scene's grid (kind 5 works). */
#include "player.h"
#include "select.h"

static void sel_ring_main( PLAYER_WORK *ew );

/* where each piece stands round the ring */
static Sint32 sel_ring_rot_tbl[17] = {
	0xfe95, 0xaab, 0x1555, 0x2000, 0x2c17, 0x399a, 0x5111, 0x5e94, 0x6c17,
	0x799a, 0x9111, 0x9d28, 0xa93f, 0xb6c1, 0xc2d8, 0xceef, 0xdb06,
} ;

/****************************************************************/
void sel_ring_make( void )
{
	PLAYER_WORK *ew ;
	Uint8 i ;

	for( i = 0 ; i < 17 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
		ew->disp_flag = 1 ;
		ew->work_func = sel_ring_main ;
		ew->type = i ;
		ew->pos.x = 0.0f ;
		ew->pos.y = 400.0f ;
		ew->pos.z = 0.0f ;
		ew->rot[0] = 0 ;
		ew->rot[1] = sel_ring_rot_tbl[i] ;
		ew->rot[2] = 0 ;
		ew->obj = obj_dm01->obj[i + 134] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
	}
}

static void sel_ring_turn( PLAYER_WORK *ew );
static void sel_ring_hide( PLAYER_WORK *ew );
static void sel_ring_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_ring_main( PLAYER_WORK *ew )
{
	static void ( *sel_ring_jmp[] )( PLAYER_WORK *ew ) = {
		sel_ring_turn, sel_ring_hide, sel_ring_free,
	} ;

	sel_ring_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			It turns until the scene goes on					*/
/*==============================================================*/
static void sel_ring_turn( PLAYER_WORK *ew )
{
	if( Sys->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	else ew->rot[1] -= 0x100 ;
}

/*--------------------------------------------------------------*/
static void sel_ring_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void sel_ring_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_ring.c" End of File	*/
