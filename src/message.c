/* Messages on the screen: lines of text in the system font or in a font drawn as polygons, words
   made of sprites, the "N-pts. GET!" of the points a side earns, two sprite displays, and the
   home game's versus record. */
#include "player.h"

/* a character of a sprite word: its place on the texture, on the screen, its scale in percent,
   and whether it is mirrored */
typedef struct SPR_CHR {
	Uint8	u, v, w, h;
	Float	x, y;
	Float	scale;
	Sint32	flip;
} SPR_CHR;

/* the system font's colour and size, by MES_WORK's type */
typedef struct PRINT_FONT {
	Uint32	col;
	Float	size;
	Float	unk_08;
} PRINT_FONT;

/* the polygon font: the colours of its four corners (from mes_col_tbl), its size, the step of
   MES_WORK's place, and its width in characters of the texture */
typedef struct MES_FONT {
	Uint8	col[4];
	Float	size;
	Float	step;
	Float	wide;
} MES_FONT;

static const Uint8 spr_mes_00[] = { 1, 0, 0 } ;
static const Uint8 spr_mes_01[] = { 5, 0, 2, 8, 4, 22, 18 } ;
static const Uint8 spr_mes_02[] = { 5, 0, 12, 8, 4, 22, 18 } ;
static const Uint8 spr_mes_03[] = { 7, 0, 14, 9, 5, 26, 28, 22, 18 } ;
static const Uint8 spr_mes_04[] = { 6, 0, 16, 9, 5, 24, 20, 18 } ;
static const Uint8 spr_mes_05[] = { 7, 0, 30, 9, 5, 26, 28, 22, 18 } ;
static const Uint8 spr_mes_06[] = { 1, 0, 1 } ;
static const Uint8 spr_mes_07[] = { 5, 0, 3, 10, 6, 23, 19 } ;
static const Uint8 spr_mes_08[] = { 5, 0, 13, 10, 6, 23, 19 } ;
static const Uint8 spr_mes_09[] = { 7, 0, 15, 11, 7, 27, 29, 23, 19 } ;
static const Uint8 spr_mes_10[] = { 6, 0, 17, 11, 7, 25, 21, 19 } ;
static const Uint8 spr_mes_11[] = { 7, 0, 31, 11, 7, 27, 29, 23, 19 } ;	/* unused */
static const Uint8 spr_mes_12[] = { 3, 0, 32, 33, 34 } ;
#ifdef TARGET_DC
static const Uint8 spr_mes_13[] = { 4, 0, 37, 38, 39, 40 } ;
#else
static const Uint8 spr_mes_13[] = { 7, 0, 35, 36, 37, 38, 39, 40, 41 } ;
#endif
static const Uint8 spr_mes_14[] = { 10, 0, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51 } ;
static const Uint8 spr_mes_15[] = { 10, 0, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61 } ;
static const Uint8 spr_mes_16[] = { 10, 0, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71 } ;

