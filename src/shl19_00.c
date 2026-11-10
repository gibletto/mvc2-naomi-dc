/* Abyss's second form's grabbing shot (effect 00): it starts beside the player, flies out and bounces
   off the screen's edges, and holds whoever it touches, who has to shake free. */
#include "player.h"
#include "pl_char.h"
#include "pl19_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL19_00_LOCAL_W {
	Sint32	unk_00;
	PLAYER_WORK	*catch_w;	/* 004: the player it holds */
} SHL19_00_LOCAL_W;
#define SHL19_00_LW( w )	( ( SHL19_00_LOCAL_W * )&( w )->obj_flag )

/* where it starts, by type: in front of the player */
static Sint16 shl19_00_x_tbl[4] = { 0, -112, 112 } ;

static void shl19_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl19_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl19_00_main ;
		ew->master_w = pl_w ;
		ew->type = a ;
		ew->sub_no = no ;
	}
	return ew ;
}

static void shl19_00_init( PLAYER_WORK *ew );
static void shl19_00_move( PLAYER_WORK *ew );
static void shl19_00_end( PLAYER_WORK *ew );
static void shl19_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl19_00_main( PLAYER_WORK *ew )
{
	static void ( *shl19_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl19_00_init, shl19_00_move, shl19_00_end, shl19_00_free,
	} ;

	shl19_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl19_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float x ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1900 ;
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
	ew->disp_dir = 0 ;
	ew->prio = 10 ;
	ew->sub_prio = 0 ;
	ew->sub_type = 0 ;
	ew->type = 0 ;
	ew->spd.z = ew->scale.x ;
	ew->acc.z = ew->scale.y ;
	x = DOT_X( shl19_00_x_tbl[ew->sub_no] ) ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = scr_work.eye[0].x + x ;
	ew->pos.y = pl_w->ground_y ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl19_at_bubble ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	pl_char_set( ew, Char_Effect, 3 ) ;
	shl19_00_move( ew ) ;
}

