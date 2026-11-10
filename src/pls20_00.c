/* Felicia's player effect 00: five kinds of work. Type 0 and 1 are copies of her sprite that follow
   her animation, type 2 is the stream of sparks round the player she holds (it makes type 3, small
   hits that stay on it) and type 4 is the piece that drops from the top of the screen. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl20_def.h"

/* the effect's own work, from obj_flag */
typedef struct PLS20_00_LOCAL_W {
	Uint16	char_no;	/* 000: her animation when the effect began */
	Sint8	cg_prm_old;		/* 002: the animation frame last shown (type 1) */
} PLS20_00_LOCAL_W;
#define PLS20_00_LW( w )	( ( PLS20_00_LOCAL_W * )&( w )->obj_flag )

static PLS20_00_LOCAL_W *pls20_00_lw_p ;	/* the running work's, set by the main routine */
static PL20_LOCAL_W *pls20_00_pl_lw_p ;		/* the master's tlocal */

/* where the sparks of type 2 sit (by the held player's character): the offset's number */
static Uint8 pls20_00_ofs_no_tbl[60] = {
	5, 5, 5, 2, 2, 0, 5, 5, 2, 0, 5, 5, 5, 1, 0, 4,
	2, 5, 3, 3, 5, 5, 0, 5, 0, 0, 0, 5, 3, 3, 5, 5,
	0, 2, 2, 5, 5, 5, 2, 5, 5, 4, 5, 2, 0, 4, 5, 5,
	5, 2, 0, 5, 3, 4, 2, 3, 5, 5, 3, 0,
} ;

/* and the offsets across and up */
static Float pls20_00_ofs_x_tbl[] = {
	10.0f, -DOT_X( 20.0f ), DOT_X( 20.0f ), 0.0f, DOT_X( 32.0f ), 20.0f,
} ;

static Float pls20_00_ofs_y_tbl[] = {
	120.0f, DOT_Y( 65.0f ), 135.0f, 105.0f, DOT_Y( 72.0f ), DOT_Y( 47.0f ), DOT_Y( 59.0f ), DOT_Y( 50.0f ),
	DOT_Y( 52.0f ), DOT_Y( 47.0f ), DOT_Y( 54.0f ), DOT_Y( 62.0f ), DOT_Y( 43.0f ), 150.0f, DOT_Y( 64.0f ), DOT_Y( 75.0f ),
	DOT_Y( 54.0f ), DOT_Y( 55.0f ), DOT_Y( 54.0f ), 135.0f, DOT_Y( 48.0f ), DOT_Y( 59.0f ), DOT_Y( 46.0f ), DOT_Y( 69.0f ),
	DOT_Y( 48.0f ), DOT_Y( 48.0f ), DOT_Y( 48.0f ), DOT_Y( 50.0f ), DOT_Y( 39.0f ), DOT_Y( 34.0f ), 120.0f, DOT_Y( 48.0f ),
	DOT_Y( 47.0f ), DOT_Y( 57.0f ), DOT_Y( 52.0f ), 120.0f, DOT_Y( 55.0f ), DOT_Y( 60.0f ), DOT_Y( 57.0f ), 120.0f,
	DOT_Y( 59.0f ), DOT_Y( 75.0f ), DOT_Y( 67.0f ), DOT_Y( 73.0f ), 150.0f, DOT_Y( 57.0f ), 120.0f, DOT_Y( 60.0f ),
	DOT_Y( 54.0f ), DOT_Y( 53.0f ), DOT_Y( 64.0f ), 120.0f, DOT_Y( 83.0f ), DOT_Y( 68.0f ), DOT_Y( 73.0f ), DOT_Y( 51.0f ),
	120.0f, DOT_Y( 50.0f ), 30.0f,
} ;

/* the frames each spark of type 2 lasts, -1 at the end */
static Sint16 pls20_00_time_tbl[] = { 16, 19, 13, 23, 30, -1 } ;

/* where type 3 sits on the type 2 it rides */
typedef struct PLS20_00_OFS {
	Float	x, y;
} PLS20_00_OFS;

static PLS20_00_OFS pls20_00_ofs_tbl[] = {
	{ -75.0f, -75.0f }, { DOT_X( 56.0f ), DOT_Y( 23.0f ) }, { -70.0f, DOT_Y( 32.0f ) }, { 40.0f, -60.0f },
	{ -40.0f, 0.0f },
} ;

