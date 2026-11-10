/* Blackheart's effect 03 (character 35's shot 03): the fiends of his Inferno. Six kinds,
   by type: the four that fly to the place and gather (0), two still pictures there (1 and 2),
   the pillars that hit (3 and 4), and the pieces that rise in them (5). */
#include "player.h"
#include "pl_char_old.h"
#include "pl35_def.h"

/* the effect's own work, from obj_flag: where the first kind gathers */
typedef struct SHL35_03_LOCAL_W {
	Sint32	unk_00;
	Float	x;
	Float	y;
} SHL35_03_LOCAL_W;
#define SHL35_03_LW( w )	( ( SHL35_03_LOCAL_W * )&( w )->obj_flag )

static void shl35_03_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			A new one beside his master's effect; `no` is		*/
/*			its sub_type										*/
/*==============================================================*/
static PLAYER_WORK *shl35_03_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew, *mw ;

	if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl35_03_main ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->master_w = pl_w ;
		ew->ef_w = pl_w->ef_w ;
		mw = pl_w->ef_w ;
		ew->pl_type = mw->pl_type ;
		ew->ef_no = 0x3503 ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

/****************************************************************/
/*			The four that gather, from Blackheart				*/
/****************************************************************/
PLAYER_WORK *shl35_03_start( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint16 i ;

	for( i = 0 ; i < 4 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
			ew->work_func = shl35_03_main ;
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->master_w = pl_w ;
			ew->ef_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			ew->ef_no = 0x3503 ;
			ew->type = 0 ;
			ew->sub_type = i ;
		}
	}
	return ew ;
}

/*==============================================================*/
/*			Eight of one kind at once							*/
/*==============================================================*/
static PLAYER_WORK *shl35_03_set8( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew, *mw ;
	Sint16 i ;

	for( i = 0 ; i < 8 ; i ++ ){
		if( ( ew = work_get( pl_w, W_SHL, LIST_BEHIND ) ) ){
			ew->work_func = shl35_03_main ;
			ew->pos.x = pl_w->pos.x ;
			ew->pos.y = pl_w->pos.y ;
			ew->master_w = pl_w ;
			ew->ef_w = pl_w->ef_w ;
			mw = pl_w->ef_w ;
			ew->pl_type = mw->pl_type ;
			ew->ef_no = 0x3503 ;
			ew->type = type ;
			ew->sub_type = no ;
		}
	}
	return ew ;
}

static void shl35_03_dummy( PLAYER_WORK *ew );
static void shl35_03_init( PLAYER_WORK *ew );
static void shl35_03_move( PLAYER_WORK *ew );
static void shl35_03_end( PLAYER_WORK *ew );
static void shl35_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_03_main( PLAYER_WORK *ew )
{
	static void ( *shl35_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_03_init, shl35_03_move, shl35_03_end, shl35_03_free,
	} ;

	shl35_03_dummy( ew ) ;
	shl35_03_jmp[ew->routine_0]( ew ) ;
}

static void shl35_03_base_init( PLAYER_WORK *ew );
static void shl35_03_init_1( PLAYER_WORK *ew );
static void shl35_03_init_2( PLAYER_WORK *ew );
static void shl35_03_pillar_init( PLAYER_WORK *ew );
static void shl35_03_pillar_up_init( PLAYER_WORK *ew );
static void shl35_03_piece_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_03_init( PLAYER_WORK *ew )
{
	static void ( *shl35_03_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_03_base_init, shl35_03_init_1, shl35_03_init_2,
		shl35_03_pillar_init, shl35_03_pillar_up_init, shl35_03_piece_init,
	} ;

	ew->routine_0 ++ ;
	shl35_03_init_jmp[ew->type]( ew ) ;
}

static void shl35_03_copy( PLAYER_WORK *ew );

