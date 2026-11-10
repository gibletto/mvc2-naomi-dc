/* SonSon's catching shot (character 14's shot 02, for the POW hyper combo): it grows as it
   flies, takes in the opponent it meets, bounces with them, holds them and throws them out. */
#include "player.h"
#include "pl_char_old.h"
#include "pl_color.h"
#include "pl14_def.h"

/* the effect's own work */
typedef struct SHL14_02_LOCAL_W {
	Uint16	char_no;	/* 000: SonSon's animation when it was set */
	Uint8	unk_02[0x04 - 0x02];
	Float	ground_y;	/* 004: her floor, where it lands */
} SHL14_02_LOCAL_W;
#define SHL14_02_LW( w )	( ( SHL14_02_LOCAL_W * )&( w )->obj_flag )

/* moves the work by its speed, and its speed by its acceleration */
#define SHL14_02_MOVE( w )	{ ( w )->pos.x += ( w )->spd.x ; ( w )->spd.x += ( w )->acc.x ; \
				  ( w )->pos.y += ( w )->spd.y ; ( w )->spd.y += ( w )->acc.y ; }

static void shl14_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl14_02_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_02_main ;
		ew->type = no ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1402 ;
		SHL14_02_LW( ew )->char_no = pl_w->char_no.w ;
		SHL14_02_LW( ew )->ground_y = pl_w->ground_y ;
	}
	return ew ;
}

