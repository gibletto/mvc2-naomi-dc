/* Thanos's effect 00 (character 36's shot 00): the energy that gathers in front of him in his Power
   Gem hyper combo. A head (type 0) swells and flies out while five parts (type 1) hold their places
   around it. */
#include "player.h"
#include "ef_char.h"
#include "pl_char.h"
#include "o_sub1.h"
#include "pl36_def.h"

/* where the head stands in front of him, by the animation's code (x and y from his position) */
typedef struct SHL36_00_OFS {
	Sint16	x;
	Sint16	y;
} SHL36_00_OFS;

/* the head's own work, from obj_flag: the hits it has left, set once it has hit, and the time of the
   hit's repeats (the top three bits turn over every eighth frame) */
typedef struct SHL36_00_LOCAL_W {
	Sint32	cnt[3];
} SHL36_00_LOCAL_W;
#define SHL36_00_LW( w )	( ( SHL36_00_LOCAL_W * )&( w )->obj_flag )

/* a part's head is kept in work3; the head's own work3 says the coil is ending */
#define SHL36_00_HEAD( w )	( ( PLAYER_WORK * )( w )->work3 )

static SHL36_00_OFS shl36_00_ofs[5] = {
	{ 0, 0 },
	{ 18, 152 },
	{ 64, 120 },
	{ 32, 64 },
	{ -96, 64 },
} ;

static void shl36_00_main( PLAYER_WORK *ew );

/****************************************************************/
PLAYER_WORK *shl36_00_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->be_flag = 1 ;
		ew->work_func = shl36_00_main ;
		ew->master_w = pl_w ;
		ew->pl_type = pl_w->pl_type ;
		ew->ef_no = 0x3600 ;
		ew->type = 0 ;
		ew->work3 = 0 ;
	}
	return ew ;
}

/*==============================================================*/
static PLAYER_WORK *shl36_00_part_set( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *w ;
	Uint8 i ;

	for( i = 0 ; i < 5 ; i ++ ){
		if( ( w = work_get( ew, W_SHL, LIST_BEHIND ) ) ){
			w->be_flag = 1 ;
			w->work_func = shl36_00_main ;
			w->master_w = pl_w ;
			w->work3 = ( Sint32 )ew ;
			w->pl_type = pl_w->pl_type ;
			w->ef_no = 0x3600 ;
			w->type = 1 ;
			w->sub_type = i ;
		}
	}
	return w ;
}

static void shl36_00_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_00_main( PLAYER_WORK *ew )
{
	if( !ew->type ){
		shl36_00_head( ew, ew->master_w ) ;
	}
	else {
		shl36_00_part( ew, ew->master_w ) ;
	}
}

static void shl36_00_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_00_head( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_00_head_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_00_head_init, shl36_00_head_move, shl36_00_end, shl36_00_free,
	} ;

	shl36_00_head_jmp[ew->routine_0]( ew, pl_w ) ;
}

/*==============================================================*/
static void shl36_00_head_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Float x, y ;

	ew->routine_0 ++ ;
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
	ew->prio = 0 ;
	SHL36_00_LW( ew )->cnt[0] = 6 ;
	SHL36_00_LW( ew )->cnt[1] = 0 ;
	SHL36_00_LW( ew )->cnt[2] = 0 ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl36_at_power ;
	HIT_RESET( ew ) ;
	ew->size_top = ew->size_bottom = ew->size_front = ew->size_back = 0xf0 ;
	ew->scale.x = ew->scale.y = 0.25f ;
	x = shl36_00_ofs[1].x ;
	y = shl36_00_ofs[1].y ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = pl_w->pos.x + DOT_X( x ) ;
	ew->pos.y = pl_w->pos.y + DOT_Y( y ) ;
	pl_char_set( ew, 22, 13 ) ;
	shl36_00_part_set( ew, pl_w ) ;
	pl_voice_req( ew, 36 ) ;
	shl36_00_head_move( ew, pl_w ) ;
}

