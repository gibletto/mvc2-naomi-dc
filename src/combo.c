/* The combo counter, and the score: the points for hits, throws and combos, and the bonuses at
   the end of a round, added up in BCD. */
#include "player.h"

static Uint8 bcd_add( Uint8 a, Uint8 b );
static Uint32 bcd_conv( Sint32 n );

/* the points for a hit, by its count / 5 (BCD) */
static Sint16 hit_score_tbl[11] = {
	0x0100, 0x0100, 0x0200, 0x0300, 0x0500, 0x1000, 0x1500, 0x2000, 0x2500, 0x3000, 0x5000,
} ;
/* the time bonus, by the seconds left / 10 (BCD, in millions) */
static Sint16 time_bonus_tbl[10] = {
	0x0000, 0x0001, 0x0003, 0x0005, 0x0010, 0x0020, 0x0040, 0x0060, 0x0080, 0x0100,
} ;
/* the bonus by unk_9a (BCD, in millions) */
static Sint16 no_ko_bonus_tbl[5] = { 0x0000, 0x0030, 0x0060, 0x0090, 0x0100 } ;

/* the carry between the digits of a BCD sum */
static Sint8 bcd_carry ;

/****************************************************************/
/*			Counts one hit of ew's owner on dm_w				*/
/****************************************************************/
void combo_hit( PLAYER_WORK *dm_w, PLAYER_WORK *ew )
{
	PLAYER_WORK *at_w = &pl_work[ew->work_no] ;

	if( !vs_sys_work.first_hit ){
		vs_sys_work.first_hit = at_w->pl_no | 0x80 ;
		score_add( at_w, 0, 0x1000000 ) ;
	}
	if( !( dm_w->combo_dm_cnt_old = dm_w->combo_dm_cnt ) ){
		dm_w->combo_vital = dm_w->combo_damage = dm_w->vital_new ;
		dm_w->vital_dm = 0 ;
	}
	if( ++ at_w->combo_cnt > 999 ) at_w->combo_cnt = 999 ;
	if( ++ dm_w->combo_dm_cnt > 999 ) dm_w->combo_dm_cnt = 999 ;
	if( ++ dm_w->dm_hit_cnt > 999 ) dm_w->dm_hit_cnt = 999 ;
	if( ++ vs_sys_work.combo_total[at_w->pl_no] > 999 ) vs_sys_work.combo_total[at_w->pl_no] = 999 ;
	if( ++ vs_sys_work.combo_hit_total[at_w->pl_no] > 999 ) vs_sys_work.combo_hit_total[at_w->pl_no] = 999 ;
	if( vs_sys_w->vs_st < VS_WIN ){
		if( at_w->combo_cnt_max < at_w->combo_cnt ) at_w->combo_cnt_max = at_w->combo_cnt ;
		if( vs_sys_w->combo_max[at_w->pl_no] < vs_sys_w->combo_total[at_w->pl_no] ){
			vs_sys_w->combo_max[at_w->pl_no] = vs_sys_w->combo_total[at_w->pl_no] ;
		}
	}
}

/****************************************************************/
/*			Counts the hit hd of ew's owner on dm_w, and the	*/
/*			kinds of hit in the combo							*/
/****************************************************************/
void combo_hit2( PLAYER_WORK *dm_w, PLAYER_WORK *ew, HIT_DATA *hd )
{
	PLAYER_WORK *at_w = &pl_work[ew->work_no] ;
	Sint16 cnt ;
	Uint8 kind ;
	Uint8 flag ;

	if( !vs_sys_work.first_hit ){
		vs_sys_work.first_hit = at_w->pl_no | 0x80 ;
		score_add( at_w, 0, 0x1000000 ) ;
	}
	if( !( dm_w->combo_dm_cnt_old = dm_w->combo_dm_cnt ) ){
		dm_w->combo_vital = dm_w->combo_damage = dm_w->vital_new ;
		dm_w->vital_dm = 0 ;
	}
	if( hd->hit_flag & NO_COMBO ) return ;
	if( ++ at_w->combo_cnt > 999 ) at_w->combo_cnt = 999 ;
	if( ++ dm_w->combo_dm_cnt > 999 ) dm_w->combo_dm_cnt = 999 ;
	if( ++ dm_w->dm_hit_cnt > 999 ) dm_w->dm_hit_cnt = 999 ;
	if( ++ vs_sys_work.combo_total[at_w->pl_no] > 999 ) vs_sys_work.combo_total[at_w->pl_no] = 999 ;
	if( !( cnt = hd->hit_cnt ) ) cnt = 1 ;
	if( ( vs_sys_work.combo_hit_total[at_w->pl_no] += cnt ) > 999 ) vs_sys_work.combo_hit_total[at_w->pl_no] = 999 ;
	if( vs_sys_w->vs_st < VS_WIN ){
		if( at_w->combo_cnt_max < at_w->combo_cnt ) at_w->combo_cnt_max = at_w->combo_cnt ;
		if( vs_sys_w->combo_max[at_w->pl_no] < vs_sys_w->combo_total[at_w->pl_no] ){
			vs_sys_w->combo_max[at_w->pl_no] = vs_sys_w->combo_total[at_w->pl_no] ;
		}
	}
	if( ( kind = hd->atck_kind & ( A_SPECIAL | A_HYPER ) ) ){
		at_w->combo_kind_old = at_w->combo_kind ;
		at_w->combo_kind |= kind ;
	}
	at_w->combo_hit_type_old = at_w->combo_hit_type ;
	flag = 0 ;
	if( at_w->pl_stance == STANCE_AIR ){
		flag |= 1 ;
		if( at_w->combo_hit_type_old & 1 ) flag |= 2 ;
		if( dm_w->pl_stance == STANCE_AIR ) flag |= 4 ;
	}
	if( !( hd->atck_kind & 0x70 ) ) flag |= 8 ;
	at_w->combo_hit_type = flag ;
	at_w->combo_hit_types |= flag ;
	flag = at_w->combo_hit_types ;
	if( ( kind = flag & 14 ) == 14 ){
		if( at_w->combo_cnt >= 3 ) at_w->combo_kind |= 1 ;
	}
}

