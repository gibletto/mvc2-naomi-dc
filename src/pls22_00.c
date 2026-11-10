/* Sakura's player effect 00: eleven kinds of work, by type. Type 0 puts out a piece (type 1) every
   sixteen frames round her in a win pose; 2 is the figure that leaves at her entrance, 4 and 5 the
   figures that run in for two win poses; 6 is the Shououken's armour, 8 the copies of her left behind
   in Ashura Senkuu, 9 and 10 the dark Shinkuu Hadouken's pieces, which follow her animation. 3 and 7
   do nothing. */
#include "player.h"
#include "pl_char_old.h"
#include "pl22_def.h"

/* where type 0's pieces start, by turns: pairs of across (turned with her) and up */
static Float pls22_00_ofs_tbl[] = {
	DOT_X( 56.0f ), DOT_Y( 88.0f ), 40.0f, DOT_Y( 120.0f ), -DOT_X( 8.0f ), DOT_Y( 136.0f ), -DOT_X( 40.0f ), DOT_Y( 104.0f ),
} ;

static void pls22_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *pls22_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( no != 10 ){
		if( !( ew = work_get( 0, W_PLS, LIST_TOP ) ) ) return 0 ;
	}else{
		if( !( ew = work_get( 0, W_PLS_ST, LIST_TOP ) ) ) return 0 ;
	}
	ew->work_func = pls22_00_main ;
	ew->type = no ;
	ew->sub_type = 0 ;
	ew->master_w = pl_w ;
	ew->pl_type = pl_w->pl_type ;
	ew->ef_no = 0x2200 ;
	ew->obj_flag = pl_w->char_no.w ;
	return ew ;
}

/*==============================================================*/
/*			A piece of type 1, from the type 0 work				*/
/*==============================================================*/
static PLAYER_WORK *pls22_00_piece_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls22_00_main ;
		ew->type = 1 ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w->master_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2200 ;
		ew->obj_flag = pl_w->obj_flag ;
	}
	return ew ;
}

/****************************************************************/
/*			Two of type 9										*/
/****************************************************************/
PLAYER_WORK *pls22_00_set3( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls22_00_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2200 ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = no ;
		ew->sub_type = 0 ;
	}
	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = pls22_00_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x2200 ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = no ;
		ew->sub_type = 1 ;
	}
	return ew ;
}

