/* Marrow's effect 03 (character 16's shot 03): the picture that follows her hand through her
   Stinger Bones and shakes the screen when it hits. It goes when her animation changes. */
#include "player.h"
#include "pl_char.h"
#include "pl16_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL16_03_LOCAL_W {
	Sint16	char_no;		/* 00: Marrow's animation when it began */
} SHL16_03_LOCAL_W;
#define SHL16_03_LW( w )	( ( SHL16_03_LOCAL_W * )&( w )->obj_flag )

static SHL16_03_LOCAL_W *shl16_03_lw ;

static void shl16_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl16_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl16_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1603 ;
	}
	return ew ;
}

static void shl16_03_init( PLAYER_WORK *ew );
static void shl16_03_move( PLAYER_WORK *ew );
static void shl16_03_end( PLAYER_WORK *ew );
static void shl16_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl16_03_main( PLAYER_WORK *ew )
{
	static void ( *shl16_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl16_03_init, shl16_03_move, shl16_03_end, shl16_03_free,
	} ;

	shl16_03_lw = SHL16_03_LW( ew ) ;
	shl16_03_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl16_03_init( PLAYER_WORK *ew )
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
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = 0 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 4.0f ) ;
	else ew->pos.x += DOT_X( 4.0f ) ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y -= DOT_Y( 6.0f ) ;
	ew->scale.x = 1.25f ;
	ew->scale.y = 1.1f ;
	shl16_03_lw->char_no = ew->master_w->char_no.w ;
	ew->timer0 = 0 ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

/*--------------------------------------------------------------*/
static void shl16_03_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( shl16_03_lw->char_no != ( Sint16 )ew->master_w->char_no.w ){
		shl16_03_end( ew ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	if( ew->disp_dir ) ew->pos.x += DOT_X( 4.0f ) ;
	else ew->pos.x += DOT_X( 4.0f ) ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.y -= DOT_Y( 6.0f ) ;
	if( pl_w->hit_stop && !( ew->cg_prm & 0x80 ) ) return ;
	if( ew->hit_flag && !ew->timer0 ){
		ew->timer0 = 1 ;
		scr_work.quake_level = 2 ;
		scr_work.quake_flag = 1 ;
	}
	if( pl_char_move( ew ) < 0 ){
		shl16_03_end( ew ) ;
		return ;
	}
	if( ew->cg_type ){
		ew->atck_no = ew->cg_type + 66 ;
		HIT_RESET( ew ) ;
		ew->cg_type = 0 ;
		ew->timer0 = 0 ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl16_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	shl16_03_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl16_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl16_03.c" End of File	*/
