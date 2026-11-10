/* Pieces that rise at the screen's bottom (kind 8 works; nothing in the game calls the two setters
   that make the spawners): a spawner that makes one piece each frame for sixty, and another that
   makes ten, each rising from where its master stands. */
#include "player.h"

static void rise_a_main( PLAYER_WORK *ew ) ;
static void rise_a_set( PLAYER_WORK *master ) ;

/*==============================================================*/
/*			It rises 50 a frame for sixty						*/
/*==============================================================*/
static void rise_a_main( PLAYER_WORK *ew )
{
	ew->pos.y += 50.0f ;
	if( ++ ew->timer0 >= 60 ) work_free( ew ) ;
}

/*==============================================================*/
static void rise_a_set( PLAYER_WORK *master )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = rise_a_main ;
		ew->obj_flag = OBJ_POS ;
		ew->obj = obj_efky->obj[10] ;
		ew->master_w = master ;
		ew->timer0 = master->timer0 ;
		ew->pos.x = master->pos.x ;
		ew->pos.y = master->pos.y ;
		ew->pos.z = master->pos.z ;
	}
}

static void rise_a_spawn_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			A piece a frame, further each time, at a random		*/
/*			place across the screen								*/
/*==============================================================*/
static void rise_a_spawn_main( PLAYER_WORK *ew )
{
	ew->pos.x = scr_work.eye[1].x + 640.0f / scr_work.eye[1].z * ( ( ew->timer0 + 1 ) * 50 ) * ( nlRandom() - 0.5f ) ;
	ew->pos.y = -50.0f ;
	ew->pos.z = -( Float )( ( ew->timer0 + 1 ) * 50 ) + scr_work.eye[1].z ;
	rise_a_set( ew ) ;
	if( ++ ew->timer0 >= 60 ) work_free( ew ) ;
}

/****************************************************************/
/*			Nothing calls this one								*/
/****************************************************************/
void rise_a_spawn_set( PLAYER_WORK *master )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = rise_a_spawn_main ;
		ew->master_w = master ;
	}
}

static void rise_b_main( PLAYER_WORK *ew ) ;
static void rise_b_set( PLAYER_WORK *master ) ;

/*==============================================================*/
/*			It rises 50 a frame, falls back 1000 each			*/
/*			twenty, and ends after sixty						*/
/*==============================================================*/
static void rise_b_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->pos.y += 50.0f ;
	if( ew->timer0 % 20 == 0 ) ew->pos.y -= 1000.0f ;
	if( ew->timer0 >= 60 ) work_free( ew ) ;
}

/*==============================================================*/
static void rise_b_set( PLAYER_WORK *master )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = rise_b_main ;
		ew->obj_flag = OBJ_POS ;
		ew->obj = obj_efky->obj[28] ;
		ew->master_w = master ;
		ew->pos.x = master->pos.x ;
		ew->pos.y = master->pos.y ;
		ew->pos.z = master->pos.z ;
	}
}

static void rise_b_spawn_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			A piece a frame for ten, at a random place about	*/
/*			the camera											*/
/*==============================================================*/
static void rise_b_spawn_main( PLAYER_WORK *ew )
{
	ew->pos.x = scr_work.eye[1].x + ( nlRand() % 3200 - 1600 ) ;
	ew->pos.y = -175.0f ;
	ew->pos.z = ew->timer0 + -2560.0f ;
	rise_b_set( ew ) ;
	if( ++ ew->timer0 >= 10 ) work_free( ew ) ;
}

/****************************************************************/
/*			Nothing calls this one								*/
/****************************************************************/
void rise_b_spawn_set( PLAYER_WORK *master )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = rise_b_spawn_main ;
		ew->master_w = master ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "rise_ef.c" End of File	*/
