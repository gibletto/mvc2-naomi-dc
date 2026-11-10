/* Venom's player effect 04: what holds the opponent in his punch throw and when the Venom Web
   catches. Type 0 is the one that holds the opponent (it catches him, takes the button-mashing and
   lets him go in a burst of sparks), type 1 is a mark behind him that the punch throw sets with it,
   type 2 the eight sparks. */
#include "player.h"
#include "pl_char.h"

/* which of the two spark patterns each character shows */
static Uint8 pls0e_04_pat_tbl[64] = {
	0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1,
	1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0,
	0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0,
} ;

/* where the eight sparks go from the one held, for each pattern */
static Sint16 pls0e_04_ofs_0[16] = {
	0, 24, 5, 14, 0, 48, 0, 36, 0, 74, 0, 61, 20, 99, -8, 86,
} ;
static Sint16 pls0e_04_ofs_1[16] = {
	0, 36, 0, 22, -19, 64, 0, 50, -19, 92, 5, 78, 0, 120, 0, 106,
} ;
static Sint16 *pls0e_04_ofs_tbl[] = { pls0e_04_ofs_0, pls0e_04_ofs_1 } ;

static void pls0e_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls0e_04_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, int type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->ef_no = 0xe04 ;
		ew->type = type ;
		ew->work_func = pls0e_04_main ;
		ew->master_w = pl_w ;
		ew->ef_w = em ;
	}
	return ew ;
}

static void pls0e_04_grab( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_04_type1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_04_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_04_main( PLAYER_WORK *ew )
{
	static void ( *pls0e_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_04_grab, pls0e_04_type1, pls0e_04_spark,
	} ;

	pls0e_04_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls0e_04_grab_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_04_grab_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_04_grab( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_04_grab_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_04_grab_init, pls0e_04_grab_move, pls0e_04_free,
	} ;

	pls0e_04_grab_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_04_grab_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	pl_catch_pass( pl_w, ew ) ;
	em = ew->ef_w ;
	ew->routine_0 ++ ;
	ew->prio = 11 ;
	ew->timer0 = 40 ;
	ew->pos = em->pos ;
	pl_se_req( ew, 13 ) ;
	em->catch_type = 6 ;
	em->nml_catch_ng_timer = 1 ;
	em->spd.x = 0 ;
	em->spd.y = 0 ;
	em->acc.x = 0 ;
	em->acc.y = 0 ;
	ew->prio = em->prio ;
	ew->sub_prio = -8 ;
	ew->sub_no = pls0e_04_pat_tbl[em->pl_type] ;
	pl_char_set( ew, Char_Effect, ew->sub_no + 29 ) ;
}

static void pls0e_04_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *em );
static void pls0e_04_mash( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *em );

/*--------------------------------------------------------------*/
static void pls0e_04_grab_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_04_grab_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *em ) = {
		pls0e_04_hold, pls0e_04_mash,
	} ;
	PLAYER_WORK *em = pl_w->catch_w ;

	em->nml_catch_ng_timer = 1 ;
	ew->pos = em->pos ;
	ew->prio = em->prio ;
	ew->sub_prio = -8 ;
	pls0e_04_grab_move_jmp[ew->routine_1]( ew, pl_w, em ) ;
}

/*==============================================================*/
/*			Plays out the grab's animation, then the			*/
/*			opponent is held and has to mash					*/
/*==============================================================*/
static void pls0e_04_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		em->disp_flag = 0 ;
		pl_char_set( em, Char_Stand, 0 ) ;
		hold_renda_set( em, 10 ) ;
	}
}

static void pls0e_04_sparks_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void pls0e_04_mash( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *em )
{
	pl_char_move( ew ) ;
	if( em->hit_box_no && em->routine_1 == PL_DAMAGE ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		ew->timer0 = 0 ;
		em->disp_flag = 1 ;
		em->auto_guard_timer = 8 ;
		em->nml_catch_ng_timer = 10 ;
		em->disp_flag = 1 ;
	}
	else {
		em->disp_flag = 0 ;
		if( hold_renda_ck( em ) ){
			ew->cg_ctr = 1 ;
			ew->timer0 -= 7 ;
			pl_char_move( ew ) ;
			hold_renda_set( em, 10 ) ;
		}
		if( -- ew->timer0 >= 0 ) return ;
		em->auto_guard_timer = 8 ;
		em->nml_catch_ng_timer = 10 ;
		em->disp_flag = 1 ;
		pl_stand_ret( em ) ;
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		ew->timer0 = 0 ;
	}
	pl_voice_req( ew, 35 ) ;
	pls0e_04_sparks_set( ew, pl_w ) ;
}

/*==============================================================*/
/*			The sparks the hold ends in: four on each side		*/
/*==============================================================*/
static void pls0e_04_sparks_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *sw ;
	int i ;

	if( work_rest_cnt <= 8 ) return ;
	for( i = 0 ; i < 8 ; i ++ ){
		if( ( sw = work_get( 0, W_PLS, LIST_END ) ) ){
			sw->ef_no = 0xe04 ;
			sw->type = 2 ;
			sw->sub_type = ( ( i & 1 ) << 7 ) | ( i >> 1 ) ;
			sw->sub_no = ew->sub_no ;
			sw->work_func = pls0e_04_main ;
			sw->master_w = pl_w ;
			sw->ef_w = ew ;
		}
	}
}

static void pls0e_04_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_04_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_04_type1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_04_type1_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_04_init1, pls0e_04_move1, pls0e_04_free,
	} ;

	pls0e_04_type1_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_04_init1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs ;

	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->routine_0 ++ ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -12 ;
	ofs = -DOT_X( 44.0f ) ;
	if( pl_w->pl_dir ) ofs = DOT_X( 44.0f ) ;
	ew->pos.x = pl_w->pos.x + ofs ;
	ew->pos.y = pl_w->pos.y + 195.0f ;
	pl_char_set( ew, Char_Effect, 75 ) ;
}

/*--------------------------------------------------------------*/
static void pls0e_04_move1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

static void pls0e_04_spark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls0e_04_spark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls0e_04_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls0e_04_spark_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls0e_04_spark_init, pls0e_04_spark_move, pls0e_04_free,
	} ;

	pls0e_04_spark_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls0e_04_spark_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 *ofs ;
	int side ;

	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	pl_w = ew->ef_w ;
	ew->routine_0 ++ ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = pl_w->sub_prio - 4 ;
	side = ( ew->sub_type & 0x80 ) >> 5 ;
	ofs = pls0e_04_ofs_tbl[ew->sub_no] ;
	ofs += ( ew->sub_type & 127 ) * 4 + ( side >> 1 ) ;
	ew->pos.x = pl_w->pos.x + *ofs ++ * 1.6666666f ;
	side += 31 ;
	ew->pos.y = pl_w->pos.y + *ofs * 2.142857f ;
	pl_char_set( ew, Char_Effect, side ) ;
}

/*--------------------------------------------------------------*/
static void pls0e_04_spark_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void pls0e_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls0e_04.c" End of File	*/