/****************************************************************/
/*			Ends the combo on pl_w, and scores it for the		*/
/*			opponent once the whole team is free				*/
/****************************************************************/
void combo_reset( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *at_w = vs_sys_work.team_w[pl_w->pl_no ^ 1][0] ;
	Uint32 pts[2] ;

	pl_w->stun_ng_flag = 0 ;
	pl_w->combo_reset_flag = 0 ;
	pl_w->assist_dm_cnt = 0 ;
	pl_w->combo_dm_cnt = 0 ;
	pl_w->dm_hit_cnt = 0 ;
	if( !vs_sys_work.combo_total[at_w->pl_no] ) return ;
	if( !vs_sys_work.combo_hit_total[at_w->pl_no] ) return ;
	if( ( Uint8 )( vs_sys_work.team_w[pl_w->pl_no][0]->combo_reset_flag
	  | vs_sys_work.team_w[pl_w->pl_no][1]->combo_reset_flag
	  | vs_sys_work.team_w[pl_w->pl_no][2]->combo_reset_flag ) ) return ;
	at_w->combo_cnt = 0 ;
	at_w->combo_kind = 0 ;
	at_w->air_combo_cnt = 0 ;
	at_w->combo_hit_types = 0 ;
	at_w->combo_hit_type_old = at_w->combo_hit_type ;
	at_w->combo_hit_type = 0 ;
	if( !Sys->vs_flag ){
		if( ( pts[0] = vs_sys_w->combo_total[at_w->pl_no] * vs_sys_w->combo_total[at_w->pl_no] ) > 1 ){
			pts[0] = bcd_conv( pts[0] ) ;
			if( vs_sys_w->combo_total[at_w->pl_no] >= 3 ) vs_sys_work.combo_pts[at_w->pl_no] = pts[0] ;
			pts[1] = pts[0] >> 20 ;
			pts[0] = pts[0] << 12 ;
			score_add( at_w, pts[1], pts[0] ) ;
		}
	}
	vs_sys_work.combo_total[at_w->pl_no] = 0 ;
	vs_sys_work.combo_hit_total[at_w->pl_no] = 0 ;
}

/****************************************************************/
/*			Clears dm_hit_cnt of the opponent's three			*/
/*			players												*/
/****************************************************************/
void combo_dm_clr( PLAYER_WORK *pl_w )
{
	int i ;

	for( i = 0 ; i < 3 ; i ++ ) vs_sys_w->team_w[pl_w->pl_no ^ 1][i]->dm_hit_cnt = 0 ;
}

/****************************************************************/
/*			The first attack of the round, by a throw			*/
/****************************************************************/
void first_atck_catch( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;

	if( !vs_sys_work.first_hit ){
		em = pl_w->catch_w ;
		vs_sys_work.first_hit = em->pl_no | 0x80 ;
		score_add( em, 0, 0x1000000 ) ;
	}
}

