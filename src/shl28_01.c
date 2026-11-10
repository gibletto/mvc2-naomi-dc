/* Gambit's effect 01 (character 28's shot 01), made by his Cajun Explosion's fall: a hit work with seven
   parts beside it (kinds 0 and 1), and the sparks that follow (kinds 2 and 3). */
#include "player.h"
#include "pl_char.h"
#include "pl28_def.h"

/* the effect's own work, from obj_mtx */
typedef struct SHL28_01_LOCAL_W {
	Uint8	unk_00[4];
	Uint16	cnt;		/* 004: parts made */
	Uint8	unk_06[0x0e - 0x06];
	Uint8	pause;		/* 00e: the game is stopped for this player */
	Uint8	unk_0f[0x18 - 0x0f];
	Sint16	*step_p;	/* 018: the next step of the kind-3 table */
} SHL28_01_LOCAL_W;
#define SHL28_01_LW( w )	( ( SHL28_01_LOCAL_W * )&( w )->obj_mtx )

/* what each kind copies from the work it follows */
#define SHL28_01_COPY( ew, mw ) \
	CHAR_PART_COPY( ew, mw ) ; \
	( ew )->disp_flag = 1 ; \
	( ew )->pl_no = ( mw )->pl_no ; \
	( ew )->pl_type = ( mw )->pl_type ; \
	( ew )->scale.x = ( mw )->scale.x ; \
	( ew )->scale.y = ( mw )->scale.y ; \
	( ew )->atck_level = ( mw )->atck_level ; \
	( ew )->work_no = ( mw )->work_no ; \
	( ew )->col_cnt = ( mw )->col_cnt ; \
	( ew )->scale = ( mw )->scale

static void shl28_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl28_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl28_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x2801 ;
		ew->pl_type = pl_w->pl_type ;
		return ew ;
	}
	return 0 ;
}

/*==============================================================*/
/*			A spark that follows the effect hw					*/
/*==============================================================*/
static PLAYER_WORK *shl28_01_part_set( PLAYER_WORK *hw, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl28_01_main ;
		ew->master_w = hw ;
		ew->type = type ;
		ew->ef_no = 0x2801 ;
		ew->pl_type = hw->pl_type ;
		ew->sub_type = 0 ;
		ew->ef_w = hw ;
		return ew ;
	}
	return 0 ;
}

static void shl28_01_0_main( PLAYER_WORK *ew );
static void shl28_01_1_main( PLAYER_WORK *ew );
static void shl28_01_2_main( PLAYER_WORK *ew );
static void shl28_01_3_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_01_main( PLAYER_WORK *ew )
{
	static void ( *shl28_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_01_0_main, shl28_01_1_main, shl28_01_2_main, shl28_01_3_main,
	} ;

	shl28_01_jmp[ew->type]( ew ) ;
}

