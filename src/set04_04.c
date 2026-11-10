/* Stage 4's object 4 (kind 5 works): when a side joins, three drops of water fall from places above
   the stage, each making a splash that runs through its own models as it reaches the water. */
#include "player.h"

/* where each drop starts and where it is done */
static VEC3 set04_04_pos[3][2] = {
	{ { -1420.0f, 2130.0f, -1380.0f }, { -1420.0f, -370.0f, -1380.0f } },
	{ { -1360.0f, 1920.0f, -1430.0f }, { -1360.0f, -150.0f, -1430.0f } },
	{ { -1500.0f, 1800.0f, -1420.0f }, { -1500.0f, -360.0f, -1420.0f } },
} ;
static Float set04_04_acc[3] = { -1.0f, -0.7f, -0.5f } ;
static Sint32 set04_04_obj[3] = { 9, 16, 23 } ;

/* the splash's models, frame by frame */
static Sint32 set04_04_splash_0[22] = { 10, 10, 10, 11, 11, 11, 12, 12, 12, 12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 15 } ;
static Sint32 set04_04_splash_1[22] = { 17, 17, 17, 18, 18, 18, 19, 19, 19, 19, 19, 19, 20, 20, 20, 20, 21, 21, 21, 22, 22, 22 } ;
static Sint32 set04_04_splash_2[22] = { 24, 24, 24, 25, 25, 25, 26, 26, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 29, 29, 29 } ;
static Sint32 *set04_04_splash_tbl[3] = { set04_04_splash_0, set04_04_splash_1, set04_04_splash_2 } ;
static Sint32 set04_04_splash_cnt[3] = { 22, 22, 22 } ;

static void set04_04_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void set04_04_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = set04_04_main ;
	}
}

static void set04_04_drop_set( Sint32 no ) ;

/*==============================================================*/
static void set04_04_main( PLAYER_WORK *ew )
{
	Sint32 i ;

	if( side_flag_get( 0 ) ){
		for( i = 0 ; i < 3 ; i ++ ) set04_04_drop_set( i ) ;
	}
}

static void set04_04_drop_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set04_04_drop_set( Sint32 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_04_drop_main ;
		ew->obj = obj_stg->obj[set04_04_obj[no]] ;
		ew->pos = set04_04_pos[no][0] ;
		ew->pos.x += nlRand() % 330 ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->type = no ;
	}
}

static void set04_04_splash_set( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void set04_04_drop_main( PLAYER_WORK *ew )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += set04_04_acc[ew->type] ;
	switch( ew->routine_0 ){
	case 0:
		if( ew->pos.y > -85.0f ) break ;
		set04_04_splash_set( ew ) ;
		ew->routine_0 ++ ;
		break ;
	case 1:
		if( ew->pos.y <= set04_04_pos[ew->type][1].y ) work_free( ew ) ;
		break ;
	}
}

static void set04_04_splash_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set04_04_splash_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set04_04_splash_main ;
		ew->pos = pl_w->pos ;
		ew->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		ew->type = pl_w->type ;
	}
}

/*==============================================================*/
static void set04_04_splash_main( PLAYER_WORK *ew )
{
	ew->obj = obj_stg->obj[set04_04_splash_tbl[ew->type][ew->timer0]] ;
	if( ++ew->timer0 >= set04_04_splash_cnt[ew->type] ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set04_04.c" End of File	*/
