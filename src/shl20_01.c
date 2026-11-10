/* Felicia's effect 01 (character 20's shot 01): the grab effect of her Please Help Me, four kinds. Each flies
   in from the edge of the screen and, if it takes hold of the opponent, carries him through the
   hold; it tells her through her tlocal how the move is going. */
#include "player.h"
#include "pl_char.h"
#include "pl20_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL20_01_LOCAL_W {
	Uint16	char_no;	/* 00: Felicia's animation when it was set */
	Uint16	unk_02;
	Float	x;		/* 04: where the hold puts the effect */
	Float	y;		/* 08 */
} SHL20_01_LOCAL_W;
#define SHL20_01_LW( w )	( ( SHL20_01_LOCAL_W * )&( w )->obj_flag )

/* where the hold takes the opponent, by his character: x from him (mirrored with the facing) and y */
static Float shl20_01_pos_tbl1[59][2] = {
	{ 95.0f, -30.0f },
	{ 70.0f, -DOT_Y( 4.0f ) },
	{ 105.0f, -DOT_Y( 5.0f ) },
	{ DOT_X( 61.0f ), -DOT_Y( 19.0f ) },
	{ DOT_X( 67.0f ), DOT_Y( 4.0f ) },
	{ 90.0f, -DOT_Y( 17.0f ) },
	{ 105.0f, -15.0f },
	{ DOT_X( 56.0f ), -45.0f },
	{ DOT_X( 67.0f ), -DOT_Y( 8.0f ) },
	{ 75.0f, -DOT_Y( 18.0f ) },
	{ DOT_X( 53.0f ), -DOT_Y( 13.0f ) },
	{ DOT_X( 47.0f ), -DOT_Y( 4.0f ) },
	{ 95.0f, -DOT_Y( 10.0f ) },
	{ 60.0f, 0.0f },
	{ DOT_X( 25.0f ), -DOT_Y( 4.0f ) },
	{ DOT_X( 67.0f ), 15.0f },
	{ DOT_X( 64.0f ), -30.0f },
	{ 70.0f, -30.0f },
	{ DOT_X( 35.0f ), -DOT_Y( 18.0f ) },
	{ 60.0f, -DOT_Y( 5.0f ) },
	{ 75.0f, -DOT_Y( 18.0f ) },
	{ DOT_X( 52.0f ), -DOT_Y( 13.0f ) },
	{ 75.0f, -DOT_Y( 18.0f ) },
	{ DOT_X( 46.0f ), -DOT_Y( 2.0f ) },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ 85.0f, -DOT_Y( 17.0f ) },
	{ DOT_X( 52.0f ), -30.0f },
	{ DOT_X( 55.0f ), -DOT_Y( 22.0f ) },
	{ 95.0f, -30.0f },
	{ 90.0f, -DOT_Y( 13.0f ) },
	{ 75.0f, -DOT_Y( 17.0f ) },
	{ DOT_X( 53.0f ), -DOT_Y( 9.0f ) },
	{ DOT_X( 59.0f ), -30.0f },
	{ 95.0f, -30.0f },
	{ DOT_X( 55.0f ), -DOT_Y( 11.0f ) },
	{ 80.0f, -DOT_Y( 10.0f ) },
	{ 90.0f, -DOT_Y( 12.0f ) },
	{ 95.0f, -30.0f },
	{ 85.0f, -DOT_Y( 11.0f ) },
	{ 140.0f, 0.0f },
	{ 70.0f, -DOT_Y( 2.0f ) },
	{ DOT_X( 40.0f ), 0.0f },
	{ 50.0f, 0.0f },
	{ 100.0f, -DOT_Y( 18.0f ) },
	{ 80.0f, -30.0f },
	{ 65.0f, -15.0f },
	{ DOT_X( 44.0f ), -30.0f },
	{ 80.0f, -DOT_Y( 12.0f ) },
	{ DOT_X( 22.0f ), -DOT_Y( 6.0f ) },
	{ 80.0f, -30.0f },
	{ 80.0f, 0.0f },
	{ DOT_X( 76.0f ), 2.142857f },
	{ 75.0f, -DOT_Y( 3.0f ) },
	{ 70.0f, -DOT_Y( 15.0f ) },
	{ 70.0f, -30.0f },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ 80.0f, -135.0f },
} ;

