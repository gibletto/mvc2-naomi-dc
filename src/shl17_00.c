/* Cable's effect 00 (character 17's shot 00): the fifteen parts of the Viper Beam, which wave along
   a curve in front of him while he fires it. */
#include "player.h"
#include "pl_char.h"
#include "pl17_def.h"

/* the part's own work: its animation when made, and its timers */
typedef struct SHL17_00_LOCAL_W {
	Uint16	char_no;	/* 000: Cable's animation when it was made */
	Sint16	unk_2;		/* 002 */
	Sint16	part_hit_flag;		/* 004: set when a part has hit */
	Sint16	hit_wait_timer;		/* 006: frames before the first hit */
	Sint16	hit_set_flag;		/* 008: set once the first hit is set up */
} SHL17_00_LOCAL_W;
#define SHL17_00_LW( w )	( ( SHL17_00_LOCAL_W * )&( w )->obj_flag )

/* the curve, y = a x^3 + b x^2 + c x, from its two turning points and its height, and
   their steps towards new ones */
typedef struct SHL17_00_CURVE {
	Float	a;		/* 000 */
	Float	b;
	Float	c;
	Float	unk_0c;
	Float	step[4];	/* 010: for spd.x, spd.y, acc.x and acc.y */
} SHL17_00_CURVE;
#define SHL17_00_CV( w )	( ( SHL17_00_CURVE * )&( w )->obj_mtx )

static const Uint8 shl17_00_num[] = { 15, 15 } ;	/* the parts, by strength */
static const Float shl17_00_lv_y[] = { 0.0f, 50.0f, 100.0f, 150.0f } ;	/* the height, by the beam's level */

/*==============================================================*/
/*			Whether the part is past the stage's end it is		*/
/*			flying to											*/
/*==============================================================*/
static int shl17_00_out_ck( PLAYER_WORK *ew )
{
	if( !ew->disp_dir ){
		if( scr_work.stage_left + -DOT_X( 88.0f ) > ew->pos.x ) return 1 ;
	} else {
		if( ew->pos.x > scr_work.stage_right + DOT_X( 88.0f ) ) return 1 ;
	}
	return 0 ;
}

static void shl17_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl17_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	PLAYER_WORK *old ;
	int i ;
	SHL17_00_LOCAL_W *lw ;

	for( i = 0 ; i < shl17_00_num[pl_w->atck_level] ; i ++ ){
		if( !i ){
			if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
				ew->ef_w = 0 ;
				old = 0 ;
			} else return ;
		} else {
			if( !( ew = work_get( ew, W_SHL, LIST_BEHIND ) ) ) return ;
		}
		lw = SHL17_00_LW( ew ) ;
		ew->work_func = shl17_00_main ;
		ew->master_w = pl_w ;
		ew->ef_no = 0x1700 ;
		ew->sub_no = i ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->ef_w = old ;
		lw->char_no = ew->master_w->char_no.w ;
		old = ew ;
	}
	return ew ;
}

static void shl17_00_init( PLAYER_WORK *ew );
static void shl17_00_move( PLAYER_WORK *ew );
static void shl17_00_end( PLAYER_WORK *ew );
static void shl17_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl17_00_main( PLAYER_WORK *ew )
{
	static void ( *shl17_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl17_00_init, shl17_00_move, shl17_00_end, shl17_00_free,
	} ;

	shl17_00_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The first hit: the even parts can hit and be hit	*/
/*==============================================================*/
static void shl17_00_hit_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;

	if( !( ew->sub_no % 2 ) ){
		ew->atck_flag = HIT_ON | HIT_SHOT ;
		ew->dm_flag = HIT_ON | HIT_SHOT ;
	} else {
		ew->atck_flag = 0 ;
		ew->dm_flag = 0 ;
	}
	if( pl_w->atck_level ){
		if( ew->sub_no / 2 % 2 == 1 ) no = 68 ;
		else no = 69 ;
	} else {
		if( ew->sub_no / 2 % 2 == 1 ) no = 48 ;
		else no = 56 ;
	}
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
}

