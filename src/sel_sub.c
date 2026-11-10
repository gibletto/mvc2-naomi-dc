/* The textures of the select scene and the rankings: faces, names and numbers built in work RAM
   from the texture file, and the requests that send them to the texture memory. */
#include "player.h"

#define FACE_TEX	( ( Uint8 * )0x0cdb0000 )	/* two 256 x 256 textures of 16 faces */
#define FACE2_TEX	( ( Uint8 * )0x0cdf0000 )	/* each side's big face */
#define NAME_PIC_TBL	( stream_tbl + 141 )	/* the files of the name pictures */


/* the order of the 64 x 64 cells in a 256 x 256 texture */
static const Sint8 cell_tbl[4][4] = { { 0, 2, 8, 10 }, { 1, 3, 9, 11 }, { 4, 6, 12, 14 }, { 5, 7, 13, 15 } } ;
/* each character's face in the texture file */
static const Sint8 face_tbl[64] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x18, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
	0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
	0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d,
} ;
/* each character's name, and with Sys->cpu_level_type set */
static const Sint8 name_tbl[60] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x18, 0x18, 0x19, 0x1a, 0x1b, 0x1c, 0x1d,
	0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
	0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x00,
} ;
static const Sint8 name2_tbl[60] = {
	0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,
	0x39, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x18, 0x18, 0x19, 0x3a, 0x1b, 0x3b, 0x3c,
	0x1e, 0x3d, 0x20, 0x21, 0x22, 0x23, 0x3e, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b, 0x2c, 0x2d,
	0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x3f, 0x00,
} ;
/* the 16 x 16 cells of a ranking's mark, four for each unk_00 */
static const Uint8 rank_mark_tbl[21 * 4] = {
	0x1f, 0x3f, 0x55, 0x60, 0x35, 0x57, 0x5d, 0x60, 0x37, 0x77, 0x5d, 0x60,
	0x5c, 0x55, 0x5f, 0x60, 0x5e, 0x55, 0x5f, 0x60, 0x74, 0x55, 0x5f, 0x60,
	0x76, 0x55, 0x5f, 0x60, 0x7c, 0x55, 0x5f, 0x60, 0x11, 0x55, 0x5f, 0x60,
	0x1f, 0x1d, 0x55, 0x5f, 0x1f, 0x1f, 0x55, 0x5f, 0x1f, 0x35, 0x55, 0x5f,
	0x1f, 0x37, 0x55, 0x5f, 0x1f, 0x5c, 0x55, 0x5f, 0x1f, 0x5e, 0x55, 0x5f,
	0x1f, 0x74, 0x55, 0x5f, 0x1f, 0x76, 0x55, 0x5f, 0x1f, 0x7c, 0x55, 0x5f,
	0x1f, 0x11, 0x55, 0x5f, 0x35, 0x1d, 0x55, 0x5f, 0x60, 0x60, 0x60, 0x60,
} ;
/* the name letters */
static const Uint8 letter_tbl[44] = {
	0x13, 0x19, 0x1b, 0x31, 0x33, 0x39, 0x3b, 0x51, 0x53, 0x59, 0x5b, 0x71, 0x73, 0x79, 0x7b, 0x10,
	0x12, 0x18, 0x1a, 0x30, 0x32, 0x38, 0x3a, 0x50, 0x52, 0x58, 0x7d, 0x75, 0x14, 0x16, 0x1c, 0x1e,
	0x34, 0x36, 0x3c, 0x3e, 0x54, 0x56, 0x17, 0x7e, 0x7f, 0x3d, 0x60, 0x7a,
} ;
/* the score's digits, then a blank */
static const Uint8 score_num_tbl[11] = { 0x1d, 0x1f, 0x35, 0x37, 0x5c, 0x5e, 0x74, 0x76, 0x7c, 0x11, 0x60 } ;
static const Uint8 rank_flag_tbl[2] = { 0x60, 0x15 } ;
/* the ranking's digits, a blank, and the letters after the count */
static const Uint8 rank_num_tbl[15] = {
	0x14, 0x16, 0x1c, 0x1e, 0x34, 0x36, 0x3c, 0x3e, 0x54, 0x56, 0x60, 0x5a, 0x70, 0x72, 0x78,
} ;
static const Uint8 win_num_tbl[15] = {
	0x04, 0x0c, 0x24, 0x2c, 0x44, 0x4c, 0x64, 0x6c, 0x00, 0x08, 0x60, 0x20, 0x28, 0x40, 0x48,
} ;
/* the places of a ranking line's 16 cells */
static const Uint8 rank_pos_tbl[16] = {
	0x00, 0x02, 0x08, 0x0a, 0x20, 0x22, 0x28, 0x2a, 0x40, 0x42, 0x48, 0x4a, 0x60, 0x62, 0x68, 0x6a,
} ;
/* each line's first cell */
static const Uint8 rank_line_tbl[8] = { 0x15, 0x14, 0x11, 0x10, 0x05, 0x04, 0x01, 0x00 } ;