static const SPR_CHR spr_chr_tbl[72] = {
	{ 144, 16, 112, 24, 16.0f, 8.0f, 100.0f, 0 },
	{ 144, 16, 112, 24, 512.0f, 8.0f, 100.0f, 0 },
	{ 0, 0, 96, 16, 24.0f, 316.0f, 100.0f, 0 },
	{ 0, 0, 96, 16, 520.0f, 316.0f, 100.0f, 0 },
	{ 0, 48, 64, 104, 76.0f, 340.0f, 100.0f, 0 },
	{ 0, 152, 64, 104, 76.0f, 340.0f, 100.0f, 0 },
	{ 0, 48, 64, 104, 500.0f, 340.0f, 100.0f, 0 },
	{ 0, 152, 64, 104, 500.0f, 340.0f, 100.0f, 0 },
	{ 0, 32, 32, 16, 92.0f, 380.0f, 100.0f, 0 },
	{ 64, 32, 32, 16, 92.0f, 380.0f, 100.0f, 0 },
	{ 32, 32, 32, 16, 516.0f, 380.0f, 100.0f, 0 },
	{ 96, 32, 32, 16, 516.0f, 380.0f, 100.0f, 0 },
	{ 0, 16, 48, 16, 48.0f, 316.0f, 100.0f, 0 },
	{ 0, 16, 48, 16, 544.0f, 316.0f, 100.0f, 0 },
	{ 48, 16, 96, 16, 24.0f, 316.0f, 100.0f, 0 },
	{ 48, 16, 96, 16, 520.0f, 316.0f, 100.0f, 0 },
	{ 96, 0, 144, 16, 24.0f, 316.0f, 100.0f, 0 },
	{ 96, 0, 144, 16, 472.0f, 316.0f, 100.0f, 0 },
	{ 72, 128, 184, 128, 16.0f, 328.0f, 100.0f, 0 },
	{ 72, 128, 184, 128, 440.0f, 328.0f, 100.0f, 1 },
	{ 72, 48, 176, 24, 16.0f, 312.0f, 100.0f, 0 },
	{ 72, 48, 176, 24, 448.0f, 312.0f, 100.0f, 1 },
	{ 72, 72, 128, 24, 16.0f, 312.0f, 100.0f, 0 },
	{ 72, 72, 128, 24, 496.0f, 312.0f, 100.0f, 1 },
	{ 72, 96, 160, 24, 16.0f, 312.0f, 100.0f, 0 },
	{ 72, 96, 160, 24, 464.0f, 312.0f, 100.0f, 1 },
	{ 72, 96, 104, 24, 16.0f, 312.0f, 100.0f, 0 },
	{ 72, 96, 104, 24, 520.0f, 312.0f, 100.0f, 1 },
	{ 224, 96, 8, 24, 120.0f, 312.0f, 100.0f, 0 },
	{ 224, 96, 8, 24, 512.0f, 312.0f, 100.0f, 1 },
	{ 96, 0, 96, 16, 24.0f, 316.0f, 100.0f, 0 },
	{ 96, 0, 96, 16, 520.0f, 316.0f, 100.0f, 0 },
	{ 0, 0, 214, 71, 0.0f, 359.0f, 100.0f, 0 },
	{ 0, 71, 214, 71, 214.0f, 359.0f, 100.0f, 0 },
	{ 0, 142, 214, 71, 428.0f, 359.0f, 100.0f, 0 },
#ifdef TARGET_DC
	{ 0, 0, 128, 32, 236.0f, 192.0f, 150.0f, 0 },	/* the home game's words are another picture */
	{ 0, 32, 160, 32, 332.0f, 192.0f, 150.0f, 0 },
	{ 0, 64, 240, 32, 164.0f, 216.0f, 150.0f, 0 },
	{ 0, 96, 224, 32, 342.0f, 216.0f, 150.0f, 0 },
	{ 0, 128, 208, 32, 164.0f, 240.0f, 150.0f, 0 },
	{ 0, 160, 255, 32, 320.0f, 240.0f, 150.0f, 0 },
#else
	{ 0, 0, 255, 32, 164.0f, 216.0f, 150.0f, 0 },
	{ 0, 32, 160, 32, 356.0f, 216.0f, 150.0f, 0 },
	{ 160, 32, 96, 32, 164.0f, 240.0f, 150.0f, 0 },
	{ 128, 0, 32, 32, 236.0f, 240.0f, 150.0f, 0 },
	{ 0, 64, 32, 32, 260.0f, 240.0f, 150.0f, 0 },
	{ 128, 0, 32, 32, 284.0f, 240.0f, 150.0f, 0 },
#endif
	{ 32, 64, 223, 32, 308.0f, 240.0f, 150.0f, 0 },
	{ 128, 0, 32, 32, 164.0f, 216.0f, 150.0f, 0 },
	{ 0, 96, 160, 32, 188.0f, 216.0f, 150.0f, 0 },
	{ 192, 0, 32, 32, 308.0f, 216.0f, 150.0f, 0 },
	{ 160, 96, 32, 32, 332.0f, 216.0f, 150.0f, 0 },
	{ 128, 32, 32, 32, 356.0f, 216.0f, 150.0f, 0 },
	{ 192, 96, 64, 32, 380.0f, 216.0f, 150.0f, 0 },
	{ 0, 128, 64, 32, 428.0f, 216.0f, 150.0f, 0 },
	{ 0, 160, 224, 32, 212.0f, 240.0f, 150.0f, 0 },
	{ 160, 160, 32, 32, 380.0f, 240.0f, 150.0f, 0 },
	{ 224, 96, 32, 32, 404.0f, 240.0f, 150.0f, 0 },
	{ 0, 192, 64, 32, 236.0f, 216.0f, 150.0f, 0 },
	{ 160, 224, 64, 32, 284.0f, 216.0f, 150.0f, 0 },
	{ 192, 224, 64, 32, 332.0f, 216.0f, 150.0f, 0 },
	{ 192, 224, 32, 32, 380.0f, 216.0f, 150.0f, 0 },
	{ 0, 160, 160, 32, 200.0f, 240.0f, 150.0f, 0 },
	{ 96, 192, 32, 32, 320.0f, 240.0f, 150.0f, 0 },
	{ 224, 160, 32, 32, 344.0f, 240.0f, 150.0f, 0 },
	{ 192, 160, 32, 32, 368.0f, 240.0f, 150.0f, 0 },
	{ 160, 160, 32, 32, 392.0f, 240.0f, 150.0f, 0 },
	{ 224, 96, 32, 32, 416.0f, 240.0f, 150.0f, 0 },
	{ 0, 160, 64, 32, 224.0f, 216.0f, 150.0f, 0 },
	{ 192, 0, 32, 32, 272.0f, 216.0f, 150.0f, 0 },
	{ 128, 128, 128, 32, 296.0f, 216.0f, 150.0f, 0 },
	{ 0, 128, 64, 32, 392.0f, 216.0f, 150.0f, 0 },
	{ 96, 160, 64, 32, 236.0f, 240.0f, 150.0f, 0 },
	{ 96, 192, 32, 32, 284.0f, 240.0f, 150.0f, 0 },
	{ 224, 160, 32, 32, 308.0f, 240.0f, 150.0f, 0 },
	{ 192, 160, 32, 32, 332.0f, 240.0f, 150.0f, 0 },
	{ 160, 160, 32, 32, 356.0f, 240.0f, 150.0f, 0 },
	{ 224, 96, 32, 32, 380.0f, 240.0f, 150.0f, 0 },
} ;

