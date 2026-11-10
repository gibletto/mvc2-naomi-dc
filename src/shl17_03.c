/* Cable's effect 03 (character 17's shot 03): the sixteen parts of the Hyper Viper Beam, which wave
   along a curve in front of him while he fires it. */
#include "player.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the part's own work: its animation when made, and the frames to the next hit */
typedef struct SHL17_03_LOCAL_W {
	Uint16	char_no;	/* 000: Cable's animation when it was made */
	Sint16	hit_timer;		/* 002: the head's frames to the next hit */
} SHL17_03_LOCAL_W;
#define SHL17_03_LW( w )	( ( SHL17_03_LOCAL_W * )&( w )->obj_flag )

/* the curve, y = k0 x^3 + k1 x^2 + k2 x, and the steps of its turning points and height */
typedef struct SHL17_03_CURVE {
	Float	k0, k1, k2;	/* 000 */
	Float	unk_0c;
	Float	step[4];	/* 010: for spd.x, spd.y, acc.x and acc.y */
} SHL17_03_CURVE;
#define SHL17_03_CV( w )	( ( SHL17_03_CURVE * )&( w )->obj_mtx )

static const Float shl17_03_lv_y[] = { 0.0f, 50.0f, 100.0f, 150.0f } ;	/* the height, by the beam's level */

/*==============================================================*/
/*			Whether the part is past the stage's end it is		*/
/*			flying to											*/
/*==============================================================*/
static int shl17_03_out_ck( PLAYER_WORK *ew )
{
	if( !ew->disp_dir ){
		if( scr_work.stage_left + -160.0f > ew->pos.x ) return 1 ;
	} else {
		if( ew->pos.x > scr_work.stage_right + 160.0f ) return 1 ;
	}
	return 0 ;
}

static void shl17_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl17_03_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *old ;
	int i ;
	SHL17_03_LOCAL_W *lw ;

	for( i = 0 ; i < 16 ; i ++ ){
		if( !i ){
			if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
				ew->ef_w = 0 ;
				old = 0 ;
			} else return ;
		} else {
			if( !( ew = work_get( ew, W_SHL, LIST_BEHIND ) ) ) return ;
		}
		lw = SHL17_03_LW( ew ) ;
		ew->work_func = shl17_03_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1703 ;
		ew->sub_no = i ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_w = old ;
		old = ew ;
		lw->char_no = ew->master_w->char_no.w ;
	}
	return ew ;
}

static void shl17_03_init( PLAYER_WORK *ew );
static void shl17_03_move( PLAYER_WORK *ew );
static void shl17_03_end( PLAYER_WORK *ew );
static void shl17_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl17_03_main( PLAYER_WORK *ew )
{
	static void ( *shl17_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl17_03_init, shl17_03_move, shl17_03_end, shl17_03_free,
	} ;

	shl17_03_jmp[ew->routine_0]( ew ) ;
}

static void shl17_03_hit_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl17_03_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;
	SHL17_03_LOCAL_W *lw = SHL17_03_LW( ew ) ;

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
	ew->disp_flag = 1 ;
	ew->pos = ew->master_w->pos ;
	if( pl_w->pl_stance == STANCE_AIR ){
		ew->pos.x += ew->disp_dir ? 182.0f : -182.0f ;
		ew->pos.y += 170.0f ;
	} else {
		ew->pos.x += ew->disp_dir ? 182.0f : -182.0f ;
		ew->pos.y += 170.0f ;
	}
	ew->spd.z = ew->pos.x ;
	ew->pos.x += ew->disp_dir ? ew->sub_no * 35 : -( ew->sub_no * 35 ) ;
	ew->acc.z = ew->pos.y ;
	if( ew->sub_no ){
		ew->prio = 0 ;
		ew->sub_prio = ew->sub_no ;
	} else {
		ew->prio = 0 ;
		ew->sub_prio = ew->sub_no ;
	}
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = ew->master_w->timer1 ;
	if( !( ew->sub_no % 3 ) ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		shl17_03_hit_set( ew, pl_w ) ;
		hit_work_set( ew ) ;
	} else {
		/* no is never set here */
		ew->atck_flag = 0 ;
		ew->dm_flag = 0 ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
	}
	lw->hit_timer = 8 ;
	PL17_LW( pl_w )->beam_hit[0] = 0 ;
	if( ew->sub_no == 15 ) pl_char_set( ew, Char_Effect, 7 ) ;
	else if( !ew->sub_no ){
		pl_char_set( ew, Char_Effect, 6 ) ;
		pl_voice_req( ew, 35 ) ;
	} else pl_char_set( ew, Char_Effect, 5 ) ;
}

