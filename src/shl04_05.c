/* Anakaris's effect 05 (character 04's shot 05): the two hands of his Pharaoh Illusion, type 0 on
   his front side and type 1 behind. Each waits beside him and, as the buttons and the stick
   pick them, runs through moves from a table and strikes. */
#include "player.h"
#include "pl_char.h"
#include "pl04_def.h"

/* one move: where it ends, from Anakaris (x for the front hand), its turn there, its animation,
   its attack (0 for none) and the frames it takes */
typedef struct SHL04_05_MOVE {
	Float	x, y;
	Sint32	rot;
	Uint8	char_no;
	Uint8	atck_no;
	Uint8	unk_e;
	Uint8	time;
} SHL04_05_MOVE;

/* the effect's own work, from obj_flag */
typedef struct SHL04_05_LOCAL_W {
	Sint32	rot_spd;	/* 000: the turn each frame of the move */
} SHL04_05_LOCAL_W;
#define SHL04_05_LW( w )	( ( SHL04_05_LOCAL_W * )&( w )->obj_flag )

static SHL04_05_MOVE shl04_05_move_tbl[] = {
	{ DOT_X( 128.0f ), DOT_Y( 80.0f ), 0, 25, 0, 1, 16 },
	{ DOT_X( 128.0f ), 120.0f, -0x5000, 25, 0, 4, 4 },
	{ DOT_X( 128.0f ), 480.0f, -0x5000, 26, 72, 4, 10 },
	{ DOT_X( 128.0f ), DOT_Y( 256.0f ), 0x2000, 25, 0, 4, 4 },
	{ DOT_X( 128.0f ), DOT_Y( 48.0f ), 0x2000, 26, 76, 4, 10 },
	{ DOT_X( 256.0f ), DOT_Y( 136.0f ), -0x2000, 25, 0, 4, 4 },
	{ DOT_X( 8.0f ), DOT_Y( 136.0f ), -0x2000, 27, 74, 4, 8 },
	{ 320.0f, DOT_Y( 192.0f ), 0, 25, 0, 4, 4 },
	{ 80.0f, DOT_Y( 96.0f ), 0, 25, 75, 4, 8 },
	{ DOT_X( 128.0f ), -DOT_Y( 32.0f ), -0x5000, 25, 0, 4, 3 },
	{ DOT_X( 128.0f ), DOT_Y( 128.0f ), -0x5000, 26, 72, 4, 8 },
	{ DOT_X( 256.0f ), DOT_Y( 32.0f ), -0x2000, 25, 0, 4, 4 },
	{ DOT_X( 8.0f ), DOT_Y( 32.0f ), -0x2000, 27, 74, 4, 8 },
	{ 320.0f, 0.0f, -0x3800, 25, 0, 4, 4 },
	{ DOT_X( 64.0f ), DOT_Y( 64.0f ), -0x3800, 25, 73, 4, 8 },
} ;

static void shl04_05_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl04_05_set( PLAYER_WORK *pl_w, Uint8 type )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl04_05_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x405 ;
	}
	return ew ;
}

static void shl04_05_init( PLAYER_WORK *ew );
static void shl04_05_move( PLAYER_WORK *ew );
static void shl04_05_end( PLAYER_WORK *ew );
static void shl04_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl04_05_main( PLAYER_WORK *ew )
{
	static void ( *shl04_05_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_05_init, shl04_05_move, shl04_05_end, shl04_05_free,
	} ;

	shl04_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			Spd.z and acc.z keep its place beside Anakaris		*/
/*==============================================================*/
static void shl04_05_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

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
	ew->timer1 = 0 ;
	ew->pos = pl_w->pos ;
	ew->prio = 12 ;
	ew->disp_dir = ew->type ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( !ew->type ) ew->spd.z = DOT_X( 128.0f ) ;
	else ew->spd.z = -DOT_X( 128.0f ) ;
	ew->acc.z = DOT_Y( 80.0f ) ;
	ew->pos.x += ew->spd.z ;
	ew->pos.y += ew->acc.z ;
	ew->rot[2] = 0 ;
	pl_char_set( ew, 22, 25 ) ;
	shl04_05_move( ew ) ;
}

static void shl04_05_wait( PLAYER_WORK *ew );
static void shl04_05_go( PLAYER_WORK *ew );
static void shl04_05_stay( PLAYER_WORK *ew );
static void shl04_05_strike( PLAYER_WORK *ew );
static void shl04_05_back( PLAYER_WORK *ew );

