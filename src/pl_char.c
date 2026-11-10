/* The animation and palette code every work uses: setting and stepping a player's or an effect's
   animation, finding a player's colours and copying them to the palette, and the data unpackers. */
#define EF_COLOR_SET_NEW
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl_color_old.h"

/* an effect's animation frame: the first eight bytes of a player's */
typedef struct EF_CHAR_DATA {
	Sint32	unk_00;		/* 000: the frame's codes; after a last frame, where the animation goes on */
	Uint8	unk_04[0x08 - 0x04];
} EF_CHAR_DATA;

/* what an effect's frame leaves in cg_prm */
typedef struct EF_CHAR_WORK {
	EF_CHAR_DATA data;
	Sint32	unk_08;
	Sint32	unk_0c;
	Sint32	unk_10;
} EF_CHAR_WORK;

#define EF_CG( w )	( *( EF_CHAR_WORK * )&( w )->cg_prm )

#define CG( w )	( *( CHAR_DATA * )&( w )->cg_prm )

/* the special palette lines, after the six colours' eight lines of 16 */
#define EF_COL_ADRS( pl )	( ( Uint16 * )( ( pl )->col_adrs + ( ( pl )->col_cnt * 128 + 0x300 ) * 2 ) )

/****************************************************************/
/*			Set an effect's animation							*/
/****************************************************************/
int ef_char_set( PLAYER_WORK *ef_w, Uint8 group, Uint8 no )
{
	Uint32 *tbl ;

	ef_w->char_no.b.grp = group ;
	ef_w->char_no.b.no = no ;
	tbl = ( Uint32 * )( ef_w->char_adrs + ef_w->char_ofs ) ;
	tbl = ( Uint32 * )( ef_w->char_adrs + tbl[group] ) ;
	ef_w->cg_adrs = ( Sint8 * )( tbl[no] + ef_w->char_adrs ) ;
	EF_CG( ef_w ).data = *( EF_CHAR_DATA * )ef_w->cg_adrs ;
	EF_CG( ef_w ).unk_08 = 0 ;
	EF_CG( ef_w ).unk_0c = 0 ;
	EF_CG( ef_w ).unk_10 = 0 ;
	if( !ef_w->cg_ctr ){
		do {
			ef_w->cg_adrs = ef_w->cg_adrs + sizeof( EF_CHAR_DATA ) ;
			if( ef_w->cg_end_flag & CG_END ) ef_w->cg_adrs = ( Sint8 * )( ef_w->char_adrs + ( ( EF_CHAR_DATA * )ef_w->cg_adrs )->unk_00 ) ;
			EF_CG( ef_w ).data = *( EF_CHAR_DATA * )ef_w->cg_adrs ;
			EF_CG( ef_w ).unk_08 = 0 ;
			EF_CG( ef_w ).unk_0c = 0 ;
			EF_CG( ef_w ).unk_10 = 0 ;
		} while( !ef_w->cg_ctr ) ;
	}
}

/****************************************************************/
/*			Set an effect's animation, starting at the given	*/
/*			frame												*/
/****************************************************************/
void ef_char_set2( PLAYER_WORK *ef_w, Uint8 group, Uint8 no, int frame )
{
	Uint32 *tbl ;

	ef_w->char_no.b.grp = group ;
	ef_w->char_no.b.no = no ;
	tbl = ( Uint32 * )( ef_w->char_adrs + ef_w->char_ofs ) ;
	tbl = ( Uint32 * )( ef_w->char_adrs + tbl[group] ) ;
	ef_w->cg_adrs = ( Sint8 * )( tbl[no] + ef_w->char_adrs ) ;
	ef_w->cg_adrs = ef_w->cg_adrs + frame * sizeof( EF_CHAR_DATA ) ;
	EF_CG( ef_w ).data = *( EF_CHAR_DATA * )ef_w->cg_adrs ;
	EF_CG( ef_w ).unk_08 = 0 ;
	EF_CG( ef_w ).unk_0c = 0 ;
	EF_CG( ef_w ).unk_10 = 0 ;
	if( !ef_w->cg_ctr ){
		do {
			ef_w->cg_adrs = ef_w->cg_adrs + sizeof( EF_CHAR_DATA ) ;
			if( ef_w->cg_end_flag & CG_END ) ef_w->cg_adrs = ( Sint8 * )( ef_w->char_adrs + ( ( EF_CHAR_DATA * )ef_w->cg_adrs )->unk_00 ) ;
			EF_CG( ef_w ).data = *( EF_CHAR_DATA * )ef_w->cg_adrs ;
			EF_CG( ef_w ).unk_08 = 0 ;
			EF_CG( ef_w ).unk_0c = 0 ;
			EF_CG( ef_w ).unk_10 = 0 ;
		} while( !ef_w->cg_ctr ) ;
	}
}

