/* The keyboard: a real one's keys read as a pad's, and the network's text entry, by a real keyboard
   or the software one on screen, with kana input and kana-kanji conversion. */
#include "player.h"
#include "nl.h"
#include "shinobi.h"
#include "dm.h"
#include "dm0d.h"

extern int sprintf( char *s, const char *fmt, ... ) ;
extern char *strcat( char *s1, const char *s2 ) ;

#define KB_CMD	100000		/* the editor's commands: kb_cmd's codes from here, the characters below */

/* ASK.BIN's entries: kana-kanji conversion */
#define ASK_INIT	( ( void ( * )( void ) )0x0cd004c2 )
#define ASK_KATAKANA	( ( int ( * )( char *roma, char *kana ) )0x0cd00064 )
#define ASK_HIRAGANA	( ( int ( * )( char *roma, char *kana ) )0x0cd00082 )
#define ASK_HENKAN	( ( int ( * )( char *yomi, char *kanji, char *rest ) )0x0cd000a0 )
#define ASK_PREV	( ( void ( * )( char *kanji, char *rest ) )0x0cd000c0 )
#define ASK_NEXT	( ( void ( * )( char *kanji, char *rest ) )0x0cd000de )
#define ASK_LONG	( ( int ( * )( char *kanji, char *rest ) )0x0cd00208 )
#define ASK_SHORT	( ( int ( * )( char *kanji, char *rest ) )0x0cd00226 )
#define ASK_YOMI	( ( void ( * )( char *kanji, char *rest ) )0x0cd00280 )
#define ASK_CANCEL	( ( int ( * )( char *kanji, char *rest ) )0x0cd0029e )

/* a character of the text being typed */
typedef struct KB_CHAR {
	Uint8	type;		/* 17 one byte, 18 two */
	Uint8	unk_01;
	union {
		Sint16	w;
		Uint8	b[2];	/* a two-byte character's second byte, then its first */
	} code;
} KB_CHAR;

/* a two-byte character's bytes, first and second */
#define SJIS_HI( c )	( ( Uint8 * )&( c ).code )[1]
#define SJIS_LO( c )	( ( Uint8 * )&( c ).code )[0]

typedef struct KB_EDIT {
	Sint16	max;		/* 000: as KB_SET's */
	Sint16	num;		/* 002: the characters typed */
	Sint16	pos;		/* 004: the cursor, before character pos */
	Sint16	unk_06;		/* 006: the cursor the frame before */
	Sint16	unk_08;		/* 008: its blink, in degrees */
	Sint8	unk_0a;		/* 00a: the one-byte characters before the cursor */
	Sint8	unk_0b;		/* 00b: the two-byte ones */
	Sint16	unk_0c;		/* 00c: the first character shown */
	Sint16	len;		/* 00e: the bytes it takes */
	char	*str;		/* 010 */
	KB_CHAR	ch[80];		/* 014 */
	Sint8	unk_154;		/* 154: the bytes of the text being converted */
	Sint8	unk_155;		/* 155: converting */
	Uint8	unk_156;
	Sint8	unk_157;
	Sint32	unk_158;		/* 158: ASK.BIN's candidates */
	Sint32	unk_15c;		/* 15c: the candidate shown, 255 for the kana */
	char	yomi[0xa0];		/* 160: the kana being converted */
	char	kanji[0xa0];		/* 200: their conversion */
	char	rest[0xa0];		/* 2a0: the part after the segment converted */
	Sint16	unk_340;		/* 340: the first character converted */
	Sint16	unk_342;
	Sint16	unk_344;		/* 344: the characters of the conversion */
	Sint8	unk_346;		/* 346: kb_henkan's command */
	Sint8	unk_347;
	char	buf[0xa4];	/* 348: the text as it is shown */
} KB_EDIT;

typedef struct KB_WORK {
	Sint16	unk_00;
	Sint16	rep;		/* 002: how long a real key has been held */
	Sint16	unk_04;
	Uint16	unk_06;
	Uint16	unk_08;
	Uint8	key[6];		/* 00a: the real keys held the frame before */
	Uint8	unk_10[6];
	Uint8	unk_16;
	Uint8	unk_17;
	Uint8	unk_18;
	Uint8	unk_19;
	Uint8	unk_1a;
	VEC3	text_pos;	/* 01c: where the text is shown */
	VEC3	key_pos;	/* 028: the software keyboard's keys */
	VEC3	mode_pos;	/* 034: its input mode */
} KB_WORK;

/* the software keyboard's keys */
typedef struct KB_KEY {
	Sint8	pos[2];		/* the cursor: its column and row */
	Sint8	old[2];		/* where it was */
	Uint8	unk_04;
	Uint8	unk_05;
	Uint8	unk_06;
	Uint8	unk_07;
	Uint16	unk_08;
	struct {
		Uint8	x, y;	/* its column and row */
		Uint8	w;	/* how many columns it takes */
	} key[75];
} KB_KEY;

/* a piece of the keyboard's textures */
typedef struct KB_SPR {
	Sint16	x0, y0, x1, y1;
	Uint16	tex;		/* 008: which texture */
} KB_SPR;

void kb_key_init( void ) ;
int kb_hard_move( const PDS_KEYBOARD *kbd ) ;
int kb_hard_key( const PDS_KEYBOARD *kbd ) ;
void kb_func_key( void ) ;
int kb_soft_move( void ) ;
int kb_soft_key( int x, int y ) ;
int kb_code_get( Uint8 key, int soft ) ;
int kb_hard_alnum( Uint8 key ) ;
int kb_soft_alnum( Uint8 key ) ;
int kb_hard_kana( Uint8 key ) ;
int kb_soft_kana( Uint8 key ) ;
int kb_cmd( int code ) ;
int kb_char_ins( int code ) ;
void kb_henkan( int no ) ;
void kb_kouho( void ) ;
void kb_str_make( KB_EDIT *ed, char *str, int all ) ;
int kb_fix_pos( void ) ;
int sjis_strlen( char *str ) ;
Uint16 kb_pad_get( Uint8 side_bit ) ;
void kb_disp( void ) ;
void kb_spr_set( KB_SPR *s, NLsprarg *spr, Float zm ) ;
void kb_disp_key( NLsprarg *spr ) ;
void kb_disp_mode( NLsprarg *spr ) ;

static Uint8 kb_unk_8C149874 = 0 ;

/* the software keyboard's three panes, at each size */
static VEC3 kb_pos_tbl[2][3] = {
	{ { 80.0f, 100.0f, 13.5f }, { 80.0f, 200.0f, 13.5f }, { 460.0f, 430.0f, 13.5f } },
	{ { 80.0f, 100.0f, 10.9f }, { 80.0f, 200.0f, 10.9f }, { 460.0f, 430.0f, 10.9f } }
} ;

/* the keys read as a pad's: the arrows, Enter and Esc */
static Uint16 kbd_key_tbl[6][2] = {
	{ 0x4f, KEY_RIGHT }, { 0x50, KEY_LEFT }, { 0x51, KEY_DOWN }, { 0x52, KEY_UP }, { 0x28, KEY_LK }, { 0x29, KEY_HK }
} ;

/* the software keyboard's keys wider than one column: the key, its column and its width */
static Uint8 kb_wide_tbl[11][3] = {
	{ 2, 2, 2 }, { 3, 4, 2 }, { 4, 6, 2 }, { 5, 8, 2 }, { 6, 10, 1 }, { 39, 13, 2 },
	{ 70, 0, 4 }, { 71, 4, 4 }, { 72, 8, 3 }, { 73, 11, 3 }, { 99, 0, 0 }
} ;

/* the key under each column and row of the software keyboard */
static Uint8 kb_soft_tbl[6][15] = {
	0, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6, 7, 8, 9, 10,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
	26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 39,
	40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54,
	55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69,
	70, 70, 70, 70, 71, 71, 71, 71, 72, 72, 72, 73, 73, 73, 74
} ;

static Uint8 kb_unk_8C149953[75] = {
	0, 0, 1, 1, 1, 1, 14, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 3, 2, 2, 0
} ;

static Uint8 kb_unk_8C14999E[16][2] = {
	{ 0, 2 }, { 1, 3 }, { 2, 0 }, { 3, 0 }, { 4, 0 }, { 5, 0 }, { 11, 1 }, { 16, 1 },
	{ 21, 1 }, { 26, 1 }, { 31, 1 }, { 36, 1 }, { 40, 1 }, { 45, 1 }, { 55, 1 }, { 60, 1 }
} ;

static Uint8 kb_unk_8C1499BE[15][2] = {
	{ 0, 36 }, { 1, 37 }, { 2, 0 }, { 7, 1 }, { 11, 2 }, { 26, 3 }, { 39, 18 }, { 40, 4 },
	{ 50, 19 }, { 55, 5 }, { 63, 20 }, { 70, 33 }, { 71, 31 }, { 74, 17 }, { 255, 0 }
} ;

static Uint8 kb_unk_8C1499DC[15][2] = {
	{ 0, 36 }, { 1, 37 }, { 2, 0 }, { 7, 1 }, { 11, 6 }, { 26, 7 }, { 39, 18 }, { 40, 8 },
	{ 50, 19 }, { 55, 9 }, { 63, 20 }, { 70, 33 }, { 71, 31 }, { 74, 17 }, { 255, 0 }
} ;

static Uint8 kb_unk_8C1499FA[22][2] = {
	{ 0, 36 }, { 1, 37 }, { 2, 0 }, { 7, 1 }, { 11, 10 }, { 26, 14 }, { 36, 21 }, { 39, 18 },
	{ 40, 15 }, { 50, 22 }, { 52, 23 }, { 53, 24 }, { 55, 16 }, { 61, 25 }, { 65, 26 }, { 66, 27 },
	{ 67, 28 }, { 68, 29 }, { 70, 32 }, { 71, 31 }, { 74, 17 }, { 255, 0 }
} ;

static Uint8 kb_unk_8C149A26[17][2] = {
	{ 0, 36 }, { 1, 37 }, { 2, 0 }, { 7, 1 }, { 11, 10 }, { 26, 11 }, { 36, 21 }, { 39, 18 },
	{ 40, 12 }, { 50, 35 }, { 51, 30 }, { 55, 13 }, { 61, 34 }, { 70, 32 }, { 71, 31 }, { 74, 17 },
	{ 255, 0 }
} ;

static Sint32 kb_hard_code0[144] = {
	0, 0, 0, 0, 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z', '1', '2',
	'3', '4', '5', '6', '7', '8', '9', '0',
	KB_CMD, KB_CMD + 1, KB_CMD + 2, KB_CMD + 3, KB_CMD + 4, '-', '^', '@',
	'[', 0, ']', ';', ':', KB_CMD + 5, ',', '.',
	'/', KB_CMD + 6, KB_CMD + 7, KB_CMD + 8, KB_CMD + 9, KB_CMD + 10, KB_CMD + 11, KB_CMD + 12,
	KB_CMD + 13, KB_CMD + 14, KB_CMD + 15, KB_CMD + 16, KB_CMD + 17, KB_CMD + 18, KB_CMD + 19, KB_CMD + 20,
	KB_CMD + 21, KB_CMD + 22, KB_CMD + 23, KB_CMD + 24, KB_CMD + 25, KB_CMD + 26, KB_CMD + 27, KB_CMD + 28,
	KB_CMD + 29, KB_CMD + 30, KB_CMD + 31, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, KB_CMD + 32, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '\\',
	KB_CMD + 33, '\\', KB_CMD + 34, KB_CMD + 35, 0, 0, 0, 0
} ;