static Float shl20_01_pos_tbl2[59][2] = {
	{ -DOT_X( 43.0f ), 2.142857f },
	{ -85.0f, DOT_Y( 13.0f ) },
	{ -DOT_X( 44.0f ), -15.0f },
	{ -DOT_X( 28.0f ), -DOT_Y( 4.0f ) },
	{ -DOT_X( 40.0f ), DOT_Y( 20.0f ) },
	{ -85.0f, -30.0f },
	{ -60.0f, DOT_Y( 4.0f ) },
	{ -DOT_X( 41.0f ), -DOT_Y( 16.0f ) },
	{ -DOT_X( 29.0f ), 0.0f },
	{ -85.0f, -DOT_Y( 9.0f ) },
	{ -DOT_X( 40.0f ), 0.0f },
	{ -DOT_X( 55.0f ), DOT_Y( 11.0f ) },
	{ -60.0f, -DOT_Y( 8.0f ) },
	{ -DOT_X( 79.0f ), 15.0f },
	{ -DOT_X( 65.0f ), 30.0f },
	{ -35.0f, DOT_Y( 25.0f ) },
	{ -DOT_X( 43.0f ), 0.0f },
	{ -DOT_X( 46.0f ), 0.0f },
	{ -DOT_X( 59.0f ), 0.0f },
	{ -85.0f, DOT_Y( 11.0f ) },
	{ -DOT_X( 41.0f ), -DOT_Y( 12.0f ) },
	{ -DOT_X( 52.0f ), -DOT_Y( 6.0f ) },
	{ -DOT_X( 52.0f ), -DOT_Y( 16.0f ) },
	{ -DOT_X( 31.0f ), DOT_Y( 12.0f ) },
	{ -DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ -DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ -DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ -DOT_X( 32.0f ), -DOT_Y( 8.0f ) },
	{ -70.0f, -30.0f },
	{ -70.0f, -DOT_Y( 22.0f ) },
	{ -DOT_X( 43.0f ), 2.142857f },
	{ -DOT_X( 40.0f ), -DOT_Y( 3.0f ) },
	{ -DOT_X( 49.0f ), -DOT_Y( 13.0f ) },
	{ -65.0f, 15.0f },
	{ -55.0f, -DOT_Y( 4.0f ) },
	{ -DOT_X( 43.0f ), 2.142857f },
	{ -DOT_X( 44.0f ), 0.0f },
	{ -DOT_X( 49.0f ), DOT_Y( 8.0f ) },
	{ -75.0f, DOT_Y( 8.0f ) },
	{ -DOT_X( 43.0f ), 2.142857f },
	{ -DOT_X( 37.0f ), 15.0f },
	{ -55.0f, DOT_Y( 23.0f ) },
	{ -DOT_X( 46.0f ), DOT_Y( 12.0f ) },
	{ -75.0f, DOT_Y( 25.0f ) },
	{ -DOT_X( 56.0f ), DOT_Y( 16.0f ) },
	{ -DOT_X( 32.0f ), 0.0f },
	{ -DOT_X( 32.0f ), DOT_Y( 4.0f ) },
	{ -85.0f, 15.0f },
	{ -DOT_X( 47.0f ), 0.0f },
	{ -40.0f, DOT_Y( 3.0f ) },
	{ -DOT_X( 62.0f ), 30.0f },
	{ -DOT_X( 32.0f ), DOT_Y( 4.0f ) },
	{ -90.0f, 30.0f },
	{ -DOT_X( 34.0f ), DOT_Y( 12.0f ) },
	{ -DOT_X( 38.0f ), DOT_Y( 23.0f ) },
	{ -DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ -80.0f, DOT_Y( 4.0f ) },
	{ -DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ -80.0f, -DOT_Y( 60.0f ) },
} ;

