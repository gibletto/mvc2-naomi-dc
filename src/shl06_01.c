/* Cyclops's effect 01 (character 06's shot 01): the beam of his Optic Blast, Super Optic Blast and
   Optic Sweep, of his throws and of a win pose. The first work (type 0) rides on him and flashes
   his colours; it lays down a trail of works (type 1), each one following the one made before it
   and showing his beam pose. */
#include "player.h"
#include "pl06_def.h"

extern void get_2d_pos( VEC3 *pos, Float *scr );

extern void *shl06_01_set2( PLAYER_WORK *pl_w, Sint8 no );

/* an effect's own work, kept in obj_mtx */
typedef struct SHL06_01_LOCAL_W {
	Uint8	beam_st;			/* 00: the pose, 0 to 64 in steps of 16 */
	Sint8	st_timer;			/* 01: frames to the next step of beam_st */
	Uint8	unk_02;			/* 02: turns over each frame */
	Uint8	cut_flag;			/* 03: 0xff once the trail is cut */
	Uint8	attack_flag;			/* 04: 0xff on the works that attack */
	Sint8	attack_cnt;			/* 05 */
	Sint8	unk_06;			/* 06 */
	Sint8	trail_timer;			/* 07: frames to the next work of the trail */
	Sint8	trail_wait;			/* 08: trail_timer as it starts again */
	Uint8	alt_no;			/* 09 */
	Uint8	trail_cnt;			/* 0a */
	Uint8	unk_0b;			/* 0b: the long beam's */
	struct PLAYER_WORK *parent;	/* 0c: the work that made this one */
	Float	ground_x;			/* 10 */
	Float	ground_y;			/* 14 */
	Uint8	unk_18[0x1c - 0x18];
	Float	cut_x;			/* 1c */
	Float	cut_y;			/* 20 */
	Uint8	unk_24[0x28 - 0x24];
	Uint8	cut_dir;			/* 28 */
	Uint8	alt_flag;			/* 29 */
	Uint8	ground_flag;			/* 2a */
	Sint8	flash_timer;			/* 2b: the flash's timer */
	Sint8	flash_step;			/* 2c: and its step */
} SHL06_01_LOCAL_W;
#define SHL06_01_LW( w )	( ( SHL06_01_LOCAL_W * )&( w )->obj_mtx )

/* where the trail's works go from the one before, by the step of the pose */
static Sint16 shl06_01_ofs_a[9][10] = {
	0, -27, 0, -29, 0, -29, 0, -29, 0, -26,
	-10, -21, -12, -23, -11, -23, -13, -23, -13, -24,
	-21, -16, -22, -18, -22, -18, -22, -19, -32, -24,
	-18, -6, -31, -10, -31, -10, -31, -10, -32, -10,
	-32, 0, -32, 0, -32, 0, -32, 0, -32, 0,
	-20, 7, -31, 10, -34, 19, -34, 19, -34, 19,
	-30, 10, -25, 19, -23, 19, -23, 19, -22, 19,
	-11, 23, -12, 23, -11, 23, -13, 23, -13, 24,
	0, 29, 0, 29, 0, 29, 0, 29, 0, 29,
} ;

static Sint16 shl06_01_ofs_b[9][10] = {
	0, -26, 0, -26, 0, -26, 0, -26, 0, -26,
	-10, -21, -11, -21, -11, -21, -11, -21, -11, -21,
	-35, -26, -21, -16, -21, -17, -21, -17, -21, -17,
	-30, -10, -29, -10, -29, -10, -29, -10, -29, -10,
	-32, 0, -32, 0, -32, 0, -32, 0, -32, 0,
	-27, 9, -28, 9, -28, 9, -27, 9, -27, 9,
	-23, 17, -22, 17, -22, 18, -22, 18, -22, 18,
	-11, 23, -11, 22, -10, 21, -12, 22, -11, 22,
	0, 26, 0, 26, 0, 26, 0, 26, 0, 26,
} ;

