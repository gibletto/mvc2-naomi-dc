/* Magneto's effect 02 (character 2c's shot 02): the columns that rise from the floor, Magnetic
   Shockwave's three and Magnetic Forcefield's one (type 3), with their chains of parts (2 and 4). */
#include "player.h"
#include "pl_char.h"
#include "pl2c_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL2C_02_LOCAL_W {
	Float	y;		/* 000: the height the parts of its chain stand at */
	Sint8	cnt;		/* 004: which part of the chain is lit, counted down */
	Uint8	wait;		/* 005: frames before the next hit */
} SHL2C_02_LOCAL_W;
#define SHL2C_02_LW( w )	( ( SHL2C_02_LOCAL_W * )&( w )->obj_flag )

static void shl2c_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl2c_02_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;
	register int i, x, n ;

	n = 3 ;
	x = 0 ;
	for( i = 0 ; i < n ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl2c_02_main ;
			ew->type = type ;
			ew->sub_type = 0 ;
			ew->master_w = pl_w ;
			ew->pl_type = pl_w->pl_type ;
			if( pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + ( i * 64 + 28 ) * 1.6666666f ;
			else ew->pos.x = pl_w->pos.x - ( i * 64 + 28 ) * 1.6666666f ;
			ew->pos.y = pl_w->ground_y + -DOT_Y( 4.0f ) ;
			ew->ef_no = 0x2c02 ;
			ew->timer0 = x ;
			x += 10 ;
			ew->timer1 = 3 ;
		}
	}
	return ew ;
}

/*==============================================================*/
/*			A part that rides on pl_w, an effect of the same	*/
/*			kind, and shares its master							*/
/*==============================================================*/
static PLAYER_WORK *shl2c_02_part_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2c_02_main ;
		ew->type = type ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->pl_type = pl_w->master_w->pl_type ;
		ew->ef_no = 0x2c01 ;
		ew->sub_type = no ;
	}
	return ew ;
}

/****************************************************************/
PLAYER_WORK *shl2c_02_set2( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2c_02_main ;
		ew->type = 3 ;
		ew->sub_type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x2c02 ;
		if( pl_w->disp_dir ) ew->pos.x = pl_w->pos.x + DOT_X( 92.0f ) ;
		else ew->pos.x = pl_w->pos.x + -DOT_X( 92.0f ) ;
		ew->pos.y = pl_w->ground_y + -DOT_Y( 4.0f ) ;
	}
	return ew ;
}

static void shl2c_02_shock( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_shock_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_field( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_field_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2c_02_main( PLAYER_WORK *ew )
{
	static void ( *shl2c_02_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2c_02_shock, shl2c_02_shock, shl2c_02_shock_part, shl2c_02_field, shl2c_02_field_part,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->routine_0 < 2 ) shl2c_02_jmp[ew->type]( ew, pl_w ) ;
	else {
		if( ew->routine_0 == 2 ){
			/* nothing is left in this test */
		}
		work_free( ew ) ;
	}
}

static void shl2c_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_shock_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_shock_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_shock_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2c_02_shock( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2c_02_shock_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2c_02_shock_wait, shl2c_02_shock_keep, shl2c_02_shock_end,
	} ;

	if( !ew->routine_0 ){
		shl2c_02_init( ew, pl_w ) ;
		SHL2C_02_LW( ew )->cnt = 11 ;
		SHL2C_02_LW( ew )->wait = 0 ;
		SHL2C_02_LW( ew )->y = ew->pos.y + -DOT_Y( 8.0f ) ;
		ew->routine_2 = 1 ;
		if( !scr_in_x_ck( ew ) ){
			work_free( ew ) ;
			return ;
		}
		pl_char_set( ew, 22, 3 ) ;
		ew->sub_prio = -8 ;
	}
	if( -- SHL2C_02_LW( ew )->cnt < 0 ) SHL2C_02_LW( ew )->cnt = 11 ;
	if( SHL2C_02_LW( ew )->wait ) SHL2C_02_LW( ew )->wait -- ;
	ew->prio = pl_w->prio ;
	shl2c_02_shock_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2c_02_shock_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int i ;

	if( -- ew->timer0 < 0 ){
		for( i = 0 ; i <= 12 ; i ++ ){
			if( !shl2c_02_part_set( ew, 2, i ) ) break ;
		}
		ew->timer0 = 10 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl2c_02_shock_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	pl_char_move( ew ) ;
	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2c_02_shock_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		if( -- ew->timer1 == 0 ){
			ew->routine_0 += 2 ;
			ew->disp_flag = 0 ;
		}
		else {
			ew->routine_1 = ew->routine_0 = 0 ;
			if( ew->disp_dir ) ew->pos.x += 320.0f ;
			else ew->pos.x -= 320.0f ;
			ew->timer0 = 0 ;
			shl2c_02_shock( ew, pl_w ) ;
		}
	}
	else if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
}

/*==============================================================*/
static void shl2c_02_shock_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	Sint8 end_flag ;

	if( !ew->routine_0 ){
		shl2c_02_init( ew, pl_w ) ;
		ew->disp_flag = 1 ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = -4 ;
		ew->pos.x = ef_w->pos.x ;
		if( ew->sub_type == 12 ){
			ew->pos.y = SHL2C_02_LW( ef_w )->y + 342.2857f ;
			ew->dir = 5 ;
		}
		else {
			ew->pos.y = SHL2C_02_LW( ef_w )->y + ( 12 - ew->sub_type ) * DOT_Y( 26.0f ) ;
			ew->dir = 19 ;
			if( !ew->sub_type ) ew->dir = 7 ;
		}
		pl_char_set( ew, 22, ew->dir ) ;
	}
	end_flag = pl_char_move( ew ) ;
	if( !ew->routine_1 ){
		if( ef_w->routine_1 != 1 ){
			ew->dir ++ ;
			pl_char_set( ew, 22, ew->dir ) ;
			ew->routine_1 ++ ;
		}
		end_flag = 0 ;
	}
	if( end_flag < 0 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->sub_type != 12 ){
		if( !SHL2C_02_LW( ef_w )->wait ){
			if( ew->hit_flag ) SHL2C_02_LW( ef_w )->wait = 1 ;
			if( SHL2C_02_LW( ef_w )->cnt == ew->sub_type ) hit_work_set( ew ) ;
		}
		if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
		else ew->disp_flag = 0 ;
	}
	if( ew->sub_type >= 1 && ew->sub_type < 12 ) ew->disp_flag = 0 ;
}

