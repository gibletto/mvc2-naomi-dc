/* The ranking screen's three frames (kind 5 works): models 4 to 6 of the
   ranking's file, fixed in place, which fade a little once the scene has begun and go when the
   screen is left. */
#include "player.h"

/* the scale a model starts at, and where the third frame stands, by Sys->rank_char */
static const VEC3 rank_frame_scale = { 1.0f, 1.0f, 1.0f } ;
static const VEC3 rank_frame_pos3_tbl[59] = {
	{ -36.33f, -29.15f, 54.18f }, { -36.26f, -29.1f, 54.05f }, { -36.57f, -29.34f, 54.6f },
	{ -36.28f, -29.1f, 54.08f }, { -36.73f, -28.3f, 54.87f }, { -35.86f, -29.1f, 53.35f },
	{ -36.3f, -29.27f, 54.12f }, { -36.4f, -28.9f, 54.3f }, { -36.4f, -28.9f, 54.3f },
	{ -36.3f, -28.9f, 54.2f }, { -36.5f, -28.95f, 54.47f }, { -35.39f, -29.09f, 53.56f },
	{ -36.41f, -28.88f, 54.32f }, { -36.6f, -29.04f, 54.69f }, { -36.91f, -29.13f, 55.18f },
	{ -36.59f, -29.27f, 54.62f }, { -36.49f, -29.27f, 54.45f }, { -36.36f, -28.74f, 54.23f },
	{ -36.27f, -28.6f, 54.07f }, { -36.2f, -27.88f, 53.94f }, { -36.38f, -28.8f, 54.29f },
	{ -36.67f, -28.6f, 54.76f }, { -36.34f, -28.93f, 54.18f }, { -36.75f, -28.89f, 54.91f },
	{ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f },
	{ -36.39f, -29.17f, 54.28f }, { -36.34f, -28.74f, 54.18f }, { -35.56f, -28.94f, 53.85f },
	{ -36.49f, -28.97f, 54.45f }, { -35.62f, -28.94f, 53.95f }, { -36.64f, -29.09f, 54.73f },
	{ -35.53f, -28.95f, 53.79f }, { -36.62f, -29.25f, 54.68f }, { -36.68f, -28.92f, 54.79f },
	{ -36.49f, -29.11f, 54.45f }, { -36.37f, -29.01f, 54.24f }, { -35.94f, -28.68f, 53.49f },
	{ -36.65f, -29.41f, 54.74f }, { -36.47f, -28.96f, 54.42f }, { -35.72f, -29.09f, 54.13f },
	{ -36.53f, -28.65f, 54.53f }, { -35.95f, -28.62f, 53.51f }, { -36.32f, -28.91f, 54.16f },
	{ -35.99f, -28.65f, 54.6f }, { -35.97f, -29.3f, 54.56f }, { -35.81f, -28.16f, 54.28f },
	{ -36.01f, -28.82f, 54.63f }, { -36.52f, -28.22f, 54.51f }, { -36.67f, -29.1f, 54.76f },
	{ -36.53f, -29.21f, 54.52f }, { -36.58f, -28.96f, 54.6f }, { -37.12f, -29.0f, 55.54f },
	{ -35.37f, -28.96f, 53.5f }, { -35.73f, -28.76f, 53.13f }, { -36.41f, -29.01f, 54.32f },
	{ -36.29f, -28.64f, 54.11f }, { -35.28f, -29.08f, 53.35f }
} ;

/* the model of each frame, and where it stands; they all start at one turn, in degrees */
static Uint8 rank_frame_obj_tbl[3] = { 4, 5, 6 } ;
static VEC3 rank_frame_pos_tbl[3] = {
	{ -34.21f, -28.63f, 42.29f },
	{ -35.48f, -29.12f, 48.82f },
	{ -35.74f, -28.65f, 54.17f },
} ;
static VEC3 rank_frame_rot = { 32.1f, 53.88f, 37.84f } ;

static void rank_frame_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void rank_frame_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = no ;
		ew->work_func = rank_frame_main ;
		ew->obj = obj_dm06->obj[rank_frame_obj_tbl[no]] ;
		ew->pos = rank_frame_pos_tbl[no] ;
		ew->rot[0] = ( int )( rank_frame_rot.x * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[1] = ( int )( rank_frame_rot.y * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->rot[2] = ( int )( rank_frame_rot.z * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->scale = rank_frame_scale ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_ROT_Z | OBJ_SCALE | OBJ_FADE | OBJ_BRIGHT ;
		ew->fade.r = 1.0f ;
		ew->fade.g = 1.0f ;
		ew->fade.b = 1.0f ;
		if( no == 2 ){
			ew->pos = rank_frame_pos3_tbl[Sys->rank_char] ;
			if( Sys->rank_char == 58 ){
				ew->scale.x = 0.75f ;
				ew->scale.y = 0.75f ;
				ew->scale.z = 0.75f ;
			}
		}
	}
}

/*==============================================================*/
static void rank_frame_main( PLAYER_WORK *ew )
{
	if( Sys->scene_no >= 3 ){
		if( ew->fade.r >= 0.5f ){
			ew->fade.r -= 0.01f ;
			ew->fade.g = ew->fade.r ;
			ew->fade.b = ew->fade.r ;
		}
	} else {
		if( Sys->join_flag ) work_free( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "rank_frame.c" End of File	*/