static Sint16 *shl06_01_ofs_a_tbl[16] = {
	shl06_01_ofs_a[0], shl06_01_ofs_a[1], shl06_01_ofs_a[2], shl06_01_ofs_a[3],
	shl06_01_ofs_a[4], shl06_01_ofs_a[5], shl06_01_ofs_a[6], shl06_01_ofs_a[7],
	shl06_01_ofs_a[8], shl06_01_ofs_a[7], shl06_01_ofs_a[6], shl06_01_ofs_a[5],
	shl06_01_ofs_a[4], shl06_01_ofs_a[3], shl06_01_ofs_a[2], shl06_01_ofs_a[1],
} ;

static Sint16 *shl06_01_ofs_b_tbl[16] = {
	shl06_01_ofs_b[0], shl06_01_ofs_b[1], shl06_01_ofs_b[2], shl06_01_ofs_b[3],
	shl06_01_ofs_b[4], shl06_01_ofs_b[5], shl06_01_ofs_b[6], shl06_01_ofs_b[7],
	shl06_01_ofs_b[8], shl06_01_ofs_b[7], shl06_01_ofs_b[6], shl06_01_ofs_b[5],
	shl06_01_ofs_b[4], shl06_01_ofs_b[3], shl06_01_ofs_b[2], shl06_01_ofs_b[1],
} ;

/* the first work's trail start, by direction */
static Sint16 shl06_01_head_ofs[16][2] = {
	0, -12, 6, -11, 12, -8, 15, -5,
	16, 0, 14, 5, 11, 7, 6, 11,
	0, 12, -6, 11, -11, 7, -14, 5,
	-16, 0, -15, -5, -12, -8, -6, -11,
} ;

/* the animation of each direction, for each pose */
static Uint8 shl06_01_char_tbl[0x60] = {
	98, 87, 76, 65, 54, 43, 32, 21, 10, 21, 32, 43, 54, 65, 76, 87,
	96, 85, 74, 63, 52, 41, 30, 19, 8, 19, 30, 41, 52, 63, 74, 85,
	94, 83, 72, 61, 50, 39, 28, 17, 6, 17, 28, 39, 50, 61, 72, 83,
	92, 81, 70, 59, 48, 37, 26, 15, 4, 15, 26, 37, 48, 59, 70, 81,
	88, 89, 77, 78, 66, 67, 55, 56, 44, 45, 33, 34, 22, 23, 11, 12,
	0, 1, 11, 12, 22, 23, 33, 34, 44, 45, 55, 56, 66, 67, 77, 78,
} ;

static Uint8 shl06_01_trail_char_tbl[0x60] = {
	98, 87, 76, 65, 54, 43, 32, 21, 10, 21, 32, 43, 54, 65, 76, 87,
	97, 86, 75, 64, 53, 42, 31, 20, 9, 20, 31, 42, 53, 64, 75, 86,
	95, 84, 73, 62, 51, 40, 29, 18, 7, 18, 29, 40, 51, 62, 73, 84,
	93, 82, 71, 60, 49, 38, 27, 16, 5, 16, 27, 38, 49, 60, 71, 82,
	90, 91, 79, 80, 68, 69, 57, 58, 46, 47, 35, 36, 24, 25, 13, 14,
	2, 3, 13, 14, 24, 25, 35, 36, 46, 47, 57, 58, 68, 69, 79, 80,
} ;

static Uint8 shl06_01_cut_char_tbl[16] = {
	103, 102, 101, 100, 99, 104, 105, 106, 107, 106, 105, 104, 99, 100, 101, 102,
} ;

/* where the first work sits on Cyclops, by direction: standing and in the air */
static Sint16 shl06_01_pos_tbl[16][2] = {
	0, -144, 22, -141, 36, -135, 46, -122,
	57, -104, 51, -85, 40, -73, 21, -66,
	0, -61, -21, -65, -40, -73, -50, -85,
	-57, -103, -46, -122, -38, -135, -22, -141,
} ;

static Sint16 shl06_01_air_pos_tbl[16][2] = {
	-4, -151, 22, -148, 38, -140, 48, -127,
	58, -109, 50, -93, 40, -81, 21, -75,
	-1, -73, -24, -74, -45, -82, -54, -94,
	-61, -109, -49, -127, -42, -139, -26, -145,
} ;

