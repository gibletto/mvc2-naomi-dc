/* The result messages after a fight (kind 12 works): WIN, DRAW, PERFECT and the others (type 0 to
   7) come in, stay and go; PERFECT hands on to the bonus lines (vs_bonus.c). */
#include "player.h"

/* one step of a message's script (its reader is elsewhere) */
typedef struct VS_RES_STEP {
	Uint8	unk_0, unk_1, unk_2, unk_3 ;
	Float	unk_4 ;
} VS_RES_STEP;

/* a work's step list, from unk_d4: the script it plays, and the step it is at */
typedef struct VS_RES_SPR_W {
	Sint32	obj_flag, unk_d0 ;
	VS_RES_STEP *step ;	/* 008 */
	Sint32	no ;		/* 00c */
} VS_RES_SPR_W;
#define VS_RES_SW( w )	( ( VS_RES_SPR_W * )&( w )->obj_flag )

/* scales that nothing reads */
static VEC3 vs_res_scale_tbl[] = {
	{ 1.5f, 1.5f, 1.0f }, { 0.66f, 0.66f, 1.0f }, { 0.5f, 1.0f, 1.0f }, { 1.0f, 0.5f, 1.0f },
	{ 2.5f, 2.5f, 1.0f },
} ;
static VEC3 *vs_res_scale_p[] = {
	&vs_res_scale_tbl[0], &vs_res_scale_tbl[0], &vs_res_scale_tbl[1], &vs_res_scale_tbl[2],
	&vs_res_scale_tbl[3], &vs_res_scale_tbl[4],
} ;

