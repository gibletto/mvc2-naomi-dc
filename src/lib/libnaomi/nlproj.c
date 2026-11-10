/* nlproj: the NAOMI graphics library's matrices for the view and the projection, the window and its pixel clipping,
   and the screen point of a model point. In the game it is one module, the pixel clipping of libnaomik2 inside it.
   Two routines are in assembler (#pragma inline_asm): the multiplication by a 4x4 matrix with ftrv, and the
   load of a transposed one. */
#include "nlsystem.h"

#define fabs( x )	_builtin_fabsf( x )

/* a 4x4 matrix, in columns */
typedef Float NLMATRIX[16];

typedef struct NLVECTOR2 {
	Float	x, y;
} NLVECTOR2;

typedef struct NLVECTOR3 {
	Float	x, y, z;
} NLVECTOR3;

typedef struct NLVECTOR4 {
	Float	x, y, z, w;
} NLVECTOR4;

typedef struct NLMATRIXENTRY {
	Sint32	unk_0, unk_4;
	NLMATRIX	*matrix;	/* 08 */
} NLMATRIXENTRY;

/* what the model drawing takes from the projection */
typedef struct NLOBJPUTPARAM {
	Uint8	unk_00[12];
	Float	znear_neg;	/* 0c */
	Uint8	unk_10[16];
	Float	znear_inv;	/* 20 */
	Float	znear;		/* 24 */
	Float	zfar;		/* 28 */
} NLOBJPUTPARAM;

extern NLSYSWORK NLSystemWork_;
extern NLOBJPUTPARAM nl_objput_param;
extern NLMATRIXENTRY nlMatrixStack[];
extern Sint32 nl_VGA_flag, nl_PAL_EXT_flag, nl_ext_pal_height;
extern void nl_requset_PixelClipping( Float left, Float top, Float width, Float height );

extern void nlMultiMatrix( NLMATRIX *m );
extern void nlMultiPushedMatrix( void );
extern void nlPushMatrix( NLMATRIX *m );
extern void nlPopMatrix( Sint32 n );
extern void nlDecMatrixPtr( Sint32 n );
extern void nlGetMatrix( NLMATRIX *m );
extern void nlGetMatrix8( NLMATRIX *m );
extern void nlLoadMatrix( NLMATRIX *m );
extern void nlLoadMatrix8( NLMATRIX *m );
extern void nlMultiMatrix8( NLMATRIX *m );
extern void nlTranslate( Float x, Float y, Float z );
extern Float nlTan( Sint32 ang );
extern Float nlSin( Sint32 ang );
extern Float nlCos( Sint32 ang );
extern Sint32 nlArcTan( Float x );
extern void nlCalcPoint( NLVECTOR3 *pos, NLVECTOR3 *out );
extern void nlCalcPoint4( NLVECTOR3 *pos, NLVECTOR4 *out );
extern void kmSetPixelClipping( Sint32 left, Sint32 top, Sint32 width, Sint32 height );

/* declared before nl_focusX */
extern Float nl_projectMatrixWidth, nl_projectMatrixHeight;

void nl_set_cnv_proj_pers( void );
void nlMakeProjConvMat( NLMATRIX *m );
void nlSetPixelClipping( Float left, Float top, Float width, Float height );

Float nl_focusX = 640.0f;
Float nl_focusY = 640.0f;
Float nl_focus = 640.0f;
Sint32 nl_reg_clip_xmin = -1;
Sint32 nl_reg_clip_xmax = -1;
Sint32 nl_reg_clip_ymin = -1;
Sint32 nl_reg_clip_ymax = -1;
Sint32 nl_reg_clip_xmin0 = -1;
Sint32 nl_reg_clip_xmax0 = -1;
Sint32 nl_reg_clip_ymin0 = -1;
Sint32 nl_reg_clip_ymax0 = -1;
Sint32 nl_reg_clip_start = 0;
Float nl_sft_pers_x = 0.0f;
Float nl_sft_pers_y = 0.0f;
Float nl_projectMatrixWidth = 640.0f;
Float nl_projectMatrixHeight = 480.0f;
Float nl_projectMatrixLeft = 0.0f;
Float nl_projectMatrixTop = 0.0f;
Sint32 nl_requset_PixelClipping_on = 0;