/* the face's place in FACE_TEX for each player */
static Sint8 face_pos_tbl[6] = { 1, 5, 2, 6, 3, 7 } ;
/* the name's 32 x 32 cells */
static Sint8 name_cell_tbl[8] = { 0x00, 0x01, 0x04, 0x05, 0x10, 0x11, 0x14, 0x15 } ;
/* the places of the three hidden faces */
static Sint8 face3_pos_tbl[2][3] = { { 1, 2, 3 }, { 5, 6, 7 } } ;

/****************************************************************/
/*			Copy a player's face and name to the select			*/
/*			textures											*/
/****************************************************************/
void face_tex_set( int pl )
{
	int no, pos ;
	Uint8 *src_top, *dst_top ;
	Uint32 *src, *dst ;
	const Sint8 *tbl, *dp, *sp ;
	int i, j ;

	no = face_tbl[pl_work[pl].load_type] ;
	pos = face_pos_tbl[pl] ;
	src_top = Sys->tex_buff + ( no & 0xfc ) * 0x8000 ;
	dst_top = FACE_TEX + ( pos & 0xfc ) * 0x8000 ;
	tbl = cell_tbl[0] ;
	i = 0 ;
	dp = &tbl[pos & 3] ;
	sp = &tbl[no & 3] ;
	do {
		src = ( Uint32 * )( src_top + *sp * 0x2000 ) ;
		dst = ( Uint32 * )( dst_top + *dp * 0x2000 ) ;
		for( j = 0x800 ; j != 0 ; j -- ) *dst++ = *src++ ;
		dp = dp + 4 ;
		sp = sp + 4 ;
	} while( ++ i < 4 ) ;
	if( Sys->cpu_level_type ) no = name2_tbl[pl_work[pl].load_type] ;
	else no = name_tbl[pl_work[pl].load_type] ;
	src_top = Sys->tex_buff + ( no & 0xf8 ) * 0x4000 + name_cell_tbl[no & 7] * 0x1000 + 0x1e0000 ;
	dst_top = FACE_TEX + ( pl & 1 ) * 0x20000 ;
	tbl = name_cell_tbl ;
	for( i = 0 ; i < 8 ; i ++ ){
		src = ( Uint32 * )( src_top + tbl[i] * 0x800 ) ;
		dst = ( Uint32 * )( dst_top + tbl[i] * 0x800 ) ;
		for( j = 0x200 ; j != 0 ; j -- ) *dst++ = *src++ ;
	}
}

