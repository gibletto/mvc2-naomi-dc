/* The sparks of a player's dash (kind 7 works, made by dash_ef_set): one work watches the dash and,
   every eight frames while fewer than four sparks live, throws a spark at a random place on the
   player's body, which swells, thins and fades. */
#include "player.h"

/* a spark's grey, a value per frame */
static const Float dash_fade_tbl[10] = {
	0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.66f, 0.33f, 0.0f,
} ;

static void dash_spark_move( PLAYER_WORK *ew );
static void dash_hide( PLAYER_WORK *ew );
static void dash_end( PLAYER_WORK *ew );

/*==============================================================*/
static void dash_spark_main( PLAYER_WORK *ew )
{
	static void ( *dash_spark_jmp[] )( PLAYER_WORK *ew ) = {
		dash_spark_move, dash_hide, dash_end,
	} ;

	dash_spark_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void dash_spark_move( PLAYER_WORK *ew )
{
	const Float *fade = dash_fade_tbl ;

	fade += ew->timer0 ;
	ew->fade.r = *fade ;
	ew->fade.g = *fade ;
	ew->fade.b = *fade ;
	ew->scale.x += 0.029999996f ;
	ew->scale.y -= 0.08f ;
	if( ++ ew->timer0 >= 10 ){
		ew->routine_0 = 1 ;
		ew->ef_w->work1 -- ;
	}
}

/* a spark's own work, from obj_flag: w and h are the body's size */
typedef struct DASH_SPARK_W {
	Sint32	obj_flag;	/* 000 */
	Sint32	w;		/* 004 */
	Sint32	h;		/* 008 */
	Sint32	rnd;		/* 00c: a random number */
} DASH_SPARK_W ;
#define DASH_SPARK_LW( w )	( ( DASH_SPARK_W * )&( w )->obj_flag )

/*==============================================================*/
/*			A spark at a random place on the player's body		*/
/*==============================================================*/
static void dash_spark_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *spark, *pl_w ;
	DASH_SPARK_W *lw ;
	Sint32 *p ;

	if( ( spark = work_get( 0, W_EF, LIST_END ) ) ){
		pl_w = ew->master_w ;
		spark->disp_flag = 1 ;
		spark->work_func = dash_spark_main ;
		spark->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		spark->fade.r = 0.0f ;
		spark->fade.g = 0.0f ;
		spark->fade.b = 0.0f ;
		spark->obj = obj_efky->obj[22] ;
		spark->work1 = ( int )( pl_w->size_front * pl_w->scale.x * 1.6666666f ) ;
		spark->work2 = ( int )( pl_w->size_top * pl_w->scale.y * 2.142857f ) ;
		spark->work3 = Rnd() ;
		lw = DASH_SPARK_LW( spark ) ;
		lw->rnd %= lw->w ;
		lw = DASH_SPARK_LW( spark ) ;
		p = &lw->rnd ;
		*p = pl_w->disp_dir ? -*p : *p ;
		spark->spd.x = spark->work3 ;
		spark->work3 = Rnd() ;
		lw = DASH_SPARK_LW( spark ) ;
		lw->rnd %= lw->h ;
		spark->spd.y = spark->work3 ;
		spark->pos.x = pl_w->pos.x + spark->spd.x ;
		spark->pos.y = pl_w->pos.y + spark->spd.y ;
		spark->master_w = pl_w ;
		spark->ef_w = ew ;
		spark->disp_dir = pl_w->disp_dir ;
		spark->scale.x = 1.0f ;
		spark->scale.y = 1.0f ;
		ew->work1 ++ ;
	}
}

static void dash_ef_move( PLAYER_WORK *ew );

/*==============================================================*/
static void dash_ef_main( PLAYER_WORK *ew )
{
	static void ( *dash_ef_jmp[] )( PLAYER_WORK *ew ) = {
		dash_ef_move, dash_hide, dash_end,
	} ;

	dash_ef_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void dash_ef_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 || pl_w->act_st != N_DASH ){
		if( !ew->work1 ) ew->routine_0 = 1 ;
		return ;
	}
	if( ew->timer1 ){
		ew->timer1 -- ;
		if( ew->work1 < 4 ){
			if( !-- ew->timer0 ){
				ew->timer0 = 8 ;
				dash_spark_set( ew ) ;
			}
		}
	}
}

/*==============================================================*/
static void dash_ef_init( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->work_func = dash_ef_main ;
		ew->master_w = pl_w ;
		ew->timer0 = 8 ;
		ew->timer1 = 120 ;
	}
}

/*--------------------------------------------------------------*/
static void dash_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void dash_end( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/****************************************************************/
/*			The sparks of a dash								*/
/****************************************************************/
void dash_ef_set( PLAYER_WORK *pl_w )
{
	dash_ef_init( pl_w ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "dash_ef.c" End of File	*/