static const PRINT_FONT print_font_tbl[14] = {
	{ 0xffffffff, 16.0f, 16.0f },
	{ 0xffff0000, 16.0f, 16.0f },
	{ 0xff00ff00, 16.0f, 16.0f },
	{ 0xffff9900, 16.0f, 16.0f },
	{ 0xffffff00, 16.0f, 16.0f },
	{ 0xff0000ff, 16.0f, 16.0f },
	{ 0xffff00ff, 16.0f, 16.0f },
	{ 0xffffffff, 8.0f, 8.0f },
	{ 0xff00ff00, 8.0f, 8.0f },
	{ 0xffff0000, 8.0f, 8.0f },
	{ 0xffffffff, 32.0f, 32.0f },
	{ 0xffffff00, 16.0f, 8.0f },
	{ 0xff0000ff, 16.0f, 8.0f },
	{ 0xff99ff00, 16.0f, 8.0f },
} ;

static const MES_FONT mes_font_tbl[57] = {
	{ { 0, 0, 0, 0 }, 16.0f, 16.0f, 1.0f },
	{ { 1, 1, 1, 1 }, 16.0f, 16.0f, 1.0f },
	{ { 2, 2, 2, 2 }, 16.0f, 16.0f, 1.0f },
	{ { 3, 3, 3, 3 }, 16.0f, 16.0f, 1.0f },
	{ { 4, 4, 4, 4 }, 16.0f, 16.0f, 1.0f },
	{ { 5, 5, 5, 5 }, 16.0f, 16.0f, 1.0f },
	{ { 6, 6, 6, 6 }, 16.0f, 16.0f, 1.0f },
	{ { 0, 0, 0, 0 }, 12.0f, 8.0f, 1.0f },
	{ { 2, 2, 2, 2 }, 12.0f, 8.0f, 1.0f },
	{ { 1, 1, 1, 1 }, 12.0f, 8.0f, 1.0f },
	{ { 0, 0, 0, 0 }, 32.0f, 32.0f, 1.0f },
	{ { 7, 4, 7, 4 }, 16.0f, 4.0f, 1.0f },
	{ { 8, 9, 8, 9 }, 16.0f, 4.0f, 1.0f },
	{ { 2, 4, 2, 4 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 10, 10 }, 32.0f, 4.0f, 1.0f },
	{ { 11, 11, 12, 12 }, 32.0f, 4.0f, 1.0f },
	{ { 0, 0, 13, 13 }, 16.0f, 4.0f, 1.0f },
	{ { 14, 15, 14, 15 }, 32.0f, 4.0f, 1.0f },
	{ { 0, 0, 16, 16 }, 16.0f, 4.0f, 1.0f },
	{ { 2, 9, 2, 9 }, 16.0f, 4.0f, 1.0f },
	{ { 2, 4, 2, 4 }, 32.0f, 4.0f, 1.0f },
	{ { 8, 9, 8, 9 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 0, 0 }, 16.0f, 4.0f, 6.0f },
	{ { 17, 17, 17, 17 }, 16.0f, 4.0f, 1.0f },
	{ { 18, 18, 18, 18 }, 16.0f, 4.0f, 1.0f },
	{ { 19, 19, 20, 20 }, 16.0f, 4.0f, 1.0f },
	{ { 21, 21, 22, 22 }, 16.0f, 4.0f, 1.0f },
	{ { 23, 23, 24, 24 }, 16.0f, 4.0f, 1.0f },
	{ { 25, 25, 26, 26 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 0, 0 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 0, 0 }, 12.0f, 4.0f, 1.0f },
	{ { 0, 0, 0, 0 }, 12.0f, 4.0f, 1.0f },
	{ { 28, 28, 28, 28 }, 16.0f, 4.0f, 1.0f },
	{ { 29, 29, 29, 29 }, 16.0f, 4.0f, 1.0f },
	{ { 30, 30, 30, 30 }, 16.0f, 4.0f, 1.0f },
	{ { 31, 31, 31, 31 }, 16.0f, 4.0f, 1.0f },
	{ { 32, 32, 32, 32 }, 16.0f, 4.0f, 1.0f },
	{ { 33, 33, 33, 33 }, 16.0f, 4.0f, 1.0f },
	{ { 34, 34, 34, 34 }, 16.0f, 4.0f, 1.0f },
	{ { 35, 35, 35, 35 }, 16.0f, 4.0f, 1.0f },
	{ { 36, 36, 36, 36 }, 16.0f, 4.0f, 1.0f },
	{ { 37, 37, 37, 37 }, 16.0f, 4.0f, 1.0f },
	{ { 38, 38, 38, 38 }, 16.0f, 4.0f, 1.0f },
	{ { 39, 39, 39, 39 }, 16.0f, 4.0f, 1.0f },
	{ { 40, 40, 40, 40 }, 16.0f, 4.0f, 1.0f },
	{ { 41, 41, 41, 41 }, 16.0f, 4.0f, 1.0f },
	{ { 42, 42, 42, 42 }, 16.0f, 4.0f, 1.0f },
	{ { 43, 43, 43, 43 }, 16.0f, 4.0f, 1.0f },
	{ { 44, 44, 44, 44 }, 16.0f, 4.0f, 1.0f },
	{ { 45, 45, 45, 45 }, 16.0f, 4.0f, 1.0f },
	{ { 46, 46, 46, 46 }, 16.0f, 4.0f, 1.0f },
	{ { 47, 47, 47, 47 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 48, 48 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 49, 49 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 50, 50 }, 16.0f, 4.0f, 1.0f },
	{ { 0, 0, 51, 51 }, 16.0f, 4.0f, 1.0f },
	{ { 52, 52, 52, 52 }, 16.0f, 4.0f, 1.0f },
} ;

