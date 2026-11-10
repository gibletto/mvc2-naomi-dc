/* Cable's effect 01 (character 17's shot 01): the grenade of Electrap, which bounces to a stop and
   bursts in a shock that can hold the opponent, with its sparks and its flash. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the grenade's own work, in obj_mtx */
typedef struct SHL17_01_LOCAL_W {
	Sint32	catch_flag;	/* 000: set while the shock holds the opponent */
	Float	catch_y;	/* 004: the height he is held at */
	Sint32	end_flag;	/* 008: set when he got away */
} SHL17_01_LOCAL_W;
#define SHL17_01_LW( w )	( ( SHL17_01_LOCAL_W * )&( w )->obj_mtx )

/* its throw, x and y, by kind and strength */
static const struct { Float x, y ; } shl17_01_spd[] = {
	{ 12.0f, 13.0f }, { 8.0f, 17.0f }, { 8.0f, 5.0f }, { 15.0f, 5.0f },
} ;
static const Float shl17_01_ofs[] = { 90.0f, 255.0f, 52.0f, 306.0f } ;	/* its start from Cable, by kind */
static const Sint16 shl17_01_time[] = { 30, 20 } ;	/* the frames it waits, by kind / 2 */

static void shl17_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl17_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_END ) ) ){
		ew->work_func = shl17_01_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x1701 ;
		ew->disp_dir = pl_w->disp_dir ;
	}
	return ew ;
}

