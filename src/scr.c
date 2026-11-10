/* The camera: scr_work's eye and the point it looks at, the projection, the fight camera's modes
   across and up and down, the paths of set keys, the screen quake and the screen's edges. */
#include "player.h"

/* degrees to an angle */
#define DEG_ANG( n )	( ( Sint32 )( ( n ) * 65536.0f / 360.0f + 0.5f ) & 0xffff )

void scr_work_init( void ) ;
void scr_matrix_set( void ) ;
void scr_far_matrix_set( void ) ;
void scr_2d_matrix_set( void ) ;
void scr_matrix_set3( void ) ;
void scr_normal( void ) ;
void scr_demo( void ) ;
void scr_demo_move( Uint8 no ) ;
void scr_demo_set( Uint8 no ) ;
void scr_move( void ) ;
void scr_eye_move( int no, int time ) ;
void scr_target_move( int no, int time ) ;
void scr_target_turn( int no, int time ) ;
void scr_eye_turn( int no, int time ) ;
void scr_x_normal( int spd ) ;
void scr_y_normal( int spd ) ;
void scr_x_stop( int spd ) ;
void scr_y_stop( int spd ) ;
void scr_x_caught( int spd ) ;
void scr_y_caught( int spd ) ;
void scr_x_catch( int spd ) ;
void scr_y_catch( int spd ) ;
void scr_x_pl1( int spd ) ;
void scr_y_pl1( int spd ) ;
void scr_x_pl2( int spd ) ;
void scr_y_pl2( int spd ) ;
void scr_y_base( int spd ) ;
void scr_y_low( int spd ) ;
void scr_x_dm( int spd ) ;
void scr_y_dm( int spd ) ;
void scr_x_hold( int spd ) ;
void scr_y_stop_pl( int spd ) ;
void scr_x_abyss( int spd ) ;
void scr_y_abyss( int spd ) ;
void scr_x_reset( int spd ) ;
void scr_y_reset( int spd ) ;
void scr_quake( Uint8 lv ) ;
void scr_quake_0( Uint8 lv ) ;
void scr_quake_1( Uint8 lv ) ;
void scr_eye_x_set( int no, int spd ) ;
void scr_eye_yz_set( int no, int spd ) ;
void scr_target_x_set( int no, int spd ) ;
void scr_target_yz_set( int no, int spd ) ;
Uint8 scr_excep_ck( void ) ;
void scr_excep_side_set( PLAYER_WORK *pl_w ) ;
void scr_mode_move( void ) ;
void scr_x_limit( VEC3 *eye, VEC3 *obj ) ;
void scr_y_limit( VEC3 *eye, VEC3 *obj ) ;
void scr_edge_set( void ) ;

/* path 0: its keys, then the frames to each */
static Sint32 path0_time[] = {
	39,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 1000
} ;
/* the eye and the point looked at at each key */
static Float path0_pos[] = {
	8.99f, 15.39f, 86.36f,   0.0f, 18.6f, 0.0f,
	8.99f, 15.37f, 83.94f,   0.0f, 18.6f, 0.0f,
	9.0f, 15.36f, 81.7f,   0.0f, 18.6f, 0.0f,
	9.01f, 15.35f, 79.55f,   0.0f, 18.6f, 0.0f,
	9.03f, 15.34f, 77.4f,   0.0f, 18.6f, 0.0f,
	9.49f, 15.33f, 75.13f,   0.0f, 18.6f, 0.0f,
	9.06f, 15.32f, 72.64f,   0.0f, 18.6f, 0.0f,
	9.08f, 15.31f, 69.85f,   0.0f, 18.6f, 0.0f,
	9.11f, 15.29f, 66.65f,   0.0f, 18.6f, 0.0f,
	9.13f, 15.28f, 63.02f,   0.0f, 18.6f, 0.0f,
	9.16f, 15.26f, 59.04f,   0.0f, 18.6f, 0.0f,
	9.19f, 15.25f, 54.79f,   0.0f, 18.6f, 0.0f,
	9.21f, 15.23f, 50.33f,   0.0f, 18.6f, 0.0f,
	9.23f, 15.21f, 45.72f,   0.0f, 18.6f, 0.0f,
	9.23f, 15.19f, 41.03f,   0.0f, 18.6f, 0.0f,
	9.21f, 15.17f, 36.33f,   0.0f, 18.6f, 0.0f,
	9.15f, 15.14f, 31.68f,   0.0f, 18.6f, 0.0f,
	8.91f, 15.11f, 27.16f,   0.0f, 18.6f, 0.0f,
	8.32f, 15.09f, 31.51f,   0.0f, 18.6f, 0.0f,
	7.99f, 15.09f, 36.2f,   0.0f, 18.6f, 0.0f,
	7.67f, 15.09f, 40.95f,   0.0f, 18.6f, 0.0f,
	7.38f, 15.09f, 45.76f,   0.0f, 18.6f, 0.0f,
	7.08f, 15.1f, 50.52f,   0.0f, 18.6f, 0.0f,
	6.8f, 15.1f, 55.21f,   0.0f, 18.6f, 0.0f,
	6.51f, 15.1f, 59.76f,   0.0f, 18.6f, 0.0f,
	6.24f, 15.1f, 64.11f,   0.0f, 18.6f, 0.0f,
	5.96f, 15.1f, 68.21f,   0.0f, 18.6f, 0.0f,
	5.7f, 15.1f, 72.01f,   0.0f, 18.6f, 0.0f,
	5.4f, 15.1f, 75.7f,   0.0f, 18.6f, 0.0f,
	5.08f, 15.09f, 79.47f,   0.0f, 18.6f, 0.0f,
	4.64f, 15.07f, 83.2f,   0.0f, 18.6f, 0.0f,
	3.93f, 15.03f, 86.75f,   0.0f, 18.6f, 0.0f,
	2.4f, 14.89f, 89.64f,   0.0f, 18.6f, 0.0f,
	0.89f, 14.73f, 87.3f,   0.0f, 18.6f, 0.0f,
	0.4f, 14.66f, 84.81f,   0.0f, 18.6f, 0.0f,
	0.16f, 14.63f, 82.88f,   0.0f, 18.6f, 0.0f,
	0.03f, 14.6f, 81.64f,   0.0f, 18.6f, 0.0f,
	0.0f, 14.6f, 81.2f,   0.0f, 18.6f, 0.0f,
	0.0f, 14.6f, 81.2f,   0.0f, 18.6f, 0.0f
} ;
/* the turns at each key: the eye's, then the point's */
static Float path0_turn[] = {
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f
} ;

/* path 1: its keys, then the frames to each */
static Sint32 path1_time[] = {
	7,
	100, 15, 30, 15, 12, 15, 18
} ;
/* the eye and the point looked at at each key */
static Float path1_pos[] = {
	0.0f, 1080.0f, 0.1f,   0.0f, 190.0f, 0.0f,
	0.0f, 1080.0f, 0.1f,   0.0f, 190.0f, 0.0f,
	0.0f, 530.0f, 0.1f,   0.0f, 190.0f, 0.0f,
	0.0f, 484.45f, 170.1f,   0.0f, 190.0f, 0.0f,
	0.0f, 360.0f, 294.54f,   0.0f, 190.0f, 0.0f,
	0.0f, 190.0f, 340.1f,   0.0f, 190.0f, 0.0f,
	0.0f, 95.0f, 812.3573f,   0.0f, 190.0f, 0.0f,
	0.0f, 95.0f, 812.3573f,   0.0f, 190.0f, 0.0f
} ;
/* the turns at each key: the eye's, then the point's */
static Float path1_turn[] = {
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f
} ;

