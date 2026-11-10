/* Stage 05's object 1 (the stage makes three of it): a model (object 3 or 4 of the stage's file) that
   starts at a random place and falls, shrinking, until it reaches the ground, where it is set up
   again and leaves a puff (set05_02). */
#include "player.h"

static VEC3 set05_01_spd = { 0.0f, 0.0f, 0.0f } ;
static VEC3 set05_01_acc = { 0.0f, -1.0f, 0.0f } ;
static VEC3 set05_01_scale = { 0.5f, 0.5f, 0.5f } ;
static VEC3 set05_01_pos[] = {	/* start places (one entry) */
	{ 0.0f, 975.0f, -2240.0f },
} ;

static void set05_01_main( PLAYER_WORK *ew ) ;
extern void set05_02_set( VEC3 *pos ) ;

/****************************************************************/
void set05_01_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set05_01_main ;
		ew->obj = obj_stg->obj[3] ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
	}
}

/*==============================================================*/
static void set05_01_main( PLAYER_WORK *ew )
{
	switch( ew->routine_0 ){
	case 0 :
		ew->routine_0 ++ ;
		ew->obj = obj_stg->obj[3 + nlRand() % 2] ;
		ew->pos = set05_01_pos[0] ;
		ew->pos.x += nlRand() % 2000 - 1000 ;
		ew->pos.y += nlRand() % 200 ;
		ew->pos.z += nlRand() % 2000 - 1000 ;
		ew->spd = set05_01_spd ;
		ew->acc = set05_01_acc ;
		ew->scale = set05_01_scale ;
		break ;
	case 1 :
		if( ew->pos.y <= -63.6f ){
			ew->routine_0 = 0 ;
			ew->pos.y = -63.6f ;
			set05_02_set( &ew->pos ) ;
		}
		ew->scale.y -= 0.05f ;
		move_pos_spd( ew ) ;
		break ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set05_01.c" End of File	*/
