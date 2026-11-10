/* The dark flames that ride Abyss' third form (character 1a; the effect is made by s_pl1a.c through
   shl1a_02_appear_set and shl1a_02_set): pieces in work lists 5 to 7, made by `type` and
   number, whose main (shl1a_02_main) first sets one up by its type, then steps it. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )


/* a piece's speed read as whole numbers (the growing flames turn by it) */
#define SHL1A_02_SPD( w )	( ( Sint32 * )&( w )->spd.x )

/* the master's own work in tlocal, as the flames read it: which of its flames to show, and their alpha */
typedef struct SHL1A_02_LW {
	Uint8	flag[4] ;
	Float	alpha ;
} SHL1A_02_LW ;
#define SHL1A_02_LW( w )	( ( SHL1A_02_LW * )( w )->tlocal )

extern void obj_argb_set( void *obj, Float a, Float b, Float c, Float d ) ;

static MOTION shl1a_02_fade_tbl[4] = { { 0, 0.0f }, { 4, 1.0f }, { 13, 1.0f }, { 15, 0.0f } } ;
static VEC3 shl1a_02_scale0 = { 0.1f, 0.1f, 0.1f } ;
static VEC3 shl1a_02_scale1 = { 1.0f, 1.0f, 1.0f } ;

static void shl1a_02_piece_set( PLAYER_WORK *master, Uint8 list, Uint8 type, Uint8 no ) ;
static void shl1a_02_main( PLAYER_WORK *ew ) ;

/****************************************************************/
void shl1a_02_appear_set( PLAYER_WORK *pl_w )
{
	shl1a_02_piece_set( pl_w, 7, 0, 0 ) ;
	shl1a_02_piece_set( pl_w, 7, 1, 0 ) ;
	shl1a_02_piece_set( pl_w, 6, 0, 1 ) ;
	shl1a_02_piece_set( pl_w, 6, 1, 1 ) ;
}

/****************************************************************/
void shl1a_02_set( PLAYER_WORK *pl_w )
{
	shl1a_02_piece_set( pl_w, 5, 3, 0 ) ;
	shl1a_02_piece_set( pl_w, 7, 4, 0 ) ;
	shl1a_02_piece_set( pl_w, 7, 4, 1 ) ;
	shl1a_02_piece_set( pl_w, 7, 4, 2 ) ;
	shl1a_02_piece_set( pl_w, 7, 4, 3 ) ;
	shl1a_02_piece_set( pl_w, 7, 4, 4 ) ;
	shl1a_02_piece_set( pl_w, 7, 4, 5 ) ;
}

/****************************************************************/
/*			The flames' end calls this one						*/
/****************************************************************/
void shl1a_02_set2( PLAYER_WORK *pl_w )
{
	shl1a_02_piece_set( pl_w, 7, 5, 0 ) ;
	shl1a_02_piece_set( pl_w, 7, 5, 1 ) ;
	shl1a_02_piece_set( pl_w, 7, 5, 2 ) ;
}

/*==============================================================*/
static void shl1a_02_piece_set( PLAYER_WORK *master, Uint8 list, Uint8 type, Uint8 no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, list, LIST_END ) ) ){
		ew->work_func = shl1a_02_main ;
		ew->master_w = master ;
		ew->type = type ;
		ew->sub_type = no ;
	}
}

