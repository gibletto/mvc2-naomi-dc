/* Omega Red's effect 03 (character 30's shot 03): the carbon coil of his Omega Strike. One
   piece (type 0) keeps a chain of parts (type 1, with a tip of type 2) that stretches out from his
   hand and winds back. Its parts work like those of shl30_01's coil. */
#include "player.h"
#include "pl_char.h"
#include "pl30_def.h"

/* the effect's own work, in obj_mtx */
typedef struct SHL30_03_OFS {
	Float	x;
	Float	y;
} SHL30_03_OFS;

typedef struct SHL30_03_LOCAL_W {
	Sint16	rect[4];	/* 00: the size of a part, from shl30_01_circle: two offsets and two joins */
	Sint16	unk_08;		/* 08 */
	Sint16	unk_0a;		/* 0a */
	SHL30_03_OFS	ofs[3];	/* 0c: where the first two parts and the end stand from the head */
	PLAYER_WORK	*tip_w;		/* 24: the last part made, at the end of the chain */
	Sint8	cnt;		/* 28: the parts beyond the first three */
	Sint8	rest_cnt;		/* 29: parts left to place this frame */
	Sint8	jump_dir;		/* 2a: the player's jump_dir */
} SHL30_03_LOCAL_W;
#define SHL30_03_LW( w )	( ( SHL30_03_LOCAL_W * )&( w )->obj_mtx )

/* a part made from the player's, as the parts of his other effects are */
#define SHL30_03_PART_SET( d, s ) \
	CHAR_PART_COPY( d, s ) ; \
	( d )->disp_flag = 1 ; \
	( d )->pl_no = ( s )->pl_no ; \
	( d )->pl_type = ( s )->pl_type ; \
	( d )->scale.x = ( s )->scale.x ; \
	( d )->scale.y = ( s )->scale.y ; \
	( d )->atck_level = ( s )->atck_level ; \
	( d )->work_no = ( s )->work_no ; \
	( d )->col_cnt = ( s )->col_cnt ; \
	( d )->scale = ( s )->scale ; \
	( d )->prio = ( s )->prio

extern Sint16 shl30_01_circle[][4];

/* where the first parts stand by his moves' step: for each side, x and y of the two and the end */
static Sint16 shl30_03_ofs_tbl[2][3][6] = {
	{ { -55, 57, -55, 82, 72, 82 }, { -66, 44, -46, 64, 61, 72 }, { -14, 67, 17, 67, 17, 35 } },
	{ { -55, 58, -55, 81, 72, 81 }, { -65, 45, -47, 63, 60, 71 }, { -13, 67, 16, 67, 16, 35 } },
} ;
static CHAR_DATA shl30_03_char = { 0, { 0 }, 0x2e } ;

static void shl30_03_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl30_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) != 0 ){
		ew->work_func = shl30_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x3003 ;
		ew->type = 0 ;
	}
	return ew ;
}

static void shl30_03_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_end( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_free( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );

/*==============================================================*/
static void shl30_03_main( PLAYER_WORK *ew )
{
	static void ( *shl30_03_jmp[] )( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw ) = {
		shl30_03_init, shl30_03_move, shl30_03_end, shl30_03_free,
	} ;

	ew->prio = ew->master_w->prio ;
	if( !ew->type || !ew->timer0 ){
		ew->disp_flag = 0 ;
		shl30_03_jmp[ew->routine_0]( ew, SHL30_03_LW( ew ) ) ;
	}
}

static void shl30_03_head_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_part_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_tip_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );

/*==============================================================*/
static void shl30_03_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	static void ( *shl30_03_init_jmp[] )( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw ) = {
		shl30_03_head_init, shl30_03_part_init, shl30_03_tip_init,
	} ;

	PLAYER_WORK *m_w = ew->master_w ;

	lw->jump_dir = m_w->jump_dir ;
	ew->dir = m_w->dir ;
	shl30_03_init_jmp[ew->type]( ew, lw ) ;
}

static void shl30_03_ofs_set( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw, PLAYER_WORK *m_w );
static void shl30_03_head_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );

