/* Jin's effect 02 (character 37's shot 02): the beam of his Blodia Punch. It runs out across the
   screen from his side, hits, shakes the screen, and then draws back. */
#include "player.h"
#include "pl_char.h"
#include "pl37_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL37_02_LOCAL_W {
	Sint16	x;		/* 000: how far across the beam is from its start */
	Sint16	y;		/* 002: and the start x, from the screen's edge */
} SHL37_02_LOCAL_W;
#define SHL37_02_LW( w )	( ( SHL37_02_LOCAL_W * )&( w )->obj_flag )

/* the beam's shake while it holds, by frames left: the x move of each frame */
static Sint8 shl37_02_shake_tbl[] = {
	-4, 0, 4, 0, -4, 0, 4, 0, -4, 0, 4, 0, -4, 0, 4, 0,
	-8, 0, 8, 0, -8, 0, 8, 0, -8, 0, 8, 0, -8, 0, 8, 0,
	-16, 0, 16, 0, -16, 0, 16, 0, -16, 0, 16, 0, -16, 0, 16, 0,
} ;

static void shl37_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl37_02_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl37_02_main ;
		ew->ef_no = 0x3702 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl37_02_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_02_main( PLAYER_WORK *ew )
{
	static void ( *shl37_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_02_type_0,
	} ;

	shl37_02_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl37_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl37_02_type_0( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_02_type_0_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_02_init, shl37_02_move, shl37_02_end, shl37_02_free,
	} ;

	shl37_02_type_0_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_02_LOCAL_W *lw = SHL37_02_LW( ew ) ;

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
	ew->disp_flag = 1 ;
	ew->sub_prio = 2 ;
	lw->x = 0 ;
	ew->timer0 = 0 ;
	ew->atck_no = pl37_at_blodia_beam ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.z = pl_w->pos.z ;
	pl_w->spd.x = 0.0f ;
	pl_w->spd.y = 0.0f ;
	pl_w->acc.x = 0.0f ;
	pl_w->acc.y = 0.0f ;
	ew->spd.x = 20.0f ;
	pl_char_set( ew, 22, 10 ) ;
	shl37_02_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			The screen's edge, on the side it faces				*/
/*==============================================================*/
static void shl37_02_edge_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_02_LOCAL_W *lw = SHL37_02_LW( ew ) ;

	if( !ew->disp_dir ) lw->y = scr_work.scr_right ;
	else lw->y = scr_work.scr_left ;
	if( lw->x > 600.0f ) lw->x = 600 ;
}

/*==============================================================*/
static void shl37_02_x_add( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_02_LOCAL_W *lw = SHL37_02_LW( ew ) ;

	lw->x += ew->spd.x ;
}

/*==============================================================*/
static void shl37_02_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_02_LOCAL_W *lw = SHL37_02_LW( ew ) ;
	Sint16 x = lw->x ;

	if( !ew->disp_dir ) x = -x ;
	ew->pos.x = x + lw->y ;
}

static void shl37_02_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_02_shake( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl37_02_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_02_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_02_run, shl37_02_shake,
	} ;

	if( pl_w->routine_1 || pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->prio = 12 ;
	shl37_02_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It runs out to the screen's far end; hit stop		*/
/*			holds it, and a hit lets it go on					*/
/*==============================================================*/
static void shl37_02_run( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_02_LOCAL_W *lw = SHL37_02_LW( ew ) ;

	shl37_02_edge_set( ew, pl_w ) ;
	if( ew->hit_stop ) ew->hit_stop -- ;
	if( ew->timer0 ) ew->timer0 -- ;
	if( !ew->hit_stop ){
		shl37_02_x_add( ew, pl_w ) ;
		shl37_02_pos_set( ew, pl_w ) ;
		if( !ew->timer0 ){
			hit_work_set( ew ) ;
			if( ew->hit_flag ){
				ew->timer0 = 8 ;
				ew->atck_no = pl37_at_blodia_beam ;
				HIT_RESET( ew ) ;
			}
		}
	}
	if( lw->x >= 600.0f ){
		lw->x = 600 ;
		shl37_02_pos_set( ew, pl_w ) ;
		if( pl_w->team_type == 4 || pl_w->team_type == 5 ){
			ew->atck_no = pl37_at_blodia_beam_team ;
			HIT_RESET( ew ) ;
		} else {
			ew->atck_no = pl37_at_blodia_beam_end ;
			HIT_RESET( ew ) ;
		}
		scr_excep_set( pl_w, 13, 0 ) ;
		scr_work.quake_level = 3 ;
		scr_work.quake_flag = 1 ;
		ew->timer0 = 48 ;
		ew->timer1 = 24 ;
		ew->routine_1 ++ ;
		pl_voice_req( ew, 30 ) ;
	}
}

/*==============================================================*/
/*			It shakes in place									*/
/*==============================================================*/
static void shl37_02_shake( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl37_02_edge_set( ew, pl_w ) ;
	shl37_02_pos_set( ew, pl_w ) ;
	if( ew->timer1 ){
		ew->timer1 -- ;
		hit_work_set( ew ) ;
	}
	{
		Sint8 d = shl37_02_shake_tbl[ew->timer1] ;

		if( ew->disp_dir ) d = -d ;
		ew->pos.x += d ;
	}
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
	}
}

static void shl37_02_end_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_02_end_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl37_02_end_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It draws back: first it turns, then runs back to	*/
/*			Jin's side											*/
/*==============================================================*/
static void shl37_02_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl37_02_end_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl37_02_end_init, shl37_02_end_wait, shl37_02_end_back,
	} ;

	ew->prio = 12 ;
	shl37_02_end_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl37_02_end_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_2 ++ ;
	ew->spd.x = -DOT_X( 16.0f ) ;
	ew->timer0 = 8 ;
}

/*--------------------------------------------------------------*/
static void shl37_02_end_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL37_LOCAL_W *pl_lw = PL37_LW( pl_w ) ;

	shl37_02_edge_set( ew, pl_w ) ;
	shl37_02_pos_set( ew, pl_w ) ;
	if( !ew->timer0 -- ){
		pl_lw->shot_end_flag = 1 ;
		ew->routine_2 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl37_02_end_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL37_02_LOCAL_W *lw = SHL37_02_LW( ew ) ;

	shl37_02_edge_set( ew, pl_w ) ;
	shl37_02_x_add( ew, pl_w ) ;
	shl37_02_pos_set( ew, pl_w ) ;
	if( lw->x < 0 ) ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl37_02_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl37_02.c" End of File	*/