static void shl19_00_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl19_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl19_00_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl19_00_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl19_00_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl19_00_move( PLAYER_WORK *ew )
{
	static void ( *shl19_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl19_00_start, shl19_00_fly, shl19_00_catch, shl19_00_hold, shl19_00_out,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_0 >= 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl19_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

typedef struct SHL19_00_SPD {
	Float	x, y;
} SHL19_00_SPD ;

static SHL19_00_SPD shl19_00_spd_tbl[3] = {
	{ -5.0f, DOT_Y( 3.0f ) },
	{ 5.0f, DOT_Y( 3.0f ) },
	{ -5.0f, DOT_Y( 3.0f ) },
} ;

static void shl19_00_em_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl19_00_wall_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl19_00_scale_move( PLAYER_WORK *ew );

/*==============================================================*/
/*			The throw animation, then it leaps out				*/
/*==============================================================*/
static void shl19_00_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float spd ;

	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 180 ;
		ew->pos.x += 25.0f ;
		ew->pos.y += DOT_Y( 115.0f ) ;
		spd = shl19_00_spd_tbl[ew->sub_no].x ;
		if( ew->sub_no == 0 ){
			if( ew->pos.x < pl_w->target_w->pos.x ) spd = -spd ;
		} else if( pl_w->disp_dir ) spd = -spd ;
		ew->spd.x = spd ;
		ew->spd.y = shl19_00_spd_tbl[ew->sub_no].y ;
		ew->acc.x = 0.0f ;
		ew->acc.y = -DOT_Y( 0.125f ) ;
		pl_char_set( ew, Char_Effect, 4 ) ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			In flight: it holds the first player it hits, or	*/
/*			ends when the time is up							*/
/*==============================================================*/
static void shl19_00_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->routine_1 || ew->hit_box_no || pl_w->tlocal[16] ) goto fly_end ;
	if( ew->hit_flag ){
		if( pl_sp_catch_ck( ew ) ){
			ew->routine_1 ++ ;
			pl_w->tlocal[16] ++ ;
			shl19_00_em_pos_set( ew, pl_w ) ;
			return ;
		}
		goto fly_end ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( --ew->timer0 == 0 ) goto fly_end ;
	shl19_00_wall_ck( ew, pl_w ) ;
	shl19_00_scale_move( ew ) ;
	hit_work_set( ew ) ;
	return ;
fly_end:
	ew->routine_1 = 4 ;
	ew->routine_2 = 0 ;
}

/*==============================================================*/
/*			Once the held player's hit stop is over, it			*/
/*			takes hold of them									*/
/*==============================================================*/
static void shl19_00_catch( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;

	if( !em_w->hit_stop ){
		ew->routine_1 ++ ;
		ew->scale.x = ew->spd.z ;
		ew->scale.y = ew->acc.z ;
		pl_char_set( em_w, Char_Dizzy, 0 ) ;
		pl_ef_catch_set( ew, em_w ) ;
		SHL19_00_LW( ew )->catch_w = pl_w->catch_w ;
		ew->timer0 = 180 ;
		hold_renda_set( pl_w->catch_w, 1 ) ;
		em_w->catch_type = 6 ;
		em_w->catch_no = pl_w->catch_no = 0xc0 ;
		pl_w->catch_flag = 1 ;
		pl_w->catch_grip_old = -1 ;
		if( em_w->pl_stance == STANCE_AIR ){
			em_w->spd.x = em_w->acc.x = em_w->spd.y = em_w->acc.y = 0.0f ;
		}
	}
}

/*==============================================================*/
/*			It holds the player until they shake free or the	*/
/*			time is up											*/
/*==============================================================*/
static void shl19_00_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = SHL19_00_LW( ew )->catch_w ;

	if( ew->catch_pair.w->catch_pair.w != ew || em_w->routine_1 != PL_CAUGHT ) goto hold_end ;
	if( em_w->hit_box_no && em_w->routine_1 == PL_DAMAGE ) goto hold_end ;
	shl19_00_em_pos_set( ew, pl_w ) ;
	if( hold_renda_move_ck( em_w ) ) ew->timer0 -= 3 ;
	if( --ew->timer0 < 0 ){
		em_w->catch_type = 0 ;
		em_w->auto_guard_timer = 8 ;
		if( !em_w->vital_new ) em_w->catch_type = 7 ;
		else if( em_w->pl_stance != STANCE_AIR ) pl_stand_ret( em_w ) ;
		else pl_sjump_ret( em_w ) ;
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
	return ;
hold_end:
	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
}

/*==============================================================*/
/*			It grows and fades away								*/
/*==============================================================*/
static void shl19_00_out( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( !ew->routine_2 ){
		ew->routine_2 ++ ;
		ew->prio = 10 ;
		ew->sub_prio = 0 ;
		ew->scale.x = ew->spd.z * 1.2f ;
		ew->scale.y = ew->acc.z * 1.2f ;
		ew->alpha = 0.5f ;
		pl_char_set( ew, Char_Effect, 5 ) ;
	}
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl19_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl19_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			It keeps to the held player							*/
/*==============================================================*/
static void shl19_00_em_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em_w = ew->em_w ;

	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->pos.y + ( em_w->size_top / 2 ) * 2.142857f ;
}

/* a flying shot swells and shrinks in turn on x and on y; type and sub_type count its steps */
static Float shl19_00_scale_tbl[30] = {
	0.8f, 0.83f, 0.86f, 0.89f, 0.92f, 0.95f, 0.98f, 1.01f, 1.04f, 1.07f,
	1.1f, 1.13f, 1.16f, 1.13f, 1.1f, 1.07f, 1.04f, 1.01f, 0.98f, 0.95f,
	0.92f, 0.89f, 0.92f, 0.95f, 0.98f, 1.01f, 1.04f, 1.07f, 1.04f, 1.01f,
} ;

/*==============================================================*/
static void shl19_00_scale_move( PLAYER_WORK *ew )
{
	if( ew->type ){
		ew->scale.x = shl19_00_scale_tbl[ew->type - 1] * ew->spd.z ;
		if( ++ew->type > 30 ){
			ew->type = 0 ;
			ew->scale.x = ew->spd.z ;
		}
	}
	if( ew->sub_type ){
		ew->scale.y = shl19_00_scale_tbl[ew->sub_type - 1] * ew->acc.z ;
		if( ++ew->sub_type > 30 ){
			ew->sub_type = 0 ;
			ew->scale.y = ew->acc.z ;
		}
	}
}

/*==============================================================*/
/*			It turns round at the screen's edges, which sets	*/
/*			the swelling off; each edge test is a statement		*/
/*			of its own after a goto								*/
/*==============================================================*/
static void shl19_00_wall_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int n = 0 ;

	if( ew->spd.x >= 0.0f ) goto right ;
	if( scr_work.scr_left + 80.0f > ew->pos.x ){
		ew->spd.x = -ew->spd.x ;
		n ++ ;
		ew->type = 1 ;
	}
	goto y_ck ;
right:
	if( ew->pos.x > scr_work.scr_right + -80.0f ){
		ew->spd.x = -ew->spd.x ;
		n ++ ;
		ew->type = 1 ;
	}
y_ck:
	if( ew->spd.y < 0.0f ) goto ground ;
	if( ew->pos.y > scr_work.scr_top + -DOT_Y( 48.0f ) ){
		ew->spd.y = -ew->spd.y ;
		n ++ ;
		ew->sub_type = 1 ;
	}
	goto done ;
ground:
	if( pl_w->ground_y + DOT_Y( 48.0f ) > ew->pos.y ){
		ew->spd.y = -ew->spd.y ;
		n ++ ;
		ew->sub_type = 1 ;
	}
done:
	if( n ){ }
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl19_00.c" End of File	*/
