/* Mega Man's and Roll's Leaf Shield (character 1c's shot 04): type 0 holds the character's
   charge and makes eight leaves that circle him (types 1 to 8), which fly off one by one. */
#include "player.h"
#include "pl_char.h"
#include "pl1c_def.h"

/* the charge's own work, from obj_flag; the arcade's code reads each word as an array element */
typedef struct SHL1C_04_LOCAL_W {
	Sint32	shot[1];	/* the shots still circling, a bit for each */
	Sint32	fire[1];	/* 1 when the character lets them go, -1 once they fly off */
	Sint32	unk_08[1];
} SHL1C_04_LOCAL_W;
#define SHL1C_04_LW( w )	( ( SHL1C_04_LOCAL_W * )&( w )->obj_flag )

static void shl1c_04_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl1c_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PL1C_LOCAL_W *lw ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_04_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
		lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
		lw->shield_st = 1 ;
		lw->shield_flag = 1 ;
		lw->shield_throw_flag = 0 ;
	}
	return ew ;
}

/*==============================================================*/
/*			A shot that circles the charge hw					*/
/*==============================================================*/
static PLAYER_WORK *shl1c_04_part_set( PLAYER_WORK *hw, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( hw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl1c_04_main ;
		ew->master_w = hw ;
		ew->type = no ;
	}
	return ew ;
}

static void shl1c_04_a_main( PLAYER_WORK *ew );
static void shl1c_04_b_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_04_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_04_a_main,
		shl1c_04_b_main, shl1c_04_b_main, shl1c_04_b_main, shl1c_04_b_main,
		shl1c_04_b_main, shl1c_04_b_main, shl1c_04_b_main, shl1c_04_b_main,
	} ;

	shl1c_04_jmp[ew->type]( ew ) ;
}

static void shl1c_04_a_init( PLAYER_WORK *ew );
static void shl1c_04_a_move( PLAYER_WORK *ew );
static void shl1c_04_a_end( PLAYER_WORK *ew );
static void shl1c_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_04_a_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_04_a_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_04_a_init, shl1c_04_a_move, shl1c_04_a_end, shl1c_04_free,
	} ;

	shl1c_04_a_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The charge, which makes the shots and keeps a		*/
/*			bit for each										*/
/*==============================================================*/
static void shl1c_04_a_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Sint16 i ;
	Uint8 n = 8 ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c04 ;
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
	ew->timer0 = 300 ;
	ew->size_top = 48 ;
	ew->size_bottom = 48 ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	SHL1C_04_LW( ew )->shot[0] = 0 ;
	SHL1C_04_LW( ew )->fire[0] = 0 ;
	SHL1C_04_LW( ew )->unk_08[0] = 0xff ;
	for( i = 0 ; i < 8 ; i ++ ){
		if( shl1c_04_part_set( ew, n ) ){
			n -- ;
			SHL1C_04_LW( ew )->shot[0] |= 1 << i ;
		} else break ;
	}
	shl1c_04_a_move( ew ) ;
}

static void shl1c_04_a_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_04_a_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_04_a_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_04_a_hold, shl1c_04_a_wait, shl1c_04_a_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL1C_LOCAL_W *lw ;

	if( pl_w->routine_0 >= 2 || ew->pl_type != pl_w->pl_type ){
		ew->routine_0 = 3 ;
		return ;
	}
	if( !SHL1C_04_LW( ew )->shot[0] ){
		ew->routine_0 = 2 ;
		lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
		lw->shield_st = 0 ;
		return ;
	}
	shl1c_04_a_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl1c_04_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_a_hold_count( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_a_hold_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The charge follows the character while he holds		*/
/*			it													*/
/*==============================================================*/
static void shl1c_04_a_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl1c_04_a_hold_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_04_a_hold_count, shl1c_04_a_hold_stop,
	} ;
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;

	if( !lw->shield_flag ){
		ew->routine_0 = 2 ;
		lw->shield_st = 0 ;
		return ;
	}
	if( lw->join_sp ){
		ew->routine_0 = 2 ;
		lw->shield_st = 0 ;
		lw->shield_flag = 0 ;
		return ;
	}
	if( pl_w->routine_1 >= PL_CAUGHT ){
		ew->routine_0 = 2 ;
		lw->shield_st = 0 ;
		lw->shield_flag = 0 ;
		return ;
	}
	shl1c_04_follow( ew, pl_w ) ;
	if( lw->shield_throw_flag ){
		ew->routine_1 ++ ;
		lw->shield_flag = 0 ;
		SHL1C_04_LW( ew )->unk_08[0] = 0 ;
		SHL1C_04_LW( ew )->fire[0] = 1 ;
		ew->timer0 = 16 ;
		ew->prio = 10 ;
		ew->spd.x = !pl_w->pl_dir ? -DOT_X( 4.0f ) : DOT_X( 4.0f ) ;
		ew->acc.x = 0.0f ;
		return ;
	}
	shl1c_04_a_hold_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl1c_04_a_hold_count( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;

	if( pl_w->hit_stop ){
		ew->routine_2 ++ ;
		return ;
	}
	if( lw->shield_st >= 0 ){
		if( !-- ew->timer0 ){
			lw->shield_flag = 0 ;
			SHL1C_04_LW( ew )->unk_08[0] = 0 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl1c_04_a_hold_stop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !pl_w->hit_stop ) ew->routine_2 = 0 ;
}

/*==============================================================*/
/*			It waits, then lets the shots go					*/
/*==============================================================*/
static void shl1c_04_a_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		SHL1C_04_LW( ew )->fire[0] = -1 ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
	}
}

