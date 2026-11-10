/* Small routines for the works: the random numbers, the sort mode, a point's place on the screen,
   whether a work is on the screen, the angle between two points, and moving along an angle. */
#include "player.h"
#include "o_sub1.h"

extern void scr_matrix_set( void );	/* the camera's matrix */

/* a point on the screen, as get_2d_pos gives it */
typedef struct SCR_POS {
	Float	x;		/* 000 */
	Float	y;		/* 004 */
} SCR_POS;

static Uint8 rnd_hi ;
static Uint8 rnd_lo ;

/****************************************************************/
/*			Start the random numbers							*/
/****************************************************************/
void rnd_init( void )
{
	rnd_hi = 1 ;
	rnd_lo = 0xc3 ;
}

/****************************************************************/
/*			The next random number, 0 to 255					*/
/****************************************************************/
Uint32 Rnd( void )
{
	Uint16 seed ;
	int n ;

	seed = ( rnd_hi << 8 ) + rnd_lo ;
	n = seed * 3 / 256 ;
	rnd_lo += n ;
	rnd_hi = n ;
	return rnd_lo ;
}

/****************************************************************/
void SortModeSet( Sint32 mode )
{
	SortMode = mode ;
	nlSysSetAutoSortMode( mode ) ;
}

/****************************************************************/
/*			Where the point pos is on the 640 by 480 screen		*/
/****************************************************************/
void get_2d_pos( VEC3 *pos, SCR_POS *scr )
{
	Float z ;

	scr_matrix_set() ;
	nlMakeModelViewProjectMatrix( 640.0f, 480.0f ) ;
	nlScreenPoint( pos->x, pos->y, pos->z, &scr->x, &scr->y, &z ) ;
}

/****************************************************************/
/*			Whether the work's body is on the screen: 1 it		*/
/*			is, 0 it is not										*/
/****************************************************************/
int scr_in_ck( PLAYER_WORK *pl_w )
{
	SCR_POS scr ;
	Float size[2] ;
	Float x ;

	get_2d_pos( &pl_w->pos, &scr ) ;
	size[0] = pl_w->size_front * pl_w->scale.x * 1.6666666f ;
	size[1] = pl_w->size_back * pl_w->scale.x * 1.6666666f ;
	x = !pl_w->disp_dir ? size[1] : size[0] ;
	size[0] = size[0] + size[1] + DOT_X( 128.0f ) ;
	scr.x += DOT_X( 64.0f ) ;
	if( ( Uint16 )( scr.x + x ) > ( Uint16 )( size[0] + 640.0f ) ) return 0 ;
	size[0] = pl_w->size_top * pl_w->scale.y * 2.142857f + pl_w->size_bottom * pl_w->scale.y * 2.142857f + DOT_Y( 32.0f ) ;
	size[1] = DOT_Y( 16.0f ) + pl_w->size_bottom * pl_w->scale.y * 2.142857f ;
	if( ( Uint16 )( scr.y + size[1] ) > ( Uint16 )( size[0] + 480.0f ) ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Whether the work's body is on the screen across:	*/
/*			1 it is, 0 it is not								*/
/****************************************************************/
int scr_in_x_ck( PLAYER_WORK *pl_w )
{
	SCR_POS scr ;
	Float size[2] ;
	Float x ;

	get_2d_pos( &pl_w->pos, &scr ) ;
	size[0] = pl_w->size_front * pl_w->scale.x * 1.6666666f ;
	size[1] = pl_w->size_back * pl_w->scale.x * 1.6666666f ;
	x = !pl_w->disp_dir ? size[1] : size[0] ;
	size[0] = size[0] + size[1] + DOT_X( 128.0f ) ;
	scr.x += DOT_X( 64.0f ) ;
	if( ( Uint16 )( scr.x + x ) > ( Uint16 )( size[0] + 640.0f ) ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Whether the work's body is on the screen up and		*/
/*			down: 1 it is, 0 it is not							*/
/****************************************************************/
int scr_in_y_ck( PLAYER_WORK *pl_w )
{
	SCR_POS scr ;
	Float size[2] ;

	get_2d_pos( &pl_w->pos, &scr ) ;
	size[0] = pl_w->size_top * pl_w->scale.y * 2.142857f + pl_w->size_bottom * pl_w->scale.y * 2.142857f + DOT_Y( 32.0f ) ;
	size[1] = DOT_Y( 16.0f ) + pl_w->size_bottom * pl_w->scale.y * 2.142857f ;
	if( ( Uint16 )( scr.y + size[1] ) > ( Uint16 )( size[0] + 480.0f ) ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Whether the top of the work's body is below the		*/
/*			top of the screen									*/
/****************************************************************/
int scr_top_ck( PLAYER_WORK *pl_w )
{
	SCR_POS scr ;
	Float h ;

	get_2d_pos( &pl_w->pos, &scr ) ;
	h = DOT_Y( 16.0f ) + pl_w->size_bottom * pl_w->scale.y * 2.142857f ;
	if( scr.y + h < 0.0f ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Whether the work's head is above the ground			*/
/****************************************************************/
int ground_ck( PLAYER_WORK *pl_w )
{
	Float h ;

	h = -DOT_Y( 16.0f ) + pl_w->size_bottom * pl_w->scale.y * 2.142857f ;
	if( pl_w->pos.y + h > scr_work.stage_top ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			Whether the work's feet are above the bottom of		*/
/*			the screen											*/
/****************************************************************/
int scr_bottom_ck( PLAYER_WORK *pl_w )
{
	SCR_POS scr ;
	Float h ;

	get_2d_pos( &pl_w->pos, &scr ) ;
	h = DOT_Y( 16.0f ) + pl_w->size_top * pl_w->scale.y * 2.142857f ;
	if( scr.y - h > 480.0f ) return 0 ;
	return 1 ;
}

/****************************************************************/
/*			The angle from one point to another, 0 to 255		*/
/****************************************************************/
Uint8 caldir_pos_256( VEC3 *from, VEC3 *to )
{
	return ( 64 - nlAtan256( to->y - from->y, to->x - from->x ) ) & 0xff ;
}

/****************************************************************/
/*			Set the work's speed spd along its direction, on	*/
/*			top of its acceleration, and move it				*/
/****************************************************************/
void dir_spd_move( PLAYER_WORK *pl_w, Sint16 spd )
{
	Float s ;
	Sint16 rad ;

	s = spd * 256.0f ;
	rad = ( ( 40 - pl_w->dir ) & 31 ) << 11 ;
	pl_w->spd.x = s * nlCos( rad ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f + pl_w->acc.x ;
	pl_w->spd.y = s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f + pl_w->acc.y ;
	pl_w->pos.x += pl_w->spd.x ;
	pl_w->pos.y += pl_w->spd.y ;
}

/****************************************************************/
/*			Move the work spd along its direction				*/
/****************************************************************/
void dir_move( PLAYER_WORK *pl_w, Sint16 spd )
{
	Float s ;
	Sint16 rad ;

	s = spd * 256.0f ;
	rad = ( ( 40 - pl_w->dir ) & 31 ) << 11 ;
	pl_w->pos.x = pl_w->pos.x + s * nlCos( rad ) * 1000.0f / 100000.0f / 256.0f * 1.6666666f ;
	pl_w->pos.y = pl_w->pos.y + s * nlSin( rad ) * 1000.0f / 125000.0f / 256.0f * 2.142857f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "o_sub1.c" End of File	*/
