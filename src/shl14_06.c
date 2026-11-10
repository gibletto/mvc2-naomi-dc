/* The pieces strung out from shl14_05's part (character 14's shot 06): five attacks, each a step
   further along the part's turn, while SonSon's hyper combo lasts. */
#include "player.h"
#include "pl_char.h"
#include "pl14_def.h"

/* the effect's own work */
typedef struct SHL14_06_LOCAL_W {
	CHAR_NO	char_no;	/* 000: SonSon's animation, kept up with her */
} SHL14_06_LOCAL_W;
#define SHL14_06_LW( w )	( ( SHL14_06_LOCAL_W * )EF_WK( w )->work )

const Sint16 shl14_06_ofs[5] = { -80, -160, -240, -320, -400 } ;	/* how far along the part each piece sits, by its number */

static void shl14_06_main( PLAYER_WORK *ew );
static void shl14_06_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew );

/****************************************************************/
void *shl14_06_set( PLAYER_WORK *pl_w, PLAYER_WORK *hw, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_06_main ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1406 ;
		ew->master_w = pl_w ;
		ew->ef_w = hw ;
		SHL14_06_LW( ew )->char_no.w = pl_w->char_no.w ;
		SHL14_06_LW( ew )->char_no.b.grp = pl_w->char_no.b.grp ;
		SHL14_06_LW( ew )->char_no.b.no = pl_w->char_no.b.no ;
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
		ew->size_top = ew->size_bottom = 32 ;
		ew->size_front = ew->size_back = 36 ;
		ew->prio = 0 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->type = no ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl14_at_enou_rush ;
		HIT_RESET( ew ) ;
		pl_w->atck_no = pl14_at_enou_rush ;
		HIT_RESET( pl_w ) ;
		ew->disp_flag = 0 ;
		shl14_06_pos_set( pl_w, ew ) ;
		pl_char_set( ew, 22, 66 ) ;
	}
	return ew ;
}

static void shl14_06_init( PLAYER_WORK *ew );
static void shl14_06_move( PLAYER_WORK *ew );
static void shl14_06_end( PLAYER_WORK *ew );
static void shl14_06_free( PLAYER_WORK *ew );
static void shl14_06_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_06_main( PLAYER_WORK *ew )
{
	static void ( *shl14_06_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_06_init, shl14_06_move, shl14_06_end, shl14_06_free,
	} ;

	if( ew->routine_0 < 2 ) shl14_06_debug( ew ) ;
	shl14_06_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The setter has done it all							*/
/*==============================================================*/
static void shl14_06_init( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*			Hits again at each cue of its animation, and		*/
/*			goes at the hyper combo's last steps				*/
/*==============================================================*/
static void shl14_06_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL14_06_LOCAL_W *lw = SHL14_06_LW( ew ) ;

	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		ew->atck_no = pl14_at_enou_rush ;
		HIT_RESET( ew ) ;
	}
	hit_work_set( ew ) ;
	if( pl_w->act_st != N_SUPER
	 || ( pl_w->pl_sp_type == pl14_sp_enou && ( pl_w->routine_2 == 3 || pl_w->routine_2 == 4 ) )
	 || ( pl_w->pl_sp_type == pl14_sp_enou_auto && ( pl_w->routine_2 == 7 || pl_w->routine_2 == 8 ) ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	lw->char_no.b.no = pl_w->char_no.b.no ;
	shl14_06_pos_set( pl_w, ew ) ;
	ew->prio = 0 ;
}

/*--------------------------------------------------------------*/
static void shl14_06_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl14_06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl14_06_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*			Puts the piece along the part (ef_w), turned by		*/
/*			its rot[2], facing SonSon's way						*/
/*==============================================================*/
static void shl14_06_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;
	Float x, y, dx, dy ;

	x = shl14_06_ofs[ew->type] ;
	y = -0.0f ;
	dx = x * nlCos( hw->rot[2] ) + y * nlSin( hw->rot[2] ) ;
	dy = x * nlSin( hw->rot[2] ) + y * nlCos( hw->rot[2] ) ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	if( pl_w->pl_dir ) dx = -dx ;
	ew->pos.x += dx ;
	ew->pos.y += dy ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_06.c" End of File	*/
