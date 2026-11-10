/* Tron Bonne's effect 02 (character 10's shot 02): a rock that spins as it flies and breaks into
   six pieces when it hits or comes down to her ground. */
#include "player.h"
#include "pl_char.h"

/* the rock's speed and start from Tron, its priority, animation and attack */
typedef struct SHL10_02_DATA {
	Sint32	spd_x, spd_y;	/* 16.16 dots */
	Sint16	ofs_x, ofs_y;
	Sint8	prio;
	Sint8	unk_0d;
	Uint8	char_no;
	Uint8	atck_no;
} SHL10_02_DATA;

static SHL10_02_DATA shl10_02_tbl[] = {
	{ -0xa0000, -0x32000, -96, 96, 11, 0, 32, 48 },
} ;

static void shl10_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl10_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl10_02_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void shl10_02_init( PLAYER_WORK *ew );
static void shl10_02_move( PLAYER_WORK *ew );
static void shl10_02_end( PLAYER_WORK *ew );
static void shl10_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl10_02_main( PLAYER_WORK *ew )
{
	static void ( *shl10_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl10_02_init, shl10_02_move, shl10_02_end, shl10_02_free,
	} ;

	shl10_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl10_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL10_02_DATA *dt ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1002 ;
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
	ew->size_top = 32 ;
	ew->size_bottom = 32 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	dt = &shl10_02_tbl[ew->type] ;
	ew->prio = dt->prio ;
	ew->pos = pl_w->pos ;
	if( !pl_w->disp_dir ){
		ew->pos.x += 1.6666666f * dt->ofs_x ;
		ew->spd.x = 1.6666666f * dt->spd_x / 65536.0f ;
	} else {
		ew->pos.x -= 1.6666666f * dt->ofs_x ;
		ew->spd.x = -( 1.6666666f * dt->spd_x / 65536.0f ) ;
	}
	ew->pos.y += 2.142857f * dt->ofs_y ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 2.142857f * dt->spd_y / 65536.0f ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = dt->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dt->char_no ) ;
	shl10_02_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_02_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 i ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->rot[2] += -0x800 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->hit_flag || ew->hit_box_no || ew->pos.y <= pl_w->ground_y + DOT_Y( 16.0f ) ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		for( i = 0 ; i < 6 ; i ++ ) pls10_07_set( pl_w, ew, i ) ;
		pl_voice_req( pl_w, 39 ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl10_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl10_02.c" End of File	*/