/* path 2: its keys, then the frames to each */
static Sint32 path2_time[] = {
	1,
	30
} ;
/* the eye and the point looked at at each key */
static Float path2_pos[] = {
	0.0f, 190.0f, 90.0f,   0.0f, 190.0f, 0.0f
} ;
/* the turns at each key: the eye's, then the point's */
static Float path2_turn[] = {
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f
} ;

/* path 3: its keys, then the frames to each */
static Sint32 path3_time[] = {
	26,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
	3000
} ;
/* the eye and the point looked at at each key */
static Float path3_pos[] = {
	-7.11f, -4.59f, 9.44f,   -7.16f, -4.45f, 8.69f,
	-8.86f, -5.79f, 11.49f,   -8.93f, -5.67f, 10.77f,
	-10.67f, -7.06f, 13.61f,   -10.78f, -6.97f, 12.91f,
	-12.48f, -8.36f, 15.72f,   -12.63f, -8.29f, 15.04f,
	-14.19f, -9.65f, 17.76f,   -14.4f, -9.6f, 17.09f,
	-15.81f, -10.9f, 19.67f,   -16.09f, -10.86f, 18.99f,
	-17.44f, -12.2f, 21.55f,   -17.8f, -12.16f, 20.86f,
	-19.07f, -13.51f, 23.42f,   -19.51f, -13.47f, 22.72f,
	-20.68f, -14.82f, 25.28f,   -21.21f, -14.79f, 24.57f,
	-22.24f, -16.13f, 27.15f,   -22.85f, -16.09f, 26.43f,
	-23.72f, -17.4f, 29.03f,   -24.42f, -17.37f, 28.3f,
	-25.08f, -18.63f, 30.95f,   -25.87f, -18.59f, 30.2f,
	-26.3f, -19.83f, 32.96f,   -27.18f, -19.79f, 32.22f,
	-27.36f, -21.03f, 35.1f,   -28.34f, -20.99f, 34.35f,
	-28.16f, -22.16f, 37.35f,   -29.25f, -22.11f, 36.61f,
	-28.72f, -23.12f, 39.64f,   -29.88f, -23.07f, 38.93f,
	-29.16f, -23.91f, 41.88f,   -30.33f, -23.88f, 41.2f,
	-29.46f, -24.55f, 43.98f,   -30.62f, -24.53f, 43.34f,
	-29.66f, -25.08f, 45.89f,   -30.8f, -25.07f, 45.28f,
	-29.8f, -25.56f, 47.77f,   -30.9f, -25.55f, 47.19f,
	-29.89f, -25.99f, 49.6f,   -30.95f, -25.99f, 49.05f,
	-29.93f, -26.34f, 51.27f,   -30.93f, -26.34f, 50.74f,
	-29.91f, -26.59f, 52.66f,   -30.86f, -26.6f, 52.15f,
	-29.85f, -26.74f, 53.66f,   -30.75f, -26.75f, 53.17f,
	-29.8f, -26.8f, 54.16f,   -30.67f, -26.81f, 53.67f,
	-29.79f, -26.8f, 54.19f,   -30.67f, -26.81f, 53.71f,
	-29.79f, -26.8f, 54.19f,   -30.67f, -26.81f, 53.71f
} ;
/* the turns at each key: the eye's, then the point's */
static Float path3_turn[] = {
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -3.78f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -4.65f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -5.55f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -6.46f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -7.35f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -8.22f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -9.09f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -9.98f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -10.88f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -11.76f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -12.64f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -13.49f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -14.34f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -15.2f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -16.05f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -16.87f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -17.65f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -18.37f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -19.01f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -19.64f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -20.25f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -20.8f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -21.26f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -21.59f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -21.76f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -21.77f,
	0.0f, 0.0f, 0.0f,   0.0f, 0.0f, -21.77f
} ;

/* path 4: its keys, then the frames to each */
static Sint32 path4_time[] = {
	1,
	60
} ;
/* the eye and the point looked at at each key */
static Float path4_pos[] = {
	0.0f, 95.0f, 812.3573f,   0.0f, 190.0f, 0.0f
} ;

static Sint32 *path_time_tbl[5] = { path0_time, path1_time, path2_time, path3_time, path4_time } ;
static Float *path_pos_tbl[5] = { path0_pos, path1_pos, path2_pos, path3_pos, path4_pos } ;
static Float *path_turn_tbl[5] = { path0_turn, path1_turn, path2_turn, path3_turn, path1_turn } ;

/* the fight camera's modes across, then up and down, run with the frames to follow over */
static void ( *scr_x_tbl[13] )( int spd ) = {
	scr_x_normal, scr_x_stop, scr_x_stop, scr_x_pl1, scr_x_pl2, scr_x_catch, scr_x_caught,
	scr_x_normal, scr_x_dm, scr_x_abyss, scr_x_hold, scr_x_reset, scr_x_normal
} ;
static void ( *scr_y_tbl[13] )( int spd ) = {
	scr_y_normal, scr_y_stop, scr_y_stop, scr_y_pl1, scr_y_pl2, scr_y_catch, scr_y_caught,
	scr_y_base, scr_y_dm, scr_y_abyss, scr_y_stop_pl, scr_y_reset, scr_y_low
} ;

/* the quake's size, frames and kind, by level */
static const Float quake_size_tbl[3] = { 5.0f, 12.5f, 20.0f } ;
static const Sint32 quake_time_tbl[3] = { 20, 20, 20 } ;
static const Uint8 quake_type_tbl[3] = { 0, 0, 0 } ;
static void ( *quake_tbl[2] )( Uint8 lv ) = { scr_quake_0, scr_quake_1 } ;

/* the cameras: the fight's, and a path of set keys */
static void ( *scr_tbl[2] )( void ) = { scr_normal, scr_demo } ;

static PLAYER_WORK *scr_pl1w ;		/* the two players the camera follows */
static PLAYER_WORK *scr_pl2w ;
static VEC3 eye_now ;		/* where the eye's and the point's moves are */
static VEC3 target_now ;
static VEC3 target_ang ;		/* the turns so far of the point looked at, and of the eye */
static VEC3 eye_ang ;
static Float quake_size ;
static Uint8 unk_fd8[2] ;
static Uint8 quake_on ;
static Uint8 unk_fdb ;
static Uint8 unk_fdc ;

