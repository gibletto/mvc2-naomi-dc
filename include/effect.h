/* The effects and shots: the work of one, the common effects' routines, then the damage effects'
   (dmXX_YY), the stage's (setXX_YY) and each character's (plsXX_YY, shlXX_YY). player.h includes it. */
#ifndef EFFECT_H
#define EFFECT_H

/* An effect or shot is a PLAYER_WORK too, taken from the pool (work_get), of which it has only
   the part before act_st. Its master_w made it, routine_0 and routine_1 run it, type and sub_type
   hold its setter's choice and ef_no its module's number. A sprite effect keeps its own work where
   a model's fields would be (from obj_mtx or obj_flag, as each module lays it out), as tlocal is a
   player's. */

/* a sprite effect's own work, from obj_flag; most modules view it as their own struct */
typedef struct EF_LOCAL_W {
	Sint32	work[4];
} EF_LOCAL_W;
#define EF_WK( w )	( ( EF_LOCAL_W * )&( w )->obj_flag )

/* hit_mark.c defines it with no an int; its callers were built against a prototype with no a
   Uint32 (shl1c_02's zero stores show it) */
#ifdef HIT_MARK_SET_NEW
extern void hit_mark_set( PLAYER_WORK *pl_w, VEC3 *ofs, int no );
#else
extern void hit_mark_set( PLAYER_WORK *pl_w, VEC3 *ofs, Uint32 no );
#endif
extern void nage_hit_set( PLAYER_WORK *pl_w, VEC3 *ofs );
extern void pl_shadow_set( PLAYER_WORK *pl_w );
extern void hit_mark_set1( PLAYER_WORK *pl_w, VEC3 *ofs, int no );
extern void catch_hit_mark_set( PLAYER_WORK *pl_w, VEC3 *ofs );
extern void land_dust_set( VEC3 *pos, int no );
extern void burst_none( VEC3 *pos, int no );
extern void pl_shadow_set( PLAYER_WORK *w );
extern void halo_ef_set( PLAYER_WORK *pl_w );
extern void hitmark_set( VEC3 *pos, Uint16 dir, int a, Uint8 b );
extern void burst_set( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 n, Uint16 flags );
extern void land_ef_set( VEC3 *pos, Sint8 no );
extern void dust_ofs_set( PLAYER_WORK *pl_w, VEC3 *ofs, Float a, Float b );
extern void dash_ef_set( PLAYER_WORK *pl_w );
extern void thunder_spark_set( PLAYER_WORK *pl_w );
extern void wall_dust_set( PLAYER_WORK *pl_w );
extern void Timer_Gauge_Set( PLAYER_WORK *pl_w, Sint16 *timer, int no );	/* the gauge of a timed power-up; it keeps the pointer in the effect */
extern void hit_burst_set( VEC3 *pos, int no, Uint8 kind );
extern void pl_shadow_set( PLAYER_WORK *w );
extern void puff_ef_set( PLAYER_WORK *pl_w, int no );
extern void hitmark_set1( VEC3 *pos, Uint16 dir, int a, Uint8 b );
extern void one_spark_set( VEC3 *pos, Uint8 dir );
extern void dust_set( PLAYER_WORK *pl_w, Uint8 x, Uint8 y, Float a, Float b );
extern void nage_nuke( VEC3 *pos );
extern void shock_set( VEC3 *pos, int no );

/* a point on an effect's curve: the frame it is reached at and its value; get_motion gives the value
   between points for a frame and steps the point number on (hit_mark.c) */
typedef struct MOTION {
	Sint32	frame;
	Float	val;
} MOTION;
extern Float get_motion( MOTION *tbl, Uint8 *no, Sint16 frame );

extern void guard_wave_set( VEC3 *pos, int dir );
extern PLAYER_WORK *hyper_ef_set( PLAYER_WORK *pl_w, Uint8 no );
extern void team_mark_set( PLAYER_WORK *pl_w );
extern void clash_ef_set( VEC3 *pos, int dir );

extern PLAYER_WORK *dm00_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *dm00_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *dm00_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *dm00_04_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *dm00_05_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *dm00_09_set( PLAYER_WORK *pl_w );
extern void dm00_0a_set( PLAYER_WORK *pl_w );