static void shl06_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl06_01_set( PLAYER_WORK *pl_w, Sint8 big, Sint8 wait, Sint8 alt )
{
	PLAYER_WORK *ew ;
	SHL06_01_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl06_01_main ;
		ew->master_w = pl_w ;
		ew->type = ew->sub_type = 0 ;
		ew->ef_no = 0x601 ;
		lw = SHL06_01_LW( ew ) ;
		lw->unk_0b = big ;
		lw->trail_wait = wait ;
		lw->alt_flag = alt ;
	}
	return ew ;
}

static void shl06_01_init( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );
static void shl06_01_move( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );
static void shl06_01_end( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );
static void shl06_01_free( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );

/*==============================================================*/
static void shl06_01_main( PLAYER_WORK *ew )
{
	static void ( *shl06_01_jmp[] )( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw ) = {
		shl06_01_init, shl06_01_move, shl06_01_end, shl06_01_free,
	} ;

	shl06_01_jmp[ew->routine_0]( ew, SHL06_01_LW( ew ) ) ;
}

/*==============================================================*/
static void shl06_01_init( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *nw ;
	SHL06_01_LOCAL_W *nlw ;

	ew->routine_0 ++ ;
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
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 64 ;
	if( !ew->type ){
		ew->sub_prio = -1 ;
		lw->beam_st = 0 ;
		lw->st_timer = 4 ;
		lw->attack_flag = 0 ;
		lw->attack_cnt = 2 ;
		lw->unk_02 = lw->unk_06 = lw->cut_flag = 0 ;
		lw->flash_step = 1 ;
		lw->flash_timer = 1 ;
		ew->dir = PL06_LW( pl_w )->beam_dir ;
		lw->trail_cnt = PL06_LW( pl_w )->beam_len ;
		if( !lw->unk_0b ){
			lw->ground_flag = 0 ;
			lw->trail_wait = 0 ;
			shl06_01_set2( ew, 5 ) ;
		}
		else if( pl_w->char_no.b.grp == Char_Catch ){
			lw->ground_flag = 0 ;
			shl06_01_set2( ew, 6 ) ;
		}
		if( ( nw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			nw->work_func = ew->work_func ;
			nw->type = 1 ;
			nw->master_w = ew->master_w ;
			nw->ef_no = 0x601 ;
			nlw = SHL06_01_LW( nw ) ;
			nlw->parent = ew ;
			nlw->attack_flag = 0xff ;
			nlw->cut_flag = 0xff ;
		}
	}
	ew->dir = pl_w->dir ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	lw->trail_timer = 0 ;
	ew->sub_prio = -1 ;
	shl06_01_move( ew, lw ) ;
}

static void shl06_01_head_move( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );
static void shl06_01_trail_move( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );

/*--------------------------------------------------------------*/
static void shl06_01_move( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	if( !ew->type ) shl06_01_head_move( ew, lw ) ;
	else shl06_01_trail_move( ew, lw ) ;
}

static void shl06_01_pos_set( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, PLAYER_WORK *pl_w );
static void shl06_01_char_set( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw );

/*==============================================================*/
/*			The first work: it keeps step with Cyclops and		*/
/*			puts out a new work of the trail					*/
/*==============================================================*/
static void shl06_01_head_move( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL06_LOCAL_W *plw = PL06_LW( pl_w ) ;
	PLAYER_WORK *nw ;
	SHL06_01_LOCAL_W *nlw ;

	ew->prio = pl_w->prio ;
	if( !plw->beam_flag ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	if( lw->unk_0b ) lw->beam_st = plw->beam_st ;
	if( lw->beam_st != plw->beam_st ){
		if( --lw->st_timer < 0 ){
			lw->st_timer = 1 ;
			if( lw->beam_st < plw->beam_st ) lw->beam_st += 16 ;
			else lw->beam_st -= 16 ;
		}
	}
	if( lw->unk_06 ) lw->unk_06 -- ;
	lw->ground_flag = 0 ;
	lw->unk_02 ^= 2 ;
	shl06_01_flash( ew, &lw->flash_timer, &lw->flash_step ) ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->dir = plw->beam_dir ;
	if( ew->dir < 8 ) ew->disp_dir ^= 1 ;
	if( --lw->trail_timer <= 0 ){
		if( lw->trail_timer < 0 ) lw->trail_timer = lw->trail_wait ;
		if( !lw->trail_timer ){
			if( !lw->unk_0b || lw->trail_cnt ){
				if( lw->trail_cnt ) lw->trail_cnt -- ;
				if( ( nw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
					nw->work_func = ew->work_func ;
					nw->type = 1 ;
					nw->master_w = ew->master_w ;
					nw->ef_no = 0x601 ;
					nlw = SHL06_01_LW( nw ) ;
					nlw->parent = ew ;
					nlw->cut_flag = 0 ;
					nlw->alt_flag = lw->alt_flag ;
					nlw->attack_flag = 0 ;
					if( --lw->attack_cnt < 0 ){
						lw->attack_cnt = 2 ;
						nlw->attack_flag = 0xff ;
						if( lw->unk_0b ){
							ew->atck_flag = HIT_ON | HIT_SHOT ;
							ew->dm_flag = HIT_ON | HIT_SHOT ;
						}
					}
				}
			}
		}
	}
	if( --ew->timer0 < 0 ){
		ew->timer0 = 10 ;
		if( !lw->unk_0b && pl_w->char_no.b.grp != Char_Catch ){
			Uint8 no = pl_w->atck_no ;

			if( lw->alt_flag ){
				lw->alt_no = ( lw->alt_no + 1 ) & 1 ;
				no += lw->alt_no ;
			}
			ew->atck_no = no ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
			hit_work_set( ew ) ;
		}
	}
	shl06_01_pos_set( ew, lw, pl_w ) ;
	shl06_01_char_set( ew, lw ) ;
	ew->disp_flag = 1 ;
}

static void shl06_01_trail_pos( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, PLAYER_WORK *bw, SHL06_01_LOCAL_W *plw );
static Uint8 shl06_01_in_view( PLAYER_WORK *ew );
static void shl06_01_hit_box( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, SHL06_01_LOCAL_W *plw );
static void shl06_01_hit( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, SHL06_01_LOCAL_W *plw );

/*==============================================================*/
/*			The works of the trail: each one takes its place	*/
/*			from the one before it								*/
/*==============================================================*/
static void shl06_01_trail_move( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	static Uint8 shl06_01_dir_tbl[16] = {
		0, 1, 2, 3, 4, 3, 2, 1, 0, 15, 14, 13, 12, 13, 14, 15,
	} ;
	PLAYER_WORK *mw = ew->master_w ;
	PLAYER_WORK *bw ;
	SHL06_01_LOCAL_W *plw = SHL06_01_LW( lw->parent ) ;
	SHL06_01_LOCAL_W *blw ;

	if( lw->parent->routine_0 != 1 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	bw = ew->before_w ;
	blw = SHL06_01_LW( bw ) ;
	if( blw->cut_flag ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		lw->cut_flag = 0xff ;
		return ;
	}
	if( !plw->unk_0b || !plw->trail_timer ) ew->type ++ ;
	lw->unk_02 = plw->unk_02 ;
	ew->disp_dir = mw->disp_dir ;
	ew->dir = bw->dir ;
	ew->prio = bw->prio ;
	ew->sub_prio = bw->sub_prio - 1 ;
	shl06_01_trail_pos( ew, lw, bw, plw ) ;
	shl06_01_char_set( ew, lw ) ;
	if( ew->pos.y > mw->ground_y ){
		/* still above the ground */
	}
	else {
		ew->dir = shl06_01_dir_tbl[ew->dir] ;
		plw->ground_x = ew->pos.x ;
		plw->ground_y = mw->ground_y ;
		plw->ground_flag = 0xff ;
		if( !plw->unk_06 ) plw->unk_06 = 30 ;
		if( plw->unk_0b ){
			ew->routine_0 = 2 ;
			ew->disp_flag = 0 ;
			return ;
		}
		shl06_01_trail_pos( ew, lw, bw, plw ) ;
		ew->pos.y -= DOT_Y( 16.0f ) ;
		shl06_01_char_set( ew, lw ) ;
	}
	if( ew->dir < 8 ) ew->disp_dir ^= 1 ;
	if( !shl06_01_in_view( ew ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		shl06_01_hit( ew, lw, plw ) ;
	}
	else if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		shl06_01_hit_box( ew, lw, plw ) ;
	}
	else if( !plw->unk_0b ){
		if( !lw->attack_flag ){
			ew->atck_flag = 0 ;
			if( lw->cut_flag ){
				if( plw->trail_timer ) goto show ;
				ew->atck_flag = HIT_ON | HIT_SHOT ;
			}
		}
		{
			Uint8 no = mw->atck_no ;

			if( lw->alt_flag ){
				lw->alt_no = ( lw->alt_no + 1 ) & 1 ;
				no += lw->alt_no ;
			}
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
			hit_work_set( ew ) ;
		}
	}
show:
	ew->disp_flag = 1 ;
}

/*--------------------------------------------------------------*/
static void shl06_01_end( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl06_01_free( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The first work's place on Cyclops while he fires	*/
/*			the beam standing (pose 0)							*/
/*==============================================================*/
static void shl06_01_blast_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float ofs[2] ;

	switch( pl_w->char_no.b.no ){
	case 0:
		ofs[0] = DOT_X( 58.0f ) ;
		ofs[1] = DOT_Y( 62.0f ) ;
		break ;
	case 1:
		ofs[0] = DOT_X( 74.0f ) ;
		ofs[1] = DOT_Y( 101.0f ) ;
		break ;
	case 2:
		ofs[0] = 80.0f ;
		ofs[1] = DOT_Y( 128.0f ) ;
		break ;
	default:
		ofs[0] = DOT_X( 76.0f ) ;
		ofs[1] = DOT_Y( 108.0f ) ;
		break ;
	}
	if( !pl_w->disp_dir ) ofs[0] = -ofs[0] ;
	ew->pos.x = pl_w->pos.x + ofs[0] ;
	ew->pos.y = pl_w->pos.y + ofs[1] ;
}

/*==============================================================*/
static void shl06_01_sweep_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y = pl_w->pos.y + DOT_Y( 11.0f ) ;
	if( pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + 225.0f ;
	else ew->pos.x = pl_w->pos.x - 225.0f ;
}

/*==============================================================*/
static void shl06_01_catch_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.y = pl_w->pos.y + DOT_Y( 93.0f ) ;
	if( pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( 70.0f ) ;
	else ew->pos.x = pl_w->pos.x - DOT_X( 70.0f ) ;
}

/*==============================================================*/
static void shl06_01_pos_set( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, PLAYER_WORK *pl_w )
{
	Float ofs[2] ;

	if( pl_w->act_st == N_SPECIAL ){
		if( !pl_w->pl_sp_type ){
			shl06_01_blast_pos( ew, pl_w ) ;
			return ;
		}
		if( pl_w->pl_sp_type == 8 ){
			shl06_01_sweep_pos( ew, pl_w ) ;
			return ;
		}
	}
	if( pl_w->char_no.b.grp == Char_Win ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 136.0f ) ;
		return ;
	}
	if( pl_w->char_no.b.grp == Char_Catch ){
		if( pl_w->char_no.b.no == 8 ){
			shl06_01_catch_pos( ew, pl_w ) ;
			return ;
		}
		if( pl_w->char_no.b.no == 9 ){
			shl06_01_catch_pos( ew, pl_w ) ;
			return ;
		}
	}
	if( pl_w->pl_stance != STANCE_AIR ){
		ofs[0] = DOT_X( shl06_01_pos_tbl[ew->dir][0] ) ;
		ofs[1] = DOT_Y( shl06_01_pos_tbl[ew->dir][1] ) ;
	}
	else {
		ofs[0] = DOT_X( shl06_01_air_pos_tbl[ew->dir][0] ) ;
		ofs[1] = DOT_Y( shl06_01_air_pos_tbl[ew->dir][1] ) ;
	}
	if( pl_w->disp_dir ) ofs[0] = -ofs[0] ;
	ew->pos.x = pl_w->pos.x + ofs[0] ;
	ew->pos.y = pl_w->pos.y - ofs[1] ;
}

/*==============================================================*/
static void shl06_01_trail_pos( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, PLAYER_WORK *bw, SHL06_01_LOCAL_W *plw )
{
	Float ofs[2] ;
	Sint16 *p ;

	if( lw->cut_flag ){
		ofs[0] = DOT_X( shl06_01_head_ofs[ew->dir][0] ) ;
		ofs[1] = DOT_Y( shl06_01_head_ofs[ew->dir][1] ) ;
	}
	else {
		if( ew->type == 1 ) p = shl06_01_ofs_a_tbl[ew->dir] ;
		else p = shl06_01_ofs_b_tbl[ew->dir] ;
		p += plw->beam_st >> 3 ;
		ofs[0] = DOT_X( p[0] ) ;
		ofs[1] = DOT_Y( p[1] ) ;
		if( ew->dir < 8 ) ofs[0] = -ofs[0] ;
	}
	if( ew->disp_dir ) ofs[0] = -ofs[0] ;
	ew->pos.x = bw->pos.x + ofs[0] ;
	ew->pos.y = bw->pos.y - ofs[1] ;
}

/*==============================================================*/
static void shl06_01_char_set( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw )
{
	SHL06_01_LOCAL_W *plw ;
	PLAYER_WORK *pw ;
	Uint8 *tbl ;
	Uint8 pose ;
	Float ofs[2] ;

	if( ew->type ){
		pw = lw->parent ;
		plw = SHL06_01_LW( pw ) ;
		if( lw->cut_flag && plw->beam_st == 64 ) goto cut ;
		tbl = shl06_01_trail_char_tbl ;
		pose = plw->beam_st ;
	}
	else {
		tbl = shl06_01_char_tbl ;
		pose = lw->beam_st ;
	}
	tbl += pose ;
	if( pose != 64 ) pl_char_set2( ew, 21, 8, tbl[ew->dir] ) ;
	else pl_char_set2( ew, 21, 8, tbl[ew->dir * 2] ) ;
	return ;
cut:
	pl_char_set2( ew, 21, 8, shl06_01_cut_char_tbl[ew->dir] ) ;
	if( plw->unk_0b ){
		plw->cut_dir = ew->dir ;
		ofs[0] = DOT_X( shl06_01_head_ofs[ew->dir][0] ) ;
		ofs[1] = DOT_Y( shl06_01_head_ofs[ew->dir][1] ) ;
		if( ew->disp_dir ) ofs[0] = -ofs[0] ;
		plw->cut_x = ew->pos.x + ofs[0] ;
		plw->cut_y = ew->pos.y - ofs[1] ;
	}
}

/*==============================================================*/
/*			1 while the work is still on the screen				*/
/*==============================================================*/
static Uint8 shl06_01_in_view( PLAYER_WORK *ew )
{
	Float scr[2] ;
	Float h ;

	if( !scr_in_x_ck( ew ) ) return 0 ;
	get_2d_pos( &ew->pos, scr ) ;
	h = DOT_Y( ew->size_bottom ) ;
	if( ( Sint16 )( scr[1] + h ) <= 0 ) return 0 ;
	return 1 ;
}

/*==============================================================*/
static void shl06_01_hit_box( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, SHL06_01_LOCAL_W *plw )
{
	lw->cut_flag = 0xff ;
	lw->attack_flag = 0 ;
	plw->trail_timer = 6 ;
}

/*==============================================================*/
static void shl06_01_hit( PLAYER_WORK *ew, SHL06_01_LOCAL_W *lw, SHL06_01_LOCAL_W *plw )
{
	ew->hit_flag = 0 ;
	lw->cut_flag = 0xff ;
	lw->attack_flag = 0 ;
	plw->trail_timer = 6 ;
}

/****************************************************************/
/*			The colours Cyclops flashes while the beam is		*/
/*			shown												*/
/****************************************************************/
void shl06_01_flash( PLAYER_WORK *ew, Sint8 *timer, Sint8 *step )
{
	static Uint8 shl06_01_flash_tbl[] = {
		2, 16, 2, 17, 1, 18, 1, 19, 1, 20,
	} ;

	if( --*timer <= 0 ){
		*timer = shl06_01_flash_tbl[*step * 2] ;
		ef_color_set( ew->master_w, 2, shl06_01_flash_tbl[*step * 2 + 1], 1 ) ;
		*step = ( *step + 1 ) % 5 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl06_01.c" End of File	*/