static Float shl20_01_pos_tbl3[59][2] = {
	{ DOT_X( 67.0f ), 30.0f },
	{ DOT_X( 65.0f ), DOT_Y( 25.0f ) },
	{ DOT_X( 71.0f ), DOT_Y( 23.0f ) },
	{ 125.0f, -DOT_Y( 2.0f ) },
	{ 130.0f, DOT_Y( 30.0f ) },
	{ DOT_X( 53.0f ), DOT_Y( 4.0f ) },
	{ DOT_X( 68.0f ), 30.0f },
	{ 120.0f, -DOT_Y( 5.0f ) },
	{ 125.0f, DOT_Y( 9.0f ) },
	{ DOT_X( 62.0f ), DOT_Y( 4.0f ) },
	{ DOT_X( 70.0f ), DOT_Y( 12.0f ) },
	{ 115.0f, 45.0f },
	{ DOT_X( 70.0f ), -2.142857f },
	{ 55.0f, DOT_Y( 36.0f ) },
	{ DOT_X( 59.0f ), 60.0f },
	{ DOT_X( 88.0f ), DOT_Y( 36.0f ) },
	{ DOT_X( 80.0f ), DOT_Y( 12.0f ) },
	{ 115.0f, DOT_Y( 13.0f ) },
	{ DOT_X( 58.0f ), DOT_Y( 8.0f ) },
	{ DOT_X( 58.0f ), DOT_Y( 22.0f ) },
	{ 110.0f, 0.0f },
	{ DOT_X( 67.0f ), DOT_Y( 19.0f ) },
	{ 110.0f, -DOT_Y( 2.0f ) },
	{ DOT_X( 71.0f ), DOT_Y( 26.0f ) },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ DOT_X( 56.0f ), -DOT_Y( 16.0f ) },
	{ 115.0f, DOT_Y( 4.0f ) },
	{ DOT_X( 53.0f ), -DOT_Y( 5.0f ) },
	{ DOT_X( 55.0f ), -DOT_Y( 12.0f ) },
	{ DOT_X( 67.0f ), 30.0f },
	{ DOT_X( 73.0f ), DOT_Y( 4.0f ) },
	{ DOT_X( 64.0f ), DOT_Y( 4.0f ) },
	{ DOT_X( 80.0f ), DOT_Y( 17.0f ) },
	{ 135.0f, DOT_Y( 8.0f ) },
	{ DOT_X( 67.0f ), 30.0f },
	{ DOT_X( 67.0f ), 30.0f },
	{ DOT_X( 71.0f ), DOT_Y( 19.0f ) },
	{ 130.0f, DOT_Y( 15.0f ) },
	{ DOT_X( 67.0f ), 30.0f },
	{ DOT_X( 74.0f ), DOT_Y( 12.0f ) },
	{ DOT_X( 91.0f ), DOT_Y( 38.0f ) },
	{ 110.0f, DOT_Y( 23.0f ) },
	{ DOT_X( 77.0f ), DOT_Y( 32.0f ) },
	{ DOT_X( 64.0f ), DOT_Y( 30.0f ) },
	{ DOT_X( 86.0f ), DOT_Y( 16.0f ) },
	{ DOT_X( 70.0f ), 30.0f },
	{ DOT_X( 74.0f ), DOT_Y( 15.0f ) },
	{ 120.0f, DOT_Y( 9.0f ) },
	{ DOT_X( 71.0f ), 15.0f },
	{ 105.0f, DOT_Y( 23.0f ) },
	{ DOT_X( 70.0f ), 30.0f },
	{ DOT_X( 67.0f ), DOT_Y( 24.0f ) },
	{ 145.0f, 60.0f },
	{ 140.0f, DOT_Y( 29.0f ) },
	{ 90.0f, DOT_Y( 5.0f ) },
	{ DOT_X( 70.0f ), DOT_Y( 13.0f ) },
	{ 120.0f, -DOT_Y( 5.0f ) },
	{ 90.0f, -DOT_Y( 47.0f ) },
} ;