/*==============================================================*/
/*			The head: it makes the end part and the first		*/
/*			two parts											*/
/*==============================================================*/
static void shl30_03_head_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *m_w = ew->master_w ;
	PLAYER_WORK *nw ;
	SHL30_03_LOCAL_W *nlw ;
	Uint8 no ;
	int i ;

	SHL30_03_PART_SET( ew, m_w ) ;
	ew->pos = m_w->pos ;
	if( m_w->disp_dir ) ew->dir = -ew->dir ;
	shl30_03_ofs_set( ew, lw, m_w ) ;
	ew->spd.x = ew->spd.y = 0.0f ;
	ew->sub_prio = -1 ;
	lw->unk_08 = 1 ;
	lw->cnt = 0 ;
	if( work_rest_cnt < 3 ){
		shl30_03_free( ew, lw ) ;
		return ;
	}
	nw = work_get( ew, W_SHL, LIST_BEHIND ) ;
	nw->work_func = shl30_03_main ;
	nw->type = 2 ;
	nw->master_w = m_w ;
	nw->ef_w = ew ;
	SHL30_03_PART_SET( nw, m_w ) ;
	nw->sub_prio = -2 ;
	nlw = SHL30_03_LW( nw ) ;
	nlw->jump_dir = lw->jump_dir ;
	nw->routine_0 = 1 ;
	lw->tip_w = nw ;
	for( i = 2 ; ; ){
		nw = work_get( ew, W_SHL, LIST_BEHIND ) ;
		nw->work_func = shl30_03_main ;
		nw->type = 1 ;
		nw->master_w = m_w ;
		nw->ef_w = ew ;
		SHL30_03_PART_SET( nw, m_w ) ;
		nw->dir = ew->dir ;
		ef_char_set2( nw, 27, 1, no = ( ( nw->dir + 4 ) & 0xf8 ) / 8 ) ;
		nw->cg_adrs = ( Sint8 * )&shl30_03_char ;
		nw->frame_box = nw->frame_box_tbl + ( ( CHAR_DATA * )ew->cg_adrs )->frame_box_no ;
		nw->prio = 0 ;
		nw->sub_prio = 0 ;
		nw->sub_prio = -1 ;
		nlw = SHL30_03_LW( nw ) ;
		nlw->rect[0] = DOT_X( shl30_01_circle[no][0] ) ;
		nlw->rect[1] = shl30_01_circle[no][1] * DOT_Y( 1.0f ) ;
		nlw->rect[2] = DOT_X( shl30_01_circle[no][2] ) ;
		nlw->rect[3] = shl30_01_circle[no][3] * DOT_Y( 1.0f ) ;
		nlw->jump_dir = lw->jump_dir ;
		nw->disp_dir = 0 ;
		nw->routine_0 = 1 ;
		if( -- i == 0 ) break ;
	}
	ew->routine_0 ++ ;
	shl30_03_head_move( ew, lw ) ;
}

/*==============================================================*/
static void shl30_03_part_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	ew->disp_dir = 0 ;
	ew->routine_0 ++ ;
}

/*==============================================================*/
static void shl30_03_tip_init( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	ew->routine_0 ++ ;
}

static void shl30_03_part_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_tip_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void shl30_03_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	static void ( *shl30_03_move_jmp[] )( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw ) = {
		shl30_03_head_move, shl30_03_part_move, shl30_03_tip_move,
	} ;
	PLAYER_WORK *w ;
	int i ;

	if( ew->type == 0 ){
		w = ew ;
		for( i = 0 ; i < lw->cnt + 3 ; i ++ ){
			w = w->behind_w ;
			if( w->work_func != shl30_03_main ) break ;
			w->timer0 = 0 ;
			w->disp_flag = 0 ;
		}
	}
	shl30_03_move_jmp[ew->type]( ew, lw ) ;
}

static void shl30_03_head_out( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_head_wind( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );

/*==============================================================*/
/*			The head while his act holds: the chain				*/
/*			stretches out, or winds back						*/
/*==============================================================*/
static void shl30_03_head_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *m_w ;

	ew->disp_flag = 0 ;
	m_w = ew->master_w ;
	lw->rest_cnt = lw->cnt ;
	if( m_w->routine_1 != PL_NORM || m_w->act_st != N_SPECIAL ){
		shl30_03_end( ew, lw ) ;
		return ;
	}
	if( m_w->hit_stop || ( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) ){
		ew->disp_flag = 1 ;
		ew = ew->behind_w ;
		ew->disp_flag = 1 ;
		ew->timer0 = 1 ;
		ew = ew->behind_w ;
		ew->disp_flag = 1 ;
		ew->timer0 = 1 ;
		return ;
	}
	if( ew->routine_1 == 0 ){
		shl30_03_head_out( ew, lw ) ;
	} else {
		shl30_03_head_wind( ew, lw ) ;
	}
}

