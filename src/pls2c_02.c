/* Magneto's player effect 02: what his first throw does to the player he holds, for as long
   as Magneto is the one holding: a copy of the held player that sheds pieces (type 0), the pieces
   that fall (types 1 to 4) and the ones those throw off (types 0x80 to 0x87). */
#include "player.h"
#include "ef_char.h"

/* a piece's own work, in obj_flag and unk_d0: where it hangs from his body */
typedef struct PLS2C_02_LOCAL_W {
	Sint32	x, y ;
} PLS2C_02_LOCAL_W ;
#define PLS2C_02_LW( w )	( ( PLS2C_02_LOCAL_W * )&( w )->obj_flag )

static void pls2c_02_main( PLAYER_WORK *ew );

/****************************************************************/
/*			Set for the player he holds in his first throw		*/
/****************************************************************/
PLAYER_WORK *pls2c_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls2c_02_main ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w->catch_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2c02 ;
	}
	return ew ;
}

static void pls2c_02_copy( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2c_02_piece( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls2c_02_chip( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls2c_02_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->catch_w->pl_type != PL_MAGNETO ){
		work_free( ew ) ;
		return ;
	}
	if( ew->routine_0 >= 2 ) work_free( ew ) ;
	else if( !ew->type ) pls2c_02_copy( ew, pl_w ) ;
	else if( ew->type & 0x80 ) pls2c_02_chip( ew, pl_w ) ;
	else pls2c_02_piece( ew, pl_w ) ;
}

/*==============================================================*/
/*			The copy of the held player, drawn only at the		*/
/*			start; while he is thrown it sheds pieces			*/
/*==============================================================*/
static void pls2c_02_copy( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *sp_w ;

	ew->disp_flag = 0 ;
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
		ew->pos.x = pl_w->catch_w->pos.x ;
		ew->pos.y = pl_w->catch_w->pos.y ;
		ew->timer1 = 10 ;
	} else if( !pl_w->dm_elem ){
		if( pl_w->routine_1 == PL_DAMAGE || pl_w->routine_1 == PL_CAUGHT ){
			if( ew->timer1 ){
				if( ( sp_w = work_get( 0, W_PLS, LIST_TOP ) ) ){
					sp_w->work_func = pls2c_02_main ;
					sp_w->type = ( Sys->game_timer0 & 3 ) + 1 ;
					sp_w->sub_type = 0 ;
					sp_w->master_w = ew->master_w ;
					sp_w->pl_type = pl_w->catch_w->pl_type ;
					sp_w->ef_no = 0x2c02 ;
					PLS2C_02_LW( sp_w )->x = ( Rnd() & 63 ) - 32 ;
					PLS2C_02_LW( sp_w )->y = 64 - ( Rnd() & 63 ) ;
				}
				ew->timer1 -- ;
			}
		} else work_free( ew ) ;
	} else work_free( ew ) ;
}

/*==============================================================*/
/*			A piece that hangs on him, by his shaking, and		*/
/*			then breaks into pieces of its own					*/
/*==============================================================*/
static void pls2c_02_piece( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *sp_w ;
	PLS2C_02_LOCAL_W *lw ;
	int i ;
	int r ;

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
		ef_char_set( ew, 27, ew->type + 5 ) ;
		ew->timer0 = 300 ;
		ew->disp_flag = 0 ;
		if( ( ew->disp_dir = pl_w->disp_dir ) ){
			lw = PLS2C_02_LW( ew ) ;
			lw->x = -lw->x ;
		}
		ew->sub_prio = -1 ;
	} else {
		ew->prio = pl_w->prio ;
		if( !pl_w->dm_elem ){
			if( pl_w->routine_1 == PL_DAMAGE || pl_w->routine_1 == PL_CAUGHT ){
				ew->disp_flag = 1 ;
				ew->pos.x = pl_w->pos.x + PLS2C_02_LW( ew )->x * 1.6666666f ;
				ew->pos.y = pl_w->pos.y + PLS2C_02_LW( ew )->y * 2.142857f ;
				ef_char_move( ew ) ;
				return ;
			}
			for( i = 0 ; i < 2 ; i ++ ){
				if( ( sp_w = work_get( 0, W_PLS, LIST_TOP ) ) ){
					sp_w->work_func = pls2c_02_main ;
					sp_w->type = ( Rnd() & 7 ) + 0x80 ;
					sp_w->sub_type = 0 ;
					sp_w->master_w = ew->master_w ;
					sp_w->pl_type = pl_w->catch_w->pl_type ;
					sp_w->ef_no = 0x2c02 ;
					sp_w->pos.x = ew->pos.x ;
					sp_w->pos.y = ew->pos.y ;
					sp_w->acc.x = 0 ;
					sp_w->acc.y = -0.13392857f ;
					r = ( ( Rnd() & 127 ) - 64 ) << 12 ;
					sp_w->spd.x = r * 1.6666666f / 65536.0f ;
					r = ( ( Rnd() & 47 ) - 32 ) << 12 ;
					sp_w->spd.y = -r * 2.142857f / 65536.0f ;
				} else break ;
			}
		}
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*			A piece one of those throws off: it flies a			*/
/*			short while and fades								*/
/*==============================================================*/
static void pls2c_02_chip( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
		ef_char_set( ew, 27, ( ew->type & 7 ) + 10 ) ;
		ew->timer0 = 30 ;
		ew->disp_flag = 0 ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = -1 ;
	} else {
		ew->prio = pl_w->prio ;
		if( pl_w->dm_elem || --ew->timer0 < 0 ) work_free( ew ) ;
		else {
			ew->disp_flag = 1 ;
			ef_char_move( ew ) ;
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls2c_02.c" End of File	*/
