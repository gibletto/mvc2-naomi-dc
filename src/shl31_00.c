/* Spiral's effect 00 (character 31's shot 00): the swords she puts out, six of them. Each stays
   beside her, turned by its place, until she fires it; then it flies, sticks, or falls away. */
#include "player.h"
#include "pl_char.h"
#include "pl31_def.h"

/* each sword's turn, by its place */
static Uint8 shl31_00_dir_tbl[] = {
	29, 24, 19, 13, 8, 3,
} ;

/* how far a fired sword moves each frame in x and y, by her strength */
static Sint8 shl31_00_ofs_tbl[] = {
	-6, 0, 0, 8,
} ;

/* the shake of a sword stuck in the enemy, by the frames left */
static Sint8 shl31_00_shake_tbl[] = {
	-2, 2, -2, 2, -1, 1, -1, 1,
} ;

/* the effect's own work */
typedef struct SHL31_00_LOCAL_W {
	Sint16	x, y;		/* 000: its place */
	Sint16	dx, dy;		/* 004: how far it moves each frame once fired */
	Sint16	spd;		/* 008: the speed of the sword that curves */
} SHL31_00_LOCAL_W;
#define SHL31_00_LW( w )	( ( SHL31_00_LOCAL_W * )&( w )->obj_flag )

static void shl31_00_main( PLAYER_WORK *ew );

/****************************************************************/
/* the callers' prototype takes ints; they are kept as bytes */
void *shl31_00_set( PLAYER_WORK *pl_w, int a, int b )
{
	PLAYER_WORK *ew ;
	Uint8 type = a ;
	Uint8 no = b ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl31_00_main ;
		ew->ef_no = 0x3100 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->type = type ;
		ew->sub_type = no ;
	}
	return ew ;
}

static void shl31_00_sword_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl31_00_main( PLAYER_WORK *ew )
{
	static void ( *shl31_00_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_00_sword_main,
	} ;

	shl31_00_jmp[ew->type]( ew, ew->master_w ) ;
}

static void shl31_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl31_00_sword_main( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_00_sword_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_00_init, shl31_00_move, shl31_00_end, shl31_00_free,
	} ;

	shl31_00_sword_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl31_00_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL31_00_LOCAL_W *lw = SHL31_00_LW( ew ) ;

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
	ew->atck_no = pl31_at_tobashi ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->dir = shl31_00_dir_tbl[ew->sub_type] ;
	if( pl_w->disp_dir ){
		ew->dir = 32 - ew->dir ;
		ew->dir &= 31 ;
	}
	ew->pos.z = pl_w->pos.z ;
	lw->x = pl_w->pos.x ;
	lw->y = pl_w->pos.y + DOT_Y( 64.0f ) ;
	ew->pos.x = lw->x ;
	ew->pos.y = lw->y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	dir_spd_move( ew, 6400 ) ;
	pl_char_set( ew, Char_Effect, 1 ) ;
	shl31_00_move( ew, pl_w ) ;
}

static void shl31_00_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl31_00_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_00_out, shl31_00_follow, shl31_00_fire,
	} ;

	ew->prio = 11 ;
	shl31_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It comes out beside her, turning to its place		*/
/*==============================================================*/
static void shl31_00_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *plw = PL31_LW( pl_w ) ;
	SHL31_00_LOCAL_W *lw = SHL31_00_LW( ew ) ;

	if( !plw->sword_flag[ew->sub_type] ){
		ew->routine_0 = 3 ;
		return ;
	}
	if( pl_w->routine_1 ){
		ew->routine_0 = 3 ;
		plw->sword_flag[ew->sub_type] = 0 ;
		return ;
	}
	lw->x = pl_w->pos.x ;
	lw->y = pl_w->pos.y + DOT_Y( 64.0f ) ;
	ew->pos.x = lw->x ;
	ew->pos.y = lw->y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	dir_spd_move( ew, 6400 ) ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		pl_char_set2( ew, Char_Effect, 3, ew->dir / 2 ) ;
	}
}

/*==============================================================*/
/*			It stays beside her until she fires it or is hit	*/
/*==============================================================*/
static void shl31_00_follow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL31_LOCAL_W *plw = PL31_LW( pl_w ) ;
	SHL31_00_LOCAL_W *lw = SHL31_00_LW( ew ) ;

	if( !plw->sword_flag[ew->sub_type] ){
		ew->routine_0 = 3 ;
		return ;
	}
	if( pl_w->routine_1 ){
		ew->routine_0 = 2 ;
		ew->routine_2 = 1 ;
		ew->routine_3 = 0 ;
		plw->sword_flag[ew->sub_type] = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->disp_dir != pl_w->disp_dir ){
		ew->disp_dir = pl_w->disp_dir ;
		ew->dir = shl31_00_dir_tbl[ew->sub_type] ;
		if( pl_w->disp_dir ){
			ew->dir = 32 - ew->dir ;
			ew->dir &= 31 ;
		}
	}
	lw->x = pl_w->pos.x ;
	lw->y = pl_w->pos.y + DOT_Y( 64.0f ) ;
	ew->pos.x = lw->x ;
	ew->pos.y = lw->y ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	dir_spd_move( ew, 6400 ) ;
	if( plw->fire_flag[ew->sub_type] ){
		plw->fire_flag[ew->sub_type] = 0 ;
		plw->sword_flag[ew->sub_type] = 0 ;
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		pl_voice_req( pl_w, 33 ) ;
	}
	else if( plw->fire_level ){
		plw->sword_flag[ew->sub_type] = 0 ;
		ew->atck_level = pl_w->atck_level ;
		switch( ew->atck_level ){
		case 0 :
			ew->routine_2 = 1 ;
			break ;
		case 1 :
			ew->routine_2 = 1 ;
			break ;
		case 2 :
			ew->routine_2 = 2 ;
			break ;
		default :
			ew->routine_2 = 3 ;
			break ;
		}
		ew->routine_1 ++ ;
		ew->routine_3 = 0 ;
		pl_voice_req( pl_w, 33 ) ;
	}
}

