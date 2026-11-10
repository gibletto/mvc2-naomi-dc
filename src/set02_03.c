/* Stage 2's object 3: three small figures in a row (kind 5 works) that wander, hop, turn and run,
   greet a player who comes close, and hop when a side joins or the fight ends (when they let some
   more fly off). Each has two wings that swing to and fro while their model changes. Its entry,
   set02_03_set, is the wrapper at the module's end. */
#include "player.h"

#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/* the stage's models */

/* where the three stand */
static const VEC3 set02_03_pos[] = {
	{ -200.0f, 9.6f, -154.0f },
	{ 0.0f, 9.6f, -124.0f },
	{ 300.0f, 9.6f, -94.0f },
} ;

static const VEC3 set02_03_wing_pos = { 30.497f, -3.406f, 0.21f } ;

/* the turn each wing swings between, in degrees */
static const Float set02_03_wing_ang[] = { 20.0f, -20.0f } ;

/* the models a wing shows in turn */
static const Sint32 set02_03_wing_obj[] = { 0x24, 0x25, 0x24, 0x26 } ;
const Uint8 set02_03_wing_no[] = { 0, 1, 0, 2 } ;	/* the same, from the first: never used */

/****************************************************************/
Uint8 set02_03_out_ck( PLAYER_WORK *ew )
{
	if( !( ew->pos.x < scr_work.stage_right ) ){
		if( ew->disp_dir ){
			return 0 ;
		}
		return 1 ;
	}
	if( ew->pos.x <= scr_work.stage_left && ew->disp_dir ){
		return 1 ;
	}
	return 0 ;
}

static void set02_03_wing_main( PLAYER_WORK *ew );
static void set02_03_wing_flap( PLAYER_WORK *ew );
static void set02_03_wing_swing( PLAYER_WORK *ew );

/*==============================================================*/
static void set02_03_wing_main( PLAYER_WORK *ew )
{
	static void ( *set02_03_wing0_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_wing_flap, set02_03_wing_swing,
	} ;

	set02_03_wing0_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
/*			Start a swing, every ten of them the next model		*/
/*==============================================================*/
static void set02_03_wing_flap( PLAYER_WORK *ew )
{
	ew->spd.y = set02_03_wing_ang[ew->routine_1] ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc.y = set02_03_wing_ang[ew->routine_1] ;
	ew->scale.y = ( ew->acc.y - ew->spd.y ) / 200.0f ;
	ew->timer1 ++ ;
	if( ew->timer1 > 9 ){
		const Sint32 *tbl = set02_03_wing_obj ;

		ew->timer1 = 0 ;
		ew->routine_2 ++ ;
		ew->routine_2 &= 3 ;
		ew->obj = obj_stg->obj[tbl[ew->routine_2]] ;
	}
	ew->routine_0 = 1 ;
	set02_03_wing_swing( ew ) ;
	return ;
}

/*==============================================================*/
/*			The turn goes on for 200 frames: scale.y is its		*/
/*			step												*/
/*==============================================================*/
static void set02_03_wing_swing( PLAYER_WORK *ew )
{
	ew->timer0 ++ ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[1] += DEG_ANG( ew->scale.y * ew->timer0 ) ;
	if( ew->timer0 > 200 ){
		ew->timer0 = 0 ;
		ew->routine_0 = 0 ;
	}
}

/****************************************************************/
void set02_03_wing_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_03_wing_main ;
		ew->obj = obj_stg->obj[36] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_03_wing_pos ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->master_w = pl_w ;
	}
}

static void set02_03_fly_wing_main( PLAYER_WORK *ew );
static void set02_03_fly_wing_flap( PLAYER_WORK *ew );
static void set02_03_fly_wing_swing( PLAYER_WORK *ew );

