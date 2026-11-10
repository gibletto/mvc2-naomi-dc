/* Strider's effect 01 (character 05's shot 01), put out by shl05_00's works: a flash on the work
   (type 0) and the shot it sends forward (type 1). */
#include "player.h"
#include "pl_char.h"
#include "pl05_def.h"

static Sint32 *lw;	/* the running work's, unused */
static PL05_LOCAL_W *pl_lw;	/* Strider's, unused */

static void shl05_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl05_01_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_01_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x501 ;
	}
	return ew ;
}

static void shl05_01_init( PLAYER_WORK *ew );
static void shl05_01_move( PLAYER_WORK *ew );
static void shl05_01_end( PLAYER_WORK *ew );
static void shl05_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_01_main( PLAYER_WORK *ew )
{
	static void ( *shl05_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_01_init, shl05_01_move, shl05_01_end, shl05_01_free,
	} ;

	pl_lw = ( PL05_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = &ew->obj_flag ;
	shl05_01_jmp[ew->routine_0]( ew ) ;
}

static void shl05_01_flash_init( PLAYER_WORK *ew );
static void shl05_01_shot_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_01_init( PLAYER_WORK *ew )
{
	static void ( *shl05_01_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_01_flash_init, shl05_01_shot_init,
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
	shl05_01_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl05_01_flash_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->prio = 0 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	shl05_01_set( ew, 1 ) ;
	pl_char_set( ew, Char_Effect, 7 ) ;
}

/*==============================================================*/
static void shl05_01_shot_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl05_at_ouroboros_shot ;
	HIT_RESET( ew ) ;
	ew->prio = 0 ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->size_top = 16 ;
	ew->size_bottom = 16 ;
	ew->size_front = 16 ;
	ew->size_back = 64 ;
	ew->spd.x = ew->disp_dir ? DOT_X( 6.5f ) : -DOT_X( 6.5f ) ;
	pl_char_set( ew, Char_Effect, 8 ) ;
}

static void shl05_01_flash( PLAYER_WORK *ew );
static void shl05_01_shot( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl05_01_move( PLAYER_WORK *ew )
{
	static void ( *shl05_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_01_flash, shl05_01_shot,
	} ;

	shl05_01_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl05_01_flash( PLAYER_WORK *ew )
{
	PLAYER_WORK *hw = ew->ef_w ;

	ew->prio = hw->prio ;
	ew->pos.x = hw->pos.x ;
	ew->pos.y = hw->pos.y ;
	if( pl_char_move( ew ) < 0 ) shl05_01_end( ew ) ;
}

/*==============================================================*/
static void shl05_01_shot( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->hit_flag || !scr_in_x_ck( ew ) ) ew->routine_0 ++ ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl05_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl05_01.c" End of File	*/
