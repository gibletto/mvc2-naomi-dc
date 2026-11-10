/* The shock of a player hitting the ground after a fall (kinds 7 and 5): four rings
   that swell by keyed steps, a ring of ten sparks that fly out and fade, and four lights that
   circle the player. */
#include "player.h"

extern void move_pos_spd( PLAYER_WORK *ew );

static VEC3 shock_scale = { 1.0f, 1.0f, 1.0f } ;

static MOTION shock_a1_key[] = {
	{ 0, 1.0f }, { 4, 1.0f }, { 15, 0.0f },
} ;
static MOTION shock_b1_key[] = {
	{ 0, 0.0f }, { 5, 4.0f }, { 15, 4.0f },
} ;
static MOTION shock_c1_key[] = {
	{ 0, 1.0f }, { 4, 1.0f }, { 15, 0.0f },
} ;
static MOTION shock_a2_key[] = {
	{ 0, 1.0f }, { 15, 1.0f },
} ;
static MOTION shock_b2_key[] = {
	{ 0, 0.0f }, { 4, 2.5f }, { 8, 2.0f }, { 15, 0.0f },
} ;
static MOTION shock_c2_key[] = {
	{ 0, 1.0f }, { 15, 1.0f },
} ;
static MOTION shock_a3_key[] = {
	{ 0, 1.0f }, { 15, 3.0f },
} ;
static MOTION shock_b3_key[] = {
	{ 0, 1.0f }, { 15, 1.0f },
} ;
static MOTION shock_c3_key[] = {
	{ 0, 1.0f }, { 15, 3.0f },
} ;
static MOTION shock_a4_key[] = {
	{ 0, 1.0f }, { 15, 1.5f },
} ;
static MOTION shock_b4_key[] = {
	{ 0, 0.0f }, { 4, 1.0f }, { 6, 1.5f }, { 8, 1.2f }, { 15, 0.0f },
} ;
static MOTION shock_c4_key[] = {
	{ 0, 1.0f }, { 15, 1.5f },
} ;

/*==============================================================*/
static void shock1_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 >= shock_a1_key[ew->routine_0 + 1].frame ) ew->routine_0 ++ ;
	if( ew->timer0 >= shock_b1_key[ew->routine_1 + 1].frame ) ew->routine_1 ++ ;
	if( ew->timer0 >= shock_c1_key[ew->routine_2 + 1].frame ) ew->routine_2 ++ ;
	ew->scale.x += ( shock_a1_key[ew->routine_0 + 1].val - shock_a1_key[ew->routine_0].val ) / ( shock_a1_key[ew->routine_0 + 1].frame - shock_a1_key[ew->routine_0].frame ) ;
	ew->scale.y += ( shock_b1_key[ew->routine_1 + 1].val - shock_b1_key[ew->routine_1].val ) / ( shock_b1_key[ew->routine_1 + 1].frame - shock_b1_key[ew->routine_1].frame ) ;
	ew->scale.z += ( shock_c1_key[ew->routine_2 + 1].val - shock_c1_key[ew->routine_2].val ) / ( shock_c1_key[ew->routine_2 + 1].frame - shock_c1_key[ew->routine_2].frame ) ;
}

/*==============================================================*/
static void shock1_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = shock1_main ;
		ew->obj = obj_efky->obj[66] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = *pos ;
		ew->scale = shock_scale ;
		ew->fade.trnsl = 0.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void shock2_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 >= shock_a2_key[ew->routine_0 + 1].frame ) ew->routine_0 ++ ;
	if( ew->timer0 >= shock_b2_key[ew->routine_1 + 1].frame ) ew->routine_1 ++ ;
	if( ew->timer0 >= shock_c2_key[ew->routine_2 + 1].frame ) ew->routine_2 ++ ;
	ew->scale.x += ( shock_a2_key[ew->routine_0 + 1].val - shock_a2_key[ew->routine_0].val ) / ( shock_a2_key[ew->routine_0 + 1].frame - shock_a2_key[ew->routine_0].frame ) ;
	ew->scale.y += ( shock_b2_key[ew->routine_1 + 1].val - shock_b2_key[ew->routine_1].val ) / ( shock_b2_key[ew->routine_1 + 1].frame - shock_b2_key[ew->routine_1].frame ) ;
	ew->scale.z += ( shock_c2_key[ew->routine_2 + 1].val - shock_c2_key[ew->routine_2].val ) / ( shock_c2_key[ew->routine_2 + 1].frame - shock_c2_key[ew->routine_2].frame ) ;
}

