/* Iceman's effect 00 (character 09's shot 00): the ice on him for each of his moves, by kind,
   which hits with him and goes when his animation changes. */
#include "player.h"
#include "pl_char_old.h"
#include "pl09_def.h"

static void shl09_00_main( PLAYER_WORK *ew );

static Uint8 shl09_00_char[7] = { 0, 1, 2, 7, 6, 0, 3 } ;	/* the animation, by kind */
static Uint8 shl09_00_atck[7] = { 61, 62, 63, 64, 0, 60, 65 } ;	/* the attack, by kind */

/****************************************************************/
void *shl09_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl09_00_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x900 ;
		ew->timer0 = pl_w->char_no.w ;
	}
	return ew ;
}

static void shl09_00_init( PLAYER_WORK *ew );
static void shl09_00_move( PLAYER_WORK *ew );
static void shl09_00_end( PLAYER_WORK *ew );
static void shl09_00_free( PLAYER_WORK *ew );
static void shl09_00_ck( PLAYER_WORK *ew );

/*==============================================================*/
static void shl09_00_main( PLAYER_WORK *ew )
{
	static void ( *shl09_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_00_init, shl09_00_move, shl09_00_end, shl09_00_free,
	} ;

	shl09_00_ck( ew ) ;
	shl09_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl09_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	PL09_LOCAL_W *lw ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	switch( ew->type ){
	case 0:
		ew->prio = pl_w->prio ;
		ew->sub_prio = -1 ;
		ew->atck_flag = HIT_OBJ | HIT_OWNER ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		ew->atck_no = shl09_00_atck[ew->type] ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_w->atck_no = shl09_00_atck[ew->type] ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		break ;
	case 2:
		lw = ( PL09_LOCAL_W * )pl_w->tlocal ;
		ew->atck_flag = HIT_OBJ | HIT_OWNER ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		if( lw->arm_flag ){
			ew->atck_flag = 0 ;
			ew->dm_flag = 0 ;
		}
		ew->prio = pl_w->prio ;
		ew->sub_prio = -1 ;
		ew->atck_no = shl09_00_atck[ew->type] ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_w->atck_no = shl09_00_atck[ew->type] ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		break ;
	case 4:
		ew->prio = 7 ;
		ew->atck_flag = 0 ;
		ew->dm_flag = 0 ;
		break ;
	case 6:
		ew->prio = 11 ;
		ew->atck_flag = HIT_ON | HIT_OBJ ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		ew->atck_no = shl09_00_atck[ew->type] ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_w->atck_no = shl09_00_atck[ew->type] ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		if( ew->disp_dir ) ew->spd.x = 5.0f ;
		else ew->spd.x = -5.0f ;
		ew->spd.y = -15.0f ;
		break ;
	default:
		ew->prio = 0 ;
		ew->atck_flag = HIT_OBJ | HIT_OWNER ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		ew->atck_no = shl09_00_atck[ew->type] ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
		pl_w->atck_no = shl09_00_atck[ew->type] ;
		pl_w->hit_flag = pl_w->atck_attr = 0 ;
		pl_w->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[pl_w->pl_no] ++ ;
		break ;
	}
	pl_char_set( ew, Char_Effect, shl09_00_char[ew->type] ) ;
}

/*--------------------------------------------------------------*/
static void shl09_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->type == 6 ){
		if( !ew->routine_1 ){
			pl_char_move( ew ) ;
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			if( ew->cg_type ){
				ew->cg_type = 0 ;
				ew->routine_1 ++ ;
			}
			return ;
		}
		if( !ew->routine_2 ){
			ew->pos.x += ew->spd.x ;
			ew->spd.x += ew->acc.x ;
			ew->pos.y += ew->spd.y ;
			ew->spd.y += ew->acc.y ;
			if( ew->hit_flag || ew->hit_box_no ){
				ew->routine_2 ++ ;
				return ;
			}
			if( ew->pos.y > pl_w->ground_y ){
				hit_work_set( ew ) ;
				return ;
			}
			ew->routine_2 ++ ;
			ew->pos.y = pl_w->ground_y ;
			return ;
		}
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			shl09_00_end( ew ) ;
		}
		return ;
	}
	if( !ew->master_w->hit_stop ){
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			shl09_00_end( ew ) ;
			return ;
		}
		if( pl_w->speed_up_flag ){
			if( pl_char_move( ew ) < 0 ){
				ew->routine_0 ++ ;
				shl09_00_end( ew ) ;
				return ;
			}
		}
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	if( ew->type != 4 ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl09_00_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl09_00_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl09_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			It ends when his animation changes, except kind		*/
/*			6 once it has left him								*/
/*==============================================================*/
static void shl09_00_ck( PLAYER_WORK *ew )
{
	if( ew->type == 6 && ew->routine_1 ) return ;
	if( ew->master_w->char_no.w == ew->timer0 ) return ;
	if( ew->routine_0 < 2 ) ew->routine_0 = 2 ;
	ew->disp_flag = 0 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl09_00.c" End of File	*/