static void shl1a_02_init( PLAYER_WORK *ew ) ;
static void shl1a_02_step( PLAYER_WORK *ew ) ;
static void shl1a_02_step_body( PLAYER_WORK *ew ) ;
static void shl1a_02_step_body_col( PLAYER_WORK *ew ) ;
static void shl1a_02_step_copy( PLAYER_WORK *ew ) ;
static void shl1a_02_step_turn( PLAYER_WORK *ew ) ;
static void shl1a_02_step_grow( PLAYER_WORK *ew ) ;
static void shl1a_02_step_spread( PLAYER_WORK *ew ) ;
static void shl1a_02_step_flat( PLAYER_WORK *ew ) ;
static void shl1a_02_place( PLAYER_WORK *ew ) ;
static void shl1a_02_hide( PLAYER_WORK *ew ) ;
static void shl1a_02_end( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void shl1a_02_main( PLAYER_WORK *ew )
{
	static void ( *shl1a_02_jmp[4] )( PLAYER_WORK *ew ) = {
		shl1a_02_init, shl1a_02_step, shl1a_02_hide, shl1a_02_end,
	} ;

	shl1a_02_jmp[ew->routine_0]( ew ) ;
}

static void shl1a_02_init_body( PLAYER_WORK *ew ) ;
static void shl1a_02_init_body_col( PLAYER_WORK *ew ) ;
static void shl1a_02_init_copy( PLAYER_WORK *ew ) ;
static void shl1a_02_init_turn( PLAYER_WORK *ew ) ;
static void shl1a_02_init_grow( PLAYER_WORK *ew ) ;
static void shl1a_02_init_spread( PLAYER_WORK *ew ) ;
static void shl1a_02_init_flat( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void shl1a_02_init( PLAYER_WORK *ew )
{
	static void ( *shl1a_02_init_jmp[7] )( PLAYER_WORK *ew ) = {
		shl1a_02_init_body, shl1a_02_init_body_col, shl1a_02_init_copy, shl1a_02_init_turn,
		shl1a_02_init_grow, shl1a_02_init_spread, shl1a_02_init_flat,
	} ;

	ew->routine_0 ++ ;
	shl1a_02_init_jmp[ew->type]( ew ) ;
	shl1a_02_step( ew ) ;
}

/*==============================================================*/
static void shl1a_02_init_body( PLAYER_WORK *ew )
{
	ew->obj_flag = 45 ;
	ew->obj = obj_stg->obj[90] ;
	ew->fade.trnsl = 1.0f ;
	if( ew->work_kind == W_EF ){
		ew->obj_flag = ew->obj_flag | 0x10 ;
		ew->scale = shl1a_02_scale0 ;
	}
}

/*==============================================================*/
static void shl1a_02_init_body_col( PLAYER_WORK *ew )
{
	PLAYER_WORK *nw ;

	ew->obj_flag = 0x40d ;
	ew->obj = obj_stg->obj[88] ;
	ew->fade.r = 1.0f ;
	ew->fade.g = 0.0f ;
	ew->fade.b = 0.0f ;
	if( ew->work_kind == W_EF ){
		ew->obj_flag = ew->obj_flag | 0x10 ;
		ew->scale = shl1a_02_scale0 ;
	}
	if( ( nw = work_get( ew, ew->work_kind, LIST_BEHIND ) ) ){
		nw->work_func = shl1a_02_main ;
		nw->master_w = ew->master_w ;
		nw->type = ew->type + 1 ;
		nw->sub_type = ew->sub_type ;
	}
}

/*==============================================================*/
/*			It copies the piece made before it					*/
/*==============================================================*/
static void shl1a_02_init_copy( PLAYER_WORK *ew )
{
	PLAYER_WORK *prev = ew->before_w ;

	ew->obj_flag = prev->obj_flag ;
	ew->obj = obj_stg->obj[89] ;
	ew->fade = prev->fade ;
	ew->scale = prev->scale ;
}

/*==============================================================*/
static void shl1a_02_init_turn( PLAYER_WORK *ew )
{
	ew->obj_flag = 61 ;
	ew->obj = obj_stg->obj[90] ;
	ew->fade.trnsl = 1.0f ;
	ew->scale = shl1a_02_scale1 ;
	ew->timer1 = 210 ;
	pl19_voice_req( ew, 16 ) ;
}

static Sint32 shl1a_02_spd_tbl[6] = { 0x305, 0x360, 0x360, 0x16c, 0x2d8, 0x2d8 } ;
static VEC3 shl1a_02_acc_tbl[6] = {
	{ 0.05f, 0.025f, 0.05f },
	{ 0.025f, 0.05f, 0.05f },
	{ 0.05f, 0.025f, 0.05f },
	{ 0.025f, 0.05f, 0.05f },
	{ 0.05f, 0.025f, 0.05f },
	{ 0.025f, 0.05f, 0.05f },
} ;

/*==============================================================*/
static void shl1a_02_init_grow( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->obj_flag = 0x41f ;
	ew->obj = obj_stg->obj[88 + ( ew->sub_type & 1 )] ;
	ew->fade.r = 1.0f ;
	ew->fade.g = 0.0f ;
	ew->fade.b = 0.0f ;
	ew->scale = shl1a_02_scale0 ;
	ew->rot[0] = ew->rot[1] = ew->rot[2] = 0 ;
	*SHL1A_02_SPD( ew ) = shl1a_02_spd_tbl[ew->sub_type] ;
	ew->timer0 = 0 ;
	ew->acc.x = shl1a_02_acc_tbl[ew->sub_type].x * 0.1f ;
	ew->acc.y = shl1a_02_acc_tbl[ew->sub_type].y * 0.1f ;
	ew->acc.z = shl1a_02_acc_tbl[ew->sub_type].z * 0.1f ;
}

static VEC3 shl1a_02_spread_spd_tbl[3] = {
	{ 0.35f, 0.0f, 0.35f },
	{ 0.35f, 0.0f, 0.35f },
	{ 0.35f, 0.0f, 0.35f },
} ;
static struct {
	Sint16	timer0, timer1 ;
} shl1a_02_time_tbl[3] = { { 0, 30 }, { 20, 30 }, { 0, 50 } } ;

/*==============================================================*/
static void shl1a_02_init_spread( PLAYER_WORK *ew )
{
	ew->obj_flag = 0x413 ;
	ew->obj = obj_efky->obj[183] ;
	ew->fade.trnsl = 1.0f ;
	ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
	ew->scale = shl1a_02_scale1 ;
	ew->rot[0] = 0x1555 ;
	if( ew->sub_type == 2 || ew->sub_type == 5 ) ew->rot[0] += 0x4000 ;
	ew->spd = shl1a_02_spread_spd_tbl[ew->sub_type] ;
	ew->timer0 = shl1a_02_time_tbl[ew->sub_type].timer0 ;
	ew->timer1 = shl1a_02_time_tbl[ew->sub_type].timer1 ;
}

/*==============================================================*/
static void shl1a_02_init_flat( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->obj_flag = 0x411 ;
	ew->obj = obj_efky->obj[134] ;
	ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
	ew->scale = shl1a_02_scale1 ;
	ew->timer0 = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1a_02_step( PLAYER_WORK *ew )
{
	static void ( *shl1a_02_step_jmp[7] )( PLAYER_WORK *ew ) = {
		shl1a_02_step_body, shl1a_02_step_body_col, shl1a_02_step_copy, shl1a_02_step_turn,
		shl1a_02_step_grow, shl1a_02_step_spread, shl1a_02_step_flat,
	} ;

	shl1a_02_step_jmp[ew->type]( ew ) ;
}

/*==============================================================*/
/*			The flame that follows its master's body: set		*/
/*			where he stands and turns on its own				*/
/*==============================================================*/
static void shl1a_02_step_body( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL1A_02_LW *lw = SHL1A_02_LW( pl_w ) ;
	Float ofs ;

	if( !( ew->disp_flag = lw->flag[ew->sub_type] ) ) return ;
	ew->fade.trnsl = lw->alpha ;
	ew->pos = pl_w->pos ;
	ofs = pl_w->scale.x * 40.0f ;
	if( pl_w->disp_dir == 0 ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->pos.y += pl_w->scale.y * DOT_Y( 96.0f ) ;
	if( pl_w->hit_stop == 0 ){
		ew->timer0 ++ ;
		ew->rot[1] = DEG_ANG( ( ew->timer0 + ew->timer1 ) * 2 ) ;
		ew->rot[2] = DEG_ANG( ( ew->timer0 + ew->timer1 ) * 3 ) ;
	}
	obj_argb_set( ew->obj, pl_w->fade.trnsl, pl_w->fade.r, pl_w->fade.b, pl_w->fade.b ) ;
}

/*==============================================================*/
/*			The same, with its colour shifting red and green	*/
/*			by the count										*/
/*==============================================================*/
static void shl1a_02_step_body_col( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	SHL1A_02_LW *lw = SHL1A_02_LW( pl_w ) ;
	Float g ;

	if( !( ew->disp_flag = lw->flag[ew->sub_type] ) ) return ;
	ew->pos = pl_w->pos ;
	g = pl_w->scale.x * 40.0f ;
	if( pl_w->disp_dir == 0 ) g = -g ;
	ew->pos.x += g ;
	ew->pos.y += pl_w->scale.y * DOT_Y( 96.0f ) ;
	if( pl_w->hit_stop == 0 ){
		ew->timer0 ++ ;
		ew->rot[1] = DEG_ANG( ( ew->timer0 + ew->timer1 ) * 2 ) ;
		ew->rot[2] = DEG_ANG( ( ew->timer0 + ew->timer1 ) * 3 ) ;
		ew->fade.r = ( nlSin( DEG_ANG( ew->timer0 % 360 ) ) * 0.5f + 0.5f ) * lw->alpha ;
		ew->fade.g = ( nlCos( DEG_ANG( ew->timer0 % 360 ) ) * 0.5f + 0.5f ) * lw->alpha ;
	}
	else{
		ew->fade.r = lw->alpha * 1.0f ;
		ew->fade.g = 0.0f ;
	}
}

/*==============================================================*/
/*			It copies the flame made before it, and stays		*/
/*			while that one does									*/
/*==============================================================*/
static void shl1a_02_step_copy( PLAYER_WORK *ew )
{
	PLAYER_WORK *prev = ew->before_w ;

	ew->disp_flag = 0 ;
	if( ( ew->routine_0 = prev->routine_0 ) < 2 ){
		if( ( ew->disp_flag = prev->disp_flag ) ){
			ew->pos = prev->pos ;
			ew->fade = prev->fade ;
			ew->scale = prev->scale ;
			ew->rot[0] = prev->rot[0] ;
			ew->rot[1] = prev->rot[1] ;
			ew->rot[2] = prev->rot[2] ;
		}
	}
}

/*==============================================================*/
/*			A flame that turns for timer1 frames, then fades	*/
/*			in 30												*/
/*==============================================================*/
static void shl1a_02_step_turn( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	shl1a_02_place( ew ) ;
	ew->timer0 ++ ;
	ew->rot[1] = DEG_ANG( ew->timer0 * 2 ) ;
	ew->rot[2] = DEG_ANG( ew->timer0 * 3 ) ;
	if( ew->routine_1 == 0 ){
		if( -- ew->timer1 <= 0 ) ew->routine_1 ++ ;
	}
	else{
		if( ( ew->fade.trnsl -= 0.033333335f ) < 0.0f ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*==============================================================*/
/*			A flame that grows by its acceleration, turning		*/
/*			by its speed, for 100 frames, then fades; at the	*/
/*			end of the first one it makes the last flames		*/
/*			and calls out										*/
/*==============================================================*/
static void shl1a_02_step_grow( PLAYER_WORK *ew )
{
	shl1a_02_place( ew ) ;
	ew->rot[0] += *SHL1A_02_SPD( ew ) ;
	ew->rot[1] += *SHL1A_02_SPD( ew ) ;
	ew->rot[2] += *SHL1A_02_SPD( ew ) ;
	ew->timer0 ++ ;
	if( ew->routine_1 == 0 && ew->timer0 == 100.0f ) ew->routine_1 = 1 ;
	if( ew->routine_2 == 0 && ew->timer0 == 160.0f ) ew->routine_2 = 1 ;
	if( ew->routine_1 ){
		ew->scale.x += ew->acc.x ;
		ew->scale.y += ew->acc.y ;
		ew->scale.z += ew->acc.z ;
	}
	if( ew->routine_2 ){
		if( ( ew->fade.r -= 0.05f ) <= 0.0f ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
			if( !ew->sub_type ){
				flash_white_set() ;
				shl1a_02_set2( ew->master_w ) ;
				pl19_voice_req( ew, 17 ) ;
			}
		}
	}
}

/*==============================================================*/
/*			A flame that waits timer0 frames, then grows by		*/
/*			its speed for timer1 and fades away					*/
/*==============================================================*/
static void shl1a_02_step_spread( PLAYER_WORK *ew )
{
	if( ew->routine_1 == 0 ){
		if( -- ew->timer0 > 0 ) return ;
		ew->routine_1 ++ ;
		ew->disp_flag = 1 ;
	}
	shl1a_02_place( ew ) ;
	ew->scale.x += ew->spd.x ;
	ew->scale.y += ew->spd.y ;
	ew->scale.z += ew->spd.z ;
	if( ew->routine_2 == 0 ){
		if( -- ew->timer1 <= 0 ) ew->routine_2 = 1 ;
	}
	if( ew->routine_2 ){
		ew->fade.r -= 0.1f ;
		ew->fade.g -= 0.1f ;
		if( ( ew->fade.b -= 0.1f ) < 0.0f ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*==============================================================*/
/*			A flat flame that waits timer0 frames, then			*/
/*			widens and fades									*/
/*==============================================================*/
static void shl1a_02_step_flat( PLAYER_WORK *ew )
{
	shl1a_02_place( ew ) ;
	ew->scale.x += 0.125f ;
	ew->scale.y += 0.125f ;
	if( ew->routine_1 == 0 ){
		if( -- ew->timer0 < 0 ) ew->routine_1 ++ ;
	} else {
		ew->fade.r -= 0.05f ;
		ew->fade.g -= 0.05f ;
		if( ( ew->fade.b -= 0.05f ) < 0.0f ){
			ew->routine_0 ++ ;
			ew->disp_flag = 0 ;
		}
	}
}

/*==============================================================*/
/*			It stands on its master's body						*/
/*==============================================================*/
static void shl1a_02_place( PLAYER_WORK *ew )
{
	PLAYER_WORK *pl_w = ew->master_w ;
	Float ofs ;

	ew->pos = pl_w->pos ;
	ofs = pl_w->scale.x * 40.0f ;
	if( pl_w->disp_dir == 0 ) ofs = -ofs ;
	ew->pos.x += ofs ;
	ew->pos.y += pl_w->scale.y * DOT_Y( 96.0f ) ;
}

/*--------------------------------------------------------------*/
static void shl1a_02_hide( PLAYER_WORK *ew )
{
	ew->routine_0 ++ ;
	ew->disp_flag = 0 ;
}

/*--------------------------------------------------------------*/
static void shl1a_02_end( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shl1a_02.c" End of File	*/