/* the versus record (the home game draws it; the arcade keeps its tables): the colours of the
   handicap's seven marks, and the lines that rule its box: four across, then five pairs down */
static const Uint32 vs_rec_col_tbl[8] = { 0xffffffff, 0xffe8f906, 0xffecd629, 0xfff38b74, 0xfff84bb4, 0xfffb2cd3, 0xffffffff, 0xff777777 } ;
static const Float vs_rec_pos_tbl[14] = { 118.0f, 208.0f, 208.0f, 298.0f, 90.0f, 96.0f, 124.0f, 148.0f, 156.0f, 180.0f, 188.0f, 212.0f, 228.0f, 252.0f } ;

/* the sprite the words and displays are put with */
static NLsprarg mes_spr = {
	0, 0.0f, 0.0f, 0.9f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0, 1.0f, 2, 0x405, -1, 0, { 0, 0, 0, 0 }
} ;
const Uint8 *spr_mes_tbl[17] = {
	spr_mes_00,
	spr_mes_01,
	spr_mes_02,
	spr_mes_03,
	spr_mes_04,
	spr_mes_05,
	spr_mes_06,
	spr_mes_07,
	spr_mes_08,
	spr_mes_09,
	spr_mes_10,
	spr_mes_05,
	spr_mes_12,
	spr_mes_13,
	spr_mes_14,
	spr_mes_15,
	spr_mes_16,
} ;
Uint32 mes_col_tbl[56] = {
	0xffffffff, 0xffff0000, 0xff00ff00, 0xffff9900, 0xffffff00, 0xff0000ff, 0xffff00ff, 0xffffa000,
	0xffb000ff, 0xff00ffff, 0xff6e6e6e, 0xffff93fc, 0xfffe09b2, 0xff909090, 0xff5a52ff, 0xff00ffd8,
	0xffa0a0a0, 0xfff77cf7, 0xff00def7, 0xfff3e817, 0xffe8f906, 0xffefb14e, 0xffecd629, 0xfff56b94,
	0xffd38b74, 0xfffb2cd3, 0xfff93cc3, 0xff00ff15, 0xffff0f50, 0xff08f0ff, 0xffffe500, 0xffff6800,
	0xffff1000, 0xffff28fa, 0xffd091ff, 0xff98a0ff, 0xffc5ffae, 0xff74ffc1, 0xffa0ffe1, 0xff92e0ff,
	0xff7ca9ff, 0xffba82ff, 0xffee40ff, 0xffff40d2, 0xffff40a6, 0xffff4062, 0xffffa200, 0xffffe000,
	0xfffffc25, 0xffff0f0f, 0xffff21f2, 0xff37cdff, 0xffffae01, 0xff000000, 0xff000000, 0xff000000,
} ;
/* the versus record's box: its frame and its ground */
static NLVERTEX00 vs_rec_frame_vtx[4] = {
	{ 108.0f, 60.0f, 0.99f, 0xffffffff },
	{ 532.0f, 60.0f, 0.99f, 0xffffffff },
	{ 108.0f, 420.0f, 0.99f, 0xffffffff },
	{ 532.0f, 420.0f, 0.99f, 0xffffffff },
} ;
static NLVERTEX00 vs_rec_back_vtx[4] = {
	{ 112.0f, 64.0f, 0.98f, 0xff0000ee },
	{ 528.0f, 64.0f, 0.98f, 0xff0000ee },
	{ 112.0f, 416.0f, 0.98f, 0xff00b4ff },
	{ 528.0f, 416.0f, 0.98f, 0xff00b4ff },
} ;
/* its textured strips: a handicap mark, an arrow, and the two halves of a line */
static NLVERTEX03 vs_rec_vtx[16] = {
	{ 136.0f, 256.0f, 0.96f, 0.0f, 1.0f, 0xffffffff, 0 },
	{ 152.0f, 256.0f, 0.96f, 0.25f, 1.0f, 0xffffffff, 0 },
	{ 136.0f, 272.0f, 0.96f, 0.0f, 0.75f, 0xffffffff, 0 },
	{ 152.0f, 272.0f, 0.96f, 0.25f, 0.75f, 0xffffffff, 0 },
	{ 312.0f, 272.0f, 0.96f, 0.75f, 0.75f, 0xffffffff, 0 },
	{ 328.0f, 272.0f, 0.96f, 1.0f, 0.75f, 0xffffffff, 0 },
	{ 312.0f, 288.0f, 0.96f, 0.75f, 0.5f, 0xffffffff, 0 },
	{ 328.0f, 288.0f, 0.96f, 1.0f, 0.5f, 0xffffffff, 0 },
	{ 118.0f, 90.0f, 0.97f, 1.0f, 0.5f, 0xffffffff, 0 },
	{ 208.0f, 90.0f, 0.97f, 0.0f, 0.5f, 0xffffffff, 0 },
	{ 118.0f, 96.0f, 0.97f, 1.0f, 0.125f, 0xffffffff, 0 },
	{ 208.0f, 96.0f, 0.97f, 0.0f, 0.125f, 0xffffffff, 0 },
	{ 208.0f, 90.0f, 0.97f, 0.0f, 0.5f, 0xffffffff, 0 },
	{ 298.0f, 90.0f, 0.97f, 1.0f, 0.5f, 0xffffffff, 0 },
	{ 208.0f, 96.0f, 0.97f, 0.0f, 0.125f, 0xffffffff, 0 },
	{ 298.0f, 96.0f, 0.97f, 1.0f, 0.125f, 0xffffffff, 0 },
} ;

