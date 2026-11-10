/* The scene of the third attract demo: its 40 works of kind 11, the camera that follows the key
   lists they step through, and the scene's task entry. The demo's works are at03_ef*.c. */
#include "player.h"

/* Kamui */
extern int obj_scale_max( VEC3 *scale ) ;	/* the demo takes it for an int, disp.c's returns a Float */
extern void bright_init( void ) ;
extern void ex_sound_load2( void ) ;
extern void total_vol_init( void ) ;

/* the uv scroll of a model */
extern void obj_uv_begin( int *src, void *obj ) ;
extern int obj_uv_next( void ) ;
extern void obj_uv_pos_get( VEC3 *pos ) ;
extern void obj_uv_pos_put( VEC3 *pos ) ;
extern void obj_uv_get( Float *x, Float *y ) ;
extern void obj_uv_put( Float *x, Float *y ) ;

/****************************************************************/
/*			The model's texture coordinates slide: the wave		*/
/*			on the water, the stars of the sky					*/
/****************************************************************/
void at02_uv_main( PLAYER_WORK *ew )
{
	Float px, py ;
	VEC3 pos ;
	int ofs, i ;

	if( ew->sub_no != 52 ){
		ew->ef_no += 36 ;
		if( ew->ef_no >= 360 ) ew->ef_no = 0 ;
		obj_uv_begin( obj_dm08->obj[ew->sub_no + 1], ew->obj ) ;
		ofs = 0 ;
		i = 0 ;
		while( obj_uv_next() == 0 ){
			obj_uv_pos_get( &pos ) ;
			pos.z = nlSin( ( int )( ( ew->ef_no + ofs ) * 65536.0f / 360.0f + 0.5f ) & 0xffff ) * 0.5f ;
			i ++ ;
			if( !( i & 1 ) ) ofs += 40 ;
			obj_uv_pos_put( &pos ) ;
		}
	} else {
		ew->ef_no ++ ;
		if( ew->ef_no > 50 ) ew->ef_no = 0 ;
		obj_uv_begin( obj_dm08->obj[ew->sub_no + 1], ew->obj ) ;
		while( obj_uv_next() == 0 ){
			obj_uv_get( &px, &py ) ;
			py += ew->ef_no * 0.02f ;
			obj_uv_put( &px, &py ) ;
		}
	}
}

/* the turn a work makes each frame about x, y and z, after its drawing flag */
typedef struct { Sint32 obj_flag, turn_x, turn_y, turn_z ; } AT02_LOCAL_W ;
#define AT02_LW( w )	( ( AT02_LOCAL_W * )&( w )->obj_flag )
/* a work of the pool; AT02_NEXT_W steps a work pointer on to the work made after it */
typedef struct { Uint8 buff[0x1d0] ; } AT02_WORK_BUFF ;
#ifdef HOST_PORT
#define AT02_NEXT_W( w )	( ( w ) = ( PLAYER_WORK * )( ( AT02_WORK_BUFF * )( w ) + 1 ) )
#else
#define AT02_NEXT_W( w )	( ( ( AT02_WORK_BUFF * )( w ) ) ++ )	/* the cast is used as an lvalue */
#endif

/****************************************************************/
/*			A point's step to another: its difference over a	*/
/*			count of frames										*/
/****************************************************************/
VEC3 at02_vec_step( Float cnt, Float *to, Float *from )
{
	VEC3 v ;

	v.x = ( *to ++ - *from ++ ) / cnt ;
	v.y = ( *to ++ - *from ++ ) / cnt ;
	v.z = ( *to - *from ) / cnt ;
	return v ;
}

/* what the scene's tables hold: a record of each work to make, and the angles of its tiles */
typedef struct {
	Sint16 frame ;	/* the frame it is made at */
	Uint8 kind, type ;
	Float alpha ;
	VEC3 pos ;
	void *key ;	/* its key list */
	Uint8 slot, pad[3] ;
} AT02_REC ;

/* a work's key list: the keys its position, rotation and scale (type bits 1, 2, 4) pass through */
typedef struct { Sint16 frame, ang ; Float v[3] ; } AT02_KEY_POS ;
typedef struct { Sint16 frame, ang ; Float v[2] ; } AT02_KEY_SCL ;
typedef struct { Sint16 frame, ang ; } AT02_KEY_ANG ;
typedef struct { Sint16 frame, pad ; Float v[3] ; Float s[2] ; } AT02_KEY_POSSCL ;
typedef struct { Sint16 frame, pad ; Float a ; Sint16 ang, pad2 ; Float v[3] ; } AT02_KEY_B ;
/* a camera key: its frame, then where the eye and the target are */
typedef struct { Sint16 frame, pad ; Float eye[3] ; Float target[3] ; } AT02_CAM_KEY ;