static Sint32 kb_hard_code1[144] = {
	0, 0, 0, 0, 'A', 'B', 'C', 'D',
	'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
	'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', '!', '"',
	'#', '$', '%', '&', '\'', '(', ')', '~',
	KB_CMD, KB_CMD + 1, KB_CMD + 2, KB_CMD + 3, KB_CMD + 4, '=', '~', '`',
	'{', '|', '}', '+', '*', KB_CMD + 5, '<', '>',
	'?', KB_CMD + 6, KB_CMD + 7, KB_CMD + 8, KB_CMD + 9, KB_CMD + 10, KB_CMD + 11, KB_CMD + 12,
	KB_CMD + 13, KB_CMD + 14, KB_CMD + 15, KB_CMD + 16, KB_CMD + 17, KB_CMD + 18, KB_CMD + 19, KB_CMD + 20,
	KB_CMD + 21, KB_CMD + 22, KB_CMD + 23, KB_CMD + 24, KB_CMD + 25, KB_CMD + 26, KB_CMD + 27, KB_CMD + 28,
	KB_CMD + 29, KB_CMD + 30, KB_CMD + 31, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, KB_CMD + 32, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '_',
	KB_CMD + 33, '|', KB_CMD + 34, KB_CMD + 35, 0, 0, 0, 0
} ;

static Sint32 kb_hard_code2[144] = {
	0, 0, 0, 0, 'A', 'B', 'C', 'D',
	'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',
	'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
	'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2',
	'3', '4', '5', '6', '7', '8', '9', '0',
	KB_CMD, KB_CMD + 1, KB_CMD + 2, KB_CMD + 3, KB_CMD + 4, '-', '^', '@',
	'[', '\\', ']', ';', ':', KB_CMD + 5, ',', '.',
	'/', KB_CMD + 6, KB_CMD + 7, KB_CMD + 8, KB_CMD + 9, KB_CMD + 10, KB_CMD + 11, KB_CMD + 12,
	KB_CMD + 13, KB_CMD + 14, KB_CMD + 15, KB_CMD + 16, KB_CMD + 17, KB_CMD + 18, KB_CMD + 19, KB_CMD + 20,
	KB_CMD + 21, KB_CMD + 22, KB_CMD + 23, KB_CMD + 24, KB_CMD + 25, KB_CMD + 26, KB_CMD + 27, KB_CMD + 28,
	KB_CMD + 29, KB_CMD + 30, KB_CMD + 31, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, KB_CMD + 32, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '_',
	KB_CMD + 33, '|', KB_CMD + 34, KB_CMD + 35, 0, 0, 0, 0
} ;

static Sint32 kb_hard_code3[144] = {
	0, 0, 0, 0, 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l',
	'm', 'n', 'o', 'p', 'q', 'r', 's', 't',
	'u', 'v', 'w', 'x', 'y', 'z', '!', '"',
	'#', '$', '%', '&', '\'', '(', ')', '~',
	KB_CMD, KB_CMD + 1, KB_CMD + 2, KB_CMD + 3, KB_CMD + 4, '=', '~', '`',
	'{', 0, '}', '+', '*', KB_CMD + 5, '>', '>',
	'?', KB_CMD + 6, KB_CMD + 7, KB_CMD + 8, KB_CMD + 9, KB_CMD + 10, KB_CMD + 11, KB_CMD + 12,
	KB_CMD + 13, KB_CMD + 14, KB_CMD + 15, KB_CMD + 16, KB_CMD + 17, KB_CMD + 18, KB_CMD + 19, KB_CMD + 20,
	KB_CMD + 21, KB_CMD + 22, KB_CMD + 23, KB_CMD + 24, KB_CMD + 25, KB_CMD + 26, KB_CMD + 27, KB_CMD + 28,
	KB_CMD + 29, KB_CMD + 30, KB_CMD + 31, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, KB_CMD + 32, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, '_',
	KB_CMD + 33, '|', KB_CMD + 34, KB_CMD + 35, 0, 0, 0, 0
} ;

static char *kb_hard_str0[144] = {
	0, 0, 0, 0, "\x82\x81", "\x82\x82", "\x82\x83", "\x82\x84",
	"\x82\x85", "\x82\x86", "\x82\x87", "\x82\x88", "\x82\x89", "\x82\x8a", "\x82\x8b", "\x82\x8c",
	"\x82\x8d", "\x82\x8e", "\x82\x8f", "\x82\x90", "\x82\x91", "\x82\x92", "\x82\x93", "\x82\x94",
	"\x82\x95", "\x82\x96", "\x82\x97", "\x82\x98", "\x82\x99", "\x82\x9a", "\x82\x50", "\x82\x51",
	"\x82\x52", "\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56", "\x82\x57", "\x82\x58", "\x82\x4f",
	0, 0, 0, 0, 0, "\x81\x7c", "\x81\x4f", "\x81\x97",
	"\x81\x6d", 0, "\x81\x6e", "\x81\x47", "\x81\x46", 0, "\x81\x43", "\x81\x44",
	"\x81\x5e", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "\x81\x8f",
	0, "\x81\x8f", 0, 0, 0, 0, 0, 0
} ;

static char *kb_hard_str1[144] = {
	0, 0, 0, 0, "\x82\x60", "\x82\x61", "\x82\x62", "\x82\x63",
	"\x82\x64", "\x82\x65", "\x82\x66", "\x82\x67", "\x82\x68", "\x82\x69", "\x82\x6a", "\x82\x6b",
	"\x82\x6c", "\x82\x6d", "\x82\x6e", "\x82\x6f", "\x82\x70", "\x82\x71", "\x82\x72", "\x82\x73",
	"\x82\x74", "\x82\x75", "\x82\x76", "\x82\x77", "\x82\x78", "\x82\x79", "\x81\x49", "\x81\x68",
	"\x81\x94", "\x81\x90", "\x81\x93", "\x81\x95", "\x81\x66", "\x81\x69", "\x81\x6a", "\x81\x60",
	0, 0, 0, 0, 0, "\x81\x81", "\x81\x60", "\x81\x65",
	"\x81\x6f", "\x81\x62", "\x81\x70", "\x81\x7b", "\x81\x96", 0, "\x81\x83", "\x81\x84",
	"\x81\x48", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "\x81\x51",
	0, "\x81\x62", 0, 0, 0, 0, 0, 0
} ;

static char *kb_hard_str2[144] = {
	0, 0, 0, 0, "\x82\x60", "\x82\x61", "\x82\x62", "\x82\x63",
	"\x82\x64", "\x82\x65", "\x82\x66", "\x82\x67", "\x82\x68", "\x82\x69", "\x82\x6a", "\x82\x6b",
	"\x82\x6c", "\x82\x6d", "\x82\x6e", "\x82\x6f", "\x82\x70", "\x82\x71", "\x82\x72", "\x82\x73",
	"\x82\x74", "\x82\x75", "\x82\x76", "\x82\x77", "\x82\x78", "\x82\x79", "\x82\x50", "\x82\x51",
	"\x82\x52", "\x82\x53", "\x82\x54", "\x82\x55", "\x82\x56", "\x82\x57", "\x82\x58", "\x82\x4f",
	0, 0, 0, 0, 0, "\x81\x7c", "\x81\x4f", "\x81\x97",
	"\x81\x6d", "\x81\x8f", "\x81\x6e", "\x81\x47", "\x81\x46", 0, "\x81\x43", "\x81\x44",
	"\x81\x5e", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "\x81\x8f",
	0, "\x81\x8f", 0, 0, 0, 0, 0, 0
} ;

static char *kb_hard_str3[144] = {
	0, 0, 0, 0, "\x82\x81", "\x82\x82", "\x82\x83", "\x82\x84",
	"\x82\x85", "\x82\x86", "\x82\x87", "\x82\x88", "\x82\x89", "\x82\x8a", "\x82\x8b", "\x82\x8c",
	"\x82\x8d", "\x82\x8e", "\x82\x8f", "\x82\x90", "\x82\x91", "\x82\x92", "\x82\x93", "\x82\x94",
	"\x82\x95", "\x82\x96", "\x82\x97", "\x82\x98", "\x82\x99", "\x82\x9a", "\x81\x49", "\x81\x68",
	"\x81\x94", "\x81\x90", "\x81\x93", "\x81\x95", "\x81\x66", "\x81\x69", "\x81\x6a", "\x81\x60",
	0, 0, 0, 0, 0, "\x81\x81", "\x81\x60", "\x81\x65",
	"\x81\x6f", 0, "\x81\x70", "\x81\x7b", "\x81\x96", 0, "\x81\x83", "\x81\x84",
	"\x81\x48", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "\x81\x51",
	0, "\x81\x62", 0, 0, 0, 0, 0, 0
} ;

static char *kb_hard_kana_str0[144] = {
	0, 0, 0, 0, "\x82\xbf", "\x82\xb1", "\x82\xbb", "\x82\xb5",
	"\x82\xa2", "\x82\xcd", "\x82\xab", "\x82\xad", "\x82\xc9", "\x82\xdc", "\x82\xcc", "\x82\xe8",
	"\x82\xe0", "\x82\xdd", "\x82\xe7", "\x82\xb9", "\x82\xbd", "\x82\xb7", "\x82\xc6", "\x82\xa9",
	"\x82\xc8", "\x82\xd0", "\x82\xc4", "\x82\xb3", "\x82\xf1", "\x82\xc2", "\x82\xca", "\x82\xd3",
	"\x82\xa0", "\x82\xa4", "\x82\xa6", "\x82\xa8", "\x82\xe2", "\x82\xe4", "\x82\xe6", "\x82\xed",
	0, 0, 0, 0, 0, "\x82\xd9", "\x82\xd6", "\x81\x4a",
	"\x81\x4b", "\x81\x5b", "\x82\xde", "\x82\xea", "\x82\xaf", 0, "\x82\xcb", "\x82\xe9",
	"\x82\xdf", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "\x82\xeb",
	0, "\x81\x5b", 0, 0, 0, 0, 0, 0
} ;

static char *kb_hard_kana_str1[144] = {
	0, 0, 0, 0, "\x82\xbf", "\x82\xb1", "\x82\xbb", "\x82\xb5",
	"\x82\xa1", "\x82\xcd", "\x82\xab", "\x82\xad", "\x82\xc9", "\x82\xdc", "\x82\xcc", "\x82\xe8",
	"\x82\xe0", "\x82\xdd", "\x82\xe7", "\x81\x77", "\x82\xbd", "\x82\xb7", "\x82\xc6", "\x82\xa9",
	"\x82\xc8", "\x82\xd0", "\x82\xc4", "\x82\xb3", "\x82\xf1", "\x82\xc1", "\x82\xca", "\x82\xd3",
	"\x82\x9f", "\x82\xa3", "\x82\xa5", "\x82\xa7", "\x82\xe1", "\x82\xe3", "\x82\xe5", "\x82\xf0",
	0, 0, 0, 0, 0, "\x81\x92", "\x81\x58", "\x81\x91",
	"\x81\x75", "\x81\xca", "\x81\x76", "\x81\x78", "\x83\x96", 0, "\x81\x41", "\x81\x42",
	"\x81\x45", 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, "\x81\x62",
	0, "\x81\xca", 0, 0, 0, 0, 0, 0
} ;