static void shl14_02_init( PLAYER_WORK *ew );
static void shl14_02_move( PLAYER_WORK *ew );
static void shl14_02_end( PLAYER_WORK *ew );
static void shl14_02_free( PLAYER_WORK *ew );
static void shl14_02_master_ck( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_02_main( PLAYER_WORK *ew )
{
	static void ( *shl14_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_02_init, shl14_02_move, shl14_02_end, shl14_02_free,
	} ;

	if( ew->routine_0 < 2 ) shl14_02_master_ck( ew ) ;
	shl14_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl14_02_init( PLAYER_WORK *ew )
{
	SHL14_02_LOCAL_W *lw = SHL14_02_LW( ew ) ;
	PLAYER_WORK *pl_w ;
	Float x, spd, acc ;

	ew->routine_0 ++ ;
	ew->timer0 = 10 ;
	pl_w = ew->master_w ;
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
	ew->size_top = ew->size_bottom = 80 ;
	ew->size_front = ew->size_back = 80 ;
	ew->prio = 7 ;
	if( lw->char_no != pl_w->char_no.w ) ew->routine_0 = 2 ;
	else {
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		if( ew->type >= 15 ){
			ew->scale.x = 0.04f ;
			ew->scale.y = 0.04f ;
			ew->disp_dir = 0 ;
			x = -20.0f ;
			spd = -5.0f ;
			acc = 0.0f ;
			if( pl_w->disp_dir ){
				x = -x ;
				spd = -spd ;
				acc = -acc ;
			}
			ew->pos.x += x ;
			ew->pos.y += DOT_Y( 64.0f ) ;
			ew->spd.x = spd ;
			ew->acc.x = acc ;
			ew->spd.y = 0.0f ;
			ew->acc.y = -0.0f ;
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			ew->dm_flag = HIT_ON | HIT_SHOT ;
			ew->atck_no = pl14_at_pow ;
			HIT_RESET( ew ) ;
			pl_char_set( ew, 22, 14 ) ;
			pl_voice_req( ew, 25 ) ;
		}
		else ew->routine_1 = ew->type ;
	}
}

static void shl14_02_shot( PLAYER_WORK *ew );
static void shl14_02_bound( PLAYER_WORK *ew );
static void shl14_02_hold( PLAYER_WORK *ew );
static void shl14_02_wait( PLAYER_WORK *ew );
static void shl14_02_throw( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_02_move( PLAYER_WORK *ew )
{
	static void ( *shl14_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_02_shot, shl14_02_bound, shl14_02_hold, shl14_02_wait, shl14_02_throw,
	} ;

	shl14_02_move_jmp[ew->routine_1]( ew ) ;
}

static void shl14_02_grow( PLAYER_WORK *ew );
static void shl14_02_fly( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_02_shot( PLAYER_WORK *ew )
{
	static void ( *shl14_02_shot_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_02_grow, shl14_02_fly,
	} ;

	shl14_02_shot_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
/*			It starts small and grows							*/
/*==============================================================*/
static void shl14_02_grow( PLAYER_WORK *ew )
{
	if( --ew->timer0 <= 0 ) ew->routine_2 ++ ;
	ew->scale.x += 0.008f ;
	ew->scale.y += 0.008f ;
	SHL14_02_MOVE( ew ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

static void shl14_02_catch( PLAYER_WORK *ew );

/*==============================================================*/
/*			It flies on, growing to full size, until it			*/
/*			leaves the screen or meets the opponent				*/
/*==============================================================*/
static void shl14_02_fly( PLAYER_WORK *ew )
{
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 2 ;
		return ;
	}
	if( ew->hit_flag ){
		if( ew->em_w->work_kind ){
			ew->hit_flag = 0 ;
			ew->atck_no = pl14_at_pow ;
			HIT_RESET( ew ) ;
			return ;
		}
		if( pl_sp_catch_ck( ew ) ){
			shl14_02_catch( ew ) ;
			return ;
		}
	}
	ew->scale.x += 0.022f ;
	ew->scale.y += 0.022f ;
	if( ew->scale.x >= 1.0f ) ew->scale.x = 1.0f ;
	if( ew->scale.y >= 1.0f ) ew->scale.y = 1.0f ;
	SHL14_02_MOVE( ew ) ;
	pl_char_move( ew ) ;
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			With the opponent inside, it jumps up, kept away	*/
/*			from the screen's edges								*/
/*==============================================================*/
static void shl14_02_bound_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	ew->routine_1 ++ ;
	ew->routine_2 = 0 ;
	ew->scale.x = ew->scale.y = 1.0f ;
	pl_char_set( ew, 22, ew->type ) ;
	pl_w = ew->master_w ;
	ew->disp_dir = pl_w->disp_dir ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 3.0f ) ;
	ew->acc.y = -0.46875f ;
	ef_color_set( ew, 2, 2, 2 ) ;
	dust_set( ew, 0, 0x90, 4.0f, 4.0f ) ;
	dust_set( ew, 16, 0x90, 4.0f, 4.0f ) ;
	dust_set( ew, 0xf0, 0x90, 4.0f, 4.0f ) ;
	if( pl_w->disp_dir ){
		if( ew->pos.x > scr_work.scr_right - 160.0f ) ew->pos.x = scr_work.scr_right - 160.0f ;
	}
	else {
		if( ew->pos.x < scr_work.scr_left + 160.0f ) ew->pos.x = scr_work.scr_left + 160.0f ;
	}
}

/*==============================================================*/
/* takes in the opponent; its new pos.y is read through pl_w before pl_w is set */
static void shl14_02_catch( PLAYER_WORK *ew )
{
	PLAYER_WORK *em_w = ew->em_w ;
	PLAYER_WORK *pl_w ;

	em_w->disp_flag = 0 ;
	em_w->cg_shadow = 0xff ;
	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->pos.y ;
	em_w->pos.y = pl_w->ground_y ;
	pl_w = ew->master_w ;
	pl_w->catch_no = 0xc3 ;
	pl_ef_catch_set( ew, em_w ) ;
	shl14_02_bound_set( ew ) ;
	pl_voice_req( pl_w, 10 ) ;
	scr_excep_set( pl_w, 1, 7 ) ;
	ew->be_flag = 1 ;
	pl_shadow_set( ew ) ;
}

static void shl14_02_fall( PLAYER_WORK *ew );
static void shl14_02_squash( PLAYER_WORK *ew );
static void shl14_02_stretch( PLAYER_WORK *ew );
static void shl14_02_fall2( PLAYER_WORK *ew );
static void shl14_02_squash2( PLAYER_WORK *ew );
static void shl14_02_stretch2( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_02_bound( PLAYER_WORK *ew )
{
	static void ( *shl14_02_bound_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_02_fall, shl14_02_squash, shl14_02_stretch,
		shl14_02_fall2, shl14_02_squash2, shl14_02_stretch2,
	} ;

	PLAYER_WORK *pl_w = ew->master_w ;

	pl_w->catch_flag = 1 ;
	shl14_02_bound_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
/*			Falls to the floor and bounces up again				*/
/*==============================================================*/
static void shl14_02_fall( PLAYER_WORK *ew )
{
	SHL14_02_MOVE( ew ) ;
	if( SHL14_02_LW( ew )->ground_y >= ew->pos.y ){
		ew->pos.y = SHL14_02_LW( ew )->ground_y ;
		ew->routine_2 ++ ;
		ew->timer0 = 5 ;
		ew->spd.x = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->spd.y = DOT_Y( 1.5f ) ;
		ew->acc.y = -0.234375f ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_02_squash( PLAYER_WORK *ew )
{
	ew->scale.x += 0.012f ;
	ew->scale.y -= 0.018f ;
	if( --ew->timer0 <= 0 ){
		ew->routine_2 ++ ;
		ew->timer0 = 5 ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_02_stretch( PLAYER_WORK *ew )
{
	ew->scale.x += 0.016f ;
	ew->scale.y -= 0.024f ;
	if( --ew->timer0 <= 0 ){
		ew->routine_2 ++ ;
		ew->scale.x = ew->scale.y = 1.0f ;
	}
}

/*==============================================================*/
/*			Falls again and stops on the floor					*/
/*==============================================================*/
static void shl14_02_fall2( PLAYER_WORK *ew )
{
	SHL14_02_MOVE( ew ) ;
	if( SHL14_02_LW( ew )->ground_y >= ew->pos.y ){
		ew->pos.y = SHL14_02_LW( ew )->ground_y ;
		ew->routine_2 ++ ;
		ew->timer0 = 5 ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_02_squash2( PLAYER_WORK *ew )
{
	ew->scale.x += 0.008f ;
	ew->scale.y -= 0.012f ;
	if( --ew->timer0 <= 0 ){
		ew->routine_2 ++ ;
		ew->timer0 = 5 ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_02_stretch2( PLAYER_WORK *ew )
{
	ew->scale.x += 0.008f ;
	ew->scale.y -= 0.012f ;
	if( --ew->timer0 <= 0 ){
		ew->timer0 = ew->timer1 = 3 ;
		ew->routine_1 ++ ;
		ew->routine_2 = 0 ;
		ew->scale.x = ew->scale.y = 1.0f ;
	}
}

/*==============================================================*/
/*			Holds the opponent, hurting them at each new		*/
/*			count of timer0, and lets them go at 0				*/
/*==============================================================*/
static void shl14_02_hold( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em_w ;

	pl_w->catch_flag = 1 ;
	if( ew->timer0 == ew->timer1 ) return ;
	if( ew->timer0 <= 0 ){
		ew->routine_1 ++ ;
		ew->disp_flag = 0 ;
		em_w = ew->em_w ;
		em_w->atck_no = pl14_at_pow_hold ;
		catch_damage( pl_w ) ;
		dust_set( ew, 0, 0x90, 4.0f, 4.0f ) ;
		dust_set( ew, 16, 0x90, 4.0f, 4.0f ) ;
		dust_set( ew, 0xf0, 0x90, 4.0f, 4.0f ) ;
		ew->be_flag = 0 ;
		return ;
	}
	ew->timer1 = ew->timer0 ;
	pl_char_set( ew, 22, ew->type + ew->timer0 ) ;
	em_w = ew->em_w ;
	em_w->atck_no = pl14_at_pow_hold ;
	catch_damage( pl_w ) ;
}

/*==============================================================*/
/*			Waits for SonSon's animation to call for the		*/
/*			throw												*/
/*==============================================================*/
static void shl14_02_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	pl_w->catch_flag = 1 ;
	if( pl_w->cg_type ){
		ew->routine_1 = 4 ;
		ew->routine_2 = 0 ;
	}
}

static void shl14_02_throw_set( PLAYER_WORK *ew );
static void shl14_02_throw_fall( PLAYER_WORK *ew );
static void shl14_02_throw_rise( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_02_throw( PLAYER_WORK *ew )
{
	static void ( *shl14_02_throw_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_02_throw_set, shl14_02_throw_fall, shl14_02_throw_rise,
	} ;
	PLAYER_WORK *pl_w ;

	shl14_02_throw_jmp[ew->routine_2]( ew ) ;
	pl_w = ew->master_w ;
	pl_w->catch_flag = 1 ;
}

/*==============================================================*/
/*			The opponent, still inside, is thrown up from in	*/
/*			front of SonSon										*/
/*==============================================================*/
static void shl14_02_throw_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Float x ;

	ew->routine_2 ++ ;
	ew->disp_flag = 1 ;
	ew->spd.x = -DOT_X( 2.0f ) ;
	ew->acc.x = 0.013020833f ;
	pl_w = ew->master_w ;
	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	x = -DOT_X( 20.0f ) ;
	if( pl_w->disp_dir ){
		ew->spd.x = DOT_X( 2.0f ) ;
		ew->acc.x = -0.013020833f ;
		x = DOT_X( 20.0f ) ;
	}
	ew->pos.x += x ;
	ew->pos.y += DOT_Y( 36.0f ) ;
	ew->spd.y = DOT_Y( 5.0f ) ;
	ew->acc.y = -0.46875f ;
	pl_char_set( ew, 22, 15 ) ;
}

/*==============================================================*/
/*			Spins down to the floor and bounces					*/
/*==============================================================*/
static void shl14_02_throw_fall( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;

	pl_char_move( ew ) ;
	ew->rot[2] += 0x1200 ;
	SHL14_02_MOVE( ew ) ;
	if( SHL14_02_LW( ew )->ground_y >= ew->pos.y ){
		ew->pos.y = SHL14_02_LW( ew )->ground_y ;
		ew->routine_2 ++ ;
		ew->spd.x = -DOT_X( 2.0f ) ;
		ew->acc.x = 0.013020833f ;
		pl_w = ew->master_w ;
		if( pl_w->disp_dir ){
			ew->spd.x = DOT_X( 2.0f ) ;
			ew->acc.x = -0.013020833f ;
		}
		ew->spd.y = DOT_Y( 2.5f ) ;
		ew->acc.y = -0.234375f ;
	}
}

/*==============================================================*/
/*			At the top of the bounce it bursts and lets the		*/
/*			opponent fall out									*/
/*==============================================================*/
static void shl14_02_throw_rise( PLAYER_WORK *ew )
{
	PLAYER_WORK *em_w, *pl_w ;

	pl_char_move( ew ) ;
	ew->rot[2] += 0x1200 ;
	SHL14_02_MOVE( ew ) ;
	if( ew->spd.y < 0.0f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		em_w = ew->em_w ;
		em_w->disp_flag = 1 ;
		em_w->catch_type = 16 ;
		em_w->atck_no = pl14_at_pow_throw ;
		em_w->pos.x = ew->pos.x ;
		em_w->pos.y = ew->pos.y ;
		em_w->pl_stance = STANCE_AIR ;
		pl_w = ew->master_w ;
		em_w->dm_hit_w = pl_w ;
		dust_set( ew, 0, 0x90, 4.0f, 4.0f ) ;
		dust_set( ew, 16, 0x90, 4.0f, 4.0f ) ;
		dust_set( ew, 0xf0, 0x90, 4.0f, 4.0f ) ;
		pl_color_set( pl_w, 0 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_02_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl14_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The shot goes when SonSon leaves her hyper combo	*/
/*==============================================================*/
static void shl14_02_master_ck( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != pl14_sp_pow ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_02.c" End of File	*/