/* the matrices lie on 8-byte boundaries */
#pragma aligndata8( nl_modelviewMatrix, nl_projectMatrix, nl_projConvMat, nl_cnv_proj_pers, nl_cnv_proj_pers0 )
NLMATRIX nl_modelviewMatrix;
NLMATRIX nl_projectMatrix;
NLMATRIX nl_projConvMat;
NLVECTOR4 nl_sideclip_vector[4];
Float nl_focusX2;
Float nl_focusY2;
Float nl_windowSizeWidth;
Float nl_windowSizeHeight;
Float nl_windowSizeLeft;
Float nl_windowSizeTop;
Float nl_requset_PixelClipping_left;
Float nl_requset_PixelClipping_top;
Float nl_requset_PixelClipping_width;
Float nl_requset_PixelClipping_height;
NLMATRIX nl_cnv_proj_pers;
NLMATRIX nl_cnv_proj_pers0[2];

void nlFrustum( Float left, Float right, Float bottom, Float top, Float znear, Float zfar )
{
	Float m[16];

	m[0] = znear * 2.0f / ( right - left );
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = znear * 2.0f / ( top - bottom );
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = ( left + right ) / ( right - left );
	m[9] = ( bottom + top ) / ( top - bottom );
	m[10] = ( -znear - zfar ) / ( zfar - znear );
	m[11] = -1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -( znear * zfar * 2.0f ) / ( zfar - znear );
	m[15] = 0.0f;
	NLSystemWork_.znear = znear;
	NLSystemWork_.zfar = zfar;
	NLSystemWork_.znear_inv = 1.0f / znear;
	nl_objput_param.znear = znear;
	nl_objput_param.znear_neg = -znear;
	nl_objput_param.zfar = zfar;
	nl_objput_param.znear_inv = 1.0f / znear;
	nlMultiMatrix( &m );
	nl_set_cnv_proj_pers();
}

void nlFrustum_L( Float left, Float right, Float bottom, Float top, Float znear, Float zfar )
{
	Float m[16];

	m[0] = znear * 2.0f / ( right - left );
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = znear * 2.0f / ( top - bottom );
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = ( left + right ) / ( right - left );
	m[9] = ( bottom + top ) / ( top - bottom );
	m[10] = ( -znear - zfar ) / ( zfar - znear );
	m[11] = 1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = znear * zfar * 2.0f / ( zfar - znear );
	m[15] = 0.0f;
	NLSystemWork_.znear = znear;
	NLSystemWork_.zfar = zfar;
	NLSystemWork_.znear_inv = 1.0f / znear;
	nl_objput_param.znear = znear;
	nl_objput_param.znear_neg = -znear;
	nl_objput_param.zfar = zfar;
	nl_objput_param.znear_inv = 1.0f / znear;
	nlMultiMatrix( &m );
	nl_set_cnv_proj_pers();
}

void nlMakeModelProjectMatrix( void )
{
	nlPushMatrix( nlMatrixStack[3].matrix );
	nlMultiPushedMatrix();
	nlGetMatrix8( &nl_projectMatrix );
	nlPopMatrix( 1 );
}

void nlMakeModelViewProjectMatrix( void )
{
	nlPushMatrix( nlMatrixStack[3].matrix );
	nlMultiMatrix( nlMatrixStack[2].matrix );
	nlMultiPushedMatrix();
	nlGetMatrix8( &nl_projectMatrix );
	nlPopMatrix( 1 );
}

void nlMakeModelViewMatrix( void )
{
	nlPushMatrix( nlMatrixStack[2].matrix );
	nlMultiPushedMatrix();
	nlGetMatrix( &nl_modelviewMatrix );
	nlPopMatrix( 1 );
}

