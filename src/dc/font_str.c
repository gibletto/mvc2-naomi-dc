/* The network's text: strings drawn through the font's glyph cache, and the lobby's message log. */
#include "player.h"
#include "nl.h"
#include "dm.h"

extern unsigned long strlen( const char *s ) ;
extern char *strcpy( char *s1, const char *s2 ) ;
extern int strcmp( const char *s1, const char *s2 ) ;
extern void *memset( void *s, int c, unsigned long n ) ;

static char *str_now ;

/* the full-width signs for the ASCII ones, the low byte after 0x81 */
static Uint8 sjis_sign0[16] = {	/* ' ' to '/' */
	0x40, 0x49, 0x68, 0x94, 0x90, 0x93, 0x95, 0x66, 0x69, 0x6a, 0x96, 0x7b, 0x43, 0x7c, 0x44, 0x5e
} ;
static Uint8 sjis_sign1[8] = { 0x46, 0x47, 0x83, 0x81, 0x84, 0x48, 0x97, 0x40 } ;	/* ':' to '@' */
static Uint8 sjis_sign2[6] = { 0x6d, 0x8f, 0x6e, 0x4f, 0x51, 0x4d } ;	/* '[' to '`' */
static Uint8 sjis_sign3[4] = { 0x6f, 0x62, 0x70, 0x60 } ;	/* '{' to '~' */

/****************************************************************/
/*			The glyph cache										*/
/****************************************************************/
/* the buffer's slot of a character in a font and colour, or -1 */
int font_cache_find( Sint16 code, int no, int col )
{
	int i ;

	for( i = 0 ; i < FONT_SLOT ; i ++ ){
		if( font_w->code[font_w->buf][i] == code && font_w->font[font_w->buf][i] == no
		 && font_w->col[font_w->buf][i] == col ) return i ;
	}
	return -1 ;
}

/* a slot to make a glyph in: an empty one, or one made and not drawn this frame */
int font_cache_get( void )
{
	int i, n ;

	for( i = 0 ; i < FONT_SLOT ; i ++ ){
		n = ( font_w->next[font_w->buf] + i ) % FONT_SLOT ;
		if( !font_w->code[font_w->buf][n]
		 || ( !( font_w->flag[font_w->buf][n] & 1 ) && ( font_w->flag[font_w->buf][n] & 4 ) ) ){
			font_w->next[font_w->buf] ++ ;
			font_w->next[font_w->buf] %= FONT_SLOT ;
			return n ;
		}
	}
	return -1 ;
}

/* 1 unless c starts a two-byte Shift-JIS character */
int sjis_1byte_ck( Uint8 c )
{
	if( c >= 0x80 && c <= 0x9f ) return 0 ;
	if( c >= 0xe0 && c <= 0xff ) return 0 ;
	return 1 ;
}

/* a colour's number in font_col_tbl, for the colours given as ARGB */
int font_col_get( Uint32 argb )
{
	if( argb == 0xffffffff ) return 0 ;
	if( argb == 0xff000000 ) return 1 ;
	if( argb == 0xffff0a0a ) return 2 ;
	if( argb == 0xff0affff ) return 3 ;
	if( argb == 0xffffff0a ) return 4 ;
	if( argb == 0xffffa00a ) return 5 ;
	return 0 ;
}

/* the full-width character for an ASCII one */
int ascii_to_sjis( Uint8 c )
{
	if( c >= ' ' && c < '0' ) return 0x8100 + sjis_sign0[c - ' '] ;
	if( c >= '0' && c <= '9' ) return 0x821fu + c ;
	if( c > '9' && c < 'A' ) return 0x8100 + sjis_sign1[c - ':'] ;
	if( c >= 'A' && c <= 'Z' ) return 0x821fu + c ;
	if( c > 'Z' && c < 'a' ) return 0x8100 + sjis_sign2[c - '['] ;
	if( c >= 'a' && c <= 'z' ) return 0x8220u + c ;
	if( c > 'z' && c <= '~' ) return 0x8100 + sjis_sign3[c - '{'] ;
	return 0x8140 ;
}