static void shl17_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_shock( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_fade( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );

/*==============================================================*/
static void shl17_01_main( PLAYER_WORK *ew )
{
	static void ( *shl17_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw ) = {
		shl17_01_init, shl17_01_move, shl17_01_burst, shl17_01_shock, shl17_01_fade,
		shl17_01_end, shl17_01_free,
	} ;

	shl17_01_jmp[ew->routine_0]( ew, ew->master_w, SHL17_01_LW( ew ) ) ;
}

/*==============================================================*/
static void shl17_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	Float ofs ;

	ew->routine_0 = 1 ;
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
	ew->prio = 7 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl17_at_electrap ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	ew->pos = pl_w->pos ;
	ofs = shl17_01_ofs[ew->type] ;
	if( !ew->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->pos.y += shl17_01_ofs[ew->type + 1] ;
	ofs = shl17_01_spd[ew->type + pl_w->atck_level].x ;
	if( !ew->disp_dir ) ofs = -ofs ;
	ew->spd.x = ofs ;
	ew->spd.y = shl17_01_spd[ew->type + pl_w->atck_level].y ;
	ew->acc.x = 0.0f ;
	ew->acc.y = -1.0f ;
	ew->timer0 = shl17_01_time[ew->type >> 1] ;
	pl_char_set( ew, Char_Effect, 13 ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			Whether the grenade is past the stage's end it		*/
/*			is flying to										*/
/*==============================================================*/
static int shl17_01_out_ck( PLAYER_WORK *ew )
{
	if( !ew->disp_dir ){
		if( scr_work.stage_left + -DOT_X( 32.0f ) > ew->pos.x ) return 1 ;
	} else {
		if( ew->pos.x > scr_work.stage_right + DOT_X( 32.0f ) ) return 1 ;
	}
	return 0 ;
}

static void shl17_01_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );

/*==============================================================*/
/*			It flies, turning, and bounces three times			*/
/*			before it lies still								*/
/*==============================================================*/
static void shl17_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	if( ew->hit_box_no ){
		ew->routine_0 = 5 ;
		ew->disp_flag = 0 ;
		return ;
	} else if( ew->hit_flag ){
		ew->routine_0 = 2 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		pl_char_set( ew, Char_Effect, 14 ) ;
		EF_WK( ew )->work[3] = 15 ;
		if( pl_sp_catch_ck( ew ) ){
			lw->catch_flag = 1 ;
			shl17_01_catch( ew, pl_w, lw ) ;
		}
		ew->rot[2] = 0 ;
		pl_voice_req( ew, 34 ) ;
		return ;
	} else if( !PL17_LW( pl_w )->sw_hold[0] && !PL17_LW( pl_w )->sw_off_flag[0] ) goto burst ;
	if( -- ew->timer0 < 0 ){
		ew->timer0 = 0 ;
		if( !PL17_LW( pl_w )->sw_hold[0] || ++ EF_WK( ew )->work[1] > 60 ) goto burst ;
	}
	if( shl17_01_out_ck( ew ) ){
		ew->routine_0 = 5 ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->routine_2 ){
		pl_char_move( ew ) ;
		ew->timer1 += ew->disp_dir ? 10 : -10 ;
		if( ew->timer1 >= 360 ) ew->timer1 = 0 ;
		else if( ew->timer1 <= 0 ) ew->timer1 = 360 ;
		ew->rot[2] = ( Sint32 )( ew->timer1 * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	}
	if( ew->routine_1 <= 3 && ew->pos.y <= pl_w->ground_y ){
		ew->pos.y = 0.0f ;
		ew->routine_1 ++ ;
		ew->spd.y *= -0.2f ;
		if( ew->routine_1 > 3 ){
			ew->routine_1 = 4 ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
			ew->routine_2 = 1 ;
		}
	}
	hit_work_set( ew ) ;
	return ;
burst:
	ew->routine_0 = 2 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_voice_req( ew, 34 ) ;
	pl_char_set( ew, Char_Effect, 14 ) ;
	ew->rot[2] = 0 ;
	EF_WK( ew )->work[3] = 15 ;
}

static int shl17_01_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw );
static void shl17_01_flash_set( PLAYER_WORK *hw );

/*==============================================================*/
/*			The burst grows; the shock that follows hits		*/
/*			once unless it already holds him					*/
/*==============================================================*/
static void shl17_01_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	ew->scale.x = 0.85f ;
	if( ew->pl_type != ew->master_w->pl_type ){
		shl17_01_free( ew, pl_w, lw ) ;
		return ;
	}
	if( lw->catch_flag && !shl17_01_hold( ew, pl_w, lw ) ){
		ew->routine_0 = 4 ;
		ew->timer1 = 20 ;
		ew->alpha = 1.0f ;
		ew->disp_flag = 1 ;
		return ;
	}
	if( pl_char_move( ew ) < 0 ){
		if( !lw->catch_flag ){
			if( ew->routine_1 ) ew->pos.y = 102.0f ;
			ew->timer1 = 48 ;
			ew->atck_no = pl17_at_electrap_burst ;
			HIT_RESET( ew ) ;
		}
		ew->routine_0 = 3 ;
		pl_char_set( ew, Char_Effect, 15 ) ;
		hit_work_set( ew ) ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->disp_flag = 0 ;
		shl17_01_flash_set( ew ) ;
	}
}

static void shl17_01_spark_set( PLAYER_WORK *hw, Uint8 no );

/*==============================================================*/
/*			The shock: it holds the opponent or hits for 48		*/
/*			frames												*/
/*==============================================================*/
static void shl17_01_shock( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	if( ew->pl_type != ew->master_w->pl_type ){
		shl17_01_free( ew, pl_w, lw ) ;
		return ;
	}
	if( lw->catch_flag ){
		if( ew->catch_pair.w->catch_pair.w != ew ){
			ew->routine_0 = 4 ;
			lw->end_flag = 1 ;
			return ;
		}
		pl_char_move( ew ) ;
		if( !shl17_01_hold( ew, pl_w, lw ) ){
			ew->routine_0 = 4 ;
			ew->timer1 = 20 ;
			ew->alpha = 1.0f ;
			ew->disp_flag = 1 ;
		}
		return ;
	}
	if( ew->timer1 ){
		pl_char_move( ew ) ;
		ew->timer1 -- ;
		shl17_01_spark_set( ew, lw->catch_flag ) ;
		if( ew->hit_flag ) ew->timer1 = 0 ;
		hit_work_set( ew ) ;
		return ;
	}
	ew->routine_0 = 4 ;
	ew->timer1 = 20 ;
	ew->alpha = 1.0f ;
	ew->disp_flag = 1 ;
}

