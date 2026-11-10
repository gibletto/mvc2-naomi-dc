/* Omega Red's effect 01 (character 30's shot 01): the coil of his Carbonadium Coil. A head (type 0)
   flies out along a direction of the player's choosing and drags a chain of 14 parts (type 1) behind
   it; when it catches the opponent it pulls him in, and when it misses it winds back. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl30_def.h"

/* the head's own work, in obj_mtx */
typedef struct SHL30_01_LOCAL_W {
	Sint16	rect[4];	/* 00: the size of a part, from shl30_01_circle: two offsets and two joins */
	Sint16	unk_08;		/* 08 */
	Sint16	unk_0a;		/* 0a */
	Sint8	aim_dir;		/* 0c */
	Sint8	turn_spd;		/* 0d */
	Sint8	rest_cnt;		/* 0e */
	Sint8	part_cnt;		/* 0f */
	Float	start_x;		/* 10: where the coil starts, x */
	Float	start_y;		/* 14: and y */
	Float	ofs_x;		/* 18: from the player to where it starts */
	Float	ofs_y;		/* 1c */
	PLAYER_WORK	*last_w;	/* 20: the last part to start */
	void	*dat;	/* 24 */
	PLAYER_WORK	*hold_w;	/* 28: the opponent's effect */
	Float	old_x;		/* 2c */
	Sint16	drain_type;		/* 30 */
	Sint8	unk_32;		/* 32 */
	Sint8	wind_turn;		/* 33 */
	Sint8	move_flag;		/* 34 */
	Sint8	unk_35;		/* 35 */
	Uint8	sp_type;		/* 36: the special move it belongs to */
} SHL30_01_LOCAL_W;
typedef struct SHL30_01_VEC2 {
	Float	x;
	Float	y;
} SHL30_01_VEC2;
#define SHL30_01_LW( w )	( ( SHL30_01_LOCAL_W * )&( w )->obj_mtx )

/* the animation and the sizes of a part for the direction no, as ef_char_set2 leaves them */
#define SHL30_01_CHAR_SET( ew, lw, no ) \
	ef_char_set2( ew, 27, 1, no ) ; \
	( ew )->cg_adrs = ( Sint8 * )&shl30_01_char ; \
	( ew )->frame_box = ( ew )->frame_box_tbl + ( ( CHAR_DATA * )( ew )->cg_adrs )->frame_box_no ; \
	( lw )->rect[0] = DOT_X( shl30_01_circle[no][0] ) ; \
	( lw )->rect[1] = shl30_01_circle[no][1] * DOT_Y( 1.0f ) ; \
	( lw )->rect[2] = DOT_X( shl30_01_circle[no][2] ) ; \
	( lw )->rect[3] = shl30_01_circle[no][3] * DOT_Y( 1.0f )

extern PLAYER_WORK *shl30_02_hold_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl30_02_drain_set( PLAYER_WORK *ew, PLAYER_WORK *head_w, Sint16 *timer );
extern void pls30_00_set( PLAYER_WORK *pl_w, PLAYER_WORK *w, int no );
extern void dm00_06_set( PLAYER_WORK *pl_w, PLAYER_WORK *w, Uint8 type );

static Sint16 shl30_01_ofs_a[4][4] = {
	{ 0, -53, 125, 0 },
	{ -40, -79, 110, 0 },
	{ -64, -109, 66, 0 },
	{ -88, -103, 36, 0 },
} ;
static Sint16 shl30_01_ofs_b[5][4] = {
	{ 0, -37, 148, 0 },
	{ -40, -63, 135, 0 },
	{ -64, -93, 90, 0 },
	{ -88, -88, 61, 0 },
	{ -128, -61, 45, 0 },
} ;
static Uint8 shl30_01_atck_no[] = {
	0x33, 0x34, 0x35, 0x36, 0x37, 0x00,
} ;
static Uint8 shl30_01_hold_no[] = {
	0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x00,
} ;
Sint16 shl30_01_circle[32][4] = {
	{ 0, -6, 0, 6 },
	{ 2, -6, -1, 6 },
	{ 4, -6, -4, 5 },
	{ 6, -5, -5, 5 },
	{ 6, -4, -5, 4 },
	{ 7, -3, -6, 4 },
	{ 8, -2, -7, 3 },
	{ 8, -2, -7, 2 },
	{ 7, 0, -8, 0 },
	{ 7, 2, -8, -2 },
	{ 7, 3, -8, -2 },
	{ 6, 4, -7, -3 },
	{ 5, 4, -6, -4 },
	{ 5, 5, -6, -5 },
	{ 4, 5, -4, -6 },
	{ 1, 6, -2, -6 },
	{ 0, 6, 0, -6 },
	{ -2, 6, 1, -6 },
	{ -4, 6, 4, -5 },
	{ -6, 5, 5, -5 },
	{ -6, 4, 5, -4 },
	{ -7, 3, 6, -4 },
	{ -8, 2, 7, -3 },
	{ -8, 2, 7, -2 },
	{ -7, 0, 8, 0 },
	{ -7, -2, 8, 2 },
	{ -7, -3, 8, 2 },
	{ -6, -4, 7, 3 },
	{ -5, -4, 6, 4 },
	{ -5, -5, 6, 5 },
	{ -4, -5, 4, 6 },
	{ -1, -6, 2, 6 },
} ;

/* a one-frame animation for the parts, which only gives their box */
static CHAR_DATA shl30_01_char = { 0, { 0 }, 0x2e } ;

/* a record of the coil's path, by the animation's code */
typedef struct SHL30_01_DATA {
	Sint8	no;		/* 00: negative for the end */
	Uint8	dir;	/* 01 */
	Uint8	unk_02;	/* 02 */
	Sint8	unk_03;	/* 03 */
	Sint16	x;		/* 04 */
	Sint16	y;		/* 06 */
	Sint8	aim_dir;	/* 08 */
	Sint8	turn_spd;	/* 09 */
	Sint8	aim_step;	/* 0a */
	Sint8	aim_acc;	/* 0b */
	Sint8	aim_limit;	/* 0c */
	Uint8	unk_0d;	/* 0d */
} SHL30_01_DATA;

