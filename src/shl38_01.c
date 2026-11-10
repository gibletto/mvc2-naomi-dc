/* Captain Commando's second effect (character 38's shot 01): three works that fly in beside him,
   stand with him and, in Captain Storm, fly across the opponent. */
#include "player.h"
#include "pl_char.h"
#include "pl38_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL38_01_LOCAL_W {
	PLAYER_WORK	*em_w;	/* 000: the opponent held */
} SHL38_01_LOCAL_W;
#define SHL38_01_LW( w )	( ( SHL38_01_LOCAL_W * )&( w )->obj_flag )

/* by sub_type and type: where it stops and how far it comes from */
static Sint16 shl38_01_pos_tbl[] = {
	64, -256, -24, 256, 88, 512,
	80, -256, 80, 256, 16, 512,
	-16, -256, 68, 256, 26, 512,
	-64, -256, -32, 256, -96, 512,
	-16, -256, 68, 256, 26, 512,
} ;

/* how a work takes off for its attack, by type: its box, turn, animation and speeds */
typedef struct SHL38_01_ATCK_REC {
	Uint8	box[4] ;
	Uint8	flip ;
	Uint8	unk_05 ;
	Sint16	no ;
	Float	spd_x, spd_y ;
} SHL38_01_ATCK_REC;

static SHL38_01_ATCK_REC shl38_01_atck_tbl[] = {
	{ { 112, 0, 112, 112 }, 0, 0, 6, -DOT_X( 16.0f ), DOT_Y( 4.0f ) },
	{ { 112, 0, 96, 64 }, 0, 0, 12, -DOT_X( 4.0f ), DOT_Y( 16.0f ) },
	{ { 112, 0, 112, 112 }, 1, 0, 18, -DOT_X( 16.0f ), DOT_Y( 6.0f ) },
} ;

/* by type and sub_type: the animation, the order among works and a timer */
static Sint16 shl38_01_anim_tbl[] = {
	4, -1, -1,	5, -1, -1,	7, 1, 1,	8, 1, 1,	22, 1, 1,
	10, 1, 1,	11, 1, 1,	13, 1, 1,	14, 1, 1,	23, 1, 1,
	16, 1, 1,	17, 1, 1,	19, 1, 1,	20, 1, 1,	24, 1, 1,
} ;

static void shl38_01_main( PLAYER_WORK *ew ) ;

/****************************************************************/
PLAYER_WORK *Shl38_01_set( PLAYER_WORK *pl_w, int a, int b )
{
	register PLAYER_WORK *ew = 0 ;

	if( work_rest_cnt > 13 ){
		ew = work_get( 0, W_SHL_ST, LIST_TOP ) ;
		ew->work_func = ( void * )shl38_01_main ;
		ew->type = 0 ;
		ew->sub_type = b ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = ew->type | 0x3800 ;
		ew = work_get( 0, W_SHL_ST, LIST_TOP ) ;
		ew->work_func = ( void * )shl38_01_main ;
		ew->type = 1 ;
		ew->sub_type = b ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = ew->type | 0x3800 ;
		ew = work_get( 0, W_SHL_ST, LIST_TOP ) ;
		ew->work_func = ( void * )shl38_01_main ;
		ew->type = 2 ;
		ew->sub_type = b ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = ew->type | 0x3800 ;
		PL38_LW( pl_w )->rush_result = 0 ;
	}
	return ew ;
}

static void shl38_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*==============================================================*/
static void shl38_01_main( PLAYER_WORK *ew )
{
	static void ( *shl38_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_01_init, shl38_01_move, shl38_01_end, shl38_01_free,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	pl_w->team_ng_timer = PL38_LW( pl_w )->shot_timer = 2 ;
	shl38_01_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Uint8 shl38_01_box_tbl[] = {
		112, 0, 112, 112,
		112, 0, 96, 64,
		112, 0, 80, 112,
	} ;
	Sint32 n ;

	ew->routine_0 ++ ;
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
	n = ew->type * 4 ;
	ew->size_top = shl38_01_box_tbl[n] ;
	ew->size_bottom = shl38_01_box_tbl[n + 1] ;
	ew->size_front = shl38_01_box_tbl[n + 2] ;
	ew->size_back = shl38_01_box_tbl[n + 3] ;
	ew->routine_1 = 0 ;
	if( ew->sub_type == 3 ) ew->routine_1 = 1 ;
	ew->routine_2 = 0 ;
	ew->disp_flag = 0 ;
}

static void shl38_01_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_01_in, shl38_01_wait, shl38_01_out, shl38_01_atck,
	} ;

	shl38_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl38_01_spd_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Float x, Float y ) ;

