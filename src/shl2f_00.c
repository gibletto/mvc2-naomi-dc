/* Silver Samurai's effect 00 (character 2f's shot 00): the shuriken he throws, in four kinds
   (0 to 3). It flies, bounces off what it hits, and then falls away; the kinds that hit more
   than once give a hit mark at each. */
#include "player.h"
#include "pl_char.h"
#include "pl2f_def.h"

static void shl2f_00_main( PLAYER_WORK *ew );

/****************************************************************/
/* the callers' prototype says int, but the kind is kept as a byte */
void *shl2f_00_set( PLAYER_WORK *pl_w, int no )
{
	PLAYER_WORK *ew ;
	Float ofs ;
	Uint8 type = no ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl2f_00_main ;
		ew->master_w = pl_w ;
		ew->type = type ;
		ew->ef_no = 0x2f00 ;
		ofs = DOT_X( 16.0f ) ;
		if( pl_w->disp_dir ) ofs = -DOT_X( 16.0f ) ;
		ew->pos.x = pl_w->pos.x + ofs ;
		ew->pos.y = pl_w->pos.y + DOT_Y( 64.0f ) ;
	}
	return ew ;
}

static void shl2f_00_init( PLAYER_WORK *ew );
static void shl2f_00_move( PLAYER_WORK *ew );
static void shl2f_00_end( PLAYER_WORK *ew );
static void shl2f_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl2f_00_main( PLAYER_WORK *ew )
{
	static void ( *shl2f_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl2f_00_init, shl2f_00_move, shl2f_00_end, shl2f_00_free,
	} ;

	PL2F_LW( ew->master_w )->shuriken_timer = 2 ;
	shl2f_00_jmp[ew->routine_0]( ew ) ;
}

/* the hit marks' places round the shuriken, by the mark */
static Float shl2f_00_mark_tbl[][2] = {
	{ 0.0f, -20.0f }, { -22.0f, -14.0f }, { -32.0f, 0.0f }, { -22.0f, 14.0f },
	{ 0.0f, 20.0f }, { 22.0f, 14.0f }, { 32.0f, 0.0f }, { 22.0f, -14.0f },
} ;

/*==============================================================*/
static void shl2f_00_init( PLAYER_WORK *ew )
{
	Uint8 dir_tbl[4] = { 0, 6, 16, 26 } ;
	Uint8 atck_tbl[4][4] = {
		{ 62, 48, 49, 50 }, { 52, 66, 67, 68 }, { 52, 66, 67, 68 }, { 52, 66, 67, 68 },
	} ;
	Float spd_tbl[3] = { -DOT_X( 6.5f ), -DOT_X( 8.0f ), -DOT_X( 9.5f ) } ;
	Sint16 time_tbl[3] = { 56, 48, 40 } ;
	Uint8 hits_tbl[8] = { 6, 1, 3, 6, 6, 5, 5, 12 } ;
	PLAYER_WORK *pl_w ;
	Sint16 time ;
	Uint16 no ;
	Float spd ;

	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	ew->size_top = 32 ;
	ew->size_bottom = 40 ;
	ew->size_front = 64 ;
	ew->size_back = 64 ;
	pl_w = ew->master_w ;
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
	ew->prio = 8 ;
	ew->routine_3 = 3 ;
	time = time_tbl[ew->atck_level] ;
	if( ew->type ) time = 64 ;
	if( PL2F_LW( pl_w )->elem == 2 ) time *= 4 ;
	ew->timer0 = time ;
	ew->timer1 = 100 ;
	ew->sub_no = 0 ;
	no = ew->type ;
	if( no ) no = 4 ;
	no += PL2F_LW( pl_w )->elem ;
	ew->sub_type = hits_tbl[no] ;
	spd = spd_tbl[ew->atck_level] ;
	if( PL2F_LW( pl_w )->elem == 1 ) spd *= 1.5f ;
	if( PL2F_LW( pl_w )->elem == 3 ) spd /= 0.75f ;
	if( ew->type ){
		spd = -10.0f ;
		if( PL2F_LW( pl_w )->elem == 1 ) spd = -20.0f ;
		if( PL2F_LW( pl_w )->elem == 3 ) spd /= 0.75f ;
	}
	if( ew->disp_dir ) spd = -spd ;
	ew->spd.x = spd ;
	ew->acc.x = 0.0f ;
	if( !ew->type ) ew->scale.x = ew->scale.y = 0.75f ;
	ew->atck_flag = HIT_ON | HIT_OBJ ;
	ew->dm_flag = HIT_ON | HIT_OBJ ;
	ew->dir = dir_tbl[ew->type] ;
	ew->atck_no = atck_tbl[ew->type][PL2F_LW( pl_w )->elem] ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, 21, 2 ) ;
}