/*==============================================================*/
static void shl2c_02_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	if( !ew->routine_2 ){
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
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->size_top = ew->size_bottom = 22 ;
	ew->size_front = ew->size_back = 30 ;
	ew->hit_stop = 0 ;
	ew->disp_flag = 0 ;
	ew->atck_no = pl2c_at_shock ;
	HIT_RESET( ew ) ;
}

static void shl2c_02_field_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_field_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl2c_02_field_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl2c_02_field_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( -- ew->timer0 < 0 ){
		if( shl2c_02_part_set( ew, 4, 0 ) && !shl2c_02_part_set( ew, 4, 1 ) ) ew->routine_0 = 2 ;
		ew->timer0 = 10 ;
		ew->routine_1 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void shl2c_02_field_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 1 ;
	pl_char_move( ew ) ;
	if( -- ew->timer0 < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 22, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl2c_02_field_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 += 2 ;
		ew->disp_flag = 0 ;
	}
	else if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
}

/*==============================================================*/
static void shl2c_02_field( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl2c_02_field_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl2c_02_field_wait, shl2c_02_field_keep, shl2c_02_field_end,
	} ;

	if( !ew->routine_0 ){
		shl2c_02_init( ew, pl_w ) ;
		ew->atck_no = pl2c_at_field ;
		HIT_RESET( ew ) ;
		pl_char_set( ew, 22, 3 ) ;
		ew->sub_prio = -8 ;
	}
	ew->prio = pl_w->prio ;
	shl2c_02_field_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl2c_02_field_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	Sint8 end_flag ;

	if( !ew->routine_0 ){
		shl2c_02_init( ew, pl_w ) ;
		ew->atck_no = pl2c_at_field ;
		HIT_RESET( ew ) ;
		ew->disp_flag = 1 ;
		ew->prio = pl_w->prio ;
		ew->sub_prio = -4 ;
		ew->pos.x = ef_w->pos.x ;
		if( ew->sub_type ){
			ew->pos.y = SHL2C_02_LW( ef_w )->y + 342.2857f ;
			ew->dir = 21 ;
		}
		else ew->dir = 7 ;
		pl_char_set( ew, 22, ew->dir ) ;
	}
	end_flag = pl_char_move( ew ) ;
	if( !ew->routine_1 ){
		if( ef_w->routine_1 != 1 ){
			ew->dir ++ ;
			pl_char_set( ew, 22, ew->dir ) ;
			ew->routine_1 ++ ;
		}
		end_flag = 0 ;
	}
	if( end_flag < 0 ){
		work_free( ew ) ;
		return ;
	}
	if( ew->sub_type ){
		hit_work_set( ew ) ;
		if( scr_in_ck( ew ) ) ew->disp_flag = 1 ;
		else ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2c_02.c" End of File	*/