/*==============================================================*/
/*			The four that gather: each starts a little			*/
/*			further along its way to the place					*/
/*==============================================================*/
static void shl35_03_base_init( PLAYER_WORK *ew )
{
	SHL35_03_LOCAL_W *lw = SHL35_03_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	shl35_03_copy( ew ) ;
	ew->prio = 11 ;
	lw->y = pl_w->ground_y ;
	ew->timer1 = ew->sub_type + 1 ;
	x = DOT_X( 32.0f ) ;
	if( pl_w->pl_dir ) x = -DOT_X( 32.0f ) ;
	ew->pos.x -= x ;
	ew->pos.y += 240.0f ;
	pl_char_set( ew, Char_Effect, ew->sub_type + 12 ) ;
	if( !pl_w->atck_level ){
		x = 160.0f ;
		if( !pl_w->pl_dir ) x = -160.0f ;
		lw->x = pl_w->pos.x + x ;
	}
	else{
		PLAYER_WORK *tw = pl_w->target_w ;
		lw->x = tw->pos.x ;
	}
	ew->acc.x = ew->acc.y = 0.0f ;
	ew->spd.x = ( lw->x - ew->pos.x ) / 64.0f ;
	ew->spd.y = ( lw->y - ew->pos.y ) / 64.0f ;
	ew->pos.x += ew->spd.x * ew->sub_type * 16.0f ;
	ew->pos.y += ew->spd.y * ew->sub_type * 16.0f ;
}

/*==============================================================*/
static void shl35_03_init_1( PLAYER_WORK *ew )
{
	shl35_03_copy( ew ) ;
	ew->prio = 12 ;
	pl_char_set( ew, 21, 13 ) ;
}

/*==============================================================*/
static void shl35_03_init_2( PLAYER_WORK *ew )
{
	shl35_03_copy( ew ) ;
	ew->prio = 9 ;
	pl_char_set( ew, 21, 19 ) ;
}

/*==============================================================*/
/*			The pillars that hit, at the place					*/
/*==============================================================*/
static void shl35_03_pillar_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	shl35_03_copy( ew ) ;
	ew->prio = 10 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	/* the body's bytes are stored with repeats */
	ew->size_top = ew->size_bottom = 212 ;
	ew->size_bottom = ew->size_bottom = 1 ;
	ew->size_back = ew->size_front = ew->size_bottom = 64 ;
	ew->size_back = 64 ;
	ew->disp_flag = 1 ;
	{
		PLAYER_WORK *mw = ew->ef_w ;

		if( mw->team_type == 8 ){
			ew->atck_no = pl35_at_inferno_assist ;
			HIT_RESET( ew ) ;
		} else if( mw->team_type == 3 ){
			ew->atck_no = pl35_at_inferno_counter ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl35_at_inferno ;
			HIT_RESET( ew ) ;
		}
	}
	pl_char_set( ew, 21, ew->sub_type + 17 ) ;
}

/*==============================================================*/
static void shl35_03_pillar_up_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	shl35_03_copy( ew ) ;
	ew->prio = 10 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	/* the body's bytes are stored with repeats */
	ew->size_top = ew->size_bottom = 212 ;
	ew->size_bottom = ew->size_bottom = 1 ;
	ew->size_back = ew->size_front = ew->size_bottom = 64 ;
	ew->size_back = 64 ;
	ew->disp_flag = 0 ;
	ew->pos.y += 480.0f ;
	ew->timer0 = 2 ;
	{
		PLAYER_WORK *mw = ew->ef_w ;

		if( mw->team_type == 8 ){
			ew->atck_no = pl35_at_inferno_assist ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl35_at_inferno ;
			HIT_RESET( ew ) ;
		}
	}
	if( !ew->sub_type ) pl_char_set( ew, 21, 21 ) ;
	else pl_char_set( ew, 21, ew->sub_type + 17 ) ;
}

/*==============================================================*/
/*			The pieces rise from random places in a pillar		*/
/*==============================================================*/
static void shl35_03_piece_init( PLAYER_WORK *ew )
{
	static Uint8 shl35_03_anim_tbl[4] = { 19, 20, 21, 19 } ;
	static Float shl35_03_spd_tbl[4] = { 1.6666666f, DOT_X( 1.25f ), 2.5f, DOT_X( 1.75f ) } ;

	shl35_03_copy( ew ) ;
	ew->prio = 11 ;
	ew->disp_flag = 1 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->pos.y += DOT_Y( 96.0f ) ;
	ew->pos.x += -DOT_X( 32.0f ) + ( Sint32 )( Rnd() & 63 ) * 1.6666666f ;
	ew->pos.y += -DOT_Y( 64.0f ) + ( Sint32 )( Rnd() & 127 ) * 2.142857f ;
	ew->spd.y = shl35_03_spd_tbl[Rnd() & 3] ;
	pl_char_set( ew, Char_Effect, shl35_03_anim_tbl[Rnd() & 3] ) ;
}

