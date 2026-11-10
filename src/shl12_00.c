/* Hayato's effects (character 12's shot 00): one module for eleven kinds, picked by the setter's
   number (type), each with its own start and moves. Parts set by another effect ride on it (ef_w). */
#include "player.h"
#include "pl_char.h"
#include "pl12_def.h"

static Sint16 shl12_00_time = 20 ;	/* the frames the Plasma Field grows for */

/* where kinds 3 and 5 start from Hayato, x and y by sub_type */
static Float shl12_00_ofs[2][8] = {
	{ DOT_X( 112.0f ), 120.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f },
	{ 160.0f, DOT_Y( 48.0f ), 0.0f, DOT_Y( 64.0f ), 0.0f, 0.0f, 0.0f, 0.0f },
} ;

/* where kinds 4 and 6 sit from the effect they ride on, by sub_no */
static Float shl12_00_pos[] = {
	0.0f, 213.0f, 426.0f, 639.0f, -213.0f, -426.0f, -639.0f,
} ;

static void shl12_00_main( PLAYER_WORK *ew );

/****************************************************************/
void *shl12_00_set( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl12_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
	}
	return ew ;
}

/****************************************************************/
void *shl12_00_set2( PLAYER_WORK *pl_w, Uint8 no, Uint8 a, Uint8 b )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl12_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_type = b ;
		ew->sub_no = a ;
	}
	return ew ;
}

/****************************************************************/
void *shl12_00_set3( PLAYER_WORK *pl_w, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl12_00_main ;
		ew->master_w = pl_w ;
		ew->type = no ;
		ew->sub_no = ew->master_w->sub_no ;
		ew->disp_dir = ew->master_w->disp_dir ;
		ew->pos.x = ew->master_w->pos.x ;
		ew->pos.y = ew->master_w->pos.y ;
		ew->pos.z = ew->master_w->pos.z ;
		ew->pos.x += ew->disp_dir ? 176 : -176 ;
	}
	return ew ;
}

/*==============================================================*/
/*			A part that rides on the effect pw					*/
/*==============================================================*/
static void *shl12_00_part_set( PLAYER_WORK *pw, Uint8 no, Uint8 a )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl12_00_main ;
		ew->master_w = pw->master_w ;
		ew->ef_w = pw ;
		ew->type = no ;
		ew->sub_no = a ;
		ew->disp_dir = ew->ef_w->disp_dir ;
	}
	return ew ;
}

/*==============================================================*/
/*			The same, starting where pw is						*/
/*==============================================================*/
static void *shl12_00_part_set2( PLAYER_WORK *pw, Uint8 no, Uint8 a )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_SHL, LIST_TOP ) ) ){
		ew->work_func = shl12_00_main ;
		ew->master_w = pw->master_w ;
		ew->ef_w = pw ;
		ew->type = no ;
		ew->sub_no = a ;
		ew->disp_dir = ew->ef_w->disp_dir ;
		ew->pos.x = ew->ef_w->pos.x ;
		ew->pos.y = ew->ef_w->pos.y ;
		ew->pos.z = ew->ef_w->pos.z ;
	}
	return ew ;
}

static void shl12_00_init( PLAYER_WORK *ew );
static void shl12_00_move( PLAYER_WORK *ew );
static void shl12_00_end( PLAYER_WORK *ew );
static void shl12_00_free( PLAYER_WORK *ew );

/*==============================================================*/
static void shl12_00_main( PLAYER_WORK *ew )
{
	static void ( *shl12_00_jmp[] )( PLAYER_WORK *ew ) = {
		shl12_00_init, shl12_00_move, shl12_00_end, shl12_00_free,
	} ;

	shl12_00_jmp[ew->routine_0]( ew ) ;
}

static void field_init( PLAYER_WORK *ew );
static void ef01_init( PLAYER_WORK *ew );
static void ef02_init( PLAYER_WORK *ew );
static void ef03_init( PLAYER_WORK *ew );
static void ef04_init( PLAYER_WORK *ew );
static void ef05_init( PLAYER_WORK *ew );
static void ef06_init( PLAYER_WORK *ew );
static void engetsu_init( PLAYER_WORK *ew );
static void engetsu_part_init( PLAYER_WORK *ew );
static void field_right_init( PLAYER_WORK *ew );
static void field_left_init( PLAYER_WORK *ew );

