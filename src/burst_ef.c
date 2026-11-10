/* The bursts of the common code (kinds 5 and 7): four layers of one burst,
   chosen by the flags the caller gives: sparks (0x40), a flash (0x20), a ring (0x10) and a puff of
   smoke (8) that steps through the 16 cells of its texture. */
#include "player.h"
#include "burst_ef.h"

extern void obj_func_set( PLAYER_WORK *w, void ( *func )( PLAYER_WORK *w ) ) ;
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

/* the smoke's models, three scales the code does not use, and the sparks' models */
static Sint32 burst_smoke_objs[2] = { 0x55, 0xa7 } ;
static VEC3 burst_unit_tbl[7] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f },
} ;
static Sint32 burst_spark_obj[4] = { 0x60, 0x61, 0x62, 0x64 } ;

/* the cell of the smoke's texture each step shows, the smoke's fade, and its length in frames */
static const Float burst_smoke_uv[32] = {
	0.0f, 0.0f, 0.25f, 0.0f, 0.5f, 0.0f, 0.75f, 0.0f,
	0.0f, 0.25f, 0.25f, 0.25f, 0.5f, 0.25f, 0.75f, 0.25f,
	0.0f, 0.5f, 0.25f, 0.5f, 0.5f, 0.5f, 0.75f, 0.5f,
	0.0f, 0.75f, 0.25f, 0.75f, 0.5f, 0.75f, 0.75f, 0.75f,
} ;
static const Float burst_smoke_fade[16] = {
	1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
	0.87f, 0.74f, 0.61f, 0.48f, 0.35f, 0.2f,
} ;
static const Sint16 burst_smoke_time[16] = { 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2 } ;

static void burst_spark_wait( PLAYER_WORK *ew ) ;
static void burst_spark_in( PLAYER_WORK *ew ) ;
static void burst_spark_move( PLAYER_WORK *ew ) ;
static void burst_spark_out( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void burst_spark_main( PLAYER_WORK *ew )
{
	static void ( *burst_spark_jmp[] )( PLAYER_WORK *ew ) = {
		burst_spark_wait, burst_spark_in, burst_spark_move, burst_spark_out,
	} ;

	burst_spark_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void burst_spark_wait( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	if( -- ew->timer0 == 0 ){
		ew->disp_flag = 1 ;
		ew->routine_0 ++ ;
		ew->timer0 = 4 ;
	}
}

/*--------------------------------------------------------------*/
static void burst_spark_in( PLAYER_WORK *ew )
{
	ew->fade.trnsl += 0.25f ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 8 ;
	}
}

/*--------------------------------------------------------------*/
static void burst_spark_move( PLAYER_WORK *ew )
{
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 == 0 ){
		ew->routine_0 ++ ;
		ew->timer0 = 2 ;
	}
}

/*--------------------------------------------------------------*/
static void burst_spark_out( PLAYER_WORK *ew )
{
	ew->fade.trnsl -= 0.5f ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->pos.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( -- ew->timer0 == 0 ) work_free( ew ) ;
}

/*==============================================================*/
/*			I-th of n bursts: sixteen sparks, each of the		*/
/*			four models four times, that fly out and slow		*/
/*==============================================================*/
static void burst_spark_set( PLAYER_WORK *pl_w, VEC3 *pos, Float scale, int n, int i, Uint16 flags )
{
	PLAYER_WORK *ew ;
	int j, k ;

	if( !( flags & 0x40 ) ) return ;
	if( n > 1 && !( i % 2 ) ) return ;
	for( j = 0 ; j < 4 ; j ++ ){
		for( k = 0 ; k < 4 ; k ++ ){
			if( flags & 0x200 ){
				if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
			} else {
				if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) return ;
			}
			ew->disp_flag = 0 ;
			ew->work_func = burst_spark_main ;
			ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL | OBJ_FADE ;
			ew->pos = *pos ;
			ew->timer0 = 4 ;
			ew->fade.trnsl = 0.0f ;
			ew->fade.r = 1.0f ;
			ew->fade.g = 0.0f ;
			ew->fade.b = 0.0f ;
			ew->obj = obj_efky->obj[burst_spark_obj[j]] ;
			ew->spd.x = ( nlRandom() - 0.5f ) * 30.0f ;
			ew->spd.y = ( nlRandom() - 0.5f ) * 30.0f ;
			ew->spd.z = nlRandom() * 15.0f ;
			ew->acc.x = -ew->spd.x / 14.0f ;
			ew->acc.y = -ew->spd.y / 14.0f ;
			ew->acc.z = -ew->spd.z / 14.0f ;
			ew->scale.x = scale ;
			ew->scale.y = scale ;
			ew->scale.z = scale ;
			ew->sub_no = i ;
			ew->work1 = i * 8 ;
		}
	}
}

