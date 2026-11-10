/* The attack that rides on SonSon's giant monkey in its rush (character 14's shot 04): it
   follows her animation from number 38 on, and sets shl14_05 on itself. */
#include "player.h"
#include "pl_char_old.h"
#include "pl14_def.h"

/* where it rides on SonSon, by her animation's number from 38 */
typedef struct SHL14_04_DATA {
	Sint16	x, y;
} SHL14_04_DATA;

/* the effect's own work */
typedef struct SHL14_04_LOCAL_W {
	CHAR_NO	char_no;	/* 000: SonSon's animation, kept up with her */
} SHL14_04_LOCAL_W;
#define SHL14_04_LW( w )	( ( SHL14_04_LOCAL_W * )&( w )->obj_flag )

const SHL14_04_DATA shl14_04_tbl[7] = {	/* by SonSon's animation number - 38 */
	{ 33, 259 }, { 33, 259 }, { 46, 250 }, { 46, 250 }, { 46, 250 }, { 48, 248 }, { 48, 248 }
} ;

static void shl14_04_main( PLAYER_WORK *ew );
static void shl14_04_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew );

/****************************************************************/
/*			Sets the attack up at once							*/
/****************************************************************/
void *shl14_04_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl14_04_main ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x1404 ;
		ew->type = no ;
		ew->master_w = pl_w ;
		SHL14_04_LW( ew )->char_no.w = pl_w->char_no.w ;
		SHL14_04_LW( ew )->char_no.b.grp = pl_w->char_no.b.grp ;
		SHL14_04_LW( ew )->char_no.b.no = pl_w->char_no.b.no ;
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
		ew->size_top = ew->size_bottom = 32 ;
		ew->size_front = ew->size_back = 36 ;
		ew->prio = 0 ;
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y ;
		ew->disp_dir = pl_w->disp_dir ;
		ew->atck_no = pl14_at_enou_rush ;
		HIT_RESET( ew ) ;
		ew->disp_flag = 1 ;
		shl14_04_pos_set( pl_w, ew ) ;
		pl_char_set( ew, 22, pl_w->char_no.b.no + 7 ) ;
		shl14_05_set( pl_w, ew ) ;
	}
	return ew ;
}

static void shl14_04_init( PLAYER_WORK *ew );
static void shl14_04_move( PLAYER_WORK *ew );
static void shl14_04_end( PLAYER_WORK *ew );
static void shl14_04_free( PLAYER_WORK *ew );
static void shl14_04_debug( PLAYER_WORK *ew );

/*==============================================================*/
static void shl14_04_main( PLAYER_WORK *ew )
{
	static void ( *shl14_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_04_init, shl14_04_move, shl14_04_end, shl14_04_free,
	} ;

	if( ew->routine_0 < 2 ) shl14_04_debug( ew ) ;
	shl14_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
/*			The setter has done it all							*/
/*==============================================================*/
static void shl14_04_init( PLAYER_WORK *ew )
{
}

static void shl14_04_ride( PLAYER_WORK *ew );
static void shl14_04_wait( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl14_04_move( PLAYER_WORK *ew )
{
	static void ( *shl14_04_move_jmp[] )( PLAYER_WORK *ew ) = {
		shl14_04_ride, shl14_04_wait,
	} ;

	shl14_04_move_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
/*			Rides on SonSon until she leaves her hyper combo	*/
/*			or reaches its last steps							*/
/*==============================================================*/
static void shl14_04_ride( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL14_04_LOCAL_W *lw = SHL14_04_LW( ew ) ;

	pl_char_move( ew ) ;
	if( pl_w->act_st != N_SUPER ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	if( pl_w->char_no.b.no < 38
	 || ( pl_w->pl_sp_type == pl14_sp_enou && pl_w->routine_2 == 4 )
	 || ( pl_w->pl_sp_type == pl14_sp_enou_auto && pl_w->routine_2 == 8 ) ){
		ew->routine_0 = 2 ;
		ew->disp_flag = 0 ;
		return ;
	}
	shl14_04_pos_set( pl_w, ew ) ;
	if( lw->char_no.b.no != pl_w->char_no.b.no ){
		lw->char_no.b.no = pl_w->char_no.b.no ;
		pl_char_set( ew, 22, lw->char_no.b.no + 7 ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl14_04_wait( PLAYER_WORK *ew )
{
}

/*--------------------------------------------------------------*/
static void shl14_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl14_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
static void shl14_04_debug( PLAYER_WORK *ew )
{
}

/*==============================================================*/
/*			Puts the work on SonSon at her animation's			*/
/*			place, facing her way, drawn first					*/
/*==============================================================*/
static void shl14_04_pos_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew )
{
	Float x ;

	ew->pos.x = pl_w->pos.x ;
	ew->pos.y = pl_w->pos.y ;
	ew->pos.y += shl14_04_tbl[pl_w->char_no.b.no - 38].y ;
	x = shl14_04_tbl[pl_w->char_no.b.no - 38].x ;
	if( !( ew->disp_dir = pl_w->disp_dir ) ) x = -x ;
	ew->pos.x += x ;
	ew->prio = pl_w->prio ;
	ew->sub_prio = -1 ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl14_04.c" End of File	*/