/****************************************************************/
/*			Step an effect's animation; negative when it ran	*/
/*			past its last frame									*/
/****************************************************************/
Sint8 ef_char_move( PLAYER_WORK *ef_w )
{
	Sint8 end = 0 ;

	if( !--ef_w->cg_ctr ){
		do {
			ef_w->cg_adrs = ef_w->cg_adrs + sizeof( EF_CHAR_DATA ) ;
			if( ( end = ef_w->cg_end_flag & CG_END ) ) ef_w->cg_adrs = ( Sint8 * )( ef_w->char_adrs + ( ( EF_CHAR_DATA * )ef_w->cg_adrs )->unk_00 ) ;
			EF_CG( ef_w ).data = *( EF_CHAR_DATA * )ef_w->cg_adrs ;
			EF_CG( ef_w ).unk_08 = 0 ;
			EF_CG( ef_w ).unk_0c = 0 ;
			EF_CG( ef_w ).unk_10 = 0 ;
		} while( !ef_w->cg_ctr ) ;
	}
	return end ;
}

/****************************************************************/
/*			Step a player's animation; negative when it ran		*/
/*			past its last frame									*/
/****************************************************************/
Sint8 pl_char_move( PLAYER_WORK *pl_w )
{
	Sint8 end = 0 ;

	if( !--pl_w->cg_ctr ){
		do {
			pl_w->cg_adrs += sizeof( CHAR_DATA ) ;
			if( ( end = pl_w->cg_end_flag & CG_END ) ) pl_w->cg_adrs = ( Sint8 * )( pl_w->char_adrs + ( ( CHAR_DATA * )pl_w->cg_adrs )->loop_ofs ) ;
			CG( pl_w ) = *( CHAR_DATA * )pl_w->cg_adrs ;
		} while( !pl_w->cg_ctr ) ;
		pl_w->frame_box = &pl_w->frame_box_tbl[( ( CHAR_DATA * )pl_w->cg_adrs )->frame_box_no] ;
		if( pl_w->cg_se ) cg_se_req( pl_w ) ;
	}
	return end ;
}

/****************************************************************/
/*			Set a player's animation							*/
/****************************************************************/
void pl_char_set( PLAYER_WORK *pl_w, Uint8 group, Uint8 no )
{
	Uint32 *tbl ;

	pl_w->char_no.b.grp = group ;
	pl_w->char_no.b.no = no ;
	tbl = ( Uint32 * )( pl_w->char_adrs + pl_w->char_ofs ) ;
	tbl = ( Uint32 * )( pl_w->char_adrs + tbl[group] ) ;
	pl_w->cg_adrs = ( Sint8 * )( tbl[no] + pl_w->char_adrs ) ;
	CG( pl_w ) = *( CHAR_DATA * )pl_w->cg_adrs ;
	if( !pl_w->cg_ctr ){
		do {
			pl_w->cg_adrs += sizeof( CHAR_DATA ) ;
			if( pl_w->cg_end_flag & CG_END ) pl_w->cg_adrs = ( Sint8 * )( pl_w->char_adrs + ( ( CHAR_DATA * )pl_w->cg_adrs )->loop_ofs ) ;
			CG( pl_w ) = *( CHAR_DATA * )pl_w->cg_adrs ;
		} while( !pl_w->cg_ctr ) ;
	}
	pl_w->frame_box = &pl_w->frame_box_tbl[( ( CHAR_DATA * )pl_w->cg_adrs )->frame_box_no] ;
	if( pl_w->cg_se ) cg_se_req( pl_w ) ;
}