static void shl30_03_head_pull( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_part_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_grow( PLAYER_WORK *ew, PLAYER_WORK *tail_w, PLAYER_WORK *m_w );

/*==============================================================*/
static void shl30_03_head_out( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *m_w = ew->master_w ;
	PLAYER_WORK *w1 ;
	PLAYER_WORK *w2 ;
	PLAYER_WORK *w3 ;

	if( m_w->routine_2 == 4 ){
		shl30_03_end( ew, lw ) ;
		return ;
	}
	if( m_w->routine_2 == 6 ){
		shl30_03_head_pull( ew, lw ) ;
		return ;
	}
	shl30_03_ofs_set( ew, lw, m_w ) ;
	ew->disp_flag = 1 ;
	w1 = ew->behind_w ;
	w1->pos.x = ew->pos.x + lw->ofs[0].x ;
	w1->pos.y = ew->pos.y + lw->ofs[0].y ;
	w1->disp_flag = 1 ;
	w1->timer0 = 1 ;
	w2 = w1->behind_w ;
	w2->pos.x = ew->pos.x + lw->ofs[1].x ;
	w2->pos.y = ew->pos.y + lw->ofs[1].y ;
	w2->disp_flag = 1 ;
	w2->timer0 = 1 ;
	if( ( lw->rest_cnt = lw->cnt ) != 0 ){
		w3 = w2->behind_w ;
		shl30_03_part_move( w3, SHL30_03_LW( w3 ) ) ;
		return ;
	}
	m_w->pos.x = w2->pos.x - lw->ofs[2].x ;
	m_w->pos.y = w2->pos.y - lw->ofs[2].y ;
	shl30_03_grow( ew, w2, m_w ) ;
}

/*==============================================================*/
/*			His act changed: every part goes on to the			*/
/*			winding back										*/
/*==============================================================*/
static void shl30_03_head_pull( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *w = ew ;
	int i ;

	for( i = 0 ; i < lw->cnt + 4 ; i ++ ){
		w->routine_1 ++ ;
		w = w->behind_w ;
	}
	shl30_03_head_wind( ew, lw ) ;
}

/*==============================================================*/
static void shl30_03_head_wind( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *m_w = ew->master_w ;
	PLAYER_WORK *w1 ;
	PLAYER_WORK *w2 ;
	PLAYER_WORK *w3 ;

	shl30_03_ofs_set( ew, lw, m_w ) ;
	ew->disp_flag = 1 ;
	w1 = ew->behind_w ;
	w1->pos.x = ew->pos.x + lw->ofs[0].x ;
	w1->pos.y = ew->pos.y + lw->ofs[0].y ;
	w1->disp_flag = 1 ;
	w1->timer0 = 1 ;
	w2 = w1->behind_w ;
	w2->pos.x = ew->pos.x + lw->ofs[1].x ;
	w2->pos.y = ew->pos.y + lw->ofs[1].y ;
	w2->disp_flag = 1 ;
	w2->timer0 = 1 ;
	if( ( lw->rest_cnt = lw->cnt ) > 2 ){
		w3 = w2->behind_w ;
		shl30_03_part_move( w3, SHL30_03_LW( w3 ) ) ;
		return ;
	}
	m_w->pos.x = w2->pos.x - lw->ofs[2].x ;
	m_w->pos.y = w2->pos.y - lw->ofs[2].y ;
	PL30_LW( m_w )->pull_flag = 1 ;
	ew->routine_0 ++ ;
	lw->cnt -= 2 ;
	w3 = w2->behind_w ;
	work_free( w3 ) ;
	work_free( w3->behind_w ) ;
}

static void shl30_03_part_out( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );
static void shl30_03_part_wind( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw );

/*==============================================================*/
/*			A part of the chain									*/
/*==============================================================*/
static void shl30_03_part_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *m_w = ew->master_w ;

	if( m_w->hit_stop || ( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) ){
		ew->disp_flag = 1 ;
		ew->timer0 = 1 ;
		return ;
	}
	if( ew->routine_1 == 0 ){
		shl30_03_part_out( ew, lw ) ;
	} else {
		shl30_03_part_wind( ew, lw ) ;
	}
}

