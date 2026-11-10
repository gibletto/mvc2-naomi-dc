/* Jill's effect 02 (character 11's shot 02): her shots. Type 0 is the grenade of the Grenade
   Launcher, on the strength of the buttons held; type 1 a rocket of the Rocket Launcher
   hyper combo; type 2 the rocket that ends Tyrant Escape, which counts as the other side's. */
#include "player.h"
#include "pl_char.h"
#include "pl11_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL11_02_LOCAL_W {
	Uint16	char_no;	/* 00: the animation Jill was in when it began */
	Uint16	unk_02;
	Float	stop_x;		/* 04: where it stops, in x */
} SHL11_02_LOCAL_W;

/* the hyper combo's shots: where they start from Jill, the point they aim at and their rise */
static Float shl11_02_tbl[][4] = {
	{ DOT_X( 92.0f ), DOT_Y( 86.0f ), DOT_X( 32.0f ), DOT_Y( 0.5f ) },
	{ DOT_X( 98.0f ), DOT_Y( 73.0f ), DOT_X( 40.0f ), -DOT_Y( 0.5f ) },
	{ DOT_X( 98.0f ), 180.0f, DOT_X( 40.0f ), DOT_Y( 0.5f ) },
	{ 155.0f, DOT_Y( 74.0f ), DOT_X( 32.0f ), -DOT_Y( 0.5f ) },
} ;

static PL11_LOCAL_W *shl11_02_lw ;	/* Jill's tlocal, set each frame by the main routine */
static SHL11_02_LOCAL_W *shl11_02_ew_w ;	/* the effect's own work */

static void shl11_02_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl11_02_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl11_02_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->sub_type = no ;
		ew->ef_no = 0x1102 ;
	}
	return ew ;
}

static void shl11_02_a_main( PLAYER_WORK *ew );
static void shl11_02_b_main( PLAYER_WORK *ew );
static void shl11_02_c_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_02_main( PLAYER_WORK *ew )
{
	static void ( *shl11_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_a_main, shl11_02_b_main, shl11_02_c_main,
	} ;

	shl11_02_lw = ( PL11_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl11_02_ew_w = ( SHL11_02_LOCAL_W * )&ew->obj_flag ;
	shl11_02_jmp[ew->type]( ew ) ;
}

static void shl11_02_a_init( PLAYER_WORK *ew );
static void shl11_02_a_move( PLAYER_WORK *ew );
static void shl11_02_end( PLAYER_WORK *ew );
static void shl11_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_02_a_main( PLAYER_WORK *ew )
{
	static void ( *shl11_02_a_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_a_init, shl11_02_a_move, shl11_02_end, shl11_02_free,
	} ;

	shl11_02_a_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_02_a_init( PLAYER_WORK *ew )
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
	ew->disp_flag = 0 ;
	shl11_02_ew_w->char_no = ew->master_w->char_no.w ;
	ew->routine_3 = 0 ;
}

static void shl11_02_a_shot( PLAYER_WORK *ew );
static void shl11_02_a_fly( PLAYER_WORK *ew );
static void shl11_02_a_hit( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_02_a_move( PLAYER_WORK *ew )
{
	static void ( *shl11_02_a_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_a_shot, shl11_02_a_fly, shl11_02_a_hit,
	} ;

	shl11_02_a_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			It waits for the animation frame that fires, and	*/
/*			goes if Jill is out of her move						*/
/*==============================================================*/
static void shl11_02_a_shot( PLAYER_WORK *ew )
{
	static Float shl11_02_a_tbl[][4] = {
		{ DOT_X( 5.0f ), 0.0f, DOT_Y( 3.0f ), -DOT_Y( 0.125f ) },
		{ 5.0f, 0.0f, DOT_Y( 6.0f ), -DOT_Y( 0.25f ) },
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( shl11_02_ew_w->char_no != pl_w->char_no.w ){
		shl11_02_end( ew ) ;
		return ;
	}
	if( pl_w->cpu_flag ) ew->routine_3 = 1 ;
	else if( !pl_w->pl_new_lvbt & ( pl_w->atck_level ? 0x100 : 0x200 ) ) ew->routine_3 = 1 ;
	if( !pl_w->cg_type ) return ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->prio = 11 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl11_at_grenade_vc ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl11_at_grenade ;
		HIT_RESET( ew ) ;
	}
	ew->spd.x = ew->disp_dir ? shl11_02_a_tbl[ew->atck_level][0] : -shl11_02_a_tbl[ew->atck_level][0] ;
	ew->acc.x = ew->disp_dir ? shl11_02_a_tbl[ew->atck_level][1] : -shl11_02_a_tbl[ew->atck_level][1] ;
	ew->spd.y = shl11_02_a_tbl[ew->atck_level][2] ;
	ew->acc.y = shl11_02_a_tbl[ew->atck_level][3] ;
	ew->rot[2] = nlArcTan2( ew->spd.y, ew->spd.x ) ;
	ew->rot[2] = ew->disp_dir ? -ew->rot[2] : ( ew->rot[2] + 0x8000 ) & 0xffff ;
	if( !ew->atck_level ){
		ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 56.0f ) : -DOT_X( 56.0f ) ) ;
		ew->pos.y = ew->master_w->pos.y + DOT_Y( 90.0f ) ;
	}
	else {
		ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ) ;
		ew->pos.y = ew->master_w->pos.y + DOT_Y( 111.0f ) ;
	}
	ew->timer0 = 0 ;
	ew->timer1 = 2 ;
	pl_char_set( ew, Char_Effect, 20 ) ;
}

