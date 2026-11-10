/* Cyclops's effect 02 (character 06's shot 02): the beam of his Mega Optic Blast. Works of three
   kinds ride on his beam pose: the controller (type 0) that puts out a chain of pieces (type 1,
   sub_type 1) and their flashes (sub_type 2), and the hit check of the pieces. */
#include "player.h"
#include "pl06_def.h"

extern void *shl06_01_set2( PLAYER_WORK *pl_w, Sint8 no );

/* an effect's own work, kept in obj_mtx */
typedef struct SHL06_02_LOCAL_W {
	Sint8	beam_st;			/* 00: the animation to show, or 0 to pick by upper_step and lower_step */
	Uint8	long_flag;			/* 01: 0xff on the pieces that show the long beam */
	Uint8	attack_flag;			/* 02: 0xff on the pieces that attack, the first and every sixth */
	Sint8	attack_cnt;			/* 03: pieces to go before the next one that attacks */
	struct PLAYER_WORK *parent;	/* 04: the work that made this one */
	Uint16	hit_no;			/* 08: the piece that has been hit */
	Uint16	upper_step;			/* 0a: where the hit box is: 0 to 4 and 10 to 14 */
	Uint16	lower_step;			/* 0c: the same for the lower box */
	Sint8	hit_timer;			/* 0e: frames the hit box is kept */
	Uint8	unk_0f;			/* 0f */
	Uint8	unk_10;			/* 10 */
	Uint8	unk_11;			/* 11 */
} SHL06_02_LOCAL_W;
#define SHL06_02_LW( w )	( ( SHL06_02_LOCAL_W * )&( w )->obj_mtx )

/* the animation of each step of the beam, for the two flashes and the long beam */
static Sint8 shl06_02_char_tbl[40] = {
	5, 4, 3, 2, 1, -1, -1, -1, -1, -1, -1, 9, 8, 7, 6, 5, 5, 5, 5, 5,
	24, 23, 22, 21, 20, -1, -1, -1, -1, -1, -1, 28, 27, 26, 25, 24, 24, 24, 24, 24,
} ;

static Sint8 shl06_02_long_tbl[40] = {
	14, 13, 12, 11, 10, -1, -1, -1, -1, -1, -1, 18, 17, 16, 15, 14, 14, 14, 14, 14,
	33, 32, 31, 30, 29, -1, -1, -1, -1, -1, -1, 37, 36, 35, 34, 33, 33, 33, 33, 33,
} ;

static Uint8 shl06_02_unk_tbl[16] = {
	0, 2, 2, 3, 3, 4, 4, 132, 132, 131, 131, 130, 130, 4, 0, 0,
} ;

static void shl06_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl06_02_set( PLAYER_WORK *pl_w, Sint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl06_02_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = 0 ;
		ew->ef_no = 0x602 ;
	}
	return ew ;
}