/****************************************************************/
/*			Copy the three hidden faces to a side's places		*/
/****************************************************************/
void face3_tex_set( int side )
{
	Uint8 *src_top, *dst_top ;
	Uint32 *src, *dst ;
	const Sint8 *dp, *sp ;
	const Sint8 *tbl ;
	int k, no, pos, i, j ;

	for( k = 0 ; k < 3 ; k ++ ){
		tbl = cell_tbl[0] ;
		no = k + 0x39 ;
		pos = face3_pos_tbl[side][k] ;
		src_top = Sys->tex_buff + ( no & 0xfc ) * 0x8000 ;
		dst_top = FACE_TEX + ( pos & 0xfc ) * 0x8000 ;
		i = 0 ;
		dp = &tbl[pos & 3] ;
		sp = &tbl[no & 3] ;
		for( ; i < 4 ; i ++ ){
			src = ( Uint32 * )( src_top + *sp * 0x2000 ) ;
			dst = ( Uint32 * )( dst_top + *dp * 0x2000 ) ;
			j = 0x800 ;
			do { *dst++ = *src++ ; j -- ; } while( j ) ;
			dp = dp + 4 ;
			sp = sp + 4 ;
		}
	}
}

/****************************************************************/
/*			Copy a player's big face to its side's texture		*/
/****************************************************************/
void face2_tex_set( int pl )
{
	int no = pl_work[pl].load_type ;
	Uint32 *src, *dst ;
	int j ;

	if( no >= 27 ) no -= 3 ;
	src = ( Uint32 * )( Sys->tex_buff + no * 0x4800 + 0x2e0000 ) ;
	dst = ( Uint32 * )( FACE2_TEX + ( pl & 1 ) * 0x4800 ) ;
	j = 0x1200 ;
	do *dst++ = *src++ ; while( -- j ) ;
}

/****************************************************************/
/*			Unpack a character's name picture for a side		*/
/****************************************************************/
void name_pic_set( int side, int no )
{
	const FILE_DATA *tbl ;
	Sint16 *src ;
	Uint16 *dst ;
	int n = no ;

	if( no >= 27 ) n -= 3 ;
	tbl = NAME_PIC_TBL ;
	src = ( Sint16 * )( tbl[n].adrs - tbl[0].adrs + 0x0c520000 ) ;
	dst = ( Uint16 * )( side * 0x20000 + 0x0c720000 ) ;
	lz_unpack16( src, dst ) ;
}

/****************************************************************/
/*			Draw a score of eleven digits, hi's low three		*/
/*			and lo's eight, in 16 x 16 cells; leading zeros		*/
/*			are left blank										*/
/****************************************************************/
void score_tex_set( Uint32 hi, Uint32 lo, Uint8 *dst_top, const Uint8 *pos )
{
	int flag = 0 ;
	Uint8 *tex = Sys->tex_buff ;
	Uint32 *src, *dst ;
	Uint32 num ;
	int no, i, j ;

	num = hi << 20 ;
	for( i = 0 ; i < 3 ; i ++ ){
		if( num & 0xf0000000 || flag ){
			flag = 1 ;
			no = num >> 28 & 15 ;
		} else no = 10 ;
		num <<= 4 ;
		src = ( Uint32 * )( tex + score_num_tbl[no] * 0x200 ) ;
		dst = ( Uint32 * )( dst_top + pos[i] * 0x200 ) ;
		for( j = 0 ; j < 128 ; j ++ ) *dst++ = *src++ ;
	}
	num = lo ;
	for( i = 3 ; i < 10 ; i ++ ){
		if( num & 0xf0000000 || flag ){
			flag = 1 ;
			no = num >> 28 & 15 ;
		} else no = 10 ;
		num <<= 4 ;
		src = ( Uint32 * )( tex + score_num_tbl[no] * 0x200 ) ;
		dst = ( Uint32 * )( dst_top + pos[i] * 0x200 ) ;
		for( j = 0 ; j < 128 ; j ++ ) *dst++ = *src++ ;
	}
	no = num >> 28 & 15 ;
	src = ( Uint32 * )( tex + score_num_tbl[no] * 0x200 ) ;
	dst = ( Uint32 * )( dst_top + pos[i] * 0x200 ) ;
	for( j = 0 ; j < 128 ; j ++ ) *dst++ = *src++ ;
}