void nlShiftPerspective( Float x, Float y )
{
	nl_sft_pers_x = fabs( x );
	nl_sft_pers_y = fabs( y );
	nlTranslate( x, y, 0.0f );
}

void nlPerspective( Sint32 fov, Float aspect, Float znear, Float zfar )
{
	NLMATRIX m;
	Sint32 ang;
	Float focal, sx, sy, side;

	focal = 1.0f / nlTan( fov >> 1 );
	if( nl_sft_pers_x != 0.0f ){
		ang = nlArcTan( aspect / focal );
		ang = nlArcTan( nlTan( ang ) * ( nl_sft_pers_x + 1.0f ) );
		sx = 1.0f / nlCos( ang );
	}else
		sx = 1.0f / nlCos( nlArcTan( aspect / focal ) );
	if( nl_sft_pers_y != 0.0f ){
		ang = nlArcTan( nlTan( fov >> 1 ) * ( nl_sft_pers_y + 1.0f ) );
		sy = 1.0f / nlCos( ang );
	}else
		sy = 1.0f / nlCos( fov >> 1 );
	nl_sft_pers_x = 0.0f;
	nl_sft_pers_y = 0.0f;
	nl_focusX = focal / aspect * sx * 0.5f;
	nl_focusY = focal * sy * 0.5f;
	nl_focusX2 = nl_projectMatrixWidth * nl_focusX;
	nl_focusY2 = nl_projectMatrixHeight * nl_focusY;
	if( nl_focusX2 > nl_focusY2 )
		nl_focus = nl_focusX2;
	else
		nl_focus = nl_focusY2;
	side = nlArcTan( nlTan( fov >> 1 ) * aspect );
	nl_sideclip_vector[0].y = nl_sideclip_vector[1].y = 0.0f;
	nl_sideclip_vector[2].x = nl_sideclip_vector[3].x = 0.0f;
	nl_sideclip_vector[0].x = nlCos( side );
	nl_sideclip_vector[1].x = -nl_sideclip_vector[0].x;
	nl_sideclip_vector[0].z = nl_sideclip_vector[1].z = nlSin( side );
	nl_sideclip_vector[2].y = nlCos( fov >> 1 );
	nl_sideclip_vector[3].y = -nl_sideclip_vector[2].y;
	nl_sideclip_vector[2].z = nl_sideclip_vector[3].z = nlSin( fov >> 1 );
	nl_sideclip_vector[0].w = nl_sideclip_vector[1].w = 0.0f;
	nl_sideclip_vector[2].w = nl_sideclip_vector[3].w = 0.0f;
	m[0] = focal / aspect;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = focal;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = ( znear + zfar ) / ( znear - zfar );
	m[11] = -1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = znear * zfar * 2.0f / ( znear - zfar );
	m[15] = 0.0f;
	NLSystemWork_.znear = znear;
	NLSystemWork_.zfar = zfar;
	NLSystemWork_.znear_inv = 1.0f / znear;
	nl_objput_param.znear = znear;
	nl_objput_param.znear_neg = -znear;
	nl_objput_param.zfar = zfar;
	nl_objput_param.znear_inv = 1.0f / znear;
	nlMultiMatrix( &m );
	nl_set_cnv_proj_pers();
}