static void shl06_02_init( PLAYER_WORK *ew );
static void shl06_02_move( PLAYER_WORK *ew );
static void shl06_02_end( PLAYER_WORK *ew );
static void shl06_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl06_02_main( PLAYER_WORK *ew )
{
	static void ( *shl06_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl06_02_init, shl06_02_move, shl06_02_end, shl06_02_free,
	} ;

	shl06_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl06_02_init( PLAYER_WORK *ew )
{
	SHL06_02_LOCAL_W *lw = SHL06_02_LW( ew ) ;
	SHL06_02_LOCAL_W *nlw ;
	PLAYER_WORK *nw ;
	PLAYER_WORK *nw2 ;
	PLAYER_WORK *nw3 ;
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	if( !ew->type ){
		lw->attack_flag = 0 ;
		lw->long_flag = 0 ;
		lw->hit_no = 0 ;
		lw->hit_timer = 0 ;
		lw->attack_cnt = 2 ;
		lw->unk_10 = lw->unk_11 = 1 ;
		if( ( nw = work_get( 0, W_SHL, LIST_END ) ) ){
			nw->work_func = ew->work_func ;
			nw->master_w = ew->master_w ;
			nw->ef_w = ew ;
			nw->type = 1 ;
			nw->sub_type = 2 ;
		}
		if( ( nw2 = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			nw2->work_func = ew->work_func ;
			nlw = SHL06_02_LW( nw2 ) ;
			nw2->master_w = ew->master_w ;
			nw2->sub_type = nw2->type = 1 ;
			nlw->long_flag = nlw->attack_flag = 0xff ;
			nlw->parent = ew ;
			if( ( nw3 = work_get( 0, W_SHL, LIST_END ) ) ){
				nw3->work_func = ew->work_func ;
				nw3->master_w = ew->master_w ;
				nw3->ef_w = nw2 ;
				nw3->type = 1 ;
				nw3->sub_type = 2 ;
			}
		}
	}
	pl_w = ew->master_w ;
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
	ew->sub_prio = -8 ;
	ew->dir = pl_w->disp_dir ? 4 : 12 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	lw->lower_step = lw->upper_step = 0 ;
	shl06_02_move( ew ) ;
}

static void shl06_02_head_move( PLAYER_WORK *ew );
static void shl06_02_piece_move( PLAYER_WORK *ew );
static void shl06_02_flash_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl06_02_move( PLAYER_WORK *ew )
{
	static void ( *shl06_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl06_02_head_move, shl06_02_piece_move, shl06_02_flash_move,
	} ;

	ew->prio = ew->master_w->prio ;
	shl06_02_move_jmp[ew->sub_type]( ew ) ;
}

/*==============================================================*/
/*			The controller: it puts out a piece each frame		*/
/*			while Cyclops fires									*/
/*==============================================================*/
static void shl06_02_head_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL06_02_LOCAL_W *lw = SHL06_02_LW( ew ) ;
	SHL06_02_LOCAL_W *nlw ;
	PLAYER_WORK *nw ;
	PLAYER_WORK *nw2 ;
	Float ofs ;

	if( pl_w->char_no.w != 0x1600 && pl_w->char_no.w != 0x1608 && pl_w->char_no.w != 0x1609 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ( lw->beam_st = PL06_LW( pl_w )->beam_st ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	if( lw->hit_timer ){
		if( --lw->hit_timer <= 0 ){
			lw->hit_no = 0 ;
			lw->upper_step = 0 ;
			lw->lower_step = 0 ;
		}
	}
	if( ( nw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		nw->work_func = ew->work_func ;
		nlw = SHL06_02_LW( nw ) ;
		nw->master_w = ew->master_w ;
		nw->type = 1 ;
		nw->sub_type = 1 ;
		nlw->attack_flag = 0 ;
		nlw->long_flag = 0 ;
		nlw->upper_step = 0 ;
		nlw->lower_step = 0 ;
		nlw->parent = ew ;
		if( --lw->attack_cnt < 0 ){
			lw->attack_cnt = 5 ;
			nlw->attack_flag = 0xff ;
		}
		if( ( nw2 = work_get( 0, W_SHL, LIST_END ) ) ){
			nw2->work_func = ew->work_func ;
			nw2->master_w = ew->master_w ;
			nw2->ef_w = nw ;
			nw2->sub_type = nw2->type = 1 ;
			nw2->sub_type = 2 ;
		}
	}
	if( --ew->timer0 < 0 ){
		ew->timer0 = 4 ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl_w->atck_no ;
		HIT_RESET( ew ) ;
	}
	ew->pos = pl_w->pos ;
	ofs = -DOT_X( 56.0f ) ;
	if( pl_w->pl_stance == STANCE_AIR ) ofs = -65.0f ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ofs = DOT_Y( 90.0f ) ;
	if( pl_w->pl_stance == STANCE_AIR ) ofs = DOT_Y( 107.0f ) ;
	ew->pos.y += ofs ;
	ew->prio = pl_w->prio ;
	pl_char_set2( ew, 22, 2, ( Sys->game_timer0 & 1 ) * 19 ) ;
	ew->disp_flag = 1 ;
}

static void shl06_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *bw );
static void shl06_02_char_set( PLAYER_WORK *ew, SHL06_02_LOCAL_W *lw );
static void shl06_02_hit_set( PLAYER_WORK *ew, SHL06_02_LOCAL_W *lw );
static void shl06_02_flash_set( PLAYER_WORK *ew );

/*==============================================================*/
/*			A piece of the chain: it follows the one before		*/
/*			it and takes the beam's hits						*/
/*==============================================================*/
static void shl06_02_piece_move( PLAYER_WORK *ew )
{
	SHL06_02_LOCAL_W *lw = SHL06_02_LW( ew ) ;
	PLAYER_WORK *pw ;
	SHL06_02_LOCAL_W *plw ;
	int box ;

	ew->disp_flag = 0 ;
	pw = lw->parent ;
	plw = SHL06_02_LW( pw ) ;
	if( pw->routine_0 != 1 || ew->before_w->routine_0 >= 3 ){
		ew->routine_0 = 2 ;
		return ;
	}
	lw->beam_st = plw->beam_st ;
	if( plw->hit_no >= ew->type ){
		lw->upper_step = lw->lower_step = 0 ;
	}
	else {
		lw->upper_step = plw->upper_step ;
		lw->lower_step = plw->lower_step ;
	}
	shl06_02_pos_set( ew, ew->before_w ) ;
	shl06_02_char_set( ew, lw ) ;
	if( !ew->before_w->type ){
		box = 104 ;
		if( Sys->game_timer0 & 1 ) box = 105 ;
		ew->frame_box = ew->frame_box_tbl + box ;
	}
	ew->type ++ ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		lw->attack_flag = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		ew->hit_flag = 0 ;
		lw->attack_flag = 0 ;
		shl06_02_hit_set( ew, lw ) ;
		shl06_02_flash_set( ew ) ;
		if( plw->hit_no <= ew->type ){
			if( ( Sint8 )( ew->type - 1 ) > 0 ) plw->hit_no = ( Uint16 )( ew->type - 1 ) ;
			else plw->hit_no = 1 ;
			plw->upper_step = lw->upper_step ;
			plw->lower_step = lw->lower_step ;
			plw->hit_timer = 8 ;
		}
	}
	else {
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		if( lw->long_flag == 0 && lw->attack_flag == 0 ) ew->atck_flag = 0 ;
		ew->atck_no = ew->master_w->atck_no ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		hit_work_set( ew ) ;
	}
}

/*==============================================================*/
/*			The flash that rides on a piece						*/
/*==============================================================*/
static void shl06_02_flash_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *fw = ew->ef_w ;
	SHL06_02_LOCAL_W *lw = SHL06_02_LW( ew ) ;
	SHL06_02_LOCAL_W *flw ;

	ew->disp_flag = fw->disp_flag ;
	flw = SHL06_02_LW( fw ) ;
	if( fw->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos = fw->pos ;
	if( fw->type ){
		lw->beam_st = flw->beam_st ;
		lw->long_flag = flw->long_flag ;
		lw->upper_step = flw->upper_step ;
		lw->lower_step = flw->lower_step ;
		shl06_02_char_set( ew, lw ) ;
	}
	else {
		if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) )
			pl_char_set2( ew, 22, 2, ( 1 - ( Sys->game_timer0 & 1 ) ) * 19 ) ;
		ew->disp_flag = 1 ;
	}
}