static void shl31_00_fire_front( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_fire_orbit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_fire_curve( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_fire_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It is fired: four ways, by her strength (0 when		*/
/*			she was hit)										*/
/*==============================================================*/
static void shl31_00_fire( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_00_fire_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_00_fire_front, shl31_00_fire_orbit, shl31_00_fire_curve, shl31_00_fire_out,
	} ;

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
	shl31_00_fire_jmp[ew->routine_2]( ew, pl_w ) ;
}

static void shl31_00_hit_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			A straight shot										*/
/*==============================================================*/
static void shl31_00_fire_front( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		if( !ew->disp_dir ) ew->dir = 24 ;
		else ew->dir = 8 ;
		pl_char_set2( ew, Char_Effect, 3, 12 ) ;
	}
	else dir_spd_move( ew, 700 ) ;
	shl31_00_hit_ck( ew, pl_w ) ;
}

/*==============================================================*/
/*			A shot that is sent along the offsets of the		*/
/*			table												*/
/*==============================================================*/
static void shl31_00_fire_orbit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL31_00_LOCAL_W *lw = SHL31_00_LW( ew ) ;
	int frame ;

	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		lw->dx = DOT_X( shl31_00_ofs_tbl[ew->atck_level * 2] ) ;
		if( ew->disp_dir ) lw->dx = -lw->dx ;
		lw->dy = DOT_X( shl31_00_ofs_tbl[ew->atck_level * 2 + 1] ) ;
	}
	else {
		if( !ew->disp_dir ) ew->dir -- ;
		else ew->dir ++ ;
		ew->dir &= 31 ;
		if( !ew->disp_dir ) frame = ew->dir / 2 - 4 ;
		else frame = 12 - ew->dir / 2 ;
		frame &= 15 ;
		pl_char_set2( ew, Char_Effect, 3, frame ) ;
		lw->x += lw->dx ;
		lw->y += lw->dy ;
		ew->pos.x = lw->x ;
		ew->pos.y = lw->y ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		dir_spd_move( ew, 4000 ) ;
	}
	shl31_00_hit_ck( ew, pl_w ) ;
}

/*==============================================================*/
/*			A shot that curves and speeds up					*/
/*==============================================================*/
static void shl31_00_fire_curve( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	SHL31_00_LOCAL_W *lw = SHL31_00_LW( ew ) ;
	int frame ;

	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		lw->spd = 6400 ;
	}
	else {
		if( !ew->disp_dir ) ew->dir -- ;
		else ew->dir ++ ;
		ew->dir &= 31 ;
		if( !ew->disp_dir ) frame = ew->dir / 2 - 4 ;
		else frame = 12 - ew->dir / 2 ;
		frame &= 15 ;
		pl_char_set2( ew, Char_Effect, 3, frame ) ;
		ew->pos.x = lw->x ;
		ew->pos.y = lw->y ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		dir_spd_move( ew, lw->spd ) ;
		lw->spd += 100 ;
	}
	shl31_00_hit_ck( ew, pl_w ) ;
}

/*==============================================================*/
/*			A straight shot the other way						*/
/*==============================================================*/
static void shl31_00_fire_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int frame ;

	if( !ew->routine_3 ){
		ew->routine_3 ++ ;
		if( !ew->disp_dir ) frame = ew->dir / 2 ;
		else frame = 16 - ew->dir / 2 ;
		frame &= 15 ;
		pl_char_set2( ew, Char_Effect, 3, frame ) ;
	}
	else dir_spd_move( ew, 700 ) ;
	shl31_00_hit_ck( ew, pl_w ) ;
}

static void shl31_00_stick( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl31_00_shake( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl31_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl31_00_end_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl31_00_stick, shl31_00_drop, shl31_00_shake,
	} ;

	shl31_00_end_jmp[ew->routine_2]( ew, pl_w ) ;
}

/*==============================================================*/
/*			It has struck the ground: it sticks there until		*/
/*			its animation ends									*/
/*==============================================================*/
static void shl31_00_stick( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void shl31_00_drop( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
static void shl31_00_shake( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
			ew->pos.x += shl31_00_shake_tbl[ew->timer1] ;
			ew->timer1 -- ;
		}
		if( !ew->timer0 -- ){
			ew->disp_flag = 0 ;
			ew->routine_0 = 3 ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl31_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*			It has hit the body or the ground (2, 1) or			*/
/*			something else (2, 2)								*/
/*==============================================================*/
static void shl31_00_hit_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
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
/* "shl31_00.c" End of File	*/
