/* The fight's front words (work kind 12): one work for each side, which flies in a word when its
   player hits first, lands a long combo (the hits, with its bonus), recovers or runs low on vital,
   and counts the hits as the combo goes on. */
#include "player.h"

extern void obj_col_set( int *obj, int no ) ;
extern Float obj_scale_max( VEC3 *scale ) ;

extern void scr_2d_matrix_set( void ) ;
extern int sprintf( char *s, const char *fmt, ... ) ;

/* one object of a word, in the 2D camera: colour (0 keeps the object's own), the number place it
   shows (or -1 for the fixed object), the scale to draw at, the fixed object, and the x it moves on by */
typedef struct EF12_CHR {
	Sint8	col ;
	Sint8	slot ;
	Sint8	scl ;
	Sint8	obj ;
	Float	x ;
} EF12_CHR ;

/* the work's own fields are PLAYER_WORK's spare ones; the digits, from atck_flag, are bytes of a
   number for each place (hits: three, bonus: eight), negative for a blank */
typedef struct EF12_NUM_W {
	union {
		Sint32	clr[4] ;
		Uint8	hit[3] ;
		Sint8	bonus[8] ;
	} num ;
} EF12_NUM_W ;
#define EF12_NUM( w )	( ( EF12_NUM_W * )&( w )->atck_flag )
#define EF12_HITS( w )	( ( w )->col_no )	/* the combo's hits */
#define EF12_BTN( w )	( ( w )->disp_dir )	/* the kinds of hit in it, from combo_kind */
#define EF12_HITS_OLD( w )	( ( w )->disp_ofs_x )
#define EF12_KIND( w )	( ( w )->size_top )	/* the word that is shown */
#define EF12_FLAG( w )	( ( w )->size_bottom )	/* set when the combo has ended */
#define EF12_MARK( w )	( ( w )->size_front )	/* a bit for each player whose low vital has been shown; 0xff for the computer */
#define EF12_WAIT( w )	( ( w )->size_back )	/* frames left of a delay set by the player's unk_1ee */
/* what is drawn and the bonus under it: lists of EF12_CHR, kept as addresses */
#define EF12_CHR0( w )	( ( EF12_CHR * )( w )->work2 )
#define EF12_CHR1( w )	( ( EF12_CHR * )( w )->work3 )

/* where each side's work stands */
static VEC3 ef12_pos_tbl[2][2] = {
	{ { -40.0f, 5.0f, -40.0f }, { 0.0f, 0.0f, 0.0f } },
	{ { 40.0f, 5.0f, -40.0f }, { 0.0f, 0.0f, 0.0f } },
} ;

/* the scales a word's objects are drawn at */
static VEC3 ef12_scale0 = { 1.5f, 1.5f, 1.0f } ;
static VEC3 ef12_scale1 = { 0.66f, 0.66f, 1.0f } ;
static VEC3 ef12_scale2 = { 0.5f, 1.0f, 1.0f } ;
static VEC3 ef12_scale3 = { 1.0f, 0.5f, 1.0f } ;
static VEC3 ef12_scale4 = { 2.5f, 2.5f, 1.0f } ;
static VEC3 *ef12_scale_p[8] = {
	&ef12_scale0, &ef12_scale0, &ef12_scale1, &ef12_scale2,
	&ef12_scale3, &ef12_scale4, &ef12_scale0, &ef12_scale0,
} ;

