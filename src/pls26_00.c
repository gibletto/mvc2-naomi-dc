/* M. Bison's player effect 00: the afterimages and flashes that ride on him, by type (0 to 14);
   each shows an animation of group 27 while his own animation stays the one it began with. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl_color.h"

typedef struct PLS26_00_LOCAL_W {
	Uint16	char_no;	/* the master's animation when the effect began */
} PLS26_00_LOCAL_W;
#define PLS26_00_LW( w )	( ( PLS26_00_LOCAL_W * )&( w )->obj_flag )

static PLS26_00_LOCAL_W *pls26_00_lw_p ;	/* the running work's, set by the main routine */
static Uint8 unk_0C2FB430[4] ;

static void pls26_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *pls26_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls26_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2600 ;
	}
	return ew ;
}

/****************************************************************/
void *pls26_00_warp_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Uint8 i = 0 ;

	while( i < 4 ){
		if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
			ew->work_func = pls26_00_main ;
			ew->master_w = pl_w ;
			ew->type = 14 ;
			ew->sub_type = i ;
			ew->ef_no = 0x2600 ;
		} else break ;
		i ++ ;
	}
	return ew ;
}

/****************************************************************/
void *pls26_00_crusher_flash_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ){
		ew->work_func = pls26_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x2600 ;
	}
	return ew ;
}

static void pls26_00_init( PLAYER_WORK *ew );
static void pls26_00_move( PLAYER_WORK *ew );
static void pls26_00_end( PLAYER_WORK *ew );
static void pls26_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void pls26_00_main( PLAYER_WORK *ew )
{
	static void ( *pls26_00_jmp[] )( PLAYER_WORK *ew ) = {
		pls26_00_init, pls26_00_move, pls26_00_end, pls26_00_free,
	} ;

	pls26_00_lw_p = PLS26_00_LW( ew ) ;
	pls26_00_jmp[ew->routine_0]( ew ) ;
}

static void pls26_00_init_00( PLAYER_WORK *ew );
static void pls26_00_init_01( PLAYER_WORK *ew );
static void pls26_00_init_02( PLAYER_WORK *ew );
static void pls26_00_init_03( PLAYER_WORK *ew );
static void pls26_00_init_04( PLAYER_WORK *ew );
static void pls26_00_init_05( PLAYER_WORK *ew );
static void pls26_00_shot_init( PLAYER_WORK *ew );
static void pls26_00_dive_init( PLAYER_WORK *ew );
static void pls26_00_crusher_g_flash_init( PLAYER_WORK *ew );
static void pls26_00_crusher_g_init( PLAYER_WORK *ew );
static void pls26_00_crusher_j_flash_init( PLAYER_WORK *ew );
static void pls26_00_crusher_j_init( PLAYER_WORK *ew );
static void pls26_00_appear_init( PLAYER_WORK *ew );
static void pls26_00_warp_init( PLAYER_WORK *ew );

/*==============================================================*/
static void pls26_00_init( PLAYER_WORK *ew )
{
	static void ( *pls26_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		pls26_00_init_00, pls26_00_init_01, pls26_00_init_02, pls26_00_init_03,
		pls26_00_init_04, pls26_00_init_05, pls26_00_shot_init, pls26_00_dive_init,
		pls26_00_crusher_g_flash_init, pls26_00_crusher_g_init, pls26_00_crusher_j_flash_init, pls26_00_crusher_j_init,
		pls26_00_init_00, pls26_00_appear_init, pls26_00_warp_init,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

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
	pls26_00_init_jmp[ew->type]( ew ) ;
}

static void pls26_00_pos_set( PLAYER_WORK *ew );

/*==============================================================*/
static void pls26_00_init_00( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 0 ) ;
}

/*==============================================================*/
static void pls26_00_init_01( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 1 ) ;
}

/*==============================================================*/
static void pls26_00_init_02( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 2 ) ;
}

/*==============================================================*/
static void pls26_00_init_03( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 3 ) ;
}

/*==============================================================*/
static void pls26_00_init_04( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 4 ) ;
}

/*==============================================================*/
static void pls26_00_init_05( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 5 ) ;
}

/*==============================================================*/
static void pls26_00_shot_init( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 6 ) ;
}

/*==============================================================*/
static void pls26_00_dive_init( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 7 ) ;
}

/*==============================================================*/
static void pls26_00_crusher_g_flash_init( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 8 ) ;
}