/****************************************************************/
/*			Set the camera up									*/
/****************************************************************/
void scr_work_init( void )
{
	scr_work.scr_type = 0 ;
	scr_work.scr_routine[0] = 0 ;
	scr_work.scr_routine[1] = 0 ;
	scr_work.scr_routine[2] = 0 ;
	scr_work.unk_0a = 0 ;
	scr_work.eye[0].x = 0.0f ;
	scr_work.eye[0].y = 95.0f ;
	scr_work.eye[0].z = 320.0f / nlTan( 0xf4a ) ;
	scr_work.unk_70 = 95.0f ;
	scr_work.rise_y = 0.0f ;
	scr_work.fov_x = 43.0f ;
	scr_work.target[0].x = 0.0f ;
	scr_work.target[0].y = 95.0f ;
	scr_work.target[0].z = 0.0f ;
	scr_work.shift_y = -0.41f ;
	scr_work.eye_move_st = 0 ;
	scr_work.target_move_st = 0 ;
	scr_work.eye_turn_st = 0 ;
	scr_work.target_turn_st = 0 ;
	scr_work.eye_goal.x = 0.0f ;
	scr_work.eye_goal.y = 95.0f ;
	scr_work.eye_goal.z = 320.0f / nlTan( 0xf4a ) ;
	scr_work.target_goal.x = 0.0f ;
	scr_work.target_goal.x = 95.0f ;
	scr_work.target_goal.x = 0.0f ;
	scr_work.target_turn.x = 0.0f ;
	scr_work.target_turn.y = 0.0f ;
	scr_work.target_turn.z = 0.0f ;
	scr_work.eye_turn.x = 0.0f ;
	scr_work.eye_turn.y = 0.0f ;
	scr_work.eye_turn.z = 0.0f ;
	scr_work.eye_step.x = 0.0f ;
	scr_work.eye_step.y = 0.0f ;
	scr_work.eye_step.z = 0.0f ;
	scr_work.target_step.x = 0.0f ;
	scr_work.target_step.y = 0.0f ;
	scr_work.target_step.z = 0.0f ;
	unk_fdb = 0 ;
	unk_fdc = 0 ;
	eye_now.x = 0.0f ;
	eye_now.y = 95.0f ;
	eye_now.z = 320.0f / nlTan( 0xf4a ) ;
	target_now.x = 0.0f ;
	target_now.y = 95.0f ;
	target_now.z = 0.0f ;
	target_ang.x = 0.0f ;
	target_ang.y = 0.0f ;
	target_ang.z = 0.0f ;
	eye_ang.x = 0.0f ;
	eye_ang.y = 0.0f ;
	eye_ang.z = 0.0f ;
	scr_work.stage_right = 1280.0f ;
	scr_work.stage_left = -1280.0f ;
	scr_work.stage_top = 900.0f + 98.4f + 240.0f ;
	scr_work.stage_bottom = 95.0f + 98.4f + -240.0f ;
	scr_work.init_flag = 0 ;
	scr_work.excep_side = 0 ;
}

/****************************************************************/
/*			The projection and the camera's view				*/
/****************************************************************/
void scr_matrix_set( void )
{
	VEC3 eye ;
	VEC3 obj ;

	eye.x = scr_work.eye[0].x ;
	eye.y = scr_work.eye[0].y ;
	eye.z = scr_work.eye[0].z ;
	obj.x = scr_work.target[0].x ;
	obj.y = scr_work.target[0].y ;
	obj.z = scr_work.target[0].z ;
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlShiftPerspective( 0.0f, scr_work.shift_y ) ;
	nlPerspectiveX( DEG_ANG( scr_work.fov_x ), 1.3333334f, 0.1f, 12000.0f ) ;
	nlMatrixMode( 2 ) ;
	nlUnitMatrix() ;
	nlLookAt( &eye, &obj, scr_work.roll ) ;
	nlMatrixMode( 1 ) ;
}

/****************************************************************/
/*			The same, with the eye and the point a tenth as		*/
/*			far													*/
/****************************************************************/
void scr_far_matrix_set( void )
{
	VEC3 eye ;
	VEC3 obj ;

	eye.x = scr_work.eye[0].x * 0.1f ;
	eye.y = scr_work.eye[0].y * 0.1f ;
	eye.z = scr_work.eye[0].z * 0.1f ;
	obj.x = scr_work.target[0].x * 0.1f ;
	obj.y = scr_work.target[0].y * 0.1f ;
	obj.z = scr_work.target[0].z * 0.1f ;
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlShiftPerspective( 0.0f, scr_work.shift_y ) ;
	nlPerspectiveX( DEG_ANG( scr_work.fov_x ), 1.3333334f, 0.1f, 12000.0f ) ;
	nlMatrixMode( 2 ) ;
	nlUnitMatrix() ;
	nlLookAt( &eye, &obj, scr_work.roll ) ;
	nlMatrixMode( 1 ) ;
}

/****************************************************************/
/*			A projection of a quarter turn, with no view		*/
/****************************************************************/
void scr_2d_matrix_set( void )
{
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlShiftPerspective( 0.0f, 0.0f ) ;
	nlPerspectiveX( 0x4000, 1.3333334f, 0.1f, 12000.0f ) ;
	nlMatrixMode( 2 ) ;
	nlUnitMatrix() ;
	nlMatrixMode( 1 ) ;
}

/****************************************************************/
/*			A narrower projection, and the camera's view		*/
/*			with no roll										*/
/****************************************************************/
void scr_matrix_set3( void )
{
	nlMatrixMode( 3 ) ;
	nlUnitMatrix() ;
	nlShiftPerspective( 0.0f, 0.0f ) ;
	nlPerspectiveX( 0x1e94, 1.3333334f, 0.1f, 12000.0f ) ;
	nlMatrixMode( 2 ) ;
	nlUnitMatrix() ;
	nlLookAt( scr_work.eye, scr_work.target, 0 ) ;
	nlMatrixMode( 1 ) ;
}

/*==============================================================*/
/*			The fight camera's set-up							*/
/*==============================================================*/
static void scr_normal_init( void )
{
	scr_work_init() ;
	scr_work.scr_routine[0] ++ ;
}

/*==============================================================*/
/*			The fight camera's frame							*/
/*==============================================================*/
static void scr_normal_move( void )
{
	scr_work.eye[1] = scr_work.eye[0] ;
	scr_work.target[1] = scr_work.target[0] ;
	scr_work.rise_y = 0.0f ;
	scr_work.eye_goal.z = 320.0f / nlTan( 0xf4a ) ;
	if( !scr_work.init_flag ) scr_work.init_flag = 1 ;
	else {
		if( scr_excep_ck() ) scr_excep_clr() ;
		scr_mode_move() ;
		scr_quake( scr_work.quake_level ) ;
		scr_edge_set() ;
	}
}

/****************************************************************/
/*			The fight camera									*/
/****************************************************************/
void scr_normal( void )
{
	switch( scr_work.scr_routine[0] ){
	case 0 :
		scr_normal_init() ;
	case 1 :
		scr_normal_move() ;
	}
}

/*==============================================================*/
/*			A path's set-up: start from its first key			*/
/*==============================================================*/
static void scr_demo_init( void )
{
	Float *pos, *turn ;

	scr_work.scr_routine[0] ++ ;
	pos = path_pos_tbl[scr_work.scr_routine[1]] ;
	turn = path_turn_tbl[scr_work.scr_routine[1]] ;
	scr_work.eye_move_st = 0 ;
	scr_work.target_move_st = 0 ;
	scr_work.eye_turn_st = 0 ;
	scr_work.target_turn_st = 0 ;
	scr_work.eye[0].x = pos[0] ;
	scr_work.eye[0].y = pos[1] ;
	scr_work.eye[0].z = pos[2] ;
	scr_work.target[0].x = pos[3] ;
	scr_work.target[0].y = pos[4] ;
	scr_work.target[0].z = pos[5] ;
	scr_work.eye_turn.x = turn[0] ;
	scr_work.eye_turn.y = turn[1] ;
	scr_work.eye_turn.z = turn[2] ;
	scr_work.target_turn.x = turn[3] ;
	scr_work.target_turn.y = turn[4] ;
	scr_work.target_turn.z = turn[5] ;
	eye_now.x = scr_work.eye[0].x ;
	eye_now.y = scr_work.eye[0].y ;
	eye_now.z = scr_work.eye[0].z ;
	target_now.x = scr_work.target[0].x ;
	target_now.y = scr_work.target[0].y ;
	target_now.z = scr_work.target[0].z ;
	scr_work.scr_routine[2] = 1 ;
}

/*==============================================================*/
/*			A path's frame										*/
/*==============================================================*/
static void scr_demo_run( void )
{
	scr_work.eye[1] = scr_work.eye[0] ;
	scr_work.target[1] = scr_work.target[0] ;
	scr_demo_move( scr_work.scr_routine[1] ) ;
}

/*==============================================================*/
/*			The end of a path: the camera stays					*/
/*==============================================================*/
static void scr_demo_end( void )
{
	scr_work.eye[1] = scr_work.eye[0] ;
	scr_work.target[1] = scr_work.target[0] ;
}

