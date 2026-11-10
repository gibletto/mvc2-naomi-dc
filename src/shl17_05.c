/* Cable's effect 05 (character 17's shot 05): the shot of Time Flip (hyper), which flies until it
   catches the opponent, then holds them while its hits play out. */
#include "player.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the shot's own work, once it has caught */
typedef struct SHL17_05_LOCAL_W {
	Float	ofs_x;		/* 000: the opponent's place behind the shot */
	PLAYER_WORK	*em_w;	/* 004: the opponent caught */
} SHL17_05_LOCAL_W;
#define SHL17_05_LW( w )	( ( SHL17_05_LOCAL_W * )&( w )->obj_flag )

static void shl17_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl17_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl17_05_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1705 ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->pl_type = pl_w->pl_type ;
		PL17_LW( pl_w )->timeflip_shot_flag[0] = 1 ;
		return ew ;
	}
}

static void shl17_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl17_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl17_05_main( PLAYER_WORK *ew )
{
	static void ( *shl17_05_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl17_05_init, shl17_05_move, shl17_05_end, shl17_05_free,
	} ;

	shl17_05_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl17_05_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->alpha = 0.7f ;
	ew->pos = pl_w->pos ;
	ew->pos.x += pl_w->disp_dir ? -60.0f : 60.0f ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl17_at_timeflip ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 18 ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = -4.0f ;
	ew->spd.x = -DOT_X( 8.0f ) ;
	if( ew->disp_dir ) ew->spd.x = -ew->spd.x ;
	pl_w->team_ng_timer = 2 ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 3 ;
	ew->timer0 = 24 ;
}

/*==============================================================*/
/*			Flying: it ends when Cable is hit or it leaves		*/
/*			the screen, and a hit on a player catches them		*/
/*==============================================================*/
static void shl17_05_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_w->routine_1 == PL_DAMAGE ){
		pls17_04_set2( ew, pl_w, 3 ) ;
		ew->routine_0 = 2 ;
		return ;
	}
	if( pl_w->team_no ){
		ew->routine_0 = 2 ;
		pls17_04_set2( ew, pl_w, 3 ) ;
		return ;
	}
	if( ew->hit_flag ){
		if( ew->em_w->work_kind ){
			ew->atck_no = pl17_at_timeflip ;
			HIT_RESET( ew ) ;
		} else {
			PL17_LW( pl_w )->timeflip_fly_flag[0] = 0 ;
			if( !( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ) && pl_sp_catch_ck( ew ) ){
				ew->routine_1 ++ ;
				ew->spd.x = 0.0f ;
				ew->spd.y = 0.0f ;
				ew->acc.x = 0.0f ;
				ew->acc.y = 0.0f ;
				PL17_LW( pl_w )->timeflip_catch_flag[0] = 1 ;
				pl_char_set( ew, Char_Effect, 12 ) ;
				em_w = ew->em_w ;
				em_w->pl_stance = STANCE_STAND ;
				em_w->pos.y = pl_w->ground_y ;
				SHL17_05_LW( ew )->ofs_x = DOT_X( 64.0f ) ;
				if( ew->disp_dir ){
					Float *p = &SHL17_05_LW( ew )->ofs_x ;
					*p = -*p ;
				}
				em_w->pos.x = ew->pos.x - SHL17_05_LW( ew )->ofs_x ;
				SHL17_05_LW( ew )->em_w = ew->em_w ;
				if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ) SHL17_05_LW( ew )->ofs_x = 0.0f ;
				ew->dir = 0 ;
				return ;
			}
			pls17_04_set2( ew, pl_w, 3 ) ;
			ew->routine_0 = 2 ;
			return ;
		}
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ){
		PL17_LW( pl_w )->timeflip_fly_flag[0] = 0 ;
		pls17_04_set2( ew, pl_w, 3 ) ;
		ew->routine_0 = 2 ;
		return ;
	}
	if( -- ew->timer0 < 0 ){
		PL17_LW( pl_w )->timeflip_fly_flag[0] = 0 ;
		ew->routine_1 = 2 ;
		ew->acc.x = DOT_X( 0.375f ) ;
		if( ew->disp_dir ) ew->acc.x = -ew->acc.x ;
		pl_char_set( ew, 2, 2 ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Holding the opponent while the animation's hits		*/
/*			play out											*/
/*==============================================================*/
static void shl17_05_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w ;

	if( pl_w->routine_1 == PL_DAMAGE ){
		pls17_04_set2( ew, pl_w, 3 ) ;
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		pls17_04_set2( ew, pl_w, 2 ) ;
		ew->routine_0 = 2 ;
		return ;
	}
	em_w = SHL17_05_LW( ew )->em_w ;
	if( ew->dir && ew->hit_flag ){
		if( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ){
			ew->hit_stop = 8 ;
			em_w->hit_stop = 8 ;
		}
		ew->dir = 0 ;
	}
	if( ew->cg_flag1 ){
		ew->atck_no = ew->cg_flag1 ;
		HIT_RESET( ew ) ;
		ew->cg_flag1 = 0 ;
		ew->dir = 1 ;
	}
	if( ew->cg_type == 2 ){
		ew->cg_type = 0 ;
		shl17_06_set( ew, 4 ) ;
		pls17_06_set( ew, 7 ) ;
		pl_voice_req( ew, 36 ) ;
	}
	if( ew->cg_type == 15 ){
		ew->cg_type = 0 ;
		shl17_06_set( ew, 5 ) ;
		pls17_06_set( ew, 7 ) ;
		pl_voice_req( ew, 36 ) ;
	}
	if( ew->cg_type == 3 ){
		ew->cg_type = 0 ;
		pls17_06_set( ew, 6 ) ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Missed: it slows to a stop and plays out its		*/
/*			animation											*/
/*==============================================================*/
static void shl17_05_miss( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		pls17_04_set2( ew, pl_w, 3 ) ;
		ew->routine_0 = 2 ;
		PL17_LW( pl_w )->timeflip_fly_flag[0] = 0 ;
		PL17_LW( pl_w )->timeflip_shot_flag[0] = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl17_05_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl17_05_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl17_05_fly, shl17_05_catch, shl17_05_miss,
	} ;

	pl_w->team_ng_timer = 2 ;
	PL17_LW( pl_w )->timeflip_wait_timer[0] = 3 ;
	shl17_05_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl17_05_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	PL17_LW( pl_w )->timeflip_shot_flag[0] = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_05_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_w->team_ng_timer = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_05.c" End of File	*/
