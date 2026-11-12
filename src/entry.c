/* Each side's entry during a game: the messages of an empty side (insert coin, press start), joining
   in, the continue count, the name entry for the rankings and the game over. */
#include "player.h"
#include "naomi.h"

#define NAME_SPACE	42	/* the letter numbers of the name entry's special letters */
#define NAME_CURSOR	43
#define NAME_BS	44
#define NAME_END	45
#define NAME_LETTERS	46


#ifndef TARGET_DC
extern Sint8 credit_enough_ck( int side, int cont ) ;
#endif

static void entry_move( PLAYER_WORK *pl_w ) ;
static void entry_wait( PLAYER_WORK *pl_w ) ;
static void entry_play( PLAYER_WORK *pl_w ) ;
static void entry_play_init( PLAYER_WORK *pl_w ) ;
static void entry_play_wait( PLAYER_WORK *pl_w ) ;
static void entry_play_end( PLAYER_WORK *pl_w ) ;
static void entry_cont( PLAYER_WORK *pl_w ) ;
static void entry_name( PLAYER_WORK *pl_w ) ;
static Sint8 name_time_ck( PLAYER_WORK *pl_w ) ;
static void name_blink( PLAYER_WORK *pl_w ) ;
static void name_blink_set( PLAYER_WORK *pl_w ) ;
static void name_end( PLAYER_WORK *pl_w ) ;
static void name_default( PLAYER_WORK *pl_w ) ;
static void name_replace( PLAYER_WORK *pl_w, Sint8 no ) ;
static void name_check( PLAYER_WORK *pl_w ) ;
static void name_input( PLAYER_WORK *pl_w ) ;
static void name_last( PLAYER_WORK *pl_w ) ;
static void name_decide( PLAYER_WORK *pl_w ) ;
static void name_end_wait( PLAYER_WORK *pl_w ) ;
static void entry_over( PLAYER_WORK *pl_w ) ;
void entry_init( PLAYER_WORK *pl_w ) ;
static void cont_add( PLAYER_WORK *pl_w ) ;
static void entry_start( PLAYER_WORK *pl_w ) ;
static void cont_init( PLAYER_WORK *pl_w ) ;
static void name_init( PLAYER_WORK *pl_w ) ;

/* names the entry won't take: the first 16 become a default name, the rest one of name_rep_tbl */
const Sint8 name_ng_tbl[20][3] = {
	{  0,  7, 14 }, { 18,  4, 23 }, {  7,  8, 21 }, {  5, 20,  2 }, {  5, 20, 10 },
	{  0, 18, 18 }, {  0, 20, 12 }, {  3,  8,  4 }, {  4, 19,  0 }, {  8, 17,  0 },
	{ 14, 18,  8 }, { 15,  4,  4 }, { 15,  8, 18 }, { 15, 11, 14 }, { 42, 42, 42 },
	{  0,  0,  0 }, { 34, 34, 34 }, { 10,  0, 22 }, { 10, 10, 10 }, { 19, 11,  3 },
} ;
const Sint8 name_rep_tbl[4][3] = {
	{ 12, 13, 17 }, { 19, 11,  3 }, {  7, 17, 18 }, { 10,  0, 22 },
} ;

/* the name entry's letters, by letter number */
char *letter_tbl[] = {
	"A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
	"N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
	".", ",", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
	"!", "?", "^A", "&", " ", "_", "^C", "^B",
} ;
/* the ranks' letters: first and second, the others */
static char *ord_tbl[] = { "^F", "^G", "^H", "^I", } ;

/****************************************************************/
void entry_main( void )
{
	Sys->cont_flag = 0 ;
	if( Sys->event_flag ) return ;
#ifdef TARGET_DC
	if( Sys->home_mode == HOME_TRAINING ) return ;		/* no one joins a training game */
#endif
	entry_move( &pl_work[0] ) ;
	entry_move( &pl_work[1] ) ;
}

/*==============================================================*/
static void entry_move( PLAYER_WORK *pl_w )
{
	static void ( *entry_jmp[] )( PLAYER_WORK *pl_w ) = {
		entry_wait, entry_play, entry_cont, entry_name, entry_over,
	} ;

	entry_jmp[pl_w->entry_routine_1]( pl_w ) ;
}

