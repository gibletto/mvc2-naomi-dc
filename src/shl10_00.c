/* Tron Bonne's effect 00 (character 10's shot 00): a puff of Gustaff's fire, which drifts forward,
   grows and fades out. */
#include "player.h"
#include "pl_char.h"

/* a puff's speed and start from Tron, its priority, animation and attack, by kind */
typedef struct SHL10_00_DATA {
	Sint32	spd_x, spd_y;	/* 16.16 dots */
	Sint16	ofs_x, ofs_y;
	Sint8	prio;
	Uint8	unk_0d;
	Uint8	char_no;
	Uint8	atck_no;
} SHL10_00_DATA;

static SHL10_00_DATA shl10_00_tbl[] = {
	{ -0x20000, 0, -48, 36, 12, 0, 23, 7 },
	{ -0x20000, 0, -48, 36, 11, 0, 24, 7 },
	{ -0x20000, 0, -48, 36, 12, 0, 23, 60 },
	{ -0x20000, 0, -48, 36, 11, 0, 24, 60 },
} ;

static void shl10_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl10_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl10_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

static void shl10_00_init( PLAYER_WORK *ew );
static void shl10_00_move( PLAYER_WORK *ew );
static void shl10_00_end( PLAYER_WORK *ew );
static void shl10_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl10_00_main( PLAYER_WORK *ew )
{
	static void ( *shl10_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl10_00_init, shl10_00_move, shl10_00_end, shl10_00_free,
	} ;

	shl10_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl10_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL10_00_DATA *dt ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1000 ;
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
	ew->size_top = 48 ;
	ew->size_bottom = 48 ;
	ew->size_front = 16 ;
	ew->size_back = 16 ;
	ew->timer0 = 30 ;
	dt = &shl10_00_tbl[ew->type] ;
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
	ew->spd.y = 2.142857f * dt->spd_y ;
	ew->acc.y = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT | HIT_OWNER ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = dt->atck_no ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, dt->char_no ) ;
	shl10_00_move( ew ) ;
}

static void shl10_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl10_00_move( PLAYER_WORK *ew )
{
	static void ( *shl10_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl10_00_fly, shl10_00_fly, shl10_00_fly, shl10_00_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl10_00_move_jmp[ew->type]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl10_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( !scr_in_x_ck( ew ) || !-- ew->timer0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->scale.x += 0.01f ;
	ew->scale.y += 0.01f ;
	ew->alpha = ew->timer0 * 0.033333335f ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl10_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl10_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl10_00.c" End of File	*/
