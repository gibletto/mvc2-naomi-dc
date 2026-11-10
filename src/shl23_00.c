/* Dan's effect 00 (character 23's shot 00), in three kinds by type: type 0 the Gadouken, a slow shot
   that bursts when it hits or times out, type 1 the Shinkuu Gadouken, which can hit several times
   before it ends, and type 2 Premium Sign's piece, which is thrown, falls to the ground and blinks
   out. Each runs through its own routine_1 steps; routine_0 ends it. */
#include "player.h"
#include "pl_char.h"
#include "pl23_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL23_00_LOCAL_W {
	Uint16	char_no;	/* 00: Dan's animation when the shot was set */
	Uint8	box_cnt;	/* 02: type 1: the boxes it may be stopped by */
	Uint8	hit_cnt;	/* 03: and the hits it may land */
} SHL23_00_LOCAL_W;
#define SHL23_00_LW( w )	( ( SHL23_00_LOCAL_W * )&( w )->obj_flag )

/* the speeds of the falling piece by level: x, its change, y, its change */
static Float shl23_00_spd_tbl[2][4] = {
	{ -DOT_X( 4.0f ), 0.104166664f, -2.142857f, -0.13392857f },
	{ -DOT_X( 8.0f ), 0.104166664f, -DOT_Y( 4.0f ), 0.6696428f },
} ;

static void shl23_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl23_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;
	SHL23_00_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = shl23_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x2300 ;
		ew->pl_type = pl_w->pl_type ;
		lw = SHL23_00_LW( ew ) ;
		lw->char_no = pl_w->char_no.w ;
		return ew ;
	}
	return 0 ;
}

static void shl23_00_start( PLAYER_WORK *ew );
static void shl23_00_kind_main( PLAYER_WORK *ew );
static void shl23_00_end( PLAYER_WORK *ew );
static void shl23_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl23_00_main( PLAYER_WORK *ew )
{
	static void ( *shl23_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl23_00_start, shl23_00_kind_main, shl23_00_end, shl23_00_free,
	} ;

	shl23_00_jmp[ew->routine_0]( ew ) ;
}

