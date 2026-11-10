/* Strider's effect 04 (character 05's shot 04): the four copies of Strider that his hyper throw
   puts round the thrown player, each slashing through him from a random side. */
#include "player.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl05_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL05_04_LOCAL_W {
	Uint8	cg;		/* 00: its slash's picture, by the angle it flies at */
} SHL05_04_LOCAL_W;
#define SHL05_04_LW( w )	( ( SHL05_04_LOCAL_W * )&( w )->obj_flag )

static SHL05_04_LOCAL_W *lw;	/* the running work's */
static PL05_LOCAL_W *pl_lw;	/* Strider's, unused */

static void shl05_04_main( PLAYER_WORK *ew );

/****************************************************************/
/*			The four copies, types 0 to 3						*/
/****************************************************************/
void *shl05_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	int i ;

	for( i = 0 ; i < 4 ; i ++ ){
		if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
			ew->work_func = shl05_04_main ;
			ew->master_w = pl_w ;
			ew->type = i ;
			ew->ef_no = 0x504 ;
		}
	}
	return ew ;
}

static void shl05_04_init( PLAYER_WORK *ew );
static void shl05_04_move( PLAYER_WORK *ew );
static void shl05_04_end( PLAYER_WORK *ew );
static void shl05_04_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_04_main( PLAYER_WORK *ew )
{
	static void ( *shl05_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_04_init, shl05_04_move, shl05_04_end, shl05_04_free,
	} ;

	pl_lw = ( PL05_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = SHL05_04_LW( ew ) ;
	shl05_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl05_04_init( PLAYER_WORK *ew )
{
	static Float shl05_04_spd[4][2] = {
		{ -DOT_X( 8.0f ), DOT_Y( 3.0f ) }, { -5.0f, DOT_Y( 8.0f ) },
		{ 5.0f, DOT_Y( 8.0f ) }, { DOT_X( 8.0f ), DOT_Y( 3.0f ) },
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
	ew->disp_flag = 1 ;
	ew->disp_dir = 0 ;
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->size_top = 56 ;
	ew->size_bottom = 56 ;
	ew->size_front = 80 ;
	ew->size_back = 80 ;
	ew->timer0 = 16 ;
	ew->timer1 = 3 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = shl05_04_spd[ew->type][0] ;
	ew->spd.y = shl05_04_spd[ew->type][1] ;
	pl_char_set( ew, 21, ew->type + 29 ) ;
}

static void shl05_04_out( PLAYER_WORK *ew );
static void shl05_04_turn( PLAYER_WORK *ew );
static void shl05_04_slash( PLAYER_WORK *ew );
static void shl05_04_pass( PLAYER_WORK *ew );
static void shl05_04_wait( PLAYER_WORK *ew );
static void shl05_04_last( PLAYER_WORK *ew );
static void shl05_04_back( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl05_04_move( PLAYER_WORK *ew )
{
	static void ( *shl05_04_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_04_out, shl05_04_turn, shl05_04_slash, shl05_04_pass,
		shl05_04_wait, shl05_04_last, shl05_04_back,
	} ;
	PLAYER_WORK *pl_w = ew->master_w ;

	if( pl_w->team_no ){
		shl05_04_end( ew ) ;
		return ;
	}
	shl05_04_move_jmp[ew->routine_1]( ew ) ;
}

static void shl05_04_attack( PLAYER_WORK *ew );
static void shl05_04_aim( PLAYER_WORK *ew );

/*==============================================================*/
/*			Each leaps out from Strider							*/
/*==============================================================*/
static void shl05_04_out( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !--ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 21, ew->type + 33 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_04_turn( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		shl05_04_attack( ew ) ;
	}
}

/*==============================================================*/
/*			Through the thrown player							*/
/*==============================================================*/
static void shl05_04_slash( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !--ew->timer0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 16 ;
	}
	if( !scr_in_ck( ew ) ){
		ew->disp_flag = 0 ;
		return ;
	}
	hit_work_set( ew ) ;
	return ;
}

/*==============================================================*/
/*			On past him, then out of sight						*/
/*==============================================================*/
static void shl05_04_pass( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	if( --ew->timer0 ){
		if( scr_in_ck( ew ) ) ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	} else {
		ew->routine_1 ++ ;
		ew->disp_flag = 0 ;
		pl_char_set( ew, 22, lw->cg + 18 ) ;
	}
}

/*==============================================================*/
/*			Three slashes, then the last one, aimed				*/
/*==============================================================*/
static void shl05_04_wait( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	if( pl_char_move( ew ) < 0 && --ew->timer0 < 0 ){
		if( --ew->timer1 ){
			ew->routine_1 = 2 ;
			shl05_04_attack( ew ) ;
			return ;
		}
		ew->routine_1 ++ ;
		ew->timer0 = 16 ;
		ew->atck_no = pl05_at_ragnarok_end ;
		HIT_RESET( ew ) ;
		shl05_04_aim( ew ) ;
		pl_char_set( ew, 21, ew->type + 37 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_04_last( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !--ew->timer0 ){
		ew->routine_1 ++ ;
		pl_char_set( ew, 21, ew->type + 41 ) ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_04_back( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( pl_char_move( ew ) < 0 ) shl05_04_end( ew ) ;
}

/*==============================================================*/
/*			The last slash: from beside the thrown player,		*/
/*			by type, at him in 16 frames						*/
/*==============================================================*/
static void shl05_04_aim( PLAYER_WORK *ew )
{
	static Float shl05_04_aim_pos[4][2] = {
		{ DOT_X( 160.0f ), DOT_Y( 96.0f ) }, { DOT_X( 80.0f ), DOT_Y( 128.0f ) },
		{ -DOT_X( 80.0f ), DOT_Y( 128.0f ) }, { -DOT_X( 160.0f ), DOT_Y( 96.0f ) },
	} ;
	PLAYER_WORK *em_w = ew->master_w->catch_w ;

	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->pos.y ;
	ew->pos.x += ew->disp_dir ? shl05_04_aim_pos[ew->type][0] : -shl05_04_aim_pos[ew->type][0] ;
	ew->pos.y += shl05_04_aim_pos[ew->type][1] ;
	ew->spd.x = ( em_w->pos.x - ew->pos.x ) / 16.0f ;
	ew->spd.y = ( em_w->pos.y - ew->pos.y ) / 16.0f ;
}

static void shl05_04_rnd_pos( PLAYER_WORK *ew );

/*==============================================================*/
/*			A slash from a random side, at the thrown			*/
/*			player's middle in 16 frames						*/
/*==============================================================*/
static void shl05_04_attack( PLAYER_WORK *ew )
{
	static Uint8 shl05_04_cg[16] = {
		17, 17, 16, 15, 14, 13, 12, 12, 11, 11, 10, 9, 8, 7, 6, 6,
	} ;
	PLAYER_WORK *em_w = ew->master_w->catch_w ;
	VEC3 pos ;
	Uint8 no ;
	Uint8 cg ;

	ew->timer0 = 16 ;
	ew->atck_no = ( ew->type & 1 ) ? 83 : 73 ;
	HIT_RESET( ew ) ;
	shl05_04_rnd_pos( ew ) ;
	ew->spd.x = ( em_w->pos.x - ew->pos.x ) / 16.0f ;
	ew->spd.y = ( em_w->pos.y - ew->pos.y + 120.0f ) / 16.0f ;
	pos.x = em_w->pos.x ;
	pos.y = em_w->pos.y + 120.0f ;
	no = caldir_pos_256( &ew->pos, &pos ) >> 4 ;
	ew->dir = no ;
	if( ew->disp_dir ){
		no ^= 15 ;
		no ++ ;
		no &= 15 ;
	}
	cg = shl05_04_cg[no] ;
	lw->cg = cg - 6 ;
	pl_char_set( ew, 22, cg ) ;
}

/*==============================================================*/
/*			A place round the thrown player, its sides by		*/
/*			type												*/
/*==============================================================*/
static void shl05_04_rnd_pos( PLAYER_WORK *ew )
{
	static Float shl05_04_rnd[16][2] = {
		{ 480.0f, 480.0f }, { DOT_X( 256.0f ), 480.0f }, { DOT_X( 224.0f ), 480.0f }, { 320.0f, 480.0f },
		{ DOT_X( 160.0f ), 480.0f }, { DOT_X( 128.0f ), 480.0f }, { DOT_X( 64.0f ), 480.0f }, { 0.0f, 480.0f },
		{ 480.0f, 480.0f }, { 480.0f, DOT_Y( 192.0f ) }, { 480.0f, DOT_Y( 160.0f ) }, { 480.0f, DOT_Y( 128.0f ) },
		{ 480.0f, DOT_Y( 96.0f ) }, { 480.0f, DOT_Y( 64.0f ) }, { 480.0f, DOT_Y( 32.0f ) }, { 480.0f, 0.0f },
	} ;
	PLAYER_WORK *em_w = ew->master_w->catch_w ;
	Uint8 no ;

	ew->pos.x = em_w->pos.x ;
	ew->pos.y = em_w->pos.y ;
	no = nlRand() & 15 ;
	ew->pos.x += ( ew->type & 1 ) ? shl05_04_rnd[no][0] : -shl05_04_rnd[no][0] ;
	ew->pos.y += ( ew->type & 2 ) ? shl05_04_rnd[no][1] : -shl05_04_rnd[no][1] ;
}

/*--------------------------------------------------------------*/
static void shl05_04_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl05_04_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl05_04.c" End of File	*/