static EF12_CHR ef12_hit_chr[] = {
	{ 2, 0, 1, 0, 2.5f }, { 2, 1, 0, 0, 3.5f }, { 2, 2, 0, 0, 3.5f }, { 4, -1, 2, 17, 2.81f },
	{ 4, -1, 0, 18, 2.68f }, { 4, -1, 0, 29, 2.38f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_bonus_chr[] = {
	{ 2, 0, 3, 0, 1.0f }, { 2, 1, 0, 0, 3.5f }, { 2, 2, 0, 0, 3.5f }, { 2, 3, 0, 0, 3.5f },
	{ 2, 4, 0, 0, 3.5f }, { 2, 5, 0, 0, 3.5f }, { 2, 6, 0, 0, 3.5f }, { 2, 7, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 3, -1, 4, 36, 3.5f },
	{ 3, -1, 0, 37, 2.25f }, { 3, -1, 0, 38, 2.25f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg0_chr[] = {
	{ 3, -1, 0, 15, 1.0f }, { 3, -1, 0, 18, 2.42f }, { 3, -1, 0, 27, 2.73f }, { 3, -1, 0, 28, 3.4299998f },
	{ 3, -1, 0, 29, 3.3000002f }, { 3, -1, 0, 10, 5.7799997f }, { 3, -1, 0, 29, 3.0599995f }, { 3, -1, 0, 29, 3.460001f },
	{ 3, -1, 0, 10, 3.039999f }, { 3, -1, 0, 12, 3.3500004f }, { 3, -1, 0, 20, 3.7000008f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg1_chr[] = {
	{ 3, -1, 0, 29, 1.5f }, { 3, -1, 0, 14, 3.06f }, { 3, -1, 0, 12, 3.06f }, { 3, -1, 0, 17, 3.58f },
	{ 3, -1, 0, 17, 6.3499994f }, { 3, -1, 0, 18, 2.6400013f }, { 3, -1, 0, 29, 2.6499996f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg2_chr[] = {
	{ 3, -1, 0, 12, 1.5f }, { 3, -1, 0, 24, 3.45f }, { 3, -1, 0, 30, 3.4799998f }, { 3, -1, 0, 23, 3.5300002f },
	{ 3, -1, 0, 29, 3.4700003f }, { 3, -1, 0, 14, 3.1299992f }, { 3, -1, 0, 27, 3.0900002f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg3_chr[] = {
	{ -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg4_chr[] = {
	{ 1, -1, 0, 16, 1.5f }, { 1, -1, 0, 24, 3.49f }, { 1, -1, 0, 24, 3.5800002f }, { 1, -1, 0, 13, 3.52f },
	{ -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg5_chr[] = {
	{ 1, -1, 0, 16, 1.5f }, { 1, -1, 0, 27, 3.55f }, { 1, -1, 0, 14, 3.1200001f }, { 1, -1, 0, 10, 3.08f },
	{ 1, -1, 0, 29, 2.8599997f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg6_chr[] = {
	{ 1, -1, 0, 31, 1.5f }, { 1, -1, 0, 14, 3.0f }, { 1, -1, 0, 27, 3.0300002f }, { 1, -1, 0, 34, 3.1699996f },
	{ 1, -1, 0, 16, 5.76f }, { 1, -1, 0, 24, 3.4399996f }, { 1, -1, 0, 24, 3.5f }, { 1, -1, 0, 13, 3.5f },
	{ -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg7_chr[] = {
	{ 1, -1, 0, 32, 3.0f }, { 1, -1, 0, 24, 4.51f }, { 1, -1, 0, 23, 3.5100002f }, { 1, -1, 0, 13, 3.5999994f },
	{ 1, -1, 0, 14, 3.1599998f }, { 1, -1, 0, 27, 3.0999994f }, { 1, -1, 0, 15, 3.18f }, { 1, -1, 0, 30, 3.03f },
	{ 1, -1, 0, 21, 3.25f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg8_chr[] = {
	{ 1, -1, 0, 15, 1.0f }, { 1, -1, 0, 10, 2.76f }, { 1, -1, 0, 23, 3.34f }, { 1, -1, 0, 29, 3.52f },
	{ 1, -1, 0, 10, 2.9400005f }, { 1, -1, 0, 28, 3.3999996f }, { 1, -1, 0, 29, 3.3200006f }, { 1, -1, 0, 18, 2.67f },
	{ 1, -1, 0, 12, 2.7299995f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg9_chr[] = {
	{ 1, -1, 0, 22, 2.0f }, { 1, -1, 0, 10, 3.8f }, { 1, -1, 0, 27, 3.45f }, { 1, -1, 0, 31, 3.3999996f },
	{ 1, -1, 0, 14, 3.3100004f }, { 1, -1, 0, 21, 3.2400007f }, { 1, -1, 0, 24, 2.7199993f }, { 1, -1, 0, 30, 3.6200008f },
	{ 1, -1, 0, 28, 3.6899986f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg10_chr[] = {
	{ 3, -1, 0, 10, 1.5f }, { 3, -1, 0, 14, 3.06f }, { 3, -1, 0, 27, 3.02f }, { 3, -1, 0, 18, 2.5900002f },
	{ 3, -1, 0, 10, 2.5500002f }, { 3, -1, 0, 21, 3.3999996f }, { 3, -1, 0, 27, 5.5299997f }, { 3, -1, 0, 10, 3.4400005f },
	{ 3, -1, 0, 31, 2.8600006f }, { 3, -1, 0, 14, 3.17f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg11_chr[] = {
	{ 3, -1, 0, 10, 1.5f }, { 3, -1, 0, 18, 2.42f }, { 3, -1, 0, 27, 2.56f }, { 3, -1, 0, 12, 7.27f },
	{ 3, -1, 0, 24, 3.54f }, { 3, -1, 0, 22, 4.000001f }, { 3, -1, 0, 11, 4.0699997f }, { 3, -1, 0, 24, 3.4099998f },
	{ -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg12_chr[] = {
	{ 3, -1, 0, 13, 1.5f }, { 3, -1, 0, 10, 3.3f }, { 3, -1, 0, 23, 3.4199998f }, { 3, -1, 0, 16, 3.56f },
	{ 3, -1, 0, 14, 3.1999998f }, { 3, -1, 0, 27, 2.960001f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg13_chr[] = {
	{ 1, -1, 0, 34, 1.5f }, { 1, -1, 0, 14, 2.93f }, { 1, -1, 0, 28, 2.9599998f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_msg14_chr[] = {
	{ 1, -1, 0, 22, 2.0f }, { 1, -1, 0, 24, 3.92f }, { 1, -1, 0, 23, 3.5099998f }, { 1, -1, 0, 28, 3.56f },
	{ 1, -1, 0, 29, 3.3900003f }, { 1, -1, 0, 14, 3.13f }, { 1, -1, 0, 27, 3.0200005f }, { -1, -1, 0, 0, 0.0f },
} ;

/* each word's objects */
static EF12_CHR *ef12_chr_tbl[16] = {
	ef12_msg0_chr, ef12_msg1_chr, ef12_msg2_chr, ef12_msg3_chr,
	ef12_msg4_chr, ef12_msg5_chr, ef12_msg6_chr, ef12_msg7_chr,
	ef12_msg8_chr, ef12_msg9_chr, ef12_msg10_chr, ef12_msg11_chr,
	ef12_msg12_chr, ef12_msg13_chr, ef12_msg14_chr, ef12_msg0_chr,
} ;

/* where a word on the second side stops short of the edge, and the sound each word asks for */
static Float ef12_x_tbl[16] = {
	37.0f, 24.5f, 23.5f, 20.0f, 14.0f, 16.0f, 28.5f, 31.5f,
	27.5f, 31.0f, 32.5f, 30.5f, 19.5f, 9.0f, 24.0f, 0.0f,
} ;
static Uint8 ef12_se_tbl[16] = { 0, 0, 0, 0, 17, 19, 18, 20, 21, 23, 0, 0, 0, 16, 22, 0 } ;

static void ef12_set( int no ) ;
static void ef12_main( PLAYER_WORK *ew ) ;
static void ef12_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_first( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_first_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_first_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_first_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_idle( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_count( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_msg( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_msg_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_msg_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_msg_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_msg_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_msg_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_over( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static int ef12_ck_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static int ef12_ck_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static int ef12_ck_act( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static int ef12_ck_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static int ef12_ck_vital( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static int ef12_hits_get( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_chr_clr( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, int kind ) ;
static void ef12_bonus_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_chr_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void ef12_hit_digit( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/****************************************************************/
/*			The two works, one for each side (bg_init calls		*/
/*			it)													*/
/****************************************************************/
void ef12_init( void )
{
	if( Sys->play_mode != 64 ){
		ef12_set( 0 ) ;
		ef12_set( 1 ) ;
	}
}

/*==============================================================*/
static void ef12_set( int no )
{
	PLAYER_WORK *ew, *pl_w ;
	Sint32 *p ;
	int i ;

	if( ( ew = work_get( 0, W_FRONT, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = ef12_main ;
		ew->obj = 0 ;
		ew->obj_flag = 0 ;
		ew->pl_no = no ;
		ew->pos = ef12_pos_tbl[no][0] ;
		ew->scale = ef12_scale0 ;
		pl_w = &pl_work[no] ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		p = EF12_NUM( ew )->num.clr ;
		i = 0 ;
		do {
			*p ++ = 0 ;
			i ++ ;
		} while( i < 4 ) ;
	}
}

/*==============================================================*/
static void ef12_main( PLAYER_WORK *ew )
{
	static void ( *ef12_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef12_start, ef12_run, ef12_free, ef12_free,
	} ;

	ew->unk_138 = vs_sys_work.team_w[ew->pl_no][0] ;
	ef12_jmp[ew->routine_0]( ew, ew->unk_138 ) ;
}

/*==============================================================*/
static void ef12_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	EF12_HITS( ew ) = 0 ;
	EF12_HITS_OLD( ew ) = 0 ;
	EF12_BTN( ew ) = 0 ;
	EF12_WAIT( ew ) = 0 ;
	EF12_MARK( ew ) = 0 ;
	ew->type = 0 ;
	if( pl_w->cpu_flag ) EF12_MARK( ew ) = 0xff ;
}

/*==============================================================*/
/*			Type 0 waits for the first hit, 1 for what comes	*/
/*			next, 2 counts a combo, 3 and 4 show a word, 5		*/
/*			waits for the round to end							*/
/*==============================================================*/
static void ef12_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *ef12_type_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef12_first, ef12_idle, ef12_hit, ef12_msg, ef12_msg, ef12_over,
	} ;
	EF12_HITS_OLD( ew ) = EF12_HITS( ew ) ;
	if( EF12_WAIT( ew ) < pl_w->vcounter_msg_timer ){
		EF12_WAIT( ew ) = pl_w->vcounter_msg_timer ;
		pl_w->vcounter_msg_timer = 0 ;
	}
	if( EF12_WAIT( ew ) ) EF12_WAIT( ew ) -- ;
	ef12_type_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Type 0: the first hit								*/
/*==============================================================*/
static void ef12_first( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *ef12_first_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef12_first_wait, ef12_first_in, ef12_first_stay,
	} ;

	ef12_first_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void ef12_first_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int side ;

	if( Sys->scene_no != 2 ) return ;
	if( vs_sys_work.vs_st >= VS_END_WAIT ) return ;
	if( vs_sys_work.vs_st < VS_FIGHT ) return ;
	side = vs_sys_work.first_hit ;
	if( !side ) return ;
	side &= 127 ;
	if( pl_w->pl_no != side ){
		ew->routine_1 = 0 ;
		ew->type = 1 ;
	} else {
		ew->routine_1 ++ ;
		EF12_KIND( ew ) = 0 ;
		ef12_pos_set( ew, pl_w, EF12_KIND( ew ) ) ;
		if( Sys->entry_side & ( 1 << ew->pl_no ) ) na_req( 10 ) ;
		ew->timer0 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ef12_first_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef12_fly( ew, pl_w ) ;
	ef12_first_stay( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ef12_first_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int t ;

	ef12_chr_set( ew, pl_w ) ;
	t = ew->timer0 ++ ;
	if( t <= 150 && t >= 120 ){
		if( ef12_ck_end( ew, pl_w ) && ef12_ck_act( ew, pl_w ) && ef12_ck_hit( ew, pl_w ) && ef12_ck_wait( ew, pl_w ) ){
			ew->routine_1 = 0 ;
			ew->type = 1 ;
			ef12_chr_clr( ew, pl_w ) ;
		}
	}
}

/*==============================================================*/
/*			Type 1: wait for a combo, the round's end, a		*/
/*			recovery, the delay or a low vital					*/
/*==============================================================*/
static void ef12_idle( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	EF12_BTN( ew ) = 0 ;
	if( !ef12_ck_hit( ew, pl_w ) ) return ;
	if( !ef12_ck_end( ew, pl_w ) ) return ;
	if( !ef12_ck_act( ew, pl_w ) ) return ;
	if( !ef12_ck_wait( ew, pl_w ) ) return ;
	ef12_ck_vital( ew, pl_w ) ;
}

/*==============================================================*/
/*			Type 2: a combo is on								*/
/*==============================================================*/
static void ef12_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *ef12_hit_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef12_hit_init, ef12_hit_in, ef12_hit_count, ef12_hit_end,
	} ;

	ef12_hit_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void ef12_hit_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->timer0 = 0 ;
	EF12_FLAG( ew ) = 0 ;
	ef12_hit_pos( ew, pl_w ) ;
	ef12_hit_in( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ef12_hit_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef12_fly( ew, pl_w ) ;
	ef12_hits_get( ew, pl_w ) ;
	ef12_hit_digit( ew, pl_w ) ;
	ef12_hit_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ef12_hit_count( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ef12_hits_get( ew, pl_w ) ){
		ef12_hit_digit( ew, pl_w ) ;
		ef12_hit_wait( ew, pl_w ) ;
	} else {
		ew->routine_1 ++ ;
		ew->timer0 = 0 ;
		ef12_hit_end( ew, pl_w ) ;
	}
}

/*==============================================================*/
/*			The combo is over: a word for its length			*/
/*==============================================================*/
static void ef12_hit_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int t, hits, kind ;

	ef12_hit_digit( ew, pl_w ) ;
	t = ew->timer0 ++ ;
	if( t <= 30 ){
		if( t == 30 ) ef12_ck_end( ew, pl_w ) ;
		return ;
	}
	hits = EF12_HITS( ew ) ;
	if( hits < 3 ){
		ew->routine_1 = 0 ;
		ew->type = 1 ;
		ef12_chr_clr( ew, pl_w ) ;
		return ;
	}
	if( ( EF12_BTN( ew ) &= 1 ) && ( pl_w->combo_hit_type_old & 1 ) ){
		EF12_BTN( ew ) = 0 ;
		EF12_KIND( ew ) = 10 ;
		if( Sys->cpu_level_type ) EF12_KIND( ew ) = 11 ;
		if( hits >= 6 ){
			if( Sys->entry_side & ( 1 << ew->pl_no ) ){
				if( !Sys->cpu_level_type ) na_req( 12 ) ;
				else na_req( 13 ) ;
			}
		}
	} else {
		kind = 13 ;
		if( hits >= 4 ) kind = 4 ;
		if( hits >= 6 ) kind = 5 ;
		if( hits >= 8 ) kind = 6 ;
		if( hits >= 10 ) kind = 7 ;
		if( hits >= 30 ) kind = 8 ;
		if( hits >= 50 ) kind = 14 ;
		if( hits >= 100 ) kind = 9 ;
		EF12_KIND( ew ) = kind ;
	}
	ew->routine_1 = 0 ;
	ew->type = 3 ;
	ef12_bonus_set( ew, pl_w ) ;
	ef12_chr_clr( ew, pl_w ) ;
	if( ( kind = ef12_se_tbl[EF12_KIND( ew ) & 15] ) ) na_req( kind ) ;
}

/*==============================================================*/
static void ef12_hit_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->timer0 >= 30 ) ef12_ck_end( ew, pl_w ) ;
	else ew->timer0 ++ ;
}

/*==============================================================*/
/*			Types 3 and 4: a word flies in, stays and goes		*/
/*==============================================================*/
static void ef12_msg( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *ef12_msg_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef12_msg_init, ef12_msg_in, ef12_msg_stay, ef12_msg_end,
	} ;

	ef12_msg_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void ef12_msg_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->timer0 = 0 ;
	ew->sub_type = 0 ;
	ef12_chr_clr( ew, pl_w ) ;
	ef12_pos_set( ew, pl_w, EF12_KIND( ew ) ) ;
	ef12_msg_in( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ef12_msg_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef12_fly( ew, pl_w ) ;
	ef12_chr_set( ew, pl_w ) ;
	ef12_msg_wait( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ef12_msg_stay( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ef12_hits_get( ew, pl_w ) ;
	ef12_chr_set( ew, pl_w ) ;
	ef12_msg_wait( ew, pl_w ) ;
}

/*==============================================================*/
static void ef12_msg_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int t = ew->timer0 ++ ;

	if( t <= 90 ){
		if( t >= 30 ){
			if( ew->type != 4 ){
				if( !ef12_ck_end( ew, pl_w ) ) return ;
				if( !ef12_ck_act( ew, pl_w ) ) return ;
				if( !ef12_ck_wait( ew, pl_w ) ) return ;
			}
			ef12_ck_hit( ew, pl_w ) ;
		}
	} else {
		ef12_chr_clr( ew, pl_w ) ;
		if( ew->type != 4 ){
			ew->routine_1 = 0 ;
			ew->type = 1 ;
		} else {
			ew->routine_1 ++ ;
			ef12_msg_end( ew, pl_w ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void ef12_msg_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st ){
		ew->routine_1 = 0 ;
		ew->type = 0 ;
		ew->sub_type = 0 ;
	}
}

/*==============================================================*/
/*			Type 5: the round is over							*/
/*==============================================================*/
static void ef12_over( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_1 ){
		ew->routine_1 ++ ;
		ef12_chr_clr( ew, pl_w ) ;
	}
	ef12_msg_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void ef12_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	work_free( ew ) ;
}

/* the checks return 1 to go on, 0 once they have moved the work to another type */

/*==============================================================*/
/*			A combo of two hits or more							*/
/*==============================================================*/
static int ef12_ck_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int n ;

	if( vs_sys_work.vs_st != VS_FIGHT ) return 1 ;
	n = vs_sys_work.combo_total[ew->pl_no] ;
	if( n < 2 ) return 1 ;
	EF12_HITS( ew ) = n ;
	EF12_BTN( ew ) |= pl_w->combo_kind ;
	ew->routine_1 = 0 ;
	ew->timer0 = 0 ;
	ew->type = 2 ;
	return 0 ;
}

/*==============================================================*/
/*			The round's end										*/
/*==============================================================*/
static int ef12_ck_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st != VS_END_WAIT ) return 1 ;
	if( !vs_sys_work.time_over && vs_sys_work.winner_id == pl_w->pl_no && vs_sys_work.ko_hit_data ) ef12_chr_clr( ew, pl_w ) ;
	ef12_chr_clr( ew, pl_w ) ;
	ew->routine_1 = 0 ;
	ew->timer0 = 0 ;
	ew->type = 5 ;
	return 0 ;
}

/*==============================================================*/
/*			The player's act 27									*/
/*==============================================================*/
static int ef12_ck_act( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st != VS_FIGHT ) return 1 ;
	if( pl_w->act_st != N_ESCAPE ) return 1 ;
	if( pl_w->old_act_st == N_ESCAPE ) return 1 ;
	pl_w->old_act_st = pl_w->act_st ;
	EF12_KIND( ew ) = 1 ;
	ew->routine_1 = 0 ;
	ew->timer0 = 0 ;
	ew->type = 3 ;
	if( Sys->entry_side & ( 1 << ew->pl_no ) ) na_req( 11 ) ;
	return 0 ;
}

/*==============================================================*/
/*			The delay the player's unk_1ee sets has run out		*/
/*==============================================================*/
static int ef12_ck_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( vs_sys_work.vs_st != VS_FIGHT ) return 1 ;
	if( !EF12_WAIT( ew ) ) return 1 ;
	EF12_WAIT( ew ) = 0 ;
	EF12_KIND( ew ) = 2 ;
	ew->routine_1 = 0 ;
	ew->timer0 = 0 ;
	ew->type = 3 ;
	na_req( 14 ) ;
	return 0 ;
}

/*==============================================================*/
/*			A vital of 32 or less, once for each player			*/
/*==============================================================*/
static int ef12_ck_vital( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int bit ;

	if( vs_sys_work.vs_st != VS_FIGHT ) return 1 ;
	bit = 1 << ( pl_w->work_no >> 1 ) ;
	if( EF12_MARK( ew ) & bit ) return 1 ;
	if( pl_w->vital_new > 32 ) return 1 ;
	EF12_MARK( ew ) |= bit ;
	EF12_KIND( ew ) = 12 ;
	ew->routine_1 = 0 ;
	ew->timer0 = 0 ;
	ew->type = 3 ;
	na_req( 15 ) ;
	return 0 ;
}

/*==============================================================*/
/*			The combo's hits so far: 1 while it goes on, 0		*/
/*			once it has ended									*/
/*==============================================================*/
static int ef12_hits_get( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int n ;

	if( EF12_FLAG( ew ) ) return 0 ;
	EF12_BTN( ew ) |= pl_w->combo_kind ;
	n = vs_sys_work.combo_total[ew->pl_no] ;
	if( n <= 1 ){
		if( n == 0 ) EF12_HITS( ew ) = EF12_HITS_OLD( ew ) ;
		EF12_FLAG( ew ) = 1 ;
		return 0 ;
	}
	EF12_HITS( ew ) = n ;
	return 1 ;
}

/*==============================================================*/
/*			The word's x, a quarter of the way to its place		*/
/*			each frame											*/
/*==============================================================*/
static void ef12_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float k = 4.0f, d ;

	d = ew->acc.z - ew->pos.x ;
	if( !d ){
		ew->routine_1 ++ ;
		d = ( ew->acc.z - ew->pos.x ) * k ;
	}
	d = d / k ;
	if( d < 0.03125f && d > -0.03125f ) d = ew->acc.z - ew->pos.x ;
	ew->pos.x += d ;
}

/*==============================================================*/
static void ef12_chr_clr( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->work2 = 0 ;
	ew->work3 = 0 ;
}

/*==============================================================*/
/*			Where the word starts, off the edge of the			*/
/*			screen, and stops									*/
/*==============================================================*/
static void ef12_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, int kind )
{
	Float x = ef12_x_tbl[kind & 15] ;
	Float az, px ;

	px = 40.0f ;
	az = 40.0f - x ;
	if( !pl_w->pl_no ){
		px = -40.0f - x ;
		az = -40.0f ;
	}
	ew->pos.x = px ;
	ew->acc.z = az ;
	ew->pos.y = 5.0f ;
	ef12_chr_set( ew, pl_w ) ;
}

/*==============================================================*/
/*			The combo's bonus, in eight digits, and where		*/
/*			its number starts									*/
/*==============================================================*/
static void ef12_bonus_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	char buf[12] ;
	Sint8 *p ;
	Uint32 blank, i ;
	Float y ;

	sprintf( buf, "%8x", vs_sys_work.combo_pts[pl_w->pl_no] ) ;
	p = EF12_NUM( ew )->num.bonus ;
	blank = 0 ;
	for( i = 0 ; i < 8 ; i ++, p ++ ){
		if( ( *p = buf[i] - '0' ) < 0 ) blank ++ ;
	}
	y = -( blank * 1.75f ) ;
	if( pl_w->pl_no ) y = ef12_x_tbl[EF12_KIND( ew ) & 15] - 24.0f ;
	ew->spd.z = y ;
}

/*==============================================================*/
static void ef12_chr_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->work2 = ( Sint32 )ef12_chr_tbl[EF12_KIND( ew ) & 15] ;
	if( ef12_se_tbl[EF12_KIND( ew ) & 15] ){
		if( !Sys->vs_flag && !pl_w->cpu_flag ) ew->work3 = ( Sint32 )ef12_bonus_chr ;
		else ew->work3 = 0 ;
	}
}

/*==============================================================*/
static void ef12_hit_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x = 40.0f, y = 16.0f ;

	if( !pl_w->pl_no ){
		x = -64.0f ;
		y = -40.0f ;
	}
	ew->pos.x = x ;
	ew->acc.z = y ;
	ew->pos.y = 5.0f ;
}

/*==============================================================*/
static void ef12_hit_digit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	char buf[4] ;
	Uint8 *p ;
	int i ;

	sprintf( buf, "%3d", EF12_HITS( ew ) ) ;
	p = EF12_NUM( ew )->num.hit ;
	for( i = 0 ; i < 3 ; i ++, p ++ ){
		*p = buf[i] - '0' ;
		if( *p < 0 ) *p = 0xff ;
	}
	ew->work2 = ( Sint32 )ef12_hit_chr ;
	ew->work3 = 0 ;
}

/****************************************************************/
/*			Draws the words, in the 2D camera, the fixed		*/
/*			objects and the digits each in its own place		*/
/****************************************************************/
void ef12_disp( PLAYER_WORK *ew )
{
	EF12_CHR *chr ;
	Sint8 *digit ;
	Sint8 i ;

	if( !ew->disp_flag ) return ;
	chr = EF12_CHR0( ew ) ;
	if( !chr ) return ;
	nlPushMatrix( 0 ) ;
	nlUnitMatrix() ;
	nlTranslateV( &ew->pos ) ;
	nlGetMatrix( &ew->obj_mtx ) ;
	nlPopMatrix( 1 ) ;
	scr_2d_matrix_set() ;
	nlPushMatrix( 0 ) ;
	for( i = 0 ; i < 2 ; i ++ ){
		nlLoadMatrix( &ew->obj_mtx ) ;
		if( i ){
			chr = EF12_CHR1( ew ) ;
			if( !chr ) break ;
			nlTranslate( ew->spd.z, -6.0f, 0.0f ) ;
		}
		digit = EF12_NUM( ew )->num.bonus ;
		for( ; chr->col >= 0 ; chr ++ ){
			nlTranslate( chr->x, 0.0f, 0.0f ) ;
			if( chr->scl ){
				nlScaleV( ef12_scale_p[chr->scl & 7] ) ;
				nlSetScaleFactor( obj_scale_max( ef12_scale_p[chr->scl & 7] ) ) ;
			}
			nlObjPutSetFadeColor( 1.0f, 1.0f, 1.0f ) ;
			if( chr->slot >= 0 ){
				if( *digit >= 0 ){
					if( chr->col > 0 ) obj_col_set( obj_dm00->obj[*digit], chr->col - 1 ) ;
					nlObjPut( obj_dm00->obj[*digit] ) ;
				}
				digit ++ ;
			} else {
				if( chr->col > 0 ) obj_col_set( obj_dm00->obj[chr->obj], chr->col - 1 ) ;
				nlObjPut( obj_dm00->obj[chr->obj] ) ;
			}
		}
	}
	nlPopMatrix( 1 ) ;
}

/****************************************************************/
/*			A debug print of a place, cut off by its return		*/
/*			(what it printed is a guess)						*/
/****************************************************************/
void ef12_dummy( VEC3 *pos )
{
	char buf[32] ;

	return ;
	sprintf( buf, "%f", pos->x ) ;
	sprintf( buf, "X : %f", pos->x ) ;
	sprintf( buf, "Y : %f", pos->y ) ;
	sprintf( buf, "Z : %f", pos->z ) ;
}

static EF12_CHR ef12_alt0_chr[] = {
	{ 3, -1, 0, 31, -24.5f }, { 3, -1, 0, 18, 2.5f }, { 3, -1, 0, 29, 2.5f }, { 3, -1, 0, 10, 2.75f },
	{ 3, -1, 0, 21, 3.0f }, { 2, 1, 3, 0, 24.75f }, { 2, 1, 0, 0, 3.5f }, { 2, 1, 0, 0, 3.5f },
	{ 2, 1, 0, 0, 3.5f }, { 2, 1, 0, 0, 3.5f }, { 2, 1, 0, 0, 3.5f }, { 2, 1, 0, 0, 3.5f },
	{ 2, 1, 0, 0, 3.5f }, { 2, 1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_alt1_chr[] = {
	{ 3, -1, 0, 29, -24.5f }, { 3, -1, 0, 18, 2.75f }, { 3, -1, 0, 22, 3.25f }, { 3, -1, 0, 14, 3.75f },
	{ 2, -1, 3, 0, 25.75f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_alt2_chr[] = {
	{ 3, -1, 0, 12, -24.5f }, { 3, -1, 0, 24, 3.5f }, { 3, -1, 0, 22, 4.0f }, { 3, -1, 0, 25, 4.0f },
	{ 3, -1, 0, 21, 3.0f }, { 3, -1, 0, 14, 2.75f }, { 3, -1, 0, 29, 3.0f }, { 3, -1, 0, 14, 3.0f },
	{ 2, -1, 3, 0, 12.25f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_alt3_chr[] = {
	{ 3, -1, 0, 29, -24.5f }, { 3, -1, 0, 24, 3.5f }, { 3, -1, 0, 29, 3.5f }, { 3, -1, 0, 10, 3.0f },
	{ 3, -1, 0, 21, 3.25f }, { 2, -1, 3, 0, 22.25f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f },
	{ 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_alt4_chr[] = {
	{ 3, -1, 0, 32, -12.0f }, { 3, -1, 0, 18, 4.0f }, { 3, -1, 0, 23, 2.75f }, { 3, -1, 0, 28, 3.5f },
	{ 2, -1, 3, 0, 17.25f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_alt5_chr[] = {
	{ 3, -1, 0, 22, -27.0f }, { 3, -1, 0, 10, 3.75f }, { 3, -1, 0, 33, 3.0f }, { 3, -1, 0, 12, 4.5f },
	{ 3, -1, 0, 24, 3.25f }, { 3, -1, 0, 22, 3.75f }, { 3, -1, 0, 11, 3.75f }, { 3, -1, 0, 24, 3.25f },
	{ 2, -1, 3, 0, 17.25f }, { 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR ef12_alt6_chr[] = {
	{ 3, -1, 0, 11, -24.0f }, { 3, -1, 0, 14, 3.0f }, { 3, -1, 0, 28, 3.0f }, { 3, -1, 0, 29, 3.25f },
	{ 3, -1, 0, 29, 4.25f }, { 3, -1, 0, 18, 2.5f }, { 3, -1, 0, 22, 3.0f }, { 3, -1, 0, 14, 3.5f },
	{ 2, -1, 0, 39, 18.0f }, { 2, -1, 0, 39, 5.25f }, { 2, -1, 3, 0, -8.0f }, { 2, -1, 0, 0, 7.0f },
	{ 2, -1, 0, 0, 3.5f }, { 2, -1, 0, 0, 7.0f }, { 2, -1, 0, 0, 3.5f }, { -1, -1, 0, 0, 0.0f },
} ;

static EF12_CHR *ef12_alt_tbl[16] = {
	ef12_alt0_chr, ef12_alt1_chr, ef12_alt2_chr, ef12_alt3_chr,
	ef12_alt4_chr, ef12_alt5_chr, ef12_alt6_chr, ef12_alt0_chr,
	ef12_alt0_chr, ef12_alt0_chr, ef12_alt0_chr, ef12_alt0_chr,
	ef12_alt0_chr, ef12_alt0_chr, ef12_alt0_chr, ef12_alt0_chr,
} ;
static EF12_CHR *ef12_zero_tbl[16] = { 0 } ;

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef12.c" End of File	*/