/*==============================================================*/
static void shl06_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *bw )
{
	Sint16 ofs ;

	if( !bw->type ) ofs = -56 ;
	else ofs = -32 ;
	if( ew->disp_dir ) ofs = -ofs ;
	ew->pos.x = bw->pos.x + DOT_X( ofs ) ;
	ew->pos.y = bw->pos.y ;
}

/*==============================================================*/
static void shl06_02_char_set( PLAYER_WORK *ew, SHL06_02_LOCAL_W *lw )
{
	int no ;
	int step ;

	ew->disp_flag = 0 ;
	if( !lw->beam_st ){
		step = lw->upper_step ;
		if( ( Sys->game_timer0 & 1 ) ^ ( ew->sub_type >> 1 ) ) step = lw->lower_step + 20 ;
		if( lw->long_flag ){
			no = shl06_02_long_tbl[step] ;
			if( no < 0 ) return ;
		}
		else {
			no = shl06_02_char_tbl[step] ;
			if( no < 0 ) return ;
		}
	}
	else no = lw->beam_st ;
	ew->disp_flag = 1 ;
	pl_char_set2( ew, 22, 2, no ) ;
}

static int shl06_02_in_range( Float y, Float hi, Float lo );
static void shl06_02_box_calc( PLAYER_WORK *ew, Uint16 no, Float *hi, Float *lo );