#ifndef TARGET_DC
/*==============================================================*/
/*			An empty side: what it takes to join in				*/
/*==============================================================*/
static void entry_wait( PLAYER_WORK *pl_w )
{
	int x ;

	if( Sys->ending_flag ) return ;
	if( Sys->entry_stop ) return ;
	if( Sys->join_flag ) return ;
	if( Sys->scene_no == 5 ) return ;
	if( Sys->no_challenge_flag ){
		int x1 ;

		if( pl_w->pl_index == 1 ) x1 = 104 ;
		else x1 = 8 ;

		STrPrint( x1, Sys->disp_line, 11, "NO CHALLENGER" ) ;
		return ;
	}
	if( pl_w->entry_timer == 0 ){
		pl_w->entry_timer = 40 ;
		pl_w->entry_timer2 ++ ;
		pl_w->entry_timer2 &= 3 ;
	}
	pl_w->entry_timer -- ;
	if( pl_w->pl_index == 1 ) x = 120 ;
	else x = 12 ;
	if( credit_conf->CoinFlag == NLA_FREEPLAY ){
		STrPrint( x, Sys->disp_line, 11, "FREE PLAY" ) ;
	} else if( pl_w->entry_timer2 & 1 ){
		if( pl_w->pl_index == 1 ) x = 100 ;
	else x = 4 ;
		if( credit_info.Player[pl_w->pl_index].Status == NLA_CREDIT_ENOUGH ){
			STrPrint( x, Sys->disp_line, 11, "PRESS START BUTTON" ) ;
		} else if( credit_info.Player[pl_w->pl_index].Status == NLA_CREDIT_MORE ){
			int i = 0 ;

			if( credit_conf->ChuteType ){
				if( pl_w->pl_index == 1 ) i = 1 ;
			}
			if( credit_info.Player[i].Credit && ( pl_w->entry_timer2 & 2 ) ){
				STrPrint( x, Sys->disp_line, 11, "%d CREDITS TO START", credit_info.Sequence[NLA_SEQUENCE_START] ) ;
			} else {
				STrPrint( x, Sys->disp_line, 11, "INSERT MORE COIN(S)" ) ;
			}
		} else {
			if( pl_w->pl_index == 1 ) x = 108 ;
	else x = 12 ;
			STrPrint( x, Sys->disp_line, 11, "INSERT COIN(S)" ) ;
		}
	} else {
		char *s ;

		if( pl_w->pl_index == 1 ) x = 108 ;
	else x = 12 ;
		s = credit_str1 ;
		if( credit_conf->ChuteType ){
			if( pl_w->pl_index == 1 ) s = credit_str2 ;
		}
		STrPrint( x, Sys->disp_line, 11, "%s", s ) ;
	}
	if( sw_work[pl_w->pl_index & 1].now & KEY_START ){
		if( credit_enough_ck( pl_w->pl_index, 0 ) ){
			entry_init( pl_w ) ;
			entry_start( pl_w ) ;
		}
	}
}
#else
/*==============================================================*/
/*			An empty side: start joins in						*/
/*==============================================================*/
/* The home version: no coins, and a mode that takes no challenger (the flag below 0) says nothing. */
static void entry_wait( PLAYER_WORK *pl_w )
{
	int x ;

	if( Sys->ending_flag || Sys->entry_stop || Sys->join_flag || Sys->scene_no == 5 ) return ;
	if( Sys->no_challenge_flag ){
		int x1 ;

		if( ( Sint8 )Sys->no_challenge_flag <= 0 ) return ;
		if( pl_w->pl_index == 1 ) x1 = 104 ;
		else x1 = 8 ;

		STrPrint( x1, Sys->disp_line, 11, "NO CHALLENGER" ) ;
		return ;
	}
	if( pl_w->entry_timer == 0 ){
		pl_w->entry_timer = 40 ;
		pl_w->entry_timer2 ++ ;
		pl_w->entry_timer2 &= 3 ;
	}
	pl_w->entry_timer -- ;
	if( pl_w->pl_index == 1 ) x = 100 ;
	else x = 4 ;
	STrPrint( x, Sys->disp_line, 11, "PRESS START BUTTON" ) ;
	if( sw_work[pl_w->pl_index & 1].now & KEY_START ){
		entry_init( pl_w ) ;
		entry_start( pl_w ) ;
		d_point_add( 100 ) ;
	}
}
#endif

