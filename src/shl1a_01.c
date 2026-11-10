/* Abyss's third form's body box (character 1a's shot 01): a work that follows him and can be
   struck while his core is open. */
#include "player.h"
#include "pl_char.h"
#include "pl1a_def.h"

static void shl1a_01_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1a_01_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1a_01_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1a01 ;
	}
	return ew ;
}

static void shl1a_01_init( PLAYER_WORK *ew );
static void shl1a_01_move( PLAYER_WORK *ew );
static void shl1a_01_end( PLAYER_WORK *ew );
static void shl1a_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1a_01_main( PLAYER_WORK *ew )
{
	static void ( *shl1a_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl1a_01_init, shl1a_01_move, shl1a_01_end, shl1a_01_free,
	} ;

	shl1a_01_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1a_01_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	ew->disp_flag = 0 ;
	ew->atck_flag = 0 ;
	ew->dm_flag = HIT_ON | HIT_OBJ | HIT_OWNER ;
	pl_char_set( ew, Char_Effect, 15 ) ;
	shl1a_01_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_01_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1A_LOCAL_W *lw ;
	Float ofs ;

	if( !pl_w->be_flag ) return ;
	ew->pos = pl_w->pos ;
	ofs = pl_w->scale.x * 40.0f ;
	if( !pl_w->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->pos.y += DOT_Y( 96.0f ) * pl_w->scale.y ;
	if( pl_w->dm_flag == 0 ) return ;
	if( ew->hit_box_no ){
		catch_hitmark( ew ) ;
		pl19_voice_req( ew, 15 ) ;
	}
	if( 0 && ( lw = PL1A_LW( pl_w ) ) ) return ;
	if( ( lw = PL1A_LW( pl_w ) )->damage_flag == 0 && PL1A_LW( pl_w )->body_atck_flag == 0 ) return ;
	if( PL1A_LW( pl_w )->body_alpha != 1.0f ) return ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl1a_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1a_01.c" End of File	*/