/****************************************************************/
/*			Strings												*/
/****************************************************************/
/* a string at x, y: its ASCII half-width in font 0, the rest in font 1, each glyph a sprite */
void str_put( Float x, Float y, Float z, Uint32 col, char *str )
{
	NLsprarg spr ;
	int len, i, no, n ;
	int hi, lo ;
	char *p ;
	Sint16 code ;
	char c ;
	Float w, h ;

	if( col > 10 ) col = font_col_get( col ) ;
	x += option_w.adjust_x + 0.0f ;
	y += option_w.adjust_y + 0.0f ;
	spr.y = y ;
	spr.v0 = 0.0f ;
	spr.u0 = 0.0f ;
	spr.ang = 0 ;
	spr.trnsl = 1.0f ;
	spr.listType = -1 ;
	spr.attr = 37 ;
	spr.base_color = 0 ;
	spr.offset_color = 0 ;
	p = str ;
	len = strlen( p ) ;
	str_now = str ;
	for( i = 0 ; i < len ; i ++ ){
		c = *p ++ ;
		if( sjis_1byte_ck( c ) ){
			hi = 0 ;
			lo = ascii_to_sjis( c ) ;
			no = 0 ;
		} else {
			hi = c << 8 ;
			lo = ( Uint8 )*p ++ ;
			i ++ ;
			no = 1 ;
		}
		spr.u1 = spr.v1 = font_w->rows[no] / 32.0f ;
		code = hi | lo ;
		n = font_cache_find( code, no, col ) ;
		if( n < 0 ){
			n = font_cache_get() ;
			if( n < 0 ) return ;
			font_w->code[font_w->buf][n] = code ;
			font_w->flag[font_w->buf][n] |= 2 ;
			font_w->flag[font_w->buf][n] &= ~4 ;
			font_w->font[font_w->buf][n] = no ;
			font_w->col[font_w->buf][n] = col ;
		}
		font_w->flag[font_w->buf][n] |= 1 ;
		spr.sprno = FONT_TEX_NO + font_w->buf * FONT_SLOT + n ;
		spr.x = x ;
		spr.z = z + i * 0.0001f ;
		w = font_w->rows[no] ;
		h = w / 32.0f ;
		if( !no ){
			spr.zm_x = w / 1.5f / 32.0f ;
			spr.zm_y = h ;
			x += w / 1.5f ;
		} else {
			spr.zm_y = h ;
			x += w ;
			spr.zm_x = h ;
		}
		nlSprPut( &spr ) ;
	}
}

/* the same in font 0 alone, set closer: the log's lines */
void str_put2( Float x, Float y, Float z, Uint32 col, char *str )
{
	NLsprarg spr ;
	int len, i, no, n ;
	int hi, lo ;
	char *p ;
	Sint16 code ;
	char c ;
	Float w, h ;

	x += option_w.adjust_x + 0.0f ;
	y += option_w.adjust_y + 0.0f ;
	spr.y = y ;
	spr.v0 = 0.0f ;
	spr.u0 = 0.0f ;
	spr.ang = 0 ;
	spr.trnsl = 1.0f ;
	spr.listType = -1 ;
	spr.attr = 37 ;
	spr.base_color = 0 ;
	spr.offset_color = 0 ;
	p = str ;
	len = strlen( str ) ;
	for( i = 0 ; i < len ; i ++ ){
		c = *p ++ ;
		if( sjis_1byte_ck( c ) ){
			hi = 0 ;
			lo = ascii_to_sjis( c ) ;
			no = 0 ;
		} else {
			hi = c << 8 ;
			lo = ( Uint8 )*p ++ ;
			i ++ ;
			no = 1 ;
		}
		spr.u1 = spr.v1 = font_w->rows[0] / 32.0f ;
		code = hi | lo ;
		n = font_cache_find( code, 0, col ) ;
		if( n < 0 ){
			n = font_cache_get() ;
			if( n < 0 ) return ;
			font_w->code[font_w->buf][n] = code ;
			font_w->flag[font_w->buf][n] |= 2 ;
			font_w->flag[font_w->buf][n] &= ~4 ;
			font_w->font[font_w->buf][n] = 0 ;
			font_w->col[font_w->buf][n] = col ;
		}
		font_w->flag[font_w->buf][n] |= 1 ;
		spr.sprno = FONT_TEX_NO + font_w->buf * FONT_SLOT + n ;
		spr.x = x ;
		spr.z = z + i * 0.0001f ;
		w = font_w->rows[0] ;
		h = w / 32.0f ;
		if( !no ){
			spr.zm_x = w * 0.5f / 32.0f ;
			spr.zm_y = h ;
			x += w * 0.5f - 0.2f ;
		} else {
			spr.zm_x = spr.zm_y = h ;
			x += w - 0.4f ;
		}
		nlSprPut( &spr ) ;
	}
}