/*--------------------------------------------------------------*/
static void entry_play( PLAYER_WORK *pl_w )
{
	static void ( *entry_play_jmp[] )( PLAYER_WORK *pl_w ) = {
		entry_play_init, entry_play_wait, entry_play_end,
	} ;

	entry_play_jmp[pl_w->entry_routine_2]( pl_w ) ;
}

/*==============================================================*/
static void entry_play_init( PLAYER_WORK *pl_w )
{
	pl_w->entry_routine_2 ++ ;
}

/*--------------------------------------------------------------*/
static void entry_play_wait( PLAYER_WORK *pl_w )
{
	if( !( ( Sys->play_side | Sys->new_side ) & pl_w->side_bit ) || Sys->ending_flag ) pl_w->entry_routine_2 ++ ;
}

/*==============================================================*/
/*			The side has lost: a continue, or the name entry	*/
/*==============================================================*/
static void entry_play_end( PLAYER_WORK *pl_w )
{
	pl_w->card_open[0] = pl_w->card_open[1] = 0 ;
	pl_w->card_ex[0] = pl_w->card_ex[1] = 0 ;
	vms_work.card_open[0] = vms_work.card_open[1] = 0 ;
	vms_work.card_ex[0] = vms_work.card_ex[1] = 0 ;
	if( !Sys->event_flag ){
		if( Sys->cont_enable || Sys->ending_flag ){
			if( !Sys->vs_flag ){
				cont_init( pl_w ) ;
#ifndef TARGET_DC
				if( !credit_conf->ChuteType ){
					pl_w->cont_credit = credit_info.Player[0].Credit ;
					pl_w->cont_remain = credit_info.Player[0].Remain ;
				} else {
					pl_w->cont_credit = credit_info.Player[pl_w->pl_index].Credit ;
					pl_w->cont_remain = credit_info.Player[pl_w->pl_index].Remain ;
				}
#endif
			} else name_init( pl_w ) ;
		} else name_init( pl_w ) ;
	} else name_init( pl_w ) ;
}

/*==============================================================*/
/*			The continue count									*/
/*==============================================================*/
static void entry_cont( PLAYER_WORK *pl_w )
{
	int x ;
#ifndef TARGET_DC
	Uint32 credit, remain ;
#endif

	Sys->cont_flag += pl_w->side_bit ;
	if( Sys->ending_flag ) goto end ;
	if( Sys->entry_stop ) return ;
	if( Sys->join_flag ) return ;
	if( pl_w->pl_index == 1 ) x = 120 ;
	else x = 12 ;
	pl_w->entry_timer2 -- ;
	if( !pl_w->entry_timer2 ){
		pl_w->entry_timer2 = 120 ;
		pl_w->entry_timer -- ;
	}
	STrPrint( x, Sys->disp_line, 11, "CONTINUE %1d", pl_w->entry_timer - 1 ) ;
#ifndef TARGET_DC
	if( sw_work[pl_w->pl_index & 1].now & KEY_START ){
		if( credit_enough_ck( pl_w->pl_index, 1 ) ){
			cont_add( pl_w ) ;
			entry_start( pl_w ) ;
		}
	}
	/* a coin starts the count again */
	if( !credit_conf->ChuteType ){
		credit = credit_info.Player[0].Credit ;
		remain = credit_info.Player[0].Remain ;
	} else {
		credit = credit_info.Player[pl_w->pl_index].Credit ;
		remain = credit_info.Player[pl_w->pl_index].Remain ;
	}
	if( pl_w->cont_credit != credit || pl_w->cont_remain != remain ){
		pl_w->cont_credit = credit ;
		pl_w->cont_remain = remain ;
		pl_w->entry_timer = 10 ;
		pl_w->entry_timer2 = 120 ;
	}
#else
	if( sw_work[pl_w->pl_index & 1].now & KEY_START ){
		cont_add( pl_w ) ;
		entry_start( pl_w ) ;
		d_point_add( 100 ) ;
	}
#endif
	if( pl_w->entry_timer < 8 && pl_w->entry_timer != 0 && pl_w->entry_routine_1 == ENTRY_CONT ){
		if( sw_work[pl_w->pl_index & 1].on & KEY_BUTTON ) pl_w->entry_timer -- ;
	}
	if( pl_w->entry_timer != 0 ) return ;
end:
	Sys->cont_flag -= pl_w->side_bit ;
	name_init( pl_w ) ;
}

