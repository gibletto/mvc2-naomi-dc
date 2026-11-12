/* The hyper combo's background (kind 6 and 8 works): three screen-sized layers whose
   fade colour shifts with the stage's request in Sys->hyper_bg (one colour for each of the first
   three hyper levels and one while it is lit), their texture scrolling, and the pieces (rings and
   flying bits) that come and go with them. */
#include "player.h"

extern void obj_uv_begin( int *src, void *obj );	/* the texture scroll: begin on two models */
extern int obj_uv_next( void );	/* the next piece of them, 0 while there is one */
extern void obj_uv_get( Float *x, Float *y );	/* its texture's place */
extern void obj_uv_put( Float *x, Float *y );	/* and set it */
extern void obj_uv_clear( void *obj );

/* nothing reads this */
static Float ef06_unused_tbl[8] = { 1.0f, 1.0f, 1.0f, 1.0f, 0.5f, 0.5f, 0.2f, 0.2f, } ;

/* the colours the layers fade to, one for each of the first three requests and the last one for the
   fourth (each layer has its own set), and the place of a texture by frame */
static const VEC3 ef06_col0_tbl[4] = {
	{ 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f },
} ;
static const Float ef06_uv_tbl[16][2] = {
	{ 0.0f, 0.0f }, { 0.25f, 0.0f }, { 0.5f, 0.0f }, { 0.75f, 0.0f },
	{ 0.0f, 0.25f }, { 0.25f, 0.25f }, { 0.5f, 0.25f }, { 0.75f, 0.25f },
	{ 0.0f, 0.5f }, { 0.25f, 0.5f }, { 0.5f, 0.5f }, { 0.75f, 0.5f },
	{ 0.0f, 0.75f }, { 0.25f, 0.75f }, { 0.5f, 0.75f }, { 0.75f, 0.75f },
} ;
static const VEC3 ef06_col1_tbl[4] = {
	{ 0.50980395f, 0.50980395f, 1.0f }, { 0.7058824f, 1.0f, 0.7058824f },
	{ 1.0f, 0.50980395f, 0.50980395f }, { 1.0f, 1.0f, 1.0f },
} ;
static const VEC3 ef06_col2_tbl[4] = {
	{ 0.7058824f, 0.7058824f, 1.0f }, { 0.8627451f, 1.0f, 0.8627451f },
	{ 1.0f, 0.7058824f, 0.7058824f }, { 1.0f, 1.0f, 1.0f },
} ;

static void ef06_a_main( PLAYER_WORK *ew ) ;
static void ef06_a_disp( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			The first ring: it takes the player's place			*/
/*==============================================================*/
static void ef06_a_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = ef06_a_main ;
		ew->obj = obj_efky->obj[165] ;
		ew->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->master_w = pl_w ;
		obj_uv_clear( ew->obj ) ;
		ew->master_w->routine_1 ++ ;
		obj_func_set( ew, ef06_a_disp ) ;
	}
}

static void ef06_a_init( PLAYER_WORK *ew ) ;
static void ef06_a_fade( PLAYER_WORK *ew ) ;
static void ef06_a_hide( PLAYER_WORK *ew ) ;
static void ef06_a_end( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_a_main( PLAYER_WORK *ew )
{
	static void ( *ef06_a_jmp[] )( PLAYER_WORK *ew ) = {
		ef06_a_init, ef06_a_fade, ef06_a_hide, ef06_a_end,
	} ;

	ef06_a_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void ef06_a_init( PLAYER_WORK *ew )
{
	Sint32 rnd ;

	ew->routine_0 ++ ;
	rnd = nlRand() % 4000 ;
	ew->pos.x = ( rnd % 2 ) ? rnd : -rnd ;
}

/*--------------------------------------------------------------*/
static void ef06_a_fade( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->master_w->disp_flag ;
	if( ew->work3 <= 20 ) ew->fade.trnsl += 0.05f ;
	else if( ew->work3 >= 40 ) ew->fade.trnsl -= 0.05f ;
	else ew->fade.trnsl = 1.0f ;
	ew->timer1 ++ ;
	if( ew->timer1 >= 80 ) ew->timer1 = 0 ;
	if( ++ ew->work3 > 60 || !ew->disp_flag ){
		ew->routine_0 ++ ;
		obj_func_set( ew, 0 ) ;
	}
}

/*==============================================================*/
/*			Its texture slides with the frame					*/
/*==============================================================*/
static void ef06_a_disp( PLAYER_WORK *ew )
{
	Float py, px ;

	obj_uv_begin( obj_efky->obj[166], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		py += 1.0f - ew->timer1 * 0.0125f ;
		obj_uv_put( &px, &py ) ;
	}
}

/*--------------------------------------------------------------*/
static void ef06_a_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void ef06_a_end( PLAYER_WORK *ew )
{
	ew->master_w->routine_1 -- ;
	work_free( ew ) ;
}

static void ef06_b_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_b_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = ef06_b_main ;
		ew->obj = obj_efky->obj[157] ;
		ew->obj_flag = OBJ_POS | OBJ_TRNSL | OBJ_BRIGHT ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->master_w = pl_w ;
		ew->fade.trnsl = 1.0f ;
	}
}

