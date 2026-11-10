/* Charlie's effect 00 (character 21's shot 00): everything that flies from or rides on him, in
   kinds by type. Types 0 and 10 are the Sonic Boom, types 1 to 6 and 9 an animation that stays
   on him while his own does, types 7 and 8 the shots of the Somersault Shell and Somersault
   Justice, which fly up and ahead, and type 11 the three booms of Sonic Break that weave. */
#include "player.h"
#include "pl_char.h"
#include "pl21_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL21_00_LOCAL_W {
	Uint16	char_no;	/* 00: Charlie's animation when the animation was set */
} SHL21_00_LOCAL_W;
#define SHL21_00_LW( w )	( ( SHL21_00_LOCAL_W * )&( w )->obj_flag )

static SHL21_00_LOCAL_W *shl21_00_lw ;
static PL21_LOCAL_W *shl21_00_pl_lw ;

static void shl21_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl21_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl21_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x2100 ;
	}
	return ew ;
}

static void shl21_00_sonic_main( PLAYER_WORK *ew );
static void shl21_00_follow_main( PLAYER_WORK *ew );
static void shl21_00_smr_main( PLAYER_WORK *ew );
static void shl21_00_justice_main( PLAYER_WORK *ew );
static void shl21_00_break_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl21_00_main( PLAYER_WORK *ew )
{
	static void ( *shl21_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl21_00_sonic_main, shl21_00_follow_main, shl21_00_follow_main, shl21_00_follow_main,
		shl21_00_follow_main, shl21_00_follow_main, shl21_00_follow_main, shl21_00_smr_main,
		shl21_00_justice_main, shl21_00_follow_main, shl21_00_sonic_main, shl21_00_break_main,
	} ;

	shl21_00_pl_lw = ( PL21_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl21_00_lw = SHL21_00_LW( ew ) ;
	shl21_00_jmp[ew->type]( ew ) ;
}

static void shl21_00_sonic_init( PLAYER_WORK *ew );
static void shl21_00_sonic_move( PLAYER_WORK *ew );
static void shl21_00_sonic_burst( PLAYER_WORK *ew );
static void shl21_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl21_00_sonic_main( PLAYER_WORK *ew )
{
	static void ( *shl21_00_sonic_jmp[] )( PLAYER_WORK *ew ) = {
		shl21_00_sonic_init, shl21_00_sonic_move, shl21_00_sonic_burst, shl21_00_free,
	} ;

	shl21_00_sonic_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl21_00_sonic_init( PLAYER_WORK *ew )
{
	static Float spd_tbl[2] = { DOT_X( 2.5f ), 7.5f } ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_dir = ew->master_w->disp_dir ;
	shl21_00_pl_lw->shot_flag = 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->atck_level ? 50 : 48 ;
	HIT_RESET( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->master_w->pl_dir ? DOT_X( 40.0f ) : -DOT_X( 40.0f ) ;
	ew->pos.y += DOT_Y( 72.0f ) ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->master_w->pl_dir ? spd_tbl[ew->atck_level] : -spd_tbl[ew->atck_level] ;
	ew->prio = 0 ;
	pl_char_set( ew, 20, 0 ) ;
}

static void shl21_00_sonic_hit( PLAYER_WORK *ew );
static void shl21_00_end( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl21_00_sonic_move( PLAYER_WORK *ew )
{
	if( ew->hit_flag || ew->hit_box_no ){
		shl21_00_sonic_hit( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl21_00_pl_lw->shot_flag = 0 ;
		shl21_00_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_sonic_burst( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		shl21_00_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
}

/*==============================================================*/
static void shl21_00_sonic_hit( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	shl21_00_pl_lw->shot_flag = 0 ;
	pl_char_set( ew, 20, 1 ) ;
}

static void shl21_00_follow_init( PLAYER_WORK *ew );
static void shl21_00_follow_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl21_00_follow_main( PLAYER_WORK *ew )
{
	static void ( *shl21_00_follow_jmp[] )( PLAYER_WORK *ew ) = {
		shl21_00_follow_init, shl21_00_follow_move, shl21_00_end, shl21_00_free,
	} ;

	shl21_00_follow_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl21_00_follow_init( PLAYER_WORK *ew )
{
	static Uint32 no_tbl[10] = { 0, 0, 1, 2, 3, 4, 5, 5, 5, 6 } ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_dir = ew->master_w->disp_dir ;
	shl21_00_lw->char_no = ew->master_w->char_no.w ;
	ew->prio = 0 ;
	pl_char_set( ew, Char_Effect, no_tbl[ew->type] ) ;
	shl21_00_follow_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_follow_move( PLAYER_WORK *ew )
{
	Sint8 n ;

	if( shl21_00_lw->char_no != ew->master_w->char_no.w ){
		shl21_00_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->disp_flag = 0 ;
	if( !( n = HI_BYTE( ew->master_w->cg_rival ) & 0xfe ) ) return ;
	if( n < 0 ){
		shl21_00_end( ew ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	while( n != HI_BYTE( ew->cg_rival ) ) pl_char_move( ew ) ;
}

static void shl21_00_smr_init( PLAYER_WORK *ew );
static void shl21_00_smr_move( PLAYER_WORK *ew );
static void shl21_00_smr_burst( PLAYER_WORK *ew );

/*==============================================================*/
static void shl21_00_smr_main( PLAYER_WORK *ew )
{
	static void ( *shl21_00_smr_jmp[] )( PLAYER_WORK *ew ) = {
		shl21_00_smr_init, shl21_00_smr_move, shl21_00_smr_burst, shl21_00_free,
	} ;

	shl21_00_smr_jmp[ew->routine_0]( ew ) ;
}

/* a pair of speeds for each level */
typedef struct SHL21_00_SMR_SPD {
	Float	x, y;
} SHL21_00_SMR_SPD;

/*==============================================================*/
static void shl21_00_smr_init( PLAYER_WORK *ew )
{
	static SHL21_00_SMR_SPD spd_tbl[2] = {
		{ DOT_X( 4.0f ), DOT_Y( 4.5f ) }, { DOT_X( 8.0f ), DOT_Y( 8.5f ) },
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->master_w->pl_dir ? DOT_X( 94.0f ) : -DOT_X( 94.0f ) ;
	ew->pos.y += DOT_Y( 121.0f ) ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->prio = 0 ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl21_at_smr_counter_ef ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = ew->atck_level ? 68 : 66 ;
		HIT_RESET( ew ) ;
	}
	ew->spd.x = ew->disp_dir ? spd_tbl[ew->atck_level].x : -spd_tbl[ew->atck_level].x ;
	ew->spd.y = spd_tbl[ew->atck_level].y ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, 20, 5 ) ;
	shl21_00_smr_move( ew ) ;
}

static void shl21_00_land( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl21_00_smr_move( PLAYER_WORK *ew )
{
	if( ew->hit_box_no ){
		shl21_00_land( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl21_00_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		shl21_00_land( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_smr_burst( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl21_00_end( ew ) ;
}

/*==============================================================*/
static void shl21_00_land( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	pl_char_set( ew, 20, 6 ) ;
}

static void shl21_00_justice_init( PLAYER_WORK *ew );
static void shl21_00_justice_move( PLAYER_WORK *ew );
static void shl21_00_justice_burst( PLAYER_WORK *ew );

/*==============================================================*/
static void shl21_00_justice_main( PLAYER_WORK *ew )
{
	static void ( *shl21_00_justice_jmp[] )( PLAYER_WORK *ew ) = {
		shl21_00_justice_init, shl21_00_justice_move, shl21_00_justice_burst, shl21_00_free,
	} ;

	shl21_00_justice_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl21_00_justice_init( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl21_at_justice_ef ;
	HIT_RESET( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.x += ew->disp_dir ? DOT_X( 94.0f ) : -DOT_X( 94.0f ) ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y += DOT_Y( 121.0f ) ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	ew->spd.x = ew->disp_dir ? 20.0f : -20.0f ;
	ew->spd.y = DOT_Y( 8.5f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->prio = 0 ;
	pl_char_set( ew, 20, 7 ) ;
	shl21_00_justice_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_justice_move( PLAYER_WORK *ew )
{
	if( ew->hit_box_no ){
		shl21_00_land( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl21_00_end( ew ) ;
		return ;
	}
	ew->prio = 0 ;
	if( ew->pos.y <= ew->master_w->ground_y ){
		shl21_00_land( ew ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_justice_burst( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl21_00_end( ew ) ;
}

static void shl21_00_break_init( PLAYER_WORK *ew );
static void shl21_00_break_move( PLAYER_WORK *ew );
static void shl21_00_break_burst( PLAYER_WORK *ew );

/*==============================================================*/
static void shl21_00_break_main( PLAYER_WORK *ew )
{
	static void ( *shl21_00_break_jmp[] )( PLAYER_WORK *ew ) = {
		shl21_00_break_init, shl21_00_break_move, shl21_00_break_burst, shl21_00_free,
	} ;

	shl21_00_break_jmp[ew->routine_0]( ew ) ;
}

/* where each of the three starts, from Charlie */
typedef struct SHL21_00_BREAK_POS {
	Float	x, y;
} SHL21_00_BREAK_POS;

/*==============================================================*/
static void shl21_00_break_init( PLAYER_WORK *ew )
{
	static Uint8 dir_tbl[4] = { 8, 8, 24, 0 } ;
	static SHL21_00_BREAK_POS pos_tbl[3] = {
		{ DOT_X( 64.0f ), DOT_Y( 64.0f ) }, { DOT_X( 32.0f ), DOT_Y( 96.0f ) }, { DOT_X( 32.0f ), DOT_Y( 32.0f ) },
	} ;

	ew->routine_0 ++ ;
	CHAR_PART_COPY( ew, ew->master_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = ew->master_w->pl_no ;
	ew->pl_type = ew->master_w->pl_type ;
	ew->scale.x = ew->master_w->scale.x ;
	ew->scale.y = ew->master_w->scale.y ;
	ew->atck_level = ew->master_w->atck_level ;
	ew->work_no = ew->master_w->work_no ;
	ew->col_cnt = ew->master_w->col_cnt ;
	ew->scale = ew->master_w->scale ;
	ew->prio = ew->master_w->prio ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = ew->sub_type ? 80 : 79 ;
	HIT_RESET( ew ) ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->disp_dir ? pos_tbl[ew->sub_type].x : -pos_tbl[ew->sub_type].x ;
	ew->pos.y += pos_tbl[ew->sub_type].y ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? DOT_X( 4.0f ) : -DOT_X( 4.0f ) ;
	ew->prio = 0 ;
	ew->timer0 = ew->timer1 = 2 ;
	ew->dir = dir_tbl[ew->sub_type] ;
	pl_char_set( ew, 20, 0 ) ;
}

static void shl21_00_break_hit( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl21_00_break_move( PLAYER_WORK *ew )
{
	if( ew->hit_box_no || ew->hit_flag ){
		shl21_00_break_hit( ew ) ;
		return ;
	}
	if( !scr_in_x_ck( ew ) ){
		shl21_00_end( ew ) ;
		return ;
	}
	if( ew->sub_type ){
		ew->pos.y += nlSin( ( ( 40 - ew->dir ) & 31 ) << 11 ) * DOT_Y( 4.0f ) ;
		if( ! -- ew->timer0 ){
			ew->timer0 = ew->timer1 ;
			ew->dir = ++ ew->dir & 31 ;
		}
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_break_burst( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl21_00_end( ew ) ;
}

/*==============================================================*/
static void shl21_00_break_hit( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	pl_char_set( ew, 20, 1 ) ;
}

/*--------------------------------------------------------------*/
static void shl21_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl21_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl21_00.c" End of File	*/
