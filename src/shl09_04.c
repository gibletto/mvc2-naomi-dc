/* Iceman's effect 04 (character 09's shot 04): ice that rides on the point his animation frame
   names (cg_flag1) and hits with his own attack, while his local work's arm_flag is set; it breaks
   after the hits counted in arm_hit_cnt. */
#include "player.h"
#include "pl_char.h"
#include "pl09_def.h"

static Sint16 shl09_04_ofs[330] = {	/* the place, x and y, for each cg_flag1 */
	0, 0, -60, -61, -83, -65, -81, -65, -70, -60, -54, -51, -91, -68, -90, -68,
	16, -96, -66, -74, -35, -53, -66, -65, -70, -66, -74, -52, -77, -87, -76, -83,
	-66, -65, -58, -69, -58, -69, -80, -12, -68, -51, -67, -51, -66, -51, -47, -43,
	-43, -37, -41, -34, -12, -38, 0, -37, 17, -36, 15, -41, 17, -41, 1, -39,
	-10, -39, -41, -44, -50, -49, -27, -51, -26, -47, -11, -45, -1, -45, 10, -46,
	9, -46, -9, -47, -21, -51, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54,
	-26, -54, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54, -26, -54,
	-48, -37, -58, -58, -55, -50, -49, -21, 15, -58, 15, -63, 27, -49, 31, -57,
	-19, -43, -40, -43, -47, -51, -41, -57, 59, -78, 21, -72, -46, -47, 38, -63,
	-42, -46, 1, -44, 26, -49, -31, -54, -67, -51, 0, 0, -15, -31, -38, -24,
	-29, -22, 0, 0, 0, 0, 0, 0, -27, -28, -11, -129, -42, -109, -59, -74,
	-26, -55, -45, -60, -45, -107, -12, -131, -32, -25, -60, -54, -56, -38, -43, -30,
	-21, -14, -53, -15, -47, -11, -25, -17, 19, -18, 66, -77, 43, -118, -44, -118,
	-26, -27, -15, -128, 27, -122, 68, -75, 8, -19, -48, -36, -62, -21, -62, -19,
	-24, -15, -44, -27, -54, -34, -62, -54, -30, -16, -15, -128, -15, -129, -15, -128,
	-15, -129, -12, -129, -11, -130, -12, -129, -12, -130, -16, -127, -16, -128, -16, -127,
	-16, -128, -13, -128, -13, -130, -13, -128, -13, -130, -15, -126, -15, -128, -15, -126,
	-15, -126, -15, -126, -15, -128, -15, -126, -15, -128, 50, -101, -97, -38, -97, -38,
	-97, -38, -97, -38, 6, -56, 51, -99, -87, -43, -86, -44, 10, -56, 7, -144,
	-113, -21, -113, -21, 71, -137, 71, -137, 7, -56,
} ;

static void shl09_04_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl09_04_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	ew = work_get( 0, W_SHL, LIST_TOP ) ;
	ew->work_func = shl09_04_main ;
	ew->master_w = pl_w ;
	ew->ef_no = 0x904 ;
	ew->timer0 = pl_w->char_no.w ;
	return ew ;
}

static void shl09_04_init( PLAYER_WORK *ew );
static void shl09_04_move( PLAYER_WORK *ew );
static void shl09_04_end( PLAYER_WORK *ew );
static void shl09_04_free( PLAYER_WORK *ew );
static void shl09_04_atck_set( PLAYER_WORK *ew, PLAYER_WORK *mw );

/*==============================================================*/
static void shl09_04_main( PLAYER_WORK *ew )
{
	static void ( *shl09_04_jmp[] )( PLAYER_WORK *ew ) = {
		shl09_04_init, shl09_04_move, shl09_04_end, shl09_04_free,
	} ;

	shl09_04_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void shl09_04_init( PLAYER_WORK *ew )
{
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
	ew->prio = 0 ;
	ew->atck_flag = HIT_ON | HIT_OBJ | HIT_OWNER ;
	ew->dm_flag = 0 ;
	ew->routine_2 = 0 ;
	ew->timer1 = 1 ;
	pl_char_set2( ew, 20, 3, ( Sint32 )Rnd() % 18 ) ;
	shl09_04_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl09_04_move( PLAYER_WORK *ew )
{
	PLAYER_WORK *mw = ew->master_w ;
	PL09_LOCAL_W *lw = ( PL09_LOCAL_W * )mw->tlocal ;
	Float x ;

	if( !ew->routine_1 ){
		if( ew->hit_flag ){
			ew->routine_2 ++ ;
			ew->hit_flag = 0 ;
			if( -- lw->arm_hit_cnt <= 0 ){
				ew->routine_1 ++ ;
				ew->disp_flag = 1 ;
				pl_char_set( ew, 20, 4 ) ;
			}
		}
		if( !lw->arm_flag ){
			ew->routine_0 ++ ;
			shl09_04_end( ew ) ;
			return ;
		}
		if( ew->master_w->cg_flag1 ){
			ew->disp_flag = 1 ;
			ew->pos.x = ew->master_w->pos.x ;
			ew->pos.y = ew->master_w->pos.y ;
			x = DOT_X( shl09_04_ofs[ew->master_w->cg_flag1 * 2] ) ;
			if( ew->master_w->disp_dir ) x = -x ;
			ew->pos.x += x ;
			ew->pos.y -= DOT_Y( shl09_04_ofs[ew->master_w->cg_flag1 * 2 + 1] ) ;
		} else {
			ew->disp_flag = 0 ;
		}
		if( ew->master_w->char_no.w != ew->timer0 ){
			ew->routine_0 ++ ;
			shl09_04_end( ew ) ;
			return ;
		}
		if( ew->disp_flag && !ew->routine_2 ){
			if( !ew->timer1 ){
				if( ew->master_w->cg_flag2 ){
					shl09_04_atck_set( ew, mw ) ;
					hit_work_set( ew ) ;
				}
			} else {
				ew->timer1 -- ;
			}
		}
	} else if( pl_char_move( ew ) < 0 ){
		ew->routine_0 ++ ;
		shl09_04_end( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void shl09_04_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
	shl09_04_free( ew ) ;
}

/*--------------------------------------------------------------*/
static void shl09_04_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*			The attack, from the player's own					*/
/*==============================================================*/
static void shl09_04_atck_set( PLAYER_WORK *ew, PLAYER_WORK *mw )
{
	switch( mw->atck_no ){
	case 0 :
	case 6 :
		ew->atck_no = pl09_at_fist_lp ;
		break ;
	case 1 :
	case 7 :
		ew->atck_no = pl09_at_fist_mp ;
		break ;
	case 2 :
	case 8 :
		ew->atck_no = pl09_at_fist_hp ;
		break ;
	case 63 :
		ew->atck_no = pl09_at_fist_jlp ;
		break ;
	case 13 :
		ew->atck_no = pl09_at_fist_jmp ;
		break ;
	case 14 :
		ew->atck_no = pl09_at_fist_jhp ;
		break ;
	}
	HIT_RESET( ew ) ;
	mw->atck_no = ew->atck_no ;
	HIT_RESET( mw ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl09_04.c" End of File	*/
