/* Angles and lengths: a point from a length and two angles, a vector's length and angles, and the
   matrix turned to look along a vector. */
#include "player.h"

const VEC3 unk_0C22A0C8[2] = { { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f } } ;	/* never used */

/* degrees to a 16-bit angle, rounded */
#define DEG_ANG( d )	( ( Sint32 )( ( d ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

/****************************************************************/
/*			The point len away at pitch degrees from			*/
/*			straight up and yaw degrees round from z			*/
/****************************************************************/
VEC3 polar_pos( Float len, Float pitch, Float yaw )
{
	VEC3 pos ;
	Float psin, ysin, pcos, ycos ;

	if( fabs( pitch ) == 90.0f ) pcos = 0.0f ;
	else pcos = nlCos( DEG_ANG( pitch ) ) ;
	if( fabs( yaw ) == 90.0f ) ycos = 0.0f ;
	else ycos = nlCos( DEG_ANG( yaw ) ) ;
	if( fabs( pitch ) == 180.0f ) psin = 0.0f ;
	else psin = nlSin( DEG_ANG( pitch ) ) ;
	if( fabs( yaw ) == 180.0f ) ysin = 0.0f ;
	else ysin = nlSin( DEG_ANG( yaw ) ) ;
	pos.x = len * psin * ysin ;
	pos.y = len * pcos ;
	pos.z = len * psin * ycos ;
	return pos ;
}

/****************************************************************/
/*			The same, with the height in degrees above level	*/
/****************************************************************/
VEC3 polar_pos2( Float len, Float elev, Float yaw )
{
	return polar_pos( len, 90.0f - elev, yaw ) ;
}

/****************************************************************/
/*			A vector's length, its angle from straight up		*/
/*			and its angle round from z							*/
/****************************************************************/
void vec_polar( VEC3 *v, Float *len, Sint32 *pitch, Sint32 *yaw )
{
	Float xz ;

	*len = v->x * v->x + v->z * v->z ;
	xz = _builtin_sqrtf( *len ) ;
	if( xz == 0.0f && v->y == 0.0f ) *pitch = 0 ;
	else *pitch = nlArcTan2( xz, v->y ) ;
	*len = _builtin_sqrtf( *len + v->y * v->y ) ;
	if( v->x == 0.0f && v->z == 0.0f ) *yaw = 0 ;
	else *yaw = nlArcTan2( v->x, v->z ) ;
}

/****************************************************************/
/*			The same, with the pitch from level					*/
/****************************************************************/
void vec_polar2( VEC3 *v, Float *len, Sint32 *pitch, Sint32 *yaw )
{
	vec_polar( v, len, pitch, yaw ) ;
	*pitch = *pitch - 0x4000 ;
}

/****************************************************************/
/*			Turn the matrix to look along v, rolled by roll		*/
/****************************************************************/
void vec_rotate( VEC3 *v, Sint32 roll )
{
	Sint32 pitch, yaw ;
	Float len ;

	vec_polar2( v, &len, &pitch, &yaw ) ;
	nlRotateAY( yaw ) ;
	nlRotateAX( pitch ) ;
	nlRotateAZ( roll ) ;
}

/****************************************************************/
/*			Turn the matrix to look from one point at			*/
/*			another												*/
/****************************************************************/
void look_rotate( VEC3 *from, VEC3 *to, Sint32 roll )
{
	VEC3 v ;

	v.x = to->x - from->x ;
	v.y = to->y - from->y ;
	v.z = to->z - from->z ;
	vec_rotate( &v, roll ) ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "matrix.c" End of File	*/