/****************************************************************/
/*			The path camera										*/
/****************************************************************/
void scr_demo( void )
{
	switch( scr_work.scr_routine[0] ){
	case 0 :
		scr_demo_init() ;
	case 1 :
		scr_demo_run() ;
		break ;
	case 2 :
		scr_demo_end() ;
		break ;
	}
}

/****************************************************************/
/*			Run the path to its next key, and on to the one		*/
/*			after once the moves are there						*/
/****************************************************************/
void scr_demo_move( Uint8 no )
{
	Sint32 *time ;
	Float *pos, *turn ;
	Uint8 flag ;

	time = path_time_tbl[scr_work.scr_routine[1]] ;
	pos = path_pos_tbl[scr_work.scr_routine[1]] ;
	turn = path_turn_tbl[scr_work.scr_routine[1]] ;
	flag = 0 ;
	if( scr_work.scr_routine[2] < *time ){
		flag |= scr_work.target_turn_st ;
		flag |= scr_work.eye_turn_st ;
		flag |= scr_work.eye_move_st ;
		flag |= scr_work.target_move_st ;
		time ++ ;
		if( flag == 2 ){
			scr_work.eye_move_st = 0 ;
			scr_work.target_move_st = 0 ;
			scr_work.eye_turn_st = 0 ;
			scr_work.target_turn_st = 0 ;
			scr_work.scr_routine[2] ++ ;
		}
		time += scr_work.scr_routine[2] ;
	}
	else {
		time += *time ;
		scr_work.scr_routine[0] ++ ;
	}
	pos += scr_work.scr_routine[2] * 6 ;
	turn += scr_work.scr_routine[2] * 6 ;
	scr_work.eye_goal.x = pos[0] ;
	scr_work.eye_goal.y = pos[1] ;
	scr_work.eye_goal.z = pos[2] ;
	scr_work.target_goal.x = pos[3] ;
	scr_work.target_goal.y = pos[4] ;
	scr_work.target_goal.z = pos[5] ;
	scr_work.eye_turn.x = turn[0] ;
	scr_work.eye_turn.y = turn[1] ;
	scr_work.eye_turn.z = turn[2] ;
	scr_work.target_turn.x = turn[3] ;
	scr_work.target_turn.y = turn[4] ;
	scr_work.target_turn.z = turn[5] ;
	scr_eye_move( 0, *time ) ;
	scr_target_move( 0, *time ) ;
	if( scr_work.eye_turn.x || scr_work.eye_turn.y || scr_work.eye_turn.z ){
		scr_eye_turn( 0, *time ) ;
	}
	if( scr_work.target_turn.x || scr_work.target_turn.y || scr_work.target_turn.z ){
		scr_target_turn( 0, *time ) ;
	}
}

/****************************************************************/
/*			Start path no										*/
/****************************************************************/
void scr_demo_set( Uint8 no )
{
	scr_work.scr_routine[0] = 1 ;
	scr_work.scr_routine[1] = no ;
	scr_work.scr_routine[2] = 0 ;
	scr_work.eye_move_st = 0 ;
	scr_work.target_move_st = 0 ;
	scr_work.eye_turn_st = 0 ;
	scr_work.target_turn_st = 0 ;
}

/****************************************************************/
/*			The camera's frame: it follows the two point		*/
/*			characters											*/
/****************************************************************/
void scr_move( void )
{
	scr_pl1w = vs_sys_work.team_w[0][0] ;
	scr_pl2w = vs_sys_work.team_w[1][0] ;
	scr_tbl[scr_work.scr_type]() ;
}

/****************************************************************/
/*			Move the eye to where it is going, over time		*/
/*			frames												*/
/****************************************************************/
void scr_eye_move( int no, int time )
{
	static Sint32 cnt ;

	if( !scr_work.eye_move_st ){
		cnt = 0 ;
		scr_work.eye_move_st = 1 ;
		scr_work.eye_step.x = scr_work.eye_goal.x - eye_now.x ? ( scr_work.eye_goal.x - eye_now.x ) / time : 0.0f ;
		scr_work.eye_step.y = scr_work.eye_goal.y - eye_now.y ? ( scr_work.eye_goal.y - eye_now.y ) / time : 0.0f ;
		scr_work.eye_step.z = scr_work.eye_goal.z - eye_now.z ? ( scr_work.eye_goal.z - eye_now.z ) / time : 0.0f ;
	}
	if( cnt < time - 1 ){
		cnt ++ ;
		scr_work.eye[0].x = eye_now.x + scr_work.eye_step.x ;
		scr_work.eye[0].y = eye_now.y + scr_work.eye_step.y ;
		scr_work.eye[0].z = eye_now.z + scr_work.eye_step.z ;
		eye_now.x = scr_work.eye[0].x ;
		eye_now.y = scr_work.eye[0].y ;
		eye_now.z = scr_work.eye[0].z ;
	}
	else {
		scr_work.eye_move_st = 2 ;
		scr_work.eye[0].x = scr_work.eye_goal.x ;
		scr_work.eye[0].y = scr_work.eye_goal.y ;
		scr_work.eye[0].z = scr_work.eye_goal.z ;
		eye_now.x = scr_work.eye[0].x ;
		eye_now.y = scr_work.eye[0].y ;
		eye_now.z = scr_work.eye[0].z ;
	}
}

/****************************************************************/
/*			Move the point looked at to where it is going,		*/
/*			over time frames									*/
/****************************************************************/
void scr_target_move( int no, int time )
{
	static Sint32 cnt ;

	if( !scr_work.target_move_st ){
		cnt = 0 ;
		scr_work.target_move_st = 1 ;
		scr_work.target_step.x = scr_work.target_goal.x - target_now.x ? ( scr_work.target_goal.x - target_now.x ) / time : 0.0f ;
		scr_work.target_step.y = scr_work.target_goal.y - target_now.y ? ( scr_work.target_goal.y - target_now.y ) / time : 0.0f ;
		scr_work.target_step.z = scr_work.target_goal.z - target_now.z ? ( scr_work.target_goal.z - target_now.z ) / time : 0.0f ;
	}
	if( cnt < time - 1 ){
		cnt ++ ;
		scr_work.target[0].x = target_now.x + scr_work.target_step.x ;
		scr_work.target[0].y = target_now.y + scr_work.target_step.y ;
		scr_work.target[0].z = target_now.z + scr_work.target_step.z ;
		target_now.x = scr_work.target[0].x ;
		target_now.y = scr_work.target[0].y ;
		target_now.z = scr_work.target[0].z ;
	}
	else {
		scr_work.target_move_st = 2 ;
		scr_work.target[0].x = scr_work.target_goal.x ;
		scr_work.target[0].y = scr_work.target_goal.y ;
		scr_work.target[0].z = scr_work.target_goal.z ;
		target_now.x = scr_work.target[0].x ;
		target_now.y = scr_work.target[0].y ;
		target_now.z = scr_work.target[0].z ;
	}
}