/****************************************************************/
/*			A line in the system font							*/
/****************************************************************/
void mes_puts( const MES_WORK *mw )
{
	const PRINT_FONT *pf = &print_font_tbl[mw->type] ;

	nlPrintColor( pf->col ) ;
	nlSetFontSize( pf->size, pf->size ) ;
	nlLocate( mw->x, mw->y ) ;
	nlPuts( mw->str ) ;
}

/****************************************************************/
/*			A line in the polygon font; "^" takes the next		*/
/*			letter from the second half of the texture			*/
/****************************************************************/
void mes_font_disp( const MES_WORK *mw )
{
	NLVERTEX03 vt[4] ;
	const MES_FONT *mf ;
	const Uint32 *tbl ;
	Uint8 *s ;
	Uint8 c ;
	Float x, y, z, u, v ;

	nlDispStripCoordMode( 0 ) ;
	for( s = ( Uint8 * )mw->str ; *s ; s ++ ) ;
	s = ( Uint8 * )mw->str ;
	mf = &mes_font_tbl[mw->type] ;
	tbl = mes_col_tbl ;
	x = mf->step * mw->x ;
	y = mf->step * mw->y ;
	z = 0.9f ;
#ifdef TARGET_DC
	x += option_w.adjust_x ;
	y += option_w.adjust_y ;
#endif
	vt[0].x = x ;
	vt[2].x = x ;
	x += mf->size * mf->wide ;
	vt[3].x = vt[1].x = x ;
	vt[1].y = vt[0].y = y ;
	y += mf->size ;
	vt[3].y = vt[2].y = y ;
	vt[3].z = vt[2].z = vt[1].z = vt[0].z = z ;
	vt[0].col = tbl[mf->col[0]] ;
	vt[1].col = tbl[mf->col[1]] ;
	vt[2].col = tbl[mf->col[2]] ;
	vt[3].col = tbl[mf->col[3]] ;
	while( c = *s ){
		if( c == '^' ){
			s ++ ;
			c = *s + 60 ;
			if( *s == 'Z' ) c = 92 ;
		}
		c -= 32 ;
		u = ( c & 15 ) / 16.0f ;
		vt[0].u = u ;
		vt[2].u = u ;
		u += mf->wide / 16.0f ;
		vt[1].u = u ;
		vt[3].u = u ;
		v = 1.0f - ( c >> 4 ) / 8.0f ;
		vt[0].v = v ;
		vt[1].v = v ;
		v = v - 0.125f ;
		vt[2].v = v ;
		vt[3].v = v ;
		nlDispStripTexTrnsl( 4, vt, 0 ) ;
		vt[0].x += 0.75f * mf->size ;
		vt[1].x += 0.75f * mf->size ;
		vt[2].x += 0.75f * mf->size ;
		vt[3].x += 0.75f * mf->size ;
		s ++ ;
	}
}