static Sint32 kb_soft_code0[75] = {
	KB_CMD + 47, KB_CMD + 48, KB_CMD + 38, KB_CMD + 39, KB_CMD + 40, KB_CMD + 41, 0, KB_CMD + 29,
	KB_CMD + 28, KB_CMD + 31, KB_CMD + 30, '1', '2', '3', '4', '5',
	'6', '7', '8', '9', '0', '@', ',', '.',
	';', ':', 'a', 'b', 'c', 'd', 'e', 'f',
	'g', 'h', 'i', 'j', '/', '~', '_', KB_CMD + 37,
	'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', '^', '&', '$', '[', ']', 'u',
	'v', 'w', 'x', 'y', 'z', '+', '-', '*',
	'=', '|', '\\', '#', '{', '}', KB_CMD + 4, KB_CMD,
	KB_CMD + 25, KB_CMD + 2, KB_CMD
} ;

static Sint32 kb_soft_code1[75] = {
	KB_CMD + 47, KB_CMD + 48, KB_CMD + 38, KB_CMD + 39, KB_CMD + 40, KB_CMD + 41, 0, KB_CMD + 29,
	KB_CMD + 28, KB_CMD + 31, KB_CMD + 30, '1', '2', '3', '4', '5',
	'6', '7', '8', '9', '0', '@', ',', '.',
	';', ':', 'A', 'B', 'C', 'D', 'E', 'F',
	'G', 'H', 'I', 'J', '/', '~', '_', KB_CMD + 37,
	'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
	'S', 'T', '^', '!', '?', '<', '>', 'U',
	'V', 'W', 'X', 'Y', 'Z', '+', '-', '*',
	'=', '%', '\'', '"', '(', ')', KB_CMD + 4, KB_CMD,
	KB_CMD + 25, KB_CMD + 2, KB_CMD
} ;

static char *kb_soft_str0[75] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, "\x82\x50", "\x82\x51", "\x82\x52", "\x82\x53", "\x82\x54",
	"\x82\x55", "\x82\x56", "\x82\x57", "\x82\x58", "\x82\x4f", "\x81\x97", "\x81\x43", "\x81\x44",
	"\x81\x47", "\x81\x46", "\x82\x81", "\x82\x82", "\x82\x83", "\x82\x84", "\x82\x85", "\x82\x86",
	"\x82\x87", "\x82\x88", "\x82\x89", "\x82\x8a", "\x81\x5e", "\x81\x60", "\x81\x51", 0,
	"\x82\x8b", "\x82\x8c", "\x82\x8d", "\x82\x8e", "\x82\x8f", "\x82\x90", "\x82\x91", "\x82\x92",
	"\x82\x93", "\x82\x94", "\x81\x4f", "\x81\x95", "\x81\x90", "\x81\x6d", "\x81\x6e", "\x82\x95",
	"\x82\x96", "\x82\x97", "\x82\x98", "\x82\x99", "\x82\x9a", "\x81\x7b", "\x81\x7c", "\x81\x96",
	"\x81\x81", "\x81\x62", "\x81\x8f", "\x81\x94", "\x81\x6f", "\x81\x70", 0, 0,
	0, 0, 0
} ;

static char *kb_soft_str1[75] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, "\x82\x50", "\x82\x51", "\x82\x52", "\x82\x53", "\x82\x54",
	"\x82\x55", "\x82\x56", "\x82\x57", "\x82\x58", "\x82\x4f", "\x81\x97", "\x81\x43", "\x81\x44",
	"\x81\x47", "\x81\x46", "\x82\x60", "\x82\x61", "\x82\x62", "\x82\x63", "\x82\x64", "\x82\x65",
	"\x82\x66", "\x82\x67", "\x82\x68", "\x82\x69", "\x81\x5e", "\x81\x60", "\x81\x51", 0,
	"\x82\x6a", "\x82\x6b", "\x82\x6c", "\x82\x6d", "\x82\x6e", "\x82\x6f", "\x82\x70", "\x82\x71",
	"\x82\x72", "\x82\x73", "\x81\x4f", "\x81\x49", "\x81\x48", "\x81\x83", "\x81\x84", "\x82\x74",
	"\x82\x75", "\x82\x76", "\x82\x77", "\x82\x78", "\x82\x79", "\x81\x7b", "\x81\x7c", "\x81\x96",
	"\x81\x81", "\x81\x93", "\x81\x66", "\x81\x68", "\x81\x69", "\x81\x6a", 0, 0,
	0, 0, 0
} ;

static char *kb_soft_kana_str[150] = {
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, "\x82\xa0", "\x82\xa2", "\x82\xa4", "\x82\xa6", "\x82\xa8",
	"\x82\xc8", "\x82\xc9", "\x82\xca", "\x82\xcb", "\x82\xcc", "\x82\xe7", "\x82\xe8", "\x82\xe9",
	"\x82\xea", "\x82\xeb", "\x82\xa9", "\x82\xab", "\x82\xad", "\x82\xaf", "\x82\xb1", "\x82\xcd",
	"\x82\xd0", "\x82\xd3", "\x82\xd6", "\x82\xd9", "\x82\xed", "\x82\xf0", "\x82\xf1", 0,
	"\x82\xb3", "\x82\xb5", "\x82\xb7", "\x82\xb9", "\x82\xbb", "\x82\xdc", "\x82\xdd", "\x82\xde",
	"\x82\xdf", "\x82\xe0", "\x81\x5b", "\x81\x49", "\x81\x48", "\x81\x75", "\x81\x76", "\x82\xbd",
	"\x82\xbf", "\x82\xc2", "\x82\xc4", "\x82\xc6", "\x82\xe2", "\x82\xe4", "\x82\xe6", "\x81\x4a",
	"\x81\x4b", "\x81\x41", "\x81\x42", "\x81\x60", "\x81\x77", "\x81\x78", 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, "\x82\x9f", "\x82\xa1",
	"\x82\xa3", "\x82\xa5", "\x82\xa7", "\x82\xc8", "\x82\xc9", "\x82\xca", "\x82\xcb", "\x82\xcc",
	"\x82\xe7", "\x82\xe8", "\x82\xe9", "\x82\xea", "\x82\xeb", "\x82\xa9", "\x82\xab", "\x82\xad",
	"\x82\xaf", "\x82\xb1", "\x82\xcd", "\x82\xd0", "\x82\xd3", "\x82\xd6", "\x82\xd9", "\x82\xec",
	"\x82\xf0", "\x82\xf1", 0, "\x82\xb3", "\x82\xb5", "\x82\xb7", "\x82\xb9", "\x82\xbb",
	"\x82\xdc", "\x82\xdd", "\x82\xde", "\x82\xdf", "\x82\xe0", "\x81\x5b", "\x81\x49", "\x81\x48",
	"\x81\x75", "\x81\x76", "\x82\xbd", "\x82\xbf", "\x82\xc1", "\x82\xc4", "\x82\xc6", "\x82\xe1",
	"\x82\xe3", "\x82\xe5", "\x81\x4a", "\x81\x4b", "\x81\x41", "\x81\x42", "\x81\x60", "\x81\x77",
	"\x81\x78", 0, 0, 0, 0, 0
} ;

/* the software keys' tops */
static KB_SPR kb_top_tbl[38] = {
	{ 128, 320, 384, 352, 1 },
	{ 320, 0, 448, 32, 1 },
	{ 0, 32, 480, 64, 1 },
	{ 0, 64, 416, 96, 1 },
	{ 0, 96, 320, 128, 1 },
	{ 0, 128, 256, 160, 1 },
	{ 0, 160, 480, 192, 1 },
	{ 0, 192, 416, 224, 1 },
	{ 0, 224, 320, 256, 1 },
	{ 0, 256, 256, 288, 1 },
	{ 0, 0, 480, 32, 0 },
	{ 0, 32, 320, 64, 0 },
	{ 0, 64, 320, 96, 0 },
	{ 0, 96, 192, 128, 0 },
	{ 0, 128, 320, 160, 0 },
	{ 0, 160, 320, 192, 0 },
	{ 0, 192, 192, 224, 0 },
	{ 448, 0, 480, 32, 1 },
	{ 416, 64, 480, 96, 1 },
	{ 320, 96, 480, 128, 1 },
	{ 256, 128, 480, 160, 1 },
	{ 320, 32, 416, 64, 0 },
	{ 192, 192, 256, 224, 0 },
	{ 320, 192, 352, 224, 0 },
	{ 384, 192, 448, 224, 0 },
	{ 192, 96, 320, 128, 0 },
	{ 288, 192, 320, 224, 0 },
	{ 352, 192, 384, 224, 0 },
	{ 256, 192, 288, 224, 0 },
	{ 448, 192, 512, 224, 0 },
	{ 320, 64, 448, 96, 0 },
	{ 128, 352, 448, 384, 1 },
	{ 0, 352, 128, 384, 1 },
	{ 0, 384, 128, 416, 1 },
	{ 192, 96, 480, 128, 0 },
	{ 192, 192, 224, 224, 0 },
	{ 0, 0, 32, 32, 1 },
	{ 32, 0, 64, 32, 1 }
} ;

/* the keyboard's other pieces */
static KB_SPR kb_spr_tbl[34] = {
	{ 0, 224, 32, 256, 0 },
	{ 32, 224, 96, 256, 0 },
	{ 96, 224, 192, 256, 0 },
	{ 192, 224, 320, 256, 0 },
	{ 320, 224, 352, 256, 0 },
	{ 352, 224, 384, 256, 0 },
	{ 0, 256, 32, 288, 0 },
	{ 32, 256, 96, 288, 0 },
	{ 96, 256, 192, 288, 0 },
	{ 192, 256, 320, 288, 0 },
	{ 0, 288, 32, 320, 0 },
	{ 32, 288, 96, 320, 0 },
	{ 96, 288, 192, 320, 0 },
	{ 192, 288, 320, 320, 0 },
	{ 0, 320, 32, 352, 0 },
	{ 32, 320, 96, 352, 0 },
	{ 96, 320, 192, 352, 0 },
	{ 192, 320, 320, 352, 0 },
	{ 0, 352, 480, 384, 0 },
	{ 0, 384, 160, 416, 0 },
	{ 0, 0, 32, 32, 1 },
	{ 32, 0, 64, 32, 1 },
	{ 64, 0, 96, 32, 1 },
	{ 96, 0, 128, 32, 1 },
	{ 128, 0, 160, 32, 1 },
	{ 160, 0, 192, 32, 1 },
	{ 96, 288, 192, 320, 1 },
	{ 0, 288, 96, 320, 1 },
	{ 352, 320, 384, 352, 0 },
	{ 320, 320, 352, 352, 0 },
	{ 320, 256, 352, 288, 0 },
	{ 352, 256, 416, 288, 0 },
	{ 320, 288, 352, 320, 0 },
	{ 352, 288, 416, 320, 0 }
} ;