/*==============================================================*/
static void shock2_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = shock2_main ;
		ew->obj = obj_efky->obj[67] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = *pos ;
		ew->scale = shock_scale ;
		ew->fade.trnsl = 0.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void shock3_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 >= shock_a3_key[ew->routine_0 + 1].frame ) ew->routine_0 ++ ;
	if( ew->timer0 >= shock_b3_key[ew->routine_1 + 1].frame ) ew->routine_1 ++ ;
	if( ew->timer0 >= shock_c3_key[ew->routine_2 + 1].frame ) ew->routine_2 ++ ;
	ew->scale.x += ( shock_a3_key[ew->routine_0 + 1].val - shock_a3_key[ew->routine_0].val ) / ( shock_a3_key[ew->routine_0 + 1].frame - shock_a3_key[ew->routine_0].frame ) ;
	ew->scale.y += ( shock_b3_key[ew->routine_1 + 1].val - shock_b3_key[ew->routine_1].val ) / ( shock_b3_key[ew->routine_1 + 1].frame - shock_b3_key[ew->routine_1].frame ) ;
	ew->scale.z += ( shock_c3_key[ew->routine_2 + 1].val - shock_c3_key[ew->routine_2].val ) / ( shock_c3_key[ew->routine_2 + 1].frame - shock_c3_key[ew->routine_2].frame ) ;
}

/*==============================================================*/
static void shock3_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = shock3_main ;
		ew->obj = obj_efky->obj[68] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = *pos ;
		ew->scale = shock_scale ;
		ew->fade.trnsl = 0.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void shock4_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 15 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->timer0 >= shock_a4_key[ew->routine_0 + 1].frame ) ew->routine_0 ++ ;
	if( ew->timer0 >= shock_b4_key[ew->routine_1 + 1].frame ) ew->routine_1 ++ ;
	if( ew->timer0 >= shock_c4_key[ew->routine_2 + 1].frame ) ew->routine_2 ++ ;
	ew->scale.x += ( shock_a4_key[ew->routine_0 + 1].val - shock_a4_key[ew->routine_0].val ) / ( shock_a4_key[ew->routine_0 + 1].frame - shock_a4_key[ew->routine_0].frame ) ;
	ew->scale.y += ( shock_b4_key[ew->routine_1 + 1].val - shock_b4_key[ew->routine_1].val ) / ( shock_b4_key[ew->routine_1 + 1].frame - shock_b4_key[ew->routine_1].frame ) ;
	ew->scale.z += ( shock_c4_key[ew->routine_2 + 1].val - shock_c4_key[ew->routine_2].val ) / ( shock_c4_key[ew->routine_2 + 1].frame - shock_c4_key[ew->routine_2].frame ) ;
}

/*==============================================================*/
static void shock4_set( VEC3 *pos )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = shock4_main ;
		ew->obj = obj_efky->obj[69] ;
		ew->obj_flag = OBJ_POS | OBJ_SCALE ;
		ew->pos = *pos ;
		ew->scale = shock_scale ;
		ew->fade.trnsl = 0.0f ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 0.0f ;
	}
}

/*==============================================================*/
static void shock_spark_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 30 ){
		work_free( ew ) ;
		return ;
	}
	move_pos_spd( ew ) ;
	ew->fade.trnsl -= 0.033333335f ;
}

/*==============================================================*/
static void shock_spark_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = shock_spark_main ;
		ew->obj = obj_efky->obj[70] ;
		ew->obj_flag = OBJ_POS | OBJ_TRNSL ;
		ew->pos = *pos ;
		ew->scale = shock_scale ;
		ew->spd.x = nlSin( ( int )( no * 36 * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 10.0f ;
		ew->spd.y = nlRand() % 20 + 30 ;
		ew->spd.z = nlCos( ( int )( no * 36 * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 10.0f ;
		ew->acc.x = -ew->spd.x / 30.0f ;
		ew->acc.y = -ew->spd.y / 30.0f ;
		ew->acc.z = -ew->spd.z / 30.0f ;
		ew->fade.trnsl = 1.0f ;
	}
}

/*==============================================================*/
static void shock_sparks_set( VEC3 *pos )
{
	int i ;

	for( i = 0 ; i < 10 ; i ++ ) shock_spark_set( pos, i ) ;
}

/****************************************************************/
void shock_set( VEC3 *pos, int no )
{
	shock1_set( pos ) ;
	shock2_set( pos ) ;
	shock3_set( pos ) ;
	shock4_set( pos ) ;
	shock_sparks_set( pos ) ;
}

/*==============================================================*/
static void shock_orb_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( ++ ew->timer0 >= 40 ) ew->timer0 = 0 ;
	ew->pos = pl_w->pos ;
	ew->pos.x += nlSin( ( int )( ( ew->timer0 * 360 / 40 + ew->type * 90 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 50.0f ;
	ew->pos.y += 250.0f ;
	ew->pos.z += nlCos( ( int )( ( ew->timer0 * 360 / 40 + ew->type * 90 ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 50.0f ;
	ew->rot[1] += 0xccd ;
	if( ++ ew->timer1 > 300 ) work_free( ew ) ;
}

/*==============================================================*/
static void shock_orb_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = shock_orb_main ;
		ew->obj = obj_efky->obj[41] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y ;
		ew->ef_w = pl_w ;
		ew->type = no ;
	}
}

/****************************************************************/
void shock_orbs_set( PLAYER_WORK *pl_w )
{
	int i ;

	for( i = 0 ; i < 4 ; i ++ ) shock_orb_set( pl_w, i ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shock_ef.c" End of File	*/