/* kinds 0 and 1 */
static void shl28_01_init( PLAYER_WORK *ew );
static void shl28_01_move( PLAYER_WORK *ew );
static void shl28_01_end( PLAYER_WORK *ew );
static void shl28_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_01_0_main( PLAYER_WORK *ew )
{
	static void ( *shl28_01_0_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_01_init, shl28_01_move, shl28_01_end, shl28_01_free,
	} ;

	SHL28_01_LOCAL_W *lw = SHL28_01_LW( ew ) ;

	lw->pause = vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ;
	shl28_01_0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_01_init( PLAYER_WORK *ew )
{
	static const Sint16 shl28_01_ofs_tbl[2][2] = { { -8448, 14336 }, { -9984, 16384 } } ;
	SHL28_01_LOCAL_W *lw ;
	PLAYER_WORK *hw ;
	PLAYER_WORK *pw ;
	const Sint16 *ofs ;
	Sint16 i ;

	if( ew->type ) hw = ew->ef_w ;
	else {
		hw = ew ;
		pl_se_req( ew->master_w, 76 ) ;
	}
	lw = SHL28_01_LW( hw ) ;
	if( lw->pause ) return ;
	ew->routine_0 ++ ;
	SHL28_01_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl28_at_explosion ;
	ew->hit_flag = ew->atck_attr = 0 ;
	ew->unk_1c4 = 0 ;
	vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	ew->prio = 0 ;
	if( !ew->type ){
		ew->size_top = 24 ;
		ew->size_bottom = 16 ;
		ew->size_front = 80 ;
		ew->size_back = 80 ;
		pw = ew->ef_w ;
		ew->pos.x = pw->pos.x ;
		ew->pos.y = pw->pos.y ;
		ew->disp_dir = pw->disp_dir ;
		for( i = 0 ; i < 7 ; i ++ ){
			if( ( pw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
				pw->work_func = shl28_01_main ;
				pw->type = 1 ;
				pw->sub_type = i ;
				pw->master_w = ew->master_w ;
				pw->ef_w = ew ;
				pw->ef_no = 0x2801 ;
				pw->pl_type = ew->pl_type ;
			} else break ;
		}
		lw->cnt = i ;
		pl_char_set( ew, 22, 8 ) ;
	}
	else {
		ew->size_top = 32 ;
		ew->size_bottom = 32 ;
		ew->size_front = 80 ;
		ew->size_back = 80 ;
		ew->pos.x = ew->before_w->pos.x ;
		ew->pos.y = ew->before_w->pos.y ;
		ew->disp_dir = ew->before_w->disp_dir ;
		ofs = shl28_01_ofs_tbl[0] ;
		if( ew->before_w->type ) ofs += 2 ;
		if( ew->disp_dir ) ew->pos.x += -( *ofs * 1.6666666f / 256.0f ) ;
		else ew->pos.x += *ofs * 1.6666666f / 256.0f ;
		ofs ++ ;
		ew->pos.y += *ofs * 2.142857f / 256.0f ;
		if( ew->sub_type & 1 ) pl_char_set( ew, 22, 9 ) ;
		else pl_char_set( ew, 22, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl28_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw ;
	SHL28_01_LOCAL_W *lw ;
	PLAYER_WORK *p ;
	Sint16 i ;
	Uint8 hit ;

	hw = ew->type ? ew->ef_w : ew ;
	lw = SHL28_01_LW( hw ) ;
	if( lw->pause ) return ;
	ew->hit_box_no = 0 ;
	if( pl_char_move( ew ) < 0 ){
		shl28_01_end( ew ) ;
		return ;
	}
	else if( !ew->type ){
		if( ew->routine_2 ) return ;
		for( p = ew, i = 0 ; i < lw->cnt ; i ++ ){
			if( ( hit = p->behind_w->hit_flag ) ){
				for( p = ew, i = 0 ; i < lw->cnt ; i ++ ){
					p->hit_flag = hit ;
					p = p->behind_w ;
				}
				ew->routine_2 ++ ;
			}
		}
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl28_01_1_main( PLAYER_WORK *ew )
{
	static void ( *shl28_01_1_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_01_init, shl28_01_move, shl28_01_end, shl28_01_free,
	} ;

	shl28_01_1_jmp[ew->routine_0]( ew ) ;
}

/* kind 2 */
static void shl28_01_2_init( PLAYER_WORK *ew );
static void shl28_01_2_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_01_2_main( PLAYER_WORK *ew )
{
	static void ( *shl28_01_2_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_01_2_init, shl28_01_2_move, shl28_01_end, shl28_01_free,
	} ;

	shl28_01_2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_01_2_init( PLAYER_WORK *ew )
{
	Sint32 rnd ;

	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		ew->routine_0 ++ ;
		ew->routine_1 = ew->routine_2 = 0 ;
		SHL28_01_COPY( ew, ew->ef_w ) ;
		ew->prio = ew->ef_w->prio ;
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl28_at_explosion ;
		HIT_RESET( ew ) ;
		ew->size_top = ew->size_front = ew->size_back = 96 ;
		ew->size_bottom = 32 ;
		ew->prio = 0 ;
		rnd = nlRand() ;
		if( rnd & 1 ) pl_char_set( ew, 22, 12 ) ;
		else pl_char_set( ew, 22, 13 ) ;
		ew->disp_flag = 1 ;
	}
	else ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl28_01_2_move( PLAYER_WORK *ew )
{
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( !ew->routine_1 ){
			if( pl_char_move( ew ) < 0 ){
				ew->routine_1 ++ ;
				pl_char_set( ew, 22, 11 ) ;
			}
		}
		else {
			if( pl_char_move( ew ) < 0 ){
				shl28_01_end( ew ) ;
				return ;
			}
			if( ew->cg_type & 1 ){
				ew->cg_type ^= 1 ;
				shl28_01_part_set( ew, 0 ) ;
			}
			hit_work_set( ew ) ;
		}
	}
}

/* kind 3 */
static void shl28_01_3_init( PLAYER_WORK *ew );
static void shl28_01_3_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl28_01_3_main( PLAYER_WORK *ew )
{
	static void ( *shl28_01_3_jmp[] )( PLAYER_WORK *ew ) = {
		shl28_01_3_init, shl28_01_3_move, shl28_01_end, shl28_01_free,
	} ;

	shl28_01_3_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl28_01_3_init( PLAYER_WORK *ew )
{
	static Sint16 shl28_01_step_tbl[] = {
		4, 1,
		-16384, 8, -28672, 8, -28672, 8, -28672, 8, -28672, 0,
	} ;
	SHL28_01_LOCAL_W *lw ;
	PLAYER_WORK *mw ;

	ew->routine_0 ++ ;
	SHL28_01_COPY( ew, ew->master_w ) ;
	ew->prio = ew->master_w->prio ;
	ew->disp_flag = 0 ;
	mw = ew->master_w ;
	ew->pos.x = mw->pos.x ;
	ew->pos.y = mw->ground_y ;
	ew->timer1 = shl28_01_step_tbl[0] ;
	ew->timer0 = shl28_01_step_tbl[1] ;
	lw = SHL28_01_LW( ew ) ;
	lw->step_p = &shl28_01_step_tbl[2] ;
	shl28_01_3_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_01_3_move( PLAYER_WORK *ew )
{
	SHL28_01_LOCAL_W *lw ;
	PLAYER_WORK *pw ;
	Sint16 *p ;
	Sint16 step ;

	if( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) return ;
	ew->timer0 -- ;
	if( ew->timer0 >= 0 ) return ;
	lw = SHL28_01_LW( ew ) ;
	p = lw->step_p ;
	step = *p ++ ;
	ew->timer0 = *p ++ ;
	lw->step_p = p ;
	if( ew->disp_dir ) step = -step ;
	ew->pos.x += step * 1.6666666f / 256.0f ;
	pw = shl28_01_part_set( ew, 2 ) ;
	if( !pw ) return ;
	pw->pos.x = ew->pos.x ;
	pw->pos.y = ew->pos.y ;
	ew->timer1 -- ;
	if( ew->timer1 > 0 ) return ;
	shl28_01_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl28_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl28_01_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl28_01.c" End of File	*/