/*==============================================================*/
/*			vec is never used									*/
/*==============================================================*/
static void shl11_02_a_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	VEC3 vec ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	ew->rot[2] = nlArcTan2( ew->spd.y, ew->spd.x ) ;
	ew->rot[2] = ew->disp_dir ? -ew->rot[2] : ( ew->rot[2] + 0x8000 ) & 0xffff ;
	ew->timer0 ++ ;
	if( !-- ew->timer1 ){
		ew->timer1 = 3 ;
		pls11_00_set2( ew, 4, 0 ) ;
	}
	if( ew->pos.y > pl_w->ground_y && !ew->hit_flag ){
		if( !ew->routine_3 && !pl_w->cpu_flag ){
			if( pl_w->pl_new_lvbt & ( pl_w->atck_level ? 0x100 : 0x200 ) ){
				if( ew->timer0 < 60 ){
					hit_work_set( ew ) ;
					return ;
				}
			}
			else if( ew->timer0 < 10 ){
				ew->routine_3 = 1 ;
				hit_work_set( ew ) ;
				return ;
			}
		}
		else if( ew->timer0 < 29 ){
			hit_work_set( ew ) ;
			return ;
		}
	}
	ew->routine_1 ++ ;
	shl11_02_lw->grenade_end_flag = 1 ;
	pl_se_req( ew, 73 ) ;
	if( pl_w->team_type == 3 ){
		ew->atck_no = pl11_at_grenade_blast_vc ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl11_at_grenade_blast ;
		HIT_RESET( ew ) ;
	}
	pl_char_set( ew, Char_Effect, 21 ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_02_a_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl11_02_end( ew ) ;
}

static void shl11_02_b_init( PLAYER_WORK *ew );
static void shl11_02_b_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_02_b_main( PLAYER_WORK *ew )
{
	static void ( *shl11_02_b_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_b_init, shl11_02_b_move, shl11_02_end, shl11_02_free,
	} ;

	shl11_02_b_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_02_b_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = ( ew->sub_type & 0x80 ) ? 73 : 64 ;
	HIT_RESET( ew ) ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 15.0f : -15.0f ;
	ew->spd.y = shl11_02_tbl[ew->sub_type & 3][3] ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? shl11_02_tbl[ew->sub_type & 3][0] : -shl11_02_tbl[ew->sub_type & 3][0] ) ;
	ew->pos.y = ew->master_w->pos.y + shl11_02_tbl[ew->sub_type & 3][1] ;
	shl11_02_ew_w->stop_x = ew->disp_dir ? -shl11_02_tbl[ew->sub_type & 3][2] + 320.0f : shl11_02_tbl[ew->sub_type & 3][2] + -320.0f ;
	ew->rot[2] = nlArcTan2( ew->spd.y, ew->spd.x ) ;
	ew->rot[2] = ew->disp_dir ? -ew->rot[2] : ( ew->rot[2] + 0x8000 ) & 0xffff ;
	ew->timer0 = 4 ;
	pl_char_set( ew, Char_Effect, 22 ) ;
}