/****************************************************************/
/*			Turn the point looked at about the eye, by a		*/
/*			path key's turns over time frames					*/
/****************************************************************/
void scr_target_turn( int no, int time )
{
	static Sint32 cnt ;
	static VEC3 spd ;
	static Float dx, dy, dz ;

	if( !scr_work.target_turn_st ){
		target_ang.x += spd.x * cnt ;
		target_ang.y += spd.y * cnt ;
		cnt = 0 ;
		scr_work.target_turn_st = 1 ;
		spd.x = ( scr_work.target_turn.x - target_ang.x ) / time ;
		spd.y = ( scr_work.target_turn.y - target_ang.y ) / time ;
		spd.z = ( scr_work.target_turn.z - target_ang.z ) / time ;
	}
	if( cnt < time - 1 ){
		dy = scr_work.target[0].y - scr_work.eye[0].y ;
		dz = scr_work.target[0].z - scr_work.eye[0].z ;
		scr_work.target[0].y = scr_work.eye[0].y + dy * nlCos( DEG_ANG( target_ang.x + cnt * spd.x ) )
			- dz * nlSin( DEG_ANG( target_ang.x + cnt * spd.x ) ) ;
		scr_work.target[0].z = scr_work.eye[0].z + dy * nlSin( DEG_ANG( target_ang.x + cnt * spd.x ) )
			+ dz * nlCos( DEG_ANG( target_ang.x + cnt * spd.x ) ) ;
		dx = scr_work.target[0].x - scr_work.eye[0].x ;
		dz = scr_work.target[0].z - scr_work.eye[0].z ;
		scr_work.target[0].x = scr_work.eye[0].x + dx * nlCos( DEG_ANG( target_ang.y + cnt * spd.y ) )
			+ dz * nlSin( DEG_ANG( target_ang.y + cnt * spd.y ) ) ;
		scr_work.target[0].z = scr_work.eye[0].z - dx * nlSin( DEG_ANG( target_ang.y + cnt * spd.y ) )
			+ dz * nlCos( DEG_ANG( target_ang.y + cnt * spd.y ) ) ;
		target_ang.z += spd.z ;
		scr_work.roll = DEG_ANG( target_ang.z ) ;
		cnt ++ ;
	}
	else {
		scr_work.target_turn_st = 2 ;
		target_ang.z = scr_work.target_turn.z ;
		scr_work.roll = DEG_ANG( target_ang.z ) ;
		dy = scr_work.target[0].y - scr_work.eye[0].y ;
		dz = scr_work.target[0].z - scr_work.eye[0].z ;
		scr_work.target[0].y = scr_work.eye[0].y + dy * nlCos( DEG_ANG( target_ang.x + cnt * spd.x ) )
			- dz * nlSin( DEG_ANG( target_ang.x + cnt * spd.x ) ) ;
		scr_work.target[0].z = scr_work.eye[0].z + dy * nlSin( DEG_ANG( target_ang.x + cnt * spd.x ) )
			+ dz * nlCos( DEG_ANG( target_ang.x + cnt * spd.x ) ) ;
		dx = scr_work.target[0].x - scr_work.eye[0].x ;
		dz = scr_work.target[0].z - scr_work.eye[0].z ;
		scr_work.target[0].x = scr_work.eye[0].x + dx * nlCos( DEG_ANG( target_ang.y + cnt * spd.y ) )
			+ dz * nlSin( DEG_ANG( target_ang.y + cnt * spd.y ) ) ;
		scr_work.target[0].z = scr_work.eye[0].z - dx * nlSin( DEG_ANG( target_ang.y + cnt * spd.y ) )
			+ dz * nlCos( DEG_ANG( target_ang.y + cnt * spd.y ) ) ;
	}
}

/****************************************************************/
/*			Turn the eye about the point looked at, by a		*/
/*			path key's turns over time frames					*/
/****************************************************************/
void scr_eye_turn( int no, int time )
{
	static Sint32 cnt ;
	static VEC3 spd ;
	static Float dx, dy, dz ;

	if( !scr_work.eye_turn_st ){
		eye_ang.x += spd.x * cnt ;
		eye_ang.y += spd.y * cnt ;
		eye_ang.z += spd.z * cnt ;
		cnt = 0 ;
		scr_work.eye_turn_st = 1 ;
		spd.x = ( scr_work.eye_turn.x - eye_ang.x ) / time ;
		spd.y = ( scr_work.eye_turn.y - eye_ang.y ) / time ;
		spd.z = ( scr_work.eye_turn.z - eye_ang.z ) / time ;
	}
	if( cnt < time - 1 ){
		dx = scr_work.eye[0].x - scr_work.target[0].x ;
		dy = scr_work.eye[0].y - scr_work.target[0].y ;
		dz = scr_work.eye[0].z - scr_work.target[0].z ;
		scr_work.eye[0].y = scr_work.target[0].y + dy * nlCos( DEG_ANG( eye_ang.x + spd.x * cnt ) )
			- dz * nlSin( DEG_ANG( eye_ang.x + spd.x * cnt ) ) ;
		scr_work.eye[0].z = scr_work.target[0].z + dy * nlSin( DEG_ANG( eye_ang.x + spd.x * cnt ) )
			+ dz * nlCos( DEG_ANG( eye_ang.x + spd.x * cnt ) ) ;
		dx = scr_work.eye[0].x - scr_work.target[0].x ;
		dy = scr_work.eye[0].y - scr_work.target[0].y ;
		dz = scr_work.eye[0].z - scr_work.target[0].z ;
		scr_work.eye[0].x = scr_work.target[0].x + dx * nlCos( DEG_ANG( eye_ang.y + spd.y * cnt ) )
			- dz * nlSin( DEG_ANG( eye_ang.y + spd.y * cnt ) ) ;
		scr_work.eye[0].z = scr_work.target[0].z - dy * nlSin( DEG_ANG( eye_ang.y + spd.y * cnt ) )
			+ dz * nlCos( DEG_ANG( eye_ang.y + spd.y * cnt ) ) ;
		dx = scr_work.eye[0].x - scr_work.target[0].x ;
		dy = scr_work.eye[0].y - scr_work.target[0].y ;
		dz = scr_work.eye[0].z - scr_work.target[0].z ;
		scr_work.eye[0].y = scr_work.eye[0].y + dy * nlCos( DEG_ANG( eye_ang.z + spd.z * cnt ) )
			+ dx * nlSin( DEG_ANG( eye_ang.z + spd.z * cnt ) ) ;
		scr_work.eye[0].x = scr_work.eye[0].x + dx * nlCos( DEG_ANG( eye_ang.z + spd.z * cnt ) )
			- dy * nlSin( DEG_ANG( eye_ang.z + spd.z * cnt ) ) ;
		cnt ++ ;
	}
	else {
		cnt = time ;
		scr_work.eye_turn_st = 2 ;
		dx = scr_work.eye[0].x - scr_work.target[0].x ;
		dy = scr_work.eye[0].y - scr_work.target[0].y ;
		dz = scr_work.eye[0].z - scr_work.target[0].z ;
		scr_work.eye[0].y = scr_work.target[0].y + dy * nlCos( DEG_ANG( eye_ang.x + spd.x * cnt ) )
			- dz * nlSin( DEG_ANG( eye_ang.x + spd.x * cnt ) ) ;
		scr_work.eye[0].z = scr_work.target[0].z + dy * nlSin( DEG_ANG( eye_ang.x + spd.x * cnt ) )
			+ dz * nlCos( DEG_ANG( eye_ang.x + spd.x * cnt ) ) ;
		dx = scr_work.eye[0].x - scr_work.target[0].x ;
		dy = scr_work.eye[0].y - scr_work.target[0].y ;
		dz = scr_work.eye[0].z - scr_work.target[0].z ;
		scr_work.eye[0].x = scr_work.target[0].x + dx * nlCos( DEG_ANG( eye_ang.y + spd.y * cnt ) )
			- dz * nlSin( DEG_ANG( eye_ang.y + spd.y * cnt ) ) ;
		scr_work.eye[0].z = scr_work.target[0].z - dy * nlSin( DEG_ANG( eye_ang.y + spd.y * cnt ) )
			+ dz * nlCos( DEG_ANG( eye_ang.y + spd.y * cnt ) ) ;
		dx = scr_work.eye[0].x - scr_work.target[0].x ;
		dy = scr_work.eye[0].y - scr_work.target[0].y ;
		dz = scr_work.eye[0].z - scr_work.target[0].z ;
		scr_work.eye[0].y = scr_work.eye[0].y + dy * nlCos( DEG_ANG( eye_ang.z + spd.z * cnt ) )
			+ dx * nlSin( DEG_ANG( eye_ang.z + spd.z * cnt ) ) ;
		scr_work.eye[0].x = scr_work.eye[0].x + dx * nlCos( DEG_ANG( eye_ang.z + spd.z * cnt ) )
			- dy * nlSin( DEG_ANG( eye_ang.z + spd.z * cnt ) ) ;
	}
}