/****************************************************************/
/*			Draw a count of wins in two digits, and the			*/
/*			plural's letter after them							*/
/****************************************************************/
void win_tex_set( Uint32 cnt, NLthd *thd )
{
	Uint8 *tex = Sys->tex_buff ;
	Uint32 *src, *dst ;
	int no, j ;

	no = cnt / 10 ;
	if( !no ) no = 10 ;
	src = ( Uint32 * )( tex + win_num_tbl[no] * 0x200 ) ;
	dst = thd[12].address ;
	for( j = 0 ; j < 0x200 ; j ++ ) *dst++ = *src++ ;
	src = ( Uint32 * )( tex + win_num_tbl[cnt % 10] * 0x200 ) ;
	dst = thd[14].address ;
	for( j = 0 ; j < 0x200 ; j ++ ) *dst++ = *src++ ;
	if( cnt > 1 ) no = 14 ;
	else no = 10 ;
	src = ( Uint32 * )( tex + win_num_tbl[no] * 0x200 ) ;
	dst = ( Uint32 * )thd[11].address + 0x400 ;
	for( j = 0 ; j < 0x200 ; j ++ ) *dst++ = *src++ ;
}

/****************************************************************/
/*			Draw place no of the ranking on line line, or		*/
/*			clear the line when no is negative					*/
/****************************************************************/
void rank_tex_set( int no, int line )
{
	Uint8 *tex ;
	Uint8 *dst_top ;
	RANK_DATA *rank ;
	Uint32 *src, *dst ;
	int i, j, n ;

	if( no < 0 ){
		dst_top = Sys->tex_buff + rank_line_tbl[line] * 0x200 + 0x20000 ;
		i = 0 ;
		do memset( dst_top + rank_pos_tbl[i] * 0x200, 0, 0x200 ) ; while( ++ i < 16 ) ;
		dst_top = dst_top + 0x10000 ;
		i = 0 ;
		do memset( dst_top + rank_pos_tbl[i] * 0x200, 0, 0x200 ) ; while( ++ i < 16 ) ;
		return ;
	}
	if( Sys->vs_flag ) rank = &rank_win[no] ;
	else rank = &rank_score[no] ;
	tex = Sys->tex_buff ;
	dst_top = Sys->tex_buff + rank_line_tbl[line] * 0x200 + 0x20000 ;
	for( i = 0 ; i < 4 ; i ++ ){
		src = ( Uint32 * )( tex + rank_mark_tbl[( rank->rank - 1 ) * 4 + i] * 0x200 ) ;
		dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
		j = 128 ;
		do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	}
	for( ; i < 7 ; i ++ ){
		src = ( Uint32 * )( tex + letter_tbl[rank->name[i - 4]] * 0x200 ) ;
		dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
		j = 128 ;
		do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	}
	src = ( Uint32 * )( tex + rank_flag_tbl[rank->ending_flag] * 0x200 ) ;
	dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
	j = 128 ;
	do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	i ++ ;
	if( !( n = rank->win_max / 10 ) ) n = 10 ;
	src = ( Uint32 * )( tex + rank_num_tbl[n] * 0x200 ) ;
	dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
	j = 128 ;
	do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	i ++ ;
	n = rank->win_max % 10 ;
	src = ( Uint32 * )( tex + rank_num_tbl[n] * 0x200 ) ;
	dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
	j = 128 ;
	do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	for( i ++ ; i < 13 ; i ++ ){
		src = ( Uint32 * )( tex + rank_num_tbl[n = i + 1] * 0x200 ) ;
		dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
		j = 128 ;
		do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	}
	if( rank->win_max > 1 ) n = 14 ;
	else n = 10 ;
	src = ( Uint32 * )( tex + rank_num_tbl[n] * 0x200 ) ;
	dst = ( Uint32 * )( dst_top + rank_pos_tbl[i] * 0x200 ) ;
copy :
	j = 128 ;
	do { *dst++ = *src++ ; j = j - 1 ; } while( j ) ;
	if( !Sys->vs_flag ) score_tex_set( rank->score_hi, rank->score, dst_top + 0x10000, rank_pos_tbl ) ;
}

