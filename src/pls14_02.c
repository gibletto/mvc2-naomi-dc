/* SonSon's player effect 02 (character 14's player effect 02): the six kinds of piece that play
   beside her special moves: a swelling aura, a palette flash, a puff at her side, a shake on the
   opponent she holds and a cloud that follows her. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl14_def.h"

/* the effect's own work: the animation she was in when it began */
typedef struct PLS14_02_LOCAL_W {
	CHAR_NO	char_no ;
} PLS14_02_LOCAL_W ;
#define PLS14_02_LW( w )	( ( PLS14_02_LOCAL_W * )&( w )->obj_flag )

static void pls14_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls14_02_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	PLS14_02_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls14_02_main ;
		ew->master_w = pl_w ;
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
		ew->ef_no = 0x1402 ;
		ew->size_top = ew->size_bottom = 32 ;
		ew->size_front = ew->size_back = 36 ;
		ew->type = type ;
		ew->routine_0 = 1 ;
		ew->routine_1 = no ;
		lw = PLS14_02_LW( ew ) ;
		lw->char_no.w = pl_w->char_no.w ;
		lw->char_no.b.grp = pl_w->char_no.b.grp ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	return ew ;
}

static void pls14_02_init( PLAYER_WORK *ew );
static void pls14_02_move( PLAYER_WORK *ew );
static void pls14_02_end( PLAYER_WORK *ew );
static void pls14_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_02_main( PLAYER_WORK *ew )
{
	static void ( *pls14_02_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_init, pls14_02_move, pls14_02_end, pls14_02_free,
	} ;

	pls14_02_jmp[ew->routine_0]( ew ) ;
}

static void pls14_02_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_02_init( PLAYER_WORK *ew )
{
}