/*--------------------------------------------------------------*/
static void shl1c_04_a_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL1C_LOCAL_W *lw ;

	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		lw = ( PL1C_LOCAL_W * )pl_w->tlocal ;
		lw->shield_st = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void shl1c_04_a_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
}

static void shl1c_04_b_init( PLAYER_WORK *ew );
static void shl1c_04_b_move( PLAYER_WORK *ew );
static void shl1c_04_b_fall( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_04_b_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_04_b_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_04_b_init, shl1c_04_b_move, shl1c_04_b_fall, shl1c_04_free,
	} ;

	shl1c_04_b_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_04_b_init( PLAYER_WORK *ew )
{
	static Sint16 shl1c_04_time_tbl[] = { 0, 7, 15, 23, 31, 39, 47, 55 } ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c04 ;
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
	ew->sub_prio = -1 ;
	ew->timer1 = shl1c_04_time_tbl[ew->type - 1] ;
	pl_char_set( ew, Char_Effect, 9 ) ;
	shl1c_04_b_move( ew ) ;
}

static void shl1c_04_b_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_b_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_b_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_04_b_move( PLAYER_WORK *ew )
{
	static void ( *shl1c_04_b_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_04_b_keep, shl1c_04_b_wait, shl1c_04_b_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		SHL1C_04_LW( pl_w )->shot[0] &= ~( 1 << ew->type ) ;
		return ;
	}
	shl1c_04_b_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl1c_04_orbit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It circles the charge until the character lets		*/
/*			it go												*/
/*==============================================================*/
static void shl1c_04_b_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( SHL1C_04_LW( pl_w )->fire[0] ){
		ew->routine_1 ++ ;
		ew->atck_flag = HIT_ON | HIT_OBJ ;
		ew->dm_flag = HIT_ON | HIT_OBJ ;
		ew->atck_no = pl1c_at_shield_throw ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, Char_Effect, 10 ) ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
		return ;
	}
	shl1c_04_orbit( ew, pl_w ) ;
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_04_b_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		SHL1C_04_LW( pl_w )->shot[0] &= ~( 1 << ew->type ) ;
		return ;
	}
	if( SHL1C_04_LW( pl_w )->fire[0] < 0 ){
		ew->routine_1 ++ ;
		shl1c_04_orbit( ew, pl_w ) ;
	}
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_04_b_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_flag || ew->hit_box_no ){
		ew->routine_0 ++ ;
		ew->routine_1 = 0 ;
		SHL1C_04_LW( pl_w )->shot[0] &= ~( 1 << ew->type ) ;
		return ;
	}
	shl1c_04_orbit( ew, pl_w ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

static void shl1c_04_b_fall_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_04_b_fall_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It falls and goes out								*/
/*==============================================================*/
static void shl1c_04_b_fall( PLAYER_WORK *ew )
{
	static void ( *shl1c_04_b_fall_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl1c_04_b_fall_init, shl1c_04_b_fall_move,
	} ;

	shl1c_04_b_fall_jmp[ew->routine_1]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void shl1c_04_b_fall_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_1 ++ ;
	ew->timer0 = 16 ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -DOT_Y( 0.125f ) ;
	if( ew->work2 ) pl_char_set2( ew, Char_Effect, 9, 4 ) ;
	shl1c_04_b_fall_move( ew, pl_w ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_04_b_fall_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ) ew->routine_0 ++ ;
	ew->disp_flag = ew->timer0 & 1 ;
}

/*--------------------------------------------------------------*/
static void shl1c_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The charge sits in front of the character, at a		*/
/*			height by his stance								*/
/*==============================================================*/
static void shl1c_04_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static Float shl1c_04_ofs_tbl[2][4] = {
		{ DOT_Y( 38.0f ), 60.0f, DOT_Y( 54.0f ), DOT_Y( 16.0f ) },
		{ DOT_Y( 31.0f ), DOT_Y( 22.0f ), DOT_Y( 36.0f ), DOT_Y( 16.0f ) },
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->pl_type == PL_MEGAMAN ){
		if( !pl_w->disp_dir ) ew->pos.x = pl_w->pos.x - DOT_X( 7.0f ) ;
		else ew->pos.x = pl_w->pos.x + DOT_X( 7.0f ) ;
		ew->pos.y = pl_w->pos.y + shl1c_04_ofs_tbl[0][pl_w->pl_stance] ;
	} else {
		if( !pl_w->disp_dir ) ew->pos.x = pl_w->pos.x - DOT_X( 4.0f ) ;
		else ew->pos.x = pl_w->pos.x + DOT_X( 4.0f ) ;
		ew->pos.y = pl_w->pos.y + shl1c_04_ofs_tbl[1][pl_w->pl_stance] ;
	}
}

/*==============================================================*/
/*			A shot goes round the charge, timer1 steps of		*/
/*			1/64 of a turn										*/
/*==============================================================*/
static void shl1c_04_orbit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 ang ;
	Float rad ;

	ew->prio = pl_w->prio ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	rad = 998400.0f ;
	ang = ( ( 72 - ew->timer1 ) & 63 ) << 10 ;
	ew->pos.x = ew->pos.x + rad * nlCos( ang ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	ew->pos.y = ew->pos.y + rad * nlSin( ang ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
	ew->timer1 ++ ;
	ew->timer1 &= 63 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_04.c" End of File	*/
