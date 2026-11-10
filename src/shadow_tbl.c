/* The tables of the fighters' shadows: two sizes for each step, and the offsets, by cg_shadow's
   low bits. A file of data alone, linked after the stage objects (its data lies between
   set08_0b's and vms's). */
#include "player.h"

/* each character's shadow sizes, two for each step, and its offsets, by cg_shadow's low 5 bits */
static Float shadow_size_00[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_01[16] = {
	9.5f, 15.0f, 9.0f, 15.0f, 9.5f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_02[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_03[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_04[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 3.5f, 8.0f
} ;

static Float shadow_size_05[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_06[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_07[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 10.0f, 12.0f, 7.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_08[16] = {
	8.0f, 9.5f, 9.0f, 9.5f, 10.0f, 9.5f, 11.0f, 9.5f,
	12.5f, 9.5f, 14.0f, 9.5f, 15.5f, 9.5f, 17.0f, 9.5f
} ;

static Float shadow_size_09[16] = {
	8.0f, 9.0f, 9.0f, 9.0f, 10.0f, 9.0f, 11.0f, 9.0f,
	12.5f, 9.0f, 14.0f, 9.0f, 15.5f, 9.0f, 17.0f, 9.0f
} ;

static Float shadow_size_0a[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 7.0f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_0b[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_0c[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_0d[16] = {
	11.5f, 15.0f, 9.0f, 15.0f, 9.5f, 15.0f, 10.0f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_0e[16] = {
	9.5f, 15.0f, 9.0f, 15.0f, 10.0f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_0f[16] = {
	11.5f, 13.5f, 9.0f, 13.5f, 10.0f, 13.5f, 8.5f, 13.5f,
	13.0f, 13.5f, 14.5f, 13.5f, 16.0f, 13.5f, 17.5f, 13.5f
} ;

static Float shadow_size_10[19] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 3.5f, 8.0f,
	0.0f, 0.0f, 0.0f
} ;

static Float shadow_size_14[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 11.0f, 15.5f, 14.0f, 15.5f
} ;

static Float shadow_size_15[18] = {
	8.75f, 12.0f, DOT_X( 4.125f ), 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f,
	0.0f, 0.0f
} ;

static Float shadow_size_18[18] = {
	12.5f, 12.0f, 11.0f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f,
	16.0f, 12.0f, 17.0f, 12.0f, 18.0f, 12.0f, 19.0f, 12.0f,
	0.0f, 0.0f
} ;

static Float shadow_size_1b[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_1c[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f, 3.5f, 8.0f
} ;

static Float shadow_size_1d[16] = {
	5.5f, 12.0f, 7.0f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 3.5f, 8.0f
} ;

static Float shadow_size_1e[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_1f[17] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 3.5f, 8.0f,
	0.0f
} ;

static Float shadow_size_21[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_22[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_23[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_24[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_25[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_26[16] = {
	10.0f, 15.0f, 9.0f, 15.0f, 9.5f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_27[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_28[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 10.0f, 12.0f, 7.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_29[16] = {
	12.5f, 15.0f, 11.5f, 15.0f, 14.0f, 15.0f, 9.5f, 15.0f,
	10.0f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_2a[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_2b[16] = {
	9.5f, 15.0f, 9.0f, 15.0f, 10.0f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_2c[16] = {
	7.0f, 12.0f, 5.5f, 12.0f, 8.5f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_2d[16] = {
	9.5f, 15.0f, 9.0f, 15.0f, 10.0f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_2e[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 7.0f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_2f[16] = {
	8.5f, 12.5f, 9.5f, 12.5f, 10.5f, 12.5f, 11.5f, 12.5f,
	12.5f, 12.5f, 13.5f, 12.5f, 14.5f, 12.5f, 15.5f, 12.5f
} ;

static Float shadow_size_30[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 7.0f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_31[16] = {
	8.0f, 9.0f, 9.0f, 9.0f, 10.0f, 9.0f, 11.0f, 9.0f,
	12.5f, 9.0f, 14.0f, 9.0f, 15.5f, 9.0f, 17.0f, 9.0f
} ;

static Float shadow_size_32[16] = {
	9.5f, 15.0f, 9.0f, 15.0f, 9.5f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_33[16] = {
	9.0f, 9.5f, 11.0f, 9.5f, 10.0f, 9.5f, 8.0f, 9.5f,
	12.5f, 9.5f, 14.0f, 9.5f, 15.5f, 9.5f, 17.0f, 9.5f
} ;

static Float shadow_size_34[16] = {
	8.5f, 12.5f, 9.5f, 12.5f, 10.5f, 12.5f, 11.5f, 12.5f,
	12.5f, 12.5f, 13.5f, 12.5f, 14.5f, 12.5f, 15.5f, 12.5f
} ;

static Float shadow_size_35[16] = {
	9.5f, 15.0f, 9.0f, 15.0f, 9.5f, 15.0f, 11.5f, 15.0f,
	12.5f, 15.0f, 14.0f, 15.0f, 15.0f, 15.0f, 16.5f, 15.0f
} ;

static Float shadow_size_36[16] = {
	11.0f, 15.5f, 10.5f, 15.5f, 9.0f, 15.5f, 12.5f, 15.5f,
	14.0f, 15.5f, 15.0f, 15.5f, 17.0f, 15.5f, 18.5f, 15.5f
} ;

static Float shadow_size_37[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 7.0f, 12.0f, 10.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_38[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 10.0f, 12.0f, 7.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_39[16] = {
	8.5f, 12.0f, 5.5f, 12.0f, 10.0f, 12.0f, 7.0f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

static Float shadow_size_3a[16] = {
	3.5f, 8.0f, 5.5f, 12.0f, 7.0f, 12.0f, 8.5f, 12.0f,
	11.0f, 12.0f, 12.5f, 12.0f, 14.0f, 12.0f, 15.0f, 12.0f
} ;

Float *shadow_size_tbl[59] = {
	shadow_size_00, shadow_size_01, shadow_size_02, shadow_size_03, shadow_size_04, shadow_size_05,
	shadow_size_06, shadow_size_07, shadow_size_08, shadow_size_09, shadow_size_0a, shadow_size_0b,
	shadow_size_0c, shadow_size_0d, shadow_size_0e, shadow_size_0f, shadow_size_10, shadow_size_00,
	shadow_size_00, shadow_size_00, shadow_size_14, shadow_size_15, shadow_size_00, shadow_size_00,
	shadow_size_18, shadow_size_00, shadow_size_00, shadow_size_1b, shadow_size_1c, shadow_size_1d,
	shadow_size_1e, shadow_size_1f, shadow_size_00, shadow_size_21, shadow_size_22, shadow_size_23,
	shadow_size_24, shadow_size_25, shadow_size_26, shadow_size_27, shadow_size_28, shadow_size_29,
	shadow_size_2a, shadow_size_2b, shadow_size_2c, shadow_size_2d, shadow_size_2e, shadow_size_2f,
	shadow_size_30, shadow_size_31, shadow_size_32, shadow_size_33, shadow_size_34, shadow_size_35,
	shadow_size_36, shadow_size_37, shadow_size_38, shadow_size_39, shadow_size_3a
} ;

static Sint16 shadow_ofs_00[32] = {
	0, -45, -42, -39, -36, -33, -30, -27,
	-24, -21, -18, -15, -12, -9, -6, -3,
	-48, 3, 6, 9, 12, 15, 18, 21,
	24, 27, 30, 33, 36, 39, 42, 45
} ;

static Sint16 shadow_ofs_09[32] = {
	0, -45, -42, -39, -36, -33, -30, -27,
	-24, -21, -18, -15, -12, -9, -6, -3,
	-48, 3, 6, 9, 12, 15, 18, 21,
	24, 27, 30, 33, 36, 39, 42, 45
} ;

static Sint16 shadow_ofs_2f[32] = {
	-48, -45, -42, -39, -36, -33, -30, -27,
	-24, -21, -18, -15, -12, -9, -6, -3,
	0, 3, 6, 9, 12, 15, 18, 21,
	24, 27, 30, 33, 36, 39, 42, 45
} ;

static Sint16 shadow_ofs_33[32] = {
	3, -45, -42, -39, -36, -33, -30, -27,
	-24, -21, -18, -15, -12, -9, -6, -3,
	-48, 0, 6, 9, 12, 15, 18, 21,
	24, 27, 30, 33, 36, 39, 42, 45
} ;

static Sint16 shadow_ofs_02[32] = {
	0, -45, -42, -39, -36, -33, -30, -27,
	-24, -21, -18, -15, -12, -9, -6, -3,
	-48, 3, 6, 9, 12, 15, 18, 21,
	24, 27, 30, 33, 36, 39, 42, 45
} ;

Sint16 *shadow_ofs_tbl[59] = {
	shadow_ofs_00, shadow_ofs_00, shadow_ofs_02, shadow_ofs_00, shadow_ofs_02, shadow_ofs_00,
	shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_09, shadow_ofs_00, shadow_ofs_00,
	shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_02, shadow_ofs_02,
	shadow_ofs_02, shadow_ofs_02, shadow_ofs_02, shadow_ofs_02, shadow_ofs_02, shadow_ofs_02,
	shadow_ofs_02, shadow_ofs_02, shadow_ofs_02, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00,
	shadow_ofs_00, shadow_ofs_02, shadow_ofs_02, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00,
	shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00,
	shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_2f,
	shadow_ofs_00, shadow_ofs_09, shadow_ofs_09, shadow_ofs_33, shadow_ofs_2f, shadow_ofs_00,
	shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_00, shadow_ofs_02
} ;

/*==============================================================*/
/*	End of File.												*/
/*==============================================================*/
/* "shadow_tbl.c" End of File	*/