static Uint8 shl30_01_tbl_a[] = {
	1, 1, 1, 1, 1, 1, 0, 2, 3, 3, 2, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
} ;
static Uint8 shl30_01_tbl_b[] = {
	0, 1, 2, 3,
} ;
static Uint8 shl30_01_tbl_c[] = {
	1, 2, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 3, 2, 1,
	3, 1, 1, 1, 0, 3, 1, 1, 1, 1, 1, 0, 0, 0, 1, 0,
	1, 1, 0, 1, 0, 1, 2, 1, 1, 3, 1, 1, 1, 2, 1, 2,
	2, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0,
} ;
static Uint8 shl30_01_tbl_d[] = {
	4, 7, 10, 13,
} ;
static struct { Uint8 no, dir ; } shl30_01_tbl_e[] = {
	{ 2, 1 }, { 1, 1 }, { 0, 1 }, { 1, 0 }, { 2, 0 }, { 1, 2 }, { 0, 2 }, { 1, 3 },
	{ 2, 0 }, { 1, 0 }, { 0, 0 }, { 1, 1 }, { 2, 1 }, { 1, 3 }, { 0, 3 }, { 1, 2 },
} ;
static Sint16 shl30_01_catch_ofs[4][16][2] = {
	{
		{ -1, -12 },
		{ 11, -11 },
		{ 15, -3 },
		{ 12, 8 },
		{ 2, 9 },
		{ -10, 10 },
		{ -14, 3 },
		{ -12, -8 },
		{ 1, -12 },
		{ -11, -10 },
		{ -14, -3 },
		{ -12, 8 },
		{ -1, 10 },
		{ 11, 11 },
		{ 14, 3 },
		{ 11, -8 },
	},
	{
		{ -1, -16 },
		{ 12, -17 },
		{ 20, -3 },
		{ 19, 8 },
		{ 0, 15 },
		{ -13, 17 },
		{ -20, 4 },
		{ -19, -9 },
		{ 2, -16 },
		{ -12, -17 },
		{ -20, -3 },
		{ -18, 8 },
		{ 0, 15 },
		{ 13, 17 },
		{ 20, 4 },
		{ 19, -9 },
	},
	{
		{ -6, -22 },
		{ 13, -18 },
		{ 29, -3 },
		{ 22, 11 },
		{ 1, 22 },
		{ -14, 18 },
		{ -28, 3 },
		{ -21, -10 },
		{ 6, -22 },
		{ -13, -18 },
		{ -29, -3 },
		{ -21, 10 },
		{ -1, 22 },
		{ 13, 18 },
		{ 29, 3 },
		{ 21, -10 },
	},
	{
		{ -12, -34 },
		{ 30, -26 },
		{ 44, -4 },
		{ 33, 14 },
		{ 11, 39 },
		{ -28, 26 },
		{ -47, 4 },
		{ -33, -13 },
		{ 12, -33 },
		{ -27, -25 },
		{ -45, -3 },
		{ -33, 14 },
		{ -11, 39 },
		{ 29, 26 },
		{ 45, 4 },
		{ 34, -13 },
	},
} ;
static SHL30_01_DATA shl30_01_dat[] = {
	{ 0, 200, 4, -1, -37, 65, 200, 16, 248, 0, 136, 0 },
	{ 0, 208, 4, -1, -25, 81, 208, 4, 244, 0, 88, 0 },
	{ 0, 216, 4, -1, -28, 79, 216, 4, 244, 0, 96, 0 },
	{ 0, 224, 4, -1, -49, 75, 224, 4, 244, 0, 104, 0 },
	{ 0, 232, 4, -1, -35, 71, 232, 4, 244, 0, 112, 0 },
	{ 0, 240, 4, -1, 14, 87, 240, 4, 244, 0, 136, 0 },
	{ -1, 0, 4, 0, 53, 125, 0, 16, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -37, 65, 200, 16, 248, 0, 136, 0 },
	{ 0, 208, 4, -1, -25, 81, 208, 4, 244, 0, 120, 0 },
	{ 0, 216, 4, -1, -28, 79, 216, 4, 244, 0, 128, 0 },
	{ 0, 224, 4, -1, -49, 75, 224, 4, 244, 0, 136, 0 },
	{ 0, 240, 4, -1, -48, 86, 240, 4, 244, 0, 144, 0 },
	{ 0, 0, 4, -1, 14, 87, 0, 4, 244, 0, 168, 0 },
	{ -1, 32, 4, 0, 79, 110, 32, 16, 0, 0, 32, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -37, 65, 232, 16, 244, 1, 136, 0 },
	{ 0, 208, 4, -1, -25, 81, 240, 4, 240, 1, 136, 0 },
	{ 0, 216, 4, -1, -28, 79, 248, 4, 240, 1, 136, 0 },
	{ 0, 224, 4, -1, -49, 75, 0, 4, 240, 0, 136, 0 },
	{ 0, 240, 4, -1, -48, 86, 16, 8, 250, 0, 168, 0 },
	{ 0, 0, 4, -1, 2, 87, 32, 8, 252, 0, 200, 0 },
	{ -1, 64, 4, 0, 109, 66, 64, 16, 0, 0, 64, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -37, 65, 224, 16, 236, 1, 136, 0 },
	{ 0, 208, 4, -1, -25, 81, 232, 4, 240, 1, 136, 0 },
	{ 0, 216, 4, -1, -28, 79, 240, 4, 240, 1, 136, 0 },
	{ 0, 224, 4, -1, -49, 75, 248, 4, 240, 1, 168, 0 },
	{ 0, 240, 4, -1, -48, 86, 0, 8, 248, 0, 200, 0 },
	{ 0, 0, 4, -1, 2, 87, 16, 8, 252, 0, 232, 0 },
	{ -1, 96, 4, 0, 103, 36, 96, 16, 0, 0, 96, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -21, 89, 200, 16, 248, 0, 136, 0 },
	{ 0, 208, 4, -1, -9, 105, 208, 4, 244, 0, 88, 0 },
	{ 0, 216, 4, -1, -28, 103, 216, 4, 244, 0, 96, 0 },
	{ 0, 224, 4, -1, -49, 99, 224, 4, 244, 0, 104, 0 },
	{ 0, 232, 4, -1, -51, 95, 232, 4, 244, 0, 112, 0 },
	{ 0, 240, 4, -1, -2, 111, 240, 4, 244, 0, 136, 0 },
	{ -1, 0, 4, 0, 37, 148, 0, 16, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -21, 89, 200, 16, 248, 0, 136, 0 },
	{ 0, 208, 4, -1, -9, 105, 208, 4, 244, 0, 120, 0 },
	{ 0, 216, 4, -1, -28, 103, 216, 4, 244, 0, 128, 0 },
	{ 0, 224, 4, -1, -49, 99, 224, 4, 244, 0, 136, 0 },
	{ 0, 240, 4, -1, -64, 110, 240, 4, 244, 0, 144, 0 },
	{ 0, 0, 4, -1, -2, 111, 0, 4, 244, 0, 168, 0 },
	{ -1, 32, 4, 0, 63, 135, 32, 16, 0, 0, 32, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -21, 89, 232, 16, 244, 1, 136, 0 },
	{ 0, 208, 4, -1, -9, 105, 240, 4, 240, 1, 136, 0 },
	{ 0, 216, 4, -1, -28, 103, 248, 4, 240, 1, 136, 0 },
	{ 0, 224, 4, -1, -49, 99, 0, 4, 240, 255, 136, 0 },
	{ 0, 240, 4, -1, -64, 110, 16, 8, 250, 0, 168, 0 },
	{ 0, 0, 4, -1, -14, 111, 32, 8, 252, 0, 200, 0 },
	{ -1, 64, 4, 0, 93, 90, 64, 16, 0, 0, 64, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -21, 89, 224, 16, 236, 1, 136, 0 },
	{ 0, 208, 4, -1, -9, 105, 232, 4, 240, 1, 136, 0 },
	{ 0, 216, 4, -1, -28, 103, 240, 4, 240, 1, 136, 0 },
	{ 0, 224, 4, -1, -49, 99, 248, 4, 240, 1, 168, 0 },
	{ 0, 240, 4, -1, -64, 110, 0, 8, 248, 0, 200, 0 },
	{ 0, 240, 4, -1, -14, 111, 16, 8, 252, 0, 232, 0 },
	{ -1, 96, 4, 0, 87, 61, 96, 16, 0, 0, 96, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 4, -1, -21, 89, 232, 16, 240, 1, 136, 0 },
	{ 0, 208, 4, -1, -9, 105, 240, 4, 240, 0, 136, 0 },
	{ 0, 216, 4, -1, -28, 103, 0, 8, 240, 0, 136, 0 },
	{ 0, 224, 4, -1, -49, 99, 32, 8, 240, 0, 160, 0 },
	{ 0, 240, 4, -1, -64, 110, 64, 8, 240, 0, 192, 0 },
	{ 0, 8, 4, -1, 9, 104, 112, 16, 240, 0, 8, 0 },
	{ -1, 128, 4, -1, 61, 37, 128, 16, 0, 0, 128, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 252, -1, -37, 65, 0, 12, 240, 1, 136, 0 },
	{ 0, 212, 252, -1, 33, 69, 64, 12, 240, 1, 224, 0 },
	{ 0, 224, 252, -1, 43, 74, 32, 12, 16, 255, 80, 0 },
	{ 0, 240, 252, -1, -14, 87, 16, 12, 16, 255, 120, 0 },
	{ -1, 0, 4, 0, -53, 125, 0, 16, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 252, -1, -21, 89, 0, 12, 240, 1, 136, 0 },
	{ 0, 212, 252, -1, 49, 93, 64, 12, 240, 1, 224, 0 },
	{ 0, 224, 252, -1, 43, 98, 32, 12, 16, 255, 80, 0 },
	{ 0, 240, 252, -1, 2, 111, 16, 12, 16, 255, 120, 0 },
	{ -1, 0, 4, 0, -37, 148, 0, 16, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 252, -1, -37, 65, 232, 16, 240, 1, 144, 0 },
	{ 0, 212, 252, -1, 33, 69, 0, 16, 240, 1, 128, 0 },
	{ 0, 224, 252, -1, 43, 74, 32, 16, 240, 1, 192, 0 },
	{ 0, 240, 252, -1, 48, 86, 0, 16, 12, 255, 60, 0 },
	{ 0, 0, 252, -1, -4, 94, 224, 16, 12, 255, 28, 0 },
	{ -1, 160, 4, 0, -103, 36, 160, 16, 0, 0, 160, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 200, 252, -1, -21, 89, 232, 16, 240, 1, 144, 0 },
	{ 0, 212, 252, -1, 49, 93, 0, 16, 240, 1, 128, 0 },
	{ 0, 224, 252, -1, 43, 98, 32, 16, 240, 1, 192, 0 },
	{ 0, 240, 252, -1, 64, 110, 0, 16, 12, 255, 60, 0 },
	{ 0, 240, 252, -1, 12, 118, 224, 16, 12, 255, 28, 0 },
	{ -1, 160, 4, 0, -87, 61, 160, 16, 0, 0, 160, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
} ;

static void shl30_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl30_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl30_01_main ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w ;
		ew->type = 0 ;
		ew->ef_no = 0x3001 ;
		SHL30_01_LW( ew )->sp_type = pl_w->pl_sp_type ;
	}
	return ew ;
}