static VS_RES_STEP vs_res_spr0[] = {
	{ 1, 0xff, 5, 0xd, 4.0f },
	{ 1, 0xff, 0, 0x1b, 3.58f },
	{ 1, 0xff, 0, 0xa, 3.3400002f },
	{ 1, 0xff, 0, 0x20, 4.08f },
	{ 1, 0xff, 0, 0x10, 6.16f },
	{ 1, 0xff, 0, 0xa, 3.4099998f },
	{ 1, 0xff, 0, 0x16, 3.8899994f },
	{ 1, 0xff, 0, 0xe, 3.5300007f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr1[] = {
	{ 1, 0xff, 5, 0xd, 4.0f },
	{ 1, 0xff, 0, 0x18, 3.49f },
	{ 1, 0xff, 0, 0x1e, 3.4599998f },
	{ 1, 0xff, 0, 0xb, 3.5500002f },
	{ 1, 0xff, 0, 0x15, 3.4099998f },
	{ 1, 0xff, 0, 0xe, 2.3199997f },
	{ 1, 0xff, 0, 0x14, 4.91f },
	{ 1, 0xff, 0, 0x2e, 3.4800014f },
	{ 1, 0xff, 0, 0x18, 1.6499996f },
	{ 1, 0xff, 0, 0x2e, 3.5499992f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr2[] = {
	{ 4, 0xff, 5, 0x1d, 4.0f },
	{ 4, 0xff, 0, 0x12, 2.48f },
	{ 4, 0xff, 0, 0x16, 3.0900002f },
	{ 4, 0xff, 0, 0xe, 3.6100001f },
	{ 4, 0xff, 0, 0x18, 5.87f },
	{ 4, 0xff, 0, 0x1f, 3.2799997f },
	{ 4, 0xff, 0, 0xe, 3.210001f },
	{ 4, 0xff, 0, 0x1b, 2.9399986f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr3[] = {
	{ 2, 0xff, 5, 0x19, 4.0f },
	{ 2, 0xff, 0, 0xe, 3.08f },
	{ 2, 0xff, 0, 0x1b, 3.0100002f },
	{ 2, 0xff, 0, 0xf, 2.9499998f },
	{ 2, 0xff, 0, 0xe, 2.8500004f },
	{ 2, 0xff, 0, 0xc, 3.0599995f },
	{ 2, 0xff, 0, 0x1d, 3.3599997f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr4[] = {
	{ 3, 0xff, 5, 0x22, 4.0f },
	{ 3, 0xff, 0, 0x18, 3.25f },
	{ 3, 0xff, 0, 0x1e, 3.46f },
	{ 3, 0xff, 0, 0x20, 6.3500004f },
	{ 3, 0xff, 0, 0x12, 3.8900003f },
	{ 3, 0xff, 0, 0x17, 2.6899986f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr5[] = {
	{ 2, 0xff, 5, 0x22, 0.0f },
	{ 2, 0xff, 0, 0x18, 3.25f },
	{ 2, 0xff, 0, 0x1e, 3.46f },
	{ 2, 0xff, 0, 0x15, 5.0f },
	{ 2, 0xff, 0, 0x18, 2.5100002f },
	{ 2, 0xff, 0, 0x1c, 3.4199991f },
	{ 2, 0xff, 0, 0xe, 2.9400005f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr6[] = {
	{ 1, 0xff, 5, 1, 3.5f },
	{ 1, 0xff, 0, 0x19, 2.98f },
	{ 1, 0xff, 0, 0x20, 5.9999995f },
	{ 1, 0xff, 0, 0x12, 3.9000006f },
	{ 1, 0xff, 0, 0x17, 2.8900003f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP vs_res_spr7[] = {
	{ 2, 0xff, 5, 2, 4.0f },
	{ 2, 0xff, 0, 0x19, 3.54f },
	{ 2, 0xff, 0, 0x20, 6.0f },
	{ 2, 0xff, 0, 0x12, 3.8999996f },
	{ 2, 0xff, 0, 0x17, 2.8900003f },
	{ 0xff, 0xff, 0, 0, 0.0f },
} ;

static VS_RES_STEP *vs_res_spr_tbl[16] = {
	vs_res_spr0, vs_res_spr1, vs_res_spr2, vs_res_spr3, vs_res_spr4, vs_res_spr5, vs_res_spr6, vs_res_spr7,
	vs_res_spr0, vs_res_spr0, vs_res_spr0, vs_res_spr0, vs_res_spr0, vs_res_spr0, vs_res_spr0, vs_res_spr0,
} ;

/* where the message starts on x, by type */
static Float vs_res_x_tbl[16] = { 77.0f, 78.0f, 70.0f, 54.0f, 57.0f, 58.0f, 47.0f, 49.0f } ;

/* the sound each type asks for */
static Uint8 vs_res_se_tbl[16] = { 0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e } ;

static void vs_res_main( PLAYER_WORK *ew );
static void vs_res_end( PLAYER_WORK *ew );
void vs_res_bonus_set( int two_p );

/****************************************************************/
void vs_res_set( int no )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_FRONT, LIST_END ) ) ){
		ew->disp_flag = 0 ;
		ew->work_func = vs_res_main ;
		ew->obj = 0 ;
		ew->obj_flag = 0 ;
		ew->ef_no = 7 ;
		ew->type = no & 15 ;
		if( no & 0x80 ) vs_res_end( ew ) ;
	}
}

static void vs_res_init( PLAYER_WORK *ew );
static void vs_res_run( PLAYER_WORK *ew );
static void vs_res_end( PLAYER_WORK *ew );

/*==============================================================*/
static void vs_res_main( PLAYER_WORK *ew )
{
	static void ( *vs_res_jmp[] )( PLAYER_WORK *ew ) = {
		vs_res_init, vs_res_run, vs_res_end, vs_res_end,
	} ;

	vs_res_jmp[ew->routine_0]( ew ) ;
}

/*==============================================================*/
static void vs_res_init( PLAYER_WORK *ew )
{
	Uint8 se ;

	if( flash_flag_get() ) return ;
	ew->routine_0 ++ ;
	ew->disp_flag = 1 ;
	ew->timer0 = 60 ;
	ew->pos.x = -vs_res_x_tbl[ew->type] / 2.0f ;
	ew->pos.y = -6.25f ;
	ew->pos.z = 0.0f ;
	VS_RES_SW( ew )->step = vs_res_spr_tbl[ew->type] ;
	VS_RES_SW( ew )->no = 0 ;
	se = vs_res_se_tbl[ew->type] ;
	if( se ) na_req( se ) ;
	vs_res_run( ew ) ;
}

static void vs_res_come_set( PLAYER_WORK *ew );
static void vs_res_come( PLAYER_WORK *ew );
static void vs_res_stay( PLAYER_WORK *ew );
static void vs_res_go( PLAYER_WORK *ew );

/*--------------------------------------------------------------*/
static void vs_res_run( PLAYER_WORK *ew )
{
	static void ( *vs_res_run_jmp[] )( PLAYER_WORK *ew ) = {
		vs_res_come_set, vs_res_come, vs_res_stay, vs_res_go,
	} ;

	vs_res_run_jmp[ew->routine_1]( ew ) ;
}

/*==============================================================*/
static void vs_res_come_set( PLAYER_WORK *ew )
{
	ew->routine_1 ++ ;
	ew->timer0 = 12 ;
	ew->spd.z = ( -ew->pos.z - 40.0f ) / 12.0f ;
	vs_res_come( ew ) ;
}

/*--------------------------------------------------------------*/
static void vs_res_come( PLAYER_WORK *ew )
{
	ew->pos.z += ew->spd.z ;
	if( -- ew->timer0 <= 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 60 ;
	}
}

/*--------------------------------------------------------------*/
static void vs_res_stay( PLAYER_WORK *ew )
{
	if( -- ew->timer0 <= 0 ){
		ew->routine_1 ++ ;
		ew->timer0 = 12 ;
		ew->spd.z = ( -ew->pos.z + 0.0f ) / 12.0f ;
	}
}

/*--------------------------------------------------------------*/
static void vs_res_go( PLAYER_WORK *ew )
{
	ew->pos.z += ew->spd.z ;
	if( -- ew->timer0 > 0 ) return ;
	vs_res_end( ew ) ;
}

/*--------------------------------------------------------------*/
static void vs_res_end( PLAYER_WORK *ew )
{
	ew->disp_flag = 0 ;
	if( ew->type == 3 ) bonus_screen_set( Sys->vs_flag ) ;
	work_free( ew ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "vs_result.c" End of File	*/