/*==============================================================*/
static void burst_flash_main( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->scale.x += 0.2777778f * ew->work2 ;
	ew->scale.z += 0.2777778f * ew->work2 ;
	if( -- ew->timer0 == 0 ){
		ew->timer0 = 1 ;
		ew->fade.r -= 0.25f ;
		ew->fade.g -= 0.25f ;
		ew->fade.b -= 0.25f ;
	}
	if( -- ew->timer1 == 0 ) work_free( ew ) ;
}

/*==============================================================*/
static void burst_flash_set( PLAYER_WORK *pl_w, VEC3 *pos, Float scale, int n, int i, Uint16 flags )
{
	PLAYER_WORK *ew ;

	if( !( flags & 0x20 ) ) return ;
	if( n > 1 ){
		if( i != n - 1 ) return ;
	}
	if( flags & 0x200 ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	} else {
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) return ;
	}
	ew->disp_flag = 0 ;
	ew->work_func = burst_flash_main ;
	ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_FADE ;
	ew->timer0 = 14 ;
	ew->timer1 = 18 ;
	ew->fade.r = 1.0f ;
	ew->fade.g = 1.0f ;
	ew->fade.b = 1.0f ;
	ew->obj = obj_efky->obj[183] ;
	ew->scale.x = scale ;
	ew->scale.y = scale ;
	ew->scale.z = scale ;
	ew->sub_no = i ;
	if( flags & 2 ){
		ew->pos.x = pl_w->pos.x ;
		ew->pos.y = pl_w->pos.y + pl_w->size_top / 8 * pl_w->scale.y * DOT_Y( 1.0f ) ;
	} else {
		ew->pos = *pos ;
	}
	ew->work1 = i * 8 ;
	ew->work2 = ( int )scale ;
}

/*==============================================================*/
static void burst_ring_main( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 = ew->work1 - 1 ;
		return ;
	}
	ew->disp_flag = 1 ;
	ew->scale.x += 0.46666667f * ew->work2 ;
	ew->scale.y += 0.46666667f * ew->work2 ;
	if( -- ew->timer0 == 0 ){
		ew->timer0 = 1 ;
		ew->fade.trnsl -= 0.083333336f ;
	}
	if( -- ew->timer1 == 0 ) work_free( ew ) ;
}

/*==============================================================*/
static void burst_ring_set( PLAYER_WORK *pl_w, VEC3 *pos, Float scale, int n, int i, Uint16 flags )
{
	PLAYER_WORK *ew ;

	if( !( flags & 0x10 ) ) return ;
	if( n > 1 ){
		if( i != n - 1 ) return ;
	}
	if( flags & 0x200 ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ) return ;
	} else {
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ) return ;
	}
	ew->disp_flag = 0 ;
	ew->work_func = burst_ring_main ;
	ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	ew->pos = *pos ;
	ew->timer0 = 9 ;
	ew->timer1 = 15 ;
	ew->fade.trnsl = 0.5f ;
	ew->obj = obj_efky->obj[182] ;
	ew->scale.x = scale ;
	ew->scale.y = scale ;
	ew->scale.z = scale ;
	ew->sub_no = i ;
	ew->work1 = i * 8 ;
	ew->work2 = ( int )scale ;
}

