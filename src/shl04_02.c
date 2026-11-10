/* Anakaris's effect 02 (character 04's shot 02): set at the start of the round, it stays with him
   and shows and strikes beside his normal attacks in the air. */
#include "player.h"
#include "pl04_def.h"

static void shl04_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl04_02_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl04_02_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x402 ;
	}
	return ew ;
}

static void shl04_02_init( PLAYER_WORK *ew );
static void shl04_02_move( PLAYER_WORK *ew );
static void shl04_02_end( PLAYER_WORK *ew );
static void shl04_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_02_main( PLAYER_WORK *ew )
{
	static void ( *shl04_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_02_init, shl04_02_move, shl04_02_end, shl04_02_free,
	} ;

	shl04_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl04_02_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	ew->sub_type = 0 ;
	ew->pos.y = ew->master_w->ground_y ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_PL_ONLY | HIT_OWNER ;
	ew->dm_flag = HIT_OWNER ;
	shl04_02_move( ew ) ;
}

/*==============================================================*/
/*			While Anakaris makes a normal attack in a plain		*/
/*			jump, it takes his frame and strikes				*/
/*==============================================================*/
static void shl04_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint8 frame ;
	Uint8 no ;

	if( ew->hit_stop ) pl_w->hit_stop = ew->hit_stop -- ;
	ew->disp_flag = 0 ;
	if( pl_w->pl_stance == STANCE_AIR && !pl_w->sjump_flag && pl_w->pos.y != pl_w->ground_y
	 && ( frame = LO_BYTE( pl_w->cg_rival ) ) && HI_BYTE( pl_w->cg_rival ) != 36
	 && pl_w->routine_1 != PL_CATCH && pl_w->atck_type != 2 ){
		switch( pl_w->nml_no ){
		case 0 :
			no = 15 ;
			break ;
		case 1 :
			no = 16 ;
			break ;
		case 2 :
			no = 17 ;
			break ;
		default :
			return ;
		}
		ew->disp_flag = 1 ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->pos.x = pl_w->pos.x ;
		if( pl_w->cg_flag2 != ew->sub_type ){
			ew->sub_type = pl_w->cg_flag2 ;
			ew->atck_no = no ;
			HIT_RESET( ew ) ;
		}
		pl_char_set2( ew, 20, 6, frame ) ;
		if( pl_w->cg_flag2 ) hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl04_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl04_02.c" End of File	*/
