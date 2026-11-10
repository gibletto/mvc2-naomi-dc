/* The shadow under each fighter (kind 6 works, made by pl_shadow_set for the player it follows):
   it keeps to the ground below the player, as the shadow of the animation's frame asks, and goes
   when he leaves the fight. */
#include "player.h"

/* each character's shadow sizes, two for each step, and its offsets, by cg_shadow's low 5 bits (shadow_tbl.c) */
extern Float *shadow_size_tbl[59] ;
extern Sint16 *shadow_ofs_tbl[59] ;

static void shadow_main( PLAYER_WORK *ew );

/****************************************************************/
void pl_shadow_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = shadow_main ;
		ew->obj = obj_efky->obj[11] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FACE_Y | OBJ_BRIGHT ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->scale.y = 1.0f ;
		ew->base_mtx = 0 ;
	}
}

/*==============================================================*/
static void shadow_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float dx ;
	const Sint16 *ofs ;
	const Float *size ;

	if( pl_w->routine_0 >= 2 ){
		if( 1 ) ew->disp_flag = 0 ;
		else ew->disp_flag = 0 ;
		work_free( ew ) ;
		return ;
	}
	if( !pl_w->be_flag || pl_w->cg_shadow == 0xff ){
		ew->disp_flag = 0 ;
	} else {
		ew->disp_flag = 1 ;
		ofs = shadow_ofs_tbl[pl_w->pl_type] ;
		dx = DOT_X( ofs[pl_w->cg_shadow & 31] ) ;
		if( pl_w->disp_dir ) dx = -dx ;
		ew->pos.x = pl_w->pos.x + dx ;
		ew->pos.y = pl_w->pos.z + 1.0f ;
		ew->pos.z = 0 ;
		size = shadow_size_tbl[pl_w->pl_type] ;
		size += ( pl_w->cg_shadow >> 4 ) & 14 ;
		ew->scale.x = *size ++ * 1.2f ;
		ew->scale.z = *size * 1.2f ;
		ew->fade.trnsl = 0.5f ;
	}
}

/* six empty routines, called by nothing */
void shadow_nop0( void ){}
void shadow_nop1( void ){}
void shadow_nop2( void ){}
void shadow_nop3( void ){}
void shadow_nop4( void ){}
void shadow_nop5( void ){}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shadow.c" End of File	*/