/*==============================================================*/
static void set02_03_fly_wing_main( PLAYER_WORK *ew )
{
	static void ( *set02_03_wing1_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_fly_wing_flap, set02_03_fly_wing_swing,
	} ;

	set02_03_wing1_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_03_fly_wing_flap( PLAYER_WORK *ew )
{
	ew->spd.y = set02_03_wing_ang[ew->routine_1] ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->routine_1 ++ ;
	ew->routine_1 &= 1 ;
	ew->acc.y = set02_03_wing_ang[ew->routine_1] ;
	ew->scale.y = ( ew->acc.y - ew->spd.y ) / 200.0f ;
	ew->timer1 ++ ;
	if( ew->timer1 > 9 ){
		const Sint32 *tbl = set02_03_wing_obj ;

		ew->timer1 = 0 ;
		ew->routine_2 ++ ;
		ew->routine_2 &= 3 ;
		ew->obj = obj_stg->obj[tbl[ew->routine_2]] ;
	}
	ew->routine_0 = 1 ;
	set02_03_fly_wing_swing( ew ) ;
	return ;
}

/*==============================================================*/
/*			It ends with the work that made it					*/
/*==============================================================*/
static void set02_03_fly_wing_swing( PLAYER_WORK *ew )
{
	if( ew->master_w->routine_0 == 2 ){
		work_free( ew ) ;
	}
	ew->timer0 ++ ;
	ew->rot[1] = DEG_ANG( ew->spd.y ) ;
	ew->rot[1] += DEG_ANG( ew->scale.y * ew->timer0 ) ;
	if( ew->timer0 > 200 ){
		ew->timer0 = 0 ;
		ew->routine_0 = 0 ;
	}
}

/****************************************************************/
void set02_03_fly_wing_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SET, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = set02_03_fly_wing_main ;
		ew->obj = obj_stg->obj[36] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->pos = set02_03_wing_pos ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->master_w = pl_w ;
	}
}

static void set02_03_roam( PLAYER_WORK *ew );
static void set02_03_greet( PLAYER_WORK *ew );
static void set02_03_cheer( PLAYER_WORK *ew );
static void set02_03_end( PLAYER_WORK *ew );
static void set02_03_roam_next( PLAYER_WORK *ew );
static void set02_03_roam_act( PLAYER_WORK *ew );
static void set02_03_stand( PLAYER_WORK *ew );
static void set02_03_stand_init( PLAYER_WORK *ew );
static void set02_03_stand_wait( PLAYER_WORK *ew );
static void set02_03_walk( PLAYER_WORK *ew );
static void set02_03_walk_init( PLAYER_WORK *ew );
static void set02_03_walk_move( PLAYER_WORK *ew );
static void set02_03_trot( PLAYER_WORK *ew );
static void set02_03_trot_init( PLAYER_WORK *ew );
static void set02_03_trot_move( PLAYER_WORK *ew );
static void set02_03_run( PLAYER_WORK *ew );
static void set02_03_run_init( PLAYER_WORK *ew );
static void set02_03_run_move( PLAYER_WORK *ew );
static void set02_03_turn( PLAYER_WORK *ew );
static void set02_03_turn_init( PLAYER_WORK *ew );
static void set02_03_turn_move( PLAYER_WORK *ew );
static void set02_03_look( PLAYER_WORK *ew );
static void set02_03_look_init( PLAYER_WORK *ew );
static void set02_03_look_in( PLAYER_WORK *ew );
static void set02_03_look_move( PLAYER_WORK *ew );
static void set02_03_look_out( PLAYER_WORK *ew );
static void set02_03_hop( PLAYER_WORK *ew );
static void set02_03_hop_init( PLAYER_WORK *ew );
static void set02_03_hop_move( PLAYER_WORK *ew );
static Uint8 set02_03_near_ck( PLAYER_WORK *ew );
static void set02_03_greet_next( PLAYER_WORK *ew );
static void set02_03_greet_act( PLAYER_WORK *ew );
static void set02_03_cheer_next( PLAYER_WORK *ew );
static void set02_03_cheer_act( PLAYER_WORK *ew );
static Uint8 set02_03_side_ck( PLAYER_WORK *ew );
static void set02_03_end_next( PLAYER_WORK *ew );
static void set02_03_end_act( PLAYER_WORK *ew );
static Uint8 set02_03_end_ck( PLAYER_WORK *ew );
static void set02_03_fly_make( PLAYER_WORK *ew );
static void set02_03_fly_main( PLAYER_WORK *ew );