/*==============================================================*/
/*			It lasts while Anakaris is in the hyper combo		*/
/*==============================================================*/
static void shl04_05_move( PLAYER_WORK *ew )
{
	static void ( *shl04_05_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl04_05_wait, shl04_05_go, shl04_05_stay, shl04_05_strike, shl04_05_back,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( ew->timer1 ) ew->timer1 -- ;
	if( pl_w->act_st != N_SUPER || pl_w->pl_sp_type != pl04_sp_illusion || pl_w->routine_2 == 5 ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_stop ){
		ew->hit_stop -- ;
		return ;
	}
	ew->alpha = pl_w->alpha ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->sub_type = 0xff ;
	shl04_05_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl04_05_pos_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->pos.x = pl_w->pos.x + ew->spd.z ;
	ew->pos.y = pl_w->pos.y + ew->acc.z ;
}

static void shl04_05_move_set( PLAYER_WORK *ew );

/*==============================================================*/
/*			A button of this hand picks a strike by the			*/
/*			stick: Anakaris's dir is the button (1 to 4),		*/
/*			his sub_type the stick, and each table gives the	*/
/*			strike's first move									*/
/*==============================================================*/
static void shl04_05_next( PLAYER_WORK *ew )
{
	static Uint8 shl04_05_next_tbl1[] = { 5, 5, 1, 5, 3, 7, 3, 5, 1, 5, 1, 5, 5, 5, 5, 5 } ;
	static Uint8 shl04_05_next_tbl2[] = { 5, 1, 5, 5, 3, 3, 7, 5, 1, 1, 5, 5, 5, 5, 5, 5 } ;
	static Uint8 shl04_05_next_tbl3[] = { 9, 11, 9, 9, 9, 11, 9, 9, 9, 13, 9, 9, 9, 9, 9, 9 } ;
	static Uint8 shl04_05_next_tbl4[] = { 9, 9, 11, 9, 9, 9, 11, 9, 9, 9, 13, 9, 9, 9, 9, 9 } ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !pl_w->dir || ew->timer1 ) return ;
	if( ew->type == 1 && pl_w->dir == 1 ){
		ew->routine_1 = 3 ;
		ew->dir = shl04_05_next_tbl1[pl_w->sub_type] ;
	}
	if( ew->type == 0 && pl_w->dir == 2 ){
		ew->routine_1 = 3 ;
		ew->dir = shl04_05_next_tbl2[pl_w->sub_type] ;
	}
	if( ew->type == 1 && pl_w->dir == 3 ){
		ew->routine_1 = 3 ;
		ew->dir = shl04_05_next_tbl3[pl_w->sub_type] ;
	}
	if( ew->type == 0 && pl_w->dir == 4 ){
		ew->routine_1 = 3 ;
		ew->dir = shl04_05_next_tbl4[pl_w->sub_type] ;
	}
	shl04_05_move_set( ew ) ;
}

/*==============================================================*/
/*			Starts move dir: the speeds and the turn that		*/
/*			take it to the move's end point in its time			*/
/*==============================================================*/
static void shl04_05_move_set( PLAYER_WORK *ew )
{
	SHL04_05_MOVE *mv = &shl04_05_move_tbl[ew->dir] ;
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL04_05_LOCAL_W *lw = SHL04_05_LW( ew ) ;
	Float x, y ;

	x = mv->x ;
	if( ew->type ) x = -x ;
	x += pl_w->pos.x ;
	ew->spd.x = ( x - ew->pos.x ) / mv->time ;
	y = mv->y + pl_w->pos.y ;
	ew->spd.y = ( y - ew->pos.y ) / mv->time ;
	lw->rot_spd = ( mv->rot - ew->rot[2] ) / mv->time ;
	ew->timer0 = mv->time ;
	pl_char_set( ew, 22, mv->char_no ) ;
	if( ( ew->atck_no = mv->atck_no ) ){
		HIT_RESET( ew ) ;
		ew->atck_attr = ATR_BACK_POS ;
	}
}

/*==============================================================*/
/*			Beside Anakaris until he starts						*/
/*==============================================================*/
static void shl04_05_wait( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	shl04_05_pos_set( ew ) ;
	if( pl_w->dir != 0xff ){
		ew->routine_1 ++ ;
		ew->dir = 0 ;
		shl04_05_move_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl04_05_go( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL04_05_LOCAL_W *lw = SHL04_05_LW( ew ) ;

	shl04_05_next( ew ) ;
	if( --ew->timer0 ){
		ew->pos.x += ew->spd.x ;
		ew->pos.y += ew->spd.y ;
		ew->rot[2] += lw->rot_spd ;
		return ;
	}
	ew->routine_1 ++ ;
	ew->pos.x = pl_w->pos.x + ew->spd.z ;
	ew->pos.y = pl_w->pos.y + ew->acc.z ;
	ew->rot[2] = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_05_stay( PLAYER_WORK *ew )
{
	shl04_05_pos_set( ew ) ;
	shl04_05_next( ew ) ;
}

/*==============================================================*/
/*			The strike's wind-up, then the strike itself		*/
/*==============================================================*/
static void shl04_05_strike( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL04_05_LOCAL_W *lw = SHL04_05_LW( ew ) ;
	SHL04_05_MOVE *mv ;
	Float x ;

	if( --ew->timer0 ){
		ew->pos.x += ew->spd.x ;
		ew->pos.y += ew->spd.y ;
		ew->rot[2] += lw->rot_spd ;
	}
	else {
		ew->routine_1 ++ ;
		mv = &shl04_05_move_tbl[ew->dir ++] ;
		x = mv->x ;
		if( ew->type ) x = -x ;
		ew->pos.x = pl_w->pos.x + x ;
		ew->pos.y = pl_w->pos.y + mv->y ;
		ew->rot[2] = mv->rot ;
		shl04_05_move_set( ew ) ;
		pl_se_req( ew, 23 ) ;
	}
}

/*==============================================================*/
/*			The strike, then back to the start of the moves		*/
/*==============================================================*/
static void shl04_05_back( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL04_05_LOCAL_W *lw = SHL04_05_LW( ew ) ;
	SHL04_05_MOVE *mv ;
	Float x ;

	if( --ew->timer0 ){
		ew->pos.x += ew->spd.x ;
		ew->pos.y += ew->spd.y ;
		ew->rot[2] += lw->rot_spd ;
		hit_work_set( ew ) ;
		return ;
	}
	ew->routine_1 = 1 ;
	mv = &shl04_05_move_tbl[ew->dir] ;
	x = mv->x ;
	if( ew->type ) x = -x ;
	ew->pos.x = pl_w->pos.x + x ;
	ew->pos.y = pl_w->pos.y + mv->y ;
	ew->rot[2] = mv->rot ;
	ew->dir = 0 ;
	shl04_05_move_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl04_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl04_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl04_05.c" End of File	*/