static void shl11_02_b_fly( PLAYER_WORK *ew );
static void shl11_02_b_hit( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_02_b_move( PLAYER_WORK *ew )
{
	static void ( *shl11_02_b_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_b_fly, shl11_02_b_hit,
	} ;

	shl11_02_b_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_02_b_fly( PLAYER_WORK *ew )
{
	VEC3 vec ;
	Float lim ;
	Float d ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->timer0 = 4 ;
		pls11_00_set2( ew, 7, 0 ) ;
	}
	if( !ew->hit_flag ){
		lim = scr_work.eye[0].x + shl11_02_ew_w->stop_x ;
		d = ew->disp_dir ? lim - ew->pos.x : ew->pos.x - lim ;
		if( d > 0.0f ){
			hit_work_set( ew ) ;
			return ;
		}
	}
	ew->routine_1 ++ ;
	ew->atck_no = ( ew->sub_type & 0x80 ) ? 74 : 65 ;
	HIT_RESET( ew ) ;
	ew->disp_flag = 0 ;
	pl_char_set( ew, Char_Effect, 21 ) ;
	vec = ew->pos ;
	burst_set( ew, &vec, 1, 0xf9 ) ;
	pl_se_req( ew, 73 ) ;
}

/*--------------------------------------------------------------*/
static void shl11_02_b_hit( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		shl11_02_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

static void shl11_02_c_init( PLAYER_WORK *ew );
static void shl11_02_c_move( PLAYER_WORK *ew );

/*==============================================================*/
static void shl11_02_c_main( PLAYER_WORK *ew )
{
	static void ( *shl11_02_c_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_c_init, shl11_02_c_move, shl11_02_end, shl11_02_free,
	} ;

	shl11_02_c_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl11_02_c_init( PLAYER_WORK *ew )
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
	ew->prio = 0 ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl11_at_tyrant_shot ;
	HIT_RESET( ew ) ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 15.0f : -15.0f ;
	ew->spd.y = shl11_02_tbl[0][3] ;
	ew->pos.x = ew->master_w->pos.x + ( ew->disp_dir ? shl11_02_tbl[0][0] : -shl11_02_tbl[0][0] ) ;
	ew->pos.y = ew->master_w->pos.y + shl11_02_tbl[0][1] ;
	ew->rot[2] = nlArcTan2( ew->spd.y, ew->spd.x ) ;
	ew->rot[2] = ew->disp_dir ? -ew->rot[2] : ( ew->rot[2] + 0x8000 ) & 0xffff ;
	ew->timer0 = 16 ;
	pl_char_set( ew, Char_Effect, 22 ) ;
}

static void shl11_02_c_fly( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl11_02_c_move( PLAYER_WORK *ew )
{
	static void ( *shl11_02_c_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl11_02_c_fly, shl11_02_end,
	} ;

	shl11_02_c_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl11_02_c_fly( PLAYER_WORK *ew )
{
	VEC3 vec ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->timer0 = 12 ;
		pls11_00_set2( ew, 7, 0 ) ;
	}
	if( !ew->hit_flag ){
		if( !scr_in_x_ck( ew ) ){
			shl11_02_end( ew ) ;
			return ;
		}
		hit_work_set( ew ) ;
		return ;
	}
	ew->routine_1 ++ ;
	vec = ew->pos ;
	burst_set( ew, &vec, 1, 0x179 ) ;
}

/*--------------------------------------------------------------*/
static void shl11_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	shl11_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl11_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl11_02.c" End of File	*/
