/* Anakaris's effect 01 (character 04's shot 01): the shots that fly down from in front of him at a
   slant: Royal Judgement's (type 0), by his attack level, and Pharaoh Magic's (types 1 to 3),
   which tell him when they strike. */
#include "player.h"
#include "pl_char.h"
#include "pl04_def.h"

static void shl04_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl04_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl04_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x401 ;
		if( !type ){
			ew->pos.x = 160.0f ;
			ew->pos.y = DOT_Y( 72.0f ) ;
		}
		else {
			ew->pos.x = 160.0f ;
			ew->pos.y = DOT_Y( 96.0f ) ;
		}
		if( !pl_w->disp_dir ) ew->pos.x = -ew->pos.x ;
		ew->pos.x += pl_w->pos.x ;
		ew->pos.y += pl_w->pos.y ;
	}
	return ew ;
}

static void shl04_01_init( PLAYER_WORK *ew );
static void shl04_01_move( PLAYER_WORK *ew );
static void shl04_01_end( PLAYER_WORK *ew );
static void shl04_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_01_main( PLAYER_WORK *ew )
{
	static void ( *shl04_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_01_init, shl04_01_move, shl04_01_end, shl04_01_free,
	} ;

	shl04_01_jmp[ew->routine_0]( ew ) ;
}

static void shl04_01_judgement_init( PLAYER_WORK *ew );
static void shl04_01_magic_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_01_init( PLAYER_WORK *ew )
{
	static void ( *shl04_01_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_01_judgement_init, shl04_01_magic_init, shl04_01_magic_init, shl04_01_magic_init,
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
	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	shl04_01_init_jmp[ew->type]( ew ) ;
	shl04_01_move( ew ) ;
}

/*==============================================================*/
static void shl04_01_judgement_init( PLAYER_WORK *ew )
{
	ew->timer0 = 96 ;
	ew->pos.x = 160.0f ;
	ew->pos.y = DOT_Y( 72.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = DOT_X( 2.5f ) ;
	ew->spd.y = -DOT_Y( 1.25f ) ;
	if( !ew->master_w->disp_dir ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x += ew->master_w->pos.x ;
	ew->pos.y += ew->master_w->pos.y ;
	ew->atck_no = ew->atck_level + 50 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, ew->atck_level + 19 ) ;
}

/*==============================================================*/
static void shl04_01_magic_init( PLAYER_WORK *ew )
{
	ew->timer0 = 96 ;
	ew->pos.x = 160.0f ;
	ew->pos.y = DOT_Y( 96.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = DOT_X( 2.5f ) ;
	ew->spd.y = -DOT_Y( 1.25f ) ;
	if( ew->type != 1 ){
		ew->spd.x = DOT_X( 5.0f ) ;
		ew->spd.y = -DOT_Y( 2.0f ) ;
	}
	if( !ew->master_w->disp_dir ){
		ew->pos.x = -ew->pos.x ;
		ew->spd.x = -ew->spd.x ;
	}
	ew->pos.x += ew->master_w->pos.x ;
	ew->pos.y += ew->master_w->pos.y ;
	ew->atck_no = ew->type + 63 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 22, ew->type + 10 ) ;
}

static void shl04_01_judgement_move( PLAYER_WORK *ew );
static void shl04_01_magic_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl04_01_move( PLAYER_WORK *ew )
{
	static void ( *shl04_01_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_01_judgement_move, shl04_01_magic_move, shl04_01_magic_move, shl04_01_magic_move,
	} ;

	shl04_01_move_jmp[ew->type]( ew ) ;
}

static void shl04_01_judgement_fly( PLAYER_WORK *ew );
static void shl04_01_wait( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_01_judgement_move( PLAYER_WORK *ew )
{
	static void ( *shl04_01_judgement_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_01_judgement_fly, shl04_01_wait,
	} ;

	shl04_01_judgement_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			It flies until its time is up or it strikes, and	*/
/*			stops a little above the ground						*/
/*==============================================================*/
static void shl04_01_judgement_fly( PLAYER_WORK *ew )
{
	Float floor_y ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	floor_y = ew->master_w->ground_y + DOT_Y( 32.0f ) ;
	if( ew->pos.y < floor_y ){
		ew->pos.y = floor_y ;
		ew->timer0 = 1 ;
	}
	if( !--ew->timer0 || ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->timer1 = 21 ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_01_wait( PLAYER_WORK *ew )
{
	if( !--ew->timer1 ) ew->routine_0 ++ ;
}

static void shl04_01_magic_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_01_magic_move( PLAYER_WORK *ew )
{
	static void ( *shl04_01_magic_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_01_magic_fly, shl04_01_wait,
	} ;

	shl04_01_magic_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			As Royal Judgement's, and a clean hit on the		*/
/*			opponent is passed to Anakaris						*/
/*==============================================================*/
static void shl04_01_magic_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w ;
	Float floor_y ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	floor_y = ew->master_w->ground_y + DOT_Y( 32.0f ) ;
	if( ew->pos.y < floor_y ){
		ew->pos.y = floor_y ;
		ew->timer0 = 1 ;
	}
	if( !--ew->timer0 || ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->timer1 = 21 ;
		if( ew->hit_flag ){
			em_w = ew->em_w ;
			if( !em_w->work_kind && !( ew->hit_flag & 0x7f ) ){
				pl_w->hit_flag = HR_HIT ;
				pl_w->ef_w = ew->em_w ;
			}
		}
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl04_01.c" End of File	*/