static KB_EDIT kb_edit ;
static KB_WORK kb_w ;
static KB_KEY kb_key ;
static Uint8 kbd_cnt[6] ;	/* how long each has been held */
static Uint32 unk_8C1F9CC8[2] ;	/* unused */

void kbd_read( void )
{
	int i, j ;
	Uint16 sw ;

	sw = 0 ;
	if( kbd_data ){
		kbd_old = kbd_now ;
		for( i = 0 ; i < 6 ; i ++ ){
			for( j = 0 ; j < 6 ; j ++ ){
				if( kbd_key_tbl[i][0] == kbd_data->key[j] ){
					sw |= kbd_key_tbl[i][1] ;
					break ;
				}
			}
		}
		kbd_now = sw ;
		kbd_on = kbd_rep = 0 ;
		for( i = 0 ; i < 6 ; i ++ ){
			if( kbd_key_tbl[i][1] & ~kbd_old & kbd_now ){
				kbd_on |= kbd_key_tbl[i][1] ;
			} else if( kbd_key_tbl[i][1] & kbd_now ){
				if( ++kbd_cnt[i] > 30 ){
					if( kbd_cnt[i] > 35 ){
						kbd_cnt[i] = 30 ;
						kbd_rep |= kbd_key_tbl[i][1] ;
					}
				}
			} else {
				kbd_cnt[i] = 0 ;
			}
		}
	}
}

/* sets the text entry up from set: the text, its length, the modes and the keyboard's size */
void kb_init( KB_SET *set )
{
	register int i, n ;
	register Uint8 len, c ;
	register Uint8 c2 ;
	register Uint8 *p ;

	kb_w.unk_00 = 4 ;
	kb_w.unk_04 = 20 ;
	kb_w.unk_06 = set->mode & set->mode_ok ;
	kb_w.unk_08 = set->mode_ok ;
	if( ( kb_w.unk_06 & 8 ) || ( kb_w.unk_08 & 0x100 ) ){
		kb_w.unk_17 = 1 ;
		kb_w.unk_06 |= 8 ;
		kb_w.unk_18 = 1 ;
		kb_w.unk_19 = 1 ;
	} else {
		kb_w.unk_17 = 0 ;
		kb_w.unk_18 = ( kb_w.unk_06 & 2 ) ? 0 : 1 ;
		kb_w.unk_19 = ( kb_w.unk_06 & 4 ) ? 1 : 0 ;
	}
	kb_w.text_pos = kb_pos_tbl[set->size][0] ;
	kb_w.key_pos = kb_pos_tbl[set->size][1] ;
	kb_w.mode_pos = kb_pos_tbl[set->size][2] ;
	kb_w.text_pos.x += option_w.adjust_x ;
	kb_w.text_pos.y += option_w.adjust_y ;
	kb_w.key_pos.x += option_w.adjust_x ;
	kb_w.key_pos.y += option_w.adjust_y ;
	kb_w.mode_pos.x += option_w.adjust_x ;
	kb_w.mode_pos.y += option_w.adjust_y ;
	kb_w.rep = 0 ;
	kb_w.unk_1a = 4 ;
	kb_w.unk_16 = 1 ;
	kb_unk_8C149874 = 0 ;
	for( i = 0 ; i < 6 ; i ++ ){
		if( kbd_data ){
			kb_w.key[i] = kbd_data->key[i] ;
		} else {
			kb_w.key[i] = 0 ;
			kb_w.unk_16 = 1 ;
		}
		kb_w.unk_10[i] = 0 ;
	}
	kb_edit.max = set->max ;
	kb_edit.str = set->str ;
	kb_edit.pos = kb_edit.num = 0 ;
	kb_edit.unk_06 = 99 ;
	kb_edit.unk_0c = 0 ;
	kb_edit.buf[0] = 0 ;
	kb_edit.len = 0 ;
	kb_edit.unk_155 = 0 ;
	kb_edit.unk_156 = 0 ;
	kb_edit.unk_347 = 0 ;
	if( set->str ){
		if( !*set->str ){
			for( i = 0 ; i < 80 ; i ++ ){
				kb_edit.ch[i].type = 0 ;
				kb_edit.ch[i].code.w = 0 ;
			}
		} else {
			p = ( Uint8 * )set->str ;
			n = 0 ;
			len = 0 ;
			while( ( c = *p ++ ) != 0 && len < set->max ){
				if( !( c & 0x80 ) ){
					kb_edit.ch[n].type = 17 ;
					kb_edit.ch[n].code.w = c ;
					len ++ ;
				} else {
					c2 = *p ++ ;
					if( len > set->max - 2 ) break ;
					kb_edit.ch[n].type = 18 ;
					kb_edit.ch[n].code.w = c << 8 | c2 ;
					len += 2 ;
				}
				n ++ ;
			}
			kb_edit.pos = kb_edit.num = n ;
			kb_edit.len = len ;
		}
	}
	kb_key_init() ;
	ASK_INIT() ;
}

/* lays the software keyboard's keys out in rows of 15 columns */
void kb_key_init( void )
{
	register int y ;
	register int i ;
	register int x ;
	Uint8 *p ;
	KB_KEY *k ;

	k = &kb_key ;
	p = kb_wide_tbl[0] ;
	for( y = x = i = 0 ; i < 75 ; i ++ ){
		if( i == p[0] ){
			k->key[i].x = p[1] ;
			k->key[i].w = p[2] ;
			x += p[2] ;
			p += 3 ;
		} else {
			k->key[i].x = x ;
			k->key[i].w = 1 ;
			x ++ ;
		}
		k->key[i].y = y ;
		if( x >= 15 ){
			x = 0 ;
			y ++ ;
		}
	}
	kb_key.pos[0] = kb_key.old[0] = 0 ;
	kb_key.pos[1] = kb_key.old[1] = 0 ;
	kb_key.unk_04 = kb_key.unk_05 = 0 ;
	kb_key.unk_06 = 99 ;
	kb_key.unk_07 = 0 ;
}

/* the text entry each frame: 0 while it goes on, else how it ended */
int kb_main( void )
{
	register int ret ;
	register int i ;

	ret = 0 ;
	if( !kb_w.unk_16 ){
		if( kb_pad_get( 3 ) & 0x3ff0 ) kb_w.unk_16 = 1 ;
	}
	if( kbd_data ) ret = kb_hard_move( kbd_data ) ;
	if( kb_w.unk_16 ){
		if( !kb_unk_8C149874 ){
			na_req( 57 ) ;
			kb_key.pos[0] = kb_key.old[0] = 0 ;
			kb_key.pos[1] = kb_key.old[1] = 0 ;
			kb_key.unk_04 = kb_key.unk_05 = 0 ;
			kb_key.unk_06 = 99 ;
			kb_key.unk_07 = 0 ;
		} else {
			ret = kb_soft_move() ;
		}
	}
	kb_unk_8C149874 = kb_w.unk_16 ;
	if( ( i = kb_fix_pos() ) >= 0 ){
		kb_edit.unk_347 = 0 ;
		for( ; i < kb_edit.num ; i ++ ){
			if( kb_edit.ch[i].type & 16 ) break ;
			kb_edit.unk_347 ++ ;
			if( kb_edit.ch[i].type & 2 ) kb_edit.unk_347 ++ ;
		}
	}
	kb_str_make( &kb_edit, kb_edit.buf, 1 ) ;
	if( !ret ){
		str_put( kb_w.text_pos.x - option_w.adjust_x + 8.0f, kb_w.text_pos.y - option_w.adjust_y + 6.0f,
			kb_w.text_pos.z + -0.1f, 0xff000000, kb_edit.buf ) ;
	} else if( kb_edit.str ){
		kb_str_make( &kb_edit, kb_edit.str, 0 ) ;
		if( !*kb_edit.str ) ret = -1 ;
	}
	SysTransSet( kb_disp ) ;
	return ret ;
}

/* a real keyboard: the keys newly pressed, and a held one given again */
int kb_hard_move( const PDS_KEYBOARD *kbd )
{
	register int i, j, n, same ;
	register int ret ;

	same = 0 ;
	if( kbd->key[0] == kb_w.key[0] && kb_w.key[0] != 0 ) same = 1 ;
	if( same ) kb_w.rep = kb_w.rep + 1 ;
	else kb_w.rep = 0 ;
	for( i = 0, n = 0 ; i < 6 ; i ++ ){
		kb_w.unk_10[i] = 0 ;
		for( j = 0 ; j < 6 ; j ++ ){
			if( kb_w.key[j] == kbd->key[i] && kbd->key[i] != 1 ) break ;
		}
		if( j == 6 ){
			kb_w.unk_10[n ++] = kbd->key[i] ;
			kb_w.unk_16 = 0 ;
			if( kb_unk_8C149874 ){
				if( kb_w.unk_08 & 4 ){
					kb_w.unk_19 = 1 ;
					kb_func_key() ;
				}
			}
		}
	}
	ret = kb_hard_key( kbd ) ;
	for( i = 0 ; i < 6 ; i ++ ) kb_w.key[i] = kbd->key[i] ;
	return ret ;
}

/* the shift and other keys that change the mode, then the keys pressed */
int kb_hard_key( const PDS_KEYBOARD *kbd )
{
	register int ret ;
	register int i ;

	ret = 0 ;
	if( kbd->ctrl == 0x22 || kbd->ctrl == 0x20 || kbd->ctrl == 0x02 ) kb_w.unk_06 |= 0x10 ;
	else kb_w.unk_06 &= ~0x10 ;
	if( kbd->ctrl == 0x44 || kbd->ctrl == 0x40 || kbd->ctrl == 0x04 ) kb_w.unk_06 |= 0x20 ;
	else kb_w.unk_06 &= ~0x20 ;
	if( ( kb_w.unk_06 & 0x10 ) && kb_w.unk_10[0] == 57 ) kb_w.unk_06 ^= 0x40 ;
	else if( !( kb_w.unk_06 & 0x10 ) && kb_w.unk_10[0] == 57 && kb_w.unk_17 ) kb_w.unk_18 = 0 ;
	if( kb_w.unk_10[0] == 73 ) kb_w.unk_06 ^= 0x80 ;
	if( kbd->info->lang == 1 && ( kb_w.unk_08 & 8 ) ){
		if( kb_w.unk_10[0] == 53 && !( kb_w.unk_08 & 0x100 ) ){
			if( kb_w.unk_17 ^= 1 ){
				kb_w.unk_19 = 1 ;
				kb_w.unk_18 = 1 ;
			} else {
				kb_w.unk_19 = 0 ;
				kb_w.unk_18 = 0 ;
			}
		}
		if( kb_w.unk_17 ){
			if( ( kb_w.unk_06 & 0x20 ) && kb_w.unk_10[0] == 0x88 ) kb_w.unk_19 ^= 1 ;
			else if( ( kb_w.unk_06 & 0x10 ) && kb_w.unk_10[0] == 0x88 ) kb_w.unk_18 = 2 ;
			else if( !( kb_w.unk_06 & 0x10 ) && kb_w.unk_10[0] == 0x88 ) kb_w.unk_18 = 1 ;
		}
	}
	kb_w.unk_06 &= kb_w.unk_08 ;
	kb_func_key() ;
	if( kbd->key[0] == kb_w.key[0] && kb_w.key[0] != 0 && kb_w.rep >= kb_w.unk_04 && !( kb_w.rep % kb_w.unk_00 ) ){
		ret = kb_cmd( kb_code_get( kbd->key[0], 0 ) ) ;
	}
	for( i = 0 ; i < 6 ; i ++ ){
		if( !ret && kb_w.unk_10[i] ) ret = kb_cmd( kb_code_get( kb_w.unk_10[i], 0 ) ) ;
	}
	return ret ;
}