/*==============================================================*/
static void shl38_01_in( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 a, b ;
	Sint32 n ;

	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		ew->disp_flag = 1 ;
		n = ew->type * 2 + ew->sub_type * 6 ;
		a = shl38_01_pos_tbl[n] ;
		b = shl38_01_pos_tbl[n + 1] ;
		if( ew->disp_dir ){
			a = -a ;
			b = -b ;
		}
		ew->pos.x = pl_w->pos.x + ( a + b ) * 1.6666666f ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 480.0f ) ;
		shl38_01_spd_set( ew, pl_w, -b * 1.6666666f, -DOT_Y( 480.0f ) ) ;
	} else {
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( pl_w->ground_y > ew->pos.y ){
			ew->pos.y = pl_w->ground_y ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
			ew->routine_1 = 1 ;
			ew->routine_2 = 0 ;
		}
	}
}

/*==============================================================*/
static void shl38_01_spd_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, Float x, Float y )
{
	static Sint16 shl38_01_spd_anim[] = { 3, 9, 15 } ;

	ew->spd.x = x / 32.0f ;
	ew->spd.y = y / 32.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 0 ;
	pl_char_set( ew, 20, shl38_01_spd_anim[ew->type] ) ;
}

static void shl38_01_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_wait_sword( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_wait_storm( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_wait_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_wait3( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_01_wait_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_01_wait_sword, shl38_01_wait_storm, shl38_01_wait_pose, shl38_01_wait3, shl38_01_wait_pose,
	} ;
	Sint16 *p ;

	ew->prio = pl_w->prio ;
	switch( ew->routine_2 ){
	case 0:
		ew->routine_2 ++ ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		p = &shl38_01_anim_tbl[ew->sub_type * 3 + ew->type * 15] ;
		pl_char_set( ew, 20, *p ++ ) ;
		ew->sub_prio = *p ++ ;
		ew->prio = pl_w->prio ;
		ew->timer1 = *p ;
		if( !ew->type ){
			if( ew->sub_type >= 4 ){ }
		}
		break ;
	case 1:
		shl38_01_wait_jmp[ew->sub_type]( ew, pl_w ) ;
		break ;
	case 2:
		if( pl_char_move( ew ) < 0 ) shl38_01_next( ew, pl_w ) ;
		break ;
	}
}

/*==============================================================*/
static void shl38_01_next( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 = 2 ;
	ew->routine_2 = 0 ;
	shl38_01_out( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_01_wait_sword( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		shl38_01_next( ew, pl_w ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = ew->timer1 ;
	if( pl_w->cg_flag1 ) pl_char_move( ew ) ;
}

/*==============================================================*/
static void shl38_01_wait_storm( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || pl_w->act_st != N_SUPER ){
		shl38_01_next( ew, pl_w ) ;
		return ;
	}
	if( !PL38_LW( pl_w )->rush_result ) return ;
	if( PL38_LW( pl_w )->rush_result < 0 ){
		shl38_01_next( ew, pl_w ) ;
	} else if( !pl_w->em_w ){
		shl38_01_next( ew, pl_w ) ;
	} else if( !pl_w->em_w->team_no ){
		SHL38_01_LW( ew )->em_w = pl_w->em_w ;
		ew->routine_1 = 3 ;
		ew->routine_2 = 0 ;
		shl38_01_atck( ew, pl_w ) ;
	}
}

/*==============================================================*/
static void shl38_01_wait_pose( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || ( pl_w->act_st != N_SUPER && pl_w->act_st != N_WIN ) ){
		shl38_01_next( ew, pl_w ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = ew->timer1 ;
}

/*==============================================================*/
static void shl38_01_wait3( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	switch( pl_w->routine_1 ){
	case 1:
		if( pl_w->cg_type ){
			if( ew->type == 2 ){
				ew->routine_2 ++ ;
				pl_char_set( ew, 20, 21 ) ;
				ew->pos.x = pl_w->pos.x ;
				ew->pos.y = pl_w->pos.y ;
				ew->prio = pl_w->prio ;
				ew->sub_prio = ew->timer1 ;
				pl_char_move( ew ) ;
				return ;
			}
		} else {
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->prio = pl_w->prio ;
			ew->sub_prio = ew->timer1 ;
			pl_char_move( ew ) ;
			return ;
		}
	}
	if( !ew->type ) pl_voice_req( pl_w, 27 ) ;
	shl38_01_next( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_01_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 a, b ;
	Sint32 n ;

	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		n = ew->type * 2 + ew->sub_type * 6 ;
		a = shl38_01_pos_tbl[n] ;
		b = shl38_01_pos_tbl[n + 1] ;
		if( ew->disp_dir ){
			a = -a ;
			b = -b ;
		}
		ew->timer1 = 50 ;
		ew->pos.x += DOT_X( a ) ;
		shl38_01_spd_set( ew, pl_w, DOT_X( b ), DOT_Y( 480.0f ) ) ;
	} else {
		if( scr_in_x_ck( ew ) ){
			pl_char_move( ew ) ;
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			if( -- ew->timer1 >= 0 ) return ;
		}
		shl38_01_end( ew, pl_w ) ;
	}
}

static void shl38_01_atck_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_atck_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_atck_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_atck_throw( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;

/*--------------------------------------------------------------*/
static void shl38_01_atck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_01_atck_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl38_01_atck_init, shl38_01_atck_move, shl38_01_atck_hold, shl38_01_atck_throw,
	} ;

	shl38_01_atck_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_01_atck_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL38_01_ATCK_REC *rec = shl38_01_atck_tbl ;
	Float x ;

	ew->routine_2 ++ ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->size_top = rec[ew->type].box[0] ;
	ew->size_bottom = rec[ew->type].box[1] ;
	ew->size_front = rec[ew->type].box[2] ;
	ew->size_back = rec[ew->type].box[3] ;
	if( rec[ew->type].flip ) ew->disp_dir ^= 1 ;
	ew->spd.y = rec[ew->type].spd_y ;
	x = rec[ew->type].spd_x ;
	if( ew->disp_dir ) x = -x ;
	ew->spd.x = x ;
	pl_char_set( ew, 20, rec[ew->type].no ) ;
	ew->timer0 = 32 ;
	shl38_01_atck_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl38_01_atck_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Sint16 shl38_01_atck_timer[] = { 22, 6, 38 } ;

	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( -- ew->timer0 < 0 ){
			ew->routine_2 ++ ;
			ew->timer0 = shl38_01_atck_timer[ew->type] ;
			ew->timer1 = 1 ;
			return ;
		}
	}
	if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl38_01_atck_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		em_w = SHL38_01_LW( ew )->em_w ;
		if( em_w->routine_1 == PL_DAMAGE && !em_w->team_no ){
			if( em_w->pos.y > em_w->ground_y + DOT_Y( 96.0f ) ) ew->routine_2 ++ ;
		} else shl38_01_end( ew, pl_w ) ;
	}
}

static void shl38_01_atck_set( PLAYER_WORK *ew, PLAYER_WORK *em_w ) ;
static void shl38_01_atck_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) ;
static void shl38_01_atck_end( PLAYER_WORK *ew, PLAYER_WORK *em_w ) ;