extern void set08_00_cross_get( Float *x, Float *y, int no );

extern PLAYER_WORK *pls00_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls00_02_set( PLAYER_WORK *pl_w );
extern void *shl00_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl00_01_set( PLAYER_WORK *pl_w, Uint8 no, Float x, Float y );

/* s_pl01 */
extern void *shl01_00_set( PLAYER_WORK *pl_w );
extern void *shl01_01_set( PLAYER_WORK *pl_w );

extern void *shl02_00_set( PLAYER_WORK *pl_w );
extern void *shl02_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl02_02_set( PLAYER_WORK *pl_w );
extern void *shl02_03_set( PLAYER_WORK *pl_w );
extern void *shl02_06_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *pls03_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls03_00_set2( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls03_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls03_03_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *pls03_04_double_set( PLAYER_WORK *w, Uint8 no );
extern PLAYER_WORK *pls03_04_set( PLAYER_WORK *pl_w, Uint8 type );
extern int shl03_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern int shl03_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl03_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl03_03_set( PLAYER_WORK *pl_w, Uint8 type );

extern const Uint16 pl04_coffin_x_tbl[];
extern const Uint16 pl04_coffins_x_tbl[];
extern PLAYER_WORK *pls04_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir );
extern PLAYER_WORK *pls04_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir );
extern PLAYER_WORK *pls04_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl04_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl04_01_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl04_02_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl04_03_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern void shl04_04_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl04_05_set( PLAYER_WORK *pl_w, Uint8 type );

extern PLAYER_WORK *pls05_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls05_02_set( PLAYER_WORK *pl_w, Uint8 flicker, Sint16 time );
extern void *shl05_00_set( PLAYER_WORK *pl_w );
extern void *shl05_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl05_02_legion_set( PLAYER_WORK *pl_w );
extern void *shl05_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl05_04_set( PLAYER_WORK *pl_w );
extern void *shl05_05_set( PLAYER_WORK *pl_w, int no );

/* s_pl06 */
extern PLAYER_WORK *pls06_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *pls06_01_set( PLAYER_WORK *pl_w, Sint8 no );
extern void *shl06_00_set( PLAYER_WORK *pl_w, Sint8 no );
extern void shl06_01_flash( PLAYER_WORK *ew, Sint8 *timer, Sint8 *step );
extern void *shl06_01_set( PLAYER_WORK *pl_w, Sint8 big, Sint8 wait, Sint8 alt );
extern void *shl06_02_set( PLAYER_WORK *pl_w, Sint8 no );

extern PLAYER_WORK *pls07_00_set( PLAYER_WORK *pl_w, int no );
extern int shl07_00_set( PLAYER_WORK *pl_w, int no );
extern int shl07_01_set( PLAYER_WORK *pl_w );
extern int shl07_02_set( PLAYER_WORK *pl_w );

extern const Sint8 pl08_spin_atck_tbl[];	/* Psylocke's sp_01: two attacks for each level */
extern const Float pl08_spin_spd[][4];	/* and its speeds */
extern const Uint8 pl08_thrust_atck_tbl[];	/* Psylocke's sp_03, by direction: the attack */
extern const int pl08_thrust_char_tbl[];	/* the animation */
extern const Uint8 pl08_thrust_dir_tbl[];
extern const Uint8 pl08_thrust_ang_tbl[];	/* the angle */
extern const Uint8 pl08_ninjitsu_lv_tbl[];	/* Psylocke's sp_04: the other copies' levels */
extern const int pl08_ninjitsu_char_tbl[];	/* and by direction: the animation */
extern const Uint8 pl08_ninjitsu_atck_tbl[];	/* the attack */
extern void *pls08_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl08_00_set( PLAYER_WORK *pl_w, Sint8 no );
extern void *shl08_01_set( PLAYER_WORK *pl_w, Sint8 no );