void nlPerspectiveX( Sint32 fov, Float aspect, Float znear, Float zfar )
{
	NLMATRIX m;
	Sint32 ang;
	Float focal, sx, sy, side;

	focal = 1.0f / nlTan( fov >> 1 );
	if( nl_sft_pers_x != 0.0f ){
		ang = nlArcTan( nlTan( fov >> 1 ) * ( nl_sft_pers_x + 1.0f ) );
		sx = 1.0f / nlCos( ang );
	}else
		sx = 1.0f / nlCos( fov >> 1 );
	if( nl_sft_pers_y != 0.0f ){
		ang = nlArcTan( 1.0f / ( aspect * focal ) );
		ang = nlArcTan( nlTan( ang ) * ( nl_sft_pers_y + 1.0f ) );
		sy = 1.0f / nlCos( ang );
	}else
		sy = 1.0f / nlCos( nlArcTan( 1.0f / ( aspect * focal ) ) );
	nl_sft_pers_x = 0.0f;
	nl_sft_pers_y = 0.0f;
	nl_focusX = focal * sx * 0.5f;
	nl_focusY = aspect * focal * sy * 0.5f;
	nl_focusX2 = nl_projectMatrixWidth * nl_focusX;
	nl_focusY2 = nl_projectMatrixHeight * nl_focusY;
	if( nl_focusX2 > nl_focusY2 )
		nl_focus = nl_focusX2;
	else
		nl_focus = nl_focusY2;
	side = nlArcTan( nlTan( fov >> 1 ) / aspect );
	nl_sideclip_vector[0].y = nl_sideclip_vector[1].y = 0.0f;
	nl_sideclip_vector[2].x = nl_sideclip_vector[3].x = 0.0f;
	nl_sideclip_vector[0].x = nlCos( fov >> 1 );
	nl_sideclip_vector[1].x = -nl_sideclip_vector[0].x;
	nl_sideclip_vector[0].z = nl_sideclip_vector[1].z = nlSin( fov >> 1 );
	nl_sideclip_vector[2].y = nlCos( side );
	nl_sideclip_vector[3].y = -nl_sideclip_vector[2].y;
	nl_sideclip_vector[2].z = nl_sideclip_vector[3].z = nlSin( side );
	nl_sideclip_vector[0].w = nl_sideclip_vector[1].w = 0.0f;
	nl_sideclip_vector[2].w = nl_sideclip_vector[3].w = 0.0f;
	m[0] = focal;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = aspect * focal;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = ( znear + zfar ) / ( znear - zfar );
	m[11] = -1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = znear * zfar * 2.0f / ( znear - zfar );
	m[15] = 0.0f;
	NLSystemWork_.znear = znear;
	NLSystemWork_.zfar = zfar;
	NLSystemWork_.znear_inv = 1.0f / znear;
	nl_objput_param.znear = znear;
	nl_objput_param.znear_neg = -znear;
	nl_objput_param.zfar = zfar;
	nl_objput_param.znear_inv = 1.0f / znear;
	nlMultiMatrix( &m );
	nl_set_cnv_proj_pers();
}

void nlPerspective_L( Sint32 fov, Float aspect, Float znear, Float zfar )
{
	NLMATRIX m;
	Sint32 ang;
	Float focal, sx, sy;

	focal = 1.0f / nlTan( fov >> 1 );
	if( nl_sft_pers_x != 0.0f ){
		ang = nlArcTan( aspect / focal );
		ang = nlArcTan( nlTan( ang ) * ( nl_sft_pers_x + 1.0f ) );
		sx = 1.0f / nlCos( ang );
	}else
		sx = 1.0f / nlCos( nlArcTan( aspect / focal ) );
	if( nl_sft_pers_y != 0.0f ){
		ang = nlArcTan( nlTan( fov >> 1 ) * ( nl_sft_pers_y + 1.0f ) );
		sy = 1.0f / nlCos( ang );
	}else
		sy = 1.0f / nlCos( fov >> 1 );
	nl_sft_pers_x = 0.0f;
	nl_sft_pers_y = 0.0f;
	nl_focusX = focal / aspect * sx * 0.5f;
	nl_focusY = focal * sy * 0.5f;
	nl_focusX2 = nl_projectMatrixWidth * nl_focusX;
	nl_focusY2 = nl_projectMatrixHeight * nl_focusY;
	if( nl_focusX2 > nl_focusY2 )
		nl_focus = nl_focusX2;
	else
		nl_focus = nl_focusY2;
	m[0] = focal / aspect;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = focal;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = ( znear + zfar ) / ( znear - zfar );
	m[11] = 1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -( znear * zfar * 2.0f ) / ( znear - zfar );
	m[15] = 0.0f;
	NLSystemWork_.znear = znear;
	NLSystemWork_.zfar = zfar;
	NLSystemWork_.znear_inv = 1.0f / znear;
	nl_objput_param.znear = znear;
	nl_objput_param.znear_neg = -znear;
	nl_objput_param.zfar = zfar;
	nl_objput_param.znear_inv = 1.0f / znear;
	nlMultiMatrix( &m );
	nl_set_cnv_proj_pers();
}

