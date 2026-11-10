/* Ruby Heart's effects (character 13's shot 00): one module for twenty-six kinds, picked by the
   setter's number (type), each with its own start and moves. Parts set by another effect ride on
   it (ef_w). */
#include "player.h"
#include "pl_char.h"
#include "pl13_def.h"

#define DEG_ANG( d )	( ( Sint32 )( ( d ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* a shot's speed and its change, across and up */
typedef struct SHL13_00_SPD {
	Float	spd_x, acc_x;
	Float	spd_y, acc_y;
} SHL13_00_SPD;

/* an effect's own work, from obj_mtx */
typedef struct SHL13_00_LOCAL_W {
	Sint32	rafale_catch_flag;
	Sint32	throw_catch_flag;		/* 004: a throw's: set once it has caught */
	Sint32	magie_catch_flag;		/* 008: the hyper combo throw's: set once it has caught */
} SHL13_00_LOCAL_W;
#define SHL13_00_LW( w )	( ( SHL13_00_LOCAL_W * )&( w )->obj_mtx )

static Sint16 shl13_00_time[] = { 30, 50 } ;		/* by the master's strength */
static Sint16 shl13_00_fantome_shot_time[] = { 120, 120 } ;
static SHL13_00_SPD shl13_00_fantome_shot_spd[] = {
	{ DOT_X( 2.0f ), -0.026041666f, 0.0f, 0.0f },
	{ DOT_X( 2.0f ), -0.026041666f, DOT_Y( 2.0f ), -0.03348214f },
} ;
static Sint16 shl13_00_mille_shot_time[] = { 120, 120, 120 } ;
static SHL13_00_SPD shl13_00_mille_shot_spd[] = {		/* by sub_no */
	{ DOT_X( 1.75f ), -0.026041666f, 0.0f, 0.0f },
	{ DOT_X( 2.125f ), -0.026041666f, DOT_Y( 2.0f ), -0.03348214f },
	{ DOT_X( 2.75f ), -0.026041666f, DOT_Y( 3.0f ), -0.100446425f },
} ;
static Float shl13_00_ofs[] = {
	0.0f, 0.0f, 26.0f, 52.0f, 78.0f, 104.0f, 130.0f, 156.0f, 182.0f, 208.0f, 234.0f, 260.0f,
} ;
static Uint8 shl13_00_tbl[] = {
	1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2,
	2, 1, 1, 1, 1, 2, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2, 1, 1, 1,
	1, 1, 2, 1, 2, 1, 2, 1, 1, 1, 0, 0, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 0, 1, 2, 2, 1, 2, 0, 1, 1,
	1, 2, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1,
	0, 1, 2, 1, 1, 2, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1,
	2, 2, 2, 1, 1, 1, 0, 0,
} ;
static SHL13_00_SPD shl13_00_throw_spd = { DOT_X( 4.0f ), -0.052083332f, DOT_Y( 4.0f ), -0.06696428f } ;
static Float shl13_00_pos[] = {
	50.0f, DOT_X( 20.0f ), DOT_X( 10.0f ), 0.0f, DOT_X( 35.0f ), DOT_X( 25.0f ), 25.0f, DOT_X( 5.0f ),
	-50.0f, -DOT_X( 20.0f ), -DOT_X( 10.0f ), 0.0f, -DOT_X( 35.0f ), -DOT_X( 25.0f ), -25.0f, -DOT_X( 5.0f ),
	DOT_X( 80.0f ), DOT_X( 70.0f ), 100.0f, DOT_X( 50.0f ), DOT_X( 40.0f ), 50.0f, DOT_X( 20.0f ), DOT_X( 10.0f ),
	125.0f, DOT_X( 65.0f ), DOT_X( 55.0f ), 75.0f, DOT_X( 35.0f ), DOT_X( 25.0f ), 25.0f, DOT_X( 5.0f ),
} ;

static void shl13_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl13_00_set( PLAYER_WORK *pl_w, Uint8 no, int a, int b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl13_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part that rides on the effect pw					*/
/*==============================================================*/
static void *shl13_00_part_set( PLAYER_WORK *pw, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl13_00_main ;
		ew->master_w = pw->master_w ;
		ew->ef_w = pw ;
		ew->type = no ;
	}
	return ew ;
}

/*==============================================================*/
/*			The same, taken from the pool after pw				*/
/*==============================================================*/
static void *shl13_00_part_set2( PLAYER_WORK *pw, Uint8 no, Uint8 a )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( pw, W_SHL, LIST_BEHIND ) ) ){
		ew->work_func = shl13_00_main ;
		ew->master_w = pw->master_w ;
		ew->ef_w = pw ;
		ew->type = no ;
		ew->sub_no = a ;
	}
	return ew ;
}

static void shl13_00_init( PLAYER_WORK *ew );
static void shl13_00_move( PLAYER_WORK *ew );
static void shl13_00_end( PLAYER_WORK *ew );
static void shl13_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl13_00_main( PLAYER_WORK *ew )
{
	static void ( *shl13_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl13_00_init, shl13_00_move, shl13_00_end, shl13_00_free,
	} ;

	shl13_00_jmp[ew->routine_0]( ew ) ;
}

static void ef_fantome_init( PLAYER_WORK *ew );
static void ef_fantome_shot_init( PLAYER_WORK *ew );
static void ef_sublimation_init( PLAYER_WORK *ew );
static void ef_sublimation_part_init( PLAYER_WORK *ew );
static void ef_throw_init( PLAYER_WORK *ew );
static void ef_power_init( PLAYER_WORK *ew );
static void ef_rafale_init( PLAYER_WORK *ew );
static void ef_rafale_link_init( PLAYER_WORK *ew );
static void ef_rafale_throw_init( PLAYER_WORK *ew );
static void ef_schwarz_front_init( PLAYER_WORK *ew );
static void ef_schwarz_mid_init( PLAYER_WORK *ew );
static void ef_schwarz_back_init( PLAYER_WORK *ew );
static void ef_magie_lp_init( PLAYER_WORK *ew );
static void ef_magie_lk_init( PLAYER_WORK *ew );
static void ef_magie_init( PLAYER_WORK *ew );
static void ef_magie_obj_init( PLAYER_WORK *ew );
static void ef_paretonnre_init( PLAYER_WORK *ew );
static void ef_paretonnre_atck_init( PLAYER_WORK *ew );
static void ef_paretonnre_blast_init( PLAYER_WORK *ew );
static void ef_paretonnre_gun_init( PLAYER_WORK *ew );
static void ef_paretonnre_fire_init( PLAYER_WORK *ew );
static void ef_flammeile_init( PLAYER_WORK *ew );
static void ef_mille_init( PLAYER_WORK *ew );
static void ef_mille_shot_init( PLAYER_WORK *ew );
static void ef_fantome_front_init( PLAYER_WORK *ew );
static void ef_mille_front_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl13_00_init( PLAYER_WORK *ew )
{
	static void ( *shl13_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		ef_fantome_init, ef_fantome_shot_init, ef_sublimation_init, ef_sublimation_part_init, ef_throw_init, ef_power_init, ef_rafale_init,
		ef_rafale_link_init, ef_rafale_throw_init, ef_schwarz_front_init, ef_schwarz_mid_init, ef_schwarz_back_init, ef_magie_lp_init, ef_magie_lk_init,
		ef_magie_init, ef_magie_obj_init, ef_paretonnre_init, ef_paretonnre_atck_init, ef_paretonnre_blast_init, ef_paretonnre_gun_init, ef_paretonnre_fire_init,
		ef_flammeile_init, ef_mille_init, ef_mille_shot_init, ef_fantome_front_init, ef_mille_front_init,
	} ;

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
	shl13_00_init_jmp[ew->type]( ew ) ;
	shl13_00_move( ew ) ;
}

/*==============================================================*/
static void ef_fantome_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;
	Sint16 *tm ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 96 : -96 ;
	ew->pos.y += 20.0f ;
	ew->prio = 11 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->fantome_shot_flag = 1 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	tm = shl13_00_time ;
	tm += ew->master_w->atck_level ;
	ew->timer0 = *tm ;
	shl13_00_part_set( ew, 24 ) ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 28 ) ;
}

/*==============================================================*/
static void ef_fantome_shot_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = pl_w->target_w ;
	SHL13_00_SPD *sp ;
	Sint16 *tm ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->pos.y += DOT_Y( 16.0f ) ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->prio = 10 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_fantome ;
	HIT_RESET( ew ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	tm = shl13_00_fantome_shot_time ;
	tm += ew->master_w->atck_level ;
	ew->timer0 = *tm ;
	ew->timer1 = 90 ;
	sp = shl13_00_fantome_shot_spd ;
	sp += ew->master_w->atck_level ;
	ew->spd.x = sp->spd_x ;
	ew->acc.x = sp->acc_x ;
	ew->spd.y = sp->spd_y ;
	ew->acc.y = sp->acc_y ;
	ew->scale.x = 0.4f ;
	ew->scale.y = 0.4f ;
	if( ew->pos.x > em->pos.x ) ew->disp_dir = 0 ;
	else ew->disp_dir = 1 ;
	ew->spd.x = ew->disp_dir ? ew->spd.x : -ew->spd.x ;
	ew->acc.x = ew->disp_dir ? -ew->acc.x : ew->acc.x ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 1 ) ;
}