extern PLAYER_WORK *pls09_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls09_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls09_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls09_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls09_04_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls09_05_set( PLAYER_WORK *pl_w );
extern void *shl09_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl09_01_set( PLAYER_WORK *pl_w );
extern void *shl09_02_set( PLAYER_WORK *pl_w, Sint8 no );
extern void *shl09_03_set( PLAYER_WORK *pl_w, Sint8 no );
extern void *shl09_04_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls0a_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls0a_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls0a_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls0a_02_set2( PLAYER_WORK *pl_w, PLAYER_WORK *w );
extern PLAYER_WORK *pls0a_03_set( PLAYER_WORK *pl_w, int a, int no );
extern void *shl0a_00_set( PLAYER_WORK *pl_w, Float x, Float y, Uint8 no );

/* s_pl0b */
extern PLAYER_WORK *pls0b_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *pls0b_00_set3( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *pls0b_00_set2( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *pls0b_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *pls0b_01_set2( PLAYER_WORK *ew, Uint8 a, Uint8 b );
extern int shl0b_01_set( PLAYER_WORK *sw );
extern void *shl0b_01_set2( PLAYER_WORK *sw );

extern const Sint32 pl0c_sting_spd_tbl[][2][4];	/* speeds in 16.16, for Spider-Man's sp_00 */
extern int pls0c_02_spin( PLAYER_WORK *ew, PLAYER_WORK *pl_w );
extern int pls0c_02_key_next( PLAYER_WORK *ew );
extern Uint8 pls0c_02_char_tbl[16];	/* the animation of each link, by its place */
extern void *pls0c_03_set( PLAYER_WORK *pl_w, int no );
extern int pls0c_04_set( PLAYER_WORK *pl_w, int no, int type ) ;
extern int pls0c_05_set( PLAYER_WORK *pl_w, int no, int type ) ;
extern int pls0c_06_set( PLAYER_WORK *pl_w, int no, int type ) ;
extern void *shl0c_00_set( PLAYER_WORK *pl_w, int air );
extern void *shl0c_01_set( PLAYER_WORK *pl_w );
extern void *shl0c_02_set( PLAYER_WORK *pl_w );

/* s_pl0d */
extern PLAYER_WORK *pls0d_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern void *shl0d_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0d_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0d_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );

extern PLAYER_WORK *pls0e_00_set( PLAYER_WORK *pl_w, int type, int no );
extern int pls0e_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls0e_02_set( PLAYER_WORK *pl_w, int no );
extern int pls0e_03_set( PLAYER_WORK *pl_w, int n, int type, int no );
extern PLAYER_WORK *pls0e_04_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, int type );
extern int pls0e_05_set( PLAYER_WORK *pl_w, int no, int type ) ;
extern PLAYER_WORK *pls0e_06_set( PLAYER_WORK *pl_w, int no );
extern void *shl0e_00_set( PLAYER_WORK *wk, PLAYER_WORK *pl_w );
extern int shl0e_01_set( PLAYER_WORK *pl_w );
extern void *shl0e_02_set( PLAYER_WORK *wk, int no, PLAYER_WORK *pl_w );
extern void *shl0e_03_set( PLAYER_WORK *pl_w );

/* s_pl0f */
extern PLAYER_WORK *pls0f_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern void *shl0f_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0f_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0f_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0f_03_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0f_04_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl0f_05_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );

extern PLAYER_WORK *pls10_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls10_00_lunch_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls10_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls10_04_set( PLAYER_WORK *pl_w, Uint8 no );
extern void pls10_05_burst_set( VEC3 *pos, Uint8 dir, Sint8 no );
extern PLAYER_WORK *pls10_07_set( PLAYER_WORK *pl_w, PLAYER_WORK *ew, Uint8 no );
extern void *shl10_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl10_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl10_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl10_03_set( PLAYER_WORK *pl_w, Sint8 no, Uint8 cnt );
extern void *shl10_04_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *pls11_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *pls11_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *shl11_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl11_01_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl11_02_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *shl11_03_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls12_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl12_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl12_00_set2( PLAYER_WORK *pl_w, Uint8 no, Uint8 a, Uint8 b );
extern void *shl12_00_set3( PLAYER_WORK *pl_w, Uint8 no );