/* the scene's tables: the name tiles' places and order, the camera's and the works' key lists, a record of each work to make, the tiles' angles */
static Float at02_name_pos[59 * 4] = {
	-1.5f, 0.0f, 8.0f, 1.0f,
	-1.5f, 0.0f, 5.0f, 1.2f,
	-1.5f, 0.0f, 6.0f, 1.02f,
	-1.5f, 0.0f, 7.0f, 1.0f,
	-1.5f, 0.0f, 4.0f, 1.36f,
	-1.5f, 0.0f, 3.0f, 1.0f,
	1.5f, 0.0f, 11.04f, 1.21f,
	1.5f, -0.01f, 10.24f, 1.05f,
	1.5f, -0.05f, -9.7f, 1.0f,
	1.5f, 0.0f, 7.75f, 1.13f,
	1.5f, -0.05f, -10.47f, 1.0f,
	1.5f, 0.0f, 8.52f, 1.1f,
	1.5f, -0.02f, 9.52f, 1.0f,
	1.5f, 0.0f, 3.0f, 1.32f,
	1.5f, 0.0f, 4.0f, 1.1f,
	1.5f, 0.0f, 5.0f, 1.2f,
	-1.5f, -0.12f, -9.320001f, 0.85f,
	-1.5f, 0.0f, -10.17f, 1.0f,
	-1.5f, 0.0f, 10.76f, 1.0f,
	-1.5f, 0.0f, 13.93f, 1.08f,
	-1.5f, 0.0f, 13.0f, 0.95f,
	-1.5f, 0.0f, 12.0f, 1.2f,
	1.5f, -0.08f, 11.96f, 1.0f,
	1.5f, 0.0f, 12.93f, 1.26f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 0.0f,
	-1.5f, -0.03f, -7.62f, 0.9f,
	-1.48f, -0.03f, -11.76f, 0.9f,
	-1.5f, -0.03f, -10.84f, 0.9f,
	-1.5f, 0.0f, 1.0f, 1.0f,
	-1.5f, -0.15f, 1e+01f, 0.86f,
	-1.5f, 0.0f, -0.53f, 1.0f,
	-1.5f, 0.0f, -4.42f, 1.02f,
	-1.5f, -0.1f, -8.36f, 1.0f,
	-1.5f, 0.0f, -2.53f, 1.0f,
	-1.5f, 0.0f, -3.52f, 1.0f,
	-1.5f, 0.0f, -5.42f, 1.0f,
	-1.5f, 0.0f, -6.32f, 1.2f,
	-1.5f, -0.03f, -1.53f, 1.053f,
	1.5f, 0.0f, 2.0f, 1.1f,
	1.5f, 0.0f, 1.0f, 1.63f,
	1.5f, 0.0f, -8.0f, 1.15f,
	1.5f, 0.0f, -3.0f, 1.2f,
	1.5f, 0.0f, 7.0f, 1.1f,
	1.5f, 0.0f, 6.0f, 1.35f,
	1.5f, 0.0f, -4.0f, 1.05f,
	1.5f, 0.0f, -11.320001f, 1.1f,
	1.48f, 0.0f, 0.0f, 1.1f,
	1.5f, -0.07f, -7.0f, 1.0f,
	1.5f, 0.0f, -2.0f, 1.3f,
	1.5f, 0.0f, -5.0f, 1.05f,
	1.5f, 0.0f, -12.47f, 1.73f,
	1.5f, 0.0f, -6.0f, 1.66f,
	1.5f, 0.0f, -1.0f, 1.5f,
	-1.5f, -0.05f, 2.0f, 1.0f,
	-1.5f, 0.0f, 9.0f, 1.02f,
	1.5f, 0.0f, -9.0f, 1.2f,
	-1.5f, -0.15f, 0.18f, 0.7f,
} ;
static Uint8 at02_name_ord0[28] = { 19, 20, 21, 18, 31, 56, 0, 3, 2, 1, 4, 5, 55, 30, 58, 32, 39, 35, 36, 33, 37, 38, 27, 34, 16, 17, 29, 28 } ;
static Uint8 at02_name_ord1[28] = { 23, 22, 6, 7, 12, 11, 9, 44, 45, 15, 14, 13, 40, 41, 48, 54, 50, 43, 46, 51, 53, 49, 42, 57, 8, 10, 47, 52 } ;
static AT02_CAM_KEY at02_cam_a[4] = {
	{ 0, 0, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -15.0f } },
	{ 235, 0, { 6.68f, 0.0f, -7.36f }, { 6.68f, 0.0f, -15.0f } },
	{ 355, 0, { 8.54f, 0.0f, -13.3f }, { 8.54f, 0.0f, -15.0f } },
	{ -1, 0, { 8.54f, 0.0f, -13.3f }, { 8.54f, 0.0f, -15.0f } },
} ;
static AT02_CAM_KEY at02_cam_b[3] = {
	{ 0, 0, { 8.54f, 0.0f, -13.3f }, { 8.54f, 0.0f, -15.0f } },
	{ 30, 0, { 8.0f, 0.0f, -6.0f }, { 8.0f, 0.0f, -15.0f } },
	{ -1, 0, { 8.0f, 0.0f, -6.0f }, { 8.0f, 0.0f, -15.0f } },
} ;
static AT02_CAM_KEY at02_cam_c[2] = {
	{ 0, 0, { 2.42f, -0.54f, 22.29f }, { -1.09f, 0.18f, 18.14f } },
	{ 300, 0, { 2.42f, -0.54f, -9.23f }, { -1.09f, 0.18f, -13.65f } },
} ;
static AT02_CAM_KEY at02_cam_d[3] = {
	{ 0, 0, { -3.41f, -0.54f, 22.29f }, { 1.1f, 0.18f, 18.14f } },
	{ 300, 0, { -3.41f, -0.54f, -11.18f }, { 1.1f, 0.18f, -15.0f } },
	{ -1, 0, { 2.42f, -0.54f, -9.23f }, { -1.09f, 0.18f, -13.65f } },
} ;
static AT02_CAM_KEY at02_cam_e[3] = {
	{ 0, 0, { 5.0f, 0.0f, 0.0f }, { 5.0f, 0.0f, -15.0f } },
	{ 30, 0, { 5.0f, 0.0f, -8.0f }, { 5.0f, 0.0f, -15.0f } },
	{ -1, 0, { -8.0f, 0.0f, -6.0f }, { -8.0f, 0.0f, -15.0f } },
} ;
static AT02_CAM_KEY at02_cam_f[3] = {
	{ 0, 0, { 5.0f, 0.0f, -8.0f }, { 5.0f, 0.0f, -15.0f } },
	{ 30, 0, { -9.14f, 0.0f, -7.28f }, { -9.14f, 0.0f, -15.0f } },
	{ -1, 0, { -9.14f, 0.0f, -7.28f }, { -9.14f, 0.0f, -15.0f } },
} ;
static AT02_CAM_KEY at02_cam_g[3] = {
	{ 0, 0, { 0.0f, -3.0f, -5.0f }, { 0.0f, -3.0f, -15.0f } },
	{ 60, 0, { 0.0f, 0.0f, -5.0f }, { 0.0f, 0.0f, -15.0f } },
	{ -1, 0, { -9.14f, 0.0f, -7.28f }, { -9.14f, 0.0f, -15.0f } },
} ;
static AT02_CAM_KEY *at02_cam_keys[7] = { at02_cam_a, at02_cam_b, at02_cam_d, at02_cam_e, at02_cam_f, at02_cam_c, at02_cam_g } ;
static AT02_KEY_POS at02_key_k01a[4] = {
	{ 0, 0, { 0.0f, 0.0f, -4.0f } },
	{ 90, 0, { 0.0f, 0.0f, -15.0f } },
	{ 270, 0, { 0.0f, 0.0f, -15.0f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_POS at02_key_k01b[4] = {
	{ 0, 0, { 0.0f, 0.0f, -4.0f } },
	{ 15, 0, { 0.0f, 0.0f, -15.0f } },
	{ 240, 0, { 0.0f, 0.0f, -15.0f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_ANG at02_key_k04[4] = {
	{ 0, 0 },
	{ 150, 180 },
	{ 270, 360 },
	{ -1, 360 },
} ;
static AT02_KEY_ANG at02_key_k05[4] = {
	{ 0, 0 },
	{ 150, -180 },
	{ 270, -360 },
	{ -1, -360 },
} ;
static AT02_KEY_SCL at02_key_k06[15] = {
	{ 0, 45, { 3.0f, 1.0f } },
	{ 60, -135, { 2.0f, 2.0f } },
	{ 120, -315, { 3.0f, 3.0f } },
	{ 240, -630, { 3.0f, 3.0f } },
	{ 360, -945, { 2.0f, 2.0f } },
	{ -1, -630, { 3.0f, 3.0f } },
	{ 0, 45, { 1.8f, 1.8f } },
	{ 30, -45, { 2.4f, 2.4f } },
	{ 60, -135, { 3.0f, 3.0f } },
	{ 120, -315, { 1.0f, 1.0f } },
	{ 180, -450, { 3.0f, 3.0f } },
	{ 260, -720, { 2.0f, 2.0f } },
	{ 320, -855, { 3.0f, 1.0f } },
	{ 380, -990, { 6.0f, 6.0f } },
	{ -1, -630, { 3.0f, 3.0f } },
} ;
static AT02_KEY_SCL at02_key_k07[13] = {
	{ 0, -45, { 3.0f, 1.0f } },
	{ 60, -225, { 2.5f, 2.5f } },
	{ 120, -405, { 1.0f, 1.0f } },
	{ 180, -630, { 3.0f, 3.0f } },
	{ 240, -765, { 1.0f, 1.0f } },
	{ 300, -900, { 3.0f, 3.0f } },
	{ -2, -765, { 1.0f, 1.0f } },
	{ 0, -45, { 2.2f, 2.2f } },
	{ 240, -765, { 2.2f, 2.2f } },
	{ 260, -825, { 2.5f, 2.5f } },
	{ 320, -1005, { 3.0f, 1.0f } },
	{ 380, -1185, { 6.0f, 6.0f } },
	{ -1, -630, { 3.0f, 3.0f } },
} ;
static AT02_KEY_SCL at02_key_k08[16] = {
	{ 0, 90, { 3.0f, 1.0f } },
	{ 60, 270, { 3.0f, 3.0f } },
	{ 120, 450, { 1.0f, 1.0f } },
	{ 240, 810, { 1.0f, 1.0f } },
	{ 360, 1170, { 1.0f, 1.0f } },
	{ -1, 810, { 1.0f, 1.0f } },
	{ 0, 0, { 2.0f, 2.0f } },
	{ 60, 90, { 1.0f, 1.0f } },
	{ 120, 180, { 3.0f, 3.0f } },
	{ -3, 810, { 1.0f, 1.0f } },
	{ 0, 90, { 1.0f, 1.0f } },
	{ 180, 630, { 1.0f, 1.0f } },
	{ 260, 870, { 2.0f, 1.0f } },
	{ 320, 1050, { 3.0f, 1.0f } },
	{ 380, 1230, { 6.0f, 6.0f } },
	{ -1, -630, { 3.0f, 3.0f } },
} ;
static AT02_KEY_SCL at02_key_k09[21] = {
	{ 0, 0, { 3.0f, 1.0f } },
	{ 60, 180, { 1.0f, 1.0f } },
	{ 120, 360, { 3.0f, 3.0f } },
	{ 180, 585, { 1.0f, 1.0f } },
	{ 240, 720, { 3.0f, 3.0f } },
	{ 300, 855, { 1.0f, 1.0f } },
	{ -2, 720, { 3.0f, 3.0f } },
	{ 0, 0, { 1.0f, 1.0f } },
	{ 60, 90, { 3.0f, 3.0f } },
	{ 120, 180, { 2.0f, 2.0f } },
	{ -3, 810, { 1.0f, 1.0f } },
	{ 0, 0, { 3.0f, 3.0f } },
	{ 30, 90, { 2.0f, 2.0f } },
	{ 60, 180, { 1.0f, 1.0f } },
	{ 120, 360, { 2.0f, 2.0f } },
	{ 180, 540, { 3.0f, 3.0f } },
	{ 240, 720, { 3.0f, 3.0f } },
	{ 260, 900, { 1.0f, 1.0f } },
	{ 320, 990, { 3.0f, 1.0f } },
	{ 380, 1080, { 6.0f, 6.0f } },
	{ -1, -630, { 3.0f, 3.0f } },
} ;
static AT02_KEY_ANG at02_key_k10[3] = {
	{ 0, -120 },
	{ 95, 120 },
	{ -1, 120 },
} ;
static AT02_KEY_ANG at02_key_k13[4] = {
	{ 0, -60 },
	{ 35, 0 },
	{ 55, 0 },
	{ -1, 0 },
} ;
static AT02_KEY_POS at02_key_k14[4] = {
	{ 0, 90, { 7.0f, 0.5f, -14.0f } },
	{ 10, 0, { 9.98f, 0.5f, -14.5f } },
	{ 15, 0, { 9.98f, 0.5f, -14.5f } },
	{ -1, 0, { 9.98f, 0.5f, -14.5f } },
} ;
static AT02_KEY_POS at02_key_k16[4] = {
	{ 0, -90, { 1e+01f, -0.5f, -14.0f } },
	{ 10, 0, { 7.1f, -0.5f, -14.5f } },
	{ 15, 0, { 7.1f, -0.5f, -14.5f } },
	{ -1, 0, { 7.1f, -0.5f, -14.5f } },
} ;
static AT02_KEY_POS at02_key_k33[5] = {
	{ 0, 0, { 13.0f, 0.0f, -14.8f } },
	{ 10, 0, { 0.58f, 0.0f, -14.0f } },
	{ 20, 0, { 0.0f, 0.0f, -13.0f } },
	{ 30, 0, { 8.0f, 0.0f, -13.0f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_POS at02_key_k31[3] = {
	{ 0, 0, { 9.0f, -2.0f, -12.01f } },
	{ 20, 0, { 9.0f, 0.0f, -12.01f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_POSSCL at02_key_k35[4] = {
	{ 0, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
	{ 10, 0, { 0.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
	{ 20, 0, { 0.0f, -3.0f, -12.0f }, { 1.0f, 1.0f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_POSSCL at02_key_k18[3] = {
	{ 0, 0, { 13.0f, 4.5f, -14.0f }, { 1.0f, 0.0f } },
	{ 8, 0, { 7.5f, 3.0f, -14.0f }, { 1.0f, 1.0f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_POSSCL at02_key_k19[3] = {
	{ 0, 0, { 13.0f, 1.5f, -14.0f }, { 1.0f, 0.0f } },
	{ 8, 0, { 7.5f, 1.0f, -14.0f }, { 1.0f, 1.0f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_POSSCL at02_key_k20[3] = {
	{ 0, 0, { 13.0f, -1.5f, -14.0f }, { 1.0f, 0.0f } },
	{ 8, 0, { 7.5f, -1.0f, -14.0f }, { 1.0f, 1.0f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_POSSCL at02_key_k21[3] = {
	{ 0, 0, { 13.0f, -4.5f, -14.0f }, { 1.0f, 0.0f } },
	{ 8, 0, { 7.5f, -3.0f, -14.0f }, { 1.0f, 1.0f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_POS at02_key_k34[4] = {
	{ 0, 0, { 9.48f, 0.0f, -1e+01f } },
	{ 15, 0, { -13.75f, 0.0f, -11.8f } },
	{ 30, 0, { -9.2f, 0.0f, -14.5f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_POS at02_key_k32[4] = {
	{ 0, 0, { 0.0f, -2.0f, -6.0f } },
	{ 10, 0, { 0.0f, -2.0f, -1e+01f } },
	{ 20, 0, { -9.0f, -0.1f, -14.4f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_POSSCL at02_key_k37[4] = {
	{ 0, 0, { -35.8f, -3.5f, -13.0f }, { 1.0f, 0.1f } },
	{ 10, 0, { -17.0f, -3.5f, -13.0f }, { 1.0f, 0.1f } },
	{ 20, 0, { -17.0f, -3.5f, -13.0f }, { 1.0f, 1.0f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k22[3] = {
	{ 0, 0, { 1.0f, 0.0f } },
	{ 15, 0, { 1.0f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k29a[3] = {
	{ 0, 0, { 0.0f, 1.0f } },
	{ 7, 0, { 1.1f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k29b[3] = {
	{ 0, 0, { 0.0f, 1.0f } },
	{ 7, 0, { 1.5f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k29c[3] = {
	{ 0, 0, { 0.0f, 1.0f } },
	{ 7, 0, { 1.8f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k30a[3] = {
	{ 0, 0, { 0.0f, 1.0f } },
	{ 7, 0, { 0.25f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k30b[3] = {
	{ 0, 0, { 0.0f, 1.0f } },
	{ 7, 0, { 0.25f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_SCL at02_key_k30c[3] = {
	{ 0, 0, { 0.0f, 1.0f } },
	{ 10, 0, { 1.0f, 1.0f } },
	{ -1, 0, { 1.0f, 0.1f } },
} ;
static AT02_KEY_B at02_key_k39[5] = {
	{ 0, 0, -13.0f, 0, 0, { 1.0f, 1.0f, 1.0f } },
	{ 26, 0, -13.0f, 0, 0, { 1.0f, 1.0f, 1.0f } },
	{ 169, 0, -13.0f, 220, 0, { 1.0f, 1.0f, 7.4f } },
	{ 402, 0, 2e+01f, 2940, 0, { 1.0f, 1.0f, 8.0f } },
	{ -1, 0, 2e+01f, 360, 0, { 1.0f, 1.0f, 1e+01f } },
} ;
static AT02_KEY_POS at02_key_k40[9] = {
	{ 0, 0, { -13.0f, 1.0f, 0.0f } },
	{ 10, 0, { -13.0f, 1.0f, 0.0f } },
	{ 120, 0, { -13.0f, 7.6f, 0.0f } },
	{ 130, 0, { -10.0933f, 8.0f, 0.0f } },
	{ 140, 0, { -4.7655f, 8.4f, 0.0f } },
	{ 150, 0, { 1.4552f, 8.9f, 0.0f } },
	{ 160, 0, { 7.0f, 9.3f, 0.0f } },
	{ 210, 0, { 12.4657f, 9.6f, 0.0f } },
	{ -1, 0, { 2e+01f, 9.0f, 0.0f } },
} ;
static AT02_KEY_POSSCL at02_key_k47[4] = {
	{ 0, 0, { 5.1f, 0.0f, -13.0f }, { 3.0f, 3.0f } },
	{ 10, 0, { 5.1f, 0.0f, -13.0f }, { 1.5f, 1.5f } },
	{ 38, 0, { 5.1f, 0.0f, -13.0f }, { 1.5f, 1.5f } },
	{ -1, 0, { 25.0f, -3.0f, -12.0f }, { 1.0f, 0.1f } },
} ;
static AT02_KEY_POS at02_key_k46[3] = {
	{ 0, 0, { 0.0f, 0.5f, -1e+01f } },
	{ 120, 0, { 0.0f, 0.5f, -1.0f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
static AT02_KEY_POS at02_key_k48[3] = {
	{ 0, 0, { 0.0f, -0.5f, -1e+01f } },
	{ 120, 0, { 0.0f, -0.5f, -1.0f } },
	{ -1, 0, { 0.0f, 0.0f, -15.0f } },
} ;
AT02_REC at02_rec[64] = {
	{ 0, 46, 1, 0.0f, { 0.0f, 0.0f, -1e+01f }, at02_key_k46, 1 },
	{ 0, 48, 1, 0.0f, { 0.0f, 0.0f, -1e+01f }, at02_key_k48, 2 },
	{ 120, 0, 0, 1.0f, { 0.0f, 0.0f, -15.1f }, 0, 0 },
	{ 120, 1, 1, 1.0f, { 0.0f, 0.0f, -4.0f }, at02_key_k01a, 1 },
	{ 120, 3, 0, 1.0f, { 0.0f, 0.0f, -15.0f }, 0, 2 },
	{ 120, 4, 2, 1.0f, { 0.0f, 0.0f, -15.0f }, at02_key_k04, 3 },
	{ 120, 5, 2, 1.0f, { 0.0f, 0.0f, -15.0f }, at02_key_k05, 4 },
	{ 120, 6, 6, 1.0f, { 0.0f, 0.0f, -15.02f }, at02_key_k06, 5 },
	{ 120, 7, 6, 1.0f, { 0.0f, 0.0f, -15.04f }, at02_key_k07, 6 },
	{ 120, 8, 6, 1.0f, { 0.0f, 0.0f, -15.03f }, at02_key_k08, 7 },
	{ 120, 9, 6, 1.0f, { 0.0f, 0.0f, -15.05f }, at02_key_k09, 8 },
	{ 328, 10, 2, 0.0f, { 0.0f, 0.0f, -14.8f }, at02_key_k10, 9 },
	{ 348, 11, 2, 0.0f, { 0.0f, 0.0f, -14.8f }, at02_key_k10, 10 },
	{ 368, 12, 2, 0.0f, { 0.0f, 0.0f, -14.8f }, at02_key_k10, 11 },
	{ 408, 13, 2, 0.0f, { 0.0f, 0.0f, -14.8f }, at02_key_k13, 12 },
	{ 463, 14, 3, 1.0f, { 7.0f, 0.5f, -14.0f }, at02_key_k14, 9 },
	{ 463, 16, 3, 1.0f, { 1e+01f, -0.5f, -14.0f }, at02_key_k16, 10 },
	{ 493, 33, 1, 1.0f, { 13.0f, 0.0f, -14.8f }, at02_key_k33, 11 },
	{ 513, 31, 1, 0.0f, { 9.0f, -2.0f, -12.0f }, at02_key_k31, 9 },
	{ 523, 35, 5, 1.0f, { 25.0f, -3.0f, -12.0f }, at02_key_k35, 10 },
	{ 573, 50, 0, 1.0f, { 0.0f, 0.0f, 0.0f }, 0, 35 },
	{ 573, 52, 0, 1.0f, { 0.0f, 0.0f, 0.0f }, 0, 36 },
	{ 848, 18, 5, 1.0f, { 13.0f, 4.5f, -14.0f }, at02_key_k18, 9 },
	{ 856, 43, 0, 1.1f, { 7.5f, 3.0f, -13.99f }, 0, 31 },
	{ 863, 19, 5, 1.0f, { 13.0f, 1.5f, -14.0f }, at02_key_k19, 10 },
	{ 863, 29, 4, 1.0f, { 4.75f, 3.0f, -13.95f }, at02_key_k29b, 20 },
	{ 871, 43, 0, 1.1f, { 7.5f, 1.0f, -13.99f }, 0, 32 },
	{ 878, 20, 5, 1.0f, { 13.0f, -1.5f, -14.0f }, at02_key_k20, 11 },
	{ 878, 29, 4, 1.0f, { 4.75f, 1.0f, -13.95f }, at02_key_k29a, 21 },
	{ 886, 43, 0, 1.1f, { 7.5f, -1.0f, -13.99f }, 0, 33 },
	{ 893, 21, 5, 1.0f, { 13.0f, -4.5f, -14.0f }, at02_key_k21, 12 },
	{ 893, 29, 4, 1.0f, { 4.75f, -1.0f, -13.95f }, at02_key_k29a, 22 },
	{ 901, 43, 0, 1.1f, { 7.5f, -3.0f, -13.99f }, 0, 34 },
	{ 908, 29, 4, 1.0f, { 4.75f, -3.0f, -13.95f }, at02_key_k29b, 23 },
	{ 918, 47, 5, 1.0f, { 1.0f, 1.0f, 1.0f }, at02_key_k47, 17 },
	{ 980, 34, 1, 1.0f, { 9.48f, 0.0f, -1e+01f }, at02_key_k34, 13 },
	{ 990, 32, 1, 0.0f, { 0.0f, -2.0f, -6.0f }, at02_key_k32, 14 },
	{ 1010, 37, 5, 1.0f, { -35.8f, -3.5f, -13.0f }, at02_key_k37, 15 },
	{ 1335, 22, 4, 1.0f, { 4.5f, -4.8f, -14.0f }, at02_key_k22, 13 },
	{ 1350, 23, 4, 1.0f, { -6.0f, -5.5f, -14.0f }, at02_key_k22, 14 },
	{ 1350, 24, 4, 1.0f, { 6.0f, -6.5f, -14.0f }, at02_key_k22, 15 },
	{ 1351, 43, 0, 1.1f, { 4.5f, -4.8f, -13.99f }, 0, 31 },
	{ 1365, 25, 4, 1.0f, { -7.0f, -2.5f, -14.0f }, at02_key_k22, 16 },
	{ 1365, 26, 4, 1.0f, { 6.0f, -8.0f, -14.0f }, at02_key_k22, 17 },
	{ 1366, 43, 0, 1.1f, { -6.0f, -5.5f, -13.99f }, 0, 32 },
	{ 1366, 43, 0, 1.1f, { 6.0f, -6.5f, -13.99f }, 0, 34 },
	{ 1380, 27, 4, 1.0f, { -8.0f, 0.5f, -14.0f }, at02_key_k22, 18 },
	{ 1380, 28, 4, 1.0f, { 6.0f, -9.5f, -14.0f }, at02_key_k22, 19 },
	{ 1381, 43, 0, 1.1f, { -7.0f, -2.5f, -13.99f }, 0, 34 },
	{ 1381, 43, 0, 1.1f, { 6.0f, -8.0f, -13.99f }, 0, 33 },
	{ 1385, 29, 4, 1.0f, { -5.25f, 0.5f, -13.95f }, at02_key_k29b, 24 },
	{ 1385, 30, 4, 1.0f, { 3.53f, -9.5f, -13.93f }, at02_key_k30a, 25 },
	{ 1396, 43, 0, 1.1f, { -8.0f, 0.5f, -13.99f }, 0, 32 },
	{ 1396, 43, 0, 1.1f, { 6.0f, -9.5f, -13.99f }, 0, 34 },
	{ 1400, 29, 4, 1.0f, { -3.25f, -5.5f, -13.95f }, at02_key_k29c, 26 },
	{ 1400, 30, 4, 1.0f, { 3.53f, -8.0f, -13.92f }, at02_key_k30a, 27 },
	{ 1400, 30, 4, 1.0f, { 2.45f, -9.58f, -13.92f }, at02_key_k30c, 28 },
	{ 1415, 29, 4, 1.0f, { -4.25f, -2.5f, -13.95f }, at02_key_k29a, 29 },
	{ 1415, 30, 4, 1.0f, { 3.53f, -6.5f, -13.91f }, at02_key_k30b, 30 },
	{ 1425, 29, 4, 1.0f, { 1.75f, -4.8f, -13.95f }, at02_key_k29a, 31 },
	{ 1525, 40, 5, 0.0f, { 0.0f, 0.0f, -13.0f }, at02_key_k40, 32 },
	{ 1525, 39, 7, 0.0f, { 0.0f, 0.0f, -13.0f }, at02_key_k39, 33 },
	{ 1605, 45, 0, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 34 },
	{ 65535, 0, 0, 0.0f, { 0.0f, 0.0f, 0.0f }, 0, 0 },
} ;
Sint16 at02_ang[12] = { 38, 14, -14, -38, 175, 0, -120, 0, -90, -150, 0, -68 } ;

/*==============================================================*/
/*			The work's step to the next key of its list; at		*/
/*			the list's end it holds still						*/
/*==============================================================*/
static void at02_key_step( PLAYER_WORK *ew )
{
	VEC3 step, tmp ;
	Float *pf, *pt, *sf, *st ;
	Float at, af, rate ;
	Sint16 nf ;
	Sint16 dt ;
	AT02_LOCAL_W *lw ;

	if( ew->sub_no == 40 ){
		AT02_KEY_POS *k ;
		Sint16 key ;

		k = ew->work_func ;
		key = k[1].frame ;
		if( key == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		ew->timer0 = dt ;
		pt = k[1].v ;
		pf = k[0].v ;
		ew->work_func = &k[1] ;
		step = at02_vec_step( ew->timer0, pt, pf ) ;
		ew->pos.z = *pf ++ * 10.0f ;
		ew->scale.z = *pf ;
		ew->spd.z = step.x ;
		ew->acc.z = step.y ;
		return ;
	}
	if( ew->sub_no == 39 ){
		AT02_KEY_B *k ;
		Sint16 key ;

		k = ew->work_func ;
		key = k[1].frame ;
		if( key == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		ew->timer0 = dt ;
		at = k[1].a ;
		af = k[0].a ;
		ew->spd.z = ( at - af ) / ew->timer0 * 10.0f ;
		at = k[1].ang ;
		af = k[0].ang ;
		rate = ( at - af ) / ew->timer0 ;
		ew->work3 = ( int )( rate * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		pt = k[1].v ;
		pf = k[0].v ;
		step = at02_vec_step( ew->timer0, pt, pf ) ;
		ew->acc = step ;
		ew->work_func = &k[1] ;
		return ;
	}
	switch( ew->type ){
	case 1: {
		AT02_KEY_POS *k ;

		k = ew->work_func ;
		nf = k[1].frame ;
		if( nf == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		pt = k[1].v ;
		pf = k[0].v ;
		ew->work_func = &k[1] ;
		break ;
	}
	case 2: {
		AT02_KEY_ANG *k ;

		k = ew->work_func ;
		nf = k[1].frame ;
		if( nf == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		at = k[1].ang ;
		af = k[0].ang ;
		ew->work_func = &k[1] ;
		break ;
	}
	case 3: {
		AT02_KEY_POS *k ;

		k = ew->work_func ;
		nf = k[1].frame ;
		if( nf == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		at = k[1].ang ;
		af = k[0].ang ;
		pt = k[1].v ;
		pf = k[0].v ;
		ew->work_func = &k[1] ;
		break ;
	}
	case 4: {
		AT02_KEY_SCL *k ;

		k = ew->work_func ;
		nf = k[1].frame ;
		if( nf == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		st = k[1].v ;
		sf = k[0].v ;
		ew->work_func = &k[1] ;
		break ;
	}
	case 5: {
		AT02_KEY_POSSCL *k ;

		k = ew->work_func ;
		nf = k[1].frame ;
		if( nf == -1 ) goto end ;
		dt = k[1].frame - k[0].frame ;
		pt = k[1].v ;
		pf = k[0].v ;
		st = k[1].s ;
		sf = k[0].s ;
		ew->work_func = &k[1] ;
		break ;
	}
	case 6: {
		AT02_KEY_SCL *k ;

		k = ew->work_func ;
		nf = k[1].frame ;
		if( nf == -1 ) goto end ;
		if( nf == -3 ){
			AT02_KEY_SCL *n ;

			n = k - 2 ;
			dt = 60 ;
			at = k[-1].ang ;
			af = n->ang ;
			st = n->v ;
			sf = k->v ;
			ew->work_func = n ;
		} else {
			AT02_KEY_SCL *n ;

			if( nf == -2 ) k -= 2 ;
			n = k + 1 ;
			dt = n->frame - k->frame ;
			at = n->ang ;
			af = k->ang ;
			st = n->v ;
			sf = k->v ;
			ew->work_func = n ;
		}
		break ;
	}
	default:
		return ;
	}
	ew->timer0 = dt ;
	if( ew->type & 1 ){
		step = at02_vec_step( ew->timer0, pt, pf ) ;
		ew->pos.x = *pf ++ * 10.0f ;
		ew->pos.y = *pf ++ * 10.0f ;
		ew->pos.z = *pf * 10.0f ;
		ew->spd.x = step.x ;
		ew->spd.y = step.y ;
		ew->spd.z = step.z ;
	}
	if( ew->type & 2 ){
		rate = ( at - af ) / ew->timer0 ;
		if( ew->type == 3 ) ew->rot[1] = ( int )( af * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		else if( nf != -3 ) ew->rot[2] = ( int )( af * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		ew->work1 = 0 ;
		if( ew->type == 3 ){
			ew->work2 = ( int )( rate * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
			ew->work3 = 0 ;
		} else {
			ew->work2 = 0 ;
			ew->work3 = ( int )( rate * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
		}
	}
	if( ew->type & 4 ){
		tmp = at02_vec_step( ew->timer0, st, sf ) ;
		if( nf != -3 ){
			ew->scale.x = *sf ++ ;
			ew->scale.y = *sf ;
		}
		ew->acc.x = tmp.x ;
		ew->acc.y = tmp.y ;
		ew->acc.z = 0.0f ;
	}
	return ;
end:
	if( !( ew->type & 2 ) ){
		lw = AT02_LW( ew ) ;
		lw->turn_x = lw->turn_y = lw->turn_z = 0 ;
	}
	ew->spd.x = ew->spd.y = ew->spd.z = 0.0f ;
	ew->acc.x = ew->acc.y = ew->acc.z = 0.0f ;
	ew->timer0 = 240 ;
}

/* the scene's works, and what its camera and background hold */
static PLAYER_WORK *at02_w[40] ;
static PLAYER_WORK *at02_cam_w ;
static VEC3 at02_eye ;
static VEC3 at02_target ;
static MATRIX at02_view_mtx ;
static Sint32 at02_rec_no, at02_cam_no, at02_ang_no, at02_name_no ;
static Uint32 at02_back_col[2] ;

/* the key list's stepper, the work's set-up and the parts of the scene not yet written */

/****************************************************************/
/*			A work of the scene, ready to run a key list		*/
/*			from its start										*/
/****************************************************************/
void at02_w_set( int no, void *key )
{
	PLAYER_WORK *ew = at02_w[no] ;

	if( !ew ) return ;
	ew->disp_flag = 1 ;
	ew->work_func = key ;
	ew->timer0 = 1 ;
	ew->rot[0] = ew->rot[1] = ew->rot[2] = 0 ;
	ew->scale.x = ew->scale.y = ew->scale.z = 1.0f ;
	at02_key_step( ew ) ;
}

/*==============================================================*/
/*			A work's frame: its end after the demo's last		*/
/*			seconds, the moves of its kind, then the key		*/
/*			list												*/
/*==============================================================*/
static void at02_w_main( PLAYER_WORK *ew )
{
	int no = ew->sub_prio ;
	int lim = 1445 ;

	if( Sys->monitor_31k ) lim = 1463 ;
	if( Sys->timer1 >= lim ){
		if( no >= 20 && no <= 31 ){
			ew->scale.x -= 0.065f ;
			if( ew->scale.x <= 0.0f ) ew->be_flag = 0 ;
			return ;
		}
		if( ( no >= 9 && no <= 19 ) || no == 1 ){
			if( ew->sub_no != 32 ){
				if( ew->sub_no != 34 ){
					if( ew->sub_no != 37 ){
						ew->pos.z += 0.75f + ( Sys->timer1 - lim ) * 0.02f ;
						if( ew->pos.z < 0.0f ) return ;
						ew->be_flag = 0 ;
						return ;
					}
				}
			}
		}
		if( no == 4 || no == 3 ){
			if( !( ew->pos.z < 0.0f ) ){
				ew->be_flag = 0 ;
				return ;
			}
		}
	}
	switch( ew->sub_no ){
	case 10:
	case 11:
	case 12:
		if( -- ew->timer1 == 0 ) goto kill ;
		break ;
	case 13:
		if( Sys->timer1 == 455 ) ew->pl_color = 2 ;
		break ;
	case 14:
	case 16:
		if( Sys->timer1 == 493 ) goto kill ;
		if( Sys->timer1 > 473 ){
			at02_uv_main( ew ) ;
			return ;
		}
		break ;
	case 31:
		if( Sys->timer1 == 533 ){
			ew->fade.trnsl = ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
		}
		break ;
	case 32:
		if( Sys->timer1 == 1010 ){
			ew->fade.trnsl = ew->fade.r = ew->fade.g = ew->fade.b = 1.0f ;
		}
		break ;
	case 47:
		if( Sys->timer1 >= 948 ){
			switch( Sys->timer1 & 3 ){
			case 0:
				ew->pos.x += 0.2f ;
				break ;
			case 1:
				ew->pos.x -= 0.2f ;
				break ;
			case 2:
				ew->pos.x -= 0.2f ;
				break ;
			case 3:
				ew->pos.x += 0.2f ;
				break ;
			}
		}
		break ;
	case 52:
		ew->rot[2] = ew->rot[2] + 182 ;
		at02_uv_main( ew ) ;
		break ;
	case 54:
		ew->rot[2] = ew->rot[2] + 910 ;
		break ;
	case 55:
	case 56:
	case 57:
	case 58:
	case 59:
		if( Sys->timer1 >= 1060 ) ew->rot[1] = nlArcTan2( at02_eye.z - ew->pos.z, at02_eye.x - ew->pos.x ) & 0xffff ;
		else ew->rot[1] = nlArcTan2( at02_eye.x - ew->pos.x, at02_eye.z - ew->pos.z ) + 0x8000 ;
		return ;
	}
	if( ew->pl_color == 1 ){
		ew->fade.r += 0.05f ;
		if( !( 1.0f > ew->fade.r ) ){
			ew->fade.r = 1.0f ;
			ew->pl_color = 0 ;
		}
		ew->fade.g = ew->fade.b = ew->fade.r ;
	}
	if( ew->pl_color == 2 ){
		ew->fade.r -= 0.033333335f ;
		if( !( ew->fade.r > 0.0f ) ) goto kill ;
		ew->fade.g = ew->fade.b = ew->fade.r ;
	}
	if( ew->type & 1 ){
		ew->pos.x += ew->spd.x * 10.0f ;
		ew->pos.y += ew->spd.y * 10.0f ;
		ew->pos.z += ew->spd.z * 10.0f ;
	}
	if( ew->type & 2 ){
		ew->rot[0] += ew->work1 ;
		ew->rot[1] += ew->work2 ;
		ew->rot[2] += ew->work3 ;
	}
	if( ew->type & 4 ){
		ew->scale.x += ew->acc.x ;
		ew->scale.y += ew->acc.y ;
		ew->scale.z += ew->acc.z ;
	}
	if( ew->type ){
		if( -- ew->timer0 <= 0 ) at02_key_step( ew ) ;
	}
	return ;
kill:
	ew->disp_flag = ew->be_flag = 0 ;
}

/****************************************************************/
/*			A work turned into the kind no + 55 of the			*/
/*			scene's second half, with its place by no + 13		*/
/****************************************************************/
void at02_w_kind_set( PLAYER_WORK *ew, int no )
{
	ew->sub_no = no + 55 ;
	ew->obj = obj_dm08->obj[ew->sub_no] ;
	ew->type = 0 ;
	ew->fade.trnsl = 0.0f ;
	ew->obj_flag = 0 ;
	ew->sub_prio = no + 13 ;
	ew->rot[0] = 0 ;
	ew->rot[2] = 0 ;
}

/* the name tiles' places and the order the characters' names come in */
extern void name_pic_set( int side, Uint32 no ) ;
extern void _quick_odd_mvn( void ) ;

/*==============================================================*/
/*			The names of the characters opened, one tile		*/
/*			each, set along the sides while the camera			*/
/*			passes												*/
/*==============================================================*/
static void at02_name_disp( void )
{
	PLAYER_WORK *ew ;
	Uint32 bits[2] ;
	VEC3 pos ;
	struct { Float x, y ; } scr ;
	Float add ;
	Uint8 no ;
	int i, on ;
	Float *e ;

	nlPushMatrix( 0 ) ;
	nlLoadMatrix( &at02_view_mtx ) ;
	bits[0] = Sys->char_open0 ;
	bits[1] = Sys->char_open1 ;
	if( at02_name_no < 28 ){
		i = at02_name_no % 5 ;
		if( Sys->timer1 >= 1060 ) no = at02_name_ord0[at02_name_no] ;
		else no = at02_name_ord1[at02_name_no] ;
		e = &at02_name_pos[no * 4] ;
		pos.x = e[0] * 10.0f ;
		pos.y = e[1] * 10.0f ;
		pos.z = e[2] * 10.0f ;
		if( Sys->timer1 >= 1060 ) add = -5.0f ;
		else if( at02_name_no == 13 || at02_name_no == 27 ) add = 12.0f ;
		else add = 6.0f ;
		pos.x += add ;
		nlProjectScreen( &pos, &scr.x ) ;
		if( Sys->timer1 >= 1060 && !( scr.x > 640.0f ) || Sys->timer1 < 1060 && !( scr.x < 0.0f ) ){
			ew = at02_w[13 + i] ;
			tex_req_flag = 1 ;
			tex_req_no = 6 + i ;
			name_pic_set( i, no ) ;
			ew->be_flag = 1 ;
			ew->disp_flag = 1 ;
			ew->pos.x = e[0] * 10.0f ;
			ew->pos.y = e[1] * 10.0f ;
			ew->pos.z = e[2] * 10.0f ;
			ew->scale.z = e[3] ;
			ew->scale.y = ew->scale.z ;
			ew->scale.x = ew->scale.y ;
			if( Sys->timer1 >= 1060 ) ew->rot[1] = 4338 ;
			else ew->rot[1] = 28343 ;
			if( no < 32 ){
				if( bits[0] & ( 1 << no ) ) on = 1 ;
				else on = 0 ;
			} else {
				if( bits[1] & ( 1 << ( no - 32 ) ) ) on = 1 ;
				else on = 0 ;
			}
			ew->fade.trnsl = ew->fade.r = ew->fade.g = ew->fade.b = on * 1.0f ;
			ew->pl_color = 0 ;
			ew->timer0 = 0 ;
			at02_name_no ++ ;
		}
	} else {
#ifdef HOST_PORT
		static PLAYER_WORK stack_w ;	/* ew is not set here: on the board it is the stack */

		ew = &stack_w ;
#endif
		pos = ew->pos ;
		if( Sys->timer1 >= 1060 ) pos.x += 20.0f ;
		else pos.x -= 20.0f ;
		nlProjectScreen( &pos, &scr.x ) ;
		if( Sys->timer1 >= 1060 && scr.x < 0.0f || Sys->timer1 < 1060 && scr.x > 640.0f ){
			ew->disp_flag = 0 ;
			ew->be_flag = 0 ;
		}
	}
	nlPopMatrix( 1 ) ;
}

/****************************************************************/
/*			The work of the scene's record no, made ready to	*/
/*			run its key list; 0 if its place has none			*/
/****************************************************************/
PLAYER_WORK *at02_rec_set( int no )
{
	AT02_REC *rec = &at02_rec[no] ;
	PLAYER_WORK *ew ;
	AT02_LOCAL_W *lw ;

	if( !at02_w[rec->slot] ) return 0 ;
	ew = at02_w[rec->slot] ;
	ew->be_flag = 1 ;
	ew->disp_flag = 1 ;
	ew->work_func = rec->key ;
	ew->sub_no = rec->kind ;
	if( Sys->cpu_level_type ){
		if( ew->sub_no == 20 ) ew->sub_no = 44 ;
		if( ew->sub_no == 22 ) ew->sub_no = 41 ;
		if( ew->sub_no == 25 ) ew->sub_no = 42 ;
	}
	ew->obj = obj_dm08->obj[ew->sub_no] ;
	ew->type = rec->type ;
	ew->fade.trnsl = 0.0f ;
	ew->obj_flag = 0 ;
	ew->sub_prio = rec->slot ;
	ew->pos.x = rec->pos.x * 10.0f ;
	ew->pos.y = rec->pos.y * 10.0f ;
	ew->pos.z = rec->pos.z * 10.0f ;
	ew->rot[0] = ew->rot[1] = ew->rot[2] = 0 ;
	ew->scale.x = ew->scale.y = ew->scale.z = 1.0f ;
	ew->fade.trnsl = ew->fade.r = ew->fade.g = ew->fade.b = rec->alpha ;
	if( ew->fade.trnsl == 1.0f ) ew->pl_color = 0 ;
	else ew->pl_color = 1 ;
	if( ew->sub_no == 31 || ew->sub_no == 32 ) ew->pl_color = 0 ;
	if( ew->sub_no == 43 ){
		if( ew->sub_prio == 34 || ew->sub_prio == 33 ) ew->scale.x = ew->scale.y = 0.88f ;
		ew->pl_color = 2 ;
	}
	if( ew->sub_no == 29 || ew->sub_no == 30 ){
		ew->rot[2] = ( int )( at02_ang[at02_ang_no ++] * 65536.0f / 360.0f + 0.5f ) & 0xffff ;
	}
	lw = AT02_LW( ew ) ;
	lw->turn_x = lw->turn_y = lw->turn_z = 0 ;
	ew->spd.x = ew->spd.y = ew->spd.z = 0.0f ;
	ew->acc.x = ew->acc.y = ew->acc.z = 0.0f ;
	at02_key_step( ew ) ;
	ew->timer1 = 240 ;
	return ew ;
}

/* the key lists the scene's works and its camera run through */
extern void fade_set( int a, int b, int c ) ;
extern void ex_se_req( Uint8 no ) ;
extern void stream_req( Sint8 no ) ;
extern void stream_start( void ) ;

/*==============================================================*/
/*			The frames of the demo: the back colour, the		*/
/*			fades, the streams and the works' key lists;		*/
/*			then the records whose frame has come				*/
/*==============================================================*/
static void at02_script( void )
{
	PLAYER_WORK *ew ;
	Sint32 f ;
	int i ;

	switch( Sys->timer1 ){
	case 0:
		at02_back_col[0] = 0xff000000 ;
		at02_back_col[1] = 0xff000000 ;
		fade_set( 0, 30, FADE_IN ) ;
		ex_se_req( 0 ) ;
		at02_rec_no = 0 ;
		at02_ang_no = 0 ;
		break ;
	case 60:
		fade_set( 0, 60, FADE_OUT ) ;
		break ;
	case 120:
		stream_req( 11 ) ;
		stream_start() ;
		fade_set( 0, 120, FADE_IN ) ;
		break ;
	case 493:
		at02_back_col[0] = 0xff0000ff ;
		at02_back_col[1] = 0xff000000 ;
		tex_req_flag = 1 ;
		tex_req_no = 4 ;
		ew = at02_w[5] ;
		ew->disp_flag = 0 ;
		AT02_NEXT_W( ew ) ;
		ew->disp_flag = 0 ;
		AT02_NEXT_W( ew ) ;
		ew->work_func = &at02_key_k08[6] ;
		ew->timer0 = 1 ;
		AT02_NEXT_W( ew ) ;
		ew->work_func = &at02_key_k09[7] ;
		ew->timer0 = 1 ;
		break ;
	case 573:
		at02_back_col[0] = -1 ;
		at02_back_col[1] = -1 ;
		if( 0 ) at02_name_no = -1 ;
		fog_work.on = 1 ;
		at02_name_no = 0 ;
		for( i = 0 ; i < 9 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 0 ;
		}
		for( ; i < 13 ; i ++ ){
			ew = at02_w[i] ;
			ew->be_flag = 0 ;
			ew->disp_flag = 0 ;
		}
		for( ; i < 18 ; i ++ ){
			ew = at02_w[i] ;
			at02_w_kind_set( ew, i - 13 ) ;
		}
		break ;
	case 848:
		at02_back_col[0] = 0xff000000 ;
		at02_back_col[1] = 0xff000000 ;
		fog_work.on = 0 ;
		for( i = 13 ; i < 18 ; i ++ ){
			ew = at02_w[i] ;
			ew->be_flag = 0 ;
			ew->disp_flag = 0 ;
		}
		for( i = 35 ; i < 38 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 0 ;
		}
		at02_ang_no = 0 ;
		at02_w_set( 5, &at02_key_k06[6] ) ;
		at02_w_set( 6, &at02_key_k07[7] ) ;
		at02_w_set( 7, &at02_key_k08[10] ) ;
		at02_w_set( 8, &at02_key_k09[11] ) ;
		for( i = 0 ; i < 5 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 1 ;
			if( i == 1 ){
				ew->obj = obj_dm08->obj[2] ;
				ew->work_func = at02_key_k01b ;
				at02_key_step( ew ) ;
			}
		}
		for( i = 9 ; i < 18 ; i ++ ){
			ew = at02_w[i] ;
			ew->be_flag = 0 ;
			ew->disp_flag = 0 ;
		}
		break ;
	case 980:
		at02_back_col[0] = 0xffff0000 ;
		at02_back_col[1] = 0xff000000 ;
		tex_req_flag = 1 ;
		tex_req_no = 5 ;
		ew = at02_w[5] ;
		ew->work_func = &at02_key_k08[6] ;
		ew->timer0 = 1 ;
		AT02_NEXT_W( ew ) ;
		ew->work_func = &at02_key_k09[7] ;
		ew->timer0 = 1 ;
		AT02_NEXT_W( ew ) ;
		ew->disp_flag = 0 ;
		AT02_NEXT_W( ew ) ;
		ew->disp_flag = 0 ;
		for( i = 9 ; i < 13 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 0 ;
		}
		ew = at02_w[17] ;
		ew->be_flag = 0 ;
		ew->disp_flag = 0 ;
		for( i = 20 ; i < 24 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 0 ;
		}
		break ;
	case 1060:
		at02_back_col[0] = -1 ;
		at02_back_col[1] = -1 ;
		if( 0 ) at02_rec_no = -1 ;
		fog_work.on = 1 ;
		at02_name_no = 0 ;
		for( i = 0 ; i < 9 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 0 ;
		}
		for( i = 13 ; i < 18 ; i ++ ){
			ew = at02_w[i] ;
			ew->be_flag = 0 ;
			ew->disp_flag = 0 ;
			at02_w_kind_set( ew, i - 13 ) ;
		}
		for( i = 35 ; i < 38 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 1 ;
		}
		break ;
	case 1335:
		at02_back_col[0] = 0xff000000 ;
		at02_back_col[1] = 0xff000000 ;
		fog_work.on = 0 ;
		at02_w_set( 5, &at02_key_k06[6] ) ;
		at02_w_set( 6, &at02_key_k07[7] ) ;
		at02_w_set( 7, &at02_key_k08[10] ) ;
		at02_w_set( 8, &at02_key_k09[11] ) ;
		for( i = 0 ; i < 13 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 1 ;
		}
		for( ; i < 18 ; i ++ ){
			ew = at02_w[i] ;
			ew->be_flag = 0 ;
			ew->disp_flag = 0 ;
		}
		for( i = 20 ; i < 24 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 1 ;
		}
		for( i = 35 ; i < 38 ; i ++ ){
			ew = at02_w[i] ;
			ew->disp_flag = 0 ;
			ew->be_flag = 0 ;
		}
		break ;
	case 1525:
		if( Sys->monitor_31k ) break ;
		ew = at02_w[3] ;
		ew->spd.z = 0.25f ;
		ew->type = 1 ;
		break ;
	case 1535:
		if( Sys->monitor_31k ) break ;
		ew = at02_w[4] ;
		ew->spd.z = 0.25f ;
		ew->type = 1 ;
		ew = at02_w[0] ;
		ew->pl_color = 2 ;
		ew = at02_w[2] ;
		ew->pl_color = 2 ;
		break ;
	case 1695:
		if( Sys->monitor_31k ) break ;
		fade_set( -1, 5, FADE_OUT ) ;
		break ;
	case 1543:
		if( !Sys->monitor_31k ) break ;
		ew = at02_w[3] ;
		ew->spd.z = 0.25f ;
		ew->type = 1 ;
		break ;
	case 1553:
		if( !Sys->monitor_31k ) break ;
		ew = at02_w[4] ;
		ew->spd.z = 0.25f ;
		ew->type = 1 ;
		ew = at02_w[0] ;
		ew->pl_color = 2 ;
		ew = at02_w[2] ;
		ew->pl_color = 2 ;
		break ;
	case 1713:
		if( !Sys->monitor_31k ) break ;
		fade_set( -1, 5, FADE_OUT ) ;
		break ;
	}
	if( 0 ){
		at02_rec_no = 28 ;
		at02_name_no = 80 ;
	}
	do {
		f = at02_rec[at02_rec_no].frame ;
		if( Sys->timer1 >= 1445 && Sys->monitor_31k ) f += 18 ;
		if( f == Sys->timer1 ){
			ew = at02_rec_set( at02_rec_no ) ;
			if( at02_rec_no >= 9 && at02_rec_no <= 11 ) ew->timer1 = 80 ;
			if( at02_rec_no == 12 ) ew->timer1 = 90 ;
			at02_rec_no ++ ;
		}
	} while( f == Sys->timer1 ) ;
}

/*==============================================================*/
/*			Draws a work's model								*/
/*==============================================================*/
static void at02_obj_disp( PLAYER_WORK *ew )
{
	Float unused[22] ;

	if( !ew->disp_flag ) return ;
	nlPushMatrix( 0 ) ;
	nlObjPutSetFadeColor( ew->fade.r, ew->fade.g, ew->fade.b ) ;
	nlTranslateV( &ew->pos ) ;
	nlRotateAZ( ew->rot[2] ) ;
	nlRotateAY( ew->rot[1] ) ;
	nlScaleV( &ew->scale ) ;
	nlSetScaleFactor( obj_scale_max( &ew->scale ) ) ;
	nlObjPutUseViewMatrix( ew->obj ) ;
	nlPopMatrix( 1 ) ;
}

/*==============================================================*/
/*			The camera: its projection at the frames it			*/
/*			changes, then its eye and target moving along		*/
/*			the key list										*/
/*==============================================================*/
static void at02_start( void )
{
	PLAYER_WORK *cam_w = at02_cam_w ;
	AT02_CAM_KEY *key, *list ;
	VEC3 step ;
	Sint16 dt ;
	int no ;

	switch( Sys->timer1 ){
	case 0:
		nlMatrixMode( 3 ) ;
		nlUnitMatrix() ;
		nlPerspective( 0x4000, 1.3333334f, 0.1f, 12000.0f ) ;
		nlMatrixMode( 1 ) ;
		at02_eye.x = 0.0f ;
		at02_eye.y = 0.0f ;
		at02_eye.z = 0.0f ;
		at02_target.x = 0.0f ;
		at02_target.y = 0.0f ;
		at02_target.z = -150.0f ;
		cam_w->spd.x = cam_w->spd.y = cam_w->spd.z = 0.0f ;
		cam_w->scale.x = cam_w->scale.y = cam_w->scale.z = 0.0f ;
		cam_w->timer0 = 300 ;
		cam_w->obj_flag = 0 ;
		at02_cam_no = 0 ;
		return ;
	case 848:
		nlMatrixMode( 3 ) ;
		nlUnitMatrix() ;
		nlPerspective( 0x4000, 1.3333334f, 0.1f, 12000.0f ) ;
		nlMatrixMode( 1 ) ;
		cam_w->obj_flag = 0 ;
		cam_w->timer0 = 1 ;
		cam_w->work_func = at02_cam_keys[at02_cam_no ++] ;
		break ;
	case 573:
	case 1060:
		nlMatrixMode( 3 ) ;
		nlUnitMatrix() ;
		nlPerspective( 0x975, 1.3333334f, 0.1f, 12000.0f ) ;
		nlMatrixMode( 1 ) ;
	case 120:
	case 493:
	case 980:
	case 1445:
		cam_w->obj_flag = 0 ;
		cam_w->timer0 = 1 ;
		cam_w->work_func = at02_cam_keys[at02_cam_no ++] ;
		break ;
	case 1335:
		nlMatrixMode( 3 ) ;
		nlUnitMatrix() ;
		nlPerspective( 0x4000, 1.3333334f, 0.1f, 12000.0f ) ;
		nlMatrixMode( 1 ) ;
		at02_eye.x = 0.0f ;
		at02_eye.y = -30.0f ;
		at02_eye.z = -50.0f ;
		at02_target.x = 0.0f ;
		at02_target.y = -30.0f ;
		at02_target.z = -150.0f ;
		cam_w->spd.x = cam_w->spd.y = cam_w->spd.z = 0.0f ;
		cam_w->scale.x = cam_w->scale.y = cam_w->scale.z = 0.0f ;
		cam_w->timer0 = 300 ;
		return ;
	}
	if( -- cam_w->timer0 == 0 ){
		no = cam_w->obj_flag ;
		list = cam_w->work_func ;
		key = list + no ;
		if( list[no + 1].frame != -1 ){
			cam_w->timer0 = key[1].frame - key->frame ;
			dt = cam_w->timer0 ;
			step = at02_vec_step( dt, key[1].eye, key->eye ) ;
			cam_w->spd = step ;
			step = at02_vec_step( cam_w->timer0, key[1].target, key->target ) ;
			cam_w->scale = step ;
			if( no == 0 ){
				at02_eye.x = key->eye[0] * 10.0f ;
				at02_eye.y = key->eye[1] * 10.0f ;
				at02_eye.z = key->eye[2] * 10.0f ;
				at02_target.x = key->target[0] * 10.0f ;
				at02_target.y = key->target[1] * 10.0f ;
				at02_target.z = key->target[2] * 10.0f ;
				cam_w->obj_flag ++ ;
				return ;
			}
			cam_w->obj_flag ++ ;
		} else {
			cam_w->spd.x = cam_w->spd.y = cam_w->spd.z = 0.0f ;
			cam_w->scale.x = cam_w->scale.y = cam_w->scale.z = 0.0f ;
			cam_w->timer0 = 240 ;
		}
	}
	at02_eye.x += cam_w->spd.x * 10.0f ;
	at02_eye.y += cam_w->spd.y * 10.0f ;
	at02_eye.z += cam_w->spd.z * 10.0f ;
	at02_target.x += cam_w->scale.x * 10.0f ;
	at02_target.y += cam_w->scale.y * 10.0f ;
	at02_target.z += cam_w->scale.z * 10.0f ;
}

/*==============================================================*/
/*			The scene's set-up									*/
/*==============================================================*/
static void at02_init( void )
{
	PLAYER_WORK **w ;

	work_init() ;
	Palette_Init() ;
	Texture_Init() ;
	bright_init() ;
	fog_init() ;
	fog_work.on = 0 ;
	fog_work.len = 15 ;
	fog_work.density = 840.0f ;
	fog_work.b = fog_work.g = fog_work.r = 0xff ;
	at02_cam_w = work_get( 0, W_SYS, LIST_END ) ;
	w = at02_w ;
	do {
		*w = work_get( 0, W_SYS, LIST_END ) ;
		w ++ ;
	} while( w < &at02_w[40] ) ;
	ex_sound_load2() ;
	total_vol_init() ;
	TexLoad( 12 ) ;
	Sys->timer1 = 0 ;
	at02_start() ;
	at02_script() ;
	Sys->scene_no ++ ;
}

/*==============================================================*/
/*			Each frame: steps the works, sets up the camera		*/
/*			and draws them; the demo ends after 1700 frames		*/
/*==============================================================*/
static void at02_main( void )
{
	PLAYER_WORK *cam_w, *ew ;
	int i, end ;

	back_color_set2( at02_back_col[0], at02_back_col[1] ) ;
	Sys->timer1 ++ ;
	at02_start() ;
	at02_script() ;
	cam_w = at02_cam_w ;
	nlMatrixMode( 2 ) ;
	nlUnitMatrix() ;
	nlLookAt( &at02_eye, &at02_target, 0 ) ;
	nlGetMatrix( &cam_w->obj_mtx ) ;
	nlMatrixMode( 1 ) ;
	nlUnitMatrix() ;
	nlPushMatrix( 0 ) ;
	nlMakeModelViewProjectMatrix( 640.0f, 480.0f ) ;
	nlGetMatrix( &at02_view_mtx ) ;
	nlPopMatrix( 1 ) ;
	if( ( Sys->timer1 >= 573 && Sys->timer1 < 848 ) || ( Sys->timer1 >= 1060 && Sys->timer1 < 1335 ) ){
		at02_name_disp() ;
	}
	for( i = 0 ; i < 40 ; i ++ ){
		if( ( ew = at02_w[i] ) && ew->be_flag ) at02_w_main( ew ) ;
	}
	for( i = 0 ; i < 40 ; i ++ ){
		if( ( ew = at02_w[i] ) && ew->be_flag ) at02_obj_disp( ew ) ;
	}
	end = 1700 ;
	if( Sys->monitor_31k ) end = 1718 ;
	if( Sys->timer1 > end ){
		Sys->scene_no = 0 ;
		Sys->task_step ++ ;
	}
	fog_set() ;
}

/****************************************************************/
/*			The third attract demo, called by attract_02		*/
/****************************************************************/
void attract_opening_main( void )
{
	if( !Sys->scene_no ) at02_init() ;
	at02_main() ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "at02_scene.c" End of File	*/