static void shl17_03_curve( PLAYER_WORK *ew );

/*==============================================================*/
/*			Every third part hits; the head starts a new hit	*/
/*			every nine frames, cycling the hit's kind			*/
/*			through five										*/
/*==============================================================*/
static void shl17_03_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL17_03_LOCAL_W *lw = SHL17_03_LW( ew ) ;

	ew->prio = pl_w->prio ;
	ew->pos = ew->master_w->pos ;
	if( pl_w->pl_stance == STANCE_AIR ){
		ew->pos.x += ew->disp_dir ? 182.0f : -182.0f ;
		ew->pos.y += 170.0f ;
	} else {
		ew->pos.x += ew->disp_dir ? 182.0f : -182.0f ;
		ew->pos.y += 170.0f ;
	}
	ew->spd.z = ew->pos.x ;
	ew->pos.x += ew->disp_dir ? ew->sub_no * 35 : -( ew->sub_no * 35 ) ;
	ew->acc.z = ew->pos.y ;
	if( ew->pl_type != ew->master_w->pl_type ){
		shl17_03_free( ew ) ;
		return ;
	}
	if( lw->char_no != ew->master_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	if( !ew->sub_no ){
		if( PL17_LW( pl_w )->beam_hit[0] ) PL17_LW( pl_w )->beam_hit[0] = 0 ;
		if( lw->hit_timer ) lw->hit_timer -- ;
		else {
			lw->hit_timer = 8 ;
			PL17_LW( pl_w )->beam_hit_cnt[0] ++ ;
			PL17_LW( pl_w )->beam_hit_cnt[0] %= 5 ;
			PL17_LW( pl_w )->beam_hit[0] = 1 ;
		}
	}
	if( !( ew->sub_no % 3 ) ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
		if( PL17_LW( pl_w )->beam_hit[0] ) shl17_03_hit_set( ew, pl_w ) ;
		hit_work_set( ew ) ;
	}
	if( !shl17_03_out_ck( ew ) ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
	shl17_03_curve( ew ) ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
/* the head picks new turning points every four frames and the others follow the part before */
static void shl17_03_curve( PLAYER_WORK *ew )
{
	SHL17_03_CURVE *cv = SHL17_03_CV( ew ) ;
	const Float *tab = shl17_03_lv_y ;
	SHL17_03_CURVE *ocv ;
	PLAYER_WORK *q ;
	Float x, y, d ;
	Uint32 n ;
	Float wk[8] ;	/* unused */

	if( !ew->ef_w ){
		if( !( ew->timer1 % 4 ) ){
			ew->timer1 = 0 ;
			n = Rnd() ;
			n %= 200 ;
			n += 150 ;
			x = ew->disp_dir ? n : n * -1.0f ;
			if( !ew->routine_3 ) ew->spd.x = x ;
			cv->step[0] = ( x - ew->spd.x ) / 4.0f ;
			n = Rnd() ;
			n %= 200 ;
			n += 450 ;
			x = ew->disp_dir ? n : n * -1.0f ;
			if( !ew->routine_3 ) ew->acc.x = x ;
			cv->step[2] = ( x - ew->acc.x ) / 4.0f ;
			n = Rnd() ;
			n %= 50 ;
			x = tab[PL17_LW( ew->master_w )->beam_lv[0]] ;
			x += n ;
			if( ew->master_w->pl_new_lvbt & KEY_UP ) y = x ;
			else if( ew->master_w->pl_new_lvbt & KEY_DOWN ){ if( 0 ) x = x ; y = -x ; }
			else y = ( n & 1 ) ? x : -x ;
			if( !ew->routine_3 ) ew->spd.y = y ;
			cv->step[1] = ( y - ew->spd.y ) / 4.0f ;
			n = Rnd() ;
			n %= 50 ;
			x = tab[PL17_LW( ew->master_w )->beam_lv[0]] ;
			x += n ;
			if( ew->master_w->pl_new_lvbt & KEY_UP ) y = x ;
			else if( ew->master_w->pl_new_lvbt & KEY_DOWN ){ if( 0 ) x = x ; y = -x ; }
			else y = ( n & 1 ) ? x : -x ;
			if( !ew->routine_3 ){
				ew->routine_3 = 1 ;
				ew->acc.y = y ;
			}
			cv->step[3] = ( y - ew->acc.y ) / 4.0f ;
		}
		ew->spd.x += cv->step[0] ;
		ew->spd.y += cv->step[1] ;
		ew->acc.x += cv->step[2] ;
		ew->acc.y += cv->step[3] ;
		d = ( x = ew->spd.x ) * x * x - ( x + ( y = ew->acc.x ) ) * x * x * 1.5f + 3.0f * ( x * x * y ) ;
		cv->k0 = ew->spd.y / d ;
		if( 0 ) cv->k0 = x + y * y * y * y * y * y ;
		cv->k1 = -( cv->k0 * ( ew->spd.x + ew->acc.x ) * 1.5f ) ;
		cv->k2 = cv->k0 * ew->spd.x * ew->acc.x * 3.0f ;
		ew->timer1 ++ ;
		ew->rot[2] = -nlArcTan( cv->k2 ) ;
		ew->rot[2] = ew->disp_dir ? ew->rot[2] : -ew->rot[2] ;
	} else {
		ocv = SHL17_03_CV( ew->ef_w ) ;
		ew->spd = ew->ef_w->spd ;
		ew->acc = ew->ef_w->acc ;
		cv->k0 = ocv->k0 ;
		cv->k1 = ocv->k1 ;
		cv->k2 = ocv->k2 ;
		cv->step[0] = ocv->step[0] ;
		cv->step[1] = ocv->step[1] ;
		cv->step[2] = ocv->step[2] ;
		cv->step[3] = ocv->step[3] ;
		q = ew->ef_w ;
		x = q->pos.x - q->spd.z ;
		x += ew->disp_dir ? nlCos( ew->ef_w->rot[2] ) * 35.0f : -( nlCos( ew->ef_w->rot[2] ) * 35.0f ) ;
		y = cv->k0 * x * x * x + cv->k1 * x * x + cv->k2 * x ;
		ew->rot[2] = -nlArcTan( 3.0f * ( cv->k0 * x * x ) + 2.0f * cv->k1 * x + cv->k2 ) ;
		ew->rot[2] = ew->disp_dir ? ew->rot[2] : -ew->rot[2] ;
		ew->pos.x = x + ew->spd.z ;
		ew->pos.y = y + ew->acc.z ;
	}
}

/*--------------------------------------------------------------*/
static void shl17_03_end( PLAYER_WORK *ew )
{
	pl_voice_req( ew, 43 ) ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The hit: the stronger one on every fifth of the		*/
/*			head's hits											*/
/*==============================================================*/
static void shl17_03_hit_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Uint8 no ;

	if( !PL17_LW( pl_w )->beam_hit_cnt[0] ) no = 58 ;
	else no = 57 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_03.c" End of File	*/