/*==============================================================*/
/*			The later hits: none for eight frames after a		*/
/*			part has hit										*/
/*==============================================================*/
static void shl17_00_hit_next( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL17_00_LOCAL_W *lw = SHL17_00_LW( ew ) ;

	if( ew->hit_flag ){
		lw->part_hit_flag = 1 ;
		PL17_LW( pl_w )->beam_hit[0] = 8 ;
	}
	if( !ew->sub_no && PL17_LW( pl_w )->beam_hit[0] ) PL17_LW( pl_w )->beam_hit[0] -- ;
	if( PL17_LW( pl_w )->beam_hit[0] ){
		Uint8 no ;

		ew->dm_flag = ew->atck_flag = 0 ;
		if( pl_w->atck_level ) no = ( ew->sub_no / 2 % 2 == 1 ) ? 68 : 69 ;
		else no = ( ew->sub_no / 2 % 2 == 1 ) ? 48 : 56 ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
	} else {
		Uint8 no ;

		if( !( ew->sub_no % 2 ) ){
			ew->atck_flag = HIT_ON | HIT_SHOT ;
			ew->dm_flag = HIT_ON | HIT_SHOT ;
		} else {
			ew->dm_flag = ew->atck_flag = 0 ;
		}
		if( pl_w->atck_level ) no = ( ew->sub_no / 2 % 2 == 1 ) ? 68 : 69 ;
		else no = ( ew->sub_no / 2 % 2 == 1 ) ? 48 : 56 ;
		ew->atck_no = no ;
		HIT_RESET( ew ) ;
	}
}

/*==============================================================*/
static void shl17_00_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 no ;
	SHL17_00_LOCAL_W *lw = SHL17_00_LW( ew ) ;

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
		ew->pos.x += ew->disp_dir ? 143.0f : -143.0f ;
		ew->pos.y += 187.0f ;
	} else {
		ew->pos.x += ew->disp_dir ? 130.0f : -130.0f ;
		ew->pos.y += 136.0f ;
	}
	ew->spd.z = ew->pos.x ;
	ew->pos.x += ew->disp_dir ? ew->sub_no * 40 : -( ew->sub_no * 40 ) ;
	ew->acc.z = ew->pos.y ;
	if( ew->sub_no ){
		ew->prio = 0 ;
		ew->sub_prio = ew->sub_no ;
	} else {
		ew->prio = 11 ;
		ew->sub_prio = ew->sub_no ;
		pl_voice_req( ew, 32 ) ;
	}
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = ew->master_w->timer1 ;
	ew->dm_flag = ew->atck_flag = 0 ;
	if( ew->sub_no / 2 % 2 == 1 ) no = 48 ;
	else no = 56 ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	lw->unk_2 = 8 ;
	PL17_LW( pl_w )->beam_hit[0] = 0 ;
	if( ew->sub_no == 14 ) pl_char_set( ew, Char_Effect, 2 ) ;
	else if( !ew->sub_no ) pl_char_set( ew, Char_Effect, 1 ) ;
	else pl_char_set( ew, Char_Effect, 0 ) ;
	lw->hit_wait_timer = 2 ;
}