/****************************************************************/
/*			Adds cnt hits to the combo of at_w on dm_w			*/
/****************************************************************/
void combo_add( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, Sint16 cnt )
{
	dm_w->combo_reset_flag = 0xff ;
	if( !( dm_w->combo_dm_cnt_old = dm_w->combo_dm_cnt ) ){
		dm_w->combo_vital = dm_w->combo_damage = dm_w->vital_new ;
		dm_w->vital_dm = 0 ;
	}
	if( ( at_w->combo_cnt += cnt ) > 999 ) at_w->combo_cnt = 999 ;
	if( ( dm_w->combo_dm_cnt += cnt ) > 999 ) dm_w->combo_dm_cnt = 999 ;
	if( ( dm_w->dm_hit_cnt += cnt ) > 999 ) dm_w->dm_hit_cnt = 999 ;
	if( ( vs_sys_work.combo_total[at_w->pl_no] += cnt ) > 999 ) vs_sys_work.combo_total[at_w->pl_no] = 999 ;
	if( ( vs_sys_work.combo_hit_total[at_w->pl_no] += cnt ) > 999 ) vs_sys_work.combo_hit_total[at_w->pl_no] = 999 ;
	if( vs_sys_w->vs_st < VS_WIN ){
		if( at_w->combo_cnt_max < at_w->combo_cnt ) at_w->combo_cnt_max = at_w->combo_cnt ;
		if( vs_sys_w->combo_max[at_w->pl_no] < vs_sys_w->combo_total[at_w->pl_no] ){
			vs_sys_w->combo_max[at_w->pl_no] = vs_sys_w->combo_total[at_w->pl_no] ;
		}
	}
}

/****************************************************************/
/*			Sets the combo of at_w on dm_w to cnt hits			*/
/****************************************************************/
void combo_set( PLAYER_WORK *dm_w, PLAYER_WORK *at_w, Sint16 cnt )
{
	dm_w->combo_reset_flag = 0xff ;
	if( !( dm_w->combo_dm_cnt_old = dm_w->combo_dm_cnt ) ){
		dm_w->combo_vital = dm_w->combo_damage = dm_w->vital_new ;
		dm_w->vital_dm = 0 ;
	}
	if( ( at_w->combo_cnt = cnt ) > 999 ) at_w->combo_cnt = 999 ;
	if( ( dm_w->combo_dm_cnt = cnt ) > 999 ) dm_w->combo_dm_cnt = 999 ;
	if( ( dm_w->dm_hit_cnt = cnt ) > 999 ) dm_w->dm_hit_cnt = 999 ;
	if( ( vs_sys_work.combo_total[at_w->pl_no] += cnt ) > 999 ) vs_sys_work.combo_total[at_w->pl_no] = 999 ;
	if( ( vs_sys_work.combo_hit_total[at_w->pl_no] += cnt ) > 999 ) vs_sys_work.combo_hit_total[at_w->pl_no] = 999 ;
	if( vs_sys_w->vs_st < VS_WIN ){
		if( at_w->combo_cnt_max < at_w->combo_cnt ) at_w->combo_cnt_max = at_w->combo_cnt ;
		if( vs_sys_w->combo_max[at_w->pl_no] < vs_sys_w->combo_total[at_w->pl_no] ){
			vs_sys_w->combo_max[at_w->pl_no] = vs_sys_w->combo_total[at_w->pl_no] ;
		}
	}
}

/****************************************************************/
/*			The points for a hit								*/
/****************************************************************/
void hit_score_add( PLAYER_WORK *pl_w, HIT_DATA *hd )
{
	Sint16 pts ;
	int i ;

	if( Sys->play_side == 3 ) return ;
	if( ( i = hd->damage / 5 ) > 10 ) i = 10 ;
	pts = hit_score_tbl[i] ;
	score_add( pl_w, 0, pts ) ;
}

/****************************************************************/
/* adds hi and lo to the score of pl_w's side; the score's first word is read and dropped first */
void score_add( PLAYER_WORK *pl_w, Uint32 hi, Uint32 lo )
{
	PLAYER_WORK *w ;
	Uint32 *score ;

	if( Sys->play_side == 3 ) return ;
	w = &pl_work[0] ;
	( ( volatile PLAYER_WORK * )w )->score ;
	score = w->score ;
	if( pl_w->pl_no ){
		w = &pl_work[1] ;
		( ( volatile PLAYER_WORK * )w )->score ;
		score = w->score ;
	}
	score_bcd_add( score, hi, lo ) ;
}

/****************************************************************/
/*			Adds hi and lo to an eleven-digit BCD score,		*/
/*			which stops at all nines							*/
/****************************************************************/
void score_bcd_add( Uint32 *score, Uint32 hi, Uint32 lo )
{
	Uint8 *d ;
	Uint8 *s ;

	bcd_carry = 0 ;
	d = ( Uint8 * )score ;
	s = ( Uint8 * )&lo ;
	d[0] = bcd_add( d[0], s[0] ) ;
	d[1] = bcd_add( d[1], s[1] ) ;
	d[2] = bcd_add( d[2], s[2] ) ;
	d[3] = bcd_add( d[3], s[3] ) ;
	s = ( Uint8 * )&hi ;
	d[4] = bcd_add( d[4], s[0] ) ;
	d[5] = bcd_add( d[5], s[1] ) ;
	d[6] = bcd_add( d[6], s[2] ) ;
	d[7] = bcd_add( d[7], s[3] ) ;
	if( score[1] > 0x999 ){
		score[1] = 0x999 ;
		score[0] = 0x99999999 ;
	}
}