/****************************************************************/
/*			X mode 0: keep both players on the screen, the		*/
/*			camera between them when they are far apart			*/
/****************************************************************/
void scr_x_normal( int spd )
{
	Float d1, d2 ;

	d1 = fabs( scr_pl1w->pos.x - scr_work.eye_goal.x ) + -200.0f ;
	d2 = fabs( scr_pl2w->pos.x - scr_work.eye_goal.x ) + -200.0f ;
	d1 = d1 >= 0.0f ? d1 : 0.0f ;
	d2 = d2 >= 0.0f ? d2 : 0.0f ;
	if( fabs( scr_pl1w->pos.x - scr_pl2w->pos.x ) > 400.0f ){
		scr_work.eye_goal.x = ( scr_pl1w->pos.x + scr_pl2w->pos.x ) / 2.0f ;
	}
	else if( d1 >= d2 ){
		if( d1 > 0.0f ){
			if( scr_pl1w->pos.x > scr_work.eye_goal.x ) scr_work.eye_goal.x += d1 ;
			else scr_work.eye_goal.x -= d1 ;
		}
	}
	else if( d2 > 0.0f ){
		if( scr_pl2w->pos.x > scr_work.eye_goal.x ) scr_work.eye_goal.x += d2 ;
		else scr_work.eye_goal.x -= d2 ;
	}
	scr_x_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.x = scr_work.eye_goal.x ;
	scr_eye_x_set( 0, 8 ) ;
	scr_target_x_set( 0, 8 ) ;
}

/****************************************************************/
/*			Y mode 0: rise with the higher of the two			*/
/*			players												*/
/****************************************************************/
void scr_y_normal( int spd )
{
	Float h1, h2 ;

	h1 = DOT_Y( scr_pl1w->size_top ) ;
	h2 = DOT_Y( scr_pl2w->size_top ) ;
	if( scr_pl1w->pl_type >= PL_ABYSS1 && scr_pl1w->pl_type <= PL_ABYSS3 ) h1 = 0.0f ;
	if( scr_pl2w->pl_type >= PL_ABYSS1 && scr_pl2w->pl_type <= PL_ABYSS3 ) h2 = 0.0f ;
	if( scr_pl1w->pos.y + h1 > scr_pl2w->pos.y + h2 ){
		if( scr_pl1w->pos.y + h1 > 350.0f ) scr_work.rise_y = scr_pl1w->pos.y + h1 + -350.0f ;
	}
	else if( scr_pl2w->pos.y + h2 > 350.0f ) scr_work.rise_y = scr_pl2w->pos.y + h2 + -350.0f ;
	scr_work.eye_goal.y = scr_work.rise_y + 95.0f ;
	scr_y_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.y = scr_work.eye_goal.y ;
	scr_eye_yz_set( 0, 4 ) ;
	scr_target_yz_set( 0, 4 ) ;
}

/****************************************************************/
/*			X modes 1 and 2: the camera stays across			*/
/****************************************************************/
void scr_x_stop( int spd )
{
	Float buf[8] ;

	scr_work.eye_move_st = 0 ;
	scr_work.target_move_st = 0 ;
	scr_x_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.x = scr_work.eye_goal.x ;
}

/****************************************************************/
/*			Y modes 1 and 2: the camera stays up and down		*/
/****************************************************************/
void scr_y_stop( int spd )
{
	scr_work.eye_move_st = 0 ;
	scr_work.target_move_st = 0 ;
	scr_y_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.y = scr_work.eye_goal.y ;
}

/****************************************************************/
/*			X mode 6: follow side 0 while its player is in		*/
/*			routine 2, else side 1								*/
/****************************************************************/
void scr_x_caught( int spd )
{
	if( scr_pl1w->routine_1 == PL_CAUGHT ) scr_x_pl1( 16 ) ;
	else scr_x_pl2( 16 ) ;
}

/****************************************************************/
/*			Y mode 6											*/
/****************************************************************/
void scr_y_caught( int spd )
{
	if( scr_pl1w->routine_1 == PL_CAUGHT ) scr_y_pl1( 4 ) ;
	else scr_y_pl2( 4 ) ;
}

/****************************************************************/
/*			X mode 5: follow side 0 while its player is in		*/
/*			routine 1, or side 1's is in routine 2				*/
/****************************************************************/
void scr_x_catch( int spd )
{
	if( scr_pl1w->routine_1 == PL_CATCH || scr_pl2w->routine_1 == PL_CAUGHT ) scr_x_pl1( 16 ) ;
	else scr_x_pl2( 16 ) ;
}

/****************************************************************/
/*			Y mode 5											*/
/****************************************************************/
void scr_y_catch( int spd )
{
	if( scr_pl1w->routine_1 == PL_CATCH || scr_pl2w->routine_1 == PL_CAUGHT ) scr_y_pl1( 4 ) ;
	else scr_y_pl2( 4 ) ;
}

/****************************************************************/
/*			X mode 3: follow side 0's player					*/
/****************************************************************/
void scr_x_pl1( int spd )
{
	scr_work.eye_goal.x = scr_pl1w->pos.x ;
	scr_x_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.x = scr_work.eye_goal.x ;
	scr_eye_x_set( 0, spd ) ;
	scr_target_x_set( 0, spd ) ;
	return ;
}

/****************************************************************/
/*			Y mode 3: follow side 0's player up and down		*/
/****************************************************************/
void scr_y_pl1( int spd )
{
	Float h ;

	h = DOT_Y( scr_pl1w->size_top ) ;
	if( scr_pl1w->pl_type >= PL_ABYSS1 && scr_pl1w->pl_type <= PL_ABYSS3 ) h = 0.0f ;
	if( scr_pl1w->pos.y + h > 350.0f ) scr_work.rise_y = scr_pl1w->pos.y + h + -350.0f ;
	scr_work.eye_goal.y = scr_work.rise_y + 95.0f ;
	scr_y_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.y = scr_work.eye_goal.y ;
	if( scr_pl1w->pl_type == PL_ABYSS3 ){
		scr_eye_yz_set( 0, 2 ) ;
		scr_target_yz_set( 0, 2 ) ;
	}
	else {
		scr_eye_yz_set( 0, 4 ) ;
		scr_target_yz_set( 0, 4 ) ;
	}
}

/****************************************************************/
/*			X mode 4: follow side 1's player					*/
/****************************************************************/
void scr_x_pl2( int spd )
{
	scr_work.eye_goal.x = scr_pl2w->pos.x ;
	scr_x_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.x = scr_work.eye_goal.x ;
	scr_eye_x_set( 0, spd ) ;
	scr_target_x_set( 0, spd ) ;
	return ;
}

/****************************************************************/
/*			Y mode 4: follow side 1's player up and down		*/
/****************************************************************/
void scr_y_pl2( int spd )
{
	Float h ;

	h = DOT_Y( scr_pl2w->size_top ) ;
	if( scr_pl2w->pl_type >= PL_ABYSS1 && scr_pl2w->pl_type <= PL_ABYSS3 ) h = 0.0f ;
	if( scr_pl2w->pos.y + h > 350.0f ) scr_work.rise_y = scr_pl2w->pos.y + h + -350.0f ;
	scr_work.eye_goal.y = scr_work.rise_y + 95.0f ;
	scr_y_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.y = scr_work.eye_goal.y ;
	if( scr_pl2w->pl_type == PL_ABYSS3 ){
		scr_eye_yz_set( 0, 2 ) ;
		scr_target_yz_set( 0, 2 ) ;
	}
	else {
		scr_eye_yz_set( 0, 4 ) ;
		scr_target_yz_set( 0, 4 ) ;
	}
}