/*==============================================================*/
static void ef06_b_main( PLAYER_WORK *ew )
{
	ew->disp_flag = ew->master_w->disp_flag ;
}

/* The colour of the three layers goes to the colour of the request, in 20 frames. Each of the three
   has its own routine pair: one for the stage's colours, one while a hyper combo lights the screen
   (it steps through the set at each new request). */
static void ef06_c_back( PLAYER_WORK *ew ) ;
static void ef06_c_hyper( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_c_main( PLAYER_WORK *ew )
{
#ifdef TARGET_DC	/* under the home rule delay_hyper_free, a team hyper combo's flag chooses */
	if( Sys->delay_hyper_free ){
		if( vs_sys_work.team_hyper_flag ) ef06_c_hyper( ew ) ;
		else ef06_c_back( ew ) ;
	} else {
		switch( Sys->hyper_bg & 15 ){
		case 4:
			ef06_c_hyper( ew ) ;
			break ;
		default:
			ef06_c_back( ew ) ;
			break ;
		}
	}
#else
	switch( Sys->hyper_bg & 15 ){
	case 4:
		ef06_c_hyper( ew ) ;
		break ;
	default:
		ef06_c_back( ew ) ;
		break ;
	}
#endif
}

/*==============================================================*/
/*			The step of each colour is kept in scale			*/
/*==============================================================*/
static void ef06_c_back( PLAYER_WORK *ew )
{
	const VEC3 *col = ef06_col0_tbl ;

	col += ( ( Sys->hyper_bg & 15 ) - 1 ) % 3 ;
	if( !ew->work3 ){
		ew->scale.x = ( col->x - ew->fade.r ) / 20.0f ;
		ew->scale.y = ( col->y - ew->fade.g ) / 20.0f ;
		ew->scale.z = ( col->z - ew->fade.b ) / 20.0f ;
		ew->work3 = 20 ;
	}
	if( ew->work3 ){
		ew->work3 -- ;
		ew->fade.r += ew->scale.x ;
		ew->fade.g += ew->scale.y ;
		ew->fade.b += ew->scale.z ;
	}
}

static void ef06_c_hyper_next( PLAYER_WORK *ew ) ;
static void ef06_c_hyper_fade( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_c_hyper( PLAYER_WORK *ew )
{
	static void ( *ef06_c_jmp[] )( PLAYER_WORK *ew ) = {
		ef06_c_hyper_next, ef06_c_hyper_fade,
	} ;

	ef06_c_jmp[ew->sub_no]( ew ) ;
}

/*==============================================================*/
static void ef06_c_hyper_next( PLAYER_WORK *ew )
{
	const VEC3 *col = ef06_col0_tbl ;

	ew->routine_3 ++ ;
	ew->routine_3 %= 3 ;
	col += ew->routine_3 ;
	ew->scale.x = ( col->x - ew->fade.r ) / 20.0f ;
	ew->scale.y = ( col->y - ew->fade.g ) / 20.0f ;
	ew->scale.z = ( col->z - ew->fade.b ) / 20.0f ;
	ew->work3 = 20 ;
	ew->sub_no ++ ;
	ef06_c_hyper_fade( ew ) ;
}

/*==============================================================*/
static void ef06_c_hyper_fade( PLAYER_WORK *ew )
{
	if( ew->work3 ){
		ew->work3 -- ;
		ew->fade.r += ew->scale.x ;
		ew->fade.g += ew->scale.y ;
		ew->fade.b += ew->scale.z ;
	} else {
		ew->sub_no = 0 ;
	}
}

static void ef06_d_back( PLAYER_WORK *ew ) ;
static void ef06_d_hyper( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_d_main( PLAYER_WORK *ew )
{
	switch( Sys->hyper_bg & 15 ){
	case 0:
		break ;
	case 4:
		ef06_d_hyper( ew ) ;
		break ;
	default:
		ef06_d_back( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void ef06_d_back( PLAYER_WORK *ew )
{
	const VEC3 *col = ef06_col1_tbl ;

	if( Sys->hyper_bg & 15 ){
		col += ( Sys->hyper_bg & 15 ) - 1 ;
		if( !ew->work3 ){
			ew->scale.x = ( col->x - ew->fade.r ) / 20.0f ;
			ew->scale.y = ( col->y - ew->fade.g ) / 20.0f ;
			ew->scale.z = ( col->z - ew->fade.b ) / 20.0f ;
			ew->work3 = 20 ;
		}
	}
	if( ew->work3 ){
		ew->work3 -- ;
		ew->fade.r += ew->scale.x ;
		ew->fade.g += ew->scale.y ;
		ew->fade.b += ew->scale.z ;
	}
}

static void ef06_d_hyper_next( PLAYER_WORK *ew ) ;
static void ef06_d_hyper_fade( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_d_hyper( PLAYER_WORK *ew )
{
	static void ( *ef06_d_jmp[] )( PLAYER_WORK *ew ) = {
		ef06_d_hyper_next, ef06_d_hyper_fade,
	} ;

	ef06_d_jmp[ew->sub_no]( ew ) ;
}

/*==============================================================*/
static void ef06_d_hyper_next( PLAYER_WORK *ew )
{
	const VEC3 *col = ef06_col1_tbl ;

	ew->routine_3 ++ ;
	ew->routine_3 %= 3 ;
	col += ew->routine_3 ;
	ew->scale.x = ( col->x - ew->fade.r ) / 20.0f ;
	ew->scale.y = ( col->y - ew->fade.g ) / 20.0f ;
	ew->scale.z = ( col->z - ew->fade.b ) / 20.0f ;
	ew->work3 = 20 ;
	ew->sub_no ++ ;
	ef06_d_hyper_fade( ew ) ;
}

/*==============================================================*/
static void ef06_d_hyper_fade( PLAYER_WORK *ew )
{
	if( ew->work3 ){
		ew->work3 -- ;
		ew->fade.r += ew->scale.x ;
		ew->fade.g += ew->scale.y ;
		ew->fade.b += ew->scale.z ;
	} else {
		ew->sub_no = 0 ;
	}
}

/* the third layer keeps its step in spd.x, spd.y and acc.x */
static void ef06_e_back( PLAYER_WORK *ew ) ;
static void ef06_e_hyper( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_e_main( PLAYER_WORK *ew )
{
	switch( Sys->hyper_bg & 15 ){
	case 0:
		break ;
	case 4:
		ef06_e_hyper( ew ) ;
		break ;
	default:
		ef06_e_back( ew ) ;
		break ;
	}
}

/*==============================================================*/
static void ef06_e_back( PLAYER_WORK *ew )
{
	const VEC3 *col = ef06_col2_tbl ;

	if( Sys->hyper_bg & 15 ){
		col += ( Sys->hyper_bg & 15 ) - 1 ;
		if( !ew->work3 ){
			ew->spd.x = ( col->x - ew->fade.r ) / 20.0f ;
			ew->spd.y = ( col->y - ew->fade.g ) / 20.0f ;
			ew->acc.x = ( col->z - ew->fade.b ) / 20.0f ;
			ew->work3 = 20 ;
		}
	}
	if( ew->work3 ){
		ew->work3 -- ;
		ew->fade.r += ew->spd.x ;
		ew->fade.g += ew->spd.y ;
		ew->fade.b += ew->acc.x ;
	}
}

static void ef06_e_hyper_next( PLAYER_WORK *ew ) ;
static void ef06_e_hyper_fade( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_e_hyper( PLAYER_WORK *ew )
{
	static void ( *ef06_e_jmp[] )( PLAYER_WORK *ew ) = {
		ef06_e_hyper_next, ef06_e_hyper_fade,
	} ;

	ef06_e_jmp[ew->sub_no]( ew ) ;
}

/*==============================================================*/
static void ef06_e_hyper_next( PLAYER_WORK *ew )
{
	const VEC3 *col = ef06_col2_tbl ;

	ew->routine_3 ++ ;
	ew->routine_3 %= 3 ;
	col += ew->routine_3 ;
	ew->spd.x = ( col->x - ew->fade.r ) / 20.0f ;
	ew->spd.y = ( col->y - ew->fade.g ) / 20.0f ;
	ew->acc.x = ( col->z - ew->fade.b ) / 20.0f ;
	ew->work3 = 20 ;
	ew->sub_no ++ ;
	ef06_e_hyper_fade( ew ) ;
}

/*==============================================================*/
static void ef06_e_hyper_fade( PLAYER_WORK *ew )
{
	if( ew->work3 ){
		ew->work3 -- ;
		ew->fade.r += ew->spd.x ;
		ew->fade.g += ew->spd.y ;
		ew->fade.b += ew->acc.x ;
	} else {
		ew->sub_no = 0 ;
	}
}

static void ef06_x_set( PLAYER_WORK *pl_w ) ;
static void ef06_y_set( PLAYER_WORK *pl_w ) ;
static void ef06_main( PLAYER_WORK *ew ) ;

/****************************************************************/
/*			The three layers (types 1, 2 and 0)					*/
/****************************************************************/
void hyper_bg_set( void )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->type = 1 ;
		ew->work_func = ef06_main ;
		ew->dir = 71 ;
		ew->obj = obj_efky->obj[71] ;
		ew->pos.x = scr_work.eye[0].x ;
		ew->pos.y = 0.0f ;
		ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
		ew->timer0 = 0 ;
		ew->timer1 = 0 ;
		obj_uv_clear( ew->obj ) ;
		ef06_b_set( ew ) ;
		if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
			ew->disp_flag = 1 ;
			ew->type = 2 ;
			ew->work_func = ef06_main ;
			ew->dir = 74 ;
			ew->obj = obj_efky->obj[74] ;
			ew->pos.x = scr_work.eye[0].x ;
			ew->pos.y = 0.0f ;
			ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
			ew->timer0 = 0 ;
			ew->timer1 = 0 ;
			obj_uv_clear( ew->obj ) ;
			if( ( ew = work_get( 0, W_BACK, LIST_END ) ) ){
				ew->disp_flag = 1 ;
				ew->type = 0 ;
				ew->work_func = ef06_main ;
				ew->obj = obj_efky->obj[73] ;
				ew->pos.x = scr_work.eye[0].x ;
				ew->pos.y = 0.0f ;
				ew->obj_flag = OBJ_POS | OBJ_FADE | OBJ_BRIGHT ;
				ew->timer0 = 0 ;
				ew->timer1 = 0 ;
				obj_uv_clear( ew->obj ) ;
			}
		}
	}
}

/*==============================================================*/
/*			The layers: type 1 and 2 scroll their texture		*/
/*			and send out the pieces of their kind				*/
/*==============================================================*/
static void ef06_main( PLAYER_WORK *ew )
{
	Float py, px ;

	if( !( ew->disp_flag = Sys->hyper_bg ) || ( ( Sys->hyper_bg & 15 ) == 1 && vs_sys_work.game_stop ) ){
		ew->disp_flag = 0 ;
		ew->fade.r = 0.0f ;
		ew->fade.g = 0.0f ;
		ew->fade.b = 1.0f ;
		ew->routine_3 = 0 ;
		ew->sub_no = 0 ;
		if( ew->type == 1 ) ew->work1 = 0 ;
		return ;
	}
	ef06_c_main( ew ) ;
	if( ew->type ){
		ew->timer1 ++ ;
		if( ew->type == 2 ){
			if( ew->timer1 >= 160 ) ew->timer1 = 0 ;
		} else if( ew->timer1 >= 40 ){
			ew->timer1 = 0 ;
		}
		obj_uv_begin( obj_efky->obj[ew->dir + 1], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &px, &py ) ;
			if( ew->type == 2 ) py += 1.0f - ew->timer1 * 0.00625f ;
			else py += 1.0f - ew->timer1 * 0.025f ;
			obj_uv_put( &px, &py ) ;
		}
	}
	if( ew->type == 1 ){
		if( ew->routine_1 < 2 ){
			if( ew->work1 ){
				ew->work1 -- ;
			} else {
				ew->work1 = 40 ;
				ef06_a_set( ew ) ;
			}
		}
	}
	if( ew->type == 2 ){
		if( ew->routine_1 < 3 ){
			if( ew->work1 ){
				ew->work1 -- ;
			} else {
				ew->work1 = 10 ;
				ef06_x_set( ew ) ;
			}
		}
		if( ew->routine_2 < 3 ){
			if( ew->work2 ){
				ew->work2 -- ;
			} else {
				ew->work2 = 50 ;
				ef06_y_set( ew ) ;
			}
		}
	}
}

static void ef06_x_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			A piece that flies in from behind the layer,		*/
/*			near the player's place								*/
/*==============================================================*/
static void ef06_x_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint32 rnd ;

	if( ( ew = work_get( 0, W_MARK, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = ef06_x_main ;
		ew->obj = obj_efky->obj[76] ;
		ew->obj_flag = OBJ_POS | OBJ_FADE ;
		ew->base_mtx = &pl_w->obj_mtx ;
		ew->master_w = pl_w ;
		rnd = nlRand() % 1500 ;
		ew->pos.x += ( rnd % 2 ) ? rnd : -rnd ;
		rnd = nlRand() % 240 ;
		ew->pos.y += ( rnd % 2 ) ? rnd : -rnd ;
		ew->pos.y += scr_work.center_y ;
		ew->pos.z = 900.0f ;
		ew->timer0 = 30 ;
		ew->master_w->routine_1 ++ ;
	}
}

static void ef06_x_init( PLAYER_WORK *ew ) ;
static void ef06_x_move( PLAYER_WORK *ew ) ;
static void ef06_hide( PLAYER_WORK *ew ) ;
static void ef06_free( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_x_main( PLAYER_WORK *ew )
{
	static void ( *ef06_x_jmp[] )( PLAYER_WORK *ew ) = {
		ef06_x_init, ef06_x_move, ef06_hide, ef06_free,
	} ;

	ef06_x_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void ef06_x_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->routine_0 ++ ;
	Rnd() ;
	ew->spd.z = -100.0f ;
	ew->acc.z = -10.0f ;
}

/*--------------------------------------------------------------*/
static void ef06_x_move( PLAYER_WORK *ew )
{
	if( !( ew->disp_flag = Sys->hyper_bg ) || ( ( Sys->hyper_bg & 15 ) == 1 && vs_sys_work.game_stop ) ){
		ew->disp_flag = 0 ;
		ew->timer0 = 0 ;
	}
	ew->disp_flag = Sys->hyper_bg ;
	ef06_d_main( ew ) ;
	if( ew->timer0 ){
		ew->timer0 -- ;
		ew->spd.z += ew->acc.z ;
		ew->pos.z += ew->spd.z ;
	} else {
		ew->master_w->routine_1 -- ;
		ew->routine_0 ++ ;
	}
}

/*--------------------------------------------------------------*/
static void ef06_hide( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	ew->routine_0 ++ ;
}

/*--------------------------------------------------------------*/
static void ef06_free( PLAYER_WORK *ew )
{
	work_free( ew ) ;
}

static void ef06_y_main( PLAYER_WORK *ew ) ;

/*==============================================================*/
/*			A piece that comes at the screen from far off		*/
/*==============================================================*/
static void ef06_y_set( PLAYER_WORK *pl_w )
{
	PLAYER_WORK *ew ;
	Sint32 rnd ;

	if( !( ew = work_get( 0, W_MARK, LIST_END ) ) ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = ef06_y_main ;
	ew->obj_flag = OBJ_POS | OBJ_ROT_Y | OBJ_SCALE | OBJ_FADE ;
	ew->base_mtx = &pl_w->obj_mtx ;
	ew->master_w = pl_w ;
	ew->obj = obj_efky->obj[77] ;
	rnd = nlRand() % 6000 ;
	ew->pos.x += ( rnd % 2 ) ? rnd + 3000.0f : -rnd + -3000.0f ;
	rnd = nlRand() % 1000 ;
	ew->pos.y = 1000.0f ;
	ew->pos.y += ( rnd % 2 ) ? rnd : -rnd ;
	rnd = nlRand() % 2400 ;
	ew->pos.z += ( rnd % 2 ) ? rnd : -rnd ;
	ew->pos.z += -1600.0f ;
	ew->timer1 = 160 ;
	ew->master_w->routine_2 ++ ;
	rnd = nlRand() % 2 ;
	ew->rot[1] = rnd ? 0 : 0x8000 ;
	ew->fade.r = 1.0f ;
	ew->fade.g = 1.0f ;
	ew->fade.b = 1.0f ;
	ew->scale.x = 4.0f ;
	ew->scale.y = 4.0f ;
	ew->scale.z = 1.0f ;
}

static void ef06_y_init( PLAYER_WORK *ew ) ;
static void ef06_y_move( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_y_main( PLAYER_WORK *ew )
{
	static void ( *ef06_y_jmp[] )( PLAYER_WORK *ew ) = {
		ef06_y_init, ef06_y_move, ef06_hide, ef06_free,
	} ;

	ef06_y_jmp[ew->routine_0]( ew ) ;
}

static void ef06_y_disp( PLAYER_WORK *ew ) ;

/*==============================================================*/
static void ef06_y_init( PLAYER_WORK *ew )
{
	ew->disp_flag = 1 ;
	ew->routine_0 ++ ;
	ew->spd.z = -90.0f ;
	ew->acc.z = -10.0f ;
	obj_func_set( ew, ef06_y_disp ) ;
}

/*--------------------------------------------------------------*/
static void ef06_y_move( PLAYER_WORK *ew )
{
	if( !( ew->disp_flag = Sys->hyper_bg ) || ( ( Sys->hyper_bg & 15 ) == 1 && vs_sys_work.game_stop ) ){
		ew->disp_flag = 0 ;
		ew->timer1 = 0 ;
	}
	ef06_e_main( ew ) ;
	ew->routine_2 ++ ;
	if( !( ew->routine_2 %= 5 ) ){
		ew->timer0 ++ ;
		ew->timer0 = ew->timer0 % 16 ;
		if( !ew->timer0 ){
			ew->routine_1 ^= 1 ;
			if( ew->routine_1 ) ew->obj = obj_efky->obj[79] ;
			else ew->obj = obj_efky->obj[77] ;
		}
	}
	if( ew->timer1 ){
		ew->timer1 -- ;
		ew->spd.z += ew->acc.z ;
		ew->pos.z += ew->spd.z ;
		if( ew->timer1 <= 8 ){
			ew->fade.r -= 0.125f ;
			ew->fade.g -= 0.125f ;
			ew->fade.b -= 0.125f ;
		}
	} else {
		ew->master_w->routine_2 -- ;
		ew->routine_0 ++ ;
		obj_func_set( ew, 0 ) ;
	}
}

/*==============================================================*/
/*			Its texture steps through a sheet of sixteen		*/
/*			pictures											*/
/*==============================================================*/
static void ef06_y_disp( PLAYER_WORK *ew )
{
	Float px, py ;
	const Float *uv = ef06_uv_tbl[0] ;

	uv += ew->timer0 * 2 ;
	if( ew->routine_1 ) obj_uv_begin( obj_efky->obj[140], ew->obj ) ;
	else obj_uv_begin( obj_efky->obj[78], ew->obj ) ;
	while( obj_uv_next() == 0 ){
		obj_uv_get( &px, &py ) ;
		px += uv[0] ;
		py -= uv[1] ;
		obj_uv_put( &px, &py ) ;
	}
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "ef06.c" End of File	*/
