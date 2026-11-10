/* Thanos's effect 04 (character 36's shot 04): the hand of his Soul Gem (sp_soul). It flies out
   from him; if it takes the opponent it holds him and drains his vitality into Thanos. Six parts
   (types 2 to 7, shl36_04_part_set) trail from the hand back to the root that stays by him
   (shl36_04_root_set), and each squeeze flashes (shl36_04_flash_set). */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl36_def.h"

/* what the effect holds, from obj_flag */
typedef struct SHL36_04_LOCAL_W {
	Sint32		char_no;	/* cc: the animation of the player it began in */
	PLAYER_WORK	*em_w;		/* d0: the opponent it holds */
	PLAYER_WORK	*pl_w;		/* d4: the player who made it */
} SHL36_04_LOCAL_W;
#define SHL36_04_LW( w )	( ( SHL36_04_LOCAL_W * )&( w )->obj_flag )

/* a part made from the player's, as the parts of his other effects are */
#define SHL36_04_PART_SET( d, s ) \
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

static void shl36_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl36_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) != 0 ){
		ew->work_func = shl36_04_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3604 ;
		SHL36_04_LW( ew )->pl_w = pl_w ;
		SHL36_04_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl36_04_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_04_main( PLAYER_WORK *ew )
{
	static void ( *shl36_04_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_04_init, shl36_04_move, shl36_04_end, shl36_04_free,
	} ;

	shl36_04_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

static void shl36_04_part_set( PLAYER_WORK *gw, int no );
static void shl36_04_root_set( PLAYER_WORK *gw );

/*==============================================================*/
static void shl36_04_init( register PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 v ;
	PLAYER_WORK *em_w ;
	Sint16 x ;
	Sint16 i ;

	ew->routine_0 ++ ;
	SHL36_04_PART_SET( ew, pl_w ) ;
	ew->sub_prio = -1 ;
	ew->prio = 8 ;
	ew->sub_type = 0 ;
	ew->atck_flag = HIT_ON | HIT_PL_ONLY ;
	ew->dm_flag = 0 ;
	ew->atck_no = pl36_at_soul ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	PL36_LW( pl_w )->shot_timer[0] = 4 ;
	x = 24 ;
	if( ew->disp_dir ) x = -24 ;
	ew->pos.x = pl_w->pos.x + DOT_X( x ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	ew->acc.x = ew->acc.y = 0.0f ;
	em_w = pl_w->target_w ;
	v = em_w->pos ;
	v.y += ( em_w->size_top >> 1 ) * 2.142857f ;
	ew->dir = ( caldir_pos_256( &ew->pos, &v ) + 4 ) >> 3 ;
	dir_spd_move( ew, 2000 ) ;
	ew->timer1 = 20 ;
	pl_char_set( ew, 22, 24 ) ;
	ew->disp_dir = 0 ;
	ew->cg_zoom = ( ( ew->dir + 8 ) & 31 ) >> 1 ;
	shl36_04_root_set( ew ) ;
	for( i = 2 ; i < 8 ; i ++ ){
		shl36_04_part_set( ew, i ) ;
	}
}

static void shl36_04_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_04_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_04_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_04_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl36_04_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_04_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_04_fly, shl36_04_catch, shl36_04_hold, shl36_04_back,
	} ;

	PL36_LW( pl_w )->shot_timer[0] = 4 ;
	if( SHL36_04_LW( ew )->char_no != pl_w->char_no.w ){
		shl36_04_end( ew, pl_w ) ;
		return ;
	}
	if( pl_w->team_no == 0 && pl_w->act_st == N_SUPER ){
		ef_color_set( pl_w, 1, ( ( Sys->game_timer0 & 2 ) >> 1 ) + 11, 1 ) ;
		shl36_04_move_jmp[ew->routine_1]( ew, pl_w ) ;
		return ;
	}
	shl36_04_end( ew, pl_w ) ;
}

/*==============================================================*/
/*			The hand flies out; it takes the opponent when		*/
/*			it hits												*/
/*==============================================================*/
static void shl36_04_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_flag ){
		if( pl_sp_catch_ck( ew ) ){
			ew->routine_1 ++ ;
			ew->prio = 8 ;
			pl_char_set( ew, 22, 21 ) ;
			return ;
		}
		ew->routine_1 = 3 ;
		ew->timer1 = 0 ;
	} else {
		if( -- ew->timer1 == 0 ){
			ew->routine_1 = 3 ;
			ew->timer1 = 0 ;
		} else {
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			hit_work_set( ew ) ;
		}
	}
}