/****************************************************************/
/*			Y mode 7: back to the camera's usual height			*/
/****************************************************************/
void scr_y_base( int spd )
{
	scr_work.eye_goal.y = 95.0f ;
	scr_y_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.y = scr_work.eye_goal.y ;
	scr_eye_yz_set( 0, 4 ) ;
	scr_target_yz_set( 0, 4 ) ;
}

/****************************************************************/
/*			Y mode 12: follow the lower of the two players		*/
/****************************************************************/
void scr_y_low( int spd )
{
	if( scr_pl1w->pos.y >= scr_pl2w->pos.y ) scr_y_pl2( 4 ) ;
	else scr_y_pl1( 4 ) ;
}

/****************************************************************/
/*			X mode 8: follow side 0 while its player is in		*/
/*			routine 3, else side 1								*/
/****************************************************************/
void scr_x_dm( int spd )
{
	if( scr_pl1w->routine_1 == PL_DAMAGE ) scr_x_pl1( 16 ) ;
	else scr_x_pl2( 16 ) ;
}

/****************************************************************/
/*			Y mode 8											*/
/****************************************************************/
void scr_y_dm( int spd )
{
	if( scr_pl1w->routine_1 == PL_DAMAGE ) scr_y_pl1( 4 ) ;
	else scr_y_pl2( 4 ) ;
}

/****************************************************************/
/*			X mode 10: stay, within the stage					*/
/****************************************************************/
void scr_x_hold( int spd )
{
	scr_x_limit( &scr_work.eye_goal, &scr_work.target_goal ) ;
	scr_work.target_goal.x = scr_work.eye_goal.x ;
	scr_eye_x_set( 0, 8 ) ;
	scr_target_x_set( 0, 8 ) ;
}

/****************************************************************/
/*			Y mode 10: follow the side whose player stops		*/
/*			the game											*/
/****************************************************************/
void scr_y_stop_pl( int spd )
{
	if( scr_pl1w->hyper_stop_flag ) scr_y_pl1( 2 ) ;
	else scr_y_pl2( 2 ) ;
}

/****************************************************************/
/*			X mode 9: follow Abyss's last form					*/
/****************************************************************/
void scr_x_abyss( int spd )
{
	if( scr_pl1w->pl_type == PL_ABYSS3 ) scr_x_pl1( 2 ) ;
	else scr_x_pl2( 2 ) ;
}

/****************************************************************/
/*			Y mode 9											*/
/****************************************************************/
void scr_y_abyss( int spd )
{
	if( scr_pl1w->pl_type == PL_ABYSS3 ) scr_y_pl1( 2 ) ;
	else scr_y_pl2( 2 ) ;
}

/****************************************************************/
/*			X mode 11: the camera at once to the stage's		*/
/*			centre												*/
/****************************************************************/
void scr_x_reset( int spd )
{
	scr_work.eye_goal.x = 0.0f ;
	scr_work.target_goal.x = scr_work.eye_goal.x ;
	eye_now.x = scr_work.eye_goal.x ;
	target_now.x = scr_work.eye_goal.x ;
	scr_work.eye[0].x = 0.0f ;
	scr_work.target[0].x = 0.0f ;
}

/****************************************************************/
/*			Y mode 11: the camera at once to its usual			*/
/*			height												*/
/****************************************************************/
void scr_y_reset( int spd )
{
	scr_work.eye_goal.y = 95.0f ;
	scr_work.target_goal.y = scr_work.eye_goal.y ;
	eye_now.y = scr_work.eye_goal.y ;
	target_now.y = scr_work.eye_goal.y ;
	scr_work.eye[0].y = 95.0f ;
	scr_work.target[0].y = scr_work.eye[0].y ;
}

/****************************************************************/
/*			The screen quake of level lv						*/
/****************************************************************/
void scr_quake( Uint8 lv )
{
	const Uint8 *type = quake_type_tbl ;

	if( lv ){
		if( scr_work.quake_flag ){
			scr_work.quake_flag = 0 ;
			quake_on = 0 ;
		}
		type += lv - 1 ;
		quake_tbl[*type]( lv ) ;
	}
}

/****************************************************************/
/*			A quake up and down, shrinking each frame			*/
/****************************************************************/
void scr_quake_0( Uint8 lv )
{
	static Sint32 timer ;
	static Sint32 dir ;
	static Float step ;
	const Float *size = quake_size_tbl ;
	const Sint32 *time = quake_time_tbl ;

	size += lv - 1 ;
	time += lv - 1 ;
	if( !quake_on ){
		timer = *time ;
		dir = quake_on = 1 ;
		quake_size = *size ;
		step = quake_size / timer ;
	}
	if( timer ){
		timer -- ;
		scr_work.eye[0].y += quake_size * dir ;
		scr_work.target[0].y = scr_work.eye[0].y ;
		dir = -dir ;
		quake_size -= step ;
		return ;
	}
	quake_on = 0 ;
	scr_work.quake_level = 0 ;
}

/****************************************************************/
/*			The same, with its own counts						*/
/****************************************************************/
void scr_quake_1( Uint8 lv )
{
	static Sint32 timer ;
	static Sint32 dir ;
	static Float step ;
	const Float *size = quake_size_tbl ;
	const Sint32 *time = quake_time_tbl ;

	size += lv - 1 ;
	time += lv - 1 ;
	if( !quake_on ){
		timer = *time ;
		dir = quake_on = 1 ;
		quake_size = *size ;
		step = quake_size / timer ;
	}
	if( timer ){
		timer -- ;
		scr_work.eye[0].y += quake_size * dir ;
		scr_work.target[0].y = scr_work.eye[0].y ;
		dir = -dir ;
		quake_size -= step ;
		return ;
	}
	quake_on = 0 ;
	scr_work.quake_level = 0 ;
}

/****************************************************************/
/*			Move the eye across toward where it is going, a		*/
/*			spd'th of the way									*/
/****************************************************************/
void scr_eye_x_set( int no, int spd )
{
	VEC3 v ;

	v.x = ( scr_work.eye_goal.x - eye_now.x ) / spd ;
	scr_work.eye[0].x = eye_now.x + v.x ;
	eye_now.x = scr_work.eye[0].x ;
}

/****************************************************************/
/*			The same up and down, and in depth					*/
/****************************************************************/
void scr_eye_yz_set( int no, int spd )
{
	VEC3 v ;

	v.y = ( scr_work.eye_goal.y - eye_now.y ) / spd ;
	v.z = ( scr_work.eye_goal.z - eye_now.z ) / spd ;
	scr_work.eye[0].y = eye_now.y + v.y ;
	scr_work.eye[0].z = eye_now.z + v.z ;
	eye_now.y = scr_work.eye[0].y ;
	eye_now.z = scr_work.eye[0].z ;
}

/****************************************************************/
/*			Move the point looked at across						*/
/****************************************************************/
void scr_target_x_set( int no, int spd )
{
	VEC3 v ;

	v.x = ( scr_work.target_goal.x - target_now.x ) / spd ;
	scr_work.target[0].x = target_now.x + v.x ;
	target_now.x = scr_work.target[0].x ;
}