/*==============================================================*/
/*			The four moods: roaming, toward a player			*/
/*			(greet), a side joining (cheer), the fight's end	*/
/*==============================================================*/
static void set02_03_main( PLAYER_WORK *ew )
{
	static void ( *set02_03_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_roam, set02_03_greet, set02_03_cheer, set02_03_end,
	} ;

	set02_03_jmp[ew->routine_0]( ew ) ;
	return ;
}

/*==============================================================*/
static void set02_03_roam( PLAYER_WORK *ew )
{
	static void ( *set02_03_roam_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_roam_next, set02_03_roam_act,
	} ;

	set02_03_roam_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			The next of seven things to do						*/
/*==============================================================*/
static void set02_03_roam_next( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->routine_2 ++ ;
	ew->routine_2 %= 7 ;
}

/*--------------------------------------------------------------*/
static void set02_03_roam_act( PLAYER_WORK *ew )
{
	static void ( *set02_03_roam_act_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_stand, set02_03_trot, set02_03_walk, set02_03_look,
		set02_03_trot, set02_03_walk, set02_03_turn,
	} ;

	set02_03_roam_act_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
static void set02_03_stand( PLAYER_WORK *ew )
{
	static void ( *set02_03_stand_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_stand_init, set02_03_stand_wait,
	} ;

	set02_03_stand_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_stand_init( PLAYER_WORK *ew )
{
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer0 = ( int )Rnd() % 90 ;
	ew->timer0 += 10 ;
	ew->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void set02_03_stand_wait( PLAYER_WORK *ew )
{
	ew->timer1 ++ ;
	ew->rot[1] = ew->disp_dir ? 0 : 0x8000 ;
	if( !( ew->timer1 %= 8 ) ) ew->type ^= 1 ;
	ew->rot[1] += ew->type ? 0x5b0 : -0x5b0 ;
	if( set02_03_end_ck( ew ) ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	if( set02_03_side_ck( ew ) ) return ;
	if( set02_03_near_ck( ew ) ) return ;
	if( !--ew->timer0 ){
		ew->routine_3 = 0 ;
		ew->routine_1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_walk( PLAYER_WORK *ew )
{
	static void ( *set02_03_walk_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_walk_init, set02_03_walk_move,
	} ;

	set02_03_walk_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_walk_init( PLAYER_WORK *ew )
{
	ew->acc.x = -0.2f ;
	ew->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void set02_03_walk_move( PLAYER_WORK *ew )
{
	if( set02_03_end_ck( ew ) || set02_03_near_ck( ew ) ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	ew->spd.x += ew->disp_dir ? ew->acc.x : -ew->acc.x ;
	ew->pos.x += ew->disp_dir ? -ew->spd.x : ew->spd.x ;
	if( ew->spd.x > 0.0f ){
		if( set02_03_side_ck( ew ) ) return ;
		if( !set02_03_out_ck( ew ) ) return ;
	}
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->routine_3 = 0 ;
	ew->routine_1 = 0 ;
}

/*--------------------------------------------------------------*/
static void set02_03_trot( PLAYER_WORK *ew )
{
	static void ( *set02_03_trot_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_trot_init, set02_03_trot_move,
	} ;

	set02_03_trot_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_trot_init( PLAYER_WORK *ew )
{
	ew->timer0 = ( int )Rnd() % 150 ;
	ew->timer0 += 10 ;
	ew->spd.x = 3.0f ;
	ew->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void set02_03_trot_move( PLAYER_WORK *ew )
{
	if( set02_03_end_ck( ew ) ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	if( set02_03_side_ck( ew ) ) return ;
	if( set02_03_near_ck( ew ) ) return ;
	ew->timer1 ++ ;
	ew->pos.x += ew->disp_dir ? -ew->spd.x : ew->spd.x ;
	ew->rot[1] = ew->disp_dir ? 0 : 0x8000 ;
	if( !( ew->timer1 %= 8 ) ) ew->type ^= 1 ;
	ew->rot[1] += ew->type ? 0x889 : -0x889 ;
	if( !--ew->timer0 || set02_03_out_ck( ew ) ){
		ew->routine_1 = ew->routine_3 = 0 ;
		ew->rot[2] = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_run( PLAYER_WORK *ew )
{
	static void ( *set02_03_run_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_run_init, set02_03_run_move,
	} ;

	set02_03_run_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_run_init( PLAYER_WORK *ew )
{
	ew->timer0 = ( int )Rnd() % 5 ;
	ew->timer0 += 20 ;
	ew->spd.x = 6.0f ;
	ew->routine_3 ++ ;
}

/*--------------------------------------------------------------*/
static void set02_03_run_move( PLAYER_WORK *ew )
{
	if( set02_03_end_ck( ew ) ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	if( set02_03_side_ck( ew ) ) return ;
	if( set02_03_near_ck( ew ) ){
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		return ;
	}
	ew->timer1 ++ ;
	ew->pos.x += ew->disp_dir ? -ew->spd.x : ew->spd.x ;
	ew->rot[1] = ew->disp_dir ? 0 : 0x8000 ;
	if( !( ew->timer1 %= 4 ) ) ew->type ^= 1 ;
	ew->rot[1] += ew->type ? 0xccd : -0xccd ;
	ew->rot[2] = ew->type ? 0xaab : -0xaab ;
	if( !--ew->timer0 || set02_03_out_ck( ew ) ){
		ew->rot[2] = 0 ;
		ew->routine_1 = ew->routine_3 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_turn( PLAYER_WORK *ew )
{
	static void ( *set02_03_turn_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_turn_init, set02_03_turn_move,
	} ;

	set02_03_turn_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_turn_init( PLAYER_WORK *ew )
{
	ew->timer1 = ( int )Rnd() % 2 ;
	ew->rot[1] = ew->disp_dir ? 0 : 0x8000 ;
	ew->routine_3 ++ ;
	ew->timer0 = 15 ;
}

/*--------------------------------------------------------------*/
static void set02_03_turn_move( PLAYER_WORK *ew )
{
	ew->rot[1] += ew->timer1 ? 0x889 : -0x889 ;
	if( ew->timer0 ){
		ew->timer0 -- ;
	}
	else {
		ew->disp_dir ^= 1 ;
		ew->routine_3 = 0 ;
		ew->routine_1 = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_look( PLAYER_WORK *ew )
{
	static void ( *set02_03_look_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_look_init, set02_03_look_in, set02_03_look_move, set02_03_look_out,
	} ;

	set02_03_look_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_look_init( PLAYER_WORK *ew )
{
	ew->spd.x = 0.0f ;
	ew->spd.y = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->timer1 = 0 ;
	ew->rot[1] = ew->disp_dir ? 0 : 0x8000 ;
	ew->routine_3 ++ ;
	ew->timer0 = 10 ;
}

/*--------------------------------------------------------------*/
static void set02_03_look_in( PLAYER_WORK *ew )
{
	if( ew->timer0 ){
		ew->timer0 -- ;
		ew->rot[1] += ew->timer1 ? 0x444 : -0x444 ;
	}
	else {
		ew->routine_3 ++ ;
		ew->timer0 = 90 ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_look_move( PLAYER_WORK *ew )
{
	if( ew->timer0 % 8 == 0 ) ew->type ^= 1 ;
	if( ew->disp_dir ){
		ew->rot[1] = ew->timer1 ? 0x4000 : -0x4000 ;
	}
	else {
		ew->rot[1] = ew->timer1 ? -0x4000 : 0x4000 ;
	}
	if( !--ew->timer0 ){
		ew->routine_3 ++ ;
		ew->timer0 = 10 ;
		return ;
	}
	ew->rot[1] += ew->type ? 0x16c : -0x16c ;
}

/*--------------------------------------------------------------*/
static void set02_03_look_out( PLAYER_WORK *ew )
{
	if( !--ew->timer0 ){
		ew->routine_1 = ew->routine_3 = 0 ;
		return ;
	}
	ew->rot[1] += ew->timer1 ? -0x444 : 0x444 ;
}

/*==============================================================*/
static void set02_03_hop( PLAYER_WORK *ew )
{
	static void ( *set02_03_hop_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_hop_init, set02_03_hop_move,
	} ;

	set02_03_hop_jmp[ew->routine_3]( ew ) ;
}

/*==============================================================*/
static void set02_03_hop_init( PLAYER_WORK *ew )
{
	ew->routine_3 ++ ;
	ew->spd.y = 30.0f ;
	ew->acc.y = -1.0f ;
}

/*--------------------------------------------------------------*/
static void set02_03_hop_move( PLAYER_WORK *ew )
{
	const VEC3 *home = set02_03_pos ;

	home += ew->sub_no ;

	ew->spd.y += ew->acc.y ;
	ew->pos.y = ew->spd.y ;
	if( ew->pos.y <= home->y ){
		ew->spd.y = 0.0f ;
		ew->acc.y = 0.0f ;
		ew->pos.y = home->y ;
		ew->routine_3 = 0 ;
		ew->routine_1 = 0 ;
	}
}

/*==============================================================*/
/*			A player stands close by							*/
/*==============================================================*/
static Uint8 set02_03_near_ck( PLAYER_WORK *ew )
{
	if( !ew->routine_0 ){
		PLAYER_WORK *pl0_w = vs_sys_work.team_w[0][0] ;
		PLAYER_WORK *pl1_w = vs_sys_work.team_w[1][0] ;

		if( fabs( pl0_w->pos.x - ew->pos.x ) > 50.0f
		 && fabs( pl1_w->pos.x - ew->pos.x ) > 50.0f ){
			return 0 ;
		}
		ew->routine_0 = 1 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static void set02_03_greet( PLAYER_WORK *ew )
{
	static void ( *set02_03_greet_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_greet_next, set02_03_greet_act,
	} ;

	set02_03_greet_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void set02_03_greet_next( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->routine_2 ++ ;
	if( ew->routine_2 == 1 ){
		ew->routine_2 = ( int )Rnd() % 2 ? 1 : 2 ;
	}
	if( ew->routine_2 >= 4 ){
		ew->routine_0 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		set02_03_main( ew ) ;
		return ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_greet_act( PLAYER_WORK *ew )
{
	static void ( *set02_03_greet_act_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_hop, set02_03_turn, set02_03_run, set02_03_walk,
	} ;

	set02_03_greet_act_jmp[ew->routine_2]( ew ) ;
}

/*--------------------------------------------------------------*/
static void set02_03_cheer( PLAYER_WORK *ew )
{
	static void ( *set02_03_cheer_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_cheer_next, set02_03_cheer_act,
	} ;

	set02_03_cheer_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void set02_03_cheer_next( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->routine_2 ++ ;
	if( ew->routine_2 >= 3 ){
		ew->routine_0 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		set02_03_main( ew ) ;
		return ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_cheer_act( PLAYER_WORK *ew )
{
	static void ( *set02_03_cheer_act_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_hop, set02_03_hop, set02_03_hop,
	} ;

	set02_03_cheer_act_jmp[ew->routine_2]( ew ) ;
}

/*==============================================================*/
/*			A side has joined									*/
/*==============================================================*/
static Uint8 set02_03_side_ck( PLAYER_WORK *ew )
{
	if( !ew->routine_0 && side_flag_get( 0 ) ){
		ew->routine_0 = 2 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*--------------------------------------------------------------*/
static void set02_03_end( PLAYER_WORK *ew )
{
	static void ( *set02_03_end_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_end_next, set02_03_end_act,
	} ;

	set02_03_end_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void set02_03_end_next( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->routine_2 ++ ;
	if( ew->routine_2 >= 10 ){
		ew->routine_0 = 0 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		set02_03_main( ew ) ;
		return ;
	}
}

/*==============================================================*/
/*			Hops and turns, and lets others fly off				*/
/*==============================================================*/
static void set02_03_end_act( PLAYER_WORK *ew )
{
	static void ( *set02_03_end_act_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_hop, set02_03_turn, set02_03_hop, set02_03_turn, set02_03_hop,
		set02_03_turn, set02_03_hop, set02_03_turn, set02_03_hop, set02_03_turn,
	} ;

	set02_03_end_act_jmp[ew->routine_2]( ew ) ;
	if( !ew->sub_no && ew->sub_type <= 12 ){
		if( !ew->dir ){
			ew->sub_type ++ ;
			set02_03_fly_make( ew ) ;
			ew->dir = 16 ;
		}
		else {
			ew->dir -- ;
		}
	}
}

/*==============================================================*/
/*			The fight is over									*/
/*==============================================================*/
static Uint8 set02_03_end_ck( PLAYER_WORK *ew )
{
	if( ew->routine_0 ) return 0 ;
	if( vs_sys_work.vs_st >= VS_WIN ){
		ew->routine_0 = 3 ;
		ew->routine_1 = 0 ;
		ew->routine_2 = 0 ;
		ew->routine_3 = 0 ;
		return 1 ;
	}
	return 0 ;
}

/*==============================================================*/
static void set02_03_fly_make( PLAYER_WORK *ew )
{
	PLAYER_WORK *fw ;
	int i ;

	for( i = 0 ; i < 3 ; i ++ ){
		if( !( fw = work_get( 0, W_SET, LIST_END ) ) ) return ;
		fw->sub_no = i ;
		fw->disp_flag = 1 ;
		fw->work_func = set02_03_fly_main ;
		fw->obj = obj_stg->obj[35] ;
		fw->obj_flag = OBJ_POS | OBJ_BRIGHT ;
		fw->disp_dir = 1 ;
		fw->pos = set02_03_pos[fw->sub_no] ;
		set02_03_fly_wing_set( fw ) ;
	}
}

static void set02_03_fly_init( PLAYER_WORK *ew ) ;
static void set02_03_fly_move( PLAYER_WORK *ew ) ;
static void set02_03_fly_free( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void set02_03_fly_main( PLAYER_WORK *ew )
{
	static void ( *set02_03_fly_jmp[] )( PLAYER_WORK *ew ) = {
		set02_03_fly_init, set02_03_fly_move, set02_03_fly_free,
	} ;

	set02_03_fly_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void set02_03_fly_init( PLAYER_WORK *ew )
{
	ew->pos.x = scr_work.stage_right + ( int )Rnd() % 150 ;
	ew->routine_0 ++ ;
	ew->spd.x = -5.0f ;
	ew->spd.x -= ( int )Rnd() % 10 ;
}

/*--------------------------------------------------------------*/
static void set02_03_fly_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	if( ew->pos.x <= scr_work.stage_left ){
		ew->disp_flag = 0 ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void set02_03_fly_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void set02_03_make( int no )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 3 ; i ++ ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
		ew->sub_no = i ;
		ew->disp_flag = 1 ;
		ew->work_func = set02_03_main ;
		ew->obj = obj_stg->obj[35] ;
		ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_BRIGHT ;
		ew->disp_dir = 1 ;
		ew->pos = set02_03_pos[ew->sub_no] ;
		set02_03_wing_set( ew ) ;
	}
}

/****************************************************************/
void set02_03_set( void )
{
	set02_03_make( 35 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "set02_03.c" End of File	*/