/*--------------------------------------------------------------*/
static void shl38_01_atck_throw( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl38_01_atck_throw_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *em_w ) = {
		shl38_01_atck_set, shl38_01_atck_fly, shl38_01_atck_end, shl38_01_atck_end,
	} ;
	PLAYER_WORK *em_w = SHL38_01_LW( ew )->em_w ;

	if( em_w->routine_1 == PL_DAMAGE ) shl38_01_atck_throw_jmp[ew->routine_3]( ew, em_w ) ;
	else shl38_01_end( ew, pl_w ) ;
}

/*==============================================================*/
static void shl38_01_atck_set( PLAYER_WORK *ew, PLAYER_WORK *em_w )
{
	static Float shl38_01_atck_ofs[] = {
		DOT_X( 416.0f ), -DOT_Y( 64.0f ), DOT_X( 416.0f ), -DOT_Y( 64.0f ), DOT_X( 416.0f ), -DOT_Y( 96.0f ), DOT_X( 416.0f ), -DOT_Y( 96.0f ),
		DOT_X( 32.0f ), -DOT_Y( 384.0f ), -DOT_X( 32.0f ), -DOT_Y( 384.0f ), DOT_X( 64.0f ), -DOT_Y( 384.0f ), -DOT_X( 64.0f ), -DOT_Y( 384.0f ),
		DOT_X( 416.0f ), -DOT_Y( 128.0f ), DOT_X( 416.0f ), -DOT_Y( 128.0f ), DOT_X( 416.0f ), -DOT_Y( 160.0f ), DOT_X( 416.0f ), -DOT_Y( 160.0f ),
	} ;
	static int shl38_01_atck_voice[] = { 27, 26, 25, 27, 26, 25 } ;
	Float x, y, *p ;

	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( !( ew->master_w->team_no || em_w->team_no ) ){
			if( -- ew->timer0 >= 0 ) return ;
			ew->routine_3 ++ ;
			ew->pos.x = em_w->pos.x ;
			ew->pos.y = em_w->pos.y ;
			p = &shl38_01_atck_ofs[( Rnd() & 3 ) * 2 + ew->type * 8] ;
			x = *p ++ ;
			y = *p ;
			if( ew->disp_dir ) x = -x ;
			ew->pos.x += x ;
			ew->pos.y += y ;
			x *= 16.0f ;
			y *= 16.0f ;
			x = -x / 256.0f ;
			y = -y / 256.0f ;
			ew->disp_dir = 0 ;
			ew->spd.x = x ;
			if( !( 0.0f > x ) ) ew->disp_dir = 1 ;
			ew->spd.y = y ;
			ew->timer0 = 32 ;
			if( ew->type == 1 ){
				ew->atck_no = pl38_at_storm_helper_1 ;
				HIT_RESET( ew ) ;
			} else {
				ew->atck_no = pl38_at_storm_helper ;
				HIT_RESET( ew ) ;
			}
			pl_voice_req( ew, shl38_01_atck_voice[ew->type] ) ;
		} else work_free( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl38_01_atck_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( ew->hit_stop ){
			ew->hit_stop -- ;
			return ;
		}
		if( vs_sys_work.game_stop & 1 << ew->master_w->pl_no ) return ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( -- ew->timer0 < 0 ){
			ew->timer0 = 0 ;
			if( !scr_in_ck( ew ) ){
				pl_w = ew->master_w ;
				em_w = SHL38_01_LW( ew )->em_w ;
				if( !( pl_w->team_no || em_w->team_no ) ){
					if( -- ew->timer1 < 0 ){
						ew->routine_3 ++ ;
						PL38_LW( pl_w )->rush_result ++ ;
					} else {
						ew->timer0 = 2 ;
						ew->routine_3 -- ;
						ew->disp_dir ^= 1 ;
					}
				} else work_free( ew ) ;
			} else hit_work_set( ew ) ;
		} else {
			if( scr_in_ck( ew ) ){
				hit_work_set( ew ) ;
				ew->disp_flag = 1 ;
			} else ew->disp_flag = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl38_01_atck_end( PLAYER_WORK *ew, PLAYER_WORK *em_w )
{
	static Sint16 shl38_01_atck_anim[] = { 3, 9, 15 } ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *tw ;

	if( PL38_LW( pl_w )->rush_result == 3 ){
		ew->routine_3 ++ ;
		tw = pl_w->target_w ;
		ew->pos.x = tw->pos.x ;
		ew->pos.y = tw->pos.y ;
		pl_char_set( ew, 20, shl38_01_atck_anim[ew->type] ) ;
	}
	shl38_01_end( ew, pl_w ) ;
	return ;
}

/*--------------------------------------------------------------*/
static void shl38_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl38_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !vs_sys_work.game_stop ) work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl38_01.c" End of File	*/