static void shl35_03_base_move( PLAYER_WORK *ew );
static void shl35_03_move_1( PLAYER_WORK *ew );
static void shl35_03_move_2( PLAYER_WORK *ew );
static void shl35_03_pillar_move( PLAYER_WORK *ew );
static void shl35_03_pillar_up_move( PLAYER_WORK *ew );
static void shl35_03_piece_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl35_03_move( PLAYER_WORK *ew )
{
	static void ( *shl35_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_03_base_move, shl35_03_move_1, shl35_03_move_2,
		shl35_03_pillar_move, shl35_03_pillar_up_move, shl35_03_piece_move,
	} ;

	shl35_03_move_jmp[ew->type]( ew ) ;
}

static void shl35_03_base_wait( PLAYER_WORK *ew );
static void shl35_03_base_fly( PLAYER_WORK *ew );
static void shl35_03_base_start( PLAYER_WORK *ew );
static void shl35_03_base_shot( PLAYER_WORK *ew );
static void shl35_03_base_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void shl35_03_base_move( PLAYER_WORK *ew )
{
	static void ( *shl35_03_base_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl35_03_base_wait, shl35_03_base_fly, shl35_03_base_start,
		shl35_03_base_shot, shl35_03_base_keep,
	} ;

	shl35_03_base_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl35_03_base_wait( PLAYER_WORK *ew )
{
	if( ! -- ew->timer1 ){
		ew->routine_1 ++ ;
		ew->timer1 = 4 ;
	}
}

/*==============================================================*/
/*			They gather, and then the first of them makes		*/
/*			the strikes											*/
/*==============================================================*/
static void shl35_03_base_fly( PLAYER_WORK *ew )
{
	SHL35_03_LOCAL_W *lw = SHL35_03_LW( ew ) ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ! -- ew->timer1 ){
		if( ew->sub_type ){
			ew->routine_0 ++ ;
			ew->routine_1 = 0 ;
			ew->disp_flag = 0 ;
		}
		else{
			ew->routine_1 ++ ;
			ew->disp_flag = 1 ;
			ew->prio = 13 ;
			ew->pos.x = lw->x ;
			ew->pos.y = lw->y ;
			pl_char_set( ew, 21, 15 ) ;
			shl35_03_set( ew, 1, 0 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl35_03_base_start( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_1 == 0 ){
		pl_char_move( ew ) ;
		if( ew->cg_type ){
			ew->routine_1 ++ ;
			ew->cg_type = 0 ;
			shl35_03_set( ew, 2, 0 ) ;
		}
	}
	else{
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_03_base_shot( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->ef_w ;

	if( pl_w->routine_1 == 0 ){
		pl_char_move( ew ) ;
		if( ew->cg_type ){
			ew->routine_1 ++ ;
			ew->cg_type = 0 ;
			ew->timer1 = 62 ;
			shl35_03_set( ew, 3, 0 ) ;
			shl35_03_set( ew, 4, 0 ) ;
			shl35_03_set( ew, 3, 1 ) ;
			shl35_03_set( ew, 4, 1 ) ;
			pl_se_req( ew, 76 ) ;
		}
	}
	else{
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_03_base_keep( PLAYER_WORK *ew )
{
	if( ! -- ew->timer1 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			The others stay for as long as his master is in		*/
/*			the move											*/
/*==============================================================*/
static void shl35_03_move_1( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl35_03_move_2( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 != 1 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl35_03_pillar_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 == 1 ){
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
		if( ew->cg_type ){
			ew->cg_type = 0 ;
			shl35_03_set8( ew, 5, 0 ) ;
		}
	}
	else{
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void shl35_03_pillar_up_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->timer0 ) ew->timer0 -- ;
	else{
		if( pl_w->routine_0 == 1 ){
			ew->disp_flag = 1 ;
			if( scr_work.scr_bottom > 480.0f ) ew->pos.y = scr_work.scr_top + -480.0f ;
			else ew->pos.y = pl_w->pos.y + 480.0f ;
			pl_char_move( ew ) ;
			hit_work_set( ew ) ;
			return ;
		}
		ew->routine_0 ++ ;
	}
	ew->disp_flag = 0 ;
}

/*==============================================================*/
static void shl35_03_piece_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_w = ew->master_w ;
	if( pl_w->routine_0 != 1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl35_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	if( ew->type == 3 ) pl_se_req( ew, 58 ) ;
}

/*--------------------------------------------------------------*/
static void shl35_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl35_03_copy( PLAYER_WORK *ew )
{
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
	ew->size_top = ew->size_bottom = 32 ;
	ew->size_front = ew->size_back = 36 ;
	ew->prio = 11 ;
}

/*==============================================================*/
static void shl35_03_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl35_03.c" End of File	*/