/****************************************************************/
/*			Set a player's animation, starting at the given		*/
/*			frame; no indexes the group's table unsigned		*/
/****************************************************************/
void pl_char_set2( PLAYER_WORK *pl_w, Uint8 group, Sint8 no, int frame )
{
	Uint32 *tbl ;

	pl_w->char_no.b.grp = group ;
	pl_w->char_no.b.no = no ;
	tbl = ( Uint32 * )( pl_w->char_adrs + pl_w->char_ofs ) ;
	tbl = ( Uint32 * )( pl_w->char_adrs + tbl[group] ) ;
	pl_w->cg_adrs = ( Sint8 * )( tbl[( Uint8 )no] + pl_w->char_adrs ) ;
	pl_w->cg_adrs += frame * sizeof( CHAR_DATA ) ;
	CG( pl_w ) = *( CHAR_DATA * )pl_w->cg_adrs ;
	if( !pl_w->cg_ctr ){
		do {
			pl_w->cg_adrs += sizeof( CHAR_DATA ) ;
			if( pl_w->cg_end_flag & CG_END ) pl_w->cg_adrs = ( Sint8 * )( pl_w->char_adrs + ( ( CHAR_DATA * )pl_w->cg_adrs )->loop_ofs ) ;
			CG( pl_w ) = *( CHAR_DATA * )pl_w->cg_adrs ;
		} while( !pl_w->cg_ctr ) ;
	}
	pl_w->frame_box = &pl_w->frame_box_tbl[( ( CHAR_DATA * )pl_w->cg_adrs )->frame_box_no] ;
}

/****************************************************************/
/*			The first of the player's palette lines in its		*/
/*			colours, or the special set it is using				*/
/****************************************************************/
Uint16 *get_pl_col_adrs( PLAYER_WORK *pl_w )
{
	Uint8 *lw = ( Uint8 * )pl_w->tlocal ;
	Uint16 *adrs ;

	adrs = ( Uint16 * )pl_w->col_adrs + pl_w->pl_color * 128 ;
	switch( pl_w->pl_type ){
	case PL_ZANGIEF :
		if( pl_w->armor_flag ) adrs = EF_COL_ADRS( pl_w ) + ( pl_w->pl_color + 8 ) * 16 ;
		break ;
	case PL_SAKURA :
		if( pl_w->obj_flag ) adrs = EF_COL_ADRS( pl_w ) + ( pl_w->pl_color + 20 ) * 16 ;
		break ;
	case PL_JIN :
		if( lw[8] ) adrs = EF_COL_ADRS( pl_w ) + pl_w->pl_color * 6 * 16 ;
		break ;
	case PL_SONSON :
		if( pl_w->armor_flag ) adrs = EF_COL_ADRS( pl_w ) + ( pl_w->pl_color + 6 ) * 16 ;
		break ;
	}
	return adrs ;
}

/*==============================================================*/
/*			Copy cnt palette lines from data to the palette		*/
/*			from line on, and have all eight sent				*/
/*==============================================================*/
static void pl_col_copy( PLAYER_WORK *pl_w, void *line, void *data, int cnt )
{
	COL_WORK *col = line ;
	Uint16 *adrs = data ;
	Uint16 *dst ;
	int i, j ;

	i = 0 ;
	while( i < cnt ){
		col->trans_flag = 1 ;
		dst = col->col ;
		for( j = 0 ; j < 16 ; j ++ ) *dst ++ = *adrs ++ ;
		col ++ ;
		i ++ ;
	}
	for( ; i < 8 ; i ++, col ++ ) col->trans_flag = 1 ;
}