/*==============================================================*/
static void pls26_00_crusher_g_init( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	ew->prio = 7 ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 11 ) ;
}

/*==============================================================*/
static void pls26_00_crusher_j_flash_init( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 12 ) ;
}

/*==============================================================*/
static void pls26_00_crusher_j_init( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	ew->prio = 7 ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 14 ) ;
}

/*==============================================================*/
static void pls26_00_appear_init( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->ground_y ;
	ew->prio = 0 ;
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ef_char_set( ew, 27, 16 ) ;
}

/*==============================================================*/
static void pls26_00_warp_init( PLAYER_WORK *ew )
{
	pls26_00_lw_p->char_no = ew->master_w->char_no.w ;
	ew->routine_1 = ew->routine_2 = 0 ;
	ew->disp_flag = 0 ;
	ew->col_flag = ew->master_w->col_flag ;
	if( ew->sub_type == ew->master_w->routine_3 ) ew->routine_0 = 3 ;
	ew->prio = ( ew->sub_type & 1 ) ? 0 : 0 ;	/* 0 either way */
}

static void pls26_00_nml_move( PLAYER_WORK *ew );
static void pls26_00_shot_move( PLAYER_WORK *ew );
static void pls26_00_dive_move( PLAYER_WORK *ew );
static void pls26_00_crusher_g_flash_move( PLAYER_WORK *ew );
static void pls26_00_crusher_move( PLAYER_WORK *ew );
static void pls26_00_crusher_j_flash_move( PLAYER_WORK *ew );
static void pls26_00_appear_move( PLAYER_WORK *ew );
static void pls26_00_warp_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void pls26_00_move( PLAYER_WORK *ew )
{
	static void ( *pls26_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls26_00_nml_move, pls26_00_nml_move, pls26_00_nml_move, pls26_00_nml_move,
		pls26_00_nml_move, pls26_00_nml_move, pls26_00_shot_move, pls26_00_dive_move,
		pls26_00_crusher_g_flash_move, pls26_00_crusher_move, pls26_00_crusher_j_flash_move, pls26_00_crusher_move,
		pls26_00_nml_move, pls26_00_appear_move, pls26_00_warp_move,
	} ;

	pls26_00_move_jmp[ew->type]( ew ) ;
}

static void pls26_00_color( PLAYER_WORK *ew );
static void pls26_00_color2( PLAYER_WORK *ew );
static void pls26_00_color3( PLAYER_WORK *ew );

/*==============================================================*/
static void pls26_00_nml_move( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	ew->disp_flag = 1 ;
	if( pls26_00_lw_p->char_no == ew->master_w->char_no.w ){
		if( HI_BYTE( ew->master_w->cg_rival ) ){
			HI_BYTE( ew->master_w->cg_rival ) = 0 ;
			if( ef_char_move( ew ) < 0 ){
				pl_color_set( ew->master_w, 0 ) ;
				pls26_00_end( ew ) ;
				return ;
			}
		}
		pls26_00_color( ew ) ;
		return ;
	}
	pl_color_set( ew->master_w, 0 ) ;
	pls26_00_end( ew ) ;
}

/*==============================================================*/
static void pls26_00_shot_move( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	ew->disp_flag = 1 ;
	if( pls26_00_lw_p->char_no == ew->master_w->char_no.w ){
		if( ef_char_move( ew ) >= 0 ){
			pls26_00_color( ew ) ;
			return ;
		}
	}
	pl_color_set( ew->master_w, 0 ) ;
	pls26_00_end( ew ) ;
}

/*==============================================================*/
static void pls26_00_dive_move( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	ew->disp_flag = 1 ;
	if( pls26_00_lw_p->char_no != ew->master_w->char_no.w ){
		pl_color_set( ew->master_w, 0 ) ;
		pls26_00_end( ew ) ;
		return ;
	}
	if( !ew->master_w->hit_stop ) ef_char_move( ew ) ;
	pls26_00_color( ew ) ;
}

/*==============================================================*/
static void pls26_00_crusher_move( PLAYER_WORK *ew )
{
	pls26_00_pos_set( ew ) ;
	ew->disp_flag = 1 ;
	ew->prio = 7 ;
	if( pls26_00_lw_p->char_no != ew->master_w->char_no.w ){
		pls26_00_end( ew ) ;
		return ;
	}
	if( HI_BYTE( ew->master_w->cg_rival ) ){
		HI_BYTE( ew->master_w->cg_rival ) = 0 ;
		ef_char_move( ew ) ;
	}
	pls26_00_color3( ew ) ;
	pls26_00_color( ew ) ;
}

