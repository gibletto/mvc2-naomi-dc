/* Akuma's effect 02 (character 1e's shot 02): the sparks of his Tenma Gou Zankuu. Kind 0 stays
   with him and sheds kind 1, which fly off, fall to the ground and strike. */
#include "player.h"
#include "pl_char.h"
#include "pl1e_def.h"

/* the effect's own work, from obj_flag */
typedef struct SHL1E_02_LOCAL_W {
	Uint16	char_no;	/* 000: Akuma's animation when it was set */
} SHL1E_02_LOCAL_W;
#define SHL1E_02_LW( w )	( ( SHL1E_02_LOCAL_W * )&( w )->obj_flag )

static void shl1e_02_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl1e_02_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl1e_02_main ;
		ew->type = 0 ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1e02 ;
		SHL1E_02_LW( ew )->char_no = pl_w->char_no.w ;
	}
	return ew ;
}

/*==============================================================*/
/*			A spark kind 1 of the kind 0 work					*/
/*==============================================================*/
static void shl1e_02_spark_set( PLAYER_WORK *ew )
{
	PLAYER_WORK *spark_w ;

	if( ( spark_w = work_get( 0, W_SHL, LIST_END ) ) ){
		spark_w->work_func = shl1e_02_main ;
		spark_w->type = 1 ;
		spark_w->master_w = ew->master_w ;
		spark_w->pl_type = ew->pl_type ;
		spark_w->ef_no = 0x1e02 ;
		spark_w->ef_w = ew ;
	}
}

static void shl1e_02_dummy( PLAYER_WORK *ew );
static void shl1e_02_init( PLAYER_WORK *ew );
static void shl1e_02_move( PLAYER_WORK *ew );
static void shl1e_02_end( PLAYER_WORK *ew );
static void shl1e_02_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl1e_02_main( PLAYER_WORK *ew )
{
	static void ( *shl1e_02_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_02_init, shl1e_02_move, shl1e_02_end, shl1e_02_free,
	} ;

	shl1e_02_dummy( ew ) ;
	shl1e_02_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl1e_02_init( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w ;
	Uint8 no ;
	Float x ;
	Float y ;

	ew->routine_0 ++ ;
	pl_w = ew->master_w ;
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
	if( !ew->type ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 46.0f ) ;
		x = -DOT_X( 59.0f ) ;
		if( pl_w->pl_dir ) x = DOT_X( 59.0f ) ;
		ew->pos.x += x ;
		x = ( int )( Rnd() & 63 ) * 1.6666666f ;
		y = ( int )( Rnd() & 31 ) * 2.142857f ;
		if( Rnd() & 1 ) x = -x ;
		if( Rnd() & 1 ) y = -y ;
		ew->pos.x += x ;
		ew->pos.y += y ;
		if( Rnd() & 1 ){
			pl_char_set( ew, Char_Effect, 8 ) ;
			ew->prio = 11 ;
			ew->sub_type = 0 ;
		} else {
			pl_char_set( ew, Char_Effect, 9 ) ;
			ew->prio = 13 ;
			ew->sub_type = 1 ;
		}
		return ;
	}
	ew->size_top = ew->size_bottom = 48 ;
	ew->size_front = ew->size_back = 48 ;
	ew->prio = ew->sub_type ? 12 : 10 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	no = 87 + ( Rnd() & 1 ) ;
	ew->atck_no = no ;
	HIT_RESET( ew ) ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->timer0 = 0 ;
	ew->spd.x = ew->disp_dir ? DOT_X( 8.0f ) : -DOT_X( 8.0f ) ;
	ew->spd.y = -DOT_Y( 5.0f ) ;
	ew->acc.x = ew->acc.y = 0.0f ;
	pl_char_set( ew, 21, 16 ) ;
}

static void shl1e_02_0_move( PLAYER_WORK *ew );
static void shl1e_02_spark_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl1e_02_move( PLAYER_WORK *ew )
{
	static void ( *shl1e_02_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl1e_02_0_move, shl1e_02_spark_move,
	} ;

	shl1e_02_move_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
static void shl1e_02_0_move( PLAYER_WORK *ew )
{
	if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl1e_02_end( ew ) ;
	}
	if( ew->cg_type ){
		ew->cg_type = 0 ;
		shl1e_02_spark_set( ew ) ;
	}
}

/*==============================================================*/
static void shl1e_02_hit( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->col_no = ew->master_w->col_no ;
	pl_char_set( ew, 21, 18 ) ;
}

/*==============================================================*/
static void shl1e_02_spark_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;

	if( !ew->routine_1 ){
		if( ew->hit_flag || ew->hit_box_no ){
			shl1e_02_hit( ew ) ;
			return ;
		}
		pl_char_move( ew ) ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		ew->timer0 ^= 1 ;
		ew->col_no = ew->master_w->col_no + ew->timer0 ;
		if( ew->pos.y <= pl_w->ground_y ){
			ew->pos.y = pl_w->ground_y ;
			shl1e_02_hit( ew ) ;
		}
		hit_work_set( ew ) ;
	} else {
		if( pl_char_move( ew ) < 0 ){
			ew->routine_0 ++ ;
			shl1e_02_end( ew ) ;
		}
	}
}

/*--------------------------------------------------------------*/
static void shl1e_02_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	shl1e_02_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl1e_02_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl1e_02_dummy( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1e_02.c" End of File	*/