static void shl17_00_curve( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl17_00_move( PLAYER_WORK *ew )
{
	SHL17_00_LOCAL_W *lw = SHL17_00_LW( ew ) ;
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->prio = pl_w->prio ;
	if( lw->hit_wait_timer ) lw->hit_wait_timer -- ;
	else if( !lw->hit_set_flag ){
		lw->hit_set_flag = 1 ;
		shl17_00_hit_set( ew ) ;
	} else shl17_00_hit_next( ew ) ;
	ew->pos = ew->master_w->pos ;
	if( pl_w->pl_stance == STANCE_AIR ){
		ew->pos.x += ew->disp_dir ? 143.0f : -143.0f ;
		ew->pos.y += 187.0f ;
	} else {
		ew->pos.x += ew->disp_dir ? 130.0f : -130.0f ;
		ew->pos.y += 136.0f ;
	}
	ew->spd.z = ew->pos.x ;
	ew->pos.x += ew->disp_dir ? ew->sub_no * 40 : -( ew->sub_no * 40 ) ;
	ew->acc.z = ew->pos.y ;
	if( ew->pl_type != ew->master_w->pl_type ){
		shl17_00_free( ew ) ;
		return ;
	}
	if( lw->char_no != ew->master_w->char_no.w ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
	}
	if( !( ew->sub_no % 2 ) && !shl17_00_out_ck( ew ) && !ew->hit_flag ) hit_work_set( ew ) ;
	if( !shl17_00_out_ck( ew ) ) ew->disp_flag = 1 ;
	else ew->disp_flag = 0 ;
	shl17_00_curve( ew ) ;
	pl_char_move( ew ) ;
}

/*==============================================================*/
/*			The head picks new turning points every four		*/
/*			frames and the others follow the part before		*/
/*==============================================================*/
static void shl17_00_curve( PLAYER_WORK *ew )
{
	SHL17_00_CURVE *cv = SHL17_00_CV( ew ) ;
	SHL17_00_CURVE *prev ;
	const Float *lv_y = shl17_00_lv_y ;
	Float x, y, den ;
	Float work[8] ;	/* unused */
	Uint32 n ;

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
			x = lv_y[PL17_LW( ew->master_w )->beam_lv[0]] ;
			x += n ;
			if( ew->master_w->pl_new_lvbt & KEY_UP ) y = x ;
			else if( ew->master_w->pl_new_lvbt & KEY_DOWN ){ if( 0 ) x = x ; y = -x ; }
			else y = ( n & 1 ) ? x : -x ;
			if( !ew->routine_3 ) ew->spd.y = y ;
			cv->step[1] = ( y - ew->spd.y ) / 4.0f ;
			n = Rnd() ;
			n %= 50 ;
			x = lv_y[PL17_LW( ew->master_w )->beam_lv[0]] ;
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
		den = ( x = ew->spd.x ) * x * x - ( x + ( y = ew->acc.x ) ) * x * x * 1.5f + 3.0f * ( x * x * y ) ;
		cv->a = ew->spd.y / den ;
		if( 0 ) cv->a = x + y * y * y * y * y * y ;
		cv->b = -( cv->a * ( ew->spd.x + ew->acc.x ) * 1.5f ) ;
		cv->c = cv->a * ew->spd.x * ew->acc.x * 3.0f ;
		ew->timer1 ++ ;
		ew->rot[2] = -nlArcTan( cv->c ) ;
		ew->rot[2] = ew->disp_dir ? ew->rot[2] : -ew->rot[2] ;
		return ;
	}
	prev = SHL17_00_CV( ew->ef_w ) ;
	ew->spd = ew->ef_w->spd ;
	ew->acc = ew->ef_w->acc ;
	cv->a = prev->a ;
	cv->b = prev->b ;
	cv->c = prev->c ;
	cv->step[0] = prev->step[0] ;
	cv->step[1] = prev->step[1] ;
	cv->step[2] = prev->step[2] ;
	cv->step[3] = prev->step[3] ;
	x = ew->ef_w->pos.x - ew->ef_w->spd.z ;
	x += ew->disp_dir ? nlCos( ew->ef_w->rot[2] ) * 40.0f : -( nlCos( ew->ef_w->rot[2] ) * 40.0f ) ;
	y = cv->a * x * x * x + cv->b * x * x + cv->c * x ;
	ew->rot[2] = -nlArcTan( 3.0f * ( cv->a * x * x ) + 2.0f * cv->b * x + cv->c ) ;
	ew->rot[2] = ew->disp_dir ? ew->rot[2] : -ew->rot[2] ;
	ew->pos.x = x + ew->spd.z ;
	ew->pos.y = y + ew->acc.z ;
}

/*--------------------------------------------------------------*/
static void shl17_00_end( PLAYER_WORK *ew )
{
	pl_voice_req( ew, 43 ) ;
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl17_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl17_00.c" End of File	*/