/*==============================================================*/
/*			It flickers, turns and fades out					*/
/*==============================================================*/
static void shl17_01_fade( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	if( ew->pl_type != ew->master_w->pl_type ){
		shl17_01_free( ew, pl_w, lw ) ;
		return ;
	}
	if( lw->end_flag ){
		ew->routine_0 = 5 ;
		return ;
	}
	EF_WK( ew )->work[3] %= 2 ;
	ew->scale.x = EF_WK( ew )->work[3] ? 0.9f : 1.0f ;
	ew->scale.y = EF_WK( ew )->work[3] ? 0.9f : 1.0f ;
	EF_WK( ew )->work[3] ++ ;
	ew->rot[2] += 0x1f4a ;
	if( ew->timer1 ){
		pl_char_move( ew ) ;
		ew->timer1 -- ;
		ew->alpha -= 0.05f ;
	} else {
		ew->routine_0 = 5 ;
		shl17_01_end( ew, pl_w, lw ) ;
	}
}

/*==============================================================*/
/*			The shock takes hold of the opponent it hit			*/
/*==============================================================*/
static void shl17_01_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	PLAYER_WORK *em = ew->em_w ;

	pl_w->catch_no = em->catch_no = 0xc1 ;
	pl_w->catch_grip_old = -1 ;
	pl_ef_catch_set( ew, em ) ;
	em->catch_type = 6 ;
	ew->timer0 = 60 ;
	ew->pos = pl_w->catch_w->pos ;
	ew->pos.y += 102.0f ;
	LO_BYTE( ew->cg_rival ) = 42 ;
	ew->catch_grip_old = -1 ;
	catch_pos_set( ew, pl_w->catch_w ) ;
	lw->catch_y = pl_w->catch_w->pos.y ;
	em->dm_elem = 3 ;
	em->unk_228 = 3 ;
	dm00_04_set( em ) ;
}

/*==============================================================*/
/*			Holds the opponent in the shock; 0 once he is		*/
/*			let go												*/
/*==============================================================*/
static int shl17_01_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->catch_flag ){
		pl_w->catch_flag = 1 ;
		em = pl_w->catch_w ;
		if( em->vital_new && ( !em->hit_box_no || em->routine_1 != PL_DAMAGE ) ){
			catch_pos_set( ew, em ) ;
			em->pos.y = lw->catch_y ;
			shl17_01_spark_set( ew, lw->catch_flag ) ;
			if( -- ew->timer0 < 0 ){
				em->catch_type = 7 ;
				lw->catch_flag = 0 ;
				pl_w->catch_flag = 0 ;
			}
		} else {
			lw->catch_flag = 0 ;
			pl_w->catch_flag = 0 ;
		}
	}
	return lw->catch_flag ;
}

/*--------------------------------------------------------------*/
static void shl17_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL17_01_LOCAL_W *lw )
{
	if( pl_w ) PL17_LW( pl_w )->grenade_flag[0] = 0 ;
	work_free( ew ) ;
}

static void shl17_01_spark_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			A spark of the shock								*/
/*==============================================================*/
static void shl17_01_spark_set( PLAYER_WORK *hw, Uint8 no )
{
	PLAYER_WORK *ew ;

	for(;;){
		if( !( ew = work_get( 0, W_PLS, LIST_TOP ) ) ) return ;
		ew->work_func = shl17_01_spark_main ;
		ew->master_w = hw ;
		ew->type = no ;
		ew->ef_no = 0x1701 ;
		ew->sub_no = 0 ;
		ew->timer0 = 0 ;
		break ;
	}
}

static void shl17_01_spark_init( PLAYER_WORK *ew, PLAYER_WORK *hw );
static void shl17_01_spark_move( PLAYER_WORK *ew, PLAYER_WORK *hw );