static void pls14_02_move0( PLAYER_WORK *ew );
static void pls14_02_aura( PLAYER_WORK *ew );
static void pls14_02_col( PLAYER_WORK *ew );
static void pls14_02_puff( PLAYER_WORK *ew );
static void pls14_02_shake( PLAYER_WORK *ew );
static void pls14_02_rise( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_02_move( PLAYER_WORK *ew )
{
	static void ( *pls14_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_move0, pls14_02_aura, pls14_02_col, pls14_02_puff, pls14_02_shake,
		pls14_02_rise,
	} ;

	pls14_02_move_jmp[ew->routine_1]( ew ) ;
	pls14_02_debug( ew ) ;
}

/* kind 0: stays while she is in the animation */
static void pls14_02_move0_init( PLAYER_WORK *ew );
static void pls14_02_move0_move( PLAYER_WORK *ew );

/*==============================================================*/
static void pls14_02_move0( PLAYER_WORK *ew )
{
	static void ( *pls14_02_move0_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_move0_init, pls14_02_move0_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_02_LOCAL_W *lw ;

	lw = PLS14_02_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_02_move0_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_02_move0_init( PLAYER_WORK *ew )
{
	ew->routine_2 ++ ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->prio = 0 ;
}

/*--------------------------------------------------------------*/
static void pls14_02_move0_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
}

/* kind 1: the aura that swells and shrinks around her, in her hyper combo */
static void pls14_02_aura_init( PLAYER_WORK *ew );
static void pls14_02_aura_swell( PLAYER_WORK *ew );
static void pls14_02_aura_shrink( PLAYER_WORK *ew );
static void pls14_02_aura_end( PLAYER_WORK *ew );
static void pls14_02_aura_monkey( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_02_aura( PLAYER_WORK *ew )
{
	static void ( *pls14_02_aura_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_aura_init, pls14_02_aura_swell, pls14_02_aura_shrink, pls14_02_aura_end,
		pls14_02_aura_monkey,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_02_LOCAL_W *lw ;

	lw = PLS14_02_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	pls14_02_aura_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_02_aura_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	pl_char_set( ew, 22, 57 ) ;
	ew->prio = 7 ;
	ef_color_set( pl_w, 2, 12, 2 ) ;
	ew->alpha = 0.5f ;
	ew->scale.x = pl_w->scale.x + 0.3f ;
	ew->scale.y = pl_w->scale.y + 0.2f ;
	ew->timer0 = 6 ;
	ew->spd.x = 0.04f ;
	ew->acc.x = -0.004f ;
	ew->spd.y = 0.04f ;
	ew->acc.y = -0.004f ;
	ew->acc.z = -DOT_Y( 2.5f ) ;
}

/*--------------------------------------------------------------*/
static void pls14_02_aura_swell( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->pos.y += ew->acc.z ;
	ew->scale.x += ew->spd.x ;
	ew->scale.y += ew->spd.y ;
	if( pl_w->scale.y >= 1.0f ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( -- ew->timer0 <= 0 ){
		ew->timer0 = 3 ;
		ew->routine_2 = 2 ;
		ew->scale.x = pl_w->scale.x + 0.3f ;
		ew->scale.y = pl_w->scale.y + 0.2f ;
		ew->spd.x = -0.02f ;
		ew->acc.x = 0.002f ;
		ew->spd.y = -0.02f ;
		ew->acc.x = 0.002f ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_02_aura_shrink( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->scale.x += ew->spd.x ;
	ew->scale.y += ew->spd.y ;
	if( pl_w->scale.y >= 1.0f ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( -- ew->timer0 <= 0 ){
		ew->timer0 = 6 ;
		ew->routine_0 = 1 ;
		ew->scale.x = pl_w->scale.x + 0.3f ;
		ew->scale.y = pl_w->scale.y + 0.2f ;
		ew->spd.x = 0.04f ;
		ew->acc.x = -0.004f ;
		ew->spd.y = 0.04f ;
		ew->acc.y = -0.004f ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_02_aura_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 2 ;
}

/*--------------------------------------------------------------*/
static void pls14_02_aura_monkey( PLAYER_WORK *ew )
{
	PL14_LOCAL_W *lw ;

	lw = ( PL14_LOCAL_W * )ew->master_w->tlocal ;
	ew->scale.x = lw->monkey_scale_x + 0.3f ;
	ew->scale.y = lw->monkey_scale_y + 0.2f ;
	if( lw->monkey_scale_x >= 1.0f ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/* kind 2: lights her palette lines up while she is in the animation */
static void pls14_02_col_init( PLAYER_WORK *ew );
static void pls14_02_col_wait( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_02_col( PLAYER_WORK *ew )
{
	static void ( *pls14_02_col_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_col_init, pls14_02_col_wait,
	} ;

	pls14_02_col_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_02_col_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	COL_WORK *cw ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 0 ;
	cw = &col_work[pl_w->col_no] + 2 ;
	cw->trans_flag = 1 ;
	cw->col[1] |= COL_ALPHA ;
	cw->col[2] |= COL_ALPHA ;
	cw->col[3] |= COL_ALPHA ;
	cw->col[1] &= 0x9fff ;
	cw->col[2] &= 0x9fff ;
	cw->col[3] &= 0x9fff ;
}

/*--------------------------------------------------------------*/
static void pls14_02_col_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLS14_02_LOCAL_W *lw ;
	COL_WORK *cw ;

	lw = PLS14_02_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		switch( pl_w->char_no.b.grp ){
		case 7:
		case 8:
		case 11:
			break ;
		default:
			cw = &col_work[pl_w->col_no] + 2 ;
			cw->trans_flag = 1 ;
			cw->col[1] |= COL_ALPHA ;
			cw->col[2] |= COL_ALPHA ;
			cw->col[3] |= COL_ALPHA ;
			break ;
		}
	}
}

/* kind 3: the puff beside her feet, on the side she faces */
static void pls14_02_puff_init( PLAYER_WORK *ew );
static void pls14_02_puff_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_02_puff( PLAYER_WORK *ew )
{
	static void ( *pls14_02_puff_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_puff_init, pls14_02_puff_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_02_LOCAL_W *lw ;

	lw = PLS14_02_LW( ew ) ;
	pl_w = ew->master_w ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_02_puff_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void pls14_02_puff_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	ew->pos.x += pl_w->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	ew->pos.y += DOT_Y( 39.0f ) ;
	ef_char_set( ew, 27, 34 ) ;
	ew->scale.x = 0.7f ;
	ew->scale.y = 0.3f ;
	ew->prio = 7 ;
}

/*--------------------------------------------------------------*/
static void pls14_02_puff_move( PLAYER_WORK *ew )
{
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
}

/*==============================================================*/
/*			Kind 4: shakes the player she holds					*/
/*==============================================================*/
static void pls14_02_shake( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *catch_w ;
	Float ofs_y ;
	Float ofs_x ;

	ew->disp_flag = 0 ;
	pl_w = ew->master_w ;
	if( pl_w->cg_type == 2 ){
		ew->routine_0 = 2 ;
	} else if( pl_w->cg_type == 3 ){
		catch_w = pl_w->catch_w ;
		ofs_y = DOT_Y( 0.25f ) ;
		ofs_x = DOT_X( 0.25f ) ;
		switch( Rnd() & 3 ){
		case 0:
			catch_w->pos.x += ofs_x ;
			catch_w->pos.y += ofs_y ;
			break ;
		case 1:
			catch_w->pos.x -= ofs_x ;
			catch_w->pos.y -= ofs_y ;
			break ;
		case 2:
			catch_w->pos.x -= ofs_x ;
			catch_w->pos.y += ofs_y ;
			break ;
		case 3:
			catch_w->pos.x += ofs_x ;
			catch_w->pos.y -= ofs_y ;
			break ;
		}
	}
}

/* kind 5: a cloud that follows her and rises */
static void pls14_02_rise_init( PLAYER_WORK *ew );
static void pls14_02_rise_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls14_02_rise( PLAYER_WORK *ew )
{
	static void ( *pls14_02_rise_jmp[] )( PLAYER_WORK *ew ) = {
		pls14_02_rise_init, pls14_02_rise_move,
	} ;
	PLAYER_WORK *pl_w ;
	PLS14_02_LOCAL_W *lw ;

	pl_w = ew->master_w ;
	lw = PLS14_02_LW( ew ) ;
	if( lw->char_no.w != pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	pls14_02_rise_jmp[ew->routine_2]( ew ) ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
}

/*==============================================================*/
static void pls14_02_rise_init( PLAYER_WORK *ew )
{
	ew->routine_2 ++ ;
	ef_char_set( ew, 27, ew->type ) ;
	ew->disp_dir = 0 ;
	ew->pos.y += DOT_Y( 72.0f ) ;
}

/*--------------------------------------------------------------*/
static void pls14_02_rise_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_w = ew->master_w ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y += DOT_Y( 0.25f ) ;
	if( ef_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
	}
}

/*--------------------------------------------------------------*/
static void pls14_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void pls14_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void pls14_02_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls14_02.c" End of File	*/