/*==============================================================*/
static void pls26_00_crusher_g_flash_move( PLAYER_WORK *ew )
{
	static Uint8 pls26_00_tbl[] = { 8, 8, 9, 10 } ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( pls26_00_lw_p->char_no == ew->master_w->char_no.w ){
		if( HI_BYTE( ew->master_w->cg_rival ) ){
			ef_char_set( ew, 27, pls26_00_tbl[HI_BYTE( ew->master_w->cg_rival )] ) ;
			return ;
		}
	}
	pls26_00_color2( ew ) ;
	pls26_00_end( ew ) ;
}

/*==============================================================*/
static void pls26_00_crusher_j_flash_move( PLAYER_WORK *ew )
{
	static Uint8 pls26_00_tbl[] = { 12, 12, 13, 0 } ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( pls26_00_lw_p->char_no == ew->master_w->char_no.w ){
		if( HI_BYTE( ew->master_w->cg_rival ) ){
			pls26_00_color( ew ) ;
			ef_char_set( ew, 27, pls26_00_tbl[HI_BYTE( ew->master_w->cg_rival )] ) ;
			return ;
		}
	}
	pls26_00_color2( ew ) ;
	pls26_00_end( ew ) ;
}

/*==============================================================*/
static void pls26_00_appear_move( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	ew->disp_flag = 1 ;
	if( ef_char_move( ew ) < 0 ) pls26_00_end( ew ) ;
}

static void pls26_00_dir_set( PLAYER_WORK *ew );
static void pls26_00_warp_wait( PLAYER_WORK *ew );
static void pls26_00_warp_appear( PLAYER_WORK *ew );
static void pls26_00_warp_vanish( PLAYER_WORK *ew );

/*==============================================================*/
static void pls26_00_warp_move( PLAYER_WORK *ew )
{
	static void ( *pls26_00_warp_move_jmp[] )( PLAYER_WORK *ew ) = {
		pls26_00_warp_wait, pls26_00_warp_appear, pls26_00_warp_vanish,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 != 1 || pl_w->routine_1 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	pls26_00_warp_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void pls26_00_warp_wait( PLAYER_WORK *ew )
{
	static Float pls26_00_ofs_tbl[] = { DOT_Y( 96.0f ), DOT_Y( 96.0f ), 0.0f, 0.0f } ;
	Float dx ;

	if( ew->master_w->disp_flag ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		dx = fabs( scr_work.eye[0].x - ew->master_w->pos.x ) ;
		ew->pos.x = scr_work.eye[0].x + ( ( ew->sub_type & 1 ) ? dx : -dx ) ;
		ew->pos.y = ew->master_w->target_w->pos.y + pls26_00_ofs_tbl[ew->sub_type] ;
		pls26_00_dir_set( ew ) ;
		pl_char_set( ew, 21, 43 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls26_00_warp_appear( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 21, 42 ) ;
	}
}

/*--------------------------------------------------------------*/
static void pls26_00_warp_vanish( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->cg_type ) ew->cg_type = 0 ;
}

/*--------------------------------------------------------------*/
static void pls26_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	pls26_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void pls26_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void pls26_00_dir_set( PLAYER_WORK *ew )
{
	ew->disp_dir = ( ew->sub_type & 1 ) ? 0 : 1 ;
}

/*==============================================================*/
static void pls26_00_pos_set( PLAYER_WORK *ew )
{
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->prio = 0 ;
}

/*==============================================================*/
static void pls26_00_color( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( Sys->game_timer0 & 1 ) ef_color_set( pl_w, 1, pl_w->pl_color * 2, 1 ) ;
	else ef_color_set( pl_w, 1, pl_w->pl_color * 2 + 1, 1 ) ;
}

/*==============================================================*/
static void pls26_00_color2( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_color_set( pl_w, 1, pl_w->pl_color * 2, 1 ) ;
}

/*==============================================================*/
static void pls26_00_color3( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( Sys->game_timer0 & 1 ) ef_color_set( pl_w, 0, pl_w->pl_color * 2, 1 ) ;
	else ef_color_set( pl_w, 0, pl_w->pl_color * 2 + 1, 1 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls26_00.c" End of File	*/