/*==============================================================*/
/*			Each part sits at the join of the two ahead of		*/
/*			it, and they go on down the chain					*/
/*==============================================================*/
static void shl30_03_part_out( PLAYER_WORK *w, SHL30_03_LOCAL_W *wlw )
{
	PLAYER_WORK *ew ;
	SHL30_03_LOCAL_W *lw ;
	PLAYER_WORK *head_w ;
	PLAYER_WORK *p1 ;
	PLAYER_WORK *p2 ;
	PLAYER_WORK *p3 ;
	PLAYER_WORK *p4 ;
	PLAYER_WORK *m_w ;
	SHL30_03_LOCAL_W *p1lw ;
	SHL30_03_LOCAL_W *p2lw ;
	SHL30_03_LOCAL_W *p3lw ;
	SHL30_03_LOCAL_W *olw ;

	m_w = w->master_w ;
	ew = w ;
	lw = wlw ;
	p1 = ew->before_w ;
	p2 = p1->before_w ;
	p2lw = SHL30_03_LW( p2 ) ;
	head_w = ew->ef_w ;
	olw = SHL30_03_LW( head_w ) ;
	p1lw = SHL30_03_LW( p1 ) ;
	ew->pos.x = p2->pos.x + p2lw->rect[0] - lw->rect[2] ;
	ew->pos.y = p2->pos.y - p2lw->rect[1] + lw->rect[3] ;
	ew->disp_flag = 1 ;
	ew->timer0 = 1 ;
	p3 = ew->behind_w ;
	p3lw = SHL30_03_LW( p3 ) ;
	p3->pos.x = p1->pos.x + p1lw->rect[0] - p3lw->rect[2] ;
	p3->pos.y = p1->pos.y - p1lw->rect[1] + p3lw->rect[3] ;
	p3->disp_flag = 1 ;
	p3->timer0 = 1 ;
	if( ( olw->rest_cnt -= 2 ) > 0 ){
		p4 = p3->behind_w ;
		shl30_03_part_out( p4, SHL30_03_LW( p4 ) ) ;
	} else if( m_w->hit_stop == 0 ){
		m_w->pos.x = p3->pos.x - olw->ofs[2].x ;
		m_w->pos.y = p3->pos.y - olw->ofs[2].y ;
		if( ( olw->unk_08 = -olw->unk_08 ) >= 0 ){
			m_w->pos.x += p3lw->rect[0] ;
			m_w->pos.y -= p3lw->rect[1] ;
			shl30_03_grow( head_w, p3, m_w ) ;
		}
	}
}

/*==============================================================*/
/*			Winding back: the same down the chain, and the		*/
/*			last two parts are freed							*/
/*==============================================================*/
static void shl30_03_part_wind( PLAYER_WORK *ew, SHL30_03_LOCAL_W *wlw )
{
	SHL30_03_LOCAL_W *lw ;
	PLAYER_WORK *p1 ;
	PLAYER_WORK *p2 ;
	PLAYER_WORK *p3 ;
	PLAYER_WORK *p4 ;
	PLAYER_WORK *m_w ;
	SHL30_03_LOCAL_W *p1lw ;
	SHL30_03_LOCAL_W *p2lw ;
	SHL30_03_LOCAL_W *p3lw ;
	SHL30_03_LOCAL_W *olw ;

	p1 = ew->before_w ;
	p2 = p1->before_w ;
	m_w = ew->master_w ;
	lw = wlw ;
	olw = SHL30_03_LW( ew->ef_w ) ;
	p1lw = SHL30_03_LW( p1 ) ;
	p2lw = SHL30_03_LW( p2 ) ;
	ew->pos.x = p2->pos.x + p2lw->rect[0] - lw->rect[2] ;
	ew->pos.y = p2->pos.y - p2lw->rect[1] + lw->rect[3] ;
	ew->disp_flag = 1 ;
	ew->timer0 = 1 ;
	p3 = ew->behind_w ;
	p3lw = SHL30_03_LW( p3 ) ;
	p3->pos.x = p1->pos.x + p1lw->rect[0] - p3lw->rect[2] ;
	p3->pos.y = p1->pos.y - p1lw->rect[1] + p3lw->rect[3] ;
	p3->disp_flag = 1 ;
	p3->timer0 = 1 ;
	olw->rest_cnt -= 2 ;
	if( olw->rest_cnt != 2 ){
		p4 = p3->behind_w ;
		shl30_03_part_wind( p4, SHL30_03_LW( p4 ) ) ;
		return ;
	}
	if( !m_w->hit_stop && !( vs_sys_work.slow_bit & ( 1 << vs_sys_work.slow_step ) ) ){
		m_w->pos.x = p3->pos.x - olw->ofs[2].x ;
		m_w->pos.y = p3->pos.y - olw->ofs[2].y ;
		olw->cnt -= 2 ;
		p4 = p3->behind_w ;
		work_free( p4 ) ;
		work_free( p4->behind_w ) ;
	}
}