/*==============================================================*/
/*			N parts behind w, each with the sizes in lw			*/
/*==============================================================*/
static void shl30_01_part_set( PLAYER_WORK *w, SHL30_01_LOCAL_W *lw, int n )
{
	PLAYER_WORK *ew ;
	SHL30_01_LOCAL_W *pw ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
		ew = work_get( w, W_SHL, LIST_BEHIND ) ;
		ew->work_func = shl30_01_main ;
		ew->pos = w->pos ;
		ew->type = 1 ;
		ew->master_w = w ;
		ew->ef_w = w->ef_w ;
		ew->dir = w->dir ;
		ew->ef_no = 0x3001 ;
		pw = SHL30_01_LW( ew ) ;
		pw->rect[0] = lw->rect[0] ;
		pw->rect[1] = lw->rect[1] ;
		pw->rect[2] = lw->rect[2] ;
		pw->rect[3] = lw->rect[3] ;
	}
}

/*==============================================================*/
/* the step of a coil flying in direction dir (256 to a circle) */
static void shl30_01_vec( Uint8 dir, SHL30_01_VEC2 *v )
{
	Sint16 ang ;
	register Float r ;

	dir += 4 ;
	dir >>= 3 ;
	r = 409600.0f ;
	ang = ( ( 40 - dir ) & 31 ) << 11 ;
	v->x = r * nlCos( ang ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	v->y = r * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * DOT_Y( 1.0f ) ;
}

static void shl30_01_init( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_move( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_end( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_free( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );

/*==============================================================*/
static void shl30_01_main( PLAYER_WORK *ew )
{
	static void ( *shl30_01_jmp[] )( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw ) = {
		shl30_01_init, shl30_01_move, shl30_01_end, shl30_01_free,
	} ;

	shl30_01_jmp[ew->routine_0]( ew, SHL30_01_LW( ew ) ) ;
}

/*==============================================================*/
static void shl30_01_init( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint16 *ofs ;
	Uint8 no ;
	Sint16 dir ;
	Float x ;
	PL30_LOCAL_W *pl_lw ;
	PLAYER_WORK *mw ;

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
	ew->sub_prio = 100 ;
	ew->disp_flag = 0 ;
	ew->pos = pl_w->pos ;
	ew->catch_grip_old = -1 ;
	if( ew->type ){
		ew->routine_0 ++ ;
		mw = ew->master_w ;
		SHL30_01_LW( mw )->last_w = ew ;
		return ;
	}
	pl_w->catch_flag = 1 ;
	pl_lw = ( PL30_LOCAL_W * )&pl_w->tlocal[0] ;
	pl_lw->key_flag = 0 ;
	lw->move_flag = 0 ;
	ofs = shl30_01_ofs_a[0] ;
	if( pl_w->pl_sp_type != 2 ) ofs = shl30_01_ofs_b[0] ;
	ofs += pl_w->atck_level * 4 ;
	dir = ofs[0] ;
	x = DOT_X( ofs[1] ) ;
	if( pl_w->disp_dir ){
		dir = -dir ;
		x = -x ;
	}
	ew->dir = dir ;
	lw->ofs_x = x ;
	lw->ofs_y = ofs[2] * DOT_Y( 1.0f ) ;
	ew->pos.x += x ;
	ew->pos.y += lw->ofs_y ;
	lw->start_x = ew->pos.x ;
	lw->start_y = ew->pos.y ;
	ew->spd.x = ew->spd.y = 0.0f ;
	ew->disp_dir = ew->hit_stop = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ | HIT_OWNER ;
	ew->dm_flag = 0 ;
	ew->atck_no = shl30_01_atck_no[pl_w->atck_level] ;
	HIT_RESET( ew ) ;
	lw->unk_32 = 0 ;
	no = ( ( ew->dir + 4 ) & 0xf8 ) / 8 ;
	SHL30_01_CHAR_SET( ew, lw, no ) ;
	lw->rest_cnt = 0 ;
	lw->part_cnt = 0 ;
	lw->hold_w = 0 ;
	if( work_rest_cnt >= 14 ){
		shl30_01_part_set( ew, lw, 14 ) ;
		ew->routine_0 ++ ;
		lw->move_flag = 1 ;
	}
	else {
		pl_w->routine_2 = 3 ;
		work_free( ew ) ;
	}
}

static void shl30_01_head_move( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_part_move( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void shl30_01_move( register PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	static void ( *shl30_01_move_jmp[] )( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw ) = {
		shl30_01_head_move, shl30_01_part_move,
	} ;

	shl30_01_move_jmp[ew->type]( ew, lw ) ;
}

static void shl30_01_head_fly( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_slow( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_wind( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_hold( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_throw( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_drain( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );

/*==============================================================*/
/*			The head, while the player is still in the move		*/
/*==============================================================*/
static void shl30_01_head_move( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	static void ( *shl30_01_head_jmp[] )( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw ) = {
		shl30_01_head_fly, shl30_01_head_slow, shl30_01_head_wind, shl30_01_head_hold, shl30_01_head_throw, shl30_01_head_drain,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *w ;
	int i ;

	if( pl_w->routine_1
	 || ( pl_w->act_st == N_SPECIAL && pl_w->pl_sp_type != lw->sp_type )
	 || ( pl_w->act_st == N_SUPER && pl_w->pl_sp_type != lw->sp_type )
	 || ( pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER ) ){
		shl30_01_end( ew, lw ) ;
		return ;
	}
	pl_w->catch_flag = 1 ;
	lw->rest_cnt = 14 ;
	lw->start_x = pl_w->pos.x + lw->ofs_x ;
	lw->start_y = pl_w->pos.y + lw->ofs_y ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 0 ;
	lw->move_flag = 0 ;
	w = ew ;
	for( i = 14 ; i > 0 ; i -- ){
		w = w->behind_w ;
		w->disp_flag = 0 ;
	}
	shl30_01_head_jmp[ew->routine_1]( ew, lw ) ;
}

static void shl30_01_hit( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_ground( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_wind_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );

/*==============================================================*/
/*			Flying out											*/
/*==============================================================*/
static void shl30_01_head_fly( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL30_01_VEC2 v ;
	Uint16 sw ;
	Uint16 mask ;

	if( pl_w->hit_flag || ew->hit_flag ){
		shl30_01_hit( ew, lw ) ;
		return ;
	}
	pl_w->pl_stop_now |= pl_w->pl_sw_now & KEY_ATCK ;
	if( lw->part_cnt > 7 ){
		sw = pl_w->pl_stop_now ;
		pl_w->pl_stop_now = 0 ;
		mask = 0x300 ;
		if( pl_w->pl_stance == STANCE_AIR ) mask = 0x360 ;
		if( sw & mask ){
			shl30_01_head_wind_set( ew, lw ) ;
			return ;
		}
	}
	if( pl_w->ground_y > ew->pos.y ){
		shl30_01_ground( ew, lw ) ;
		return ;
	}
	shl30_01_vec( ew->dir, &v ) ;
	ew->spd.x += v.x ;
	ew->spd.y += v.y ;
	ew->pos.x = lw->start_x + ew->spd.x ;
	ew->pos.y = lw->start_y + ew->spd.y ;
	ew->disp_flag = 1 ;
	lw->part_cnt = 0 ;
	lw->move_flag = 1 ;
}

/*==============================================================*/
/*			Flying out, its step halved every eight frames		*/
/*==============================================================*/
static void shl30_01_head_slow( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL30_01_VEC2 v ;
	Uint16 sw ;
	Uint16 mask ;

	if( pl_w->hit_flag || ew->hit_flag ){
		shl30_01_hit( ew, lw ) ;
		return ;
	}
	pl_w->pl_stop_now |= pl_w->pl_sw_now & KEY_ATCK ;
	if( lw->part_cnt > 7 ){
		sw = pl_w->pl_stop_now ;
		pl_w->pl_stop_now = 0 ;
		mask = 0x320 ;
		if( pl_w->pl_stance == STANCE_AIR ) mask = 0x360 ;
		if( sw & mask ){
			shl30_01_head_wind_set( ew, lw ) ;
			return ;
		}
	}
	if( pl_w->ground_y > ew->pos.y ){
		shl30_01_ground( ew, lw ) ;
		return ;
	}
	shl30_01_vec( ew->dir, &v ) ;
	v.x /= ew->acc.x ;
	v.y /= ew->acc.x ;
	ew->spd.x += v.x ;
	ew->spd.y += v.y ;
	ew->pos.x = lw->start_x + ew->spd.x ;
	ew->pos.y = lw->start_y + ew->spd.y ;
	ew->disp_flag = 1 ;
	lw->move_flag = 1 ;
	if( -- lw->unk_08 <= 0 ){
		lw->unk_08 = 8 ;
		if( ( ew->acc.x *= 2.0f ) > 32.0f ){
			pl_w->routine_2 = 3 ;
			ew->routine_1 = 2 ;
			lw->unk_08 = 0xe00 ;
			lw->unk_0a = ew->dir ;
			lw->wind_turn = 0 ;
		}
	}
	lw->part_cnt = 0 ;
}

static void shl30_01_head_drain_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_head_throw_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_catch_pos( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, PLAYER_WORK *w, PLAYER_WORK *pl_w );
static void shl30_01_follow( PLAYER_WORK *ew, PLAYER_WORK *head_w );

/*==============================================================*/
/*			Winds back: the coil turns as it comes in			*/
/*==============================================================*/
static void shl30_01_head_wind_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	scr_excep_clr() ;
	pl_w->pl_stop_now = 0 ;
	ew->routine_1 = 2 ;
	HI_BYTE( lw->unk_08 ) = lw->part_cnt ;
	LO_BYTE( lw->unk_08 ) = 0 ;
	lw->unk_0a = ew->dir ;
	if( lw->hold_w ){
		lw->hold_w->routine_0 = 2 ;
		lw->hold_w = 0 ;
	}
	shl30_01_head_wind( ew, lw ) ;
}

/*==============================================================*/
/*			Winds back: walks the coil from the head (ew,		*/
/*			lw) to its tail. The angles are ints written		*/
/*			through their low bytes.							*/
/*==============================================================*/
static void shl30_01_head_wind( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *head_w = ew ;
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *tail_w ;
	SHL30_01_LOCAL_W *head_lw ;
	int ang ;
	int turn ;
	int d ;
	int flag ;
	int cnt ;

	head_lw = lw ;
	pl_w = ew->master_w ;
	flag = 0 ;
	cnt = ( Uint8 )HI_BYTE( lw->unk_08 ) ;
	lw->unk_08 -= 0x140 ;
	if( ( Uint8 )HI_BYTE( lw->unk_08 ) != ( Uint8 )cnt ) flag = 1 ;
	if( ( Sint8 )cnt >= 0 ){
		LO_BYTE( turn ) = lw->wind_turn ;
		do {
			ew->prio = pl_w->prio ;
			if( ( LO_BYTE( ang ) = LO_BYTE( turn ) ) < 0 ) {
				ang = -( ang & 127 ) ;
				ang += 64 ;
				ang &= 0xff ;
			}
			LO_BYTE( ang ) += head_lw->unk_0a ;
			ew->dir = ang + 0xe0 ;
			LO_BYTE( ang ) = ( turn & 127 ) + 12 ;
			if( ( Sint8 )ang >= 64 ){
				ang &= 63 ;
				turn ^= 0x80 ;
			}
			turn = ( turn & 0x80 ) | ang ;
			lw->wind_turn = turn ;
			if( flag ){
				flag = 0 ;
				d = ang + 12 ;
				if( ( Sint8 )d >= 64 ){
					d &= 63 ;
					turn ^= 0x80 ;
				}
				turn = ( turn & 0x80 ) | d ;
				lw->wind_turn = turn ;
			}
			ang = ( ( ew->dir + 4 ) & 0xf8 ) / 8 ;
			SHL30_01_CHAR_SET( ew, lw, ang ) ;
			tail_w = ew ;
			ew = ew->behind_w ;
			lw = SHL30_01_LW( ew ) ;
		} while( -- cnt >= 0 ) ;
		if( 0 ) ew->pos.x = 1.6666666f ;
		if( 0 ) ew->pos.y = DOT_Y( 1.0f ) ;
		tail_w->pos.x = head_lw->start_x ;
		tail_w->pos.y = head_lw->start_y ;
		tail_w->disp_flag = 1 ;
		if( ( Uint8 )HI_BYTE( head_lw->unk_08 ) > 0 ){
			shl30_01_follow( tail_w, head_w ) ;
			return ;
		}
	}
	head_w->routine_0 ++ ;
	if( pl_w->routine_2 == 6 ) pl_w->routine_2 = 7 ;
	else pl_w->routine_2 = 3 ;
}

/*==============================================================*/
/*			The head has hit: the player holds the opponent		*/
/*==============================================================*/
static void shl30_01_hit( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w = pl_w->em_w ;

	if( !pl_sp_catch_ck( pl_w ) ){
		if( !pl_sp_catch_ck( ew ) ){
			shl30_01_head_wind_set( ew, lw ) ;
			return ;
		}
		em_w = ew->em_w ;
	}
	pl_w->atck_flag = 0 ;
	pl_w->routine_2 = 5 ;
	ew->routine_1 = 3 ;
	ew->timer0 = 45 ;
	scr_excep_set( pl_w, 13, 5 ) ;
	pl_w->dm_flag = 0 ;
	pl_w->catch_ng_timer = 3 ;
	pl_w->sp_ng_timer = 3 ;
	pl_w->catch_no = 0xc0 ;
	if( pl_w->pl_sp_type == 10 ) pl_w->catch_no = 0xc0 ;
	pl_ef_catch_set( ew, em_w ) ;
	lw->hold_w = shl30_02_hold_set( pl_w ) ;
	shl30_01_head_hold( ew, lw ) ;
}

/*==============================================================*/
/*			Pulling the opponent in								*/
/*==============================================================*/
static void shl30_01_head_hold( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w ;
	Float d ;

	pl_w->catch_ng_timer = 3 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_w->routine_2 == 6 ){
		shl30_01_head_throw_set( ew, lw ) ;
		return ;
	}
	if( pl_w->routine_2 == 9 || pl_w->routine_2 == 10 ){
		shl30_01_head_drain_set( ew, lw ) ;
		return ;
	}
	em_w = pl_w->catch_w ;
	if( em_w->routine_1 == PL_CAUGHT ){
		if( -- ew->timer0 <= 0 ){
			em_w->catch_type = 7 ;
			pl_w->routine_2 = 2 ;
			shl30_01_head_wind_set( ew, lw ) ;
			return ;
		}
	} else {
		pl_w->routine_2 = 2 ;
		shl30_01_head_wind_set( ew, lw ) ;
		return ;
	}
	ew->pos.x = ew->spd.x + lw->start_x ;
	ew->pos.y = ew->spd.y + lw->start_y ;
	lw->old_x = ew->pos.x ;
	d = ew->pos.y - pl_w->ground_y + -DOT_Y( 64.0f ) ;
	if( d < 0 ){
		pl_w->pos.y -= d ;
		ew->pos.y -= d ;
	}
	shl30_01_catch_pos( ew, lw, lw->hold_w, pl_w ) ;
	pl_w->pos.x += ew->pos.x - lw->old_x ;
	pl_wall_ck( pl_w ) ;
	ew->disp_flag = 1 ;
	lw->move_flag = 1 ;
	lw->part_cnt = 0 ;
}

static void shl30_01_chase_0( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_chase_1( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_chase_2( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_enter_1( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );
static void shl30_01_enter_2( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw );

/*==============================================================*/
/*			The opponent is in: the coil takes the next step	*/
/*			by the player's type								*/
/*==============================================================*/
static void shl30_01_head_throw_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	static void ( *shl30_01_enter_jmp[] )( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw ) = {
		shl30_01_chase_0, shl30_01_enter_1, shl30_01_enter_2,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_1 = 4 ;
	shl30_01_enter_jmp[pl_w->type]( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl30_01_head_throw( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	static void ( *shl30_01_chase_jmp[] )( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw ) = {
		shl30_01_chase_0, shl30_01_chase_1, shl30_01_chase_2,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pl_w->catch_ng_timer = 3 ;
	pl_w->sp_ng_timer = 3 ;
	shl30_01_chase_jmp[pl_w->type]( ew, lw ) ;
}

/*==============================================================*/
static void shl30_01_enter_1( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w = pl_w->catch_w ;

	if( em_w->pos.y > pl_w->pos.y + DOT_Y( 65.0f ) ) em_w->pos.y = pl_w->pos.y ;
	shl30_01_head_throw( ew, lw ) ;
}

static void shl30_01_link( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, Uint8 dir );
static void shl30_01_pos( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, SHL30_01_DATA *dat );
static void shl30_01_dat_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, SHL30_01_DATA *dat );
static void shl30_01_catch_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *plw );
static void shl30_01_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL30_LOCAL_W *plw );

/* chase of a coil that follows the data's path */
/*==============================================================*/
static void shl30_01_chase_1( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *head_w ;
	PLAYER_WORK *em_w ;
	PLAYER_WORK *w ;
	SHL30_01_LOCAL_W *head_lw ;
	SHL30_01_LOCAL_W *wlw ;
	SHL30_01_DATA *dat ;
	VEC3 p ;
	Float d ;
	Uint8 tgt ;
	Uint8 cur ;
	Uint8 diff ;
	Uint8 back ;
	Uint8 lim ;
	int i ;

	if( pl_w->cg_type >= 0 ){
		dat = &shl30_01_dat[pl_w->cg_type] ;
		lw->dat = dat ;
		if( dat->no < 0 ) scr_excep_x_clr() ;
		else {
			shl30_01_pos( ew, lw, dat ) ;
			ew->disp_flag = 1 ;
			w = ew ;
			head_w = ew ;
			em_w = pl_w->catch_w ;
			head_lw = lw ;
			wlw = lw ;
			for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
				p = w->pos ;
				p.x += wlw->rect[0] ;
				p.y -= wlw->rect[1] ;
				tgt = caldir_pos_256( &p, &em_w->pos ) ;
				diff = ( cur = w->dir ) - tgt ;
				if( diff ){
					if( !pl_w->disp_dir ){
						if( diff > 12 ) tgt = cur + 244 ;
					}
					else {
						if( diff < 244 ) tgt = cur + 12 ;
					}
				}
				back = head_w->dir + 0x80 ;
				lim = back ;
				if( pl_w->disp_dir ) lim = lim + 0x80 ;
				if( ( Sint8 )( lim - tgt ) >= 0 ) tgt = back ;
				w = w->behind_w ;
				wlw = SHL30_01_LW( w ) ;
				shl30_01_link( w, wlw, tgt ) ;
			}
			shl30_01_catch_pos( w, wlw, head_lw->hold_w, pl_w ) ;
			d = w->pos.x - wlw->old_x ;
			if( d != 0 ){
			shift :
				pl_w->pos.x += d ;
				w = head_w ;
				for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
					w->pos.x += d ;
					w = w->behind_w ;
				}
			}
			return ;
		}
	}
	else dat = lw->dat ;
	if( 1 ) ; else i = 0 ;
	pl_w->cg_type = -1 ;
	shl30_01_pos( ew, lw, dat ) ;
	ew->disp_flag = 1 ;
	w = ew ;
	head_w = ew ;
	wlw = lw ;
	head_lw = lw ;
	for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
		w = w->behind_w ;
		wlw = SHL30_01_LW( w ) ;
		tgt = head_w->dir ;
		cur = w->dir ;
		if( pl_w->disp_dir ){
			if( ( Uint8 )( cur - tgt ) > 17 ) tgt = cur + 239 ;
		}
		else {
			lim = tgt - cur ;
			if( lim > 17 ) tgt = cur + 17 ;
		}
		shl30_01_link( w, wlw, tgt ) ;
		if( 0 ) w->dir = cur ; else tgt = 14 ;
	}
	shl30_01_catch_pos( w, wlw, head_lw->hold_w, pl_w ) ;
	d = w->pos.x - wlw->old_x ;
	if( d != 0 ){
	add_b :
		pl_w->pos.x += d ;
		w = head_w ;
		for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
			w->pos.x += d ;
			w = w->behind_w ;
		}
	}
	shl30_01_catch_set( pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
	if( w->dir != head_w->dir ){
		if( 1 ) ; else i = 0 ;
		if( !head_lw->hold_w ) return ;
		if( head_lw->hold_w->pos.y > pl_w->ground_y ) return ;
		down_se_req( pl_w->catch_w ) ;
		shl30_01_catch_hold( head_w, pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
		if( head_w->routine_1 == 3 ){
			pl_voice_req( pl_w, 7 ) ;
			return ;
		}
	}
	em_w = pl_w->catch_w ;
	em_w->dm_hit_w = pl_w ;
	if( !pl_w->team_no ) em_w->catch_type = 1 ;
	else em_w->catch_type = 16 ;
	em_w->pl_dir = pl_w->pl_dir ;
	em_w->atck_no = shl30_01_hold_no[( ( PL30_LOCAL_W * )pl_w->tlocal )->level / 2] ;
	scr_excep_set( pl_w, 13, 6 ) ;
	head_w->routine_1 = 2 ;
	HI_BYTE( head_lw->unk_08 ) = head_lw->part_cnt ;
	LO_BYTE( head_lw->unk_08 ) = 0 ;
	head_lw->unk_0a = ew->dir ;
	head_lw->wind_turn = 0 ;
	if( head_lw->hold_w ){
		head_lw->hold_w->routine_0 = 2 ;
		head_lw->hold_w = 0 ;
	}
}

/*==============================================================*/
/*			A part takes direction dir, and its place from		*/
/*			the part before it									*/
/*==============================================================*/
static void shl30_01_link( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, Uint8 dir )
{
	PLAYER_WORK *bw ;
	SHL30_01_LOCAL_W *blw ;
	Uint8 no ;

	ew->dir = dir ;
	no = ( ( dir + 4 ) & 0xf8 ) / 8 ;
	SHL30_01_CHAR_SET( ew, lw, no ) ;
	bw = ew->before_w ;
	blw = SHL30_01_LW( bw ) ;
	ew->pos.x = lw->old_x = bw->pos.x + blw->rect[0] - lw->rect[2] ;
	ew->pos.y = bw->pos.y - blw->rect[1] + lw->rect[3] ;
	ew->prio = bw->prio ;
	ew->disp_flag = 1 ;
}

/*==============================================================*/
/*			Chase of a coil that turns the parts by their		*/
/*			limits												*/
/*==============================================================*/
static void shl30_01_chase_0( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *head_w ;
	PLAYER_WORK *w ;
	PLAYER_WORK *last_w ;
	PLAYER_WORK *bw ;
	PLAYER_WORK *em_w ;
	Float x ;
	SHL30_01_LOCAL_W *head_lw ;
	SHL30_01_LOCAL_W *blw ;
	SHL30_01_LOCAL_W *wlw ;
	SHL30_01_LOCAL_W *last_lw ;
	SHL30_01_DATA *dat ;
	SHL30_01_VEC2 v ;
	int turn ;
	Float d ;
	Uint8 tgt ;
	Uint8 cur ;
	Uint8 diff ;
	Uint8 no ;
	int i ;

	pl_w->catch_ng_timer = 3 ;
	pl_w->sp_ng_timer = 3 ;
	head_w = ew ;
	head_lw = lw ;
	if( pl_w->cg_type >= 0 ){
		dat = &shl30_01_dat[pl_w->cg_type] ;
		lw->dat = dat ;
		shl30_01_dat_set( ew, lw, dat ) ;
		if( dat->no >= 0 ) scr_excep_x_clr() ;
	}
	w = ew ;
	wlw = lw ;
	if( 0 ) head_lw->wind_turn = lw->part_cnt ;
	for( i = 0 ; i <= head_lw->part_cnt ; i ++ ){
		tgt = wlw->aim_dir ;
		diff = w->dir ;
		cur = diff ;
		turn = head_lw->turn_spd ;
		diff -= tgt ;
		if( diff ){
			if( 1 ) ; else no = 0 ;
			if( ( Sint8 )diff < 0 ){
			neg :
				turn = -turn ;
				if( ( Uint8 )turn > diff ){
					cur += head_lw->turn_spd ;
					goto set ;
				}
			}
			else {
			pos :
				if( 1 ) ; else no = 0 ;
				if( ( Uint8 )turn < diff ){
					cur -= turn ;
				set :
					tgt = cur ;
				}
			}
		}
		w->dir = tgt ;
		no = ( ( w->dir + 4 ) & 0xf8 ) / 8 ;
		SHL30_01_CHAR_SET( w, wlw, no ) ;
		if( !i ){
			v.x = wlw->start_x ;
			v.y = wlw->start_y ;
			if( head_lw->unk_35 ){
				v.x -= wlw->rect[2] ;
				v.y = v.y + wlw->rect[3] ;
			}
			w->pos.x = wlw->old_x = v.x ;
			w->pos.y = v.y ;
			w->disp_flag = 1 ;
			last_w = w ;
			last_lw = wlw ;
			w = w->behind_w ;
			wlw = SHL30_01_LW( w ) ;
			continue ;
		}
		bw = w->before_w ;
		blw = SHL30_01_LW( bw ) ;
		x = bw->pos.x + blw->rect[0] - wlw->rect[2] ;
		wlw->old_x = x ;
		w->pos.x = x ;
		w->pos.y = bw->pos.y - blw->rect[1] + wlw->rect[3] ;
		w->disp_flag = 1 ;
		last_w = w ;
		last_lw = wlw ;
		w = w->behind_w ;
		wlw = SHL30_01_LW( w ) ;
	}
	w = last_w ;
	shl30_01_catch_pos( w, last_lw, head_lw->hold_w, pl_w ) ;
	d = w->pos.x - last_lw->old_x ;
	if( d != 0 ){
	add_b :
		pl_w->pos.x += d ;
		bw = head_w ;
		for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
			bw->pos.x += d ;
			last_w = bw ;
			bw = bw->behind_w ;
		}
		w = last_w ;
	}
	if( head_lw->unk_0a ){
		shl30_01_catch_set( pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
		if( w->dir != head_w->dir ){
			if( 1 ) ; else no = 0 ;
			if( !head_lw->hold_w ) return ;
			if( head_lw->hold_w->pos.y > pl_w->ground_y ) return ;
			down_se_req( pl_w->catch_w ) ;
			shl30_01_catch_hold( head_w, pl_w, ( PL30_LOCAL_W * )pl_w->tlocal ) ;
			if( head_w->routine_1 == 3 ){
				pl_voice_req( pl_w, 7 ) ;
				return ;
			}
		}
		em_w = pl_w->catch_w ;
		em_w->dm_hit_w = pl_w ;
		if( !pl_w->team_no ) em_w->catch_type = 1 ;
		else em_w->catch_type = 16 ;
		em_w->pl_dir = pl_w->pl_dir ;
		em_w->atck_no = shl30_01_hold_no[( ( PL30_LOCAL_W * )pl_w->tlocal )->level / 2] ;
		scr_excep_set( pl_w, 13, 6 ) ;
		head_w->routine_1 = 2 ;
		HI_BYTE( head_lw->unk_08 ) = head_lw->part_cnt ;
		LO_BYTE( head_lw->unk_08 ) = 0 ;
		head_lw->unk_0a = ew->dir ;
		head_lw->wind_turn = 0 ;
		if( head_lw->hold_w ){
			head_lw->hold_w->routine_0 = 2 ;
			head_lw->hold_w = 0 ;
		}
	}
}

/*==============================================================*/
static void shl30_01_enter_2( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float y = pl_w->pos.y + 0.544084787f ;

	if( pl_w->catch_w->pos.y > y ) pl_w->catch_w->pos.y = pl_w->pos.y ;
	shl30_01_chase_2( ew, lw ) ;
}

/*==============================================================*/
/*			Chase of a coil that is let out and drawn in		*/
/*==============================================================*/
static void shl30_01_chase_2( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *head_w ;
	PLAYER_WORK *em_w ;
	PLAYER_WORK *w ;
	SHL30_01_LOCAL_W *head_lw ;
	SHL30_01_LOCAL_W *wlw ;
	SHL30_01_DATA *dat ;
	VEC3 p ;
	Float d ;
	Uint8 tgt ;
	Uint8 cur ;
	Uint8 diff ;
	Uint8 back ;
	Uint8 lim ;
	int i ;

	if( lw->part_cnt != 14 ){
		if( lw->part_cnt < 14 ) lw->part_cnt ++ ;
		else lw->part_cnt -- ;
	}
	if( pl_w->cg_type < 0 ) scr_excep_x_clr() ;
	else {
		dat = &shl30_01_dat[pl_w->cg_type] ;
		lw->dat = dat ;
		if( dat->no < 0 ){
			scr_excep_x_clr() ;
			goto steer_b ;
		}
		shl30_01_pos( ew, lw, dat ) ;
		ew->disp_flag = 1 ;
		w = ew ;
		head_w = ew ;
		em_w = pl_w->catch_w ;
		head_lw = lw ;
		wlw = lw ;
		for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
			p = w->pos ;
			p.x += wlw->rect[0] ;
			p.y -= wlw->rect[1] ;
			tgt = caldir_pos_256( &p, &em_w->pos ) ;
			diff = cur = w->dir ;
			diff -= tgt ;
			if( diff ){
				if( !pl_w->disp_dir ){
					if( diff > 12 ) tgt = cur + 244 ;
				}
				else {
					if( diff < 244 ) tgt = cur + 12 ;
				}
			}
			back = head_w->dir + 0x80 ;
			lim = back ;
			if( pl_w->disp_dir ) lim = lim + 0x80 ;
			if( ( Sint8 )( lim - tgt ) >= 0 ) tgt = back ;
			w = w->behind_w ;
			wlw = SHL30_01_LW( w ) ;
			shl30_01_link( w, wlw, tgt ) ;
		}
		shl30_01_catch_pos( w, wlw, head_lw->hold_w, pl_w ) ;
		d = w->pos.x - wlw->old_x ;
		if( d != 0 ){
		shift :
			pl_w->pos.x += d ;
			w = head_w ;
			for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
				w->pos.x += d ;
				w = w->behind_w ;
			}
		}
		return ;
	}
	dat = lw->dat ;
steer_b :
	pl_w->cg_type = -1 ;
	shl30_01_pos( ew, lw, dat ) ;
	ew->disp_flag = 1 ;
	if( 0 ) head_lw->wind_turn = lw->part_cnt ;
	w = ew ;
	head_w = ew ;
	wlw = lw ;
	head_lw = lw ;
	for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
		w = w->behind_w ;
		wlw = SHL30_01_LW( w ) ;
		tgt = head_w->dir ;
		cur = w->dir ;
		if( pl_w->disp_dir ){
			if( ( Uint8 )( cur - tgt ) > 17 ) tgt = cur + 239 ;
		}
		else {
			lim = tgt - cur ;
			if( lim > 17 ) tgt = cur + 17 ;
		}
		shl30_01_link( w, wlw, tgt ) ;
		if( 0 ) w->dir = cur ; else tgt = 14 ;
	}
	shl30_01_catch_pos( w, wlw, head_lw->hold_w, pl_w ) ;
	d = w->pos.x - wlw->old_x ;
	if( d != 0 ){
	add_b :
		pl_w->pos.x += d ;
		w = head_w ;
		for( i = 0 ; i < head_lw->part_cnt ; i ++ ){
			w->pos.x += d ;
			w = w->behind_w ;
		}
	}
	if( w->dir != head_w->dir ){
		if( 1 ) ; else i = 0 ;
		if( !head_lw->hold_w ) return ;
		if( head_lw->hold_w->pos.y > pl_w->ground_y ) return ;
		if( head_w->routine_1 == 3 ){
			pl_voice_req( pl_w, 7 ) ;
			return ;
		}
	}
	p = pl_w->catch_w->pos ;
	p.y = pl_w->catch_w->ground_y ;
	burst_set( pl_w->catch_w, &p, 1, 0x179 ) ;
	em_w = pl_w->catch_w ;
	em_w->dm_hit_w = pl_w ;
	if( !pl_w->team_no ) em_w->catch_type = 1 ;
	else em_w->catch_type = 16 ;
	em_w->pl_dir = pl_w->pl_dir ;
	em_w->atck_no = pl30_at_coil_slam ;
	scr_excep_set( pl_w, 13, 6 ) ;
	head_w->routine_1 = 2 ;
	HI_BYTE( head_lw->unk_08 ) = head_lw->part_cnt ;
	LO_BYTE( head_lw->unk_08 ) = 0 ;
	head_lw->unk_0a = ew->dir ;
	head_lw->wind_turn = 0 ;
	if( head_lw->hold_w ){
		head_lw->hold_w->routine_0 = 2 ;
		head_lw->hold_w = 0 ;
	}
}

/*==============================================================*/
/*			Sets the parts, from the last to the head, to		*/
/*			follow one another									*/
/*==============================================================*/
static void shl30_01_follow( PLAYER_WORK *w, PLAYER_WORK *head_w )
{
	PLAYER_WORK *aw ;
	SHL30_01_LOCAL_W *alw ;
	SHL30_01_LOCAL_W *wlw ;

	do {
		aw = w ;
		w = w->before_w ;
		alw = SHL30_01_LW( aw ) ;
		wlw = SHL30_01_LW( w ) ;
		w->pos.x = aw->pos.x + alw->rect[0] - wlw->rect[2] ;
		w->pos.y = aw->pos.y - alw->rect[1] + wlw->rect[3] ;
		w->disp_flag = 1 ;
	} while( head_w != w ) ;
}

/*==============================================================*/
/*			The opponent is held: each part gets a spark		*/
/*==============================================================*/
static void shl30_01_head_drain_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w ;
	PLAYER_WORK *w ;
	PLAYER_WORK *sw ;
	SHL30_01_LOCAL_W *wlw ;
	int i ;
	Sint16 k ;

	em_w = pl_w->catch_w ;
	em_w->catch_cnt += 2 ;
	ew->routine_1 = 5 ;
	lw->unk_08 = 0 ;
	ew->timer0 = 60 ;
	hold_renda_set( em_w, 24 ) ;
	catch_renda_set( pl_w, 6 ) ;
	if( pl_w->routine_2 == 9 ) lw->drain_type = 1 ;
	else lw->drain_type = 2 ;
	w = ew ;
	wlw = lw ;
	k = 0 ;
	for( i = 0 ; i <= lw->part_cnt ; i ++ ){
		if( ( wlw->unk_0a = k ) ){
			if( !( k & 1 ) ){
				if( ( sw = shl30_02_drain_set( w, ew, &lw->unk_08 ) ) ){
					sw->timer0 = 2 ;
					sw->timer1 = lw->part_cnt - i ;
					sw->sub_type = lw->drain_type ;
				}
			}
		}
		w = w->behind_w ;
		wlw = SHL30_01_LW( w ) ;
		k -- ;
	}
	pls30_00_set( pl_w, 0, 0 ) ;
	pls30_00_set( pl_w, 0, 1 ) ;
	dm00_06_set( pl_w, lw->hold_w, 0 ) ;
	shl30_01_head_drain( ew, lw ) ;
}

static void shl30_01_dm( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Held: the opponent is hurt each time the buttons	*/
/*			are mashed											*/
/*==============================================================*/
static void shl30_01_head_drain( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *w ;
	PLAYER_WORK *sw ;
	SHL30_01_LOCAL_W *wlw ;
	Uint16 sw_now ;
	Float d ;
	Uint8 no ;
	int i ;

	pl_w->catch_ng_timer = 3 ;
	pl_w->sp_ng_timer = 3 ;
	if( pl_w->routine_2 == 6 ){
		shl30_01_head_throw_set( ew, lw ) ;
		return ;
	}
	if( ( sw_now = pl_w->pl_sw_now & KEY_ATCK ) ){
		if( sw_now & KEY_PUNCH ){
			lw->drain_type = 1 ;
			pl_w->routine_2 = 9 ;
		}
		else {
			lw->drain_type = 2 ;
			pl_w->routine_2 = 10 ;
		}
	}
	if( -- ew->timer0 > 0 ){
		if( !( ew->timer0 & 7 ) ){
			pl_voice_req( pl_w, 30 ) ;
			shl30_01_dm( ew, lw, pl_w ) ;
		}
		if( hold_renda_ck( pl_w->catch_w ) ) ew->timer0 = -1 ;
	}
	if( ew->timer0 < 0 ){
		pl_w->routine_2 = 2 ;
		pl_w->catch_w->catch_type = 7 ;
		shl30_01_head_wind_set( ew, lw ) ;
		return ;
	}
	if( catch_renda_ck( pl_w ) ) shl30_01_dm( ew, lw, pl_w ) ;
	if( -- lw->unk_08 <= 0 ){
		lw->unk_08 = 2 ;
		if( !( lw->unk_0a & 1 ) ){
			if( ( sw = shl30_02_drain_set( ew, ew, &lw->unk_08 ) ) ){
				sw->sub_type = lw->drain_type ;
				sw->timer0 = lw->unk_0a / 2 ;
				sw->timer1 = lw->part_cnt ;
			}
		}
		w = ew ;
		wlw = lw ;
		for( i = 0 ; i <= lw->part_cnt ; i ++ ){
			if( wlw->unk_0a < 0 ) wlw->unk_0a ++ ;
			else {
				w->char_no.b.no = shl30_01_tbl_b[shl30_01_tbl_a[wlw->unk_0a]] ;
				no = ( ( w->dir + 4 ) & 0xf8 ) / 8 ;
				ef_char_set2( w, 27, w->char_no.b.no, no ) ;
				wlw->unk_0a = ( wlw->unk_0a + 1 ) & 31 ;
			}
			w = w->behind_w ;
			wlw = SHL30_01_LW( w ) ;
		}
	}
	ew->pos.x = lw->old_x = ew->spd.x + lw->start_x ;
	ew->pos.y = ew->spd.y + lw->start_y ;
	d = ew->pos.y - pl_w->ground_y + -DOT_Y( 64.0f ) ;
	if( d < 0 ){
		ew->pos.y += d ;
		pl_w->pos.y += d ;
	}
	shl30_01_catch_pos( ew, lw, lw->hold_w, pl_w ) ;
	pl_w->pos.x += ew->pos.x - lw->old_x ;
	pl_wall_ck( pl_w ) ;
	ew->disp_flag = 1 ;
	lw->move_flag = 1 ;
	lw->part_cnt = 0 ;
}

/*==============================================================*/
/*			The hit the held opponent takes						*/
/*==============================================================*/
static void shl30_01_dm( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 9 ){
		vital_add( pl_w, 1 ) ;
		hold_damage( pl_w->catch_w, -2 ) ;
	}
	else {
		pl_gauge_add( pl_w, 1 ) ;
		pl_gauge_add( pl_w->catch_w, -2 ) ;
	}
}

/*==============================================================*/
/*			A part: it follows the part before it, and shows	*/
/*			while it is past the head's limit					*/
/*==============================================================*/
static void shl30_01_part_move( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *hew = ew->master_w ;
	SHL30_01_LOCAL_W *hlw = SHL30_01_LW( hew ) ;
	PLAYER_WORK *pl_w = ew->ef_w ;
	PLAYER_WORK *bw ;
	SHL30_01_LOCAL_W *blw ;
	Float rng[2] ;

	if( !hlw->move_flag ) return ;
	bw = ew->before_w ;
	blw = SHL30_01_LW( bw ) ;
	ew->prio = pl_w->prio ;
	ew->pos.x = bw->pos.x + blw->rect[2] - lw->rect[0] ;
	ew->pos.y = bw->pos.y - blw->rect[3] + lw->rect[1] ;
	ew->disp_flag = 0 ;
	rng[0] = ew->pos.x ;
	rng[1] = hlw->start_x ;
	if( ( Sint8 )ew->dir < 0 ){
		rng[0] = hlw->start_x ;
		rng[1] = ew->pos.x ;
	}
	if( !( rng[0] > rng[1] ) ){
		rng[0] = ew->pos.y ;
		rng[1] = hlw->start_y ;
		if( ew->dir > 64 && ew->dir < 192 ){
			rng[0] = hlw->start_y ;
			rng[1] = ew->pos.y ;
		}
		if( !( rng[0] > rng[1] ) ) goto next ;
	}
	ew->disp_flag = 1 ;
	hlw->part_cnt ++ ;
	if( !hew->routine_1 && hlw->rest_cnt < 4 ){
		hew->routine_1 = 1 ;
		hlw->unk_08 = 0xa00 ;		/* overwritten at once */
		hlw->unk_08 = 0 ;
		hew->acc.x = 4.0f ;
	}
next :
	if( -- hlw->rest_cnt <= 0 ){
		if( hew->routine_1 <= 1 ){
			hew->cg_atck_flag = CG_ATCK_ON ;
			hit_work_set( hew ) ;
			if( !hlw->unk_32 ){
				hlw->unk_32 = 1 ;
				pl_w->atck_flag = -128 ;
			}
		}
	}
}

/*--------------------------------------------------------------*/
static void shl30_01_end( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	PLAYER_WORK *cw = lw->hold_w ;
	PLAYER_WORK *w ;
	int i ;

	if( cw ) cw->routine_0 = 2 ;
	w = ew ;
	for( i = 14 ; i > 0 ; i -- ){
		w = w->behind_w ;
		work_free( w ) ;
	}
	shl30_01_free( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl30_01_free( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The opponent's effect takes the player's hold		*/
/*			position											*/
/*==============================================================*/
static void shl30_01_catch_pos( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, PLAYER_WORK *ow, PLAYER_WORK *pl_w )
{
	static Uint8 shl30_01_hold_tbl[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x10, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09,
	} ;
	PLAYER_WORK *w ;
	Sint16 *ofs ;
	Uint8 kind ;
	Uint8 idx ;
	int base = 16 ;
	int add = 0 ;

	if( !pl_w->disp_dir ){
		base = 0xf0 ;
		add = 8 ;
	}
	idx = ( ( Uint8 )( base + ew->dir ) >> 5 ) + add ;
	LO_BYTE( ew->cg_rival ) = shl30_01_hold_tbl[idx] ;
	catch_pos_set( ew, pl_w->catch_w ) ;
	w = ow ;
	if( w ){
		kind = shl30_01_tbl_c[pl_w->catch_w->pl_type] ;
		idx = LO_BYTE( ew->cg_rival ) + 0xff ;
		w->pos = ew->pos ;
		ofs = shl30_01_catch_ofs[kind][idx] ;
		w->pos.x += DOT_X( ofs[0] ) ;
		w->pos.y -= ofs[1] * DOT_Y( 1.0f ) ;
		switch( w->disp_dir = shl30_01_tbl_e[idx].dir ){
		case 2 :
			w->rot[2] = 0x8000 ;
			w->disp_dir = 1 ;
			break ;
		case 3 :
			w->rot[2] = 0x8000 ;
			w->disp_dir = 0 ;
			break ;
		default :
			w->rot[2] = 0 ;
			break ;
		}
		pl_char_set( w, 20, shl30_01_tbl_e[idx].no + shl30_01_tbl_d[kind] ) ;
	}
}

/*==============================================================*/
/*			The first part of the chain takes the data's		*/
/*			start												*/
/*==============================================================*/
static void shl30_01_pos( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, SHL30_01_DATA *dat )
{
	PLAYER_WORK *pl_w = ew->ef_w ;
	Float v[2] ;
	Uint8 dir = dat->dir ;
	Uint8 no ;

	v[0] = DOT_X( dat->x ) ;
	if( pl_w->disp_dir ){
		v[0] = -v[0] ;
		dir = -dir ;
	}
	ew->dir = dir ;
	lw->ofs_x = v[0] ;
	lw->ofs_y = dat->y * DOT_Y( 1.0f ) ;
	v[0] = lw->start_x = pl_w->pos.x + lw->ofs_x ;
	v[1] = lw->start_y = pl_w->pos.y + lw->ofs_y ;
	no = ( ( dir + 4 ) & 0xf8 ) / 8 ;
	SHL30_01_CHAR_SET( ew, lw, no ) ;
	if( dat->unk_03 ){
		v[0] = v[0] - lw->rect[2] ;
		v[1] += lw->rect[3] ;
	}
	ew->pos.x = lw->old_x = v[0] ;
	ew->pos.y = v[1] ;
	ew->prio = pl_w->prio ;
}

/*==============================================================*/
/*			The data's turning limits go to each part			*/
/*==============================================================*/
static void shl30_01_dat_set( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw, SHL30_01_DATA *dat )
{
	PLAYER_WORK *pl_w = ew->ef_w ;
	SHL30_01_LOCAL_W *wlw ;
	Float v[2] ;
	Sint8 cur ;
	Uint8 step ;
	Uint8 accel ;
	Sint8 limit ;
	Uint8 n ;
	int i ;

	v[0] = DOT_X( dat->x ) ;
	v[1] = dat->y * DOT_Y( 1.0f ) ;
	lw->unk_0a = dat->no ;
	lw->unk_35 = dat->unk_03 ;
	cur = dat->aim_dir ;
	step = dat->aim_step ;
	accel = dat->aim_acc ;
	limit = dat->aim_limit ;
	lw->turn_spd = dat->turn_spd ;
	if( pl_w->disp_dir ){
		v[0] = -v[0] ;
		cur = -cur ;
		step = -step ;
		accel = -accel ;
		limit = -limit ;
	}
	lw->ofs_x = v[0] ;
	wlw = lw ;
	lw->ofs_y = v[1] ;
	lw->start_x = pl_w->pos.x + v[0] ;
	lw->start_y = pl_w->pos.y + v[1] ;
	ew->prio = pl_w->prio ;
	i = 0 ;
	goto test ;
	{
	next :
		wlw->aim_dir = cur ;
		cur += step ;
		if( ( Sint8 )step >= 0 ){
			if( ( Sint8 )( limit - cur ) >= 0 ) goto acc ;
		}
		else {
			if( ( Sint8 )( limit - cur ) < 0 ) goto acc ;
		}
		cur = limit ;
	acc :
		if( 0 ) ew->disp_flag = 0 ;
		step += accel ;
		if( !accel ) goto next_w ;
		if( ( Sint8 )accel < 0 ){
			if( ( Sint8 )step > 0 ) goto next_w ;
			n = 1 ;
		}
		else {
			if( ( Sint8 )step < 0 ) goto next_w ;
			n = 0xff ;
		}
		step = n ;
	next_w :
		ew = ew->behind_w ;
		wlw = SHL30_01_LW( ew ) ;
		i ++ ;
	}
test :
	if( i <= lw->part_cnt ) goto next ;
}

/*==============================================================*/
static void shl30_01_ground( PLAYER_WORK *ew, SHL30_01_LOCAL_W *lw )
{
	pls30_00_set( ew->master_w, ew, 2 ) ;
	shl30_01_head_wind_set( ew, lw ) ;
}

/*==============================================================*/
/*			The player may take the hold by a button			*/
/*==============================================================*/
static void shl30_01_catch_set( PLAYER_WORK *pl_w, PL30_LOCAL_W *plw )
{
	Uint16 k ;

	if( pl_w->catch_w->catch_cnt >= 1 ) return ;
	if( !( pl_w->pl_sw_new & KEY_LEVER ) ) return ;
	if( pl_w->pl_sw_now & KEY_PUNCH ) k = 0 ;
	else if( pl_w->pl_sw_now & KEY_KICK ) k = 1 ;
	else return ;
	pl_w->catch_w->catch_cnt += 2 ;
	plw->type = k ;
	plw->lever = pl_w->pl_sw_new ;
	plw->key_flag = 1 ;
}

/*==============================================================*/
/*			The hold starts: the opponent is hurt and thrown	*/
/*==============================================================*/
static void shl30_01_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PL30_LOCAL_W *plw )
{
	VEC3 p ;
	Uint8 k ;

	if( plw->key_flag ){
		plw->key_flag = 0 ;
		k = 0 ;
		if( plw->level < 10 ){
			k = 1 ;
			if( plw->lever & 0xc00 ) plw->lever ^= 0xc00 ;
		}
		plw->turn = k ;
		pl_w->catch_w->dm_hit_w = pl_w ;
		pl_w->catch_w->atck_no = pl30_at_coil_throw ;
		catch_damage( pl_w ) ;
		if( pl_w->catch_w->vital_new ){
			p = pl_w->catch_w->pos ;
			p.y = pl_w->catch_w->ground_y ;
			land_dust_set( &p, pl_w->catch_w->pl_no ) ;
			ew->routine_1 = 3 ;
			pl_w->routine_2 = 11 ;
		}
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl30_01.c" End of File	*/
