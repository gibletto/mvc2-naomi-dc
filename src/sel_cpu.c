/* The computer's team at the select scene. */
#include "player.h"
#include "select.h"

extern Sint8 sel_personal_tbl[];	/* each character's assist type to start with */

/****************************************************************/
/*			The computer's team, from the characters the		*/
/*			other side can take: sixteen random tries, then		*/
/*			a search in order									*/
/****************************************************************/
void sel_cpu_team_set( Sint8 side )
{
	PLAYER_WORK *pl_w, *ow ;
	Uint32 got[2] ;
	Uint32 cnt, bit, i ;
	Sint8 hi ;
	Uint8 no ;

	pl_w = &pl_work[side] ;
	ow = &pl_work[side ^ 1] ;
	sel_w.pick_flag[side] = 0 ;
	if( Sys->stage_cnt == 7 ){
		for( pl_w->timer1 = 0 ; pl_w->timer1 < 3 ; pl_w->timer1 ++ ){
			pl_work[pl_w->timer1 * 2 + side].load_type = pl_w->timer1 + 24 ;
			pl_work[pl_w->timer1 * 2 + side].pl_type = pl_w->timer1 + 24 ;
			pl_work[pl_w->timer1 * 2 + side].pl_personal = 0 ;
			sel_w.pick_flag[side] |= 0x11 << pl_w->timer1 ;
		}
		return ;
	}
	got[0] = got[1] = 0 ;
	for( pl_w->timer1 = 0 ; pl_w->timer1 < 3 ; pl_w->timer1 ++ ){
		for( cnt = 0 ; cnt < 16 ; cnt ++ ){
			bit = nlRand() ;
			bit &= 31 ;
			hi = nlRand() & 1 ;
			no = bit ;
			bit = 1 << bit ;
			if( !( ow->beat_flag[hi] & bit ) && ( sel_w.char_open[ow->pl_index][hi] & bit ) ){
				if( !( got[hi] & bit ) ) goto found ;
			}
		}
		for( hi = 0 ; hi < 2 ; hi ++ ){
			for( i = 0 ; i < 32 ; i ++ ){
				bit = ow->beat_flag[hi] ^ sel_w.char_open[ow->pl_index][hi] ;
				if( bit & ( 1 << i ) ){
					bit = 1 << i ;
					no = i ;
					if( !( got[hi] & bit ) ) goto found ;
				}
			}
		}
		for( hi = 0 ; hi < 2 ; hi ++ ){
			for( i = 0 ; i < 32 ; i ++ ){
				bit = sel_w.char_open[ow->pl_index][hi] ;
				if( bit & ( 1 << i ) ){
					bit = 1 << i ;
					no = i ;
					if( !( got[hi] & bit ) ) goto found ;
				}
			}
		}
found:
		got[hi] |= bit ;
		sel_w.pick_flag[side] |= 0x11 << pl_w->timer1 ;
		pl_work[pl_w->timer1 * 2 + side].load_type = hi * 32 + no ;
		pl_work[pl_w->timer1 * 2 + side].pl_type = pl_work[pl_w->timer1 * 2 + side].load_type ;
		pl_work[pl_w->timer1 * 2 + side].pl_personal = sel_personal_tbl[pl_work[pl_w->timer1 * 2 + side].load_type] ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_cpu.c" End of File	*/
