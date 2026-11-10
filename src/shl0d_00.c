/* Hulk's Gamma Crush effects (character 0d's shot 00), by kind: 0 the attack that rides on him, 1 a rock
   thrown up where he lands, 2 the trail of dust behind him. */
#include "player.h"
#include "pl_char.h"
#include "pl0d_def.h"

extern void dm_ef2_set( PLAYER_WORK *pl_w );

/* a rock's speed x and y and its fall (8.8), sixteen of them chosen at random */
Sint16 shl0d_00_rock_spd[16 * 3] = {
	-128, 640, -32,		0, 256, -24,	0, 256, -24,	512, 768, -40,
	768, 512, -32,		-384, 640, -32,	-1024, 512, -16,	1024, 768, -48,
	160, 640, -64,		-448, 608, -56,	128, 448, -40,	640, 864, -56,
	-896, 544, -48,		-448, 704, -56,	1024, 576, -32,	0, 256, -16,
} ;

/* the trail's speed x and y, by the setter's second number */
Sint16 shl0d_00_trail_spd[] = { -127, 24, -67, 54, -61, 2, 11, 27 } ;

static void shl0d_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl0d_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl0d_00_main ;
		ew->ef_no = 0x0d00 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = a ;
		ew->sub_type = b ;
	}
	return ew ;
}

static void shl0d_00_atck_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_trail_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_00_main( PLAYER_WORK *ew )
{
	static void ( *shl0d_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_atck_main, shl0d_00_rock_main, shl0d_00_trail_main,
	} ;

	shl0d_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl0d_00_atck_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_atck_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_00_atck_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_00_atck_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_atck_init, shl0d_00_atck_move, shl0d_00_end, shl0d_00_free,
	} ;

	shl0d_00_atck_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_00_atck_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
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
	ew->sub_prio = 1 ;
	ew->atck_no = pl_w->type + 63 ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	pl_char_set( ew, 22, pl_w->type + 6 ) ;
	ew->timer1 = 0 ;
	shl0d_00_atck_move( ew, pl_w ) ;
}