/*==============================================================*/
/*			The name entry										*/
/*==============================================================*/
static void entry_name( PLAYER_WORK *pl_w )
{
	static void ( *entry_name_jmp[] )( PLAYER_WORK *pl_w ) = {
		name_input, name_input, name_input, name_last, name_decide, name_end_wait,
	} ;
	int x ;
	Uint8 ten, one ;
	Uint8 flag ;
	int i ;

	if( Sys->scene_no != 5 ){
		if( Sys->entry_stop ) return ;
		if( Sys->join_flag ) return ;
	}
	if( Sys->name_stop ) return ;
	flag = 0 ;
	pl_w->best_rank = pl_w->score_rank ;
	if( pl_w->win_rank < pl_w->score_rank ){
		flag = 1 ;
		pl_w->best_rank = pl_w->win_rank ;
	}
	if( pl_w->best_rank < 20 ) Sys->demo_no = flag ;
	else {
		pl_w->entry_routine_1 = ENTRY_OVER ;
		pl_w->entry_routine_2 = 0 ;
		pl_w->name_cursor = 0 ;
		pl_w->blink_flag = 0 ;
		pl_w->entry_timer = 60 ;
		pl_w->entry_timer2 = 0 ;
		pl_w->name_letter = 0 ;
		return ;
	}
	entry_name_jmp[pl_w->name_cursor]( pl_w ) ;
	if( pl_w->pl_index == 1 ) x = 120 ;
	else x = 12 ;
	ten = ( pl_w->best_rank + 1 ) / 10 ;
	one = ( pl_w->best_rank + 1 ) - ten * 10 ;
	if( ten ) STrPrint( x, Sys->disp_line, 11, "%s", letter_tbl[ten + 28] ) ;
	else STrPrint( x, Sys->disp_line, 11, "%s", letter_tbl[NAME_SPACE] ) ;
	x += 4 ;
	STrPrint( x, Sys->disp_line, 11, "%s", letter_tbl[one + 28] ) ;
	x += 4 ;
	if( pl_w->best_rank < 2 ) STrPrint( x, Sys->disp_line, 11, "%s", ord_tbl[pl_w->best_rank] ) ;
	else STrPrint( x, Sys->disp_line, 11, "%s", ord_tbl[3] ) ;
	x += 8 ;
	for( i = 0 ; i < 4 ; i ++ ){
		if( pl_w->name_cursor == 5 ){
			if( !pl_w->blink_flag ) STrPrint( x + i * 4, Sys->disp_line, 12, "%s", letter_tbl[pl_w->name[i]] ) ;
			else STrPrint( x + i * 4, Sys->disp_line, 11, "%s", letter_tbl[pl_w->name[i]] ) ;
		} else if( i != pl_w->name_cursor || pl_w->blink_flag ){
			STrPrint( x + i * 4, Sys->disp_line, 11, "%s", letter_tbl[pl_w->name[i]] ) ;
		} else {
			STrPrint( x + i * 4, Sys->disp_line, 11, "%s", letter_tbl[NAME_CURSOR] ) ;
		}
	}
}