static void shl23_00_gadou_main( PLAYER_WORK *ew );
static void shl23_00_shinkuu_main( PLAYER_WORK *ew );
static void shl23_00_sign_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl23_00_start( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	shl23_00_kind_main( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_kind_main( PLAYER_WORK *ew )
{
	static void ( *shl23_00_kind_jmp[] )( PLAYER_WORK *ew ) = {
		shl23_00_gadou_main, shl23_00_shinkuu_main, shl23_00_sign_main,
	} ;

	shl23_00_kind_jmp[ew->type]( ew ) ;
}

static void shl23_00_gadou_init( PLAYER_WORK *ew );
static void shl23_00_gadou_move( PLAYER_WORK *ew );
static void shl23_00_gadou_burst( PLAYER_WORK *ew );

/*==============================================================*/
static void shl23_00_gadou_main( PLAYER_WORK *ew )
{
	static void ( *shl23_00_gadou_jmp[] )( PLAYER_WORK *ew ) = {
		shl23_00_gadou_init, shl23_00_gadou_move, shl23_00_gadou_burst,
	} ;

	shl23_00_gadou_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl23_00_gadou_init( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
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
	ew->disp_flag = 1 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 65.0f ) ;
	if( ew->disp_dir ){
		ew->pos.x -= -DOT_X( 98.0f ) ;
		ew->spd.x = DOT_X( 0.5f ) ;
	} else {
		ew->pos.x -= DOT_X( 98.0f ) ;
		ew->spd.x = -DOT_X( 0.5f ) ;
	}
	if( !ew->atck_level ){
		ew->timer0 = 4 ;
		ew->atck_no = pl23_at_gadou_l ;
		HIT_RESET( ew ) ;
	} else {
		ew->timer0 = 16 ;
		ew->atck_no = pl23_at_gadou_m ;
		HIT_RESET( ew ) ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_gadou_move( PLAYER_WORK *ew )
{
	SHL23_00_LOCAL_W *lw = SHL23_00_LW( ew ) ;

	if( lw->char_no != ew->master_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = 0 ;
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_1 ++ ;
		ew->master_w->hit_stop = ew->hit_stop ;
		pl_char_set( ew, Char_Effect, 5 ) ;
		return ;
	}
	ew->timer0 -- ;
	if( ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 4 ) ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_gadou_burst( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 2 ;
	}
}

static void shl23_00_shinkuu_init( PLAYER_WORK *ew );
static void shl23_00_shinkuu_move( PLAYER_WORK *ew );
static void shl23_00_shinkuu_burst( PLAYER_WORK *ew );

/*==============================================================*/
static void shl23_00_shinkuu_main( PLAYER_WORK *ew )
{
	static void ( *shl23_00_shinkuu_jmp[] )( PLAYER_WORK *ew ) = {
		shl23_00_shinkuu_init, shl23_00_shinkuu_move, shl23_00_shinkuu_burst,
	} ;

	shl23_00_shinkuu_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl23_00_shinkuu_init( PLAYER_WORK *ew )
{
	SHL23_00_LOCAL_W *lw = SHL23_00_LW( ew ) ;

	ew->routine_1 ++ ;
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
	ew->disp_flag = 1 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	lw->box_cnt = 2 ;
	lw->hit_cnt = 8 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 65.0f ) ;
	if( ew->disp_dir ){
		ew->pos.x -= -DOT_X( 74.0f ) ;
		ew->spd.x = 2.5f ;
	} else {
		ew->pos.x -= DOT_X( 74.0f ) ;
		ew->spd.x = -2.5f ;
	}
	ew->timer0 = 60 ;
	ew->atck_no = pl23_at_shinkuu ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_shinkuu_move( PLAYER_WORK *ew )
{
	SHL23_00_LOCAL_W *lw = SHL23_00_LW( ew ) ;

	ew->prio = 0 ;
	if( ew->hit_stop ){
		ew->hit_stop -- ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( ew->hit_flag ){
		ew->hit_stop = 4 ;
		lw->hit_cnt -- ;
		if( lw->hit_cnt <= 0 ) goto end ;
		else {
			ew->atck_no = pl23_at_shinkuu ;
			HIT_RESET( ew ) ;
		}
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		lw->box_cnt -- ;
		if( lw->box_cnt <= 0 ) goto end ;
	}
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ) goto end ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
	return ;
end:
	ew->routine_1 ++ ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_shinkuu_burst( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

static void shl23_00_sign_init( PLAYER_WORK *ew );
static void shl23_00_sign_move( PLAYER_WORK *ew );
static void shl23_00_sign_blink( PLAYER_WORK *ew );

/*==============================================================*/
static void shl23_00_sign_main( PLAYER_WORK *ew )
{
	static void ( *shl23_00_sign_jmp[] )( PLAYER_WORK *ew ) = {
		shl23_00_sign_init, shl23_00_sign_move, shl23_00_sign_blink,
	} ;

	shl23_00_sign_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl23_00_sign_init( PLAYER_WORK *ew )
{
	Float *spd ;

	ew->routine_1 ++ ;
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
	spd = shl23_00_spd_tbl[ew->master_w->atck_level] ;
	ew->spd.x = *spd ++ ;
	ew->acc.x = *spd ++ ;
	ew->spd.y = *spd ++ ;
	ew->acc.y = *spd ;
	ew->pos = ew->master_w->pos ;
	if( ew->disp_dir ){
		ew->pos.x -= -DOT_X( 82.0f ) ;
		ew->spd.x = -ew->spd.x ;
		ew->acc.x = -ew->acc.x ;
	} else {
		ew->pos.x -= DOT_X( 82.0f ) ;
	}
	ew->pos.y += DOT_Y( 72.0f ) ;
	if( ew->master_w->atck_level == 0 ){
		ew->atck_no = pl23_at_sign_hit ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, Char_Effect, 6 ) ;
	} else {
		ew->atck_no = pl23_at_sign_piece ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, Char_Effect, 8 ) ;
	}
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->size_top = ew->size_bottom = 48 ;
	ew->size_back = ew->size_front = 64 ;
}

static void shl23_00_sign_fly( PLAYER_WORK *ew );
static void shl23_00_sign_bound( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl23_00_sign_move( PLAYER_WORK *ew )
{
	static void ( *shl23_00_sign_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl23_00_sign_fly, shl23_00_sign_bound,
	} ;

	ew->prio = 0 ;
	shl23_00_sign_move_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void shl23_00_sign_fly( PLAYER_WORK *ew )
{
	Float gy ;

	if( ew->hit_flag ) goto hit ;
	if( ew->hit_box_no ) goto hit ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		return ;
	}
	gy = ew->master_w->ground_y ;
	if( ew->pos.y < gy ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
	} else {
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
		return ;
	}
hit:
	ew->routine_2 ++ ;
	ew->timer0 = 30 ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_sign_bound( PLAYER_WORK *ew )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	} else if( ew->timer0 & 1 ){
		ew->disp_flag = 1 ;
		pl_char_move( ew ) ;
	} else {
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl23_00_sign_blink( PLAYER_WORK *ew )
{
	ew->prio = 0 ;
	ew->timer0 -- ;
	if( ew->timer0 <= 0 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->timer0 < 30 ){
		if( ew->timer0 & 1 ) ew->disp_flag = 0 ;
		else {
			ew->disp_flag = 0 ;
			return ;
		}
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl23_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_0 = 3 ;
}

/*--------------------------------------------------------------*/
static void shl23_00_free( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl23_00.c" End of File	*/
