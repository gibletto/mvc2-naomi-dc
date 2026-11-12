/* DM0D, the player's data card: the handle and ID, the wins and losses, the points and the time
   played, with the helpers that write numbers and wide letters for the lobby's screens. */
#include "player.h"
#include "nl.h"
#include "dm0d.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

extern int sprintf( char *s, const char *fmt, ... ) ;
extern unsigned long strlen( const char *s ) ;

void data_move( PLAYER_WORK *w ) ;
void data_parts( PLAYER_WORK *w ) ;
void data_cursor( PLAYER_WORK *w ) ;
void data_blink( PLAYER_WORK *w ) ;
void data_rank( PLAYER_WORK *w ) ;
void data_put( PLAYER_WORK *w ) ;
void str_wide_set( char *src, char *dst ) ;
int num_bcd_get( int num ) ;

typedef struct DATA_PARTS {
	Uint8 pos, rot, scale ;
	Sint32 obj ;
	Uint32 flag ;
} DATA_PARTS ;

static DATA_PARTS data_parts_tbl[8] = {
	{ 1, 1, 1, 100, OBJ_POS | OBJ_ROT_Z | OBJ_SCALE },
	{ 2, 0, 0, 50, OBJ_POS },
	{ 3, 0, 2, 178, OBJ_POS | OBJ_SCALE },
	{ 0, 0, 0, 0x120, OBJ_POS },
	{ 0, 0, 0, 0x121, OBJ_POS },
	{ 0, 0, 0, 0x123, OBJ_POS },
	{ 0, 0, 0, 0x125, OBJ_POS },
	{ 0, 0, 0, 0x124, OBJ_POS },
} ;
static VEC3 data_pos[4] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 8.4f, 3.0f, -13.0f },
	{ 9.03f, -6.788f, -12.79f },
	{ 3.6944f, -3.0614f, -4.472f },
} ;
static Sint32 data_rot[2][3] = { { 0, 0, 0 }, { 0, 0, -180 } } ;
static VEC3 data_scale[3] = {
	{ 1.0f, 1.0f, 1.0f },
	{ 1.6f, 1.9f, 1.8f },
	{ 0.4f, 0.4f, 0.4f },
} ;

/****************************************************************/
/*			The card's works: type picks the routine			*/
/****************************************************************/
void dm0d_data_set( void )
{
	static Sint8 data_list[13][2] = {
		{ 0, 0 }, { 0, 1 }, { 0, 2 }, { 0, 3 }, { 0, 4 }, { 0, 5 }, { 0, 6 },
		{ 1, 0 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 0, 7 }, { -1, 0 }
	} ;
	PLAYER_WORK *w ;
	register int i ;

	for( i = 0 ; data_list[i][0] >= 0 ; i ++ ){
		if( ( w = work_get( 0, W_SYS, LIST_END ) ) ){
			w->work_func = data_move ;
			w->type = data_list[i][0] ;
			w->sub_type = data_list[i][1] ;
		}
	}
	win_pict_set( 7 ) ;
}

void data_move( PLAYER_WORK *w )
{
	static void ( *data_jmp[5] )( PLAYER_WORK *w ) = {
		data_parts, data_cursor, data_blink, data_rank, data_put
	} ;

	data_jmp[w->type]( w ) ;
}

/* a still picture of the card, from the table */
void data_parts( PLAYER_WORK *w )
{
	DATA_PARTS *p ;

	if( !w->routine_0 ){
		p = &data_parts_tbl[w->sub_type] ;
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
		w->pos = data_pos[p->pos] ;
		w->rot[0] = DEG_ANG( data_rot[p->rot][0] ) ;
		w->rot[1] = DEG_ANG( data_rot[p->rot][1] ) ;
		w->rot[2] = DEG_ANG( data_rot[p->rot][2] ) ;
		w->scale = data_scale[p->scale] ;
		w->obj_flag = p->flag ;
		w->obj = obj_dm0d->obj[p->obj] ;
	}
}