/*==============================================================*/
static void ef_sublimation_init( PLAYER_WORK *ew )
{
	register PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->disp_dir = ew->master_w->disp_dir ;
	if( !ew->master_w->atck_level ) ew->pos.x += ew->disp_dir ? DOT_X( 112.0f ) : -DOT_X( 112.0f ) ;
	else ew->pos.x += ew->disp_dir ? DOT_X( 224.0f ) : -DOT_X( 224.0f ) ;
	if( ew->disp_dir ){
		if( ew->pos.x > scr_work.scr_right ) ew->pos.x = scr_work.scr_right - 80.0f ;
	}
	else {
		if( scr_work.scr_left > ew->pos.x ) ew->pos.x = scr_work.scr_left + 80.0f ;
	}
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->sublimation_shot_flag = 1 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_sublimation ;
	HIT_RESET( ew ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->prio = 0 ;
	ew->scale.y = 0.0f ;
	ew->timer1 = 4 ;
	pl_se_req( ew, 76 ) ;
	pl_char_set( ew, Char_Effect, 31 ) ;
}

/*==============================================================*/
static void ef_sublimation_part_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->prio = 0 ;
	ew->scale.y = 0.0f ;
	ew->alpha = 1.0f ;
	ew->timer1 = 3 ;
	pl_char_set( ew, Char_Effect, 32 ) ;
}

/*==============================================================*/
static void ef_throw_init( PLAYER_WORK *ew )
{
	SHL13_00_SPD *sp ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 112 : -112 ;
	ew->pos.y += 176.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_throw ;
	HIT_RESET( ew ) ;
	ew->atck_attr |= ATR_SHOT_HOLD ;
	ew->timer0 = 80 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->prio = 0 ;
	sp = &shl13_00_throw_spd ;
	ew->spd.x = sp->spd_x ;
	ew->acc.x = sp->acc_x ;
	ew->spd.y = sp->spd_y ;
	ew->acc.y = sp->acc_y ;
	ew->spd.x = ew->master_w->disp_dir ? ew->spd.x : -ew->spd.x ;
	ew->acc.x = ew->master_w->disp_dir ? -ew->acc.x : ew->acc.x ;
	pl_char_set( ew, Char_Effect, 34 ) ;
}

/*==============================================================*/
static void ef_power_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos = ew->master_w->pos ;
	ew->pos.x += ew->master_w->disp_dir ? 112 : -112 ;
	ew->pos.y += 208.0f ;
	ew->atck_no = pl13_at_power ;
	HIT_RESET( ew ) ;
	ew->pl_no ^= 1 ;
	ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
	ew->dm_flag = HIT_PL_ONLY ;
	ew->prio = 0 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 60 ;
	ew->alpha = 0.7f ;
	ew->scale.x /= 1.6666666f ;
	ew->scale.y /= 2.142857f ;
	ef_color_set( ew, 2, 5, 1 ) ;
	pl_char_set( ew, Char_Effect, 43 ) ;
}

/*==============================================================*/
static void ef_rafale_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_rafale ;
	HIT_RESET( ew ) ;
	ew->prio = 0 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->master_w->cg_type = 0 ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 35 ) ;
}

/*==============================================================*/
static void ef_rafale_link_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->prio = 0 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 36 ) ;
}

/*==============================================================*/
static void ef_rafale_throw_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? -16 : 16 ;
	ew->pos.y += 176.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 33 ) ;
}

/*==============================================================*/
static void ef_schwarz_front_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->prio = 9 ;
	pl_char_set( ew, Char_Effect, ew->master_w->routine_3 + 8 ) ;
}

/*==============================================================*/
static void ef_schwarz_mid_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->prio = 11 ;
	ew->alpha = 0.5f ;
	pl_char_set( ew, Char_Effect, ew->master_w->routine_3 + 13 ) ;
}

/*==============================================================*/
static void ef_schwarz_back_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->prio = 12 ;
	pl_char_set( ew, Char_Effect, ew->master_w->routine_3 + 18 ) ;
}

/*==============================================================*/
static void ef_magie_lp_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.y += 240.0f ;
	ew->prio = 10 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 44 ) ;
}

/*==============================================================*/
static void ef_magie_lk_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.y += DOT_Y( 64.0f ) ;
	ew->prio = 10 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 44 ) ;
}

/*==============================================================*/
static void ef_magie_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = pl_w->target_w ;

	ew->disp_flag = 1 ;
	ew->pos.x = em->pos.x ;
	ew->pos.y = scr_work.scr_top + DOT_Y( 80.0f ) ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->disp_dir ){
		if( ew->pos.x > scr_work.scr_right ) ew->pos.x = scr_work.scr_right - DOT_X( 32.0f ) ;
	}
	else {
		if( scr_work.scr_left > ew->pos.x ) ew->pos.x = scr_work.scr_left + DOT_X( 32.0f ) ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_magie ;
	HIT_RESET( ew ) ;
	ew->prio = 11 ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = -1.2053571f ;
	pl_char_set( ew, Char_Effect, 23 ) ;
}

/*==============================================================*/
static void ef_magie_obj_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 433.33334f : -433.33334f ;
	ew->pos.x += ew->master_w->disp_dir ? 48 : -48 ;
	ew->prio = 9 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ef_color_set( ew, 4, 4, 1 ) ;
	pl_char_set( ew, Char_Effect, 45 ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
}

/*==============================================================*/
static void ef_paretonnre_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->disp_dir = ew->master_w->disp_dir ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x = ew->disp_dir ? scr_work.scr_left + -400.0f : scr_work.scr_right + 400.0f ;
	ew->pos.y += 240.0f ;
	ew->prio = 14 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->scale.x = 1.4f ;
	ew->scale.y = 1.4f ;
	shl13_00_part_set( ew, 17 ) ;
	shl13_00_part_set2( ew, 19, 1 ) ;
	shl13_00_part_set2( ew, 19, 0 ) ;
	pl_char_set( ew, Char_Effect, 2 ) ;
}

/*==============================================================*/
static void ef_paretonnre_atck_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.z = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->timer1 = ew->timer0 = 0 ;
	ew->spd.x = 0.0f ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_paretonnre ;
	HIT_RESET( ew ) ;
	ew->prio = 15 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->scale.x = 1.4f ;
	ew->scale.y = 1.4f ;
	pl_char_set( ew, Char_Effect, 49 ) ;
}

/*==============================================================*/
static void ef_paretonnre_blast_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = pl_w->target_w ;

	ew->disp_flag = 0 ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = em->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = em->pos.z ;
	ew->pos.y += DOT_Y( 16.0f ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( ew->sub_no ){
		ew->atck_no = pl13_at_paretonnre_blast_2 ;
		HIT_RESET( ew ) ;
	} else {
		ew->atck_no = pl13_at_paretonnre_blast ;
		HIT_RESET( ew ) ;
	}
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 28 ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 4 ) ;
}

/*==============================================================*/
static void ef_paretonnre_gun_init( PLAYER_WORK *ew )
{
	Float ofs ;

	ew->disp_flag = 1 ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->prio = 13 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->scale.x = 1.4f ;
	ew->scale.y = 1.4f ;
	ew->pos.y -= 95.99999f ;
	if( ew->sub_no ){
		ofs = 112.0f ;
		ew->pos.x += ew->disp_dir ? ofs : -ofs ;
		pl_char_set( ew, Char_Effect, 3 ) ;
	}
	else {
		ofs = -18.666666f ;
		ew->pos.x += ew->disp_dir ? ofs : -ofs ;
		pl_char_set( ew, Char_Effect, 60 ) ;
	}
}

/*==============================================================*/
static void ef_paretonnre_fire_init( PLAYER_WORK *ew )
{
	Float ofs ;

	ew->disp_flag = 1 ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ofs = 74.666664f ;
	ew->pos.x += ew->disp_dir ? ofs : -ofs ;
	ew->pos.y -= 11.999999f ;
	ew->prio = 12 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->scale.x = 1.4f ;
	ew->scale.y = 1.4f ;
	pl_char_set( ew, Char_Effect, 55 ) ;
}

/*==============================================================*/
static void ef_flammeile_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->prio = 10 ;
	ew->alpha = 1.0f ;
	pl_char_set( ew, Char_Effect, ew->master_w->routine_3 + 50 ) ;
}