/****************************************************************/
/*			Send the face textures								*/
/****************************************************************/
void face_tex_send( void )
{
	nlSetCommonTextureOffset( 0 ) ;
	nlCommonTextureReloadA( 0x810, ( Uint32 * )0x0cdb0000 ) ;
	nlCommonTextureReloadA( 0x81c, ( Uint32 * )0x0cdf0000 ) ;
}

/****************************************************************/
void face_tex_send_2p( void )
{
	nlSetCommonTextureOffset( 0 ) ;
	nlCommonTextureReloadA( 0x811, ( Uint32 * )0x0cdd0000 ) ;
	nlCommonTextureReloadA( 0x81d, ( Uint32 * )0x0cdf4800 ) ;
}

/****************************************************************/
/*			Unpack and send the player's first and second		*/
/*			pictures from its loaded file						*/
/****************************************************************/
void pl_pic_send( PLAYER_WORK *pl_w )
{
	Sint16 *src = ( Sint16 * )( pl_w->pic_adrs[1] + ( Uint8 * )pl_w->pic_adrs ) ;

	lz_unpack16( src, ( Uint16 * )0x0ce60000 ) ;
	nlSetCommonTextureOffset( 0 ) ;
	nlCommonTextureReloadA( 0xc99, ( Uint32 * )0x0ce60000 ) ;
}

/****************************************************************/
void pl_pic_send2( PLAYER_WORK *pl_w )
{
	Sint16 *src = ( Sint16 * )( pl_w->pic_adrs[2] + ( Uint8 * )pl_w->pic_adrs ) ;

	lz_unpack16( src, ( Uint16 * )0x0ce60000 ) ;
	nlSetCommonTextureOffset( 0 ) ;
	nlCommonTextureReloadA( ( pl_w->work_no >> 1 ) + 0xca6, ( Uint32 * )0x0ce60000 ) ;
}

/****************************************************************/
/*			Send the texture asked for in tex_req_no			*/
/****************************************************************/
void tex_req_send( void )
{
	if( tex_req_flag ){
		switch( tex_req_no ){
		case 1 :
			face_tex_send() ;
			break ;
		case 2 :
			face_tex_send_2p() ;
			break ;
		case 3 :
			face_tex_send() ;
			tex_req_flag = 1 ;
			tex_req_no = 2 ;
			return ;
		case 4 :
		case 5 :
			nlSetCommonTextureOffset( 0 ) ;
			nlCommonTextureReloadA( 0x81c, ( Uint32 * )( ( tex_req_no - 4 ) * 0x80000 + 0x0c420000 ) ) ;
			break ;
		case 6 :
		case 7 :
		case 8 :
		case 9 :
		case 10 :
			nlSetCommonTextureOffset( 0 ) ;
			nlCommonTextureReloadA( tex_req_no + 0x817, ( Uint32 * )( ( tex_req_no - 6 ) * 0x20000 + 0x0c720000 ) ) ;
			break ;
		case 11 :
		case 12 :
			nlSetCommonTextureOffset( 0 ) ;
			nlCommonTextureReloadA( 0x81d, ( Uint32 * )( Sys->tex_buff + 0x20000 ) ) ;
			nlCommonTextureReloadA( 0x81f, ( Uint32 * )( Sys->tex_buff + 0x30000 ) ) ;
			break ;
		case 13 : case 14 : case 15 : case 16 : case 17 : case 18 : case 19 : case 20 :
		case 21 : case 22 : case 23 : case 24 : case 25 : case 26 : case 27 : case 28 :
			nlSetCommonTextureOffset( 0 ) ;
			nlCommonTextureReloadA( 0x910 + ( ( tex_req_no - 13 ) & 1 ), ( Uint32 * )( Sys->tex_buff + ( tex_req_no - 13 ) * 0x20000 ) ) ;
			break ;
		case 29 :
			nlSetCommonTextureOffset( 0 ) ;
			nlCommonTextureReloadA( 0x914, ( Uint32 * )( stage_no * 0x8000 + 0x0cc60000 ) ) ;
			break ;
		}
	}
	tex_req_flag = 0 ;
	tex_req_no = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "sel_sub.c" End of File	*/