static void pls22_00_piece_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_piece( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_armor( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_ashura( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
/* pl_char_set takes an int here */
static void pls22_00_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void pls22_00_t10( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void pls22_00_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	static void ( *pls22_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		pls22_00_piece_out, pls22_00_piece, pls22_00_t2, pls22_00_t3, pls22_00_t4, pls22_00_t5,
		pls22_00_armor, pls22_00_t7, pls22_00_ashura, pls22_00_t9, pls22_00_t10,
	} ;

	if( ew->routine_0 >= 2 ) work_free( ew ) ;
	else pls22_00_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void pls22_00_piece_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *piece_w ;
	Float dx ;

	if( !ew->routine_0 ){
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
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->work1 = 0 ;
		ew->timer0 = 1 ;
		ew->disp_flag = 0 ;
	}
	if( ew->obj_flag != pl_w->char_no.w ){
		work_free( ew ) ;
		return ;
	}
	if( --ew->timer0 <= 0 ){
		ew->timer0 = 16 ;
		if( ( piece_w = pls22_00_piece_set( ew ) ) ){
			ew->work1 ++ ;
			ew->work1 = ew->work1 & 3 ;
			dx = *( pls22_00_ofs_tbl + ew->work1 * 2 ) ;
			piece_w->pos.y = ew->pos.y + *( pls22_00_ofs_tbl + ew->work1 * 2 + 1 ) ;
			if( ew->disp_dir ) dx = -dx ;
			piece_w->pos.x = ew->pos.x + dx ;
		}
	}
}

/* the piece's animation, from a table: its number counts it */
static Sint8 pls22_00_char_tbl[] = {
	13, 14, 15, 16, 17, 18, 18, 17, 16, 15, 14, 13, 24, 23, 22, 21, 20, 19,
} ;

/*==============================================================*/
static void pls22_00_char_set( PLAYER_WORK *ew, Uint8 no )
{
	pl_char_set( ew, Char_Effect, pls22_00_char_tbl[no] ) ;
}

/*==============================================================*/
/*			Type 1: a piece that sinks and shrinks through		*/
/*			twelve of the table's animations					*/
/*==============================================================*/
static void pls22_00_piece( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->acc.x = ew->acc.y = 0.0f ;
		ew->spd.x = DOT_X( 0.125f ) ;
		ew->spd.y = -DOT_Y( 0.5f ) ;
		if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
		pls22_00_char_set( ew, 17 ) ;
		ew->timer0 = 16 ;
		ew->timer1 = 11 ;
	}else{
		if( ew->obj_flag != pl_w->char_no.w ){
			work_free( ew ) ;
			return ;
		}
		if( --ew->timer0 <= 0 ){
			ew->timer0 = 16 ;
			if( --ew->timer1 <= 0 ){
				work_free( ew ) ;
				return ;
			}
			pls22_00_char_set( ew, ew->timer1 ) ;
		}
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
	}
}

/*==============================================================*/
static void pls22_00_t2( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs_x ;

	if( !ew->routine_0 ){
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
		ew->prio = 12 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ofs_x = 80.0f ;
		ew->spd.x = DOT_X( 7.0f ) ;
		if( ew->disp_dir ){
			ofs_x = -80.0f ;
			ew->spd.x = -ew->spd.x ;
		}
		ew->pos.x = pl_w->pos.x + ofs_x ;
		ew->pos.y = pl_w->pos.y + -DOT_Y( 6.0f ) ;
		pl_char_set( ew, 18, 4 ) ;
		ew->be_flag = 1 ;
		pl_shadow_set( ew ) ;
	}else if( !ew->routine_1 ){
		pl_char_move( ew ) ;
		if( ew->cg_type ){
			ew->cg_type = 0 ;
			ew->routine_1 ++ ;
			ew->timer0 = 28 ;
		}
	}else{
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		if( --ew->timer0 < 0 ) ew->routine_0 ++ ;
		else pl_char_move( ew ) ;
	}
}

/*==============================================================*/
static void pls22_00_t3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls22_00_t4( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs_x, dx ;

	if( !ew->routine_0 ){
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
		ew->sub_prio = 8 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ofs_x = 640.0f ;
		ew->spd.x = DOT_X( 7.0f ) ;
		if( ew->disp_dir ){
			ofs_x = -640.0f ;
			ew->spd.x = -ew->spd.x ;
		}
		ew->pos.x = pl_w->pos.x - ofs_x ;
		ew->pos.y = pl_w->pos.y ;
		pl_char_set( ew, 19, 8 ) ;
		ew->be_flag = 1 ;
		pl_shadow_set( ew ) ;
	}else{
		ew->prio = pl_w->prio ;
		if( !ew->routine_1 ){
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			pl_char_move( ew ) ;
			dx = pl_w->pos.x - ew->pos.x ;
			if( dx < 0 ) dx = -dx ;
			if( 160.0f > dx ){
				ew->routine_1 ++ ;
				PL22_LW( pl_w )->win5_flag = 1 ;
				pl_char_set( ew, 19, 9 ) ;
			}
		}else pl_char_move( ew ) ;
	}
}

/*==============================================================*/
static void pls22_00_t5( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs_x, dx, old_spd ;

	if( !ew->routine_0 ){
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
		ew->sub_prio -= 8 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ofs_x = 640.0f ;
		ew->spd.x = -DOT_X( 7.0f ) ;
		if( ew->disp_dir ){
			ofs_x = -640.0f ;
			ew->spd.x = -ew->spd.x ;
		}
		ew->pos.x = pl_w->pos.x + ofs_x ;
		ew->pos.y = pl_w->pos.y ;
		pl_char_set( ew, 19, 10 ) ;
		ew->be_flag = 1 ;
		pl_shadow_set( ew ) ;
	}else{
		ew->prio = pl_w->prio ;
		if( !ew->routine_1 ){
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			pl_char_move( ew ) ;
			dx = pl_w->pos.x - ew->pos.x ;
			if( dx < 0 ) dx = -dx ;
			if( DOT_X( 256.0f ) > dx ){
				ew->routine_1 ++ ;
				PL22_LW( pl_w )->win6_flag = 1 ;
				ofs_x = DOT_X( 32.0f ) ;
				ew->acc.x = DOT_X( 0.25f ) ;
				if( ew->disp_dir ){
					ofs_x = -DOT_X( 32.0f ) ;
					ew->acc.x = -ew->acc.x ;
				}
				ew->pos.x -= ofs_x ;
				pl_char_set( ew, 19, 11 ) ;
				pl_se_req( pl_w, 41 ) ;
			}
		}else{
			if( ew->routine_1 == 1 ){
				old_spd = ew->spd.x ;
				ew->pos.x += ew->spd.x ;
				ew->spd.x += ew->acc.x ;
				if( ew->spd.x * old_spd < 0 ) ew->routine_1 ++ ;
			}
			pl_char_move( ew ) ;
		}
	}
}

/*==============================================================*/
static void pls22_00_armor( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->prio = pl_w->prio ;
		ew->sub_prio = -8 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		pl_char_set( ew, Char_Effect, 25 ) ;
	}else{
		ew->prio = pl_w->prio ;
		if( ( ( ew->obj_flag >> 8 ) & 31 ) != pl_w->char_no.b.grp ){
			ew->routine_0 = 3 ;
			work_free( ew ) ;
			return ;
		}
		if( !ew->routine_1 ){
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			if( PL22_LW( pl_w )->armor_end == 2 ){
				work_free( ew ) ;
				return ;
			}
			if( PL22_LW( pl_w )->armor_end == 1 ){
				ew->routine_1 ++ ;
				ew->spd.x = -10.0f ;
				ew->acc.x = 0.0f ;
				if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
				ew->timer0 = 32 ;
			}
			pl_char_move( ew ) ;
			return ;
		}
		if( ew->routine_1 == 1 ){
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			pl_char_move( ew ) ;
			if( --ew->timer0 <= 0 ){
				ew->routine_1 ++ ;
				ew->spd.x /= 4.0f ;
				ew->acc.x /= 4.0f ;
				pl_char_set( ew, Char_Effect, 26 ) ;
			}
			return ;
		}
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		if( pl_char_move( ew ) < 0 ) work_free( ew ) ;
	}
}

/*==============================================================*/
static void pls22_00_t7( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
}

/*==============================================================*/
static void pls22_00_ashura( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->disp_flag = 1 ;
		ew->prio = 7 ;
		ew->timer0 = 8 ;
		ew->pos = pl_w->pos ;
	}
	if( --ew->timer0 < 0 ) ew->routine_0 ++ ;
}