/*==============================================================*/
static void ef_mille_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;
	Sint16 *tm ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 96 : -96 ;
	ew->pos.y += 20.0f ;
	ew->prio = 11 ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	lw->mille_shot_flag = 1 ;
	ew->scale.x = 1.5f ;
	ew->scale.y = 1.5f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	tm = shl13_00_time ;
	tm += ew->master_w->atck_level ;
	ew->timer0 = *tm ;
	shl13_00_part_set( ew, 25 ) ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 58 ) ;
}

/*==============================================================*/
/* em is unused, as in ef_paretonnre_blast_init's start */
static void ef_mille_shot_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = pl_w->target_w ;
	SHL13_00_SPD *sp ;
	Sint16 *tm ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->pos.y += DOT_Y( 16.0f ) ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->prio = 10 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl13_at_mille ;
	HIT_RESET( ew ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->sub_type = 0 ;
	tm = shl13_00_mille_shot_time ;
	tm += ew->master_w->atck_level ;
	ew->timer0 = *tm ;
	ew->timer1 = 90 ;
	sp = shl13_00_mille_shot_spd ;
	sp += ew->sub_no ;
	ew->spd.x = sp->spd_x ;
	ew->acc.x = sp->acc_x ;
	ew->spd.y = sp->spd_y ;
	ew->acc.y = sp->acc_y ;
	ew->scale.x = 0.4f ;
	ew->scale.y = 0.4f ;
	ew->spd.x = ew->disp_dir ? ew->spd.x : -ew->spd.x ;
	ew->acc.x = ew->disp_dir ? -ew->acc.x : ew->acc.x ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 57 ) ;
}

/*==============================================================*/
static void ef_fantome_front_init( PLAYER_WORK *ew )
{
	Sint16 *tm ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->prio = 9 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	tm = shl13_00_time ;
	tm += ew->master_w->atck_level ;
	ew->timer0 = *tm ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 56 ) ;
}

/*==============================================================*/
static void ef_mille_front_init( PLAYER_WORK *ew )
{
	Sint16 *tm ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->prio = 9 ;
	ew->scale.x = 1.5f ;
	ew->scale.y = 1.5f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	tm = shl13_00_time ;
	tm += ew->master_w->atck_level ;
	ew->timer0 = *tm ;
	ew->char_no.b.grp = 23 ;
	pl_char_set( ew, ew->char_no.b.grp, 59 ) ;
}

static void ef_fantome_move( PLAYER_WORK *ew );
static void ef_fantome_shot_move( PLAYER_WORK *ew );
static void ef_sublimation_move( PLAYER_WORK *ew );
static void ef_sublimation_part_move( PLAYER_WORK *ew );
static void ef_throw_move( PLAYER_WORK *ew );
static void ef_power_move( PLAYER_WORK *ew );
static void ef_rafale_move( PLAYER_WORK *ew );
static void ef_rafale_link_move( PLAYER_WORK *ew );
static void ef_rafale_throw_move( PLAYER_WORK *ew );
static void ef_schwarz_move( PLAYER_WORK *ew );
static void ef_magie_lp_move( PLAYER_WORK *ew );
static void ef_magie_lk_move( PLAYER_WORK *ew );
static void ef_magie_move( PLAYER_WORK *ew );
static void ef_magie_obj_move( PLAYER_WORK *ew );
static void ef_paretonnre_move( PLAYER_WORK *ew );
static void ef_paretonnre_atck_move( PLAYER_WORK *ew );
static void ef_paretonnre_blast_move( PLAYER_WORK *ew );
static void ef_paretonnre_gun_move( PLAYER_WORK *ew );
static void ef_paretonnre_fire_move( PLAYER_WORK *ew );
static void ef_flammeile_move( PLAYER_WORK *ew );
static void ef_mille_move( PLAYER_WORK *ew );
static void ef_mille_shot_move( PLAYER_WORK *ew );
static void ef_fantome_front_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl13_00_move( PLAYER_WORK *ew )
{
	static void ( *shl13_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		ef_fantome_move, ef_fantome_shot_move, ef_sublimation_move, ef_sublimation_part_move, ef_throw_move, ef_power_move, ef_rafale_move, ef_rafale_link_move, ef_rafale_throw_move, ef_schwarz_move, ef_schwarz_move, ef_schwarz_move, ef_magie_lp_move,
		ef_magie_lk_move, ef_magie_move, ef_magie_obj_move, ef_paretonnre_move, ef_paretonnre_atck_move, ef_paretonnre_blast_move, ef_paretonnre_gun_move, ef_paretonnre_fire_move, ef_flammeile_move, ef_mille_move, ef_mille_shot_move, ef_fantome_front_move, ef_fantome_front_move,
	} ;

	shl13_00_move_jmp[ew->type]( ew ) ;
}