static void pls20_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls20_00_set( PLAYER_WORK *pl_w, Uint8 no, Uint8 unk )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls20_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = unk ;
		ew->ef_no = 0x2000 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The piece of type 3: it rides on the type 2 work	*/
/*			(ef_w)												*/
/*==============================================================*/
static PLAYER_WORK *pls20_00_help_part_set( PLAYER_WORK *pl_w, Uint8 no, Uint8 unk )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls20_00_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = no ;
		ew->sub_type = unk ;
		ew->ef_no = 0x2000 ;
	}
	return ew ;
}

static void pls20_00_rolling( PLAYER_WORK *ew );
static void pls20_00_spike( PLAYER_WORK *ew );
static void pls20_00_help( PLAYER_WORK *ew );
static void pls20_00_help_part( PLAYER_WORK *ew );
static void pls20_00_win( PLAYER_WORK *ew );

/*==============================================================*/
static void pls20_00_main( PLAYER_WORK *ew )
{
	static void ( *pls20_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_rolling, pls20_00_spike, pls20_00_help, pls20_00_help_part, pls20_00_win,
	} ;

	pls20_00_pl_lw_p = ( PL20_LOCAL_W * )&ew->master_w->tlocal[0] ;
	pls20_00_lw_p = PLS20_00_LW( ew ) ;
	pls20_00_jmp[ew->type]( ew ) ;
}