void nlPerspectiveX_L( Sint32 fov, Float aspect, Float znear, Float zfar )
{
	NLMATRIX m;
	Sint32 ang;
	Float focal, sx, sy;

	focal = 1.0f / nlTan( fov >> 1 );
	if( nl_sft_pers_x != 0.0f ){
		ang = nlArcTan( nlTan( fov >> 1 ) * ( nl_sft_pers_x + 1.0f ) );
		sx = 1.0f / nlCos( ang );
	}else
		sx = 1.0f / nlCos( fov >> 1 );
	if( nl_sft_pers_y != 0.0f ){
		ang = nlArcTan( 1.0f / ( aspect * focal ) );
		ang = nlArcTan( nlTan( ang ) * ( nl_sft_pers_y + 1.0f ) );
		sy = 1.0f / nlCos( ang );
	}else
		sy = 1.0f / nlCos( nlArcTan( 1.0f / ( aspect * focal ) ) );
	nl_sft_pers_x = 0.0f;
	nl_sft_pers_y = 0.0f;
	nl_focusX = focal * sx * 0.5f;
	nl_focusY = aspect * focal * sy * 0.5f;
	nl_focusX2 = nl_projectMatrixWidth * nl_focusX;
	nl_focusY2 = nl_projectMatrixHeight * nl_focusY;
	if( nl_focusX2 > nl_focusY2 )
		nl_focus = nl_focusX2;
	else
		nl_focus = nl_focusY2;
	m[0] = focal;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = aspect * focal;
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = ( znear + zfar ) / ( znear - zfar );
	m[11] = 1.0f;
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = -( znear * zfar * 2.0f ) / ( znear - zfar );
	m[15] = 0.0f;
	NLSystemWork_.znear = znear;
	NLSystemWork_.zfar = zfar;
	NLSystemWork_.znear_inv = 1.0f / znear;
	nl_objput_param.znear = znear;
	nl_objput_param.znear_neg = -znear;
	nl_objput_param.zfar = zfar;
	nl_objput_param.znear_inv = 1.0f / znear;
	nlMultiMatrix( &m );
	nl_set_cnv_proj_pers();
}

void nlProjectScreen( NLVECTOR3 *pos, NLVECTOR2 *scr )
{
	NLVECTOR4 v;
	Float w;

	nlPushMatrix( &nl_projectMatrix );
	nlCalcPoint4( pos, &v );
	w = 1.0f / v.w;
	v.x *= w;
	v.y *= w;
	scr->x = ( v.x + 1.0f ) * nl_projectMatrixWidth * 0.5f + nl_projectMatrixTop;
	scr->y = ( 1.0f - v.y ) * nl_projectMatrixHeight * 0.5f + nl_projectMatrixLeft;
	nlPopMatrix( 1 );
}

void nlProjectScreen3D( NLVECTOR3 *pos, NLVECTOR3 *out )
{
	nlPushMatrix( &nl_modelviewMatrix );
	nlCalcPoint( pos, out );
	nlPopMatrix( 1 );
}