static PL20_LOCAL_W *shl20_01_pl_lw ;
static SHL20_01_LOCAL_W *shl20_01_lw ;

static void shl20_01_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl20_01_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl20_01_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x2000 ;
	}
	return ew ;
}

static void shl20_01_0_main( PLAYER_WORK *ew );
static void shl20_01_1_main( PLAYER_WORK *ew );
static void shl20_01_2_main( PLAYER_WORK *ew );
static void shl20_01_3_main( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_01_main( PLAYER_WORK *ew )
{
	static void ( *shl20_01_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_0_main, shl20_01_1_main, shl20_01_2_main, shl20_01_3_main,
	} ;

	shl20_01_pl_lw = ( PL20_LOCAL_W * )&ew->master_w->tlocal[0] ;
	shl20_01_lw = SHL20_01_LW( ew ) ;
	shl20_01_jmp[ew->type]( ew ) ;
}

static void shl20_01_0_init( PLAYER_WORK *ew );
static void shl20_01_0_move( PLAYER_WORK *ew );
static void shl20_01_end( PLAYER_WORK *ew );
static void shl20_01_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_01_0_main( PLAYER_WORK *ew )
{
	static void ( *shl20_01_0_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_0_init, shl20_01_0_move, shl20_01_end, shl20_01_free,
	} ;

	shl20_01_0_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl20_01_0_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *tw = pl_w->target_w ;

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
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl20_at_help ;
	HIT_RESET( ew ) ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->pos.x = scr_work.eye[0].x + ( ew->disp_dir ? -373.33334f : 373.33334f ) ;
	ew->pos.y = pl_w->ground_y + DOT_X( 224.0f ) ;
	ew->spd.x = ( tw->pos.x - ew->pos.x ) / 32.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.y = ( tw->pos.y - ew->pos.y ) * 2.0f / 1024.0f ;
	if( ew->acc.y > -DOT_Y( 0.375f ) ) ew->acc.y = -DOT_Y( 0.375f ) ;
	shl20_01_lw->char_no = ew->master_w->char_no.w ;
	ew->dir = 0 ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

static void shl20_01_0_fly( PLAYER_WORK *ew );
static void shl20_01_0_fall( PLAYER_WORK *ew );
static void shl20_01_0_land( PLAYER_WORK *ew );
static void shl20_01_0_back( PLAYER_WORK *ew );
static void shl20_01_0_catch( PLAYER_WORK *ew );
static void shl20_01_0_hit( PLAYER_WORK *ew );
static void shl20_01_0_keep( PLAYER_WORK *ew );
static void shl20_01_0_last( PLAYER_WORK *ew );
static void shl20_01_0_out( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl20_01_0_move( PLAYER_WORK *ew )
{
	static void ( *shl20_01_0_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_0_fly, shl20_01_0_fall, shl20_01_0_land, shl20_01_0_back,
		shl20_01_0_catch, shl20_01_0_hit, shl20_01_0_keep, shl20_01_0_last,
		shl20_01_0_out,
	} ;

	shl20_01_0_move_jmp[ew->routine_1]( ew ) ;
}

static Uint8 shl20_01_off_ck( PLAYER_WORK *ew );
static void shl20_01_catch_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_01_0_fly( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->dir && shl20_01_lw->char_no != pl_w->char_no.w ) ew->dir = 1 ;
	if( ew->hit_flag ){
		if( !pl_sp_catch_ck( ew ) || ew->dir || shl20_01_off_ck( ew ) ){
			ew->routine_1 ++ ;
			shl20_01_pl_lw->catch_flag = 2 ;
			return ;
		} else {
			ew->routine_1 = 4 ;
			em = ew->em_w ;
			pl_w->dm_flag = 0 ;
			pl_w->catch_ng_timer = 2 ;
			pl_w->push_ng_timer = 2 ;
			shl20_01_pl_lw->catch_flag = 1 ;
			pl_w->catch_no = 0xc4 ;
			pl_ef_catch_set( ew, em ) ;
			pl_char_set( ew, 15, 7 ) ;
			if( PL_CK( em, PL_SERVBOT ) ) em->pos.y = pl_w->ground_y + DOT_Y( 40.0f ) ;
			else if( PL_CK( em, PL_ROLL ) ) em->pos.y = pl_w->ground_y + DOT_Y( 24.0f ) ;
			catch_em_pos_set1( ew, em ) ;
		}
		return ;
	}
	if( ew->pos.y <= ew->master_w->ground_y ){
		ew->routine_1 += 2 ;
		pl_char_set( ew, Char_Effect, 8 ) ;
		return ;
	}
	if( !ew->dir ) hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_0_fall( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->pos.y > ew->master_w->ground_y ) return ;
	ew->routine_1 ++ ;
	ew->pos.y = ew->master_w->ground_y ;
	pl_char_set( ew, Char_Effect, 8 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_0_land( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) >= 0 ) return ;
	ew->routine_1 ++ ;
	ew->disp_dir ^= 1 ;
	ew->spd.y = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = -ew->spd.x ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_0_back( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ){
		shl20_01_end( ew ) ;
		shl20_01_pl_lw->catch_flag = -1 ;
	}
}

/*--------------------------------------------------------------*/
static void shl20_01_0_catch( PLAYER_WORK *ew )
{
	PLAYER_WORK *em = ew->catch_w ;

	pl_char_move( ew ) ;
	catch_em_pos_set1( ew, em ) ;
	ew->master_w->catch_flag = 1 ;
	ew->master_w->catch_ng_timer = 2 ;
	ew->master_w->push_ng_timer = 2 ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase ){
		ew->routine_1 ++ ;
		ew->timer0 = 2 ;
		ew->timer1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl20_01_0_hit( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;

	ew->master_w->catch_flag = 1 ;
	ew->master_w->catch_ng_timer = 2 ;
	ew->master_w->push_ng_timer = 2 ;
	pl_char_move( ew ) ;
	catch_em_pos_set1( ew, em ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_catch_end( ew ) ;
		return ;
	}
	if( ! -- ew->timer0 ){
		ew->timer0 = 2 ;
		em->atck_no = pl20_at_help_hit ;
		catch_damage( ew ) ;
		combo_add( em, pl_w, 1 ) ;
		if( ++ ew->timer1 & 1 ) pl_se_req( ew, 2 ) ;
	}
	if( shl20_01_pl_lw->help_phase >= 2 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_0_keep( PLAYER_WORK *ew )
{
	PLAYER_WORK *em = ew->catch_w ;

	ew->master_w->catch_flag = 1 ;
	ew->master_w->catch_ng_timer = 2 ;
	ew->master_w->push_ng_timer = 2 ;
	catch_em_pos_set1( ew, em ) ;
	ew->routine_1 ++ ;
	if( shl20_01_off_ck( ew ) ) shl20_01_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_0_last( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em = ew->catch_w ;

	ew->master_w->catch_flag = 1 ;
	ew->master_w->catch_ng_timer = 2 ;
	ew->master_w->push_ng_timer = 2 ;
	catch_em_pos_set1( ew, em ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase < 3 ) return ;
	shl20_01_catch_end( ew ) ;
	em->dm_hit_w = pl_w ;
	em->catch_type = 1 ;
	em->atck_no = pl20_at_help_last ;
}

/*--------------------------------------------------------------*/
static void shl20_01_0_out( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) || ew->pos.y <= ew->master_w->ground_y ){
		shl20_01_end( ew ) ;
		shl20_01_pl_lw->catch_flag = -1 ;
	}
}

/*==============================================================*/
static Uint8 shl20_01_off_ck( PLAYER_WORK *ew )
{
	ew = ew->master_w ;
	if( ew->team_no ) return 1 ;
	return 0 ;
}

/*==============================================================*/
static void shl20_01_catch_end( PLAYER_WORK *ew )
{
	ew->routine_1 = 8 ;
	ew->master_w->catch_flag = 0 ;
	ew->spd.x = ew->disp_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = 15.0f ;
	ew->acc.y = -DOT_Y( 0.25f ) ;
	pl_char_set( ew, Char_Effect, 13 ) ;
}

static void shl20_01_1_init( PLAYER_WORK *ew );
static void shl20_01_1_move( PLAYER_WORK *ew );
static void shl20_01_1_catch_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_01_1_main( PLAYER_WORK *ew )
{
	static void ( *shl20_01_1_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_1_init, shl20_01_1_move, shl20_01_end, shl20_01_free,
	} ;

	shl20_01_1_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl20_01_1_init( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->disp_flag = 0 ;
	shl20_01_lw->char_no = ew->master_w->char_no.w ;
}

static void shl20_01_1_wait( PLAYER_WORK *ew );
static void shl20_01_1_fly( PLAYER_WORK *ew );
static void shl20_01_1_catch( PLAYER_WORK *ew );
static void shl20_01_1_hit( PLAYER_WORK *ew );
static void shl20_01_1_keep( PLAYER_WORK *ew );
static void shl20_01_1_last( PLAYER_WORK *ew );
static void shl20_01_1_out( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl20_01_1_move( PLAYER_WORK *ew )
{
	static void ( *shl20_01_1_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_1_wait, shl20_01_1_fly, shl20_01_1_catch, shl20_01_1_hit, shl20_01_1_keep, shl20_01_1_last, shl20_01_1_out,
	} ;

	shl20_01_1_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl20_01_1_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	if( shl20_01_lw->char_no != ew->master_w->char_no.w || shl20_01_off_ck( ew ) ){
		shl20_01_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->catch_flag == 0 ) return ;
	if( shl20_01_pl_lw->catch_flag != 1 ){
		shl20_01_end( ew ) ;
		return ;
	}
	em = pl_w->catch_w ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->size_front = 40 ;
	ew->size_back = 40 ;
	shl20_01_lw->x = em->pos.x + ( ew->disp_dir ? shl20_01_pos_tbl1[em->pl_type][0] : -shl20_01_pos_tbl1[em->pl_type][0] ) ;
	shl20_01_lw->y = em->pos.y + shl20_01_pos_tbl1[em->pl_type][1] ;
	ew->pos.x = scr_work.eye[0].x + ( ew->disp_dir ? DOT_X( 232.0f ) : -DOT_X( 232.0f ) ) ;
	ew->pos.y = shl20_01_lw->y ;
	ew->spd.x = ( shl20_01_lw->x - ew->pos.x ) / 16.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->timer0 = 16 ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 5 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_1_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->pos.x = shl20_01_lw->x ;
		ew->pos.y = shl20_01_lw->y ;
		pl_char_set( ew, Char_Effect, 10 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl20_01_1_catch( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_1_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_1_hit( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_1_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase >= 2 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_1_keep( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	if( shl20_01_off_ck( ew ) ) shl20_01_1_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_1_last( PLAYER_WORK *ew )
{
	if( shl20_01_off_ck( ew ) ){
		shl20_01_1_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase >= 3 ) shl20_01_1_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_1_out( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) || ( ew->spd.y < 0.0f && ew->pos.y <= ew->master_w->ground_y ) ) shl20_01_end( ew ) ;
}

/*==============================================================*/
static void shl20_01_1_catch_end( PLAYER_WORK *ew )
{
	ew->routine_1 = 6 ;
	ew->spd.x = ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	ew->acc.y = -0.03348214f ;
	pl_char_set( ew, Char_Effect, 14 ) ;
}

static void shl20_01_2_init( PLAYER_WORK *ew );
static void shl20_01_2_move( PLAYER_WORK *ew );
static void shl20_01_2_catch_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_01_2_main( PLAYER_WORK *ew )
{
	static void ( *shl20_01_2_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_2_init, shl20_01_2_move, shl20_01_end, shl20_01_free,
	} ;

	shl20_01_2_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl20_01_2_init( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->disp_flag = 0 ;
	shl20_01_lw->char_no = ew->master_w->char_no.w ;
}

static void shl20_01_2_wait( PLAYER_WORK *ew );
static void shl20_01_2_fly( PLAYER_WORK *ew );
static void shl20_01_2_catch( PLAYER_WORK *ew );
static void shl20_01_2_hit( PLAYER_WORK *ew );
static void shl20_01_2_keep( PLAYER_WORK *ew );
static void shl20_01_2_last( PLAYER_WORK *ew );
static void shl20_01_2_out( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl20_01_2_move( PLAYER_WORK *ew )
{
	static void ( *shl20_01_2_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_2_wait, shl20_01_2_fly, shl20_01_2_catch, shl20_01_2_hit, shl20_01_2_keep, shl20_01_2_last, shl20_01_2_out,
	} ;

	shl20_01_2_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl20_01_2_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	if( shl20_01_lw->char_no != ew->master_w->char_no.w || shl20_01_off_ck( ew ) ){
		shl20_01_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->catch_flag == 0 ) return ;
	if( shl20_01_pl_lw->catch_flag != 1 ){
		shl20_01_end( ew ) ;
		return ;
	}
	em = pl_w->catch_w ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->size_front = 48 ;
	ew->size_back = 48 ;
	shl20_01_lw->x = em->pos.x + ( ew->disp_dir ? shl20_01_pos_tbl2[em->pl_type][0] : -shl20_01_pos_tbl2[em->pl_type][0] ) ;
	shl20_01_lw->y = em->pos.y + shl20_01_pos_tbl2[em->pl_type][1] ;
	ew->pos.x = scr_work.eye[0].x + ( ew->disp_dir ? -400.0f : 400.0f ) ;
	ew->pos.y = shl20_01_lw->y ;
	ew->spd.x = ( shl20_01_lw->x - ew->pos.x ) / 16.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 4.0f ) ;
	ew->acc.y = -DOT_Y( 0.5f ) ;
	ew->timer0 = 16 ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 6 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_2_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->pos.x = shl20_01_lw->x ;
		ew->pos.y = shl20_01_lw->y ;
		pl_char_set( ew, Char_Effect, 11 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl20_01_2_catch( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_2_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_2_hit( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_2_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase >= 2 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_2_keep( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	if( shl20_01_off_ck( ew ) ) shl20_01_2_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_2_last( PLAYER_WORK *ew )
{
	if( shl20_01_off_ck( ew ) ){
		shl20_01_2_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase >= 3 ) shl20_01_2_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_2_out( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) || ( ew->spd.y < 0.0f && ew->pos.y <= ew->master_w->ground_y ) ) shl20_01_end( ew ) ;
}

/*==============================================================*/
static void shl20_01_2_catch_end( PLAYER_WORK *ew )
{
	ew->routine_1 = 6 ;
	ew->spd.x = ew->disp_dir ? -DOT_X( 8.0f ) : DOT_X( 8.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 2.0f ) ;
	ew->acc.y = -0.03348214f ;
	pl_char_set( ew, Char_Effect, 15 ) ;
}

static void shl20_01_3_init( PLAYER_WORK *ew );
static void shl20_01_3_move( PLAYER_WORK *ew );
static void shl20_01_3_catch_end( PLAYER_WORK *ew );

/*==============================================================*/
static void shl20_01_3_main( PLAYER_WORK *ew )
{
	static void ( *shl20_01_3_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_3_init, shl20_01_3_move, shl20_01_end, shl20_01_free,
	} ;

	shl20_01_3_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl20_01_3_init( PLAYER_WORK *ew )
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
	ew->prio = 11 ;
	ew->disp_flag = 0 ;
	shl20_01_lw->char_no = ew->master_w->char_no.w ;
}

static void shl20_01_3_wait( PLAYER_WORK *ew );
static void shl20_01_3_fly( PLAYER_WORK *ew );
static void shl20_01_3_catch( PLAYER_WORK *ew );
static void shl20_01_3_hit( PLAYER_WORK *ew );
static void shl20_01_3_keep( PLAYER_WORK *ew );
static void shl20_01_3_last( PLAYER_WORK *ew );
static void shl20_01_3_out( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl20_01_3_move( PLAYER_WORK *ew )
{
	static void ( *shl20_01_3_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl20_01_3_wait, shl20_01_3_fly, shl20_01_3_catch, shl20_01_3_hit, shl20_01_3_keep, shl20_01_3_last, shl20_01_3_out,
	} ;

	shl20_01_3_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl20_01_3_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *em ;

	if( shl20_01_lw->char_no != ew->master_w->char_no.w || shl20_01_off_ck( ew ) ){
		shl20_01_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->catch_flag == 0 ) return ;
	if( shl20_01_pl_lw->catch_flag != 1 ){
		shl20_01_end( ew ) ;
		return ;
	}
	em = pl_w->catch_w ;
	ew->routine_1 ++ ;
	ew->disp_flag = 1 ;
	ew->size_top = 16 ;
	ew->size_bottom = 120 ;
	shl20_01_lw->x = em->pos.x + ( ew->disp_dir ? shl20_01_pos_tbl3[em->pl_type][0] : -shl20_01_pos_tbl3[em->pl_type][0] ) ;
	shl20_01_lw->y = em->pos.y + shl20_01_pos_tbl3[em->pl_type][1] ;
	ew->pos.x = shl20_01_lw->x + ( ew->disp_dir ? 80.0f : -80.0f ) ;
	ew->pos.y = scr_work.eye[0].y + DOT_Y( 160.0f ) ;
	ew->spd.x = ( shl20_01_lw->x - ew->pos.x ) / 16.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = ( shl20_01_lw->y - ew->pos.y ) / 16.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = 16 ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 7 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_3_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ! -- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->pos.x = shl20_01_lw->x ;
		ew->pos.y = shl20_01_lw->y ;
		pl_char_set( ew, Char_Effect, 12 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl20_01_3_catch( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_3_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_3_hit( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( shl20_01_off_ck( ew ) ){
		shl20_01_3_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase >= 2 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl20_01_3_keep( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	if( shl20_01_off_ck( ew ) ) shl20_01_3_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_3_last( PLAYER_WORK *ew )
{
	if( shl20_01_off_ck( ew ) ){
		shl20_01_3_catch_end( ew ) ;
		return ;
	}
	if( shl20_01_pl_lw->help_phase >= 3 ) shl20_01_3_catch_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_3_out( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_y_ck( ew ) || ( ew->spd.y < 0.0f && ew->pos.y <= ew->master_w->ground_y ) ) shl20_01_end( ew ) ;
}

/*==============================================================*/
static void shl20_01_3_catch_end( PLAYER_WORK *ew )
{
	ew->routine_1 = 6 ;
	ew->spd.x = ew->disp_dir ? DOT_X( 2.0f ) : -DOT_X( 2.0f ) ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 6.0f ) ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 16 ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	shl20_01_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl20_01_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl20_01.c" End of File	*/