static void pls20_00_rolling_init( PLAYER_WORK *ew );
static void pls20_00_rolling_move( PLAYER_WORK *ew );
static void pls20_00_end( PLAYER_WORK *ew );
static void pls20_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls20_00_rolling( PLAYER_WORK *ew )
{
	static void ( *pls20_00_rolling_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_rolling_init, pls20_00_rolling_move, pls20_00_end, pls20_00_free,
	} ;

	pls20_00_rolling_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls20_00_rolling_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	pls20_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 0 ) ;
	pls20_00_rolling_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls20_00_rolling_move( PLAYER_WORK *ew )
{
	if( pls20_00_lw_p->char_no != ew->master_w->char_no.w ){
		pls20_00_end( ew ) ;
		return ;
	}
	if( ew->master_w->cg_type ){
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ef_char_move( ew ) ;
}

static void pls20_00_spike_init( PLAYER_WORK *ew );
static void pls20_00_spike_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls20_00_spike( PLAYER_WORK *ew )
{
	static void ( *pls20_00_spike_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_spike_init, pls20_00_spike_move, pls20_00_end, pls20_00_free,
	} ;

	pls20_00_spike_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls20_00_spike_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 0 ;
	pls20_00_lw_p->char_no = ew->master_w->char_no.w ;
	pls20_00_lw_p->cg_prm_old = 0 ;
	ef_char_set( ew, 27, 1 ) ;
	pls20_00_spike_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls20_00_spike_move( PLAYER_WORK *ew )
{
	if( pls20_00_lw_p->char_no != ew->master_w->char_no.w ){
		pls20_00_end( ew ) ;
		return ;
	}
	if( ew->master_w->cg_prm <= 0 ){
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( pls20_00_lw_p->cg_prm_old == ew->master_w->cg_prm ){
		ef_char_move( ew ) ;
		return ;
	}
	pls20_00_lw_p->cg_prm_old = ew->master_w->cg_prm ;
	ef_char_set( ew, 27, pls20_00_lw_p->cg_prm_old ) ;
}

static void pls20_00_help_init( PLAYER_WORK *ew );
static void pls20_00_help_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls20_00_help( PLAYER_WORK *ew )
{
	static void ( *pls20_00_help_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_help_init, pls20_00_help_move, pls20_00_end, pls20_00_free,
	} ;

	pls20_00_help_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls20_00_help_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *catch_w = ew->master_w->catch_w ;
	Uint8 no ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 10 ;
	ew->sub_prio = -1 ;
	no = pls20_00_ofs_no_tbl[catch_w->pl_type] ;
	ew->pos.x = ew->disp_dir ? pls20_00_ofs_x_tbl[no] : -pls20_00_ofs_x_tbl[no] ;
	ew->pos.x += catch_w->pos.x ;
	ew->pos.y = catch_w->pos.y + pls20_00_ofs_y_tbl[catch_w->pl_type] ;
	ew->timer1 = 0 ;
	ew->timer0 = pls20_00_time_tbl[ew->timer1] ;
	ew->dir = 8 ;
	ef_char_set( ew, 27, 8 ) ;
}

static void pls20_00_help_hit( PLAYER_WORK *ew );
static void pls20_00_help_wait( PLAYER_WORK *ew );
static void pls20_00_help_end( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls20_00_help_move( PLAYER_WORK *ew )
{
	static void ( *pls20_00_help_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_help_hit, pls20_00_help_wait, pls20_00_help_end,
	} ;

	pls20_00_help_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls20_00_help_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VEC3 ofs ;

	ef_char_move( ew ) ;
	if( !--ew->dir ){
		ew->dir = 4 ;
		ofs.x = ( float )( Rnd() & 127 ) + -63.0f ;
		ofs.x = DOT_X( ofs.x ) ;
		ofs.y = ( float )( Rnd() & 127 ) + -63.0f ;
		ofs.y = DOT_Y( ofs.y ) ;
		ofs.z = 0.0f ;
		hit_mark_set( ew, &ofs, 0 ) ;
	}
	if( !--ew->timer0 ){
		pls20_00_help_part_set( ew, 3, ew->timer1 ) ;
		ew->timer1 ++ ;
		if( ( ew->timer0 = pls20_00_time_tbl[ew->timer1] ) < 0 ){
			ew->timer1 = 0 ;
			ew->timer0 = pls20_00_time_tbl[ew->timer1] ;
		}
	}
	if( pl_w->team_no || pls20_00_pl_lw_p->help_phase >= 2 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void pls20_00_help_wait( PLAYER_WORK *ew )
{
	if( !--ew->timer0 ){
		pls20_00_help_part_set( ew, 3, ew->timer1 ) ;
		ew->timer1 ++ ;
		if( ( ew->timer0 = pls20_00_time_tbl[ew->timer1] ) < 0 ){
			ew->timer1 = 0 ;
			ew->timer0 = pls20_00_time_tbl[ew->timer1] ;
		}
	}
	if( ef_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ef_char_set( ew, 27, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls20_00_help_end( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls20_00_end( ew ) ;
}

static void pls20_00_help_part_init( PLAYER_WORK *ew );
static void pls20_00_help_part_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls20_00_help_part( PLAYER_WORK *ew )
{
	static void ( *pls20_00_help_part_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_help_part_init, pls20_00_help_part_move, pls20_00_end, pls20_00_free,
	} ;

	pls20_00_help_part_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls20_00_help_part_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 10 ;
	ew->sub_prio = -2 ;
	ew->pos.x = ew->ef_w->pos.x + pls20_00_ofs_tbl[ew->sub_type].x ;
	ew->pos.y = ew->ef_w->pos.y + pls20_00_ofs_tbl[ew->sub_type].y ;
	ef_char_set( ew, 27, 7 ) ;
}

/*--------------------------------------------------------------*/
static void pls20_00_help_part_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ) pls20_00_end( ew ) ;
}

static void pls20_00_win_init( PLAYER_WORK *ew );
static void pls20_00_win_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls20_00_win( PLAYER_WORK *ew )
{
	static void ( *pls20_00_win_jmp[] )( PLAYER_WORK *ew ) = {
		pls20_00_win_init, pls20_00_win_move, pls20_00_end, pls20_00_free,
	} ;

	pls20_00_win_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void pls20_00_win_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? 80.0f : -80.0f ) ;
	ew->pos.y = scr_work.eye[0].y + DOT_Y( 256.0f ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.y = -DOT_Y( 2.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, ew->sub_type + 62 ) ;
}

/*--------------------------------------------------------------*/
static void pls20_00_win_move( PLAYER_WORK *ew )
{
	if( !ew->routine_1 ){
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( ew->pos.y > ew->master_w->ground_y ) return ;
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_se_req( ew, 52 ) ;
		pl_char_set( ew, Char_Effect, ew->sub_type + 66 ) ;
	}else{
		pl_char_move( ew ) ;
		if( ew->master_w->act_st != N_WIN ) pls20_00_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls20_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	pls20_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls20_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/* nothing reads it: four heights, 160 apart from 80 up */
static Float pls20_00_unk_tbl[] = { 80.0f, 240.0f, 400.0f, 560.0f } ;

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls20_00.c" End of File	*/