static void shl0d_00_atck_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_atck_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_atck_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_00_atck_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_00_atck_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_atck_rise, shl0d_00_atck_fall, shl0d_00_atck_land,
	} ;

	ew->prio = pl_w->prio ;
	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		shl0d_00_end( ew, pl_w ) ;
		return ;
	}
	shl0d_00_atck_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_00_atck_rise( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !ew->timer1 ){
		hit_work_set( ew ) ;
		if( ew->hit_flag ){
			pl_w->hit_stop = ew->hit_stop ;
			ew->timer1 = 1 ;
		}
	}
	if( pl_w->pos.y <= pl_w->ground_y + 480.0f ){
		ew->atck_no = pl0d_at_crush_fall ;
		HIT_RESET( ew ) ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl0d_00_atck_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0D_LOCAL_W *lw = ( PL0D_LOCAL_W * )pl_w->tlocal ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	if( !( vs_sys_work.slow_bit & 1 << vs_sys_work.slow_step ) ){
		if( ew->hit_stop ){
			pl_w->hit_stop = ew->hit_stop ;
			ew->hit_stop -- ;
		} else {
			ew->atck_no = pl0d_at_crush_fall ;
			HIT_RESET( ew ) ;
			ew->atck_attr |= ATR_DOWN_LIFT ;
			hit_work_set( ew ) ;
		}
		if( lw->crush_land_flag ){
			ew->routine_1 ++ ;
			ew->atck_no = pl_w->type + 63 ;
			HIT_RESET( ew ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl0d_00_atck_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0D_LOCAL_W *lw = ( PL0D_LOCAL_W * )&pl_w->tlocal[0] ;

	hit_work_set( ew ) ;
	if( lw->crush_bound_flag ) ew->routine_0 ++ ;
}

static void shl0d_00_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_00_rock_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_00_rock_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_rock_init, shl0d_00_rock_move, shl0d_00_end, shl0d_00_free,
	} ;

	shl0d_00_rock_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_00_rock_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 ofs ;
	Uint8 no ;

	ew->routine_0 ++ ;
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
	ew->sub_prio = -1 ;
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ofs = Rnd() & 127 ;
	ew->pos.x += ( 64 - ofs ) * 1.6666666f ;
	ofs = Rnd() & 127 ;
	ew->pos.y += ( 64 - ofs ) * 2.142857f ;
	switch( pl_w->type ){
	case 0 :
		ofs = -106 ;
		break ;
	case 1 :
		ofs = 106 ;
		break ;
	}
	if( pl_w->pl_dir ) ofs = -ofs ;
	ew->pos.x += ofs ;
	no = Rnd() & 15 ;
	ew->spd.x = ( shl0d_00_rock_spd[no * 3] << 8 ) * 1.6666666f / 65536.0f ;
	ew->spd.y = ( shl0d_00_rock_spd[no * 3 + 1] << 8 ) * 2.142857f / 65536.0f ;
	ew->acc.y = ( shl0d_00_rock_spd[no * 3 + 2] << 8 ) * 2.142857f / 65536.0f ;
	ew->timer0 = ( Rnd() & 15 ) + 1 ;
	pl_char_set( ew, Char_Effect, ( Rnd() & 7 ) + 11 ) ;
	shl0d_00_rock_move( ew, pl_w ) ;
}

static void shl0d_00_rock_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_rock_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_00_rock_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_00_rock_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_rock_wait, shl0d_00_rock_fly,
	} ;

	ew->prio = pl_w->prio ;
	shl0d_00_rock_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_00_rock_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
		ew->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void shl0d_00_rock_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !-- ew->timer0 ){
		ew->routine_0 ++ ;
		shl0d_00_end( ew, pl_w ) ;
		return ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

static void shl0d_00_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl0d_00_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl0d_00_trail_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_00_trail_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_trail_init, shl0d_00_trail_move, shl0d_00_end, shl0d_00_free,
	} ;

	shl0d_00_trail_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_00_trail_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 *dt ;
	Sint32 x, y ;

	ew->routine_0 ++ ;
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
	ew->sub_prio = -1 ;
	ew->disp_flag = 0 ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	dt = &shl0d_00_trail_spd[ew->sub_type * 2] ;
	x = dt[0] << 16 ;
	y = dt[1] << 16 ;
	ew->spd.x = x * 1.6666666f / 65536.0f ;
	ew->spd.y = y * 2.142857f / 65536.0f ;
	if( pl_w->type == 1 ) ew->disp_dir ^= 1 ;
	pl_char_set( ew, Char_Effect, 19 ) ;
	ew->timer0 = 0 ;
	shl0d_00_trail_move( ew, pl_w ) ;
}

static void shl0d_00_trail_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl0d_00_trail_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl0d_00_trail_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl0d_00_trail_fly,
	} ;

	if( pl_w->char_no.b.grp != Char_Super ){
		ew->routine_0 ++ ;
		shl0d_00_end( ew, pl_w ) ;
		return ;
	}
	shl0d_00_trail_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl0d_00_trail_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL0D_LOCAL_W *lw = ( PL0D_LOCAL_W * )pl_w->tlocal ;
	VEC3 pos ;

	if( lw->crush_bound_flag ){
		ew->routine_0 ++ ;
		shl0d_00_end( ew, pl_w ) ;
		return ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.z = pl_w->pos.z ;
	ew->pos.x += ew->spd.x ;
	ew->pos.y += ew->spd.y ;
	if( !( ew->timer0 & 15 ) ){
		pos.x = ew->pos.x ;
		pos.y = ew->pos.y ;
		pos.z = 0.0f ;
		dm_ef2_set( pl_w ) ;
	}
	ew->timer0 ++ ;
}

/*--------------------------------------------------------------*/
static void shl0d_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl0d_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl0d_00.c" End of File	*/