/* the input mode the mode switches make */
void kb_func_key( void )
{
	if( !kb_w.unk_17 ){
		kb_w.unk_06 &= ~0x0d ;
		kb_w.unk_06 |= 0x02 ;
		if( kb_fix_pos() >= 0 ) kb_cmd( 100000 ) ;
	} else if( !kb_w.unk_18 ){
		kb_w.unk_06 &= ~0x05 ;
		kb_w.unk_06 |= 0x0a ;
	} else if( kb_w.unk_18 == 1 ){
		if( kb_w.unk_19 ){
			kb_w.unk_06 &= ~0x09 ;
			kb_w.unk_06 |= 0x06 ;
		} else {
			kb_w.unk_06 &= ~0x07 ;
			kb_w.unk_06 |= 0x08 ;
		}
	} else {
		if( kb_w.unk_19 ){
			kb_w.unk_06 &= ~0x08 ;
			kb_w.unk_06 |= 0x07 ;
		} else {
			kb_w.unk_06 &= ~0x06 ;
			kb_w.unk_06 |= 0x09 ;
		}
	}
}

/* the software keyboard by the pad: the lever moves over the keys, the buttons press them */
int kb_soft_move( void )
{
	register int ret ;
	Uint16 sw ;

	ret = 0 ;
	sw = kb_pad_get( 3 ) ;
	if( sw & KEY_LEVER ){
		kb_key.old[0] = kb_key.pos[0] ;
		kb_key.old[1] = kb_key.pos[1] ;
		if( sw & KEY_UP ) kb_key.pos[1] -- ;
		else if( sw & KEY_DOWN ) kb_key.pos[1] ++ ;
		if( sw & KEY_LEFT ){
			kb_key.pos[0] = kb_key.key[kb_key.unk_04].x ;
			kb_key.pos[0] -- ;
		} else if( sw & KEY_RIGHT ){
			kb_key.pos[0] = kb_key.key[kb_key.unk_04].x + kb_key.key[kb_key.unk_04].w ;
		}
		kb_key.pos[0] = ( kb_key.pos[0] + 15 ) % 15 ;
		kb_key.pos[1] = ( kb_key.pos[1] + 6 ) % 6 ;
		if( kb_key.pos[1] == 0 && kb_key.pos[0] == 10 ){
			if( sw & KEY_UP ) kb_key.pos[1] = 5 ;
			else if( sw & KEY_DOWN ) kb_key.pos[1] = 1 ;
			else if( sw & KEY_LEFT ) kb_key.pos[0] = 9 ;
			else kb_key.pos[0] = 11 ;
		}
		kb_key.unk_05 = kb_key.unk_04 ;
		kb_key.unk_04 = kb_soft_key( kb_key.pos[0], kb_key.pos[1] ) ;
		if( kb_key.pos[0] != kb_key.old[0] ) kb_key.pos[0] = kb_key.key[kb_key.unk_04].x ;
		na_req( 1 ) ;
	}
	if( kb_w.unk_06 & 4 ){
		kb_w.unk_06 &= ~4 ;
		kb_w.unk_19 = 0 ;
		kb_func_key() ;
	}
	if( kb_key.unk_07 ){
		if( !-- kb_key.unk_07 ) kb_key.unk_06 = 99 ;
	}
	if( sw & KEY_LK ){
		kb_key.unk_06 = kb_key.unk_04 ;
		kb_key.unk_07 = 10 ;
		ret = kb_cmd( kb_code_get( kb_key.unk_04, 1 ) ) ;
		if( ret ) na_req( 2 ) ;
		else na_req( 4 ) ;
	} else if( sw & KEY_HP ){
		ret = kb_cmd( KB_CMD ) ;
		if( ret ) na_req( 2 ) ;
		else na_req( 4 ) ;
	} else if( sw & KEY_HK ){
		ret = kb_cmd( KB_CMD + 43 ) ;
	} else if( sw & KEY_LP ){
		ret = kb_cmd( KB_CMD + 44 ) ;
	} else if( sw & KEY_A1 ){
		if( kb_w.unk_18 == 1 ) ret = kb_cmd( KB_CMD + 39 ) ;
		else if( kb_w.unk_18 == 2 ) ret = kb_cmd( KB_CMD + 40 ) ;
		else if( kb_w.unk_18 == 0 && !( kb_w.unk_06 & 0x40 ) ) ret = kb_cmd( KB_CMD + 41 ) ;
		else if( kb_w.unk_18 == 0 && ( kb_w.unk_06 & 0x40 ) ){
			if( kb_w.unk_08 & 8 ) ret = kb_cmd( KB_CMD + 38 ) ;
			else ret = kb_cmd( KB_CMD + 40 ) ;
		}
		na_req( 4 ) ;
	}
	return ret ;
}

int kb_soft_key( int x, int y )
{
	return kb_soft_tbl[y][x] ;
}

/* the character or command of a key in the input mode */
int kb_code_get( Uint8 key, int soft )
{
	int code ;

	if( kb_w.unk_06 & 2 ){
		if( !soft ) code = kb_hard_alnum( key ) ;
		else code = kb_soft_alnum( key ) ;
	} else {
		if( !soft ) code = kb_hard_kana( key ) ;
		else code = kb_soft_kana( key ) ;
		if( code == KB_CMD + 49 ){
			if( !soft ) code = kb_hard_alnum( key ) ;
			else code = kb_soft_alnum( key ) ;
		} else if( kb_w.unk_06 & 1 ){
			if( code >= 0x829f && code <= 0x82dd ) code += 0xa1 ;
			else if( code >= 0x82de && code <= 0x82f1 ) code += 0xa2 ;
		}
	}
	return code ;
}

/* a real keyboard's key, by shift and caps lock: full width where the mode asks for it */
int kb_hard_alnum( Uint8 key )
{
	char *p ;

	if( kb_w.unk_06 & 8 ){
		if( ( kb_w.unk_06 & 0x10 ) && ( kb_w.unk_06 & 0x40 ) ) p = kb_hard_str3[key] ;
		else if( !( kb_w.unk_06 & 0x10 ) && ( kb_w.unk_06 & 0x40 ) ) p = kb_hard_str2[key] ;
		else if( ( kb_w.unk_06 & 0x10 ) && !( kb_w.unk_06 & 0x40 ) ) p = kb_hard_str1[key] ;
		else p = kb_hard_str0[key] ;
		if( p ) return ( Uint8 )p[0] << 8 | ( Uint8 )p[1] ;
	}
	if( ( kb_w.unk_06 & 0x10 ) && ( kb_w.unk_06 & 0x40 ) ) return kb_hard_code3[key] ;
	else if( !( kb_w.unk_06 & 0x10 ) && ( kb_w.unk_06 & 0x40 ) ) return kb_hard_code2[key] ;
	else if( ( kb_w.unk_06 & 0x10 ) && !( kb_w.unk_06 & 0x40 ) ) return kb_hard_code1[key] ;
	else return kb_hard_code0[key] ;
}

/* the software keyboard's key */
int kb_soft_alnum( Uint8 key )
{
	char *p ;

	if( kb_w.unk_06 & 8 ){
		if( ( ( kb_w.unk_06 & 0x10 ) && !( kb_w.unk_06 & 0x40 ) ) || ( ( kb_w.unk_06 & 0x40 ) && !( kb_w.unk_06 & 0x10 ) ) ) p = kb_soft_str1[key] ;
		else p = kb_soft_str0[key] ;
		if( p ) return ( Uint8 )p[0] << 8 | ( Uint8 )p[1] ;
	}
	if( ( ( kb_w.unk_06 & 0x10 ) && !( kb_w.unk_06 & 0x40 ) ) || ( ( kb_w.unk_06 & 0x40 ) && !( kb_w.unk_06 & 0x10 ) ) ) return kb_soft_code1[key] ;
	else return kb_soft_code0[key] ;
}

/* a real keyboard's key in kana */
int kb_hard_kana( Uint8 key )
{
	int m ;
	char *p ;

	m = kb_w.unk_06 ;
	if( ( ( m & 0x10 ) && !( m & 0x40 ) ) || ( ( m & 0x40 ) && !( m & 0x10 ) ) ) p = kb_hard_kana_str1[key] ;
	else p = kb_hard_kana_str0[key] ;
	if( p ) return ( Uint8 )p[0] << 8 | ( Uint8 )p[1] ;
	return KB_CMD + 49 ;
}

/* the software keyboard's key in kana */
int kb_soft_kana( Uint8 key )
{
	char *p ;

	p = kb_soft_kana_str[key] ;
	if( p ) return ( Uint8 )p[0] << 8 | ( Uint8 )p[1] ;
	return KB_CMD + 49 ;
}