/*==============================================================*/
static void shl12_00_init( PLAYER_WORK *ew )
{
	static void ( *shl12_00_init_jmp[] )( PLAYER_WORK *ew ) = {
		field_init, ef01_init, ef02_init, ef03_init, ef04_init, ef05_init,
		ef06_init, engetsu_init, engetsu_part_init, field_right_init, field_left_init,
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
	shl12_00_init_jmp[ew->type]( ew ) ;
	shl12_00_move( ew ) ;
}

/*==============================================================*/
/*			The Plasma Field: starts above the player at a		*/
/*			hundredth of full size								*/
/*==============================================================*/
static void field_init( PLAYER_WORK *ew )
{
	Sint16 *tp ;

	ew->disp_flag = 1 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl12_at_field ;
	HIT_RESET( ew ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	tp = &shl12_00_time ;
	ew->timer0 = *tp ;
	ew->timer1 = 1 ;
	ew->pos.y += 180.0f ;
	ew->alpha = 0.3f ;
	ew->acc.x = 0.04f ;
	ew->acc.y = 0.28f ;
	ew->scale.x = 1.0f ;
	ew->scale.y = 1.0f ;
	ew->scale.x = ew->scale.x / 100.0f ;
	ew->scale.y /= 100.0f ;
	pl_char_set( ew, Char_Effect, 0 ) ;
}

/*==============================================================*/
static void ef01_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.y += 120.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->alpha = 1.0f ;
	pl_char_set( ew, Char_Effect, 1 ) ;
}

/*==============================================================*/
static void ef02_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 7 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ew->pos.y += 120.0f ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->alpha = 1.0f ;
	pl_char_set( ew, Char_Effect, 2 ) ;
}

/*==============================================================*/
static void ef03_init( PLAYER_WORK *ew )
{
	Uint8 i ;
	Float *ofs ;

	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ofs = shl12_00_ofs[0] ;
	ofs += ew->sub_type * 2 ;
	ew->pos.x += ew->master_w->disp_dir ? ofs[0] : -ofs[0] ;
	ew->pos.y += ofs[1] ;
	for( i = 0 ; i < 8 ; i ++ ) shl12_00_part_set( ew, 4, i ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 3 ) ;
}

/*==============================================================*/
static void ef04_init( PLAYER_WORK *ew )
{
	Float *pos = shl12_00_pos ;

	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->pos.x += pos[ew->sub_no] ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 4 ) ;
}

/*==============================================================*/
static void ef05_init( PLAYER_WORK *ew )
{
	Uint8 i ;
	Float *ofs ;

	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->master_w->pos.x ;
	ew->pos.y = ew->master_w->pos.y ;
	ew->pos.z = ew->master_w->pos.z ;
	ofs = shl12_00_ofs[1] ;
	ofs += ew->sub_type * 2 ;
	ew->pos.x += ew->master_w->disp_dir ? ofs[0] : -ofs[0] ;
	ew->pos.y += ofs[1] ;
	for( i = 0 ; i < 8 ; i ++ ) shl12_00_part_set( ew, 6, i ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 5 ) ;
}

/*==============================================================*/
static void ef06_init( PLAYER_WORK *ew )
{
	Float *pos = shl12_00_pos ;

	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->pos.y += pos[ew->sub_no] ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 6 ) ;
}

/*==============================================================*/
static void engetsu_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x += ew->disp_dir ? DOT_X( 38.0f ) : -DOT_X( 38.0f ) ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->scale.y = 0.01f ;
	ew->spd.y = 0.38f ;
	ew->acc.y = -0.02f ;
	ew->alpha = 1.0f ;
	ew->timer0 = 5 ;
	ew->spd.x = ew->disp_dir ? 1.6666666f : -1.6666666f ;
	ew->acc.x = ew->disp_dir ? -0.026041666f : 0.026041666f ;
	shl12_00_part_set2( ew, 8, ew->sub_no ) ;
	ew->sub_no ++ ;
	ew->atck_flag = HIT_ON | HIT_SHOT ;
	ew->dm_flag = HIT_ON | HIT_SHOT ;
	ew->atck_no = pl12_at_engetsu_2 ;
	HIT_RESET( ew ) ;
	pl_char_set( ew, Char_Effect, 7 ) ;
}

/*==============================================================*/
static void engetsu_part_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	ew->spd.x = 0.18f ;
	ew->acc.x = -0.03f ;
	ew->timer0 = 6 ;
	pl_char_set( ew, Char_Effect, 8 ) ;
}

/*==============================================================*/
static void field_right_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->pos.x += ew->sub_no * 80 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 10 ) ;
}

/*==============================================================*/
static void field_left_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->prio = 0 ;
	ew->pos.x = ew->ef_w->pos.x ;
	ew->pos.y = ew->ef_w->pos.y ;
	ew->pos.z = ew->ef_w->pos.z ;
	ew->pos.x -= ew->sub_no * 80 ;
	ew->acc.x = 0.0f ;
	ew->acc.y = 0.0f ;
	pl_char_set( ew, Char_Effect, 10 ) ;
}

