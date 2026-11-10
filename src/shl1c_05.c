/* The main fire of Hyper Mega Man (character 1c's shot 05): a head that rides
   on the player and makes a string of parts, each placed on the one before. */
#include "player.h"
#include "pl1c_def.h"

static void shl1c_05_main( PLAYER_WORK *ew );

/* a part's own work, at obj_flag */
typedef struct SHL1C_05_LOCAL_W {
	Sint32	first_flag;			/* 255 on the first part, 0 on those after it */
	Sint32	atck_on_flag;			/* 255 on every fifth, which hits */
} SHL1C_05_LOCAL_W;
#define SHL1C_05_LW( w )	( ( SHL1C_05_LOCAL_W * )&( w )->obj_flag )

/****************************************************************/
void *shl1c_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1c_05_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
	}
	return ew ;
}

static void shl1c_05_init( PLAYER_WORK *ew );
static void shl1c_05_move( PLAYER_WORK *ew );
static void shl1c_05_end( PLAYER_WORK *ew );
static void shl1c_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_05_main( PLAYER_WORK *ew )
{
	static void ( *shl1c_05_jmp[] )( PLAYER_WORK *ew ) = {
		shl1c_05_init, shl1c_05_move, shl1c_05_end, shl1c_05_free,
	} ;

	shl1c_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1c_05_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	PLAYER_WORK *tw ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1c05 ;
	if( !ew->type ){
		ew->work1 = 0 ;
		ew->obj_flag = 0 ;
		ew->work2 = 2 ;
		if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			tw->work_func = shl1c_05_main ;
			tw->master_w = ew->master_w ;
			tw->type = 1 ;
			tw->work1 = 255 ;
			tw->obj_flag = 255 ;
			tw->ef_w = ew ;
		}
	}
	CHAR_PART_COPY( ew, pl_w ) ;
	ew->disp_flag = 1 ;
	ew->pl_no = pl_w->pl_no ;
	ew->pl_type = pl_w->pl_type ;
	ew->scale.x = pl_w->scale.x ;
	ew->scale.y = pl_w->scale.y ;
	ew->atck_level = pl_w->atck_level ;
	ew->work_no = pl_w->work_no ;
	ew->col_cnt = pl_w->col_cnt ;
	ew->scale = pl_w->scale ;
	ew->prio = pl_w->prio ;
	ew->disp_flag = 1 ;
	ew->prio = 10 ;
	ew->size_top = 64 ;
	ew->size_bottom = 64 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	ew->scale.x = 1.01f ;
	if( ew->disp_dir == 0 ){
		ew->dir = 12 ;
	} else {
		ew->dir = 4 ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	shl1c_05_move( ew ) ;
}

static void shl1c_05_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_05_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1c_05_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->type ){
		shl1c_05_head_move( ew, pl_w ) ;
	} else {
		shl1c_05_part_move( ew, pl_w ) ;
	}
}

static void shl1c_05_head_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1c_05_head_char( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_05_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *tw ;

	if( pl_w->char_no.w != 0x1603 && pl_w->char_no.w != 0x161b ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ( tw = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		tw->work_func = shl1c_05_main ;
		tw->master_w = ew->master_w ;
		tw->type = 1 ;
		tw->obj_flag = 0 ;
		tw->ef_w = ew ;
		tw->work1 = 0 ;
		if( ! -- ew->work2 ){
			ew->work2 = 5 ;
			tw->work1 = 255 ;
		}
	}
	shl1c_05_head_pos( ew, pl_w ) ;
	shl1c_05_head_char( ew ) ;
}

static void shl1c_05_part_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *bw );
static void shl1c_05_part_char( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1c_05_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *rw = ew->ef_w ;
	PLAYER_WORK *bw = ew->before_w ;

	if( rw->routine_0 >= 2 || bw->routine_0 >= 2 ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1c_05_part_pos( ew, pl_w, bw ) ;
	shl1c_05_part_char( ew ) ;
	ew->type ++ ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		ew->work1 = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		ew->hit_flag = 0 ;
		ew->work1 = 0 ;
		return ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	{
		SHL1C_05_LOCAL_W *lw = SHL1C_05_LW( ew ) ;

		if( !lw->first_flag && !lw->atck_on_flag ) ew->atck_flag = 0 ;
	}
	ew->atck_no = pl_w->atck_no ;
	HIT_RESET( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1c_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1c_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1c_05_head_char( PLAYER_WORK *ew )
{
	if( !( Sys->game_timer0 & 1 ) ){
		pl_char_set2( ew, 22, 29, 0 ) ;
	} else {
		pl_char_set2( ew, 22, 29, 1 ) ;
	}
}

/*==============================================================*/
static void shl1c_05_part_char( PLAYER_WORK *ew )
{
	static Uint8 shl1c_05_char0[2] = { 2, 3 } ;
	static Uint8 shl1c_05_char1[2] = { 4, 5 } ;
	int no = Sys->game_timer0 & 1 ;

	if( ew->obj_flag ) pl_char_set2( ew, 22, 29, shl1c_05_char1[no] ) ;
	else pl_char_set2( ew, 22, 29, shl1c_05_char0[no] ) ;
}

/*==============================================================*/
static void shl1c_05_head_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x + ( !pl_w->disp_dir ? -DOT_X( 110.0f ) : DOT_X( 110.0f ) ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 115.0f ) ;
}

/*==============================================================*/
static void shl1c_05_part_pos( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *bw )
{
	if( bw->type == 0 ){
		if( !pl_w->disp_dir ) ew->pos.x = bw->pos.x - DOT_X( 64.0f ) ;
		else ew->pos.x = bw->pos.x + DOT_X( 64.0f ) ;
	} else ew->pos.x = bw->pos.x + ( !pl_w->disp_dir ? -DOT_X( 32.0f ) : DOT_X( 32.0f ) ) ;
	ew->pos.y = bw->pos.y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1c_05.c" End of File	*/
