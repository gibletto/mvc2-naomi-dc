/* The arms of Mega Man's and Roll's Beat Plane (character 1c's shot 08): five kinds that ride on
   the player in the hyper combo; the first and last also hit, and fire a shot 09 each time their
   animation turns over. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

/* a kind's drawing order and its animation */
typedef struct SHL1C_08_DATA {
	Sint8	order;
	Uint8	char_no;
} SHL1C_08_DATA;

static SHL1C_08_DATA shl1c_08_data[5] = {
	{ 2, 0x13 }, { -2, 0x15 }, { 1, 0x16 }, { -1, 0x17 }, { -1, 0x18 },
} ;

static void shl1c_08_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1c_08_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_08_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void shl1c_08_init( PLAYER_WORK *ew );
static void shl1c_08_move( PLAYER_WORK *ew );
static void shl1c_08_end( PLAYER_WORK *ew );
static void shl1c_08_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_08_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_08_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_08_init, shl1c_08_move, shl1c_08_end, shl1c_08_free,
	} ;

	shl1c_08_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_08_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL1C_08_DATA *dp ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c08 ;
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
	ew->timer0 = 0 ;
	dp = &shl1c_08_data[ew->type] ;
	ew->sub_prio = dp->order ;
	if( !ew->type ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl1c_at_beat ;
		HIT_RESET( ew ) ;
	} else if( ew->type == 4 ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl1c_at_beat_2 ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, 22, dp->char_no ) ;
	shl1c_08_move( ew ) ;
}

static void shl1c_08_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_type( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_08_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_08_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_08_type, shl1c_08_ck,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->prio = pl_w->prio ;
	shl1c_08_pos( ew, pl_w ) ;
	shl1c_08_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl1c_08_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_1( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_bomb( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_08_type( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_08_type_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_08_shot, shl1c_08_1, shl1c_08_1, shl1c_08_1, shl1c_08_bomb,
	} ;
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( !lw->sp_time ){
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->disp_flag = 1 ;
		shl1c_08_ck( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = 1 ;
	if( lw->turn ) ew->disp_flag = 0 ;
	shl1c_08_type_jmp[ew->type]( ew, pl_w ) ;
}

static void shl1c_08_shot_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_shot_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_08_shot( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_08_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_08_shot_wait, shl1c_08_shot_fire, shl1c_08_shot_end,
	} ;

	shl1c_08_shot_jmp[ew->routine_2]( ew, pl_w ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl1c_08_shot_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( lw->sw_shot & 1 ){
		ew->routine_2 ++ ;
		lw->arm_fire |= 1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_08_shot_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_2 ++ ;
		if( !( ++ ew->timer0 & 3 ) ){
			shl1c_09_set( ew, 0 ) ;
		} else {
			shl1c_09_set( ew, 2 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl1c_08_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		PL1C_LOCAL_W *lw ;

		ew->routine_2 = 0 ;
		lw = PL1C_LW( pl_w ) ;
		lw->arm_fire &= 0xfe ;
	}
}

/*==============================================================*/
static void shl1c_08_1( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
}

static void shl1c_08_bomb_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_bomb_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_08_bomb_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl1c_08_bomb( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_08_bomb_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_08_bomb_wait, shl1c_08_bomb_fire, shl1c_08_bomb_end,
	} ;

	shl1c_08_bomb_jmp[ew->routine_2]( ew, pl_w ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl1c_08_bomb_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = PL1C_LW( pl_w ) ;

	if( lw->sw_shot & 2 ){
		ew->routine_2 ++ ;
		lw->arm_fire |= 2 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_08_bomb_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_2 ++ ;
		if( !( ++ ew->timer0 & 3 ) ){
			shl1c_09_set( ew, 1 ) ;
		} else {
			shl1c_09_set( ew, 3 ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl1c_08_bomb_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		PL1C_LOCAL_W *lw ;

		ew->routine_2 = 0 ;
		lw = PL1C_LW( pl_w ) ;
		lw->arm_fire &= 0xfd ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_08_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->cg_type ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_08_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_08_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1c_08_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_dir = pl_w->disp_dir ;
	ew->pos = pl_w->pos ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_08.c" End of File	*/
