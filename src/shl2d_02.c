/* Shuma-Gorath's effect 02 (character 2d's shot 02): the four after-images that trail him through
   his Mystic Smash. Each keeps the last four places and animation frames of the one before it, and
   draws the oldest. */
#include "player.h"
#include "pl2d_def.h"

/* where he was, kept in obj_mtx: the position, his animation and its frame flag */
typedef struct SHL2D_02_REC {
	Float	x;
	Float	y;
	Sint32	char_no;
	Sint32	flag;
} SHL2D_02_REC;
#define SHL2D_02_REC_TBL( w )	( ( SHL2D_02_REC * )&( w )->obj_mtx )

static void shl2d_02_main( PLAYER_WORK *ew );

/****************************************************************/
int shl2d_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int type ;

	if( work_rest_cnt <= 4 ) return 0 ;
	for( type = 0 ; type < 4 ; type ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_END ) ) != 0 ){
			ew->ef_no = 0x2d02 ;
			ew->type = type ;
			ew->work_func = shl2d_02_main ;
			ew->master_w = pl_w ;
		}
	}
	return type ;
}

static void shl2d_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2d_02_main( PLAYER_WORK *ew )
{
	static void ( *shl2d_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2d_02_init, shl2d_02_move, shl2d_02_free,
	} ;

	shl2d_02_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl2d_02_rec_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2d_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2d_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int no ;

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
	ew->sub_prio = ew->type + 1 ;
	ew->atck_flag = HIT_ON ;
	ew->dm_flag = HIT_ON ;
	ew->col_flag = 1 ;
	ew->col_no = pl_w->col_no + 1 ;
	shl2d_02_rec_set( ew, pl_w ) ;
	shl2d_02_follow( ew, pl_w ) ;
	no = 50 ;
	if( ew->type == 3 ) no = 53 ;
	no += ew->atck_level ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_BACK_POS ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl2d_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL2D_LOCAL_W *lw = PL2D_LW( pl_w ) ;

	if( pl_w->routine_1 != PL_NORM || pl_w->pl_sp_type != 1 || pl_w->char_no.b.grp != Char_Special ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	ew->sub_prio = ew->type + 1 ;
	ew->col_flag = 1 ;
	ew->col_no = pl_w->col_no + 1 ;
	if( ew->routine_1 == 0 ){
		if( lw->smash_type < 0 ){
			ew->routine_1 ++ ;
			ew->timer0 = 4 - pl_w->type ;
			return ;
		}
		shl2d_02_follow( ew, pl_w ) ;
		if( !ew->hit_flag ) hit_work_set( ew ) ;
		return ;
	}
	if( -- ew->timer0 == 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	} else shl2d_02_follow( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl2d_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			Starts the places with where he stands				*/
/*==============================================================*/
static void shl2d_02_rec_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL2D_02_REC *rec = SHL2D_02_REC_TBL( ew ) ;
	int i ;

	for( i = 0 ; i < 4 ; i ++, rec ++ ){
		rec->x = pl_w->pos.x ;
		rec->y = pl_w->pos.y ;
		rec->char_no = pl_w->char_no.w ;
		rec->flag = pl_w->cg_flag1 ;
	}
}

/*==============================================================*/
/*			Shows the oldest place, and moves the others up;	*/
/*			the newest comes from him, or from the after-		*/
/*			image before this one								*/
/*==============================================================*/
static void shl2d_02_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL2D_02_REC *rec = SHL2D_02_REC_TBL( ew ) ;
	PLAYER_WORK *prev_w ;
	int i ;

	ew->pos.x = rec->x ;
	ew->pos.y = rec->y ;
	ew->char_no.w = rec->char_no ;
	pl_char_set2( ew, ew->char_no.b.grp, ew->char_no.b.no, rec->flag ) ;
	for( i = 0 ; i < 3 ; i ++, rec ++ ){
		*rec = rec[1] ;
	}
	if( ew->type == 0 ){
		rec->x = pl_w->pos.x ;
		rec->y = pl_w->pos.y ;
		rec->char_no = pl_w->char_no.w ;
		rec->flag = pl_w->cg_flag1 ;
	} else {
		prev_w = ew->before_w ;
		rec->x = prev_w->pos.x ;
		rec->y = prev_w->pos.y ;
		rec->char_no = prev_w->char_no.w ;
		rec->flag = prev_w->cg_flag1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2d_02.c" End of File	*/