static void shl36_00_head_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_head_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_head_homing( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_head_go( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The head follows his animation, grows while it		*/
/*			comes out and flies; it stops short if he is		*/
/*			hit. Each eighth frame of a hit takes one of its	*/
/*			hits, and the last ends it							*/
/*==============================================================*/
static void shl36_00_head_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_00_move_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_00_head_grow, shl36_00_head_fly, shl36_00_head_homing, shl36_00_head_go,
	} ;

	PL36_LW( pl_w )->hyper_timer = 4 ;
	if( ew->hit_flag ){
		SHL36_00_LW( ew )->cnt[1] = 1 ;
		if( !( SHL36_00_LW( ew )->cnt[2] += 0x20000000 ) ){
			if( -- SHL36_00_LW( ew )->cnt[0] > 0 ){
				ew->atck_no = pl36_at_power ;
				ew->hit_flag = ew->atck_attr = 0 ;
				ew->unk_1c4 = 0 ;
				vs_sys_w->atck_cnt[ew->pl_no] ++ ;
			}
			else {
				ew->work3 = 1 ;
				ew->spd.x /= 2.0f ;
				ew->spd.y /= 2.0f ;
			}
		}
	}
	if( pl_w->routine_1 ){
		ew->work3 = 1 ;
		ew->spd.x /= 2.0f ;
		ew->spd.y /= 2.0f ;
	}
	if( ew->work3 ){
		ew->disp_flag ^= 1 ;
		ew->scale.x -= 0.05f ;
		if( ew->scale.x < 0.01f ){
			shl36_00_end( ew, pl_w ) ;
			return ;
		}
		ew->scale.y = ew->scale.x ;
	}
	if( ew->hit_stop ){
		shl36_00_anim( ew, pl_w ) ;
		if( -- ew->hit_stop ) return ;
	}
	shl36_00_move_jmp[ew->routine_1]( ew, pl_w ) ;
}

static void shl36_00_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			It comes out of his hand, following the hold		*/
/*			position of his animation							*/
/*==============================================================*/
static void shl36_00_head_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	int spd ;
	Uint8 dir ;
	SHL36_00_OFS *ofs ;
	int no ;
	int x, y ;

	if( pl_w->routine_1 ){
		shl36_00_end( ew, pl_w ) ;
		return ;
	}
	if( pl_w->cg_type < 0 ){
		ew->routine_1 ++ ;
		pl_w->cg_type = 4 ;
		ew->scale.x = 1.0f ;
		ew->timer0 = 16 ;
		ew->spd.x = 0.0f ;
		ew->spd.y = 0.0f ;
		ew->acc.x = 0.0f ;
		ew->acc.y = 0.0f ;
		spd = 0x71600 ;
		dir = 8 ;
		if( !pl_w->disp_dir ){
			spd = -0x71600 ;
			dir = 24 ;
		}
		ew->spd.x = spd * 1.6666666f / 65536.0f ;
		ew->dir = dir ;
	}
	no = pl_w->cg_type ;
	ew->scale.x += 0.1f ;
	if( ew->scale.x > 1.0f ) ew->scale.x = 1.0f ;
	ew->scale.y = ew->scale.x ;
	ofs = &shl36_00_ofs[no] ;
	x = ofs->x ;
	y = ofs->y ;
	if( pl_w->disp_dir ) x = -x ;
	ew->pos.x = ( ew->pos.x + pl_w->pos.x + DOT_X( x ) ) / 2.0f ;
	ew->pos.y = ( ew->pos.y + pl_w->pos.y + DOT_Y( y ) ) / 2.0f ;
}

/*==============================================================*/
/*			It flies on by its speed until its time is up		*/
/*==============================================================*/
static void shl36_00_head_fly( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 48 ;
		ew->timer1 = 0 ;
	}
	shl36_00_hit( ew, pl_w ) ;
}

/*==============================================================*/
/*			It turns toward the opponent while its time			*/
/*			lasts												*/
/*==============================================================*/
static void shl36_00_head_homing( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	VEC3 pos ;
	Uint8 dir, d ;

	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !-- ew->timer0 ) ew->routine_1 ++ ;
	if( !SHL36_00_LW( ew )->cnt[1] ){
		if( !( ew->timer1 += 0x4000 ) ){
			pos = pl_w->target_w->pos ;
			pos.y += DOT_Y( 64.0f ) ;
			dir = caldir_pos_256( &ew->pos, &pos ) ;
			dir = dir >> 3 ;
			if( dir != ew->dir ){
				ew->dir ++ ;
				d = ( ew->dir - dir ) & 31 ;
				if( d < 16 ) ew->dir -= 2 ;
				ew->dir &= 31 ;
				dir_spd_move( ew, 0x2bc ) ;
			}
		}
	}
	shl36_00_hit( ew, pl_w ) ;
}

