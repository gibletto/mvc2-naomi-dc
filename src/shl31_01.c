/* Spiral's effect 01 (character 31's shot 01): a sword she has put out, fired. It appears beside
   her, waits, flies at the enemy, and sticks in or falls away. */
#include "player.h"
#include "pl_char.h"
#include "pl31_def.h"

/* where each of the six swords starts from her, x and y in turn */
static Sint16 shl31_01_pos_tbl[] = {
	-54, 99, -60, 83, -56, 67, -54, 51, -50, 35, -46, 19,
} ;

/* the shake of a sword stuck in the enemy, by the frames left */
static Sint8 shl31_01_shake_tbl[] = {
	-2, 2, -2, 2, -1, 1, -1, 1,
} ;

static void shl31_01_main( PLAYER_WORK *ew );

/****************************************************************/
/* the callers' prototype takes ints; they are kept as bytes */
void *shl31_01_set( PLAYER_WORK *pl_w, int a, int b )
{
	PLAYER_WORK *ew ;
	Uint8 type = a ;
	Uint8 no = b ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl31_01_main ;
		ew->ef_no = 0x3101 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl31_01_sword_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl31_01_main( PLAYER_WORK *ew )
{
	static void ( *shl31_01_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_01_sword_main,
	} ;

	shl31_01_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl31_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl31_01_sword_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_01_sword_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_01_init, shl31_01_move, shl31_01_end, shl31_01_free,
	} ;

	shl31_01_sword_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
/*			Each sword strikes with one of two attacks, by		*/
/*			the low bit of its number							*/
/*==============================================================*/
static void shl31_01_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
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
	ew->disp_flag = 1 ;
	ew->sub_prio = -1 ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 16 ;
	no = ew->sub_type ;
	if( no & 1 ){
		ew->atck_no = pl31_at_stampede ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl31_at_stampede_2 ;
		HIT_RESET( ew ) ;
	}
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->disp_dir = 0 ;
	ew->dir = 24 ;
	if( pl_w->disp_dir ){
		ew->dir = 32 - ew->dir ;
		ew->dir &= 31 ;
	}
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y + DOT_Y( shl31_01_pos_tbl[ew->sub_type * 2 + 1] ) ;
	ew->pos.z = pl_w->pos.z ;
	ew->spd.x = DOT_X( shl31_01_pos_tbl[ew->sub_type * 2] ) ;
	if( pl_w->disp_dir ) ew->spd.x = -ew->spd.x ;
	ew->pos.x += ew->spd.x ;
	pl_char_set( ew, Char_Effect, 1 ) ;
	shl31_01_move( ew, pl_w ) ;
}

static void shl31_01_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl31_01_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_01_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_01_appear, shl31_01_wait, shl31_01_fly,
	} ;

	ew->prio = pl_w->prio ;
	shl31_01_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It turns to face the enemy, then waits a few		*/
/*			frames												*/
/*==============================================================*/
static void shl31_01_appear( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int frame ;
	Sint16 time ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		if( !ew->disp_dir ) frame = ew->dir / 2 ;
		else frame = 16 - ew->dir / 2 ;
		frame &= 15 ;
		pl_char_set2( ew, Char_Effect, 3, frame ) ;
		time = Rnd() & 7 ;
		time += 4 ;
		ew->timer0 = time ;
	}
}

/*--------------------------------------------------------------*/
static void shl31_01_wait( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		pl_voice_req( pl_w, 33 ) ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
	}
}

static void shl31_01_hit_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl31_01_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !scr_in_x_ck( ew ) || !ground_ck( ew ) ){
		ew->routine_0 = 3 ;
		return ;
	}
	if( pl_w->ground_y > ew->pos.y ){
		ew->routine_0 = 2 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		return ;
	}
	dir_spd_move( ew, 1000 ) ;
	shl31_01_hit_ck( ew, pl_w ) ;
}

static void shl31_01_stick( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_01_shake( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl31_01_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_01_end_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_01_stick, shl31_01_drop, shl31_01_shake,
	} ;

	shl31_01_end_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It has struck the ground: it sticks there until		*/
/*			its animation ends									*/
/*==============================================================*/
static void shl31_01_stick( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		pl_char_set( ew, Char_Effect, 2 ) ;
	}
	else if( pl_char_move( ew ) < 0 ){
		ew->disp_flag = 0 ;
		ew->routine_0 = 3 ;
	}
}

/*==============================================================*/
/*			It has hit or been struck: it turns back and		*/
/*			falls												*/
/*==============================================================*/
static void shl31_01_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->dir = 32 - ew->dir ;
		ew->dir &= 31 ;
		dir_spd_move( ew, 600 ) ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
	}
	else {
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		if( !scr_in_x_ck( ew ) ){
			ew->routine_0 = 3 ;
			return ;
		}
		if( pl_w->ground_y > ew->pos.y ){
			ew->routine_0 = 2 ;
			ew->routine_2 = 0 ;
			ew->routine_3 = 0 ;
		}
	}
}

/*==============================================================*/
/*			It is stuck in the enemy, and shakes until its		*/
/*			time is up											*/
/*==============================================================*/
static void shl31_01_shake( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int frame ;

	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		if( !ew->disp_dir ) frame = ew->dir / 2 ;
		else frame = 16 - ew->dir / 2 ;
		frame &= 15 ;
		pl_char_set2( ew, Char_Effect, 4, frame ) ;
		ew->timer0 = 20 ;
		ew->timer1 = 7 ;
	}
	else {
		ew->pos.x = ew->em_w->pos.x ;
		if( ew->timer1 ){
			ew->pos.x += shl31_01_shake_tbl[ew->timer1] ;
			ew->timer1 -- ;
		}
		if( !ew->timer0 -- ){
			ew->disp_flag = 0 ;
			ew->routine_0 = 3 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl31_01_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			It has hit the body or the ground (2, 1) or			*/
/*			something else (2, 2)								*/
/*==============================================================*/
static void shl31_01_hit_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->hit_box_no ){
		ew->routine_0 = 2 ;
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
	if( !ew->hit_flag ) return ;
	if( ( ew->hit_flag & HR_GUARD ) || ( ew->hit_flag & HR_ARMOR ) ){
		ew->routine_0 = 2 ;
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
	}
	else {
		ew->routine_0 = 2 ;
		ew->routine_2 = 2 ;
		ew->routine_3 = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl31_01.c" End of File	*/