/*==============================================================*/
/*			The end part: it stands on his hand and takes		*/
/*			the picture of the step								*/
/*==============================================================*/
static void shl30_03_tip_move( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	static Uint8 shl30_03_no_a[3] = { 6, 5, 4 } ;
	PLAYER_WORK *m_w = ew->master_w ;
	int flag ;

	if( m_w->cg_type >= 0 ) flag = 0 ;
	else flag = 1 ;
	ew->pos = m_w->pos ;
	ef_char_set2( ew, 27, shl30_03_no_a[PL30_LW( m_w )->level], flag ) ;
	ew->disp_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void shl30_03_end( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	PLAYER_WORK *w = lw->tip_w ;
	PLAYER_WORK *next ;
	int n = lw->cnt + 3 ;
	int i ;

	for( i = 0 ; i < n ; i ++ ){
		next = w->before_w ;
		work_free( w ) ;
		w = next ;
	}
	shl30_03_free( ew, lw ) ;
}

/*--------------------------------------------------------------*/
static void shl30_03_free( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw )
{
	work_free( ew ) ;
}

/* the picture of each step, by the player's level */
static Uint8 shl30_03_no_b[3] = { 9, 8, 7 } ;

/*==============================================================*/
static void shl30_03_ofs_set( PLAYER_WORK *ew, SHL30_03_LOCAL_W *lw, PLAYER_WORK *m_w )
{
	PL30_LOCAL_W *plw = PL30_LW( m_w ) ;
	Sint16 *p ;
	Uint8 flag = 0 ;
	Float v[4] ;

	if( m_w->cg_type < 0 ) flag = 1 ;
	p = shl30_03_ofs_tbl[flag][plw->level] ;
	lw->ofs[0].y = p[1] * DOT_Y( 1.0f ) ;
	lw->ofs[1].y = p[3] * DOT_Y( 1.0f ) ;
	lw->ofs[2].y = p[5] * DOT_Y( 1.0f ) ;
	v[0] = DOT_X( p[0] ) ;
	v[1] = DOT_X( p[2] ) ;
	v[2] = DOT_X( p[4] ) ;
	if( m_w->disp_dir ){
		v[0] = -v[0] ;
		v[1] = -v[1] ;
		v[2] = -v[2] ;
	}
	lw->ofs[0].x = v[0] ;
	lw->ofs[1].x = v[1] ;
	lw->ofs[2].x = v[2] ;
	ef_char_set2( ew, 27, shl30_03_no_b[plw->level], flag ) ;
}

/*==============================================================*/
/*			Two more parts behind the last, like it				*/
/*==============================================================*/
static void shl30_03_grow( PLAYER_WORK *ew, PLAYER_WORK *tail_w, PLAYER_WORK *m_w )
{
	SHL30_03_LOCAL_W *tlw ;
	SHL30_03_LOCAL_W *nlw ;
	PLAYER_WORK *nw ;
	int i ;

	if( work_rest_cnt >= 2 ){
		tlw = SHL30_03_LW( tail_w ) ;
		for( i = 2 ; i > 0 ; i -- ){
			nw = work_get( tail_w, W_SHL, LIST_BEHIND ) ;
			nlw = SHL30_03_LW( nw ) ;
			nw->work_func = shl30_03_main ;
			nw->type = 1 ;
			nw->master_w = m_w ;
			nw->ef_w = ew ;
			nw->dir = tail_w->dir ;
			SHL30_03_PART_SET( nw, tail_w ) ;
			nw->sub_prio = -1 ;
			nlw->rect[0] = tlw->rect[0] ;
			nlw->rect[1] = tlw->rect[1] ;
			nlw->rect[2] = tlw->rect[2] ;
			nlw->rect[3] = tlw->rect[3] ;
		}
		nlw = SHL30_03_LW( ew ) ;
		nlw->cnt += 2 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl30_03.c" End of File	*/