/*==============================================================*/
/*			Where on the opponent the beam's piece struck:		*/
/*			upper_step for the upper box and lower_step for		*/
/*			the lower											*/
/*==============================================================*/
static void shl06_02_hit_set( PLAYER_WORK *ew, SHL06_02_LOCAL_W *lw )
{
	PLAYER_WORK *em = ew->em_w ;
	Float lo ;
	Float hi ;
	Float y ;
	int cnt ;
	int i ;

	if( em->work_kind != W_PL ) return ;
	if( em->frame_box == 0 ) return ;
	hi = -500.0f ;
	lo = 500.0f ;
	if( em->frame_box->body_box0 ) shl06_02_box_calc( em, em->frame_box->body_box0, &hi, &lo ) ;
	if( em->frame_box->body_box1 ) shl06_02_box_calc( em, em->frame_box->body_box1, &hi, &lo ) ;
	if( em->frame_box->body_box2 ) shl06_02_box_calc( em, em->frame_box->body_box2, &hi, &lo ) ;
	if( em->frame_box->body_box3 & BOX_MASK ) shl06_02_box_calc( em, em->frame_box->body_box3 & BOX_MASK, &hi, &lo ) ;
	cnt = 0 ;
	y = ew->pos.y + DOT_Y( 72.0f ) ;
	if( shl06_02_in_range( y, hi, lo ) ){
		for( i = 0 ; i < 4 ; i ++ ){
			y -= DOT_Y( 16.0f ) ;
			cnt ++ ;
			if( !shl06_02_in_range( y, hi, lo ) ) goto upper ;
		}
		cnt = 10 ;
	}
	else {
		cnt = 10 ;
		for( i = 0 ; i < 4 ; i ++ ){
			y -= DOT_Y( 16.0f ) ;
			cnt ++ ;
			if( shl06_02_in_range( y, hi, lo ) ) goto upper ;
		}
		cnt = 0 ;
	}
upper:
	lw->upper_step = cnt ;
	y = ew->pos.y + DOT_Y( 8.0f ) ;
	cnt = 0 ;
	if( shl06_02_in_range( y, hi, lo ) ){
		for( i = 0 ; i < 4 ; i ++ ){
			y -= DOT_Y( 16.0f ) ;
			cnt ++ ;
			if( !shl06_02_in_range( y, hi, lo ) ) goto lower ;
		}
		cnt = 10 ;
	}
	else {
		cnt = 10 ;
		for( i = 0 ; i < 4 ; i ++ ){
			y -= DOT_Y( 16.0f ) ;
			cnt ++ ;
			if( shl06_02_in_range( y, hi, lo ) ) goto lower ;
		}
		cnt = 0 ;
	}
lower:
	lw->lower_step = cnt ;
}

/*==============================================================*/
static int shl06_02_in_range( Float y, Float hi, Float lo )
{
	if( hi > y && y > lo ) return 1 ;
	return 0 ;
}

/*==============================================================*/
/*			Widens hi and lo to take in the box of the			*/
/*			opponent's frame									*/
/*==============================================================*/
static void shl06_02_box_calc( PLAYER_WORK *ew, Uint16 no, Float *hi, Float *lo )
{
	HIT_BOX *box = ew->hit_box_tbl + no ;
	Float tmp[2] ;

	tmp[0] = tmp[1] = ew->pos.y - box->y * 2.142857f ;
	tmp[0] += DOT_Y( box->h ) ;
	tmp[1] -= DOT_Y( box->h ) ;
	if( *hi < tmp[0] ) *hi = tmp[0] ;
	if( *lo > tmp[1] ) *lo = tmp[1] ;
}

/*==============================================================*/
static void shl06_02_flash_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *nw ;

	if( ( nw = shl06_01_set2( ew, 8 ) ) ) nw->pos = ew->pos ;
}

/*--------------------------------------------------------------*/
static void shl06_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl06_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl06_02.c" End of File	*/