/* s_pl13 */
extern PLAYER_WORK *pls13_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls13_00_set2( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl13_00_set( PLAYER_WORK *pl_w, Uint8 no, int a, int b );	/* the routine reads only no */
extern void pl_shadow_set( PLAYER_WORK *ew );	/* shl13_00's kind 15 calls it once it is set */

/* s_pl14 */
/* shl14_00 to shl16_03: SonSon's, Amingo's and Marrow's shots and effects */
extern PLAYER_WORK *pls14_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );	/* an effect, moved like a player */
extern PLAYER_WORK *pls14_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl14_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl14_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl14_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl14_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl14_04_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl14_05_set( PLAYER_WORK *pl_w, PLAYER_WORK *hw );
extern void *shl14_06_set( PLAYER_WORK *pl_w, PLAYER_WORK *hw, Uint8 no );
extern void pl_shadow_set( PLAYER_WORK *w );	/* an effect set on a shot (shl14_01) */

extern PLAYER_WORK *pls15_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls15_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls15_01_ef_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls15_02_set( PLAYER_WORK *pl_w );
extern void *shl15_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl15_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl15_02_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *pls16_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls16_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls16_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls16_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls16_04_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls16_05_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls16_06_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls16_07_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls16_08_set( PLAYER_WORK *pl_w );
extern void *shl16_00_set( PLAYER_WORK *pl_w );
extern void *shl16_01_set( PLAYER_WORK *pl_w );
extern void *shl16_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl16_03_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls17_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_00_set2( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_00_set3( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_04_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_04_set2( PLAYER_WORK *ef_w, PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_05_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls17_06_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl17_00_set( PLAYER_WORK *pl_w );
extern void *shl17_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl17_02_set( PLAYER_WORK *pl_w );
extern void *shl17_03_set( PLAYER_WORK *pl_w );
extern void *shl17_04_set( PLAYER_WORK *pl_w );
extern void *shl17_05_set( PLAYER_WORK *pl_w );
extern void *shl17_06_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *pls18_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls18_00_idle_set( PLAYER_WORK *pl_w, PLAYER_WORK *ef_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *pls18_00_pillar_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls18_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl18_00_set_all( PLAYER_WORK *pl_w, Uint8 no, Uint8 pos );
extern void *shl18_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl18_02_set( PLAYER_WORK *pl_w, PLAYER_WORK *em, Uint8 level );
extern void *shl18_03_set( PLAYER_WORK *pl_w, Uint8 no, Float x, Float y );
extern void shl18_04_set_all( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 no );

extern PLAYER_WORK *pls19_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls19_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl19_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 no );
extern PLAYER_WORK *shl19_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl19_02_set( PLAYER_WORK *pl_w, Uint8 a, Sint8 no );
extern PLAYER_WORK *shl19_03_set( PLAYER_WORK *pl_w, Sint8 a );

/* shl1a to shl1c: the shots of Abyss, Chun-Li and Mega Man */
extern PLAYER_WORK *pls1a_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1a_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern void *shl1a_00_pos_set( PLAYER_WORK *pl_w, VEC3 *pos, Uint8 type, Uint8 no );
extern void *shl1a_00_set2( PLAYER_WORK *hw, Uint8 type );
extern void *shl1a_01_set( PLAYER_WORK *pl_w );
extern void shl1a_02_appear_set( PLAYER_WORK *pl_w );
extern void shl1a_02_set( PLAYER_WORK *pl_w );

extern void *shl1b_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl1b_00_set2( PLAYER_WORK *hw, Uint8 type, Uint8 n );

/* s_pl1c */
extern PLAYER_WORK *pls1c_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls1c_00_set2( PLAYER_WORK *pl_w, PLAYER_WORK *ef_w, Uint8 type );
extern PLAYER_WORK *pls1c_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls1c_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls1c_04_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls1c_05_set( PLAYER_WORK *pl_w, Uint8 no );
extern void pls1c_06_start( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls1c_07_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls1c_07_set2( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1c_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1c_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl1c_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl1c_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl1c_04_set( PLAYER_WORK *pl_w );
extern void *shl1c_05_set( PLAYER_WORK *pl_w );
extern void *shl1c_06_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1c_07_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1c_08_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1c_09_set( PLAYER_WORK *pl_w, Uint8 no );

/* s_pl1d */
extern void *shl1d_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1d_05_set( PLAYER_WORK *pl_w );
extern void *shl1d_06_set( PLAYER_WORK *pl_w, Uint8 no );

/* s_pl1e */
extern PLAYER_WORK *pls1e_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls1e_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls1e_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls1e_04_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1e_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl1e_01_set( PLAYER_WORK *pl_w, Uint8 no, Float x, Float y );
extern void *shl1e_02_set( PLAYER_WORK *pl_w );

/* s_pl1f */
extern PLAYER_WORK *pls1f_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir );
extern PLAYER_WORK *pls1f_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir );
extern PLAYER_WORK *pls1f_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls1f_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls1f_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl1f_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl1f_00_spark_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 dir );
extern PLAYER_WORK *shl1f_01_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl1f_02_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl1f_03_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *shl1f_03_set( PLAYER_WORK *pl_w, Uint8 type );

/* s_pl20 */
extern PLAYER_WORK *pls20_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl20_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl20_01_set( PLAYER_WORK *pl_w, Uint8 type );

extern PLAYER_WORK *shl21_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );

extern PLAYER_WORK *pls22_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls22_00_set3( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl22_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl22_00_set_ex( PLAYER_WORK *pl_w, Uint8 no, Uint8 arg );
extern PLAYER_WORK *shl22_00_set2( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl22_01_set( PLAYER_WORK *pl_w, Float x, Float y );

/* s_pl23 */
extern PLAYER_WORK *pls23_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl23_00_set();	/* ( PLAYER_WORK *pl_w, Uint8 type, Uint8 no ), the callers pass two */

/* s_pl24 */
extern PLAYER_WORK *pls24_00_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *shl25_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl25_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl25_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl25_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl25_04_set( PLAYER_WORK *pl_w, Sint8 no, Sint8 flame_no );

/* s_pl26 */
extern void *pls26_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *pls26_00_warp_set( PLAYER_WORK *pl_w );
extern void *pls26_00_crusher_flash_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl26_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl26_00_field_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl26_01_set( PLAYER_WORK *pl_w );
extern void *shl26_02_set( PLAYER_WORK *pl_w, Uint8 no );

/* s_pl27 */
extern PLAYER_WORK *pls27_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls27_02_set( PLAYER_WORK *pl_w, Uint8 no );
extern void *shl27_00_set( PLAYER_WORK *pl_w, Uint8 no );

extern void pls28_00_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *pls29_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl29_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );

extern PLAYER_WORK *pls2a_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls2a_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *pls2a_04_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2a_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2a_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2a_01_set0( PLAYER_WORK *w, Uint16 y );
extern PLAYER_WORK *shl2a_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2a_02_set0( PLAYER_WORK *w );
extern PLAYER_WORK *shl2a_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2a_04_set( PLAYER_WORK *pl_w, Uint8 no );

extern PLAYER_WORK *pls2b_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls2b_02_set( PLAYER_WORK *pl_w, int type, int kind );
extern PLAYER_WORK *pls2b_04_set( PLAYER_WORK *pl_w, int type, int kind );
extern PLAYER_WORK *pls2b_05_set( PLAYER_WORK *pl_w, int type );
extern PLAYER_WORK *pls2b_06_set( PLAYER_WORK *pl_w, int type );
extern PLAYER_WORK *shl2b_00_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *shl2b_01_set( PLAYER_WORK *ef_w, PLAYER_WORK *pl_w, int type, int no );
extern PLAYER_WORK *shl2b_02_set( PLAYER_WORK *pl_w, int no );

extern PLAYER_WORK *pls2c_01_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *pls2c_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2c_00_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl2c_01_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl2c_02_set( PLAYER_WORK *pl_w, Uint8 type );
extern PLAYER_WORK *shl2c_02_set2( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2c_03_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls2d_00_set( PLAYER_WORK *pl_w, int no );
extern int shl2d_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl2d_01_set( PLAYER_WORK *pl_w );
extern int shl2d_02_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls2e_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl2e_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl2e_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl2e_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl2e_03_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl2e_05_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl2e_06_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );

extern void *pls2f_00_set( PLAYER_WORK *pl_w, Sint8 no );
extern void *pls2f_01_set( PLAYER_WORK *pl_w );
extern void *pls2f_02_set( PLAYER_WORK *pl_w );
extern void *pls2f_03_set( PLAYER_WORK *pl_w );
extern void *pls2f_04_set( PLAYER_WORK *pl_w );
extern void *shl2f_00_set( PLAYER_WORK *pl_w, int no );
extern void *shl2f_01_set( PLAYER_WORK *pl_w );
extern void *shl2f_02_start( PLAYER_WORK *pl_w );
extern void *shl2f_03_set( PLAYER_WORK *pl_w, PLAYER_WORK *em );
extern void *shl2f_04_set( PLAYER_WORK *pl_w );
extern void *shl2f_05_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *shl30_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl30_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl30_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl30_03_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls31_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *pls31_00_set2( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl31_00_set( PLAYER_WORK *pl_w, int a, int no );	/* puts out a sword */
extern void *shl31_01_set( PLAYER_WORK *pl_w, int a, int no );	/* fires a sword */

extern PLAYER_WORK *pls32_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );

extern PLAYER_WORK *pls33_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl33_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl33_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl33_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl33_03_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl33_04_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern void *shl33_05_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );

extern PLAYER_WORK *pls34_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls34_00_set2( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls34_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl34_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl34_00_set_1( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl34_01_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl34_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl34_03_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *shl34_04_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls35_00_set( PLAYER_WORK *pl_w, Uint8 no );
extern PLAYER_WORK *pls35_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls35_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl35_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl35_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl35_02_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl35_03_start( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl35_04_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );

extern PLAYER_WORK *pls36_01_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *shl36_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl36_01_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl36_02_set( PLAYER_WORK *pl_w, Uint16 no );
extern PLAYER_WORK *shl36_02_set1( PLAYER_WORK *pl_w, Uint16 no );
extern PLAYER_WORK *shl36_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl36_04_set( PLAYER_WORK *pl_w );

extern PLAYER_WORK *pls37_00_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *pls37_00_set2( PLAYER_WORK *pl_w, Uint8 type, Uint8 no );
extern PLAYER_WORK *shl37_00_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl37_01_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl37_01_set2( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl37_02_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );
extern PLAYER_WORK *shl37_03_set( PLAYER_WORK *pl_w, Uint8 a, Uint8 b );

extern PLAYER_WORK *pls38_00_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls38_01_sword_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls38_02_set( PLAYER_WORK *pl_w, Sint16 no );
extern PLAYER_WORK *Shl38_set( PLAYER_WORK *pl_w, Uint8 type, Uint8 b );
extern PLAYER_WORK *Shl38_set2( PLAYER_WORK *w, Uint8 type, Uint8 b );
extern PLAYER_WORK *Shl38_01_set( PLAYER_WORK *pl_w, int a, int b );
extern PLAYER_WORK *Shl38_02_set( PLAYER_WORK *pl_w );

extern void *pls3a_00_set( PLAYER_WORK *pl_w, int no );
extern void *pls3a_01_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *pls3a_02_set( PLAYER_WORK *pl_w, Uint32 no );
extern PLAYER_WORK *pls3a_03_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *pls3a_03_set2( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl3a_00_set( PLAYER_WORK *pl_w, int type, int time );
extern PLAYER_WORK *shl3a_01_line_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl3a_01_piece_set( PLAYER_WORK *pl_w, int no );
extern PLAYER_WORK *shl3a_01_burst_set( PLAYER_WORK *pl_w );
extern PLAYER_WORK *shl3a_01_set( PLAYER_WORK *pl_w, int type );
extern PLAYER_WORK *shl3a_02_set( PLAYER_WORK *pl_w, int type );
extern PLAYER_WORK *shl3a_03_set( PLAYER_WORK *pl_w, int type );
extern PLAYER_WORK *shl3a_04_set( PLAYER_WORK *pl_w, int no );

#endif
