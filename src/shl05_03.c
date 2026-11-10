/* Strider's effect 03 (character 05's shot 03): the bird that flies in from the screen's edge and
   drops a bomb (types 0 and 2), and the pieces that come out from Strider himself (1, 3 to 6). */
#include "player.h"
#include "pl_char.h"
#include "pl05_def.h"


/* each type's init starts from the master's picture and place in the draw order */
#define SHL05_03_COPY( ew )									\
	CHAR_PART_COPY( ew, ew->master_w ) ;						\
	ew->disp_flag = 1 ;										\
	ew->pl_no = ew->master_w->pl_no ;							\
	ew->pl_type = ew->master_w->pl_type ;						\
	ew->scale.x = ew->master_w->scale.x ;						\
	ew->scale.y = ew->master_w->scale.y ;						\
	ew->atck_level = ew->master_w->atck_level ;				\
	ew->work_no = ew->master_w->work_no ;						\
	ew->col_cnt = ew->master_w->col_cnt ;						\
	ew->scale = ew->master_w->scale ;							\
	ew->prio = ew->master_w->prio ;							\
	ew->disp_dir = ew->master_w->disp_dir

static Sint32 *lw;	/* the running work's, unused */
static PL05_LOCAL_W *pl_lw;	/* Strider's */

static void shl05_03_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl05_03_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_03_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->ef_no = 0x503 ;
	}
	return ew ;
}

/*==============================================================*/
/*			The bomb, dropped by the bird pl_w					*/
/*==============================================================*/
static void *shl05_03_bomb_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl05_03_main ;
		ew->master_w = pl_w->master_w ;
		ew->ef_w = pl_w ;
		ew->type = 2 ;
		ew->ef_no = 0x503 ;
	}
	return ew ;
}

static void shl05_03_init( PLAYER_WORK *ew );
static void shl05_03_move( PLAYER_WORK *ew );
static void shl05_03_end( PLAYER_WORK *ew );
static void shl05_03_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_main( PLAYER_WORK *ew )
{
	static void ( *shl05_03_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_init, shl05_03_move, shl05_03_end, shl05_03_free,
	} ;

	pl_lw = ( PL05_LOCAL_W * )&ew->master_w->tlocal[0] ;
	lw = &ew->obj_flag ;
	shl05_03_jmp[ew->routine_0]( ew ) ;
}

