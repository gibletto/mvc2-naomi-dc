/* The computer's script commands: walks, jumps, attacks, guards and throws, the tests that skip
   ahead to an endif, and the jumps between scripts (em_play.c is the
   computer's control before it). */
#include "player.h"
#include "pl_act_old.h"

extern Uint8 pl_guard_ok( PLAYER_WORK *pl_w );

typedef int ( *CMD_FUNC )( PLAYER_WORK *pl_w, CPU_SCRIPT *sc );

/* the part of a character's own work (tlocal) that command 114 tests */
typedef struct EM_TLOCAL_W {
	Uint8	unk_00[0x0e];
	Uint8	unk_0e;		/* 0e: a flag the character keeps for its script */
} EM_TLOCAL_W;

/* each command's length in the script, with its parameters */
static Uint8 cmd_size_tbl[128] = {
	1, 3, 3, 1, 2, 2, 1, 1, 1, 1, 1, 2, 2, 2, 3, 2,
	2, 6, 3, 3, 3, 3, 1, 1, 1, 2, 2, 2, 3, 1, 5, 1,
	9, 8, 1, 1, 1, 2, 2, 17, 2, 1, 2, 3, 3, 1, 1, 2,
	1, 3, 3, 3, 3, 3, 3, 3, 1, 3, 3, 1, 2, 1, 1, 4,
	4, 4, 5, 1, 1, 1, 2, 2, 1, 1, 2, 2, 1, 1, 1, 1,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	9, 2, 1, 9, 1, 1, 1, 2, 2, 1, 2, 9, 1, 2, 1, 2,
	2, 4, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 1, 2, 1,
} ;

static CMD_FUNC cmd_start_jmp[128] ;
static CMD_FUNC cmd_move_jmp[128] ;

/****************************************************************/
/*			Reads the next command and starts it				*/
/****************************************************************/
int cmd_start( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	register Uint8 *p = sc->adrs + sc->ofs ;

	return cmd_start_jmp[*p]( pl_w, sc ) ;
}

/****************************************************************/
/*			Runs the command under way							*/
/****************************************************************/
int cmd_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	return cmd_move_jmp[pl_w->cpu_cmd]( pl_w, sc ) ;
}

/*==============================================================*/
static int cmd_nop( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return 0 ;
}

/*==============================================================*/
static int cmd_end( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	pl_w->cpu_routine_2 -- ;
	return 1 ;
}

static int walk_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_b_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_em_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_em2_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_pos_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_pos_y_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_dist_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;
static int walk_back_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;

