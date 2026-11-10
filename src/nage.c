/* The sparks where a throw takes hold (`nage_hit_set`, `nage_seikou`). */
#include "player.h"

extern void move_pos_spd( PLAYER_WORK *ew );

void nage_seikou( VEC3 *pos );
static void nage_spark_set( VEC3 *pos, int no );

/****************************************************************/
void nage_hit_set( PLAYER_WORK *pl_w, VEC3 *ofs )
{
	VEC3 pos ;

	if( pl_w->disp_dir ) pos.x = pl_w->pos.x - ofs->x ;
	else pos.x = pl_w->pos.x + ofs->x ;
	pos.y = pl_w->pos.y + ofs->y ;
	pos.z = pl_w->pos.z ;
	nage_seikou( &pos ) ;
	nage_se_req( pl_w ) ;
}

/****************************************************************/
void nage_seikou( VEC3 *pos )
{
	int i, j ;

	for( j = 0 ; j < 5 ; j ++ ){
		for( i = 0 ; i < 3 ; i ++ ) nage_spark_set( pos, i ) ;
	}
}

static void nage_spark_main( PLAYER_WORK *ew );

/*==============================================================*/
static void nage_spark_set( VEC3 *pos, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = nage_spark_main ;
		ew->obj = obj_efky->obj[121 + no] ;
		ew->pos = *pos ;
		ew->spd.x = nlRand() % 30 - 15 ;
		ew->spd.y = nlRand() % 30 - 15 ;
		ew->spd.z = nlRand() % 30 - 15 ;
		ew->acc.x = -ew->spd.x / 30.0f ;
		ew->acc.y = -ew->spd.y / 30.0f ;
		ew->acc.z = -ew->spd.z / 30.0f ;
		ew->fade.trnsl = 1.0f ;
		ew->rot[2] = nlRand() % 0x10000 ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_X | OBJ_ROT_Y | OBJ_TRNSL ;
	}
}

/*==============================================================*/
static void nage_spark_main( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	if( ew->timer0 > 30 ){
		work_free( ew ) ;
		return ;
	}
	move_pos_spd( ew ) ;
	ew->rot[0] += ew->rot[2] ;
	ew->rot[1] += ew->rot[2] / 2 ;
	ew->fade.trnsl -= 0.016666668f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "nage.c" End of File	*/