static void shl36_04_flash_set( PLAYER_WORK *gw );

/*==============================================================*/
/*			The hand has the opponent: it takes him in, then	*/
/*			holds and squeezes									*/
/*==============================================================*/
static void shl36_04_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;

	ew->routine_1 ++ ;
	ew->prio = em_w->prio ;
	pl_w->dm_flag = 0 ;
	pl_w->catch_no = 0xc3 ;
	pl_ef_catch_set( ew, em_w ) ;
	SHL36_04_LW( ew )->em_w = pl_w->catch_w ;
	ew->timer0 = 120 ;
	ew->timer1 = 0 ;
	shl36_04_hold( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl36_04_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 v ;
	PLAYER_WORK *em_w = SHL36_04_LW( ew )->em_w ;

	pl_w->catch_flag = 1 ;
	pl_w->catch_ng_timer = 2 ;
	ew->prio = em_w->prio ;
	v.x = -320.0f ;
	if( pl_w->disp_dir ) v.x = -v.x ;
	v.x += pl_w->pos.x ;
	v.y = pl_w->pos.y + 360.0f ;
	ew->pos.x = ( v.x + ew->pos.x * 7.0f ) / 8.0f ;
	ew->pos.y = ( v.y + ew->pos.y * 7.0f ) / 8.0f ;
	v = pl_w->pos ;
	v.x = 40.0f ;
	if( pl_w->disp_dir ) v.x = -v.x ;
	v.x += pl_w->pos.x ;
	v.y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	ew->dir = ( caldir_pos_256( &ew->pos, &v ) + 4 ) >> 3 ;
	ew->disp_dir = ew->dir >> 4 ;
	pl_char_move( ew ) ;
	catch_pos_set( ew, em_w ) ;
	ew->disp_dir = 0 ;
	ew->cg_zoom = ( ( ew->dir - 8 ) & 31 ) >> 1 ;
	if( ( ew->timer1 += 0x1000 ) == 0 ){
		shl36_04_flash_set( ew ) ;
		vital_add( pl_w, 1 ) ;
		hold_damage( em_w, -2 ) ;
		if( em_w->vital_new == 0 ) ew->timer0 = 0 ;
	}
	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		ew->timer1 = 0 ;
		pl_w->catch_flag = 0 ;
		em_w->dm_hit_w = pl_w ;
		em_w->atck_no = pl36_at_soul_release ;
		em_w->pl_stance = STANCE_AIR ;
		em_w->pl_dir = pl_w->pl_dir ^ 1 ;
		em_w->catch_type = 1 ;
		pl_char_set( ew, 22, 24 ) ;
		ew->disp_dir = 0 ;
		ew->cg_zoom = ( ( ew->dir + 8 ) & 31 ) >> 1 ;
	}
}

/*==============================================================*/
/*			The hand comes back to the player					*/
/*==============================================================*/
static void shl36_04_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x = 40.0f ;
	Float y ;

	if( pl_w->disp_dir ) x = -40.0f ;
	x += pl_w->pos.x ;
	y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	ew->pos.x = ( x + 3.0f * ew->pos.x ) / 4.0f ;
	ew->pos.y = ( y + 3.0f * ew->pos.y ) / 4.0f ;
	if( ( ew->timer1 += 0x800 ) != 0 ) return ;
	shl36_04_end( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl36_04_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl36_04_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_w->timer0 = 50 ;
	work_free( ew ) ;
}