/* the cursor, pulsing */
void data_cursor( PLAYER_WORK *w )
{
	static VEC3 data_cursor_pos = { 3.5438f, -3.0614f, -4.472f } ;

	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
		w->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
		w->scale = data_scale[2] ;
		w->pos = data_cursor_pos ;
		w->obj = obj_dm0d->obj[175] ;
		w->timer1 = 0 ;
		w->fade.trnsl = 1.0f ;
	}
	w->timer1 += 9 ;
	if( w->timer1 >= 360 ) w->timer1 = 0 ;
	w->fade.r = w->fade.g = w->fade.b = nlCos( DEG_ANG( w->timer1 ) ) * 0.5f + 0.5f ;
}

/* a picture that flickers every other frame */
void data_blink( PLAYER_WORK *w )
{
	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
		w->timer0 = 0 ;
		w->obj_flag = OBJ_TRNSL ;
		w->obj = obj_dm0d->obj[290] ;
		w->timer0 = 0 ;
	}
	w->timer0 ++ ;
	if( w->timer0 & 1 ) w->fade.trnsl = 0.8f ;
	else w->fade.trnsl = 1.0f ;
}

/* the player's rank mark */
void data_rank( PLAYER_WORK *w )
{
	static VEC3 data_rank_pos = { 8.12f, 2.1f, -12.6f } ;

	if( !w->routine_0 ){
		w->routine_0 ++ ;
		w->disp_flag = 1 ;
		w->timer0 = 0 ;
		w->obj_flag = OBJ_POS | OBJ_ROT_X ;
		w->pos = data_rank_pos ;
		w->obj = obj_dm0d->obj[7 - net_rank + 108] ;
	}
}

/* the card's text */
void data_put( PLAYER_WORK *w )
{
	char buf[32], wide[32] ;
	register int n ;

	sprintf( buf, "%s", net_handle ) ;
	str_put( 440.0f, 38.0f, 11.91f, 0xffffffff, buf ) ;
	sprintf( buf, "%s", net_file_key ) ;
	str_wide_set( buf, wide ) ;
	str_put( 80.0f, 262.0f, 11.91f, 0xffffffff, wide ) ;

	sprintf( buf, "%4d", net_win_cnt + net_lose_cnt ) ;
	str_wide_set( buf, wide ) ;
	str_put( 62.0f, 116.0f, 11.91f, 0xff0affff, wide ) ;
	sprintf( buf, "%4d", net_win_cnt ) ;
	str_wide_set( buf, wide ) ;
	str_put( 176.0f, 116.0f, 11.91f, 0xff0affff, wide ) ;
	sprintf( buf, "%4d", net_lose_cnt ) ;
	str_wide_set( buf, wide ) ;
	str_put( 284.0f, 116.0f, 11.91f, 0xff0affff, wide ) ;

	sprintf( buf, "%4d", unk_8C2152A0 + unk_8C2152A2 ) ;
	str_wide_set( buf, wide ) ;
	str_put( 62.0f, 200.0f, 11.91f, 0xff0affff, wide ) ;
	sprintf( buf, "%4d", unk_8C2152A0 ) ;
	str_wide_set( buf, wide ) ;
	str_put( 176.0f, 200.0f, 11.91f, 0xff0affff, wide ) ;
	sprintf( buf, "%4d", unk_8C2152A2 ) ;
	str_wide_set( buf, wide ) ;
	str_put( 284.0f, 200.0f, 11.91f, 0xff0affff, wide ) ;

	num_comma_set( unk_8C213AF8, buf ) ;
	str_put( 150.0f, 332.0f, 14.45f, 0xffffa00a, buf ) ;
	num_comma_set( net_entry_num, buf ) ;
	str_put( 450.0f, 332.0f, 14.45f, 0xffffa00a, buf ) ;

	n = ( Uint32 )net_play_time / ( 60 * 60 ) ;
	if( n > 9999 ) n = 9999 ;
	sprintf( buf, "%4d", n ) ;
	str_put( 198.0f, 400.0f, 14.45f, 0xffffffff, buf ) ;
	sprintf( buf, "%2d", ( Uint32 )net_play_time % ( 60 * 60 ) / 60 ) ;
	str_put( 300.0f, 400.0f, 14.45f, 0xffffffff, buf ) ;
}