static void field_move( PLAYER_WORK *ew );
static void ef01_move( PLAYER_WORK *ew );
static void ef02_move( PLAYER_WORK *ew );
static void ef03_move( PLAYER_WORK *ew );
static void engetsu_move( PLAYER_WORK *ew );
static void engetsu_part_move( PLAYER_WORK *ew );
static void field_part_move( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void shl12_00_move( PLAYER_WORK *ew )
{
	static void ( *shl12_00_move_jmp[] )( PLAYER_WORK *ew ) = {
		field_move, ef01_move, ef02_move, ef03_move, ef03_move, ef03_move,
		ef03_move, engetsu_move, engetsu_part_move, field_part_move, field_part_move,
	} ;

	if( ew->pl_type != ew->master_w->pl_type ){
		shl12_00_free( ew ) ;
		return ;
	}
	shl12_00_move_jmp[ew->type]( ew ) ;
}

static void field_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
static void field_flat( PLAYER_WORK *ew, PLAYER_WORK *pl_w );

/*==============================================================*/
/*			The Plasma Field grows over Hayato; while it hits	*/
/*			the opponent on point his hyper combos are free		*/
/*==============================================================*/
static void field_move( PLAYER_WORK *ew )
{
	static void ( *field_jmp[] )( PLAYER_WORK *ew, PLAYER_WORK *pl_w ) = {
		field_grow, field_flat,
	} ;

	field_jmp[ew->routine_1]( ew, ew->master_w ) ;
	if( ew->master_w->hit_box_no ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void field_grow( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	PL12_LOCAL_W *lw = ( PL12_LOCAL_W * )pl_w->tlocal ;
	PLAYER_WORK *em ;

	pl_char_move( ew ) ;
	ew->timer1 = 1 ;
	ew->rot[2] += 0x4000 ;
	if( ew->rot[2] == 0xf000 ) ew->rot[2] = 0 ;
	ew->scale.x /= 1.6666666f ;
	ew->scale.y /= 2.142857f ;
	ew->acc.y += ew->acc.x ;
	ew->scale.x += ew->acc.y ;
	ew->scale.y += ew->acc.y ;
	ew->alpha += ew->acc.x ;
	if( ew->scale.x > 1.44f ){
		ew->scale.x = 1.44f ;
		ew->scale.y = 1.1200001f ;
	}
	if( ew->alpha > 1.0f ) ew->alpha = 1.0f ;
	if( !ew->timer0 -- ){
		ew->routine_1 ++ ;
		ew->timer0 = 35 ;
		ew->rot[2] = 0 ;
		ew->acc.y = 2.0f ;
		ew->acc.x = 0.01f ;
		return ;
	}
	if( ew->hit_flag & HR_GUARD ) return ;
	else {
		if( ew->hit_flag ){
			em = ew->em_w ;
			if( em->work_kind == W_PL && !em->team_no && em->vital_new ){
				lw->hyper_timer = 600 ;
				Timer_Gauge_Set( pl_w, &lw->hyper_timer, 6 ) ;
			}
		}
		hit_work_set( ew ) ;
	}
}

/*--------------------------------------------------------------*/
static void field_flat( PLAYER_WORK *ew, PLAYER_WORK *pl_w )
{
	pl_char_move( ew ) ;
	ew->scale.x /= 1.6666666f ;
	ew->scale.y /= 2.142857f ;
	ew->scale.x += ew->acc.y ;
	ew->scale.y -= ew->acc.x ;
	if( ew->scale.y <= 0.0046666665f ){
		ew->scale.y = 0.0046666665f ;
		if( !( ew->timer0 % 5 ) ){
			shl12_00_part_set( ew, 9, ew->sub_no ) ;
			shl12_00_part_set( ew, 10, ew->sub_no ) ;
			ew->sub_no ++ ;
		}
	}
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void ef01_fade( PLAYER_WORK *ew );

/*==============================================================*/
static void ef01_move( PLAYER_WORK *ew )
{
	static void ( *ef01_jmp[] )( PLAYER_WORK *ew ) = {
		ef01_fade,
	} ;

	ef01_jmp[ew->routine_1]( ew ) ;
	if( ew->hit_flag ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			Fades out to the animation's end					*/
/*==============================================================*/
static void ef01_fade( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->alpha -= 0.04f ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
static void ef02_move( PLAYER_WORK *ew )
{
	static void ( *ef02_jmp[] )( PLAYER_WORK *ew ) = {
		ef01_fade,
	} ;

	ef02_jmp[ew->routine_1]( ew ) ;
	if( ew->hit_flag ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			To the animation's end								*/
/*==============================================================*/
static void ef03_move( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

static void engetsu_wait( PLAYER_WORK *ew );
static void engetsu_fly( PLAYER_WORK *ew );
static void engetsu_thin( PLAYER_WORK *ew );
static void engetsu_fade( PLAYER_WORK *ew );
static void engetsu_fade_end( PLAYER_WORK *ew );

/*==============================================================*/
static void engetsu_move( PLAYER_WORK *ew )
{
	static void ( *engetsu_jmp[] )( PLAYER_WORK *ew ) = {
		engetsu_wait, engetsu_fly, engetsu_thin, engetsu_fade, engetsu_fade_end,
	} ;

	engetsu_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void engetsu_wait( PLAYER_WORK *ew )
{
	if( ew->cg_type == 1 ){
		ew->routine_1 ++ ;
		ew->cg_type = 0 ;
		pl_se_req( ew, 76 ) ;
	}
	pl_char_move( ew ) ;
}

/*--------------------------------------------------------------*/
static void engetsu_fly( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->timer0 -- < 0 ){
		ew->routine_1 ++ ;
		if( ew->sub_no < 5 ) shl12_00_part_set2( ew, 7, ew->sub_no ) ;
		return ;
	}
	hit_work_set( ew ) ;
}

/*--------------------------------------------------------------*/
static void engetsu_thin( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->scale.x -= ew->timer0 * 0.02f + 0.06f ;
	if( ew->scale.x < 0.01f ){
		ew->routine_1 ++ ;
		ew->timer0 = 8 ;
		ew->scale.x = 0.04f ;
		ew->acc.y = -0.1f ;
	}
	ew->timer0 ++ ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->scale.y < 0.01f ) ew->scale.y = 0.01f ;
}

/*--------------------------------------------------------------*/
static void engetsu_fade( PLAYER_WORK *ew )
{
	ew->alpha -= 0.06f ;
	if( ew->alpha < 0.1f ) ew->alpha = 0.1f ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->scale.y < 0.01f ) ew->scale.y = 0.01f ;
	if( ew->timer0 -- < 0 ){
		ew->routine_1 ++ ;
		ew->spd.y /= 100.0f ;
		ew->acc.y /= 100.0f ;
	}
}

/*--------------------------------------------------------------*/
static void engetsu_fade_end( PLAYER_WORK *ew )
{
	ew->alpha -= 0.08f ;
	if( ew->alpha < 0.1f ) ew->alpha = 0.1f ;
	ew->pos.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( ew->scale.y < 0.01f ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
		ew->scale.y = 0.01f ;
	}
}

static void engetsu_part_widen( PLAYER_WORK *ew );
static void engetsu_part_grow( PLAYER_WORK *ew );

/*==============================================================*/
static void engetsu_part_move( PLAYER_WORK *ew )
{
	static void ( *engetsu_part_jmp[] )( PLAYER_WORK *ew ) = {
		engetsu_part_widen, engetsu_part_grow,
	} ;

	engetsu_part_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void engetsu_part_widen( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->scale.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	if( ew->timer0 <= 2 ){
		ew->alpha -= 0.2f ;
		if( ew->alpha < 0.1f ) ew->alpha = 0.1f ;
	}
	if( ew->timer0 -- < 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 6 ;
		ew->alpha = 1.0f ;
		ew->scale.x = 1.0f ;
		ew->scale.y = 1.0f ;
		ew->spd.x = 0.01f ;
		ew->acc.x = -0.0003f ;
		ew->spd.y = 0.02f ;
		ew->acc.y = -0.0003f ;
		pl_char_set( ew, Char_Effect, 9 ) ;
	}
}

/*--------------------------------------------------------------*/
static void engetsu_part_grow( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	ew->scale.x += ew->spd.x ;
	ew->spd.x += ew->acc.x ;
	ew->scale.y += ew->spd.y ;
	ew->spd.y += ew->acc.y ;
	if( !ew->timer0 -- ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*==============================================================*/
/*			To the animation's end								*/
/*==============================================================*/
static void field_part_move( PLAYER_WORK *ew )
{
	pl_char_move( ew ) ;
	if( ew->cg_end_flag < 0 ){
		ew->routine_0 ++ ;
		ew->disp_flag = 0 ;
	}
}

/*--------------------------------------------------------------*/
static void shl12_00_end( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl12_00_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl12_00.c" End of File	*/