/* a command or a character for the text: 1 when the text is finished */
int kb_cmd( int code )
{
	int daku[40] = {
		0x82a9, 0x82ab, 0x82ad, 0x82af, 0x82b1, 0x82b3, 0x82b5, 0x82b7, 0x82b9, 0x82bb,
		0x82bd, 0x82bf, 0x82c2, 0x82c4, 0x82c6, 0x834a, 0x834c, 0x834e, 0x8350, 0x8352,
		0x8354, 0x8356, 0x8358, 0x835a, 0x835c, 0x835e, 0x8360, 0x8363, 0x8365, 0x8367,
		0x836e, 0x8371, 0x8374, 0x8377, 0x837a, 0x82cd, 0x82d0, 0x82d3, 0x82d6, 0x82d9
	} ;
	int small[20] = {
		0x82a0, 0x82a2, 0x82a4, 0x82a6, 0x82a8, 0x82c2, 0x82e2, 0x82e4, 0x82e6, 0x82ed,
		0x8341, 0x8343, 0x8345, 0x8347, 0x8349, 0x8363, 0x8384, 0x8386, 0x8388, 0x838f
	} ;
	register int ret ;
	register int j ;
	register int i ;
	int w ;
	Uint8 fix ;

	ret = 0 ;
	switch( code ){
	case 0 :
		break ;
	case KB_CMD + 42 :
	case KB_CMD + 45 :
	case KB_CMD :
		fix = 16 ;
		for( w = 0 ; w < kb_edit.num ; w ++ ) fix &= kb_edit.ch[w].type & 16 ;
		if( fix ){
			ret = 1 ;
		} else {
			for( i = 0 ; i < kb_edit.pos ; i ++ ) kb_edit.ch[i].type = ( kb_edit.ch[i].type & 15 ) | 0x50 ;
			if( kb_edit.unk_155 || kb_edit.unk_156 ) kb_edit.len -= kb_edit.unk_154 - kb_edit.unk_157 ;
			kb_edit.unk_156 = kb_edit.unk_155 = 0 ;
			kb_edit.unk_347 = 0 ;
			while( kb_edit.pos < kb_edit.num && !( kb_edit.ch[kb_edit.pos].type & 16 ) ){
				kb_edit.ch[kb_edit.pos].type = ( kb_edit.ch[kb_edit.pos].type & 15 ) | 0x50 ;
				kb_edit.pos ++ ;
			}
		}
		break ;
	case KB_CMD + 1 :
		if( ( i = kb_fix_pos() ) >= 0 ){
			kb_edit.pos = i ;
			w = j = 0 ;
			while( j + i < kb_edit.num && !( kb_edit.ch[j + i].type & 16 ) ){
				w ++ ;
				if( kb_edit.ch[j + i].type & 2 ) w ++ ;
				j ++ ;
			}
			for( ; i < kb_edit.num - 1 ; i ++ ) kb_edit.ch[i] = kb_edit.ch[j + i] ;
			kb_edit.ch[kb_edit.num - 1].type = 0 ;
			kb_edit.num -= j ;
			if( kb_edit.unk_155 || kb_edit.unk_156 ){
				kb_edit.len -= kb_edit.unk_154 ;
				kb_edit.unk_156 = kb_edit.unk_155 = 0 ;
			} else {
				kb_edit.len -= w ;
			}
		}
		break ;
	case KB_CMD + 2 :
		if( kb_edit.unk_155 ){
			kb_edit.unk_346 = 6 ;
			kb_cmd( KB_CMD + 34 ) ;
			kb_edit.unk_156 = kb_edit.unk_155 = 0 ;
		} else {
			if( ( i = kb_fix_pos() ) < 0 ) i = 0 ;
			if( kb_edit.pos > i ){
				kb_edit.len -- ;
				if( kb_edit.ch[kb_edit.pos - 1].type & 2 ) kb_edit.len -- ;
				if( kb_edit.pos != kb_edit.num ){
					for( i = kb_edit.pos ; i < kb_edit.num ; i ++ ) kb_edit.ch[i - 1] = kb_edit.ch[i] ;
				}
				kb_edit.ch[kb_edit.num - 1].type = 0 ;
				kb_edit.num -- ;
				kb_edit.pos -- ;
			}
		}
		break ;
	case KB_CMD + 3 :
		break ;
	case KB_CMD + 4 :
		if( kb_fix_pos() >= 0 && !kb_edit.unk_155 ){
			kb_edit.unk_346 = 0 ;
			kb_cmd( KB_CMD + 34 ) ;
		} else if( kb_edit.unk_155 ){
			if( kb_w.unk_06 & 16 ) kb_edit.unk_346 = 2 ;
			else kb_edit.unk_346 = 1 ;
			kb_cmd( KB_CMD + 34 ) ;
		} else {
			kb_char_ins( ' ' ) ;
		}
		break ;
	case KB_CMD + 5 : case KB_CMD + 6 : case KB_CMD + 7 : case KB_CMD + 8 : case KB_CMD + 9 :
	case KB_CMD + 10 : case KB_CMD + 11 : case KB_CMD + 12 : case KB_CMD + 13 : case KB_CMD + 14 :
	case KB_CMD + 15 : case KB_CMD + 16 : case KB_CMD + 17 : case KB_CMD + 18 : case KB_CMD + 19 :
	case KB_CMD + 20 : case KB_CMD + 21 : case KB_CMD + 22 : case KB_CMD + 23 : case KB_CMD + 24 :
		break ;
	case KB_CMD + 25 :
		if( kb_edit.unk_155 ) break ;
		if( kb_fix_pos() >= 0 ){
			if( kb_edit.ch[kb_edit.pos].type & 16 ) break ;
		}
		if( kb_edit.pos < kb_edit.num ){
			kb_edit.len -- ;
			if( kb_edit.ch[kb_edit.pos].type & 2 ) kb_edit.len -- ;
			if( kb_edit.pos != kb_edit.num - 1 ){
				for( i = kb_edit.pos ; i < kb_edit.num - 1 ; i ++ ) kb_edit.ch[i] = kb_edit.ch[i + 1] ;
			}
			kb_edit.ch[kb_edit.num - 1].type = 0 ;
			kb_edit.num -- ;
		}
		break ;
	case KB_CMD + 26 :
		break ;
	case KB_CMD + 27 :
		break ;
	case KB_CMD + 28 :
		if( kb_edit.unk_155 ){
			kb_edit.unk_346 = 4 ;
			kb_cmd( KB_CMD + 34 ) ;
		} else {
			if( kb_edit.pos < kb_edit.num ) kb_edit.pos ++ ;
			if( kb_fix_pos() >= 0 ){
				if( kb_edit.ch[kb_edit.pos - 1].type & 16 ) kb_edit.pos -- ;
			}
		}
		break ;
	case KB_CMD + 29 :
		if( kb_edit.unk_155 ){
			kb_edit.unk_346 = 3 ;
			kb_cmd( KB_CMD + 34 ) ;
		} else {
			if( kb_edit.pos > 0 ) kb_edit.pos -- ;
			if( ( i = kb_fix_pos() ) >= 0 ){
				if( kb_edit.pos < i ) kb_edit.pos = i ;
			}
		}
		break ;
	case KB_CMD + 30 :
		if( kb_edit.unk_155 ) kb_edit.unk_346 = 1 ;
		kb_cmd( KB_CMD + 34 ) ;
		break ;
	case KB_CMD + 31 :
		if( kb_edit.unk_155 ) kb_edit.unk_346 = 2 ;
		kb_cmd( KB_CMD + 34 ) ;
		break ;
	case KB_CMD + 32 :
	case KB_CMD + 33 :
		break ;
	case KB_CMD + 34 :
		kb_henkan( 0 ) ;
		break ;
	case KB_CMD + 35 :
		if( kb_fix_pos() >= 0 ){
			if( !kb_edit.unk_155 ){
				kb_cmd( KB_CMD ) ;
				break ;
			} else {
				kb_edit.unk_346 = 5 ;
				kb_cmd( KB_CMD + 34 ) ;
			}
		}
		break ;
	case KB_CMD + 37 :
		for( i = 0 ; i < sizeof( small ) / sizeof( small[0] ) ; i ++ ){
			if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == small[i] ) kb_edit.ch[kb_edit.pos - 1].code.w -- ;
			else if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == small[i] - 1 ) kb_edit.ch[kb_edit.pos - 1].code.w ++ ;
		}
		if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == 0x834a ) kb_edit.ch[kb_edit.pos - 1].code.w = 0x8395 ;
		else if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == 0x8395 ) kb_edit.ch[kb_edit.pos - 1].code.w = 0x834a ;
		if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == 0x8350 ) kb_edit.ch[kb_edit.pos - 1].code.w = 0x8396 ;
		else if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == 0x8396 ) kb_edit.ch[kb_edit.pos - 1].code.w = 0x8350 ;
		break ;
	case KB_CMD + 38 :
		if( kb_w.unk_08 & 8 ){
			kb_w.unk_17 = 1 ;
			kb_w.unk_19 = 0 ;
			kb_w.unk_18 = 1 ;
			kb_func_key() ;
		}
		break ;
	case KB_CMD + 39 :
		if( kb_w.unk_08 & 8 ){
			kb_w.unk_17 = 1 ;
			kb_w.unk_19 = 0 ;
			kb_w.unk_18 = 2 ;
			kb_func_key() ;
		}
		break ;
	case KB_CMD + 40 :
		kb_w.unk_06 &= ~0x51 ;
		kb_w.unk_06 |= 0x02 ;
		kb_w.unk_18 = 0 ;
		kb_func_key() ;
		break ;
	case KB_CMD + 41 :
		kb_w.unk_06 &= ~0x51 ;
		kb_w.unk_06 |= 0x42 ;
		kb_w.unk_18 = 0 ;
		kb_func_key() ;
		break ;
	case KB_CMD + 43 :
		if( kb_fix_pos() >= 0 && !kb_edit.unk_155 ){
			kb_edit.unk_346 = 0 ;
			kb_cmd( KB_CMD + 34 ) ;
		} else if( kb_edit.unk_155 ){
			kb_edit.unk_346 = 2 ;
			kb_cmd( KB_CMD + 34 ) ;
		} else {
			kb_cmd( KB_CMD + 2 ) ;
		}
		if( 1 ) na_req( 4 ) ;	/* the ROM's shared kb_func_key call needs the test */
		break ;
	case KB_CMD + 44 :
		if( kb_fix_pos() >= 0 && !kb_edit.unk_155 ){
			kb_edit.unk_346 = 0 ;
			kb_cmd( KB_CMD + 34 ) ;
			na_req( 4 ) ;
		} else if( kb_edit.unk_155 ){
			kb_edit.unk_346 = 1 ;
			kb_cmd( KB_CMD + 34 ) ;
			na_req( 4 ) ;
		}
		break ;
	case KB_CMD + 47 :
		if( kb_w.unk_08 & 8 ){
			kb_w.unk_17 = 1 ;
			kb_func_key() ;
		}
		break ;
	case KB_CMD + 48 :
		if( !( kb_w.unk_08 & 0x100 ) ){
			if( kb_fix_pos() >= 0 ) kb_cmd( KB_CMD ) ;
			kb_w.unk_17 = 0 ;
			kb_w.unk_18 = 0 ;
			kb_func_key() ;
		}
		break ;
	case KB_CMD + 49 :
		break ;
	default :
		if( kb_fix_pos() >= 0 && kb_edit.unk_155 ) kb_cmd( KB_CMD ) ;
		if( code == 0x814a ){
			for( i = 0 ; i < sizeof( daku ) / sizeof( daku[0] ) ; i ++ ){
				if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == daku[i] ) kb_edit.ch[kb_edit.pos - 1].code.w ++ ;
			}
			if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == 0x8345 ) kb_edit.ch[kb_edit.pos - 1].code.w = 0x8394 ;
		} else if( code == 0x814b ){
			for( i = 30 ; i < 40 ; i ++ ){
				if( ( Uint16 )kb_edit.ch[kb_edit.pos - 1].code.w == daku[i] ) kb_edit.ch[kb_edit.pos - 1].code.w += 2 ;
			}
		} else {
			kb_char_ins( code ) ;
		}
		break ;
	}
	return ret ;
}

