/* Servbot's player effect 02: small pieces by type, each following him or flying off (type 0
   the piece left where he came in, which smokes and bursts; 2 a puff of smoke that drifts up;
   3 a piece that rides his frame in the tackle; 4 the smoke he sheds while his vital is low). */
#include "player.h"
#include "ef_char.h"

static void pls3a_02_main( PLAYER_WORK *ew );

/****************************************************************/
/*			No: type in the low byte, then sub_type				*/
/****************************************************************/
PLAYER_WORK *pls3a_02_set( PLAYER_WORK *pl_w, Uint32 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls3a_02_main ;
		ew->master_w = pl_w ;
		ew->obj_flag = pl_w->char_no.w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3a02 ;
		ew->type = no ;
		ew->sub_type = no >> 8 ;
	}
	return ew ;
}

static void pls3a_02_appear_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_t1_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_smoke_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_tackle_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_low_vital_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_02_main( PLAYER_WORK *ew )
{
	static void ( *pls3a_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_02_appear_main, pls3a_02_t1_main, pls3a_02_smoke_main, pls3a_02_tackle_main, pls3a_02_low_vital_main,
	} ;

	pls3a_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void pls3a_02_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_02_appear_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_02_appear_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_02_appear_init, pls3a_02_appear_move, pls3a_02_free, pls3a_02_end,
	} ;

	pls3a_02_appear_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls3a_02_appear_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->prio = 8 ;
	ew->pos = pl_w->pos ;
	ew->timer0 = 180 ;
	ew->timer1 = 130 ;
	ew->sub_no = 0 ;
	ef_char_set( ew, 27, 13 ) ;
	ew->be_flag = 1 ;
	ew->cg_shadow = 0 ;
	pl_shadow_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_02_appear_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	PLAYER_WORK *sp_w ;
	Uint32 i ;

	if( ew->timer1 && --ew->timer1 <= 0 ){
		ew->timer1 = 0 ;
		pos = ew->pos ;
		pos.y += DOT_Y( 8.0f ) ;
		burst_set( ew, &pos, 1, 8 ) ;
		for( i = 0 ; i < 0x800 ; i += 0x100 ) shl3a_04_set( ew, i | 0x80000001 ) ;
		pl_se_req( ew, 73 ) ;
	}
	if( ew->timer0 < 60 ){
		ew->disp_flag = 1 ;
		if( ew->timer0 & 1 ) ew->disp_flag = 0 ;
	} else {
		if( !( ew->sub_no = ew->sub_no + 32 ) ){
			if( ( sp_w = pls3a_02_set( pl_w, 2 ) ) ){
				pos.x = DOT_X( 20.0f ) ;
				if( ew->disp_dir ) pos.x = -pos.x ;
				sp_w->pos.x = ew->pos.x + pos.x ;
				sp_w->pos.y = DOT_Y( 8.0f ) ;
			}
		}
	}
	if( --ew->timer0 <= 0 ) pls3a_02_free( ew, pl_w ) ;
}

/*==============================================================*/
/*			Nothing for type 1									*/
/*==============================================================*/
static void pls3a_02_t1_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

static void pls3a_02_smoke_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_smoke_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_02_smoke_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_02_smoke_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_02_smoke_init, pls3a_02_smoke_move, pls3a_02_free, pls3a_02_end,
	} ;

	pls3a_02_smoke_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls3a_02_smoke_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 d ;

	ew->routine_0 ++ ;
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
	ew->prio = 15 ;
	d = ( Rnd() & 0xff ) - 128 ;
	ew->spd.x = d * 1.6666666f / 256.0f ;
	ew->acc.x = 0 ;
	ew->spd.y = 2.142857f ;
	ew->acc.y = -0.06696428f ;
	ef_char_set( ew, 27, 14 ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_02_smoke_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ef_char_move( ew ) < 0 ) pls3a_02_free( ew, pl_w ) ;
}

static void pls3a_02_tackle_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_tackle_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_02_tackle_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_02_tackle_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_02_tackle_init, pls3a_02_tackle_move, pls3a_02_free, pls3a_02_end,
	} ;

	pls3a_02_tackle_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls3a_02_tackle_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->sub_prio = -1 ;
	ef_char_set( ew, 27, 9 ) ;
	pls3a_02_tackle_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_02_tackle_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x ;

	ew->prio = pl_w->prio ;
	x = DOT_X( pl_w->cg_type ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( pl_w->cg_prm ) ;
	ef_char_move( ew ) ;
	if( pl_w->char_no.w != ew->obj_flag ) pls3a_02_free( ew, pl_w ) ;
}

static void pls3a_02_low_vital_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls3a_02_low_vital_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls3a_02_low_vital_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *pls3a_02_low_vital_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls3a_02_low_vital_init, pls3a_02_low_vital_move, pls3a_02_free, pls3a_02_end,
	} ;

	pls3a_02_low_vital_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls3a_02_low_vital_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls3a_02_low_vital_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			The vital below which a piece may come off, by		*/
/*			sixteenth of the frame count						*/
/*==============================================================*/
static void pls3a_02_low_vital_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 pls3a_02_vital_tbl[8] = { 32, 8, 16, 8, 24, 8, 16, 8 } ;
	PLAYER_WORK *sp_w ;
	VEC3 ofs ;
	int t ;

	if( pl_w->vital_new >= 32 ){
		pl_w->work1 = 0 ;
		pls3a_02_free( ew, pl_w ) ;
		return ;
	}
	t = ( Sys->game_timer0 + pl_w->work_no ) & 127 ;
	if( !( t & 15 ) ){
		t >>= 4 ;
		if( pl_w->vital_new < pls3a_02_vital_tbl[t] ){
			if( !pl_w->act_st ){
				if( ( sp_w = pls3a_02_set( pl_w, 2 ) ) ){
					ofs.x = ( ( Rnd() & 15 ) - 8 ) * 1.6666666f ;
					if( ew->disp_dir ) ofs.x = -ofs.x ;
					sp_w->pos.x = pl_w->pos.x + ofs.x ;
					sp_w->pos.y = pl_w->pos.y + DOT_Y( 32.0f ) ;
				}
			}
		}
	}
	if( !pl_w->ko_flag ) return ;
	pls3a_02_free( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void pls3a_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 3 ;
	ew->be_flag = 0 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls3a_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls3a_02.c" End of File	*/