/****************************************************************/
/*			Set the player's palette lines: its colours,		*/
/*			flashes and the special sets						*/
/****************************************************************/
void pl_color_set( PLAYER_WORK *pl_w, int no )
{
	COL_WORK *col = &col_work[pl_w->col_no] ;
	Uint16 *adrs, *dst ;
	int i, j, cnt ;

	switch( no ){
	default :
	case PC_DEFAULT :
		pl_w->col_no = col_no_tbl[pl_w->work_no] ;
		adrs = get_pl_col_adrs( pl_w ) ;
		col->trans_flag = 1 ;
		dst = col->col ;
		for( j = 0 ; j < 16 ; j ++ ) *dst ++ = *adrs ++ ;
		col ++ ;
		i = pl_w->pl_color ;
		i = ( i * 8 + 1 ) * 16 ;
		adrs = ( Uint16 * )pl_w->col_adrs + i ;
		i = 1 ;
		while( i < 8 ){
			col->trans_flag = 1 ;
			dst = col->col ;
			for( j = 0 ; j < 16 ; j ++ ) *dst ++ = *adrs ++ ;
			col ++ ;
			i ++ ;
		}
		break ;
	case 8 :
		i = 0 ;
		while( i < pl_w->col_cnt ){
			col->trans_flag = 1 ;
			dst = &col->col[1] ;
			for( j = 1 ; j < 16 ; j ++ ) *dst ++ = 0xffff ;
			col ++ ;
			i ++ ;
		}
		break ;
	case 9 :
		i = 0 ;
		while( i < pl_w->col_cnt ){
			col->trans_flag = 1 ;
			dst = &col->col[1] ;
			for( j = 1 ; j < 16 ; j ++ ) *dst ++ = 0xff00 ;
			col ++ ;
			i ++ ;
		}
		break ;
	case 3 :
		if( pl_w->pl_type == PL_SONSON && pl_w->armor_flag ){
			adrs = EF_COL_ADRS( pl_w ) + ( pl_w->pl_color + 14 ) * 16 ;
			pl_col_copy( pl_w, col, adrs, pl_w->col_cnt ) ;
			break ;
		}
	case 4 :
	case 5 :
	case 6 :
		i = pl_w->col_cnt * ( ( no - 3 ) * 2 + ( Sys->game_timer0 & 1 ) ) * 16 + 0x300 ;
		adrs = ( Uint16 * )pl_w->col_adrs + i ;
		pl_col_copy( pl_w, col, adrs, pl_w->col_cnt ) ;
		break ;
	case 7 :
		i = pl_w->col_cnt * ( ( Sys->game_timer0 & 1 ) * 2 + 3 ) * 16 + 0x300 ;
		adrs = ( Uint16 * )pl_w->col_adrs + i ;
		pl_col_copy( pl_w, col, adrs, pl_w->col_cnt ) ;
		return ;
	case 2 :
		i = 0 ;
		while( i < 8 ){
			col->trans_flag = 2 ;
			col ++ ;
			i ++ ;
		}
		break ;
	case 10 :
		i = 0 ;
		while( i < 8 ){
			col->trans_flag = 1 ;
			col ++ ;
			i ++ ;
		}
		break ;
	case 1 :
		pl_w->col_no = col_no_tbl[pl_w->work_no] ;
		adrs = get_pl_col_adrs( pl_w ) ;
		switch( pl_w->pl_type ){
		case PL_SONSON :
		case PL_MEGAMAN :
		case PL_ROLL :
			cnt = pl_w->col_cnt + 2 ;
			break ;
		case PL_SHUMA_GORATH :
			cnt = pl_w->col_cnt + 1 ;
			break ;
		default :
			cnt = pl_w->col_cnt ;
			break ;
		}
		pl_col_copy( pl_w, col, adrs, cnt ) ;
		break ;
	}
}

