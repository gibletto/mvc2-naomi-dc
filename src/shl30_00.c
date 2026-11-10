/* Omega Red's effect 00 (character 30's shot 00): the two flashes beside him in his Omega
   Destroyer hyper combo. They keep his animation's frame, stand where a table puts them for his hold position
   and strike with it; they end when his animation changes. */
#include "player.h"
#include "pl30_def.h"

/* the offsets from his position, by his frame's flag: x and y for each of the two */
static Sint16 shl30_00_ofs[][2][2] = {
	{ { 0, 0 }, { 0, 0 } },
	{ { -31, 40 }, { 46, 44 } },
	{ { -14, 54 }, { 25, 54 } },
	{ { -13, 53 }, { 24, 53 } },
	{ { -60, 85 }, { 75, 85 } },
	{ { -60, 83 }, { 75, 83 } },
	{ { -19, 115 }, { 34, 115 } },
	{ { -7, 115 }, { 22, 115 } },
	{ { -3, 108 }, { 20, 107 } },
	{ { 0, 104 }, { 15, 102 } },
	{ { 1, 102 }, { 14, 100 } },
} ;

static void shl30_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl30_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int type ;

	if( work_rest_cnt < 2 ) return 0 ;
	for( type = 0 ; type < 2 ; type ++ ){
		ew = work_get( 0, W_SHL, LIST_TOP ) ;
		ew->work_func = shl30_00_main ;
		ew->master_w = pl_w ;
		ew->timer0 = pl_w->char_no.w ;
		ew->type = type ;
		ew->ef_no = 0x3000 ;
	}
	return ew ;
}

static void shl30_00_init( PLAYER_WORK *ew );
static void shl30_00_move( PLAYER_WORK *ew );
static void shl30_00_end( PLAYER_WORK *ew );
static void shl30_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl30_00_main( PLAYER_WORK *ew )
{
	static void ( *shl30_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl30_00_init, shl30_00_move, shl30_00_end, shl30_00_free,
	} ;

	shl30_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl30_00_init( PLAYER_WORK *ew )
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
	ew->atck_flag = HIT_ON | HIT_OBJ | HIT_OWNER ;
	ew->dm_flag = 0 ;
	ew->dir = 0xff ;
	ew->disp_dir ^= ew->type ;
	ew->sub_prio = -1 ;
	shl30_00_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl30_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 frame ;
	Float ofs ;

	ew->disp_flag = 0 ;
	if( ew->timer0 != (Sint16)pl_w->char_no.w ){
		ew->routine_0 = 2 ;
		return ;
	}
	frame = HI_BYTE( pl_w->cg_rival ) ;
	if( (Sint8)frame < 0 ) return ;
	if( ew->dir != frame ){
		ew->dir = frame ;
		pl_char_set2( ew, Char_Effect, 0, ew->dir ) ;
		ew->atck_no = ew->cg_flag1 ;
		ew->hit_flag = ew->atck_attr = 0 ;
		ew->unk_1c4 = 0 ;
		vs_sys_w->atck_cnt[ew->pl_no] ++ ;
	}
	ew->pos = pl_w->pos ;
	ofs = DOT_X( shl30_00_ofs[pl_w->cg_flag1][ew->type][0] ) ;
	if( pl_w->disp_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->pos.y += shl30_00_ofs[pl_w->cg_flag1][ew->type][1] * DOT_Y( 1.0f ) ;
	ew->disp_flag = 1 ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl30_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl30_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl30_00.c" End of File	*/