/*==============================================================*/
/*			Adds two BCD digit pairs and the carry				*/
/*==============================================================*/
static Uint8 bcd_add( Uint8 a, Uint8 b )
{
	Uint16 sum ;

	if( ( sum = ( bcd_carry & 1 ) + ( a & 0x0f ) + ( b & 0x0f ) ) > 9 ){
		sum -= 10 ;
		sum |= 0x10 ;
	}
	if( ( sum += ( a & 0xf0 ) + ( b & 0xf0 ) ) > 0x99 ){
		sum -= 0xa0 ;
		sum &= 0xff ;
		bcd_carry = 1 ;
	} else {
		bcd_carry = 0 ;
	}
	return sum ;
}

/*==============================================================*/
/*			A number in BCD										*/
/*==============================================================*/
static Uint32 bcd_conv( Sint32 n )
{
	Uint32 bcd = 0 ;
	int sft = 0 ;

	do {
		bcd += ( n % 10 ) << sft ;
		sft += 4 ;
	} while( ( n /= 10 ) ) ;
	return bcd ;
}

/****************************************************************/
/*			The winner's vital left, 200 points for each 144	*/
/*			of it, with 300 for each player at full vital		*/
/****************************************************************/
void vital_bonus( Uint32 *hi, Uint32 *lo )
{
	Uint32 full ;
	Uint8 win ;
	Uint8 *d, *s ;
	int i ;

	*lo = *hi = 0 ;
	if( ( win = vs_sys_w->winner_id ) >= 0 ){
		*lo = vs_sys_w->team_w[win][0]->vital_new ;
		*lo += vs_sys_w->team_w[win][1]->vital_new ;
		*lo += vs_sys_w->team_w[win][2]->vital_new ;
		*lo = ( Float )*lo * 200.0f / 144.0f ;
		*lo = bcd_conv( *lo ) ;
		full = 0x300 ;
		for( i = 0 ; i < 3 ; i ++ ){
			if( vs_sys_w->team_w[win][i]->vital_new == 0x90 ){
				d = ( Uint8 * )lo ;
				s = ( Uint8 * )&full ;
				bcd_carry = 0 ;
				d[0] = bcd_add( d[0], s[0] ) ;
				d[1] = bcd_add( d[1], s[1] ) ;
				d[2] = bcd_add( d[2], s[2] ) ;
				d[3] = bcd_add( d[3], s[3] ) ;
			}
		}
		*hi = *lo >> 12 ;
		*lo = *lo << 20 ;
	}
}

/****************************************************************/
/*			The time bonus, by the seconds left					*/
/****************************************************************/
void time_bonus( Uint32 *hi, Uint32 *lo )
{
	Uint8 i ;

	*lo = *hi = 0 ;
	if( ( i = vs_sys_w->round_time / 10 ) > 9 ) i = 9 ;
	if( ( *lo = time_bonus_tbl[i] ) ){
		*hi = *lo >> 8 ;
		*lo = *lo << 24 ;
	}
}

/****************************************************************/
/*			The bonus by the winner's unk_9a					*/
/****************************************************************/
void unk_9a_bonus( Uint32 *hi, Uint32 *lo )
{
	Uint8 i ;

	*lo = *hi = 0 ;
	if( ( i = vs_sys_w->no_ko_cnt[vs_sys_w->winner_id] ) > 4 ) i = 4 ;
	if( ( *lo = no_ko_bonus_tbl[i] ) ){
		*hi = *lo >> 8 ;
		*lo = *lo << 24 ;
	}
}

/****************************************************************/
/*			The bonuses at the end of a round, added to the		*/
/*			winner's score										*/
/****************************************************************/
void bonus_add( Uint32 *hi, Uint32 *lo )
{
	Uint32 b_hi, b_lo ;
	Uint32 pts[2] ;
	PLAYER_WORK *pl_w ;

	pts[0] = pts[1] = 0 ;
	vital_bonus( &b_hi, &b_lo ) ;
	score_bcd_add( &pts[0], b_hi, b_lo ) ;
	time_bonus( &b_hi, &b_lo ) ;
	score_bcd_add( &pts[0], b_hi, b_lo ) ;
	unk_9a_bonus( &b_hi, &b_lo ) ;
	score_bcd_add( &pts[0], b_hi, b_lo ) ;
	*hi = pts[1] ;
	*lo = pts[0] ;
	if( vs_sys_w->winner_id >= 0 ){
		if( vs_sys_w->winner_id == 0 ) pl_w = &pl_work[0] ;
		else pl_w = &pl_work[1] ;
		score_add( pl_w, pts[1], pts[0] ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "combo.c" End of File	*/
