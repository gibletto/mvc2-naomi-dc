/* The halos that ride the select scene's door lights (kind 5 works): they pulse and their texture scrolls. */
#include "player.h"
#include "select.h"

extern void obj_uv_begin( int *src, void *obj );
extern int obj_uv_next( void );
extern void obj_uv_get( Float *x, Float *y );
extern void obj_uv_put( Float *x, Float *y );
extern void obj_uv_clear( void *obj );

static void sel_halo_main( PLAYER_WORK *ew );
static void sel_halo_speed_main( PLAYER_WORK *ew );

/****************************************************************/
/*			A halo on the part pl_w: flag picks which of the	*/
/*			two kinds											*/
/****************************************************************/
void sel_halo_set( PLAYER_WORK *pl_w, Uint8 flag )
{
	PLAYER_WORK *ew ;

	if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->master_w = pl_w->master_w ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->ef_w = pl_w->ef_w ;
	ew->type = pl_w->type ;
	ew->sub_type = pl_w->sub_type ;
	ew->obj = obj_dm01->obj[175] ;
	if( flag ) ew->work_func = sel_halo_speed_main ;
	else ew->work_func = sel_halo_main ;
	ew->obj_flag = OBJ_SCALE | OBJ_BRIGHT ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->scale.z = 1.0f ;
	ew->dir = 0 ;
	ew->timer0 = 0 ;
	ew->timer1 = 0 ;
	ew->work1 = 6 ;
	ew->spd.x = 0.01f ;
	obj_uv_clear( ew->obj ) ;
}

static void sel_halo_hide( PLAYER_WORK *ew );
static void sel_halo_in( PLAYER_WORK *ew );
static void sel_halo_go( PLAYER_WORK *ew );
static void sel_halo_off( PLAYER_WORK *ew );
static void sel_halo_free( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_halo_main( PLAYER_WORK *ew )
{
	static void ( *sel_halo_jmp[] )( PLAYER_WORK *ew ) = {
		sel_halo_hide, sel_halo_hide, sel_halo_in, sel_halo_go, sel_halo_off, sel_halo_free,
	} ;

	sel_halo_jmp[ew->master_w->routine_0]( ew ) ;
}

/*==============================================================*/
static void sel_halo_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
}

static void sel_halo_pulse( PLAYER_WORK *ew );
static void sel_halo_scroll( PLAYER_WORK *ew );

/*==============================================================*/
/*			Only the halo of the side's own personal place		*/
/*			shows												*/
/*==============================================================*/
static void sel_halo_in( PLAYER_WORK *ew )
{
	PLAYER_WORK *tw ;

	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
	case 1:
		tw = ew->ef_w ;
		if( ew->sub_type != tw->pl_personal ) ew->disp_flag = 0 ;
		else ew->disp_flag = 1 ;
		sel_halo_pulse( ew ) ;
		sel_halo_scroll( ew ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_halo_go( PLAYER_WORK *ew )
{
	switch( ew->routine_2 ){
	case 0:
		ew->routine_2 ++ ;
		ew->work1 = 3 ;
		ew->spd.x = 0.04f ;
	case 1:
		sel_halo_pulse( ew ) ;
		sel_halo_scroll( ew ) ;
		break ;
	}
}

/*--------------------------------------------------------------*/
static void sel_halo_off( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void sel_halo_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

static void sel_halo_speed_in( PLAYER_WORK *ew );

/*==============================================================*/
static void sel_halo_speed_main( PLAYER_WORK *ew )
{
	static void ( *sel_halo2_jmp[] )( PLAYER_WORK *ew ) = {
		sel_halo_hide, sel_halo_hide, sel_halo_speed_in, sel_halo_go, sel_halo_off, sel_halo_free,
	} ;

	sel_halo2_jmp[ew->master_w->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The same, for the setting's own level				*/
/*==============================================================*/
static void sel_halo_speed_in( PLAYER_WORK *ew )
{
	switch( ew->routine_1 ){
	case 0:
		ew->routine_1 ++ ;
	case 1:
		if( ew->sub_type != Sys->game_speed ) ew->disp_flag = 0 ;
		else ew->disp_flag = 1 ;
		sel_halo_pulse( ew ) ;
		sel_halo_scroll( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*			The halo swells and shrinks between its size and	*/
/*			a little more										*/
/*==============================================================*/
static void sel_halo_pulse( PLAYER_WORK *ew )
{
	ew->timer1 -- ;
	if( ew->timer1 <= 0 ){
		ew->timer1 = ew->work1 ;
		if( ew->dir ){
			ew->scale.x += ew->spd.x ;
			if( ew->scale.x >= 1.03f ){
				ew->scale.x = 1.03f ;
				ew->dir = 0 ;
			}
		}
		else{
			ew->scale.x -= ew->spd.x ;
			if( ew->scale.x <= 1.0f ){
				ew->scale.x = 1.0f ;
				ew->dir = 1 ;
			}
		}
	}
}

/*==============================================================*/
/*			The texture slides									*/
/*==============================================================*/
static void sel_halo_scroll( PLAYER_WORK *ew )
{
	Float py, px ;

	if( !ew->sub_type ){
		ew->timer0 ++ ;
		if( ew->timer0 >= 500 ) ew->timer0 = 0 ;
		obj_uv_begin( obj_dm01->obj[176], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &px, &py ) ;
			py -= ew->timer0 * 0.002f ;
			obj_uv_put( &px, &py ) ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_halo.c" End of File	*/
