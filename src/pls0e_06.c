/* Venom's player effect 06: the sparks of his animations. Type 0, set as the Web Throw lets go,
   flies off and falls to the ground; types 1 and 2, set by the taunt and by the landing when he is
   tagged in, stay on him and flash again each time his animation passes the code. */
#include "player.h"
#include "pl_char.h"

/* the work takes the master's look */
#define PLS0E_06_LOOK( ew, pl_w ) \
	CHAR_PART_COPY( ew, pl_w ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( pl_w )->pl_no ; \
	( ew )->pl_type = ( pl_w )->pl_type ; \
	( ew )->scale.x = ( pl_w )->scale.x ; \
	( ew )->scale.y = ( pl_w )->scale.y ; \
	( ew )->atck_level = ( pl_w )->atck_level ; \
	( ew )->work_no = ( pl_w )->work_no ; \
	( ew )->col_cnt = ( pl_w )->col_cnt ; \
	( ew )->scale = ( pl_w )->scale

static void pls0e_06_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0e_06_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_END ) ) ){
		ew->ef_no = 0xe06 ;
		ew->type = no ;
		ew->work_func = pls0e_06_main ;
		ew->master_w = pl_w ;
	}
	return ew ;
}

static void pls0e_06_webthrow_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_06_spark_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_06_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_06_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_06_webthrow_main, pls0e_06_spark_main, pls0e_06_spark_main,
	} ;

	pls0e_06_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls0e_06_webthrow_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_06_webthrow_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_06_webthrow_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_06_webthrow_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_06_webthrow_init, pls0e_06_webthrow_move, pls0e_06_free,
	} ;

	pls0e_06_webthrow_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_06_webthrow_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	float x ;

	PLS0E_06_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->prio = pl_w->prio ;
	ew->spd.x = 20.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	x = 116 * 1.6666666f ;
	if( ew->disp_dir ){
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
		x = -116 * 1.6666666f ;
	}
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + 100 * 2.142857f ;
	pl_char_set( ew, Char_Effect, 44 ) ;
}

/*--------------------------------------------------------------*/
static void pls0e_06_webthrow_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) return ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y < pl_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
		}
	}
}

static void pls0e_06_spark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_06_spark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_06_spark_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_06_spark_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_06_spark_init, pls0e_06_spark_move, pls0e_06_free,
	} ;

	pls0e_06_spark_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_06_spark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLS0E_06_LOOK( ew, pl_w ) ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	ew->disp_dir = 0 ;
	pls0e_06_spark_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			A spark flashes each time the animation's code		*/
/*			has bit 0, and is gone at bit 1						*/
/*==============================================================*/
static void pls0e_06_spark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int anim ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Extra
	 || pl_w->char_no.b.no != 2 && pl_w->char_no.b.no != 1 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->sub_prio = -8 ;
	ew->pos = pl_w->pos ;
	if( pl_w->cg_type & 1 ){
		pl_w->cg_type ^= 1 ;
		ew->disp_flag = 1 ;
		anim = ( Rnd() & 1 ) * 2 + pl_w->pl_dir ;
		if( ew->type == 2 ) anim += 21 ;
		else anim += 25 ;
		pl_char_set( ew, Char_Effect, anim ) ;
	}
	if( !( pl_w->cg_type & 2 ) ) return ;
	ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls0e_06_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0e_06.c" End of File	*/