void nlMakeProjConvMat( NLMATRIX *mat )
{
	Float *m = *mat;

	m[0] = nl_projectMatrixWidth * 0.5f;
	m[1] = 0.0f;
	m[2] = 0.0f;
	m[3] = 0.0f;
	m[4] = 0.0f;
	m[5] = -( nl_projectMatrixHeight * 0.5f );
	m[6] = 0.0f;
	m[7] = 0.0f;
	m[8] = 0.0f;
	m[9] = 0.0f;
	m[10] = -0.5f;
	m[11] = 0.0f;
	m[12] = nl_projectMatrixLeft + nl_projectMatrixWidth * 0.5f;
	m[13] = nl_projectMatrixTop + nl_projectMatrixHeight * 0.5f;
	m[14] = 0.5f;
	m[15] = 1.0f;
}

void nlSetWindowSize( Float left, Float top, Float width, Float height )
{
	Float t = top;
	Float h = height;

	if( nl_scr.vga != 0 ){
		if( nl_scr.height * 2 <= 480 ){
			t *= 2.0f;
			h *= 2.0f;
		}
	}
	nl_windowSizeLeft = left;
	nl_windowSizeTop = top;
	nl_windowSizeWidth = width;
	nl_windowSizeHeight = height;
	nl_projectMatrixTop = t;
	nl_projectMatrixHeight = h;
	if( nl_scr.vga != 0 ){
		nl_projectMatrixLeft = left * 2.0f;
		nl_projectMatrixWidth = width * 2.0f;
	}else{
		nl_projectMatrixLeft = left;
		nl_projectMatrixWidth = width;
	}
	nl_focusX2 = nl_projectMatrixWidth * nl_focusX;
	nl_focusY2 = nl_projectMatrixHeight * nl_focusY;
	if( nl_focusX2 > nl_focusY2 )
		nl_focus = nl_focusX2;
	else
		nl_focus = nl_focusY2;
	nlMakeProjConvMat( &nl_projConvMat );
	nlSetPixelClipping( left, top, width, height );
}

void nl_requset_PixelClipping( Float left, Float top, Float width, Float height )
{
	nl_requset_PixelClipping_on = 1;
	nl_requset_PixelClipping_left = left;
	nl_requset_PixelClipping_top = top;
	nl_requset_PixelClipping_width = width;
	nl_requset_PixelClipping_height = height;
}

void nl_action_PixelClipping( void )
{
	if( nl_requset_PixelClipping_on ){
		kmSetPixelClipping( nl_requset_PixelClipping_left, nl_requset_PixelClipping_top,
			nl_requset_PixelClipping_width, nl_requset_PixelClipping_height );
		nl_requset_PixelClipping_on = 0;
	}
}

void nl_cancel_PixelClipping( void )
{
	nl_requset_PixelClipping_on = 0;
}

