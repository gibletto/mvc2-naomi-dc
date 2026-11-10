/* Stage 07's object 1 (the stage's table has no entry for it): two models that,
   every sixteen frames, jump to one of four places picked at random, and change through eight
   models (objects 18 to 25 of the stage's file) as they do. */
#include "player.h"

static Sint32 set07_01_obj_tbl[8] = {
	18, 19, 20, 21, 22, 23, 24, 25,
} ;
static VEC3 set07_01_pos_tbl[2][4] = {
	{
		{ 1480.817f, 39.08f, -604.619f },
		{ 1211.958f, 30.0f, -696.109f },
		{ 603.0f, 28.0f, -889.999f },
		{ 247.0f, 39.08f, -889.999f },
	},
	{
		{ -17.6f, 20.0f, -889.999f },
		{ -790.0f, -7.28f, -852.513f },
		{ -1100.0f, 25.0f, -725.0f },
		{ -1341.06f, 39.08f, -581.746f },
	},
} ;
static Sint32 set07_01_rot_tbl[2][4] = {
	{ 62116, 62116, 0, 0, },
	{ 32768, 37482, 37482, 37482, },
} ;

static void set07_01_make( int no ) ;

/****************************************************************/
void set07_01_set( void )
{
	set07_01_make( 0 ) ;
	set07_01_make( 1 ) ;
}

static void set07_01_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set07_01_make( int no )
{
	PLAYER_WORK *ew ;
	int pos_no ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set07_01_main ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		pos_no = nlRand() % 4 ;
		ew->obj = obj_stg->obj[set07_01_obj_tbl[0]] ;
		ew->pos = set07_01_pos_tbl[no][pos_no] ;
		ew->rot[1] = set07_01_rot_tbl[no][pos_no] ;
		ew->type = no ;
	}
}

/*==============================================================*/
static void set07_01_main( PLAYER_WORK *ew )
{
	int pos_no ;

	if( ++ ew->timer1 >= 2 ){
		ew->timer1 = 0 ;
		if( ( Uint32 )( ++ ew->timer0 ) >= 8 ){
			ew->timer0 = 0 ;
			pos_no = nlRand() % 4 ;
			ew->pos = set07_01_pos_tbl[ew->type][pos_no] ;
			ew->rot[1] = set07_01_rot_tbl[ew->type][pos_no] ;
		}
		ew->obj = obj_stg->obj[set07_01_obj_tbl[ew->timer0]] ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set07_01.c" End of File	*/
