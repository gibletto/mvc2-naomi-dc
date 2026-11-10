/* Iceman's player effect 05: the sparks on the opponent that he holds. One work (type 0)
   tints the opponent and puts out the sparks (types 1 to 4), which hover by it and fade. */
#include "player.h"
#include "ef_char.h"
#include "pl_color.h"

/* the effect's own work, from obj_flag */
typedef struct PLS09_05_LOCAL_W {
	Sint32	x;	/* 000: where it sits from the opponent's centre, in 1/256 */
	Sint32	y;	/* 004 */
} PLS09_05_LOCAL_W;
#define PLS09_05_LW( w )	( ( PLS09_05_LOCAL_W * )&( w )->obj_flag )

static void pls09_05_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls09_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls09_05_main ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w->catch_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x905 ;
	}
	return ew ;
}

static void pls09_05_tint( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls09_05_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls09_05_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 >= 2 ) work_free( ew ) ;
	else if( !ew->type ) pls09_05_tint( ew, pl_w ) ;
	else pls09_05_spark( ew, pl_w ) ;
}

/*==============================================================*/
static void pls09_05_tint( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *sp_w ;

	ew->disp_flag = 0 ;
	ew->prio = pl_w->prio ;
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		CHAR_PART_COPY( ew, pl_w->catch_w ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = pl_w->catch_w->pl_no ;
		ew->pl_type = pl_w->catch_w->pl_type ;
		ew->scale.x = pl_w->catch_w->scale.x ;
		ew->scale.y = pl_w->catch_w->scale.y ;
		ew->atck_level = pl_w->catch_w->atck_level ;
		ew->work_no = pl_w->catch_w->work_no ;
		ew->col_cnt = pl_w->catch_w->col_cnt ;
		ew->scale = pl_w->catch_w->scale ;
		ew->prio = pl_w->catch_w->prio ;
		ew->sub_prio = -1 ;
		ew->pos.x = pl_w->catch_w->pos.x ;
		ew->pos.y = pl_w->catch_w->pos.y ;
		ew->timer1 = 10 ;
	} else if( pl_w->dm_elem ){
		work_free( ew ) ;
	} else if( pl_w->routine_1 == PL_DAMAGE || pl_w->routine_1 == PL_CAUGHT ){
		pl_color_set( ew->master_w, 7 ) ;
		if( ew->timer1 ){
			if( ( sp_w = work_get( 0, W_PLS, LIST_TOP ) ) ){
				sp_w->work_func = pls09_05_main ;
				sp_w->type = ( Sys->game_timer0 & 3 ) + 1 ;
				sp_w->sub_type = 0 ;
				sp_w->master_w = ew->master_w ;
				sp_w->pl_type = pl_w->catch_w->pl_type ;
				sp_w->ef_no = 0x905 ;
				PLS09_05_LW( sp_w )->x = ( Rnd() & 63 ) - 32 ;
				PLS09_05_LW( sp_w )->y = 64 - ( Rnd() & 63 ) ;
			}
			ew->timer1 -- ;
		}
	} else {
		pl_color_set( ew->master_w, 1 ) ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
static void pls09_05_spark( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->prio = pl_w->prio ;
	if( !ew->routine_0 ){
		ew->routine_0 ++ ;
		CHAR_PART_COPY( ew, pl_w->catch_w ) ;
		ew->disp_flag = 1 ;
		ew->pl_no = pl_w->catch_w->pl_no ;
		ew->pl_type = pl_w->catch_w->pl_type ;
		ew->scale.x = pl_w->catch_w->scale.x ;
		ew->scale.y = pl_w->catch_w->scale.y ;
		ew->atck_level = pl_w->catch_w->atck_level ;
		ew->work_no = pl_w->catch_w->work_no ;
		ew->col_cnt = pl_w->catch_w->col_cnt ;
		ew->scale = pl_w->catch_w->scale ;
		ew->prio = pl_w->catch_w->prio ;
		ew->sub_prio = -1 ;
		ef_char_set( ew, 27, ew->type + 0xff ) ;
		ew->timer0 = 300 ;
		ew->disp_flag = 0 ;
		if( ( ew->disp_dir = pl_w->disp_dir ) ){
			PLS09_05_LOCAL_W *lw = PLS09_05_LW( ew ) ;
			lw->x = -lw->x ;
		}
		return ;
	}
	if( !ew->routine_1 ){
		if( pl_w->dm_elem ){
			work_free( ew ) ;
			return ;
		}
		if( pl_w->routine_1 == PL_DAMAGE || pl_w->routine_1 == PL_CAUGHT ){
			ew->disp_flag = 1 ;
			ew->pos.x = pl_w->pos.x + PLS09_05_LW( ew )->x * 1.6666666f ;
			ew->pos.y = pl_w->pos.y + PLS09_05_LW( ew )->y * 2.142857f ;
			ef_char_move( ew ) ;
		} else {
			ew->routine_1 ++ ;
			ef_char_set( ew, 27, 4 ) ;
		}
	} else if( ef_char_move( ew ) < 0 ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls09_05.c" End of File	*/