/****************************************************************/
/*			Copy cnt of the player's palette lines, from		*/
/*			line col of its colours, to its lines from no on	*/
/****************************************************************/
void pl_color_trans( PLAYER_WORK *pl_w, int no, Uint16 col, int cnt )
{
	COL_WORK *cw = &col_work[pl_w->col_no + no] ;
	Uint16 *adrs = ( Uint16 * )( pl_w->col_adrs + ( pl_w->pl_color * 8 + col ) * 32 ) ;
	Uint16 *dst ;
	int i, j ;

	i = 0 ;
	while( i < cnt ){
		cw->trans_flag = 1 ;
		dst = cw->col ;
		for( j = 0 ; j < 16 ; j ++ ) *dst ++ = *adrs ++ ;
		cw ++ ;
		i ++ ;
	}
}

/****************************************************************/
/*			The same from the effects' palette lines, after		*/
/*			the special sets									*/
/****************************************************************/
void ef_color_set( PLAYER_WORK *pl_w, int no, Uint16 col, int cnt )
{
	COL_WORK *cw = &col_work[pl_w->col_no + no] ;
	Uint16 *adrs = EF_COL_ADRS( pl_w ) + col * 16 ;
	Uint16 *dst ;
	int i, j ;

	i = 0 ;
	while( i < cnt ){
		cw->trans_flag = 1 ;
		dst = cw->col ;
		for( j = 0 ; j < 16 ; j ++ ) *dst ++ = *adrs ++ ;
		cw ++ ;
		i ++ ;
	}
}

/****************************************************************/
/*			Unpack byte data: each bit of a flag byte, from		*/
/*			the top, says whether the next byte is a literal	*/
/*			or a copy of ( n & 15 ) + 2 bytes from ( n >> 4		*/
/*			) + 1 back											*/
/****************************************************************/
int lz_unpack( Sint8 *src, int size, Uint8 *dst )
{
	Uint8 *end = dst + size ;
	Uint8 *p ;
	int flag ;
	int bit = 0 ;
	Uint32 c ;
	int n ;

	do {
		if( !bit ){
			flag = *src ++ ;
			bit = 0x80 ;
		}
		if( flag & bit ){
			c = *src ++ & 0xff ;
			n = c & 15 ;
			p = dst - ( c >> 4 ) - 1 ;
			*dst ++ = *p ++ ;
			do {
				*dst ++ = *p ++ ;
			} while( n -- ) ;
		} else {
			*dst ++ = *src ++ ;
		}
		bit >>= 1 ;
	} while( dst < end ) ;
	return size ;
}

/****************************************************************/
/*			Unpack word data the same way: a copy is ( w >>		*/
/*			11 ) words, or the next word's count when that		*/
/*			is 0, from ( w & 0x7ff ) back, or of zeros when		*/
/*			that is 0; no count ends the data					*/
/****************************************************************/
void lz_unpack16( Sint16 *src, Uint16 *dst )
{
	Uint16 *p ;
	int flag ;
	int bit = 0 ;
	Uint32 w, n ;

	for( ;; ){
		if( !bit ){
			flag = *src ++ ;
			bit = 0x8000 ;
		}
		if( flag & bit ){
			w = *src ++ & 0xffff ;
			if( w >> 11 ){
				n = w >> 11 ;
				w = w & 0x7ff ;
			} else n = *src ++ & 0xffff ;
			if( !w ){
				if( !n ) return ;
				do {
					*dst ++ = 0 ;
				} while( --n ) ;
			} else {
				p = dst - w ;
				do {
					*dst ++ = *p ++ ;
				} while( --n ) ;
			}
		} else {
			*dst ++ = *src ++ ;
		}
		bit >>= 1 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pl_char.c" End of File	*/