static void ef_fantome_start( PLAYER_WORK *ew );
static void ef_fantome_end( PLAYER_WORK *ew );
static void ef_fantome_vanish( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_fantome_move( PLAYER_WORK *ew )
{
	static void ( *ef_fantome_jmp[] )( PLAYER_WORK *ew ) = {
		ef_fantome_start, ef_fantome_end, ef_fantome_vanish,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	ef_fantome_jmp[ew->routine_1]( ew ) ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->mille_shot_flag ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->fantome_shot_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_fantome_start( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 ){
		ew->routine_1 = 2 ;
		ew->timer0 = 10 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_type ){
		ew->routine_1 ++ ;
		shl13_00_part_set2( ew, 1, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef_fantome_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 ){
		ew->routine_1 = 2 ;
		ew->timer0 = 10 ;
		return ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ef_fantome_vanish( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	ew->alpha -= 0.06f ;
	if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	if( ew->timer0 -- < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->fantome_shot_flag = 0 ;
	}
}

static void ef_fantome_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_fantome_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_fantome_shot_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_fantome_shot_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_fantome_shot_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void ef_fantome_shot_move( PLAYER_WORK *ew )
{
	static void ( *ef_fantome_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef_fantome_shot_fly, ef_fantome_shot_end, ef_fantome_shot_vanish, ef_fantome_shot_leave, ef_fantome_shot_hold,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	ef_fantome_shot_jmp[ew->routine_1]( ew, pl_w ) ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->mille_shot_flag ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->fantome_shot_flag = 0 ;
	}
}

/*==============================================================*/
/*			The shot flies; on a hit it holds the opponent		*/
/*==============================================================*/
static void ef_fantome_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float wave ;

	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	if( ew->hit_flag ){
		em = ew->em_w ;
		if( em->work_kind == W_PL && em->team_no == 0 && !( ew->hit_flag & HR_GUARD ) ){
			if( !ABYSS_CK( em ) ){
				ew->routine_1 = 4 ;
				ew->timer0 = 300 ;
				ew->timer1 = 90 ;
				ew->sub_no = em->pl_type ;
				ew->spd.x = 0.0f ;
				ew->spd.y = 0.0f ;
				ew->acc.x = 0.0f ;
				ew->acc.y = 0.0f ;
				pl_char_set( ew, Char_Effect, 29 ) ;
				ef_fantome_shot_hold( ew, pl_w ) ;
				return ;
			}
		}
		ew->routine_1 ++ ;
		ew->timer0 = 40 ;
		pl_char_set( ew, Char_Effect, 29 ) ;
		return ;
	}
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 1 ) ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	wave = nlSin( DEG_ANG( ew->timer1 ) ) * 5.0f ;
	ew->pos.y = ew->pos.y + wave ;
	if( !( ew->timer1 -= 6 ) ) ew->timer1 = 360 ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_fantome_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef_fantome_shot_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	pl_char_move( ew ) ;
	ew->scale.x -= 0.05f ;
	if( ew->scale.x <= 0.0f ) ew->scale.x = 0.0f ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->fantome_shot_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ef_fantome_shot_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	Float wave ;

	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	pl_char_move( ew ) ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->fantome_shot_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	wave = nlSin( DEG_ANG( ew->timer1 ) ) * 5.0f ;
	ew->pos.y = ew->pos.y + wave ;
	if( !( ew->timer1 -= 6 ) ) ew->timer1 = 360 ;
}

/*==============================================================*/
/*			Holding the opponent: it rides on them and			*/
/*			drains their gauge									*/
/*==============================================================*/
static void ef_fantome_shot_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em = pl_w->target_w ;
	PLAYER_WORK *em_w ;
	Float wave ;
	VEC3 pos ;		/* unused */

	ew->disp_flag = 1 ;
	ew->prio = 9 ;
	pl_char_move( ew ) ;
	if( ew->master_w->hit_box_no || !ew->timer0 -- ){
		ew->routine_1 = 1 ;
		ew->timer0 = 30 ;
		pl_char_set( ew, Char_Effect, 29 ) ;
		return ;
	}
	em_w = ew->em_w ;
	if( em_w->team_no || !em_w->vital_new ){
		ew->routine_1 = 1 ;
		ew->timer0 = 30 ;
		pl_char_set( ew, Char_Effect, 29 ) ;
		return ;
	}
	if( !pl_w->vital_new ){
		ew->routine_1 = 1 ;
		ew->timer0 = 30 ;
		pl_char_set( ew, Char_Effect, 29 ) ;
		return ;
	}
	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	if( !( ew->timer0 % 10 ) ) pl_gauge_add( em, -2 ) ;
	ew->disp_dir = em->disp_dir ;
	ew->pos.x = em->pos.x ;
	ew->pos.x += ew->disp_dir ? -DOT_X( 32.0f ) : DOT_X( 32.0f ) ;
	ew->pos.y = em->pos.y + DOT_Y( 32.0f ) ;
	ew->pos.z = em->pos.z ;
	wave = nlSin( DEG_ANG( ew->timer1 ) ) * DOT_Y( 20.0f ) ;
	ew->pos.y = ew->pos.y + wave ;
	if( !( ew->timer1 -= 3 ) ) ew->timer1 = 360 ;
}

static void ef_sublimation_rise( PLAYER_WORK *ew );
static void ef_sublimation_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_sublimation_move( PLAYER_WORK *ew )
{
	static void ( *ef_sublimation_jmp[] )( PLAYER_WORK *ew ) = {
		ef_sublimation_rise, ef_sublimation_keep,
	} ;

	ef_sublimation_jmp[ew->routine_1]( ew ) ;
	if( ew->pl_type != ew->master_w->pl_type ) shl13_00_free( ew ) ;
}

/*==============================================================*/
static void ef_sublimation_rise( PLAYER_WORK *ew )
{
	if( ew->cg_type == 1 ){
		pl_char_move( ew ) ;
		return ;
	}
	if( !( Sys->game_timer0 % 14 ) && ew->sub_no <= 3 ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl13_at_sublimation ;
		HIT_RESET( ew ) ;
	}
	pl_char_move( ew ) ;
	ew->scale.y += 0.2f ;
	if( ew->scale.y >= 1.0f ){
		ew->routine_1 ++ ;
		shl13_00_part_set2( ew, 3, 0 ) ;
	}
	ew->scale.x = 1.0f ;
	if( !( Sys->game_timer0 & 1 ) ) ew->scale.x = 0.8f ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_sublimation_keep( PLAYER_WORK *ew )
{
	if( !( Sys->game_timer0 % 14 ) && ew->sub_no <= 3 ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		ew->atck_no = pl13_at_sublimation ;
		HIT_RESET( ew ) ;
	}
	pl_char_move( ew ) ;
	ew->scale.y += 0.2f ;
	if( ew->scale.y >= 1.0f ) ew->prio = 11 ;
	if( ew->scale.y >= 2.0f ){
		ew->scale.y = 0.0f ;
		ew->prio = 10 ;
		ew->timer1 -- ;
		if( !ew->timer1 ) ew->routine_0 ++ ;
	}
	ew->scale.x = 1.0f ;
	if( !( Sys->game_timer0 & 1 ) ) ew->scale.x = 0.8f ;
	hit_work_set( ew ) ;
}

static void ef_sublimation_part_keep( PLAYER_WORK *ew );
static void ef_sublimation_part_fade( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_sublimation_part_move( PLAYER_WORK *ew )
{
	static void ( *ef_sublimation_part_jmp[] )( PLAYER_WORK *ew ) = {
		ef_sublimation_part_keep, ef_sublimation_part_fade,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;	/* unused */

	ef_sublimation_part_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_sublimation_part_keep( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->scale.y += 0.2f ;
	if( ew->scale.y >= 1.0f ) ew->prio = 11 ;
	if( ew->scale.y >= 2.0f ){
		ew->scale.y = 0.0f ;
		ew->prio = 10 ;
		ew->timer1 -- ;
		if( !ew->timer1 ) ew->routine_1 ++ ;
	}
	ew->scale.x = 1.0f ;
	if( !( Sys->game_timer0 & 1 ) ) ew->scale.x = 0.8f ;
}

/*--------------------------------------------------------------*/
static void ef_sublimation_part_fade( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	pl_char_move( ew ) ;
	if( ew->alpha ) ew->alpha -= 0.06f ;
	ew->scale.x -= 0.08f ;
	if( ew->scale.x <= 0.0f ) ew->scale.x = 0.02f ;
	ew->scale.y += 0.2f ;
	if( ew->scale.y >= 2.0f ){
		ew->routine_0 ++ ;
		ew->scale.y = 0.0f ;
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->sublimation_shot_flag = 0 ;
	}
	ew->scale.x = 1.0f ;
	if( !( Sys->game_timer0 & 1 ) ) ew->scale.x = 0.8f ;
}

static void ef_throw_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_throw_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_throw_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_throw_miss( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );

/*==============================================================*/
/*			Kind 4 is a throw: it catches the opponent and		*/
/*			carries them										*/
/*==============================================================*/
static void ef_throw_move( PLAYER_WORK *ew )
{
	static void ( *ef_throw_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw ) = {
		ef_throw_fly, ef_throw_fall, ef_throw_land, ef_throw_miss,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL13_00_LOCAL_W *lw = SHL13_00_LW( ew ) ;

	ef_throw_jmp[ew->routine_1]( ew, pl_w, lw ) ;
	if( ew->pl_type != ew->master_w->pl_type ) shl13_00_free( ew ) ;
}

static void ef_throw_catch_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_throw_catch_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static int ef_throw_catch_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static int ef_throw_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );

/*==============================================================*/
static void ef_throw_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->target_w ;
	Uint8 no ;

	if( ew->hit_flag < 0 && ew->hit_flag & HR_HIT ){
		ew->hit_flag |= -128 ;
		ef_throw_catch_ck( ew, pl_w, lw ) ;
		if( lw->throw_catch_flag ){
			ew->routine_1 ++ ;
			ef_throw_catch_set( ew, pl_w, lw ) ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
			ew->acc.y = -0.4017857f ;
			ew->disp_dir = pl_w->disp_dir ^ 1 ;
			no = shl13_00_tbl[em->pl_type] ;
			pl_char_set( ew, Char_Effect, no + 37 ) ;
			ef_throw_catch_end( ew, pl_w, lw ) ;
			return ;
		}
		ew->routine_1 = 3 ;
		return ;
	}
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_throw_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->catch_w ;
	Uint8 no ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( em->pos.y < pl_w->ground_y ){
		ew->routine_1 ++ ;
		ew->timer1 = 20 ;
		em->pos.y = pl_w->ground_y ;
		em->pl_stance = STANCE_STAND ;
		ew->disp_dir = pl_w->disp_dir ^ 1 ;
		no = shl13_00_tbl[em->pl_type] ;
		pl_char_set( ew, Char_Effect, no + 37 ) ;
		pl_char_move( ew ) ;
		return ;
	}
	ew->timer1 = 2 ;
	ef_throw_catch_hold( ew, pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ef_throw_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->catch_w ;

	ew->pos.y = pl_w->ground_y + ew->pos.y - em->pos.y ;
	ef_throw_catch_hold( ew, pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ef_throw_miss( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*==============================================================*/
static void ef_throw_catch_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	if( pl_sp_catch_ck( ew ) ){
		pl_voice_req( ew, 36 ) ;
		lw->throw_catch_flag = 1 ;
	}
}

/*==============================================================*/
static void ef_throw_catch_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = ew->em_w ;

	pl_w->catch_no = em->catch_no = 0xc2 ;
	pl_ef_catch_set( ew, em ) ;
	em->catch_type = 6 ;
}

/*==============================================================*/
/*			The throw lets go once the opponent is out of it	*/
/*==============================================================*/
static int ef_throw_catch_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->throw_catch_flag ){
		em = pl_w->target_w ;
		if( ew->catch_pair.w->catch_pair.w != ew ){
			pl_dir_set( em ) ;
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
	}
	return lw->throw_catch_flag ;
}

/*==============================================================*/
static int ef_throw_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->throw_catch_flag ){
		em = pl_w->target_w ;
		if( ew->catch_pair.w->catch_pair.w != ew ){
			pl_dir_set( em ) ;
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			return ;
		}
		catch_pos_set( ew, em ) ;
		if( em->hit_box_no ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			em->catch_type = 0 ;
			pl_dir_set( em ) ;
			return ;
		}
		if( ew->timer1 -- <= 0 ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			pl_dir_set( em ) ;
			if( em->vital_new ) pl_stand_ret( em ) ;
			return ;
		}
	}
	return lw->throw_catch_flag ;
}

/*==============================================================*/
/*			Kind 5 rides in front of Ruby Heart; a hit on a		*/
/*			player powers their attack up						*/
/*==============================================================*/
static void ef_power_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	pl_char_move( ew ) ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.x += ew->master_w->disp_dir ? 112 : -112 ;
	if( ew->hit_flag ){
		em = ew->em_w ;
		if( !em->team_no ){
			atck_plus_set( ew->em_w, 300, 40 ) ;
			puff_ef_set( ew->em_w, 0 ) ;
			pl_se_req( ew, 42 ) ;
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			Timer_Gauge_Set( ew->em_w, &em->atck_plus_timer, 0 ) ;
			return ;
		}
		ew->atck_no = pl13_at_power ;
		HIT_RESET( ew ) ;
		ew->pl_no ^= 1 ;
		ew->atck_flag = HIT_ON | HIT_TOUCH | HIT_PL_ONLY ;
		ew->dm_flag = HIT_PL_ONLY ;
	}
	hit_work_set( ew ) ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void ef_rafale_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_rafale_reach( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_rafale_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_rafale_lift( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_rafale_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );

/*==============================================================*/
/*			Kind 6 is the hand of Rafale Cannon (sp_rafale), a	*/
/*			throw kept on Ruby Heart's animation				*/
/*==============================================================*/
static void ef_rafale_move( PLAYER_WORK *ew )
{
	static void ( *ef_rafale_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw ) = {
		ef_rafale_start, ef_rafale_reach, ef_rafale_back, ef_rafale_lift, ef_rafale_end,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL13_00_LOCAL_W *lw = SHL13_00_LW( ew ) ;

	ef_rafale_jmp[ew->routine_1]( ew, pl_w, lw ) ;
	if( ew->pl_type != ew->master_w->pl_type ){
		shl13_00_free( ew ) ;
		return ;
	}
	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 6 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void ef_rafale_catch_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_rafale_catch_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static int ef_rafale_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );

/*==============================================================*/
static void ef_rafale_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 159 : -159 ;
	ew->pos.y += 165.0f ;
	pl_voice_req( ew, 35 ) ;
	ef_rafale_reach( ew, pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ef_rafale_reach( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->target_w ;
	Float *ofs = shl13_00_ofs ;
	Uint8 no ;

	if( ew->master_w->cg_type == 11 ){
		ew->master_w->cg_type = 0 ;
		ew->routine_1 ++ ;
		ef_rafale_back( ew, pl_w, lw ) ;
		return ;
	}
	if( ew->hit_flag < 0 ){
		ef_rafale_catch_ck( ew, pl_w, lw ) ;
		ef_rafale_catch_set( ew, pl_w, lw ) ;
		if( lw->rafale_catch_flag ){
			ew->master_w->sub_no = ew->sub_no + 1 ;
			ew->routine_1 ++ ;
			no = shl13_00_tbl[em->pl_type] ;
			pl_char_set( ew, Char_Effect, no + 37 ) ;
			ef_rafale_back( ew, pl_w, lw ) ;
			return ;
		}
		pl_voice_req( ew, 35 ) ;
	}
	if( ew->master_w->cg_type > 0 ){
		ew->pos.x = ew->master_w->pos.x ;
		ew->pos.y = ew->master_w->pos.y ;
		ew->pos.z = ew->master_w->pos.z ;
		ew->pos.x += ew->master_w->disp_dir ? 159 : -159 ;
		ew->pos.y += 165.0f ;
		ew->pos.x += ew->master_w->disp_dir ? ofs[ew->master_w->cg_type] : -ofs[ew->master_w->cg_type] ;
		ew->sub_no = ew->master_w->cg_type - 1 ;
		if( ew->master_w->cg_type > 0 ) shl13_00_part_set2( ew, 7, ew->sub_no ) ;
		ew->master_w->cg_type = 0 ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_rafale_back( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	Float *ofs = shl13_00_ofs ;

	if( ew->master_w->cg_type == 12 ){
		if( lw->rafale_catch_flag ){
			ew->routine_1 ++ ;
			ef_rafale_catch_hold( ew, pl_w, lw ) ;
			return ;
		}
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->master_w->cg_type > 0 ){
		ew->pos.x = ew->master_w->pos.x ;
		ew->pos.y = ew->master_w->pos.y ;
		ew->pos.z = ew->master_w->pos.z ;
		ew->pos.x += ew->master_w->disp_dir ? 148 : -148 ;
		ew->pos.y += 165.0f ;
		ew->pos.x += ew->master_w->disp_dir ? ofs[ew->master_w->cg_type] : -ofs[ew->master_w->cg_type] ;
	}
	ef_rafale_catch_hold( ew, pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ef_rafale_lift( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	if( ew->master_w->cg_end_flag < 0 ) ew->routine_1 ++ ;
	else {
		if( ew->master_w->cg_type == 1 ){
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			ew->pos.z = ew->master_w->pos.z ;
			ew->pos.x += ew->master_w->disp_dir ? 136 : -136 ;
			ew->pos.y += 165.0f ;
		}
		if( ew->master_w->cg_type == 2 ){
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			ew->pos.z = ew->master_w->pos.z ;
			ew->pos.x += ew->master_w->disp_dir ? 106 : -106 ;
			ew->pos.y += 146.0f ;
		}
		if( ew->master_w->cg_type == 3 ){
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			ew->pos.z = ew->master_w->pos.z ;
			ew->pos.x += ew->master_w->disp_dir ? 106 : -106 ;
			ew->pos.y += 151.0f ;
		}
		if( ew->master_w->cg_type == 4 ){
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			ew->pos.z = ew->master_w->pos.z ;
			ew->pos.x += ew->master_w->disp_dir ? 48 : -48 ;
			ew->pos.y += 202.0f ;
		}
	}
	ef_rafale_catch_hold( ew, pl_w, lw ) ;
}

/*--------------------------------------------------------------*/
static void ef_rafale_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	ef_rafale_catch_hold( ew, pl_w, lw ) ;
	if( ew->master_w->cg_type == 15 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_rafale_catch_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = ew->em_w ;

	if( !em->work_kind && em->routine_1 == PL_DAMAGE && em->routine_1 != PL_CAUGHT && !em->team_no && em->pl_stance != STANCE_DOWN
	 && em->vital_new ){
		if( ABYSS_CK( em ) ) return ;
		if( em->routine_1 == PL_DAMAGE ){
			pl_voice_req( ew, 36 ) ;
			lw->rafale_catch_flag = 1 ;
			return ;
		}
	}
	pl_w->timer0 = 1 ;
	pl_w->sub_no = ew->sub_no + 1 ;
	pl_w->cg_type = 0 ;
	ew->routine_1 = 2 ;
}

/*==============================================================*/
static void ef_rafale_catch_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->rafale_catch_flag ){
		em = ew->em_w ;
		pl_w->catch_no = em->catch_no = 0xc2 ;
		pl_ef_catch_set( ew, em ) ;
	}
}

/*==============================================================*/
static int ef_rafale_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->rafale_catch_flag ){
		em = pl_w->target_w ;
		pl_w->catch_flag = 1 ;
		catch_pos_set( ew, em ) ;
		if( ew->master_w->cg_type == 15 ){
			em->catch_type = 1 ;
			em->atck_no = pl13_at_rafale ;
			lw->rafale_catch_flag = 0 ;
			pl_se_req( em, 2 ) ;
		}
	}
	return lw->rafale_catch_flag ;
}

static void ef_rafale_link_start( PLAYER_WORK *ew );
static void ef_rafale_link_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_rafale_link_move( PLAYER_WORK *ew )
{
	static void ( *ef_rafale_link_jmp[] )( PLAYER_WORK *ew ) = {
		ef_rafale_link_start, ef_rafale_link_keep,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_rafale_link_jmp[ew->routine_1]( ew ) ;
	if( ew->master_w->hit_box_no || pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 6 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_rafale_link_start( PLAYER_WORK *ew )
{
	Float *ofs = shl13_00_ofs ;

	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 146 : -146 ;
	ew->pos.y += 165.0f ;
	ew->pos.x += ew->master_w->disp_dir ? ofs[ew->sub_no] : -ofs[ew->sub_no] ;
}

/*--------------------------------------------------------------*/
static void ef_rafale_link_keep( PLAYER_WORK *ew )
{
	Float *ofs = shl13_00_ofs ;

	if( ew->sub_no == ew->master_w->cg_type ){
		ew->master_w->cg_type = 0 ;
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	if( ew->master_w->routine_2 == 2 ){
		ew->pos.x += ew->master_w->disp_dir ? 135 : -135 ;
		ew->pos.y += 165.0f ;
		ew->pos.x += ew->master_w->disp_dir ? ofs[ew->sub_no] : -ofs[ew->sub_no] ;
	}
	else {
		ew->pos.x += ew->master_w->disp_dir ? 146 : -146 ;
		ew->pos.y += 165.0f ;
		ew->pos.x += ew->master_w->disp_dir ? ofs[ew->sub_no] : -ofs[ew->sub_no] ;
	}
}

static void ef_rafale_throw_end( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_rafale_throw_move( PLAYER_WORK *ew )
{
	static void ( *ef_rafale_throw_jmp[] )( PLAYER_WORK *ew ) = {
		ef_rafale_throw_end,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_rafale_throw_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_SPECIAL || pl_w->pl_sp_type != 6 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_rafale_throw_end( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void ef_schwarz_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_schwarz_move( PLAYER_WORK *ew )
{
	static void ( *ef_schwarz_jmp[] )( PLAYER_WORK *ew ) = {
		ef_schwarz_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;	/* unused */

	ef_schwarz_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_schwarz_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->master_w->routine_2 != 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
}

static void ef_magie_lp_fly( PLAYER_WORK *ew );
static void ef_magie_lp_arrive( PLAYER_WORK *ew );
static void ef_magie_lp_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_magie_lp_move( PLAYER_WORK *ew )
{
	static void ( *ef_magie_lp_jmp[] )( PLAYER_WORK *ew ) = {
		ef_magie_lp_fly, ef_magie_lp_arrive, ef_magie_lp_keep,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_magie_lp_jmp[ew->routine_1]( ew ) ;
	if( ew->master_w->routine_2 == 12 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 9 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_magie_lp_fly( PLAYER_WORK *ew )
{
	if( ew->cg_type == 2 ) pl_se_req( ew, 24 ) ;
	if( !ew->cg_type ){
		if( ew->master_w->routine_2 == 5 ) ew->pos.x += ew->master_w->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
		if( ew->master_w->routine_2 == 6 ){
			ew->routine_1 ++ ;
			pl_voice_req( ew, 38 ) ;
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			ew->pos.z = ew->master_w->pos.z ;
			ew->pos.x += ew->master_w->disp_dir ? 400.0f : -400.0f ;
			ew->pos.y += DOT_Y( 80.0f ) ;
			pl_char_set( ew, Char_Effect, 24 ) ;
			ef_magie_lp_arrive( ew ) ;
		}
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_magie_lp_arrive( PLAYER_WORK *ew )
{
	if( ew->cg_end_flag < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 26 ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_magie_lp_keep( PLAYER_WORK *ew )
{
	if( ew->master_w->routine_2 == 8 ) pl_char_move( ew ) ;
}

static void ef_magie_lk_fly( PLAYER_WORK *ew );
static void ef_magie_lk_arrive( PLAYER_WORK *ew );
static void ef_magie_lk_keep( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_magie_lk_move( PLAYER_WORK *ew )
{
	static void ( *ef_magie_lk_jmp[] )( PLAYER_WORK *ew ) = {
		ef_magie_lk_fly, ef_magie_lk_arrive, ef_magie_lk_keep,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_magie_lk_jmp[ew->routine_1]( ew ) ;
	if( ew->master_w->routine_2 == 12 || pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 9 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_magie_lk_fly( PLAYER_WORK *ew )
{
	if( ew->cg_type == 2 ) pl_se_req( ew, 24 ) ;
	if( !ew->cg_type ){
		if( ew->master_w->routine_2 == 5 ) ew->pos.x += ew->master_w->disp_dir ? DOT_X( 16.0f ) : -DOT_X( 16.0f ) ;
		if( ew->master_w->routine_2 == 6 ){
			ew->routine_1 ++ ;
			pl_voice_req( ew, 38 ) ;
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			ew->pos.z = ew->master_w->pos.z ;
			ew->pos.x += ew->master_w->disp_dir ? 400.0f : -400.0f ;
			ew->pos.y += DOT_Y( 80.0f ) ;
			pl_char_set( ew, Char_Effect, 25 ) ;
			ef_magie_lk_arrive( ew ) ;
		}
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_magie_lk_arrive( PLAYER_WORK *ew )
{
	if( ew->cg_end_flag < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 27 ) ;
		return ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_magie_lk_keep( PLAYER_WORK *ew )
{
	if( ew->master_w->routine_2 == 8 ) pl_char_move( ew ) ;
}

static void ef_magie_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_magie_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_magie_place( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_magie_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_magie_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_magie_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );

/*==============================================================*/
/*			Kind 14 is the anchor of a hyper combo throw: it	*/
/*			drops, catches and is pulled up						*/
/*==============================================================*/
static void ef_magie_move( PLAYER_WORK *ew )
{
	static void ( *ef_magie_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw ) = {
		ef_magie_fall, ef_magie_land, ef_magie_place, ef_magie_keep, ef_magie_burst, ef_magie_end,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL13_00_LOCAL_W *lw = SHL13_00_LW( ew ) ;

	ef_magie_jmp[ew->routine_1]( ew, pl_w, lw ) ;
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 9 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void ef_magie_catch_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static void ef_magie_catch_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );
static int ef_magie_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw );

/*==============================================================*/
static void ef_magie_fall( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = ew->em_w ;

	if( ew->pos.y <= ew->master_w->pos.y + DOT_Y( 48.0f ) ){
		ew->master_w->routine_2 = 11 ;
		ew->routine_1 = 4 ;
		ew->timer0 = 20 ;
		return ;
	}
	if( ew->hit_flag < 0 ){
		ef_magie_catch_ck( ew, pl_w, lw ) ;
		ef_magie_catch_set( ew, pl_w, lw ) ;
		if( lw->magie_catch_flag ){
			ew->routine_1 ++ ;
			em->disp_flag = 0 ;
			ew->timer0 = 0 ;
			ew->sub_no = 0 ;
			pl_char_set( ew, Char_Effect, 47 ) ;
			return ;
		}
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_magie_land( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	ef_magie_catch_hold( ew, pl_w, lw ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->sub_no ){
		if( ew->pos.y <= ew->master_w->pos.y + DOT_Y( 48.0f ) ){
			ew->sub_no = 1 ;
			ew->spd.x = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->timer0 = 0 ;
			ew->spd.y = ew->acc.y / 2.0f ;
			ew->acc.y = -( ew->acc.y / 8.0f ) ;
			return ;
		}
	}
	if( ew->timer0 < 10 ){
		ew->scale.x += 0.128f ;
		ew->scale.y -= 0.012f ;
		ew->timer0 ++ ;
		return ;
	}
	if( ew->timer0 < 20 ){
		ew->scale.x -= 0.128f ;
		ew->scale.y += 0.012f ;
		ew->timer0 ++ ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->master_w->routine_2 = 3 ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->timer0 = 0 ;
	flash_dark_set() ;
}

/*--------------------------------------------------------------*/
static void ef_magie_place( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = pl_w->target_w ;

	ef_magie_catch_hold( ew, pl_w, lw ) ;
	ew->routine_1 ++ ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.x += ew->master_w->disp_dir ? 400.0f : -400.0f ;
	ew->pos.y += DOT_Y( 80.0f ) ;
	em->pos.x = ew->pos.x ;
	ew->spd.x = ew->pos.x ;
	ew->spd.y = ew->pos.y ;
}

/*--------------------------------------------------------------*/
static void ef_magie_keep( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	ef_magie_catch_hold( ew, pl_w, lw ) ;
	if( ew->master_w->sub_no ){
		ew->pos.y = ew->spd.y ;
		if( !( Sys->game_timer0 & 1 ) ) ew->pos.y -= 5.0f ;
	}
	if( !ew->master_w->sub_no ) ew->pos.y = ew->spd.y ;
	if( ew->master_w->routine_2 == 8 ) pl_char_move( ew ) ;
	if( ew->master_w->routine_2 == 12 ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = -DOT_Y( 0.375f ) ;
		pl_char_set( ew, Char_Effect, 48 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef_magie_burst( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	Float *pos = shl13_00_pos ;
	VEC3 v ;
	Uint8 no ;

	ef_magie_catch_hold( ew, pl_w, lw ) ;
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->timer0 = 8 ;
		return ;
	}
	if( !( ew->timer0 % 6 ) ){
		v.x = ew->pos.x ;
		v.y = ew->pos.y + -DOT_Y( 48.0f ) ;
		no = Rnd() & 15 ;
		v.x += pos[no] ;
		no = Rnd() & 15 ;
		v.y += pos[no + 16] ;
		pl_se_req( ew, 73 ) ;
		burst_set( ew, &v, 1, 0xc9 ) ;
	}
	if( ew->pos.y <= ew->master_w->pos.y + DOT_Y( 48.0f ) ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void ef_magie_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	ef_magie_catch_hold( ew, pl_w, lw ) ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_magie_catch_ck( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em = ew->em_w ;

	if( !em->work_kind && em->routine_1 == PL_DAMAGE && em->routine_1 != PL_CAUGHT && !em->team_no && em->pl_stance != STANCE_DOWN
	 && em->vital_new ){
		if( ABYSS_CK( em ) ) return ;
		if( em->routine_1 == PL_DAMAGE ){
			if( em->pl_stance < STANCE_AIR ){
				em->pos.x = ew->pos.x ;
				lw->magie_catch_flag = 1 ;
			}
		}
	}
}

/*==============================================================*/
static void ef_magie_catch_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->magie_catch_flag ){
		em = ew->em_w ;
		pl_w->catch_no = em->catch_no = 0xc2 ;
		pl_ef_catch_set( ew, em ) ;
	}
}

/*==============================================================*/
static int ef_magie_catch_hold( PLAYER_WORK *ew, PLAYER_WORK *pl_w, SHL13_00_LOCAL_W *lw )
{
	PLAYER_WORK *em ;

	if( lw->magie_catch_flag ){
		em = pl_w->target_w ;
		pl_w->catch_flag = 1 ;
		if( ew->routine_1 == 5 ){
			em->pos.x = ew->pos.x ;
			em->pos.y = ew->pos.y ;
			em->disp_flag = 1 ;
			em->catch_type = 15 ;
			em->atck_no = pl13_at_magie_catch ;
			lw->magie_catch_flag = 0 ;
		}
	}
	return lw->magie_catch_flag ;
}

static void ef_magie_obj_keep( PLAYER_WORK *ew );
static void ef_magie_obj_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void ef_magie_obj_move( PLAYER_WORK *ew )
{
	static void ( *ef_magie_obj_jmp[] )( PLAYER_WORK *ew ) = {
		ef_magie_obj_keep, ef_magie_obj_fly,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_magie_obj_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 9 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_magie_obj_keep( PLAYER_WORK *ew )
{
	if( ew->master_w->routine_2 == 8 ) pl_char_move( ew ) ;
	if( ew->master_w->routine_2 == 12 ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->master_w->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 3.0f ) ;
		ew->acc.y = 2.0089285f ;
		ew->timer0 = 30 ;
		ew->pos.y = DOT_Y( 24.0f ) ;
		pl_char_set( ew, Char_Effect, 46 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef_magie_obj_fly( PLAYER_WORK *ew )
{
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->rot[2] += 0x2000 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
}

static void ef_paretonnre_start( PLAYER_WORK *ew );
static void ef_paretonnre_rush( PLAYER_WORK *ew );
static void ef_paretonnre_back( PLAYER_WORK *ew );
static void ef_paretonnre_wait( PLAYER_WORK *ew );
static void ef_paretonnre_keep( PLAYER_WORK *ew );
static void ef_paretonnre_leave( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 16 comes in from the screen's edge, charges	*/
/*			and goes back										*/
/*==============================================================*/
static void ef_paretonnre_move( PLAYER_WORK *ew )
{
	static void ( *ef_paretonnre_jmp[] )( PLAYER_WORK *ew ) = {
		ef_paretonnre_start, ef_paretonnre_rush, ef_paretonnre_back, ef_paretonnre_wait, ef_paretonnre_keep, ef_paretonnre_leave,
	} ;

	ef_paretonnre_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_paretonnre_start( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 5 ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
		lw->shot_on = 0 ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->spd.x = ew->disp_dir ? 25.0f : -25.0f ;
	ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 24 ;
	lw->paretonnre_wait_cnt = 0 ;
	ef_paretonnre_rush( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_rush( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 5 ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
		pl_w->sub_no = 0 ;
		lw->shot_on = 0 ;
		return ;
	}
	if( pl_w->sub_no ){
		lw->paretonnre_wait_cnt ++ ;
		return ;
	}
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? -20.0f : 20.0f ;
		ew->acc.x = ew->disp_dir ? 0.3125f : -0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 2 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_back( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 5 ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->shot_on = 0 ;
		return ;
	}
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->timer0 = 24 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	if( !pl_w->timer1 ){
		ew->routine_1 = 5 ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
		return ;
	}
	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 5 ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
		lw->shot_on = 0 ;
		return ;
	}
	if( ew->timer0 -- ) return ;
	ew->routine_1 ++ ;
	ew->timer0 = 120 ;
	ew->timer0 += lw->paretonnre_wait_cnt * 2 ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_keep( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;
	VEC3 unk_pos ;	/* unused */

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 5 ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->shot_on = 0 ;
		return ;
	}
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? -DOT_X( 9.5f ) : DOT_X( 9.5f ) ;
		ew->acc.x = ew->disp_dir ? -0.3125f : 0.3125f ;
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_leave( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	if( ew->timer0 == 40 ){
		lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
		lw->shot_on = 0 ;
	}
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
}

static void ef_paretonnre_atck_hit( PLAYER_WORK *ew );
static void ef_paretonnre_atck_end( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 17 rides on kind 16 and is its attack			*/
/*==============================================================*/
static void ef_paretonnre_atck_move( PLAYER_WORK *ew )
{
	static void ( *ef_paretonnre_atck_jmp[] )( PLAYER_WORK *ew ) = {
		ef_paretonnre_atck_hit, ef_paretonnre_atck_end,
	} ;

	ef_paretonnre_atck_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_paretonnre_atck_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 1 ;
		pl_w->sub_no = 0 ;
	}
	if( ew->ef_w->routine_1 == 3 ){
		ew->routine_1 ++ ;
		pl_w->sub_no = 0 ;
	}
	if( ew->timer1 ){
		pl_w->sub_no = 1 ;
		ew->timer1 -- ;
		return ;
	}
	pl_w->sub_no = 0 ;
	if( ew->ef_w->routine_1 == 1 ) pl_char_move( ew ) ;
	if( ew->timer0 ) ew->timer0 -- ;
	else {
		if( ew->hit_flag ){
			ew->timer0 = 3 ;
			ew->timer1 = 6 ;
			em = ew->em_w ;
			if( !pl_w->timer1 ){
				if( ew->hit_flag & HR_GUARD ) ew->spd.x = 0.0f ;
				else if( !em->team_no ) ew->spd.x = 1.0f ;
			}
			pl_w->timer1 = ew->spd.x ;
			if( ew->sub_type < 6 ){
				if( ew->sub_type == 5 || ew->cg_type == 2 ){
					ew->atck_no = pl13_at_paretonnre_last ;
					ew->hit_flag = ew->atck_attr = 0 ;
					ew->unk_1c4 = 0 ;
					vs_sys_w->atck_cnt[ew->pl_no] ++ ;
					scr_work.quake_level = 2 ;
					scr_work.quake_flag = 1 ;
					ew->sub_type = 6 ;
				}
				else {
					ew->atck_no = pl13_at_paretonnre ;
					ew->hit_flag = ew->atck_attr = 0 ;
					ew->unk_1c4 = 0 ;
					vs_sys_w->atck_cnt[ew->pl_no] ++ ;
					ew->sub_type ++ ;
				}
			}
		}
		hit_work_set( ew ) ;
	}
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_atck_end( PLAYER_WORK *ew )
{
	if( ew->ef_w->routine_0 == 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
}

static void ef_paretonnre_blast_hit( PLAYER_WORK *ew );
static void ef_paretonnre_blast_end( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 18 is a blast on the opponent					*/
/*==============================================================*/
static void ef_paretonnre_blast_move( PLAYER_WORK *ew )
{
	static void ( *ef_paretonnre_blast_jmp[] )( PLAYER_WORK *ew ) = {
		ef_paretonnre_blast_hit, ef_paretonnre_blast_end,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;	/* unused */

	ef_paretonnre_blast_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_paretonnre_blast_hit( PLAYER_WORK *ew )
{
	if( ew->hit_flag ){
		ew->routine_1 ++ ;
		ew->timer0 = 2 ;
		pl_se_req( ew, 73 ) ;
		ew->pos.y += DOT_Y( 48.0f ) ;
		burst_set( ew, &ew->pos, 1, 0xc9 ) ;
		return ;
	}
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->timer0 = 2 ;
		pl_se_req( ew, 73 ) ;
		ew->pos.y += DOT_Y( 32.0f ) ;
		burst_set( ew, &ew->pos, 1, 0xc9 ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_blast_end( PLAYER_WORK *ew )
{
	if( !ew->timer0 -- ) ew->routine_0 ++ ;
}

static void ef_paretonnre_gun_wait( PLAYER_WORK *ew );
static void ef_paretonnre_gun_shot( PLAYER_WORK *ew );
static void ef_paretonnre_gun_leave( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 19 rides on kind 16							*/
/*==============================================================*/
static void ef_paretonnre_gun_move( PLAYER_WORK *ew )
{
	static void ( *ef_paretonnre_gun_jmp[] )( PLAYER_WORK *ew ) = {
		ef_paretonnre_gun_wait, ef_paretonnre_gun_shot, ef_paretonnre_gun_leave,
	} ;

	ef_paretonnre_gun_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_paretonnre_gun_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float ofs ;

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 = 2 ;
		return ;
	}
	if( ew->ef_w->routine_1 >= 5 ){
		ew->routine_1 = 2 ;
		return ;
	}
	if( ew->ef_w->routine_1 == 4 ) ew->routine_1 ++ ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	if( ew->sub_no ){
		ofs = 112.0f ;
		ew->pos.x += ew->disp_dir ? ofs : -ofs ;
	}
	else {
		ofs = -18.666666f ;
		ew->pos.x += ew->disp_dir ? ofs : -ofs ;
	}
	ew->pos.y -= 95.99999f ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_gun_shot( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !( pl_w->act_st == N_SUPER && pl_w->pl_sp_type == 5 ) ){
		ew->routine_1 ++ ;
		return ;
	}
	if( ew->ef_w->routine_1 >= 5 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 3 ) ;
		return ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		if( ew->sub_no ){
			pl_voice_req( ew, 32 ) ;
			shl13_00_part_set2( ew, 18, 0 ) ;
		}
		else {
			pl_voice_req( ew, 32 ) ;
			shl13_00_part_set2( ew, 18, 1 ) ;
		}
		shl13_00_part_set( ew, 20 ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_paretonnre_gun_leave( PLAYER_WORK *ew )
{
	Float ofs ;

	if( ew->ef_w->routine_0 == 2 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	if( ew->sub_no ){
		ofs = 112.0f ;
		ew->pos.x += ew->disp_dir ? ofs : -ofs ;
	}
	else {
		ofs = -18.666666f ;
		ew->pos.x += ew->disp_dir ? ofs : -ofs ;
	}
	ew->pos.y -= 95.99999f ;
}

static void ef_paretonnre_fire_end( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 20 plays its animation once					*/
/*==============================================================*/
static void ef_paretonnre_fire_move( PLAYER_WORK *ew )
{
	static void ( *ef_paretonnre_fire_jmp[] )( PLAYER_WORK *ew ) = {
		ef_paretonnre_fire_end,
	} ;

	ef_paretonnre_fire_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_paretonnre_fire_end( PLAYER_WORK *ew )
{
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void ef_flammeile_fade( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 21 fades out on the player during the move		*/
/*==============================================================*/
static void ef_flammeile_move( PLAYER_WORK *ew )
{
	static void ( *ef_flammeile_jmp[] )( PLAYER_WORK *ew ) = {
		ef_flammeile_fade,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ef_flammeile_jmp[ew->routine_1]( ew ) ;
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != 3 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_flammeile_fade( PLAYER_WORK *ew )
{
	if( ew->master_w->routine_2 != 2 ){
		ew->routine_0 ++ ;
		return ;
	}
	if( ew->alpha ){
		ew->alpha -= 0.06f ;
		if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	}
	if( ew->cg_end_flag < 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	pl_char_move( ew ) ;
}

static void ef_mille_start( PLAYER_WORK *ew );
static void ef_mille_end( PLAYER_WORK *ew );
static void ef_mille_vanish( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kind 22 is in front of the player; its frames		*/
/*			send out the shots									*/
/*==============================================================*/
static void ef_mille_move( PLAYER_WORK *ew )
{
	static void ( *ef_mille_jmp[] )( PLAYER_WORK *ew ) = {
		ef_mille_start, ef_mille_end, ef_mille_vanish,
	} ;

	ef_mille_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void ef_mille_start( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->routine_1 ){
		ew->routine_1 = 2 ;
		ew->timer0 = 10 ;
		return ;
	}
	if( ew->cg_type == 1 ){
		ew->cg_type = 0 ;
		shl13_00_part_set2( ew, 23, 0 ) ;
	}
	if( ew->cg_type == 2 ){
		ew->cg_type = 0 ;
		shl13_00_part_set2( ew, 23, 2 ) ;
	}
	if( ew->cg_type == 3 ){
		ew->cg_type = 0 ;
		shl13_00_part_set2( ew, 23, 1 ) ;
	}
	if( ew->cg_type == 4 ){
		ew->cg_type = 0 ;
		shl13_00_part_set2( ew, 23, 0 ) ;
	}
	if( ew->cg_type == 5 ){
		ew->cg_type = 0 ;
		shl13_00_part_set2( ew, 23, 2 ) ;
	}
	if( ew->cg_type == 6 ){
		ew->cg_type = 0 ;
		shl13_00_part_set2( ew, 23, 1 ) ;
		ew->routine_1 ++ ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_mille_end( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	pl_char_move( ew ) ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( pl_w->routine_1 ){
		ew->routine_1 = 2 ;
		ew->timer0 = 10 ;
		return ;
	}
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->mille_shot_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ef_mille_vanish( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw = ( PL13_LOCAL_W * )pl_w->tlocal ;

	ew->alpha -= 0.06f ;
	if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	if( ew->timer0 -- < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->mille_shot_flag = 0 ;
	}
}

static void ef_mille_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_mille_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_mille_shot_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void ef_mille_shot_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			Kind 23 is a shot that hits three times				*/
/*==============================================================*/
static void ef_mille_shot_move( PLAYER_WORK *ew )
{
	static void ( *ef_mille_shot_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		ef_mille_shot_fly, ef_mille_shot_end, ef_mille_shot_vanish, ef_mille_shot_leave,
	} ;

	ef_mille_shot_jmp[ew->routine_1]( ew, ew->master_w ) ;
}

/*==============================================================*/
static void ef_mille_shot_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *em ;
	Float wave ;

	if( ew->hit_flag ){
		if( ew->sub_type >= 3 ){
			em = ew->em_w ;
			if( em->work_kind == W_PL ){
				if( !em->team_no ){
					if( !( ew->hit_flag & HR_GUARD ) ){
						ew->routine_1 = 3 ;
						ew->timer0 = 60 ;
						pl_char_set( ew, Char_Effect, 29 ) ;
						return ;
					}
				}
				else {
					ew->routine_1 = 3 ;
					ew->timer0 = 40 ;
					pl_char_set( ew, Char_Effect, 29 ) ;
					return ;
				}
			}
			else {
				ew->routine_1 ++ ;
				ew->timer0 = 40 ;
				pl_char_set( ew, Char_Effect, 29 ) ;
				return ;
			}
		}
		if( ew->routine_3 == 6 ){
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			ew->dm_flag = HIT_ON | HIT_SHOT ;
			ew->atck_no = pl13_at_mille_2 ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
			ew->sub_type ++ ;
		}
		if( ew->routine_3 == 12 ){
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			ew->dm_flag = HIT_ON | HIT_SHOT ;
			ew->atck_no = pl13_at_mille ;
			ew->hit_flag = ew->atck_attr = 0 ;
			ew->unk_1c4 = 0 ;
			vs_sys_w->atck_cnt[ew->pl_no] ++ ;
			ew->sub_type ++ ;
		}
		ew->routine_3 ++ ;
	}
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 1 ) ;
	}
	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	wave = nlSin( DEG_ANG( ew->timer1 ) ) * 5.0f ;
	ew->pos.y = ew->pos.y + wave ;
	if( !( ew->timer1 -= 6 ) ) ew->timer1 = 360 ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void ef_mille_shot_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, Char_Effect, 30 ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef_mille_shot_vanish( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	pl_char_move( ew ) ;
	ew->scale.x -= 0.05f ;
	if( ew->scale.x <= 0.0f ) ew->scale.x = 0.0f ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ef_mille_shot_leave( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float wave ;

	if( ew->scale.x > 1.0f ){
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
	}
	else {
		ew->scale.x += 0.04f ;
		ew->scale.y += 0.04f ;
	}
	pl_char_move( ew ) ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	wave = nlSin( DEG_ANG( ew->timer1 ) ) * 5.0f ;
	ew->pos.y = ew->pos.y + wave ;
	if( !( ew->timer1 -= 6 ) ) ew->timer1 = 360 ;
}

static void ef_fantome_front_fade( PLAYER_WORK *ew );
static void ef_fantome_front_1( PLAYER_WORK *ew );

/*==============================================================*/
/*			Kinds 24 and 25 play their animation out; they		*/
/*			end if kind 22 is out or she is knocked out of it	*/
/*==============================================================*/
static void ef_fantome_front_move( PLAYER_WORK *ew )
{
	static void ( *ef_fantome_front_jmp[] )( PLAYER_WORK *ew ) = {
		ef_fantome_front_fade, ef_fantome_front_1,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;
	PL13_LOCAL_W *lw ;

	ef_fantome_front_jmp[ew->routine_1]( ew ) ;
	lw = ( PL13_LOCAL_W * )pl_w->tlocal ;
	if( lw->mille_shot_flag || pl_w->routine_1 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		lw->fantome_shot_flag = 0 ;
	}
}

/*==============================================================*/
static void ef_fantome_front_fade( PLAYER_WORK *ew )
{
	if( ew->cg_type == 1 ){
		ew->alpha -= 0.06f ;
		if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	}
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void ef_fantome_front_1( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->cg_type == 1 ){
		ew->alpha -= 0.06f ;
		if( ew->alpha < 0.0f ) ew->alpha = 0.0f ;
	}
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl13_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl13_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl13_00.c" End of File	*/