/*==============================================================*/
/*			pl_char_set takes an int here						*/
/*==============================================================*/
static void pls22_00_t9( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_0 ){
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
		ew->disp_flag = 1 ;
		ew->sub_prio = ew->sub_type ? -4 : -18 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->timer0 = 0 ;
		pl_char_set( ew, Char_Effect, ew->sub_type * 2 ) ;
	}
	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	if( HI_BYTE( pl_w->cg_rival ) == 5 || HI_BYTE( pl_w->cg_rival ) == 8 ) pl_char_move( ew ) ;
	else work_free( ew ) ;
}

/*==============================================================*/
/*			The code (cg_rival's high byte) is read signed		*/
/*			and passed on as an int, but tested as a byte		*/
/*==============================================================*/
static void pls22_00_t10( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int no ;

	if( !ew->routine_0 ){
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
		ew->disp_flag = 0 ;
		ew->sub_prio = -1 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
	}
	if( pl_w->char_no.w != ew->obj_flag ) work_free( ew ) ;
	else {
		ew->prio = pl_w->prio ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		no = HI_BYTE( pl_w->cg_rival ) ;
		if( ( Uint8 )no ){
			ew->disp_flag = 1 ;
			if( ( Uint8 )no != ew->timer0 ){
				ew->timer0 = ( Uint8 )no ;
				pl_char_set( ew, Char_Effect, no ) ;
				return ;
			}
		}else if( !ew->timer0 ) return ;
		pl_char_move( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls22_00.c" End of File	*/