void nlSetPixelClipping( Float left, Float top, Float width, Float height )
{
	Sint32 x0 = left;
	Sint32 y0 = top;
	Sint32 x1 = left + width;
	Sint32 y1 = top + height;
	Float scale;

	if( x0 < 0 )
		x0 = 0;
	else if( x0 >= nl_scr.width )
		x0 = nl_scr.width;
	if( x1 < 0 )
		x1 = 0;
	else if( x1 >= nl_scr.width )
		x1 = nl_scr.width;
	if( y0 < 0 )
		y0 = 0;
	else if( y0 >= nl_scr.height )
		y0 = nl_scr.height;
	if( y1 < 0 )
		y1 = 0;
	else if( y1 >= nl_scr.height )
		y1 = nl_scr.height;
	scale = 1.0f;
	if( nl_PAL_EXT_flag == 1 ){
		switch( nl_ext_pal_height ){
		case 0:
			scale = 1.033f;
			break;
		case 1:
			scale = 1.066f;
			break;
		case 2:
			scale = 1.1f;
			break;
		case 3:
			scale = 1.133f;
			break;
		case 4:
			scale = 1.166f;
			break;
		}
		nl_requset_PixelClipping( x0, y0 * scale, x1, y1 * scale );
	}else
		nl_requset_PixelClipping( x0, y0, x1, y1 );
	if( nl_scr.vga == 0 ){
		Sint32 tx0, ty0, tx1, ty1;

		tx0 = x0 >> 5;
		tx1 = x1 >> 5;
		ty0 = y0 >> 5;
		ty1 = y1 >> 5;
		if( nl_VGA_flag == 0 ){
			ty0 = 0;
			if( nl_scr.height * 2 <= 480 )
				ty1 = ( ( nl_scr.height + 15 ) >> 4 ) - 1;
			else
				ty1 = ( ( nl_scr.height + 31 ) >> 5 ) - 1;
		}
		nl_reg_clip_xmin = tx0;
		nl_reg_clip_xmax = tx1;
		nl_reg_clip_ymin = ty0;
		nl_reg_clip_ymax = ty1;
		if( nl_reg_clip_xmin0 != nl_reg_clip_xmin || nl_reg_clip_xmax0 != nl_reg_clip_xmax
		 || nl_reg_clip_ymin0 != nl_reg_clip_ymin || nl_reg_clip_ymax0 != nl_reg_clip_ymax )
			nl_reg_clip_start = 1;
		else
			nl_reg_clip_start = 0;
	}else{
		Sint32 tx0, ty0, tx1, ty1;

		tx0 = x0 >> 4;
		tx1 = x1 >> 4;
		ty0 = 0;
		if( nl_scr.height * 2 <= 480 )
			ty1 = ( ( nl_scr.height + 15 ) >> 4 ) - 1;
		else
			ty1 = ( ( nl_scr.height + 31 ) >> 5 ) - 1;
		nl_reg_clip_xmin = tx0;
		nl_reg_clip_xmax = tx1;
		nl_reg_clip_ymin = ty0;
		nl_reg_clip_ymax = ty1;
		if( nl_reg_clip_xmin0 != nl_reg_clip_xmin || nl_reg_clip_xmax0 != nl_reg_clip_xmax
		 || nl_reg_clip_ymin0 != nl_reg_clip_ymin || nl_reg_clip_ymax0 != nl_reg_clip_ymax )
			nl_reg_clip_start = 1;
		else
			nl_reg_clip_start = 0;
	}
}

void nlScreenPoint( Float x, Float y, Float z, Float *sx, Float *sy, Float *sz )
{
	NLVECTOR3 pos;
	NLVECTOR4 v;
	Float w;

	nlPushMatrix( &nl_projConvMat );
	nlMultiMatrix( &nl_projectMatrix );
	pos.x = x;
	pos.y = y;
	pos.z = z;
	nlCalcPoint4( &pos, &v );
	w = 1.0f / v.w;
	*sx = v.x * w;
	*sy = v.y * w;
	*sz = w;
	nlPopMatrix( 1 );
}

void nlChangeFastProjectMatrix( void )
{
	nlLoadMatrix( &nl_projConvMat );
	nlMultiMatrix( &nl_projectMatrix );
}

void nlFastScreenPoint( NLVECTOR3 *pos, Float *scr )
{
	NLVECTOR4 v;
	Float w;

	nlCalcPoint4( pos, &v );
	w = 1.0f / v.w;
	scr[0] = v.x * w;
	scr[1] = v.y * w;
	scr[2] = w;
}