/*==============================================================*/
/*			01: walks forward a distance						*/
/*==============================================================*/
static int walk_f( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	pl_w->cpu_work1.f = pl_w->pos.x ;
	pl_w->cpu_work2.l = 1 ;
	pl_w->cpu_work3.l = pl_w->pl_dir * 0xc00 ^ 0x800 ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->jump_dir = 0 ;
	pl_w->pad_sw.now = pl_w->cpu_work3.l ;
	pl_w->pl_sw_new = KEY_FORWARD ;
	pl_act_set( pl_w, N_WALK ) ;
	pl_w->pl_sw_new = 0 ;
	return walk_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Float d ;
	int wall ;

	if( pl_w->act_st != pl_w->cpu_work2.l ) goto end ;
	if( vs_sys_work.push_bit ) goto end ;
	wall = pl_w->wall_flag ;
	if( wall && wall & 2 >> pl_w->pl_dir ) goto end ;
	d = pl_w->pos.x - pl_w->cpu_work1.f ;
	if( d < 0.0f ) d = -d ;
	if( ( pl_w->cpu_work0.f -= d ) > 0.0f ){
		pl_w->cpu_work1.f = pl_w->pos.x ;
		pl_w->pad_sw.now = pl_w->cpu_work3.l ;
		if( pl_w->cpu_work2.l == 26 ) pl_w->atck_lever = pl_w->cpu_work3.l ;
		pl_w->jump_dir = 0 ;
		return 0 ;
	}
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			02: walks back a distance							*/
/*==============================================================*/
static int walk_b( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Sint8 wall ;

	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	if( em_atck_near( pl_w ) ) return 0 ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	pl_w->cpu_work1.f = pl_w->pos.x ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->jump_dir = 1 ;
	wall = pl_w->wall_flag ;
	if( wall && wall & 1 << pl_w->pl_dir ) return 0 ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work2.l = 1 ;
	pl_w->cpu_work3.l = pl_w->pl_dir * 0xc00 ^ 0x400 ;
	pl_w->pad_sw.now = pl_w->cpu_work3.l ;
	pl_w->pl_sw_new = KEY_BACK ;
	pl_act_set( pl_w, N_WALK ) ;
	pl_w->pl_sw_new = 0 ;
	return walk_b_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_b_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( em_atck_near( pl_w ) ){
		pl_w->cpu_routine_2 -- ;
		pl_w->pad_sw.now = 0 ;
		return 1 ;
	}
	return walk_back_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			14: walks up to a distance from the opponent		*/
/*==============================================================*/
static int walk_em( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x, a, b ;

	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	em = pl_w->target_w ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f + DOT_X( em->size_front ) ;
	pl_w->jump_dir = 0 ;
	pl_w->pos.y = pl_w->ground_y ;
	if( vs_sys_work.push_bit ) return 0 ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	b = x + em->pos.x ;
	a = pl_w->pos.x ;
	if( pl_w->pl_dir ){
		a = b ;
		b = pl_w->pos.x ;
	}
	if( b > a ) return 0 ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work2.l = 1 ;
	pl_w->cpu_work3.l = pl_w->pl_dir * 0xc00 ^ 0x800 ;
	pl_w->pad_sw.now = pl_w->cpu_work3.l ;
	pl_w->pl_sw_new = KEY_FORWARD ;
	pl_act_set( pl_w, N_WALK ) ;
	pl_w->pl_sw_new = 0 ;
	return walk_em_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_em_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x, a, b ;
	int wall ;

	if( pl_w->act_st != pl_w->cpu_work2.l ) goto end ;
	if( !--pl_w->cpu_walk_timer ) goto end ;
	if( vs_sys_work.push_bit ) goto end ;
	wall = pl_w->wall_flag ;
	if( wall && wall & 2 >> pl_w->pl_dir ) goto end ;
	em = pl_w->target_w ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	b = x + em->pos.x ;
	a = pl_w->pos.x ;
	if( pl_w->pl_dir ){
		a = b ;
		b = pl_w->pos.x ;
	}
	if( a - b > 0.0f ){
		pl_w->pad_sw.now = pl_w->cpu_work3.l ;
		pl_w->jump_dir = 0 ;
		return 0 ;
	}
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			63: walks up to a distance from the opponent,		*/
/*			holding the stick up or down too					*/
/*==============================================================*/
static int walk_em2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Uint16 sw ;

	if( !cmd_act_ck( pl_w, 6 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	em = pl_w->target_w ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f + DOT_X( em->size_front ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work2.l = pl_w->cpu_prm.l ;
	sw = pl_w->pl_dir * 0xc00 ^ 0x800 ;
	if( pl_w->cpu_work2.l < 0 ) sw |= 0x1000 ;
	if( pl_w->cpu_work2.l > 0 ) sw |= 0x2000 ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work1.l = pl_w->act_st ;
	pl_w->cpu_work2.l = sw ;
	pl_w->pad_sw.now = sw ;
	pl_w->cpu_walk_timer = 180 ;
	return walk_em2_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_em2_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x, a, b ;
	int wall ;

	if( pl_w->act_st != pl_w->cpu_work1.l ) goto end ;
	if( !--pl_w->cpu_walk_timer ) goto end ;
	if( vs_sys_work.push_bit ) goto end ;
	wall = pl_w->wall_flag ;
	if( wall && wall & 2 >> pl_w->pl_dir ) goto end ;
	em = pl_w->target_w ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	b = x + em->pos.x ;
	a = pl_w->pos.x ;
	if( pl_w->pl_dir ){
		a = b ;
		b = pl_w->pos.x ;
	}
	if( a - b > 0.0f ){
		pl_w->pad_sw.now = pl_w->cpu_work2.l ;
		return 0 ;
	}
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			64: walks to a place beside the opponent, from		*/
/*			either side											*/
/*==============================================================*/
static int walk_pos( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x ;
	Uint16 sw ;

	if( !cmd_act_ck( pl_w, 6 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	em = pl_w->target_w ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f + DOT_X( em->size_front ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work2.l = pl_w->cpu_prm.l ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	if( pl_w->pos.x > x + em->pos.x ){
		pl_w->cpu_work3.l = 0 ;
		sw = 0x800 ;
	} else {
		pl_w->cpu_work3.l = 1 ;
		sw = 0x400 ;
	}
	if( pl_w->cpu_work2.l < 0 ) sw |= 0x1000 ;
	if( pl_w->cpu_work2.l > 0 ) sw |= 0x2000 ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work1.l = pl_w->act_st ;
	pl_w->cpu_work2.l = sw ;
	pl_w->pad_sw.now = sw ;
	pl_w->cpu_walk_timer = 180 ;
	return walk_pos_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_pos_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x, a, b ;
	int wall ;
	int side ;

	if( pl_w->act_st != pl_w->cpu_work1.l ) goto end ;
	if( !--pl_w->cpu_walk_timer ) goto end ;
	if( vs_sys_work.push_bit ) goto end ;
	wall = pl_w->wall_flag ;
	side = 2 >> pl_w->pl_dir ;
	if( pl_w->cpu_work2.l & 0x400 ) side ^= 3 ;
	if( wall && wall & side ) goto end ;
	em = pl_w->target_w ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	b = x + em->pos.x ;
	a = pl_w->pos.x ;
	if( pl_w->cpu_work3.l ){
		a = b ;
		b = pl_w->pos.x ;
	}
	if( a - b > 0.0f ){
		pl_w->pad_sw.now = pl_w->cpu_work2.l ;
		return 0 ;
	}
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			66: walks to a place beside the opponent and at		*/
/*			a height above it, for a flier						*/
/*==============================================================*/
static int walk_pos_y( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x ;
	Uint16 sw ;

	if( !cmd_act_ck( pl_w, 6 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	em = pl_w->target_w ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f + DOT_X( em->size_front ) ;
	cmd_read( pl_w, sc, 4 ) ;
	pl_w->cpu_work1.f = pl_w->cpu_prm.f ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	if( pl_w->pos.x > x + em->pos.x ){
		pl_w->cpu_work3.l = 0 ;
		sw = 0x800 ;
	} else {
		pl_w->cpu_work3.l = 1 ;
		sw = 0x400 ;
	}
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_walk_act = pl_w->act_st ;
	pl_w->cpu_work2.l = sw ;
	pl_w->pad_sw.now = sw ;
	pl_w->cpu_walk_timer = 180 ;
	return walk_pos_y_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_pos_y_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x, a, b, y ;
	int wall ;
	int side ;
	Uint16 sw ;

	if( pl_w->act_st != pl_w->cpu_walk_act ) goto end ;
	if( !--pl_w->cpu_walk_timer ) goto end ;
	if( vs_sys_work.push_bit ) goto end ;
	wall = pl_w->wall_flag ;
	side = 2 >> pl_w->pl_dir ;
	if( pl_w->cpu_work2.l & 0x400 ) side = side ^ 3 ;
	if( wall && wall & side ) goto end ;
	em = pl_w->target_w ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	x = x + em->pos.x ;
	a = pl_w->pos.x ;
	b = x ;
	if( pl_w->cpu_work3.l ){
		a = b ;
		b = pl_w->pos.x ;
	}
	a -= b ;
	if( a < 0.0f ) goto end ;
	sw = 0 ;
	y = em->pos.y - pl_w->cpu_work1.f - pl_w->pos.y ;
	if( y < -DOT_Y( 24.0f ) ) sw = 0x2000 ;
	if( y > DOT_Y( 24.0f ) ) sw = 0x1000 ;
	pl_w->pad_sw.now = pl_w->cpu_work2.l | sw ;
	return 0 ;
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			38: walks to a distance from the opponent picked	*/
/*			at random from a table								*/
/*==============================================================*/
static int walk_rnd( register PLAYER_WORK *pl_w, register CPU_SCRIPT *sc )
{
	/* a row of 32 distances for each parameter */
	static Sint16 dist_tbl[5 * 32] = {
		0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50,
		0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x40, 0x40, 0x50, 0x50, 0x50, 0x50, 0x60, 0x60,
		0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x50, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60,
		0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x50, 0x50, 0x60, 0x60, 0x60, 0x60, 0x70, 0x70,
		0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0, 0xd0,
		0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xc0, 0xc0, 0xd0, 0xd0, 0xd0, 0xd0, 0xe0, 0xe0,
		0x110, 0x110, 0x110, 0x110, 0x110, 0x110, 0x110, 0x110, 0x120, 0x120, 0x120, 0x120, 0x120, 0x120, 0x120, 0x120,
		0x130, 0x130, 0x130, 0x130, 0x130, 0x130, 0x130, 0x130, 0x110, 0x110, 0x120, 0x120, 0x120, 0x120, 0x130, 0x130,
		0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70, 0x70,
		0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0xe0, 0x130, 0x130, 0x130, 0x130, 0x130, 0x130, 0x130, 0x130,
	} ;
	register Sint16 *tbl ;

	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	tbl = &dist_tbl[pl_w->cpu_prm.l * 32] ;
	pl_w->cpu_work0.f = DOT_X( tbl[Rnd() & 31] ) ;
	return walk_dist_set( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_dist_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Float x, a, b ;
	int wall ;
	int side ;

	if( pl_w->act_st != pl_w->cpu_work2.l ) goto end ;
	if( !--pl_w->cpu_walk_timer ) goto end ;
	if( vs_sys_work.push_bit ) goto end ;
	wall = pl_w->wall_flag ;
	side = 2 >> pl_w->pl_dir ;
	if( pl_w->jump_dir ) side ^= 3 ;
	if( wall && wall & side ) goto end ;
	em = pl_w->target_w ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	b = x + em->pos.x ;
	a = pl_w->pos.x ;
	if( pl_w->cpu_work3.l ){
		a = b ;
		b = pl_w->pos.x ;
	}
	if( a - b > 0.0f ){
		pl_w->jump_dir = ( ( pl_w->pl_dir * 0xc00 ^ pl_w->cpu_work1.l ) & 0x400 ) >> 10 ;
		pl_w->pad_sw.now = pl_w->cpu_work1.l ;
		return 0 ;
	}
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			49: walks to a distance from the opponent			*/
/*==============================================================*/
static int walk_dist( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	return walk_dist_set( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_dist_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em = pl_w->target_w ;
	Float x ;

	pl_w->cpu_work0.f += DOT_X( em->size_front ) ;
	x = pl_w->cpu_work0.f ;
	if( !em->pl_dir ) x = -x ;
	x += em->pos.x ;
	if( pl_w->pos.x > x ){
		if( pl_w->cpu_cmd == 38 && pl_w->pl_dir ) return 0 ;
		pl_w->cpu_work1.l = 0x800 ;
		pl_w->cpu_work3.l = 0 ;
		pl_w->jump_dir = pl_w->pl_dir ;
	} else {
		if( pl_w->cpu_cmd == 38 && !pl_w->pl_dir ) return 0 ;
		pl_w->cpu_work1.l = 0x400 ;
		pl_w->cpu_work3.l = 1 ;
		pl_w->jump_dir = pl_w->pl_dir ^ 1 ;
	}
	pl_w->cpu_routine_2 ++ ;
	pl_w->pos.y = pl_w->ground_y ;
	pl_w->cpu_work2.l = 1 ;
	pl_w->pad_sw.now = pl_w->cpu_work1.l ;
	pl_w->cpu_walk_timer = 180 ;
	pl_w->pl_sw_new = pl_w->pad_sw.now ;
	pl_act_set( pl_w, N_WALK ) ;
	pl_w->pl_sw_new = 0 ;
	return walk_dist_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			18: dashes forward a distance						*/
/*==============================================================*/
static int dash_f( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	pl_w->cpu_work1.f = pl_w->pos.x ;
	pl_w->cpu_work2.l = 10 ;
	pl_w->cpu_work3.l = 0 ;
	pl_act_set( pl_w, N_DASH ) ;
	return walk_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			19: dashes back a distance							*/
/*==============================================================*/
static int dash_b( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	int wall ;

	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	if( em_atck_near( pl_w ) ) return 0 ;
	wall = pl_w->wall_flag ;
	if( wall && wall & 1 << pl_w->pl_dir ) return 0 ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	pl_w->cpu_work1.f = pl_w->pos.x ;
	pl_w->cpu_work2.l = 11 ;
	pl_w->cpu_work3.l = 0 ;
	pl_act_set( pl_w, N_BACK_DASH ) ;
	return walk_back_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_back_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Float d ;
	int wall ;

	if( pl_w->act_st != pl_w->cpu_work2.l ) goto end ;
	if( pl_w->cpu_cmd != 19 ){
		wall = pl_w->wall_flag ;
		if( wall && wall & 1 << pl_w->pl_dir ) goto end ;
	}
	d = pl_w->pos.x - pl_w->cpu_work1.f ;
	if( d < 0.0f ) d = -d ;
	if( ( pl_w->cpu_work0.f -= d ) > 0.0f ){
		pl_w->cpu_work1.f = pl_w->pos.x ;
		pl_w->pad_sw.now = pl_w->cpu_work3.l ;
		pl_w->jump_dir = 1 ;
		return 0 ;
	}
end:
	pl_w->cpu_routine_2 -- ;
	pl_w->pad_sw.now = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			20: dashes up to a distance from the opponent		*/
/*==============================================================*/
static int dash_em( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;

	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	em = pl_w->target_w ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f + DOT_X( em->size_front ) ;
	pl_w->cpu_work2.l = 10 ;
	pl_w->cpu_work3.l = 0 ;
	pl_w->cpu_walk_timer = 180 ;
	pl_act_set( pl_w, N_DASH ) ;
	return walk_em_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			65: flies a distance, the stick given				*/
/*==============================================================*/
static int fly( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 7 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->dir = pl_w->cpu_prm.l >> 2 ;
	pl_w->cpu_work3.l = pl_w->cpu_prm.l << 8 ;
	if( pl_w->cpu_work3.l & 0xc00 ) pl_w->cpu_work3.l = pl_w->cpu_work3.l ^ pl_w->pl_dir * 0xc00 ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	pl_w->cpu_work1.f = pl_w->pos.x ;
	pl_w->cpu_work2.l = 26 ;
	pl_w->pad_sw.now = pl_w->cpu_work3.l ;
	pl_w->atck_lever = pl_w->cpu_work3.l ;
	pl_act_set( pl_w, N_AIR_DASH ) ;
	return walk_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			50 to 55: read their distance and do nothing		*/
/*==============================================================*/
static int walk_f2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	return 0 ;
}

/*==============================================================*/
static int walk_b2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	return 0 ;
}

/*==============================================================*/
static int walk_f3( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	return 0 ;
}

/*==============================================================*/
static int walk_b3( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	return 0 ;
}

/*==============================================================*/
static int walk_f4( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	return 0 ;
}

/*==============================================================*/
static int walk_b4( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	return 0 ;
}

/*==============================================================*/
/*			06: crouches										*/
/*==============================================================*/
static int crouch( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	int act ;

	if( !cmd_start_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	act = pl_w->act_st ;
	if( act != N_CROUCHING && act != N_CROUCH ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->cpu_crouch_flag = 1 ;
	}
	return 1 ;
}

/*==============================================================*/
/*			07: stands up										*/
/*==============================================================*/
static int stand( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	int act ;

	if( !cmd_start_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	act = pl_w->act_st ;
	if( act && act != N_STAND_UP ){
		pl_w->pos.y = pl_w->ground_y ;
		pl_w->cpu_crouch_flag = 0 ;
	}
	return 1 ;
}

/*==============================================================*/
/*			13: jumps forward, straight up (0xff) or back		*/
/*			(1)													*/
/*==============================================================*/
static int jump( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	switch( pl_w->cpu_prm.l ){
	case 0 :
		sw = 0x2800 ;
		break ;
	case 0xff :
		sw = 0x2000 ;
		break ;
	case 1 :
		sw = 0x2400 ;
		break ;
	}
	pl_w->jump_dir = pl_w->cpu_prm.l ;
	pl_w->pl_sw_new = sw ;
	pl_act_set( pl_w, N_JUMP ) ;
	pl_w->pl_sw_new = 0 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			16: super jumps forward, straight up (0xff) or		*/
/*			back (1)											*/
/*==============================================================*/
static int sjump( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_act_ck( pl_w, 5 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	switch( pl_w->cpu_prm.l ){
	case 0 :
		sw = 0x2800 ;
		break ;
	case 0xff :
		sw = 0x2000 ;
		break ;
	case 1 :
		sw = 0x2400 ;
		break ;
	}
	pl_w->jump_dir = pl_w->cpu_prm.l ;
	pl_w->pl_sw_new = sw ;
	pl_act_set( pl_w, N_SJUMP ) ;
	pl_w->pl_sw_new = 0 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			27: the same, when a command may start				*/
/*==============================================================*/
static int sjump2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_start_ck( pl_w, 5 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	switch( pl_w->cpu_prm.l ){
	case 0 :
		sw = 0x2800 ;
		break ;
	case 0xff :
		sw = 0x2000 ;
		break ;
	case 1 :
		sw = 0x2400 ;
		break ;
	}
	pl_w->jump_dir = pl_w->cpu_prm.l ;
	pl_w->pl_sw_new = sw ;
	pl_act_set( pl_w, N_SJUMP ) ;
	pl_w->pl_sw_new = 0 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			70: the same, as a cancel							*/
/*==============================================================*/
static int sjump3( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	pl_w->cpu_cancel_flag |= CC_SKIP ;
	if( !cmd_start_ck( pl_w, 0 ) || !cmd_flag_ck( pl_w, sc ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	switch( pl_w->cpu_prm.l ){
	case 0 :
		sw = 0x2800 ;
		break ;
	case 0xff :
		sw = 0x2000 ;
		break ;
	case 1 :
		sw = 0x2400 ;
		break ;
	}
	pl_w->jump_dir = pl_w->cpu_prm.l ;
	pl_w->pl_sw_new = sw ;
	pl_act_set( pl_w, N_SJUMP ) ;
	pl_w->pl_sw_new = 0 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			21: lets it make an air jump						*/
/*==============================================================*/
static int air_jump( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( cmd_act_ck( pl_w, 4 ) ){
		cmd_read( pl_w, sc, 0 ) ;
		cmd_read( pl_w, sc, 1 ) ;
		pl_w->jump_dir = pl_w->cpu_prm.l ;
		pl_w->cpu_air_jump_flag |= 2 ;
	}
	return 0 ;
}

static int wait_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;

/*==============================================================*/
/*			04: waits a number of frames						*/
/*==============================================================*/
static int wait( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	return wait_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			05: waits a number of frames worked out from it		*/
/*==============================================================*/
static int wait2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.l = em_rnd_get( pl_w, pl_w->cpu_prm.l ) ;
	return wait_move( pl_w, sc ) ;
}

/*==============================================================*/
static int wait_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !pl_w->hit_stop ){
		if( !pl_w->cpu_work0.l ){
			pl_w->cpu_routine_2 -- ;
			return 1 ;
		}
		pl_w->cpu_work0.l -- ;
	}
	return 0 ;
}

/*==============================================================*/
/*			28: waits a number of frames, in the air			*/
/*==============================================================*/
static int air_wait( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 5 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	return wait_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			25: sets the time an if waits, or one picked at		*/
/*			random												*/
/*==============================================================*/
static int timer_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	static Sint8 timer_tbl[4] = { 120, 90, 60, 30 } ;
	int timer ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	timer = pl_w->cpu_prm.l ;
	if( !timer ) timer = timer_tbl[Rnd() & 3] ;
	pl_w->cpu_cmd_timer = timer ;
	return 1 ;
}

/*==============================================================*/
/*			40: the same for cpu_guard_timer							*/
/*==============================================================*/
static int timer2_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	static Sint8 timer_tbl[4] = { 120, 90, 60, 30 } ;
	int timer ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	timer = pl_w->cpu_prm.l ;
	if( !timer ) timer = timer_tbl[Rnd() & 3] ;
	pl_w->cpu_guard_timer = timer ;
	return 1 ;
}

static int if_ck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc, int ok ) ;

/*==============================================================*/
/*			96: if the opponent is within a box around a		*/
/*			point: x and its half width, y and its half			*/
/*			height												*/
/*==============================================================*/
static int if_dist( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Float lo, hi, t ;

	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_work0.f = pl_w->cpu_prm.f ;
	cmd_read( pl_w, sc, 4 ) ;
	pl_w->cpu_work1.f = pl_w->cpu_prm.f ;
	cmd_read( pl_w, sc, 3 ) ;
	pl_w->cpu_work2.f = pl_w->cpu_prm.f ;
	cmd_read( pl_w, sc, 4 ) ;
	pl_w->cpu_work3.f = pl_w->cpu_prm.f ;
	if( pl_w->cpu_work0.f != 0.0f ){
		lo = pl_w->cpu_work2.f - pl_w->cpu_work0.f ;
		hi = pl_w->cpu_work2.f + pl_w->cpu_work0.f ;
		if( lo > hi ){
			t = lo ;
			lo = hi ;
			hi = t ;
		}
		if( lo > pl_w->em_dist_x || pl_w->em_dist_x > hi ) return if_ck( pl_w, sc, 0 ) ;
	}
	if( pl_w->cpu_work1.f != 0.0f ){
		lo = pl_w->cpu_work3.f - pl_w->cpu_work1.f ;
		hi = pl_w->cpu_work3.f + pl_w->cpu_work1.f ;
		if( lo > hi ){
			t = lo ;
			lo = hi ;
			hi = t ;
		}
		if( lo > pl_w->em_dist_y || pl_w->em_dist_y > hi ) return if_ck( pl_w, sc, 0 ) ;
	}
	return if_ck( pl_w, sc, 1 ) ;
}

/*==============================================================*/
/*			97: if an attack of the opponent's is of a kind		*/
/*==============================================================*/
static int if_em_atck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;

	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	em = pl_w->target_w ;
	return if_ck( pl_w, sc, em_atck_hit_ck( pl_w, em, pl_w->cpu_work0.l ) ) ;
}

/*==============================================================*/
/*			110: if an attack is coming that is of a kind		*/
/*==============================================================*/
static int if_atck_come( register PLAYER_WORK *pl_w, register CPU_SCRIPT *sc )
{
	PLAYER_WORK *at_w ;
	int ok ;

	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	if( !( at_w = em_atck_near( pl_w ) ) ) ok = 0 ;
	else ok = em_atck_hit_ck( pl_w, at_w, at_w->atck_no & ATCK_NO_MASK ) ;
	return if_ck( pl_w, sc, ok ) ;
}

/*==============================================================*/
/*			99: if the opponent is within an area				*/
/*==============================================================*/
static int if_area( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work1.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work2.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work3.l = pl_w->cpu_prm.l ;
	return if_ck( pl_w, sc, em_area_ck( pl_w, &pl_w->cpu_work0 ) ) ;
}

/*==============================================================*/
/*			107: the same with the other test					*/
/*==============================================================*/
static int if_area2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work1.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work2.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 2 ) ;
	pl_w->cpu_work3.l = pl_w->cpu_prm.l ;
	return if_ck( pl_w, sc, em_atck_area_ck( pl_w, &pl_w->cpu_work0 ) ) ;
}

/*==============================================================*/
/*			113: if it is rising above a height, or falling		*/
/*			below one											*/
/*==============================================================*/
static int if_height( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Sint8 dir ;

	if( !cmd_act_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	dir = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 4 ) ;
	if( pl_w->cpu_prm.f > pl_w->pos.y - pl_w->ground_y ){
		if( dir >= 0 || pl_w->spd.y > 0.0f ) return if_ck( pl_w, sc, 0 ) ;
	} else {
		if( dir < 0 || pl_w->spd.y < 0.0f ) return if_ck( pl_w, sc, 0 ) ;
	}
	return if_ck( pl_w, sc, 1 ) ;
}

/*==============================================================*/
/*			114: if the character's own work has its flag 14	*/
/*			clear												*/
/*==============================================================*/
static int if_tlocal( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	EM_TLOCAL_W *lw = ( EM_TLOCAL_W * )&pl_w->tlocal[0] ;

	if( !cmd_start_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	return if_ck( pl_w, sc, lw->unk_0e == 0 ) ;
}

/*==============================================================*/
/*			105: if a throw would catch the opponent, with a	*/
/*			punch or a kick										*/
/*==============================================================*/
static int if_catch( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	int ok ;

	if( !cmd_act_ck( pl_w, 2 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	ok = 1 ;
	pl_w->atck_pk = 0 ;
	pl_w->atck_level = 1 ;
	pl_w->atck_lever = KEY_FORWARD ;
	if( !pl_tbl_catch_ck( pl_w ) ){
		pl_w->atck_pk = 1 ;
		ok = 0 ;
		if( pl_tbl_catch_ck( pl_w ) ) ok = 1 ;
	}
	pl_w->atck_pk = 0 ;
	pl_w->atck_level = 0 ;
	pl_w->atck_lever = 0 ;
	return if_ck( pl_w, sc, ok ) ;
}

/*==============================================================*/
/*			106: if the computer plays at a level or above		*/
/*==============================================================*/
static int if_level( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	return if_ck( pl_w, sc, pl_w->cpu_work0.l <= cpu_level_work.level ) ;
}

/*==============================================================*/
/*			109: at random, the parameter in 32					*/
/*==============================================================*/
static int if_rnd( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	return if_ck( pl_w, sc, pl_w->cpu_prm.l >= ( Rnd() & 31 ) ) ;
}

static int walk_atck_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc ) ;

/*==============================================================*/
/*			103: walks forward until an attack would reach		*/
/*==============================================================*/
static int walk_atck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	if( vs_sys_work.push_bit ) return 0 ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	pl_w->cpu_work2.l = 1 ;
	pl_w->cpu_work3.l = pl_w->pl_dir * 0xc00 ^ 0x800 ;
	pl_w->cpu_walk_timer = 180 ;
	pl_w->pad_sw.now = pl_w->cpu_work3.l ;
	pl_w->pl_sw_new = KEY_FORWARD ;
	pl_act_set( pl_w, N_WALK ) ;
	pl_w->pl_sw_new = 0 ;
	return walk_atck_move( pl_w, sc ) ;
}

/*==============================================================*/
static int walk_atck_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	int wall ;

	if( pl_w->act_st != pl_w->cpu_work2.l ) return goto_endif( pl_w, sc ) ;
	if( !--pl_w->cpu_walk_timer ) return goto_endif( pl_w, sc ) ;
	if( vs_sys_work.push_bit ) return goto_endif( pl_w, sc ) ;
	wall = pl_w->wall_flag ;
	if( wall && wall & 2 >> pl_w->pl_dir ) return goto_endif( pl_w, sc ) ;
	if( em_atck_hit_ck( pl_w, pl_w->target_w, pl_w->cpu_work0.l ) ) return if_ck( pl_w, sc, 1 ) ;
	pl_w->pad_sw.now = pl_w->cpu_work3.l ;
	pl_w->jump_dir = 0 ;
	return 0 ;
}

/*==============================================================*/
/*			104: dashes forward until an attack would reach		*/
/*==============================================================*/
static int dash_atck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 0 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	pl_w->cpu_work2.l = 10 ;
	pl_w->cpu_work3.l = 0 ;
	pl_w->cpu_walk_timer = 180 ;
	pl_act_set( pl_w, N_DASH ) ;
	return walk_atck_move( pl_w, sc ) ;
}

/*==============================================================*/
/*			98: the next command waits for the one before		*/
/*==============================================================*/
static int cancel_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 2 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_cancel_flag = CC_ON | CC_GO ;
	return 1 ;
}

/*==============================================================*/
/*			100: the same, and skips to the endif if it			*/
/*			can't												*/
/*==============================================================*/
static int cancel_set2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 2 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_cancel_flag = CC_ON | CC_SKIP | CC_GO ;
	return 1 ;
}

/*==============================================================*/
/*			111: if the hyper combo gauge is at a level or		*/
/*			above												*/
/*==============================================================*/
static int if_gauge( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_start_ck( pl_w, 3 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	return if_ck( pl_w, sc, pl_w->cpu_prm.l <= *pl_w->gauge_level ) ;
}

/*==============================================================*/
/*			112: if fight_sec is over the parameter				*/
/*==============================================================*/
static int if_fight_sec( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	return if_ck( pl_w, sc, pl_w->cpu_prm.l < pl_w->fight_sec ) ;
}

/*==============================================================*/
/*			115: if the count from 99 has gone past the			*/
/*			parameter											*/
/*==============================================================*/
static int if_time( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	return if_ck( pl_w, sc, pl_w->cpu_prm.l <= 99 - vs_sys_work.cpu_time ) ;
}

/*==============================================================*/
/*			101, 102, 108: the script skips to its endif			*/
/*			once the jump, super jump or flight is over			*/
/*==============================================================*/
static int jump_flag_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_jump_flag = 1 ;
	return 1 ;
}

/*==============================================================*/
static int sjump_flag_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_jump_flag = -128 ;
	return 1 ;
}

/*==============================================================*/
static int fly_flag_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !cmd_act_ck( pl_w, 4 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_fly_flag = 1 ;
	return 1 ;
}

/*==============================================================*/
/*			31, 34, 62: stop the waits							*/
/*==============================================================*/
static int cancel_clr( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_cancel_flag = 0 ;
	return 1 ;
}

/*==============================================================*/
static int jump_flag_clr( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_jump_flag = 0 ;
	return 1 ;
}

/*==============================================================*/
static int fly_flag_clr( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_fly_flag = 0 ;
	pl_w->fly_flag = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			An if's end: goes on if the test held, or tries		*/
/*			again until the timer runs out						*/
/*==============================================================*/
static int if_ck( PLAYER_WORK *pl_w, CPU_SCRIPT *sc, int ok )
{
	if( ok ){
		pl_w->cpu_routine_2 = 1 ;
		pl_w->cpu_cmd_timer = 0 ;
		return 1 ;
	}
	pl_w->cpu_routine_2 = 1 ;
	if( pl_w->cpu_cmd_timer == 0 ){
		pl_w->cpu_cmd_timer = 0 ;
		return goto_endif( pl_w, sc ) ;
	}
	pl_w->cpu_cmd_timer -- ;
	if( pl_w->hit_stop ) pl_w->cpu_cmd_timer ++ ;
	sc->cmd_cnt -- ;
	sc->ofs -= cmd_size_tbl[pl_w->cpu_cmd] ;
	return 0 ;
}

/*==============================================================*/
/*			23: else											*/
/*==============================================================*/
static int cmd_else( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return goto_endif( pl_w, sc ) ;
}

/*==============================================================*/
/*			24 and 73: endif									*/
/*==============================================================*/
static int cmd_endif( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return 1 ;
}

/*==============================================================*/
/*			03: starts the computer's mode over					*/
/*==============================================================*/
static int cmd_reset( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	em_cmd_sel( pl_w, 0, 0 ) ;
	return 0 ;
}

static int script_exit( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			08: leaves the script								*/
/*==============================================================*/
static int cmd_exit( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return script_exit( pl_w ) ;
}

/*==============================================================*/
static int script_exit( PLAYER_WORK *pl_w )
{
	pl_w->cpu_routine_1 = 0 ;
	pl_w->cpu_routine_2 = 1 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	pl_w->cpu_ev_lv = 0 ;
	em_cmd_clr( pl_w ) ;
	pl_w->em_atck_no = EM_ATCK_NONE ;
	return 2 ;
}

/*==============================================================*/
/*			15: goes to another script							*/
/*==============================================================*/
static int cmd_jump( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_ex_sc.no = pl_w->cpu_prm.l ;
	pl_w->cpu_ex_flag = 1 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			29: does nothing									*/
/*==============================================================*/
static int cmd_dummy( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return 1 ;
}

/*==============================================================*/
/*			30: goes to one of four scripts, at random			*/
/*==============================================================*/
static int jump_rnd( register PLAYER_WORK *pl_w, register CPU_SCRIPT *sc )
{
	register Uint32 rnd ;
	register Uint8 *p ;

	cmd_read( pl_w, sc, 0 ) ;
	p = sc->adrs + sc->ofs ;
	sc->ofs += 4 ;
	rnd = Rnd() & 3 ;
	pl_w->cpu_ex_sc.no = p[rnd] ;
	pl_w->cpu_ex_flag = 1 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			32: goes to one of eight scripts, by the			*/
/*			opponent											*/
/*==============================================================*/
static int jump_em( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *p ;
	PLAYER_WORK *em ;
	int no ;

	cmd_read( pl_w, sc, 0 ) ;
	p = sc->adrs + sc->ofs ;
	sc->ofs += 8 ;
	em = pl_w->target_w ;
	no = em->weight & 7 ;
	pl_w->cpu_ex_sc.no = p[no] ;
	pl_w->cpu_ex_flag = 1 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			33: goes to a script by the computer's level:		*/
/*			three levels and the scripts below them				*/
/*==============================================================*/
static int jump_level( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *p ;
	int i ;

	cmd_read( pl_w, sc, 0 ) ;
	p = sc->adrs + sc->ofs ;
	sc->ofs += 7 ;
	for( i = 0 ; i < 3 ; i ++, p += 2 ){
		if( p[1] > cpu_level_work.level ) break ;
	}
	pl_w->cpu_ex_sc.no = *p ;
	pl_w->cpu_ex_flag = 1 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			39: reads up to three scripts, each with a			*/
/*			distance, and goes to the first whose distance		*/
/*			is beyond the opponent								*/
/*==============================================================*/
static int jump_dist( register PLAYER_WORK *pl_w, register CPU_SCRIPT *sc )
{
	int i ;

	cmd_read( pl_w, sc, 0 ) ;
	for( i = 0 ; i < 3 ; i ++ ){
		cmd_read( pl_w, sc, 1 ) ;
		pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
		cmd_read( pl_w, sc, 3 ) ;
		if( pl_w->cpu_prm.f > pl_w->em_dist_x ){
			i ++ ;
			break ;
		}
	}
	sc->ofs += ( 3 - i ) * 3 + 1 ;
	pl_w->cpu_ex_sc.no = pl_w->cpu_work0.l ;
	pl_w->cpu_ex_flag = 1 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			22: goes back to the script it came from			*/
/*==============================================================*/
static int jump_ret( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_ex_flag = 0 ;
	pl_w->cpu_routine_2 = 1 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	return 1 ;
}

static int script_next( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			09 and 67: goes on to the next script, if there		*/
/*			is one												*/
/*==============================================================*/
static int cmd_next( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	if( em_rate_ck( pl_w, 0, 0 ) ) return script_next( pl_w ) ;
	return script_exit( pl_w ) ;
}

/*==============================================================*/
static int cmd_next2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	if( em_rate_ck( pl_w, 1, 1 ) ) return script_next( pl_w ) ;
	return script_exit( pl_w ) ;
}

/*==============================================================*/
/*			10: goes on to the next script						*/
/*==============================================================*/
static int cmd_next3( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return script_next( pl_w ) ;
}

/*==============================================================*/
static int script_next( PLAYER_WORK *pl_w )
{
	pl_w->cpu_routine_1 = 1 ;
	pl_w->cpu_routine_2 = 0 ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	pl_w->cpu_ev_lv = 1 ;
	em_cmd_clr( pl_w ) ;
	return 1 ;
}

/*==============================================================*/
/*			35 and 36: lock and unlock the computer's mode		*/
/*==============================================================*/
static int lock_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_lock_flag = 1 ;
	return 1 ;
}

/*==============================================================*/
static int lock_clr( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_lock_flag = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			11: presses a punch, heavy (1) or light (0), or		*/
/*			lets go of one held									*/
/*==============================================================*/
static int punch( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_start_ck( pl_w, 2 ) || !cmd_flag_ck( pl_w, sc ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	if( pl_w->cpu_prm.l ) sw = 0x100 ;
	else sw = 0x200 ;
	if( pl_w->cpu_hold_sw & sw ) pl_w->cpu_hold_sw ^= sw ;
	else pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			12: the same for a kick								*/
/*==============================================================*/
static int kick( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_start_ck( pl_w, 2 ) || !cmd_flag_ck( pl_w, sc ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	if( pl_w->cpu_prm.l ) sw = 0x20 ;
	else sw = 0x40 ;
	if( pl_w->cpu_hold_sw & sw ) pl_w->cpu_hold_sw = sw ;
	else pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			43: a throw, the stick turned by the facing,		*/
/*			with a heavy punch									*/
/*==============================================================*/
static int nag_vflip_adj( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( cmd_start_ck( pl_w, 2 ) && cmd_flag_ck( pl_w, sc ) ){
		cmd_read( pl_w, sc, 0 ) ;
		cmd_read( pl_w, sc, 1 ) ;
		cmd_read( pl_w, sc, 1 ) ;
		sw = pl_w->cpu_prm.l << 8 & 0x3c00 | 0x100 ;
		if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
		pl_w->pad_sw.now = sw ;
	}
	return 0 ;
}

/*==============================================================*/
/*			44: the same with a heavy kick						*/
/*==============================================================*/
static int nag_vflip_adj2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( cmd_start_ck( pl_w, 2 ) && cmd_flag_ck( pl_w, sc ) ){
		cmd_read( pl_w, sc, 0 ) ;
		cmd_read( pl_w, sc, 1 ) ;
		cmd_read( pl_w, sc, 1 ) ;
		sw = pl_w->cpu_prm.l << 8 & 0x3c00 | 0x20 ;
		if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
		pl_w->pad_sw.now = sw ;
	}
	return 0 ;
}

/*==============================================================*/
/*			57: the same with a punch, heavy (1) or light		*/
/*			(0)													*/
/*==============================================================*/
static int nag_vflip_adj3( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_start_ck( pl_w, 2 ) || !cmd_flag_ck( pl_w, sc ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	if( pl_w->cpu_prm.l ) sw = 0x100 ;
	else sw = 0x200 ;
	cmd_read( pl_w, sc, 1 ) ;
	sw |= pl_w->cpu_prm.l << 8 & 0x3c00 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = sw ;
	return 0 ;
}

/*==============================================================*/
/*			58: and with a kick									*/
/*==============================================================*/
static int nag_vflip_adj4( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	if( !cmd_start_ck( pl_w, 2 ) || !cmd_flag_ck( pl_w, sc ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	if( pl_w->cpu_prm.l ) sw = 0x20 ;
	else sw = 0x40 ;
	cmd_read( pl_w, sc, 1 ) ;
	sw |= pl_w->cpu_prm.l << 8 & 0x3c00 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = sw ;
	return 0 ;
}

static int sp_renda_none( PLAYER_WORK *pl_w ) ;
static int sp_renda_back( PLAYER_WORK *pl_w ) ;
static int sp_renda_down( PLAYER_WORK *pl_w ) ;
static int sp_renda_up( PLAYER_WORK *pl_w ) ;
static int sp_renda_up_back( PLAYER_WORK *pl_w ) ;

/*==============================================================*/
/*			17: does a special move: its number, the mashing	*/
/*			after it, its level, its button and stick			*/
/*==============================================================*/
static int sp_cmd( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	static int ( *renda_jmp[5] )( PLAYER_WORK *pl_w ) = {
		sp_renda_none, sp_renda_back, sp_renda_down, sp_renda_up, sp_renda_up_back,
	} ;
	Uint16 sw ;

	if( !cmd_start_ck( pl_w, 3 ) || !cmd_flag_ck( pl_w, sc ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_routine_3 = 0 ;
	pl_w->cpu_routine_4 = 0 ;
	pl_w->cpu_sp_flag = 1 ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_sp_no = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_atck_pk = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_atck_level = pl_w->cpu_prm.l ;
	cmd_read( pl_w, sc, 1 ) ;
	sw = pl_w->cpu_prm.l << 8 ;
	if( sw & 0xc00 ) sw ^= pl_w->pl_dir * 0xc00 ;
	pl_w->cpu_atck_lever = pl_w->cpu_prm.l << 8 ;
	pl_w->cpu_work1.l = sw ;
	pl_w->pad_sw.now = sw ;
	switch( pl_w->pl_type ){
	case PL_OMEGA_RED :
		pl_w->cpu_renda = renda_jmp[pl_w->cpu_work0.l] ;
		break ;
	default :
		pl_w->cpu_renda = sp_renda_none ;
		break ;
	}
	return 0 ;
}

/*==============================================================*/
/*			The special move the script set up: its numbers		*/
/*			read from the script once the wait ends				*/
/*==============================================================*/
static int sp_cmd_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *p ;
	Uint16 sw ;

	if( pl_w->routine_1 || pl_w->act_st != N_SPECIAL && pl_w->act_st != N_SUPER ){
		pl_w->cpu_routine_2 -- ;
		pl_w->cpu_sp_timer = 0 ;
		return 1 ;
	}
	if( pl_w->cpu_sp_timer && !--pl_w->cpu_sp_timer ){
		p = sc->adrs ;
		p += pl_w->cpu_sp_ofs ;
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_sp_no = *p++ ;
		pl_w->cpu_work0.l = *p++ ;
		pl_w->cpu_atck_pk = *p++ ;
		pl_w->cpu_atck_level = *p++ ;
		sw = *p << 8 ;
		if( sw & 0xc00 ) pl_w->cpu_atck_lever = sw ^ pl_w->pl_dir * 0xc00 ;
	}
	pl_w->pad_sw.now = pl_w->cpu_work1.l ;
	return pl_w->cpu_renda( pl_w ) ;
}

/*==============================================================*/
/*			42: sets unk_45f									*/
/*==============================================================*/
static int unk45f_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	pl_w->cpu_vcounter_cnt = pl_w->cpu_prm.l + 1 ;
	return 1 ;
}

/*==============================================================*/
/*			71: sets unk_4a7									*/
/*==============================================================*/
static int unk4a7_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	pl_w->cpu_guard_back_cnt = pl_w->cpu_work0.l + 1 ;
	return 1 ;
}

/*==============================================================*/
/* 26 and 60: guards, for a number of frames, if an attack is coming; 60 guards low */
static int guard( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 gd ;
	Uint16 sw ;
	Uint16 act ;

	if( !cmd_act_ck( pl_w, 1 ) ) return 0 ;
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	if( pl_w->armor_flag || pl_w->guard_ng_timer || !pl_guard_ok( pl_w ) || !( gd = em_atck_kind( pl_w ) ) ) return 1 ;
	pl_w->cpu_routine_2 ++ ;
	sw = 0x400 ;
	if( pl_w->pl_stance == STANCE_AIR ) act = 19 ;
	else {
		if( pl_w->cpu_cmd == 60 || gd & 4 ){
			sw = 0x1400 ;
			act = 5 ;
		} else act = 7 ;
		pl_act_set( pl_w, act ) ;
		act = 18 ;
	}
	pl_w->cpu_work1.l = sw ^ pl_w->pl_dir * 0xc00 ;
	pl_w->pad_sw.now = pl_w->cpu_work1.l ;
	pl_w->pl_sw_new = sw ;
	pl_act_set( pl_w, act ) ;
	pl_w->pl_sw_new = 0 ;
	return 0 ;
}

/*==============================================================*/
static int guard_move( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *lw ;
	int ok ;
	int gd ;

	if( pl_w->team_no || pl_w->act_st != N_GUARD && pl_w->act_st != N_AIR_GUARD ){
		pl_w->cpu_routine_2 -- ;
		return 1 ;
	}
	if( pl_w->cpu_vcounter_cnt == 1 ){
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_sp_no = 125 ;
		pl_w->cpu_atck_pk = 0 ;
		pl_w->cpu_atck_level = 0 ;
		pl_w->cpu_atck_lever = 0 ;
	} else if( pl_w->cpu_guard_back_cnt == 1 ){
		ok = 0 ;
		if( FLAG_CK( pl_w->pl_bit, PLB0( PL_MEGAMAN ) | PLB0( PL_ROLL ), 0 ) ){
			if( pl_w->pl_type == PL_MEGAMAN ){
				lw = ( Uint8 * )&pl_w->tlocal[0] ;
				ok = lw[1] ;
			}
			if( pl_w->pl_type == PL_ROLL ){
				lw = ( Uint8 * )pl_w->tlocal ;
				ok = lw[1] ;
			}
		}
		if( !ok ){
			pl_w->guard_back_flag = -1 ;
			pl_w->cpu_guard_back_cnt = 0 ;
			pl_w->dash_w.step = 0 ;
			pl_w->dash_w.timer = 0 ;
			pl_w->dash_w.cnt = 0 ;
			pl_w->dash_w.rot_dir = 0 ;
			pl_w->dash_w.dir = 0 ;
			pl_w->dash_w.ok_flag = 0 ;
			pl_w->dash_w.sw = 0 ;
		}
	}
	if( --pl_w->cpu_work0.l ){
		if( gd = em_atck_kind( pl_w ) ){
			if( pl_w->pl_stance == STANCE_AIR || gd & 2 ) pl_w->cpu_work1.l &= ~0x1000 ;
			if( pl_w->pl_stance == STANCE_CROUCH || gd & 4 || pl_w->cpu_cmd == 60 ) pl_w->cpu_work1.l |= 0x1000 ;
		}
		pl_w->pad_sw.now = pl_w->cpu_work1.l ;
	}
	return 0 ;
}

/*==============================================================*/
/*			37: guards on, for a number of frames, while on		*/
/*			point												*/
/*==============================================================*/
static int guard_cont( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( !pl_w->team_no ){
		cmd_read( pl_w, sc, 0 ) ;
		cmd_read( pl_w, sc, 1 ) ;
		pl_w->cpu_routine_2 ++ ;
		pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	}
	return 1 ;
}

/*==============================================================*/
/*			47: can't be thrown, or hit, for a number of		*/
/*			frames												*/
/*==============================================================*/
static int catch_ng_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->cpu_routine_2 ++ ;
	pl_w->cpu_work0.l = pl_w->cpu_prm.l ;
	pl_w->catch_ng_timer = pl_w->cpu_prm.l ;
	pl_w->dm_flag = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			72: calls a partner									*/
/*==============================================================*/
static int partner_call( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( cmd_start_ck( pl_w, 3 ) ){
		cmd_read( pl_w, sc, 0 ) ;
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_sp_no = 123 ;
		pl_w->cpu_atck_pk = 0 ;
		pl_w->cpu_atck_level = 0 ;
		pl_w->cpu_atck_lever = 0 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			77: tags in the partner with the least vitality		*/
/*==============================================================*/
static int partner_change( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em, *w ;
	int i, no, vital ;

	if( cmd_start_ck( pl_w, 3 ) ){
		cmd_read( pl_w, sc, 0 ) ;
		no = 0 ;
		vital = 145 ;
		em = pl_w->target_w ;
		i = 0 ;
		w = &pl_work[1 >> ( pl_w->pl_no & 1 )] ;
		for( ; i < 3 ; i ++, w ++ ){
			if( w->pl_type == em->pl_type ) continue ;
			if( !w->vital_new ) continue ;
			if( w->vital_new > vital ) continue ;
			vital = w->vital_new ;
			no ++ ;
		}
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_sp_no = 124 ;
		pl_w->cpu_atck_pk = 0 ;
		pl_w->cpu_atck_level = no ;
		pl_w->cpu_atck_lever = 0 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			68 and 69: special commands 122 and 126				*/
/*==============================================================*/
static int sp_change( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( cmd_start_ck( pl_w, 3 ) ){
		cmd_read( pl_w, sc, 0 ) ;
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_sp_no = 122 ;
		pl_w->cpu_atck_pk = 0 ;
		pl_w->cpu_atck_level = 0 ;
		pl_w->cpu_atck_lever = 0 ;
	}
	return 0 ;
}

/*==============================================================*/
static int sp_v_combi( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	if( cmd_start_ck( pl_w, 3 ) ){
		cmd_read( pl_w, sc, 0 ) ;
		pl_w->cpu_sp_flag = 1 ;
		pl_w->cpu_sp_no = 126 ;
		pl_w->cpu_atck_pk = 0 ;
		pl_w->cpu_atck_level = 0 ;
		pl_w->cpu_atck_lever = 0 ;
	}
	return 0 ;
}

/*==============================================================*/
/*			116: if the opponent stands, crouches or is in		*/
/*			the air												*/
/*==============================================================*/
static int if_em_stance( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	em = pl_w->target_w ;
	return if_ck( pl_w, sc, em->pl_stance == pl_w->cpu_prm.l ) ;
}

/*==============================================================*/
/*			117: if a character's own count is at a number		*/
/*==============================================================*/
static int if_char( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	int no ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	no = 0 ;
	switch( pl_w->pl_type ){
	case PL_SILVER_SAMURAI :
		no = pl_w->tlocal[7] ;
		break ;
	case PL_SPIRAL :
		no = ( ( Uint8 * )pl_w->tlocal )[0x16] ;
		break ;
	case PL_SAKURA :
		no = pl_w->obj_flag ;
		break ;
	case PL_SHUMA_GORATH :
		no = ( ( Sint8 * )pl_w->tlocal )[9] ;
		break ;
	}
	return if_ck( pl_w, sc, no == pl_w->cpu_prm.l ) ;
}

/*==============================================================*/
/*			118: if the character's own work starts with a		*/
/*			number												*/
/*==============================================================*/
static int if_tlocal2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *lw = ( Uint8 * )&pl_w->tlocal[0] ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	return if_ck( pl_w, sc, *lw == pl_w->cpu_prm.l ) ;
}

/*==============================================================*/
/*			119: if its vitality is at or below a share of		*/
/*			the most, out of 200								*/
/*==============================================================*/
static int if_vital( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Sint16 vital ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	vital = pl_w->cpu_prm.l * 144 / 200 ;
	return if_ck( pl_w, sc, pl_w->vital_new <= vital ) ;
}

/*==============================================================*/
/*			120: the same for the opponent						*/
/*==============================================================*/
static int if_em_vital( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;
	Sint16 vital ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	em = pl_w->target_w ;
	vital = pl_w->cpu_prm.l * 144 / 200 ;
	return if_ck( pl_w, sc, em->vital_new <= vital ) ;
}

/*==============================================================*/
/*			121 and 122: tests that never hold					*/
/*==============================================================*/
static int if_ng( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	return if_ck( pl_w, sc, 0 ) ;
}

/*==============================================================*/
static int if_ng2( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	return if_ck( pl_w, sc, 0 ) ;
}

/*==============================================================*/
/*			123: if its team's vitality is at or below a		*/
/*			share												*/
/*==============================================================*/
static int if_team_vital( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *w ;
	int i, vital, a, b ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	w = &pl_work[pl_w->pl_no] ;
	vital = 0 ;
	for( i = 0 ; i < 3 ; i ++, w += 2 ) vital += w->vital_new ;
	a = vital * 144 / 200 ;
	b = pl_w->cpu_prm.l * 144 / 200 ;
	return if_ck( pl_w, sc, a <= b ) ;
}

/*==============================================================*/
/*			124: the same for the opponent's team				*/
/*==============================================================*/
static int if_em_team_vital( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *w ;
	int i, vital, a, b ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 2 ) ;
	w = &pl_work[pl_w->pl_no ^ 1] ;
	vital = 0 ;
	for( i = 0 ; i < 3 ; i ++, w += 2 ) vital += w->vital_new ;
	a = vital * 144 / 200 ;
	b = pl_w->cpu_prm.l * 144 / 200 ;
	return if_ck( pl_w, sc, a <= b ) ;
}

/*==============================================================*/
/*			74 and 75: holds a punch (74) or a kick (75),		*/
/*			light or heavy										*/
/*==============================================================*/
static int btn_hold( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint16 sw ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	pl_w->atck_lever = 0 ;
	pl_w->cpu_atck_lever = 0 ;
	sw = 0x240 >> pl_w->cpu_prm.l ;
	if( pl_w->cpu_cmd == 74 ){
		sw &= 0x380 ;
		pl_w->cpu_atck_pk = 0 ;
	} else {
		sw &= 0x70 ;
		pl_w->cpu_atck_pk = 1 ;
	}
	if( !( pl_w->cpu_hold_sw & sw ) ){
		pl_w->cpu_hold_sw |= sw ;
		if( !cmd_act_ck( pl_w, 2 ) ) return 0 ;
		pl_w->pad_sw.now |= sw ;
		pl_w->cpu_atck_pk |= 0x80 ;
		pl_w->cpu_atck_level = pl_w->cpu_prm.l ;
	}
	return 0 ;
}

/*==============================================================*/
/*			76: waits											*/
/*==============================================================*/
static int cmd_stop( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	return 0 ;
}

/*==============================================================*/
/*			126: if the opponent jumped a way: forward,			*/
/*			straight up or back									*/
/*==============================================================*/
static int if_em_jump( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	PLAYER_WORK *em ;

	cmd_read( pl_w, sc, 0 ) ;
	cmd_read( pl_w, sc, 1 ) ;
	em = pl_w->target_w ;
	return if_ck( pl_w, sc, em->jump_dir == ( Sint8 )pl_w->cpu_prm.l ) ;
}

/*==============================================================*/
/*			78: a special move to do later, if the gauge is		*/
/*			at a level: after a number of frames				*/
/*==============================================================*/
static int sp_cmd_set( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	cmd_read( pl_w, sc, 0 ) ;
	pl_w->cpu_sp_ofs = sc->ofs ;
	sc->ofs += 5 ;
	cmd_read( pl_w, sc, 1 ) ;
	if( *pl_w->gauge_level < pl_w->cpu_prm.l ){
		sc->ofs += 2 ;
		pl_w->cpu_sp_timer = 0 ;
	} else {
		cmd_read( pl_w, sc, 2 ) ;
		if( pl_w->cpu_prm.l < 0 ) pl_w->cpu_prm.l = 0 ;
		pl_w->cpu_sp_timer = pl_w->cpu_prm.l ;
	}
	return 1 ;
}

/****************************************************************/
/*			Skips to the endif of the if just read, past		*/
/*			those nested in it									*/
/****************************************************************/
int goto_endif( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *p ;
	Uint8 *size ;
	int ofs ;
	int nest ;
	int cmd ;

	pl_w->cpu_routine_2 = 1 ;
	nest = 0 ;
	size = cmd_size_tbl ;
	ofs = sc->ofs ;
	p = sc->adrs ;
	do {
		sc->cmd_cnt ++ ;
		cmd = p[ofs] ;
		ofs += size[cmd] ;
		if( cmd >= 96 ) nest ++ ;
		else switch( cmd ){
		case 3 :
			return cmd_reset( pl_w, sc ) ;
		case 8 :
			return cmd_exit( pl_w, sc ) ;
		case 22 :
			return jump_ret( pl_w, sc ) ;
		case 67 :
			return cmd_next2( pl_w, sc ) ;
		case 23 :
			if( nest ) break ;
		case 24 :
		case 73 :
			nest -- ;
			break ;
		}
	} while( nest >= 0 ) ;
	sc->ofs = ofs ;
	pl_w->cpu_cmd_timer = 0 ;
	return 1 ;
}

/****************************************************************/
/*			Skips to the next hi-endif (73)						*/
/****************************************************************/
int goto_hiendif( PLAYER_WORK *pl_w, CPU_SCRIPT *sc )
{
	Uint8 *p ;
	Uint8 *size ;
	int ofs ;
	int nest ;
	int cmd ;

	pl_w->cpu_routine_2 = 1 ;
	nest = 0 ;
	size = cmd_size_tbl ;
	ofs = sc->ofs ;
	p = sc->adrs ;
	do {
		sc->cmd_cnt ++ ;
		cmd = p[ofs] ;
		ofs += size[cmd] ;
		switch( cmd ){
		case 3 :
			return cmd_reset( pl_w, sc ) ;
		case 8 :
			return cmd_exit( pl_w, sc ) ;
		case 22 :
			return jump_ret( pl_w, sc ) ;
		case 67 :
			return cmd_next2( pl_w, sc ) ;
		case 73 :
			nest -- ;
			break ;
		}
	} while( nest >= 0 ) ;
	sc->ofs = ofs ;
	pl_w->cpu_cmd_timer = 0 ;
	return 1 ;
}

/*==============================================================*/
/*			The mashing during a special move, for Omega		*/
/*			Red: none, or punch with the stick back, down,		*/
/*			up or up and back, on every other frame				*/
/*==============================================================*/
static int sp_renda_none( PLAYER_WORK *pl_w )
{
	return 0 ;
}

/*==============================================================*/
static int sp_renda_back( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 5 && Sys->game_timer0 & 1 ) pl_w->pad_sw.now = pl_w->pl_dir * 0xc00 ^ 0x500 ;
	return 0 ;
}

/*==============================================================*/
static int sp_renda_down( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 5 && Sys->game_timer0 & 1 ) pl_w->pad_sw.now = ( KEY_DOWN | KEY_HP ) ;
	return 0 ;
}

/*==============================================================*/
static int sp_renda_up( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 5 && Sys->game_timer0 & 1 ) pl_w->pad_sw.now = ( KEY_UP | KEY_HP ) ;
	return 0 ;
}

/*==============================================================*/
static int sp_renda_up_back( PLAYER_WORK *pl_w )
{
	if( pl_w->routine_2 == 5 && Sys->game_timer0 & 1 ) pl_w->pad_sw.now = pl_w->pl_dir * 0xc00 ^ 0x2500 ;
	return 0 ;
}

/* the command tables, by the command's number */

static CMD_FUNC cmd_start_jmp[128] = {
	cmd_nop, walk_f, walk_b, cmd_reset,	/* 00 */
	wait, wait2, crouch, stand,	/* 04 */
	cmd_exit, cmd_next, cmd_next3, punch,	/* 08 */
	kick, jump, walk_em, cmd_jump,	/* 12 */
	sjump, sp_cmd, dash_f, dash_b,	/* 16 */
	dash_em, air_jump, jump_ret, cmd_else,	/* 20 */
	cmd_endif, timer_set, guard, sjump2,	/* 24 */
	air_wait, cmd_dummy, jump_rnd, cancel_clr,	/* 28 */
	jump_em, jump_level, jump_flag_clr, lock_set,	/* 32 */
	lock_clr, guard_cont, walk_rnd, jump_dist,	/* 36 */
	timer2_set, cmd_nop, unk45f_set, nag_vflip_adj,	/* 40 */
	nag_vflip_adj2, cmd_nop, cmd_nop, catch_ng_set,	/* 44 */
	cmd_nop, walk_dist, walk_f2, walk_b2,	/* 48 */
	walk_f3, walk_b3, walk_f4, walk_b4,	/* 52 */
	cmd_nop, nag_vflip_adj3, nag_vflip_adj4, cmd_nop,	/* 56 */
	guard, cmd_nop, fly_flag_clr, walk_em2,	/* 60 */
	walk_pos, fly, walk_pos_y, cmd_next2,	/* 64 */
	sp_change, sp_v_combi, sjump3, unk4a7_set,	/* 68 */
	partner_call, cmd_endif, btn_hold, btn_hold,	/* 72 */
	cmd_stop, partner_change, sp_cmd_set, cmd_nop,	/* 76 */
	cmd_nop, cmd_nop, cmd_nop, cmd_nop,	/* 80 */
	cmd_nop, cmd_nop, cmd_nop, cmd_nop,	/* 84 */
	cmd_nop, cmd_nop, cmd_nop, cmd_nop,	/* 88 */
	cmd_nop, cmd_nop, cmd_nop, cmd_nop,	/* 92 */
	if_dist, if_em_atck, cancel_set, if_area,	/* 96 */
	cancel_set2, jump_flag_set, sjump_flag_set, walk_atck,	/* 100 */
	dash_atck, if_catch, if_level, if_area2,	/* 104 */
	fly_flag_set, if_rnd, if_atck_come, if_gauge,	/* 108 */
	if_fight_sec, if_height, if_tlocal, if_time,	/* 112 */
	if_em_stance, if_char, if_tlocal2, if_vital,	/* 116 */
	if_em_vital, if_ng, if_ng2, if_team_vital,	/* 120 */
	if_em_team_vital, cmd_nop, if_em_jump, cmd_nop,	/* 124 */
} ;

static CMD_FUNC cmd_move_jmp[128] = {
	cmd_end, walk_move, walk_b_move, cmd_end,	/* 00 */
	wait_move, wait_move, cmd_end, cmd_end,	/* 04 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 08 */
	cmd_end, cmd_end, walk_em_move, cmd_end,	/* 12 */
	cmd_end, sp_cmd_move, walk_move, walk_back_move,	/* 16 */
	walk_em_move, cmd_end, cmd_end, cmd_end,	/* 20 */
	cmd_end, cmd_end, guard_move, cmd_end,	/* 24 */
	wait_move, cmd_end, cmd_end, cmd_end,	/* 28 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 32 */
	cmd_end, guard_move, walk_dist_move, cmd_end,	/* 36 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 40 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 44 */
	cmd_end, walk_dist_move, walk_move, walk_back_move,	/* 48 */
	walk_move, walk_back_move, walk_move, walk_back_move,	/* 52 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 56 */
	guard_move, cmd_end, cmd_end, walk_em2_move,	/* 60 */
	walk_pos_move, walk_move, walk_pos_y_move, cmd_end,	/* 64 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 68 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 72 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 76 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 80 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 84 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 88 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 92 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 96 */
	cmd_end, cmd_end, cmd_end, walk_atck_move,	/* 100 */
	walk_atck_move, cmd_end, cmd_end, cmd_end,	/* 104 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 108 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 112 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 116 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 120 */
	cmd_end, cmd_end, cmd_end, cmd_end,	/* 124 */
} ;

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "em_script.c" End of File	*/