static void shl05_03_bird_init( PLAYER_WORK *ew );
static void shl05_03_init1( PLAYER_WORK *ew );
static void shl05_03_bomb_init( PLAYER_WORK *ew );
static void shl05_03_init3( PLAYER_WORK *ew );
static void shl05_03_init4( PLAYER_WORK *ew );
static void shl05_03_init5( PLAYER_WORK *ew );
static void shl05_03_init6( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_init( PLAYER_WORK *ew )
{
	static void ( *shl05_03_init_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_bird_init, shl05_03_init1, shl05_03_bomb_init, shl05_03_init3,
		shl05_03_init4, shl05_03_init5, shl05_03_init6,
	} ;

	ew->routine_0 ++ ;
	shl05_03_init_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl05_03_bird_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	SHL05_03_COPY( ew ) ;
	pl_lw->bird_flag = 1 ;
	ew->prio = 11 ;
	ew->pos.x = scr_work.eye[0].x + ( ew->disp_dir ? -DOT_X( 256.0f ) : DOT_X( 256.0f ) ) ;
	ew->pos.y = ew->master_w->ground_y + DOT_Y( 192.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	ew->spd.y = -DOT_Y( 0.5f ) ;
	ew->size_top = 64 ;
	ew->size_bottom = 16 ;
	ew->size_front = 96 ;
	ew->size_back = 96 ;
	ew->timer0 = ew->atck_level ? 64 : 32 ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*==============================================================*/
static void shl05_03_init1( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	SHL05_03_COPY( ew ) ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -2.142857f ;
	ew->acc.y = 0.13392857f ;
	pl_char_set( ew, 18, 2 ) ;
}

/*==============================================================*/
static void shl05_03_bomb_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	SHL05_03_COPY( ew ) ;
	ew->prio = 11 ;
	ew->disp_dir = ew->ef_w->disp_dir ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->atck_no = pl05_at_formation_c ;
	HIT_RESET( ew ) ;
	ew->spd.x = 0.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = -DOT_Y( 3.0f ) ;
	ew->acc.y = -0.13392857f ;
	pl_char_set( ew, Char_Effect, 26 ) ;
}

/*==============================================================*/
static void shl05_03_init3( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	SHL05_03_COPY( ew ) ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.x += ew->disp_dir ? DOT_X( 64.0f ) : -DOT_X( 64.0f ) ;
	ew->size_top = 64 ;
	ew->size_bottom = 64 ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	pl_char_set( ew, Char_Effect, 22 ) ;
	shl05_03_move( ew ) ;
}

/*==============================================================*/
static void shl05_03_init4( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	SHL05_03_COPY( ew ) ;
	ew->prio = 11 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y + DOT_Y( 8.0f ) ;
	pl_se_req( ew, 73 ) ;
	pl_char_set( ew, Char_Effect, 24 ) ;
	shl05_03_move( ew ) ;
}

/*==============================================================*/
static void shl05_03_init5( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	SHL05_03_COPY( ew ) ;
	ew->prio = 0 ;
	ew->pos.x = scr_work.eye[0].x + ( ew->disp_dir ? -DOT_X( 256.0f ) : DOT_X( 256.0f ) ) ;
	ew->pos.y = ew->master_w->ground_y + DOT_Y( 304.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	ew->spd.y = -DOT_Y( 0.5f ) ;
	ew->timer0 = 48 ;
	ew->size_top = 64 ;
	ew->size_bottom = 16 ;
	ew->size_front = 96 ;
	ew->size_back = 96 ;
	pl_char_set( ew, Char_Effect, 35 ) ;
}

/*==============================================================*/
static void shl05_03_init6( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	SHL05_03_COPY( ew ) ;
	ew->prio = 0 ;
	ew->pos.x = scr_work.eye[0].x + ( ew->disp_dir ? -DOT_X( 256.0f ) : DOT_X( 256.0f ) ) ;
	ew->pos.y = ew->master_w->ground_y + 240.0f ;
	ew->spd.x = ew->disp_dir ? 10.0f : -10.0f ;
	ew->acc.x = 0.0f ;
	ew->spd.y = DOT_Y( 3.0f ) ;
	ew->acc.y = -DOT_Y( 0.375f ) ;
	pl_shadow_set( ew ) ;
	ew->be_flag = 1 ;
	pl_char_set( ew, Char_Effect, 36 ) ;
}

static void shl05_03_bird_move( PLAYER_WORK *ew );
static void shl05_03_move1( PLAYER_WORK *ew );
static void shl05_03_bomb_move( PLAYER_WORK *ew );
static void shl05_03_move3( PLAYER_WORK *ew );
static void shl05_03_move4( PLAYER_WORK *ew );
static void shl05_03_move5( PLAYER_WORK *ew );
static void shl05_03_move6( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl05_03_move( PLAYER_WORK *ew )
{
	static void ( *shl05_03_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_bird_move, shl05_03_move1, shl05_03_bomb_move, shl05_03_move3,
		shl05_03_move4, shl05_03_move5, shl05_03_move6,
	} ;

	shl05_03_move_jmp[ew->type]( ew ) ;
}

static void shl05_03_bird_fly( PLAYER_WORK *ew );
static void shl05_03_bird_go( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_bird_move( PLAYER_WORK *ew )
{
	static void ( *shl05_03_bird_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_bird_fly, shl05_03_bird_go,
	} ;

	shl05_03_bird_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Flies in until its timer runs out, then drops		*/
/*			the bomb and climbs away							*/
/*==============================================================*/
static void shl05_03_bird_fly( PLAYER_WORK *ew )
{
	VEC3 pos ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	hit_work_set( ew ) ;
	if( !--ew->timer0 ){
		ew->routine_1 ++ ;
		ew->spd.y = DOT_Y( 1.5f ) ;
		ew->acc.y = 0.13392857f ;
		if( !shl05_03_bomb_set( ew ) ) pl_lw->bird_flag = 0 ;
		pl_char_set( ew, Char_Effect, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_03_bird_go( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ){
		shl05_03_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
static void shl05_03_move1( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ) shl05_03_end( ew ) ;
}

static void shl05_03_bomb_fall( PLAYER_WORK *ew );
static void shl05_03_bomb_burst( PLAYER_WORK *ew );
static void shl05_03_bomb_smoke( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_bomb_move( PLAYER_WORK *ew )
{
	static void ( *shl05_03_bomb_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_bomb_fall, shl05_03_bomb_burst, shl05_03_bomb_smoke,
	} ;

	shl05_03_bomb_jmp[ew->routine_1]( ew ) ;
}

static void shl05_03_bomb_land( PLAYER_WORK *ew );
static void shl05_03_bomb_hit( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_bomb_fall( PLAYER_WORK *ew )
{
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->hit_flag ) shl05_03_bomb_land( ew ) ;
	else {
		if( ew->hit_box_no ){
			shl05_03_bomb_hit( ew ) ;
			return ;
		}
		if( ew->pos.y < ew->master_w->ground_y ){
			ew->pos.y = ew->master_w->ground_y ;
			shl05_03_bomb_land( ew ) ;
		} else {
			pl_char_move( ew ) ;
			if( ew->cg_type ){
				ew->cg_type = 0 ;
				ew->spd.y = -DOT_Y( 2.0f ) ;
				ew->acc.y = 0.0f ;
			}
		}
	}
	hit_work_set( ew ) ;
}

/*==============================================================*/
/*			It bursts: a new attack and the blast's mark		*/
/*==============================================================*/
static void shl05_03_bomb_land( PLAYER_WORK *ew )
{
	VEC3 pos ;

	ew->routine_1 ++ ;
	ew->atck_no = pl05_at_formation_c_blast ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 38 ) ;
	pos = ew->pos ;
	pos.y += DOT_Y( 24.0f ) ;
	burst_set( ew, &pos, 1, 0x179 ) ;
	pl_se_req( ew, 74 ) ;
}

/*==============================================================*/
/*			Struck by an attack									*/
/*==============================================================*/
static void shl05_03_bomb_hit( PLAYER_WORK *ew )
{
	VEC3 pos ;

	pos = ew->pos ;
	burst_set( ew, &pos, 1, 0xc9 ) ;
	pl_lw->bird_flag = 0 ;
	shl05_03_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_03_bomb_burst( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		pl_lw->bird_flag = 0 ;
		shl05_03_end( ew ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_03_bomb_smoke( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl05_03_end( ew ) ;
}

static void shl05_03_3_wait( PLAYER_WORK *ew );
static void shl05_03_3_appear( PLAYER_WORK *ew );
static void shl05_03_3_stop( PLAYER_WORK *ew );
static void shl05_03_3_go( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_move3( PLAYER_WORK *ew )
{
	static void ( *shl05_03_3_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_3_wait, shl05_03_3_appear, shl05_03_3_stop, shl05_03_3_go,
	} ;

	shl05_03_3_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Waits for Strider's picture to give the cue			*/
/*==============================================================*/
static void shl05_03_3_wait( PLAYER_WORK *ew )
{
	VEC3 pos ;

	if( ew->master_w->cg_type ){
		ew->routine_1 ++ ;
		pos = ew->pos ;
		pos.y += DOT_Y( 12.0f ) ;
		burst_set( ew, &pos, 1, 0xc9 ) ;
		pl_se_req( ew, 73 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_03_3_appear( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_1 ++ ;
		ew->spd.x = ew->disp_dir ? -5.0f : 5.0f ;
		ew->acc.x = 0.0f ;
		ew->timer0 = 44 ;
		pl_char_set( ew, Char_Effect, 4 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_03_3_stop( PLAYER_WORK *ew )
{
	if( !--ew->timer0 ) ew->routine_1 ++ ;
}

/*--------------------------------------------------------------*/
static void shl05_03_3_go( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) ) shl05_03_end( ew ) ;
}

/*==============================================================*/
static void shl05_03_move4( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ) shl05_03_end( ew ) ;
}

static void shl05_03_5_fly( PLAYER_WORK *ew );
static void shl05_03_5_go( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_move5( PLAYER_WORK *ew )
{
	static void ( *shl05_03_5_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_5_fly, shl05_03_5_go,
	} ;

	shl05_03_5_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl05_03_5_fly( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !--ew->timer0 ){
		ew->routine_1 ++ ;
		ew->spd.y = DOT_Y( 1.5f ) ;
		ew->acc.y = 0.13392857f ;
		pl_char_set( ew, Char_Effect, 1 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl05_03_5_go( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( !scr_in_x_ck( ew ) ) shl05_03_end( ew ) ;
}

static void shl05_03_6_fall( PLAYER_WORK *ew );
static void shl05_03_6_land( PLAYER_WORK *ew );

/*==============================================================*/
static void shl05_03_move6( PLAYER_WORK *ew )
{
	static void ( *shl05_03_6_jmp[] )( PLAYER_WORK *ew ) = {
		shl05_03_6_fall, shl05_03_6_land,
	} ;

	shl05_03_6_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void shl05_03_6_fall( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	pl_char_move( ew ) ;
	if( ew->pos.y < ew->master_w->ground_y ){
		ew->routine_1 ++ ;
		ew->pos.y = ew->master_w->ground_y ;
		pl_char_set( ew, Char_Effect, 37 ) ;
	}
}

/*==============================================================*/
/*			Stays while Strider's act holds						*/
/*==============================================================*/
static void shl05_03_6_land( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->master_w->act_st != N_WIN ) shl05_03_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_03_end( PLAYER_WORK *ew )
{
	ew->routine_0 = 3 ;
	ew->disp_flag = 0 ;
	shl05_03_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl05_03_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl05_03.c" End of File	*/