/****************************************************************/
/*			And up and down, and in depth						*/
/****************************************************************/
void scr_target_yz_set( int no, int spd )
{
	VEC3 v ;

	v.y = ( scr_work.target_goal.y - target_now.y ) / spd ;
	v.z = ( scr_work.target_goal.z - target_now.z ) / spd ;
	scr_work.target[0].y = target_now.y + v.y ;
	scr_work.target[0].z = target_now.z + v.z ;
	target_now.y = scr_work.target[0].y ;
	target_now.z = scr_work.target[0].z ;
}

/****************************************************************/
/*			Back to the fight camera's own modes: Abyss's		*/
/*			last form keeps one of its own						*/
/****************************************************************/
void scr_excep_clr( void )
{
	if( scr_pl1w->pl_type == PL_ABYSS3 ){
		scr_work.x_mode = 4 ;
		scr_work.y_mode = 4 ;
		return ;
	}
	else if( scr_pl2w->pl_type == PL_ABYSS3 ){
		scr_work.x_mode = 3 ;
		scr_work.y_mode = 3 ;
		return ;
	}
	else {
		scr_work.x_mode = 0 ;
		scr_work.y_mode = 0 ;
		return ;
	}
}

/****************************************************************/
/*			The same, for the mode across alone					*/
/****************************************************************/
void scr_excep_x_clr( void )
{
	if( scr_work.x_mode == 2 ) return ;
	scr_work.x_mode = scr_pl1w->pl_type == PL_ABYSS3 ? 4 : scr_pl2w->pl_type == PL_ABYSS3 ? 3 : 0 ;
}

/****************************************************************/
/*			And for the mode up and down						*/
/****************************************************************/
void scr_excep_y_clr( void )
{
	if( scr_work.y_mode == 2 ) return ;
	scr_work.y_mode = scr_pl1w->pl_type == PL_ABYSS3 ? 4 : scr_pl2w->pl_type == PL_ABYSS3 ? 3 : 0 ;
}

/****************************************************************/
/*			Whether a move's camera mode is over and the		*/
/*			fight's should come back							*/
/****************************************************************/
Uint8 scr_excep_ck( void )
{
	if( vs_sys_work.vs_st <= VS_READY ) return 0 ;
	if( scr_work.x_mode == 1 || scr_work.y_mode == 1 ){
		if( scr_work.x_mode != 8 && scr_work.y_mode != 8 ){
			if( scr_work.excep_side ){
				if( scr_pl2w->routine_1 == PL_DAMAGE ) return 1 ;
			}
			else if( scr_pl1w->routine_1 == PL_DAMAGE ) return 1 ;
		}
	}
	if( scr_pl1w->routine_1 != scr_pl2w->routine_1 || scr_work.x_mode == 2 || scr_work.y_mode == 2 ) return 0 ;
	if( scr_pl1w->routine_1 ) return 1 ;
	if( scr_pl1w->pl_type == PL_ABYSS3 ){
		if( scr_pl2w->act_st >= N_SPECIAL ) return 0 ;
	}
	else if( scr_pl2w->pl_type == PL_ABYSS3 ){
		if( scr_pl1w->act_st >= N_SPECIAL ) return 0 ;
	}
	else {
		if( scr_pl1w->act_st >= N_SPECIAL ){
			if( scr_pl1w->act_st != N_DAMAGE ) return 0 ;
		}
		if( scr_pl2w->act_st >= N_SPECIAL ){
			if( scr_pl1w->act_st != N_DAMAGE ) return 0 ;
		}
	}
	return 1 ;
}

/****************************************************************/
/*			A move sets the camera's modes, 13 for one it		*/
/*			leaves alone; 0 goes back to the fight's own		*/
/****************************************************************/
void scr_excep_set( PLAYER_WORK *pl_w, Sint8 a, Sint8 b )
{
	if( !pl_w ){
		scr_work.x_mode = a ;
		scr_work.y_mode = b ;
		return ;
	}
	if( pl_w->team_no ) return ;
	if( a != 13 ){
		scr_work.x_mode = a ;
		if( a == 0 ) scr_excep_x_clr() ;
	}
	if( b != 13 ){
		scr_work.y_mode = b ;
		if( b == 0 ) scr_excep_y_clr() ;
	}
	if( ( a == 1 || b == 1 ) && pl_w ) scr_excep_side_set( pl_w ) ;
}

/****************************************************************/
/*			Remember the side whose move set the camera			*/
/****************************************************************/
void scr_excep_side_set( PLAYER_WORK *pl_w )
{
	scr_work.excep_side = pl_w->pl_no ;
}

/****************************************************************/
/*			Run the fight camera's modes, across and then up	*/
/*			and down											*/
/****************************************************************/
void scr_mode_move( void )
{
	if( scr_pl1w->hyper_stop_flag | scr_pl2w->hyper_stop_flag ){
		if( scr_work.x_mode == 1 ) scr_x_tbl[1]( 8 ) ;
		else scr_x_tbl[scr_work.stop_x_mode]( 8 ) ;
		scr_y_tbl[scr_work.stop_y_mode]( 8 ) ;
		return ;
	}
	if( scr_pl1w->snap_partner ){
		scr_x_stop( 2 ) ;
		scr_y_tbl[0]( 2 ) ;
		return ;
	}
	if( scr_pl2w->snap_partner ){
		scr_x_stop( 2 ) ;
		scr_y_tbl[0]( 2 ) ;
		return ;
	}
	if( scr_pl1w->fly_screen_flag | scr_pl2w->fly_screen_flag ){
		if( scr_pl1w->hit_stop != scr_pl2w->hit_stop ){
			if( scr_pl1w->fly_screen_flag ){
				scr_x_pl1( 2 ) ;
				scr_y_pl1( 4 ) ;
			}
			else {
				scr_x_pl2( 2 ) ;
				scr_y_pl2( 4 ) ;
			}
			return ;
		}
		if( !scr_pl1w->hit_stop && !scr_pl2w->hit_stop ){
			if( scr_pl1w->fly_screen_flag ){
				scr_x_pl1( 2 ) ;
				scr_y_pl1( 4 ) ;
			}
			else {
				scr_x_pl2( 2 ) ;
				scr_y_pl2( 4 ) ;
			}
			return ;
		}
	}
	scr_x_tbl[scr_work.x_mode]( 8 ) ;
	scr_y_tbl[scr_work.y_mode]( 8 ) ;
}

/****************************************************************/
/*			Keep the eye and the point looked at within the		*/
/*			stage's ends										*/
/****************************************************************/
void scr_x_limit( VEC3 *eye, VEC3 *obj )
{
	if( eye->x + 320.0f > scr_work.stage_right ){
		eye->x = scr_work.stage_right + -320.0f ;
		obj->x = scr_work.stage_right + -320.0f ;
	}
	if( scr_work.stage_left > eye->x + -320.0f ){
		eye->x = scr_work.stage_left + 320.0f ;
		obj->x = scr_work.stage_left + 320.0f ;
	}
}

/****************************************************************/
/*			And the eye between its lowest and highest			*/
/****************************************************************/
void scr_y_limit( VEC3 *eye, VEC3 *obj )
{
	if( 95.0f > eye->y ) eye->y = 95.0f ;
	if( !( 900.0f > eye->y ) ) eye->y = 900.0f ;
}

/****************************************************************/
/*			The screen's edges from the eye						*/
/****************************************************************/
void scr_edge_set( void )
{
	scr_work.scr_right = scr_work.eye[0].x + 320.0f ;
	scr_work.scr_left = scr_work.eye[0].x + -320.0f ;
	scr_work.center_y = scr_work.eye[0].y + 98.4f ;
	scr_work.scr_top = scr_work.center_y + 240.0f ;
	scr_work.scr_bottom = scr_work.center_y + -240.0f ;
}

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "scr.c" End of File	*/