static void burst_smoke_next( PLAYER_WORK *ew ) ;
static void burst_smoke_wait( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void burst_smoke_main( PLAYER_WORK *ew )
{
	static void ( *burst_smoke_jmp[] )( PLAYER_WORK *ew ) = {
		burst_smoke_next, burst_smoke_wait,
	} ;

	burst_smoke_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void burst_smoke_next( PLAYER_WORK *ew )
{
	if( ew->work1 ){
		ew->work1 -- ;
		return ;
	}
	if( ew->timer0 ){
		const Float *fade = burst_smoke_fade ;
		const Sint16 *time = burst_smoke_time ;

		ew->obj = obj_efky->obj[burst_smoke_objs[1]] ;
		time += ew->timer0 - 1 ;
		ew->timer1 = *time ;
		fade += ew->timer0 - 1 ;
		ew->fade.trnsl = *fade ;
	}
	ew->routine_0 ++ ;
	burst_smoke_wait( ew ) ;
}

/*--------------------------------------------------------------*/
static void burst_smoke_wait( PLAYER_WORK *ew )
{
	if( -- ew->timer1 == 0 ){
		ew->routine_0 = 0 ;
		if( ++ ew->timer0 >= 17 ){
			vs_sys_work.burst_cnt -- ;
			work_free( ew ) ;
		}
	}
}

static void burst_smoke_disp( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void burst_smoke_set( PLAYER_WORK *pl_w, VEC3 *pos, Float scale, int n, int i, Uint16 flags )
{
	PLAYER_WORK *ew ;

	if( !( flags & 8 ) ){
		vs_sys_work.burst_cnt -- ;
		return ;
	}
	if( flags & 0x200 ){
		if( !( ew = work_get( 0, W_SET, LIST_END ) ) ){
			vs_sys_work.burst_cnt -- ;
			return ;
		}
	} else {
		if( !( ew = work_get( 0, W_EF, LIST_END ) ) ){
			vs_sys_work.burst_cnt -- ;
			return ;
		}
	}
	ew->disp_flag = 1 ;
	ew->work_func = burst_smoke_main ;
	ew->obj_flag = OBJ_POS | OBJ_SCALE | OBJ_TRNSL ;
	ew->pos = *pos ;
	ew->timer0 = 0 ;
	ew->timer1 = 3 ;
	ew->obj = obj_efky->obj[burst_smoke_objs[0]] ;
	ew->scale.x = scale ;
	ew->scale.y = scale ;
	ew->scale.z = scale ;
	ew->sub_no = i ;
	ew->work1 = i * 8 ;
	obj_func_set( ew, burst_smoke_disp ) ;
}

/*==============================================================*/
/*			The smoke's texture slides to the cell of its		*/
/*			step												*/
/*==============================================================*/
static void burst_smoke_disp( PLAYER_WORK *ew )
{
	Float x, y ;
	const Float *uv = burst_smoke_uv ;
	const Float *uy ;

	if( ew->timer0 ){
		obj_uv_begin( ( obj_efky->obj + burst_smoke_objs[1] )[1], ew->obj ) ;
		uv += ( ew->timer0 - 1 ) * 2 ;
		uy = uv + 1 ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &x, &y ) ;
			x += *uv ;
			y -= *uy ;
			obj_uv_put( &x, &y ) ;
		}
	}
}

/****************************************************************/
/*			A burst that does nothing: shl37_01 calls it		*/
/*			where a burst would be								*/
/****************************************************************/
void burst_none( VEC3 *pos, int no )
{
}

/****************************************************************/
/*			N bursts of the layers the flags name, at pos		*/
/*			or, with the 2 flag, at the player's body; the		*/
/*			flags 4, 0x100 and 0x400 pick the size and 0x200	*/
/*			puts them in kind 5; vs_sys_work.burst_cnt			*/
/*			counts the bursts alive, at most six				*/
/****************************************************************/
void burst_set( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 n, Uint16 flags )
{
	VEC3 p ;
	Float f ;
	int i, r, ofs ;

	if( vs_sys_work.burst_cnt > 6 ) return ;
	else {
		if( vs_sys_work.burst_cnt + n > 6 ) n = 6 - vs_sys_work.burst_cnt ;
		vs_sys_work.burst_cnt += n ;
		for( i = 0 ; i < n ; i ++ ){
			if( flags & 2 ){
				p = pl_w->pos ;
				r = nlRand() ;
				ofs = ( int )( pl_w->size_front * pl_w->scale.x * 1.6666666f ) ;
				p.x += r % 2 ? r % ofs : -( r % ofs ) ;
				r = nlRand() ;
				ofs = ( int )( pl_w->size_top * pl_w->scale.y * DOT_Y( 1.0f ) ) ;
				p.y += r % ofs ;
				if( flags & 4 ){
					r = nlRand() ;
					f = r % 2 ? 1.0f : 1.5f ;
				} else if( flags & 0x100 ){
					f = 1.5f ;
				} else if( flags & 0x400 ){
					f = 0.6f ;
				} else {
					f = 1.0f ;
				}
				if( flags & 0x200 ) f *= 10.0f ;
				burst_smoke_set( pl_w, &p, f, n, i, flags ) ;
				burst_ring_set( pl_w, &p, f, n, i, flags ) ;
				burst_flash_set( pl_w, &p, f, n, i, flags ) ;
				burst_spark_set( pl_w, &p, f, n, i, flags ) ;
			} else {
				if( flags & 4 ){
					r = nlRand() ;
					f = r % 2 ? 1.0f : 1.5f ;
				} else if( flags & 0x100 ){
					f = 1.5f ;
				} else if( flags & 0x400 ){
					f = 0.6f ;
				} else {
					f = 1.0f ;
				}
				if( flags & 0x200 ) f *= 10.0f ;
				burst_smoke_set( pl_w, pos, f, n, i, flags ) ;
				burst_ring_set( pl_w, pos, f, n, i, flags ) ;
				burst_flash_set( pl_w, pos, f, n, i, flags ) ;
				burst_spark_set( pl_w, pos, f, n, i, flags ) ;
			}
		}
	}
}

/* one full burst at pos, big or not */
/****************************************************************/
/*			No is a Uint8 here, but its callers were built		*/
/*			against an int (burst_ef_old.h)						*/
/****************************************************************/
void burst_pos_set( VEC3 *pos, Uint8 no )
{
	Float f ;

	if( vs_sys_work.burst_cnt >= 6 ) return ;
	vs_sys_work.burst_cnt ++ ;
	if( no ) f = 1.5f ;
	else f = 1.0f ;
	burst_smoke_set( 0, pos, f, 1, 0, 121 ) ;
	burst_ring_set( 0, pos, f, 1, 0, 121 ) ;
	burst_flash_set( 0, pos, f, 1, 0, 121 ) ;
	burst_spark_set( 0, pos, f, 1, 0, 121 ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "burst_ef.c" End of File	*/