/* puts a character at the cursor: 0 when the text is full */
int kb_char_ins( int code )
{
	register int i ;

	if( kb_edit.unk_347 >= 36 || kb_edit.len > 156 ) return 0 ;
	if( kb_w.unk_06 & 0x80 ){
		if( kb_edit.pos < kb_edit.num ){
			for( i = kb_edit.num - 1 ; i >= kb_edit.pos ; i -- ){
				kb_edit.ch[i + 1] = kb_edit.ch[i] ;
				kb_edit.ch[i + 1].type |= 0x40 ;
			}
		}
	}
	if( code == ' ' && kb_w.unk_17 ){
		kb_edit.ch[kb_edit.pos].code.w = 0x8140 ;
		kb_edit.ch[kb_edit.pos].type = 0x52 ;
		kb_edit.len += 2 ;
	} else if( !( kb_w.unk_06 & 8 ) || ( kb_w.unk_06 & 4 ) ){
		kb_edit.ch[kb_edit.pos].type = 0x51 ;
		kb_edit.len ++ ;
		kb_edit.ch[kb_edit.pos].code.w = code ;
		if( kb_w.unk_06 & 4 && code != ' ' ){
			kb_edit.ch[kb_edit.pos].type &= ~0x10 ;
			kb_edit.ch[kb_edit.pos].type |= 0x20 ;
		}
	} else {
		kb_edit.ch[kb_edit.pos].code.w = code ;
		kb_edit.ch[kb_edit.pos].type = 0x42 ;
		kb_edit.len += 2 ;
		kb_edit.ch[kb_edit.pos].type |= 0x40 ;
	}
	kb_edit.ch[kb_edit.pos].type |= 0x40 ;
	if( kb_w.unk_06 & 0x80 || kb_edit.pos == kb_edit.num ) kb_edit.num ++ ;
	kb_edit.pos ++ ;
	return 1 ;
}

/* the text as a string: all for the whole of it, in the box's width, with a password's characters hidden */
void kb_str_make( KB_EDIT *ed, char *str, int all )
{
	Float w, lim ;
	register int i, n, num ;
	char c ;	/* the mask: the ROM loads it with the loop's start, as a local */

	kb_kouho() ;
	i = 0 ;
	ed->unk_0b = 0 ;
	ed->unk_0a = 0 ;
	num = ed->num ;
	if( !num ){
		*str = 0 ;
		return ;
	}
	if( all ){
		if( ed->unk_0c > ed->pos - 2 ){
			if( ( ed->unk_0c = ed->pos - 2 ) < 0 ) ed->unk_0c = 0 ;
		}
		lim = 440.0f ;
		for( i = ed->unk_0c, w = 0.0f ; i < ed->pos - 1 ; i ++ ){
			if( ed->ch[i].type & 2 ) w += 20.0f ;
			else if( ed->ch[i].type & 1 ) w += 12.0f ;
			if( w >= lim ){
				if( ed->ch[ed->unk_0c].type & 2 ) w -= 20.0f ;
				else w -= 12.0f ;
				ed->unk_0c ++ ;
			}
		}
		i = ed->unk_0c ;
	}
	for( w = 0.0f, n = 0, c = '*' ; i < num ; i ++ ){
		if( ed->ch[i].type & 2 ){
			if( !all && n > kb_edit.max - 2 ) break ;
			*str++ = SJIS_HI( ed->ch[i] ) ;
			*str++ = SJIS_LO( ed->ch[i] ) ;
			if( ed->pos > i ) ed->unk_0b ++ ;
			w += 20.0f ;
			n += 2 ;
		} else if( ed->ch[i].type & 1 ){
			if( !all && n > kb_edit.max - 1 ) break ;
			if( all && ( kb_w.unk_06 & 0x200 ) ) *str++ = c ;
			else *str++ = SJIS_LO( ed->ch[i] ) ;
			if( ed->pos > i ) ed->unk_0a ++ ;
			w += 12.0f ;
			n ++ ;
		}
		if( 0 ) w += 20.0f, w += 12.0f ;	/* left out of the build */
		if( all && w >= 440.0f ) break ;
	}
	*str = 0 ;
}

/* the conversion of the text not yet fixed: a command of kb_edit.unk_346 to ASK.BIN */
void kb_henkan( int no )
{
	KB_CHAR ch[80] ;
	char str[80] ;
	register int i ;
	int n ;
	register int len ;
	int d, old ;
	register char *p, *q ;
	char *s ;
	Uint8 c ;

	for( i = 0 ; i < kb_edit.num ; i ++ ){
		if( !( kb_edit.ch[i].type & 16 ) ) break ;
	}
	if( i == kb_edit.num ) return ;
	if( !kb_edit.unk_155 ){
		kb_edit.unk_340 = i ;
		p = kb_edit.yomi ;
		n = 0 ;
		for( ; i < kb_edit.num ; i ++ ){
			if( kb_edit.ch[i].type & 16 ) break ;
			if( kb_edit.ch[i].type & 2 ){
				*p++ = SJIS_HI( kb_edit.ch[i] ) ;
				*p++ = SJIS_LO( kb_edit.ch[i] ) ;
				n += 2 ;
			} else {
				*p++ = SJIS_LO( kb_edit.ch[i] ) ;
				n ++ ;
			}
		}
		*p = 0 ;
		kb_edit.unk_342 = i ;
		kb_edit.unk_155 = 1 ;
		kb_edit.unk_154 = n ;
		kb_edit.unk_346 = 0 ;
		kb_edit.unk_344 = kb_edit.unk_342 - kb_edit.unk_340 ;
	}
	p = kb_edit.yomi ;
	if( !kb_edit.unk_156 ){
		if( ( kb_edit.unk_158 = ASK_HENKAN( p, kb_edit.kanji, kb_edit.rest ) ) > 0 ){
			kb_edit.unk_156 = 1 ;
			kb_edit.unk_15c = 1 ;
		} else {
			kb_edit.unk_155 = 0 ;
			return ;
		}
	} else {
		switch( q = kb_edit.rest, kb_edit.unk_346 ){	/* the ROM loads q after the old count */
		case 1 :
			if( kb_edit.unk_15c != 255 && ++ kb_edit.unk_15c == kb_edit.unk_158 + 1 ){
				ASK_YOMI( kb_edit.kanji, kb_edit.rest ) ;
				kb_edit.unk_15c = 255 ;
			} else {
				ASK_NEXT( kb_edit.kanji, kb_edit.rest ) ;
			}
			break ;
		case 2 :
			if( kb_edit.unk_15c != 255 && !-- kb_edit.unk_15c ){
				ASK_YOMI( kb_edit.kanji, kb_edit.rest ) ;
				kb_edit.unk_15c = 255 ;
			} else {
				ASK_PREV( kb_edit.kanji, kb_edit.rest ) ;
			}
			break ;
		case 3 :
			old = kb_edit.unk_158 ;
			s = kb_edit.kanji ;	/* through a local in these two cases: the ROM keeps q in r5 */
			if( kb_edit.unk_158 = ASK_SHORT( s, q ) ) kb_edit.unk_15c = 1 ;
			else kb_edit.unk_158 = old ;
			break ;
		case 4 :
			old = kb_edit.unk_158 ;
			s = kb_edit.kanji ;
			if( kb_edit.unk_158 = ASK_LONG( s, q ) ) kb_edit.unk_15c = 1 ;
			else kb_edit.unk_158 = old ;
			break ;
		case 5 :
			kb_edit.unk_15c = 1 ;
			kb_edit.unk_158 = ASK_CANCEL( kb_edit.kanji, kb_edit.rest ) ;
			len = sjis_strlen( kb_edit.kanji ) ;
			q = kb_edit.kanji ;
			p = kb_edit.yomi ;
			for( i = 0 ; i < len ; i ++ ){
				c = *p & 0xf0 ;
				if( c == 0x80 || c == 0x90 || c == 0xe0 ) *q++ = *p++ ;
				*q++ = *p++ ;
			}
			*q = 0 ;
			break ;
		case 6 :
			sprintf( ( char * )kb_edit.kanji, p ) ;	/* the ROM needs the cast: p is in r14 */
			kb_edit.rest[0] = 0 ;
			break ;
		}
	}
	len = sjis_strlen( kb_edit.kanji ) ;
	kb_edit.pos = kb_edit.unk_340 + len ;
	sprintf( str, kb_edit.kanji ) ;
	if( kb_edit.rest[0] ) strcat( str, kb_edit.rest ) ;
	p = str ;
	n = 0 ;
	len = 0 ;
	while( *p ){
		c = *p & 0xf0 ;
		if( c == 0x80 || c == 0x90 || c == 0xe0 ){
			ch[n].type = 0x42 ;
			SJIS_HI( ch[n] ) = *p++ ;
			SJIS_LO( ch[n] ) = *p++ ;
			len += 2 ;
		} else {
			len ++ ;
			ch[n].type = 0x41 ;
			SJIS_LO( ch[n] ) = *p++ ;
		}
		kb_edit.unk_157 = len ;
		n ++ ;
	}
	d = kb_edit.unk_344 - n ;
	kb_edit.unk_344 = n ;
	if( 0 ){ if( kb_edit.unk_344 ) if( kb_edit.unk_344 ) ; }	/* left out of the build */
	if( !d ){
		for( i = 0 ; i < n ; i ++ ) kb_edit.ch[kb_edit.unk_340 + i] = ch[i] ;
	} else if( d > 0 ){
		for( i = kb_edit.unk_340 + n ; i <= kb_edit.num - d - 1 ; i ++ ){
			kb_edit.ch[i] = kb_edit.ch[i + d] ;
			kb_edit.ch[i].type |= 0x50 ;
		}
		for( i = 0 ; i < d ; i ++ ) kb_edit.ch[kb_edit.num - i - 1].type = 0 ;
		for( i = 0 ; i < n ; i ++ ){
			kb_edit.ch[kb_edit.unk_340 + i] = ch[i] ;
			kb_edit.ch[kb_edit.unk_340 + i].type |= 0x40 ;
		}
		kb_edit.num -= d ;
	} else if( d < 0 ){
		for( i = kb_edit.num - 1 ; i >= kb_edit.unk_342 - 1 ; i -- ){
			kb_edit.ch[i - d].type = kb_edit.ch[i].type | 0x40 ;
			kb_edit.ch[i - d].code.w = kb_edit.ch[i].code.w ;
		}
		for( i = 0 ; i < n ; i ++ ) kb_edit.ch[kb_edit.unk_340 + i] = ch[i] ;
		kb_edit.num -= d ;
	}
	kb_edit.unk_342 = kb_edit.pos ;
}

/* romaji typed before the cursor, made kana */
void kb_kouho( void )
{
	char in[80] ;
	char out[80] ;
	register int i ;
	int n ;
	int d ;
	int w ;
	char *p ;
	char *q ;

	if( !kb_edit.num ) return ;
	if( !( kb_w.unk_06 & 4 ) ) return ;
	n = 0 ;
	while( kb_edit.ch[kb_edit.pos - n - 1].type & 0x20 && kb_edit.pos - n - 1 >= 0 ) n ++ ;
	if( !n ) return ;
	i = 0 ;
	p = in ;
	for( ; i < n ; i ++ ) p[i] = SJIS_LO( kb_edit.ch[kb_edit.pos - n + i] ) ;
	if( n == 1 ){
		if( *( char * )&in == 'n' ) return ;	/* the ROM reads the array itself, not through p */
		if( *( char * )&in == 'N' ) return ;
	}
	p[n] = 0 ;
	q = out ;
	d = ( kb_w.unk_06 & 1 ) ? ASK_KATAKANA( p, q ) : ASK_HIRAGANA( p, q ) ;
	if( d ) return ;
	i = 0 ;
	p = q ;
	while( *p ){
		SJIS_HI( kb_edit.ch[kb_edit.pos - n + i] ) = *p++ ;
		SJIS_LO( kb_edit.ch[kb_edit.pos - n + i] ) = *p++ ;
		kb_edit.ch[kb_edit.pos - n + i].type = 0x42 ;
		i ++ ;
	}
	d = n - i ;
	if( 0 ) *p = 0 ;	/* left out of the build */
	w = n - i * 2 ;
	if( kb_edit.pos < kb_edit.num && d > 0 ){
		for( i = kb_edit.pos ; i < kb_edit.num ; i ++ ){
			kb_edit.ch[i - d].code = kb_edit.ch[i].code ;
			kb_edit.ch[i - d].type = kb_edit.ch[i].type | 0x40 ;
		}
	}
	for( i = 0 ; i < d ; i ++ ) kb_edit.ch[kb_edit.num - i - i].type = 0 ;
	kb_edit.num -= d ;
	kb_edit.pos -= d ;
	kb_edit.len -= w ;
}