/****************************************************************/
/*			The message log										*/
/****************************************************************/
/* the log's next line; once it is full, the oldest goes */
void log_next( void )
{
	int top ;

	font_w->log.w ++ ;
	if( font_w->log.w >= LOG_LINE ) font_w->log.w -= LOG_LINE ;
	if( font_w->log.w == font_w->log.top ){
		top = font_w->log.top ;
		font_w->log.top ++ ;
		if( font_w->log.top >= LOG_LINE ) font_w->log.top -= LOG_LINE ;
		if( log_scroll && top == font_w->log.view ) font_w->log.view = font_w->log.top ;
	}
}

/* a message, over two lines when it is too long for one */
void log_add( char *tag, char *name, char *text )
{
	char buf[43] ;
	int len, max, i, n ;

	if( 0 ){	/* left out of the build */
		strcpy( name, tag ) ;
		buf[0] = 0 ;
	}
	strcpy( font_w->log.tag[font_w->log.w], tag ) ;
	strcpy( font_w->log.name[font_w->log.w], name ) ;
	if( *tag != 0 ){
		len = strlen( name ) + strlen( text ) + 8 ;
		n = 26 - strlen( name ) ;
	} else {
		len = strlen( name ) + strlen( text ) + 1 ;
		n = 33 - strlen( name ) ;
	}
	if( len > 34 ){
		i = 0 ;
		max = n ;
		do {
			if( !sjis_1byte_ck( *text ) ){
				if( i + 1 >= max ) break ;
				buf[i ++] = *text ++ ;
			}
			buf[i ++] = *text ++ ;
			continue ;
		} while( i < max ) ;
		buf[i] = 0 ;
		strcpy( font_w->log.text[font_w->log.w], buf ) ;
		log_next() ;
		font_w->log.name[font_w->log.w][0] = 0 ;
	}
	strcpy( font_w->log.text[font_w->log.w], text ) ;
	log_next() ;
}

void log_clear( void )
{
	memset( &font_w->log, 0, sizeof( font_w->log ) ) ;
}

/* the log's 18 lines; while it scrolls, up and down move them */
void log_disp( void )
{
	Float w, dy, tag_w, sep_w ;
	Float x, y, z, len ;
	Uint16 sw ;
	int i, line, n ;

	w = dy = font_w->rows[0] ;
	w -= 0.2f ;
	dy += 0.2f ;
	z = 15.0f ;
	if( log_scroll ){
		sw = menu_sw_get( 3 ) ;
		if( sw & 0x2000 ){
			if( font_w->log.top != font_w->log.view ){
				n = font_w->log.view - 1 ;
				if( n < 0 ) n += LOG_LINE ;
				font_w->log.view = n ;
			}
		} else if( sw & 0x1000 ){
			n = font_w->log.w - font_w->log.view ;
			if( n < 0 ) n += LOG_LINE ;
			if( n > 18 ){
				font_w->log.view ++ ;
				if( font_w->log.view == LOG_LINE ) font_w->log.view = 0 ;
			}
		}
		i = 0 ;
		line = font_w->log.view ;
	} else {
		n = font_w->log.w - font_w->log.top ;
		if( n < 0 ) n += LOG_LINE ;
		if( n < 18 ){
			font_w->log.view = font_w->log.top ;
		} else {
			n = font_w->log.w - 18 ;
			if( n < 0 ) n += LOG_LINE ;
			font_w->log.view = n ;
		}
		i = 0 ;
		line = font_w->log.view ;
	}
	tag_w = w * 3.0f ;
	sep_w = w / 2.0f ;
	while( i < 18 ){
		x = 46.0f ;
		if( font_w->log.name[line][0] != 0 ){
			if( font_w->log.tag[line][0] ){
				str_put2( x, 87.0f + dy * i, z, 8, font_w->log.tag[line] ) ;
				x += tag_w ;
				if( 0 ){	/* left out of the build */
					dy = w ;
					line = 0 ;
				}
			}
			if( !strcmp( font_w->log.name[line], ( char * )net_handle ) ){
				str_put2( x, 87.0f + dy * i, z, 7, font_w->log.name[line] ) ;
			} else {
				str_put2( x, 87.0f + dy * i, z, 9, font_w->log.name[line] ) ;
			}
			len = strlen( font_w->log.name[line] ) ;
			x += w * ( len / 2.0f ) ;
			if( 0 ) i = line ;	/* left out of the build */
		}
		y = 87.0f + dy * i ;
		str_put2( x, y, z, 6, " " ) ;
		x += sep_w ;
		font_w->log.text[line][42] = 0 ;
		if( !log_scroll ){
			str_put2( x, y, z, 6, font_w->log.text[line] ) ;
		} else {
			str_put2( x, y, z, 8, font_w->log.text[line] ) ;
		}
		line ++ ;
		i ++ ;
		if( line >= LOG_LINE ) line = 0 ;
	}
}
