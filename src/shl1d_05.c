/* Roll's effect 05 (character 1d's shot 05): in her Hyper Roll, a head work that stays
   beside her and a chain of parts it makes, each part riding on the one before it. */
#include "player.h"
#include "pl_char.h"
#include "pl1d_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL1D_05_LOCAL_W {
	Sint32	first_flag;		/* 000: 0xff on the first part, 0 on those after it and on the head */
	Sint32	atck_on_flag;		/* 004: 0xff on a part just made, while it may attack */
	Sint32	atck_cnt;		/* 008: the head's count to the next bigger part */
} SHL1D_05_LOCAL_W;
#define SHL1D_05_LW( w )	( ( SHL1D_05_LOCAL_W * )&( w )->obj_flag )

static void shl1d_05_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1d_05_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1d_05_main ;
		ew->master_w = pl_w ;
		ew->type = 0 ;
	}
	return ew ;
}

static void shl1d_05_init( PLAYER_WORK *ew );
static void shl1d_05_move( PLAYER_WORK *ew );
static void shl1d_05_end( PLAYER_WORK *ew );
static void shl1d_05_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1d_05_main( PLAYER_WORK *ew )
{
	static void ( *shl1d_05_jmp[] )( PLAYER_WORK *ew ) = {
		shl1d_05_init, shl1d_05_move, shl1d_05_end, shl1d_05_free,
	} ;

	shl1d_05_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1d_05_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	ew->routine_0 ++ ;
	ew->ef_no = 0x1d05 ;
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
	ew->size_top = 32 ;
	ew->size_bottom = 32 ;
	ew->size_front = 32 ;
	ew->size_back = 32 ;
	if( ew->disp_dir == 0 ) ew->dir = 12 ;
	else ew->dir = 4 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	if( !ew->type ){
		SHL1D_05_LW( ew )->atck_on_flag = 0 ;
		SHL1D_05_LW( ew )->first_flag = 0 ;
		SHL1D_05_LW( ew )->atck_cnt = 2 ;
		pl_char_set( ew, 22, 29 ) ;
	} else {
		if( SHL1D_05_LW( ew )->first_flag ) pl_char_set( ew, 22, 42 ) ;
		else pl_char_set( ew, 22, 41 ) ;
	}
	shl1d_05_move( ew ) ;
}

static void shl1d_05_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_05_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*--------------------------------------------------------------*/
static void shl1d_05_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->type ) shl1d_05_head( ew, pl_w ) ;
	else shl1d_05_part( ew, pl_w ) ;
}

static void shl1d_05_head_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_05_head_make( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl1d_05_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The head lasts while Roll is in the hyper			*/
/*			combo's animations									*/
/*==============================================================*/
static void shl1d_05_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_w->char_no.w != 0x1603 && pl_w->char_no.w != 0x1601 && pl_w->char_no.w != 0x1602 && pl_w->char_no.w != 0x161b ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( !ew->routine_1 ) shl1d_05_head_start( ew, pl_w ) ;
	else shl1d_05_head_make( ew, pl_w ) ;
	shl1d_05_pos_set( ew, pl_w ) ;
}

/*==============================================================*/
/*			The first part is made when the head's animation	*/
/*			has no code to pass									*/
/*==============================================================*/
static void shl1d_05_head_start( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *part_w ;

	pl_char_move( ew ) ;
	if( !ew->cg_type ){
		ew->routine_1 ++ ;
		if( ( part_w = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			part_w->work_func = shl1d_05_main ;
			part_w->master_w = ew->master_w ;
			part_w->type = 1 ;
			SHL1D_05_LW( part_w )->atck_on_flag = 0xff ;
			SHL1D_05_LW( part_w )->first_flag = 0xff ;
			part_w->ef_w = ew ;
		}
	}
}

/*==============================================================*/
static void shl1d_05_head_make( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *part_w ;

	pl_char_move( ew ) ;
	if( ( part_w = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
		part_w->work_func = shl1d_05_main ;
		part_w->master_w = ew->master_w ;
		part_w->type = 1 ;
		SHL1D_05_LW( part_w )->first_flag = 0 ;
		part_w->ef_w = ew ;
		SHL1D_05_LW( part_w )->atck_on_flag = 0 ;
		if( ! -- SHL1D_05_LW( ew )->atck_cnt ){
			SHL1D_05_LW( ew )->atck_cnt = 5 ;
			SHL1D_05_LW( part_w )->atck_on_flag = 0xff ;
		}
	}
}

static void shl1d_05_char_set( PLAYER_WORK *ew, PLAYER_WORK *ef_w );
static void shl1d_05_part_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *bw );

/*==============================================================*/
/*			A part follows the one before it and attacks		*/
/*			while it has hit nothing							*/
/*==============================================================*/
static void shl1d_05_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ef_w = ew->ef_w ;
	PLAYER_WORK *bw = ew->before_w ;
	SHL1D_05_LOCAL_W *lw ;

	if( ef_w->routine_0 >= 2 || bw->routine_0 >= 2 ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl1d_05_part_pos_set( ew, pl_w, bw ) ;
	shl1d_05_char_set( ew, ef_w ) ;
	ew->type ++ ;
	if( !scr_in_x_ck( ew ) ){
		ew->routine_0 = 3 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( ew->hit_box_no ){
		ew->hit_box_no = 0 ;
		SHL1D_05_LW( ew )->atck_on_flag = 0 ;
		return ;
	}
	if( ew->hit_flag ){
		ew->hit_flag = 0 ;
		SHL1D_05_LW( ew )->atck_on_flag = 0 ;
		return ;
	}
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	lw = SHL1D_05_LW( ew ) ;
	if( !lw->first_flag && !lw->atck_on_flag ) ew->atck_flag = 0 ;
	ew->atck_no = pl_w->atck_no ;
	HIT_RESET( ew ) ;
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1d_05_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1d_05_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1d_05_char_set( PLAYER_WORK *ew, PLAYER_WORK *ef_w )
{
	if( SHL1D_05_LW( ew )->first_flag ) pl_char_set2( ew, 22, 42, ef_w->cg_type & 1 ) ;
	else pl_char_set2( ew, 22, 41, ef_w->cg_type ) ;
}

/*==============================================================*/
static void shl1d_05_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x = pl_w->pos.x + ( !pl_w->disp_dir ? -0.0f : 0.0f ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( 128.0f ) ;
}

/*==============================================================*/
/*			The part sits behind the one before it				*/
/*==============================================================*/
static void shl1d_05_part_pos_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w, PLAYER_WORK *bw )
{
	Float x ;

	if( SHL1D_05_LW( ew )->first_flag ){
		if( !bw->type ) x = -DOT_X( 20.0f ) ;
		else x = -DOT_X( 32.0f ) ;
	} else {
		if( !bw->type ) x = -DOT_X( 50.0f ) ;
		else x = -DOT_X( 32.0f ) ;
	}
	if( !pl_w->disp_dir ) ew->pos.x = bw->pos.x + x ;
	else ew->pos.x = bw->pos.x - x ;
	ew->pos.y = bw->pos.y ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1d_05.c" End of File	*/