/****************************************************************/
/*			Helpers for the lobby's text						*/
/****************************************************************/
/* turns letters into their full-width ones; two-byte letters are left out */
void str_wide_set( char *src, char *dst )
{
	static char *wide_tbl[96] = {
		"\x81\x40", "\x81\x49", "\x81\x68", "\x81\x94", "\x81\x90", "\x81\x93", "\x81\x95", "\x81\x66",
		"\x81\x69", "\x81\x6a", "\x81\x96", "\x81\x7b", "\x81\x43", "\x81\x7c", "\x81\x44", "\x81\x5e",
		"\x82\x4f", "\x82\x50", "\x82\x51", "\x82\x52", "\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56",
		"\x82\x57", "\x82\x58", "\x81\x46", "\x81\x47", "\x81\x83", "\x81\x81", "\x81\x84", "\x81\x48",
		"\x81\x97", "\x82\x60", "\x82\x61", "\x82\x62", "\x82\x63", "\x82\x64", "\x82\x65", "\x82\x66",
		"\x82\x67", "\x82\x68", "\x82\x69", "\x82\x6a", "\x82\x6b", "\x82\x6c", "\x82\x6d", "\x82\x6e",
		"\x82\x6f", "\x82\x70", "\x82\x71", "\x82\x72", "\x82\x73", "\x82\x74", "\x82\x75", "\x82\x76",
		"\x82\x77", "\x82\x78", "\x82\x79", "\x81\x6d", "\x81\x8f", "\x81\x6e", "\x81\x40", "\x81\x51",
		"\x81\x40", "\x82\x81", "\x82\x82", "\x82\x83", "\x82\x84", "\x82\x85", "\x82\x86", "\x82\x87",
		"\x82\x88", "\x82\x89", "\x82\x8a", "\x82\x8b", "\x82\x8c", "\x82\x8d", "\x82\x8e", "\x82\x8f",
		"\x82\x90", "\x82\x91", "\x82\x92", "\x82\x93", "\x82\x94", "\x82\x95", "\x82\x96", "\x82\x97",
		"\x82\x98", "\x82\x99", "\x82\x9a", "\x81\x6f", "\x81\x62", "\x81\x70", "\x81\x40", "\x81\x40",
	} ;
	char *p ;
	register int i, len, c ;

	len = strlen( p = src ) ;
	for( i = 0 ; i < len ; i ++ ){
		c = *p ++ ;
		if( !( c & 0x80 ) ){
			c -= ' ' ;
			*dst ++ = wide_tbl[c][0] ;
			*dst ++ = wide_tbl[c][1] ;
		} else {
			p ++ ;
		}
	}
	*dst = '\0' ;
}

/* a number up to 9,999,999, right-aligned in nine places with commas */
void num_comma_set( int num, char *buf )
{
	static char num_char[] = "0123456789" ;
	char *p ;
	register int i, bcd ;

	if( ( bcd = num ) > 9999999 ) bcd = 9999999 ;
	bcd = num_bcd_get( bcd ) ;
	sprintf( buf, "         " ) ;
	p = buf + 8 ;
	for( i = 0 ; i < 9 ; i ++ ){
		if( i == 3 || i == 7 ){
			*p -- = ',' ;
		} else {
			*p -- = num_char[bcd & 15] ;
			if( !( bcd >>= 4 ) ) break ;
		}
	}
}

int num_bcd_get( int num )
{
	int bcd, shift ;

	bcd = 0 ;
	shift = 0 ;
	do {
		bcd += num % 10 << shift ;
		shift += 4 ;
	} while( ( num /= 10 ) ) ;
	return bcd ;
}

/* reads a number written by num_comma_set; -1 if it is not one */
int str_num_get( char *str )
{
	char *p ;
	register int len, i, n, place, c ;

	len = strlen( p = str ) ;
	p = p + len - 1 ;
	for( i = 0, n = 0, place = 0 ; i < len ; i ++ ){
		c = *p -- ;
		if( c & 0x80 ) goto err ;
		if( c == ',' ) continue ;
		if( c == ' ' ){
			c = '0' ;
		} else {
			if( c < '0' ) goto err ;
			if( c > '9' ) goto err ;
		}
		c -= '0' ;
		if( !place ) n += c ;
		else n += c * nlPow( 10.0f, place ) ;
		place ++ ;
	}
	return n ;
err :
	return -1 ;
}