/* the first character not yet fixed, or -1 */
int kb_fix_pos( void )
{
	int i ;

	for( i = 0 ; i < kb_edit.num ; i ++ ){
		if( !( kb_edit.ch[i].type & 16 ) ) break ;
	}
	if( i == kb_edit.num ) i = -1 ;
	return i ;
}

/* the characters of a Shift-JIS string */
int sjis_strlen( char *str )
{
	register int n ;
	register Uint8 c ;

	n = 0 ;
	while( *str ){
		c = *str & 0xf0 ;
		if( c == 0x80 || c == 0x90 || c == 0xe0 ) str += 2 ;
		else str ++ ;
		n ++ ;
	}
	return n ;
}

/* the pads' keys of the sides asked for, or in the network game of the sides in it */
Uint16 kb_pad_get( Uint8 side_bit )
{
	Uint16 sw ;

	if( Sys->home_mode == HOME_NETWORK ) side_bit = Sys->entry_side ;
	sw = 0 ;
	if( side_bit & 1 ) sw |= sw_work[0].on | ( sw_work[0].rep & ( KEY_LEVER | KEY_ATCK ) ) ;
	if( side_bit & 2 ) sw |= sw_work[1].on | ( sw_work[1].rep & ( KEY_LEVER | KEY_ATCK ) ) ;
	return sw ;
}

/* draws the box, the cursor, the text's marks and the keyboard, set with SysTransSet */
void kb_disp( void )
{
	NLsprarg spr ;
	register int i, n, w ;
	KB_WORK *kw = &kb_w ;

	spr.ang = 0 ;
	spr.trnsl = 1.0f ;
	spr.listType = -1 ;
	spr.attr = NLSPR_DISP_LT ;
	spr.base_color = -1 ;
	spr.offset_color = 0 ;
	kb_spr_set( &kb_spr_tbl[18], &spr, 1.0f ) ;
	spr.x = kw->text_pos.x ;
	spr.y = kw->text_pos.y ;
	spr.z = kw->text_pos.z ;
	nlSprPut( &spr ) ;
	if( kb_edit.pos != kb_edit.unk_06 ){
		kb_edit.unk_06 = kb_edit.pos ;
		kb_edit.unk_08 = 0 ;
	}
	kb_edit.unk_08 = ( kb_edit.unk_08 + 9 ) % 360 ;
	kb_spr_set( &kb_spr_tbl[5], &spr, 1.0f ) ;
	spr.x = kw->text_pos.x + kb_edit.unk_0a * 12.0f + kb_edit.unk_0b * 20.0f + -10.0f ;
	spr.y = kw->text_pos.y ;
	spr.z = kw->text_pos.z + -0.2f ;
	spr.trnsl = nlCos( DEG_ANG( kb_edit.unk_08 ) ) * 0.5f + 0.5f ;
	nlSprPut( &spr ) ;
	spr.trnsl = 1.0f ;
	if( kb_fix_pos() >= 0 ){
		n = 0 ;
		for( i = kb_edit.unk_0c ; i < kb_edit.num ; i ++ ){
			if( !( kb_edit.ch[i].type & 16 ) ) break ;
			if( kb_edit.ch[i].type & 2 ) n += 20.0f ;
			else n += 12.0f ;
		}
		if( i <= kb_edit.num - 1 ){
			for( w = 0 ; i < kb_edit.num ; i ++ ){
				if( kb_edit.ch[i].type & 16 ) break ;
				if( kb_edit.ch[i].type & 2 ) w += 20.0f ;
				else w += 12.0f ;
				if( w >= 440.0f ) break ;
			}
			if( w ){
				kb_spr_set( &kb_spr_tbl[28], &spr, 1.0f ) ;
				spr.x = kb_w.text_pos.x + n + 8.0f ;
				spr.y = kb_w.text_pos.y + -2.0f ;
				spr.z = kb_w.text_pos.z + -0.3f ;
				spr.zm_x = spr.zm_x / 32.0f * w ;
				nlSprPut( &spr ) ;
				if( kb_edit.unk_155 ){
					kb_spr_set( &kb_spr_tbl[29], &spr, 1.0f ) ;
					spr.x = kb_w.text_pos.x + n + 8.0f ;
					spr.y = kb_w.text_pos.y + 0.0f ;
					spr.z = kb_w.text_pos.z + 0.0f ;
					spr.zm_x = spr.zm_x / 32.0f * w ;
					nlSprPut( &spr ) ;
				}
			}
		}
	}
	if( kw->unk_16 ) kb_disp_key( &spr ) ;
	else kb_disp_mode( &spr ) ;
}

/* a sprite's texture and size from a piece of the keyboard's texture */
void kb_spr_set( KB_SPR *s, NLsprarg *spr, Float zm )
{
	spr->sprno = s->tex + 0x850 ;
	spr->u0 = s->x0 / 512.0f ;
	spr->v0 = s->y0 / 512.0f ;
	spr->u1 = s->x1 / 512.0f ;
	spr->v1 = s->y1 / 512.0f ;
	spr->zm_x = ( s->x1 - s->x0 ) / 512.0f * zm ;
	spr->zm_y = ( s->y1 - s->y0 ) / 512.0f * zm ;
}

/* the software keyboard: its keys, their tops and the cursor */
void kb_disp_key( NLsprarg *spr )
{
	register int i ;
	register int j ;
	register int k ;
	register Uint8 ( *p )[2] ;
	Float zm ;

	zm = 1.0f ;
	spr->z = kb_w.key_pos.z ;
	for( i = 0 ; i < 75 ; i ++ ){
		spr->x = kb_w.key_pos.x + kb_key.key[i].x * zm * 32.0f ;
		spr->y = kb_w.key_pos.y + kb_key.key[i].y * zm * 32.0f ;
		k = kb_unk_8C149953[i] ;
		for( j = 0 ; j < sizeof( kb_unk_8C14999E ) / sizeof( kb_unk_8C14999E[0] ) ; j ++ ){
			if( i == kb_unk_8C14999E[j][0] ){
				switch( kb_unk_8C14999E[j][1] ){
				case 0 :
					switch( i ){
					case 2 :
						if( ( kb_w.unk_08 & 8 ) == 8 ){
							if( kb_w.unk_18 == 1 ) k = 31 ;
						} else k = 15 ;
						break ;
					case 3 :
						if( ( kb_w.unk_08 & 8 ) != 8 ) k = 15 ;
						else if( kb_w.unk_18 == 2 ) k = 31 ;
						break ;
					default :
						if( !kb_w.unk_18 ){
							if( !( kb_w.unk_06 & 0x40 ) ){
								if( i == 4 ) k = 31 ;
							} else {
								if( i == 5 ) k = 31 ;
							}
						}
						break ;
					}
					break ;
				case 1 :
					if( kb_w.unk_18 ) k = 4 ;
					break ;
				case 2 :
					if( kb_w.unk_17 ) k = 30 ;
					else if( !( kb_w.unk_08 & 8 ) ) k = 14 ;
					break ;
				case 3 :
					if( !kb_w.unk_17 ) k = 30 ;
					else if( kb_w.unk_08 & 0x100 ) k = 14 ;
					break ;
				}
			}
		}
		kb_spr_set( &kb_spr_tbl[k], spr, zm ) ;
		nlSprPut( spr ) ;
	}
	if( kb_w.unk_06 & 2 ){
		p = kb_unk_8C1499FA ;
		if( ( kb_w.unk_06 & 16 ) && !( kb_w.unk_06 & 0x40 ) || ( kb_w.unk_06 & 0x40 ) && !( kb_w.unk_06 & 16 ) ) p = kb_unk_8C149A26 ;
	} else {
		p = kb_unk_8C1499BE ;
		if( kb_w.unk_06 & 1 ) p = kb_unk_8C1499DC ;
	}
	spr->z -= 0.2f ;
	for( i = 0 ; i < 75 ; i ++ ){
		if( ( *p )[0] != i ) continue ;
		spr->x = kb_w.key_pos.x + kb_key.key[i].x * zm * 32.0f ;
		spr->y = kb_w.key_pos.y + kb_key.key[i].y * zm * 32.0f ;
		k = ( *p )[1] ;
		if( i == 70 && k == 32 ) k += kb_w.unk_17 ;
		kb_spr_set( &kb_top_tbl[k], spr, zm ) ;
		p ++ ;
		nlSprPut( spr ) ;
	}
	spr->z += 0.1f ;
	spr->x = kb_w.key_pos.x + kb_key.key[kb_key.unk_04].x * zm * 32.0f ;
	spr->y = kb_w.key_pos.y + kb_key.key[kb_key.unk_04].y * zm * 32.0f ;
	if( kb_key.unk_06 != kb_key.unk_04 ) k = kb_key.key[kb_key.unk_04].w + 5 ;
	else k = kb_key.key[kb_key.unk_04].w + 9 ;
	kb_spr_set( &kb_spr_tbl[k], spr, zm ) ;
	nlSprPut( spr ) ;
}

/* the marks of the input modes, with a real keyboard */
void kb_disp_mode( NLsprarg *spr )
{
	register int k ;

	kb_spr_set( &kb_spr_tbl[19], spr, 1.0f ) ;
	spr->x = kb_w.mode_pos.x ;
	spr->y = kb_w.mode_pos.y ;
	spr->z = kb_w.mode_pos.z ;
	nlSprPut( spr ) ;
	k = 20 ;
	if( !kb_w.unk_17 ) k ++ ;
	kb_spr_set( &kb_spr_tbl[k], spr, 1.0f ) ;
	spr->x = kb_w.mode_pos.x ;
	spr->y = kb_w.mode_pos.y ;
	spr->z -= 0.1f ;
	nlSprPut( spr ) ;
	if( kb_w.unk_18 == 1 ) k = 22 ;
	else if( kb_w.unk_18 == 2 ) k = 23 ;
	else if( !( kb_w.unk_06 & 0x40 ) ) k = 24 ;
	else k = 25 ;
	kb_spr_set( &kb_spr_tbl[k], spr, 1.0f ) ;
	spr->x = kb_w.mode_pos.x + 32.0f ;
	nlSprPut( spr ) ;
	if( kb_w.unk_17 ){
		if( kb_w.unk_19 ) k = 27 ;
		else k = 26 ;
		kb_spr_set( &kb_spr_tbl[k], spr, 1.0f ) ;
		spr->x = kb_w.mode_pos.x + 64.0f ;
		nlSprPut( spr ) ;
	}
}