static void shl2f_00_hit( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl2f_00_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Uint8 atck_tbl[4][4] = {
		{ 51, 51, 51, 51 }, { 52, 66, 67, 68 }, { 52, 66, 67, 68 }, { 52, 66, 67, 68 },
	} ;
	Float dy ;
	Float spd_x, acc_x ;
	Sint16 t ;

	if( !ew->routine_1 ){
		if( !scr_in_x_ck( ew ) ){
			shl2f_00_end( ew ) ;
			return ;
		}
		if( ew->hit_box_no || ew->pos.y < pl_w->ground_y ){
			spd_x = DOT_X( 2.0f ) ;
			acc_x = -0.0065104165f ;
			if( ew->disp_dir ){
				spd_x = -DOT_X( 2.0f ) ;
				acc_x = 0.0065104165f ;
			}
			ew->spd.x = spd_x ;
			ew->spd.y = -DOT_Y( 3.0f ) ;
			ew->acc.x = acc_x ;
			ew->acc.y = -DOT_Y( 0.125f ) ;
			ew->routine_2 = 1 ;
			ew->routine_1 ++ ;
			pl_char_set( ew, 21, 3 ) ;
			return ;
		}
		if( !-- ew->timer0 ){
			shl2f_00_hit( ew ) ;
			ew->spd.x = 0.0f ;
			ew->spd.y = 0.0f ;
			ew->acc.x = 0.0f ;
			ew->acc.y = 0.0f ;
			ew->routine_1 ++ ;
			pl_char_set( ew, 21, 3 ) ;
			return ;
		}
		else if( ew->hit_stop ){
			if( ( ew->hit_stop -= 2 ) > 0 ) return ;
			ew->hit_stop = 0 ;
			shl2f_00_hit( ew ) ;
			if( !-- ew->sub_type ){
				ew->spd.x = 0.0f ;
				ew->spd.y = 0.0f ;
				ew->acc.x = 0.0f ;
				ew->acc.y = 0.0f ;
				ew->routine_1 ++ ;
				pl_char_set( ew, 21, 3 ) ;
				return ;
			}
			ew->atck_no = atck_tbl[ew->type][PL2F_LW( pl_w )->elem] ;
			HIT_RESET( ew ) ;
			pl_char_move( ew ) ;
			if( !ew->type ) hit_work_set( ew ) ;
			return ;
		}
		else if( ew->hit_flag ){
			ew->hit_flag = 0 ;
			shl2f_00_hit( ew ) ;
			if( !-- ew->sub_type ){
				ew->spd.x = 0.0f ;
				ew->spd.y = 0.0f ;
				ew->acc.x = 0.0f ;
				ew->acc.y = 0.0f ;
				ew->routine_1 ++ ;
				pl_char_set( ew, 21, 3 ) ;
				return ;
			}
		}
		if( ew->type ){
			ew->dir -- ;
			if( ew->disp_dir ) ew->dir += 2 ;
			ew->dir &= 31 ;
			if( !-- ew->routine_3 ){
				ew->routine_3 = 3 ;
				t = ew->timer1 + 100 ;
				if( ( Uint16 )t > 1000 ) t = 1000 ;
				ew->timer1 = t ;
			}
			dir_move( ew, ew->timer1 ) ;
		}
		if( !pl_w->cpu_flag ){
			dy = 0.0f ;
			if( pl_w->pl_sw_new & KEY_UP ) dy = 3.75f ;
			if( pl_w->pl_sw_new & KEY_DOWN ) dy = -3.75f ;
			ew->pos.y += dy ;
		}
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		pl_char_move( ew ) ;
		hit_work_set( ew ) ;
	}
	else {
		if( ew->routine_2 ) ew->disp_flag ^= 1 ;
		ew->pos.x += ew->spd.x ;
		ew->spd.x += ew->acc.x ;
		ew->pos.y += ew->spd.y ;
		ew->spd.y += ew->acc.y ;
		if( pl_char_move( ew ) < 0 ) ew->routine_0 ++ ;
	}
}

/*==============================================================*/
/*			A hit mark, one place on from the last				*/
/*==============================================================*/
static void shl2f_00_hit( PLAYER_WORK *ew )
{
	VEC3 ofs ;

	if( ew->hit_flag ){
	if( !( ew->hit_flag & ( HR_GUARD | HR_ARMOR ) ) ){
		ofs.x = DOT_X( shl2f_00_mark_tbl[ew->sub_no][0] ) ;
		ofs.y = DOT_Y( shl2f_00_mark_tbl[ew->sub_no][1] ) ;
		hit_mark_set( ew, &ofs, ew->sub_no ++ + 9 ) ;
		ew->sub_no &= 7 ;
	}
	}
}

/*--------------------------------------------------------------*/
static void shl2f_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl2f_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl2f_00.c" End of File	*/
