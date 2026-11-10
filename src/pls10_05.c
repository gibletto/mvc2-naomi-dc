/* Tron Bonne's player effect 05: six sparks thrown out round a point, each a model of the file
   that is shown through nine frames in turn. */
#include "player.h"

/* the model each frame shows */
static Sint32 pls10_05_obj_no[9] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 } ;
static Sint32 *pls10_05_obj_tbl[3] = { pls10_05_obj_no, pls10_05_obj_no, pls10_05_obj_no } ;
static Sint32 pls10_05_frame_cnt[3] = { 9, 9, 9 } ;
static Float pls10_05_unk_0C25894C[3] = { 40.0f, 70.0f, 100.0f } ;
/* how fast they fly out, and their scale, for each of the three sizes */
static Float pls10_05_spd_tbl[3] = { 15.0f, 30.0f, 15.0f } ;
static VEC3 pls10_05_scale_tbl[3] = {
	{ 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f }, { 0.3f, 0.3f, 0.3f },
} ;

/* the sixth part of a turn for each piece, as a 16-bit angle */
#define PLS10_05_ANG( i )	( ( int )( ( i ) * 60 * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/*==============================================================*/
static void pls10_05_main( PLAYER_WORK *ew )
{
	ew->obj = obj_efky->obj[pls10_05_obj_tbl[0][ew->timer0]] ;
	ew->pos.x += ew->spd.x ;
	ew->pos.z += ew->spd.z ;
	if( ++ ew->timer0 >= pls10_05_frame_cnt[0] ) work_free( ew ) ;
}

/*==============================================================*/
static void pls10_05_set( VEC3 *pos, Uint8 dir, int no, int i )
{
	PLAYER_WORK *ew ;

	if( ( ew = work_get( 0, W_EF, LIST_END ) ) ){
		ew->disp_flag = 1 ;
		ew->work_func = pls10_05_main ;
		ew->obj_flag = 0x109 ;
		ew->pos = *pos ;
		ew->spd.x = pls10_05_spd_tbl[no] * nlSin( PLS10_05_ANG( i ) ) ;
		ew->spd.z = pls10_05_spd_tbl[no] * nlCos( PLS10_05_ANG( i ) ) ;
		ew->rot[2] = nlRand() % 0x4000 ;
		ew->scale = pls10_05_scale_tbl[no] ;
	}
}

/****************************************************************/
void pls10_05_burst_set( VEC3 *pos, Uint8 dir, Sint8 no )
{
	int i ;

	if( no > 2 ) return ;
	for( i = 0 ; i < 6 ; i ++ ) pls10_05_set( pos, dir, no, i ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "pls10_05.c" End of File	*/