/*==============================================================*/
/*			Start ends the name entry, and so does time			*/
/*==============================================================*/
static Sint8 name_time_ck( PLAYER_WORK *pl_w )
{
	if( sw_work[pl_w->pl_index & 1].on & KEY_START ){
		pl_w->entry_timer2 = 1 ;
		pl_w->entry_timer = 1 ;
	}
	pl_w->entry_timer -- ;
	pl_w->entry_timer2 -- ;
	if( pl_w->entry_timer <= 0 || pl_w->entry_timer2 <= 0 ){
		name_decide( pl_w ) ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void name_blink( PLAYER_WORK *pl_w )
{
	pl_w->blink_timer -- ;
	if( pl_w->blink_timer <= 0 ){
		pl_w->blink_flag ^= 1 ;
		if( pl_w->name_cursor == 5 ) pl_w->blink_timer = 1 ;
		else if( pl_w->blink_flag ) pl_w->blink_timer = 40 ;
		else pl_w->blink_timer = 20 ;
	}
}

/*==============================================================*/
static void name_blink_set( PLAYER_WORK *pl_w )
{
	pl_w->blink_timer = 40 ;
	pl_w->blink_flag = 1 ;
}

/*==============================================================*/
static void name_end( PLAYER_WORK *pl_w )
{
	Sint8 i ;

	pl_w->name_cursor = 5 ;
	pl_w->entry_timer = 120 ;
	pl_w->entry_timer2 = 40 ;
	pl_w->blink_timer = 1 ;
	for( i = 0 ; i < 3 ; i ++ ){
		if( pl_w->name[i] == NAME_CURSOR || pl_w->name[i] == NAME_END || !( pl_w->name_flag & 1 << i ) ){
			pl_w->name[i] = NAME_SPACE ;
		}
	}
	pl_w->name[3] = NAME_SPACE ;
}

/*==============================================================*/
static void name_default( PLAYER_WORK *pl_w )
{
	if( nlRand() & 1 ){
		pl_w->name[0] = 2 ;
		pl_w->name[1] = 0 ;
		pl_w->name[2] = 15 ;
	} else {
		pl_w->name[0] = 2 ;
		pl_w->name[1] = 14 ;
		pl_w->name[2] = 12 ;
	}
	pl_w->name[3] = NAME_SPACE ;
	pl_w->name_flag = 7 ;
}

/*==============================================================*/
static void name_replace( PLAYER_WORK *pl_w, Sint8 no )
{
	Sint8 i ;

	for( i = 0 ; i < 3 ; i ++ ) pl_w->name[i] = name_rep_tbl[no - 16][i] ;
	pl_w->name[3] = NAME_SPACE ;
	pl_w->name_flag = 7 ;
}

/*==============================================================*/
static void name_check( PLAYER_WORK *pl_w )
{
	Sint8 i, j, n ;

	for( i = 0 ; i < 20 ; i ++ ){
		n = 0 ;
		if( i >= 16 ){
			for( j = 0 ; j < 3 ; j ++ ){
				if( name_ng_tbl[i][j] == pl_w->name[j] ) n ++ ;
				else break ;
			}
			if( n == 3 ){
				name_replace( pl_w, i ) ;
				return ;
			}
		} else {
			for( j = 0 ; j < 3 ; j ++ ){
				if( name_ng_tbl[i][j] == pl_w->name[j] ) n ++ ;
				else break ;
			}
			if( n == 3 ){
				name_default( pl_w ) ;
				return ;
			}
		}
	}
}

/*==============================================================*/
/*			Picking the three letters							*/
/*==============================================================*/
static void name_input( PLAYER_WORK *pl_w )
{
	if( name_time_ck( pl_w ) ) return ;
	if( sw_work[pl_w->pl_index & 1].on & KEY_BUTTON ){
		pl_w->entry_timer2 += 300 ;
		name_blink_set( pl_w ) ;
		if( pl_w->name_letter == NAME_BS ){
			pl_w->name[pl_w->name_cursor] = NAME_CURSOR ;
			pl_w->name_cursor -- ;
			if( pl_w->name_cursor < 0 ){
				pl_w->name_cursor = 0 ;
				pl_w->name[pl_w->name_cursor] = 0 ;
			}
			pl_w->name_flag ^= 1 << pl_w->name_cursor ;
			pl_w->name_letter = pl_w->name[pl_w->name_cursor] ;
		} else if( pl_w->name_letter == NAME_END ){
			name_decide( pl_w ) ;
		} else {
			if( pl_w->name[pl_w->name_cursor] == NAME_CURSOR ) pl_w->name[pl_w->name_cursor] = 0 ;
			pl_w->name_flag |= 1 << pl_w->name_cursor ;
			pl_w->name_cursor ++ ;
			if( pl_w->name_cursor == 3 ){
				pl_w->name[3] = NAME_END ;
				pl_w->name_letter = 0 ;
			}
		}
	} else {
		if( ( sw_work[pl_w->pl_index & 1].on & ( KEY_LEFT | KEY_RIGHT ) ) || ( sw_work[pl_w->pl_index & 1].rep & ( KEY_LEFT | KEY_RIGHT ) ) ){
			name_blink_set( pl_w ) ;
		} else {
			name_blink( pl_w ) ;
		}
		if( sw_work[pl_w->pl_index & 1].on == KEY_RIGHT || sw_work[pl_w->pl_index & 1].rep == KEY_RIGHT ){
			pl_w->name_letter ++ ;
			if( pl_w->name_letter == NAME_CURSOR ) pl_w->name_letter ++ ;
			pl_w->name_letter = ( pl_w->name_letter + NAME_LETTERS ) % NAME_LETTERS ;
		}
		if( sw_work[pl_w->pl_index & 1].on == KEY_LEFT || sw_work[pl_w->pl_index & 1].rep == KEY_LEFT ){
			pl_w->name_letter -- ;
			if( pl_w->name_letter == NAME_CURSOR ) pl_w->name_letter -- ;
			pl_w->name_letter = ( pl_w->name_letter + NAME_LETTERS ) % NAME_LETTERS ;
		}
		pl_w->name[pl_w->name_cursor] = pl_w->name_letter ;
	}
}

/*==============================================================*/
/*			The last place: back space or end					*/
/*==============================================================*/
static void name_last( PLAYER_WORK *pl_w )
{
	if( name_time_ck( pl_w ) ) return ;
	if( sw_work[pl_w->pl_index & 1].on & KEY_BUTTON ){
		name_blink_set( pl_w ) ;
		if( pl_w->name_letter ){
			pl_w->entry_timer2 += 300 ;
			pl_w->name[pl_w->name_cursor] = NAME_SPACE ;
			pl_w->name_cursor -- ;
			pl_w->name_letter = pl_w->name[pl_w->name_cursor] ;
			pl_w->name_flag ^= 1 << pl_w->name_cursor ;
		} else {
			name_check( pl_w ) ;
			name_end( pl_w ) ;
		}
	} else {
		if( ( sw_work[pl_w->pl_index & 1].on & ( KEY_LEFT | KEY_RIGHT ) ) || ( sw_work[pl_w->pl_index & 1].rep & ( KEY_LEFT | KEY_RIGHT ) ) ){
			name_blink_set( pl_w ) ;
		} else {
			name_blink( pl_w ) ;
		}
		if( sw_work[pl_w->pl_index & 1].on == KEY_RIGHT || sw_work[pl_w->pl_index & 1].rep == KEY_RIGHT ){
			pl_w->name_letter ++ ;
		}
		if( sw_work[pl_w->pl_index & 1].on == KEY_LEFT || sw_work[pl_w->pl_index & 1].rep == KEY_LEFT ){
			pl_w->name_letter -- ;
		}
		pl_w->name_letter &= 1 ;
		pl_w->name[3] = NAME_END - pl_w->name_letter ;
	}
}

/*==============================================================*/
static void name_decide( PLAYER_WORK *pl_w )
{
	if( pl_w->name_flag ) name_check( pl_w ) ;
	else name_default( pl_w ) ;
	name_end( pl_w ) ;
}

/*==============================================================*/
/*			The name shown done; then it goes in the			*/
/*			rankings											*/
/*==============================================================*/
static void name_end_wait( PLAYER_WORK *pl_w )
{
	Sint8 i ;

	pl_w->entry_timer -- ;
	if( pl_w->entry_timer == 0 ){
		pl_w->entry_routine_1 = ENTRY_OVER ;
		pl_w->entry_routine_2 = 0 ;
		pl_w->name_cursor = 0 ;
		pl_w->blink_flag = 0 ;
		pl_w->entry_timer = 60 ;
		pl_w->entry_timer2 = 0 ;
		pl_w->name_letter = 0 ;
		for( i = 0 ; i < 4 ; i ++ ){
			if( pl_w->name[i] == NAME_END || pl_w->name[i] == NAME_BS ) pl_w->name[i] = NAME_SPACE ;
#ifdef TARGET_DC
			/* the score attack has its own ranking */
			if( pl_w->score_rank < 20 ){
				if( Sys->home_mode != HOME_SCORE ){
					rank_score[pl_w->score_rank].name[0] = pl_w->name[0] ;
					rank_score[pl_w->score_rank].name[1] = pl_w->name[1] ;
					rank_score[pl_w->score_rank].name[2] = pl_w->name[2] ;
				} else {
					rank_sa[pl_w->score_rank].name[0] = pl_w->name[0] ;
					rank_sa[pl_w->score_rank].name[1] = pl_w->name[1] ;
					rank_sa[pl_w->score_rank].name[2] = pl_w->name[2] ;
				}
			}
#else
			if( pl_w->score_rank < 20 ){
				rank_score[pl_w->score_rank].name[0] = pl_w->name[0] ;
				rank_score[pl_w->score_rank].name[1] = pl_w->name[1] ;
				rank_score[pl_w->score_rank].name[2] = pl_w->name[2] ;
			}
#endif
			if( pl_w->win_rank < 20 ){
				rank_win[pl_w->win_rank].name[0] = pl_w->name[0] ;
				rank_win[pl_w->win_rank].name[1] = pl_w->name[1] ;
				rank_win[pl_w->win_rank].name[2] = pl_w->name[2] ;
			}
		}
	} else {
		if( pl_w->entry_timer2 ) pl_w->entry_timer2 -- ;
		if( pl_w->entry_timer2 > 0 ) name_blink( pl_w ) ;
		else name_blink_set( pl_w ) ;
	}
}

/*--------------------------------------------------------------*/
static void entry_over( PLAYER_WORK *pl_w )
{
	int x ;

	if( pl_w->pl_index == 1 ) x = 120 ;
	else x = 12 ;
	STrPrint( x, Sys->disp_line, 11, "GAME OVER" ) ;
	pl_w->entry_timer -- ;
	if( !pl_w->entry_timer || ( sw_work[pl_w->pl_index & 1].now & KEY_START ) ){
		pl_w->entry_routine_1 = ENTRY_WAIT ;
		pl_w->entry_routine_2 = 0 ;
		pl_w->entry_timer = 0 ;
		pl_w->entry_timer2 = 0 ;
		entry_init( pl_w ) ;
		Sys->entry_side -= pl_w->side_bit ;
	}
}

/****************************************************************/
void entry_init( PLAYER_WORK *pl_w )
{
	pl_w->win_cnt = 0 ;
	pl_w->win_max = 0 ;
	pl_w->stage_cnt = 0 ;
	pl_w->sel_speed = 1 ;
	pl_w->old_score_hi = 0 ;
	pl_w->old_score = 0 ;
	pl_w->score_rank = 20 ;
	pl_w->win_rank = 20 ;
	pl_w->card_open[0] = 0 ;
	pl_w->card_open[1] = 0 ;
	pl_w->card_ex[0] = 0 ;
	pl_w->card_ex[1] = 0 ;
	pl_w->beat_flag[0] = 0 ;
	pl_w->beat_flag[1] = 0 ;
	pl_w->play_time = 0 ;
	pl_w->vs_record[0] = 0 ;
	pl_w->vs_record[1] = 0 ;
	pl_w->vs_record[2] = 0 ;
}

/*==============================================================*/
static void cont_add( PLAYER_WORK *pl_w )
{
	Sint32 cnt, n, hi ;

	pl_w->win_cnt = 0 ;
	n = pl_w->old_score ;
	hi = n & ~15 ;
	cnt = ( n & 15 ) + 1 ;
	if( cnt > 9 ) cnt = 9 ;
	hi |= cnt ;
	pl_w->old_score = hi ;
}

/*==============================================================*/
/*			The side joins the game								*/
/*==============================================================*/
static void entry_start( PLAYER_WORK *pl_w )
{
	Sys->entry_side |= pl_w->side_bit ;
	Sys->new_side |= pl_w->side_bit ;
	Sys->join_side |= pl_w->side_bit ;
	Sys->join_flag = 1 ;
	Sys->heart_step = 0 ;
	Sys->entry_flag = 1 ;
	pl_w->entry_routine_1 = ENTRY_PLAY ;
	pl_w->entry_routine_2 = 0 ;
}

/*==============================================================*/
static void cont_init( PLAYER_WORK *pl_w )
{
	pl_w->entry_routine_1 = ENTRY_CONT ;
	pl_w->entry_routine_2 = 0 ;
	pl_w->entry_timer = 10 ;
	pl_w->entry_timer2 = 120 ;
}

/*==============================================================*/
static void name_init( PLAYER_WORK *pl_w )
{
	Uint8 flag ;
	int i ;

	pl_w->entry_routine_1 = ENTRY_NAME ;
	pl_w->entry_routine_2 = 0 ;
	flag = 0 ;
	pl_w->best_rank = pl_w->score_rank ;
	if( pl_w->win_rank < pl_w->score_rank ){
		flag = 1 ;
		pl_w->best_rank = pl_w->win_rank ;
	}
	if( pl_w->best_rank < 20 ){
		for( i = 0 ; i < 3 ; i ++ ) pl_w->name[i] = NAME_CURSOR ;
		pl_w->name[3] = NAME_SPACE ;
		pl_w->entry_timer = 3600 ;
		pl_w->entry_timer2 = 2400 ;
		pl_w->name_letter = 0 ;
		pl_w->blink_timer = 40 ;
		pl_w->name_cursor = 0 ;
		pl_w->blink_flag = 1 ;
		Sys->demo_no = flag ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "entry.c" End of File	*/