/****************************************************************/
/*			Sprite word no of spr_mes_tbl, from texture			*/
/*			sprno on											*/
/****************************************************************/
void spr_mes_put( int sprno, int no )
{
	const Uint8 *mes = spr_mes_tbl[no] ;
	const Uint8 *p ;
	const SPR_CHR *sc ;
	int cnt ;
	int i ;
	Float u ;

	cnt = *mes ++ ;
	mes_spr.sprno = sprno + *mes ++ ;
	mes_spr.z = 0.95f ;
	for( i = 0, p = mes ; i < cnt ; i ++ ){
		sc = &spr_chr_tbl[*p] ;
		mes_spr.x = sc->x ;
		mes_spr.y = sc->y ;
#ifdef TARGET_DC
		mes_spr.x += option_w.adjust_x ;
		mes_spr.y += option_w.adjust_y ;
#endif
		mes_spr.u0 = sc->u / 256.0f ;
		mes_spr.v0 = sc->v / 256.0f ;
		mes_spr.u1 = ( sc->u + sc->w ) / 256.0f ;
		mes_spr.v1 = ( sc->v + sc->h ) / 256.0f ;
		mes_spr.zm_x = sc->w / 256.0f * sc->scale / 100.0f ;
		mes_spr.zm_y = sc->h / 256.0f * sc->scale / 100.0f ;
		if( sc->flip ){
			u = mes_spr.u0 ;
			mes_spr.u0 = mes_spr.u1 ;
			mes_spr.u1 = u ;
		}
		nlSprPut( &mes_spr ) ;
		mes_spr.z += 0.001f ;
		p ++ ;
	}
}

/****************************************************************/
/*			The points a side earns: "N-pts. GET!"				*/
/****************************************************************/
void pts_get_disp( int side, int pts )
{
	int x ;
	int x0 ;

#ifdef TARGET_DC
	return ;	/* the home game earns its points another way */
#endif
	if( !Sys->vm_write ) return ;
	x = 23 ;
	if( side ) x = 129 ;
	x0 = x ;
	if( pts > 9 ) x -= 3 ;
	if( pts > 99 ) x -= 3 ;
	STrPrint( x, 88, 20, "%d", pts ) ;
	STrPrint( x0 - 5, 96, 13, "N-pts.", pts ) ;
	STrPrint( x0 - 8, 102, 20, "GET!" ) ;
}

/****************************************************************/
/*			A frame, a two-digit number a and a six-digit		*/
/*			number b, in sprites of texture 1					*/
/****************************************************************/
void spr_num_disp( int a, int b )
{
	int i ;
	int n ;
	int d ;
	Float u, v ;

#ifdef TARGET_DC
	return ;
#endif
	mes_spr.sprno = 1 ;
	mes_spr.x = 194.0f ;
	mes_spr.y = 426.0f ;
	mes_spr.z = 0.96f ;
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = 0.0f ;
	mes_spr.u1 = 0.75f ;
	mes_spr.v1 = 0.25f ;
	mes_spr.zm_x = 0.75f ;
	mes_spr.zm_y = 0.25f ;
	mes_spr.ang = 0 ;
	mes_spr.trnsl = 1.0f ;
	mes_spr.listType = -1 ;
	mes_spr.attr = NLSPR_DISP_LT ;
	mes_spr.base_color = -1 ;
	mes_spr.offset_color = 0 ;
	nlSprPut( &mes_spr ) ;
	mes_spr.x = 274.0f ;
	mes_spr.u0 = 0.75f ;
	mes_spr.v0 = 0.0f ;
	mes_spr.u1 = 1.0f ;
	mes_spr.v1 = 0.25f ;
	mes_spr.zm_x = 0.25f ;
	mes_spr.zm_y = 0.25f ;
	nlSprPut( &mes_spr ) ;
	mes_spr.x = 298.0f ;
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = 0.0f ;
	mes_spr.u1 = 0.25f ;
	mes_spr.v1 = 0.25f ;
	mes_spr.zm_x = 0.25f ;
	mes_spr.zm_y = 0.25f ;
	nlSprPut( &mes_spr ) ;
	mes_spr.x = 314.0f ;
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = 0.25f ;
	mes_spr.u1 = 0.5f ;
	mes_spr.v1 = 0.5f ;
	mes_spr.zm_x = 0.5f ;
	mes_spr.zm_y = 0.25f ;
	nlSprPut( &mes_spr ) ;
	mes_spr.x = 442.0f ;
	mes_spr.u0 = 0.75f ;
	mes_spr.v0 = 0.0f ;
	mes_spr.u1 = 1.0f ;
	mes_spr.v1 = 0.25f ;
	mes_spr.zm_x = 0.25f ;
	mes_spr.zm_y = 0.25f ;
	nlSprPut( &mes_spr ) ;
	mes_spr.x = 274.0f ;
	mes_spr.z = 0.95f ;
	mes_spr.zm_x = 0.25f ;
	mes_spr.zm_y = 0.25f ;
	n = a ;
	for( i = 0 ; i < 2 ; i ++ ){
		d = n % 10 + 2 ;
		u = ( d & 3 ) * 0.25f ;
		v = ( d / 4 + 1 ) * 0.25f ;
		n /= 10 ;
		mes_spr.x -= 16.0f ;
		mes_spr.u0 = u ;
		mes_spr.v0 = v ;
		mes_spr.u1 = u + 0.25f ;
		mes_spr.v1 = v + 0.25f ;
		nlSprPut( &mes_spr ) ;
	}
	mes_spr.x = 442.0f ;
	mes_spr.zm_x = 0.25f ;
	mes_spr.zm_y = 0.25f ;
	n = b ;
	for( i = 0 ; i < 6 ; i ++ ){
		d = n % 10 + 2 ;
		u = ( d & 3 ) * 0.25f ;
		v = ( d / 4 + 1 ) * 0.25f ;
		n /= 10 ;
		mes_spr.x -= 16.0f ;
		mes_spr.u0 = u ;
		mes_spr.v0 = v ;
		mes_spr.u1 = u + 0.25f ;
		mes_spr.v1 = v + 0.25f ;
		nlSprPut( &mes_spr ) ;
	}
}