/*==============================================================*/
static void shl17_01_spark_main( PLAYER_WORK *ew )
{
	static void ( *shl17_01_spark_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *hw ) = {
		shl17_01_spark_init, shl17_01_spark_move,
	} ;

	shl17_01_spark_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl17_01_spark_init( PLAYER_WORK *ew, PLAYER_WORK *hw )
{
	ew->disp_flag = 0 ;
	if( ew->timer0 ){
		ew->timer0 -- ;
		return ;
	}
	CHAR_PART_COPY( ew, hw ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = hw->pl_no ;
	ew->pl_type = hw->pl_type ;
	ew->scale.x = hw->scale.x ;
	ew->scale.y = hw->scale.y ;
	ew->atck_level = hw->atck_level ;
	ew->work_no = hw->work_no ;
	ew->col_cnt = hw->col_cnt ;
	ew->scale = hw->scale ;
	ew->prio = hw->prio ;
	ew->prio = 0 ;
	ew->routine_0 ++ ;
	ew->timer0 = 8 ;
	ew->pos = hw->pos ;
	EF_WK( ew )->work[1] = nlRand() ;
	EF_WK( ew )->work[1] %= 360 ;
	ew->rot[2] = ( Sint32 )( EF_WK( ew )->work[1] * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	if( EF_WK( ew )->work[1] % 2 ) ef_char_set( ew, 27, 14 ) ;
	else ef_char_set( ew, 27, 15 ) ;
}

/*--------------------------------------------------------------*/
static void shl17_01_spark_move( PLAYER_WORK *ew, PLAYER_WORK *hw )
{
	ew->disp_flag = 1 ;
	if( ew->timer0 ){
		ew->alpha -= 0.125f ;
		ef_char_move( ew ) ;
		ew->timer0 -- ;
		ew->scale.x += -0.1125f ;
		ew->scale.y += 0.3125f ;
	} else {
		ew->disp_flag = 0 ;
		shl17_01_free( ew, 0, 0 ) ;
	}
}

static void shl17_01_flash_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The flash of the burst								*/
/*==============================================================*/
static void shl17_01_flash_set( PLAYER_WORK *hw )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_PLS, LIST_TOP ) ) ){
		ew->work_func = shl17_01_flash_main ;
		ew->master_w = hw ;
		ew->ef_no = 0x1701 ;
	}
}

static void shl17_01_flash_init( PLAYER_WORK *ew, PLAYER_WORK *hw );
static void shl17_01_flash_move( PLAYER_WORK *ew, PLAYER_WORK *hw );

/*==============================================================*/
static void shl17_01_flash_main( PLAYER_WORK *ew )
{
	static void ( *shl17_01_flash_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *hw ) = {
		shl17_01_flash_init, shl17_01_flash_move,
	} ;

	shl17_01_flash_jmp[ew->routine_0]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl17_01_flash_init( PLAYER_WORK *ew, PLAYER_WORK *hw )
{
	CHAR_PART_COPY( ew, hw ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = hw->pl_no ;
	ew->pl_type = hw->pl_type ;
	ew->scale.x = hw->scale.x ;
	ew->scale.y = hw->scale.y ;
	ew->atck_level = hw->atck_level ;
	ew->work_no = hw->work_no ;
	ew->col_cnt = hw->col_cnt ;
	ew->scale = hw->scale ;
	ew->prio = hw->prio ;
	ew->disp_flag = 1 ;
	ew->prio = 7 ;
	ew->routine_0 ++ ;
	ew->pos = hw->pos ;
	ef_char_set( ew, 27, 16 ) ;
}

/*==============================================================*/
/*			It flickers and turns with the grenade until the	*/
/*			grenade fades										*/
/*==============================================================*/
static void shl17_01_flash_move( PLAYER_WORK *ew, PLAYER_WORK *hw )
{
	if( hw->pl_type != hw->master_w->pl_type || hw->routine_0 == 4 ){
		shl17_01_free( ew, 0, 0 ) ;
		return ;
	}
	EF_WK( ew )->work[3] %= 2 ;
	ew->scale.x = EF_WK( ew )->work[3] ? 0.9f : 1.0f ;
	ew->scale.y = EF_WK( ew )->work[3] ? 0.9f : 1.0f ;
	EF_WK( ew )->work[3] ++ ;
	ew->rot[2] += 0x1f4a ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_01.c" End of File	*/
