/* The network's text font: the console's fonts loaded whole, and a cache of their glyphs made
   into common textures as they are needed, two buffers of FONT_SLOT taken in turn. */
#include "player.h"
#include "nl.h"
#include "shinobi.h"

#define FONT_ADRS	( ( FONT_WORK * )0x0c420000 )
#define FONT_PIXEL	( ( void * )0x0c678000 )	/* the glyph textures' pixels */

extern Uint32 file_load( Sint32 no, void *adrs, Sint32 a ) ;

FONT_WORK *font_w ;

/* the four shades of a glyph's pixels in each of its colours */
static Uint16 font_col_tbl[11][4] = {
	{ 0x0000, 0xd28c, 0xe314, 0xf39c }, { 0x0000, 0xd294, 0xb18c, 0x9084 }, { 0x0000, 0xd000, 0xe000, 0xf000 },
	{ 0x0000, 0x818c, 0x8294, 0x839c }, { 0x0000, 0xb180, 0xd280, 0xf380 }, { 0x0000, 0xb0c0, 0xd140, 0xf1c0 },
	{ 0x8000, 0xb18c, 0xd294, 0xf39c }, { 0x8000, 0xb000, 0xd000, 0xf000 }, { 0x8000, 0x818c, 0x8294, 0x839c },
	{ 0x8000, 0xb180, 0xd280, 0xf380 }, { 0x8000, 0xb0c0, 0xd140, 0xf1c0 }
} ;
static Sint32 font_rows_tbl[4] = { 18, 20, 24, 26 } ;	/* S18RM04.BIN, S20RM04.BIN, S24RM04.BIN, S26RM04.BIN */
static Sint32 font_bytes_tbl[4] = { 5, 5, 6, 7 } ;
static NLthd font_thd[1] = { { 32, 32, 0x900, FONT_PIXEL, 0 } } ;
static Sint32 font_unk[4] = { -1, -1, -1, -1 } ;

void font_file_load( Uint8 *adrs, int no )
{
	no &= 3 ;
	font_w->rows[no] = font_rows_tbl[no] ;
	font_w->bytes[no] = font_bytes_tbl[no] ;
	font_w->size[no] = font_w->rows[no] * font_w->bytes[no] + 16 ;
	font_w->adrs[no] = adrs ;
	file_load( 0x2c1 + no, font_w->adrs[no], 0 ) ;
}

void font_init( void )
{
	int i ;

	font_w = FONT_ADRS ;
	memset( font_w, 0, sizeof( FONT_WORK ) ) ;
	font_file_load( ( Uint8 * )0x0c4c8000, 0 ) ;
	font_file_load( ( Uint8 * )0x0c598000, 1 ) ;
	memset( FONT_PIXEL, 0, 0x800 ) ;
	for( i = 0 ; i < FONT_SLOT * 2 ; i ++ ){
		nlSetCommonTextureOffset( FONT_TEX_NO + i ) ;
		nlSpriteLoadTextureOnce( font_thd, NULL, 0 ) ;
	}
	memset( font_w->code, 0, sizeof( font_w->code ) ) ;
	memset( font_w->flag, 0, sizeof( font_w->flag ) ) ;
	font_w->next[0] = 0 ;
	font_w->next[1] = 0 ;
	font_w->buf = 0 ;
	memset( &font_w->log, 0, sizeof( font_w->log ) ) ;
}

void font_exit( void )
{
	int i ;

	for( i = 0 ; i < FONT_SLOT * 2 ; i ++ ){
		nlSetCommonTextureOffset( FONT_TEX_NO + i ) ;
		nlSpriteFreeTextureOnce( font_thd, NULL, 0 ) ;
	}
}

/* a character's glyph in the font, which runs in JIS order */
int font_sjis_index( Sint16 code )
{
	Sint16 jis ;
	Uint8 hi, lo ;

	jis = syBtFntSjis2Jis( code ) ;
	hi = jis >> 8 ;
	lo = jis ;
	return ( Sint16 )( hi - 0x21 ) * 94 + lo - 0x21 ;
}

/* a glyph's two-bit pixels in a colour's shades, into rows of 32 */
void font_char_make( Sint16 index, Uint16 *dst, int no, int col )
{
	Uint8 *src ;
	Uint16 *d ;
	Uint16 pal[4] ;
	Uint8 c ;
	int x, y, i, sh, mask, v ;

	pal[0] = font_col_tbl[col][0] ;
	pal[1] = font_col_tbl[col][1] ;
	pal[2] = font_col_tbl[col][2] ;
	pal[3] = font_col_tbl[col][3] ;
	src = font_w->adrs[no] + font_w->size[no] * index + 16 ;
	for( y = 0 ; y < font_w->rows[no] ; y ++ ){
		d = dst ;
		for( x = 0 ; x < font_w->bytes[no] ; x ++ ){
			c = *src ++ ;
			sh = 6 ;
			mask = 0xc0 ;
			for( i = 4 ; i ; i -- ){
				v = ( c & mask ) >> sh ;
				if( !v -- ) *d = pal[0] ;
				else if( !v -- ) *d = pal[1] ;
				else if( !v ) *d = pal[2] ;
				else *d = pal[3] ;
				d ++ ;
				mask >>= 2 ;
				sh -= 2 ;
			}
		}
		dst += 32 ;
	}
}

/* each frame: the glyphs the buffer wants made into its textures */
void font_trans( void )
{
	int i ;
	Sint16 n ;
	Sint32 no ;

	nlSetCommonTextureOffset( 0 ) ;
	for( i = 0 ; i < FONT_SLOT ; i ++ ){
		if( font_w->flag[font_w->buf][i] & 2 ){
			n = font_sjis_index( font_w->code[font_w->buf][i] ) ;
			font_char_make( n, font_w->pixel[0], font_w->font[font_w->buf][i], font_w->col[font_w->buf][i] ) ;
			cache_purge( font_w->pixel, sizeof( font_w->pixel ) ) ;
			no = FONT_TEX_NO + font_w->buf * FONT_SLOT + i ;
			nlCommonTextureReloadA( no, ( Uint32 * )font_w->pixel ) ;
			font_w->flag[font_w->buf][i] |= 4 ;
			font_w->flag[font_w->buf][i] &= ~2 ;
		}
		font_w->flag[font_w->buf][i] &= ~1 ;
	}
	font_w->buf ^= 1 ;
}