/****************************************************************/
/*			Three sprites of texture 0xd50 (from the			*/
/*			European disc on, the first alone)					*/
/****************************************************************/
void spr_mark_disp( void )
{
	mes_spr.sprno = 0xd50 ;
	mes_spr.x = 40.0f ;
	mes_spr.y = 394.0f ;
#ifdef TARGET_DC
	mes_spr.x += option_w.adjust_x ;
	mes_spr.y += option_w.adjust_y ;
#endif
	mes_spr.z = 0.96f ;
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = 0.0f ;
	mes_spr.u1 = 1.0f ;
	mes_spr.v1 = DOT_X( 0.375f ) ;
	mes_spr.zm_x = 1.0f ;
	mes_spr.zm_y = DOT_X( 0.375f ) ;
	mes_spr.ang = 0 ;
	mes_spr.trnsl = 1.0f ;
	mes_spr.listType = -1 ;
	mes_spr.attr = NLSPR_DISP_LT ;
	mes_spr.base_color = -1 ;
	mes_spr.offset_color = 0 ;
	nlSprPut( &mes_spr ) ;
#if BUILD_DATE < 20000412
	mes_spr.x = 222.0f ;
	mes_spr.y = 448.0f ;
#ifdef TARGET_DC
	mes_spr.x += option_w.adjust_x ;
	mes_spr.y += option_w.adjust_y ;
#endif
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = DOT_X( 0.375f ) ;
	mes_spr.u1 = 1.0f ;
	mes_spr.v1 = 0.75f ;
	mes_spr.zm_x = 1.0f ;
	mes_spr.zm_y = 0.125f ;
	nlSprPut( &mes_spr ) ;
	mes_spr.x = 350.0f ;
#ifdef TARGET_DC
	mes_spr.x += option_w.adjust_x ;
#endif
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = 0.75f ;
	mes_spr.u1 = 0.5f ;
	mes_spr.v1 = 0.875f ;
	mes_spr.zm_x = 0.5f ;
	mes_spr.zm_y = 0.125f ;
	nlSprPut( &mes_spr ) ;
#endif
}