#ifdef HOST_PORT
extern void nl_host_mul_matrix_ftrv( NLMATRIX *a, NLMATRIX *b );
#define mul_matrix_ftrv( a, b )	nl_host_mul_matrix_ftrv( a, b )
#else
#pragma inline_asm(mul_matrix_ftrv)
static void mul_matrix_ftrv( NLMATRIX *a, NLMATRIX *b )
{
	FMOV.S	FR15,@-R15
	FMOV.S	FR14,@-R15
	FMOV.S	FR13,@-R15
	FMOV.S	FR12,@-R15
	FSCHG
	FMOV	@R4+,DR0
	FMOV	@R4+,DR2
	FMOV	@R4+,DR4
	FMOV	@R4+,DR6
	FMOV	@R4+,DR8
	FMOV	@R4+,DR10
	FMOV	@R4+,DR12
	FMOV	@R4+,DR14
	FSCHG
	FRCHG
	MOV	R5,R4
	PREF	@R4
	FTRV	XMTRX,FV0
	ADD	#32,R4
	FTRV	XMTRX,FV4
	FTRV	XMTRX,FV8
	FTRV	XMTRX,FV12
	PREF	@R4
	FRCHG
	FMOV.S	@R5+,FR0
	FMOV.S	@R5+,FR1
	FMOV.S	@R5+,FR2
	FMOV.S	@R5+,FR3
	FMOV.S	@R5+,FR4
	FMOV.S	@R5+,FR5
	FMOV.S	@R5+,FR6
	FMOV.S	@R5+,FR7
	FMOV.S	@R5+,FR8
	FMOV.S	@R5+,FR9
	FMOV.S	@R5+,FR10
	FMOV.S	@R5+,FR11
	FMOV.S	@R5+,FR12
	FMOV.S	@R5+,FR13
	FMOV.S	@R5+,FR14
	FMOV.S	@R5+,FR15
	FRCHG
	FTRV	XMTRX,FV0
	FTRV	XMTRX,FV4
	FTRV	XMTRX,FV8
	FTRV	XMTRX,FV12
	FRCHG
	FMOV.S	@R15+,FR12
	FMOV.S	@R15+,FR13
	FMOV.S	@R15+,FR14
	FMOV.S	@R15+,FR15
}
#endif

void nlMakeFastModelProjectMatrix( void )
{
	mul_matrix_ftrv( nlMatrixStack[3].matrix, &nl_projConvMat );
}

void nlMakeFastModelProjectMatrixVM( void )
{
	nlPushMatrix( &nl_projConvMat );
	nlMultiMatrix( nlMatrixStack[3].matrix );
	nlMultiMatrix( nlMatrixStack[2].matrix );
	nlMultiPushedMatrix();
	nlDecMatrixPtr( 1 );
}

void nlSetScaleFactor( Float factor )
{
	NLSystemWork_.scale_factor = factor;
}

#ifdef HOST_PORT
extern void nl_host_load_matrix_ftrv( NLMATRIX *m );
#define load_matrix_ftrv_asm( m )	nl_host_load_matrix_ftrv( m )
#else
#pragma inline_asm(load_matrix_ftrv_asm)
static void load_matrix_ftrv_asm( NLMATRIX *m )
{
	FRCHG
	FMOV.S	@R4+,FR1
	FMOV.S	@R4,FR2
	ADD	#8,R4
	FMOV.S	@R4,FR3
	FMOV.S	@R4+,FR0
	FNEG	FR3
	FMOV.S	@R4+,FR5
	FMOV.S	@R4,FR6
	ADD	#8,R4
	FMOV.S	@R4,FR7
	FMOV.S	@R4+,FR4
	FNEG	FR7
	FMOV.S	@R4+,FR9
	FMOV.S	@R4,FR10
	ADD	#8,R4
	FMOV.S	@R4,FR11
	FMOV.S	@R4+,FR8
	FNEG	FR11
	FMOV.S	@R4+,FR13
	FMOV.S	@R4,FR14
	ADD	#8,R4
	FMOV.S	@R4,FR15
	FMOV.S	@R4+,FR12
	FNEG	FR15
	FRCHG
}
#endif

/* a routine of its own around the assembler, so that callers call it rather than expand it */
static void load_matrix_ftrv( NLMATRIX *m )
{
	load_matrix_ftrv_asm( m );
}

void nl_set_cnv_proj_pers( void )
{
	NLMATRIX *work = &nl_cnv_proj_pers0[1];

	nlGetMatrix8( work );
	load_matrix_ftrv( &nl_projConvMat );
	nlMultiMatrix8( work );
	nlGetMatrix8( &nl_cnv_proj_pers );
	nlGetMatrix8( &nl_cnv_proj_pers0[0] );
	nlLoadMatrix8( work );
}