/*==============================================================*/
/*			It flies on until it leaves the screen				*/
/*==============================================================*/
static void shl36_00_head_go( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !scr_in_x_ck( ew ) || !scr_in_y_ck( ew ) ){
		ew->routine_0 ++ ;
	}
	shl36_00_hit( ew, pl_w ) ;
}

/*==============================================================*/
/*			The animation of the head, and his colour while		*/
/*			it is in the hit's stop								*/
/*==============================================================*/
static void shl36_00_anim( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	if( pl_char_move( ew ) < 0 ){
		ew->prio = 0 ;
		ef_color_set( pl_w, 3, 8, 1 ) ;
	}
	if( ew->cg_type ){
		ew->prio = 15 ;
		ef_color_set( pl_w, 3, 5, 1 ) ;
	}
}

/*==============================================================*/
static void shl36_00_hit( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	shl36_00_anim( ew, pl_w ) ;
	if( !ew->work3 ) hit_work_set( ew ) ;
}

static void shl36_00_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void shl36_00_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
static void shl36_00_part( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	static void ( *shl36_00_part_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		shl36_00_part_init, shl36_00_part_move, shl36_00_end, shl36_00_free,
	} ;

	shl36_00_part_jmp[ew->routine_0]( ew, pl_w ) ;
}

/* where a part stands about the head: x, y, turned the other way, and the draw priority */
static Sint16 shl36_00_part_tbl[5][4] = {
	{ -96, 48, 0, 15 },
	{ 35, -85, 1, 15 },
	{ 48, -56, 0, 8 },
	{ -48, -40, 0, 8 },
	{ 16, 16, 0, 8 },
} ;

/*==============================================================*/
static void shl36_00_part_init( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	Sint16 *p ;
	PLAYER_WORK *head_w ;
	Sint16 x, y ;

	ew->routine_0 ++ ;
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
	ew->atck_flag = 0 ;
	ew->dm_flag = 0 ;
	p = shl36_00_part_tbl[ew->sub_type] ;
	x = *p ++ ;
	ew->obj_flag = x ;
	y = *p ++ ;
	ew->work1 = y ;
	if( ew->disp_dir ) x = -x ;
	head_w = SHL36_00_HEAD( ew ) ;
	ew->scale = head_w->scale ;
	ew->pos.x = head_w->pos.x + x * head_w->scale.x * 1.6666666f ;
	ew->pos.y = head_w->pos.y + y * head_w->scale.y * 2.142857f ;
	if( *p ++ ) ew->disp_dir ^= 1 ;
	ew->prio = *p ;
	ef_char_set( ew, 27, ew->sub_type ) ;
	shl36_00_part_move( ew, pl_w ) ;
}

/*==============================================================*/
/*			A part keeps its place on the head, and shows		*/
/*			with it												*/
/*==============================================================*/
static void shl36_00_part_move( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PLAYER_WORK *head_w = SHL36_00_HEAD( ew ) ;
	Sint16 x ;

	if( head_w->routine_0 > 1 ){
		shl36_00_end( ew, pl_w ) ;
		return ;
	}
	ew->disp_flag = head_w->disp_flag ;
	x = ew->obj_flag ;
	if( ew->disp_dir ) x = -x ;
	ew->scale = head_w->scale ;
	ew->pos.x = head_w->pos.x + x * head_w->scale.x * 1.6666666f ;
	ew->pos.y = head_w->pos.y + ew->work1 * head_w->scale.y * 2.142857f ;
	if( !head_w->hit_stop ) ef_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl36_00_end( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl36_00_free( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	ew->be_flag = 0 ;
	ew->disp_flag = 0 ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl36_00.c" End of File	*/