#ifdef TARGET_DC
/****************************************************************/
/*			The versus record, while the sides choose: each		*/
/*			side's wins, losses and draws, and its handicap		*/
/****************************************************************/
void vs_record_disp( void )
{
	NLVERTEX00 *bv ;
	NLVERTEX03 *vt ;
	PLAYER_WORK *pl_w ;
	const Float *pos ;
	int side ;
	int i, j ;

	if( Sys->home_mode != HOME_VERSUS ) return ;
	if( Sys->routine_0 != 2 ) return ;
	nlDispStripCoordMode( 0 ) ;
	STrPrint( 41, 20, 23, "PLAYER" ) ;
	STrPrint( 97, 20, 24, "PLAYER" ) ;
	STrPrint( 60, 20, 23, "1" ) ;
	STrPrint( 116, 20, 24, "2" ) ;
	for( side = 0 ; side < 2 ; side ++ ){
		pl_w = &pl_work[side] ;
		STrPrint( side * 56 + 37, 32, 25, "WIN     %02d", pl_w->vs_record[0] ) ;
		STrPrint( side * 56 + 37, 40, 26, "LOSE    %02d", pl_w->vs_record[1] ) ;
		STrPrint( side * 56 + 37, 48, 27, "DRAW    %02d", pl_w->vs_record[2] ) ;
		STrPrint( side * 56 + 40, 58, 28, "HANDICAP" ) ;
	}

	/***** the box *****/
	bv = vs_rec_frame_vtx ;
	bv[0].x = bv[2].x = option_w.adjust_x + 108.0f ;
	bv[1].x = bv[3].x = option_w.adjust_x + 532.0f ;
	bv[0].y = bv[1].y = option_w.adjust_y + 60.0f ;
	bv[2].y = bv[3].y = option_w.adjust_y + 420.0f ;
	bv[0].x ;	/* left out of the build */
	bv[1].x ;	/* left out of the build */
	bv[3].x ;	/* left out of the build */
	nlDispStripTrnsl( 4, bv ) ;
	bv = vs_rec_back_vtx ;
	bv[0].x = bv[2].x = option_w.adjust_x + 112.0f ;
	bv[1].x = bv[3].x = option_w.adjust_x + 528.0f ;
	bv[0].y = bv[1].y = option_w.adjust_y + 64.0f ;
	bv[2].y = bv[3].y = option_w.adjust_y + 416.0f ;
	nlDispStripTrnsl( 4, bv ) ;

	/***** its lines, for each side *****/
	vt = &vs_rec_vtx[8] ;
	pos = vs_rec_pos_tbl ;
	if( 0 ) ;	/* left out of the build */
	vt[0].x = vt[2].x = *pos ++ + option_w.adjust_x ;
	vt[1].x = vt[3].x = *pos ++ + option_w.adjust_x ;
	vt[4].x = vt[6].x = *pos ++ + option_w.adjust_x ;
	vt[5].x = vt[7].x = *pos ++ + option_w.adjust_x ;
	for( i = 0 ; i < 2 ; i ++ ){
		for( j = 0 ; j < 10 ; j += 2 ){
			vt[0].y = vt[1].y = vt[4].y = vt[5].y = pos[j] + option_w.adjust_y ;
			vt[2].y = vt[3].y = vt[6].y = vt[7].y = pos[j + 1] + option_w.adjust_y ;
			nlDispStripTexTrnsl( 4, vt, 11 ) ;
			nlDispStripTexTrnsl( 4, &vt[4], 11 ) ;
		}
		for( j = 0 ; j < 8 ; j ++ ) vt[j].x += 224.0f ;
	}

	/***** the handicap's marks *****/
	vt = vs_rec_vtx ;
	vt[0].x = vt[2].x = option_w.adjust_x + 136.0f ;
	vt[1].x = vt[3].x = option_w.adjust_x + 152.0f ;
	vt[0].y = vt[1].y = option_w.adjust_y + 256.0f ;
	vt[2].y = vt[3].y = option_w.adjust_y + 272.0f ;
	if( 0 ) option_w.adjust_x = 224.0f ;	/* left out of the build */
	for( side = 0 ; side < 2 ; side ++ ){
		pl_w = &pl_work[side] ;
		for( i = 0 ; i < 7 ; i ++ ){
			vt[0].u = vt[2].u = ( i & 3 ) * 0.25f ;
			vt[1].u = vt[3].u = ( ( i & 3 ) + 1 ) * 0.25f ;
			vt[0].v = vt[1].v = 1.0f - ( i / 4 ) * 0.25f ;
			vt[2].v = vt[3].v = 1.0f - ( i / 4 + 1 ) * 0.25f ;
			for( j = 0 ; j < 4 ; j ++ ){
				vt[j].x += 16.0f ;
				vt[j].col = vs_rec_col_tbl[i] ;
				if( i > pl_w->handicap + 1 && i != 6 ) vt[j].col = 0xff777777 ;
			}
			nlDispStripTexTrnsl( 4, vt, 11 ) ;
		}
		for( j = 0 ; j < 4 ; j ++ ) vt[j].x += 112.0f ;
	}

	/***** the arrows *****/
	vt = &vs_rec_vtx[4] ;
	vt[0].x = vt[2].x = option_w.adjust_x + 312.0f ;
	vt[1].x = vt[3].x = option_w.adjust_x + 328.0f ;
	vt[0].y = vt[1].y = option_w.adjust_y + 266.0f ;
	vt[2].y = vt[3].y = option_w.adjust_y + 282.0f ;
	vt[0].v = vt[1].v = 0.75f ;
	vt[2].v = vt[3].v = 0.5f ;
	nlDispStripTexTrnsl( 4, vt, 11 ) ;
	vt[0].y = vt[1].y = option_w.adjust_y + 402.0f ;
	vt[2].y = vt[3].y = option_w.adjust_y + 418.0f ;
	vt[0].v = vt[1].v = 0.5f ;
	vt[2].v = vt[3].v = 0.75f ;
	nlDispStripTexTrnsl( 4, vt, 11 ) ;

	mes_spr.sprno = 0x914 ;
	mes_spr.x = option_w.adjust_x + 240.0f ;
	mes_spr.y = option_w.adjust_y + 282.0f ;
	mes_spr.z = 0.96f ;
	mes_spr.u0 = 0.0f ;
	mes_spr.v0 = 0.0f ;
	mes_spr.u1 = 1.0f ;
	mes_spr.v1 = 1.0f ;
	mes_spr.zm_x = 1.25f ;
	mes_spr.zm_y = 0.9375f ;
	mes_spr.ang = 0 ;
	mes_spr.trnsl = 1.0f ;
	mes_spr.listType = 0 ;
	mes_spr.attr = NLSPR_DISP_LT ;
	mes_spr.base_color = -1 ;
	mes_spr.offset_color = 0 ;
	nlSetCommonTextureOffset( 0 ) ;
	nlSprPut( &mes_spr ) ;
	mes_spr.listType = -1 ;
}
#endif

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "message.c" End of File	*/