static void shl36_04_part_main( PLAYER_WORK *ew );
static void shl36_04_root_main( PLAYER_WORK *ew );
static void shl36_04_flash_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			A part of the ring: no says how far it trails		*/
/*			the hand											*/
/*==============================================================*/
static void shl36_04_part_set( PLAYER_WORK *gw, int no )
{
	PLAYER_WORK *nw ;

	if( ( nw = work_get( gw, W_SHL, LIST_BEHIND ) ) != 0 ){
		nw->work_func = shl36_04_part_main ;
		nw->master_w = gw ;
		nw->pl_type = gw->pl_type ;
		nw->type = no ;
		nw->ef_no = 0x3604 ;
		SHL36_04_LW( nw )->pl_w = SHL36_04_LW( gw )->pl_w ;
		SHL36_04_PART_SET( nw, SHL36_04_LW( gw )->pl_w ) ;
		nw->disp_flag = 1 ;
		nw->disp_dir = 0 ;
		nw->prio = 8 ;
		ef_char_set( nw, 27, 15 ) ;
	}
}

/*==============================================================*/
static void shl36_04_part_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *gw ;
	PLAYER_WORK *pl_w ;
	Float x ;
	Float y ;
	Float dx ;
	Float dy ;

	ef_char_move( ew ) ;
	pl_w = SHL36_04_LW( ew )->pl_w ;
	gw = ew->master_w ;
	ew->pos = gw->pos ;
	x = 40.0f ;
	if( pl_w->disp_dir ) x = -40.0f ;
	x += pl_w->pos.x ;
	y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	dx = ( x - ew->pos.x ) / 8.0f ;
	dy = ( y - ew->pos.y ) / 8.0f ;
	ew->pos.x += dx * ew->type ;
	ew->pos.y += dy * ew->type ;
	ew->cg_zoom = ( gw->dir & 31 ) >> 1 ;
	if( gw->routine_0 > 1 ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
static void shl36_04_root_set( PLAYER_WORK *gw )
{
	PLAYER_WORK *nw ;

	if( ( nw = work_get( gw, W_SHL, LIST_BEHIND ) ) != 0 ){
		nw->work_func = shl36_04_root_main ;
		nw->master_w = gw ;
		nw->pl_type = gw->pl_type ;
		nw->ef_no = 0x3604 ;
		SHL36_04_LW( nw )->pl_w = SHL36_04_LW( gw )->pl_w ;
		SHL36_04_PART_SET( nw, SHL36_04_LW( gw )->pl_w ) ;
		nw->disp_flag = 1 ;
		nw->sub_prio = -1 ;
		nw->acc.x = 40.0f ;
		nw->acc.y = DOT_Y( 160.0f ) ;
		pl_char_set( nw, 22, 20 ) ;
	}
}

/*==============================================================*/
static void shl36_04_root_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PLAYER_WORK *gw ;
	Float x ;

	pl_char_move( ew ) ;
	pl_w = SHL36_04_LW( ew )->pl_w ;
	ew->prio = pl_w->prio ;
	x = ew->acc.x ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + ew->acc.y ;
	gw = ew->master_w ;
	if( gw->routine_0 > 1 ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*			The flash of the squeeze							*/
/*==============================================================*/
static void shl36_04_flash_set( PLAYER_WORK *gw )
{
	PLAYER_WORK *nw ;

	if( ( nw = work_get( 0, W_SHL, LIST_TOP ) ) != 0 ){
		nw->work_func = shl36_04_flash_main ;
		nw->master_w = gw ;
		nw->pl_type = gw->pl_type ;
		nw->ef_no = 0x3604 ;
		SHL36_04_LW( nw )->pl_w = SHL36_04_LW( gw )->pl_w ;
		SHL36_04_PART_SET( nw, SHL36_04_LW( gw )->pl_w ) ;
		nw->disp_flag = 1 ;
		nw->timer1 = 32 ;
		nw->prio = 8 ;
		nw->pos = gw->pos ;
		pl_char_set( nw, 22, 22 ) ;
		pl_se_req( nw, 9 ) ;
	}
}

/*==============================================================*/
static void shl36_04_flash_main( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = SHL36_04_LW( ew )->pl_w ;
	Float x = 40.0f ;
	Float y ;

	if( pl_w->disp_dir ) x = -40.0f ;
	x += pl_w->pos.x ;
	y = pl_w->pos.y + DOT_Y( 160.0f ) ;
	ew->pos.x = ( x + 3.0f * ew->pos.x ) / 4.0f ;
	ew->pos.y = ( y + 3.0f * ew->pos.y ) / 4.0f ;
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		work_free( ew ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl36_04.c" End of File	*/
