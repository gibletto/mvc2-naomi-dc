/* Amingo (character 15): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_15      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_15     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl15.c) says which group
                   and number each move uses.
   pl15_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl15_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl15_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl15_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_15
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_15, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_15 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl15_stand_p_00[], pl15_stand_p_01[], pl15_stand_p_02[], pl15_stand_k_00[],
	pl15_stand_k_01[], pl15_stand_k_02[], pl15_crouch_p_00[], pl15_crouch_p_01[], pl15_crouch_p_02[],
	pl15_crouch_k_00[], pl15_crouch_k_01[], pl15_crouch_k_02[], pl15_jump_p_00[], pl15_jump_p_01[],
	pl15_jump_p_02[], pl15_jump_k_00[], pl15_jump_k_01[], pl15_unused_00[], pl15_jump_k_02[],
	pl15_appear_00[], pl15_win_00[], pl15_win_00_loop[], pl15_win_01[], pl15_win_01_loop[],
	pl15_win_02[], pl15_win_02_loop[], pl15_stand_00[], pl15_stand_02[], pl15_stand_03[],
	pl15_jump_00[], pl15_jump_00_loop[], pl15_jump_01[], pl15_jump_01_loop[], pl15_jump_02[],
	pl15_jump_02_loop[], pl15_jump_03[], pl15_jump_03_loop[], pl15_jump_06[], pl15_jump_09[],
	pl15_dash_00[], pl15_dash_00_loop[], pl15_dash_02[], pl15_dash_02_loop[], pl15_dash_01[],
	pl15_dash_01_loop[], pl15_dash_03[], pl15_crouch_00[], pl15_crouch_01[], pl15_crouch_02[],
	pl15_turn_00[], pl15_turn_01[], pl15_guard_00[], pl15_guard_01[], pl15_guard_02[],
	pl15_guard_04[], pl15_guard_05[], pl15_guard_06[], pl15_air_guard_00[], pl15_air_guard_01[],
	pl15_air_guard_03[], pl15_get_up_00[], pl15_get_up_01[], pl15_get_up_02[], pl15_get_up_03[],
	pl15_damage_13[], pl15_damage_13_loop[], pl15_caught_12[], pl15_damage_27[], pl15_damage_33[],
	pl15_dizzy_00[], pl15_catch_00[], pl15_catch_02[], pl15_catch_03[], pl15_damage_00[],
	pl15_caught_00[], pl15_damage_01[], pl15_caught_01[], pl15_damage_02[], pl15_caught_02[],
	pl15_damage_03[], pl15_caught_03[], pl15_damage_04[], pl15_caught_04[], pl15_damage_05[],
	pl15_caught_05[], pl15_damage_06[], pl15_caught_06[], pl15_damage_07[], pl15_caught_07[],
	pl15_damage_08[], pl15_caught_08[], pl15_damage_10[], pl15_damage_11[], pl15_damage_12[],
	pl15_caught_09[], pl15_caught_10[], pl15_caught_11[], pl15_damage_09[], pl15_damage_09_loop[],
	pl15_damage_16[], pl15_damage_17[], pl15_damage_18[], pl15_damage_19[], pl15_damage_20[],
	pl15_damage_21[], pl15_damage_22[], pl15_damage_23[], pl15_damage_24[], pl15_damage_25[],
	pl15_caught_13[], pl15_caught_14[], pl15_caught_15[], pl15_caught_16[], pl15_caught_17[],
	pl15_caught_18[], pl15_caught_19[], pl15_caught_20[], pl15_caught_21[], pl15_damage_26[],
	pl15_damage_28[], pl15_damage_29[], pl15_damage_31[], pl15_extra_00[], pl15_unused_01[],
	pl15_change_00[], pl15_change_01[], pl15_change_02[], pl15_extra_01[], pl15_extra_01_loop[],
	pl15_change_03[], pl15_change_04[], pl15_change_04_loop[], pl15_extra_02[], pl15_extra_03[],
	pl15_extra_04[], pl15_special_10[], pl15_special_10_loop[], pl15_special_11[], pl15_special_12[],
	pl15_special_12_loop[], pl15_unused_02[], pl15_special_18[], pl15_special_18_loop[],
	pl15_special_15[], pl15_special_15_loop[], pl15_special_19[], pl15_special_19_loop[],
	pl15_special_02[], pl15_special_02_loop[], pl15_special_13[], pl15_special_16[],
	pl15_special_14[], pl15_special_17[], pl15_effect_00[], pl15_effect_00_loop[], pl15_effect_01[],
	pl15_effect_02[], pl15_effect_03[], pl15_effect_16[], pl15_special_00[], pl15_special_00_loop[],
	pl15_special_03[], pl15_special_06[], pl15_special_06_loop[], pl15_special_07[],
	pl15_special_07_loop[], pl15_special_08[], pl15_special_09[], pl15_super_00[], pl15_super_01[],
	pl15_super_01_loop[], pl15_super_02[], pl15_super_02_loop[], pl15_super_03[],
	pl15_super_03_loop[], pl15_super_04[], pl15_super_05[], pl15_effect_04[], pl15_effect_04_loop[],
	pl15_effect_05[], pl15_effect_07[], pl15_effect_07_loop[], pl15_effect_08[],
	pl15_effect_08_loop[], pl15_effect_09[], pl15_effect_09_loop[], pl15_catch_01[],
	pl15_effect_11[], pl15_effect_12[], pl15_effect_12_loop[], pl15_effect_13[],
	pl15_effect_13_loop[], pl15_effect_17[], pl15_effect_14[], pl15_effect_14_loop[],
	pl15_effect_15[], pl15_unused_03[], pl15_unused_04[], pl15_grp27_00[], pl15_grp27_01[],
	pl15_grp27_02[], pl15_grp27_03[], pl15_grp27_04[], pl15_grp27_05[], pl15_grp27_06[],
	pl15_grp27_07[], pl15_grp27_08[], pl15_grp27_09[], pl15_grp27_10[];
extern const Sint32 *const pl15_stand_tbl[], *const pl15_jump_tbl[], *const pl15_dash_tbl[],
	*const pl15_crouch_tbl[], *const pl15_turn_tbl[], *const pl15_guard_tbl[],
	*const pl15_air_guard_tbl[], *const pl15_stand_p_tbl[], *const pl15_stand_k_tbl[],
	*const pl15_crouch_p_tbl[], *const pl15_crouch_k_tbl[], *const pl15_jump_p_tbl[],
	*const pl15_jump_k_tbl[], *const pl15_damage_tbl[], *const pl15_dizzy_tbl[],
	*const pl15_catch_tbl[], *const pl15_caught_tbl[], *const pl15_get_up_tbl[],
	*const pl15_appear_tbl[], *const pl15_win_tbl[], *const pl15_extra_tbl[],
	*const pl15_special_tbl[], *const pl15_super_tbl[], *const pl15_effect_tbl[],
	*const pl15_change_tbl[], *const pl15_grp25_tbl[], *const pl15_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_15[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_15[] = {
	pl15_stand_tbl, pl15_jump_tbl, pl15_dash_tbl, pl15_crouch_tbl, pl15_turn_tbl, pl15_guard_tbl,
	pl15_air_guard_tbl, pl15_stand_p_tbl, pl15_stand_k_tbl, pl15_crouch_p_tbl, pl15_crouch_k_tbl, pl15_jump_p_tbl,
	pl15_jump_k_tbl, pl15_damage_tbl, pl15_dizzy_tbl, pl15_catch_tbl, pl15_caught_tbl, pl15_get_up_tbl,
	pl15_appear_tbl, pl15_win_tbl, pl15_extra_tbl, pl15_special_tbl, pl15_super_tbl, pl15_effect_tbl,
	pl15_change_tbl, pl15_grp25_tbl, pl15_grp25_tbl, pl15_grp27_tbl, 0,
};

const Sint32 *const pl15_stand_tbl[] = {
	pl15_stand_00, pl15_stand_00, pl15_stand_02, pl15_stand_03, pl15_stand_00, pl15_stand_00,
	pl15_stand_02, pl15_stand_03, pl15_stand_02, pl15_stand_03, 0,
};

const Sint32 *const pl15_jump_tbl[] = {
	pl15_jump_00, pl15_jump_01, pl15_jump_02, pl15_jump_03, pl15_jump_03, pl15_jump_03,
	pl15_jump_06, pl15_jump_06, pl15_jump_06, pl15_jump_09, pl15_jump_09, pl15_jump_09,
	0,
};

const Sint32 *const pl15_dash_tbl[] = {
	pl15_dash_00, pl15_dash_01, pl15_dash_02, pl15_dash_03, 0,
};

const Sint32 *const pl15_crouch_tbl[] = {
	pl15_crouch_00, pl15_crouch_01, pl15_crouch_02, 0,
};

const Sint32 *const pl15_turn_tbl[] = {
	pl15_turn_00, pl15_turn_01, 0,
};

const Sint32 *const pl15_guard_tbl[] = {
	pl15_guard_00, pl15_guard_01, pl15_guard_02, pl15_guard_01, pl15_guard_04, pl15_guard_05,
	pl15_guard_06, pl15_guard_05, 0,
};

const Sint32 *const pl15_air_guard_tbl[] = {
	pl15_air_guard_00, pl15_air_guard_01, pl15_air_guard_01, pl15_air_guard_03, 0,
};

const Sint32 *const pl15_stand_p_tbl[] = {
	pl15_stand_p_00, pl15_stand_p_01, pl15_stand_p_02, 0,
};

const Sint32 *const pl15_stand_k_tbl[] = {
	pl15_stand_k_00, pl15_stand_k_01, pl15_stand_k_02, 0,
};

const Sint32 *const pl15_crouch_p_tbl[] = {
	pl15_crouch_p_00, pl15_crouch_p_01, pl15_crouch_p_02, 0,
};

const Sint32 *const pl15_crouch_k_tbl[] = {
	pl15_crouch_k_00, pl15_crouch_k_01, pl15_crouch_k_02, 0,
};

const Sint32 *const pl15_jump_p_tbl[] = {
	pl15_jump_p_00, pl15_jump_p_01, pl15_jump_p_02, pl15_jump_p_00, pl15_jump_p_01, pl15_jump_p_02,
	0,
};

const Sint32 *const pl15_jump_k_tbl[] = {
	pl15_jump_k_00, pl15_jump_k_01, pl15_jump_k_02, pl15_jump_k_00, pl15_jump_k_01, pl15_jump_k_02,
	0,
};

const Sint32 *const pl15_damage_tbl[] = {
	pl15_damage_00, pl15_damage_01, pl15_damage_02, pl15_damage_03, pl15_damage_04, pl15_damage_05,
	pl15_damage_06, pl15_damage_07, pl15_damage_08, pl15_damage_09, pl15_damage_10, pl15_damage_11,
	pl15_damage_12, pl15_damage_13, pl15_damage_13, pl15_damage_13, pl15_damage_16, pl15_damage_17,
	pl15_damage_18, pl15_damage_19, pl15_damage_20, pl15_damage_21, pl15_damage_22, pl15_damage_23,
	pl15_damage_24, pl15_damage_25, pl15_damage_26, pl15_damage_27, pl15_damage_28, pl15_damage_29,
	pl15_damage_27, pl15_damage_31, pl15_damage_27, pl15_damage_33, pl15_damage_13, pl15_damage_26,
	0,
};

const Sint32 *const pl15_dizzy_tbl[] = {
	pl15_dizzy_00, 0,
};

const Sint32 *const pl15_catch_tbl[] = {
	pl15_catch_00, pl15_catch_01, pl15_catch_02, pl15_catch_03, 0,
};

const Sint32 *const pl15_caught_tbl[] = {
	pl15_caught_00, pl15_caught_01, pl15_caught_02, pl15_caught_03, pl15_caught_04, pl15_caught_05,
	pl15_caught_06, pl15_caught_07, pl15_caught_08, pl15_caught_09, pl15_caught_10, pl15_caught_11,
	pl15_caught_12, pl15_caught_13, pl15_caught_14, pl15_caught_15, pl15_caught_16, pl15_caught_17,
	pl15_caught_18, pl15_caught_19, pl15_caught_20, pl15_caught_21, 0,
};

const Sint32 *const pl15_get_up_tbl[] = {
	pl15_get_up_00, pl15_get_up_01, pl15_get_up_02, pl15_get_up_03, 0,
};

const Sint32 *const pl15_appear_tbl[] = {
	pl15_appear_00, 0,
};

const Sint32 *const pl15_win_tbl[] = {
	pl15_win_00, pl15_win_01, pl15_win_02, 0,
};

const Sint32 *const pl15_extra_tbl[] = {
	pl15_extra_00, pl15_extra_01, pl15_extra_02, pl15_extra_03, pl15_extra_04, 0,
};

const Sint32 *const pl15_special_tbl[] = {
	pl15_special_00, pl15_special_00, pl15_special_02, pl15_special_03, pl15_special_03, pl15_special_03,
	pl15_special_06, pl15_special_07, pl15_special_08, pl15_special_09, pl15_special_10, pl15_special_11,
	pl15_special_12, pl15_special_13, pl15_special_14, pl15_special_15, pl15_special_16, pl15_special_17,
	pl15_special_18, pl15_special_19, 0,
};

const Sint32 *const pl15_super_tbl[] = {
	pl15_super_00, pl15_super_01, pl15_super_02, pl15_super_03, pl15_super_04, pl15_super_05,
	0,
};

const Sint32 *const pl15_effect_tbl[] = {
	pl15_effect_00, pl15_effect_01, pl15_effect_02, pl15_effect_03, pl15_effect_04, pl15_effect_05,
	pl15_effect_05, pl15_effect_07, pl15_effect_08, pl15_effect_09, pl15_catch_01, pl15_effect_11,
	pl15_effect_12, pl15_effect_13, pl15_effect_14, pl15_effect_15, pl15_effect_16, pl15_effect_17,
	0,
};

const Sint32 *const pl15_change_tbl[] = {
	pl15_change_00, pl15_change_01, pl15_change_02, pl15_change_03, pl15_change_04, 0,
};

const Sint32 *const pl15_grp25_tbl[] = {
	pl15_win_00, 0,
};

const Sint32 *const pl15_grp27_tbl[] = {
	pl15_grp27_00, pl15_grp27_01, pl15_grp27_02, pl15_grp27_03, pl15_grp27_04, pl15_grp27_05,
	pl15_grp27_06, pl15_grp27_07, pl15_grp27_08, pl15_grp27_09, pl15_grp27_10, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand_P 0 (light) */
const Sint32 pl15_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   151, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,   2,           0,   152, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0, 0x81,   8,           0,   153, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl15_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   155, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0x6c,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   157, 0x80,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x81,    0,   8,           0,   158, 0x80,   0x81,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   159, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   160, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl15_stand_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   162, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   164, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   165, 0x80,   0xe1,  ATK,  0x01,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   167, 0x80,   0xe1,  ATK,  0x01,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x81,    0,   3,           0,   168, 0x80,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   173, 0x80,   0xe1,  ATK,  0x01,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x81,    0,   3,           0,   167, 0x80,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   3,           0,   168, 0x80,   0xe1,  ATK,  0x01,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x81,    0,   3,           0,   173, 0x80,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   172, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl15_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   175, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x81,    0,   6,           0,   176, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   177, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl15_stand_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   178, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x66,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   179, 0x80,   0x66,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   180, 0x80,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   8,           0,   181, 0x80,   0xc2,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   8,           0,   182, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   183, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl15_stand_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,    71, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   8,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    71, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl15_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   196, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl15_crouch_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   198, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   4,           0,   199, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x81,    0,   4,           0,   200, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl15_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   4,           0,   205, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x81,    0,   4,           0,   206, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   4,           0,   209, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x81,    0,   4,           0,   210, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   212, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl15_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   214, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   215, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   217, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl15_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   2,           0,   221, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   8,           0,   222, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    96, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl15_crouch_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   234, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x0b,  ATK,  0x00,       0x23,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   3,           0,   230, 0x80,   0x0b,  ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   3,           0,   231, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x83,    0,   3,           0,   232, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   3,           0,   230, 0x80,   0x0b,  ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   3,           0,   231, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x83,    0,   3,           0,   232, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x0b,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x0b,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x0b,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   236, 0x80,   0x0b,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl15_jump_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   3,           0,   238, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   7,           0,   239, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   240, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl15_jump_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   242, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   3,           0,   243, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x81,    0,   8,           0,   244, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   246, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl15_jump_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6,           0,   250, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   254, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl15_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   260, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   8,           0,   261, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   262, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl15_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl15_jump_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(   -8,    0,   2,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   2,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x83,    0,   2,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    0,   1,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   1,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x83,    0,   1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   272, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    81, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_k_01 ),
};

const Sint32 pl15_unused_00[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   2,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x83,    0,   2, END_OF_DATA,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	0x00000f50,
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl15_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   3,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   276, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x83,    0,   2,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   276, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x83,    0,   2,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   3,           0,   276, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   4,           0,   277, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x83,    0,   5,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_k_02 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl15_appear_00[] = {
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   333, 0x80,   0x00,    0,  0x81,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   332, 0x80,   0x00,    0,  0x80,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   139, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_appear_00 ),
};

/* Char_Win 0: win_ko, win_time
   group 25 0
   Char_Fly 0 */
const Sint32 pl15_win_00[] = {
	CG_FRAME(    0,    0,  10,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   126, 0x80,   0x8b,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_win_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   127, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_win_00_loop ),
};

/* Char_Win 1: win_ko */
const Sint32 pl15_win_01[] = {
	CG_FRAME(    0,    0,   5,           0,   332, 0x80,   0x00,    0,  0x80,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   333, 0x80,   0x00,    0,  0x81,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   333, 0x80,   0x00,    0,  0x81,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   147, 0x80,   0x00,    0,  0x92,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_win_01_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   148, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_win_01_loop ),
};

/* Char_Win 2: lose_set */
const Sint32 pl15_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_win_02_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_win_02_loop ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl15_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   Char_Stand 8 */
const Sint32 pl15_stand_02[] = {
	CG_FRAME(    0,    0,   5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   Char_Stand 9 */
const Sint32 pl15_stand_03[] = {
	CG_FRAME(    0,    0,   5,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl15_jump_00[] = {
	CG_FRAME(    0,    1,   2,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl15_jump_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl15_jump_01[] = {
	CG_FRAME(    0,    1,   2,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl15_jump_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl15_jump_02[] = {
	CG_FRAME(    0,    1,   2,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl15_jump_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl15_jump_03[] = {
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl15_jump_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_jump_03_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl15_jump_06[] = {
	CG_FRAME(    0,    1,   2,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl15_jump_09[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl15_dash_00[] = {
	CG_FRAME(    0,    1,   3,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl15_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl15_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl15_dash_02[] = {
	CG_FRAME(    0,    0,   6,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl15_dash_02_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl15_dash_01[] = {
	CG_FRAME(    0,    1,   3,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl15_dash_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl15_dash_01_loop ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl15_dash_03[] = {
	CG_FRAME(    0,    0,   6,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl15_crouch_00[] = {
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl15_crouch_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl15_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl15_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl15_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl15_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl15_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl15_guard_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl15_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl15_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl15_guard_06[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl15_air_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl15_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_air_guard_01 ),
};

/* Char_Air_Guard 3 */
const Sint32 pl15_air_guard_03[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_air_guard_03 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl15_get_up_00[] = {
	CG_FRAME(    0,    0,   8,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl15_get_up_01[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl15_get_up_02[] = {
	CG_FRAME(    0,    0,   6,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl15_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_get_up_03 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl15_damage_13[] = {
	CG_FRAME(    0,    0,   3,           0,   518, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl15_damage_13_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   520, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl15_damage_13_loop ),
};

/* Char_Caught 12 */
const Sint32 pl15_caught_12[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   518, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl15_caught_12 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl15_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_damage_27 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl15_damage_33[] = {
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl15_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set, jump_p_set */
const Sint32 pl15_catch_00[] = {
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    0,   5,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   5,           0,   281, 0x80,   0x00,    0,  0x87,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   5,           0,   282, 0x80,   0x00,    0,  0x88,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   5,           0,   283, 0x80,   0x00,    0,  0x89,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   5,           0,   284, 0x80,   0x00,    0,  0x8a,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   5,           0,   285, 0x80,   0x00,    0,  0x8b,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   5,           0,   286, 0x80,   0x00,    0,  0x8c,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    1,    0,   5,           0,   287, 0x80,   0x00,    0,  0x8d,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x00,    0,  0x8e,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x00,    0,  0x8f,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   5,           0,   286, 0x80,   0x00,    0,  0x8c,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    1,    0,   5,           0,   287, 0x80,   0x00,    0,  0x8d,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x00,    0,  0x8e,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x00,    0,  0x8f,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   5,           0,   286, 0x80,   0x00,    0,  0x8c,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    1,    0,   5,           0,   287, 0x80,   0x00,    0,  0x8d,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x00,    0,  0x8e,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x00,    0,  0x8f,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   5,           0,   285, 0x80,   0x00,    0,  0x8b,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   5,           0,   284, 0x80,   0x00,    0,  0x8a,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x00,    0,  0x89,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x00,    0,  0x88,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x00,    0,  0x87,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    1,   4,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_catch_00 ),
};

/* Char_Catch 2: shl15_02_reach, shl15_02_hold */
const Sint32 pl15_catch_02[] = {
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   1,           0,   459, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   1,           0,   460, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   1,           0,   461, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   4,           0,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_catch_02 ),
};

/* Char_Catch 3: okite_catch_pull (Shokubutsu no Okite (hyper)) */
const Sint32 pl15_catch_03[] = {
	CG_FRAME(    1,    0,   3,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    1,    0,   1,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    2,    0,   4,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   4,           0,   435, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,    0,  0x01,       0x27,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,    0,  0x00,       0x33,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x00,    0,  0x00,       0x26,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   444, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x00,    0,  0x00,       0x33,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,    0,  0x01,       0x27,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x00,    0,  0x00,       0x33,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   443, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   444, 0x80,   0x00,    0,  0x01,       0x26,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x00,    0,  0x00,       0x33,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,    0,  0x00,       0x27,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x00,    0,  0x00,       0x26,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   447, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,   3,           0,   447, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   449, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   450, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_catch_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl15_damage_00[] = {
	CG_FRAME(    0,    0,   8,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_damage_00 ),
};

/* Char_Caught 0 */
const Sint32 pl15_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl15_caught_00 ),
};

/* Char_Damage 1 */
const Sint32 pl15_damage_01[] = {
	CG_FRAME(    0,    0,   8,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_damage_01 ),
};

/* Char_Caught 1 */
const Sint32 pl15_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl15_caught_01 ),
};

/* Char_Damage 2 */
const Sint32 pl15_damage_02[] = {
	CG_FRAME(    0,    0,   6,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_damage_02 ),
};

/* Char_Caught 2 */
const Sint32 pl15_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl15_caught_02 ),
};

/* Char_Damage 3 */
const Sint32 pl15_damage_03[] = {
	CG_FRAME(    0,    0,   8,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_damage_03 ),
};

/* Char_Caught 3 */
const Sint32 pl15_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl15_caught_03 ),
};

/* Char_Damage 4 */
const Sint32 pl15_damage_04[] = {
	CG_FRAME(    0,    0,   6,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_damage_04 ),
};

/* Char_Caught 4 */
const Sint32 pl15_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl15_caught_04 ),
};

/* Char_Damage 5 */
const Sint32 pl15_damage_05[] = {
	CG_FRAME(    0,    0,  10,           0,   499, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_damage_05 ),
};

/* Char_Caught 5 */
const Sint32 pl15_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   499, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl15_caught_05 ),
};

/* Char_Damage 6 */
const Sint32 pl15_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_damage_06 ),
};

/* Char_Caught 6 */
const Sint32 pl15_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl15_caught_06 ),
};

/* Char_Damage 7 */
const Sint32 pl15_damage_07[] = {
	CG_FRAME(    0,    0,  10,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_damage_07 ),
};

/* Char_Caught 7 */
const Sint32 pl15_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl15_caught_07 ),
};

/* Char_Damage 8 */
const Sint32 pl15_damage_08[] = {
	CG_FRAME(    0,    0,  10,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   7,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_damage_08 ),
};

/* Char_Caught 8 */
const Sint32 pl15_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl15_caught_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl15_damage_10[] = {
	CG_FRAME(    0,    0,   5,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl15_damage_11[] = {
	CG_FRAME(    0,    0,   3,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl15_damage_12[] = {
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_12 ),
};

/* Char_Caught 9 */
const Sint32 pl15_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl15_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl15_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl15_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl15_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl15_caught_11 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl15_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   515, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl15_damage_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   520, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl15_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl15_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   511, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl15_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl15_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl15_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl15_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   515, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl15_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 22 */
const Sint32 pl15_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 23 */
const Sint32 pl15_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 24 */
const Sint32 pl15_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 25 */
const Sint32 pl15_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   515, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_damage_25 ),
};

/* Char_Caught 13 */
const Sint32 pl15_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl15_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl15_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl15_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl15_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   515, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl15_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   522, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl15_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   528, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl15_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl15_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl15_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   518, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl15_caught_21 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ...
   Char_Damage 35 */
const Sint32 pl15_damage_26[] = {
	CG_FRAME(    0,    0,   4,           0,   516, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   517, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   518, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   520, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl15_damage_26 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl15_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   522, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   523, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   524, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   525, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   526, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   527, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl15_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   528, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   530, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   531, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   533, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl15_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl15_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl15_damage_31 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl15_extra_00[] = {
	CG_FRAME( 0x80,    0,   4,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   4,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x83,    0,   4, END_OF_DATA,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl15_extra_00 ),
};

const Sint32 pl15_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    71, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000030a0,
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl15_change_00[] = {
	CG_FRAME(    0,    0,   8,           0,   125, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   126, 0x80,   0x8b,  ATK,  0x00,       0x0f,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   127, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   128, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl15_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl15_change_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   123, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl15_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl15_change_02[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl15_change_02 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl15_extra_01[] = {
	CG_FRAME(    0,    0,  10,           0,   139, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl15_extra_01_loop[] = {
	CG_FRAME(    0,    1,  10, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_extra_01_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl15_change_03[] = {
	CG_FRAME(    0,    0,   6,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,    0,  0x00,       0x32,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_change_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl15_change_04[] = {
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl15_change_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_change_04_loop ),
};

/* Char_Extra 2: chouhatsu_init (the taunt), fly_start5 */
const Sint32 pl15_extra_02[] = {
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_extra_02 ),
};

/* Char_Extra 3: pl15_upper, fly_start5 */
const Sint32 pl15_extra_03[] = {
	CG_FRAME(    0,    0,   6,           0,   198, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   4,           0,   199, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x81,    0,   4,           0,   200, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_extra_03 ),
};

/* Char_Extra 4: pl15_upper */
const Sint32 pl15_extra_04[] = {
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    96, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_extra_04 ),
};

/* Char_Special 10: pls15_00_t0_init, pls15_00_t3_init, midori_init (Midori no Kanki) */
const Sint32 pl15_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   332, 0x80,   0x00,  ATK,  0x80,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   333, 0x80,   0x00,  ATK,  0x81,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   334, 0x80,   0x00,  ATK,  0x03,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   2,           0,   337, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,   4,           0,   338, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   339, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_special_10_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   340, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_special_10_loop ),
};

/* Char_Special 11: pls15_00_t1_init, midori_land (Midori no Kanki) */
const Sint32 pl15_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   344, 0x80,   0x00,    0,  0x91,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,    0,  0x00,       0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   333, 0x80,   0x00,    0,  0x81,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   332, 0x80,   0x00,    0,  0x80,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_special_11 ),
};

/* Char_Special 12: midori_start (Midori no Kanki) */
const Sint32 pl15_special_12[] = {
	CG_FRAME(    0,    0,   2,           0,   354, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(  -48,    0,   4,           0,   351, 0x80,   0x00,  ATK,  0x01,       0x1f,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl15_special_12_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl15_special_12_loop ),
};

const Sint32 pl15_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4, END_OF_DATA,   352, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl15_special_12_loop ),
};

/* Char_Special 18: midori_rise (Midori no Kanki, 18 + atck_level) */
const Sint32 pl15_special_18[] = {
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,  ATK,  0x01,       0x1f,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  22 ),
};

const Sint32 pl15_special_18_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl15_special_18_loop ),
};

/* Char_Special 15: midori_start (Midori no Kanki) */
const Sint32 pl15_special_15[] = {
	CG_FRAME(    0,    0,   2,           0,   354, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,  ATK,  0x01,       0x1f,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl15_special_15_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl15_special_15_loop ),
};

/* Char_Special 19 */
const Sint32 pl15_special_19[] = {
	CG_FRAME(    0,    0,   2,           0,   351, 0x80,   0x00,  ATK,  0x02,       0x1f,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   352, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  22 ),
};

const Sint32 pl15_special_19_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl15_special_19_loop ),
};

/* Char_Special 2: midori_rise (Midori no Kanki) */
const Sint32 pl15_special_02[] = {
	CG_FRAME(  -59,    0,  48,           0,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
};

const Sint32 pl15_special_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl15_special_02_loop ),
};

/* Char_Special 13: pls15_00_t1_move */
const Sint32 pl15_special_13[] = {
	CG_FRAME(    0,    0,   5,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_special_13 ),
};

/* Char_Special 16: pls15_00_t0_init */
const Sint32 pl15_special_16[] = {
	CG_FRAME(    0,    0,  10,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6,           0,   344, 0x80,   0x00,    0,  0x91,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   5,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   346, 0x80,   0x00,    0,  0x00,       0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   333, 0x80,   0x00,    0,  0x81,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   332, 0x80,   0x00,    0,  0x80,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_special_16 ),
};

/* Char_Special 14: sp_bite (a bite (assist)) */
const Sint32 pl15_special_14[] = {
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x00,    0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   304, 0x80,   0x00,    0,  0x00,       0x2d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_special_14 ),
};

/* Char_Special 17: snapback_init (the snapback) */
const Sint32 pl15_special_17[] = {
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   4,           0,   164, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   5,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   5,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   5,           0,   168, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x83,    0,   3,           0,   173, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   5,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   5,           0,   168, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x83,    0,   3,           0,   173, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   172, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl15_special_17 ),
};

/* Char_Effect 0: shl15_00_init */
const Sint32 pl15_effect_00[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0, 32773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0, 32774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0, 32775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   1,           0, 32776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl15_effect_00_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA, 32777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl15_effect_00_loop ),
};

/* Char_Effect 1: shl15_00_head_init */
const Sint32 pl15_effect_01[] = {
	CG_FRAME(    0,    0,   4,           0, 32779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0, 32780, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32781, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl15_effect_01 ),
};

/* Char_Effect 2: shl15_00_back */
const Sint32 pl15_effect_02[] = {
	CG_FRAME(    0,    9,   5,           0, 32788, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 32789, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 32790, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 32791, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 32792, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32793, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_02 ),
};

/* Char_Effect 3: shl15_00_ck, shl15_00_stay */
const Sint32 pl15_effect_03[] = {
	CG_FRAME(    0,    0,   5,           0, 32783, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0, 32784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0, 32785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0, 32786, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA, 32787, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_03 ),
};

/* Char_Effect 16: shl15_00_bite_end */
const Sint32 pl15_effect_16[] = {
	CG_FRAME(    0,    0,   4,           0, 32779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0, 32780, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32782, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_16 ),
};

/* Char_Special 0
   Char_Special 1 */
const Sint32 pl15_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   299, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   303, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,  ATK,  0x00,       0x2d,    0,   0,     0,      0,   1 ),
};

const Sint32 pl15_special_00_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_special_00_loop ),
};

/* Char_Special 3
   Char_Special 4
   Char_Special 5 */
const Sint32 pl15_special_03[] = {
	CG_FRAME(    0,    1,   3, END_OF_DATA,   299, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_special_03 ),
};

/* Char_Special 6: kaze_init (Kaze no Sakebi, 6 + atck_level) */
const Sint32 pl15_special_06[] = {
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   361, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   362, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   363, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   364, 0x80,   0x00,  ATK,  0x83,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   365, 0x80,   0x00,  ATK,  0x84,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   366, 0x80,   0x00,  ATK,  0x85,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   367, 0x80,   0x00,  ATK,  0x86,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   1,           0,   368, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   3,           0,   372, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   3,           0,   373, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   3,           0,   374, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   3,           0,   375, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   3,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   3,           0,   374, 0x80,   0x00,  ATK,  0x04,          0,    0,   0,     0,      0,  26 ),
};

const Sint32 pl15_special_06_loop[] = {
	CG_FRAME(    0,    1,   3, END_OF_DATA,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl15_special_06_loop ),
};

/* Char_Special 7 */
const Sint32 pl15_special_07[] = {
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   361, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   362, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   363, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   364, 0x80,   0x00,  ATK,  0x83,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x80,   0x00,  ATK,  0x84,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x00,  ATK,  0x85,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,  ATK,  0x86,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   368, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   372, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   1,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   374, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   1,           0,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   372, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   1,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   374, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   1,           0,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   372, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   1,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   1,           0,   374, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0,      0,  26 ),
};

const Sint32 pl15_special_07_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl15_special_07_loop ),
};

/* Char_Special 8: kaze_move (Kaze no Sakebi, 8 + atck_level) */
const Sint32 pl15_special_08[] = {
	CG_FRAME(    0,    0,   3,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    9,   2,           0,   359, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   358, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl15_special_09[] = {
	CG_FRAME(    0,    0,   6,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   360, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    9,   4,           0,   359, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_special_09 ),
};

/* Char_Super 0: taiyou_init (Taiyou no Megumi (hyper)) */
const Sint32 pl15_super_00[] = {
	CG_FRAME(    0,    0,   1,           0,   100, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   469, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    99, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   471, 0x80,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   472, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   473, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   474, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   475, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   476, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   477, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   478, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   479, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     1,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   480, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     4,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   481, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     1,      0,  54 ),
	CG_NEXT( pl15_super_00 ),
};

/* Char_Super 1: taiyou_init (Taiyou no Megumi (hyper)) */
const Sint32 pl15_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl15_super_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   483, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   1,           0,   484, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   485, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   1,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   484, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   485, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl15_super_01_loop ),
};

/* Char_Super 2: taiyou_move (Taiyou no Megumi (hyper)) */
const Sint32 pl15_super_02[] = {
	CG_FRAME(    0,    0,   3,           0,   485, 0x80,   0x00,  ATK,  0x00,       0x2b,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   486, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   487, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   488, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   489, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   480, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   490, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl15_super_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    71, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl15_super_02_loop ),
};

/* Char_Super 3: okite_init (Shokubutsu no Okite (hyper)) */
const Sint32 pl15_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   422, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   422, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl15_super_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   424, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_super_03_loop ),
};

/* Char_Super 4: okite_reach_start (Shokubutsu no Okite (hyper)), okite_reach_wait (Shokubutsu no Okite (hyper)) */
const Sint32 pl15_super_04[] = {
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   428, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl15_super_04 ),
};

/* Char_Super 5: okite_catch_init (Shokubutsu no Okite (hyper)) */
const Sint32 pl15_super_05[] = {
	CG_FRAME(    0,    1,   4,           0,   429, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   430, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0,   432, 0x80,   0x00,    0,  0x00,       0x25,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0,   433, 0x80,   0x00,    0,  0x90,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    7,   4, END_OF_DATA,   433, 0x80,   0x00,    0,  0x90,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_super_05 ),
};

/* Char_Effect 4: shl15_01_init */
const Sint32 pl15_effect_04[] = {
	CG_FRAME(    0,    0,   3,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32770, 0x80,   0x00,    0,  0x00,       0x2e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 32773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   388, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   390, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   391, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   392, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
};

const Sint32 pl15_effect_04_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   395, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl15_effect_04_loop ),
};

/* Char_Effect 5
   Char_Effect 6: shl15_01_open */
const Sint32 pl15_effect_05[] = {
	CG_FRAME(    0,    0,   5,           0,   395, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   396, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   4,           0,   397, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   398, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   399, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   400, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   396, 0x80,   0x20,    0,  0x00,       0x2f,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl15_effect_05 ),
};

/* Char_Effect 7: shl15_01_t0_hop */
const Sint32 pl15_effect_07[] = {
	CG_FRAME(    0,    0,   6,           0,   395, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   396, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   6,           0,   397, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   5,           0,   398, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    1,   5,           0,   399, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    1,   5,           0,   400, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl15_effect_07_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   396, 0x80,   0x20,    0,  0x00,       0x2f,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl15_effect_07_loop ),
};

/* Char_Effect 8 */
const Sint32 pl15_effect_08[] = {
	CG_FRAME(    0,    0,   5,           0,   395, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   396, 0x80,   0x20,  ATK,  0x00,       0x2f,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   397, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   398, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   8,           0,   401, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   4,           0,   402, 0x80,   0x20,  ATK,  0x00,       0x36,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,   4,           0,   403, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    2,   4,           0,   403, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   4,           0,   399, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   4,           0,   400, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
};

const Sint32 pl15_effect_08_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   396, 0x80,   0x20,  ATK,  0x00,       0x2f,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl15_effect_08_loop ),
};

/* Char_Effect 9 */
const Sint32 pl15_effect_09[] = {
	CG_FRAME(    0,    0,   5,           0,   395, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   396, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   397, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   3,           0,   398, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   8,           0,   404, 0x80,   0x20,  ATK,  0x00,       0x37,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,   4,           0,   404, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,   4,           0,   399, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   4,           0,   400, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
};

const Sint32 pl15_effect_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   396, 0x80,   0x20,  ATK,  0x00,       0x2f,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl15_effect_09_loop ),
};

/* Char_Catch 1: shl15_01_t0_jump
   Char_Effect 10 */
const Sint32 pl15_catch_01[] = {
	CG_FRAME(    0,    0,   8,           0,   405, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   406, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_NEXT( pl15_catch_01 ),
};

/* Char_Effect 11: shl15_01_hit_ck */
const Sint32 pl15_effect_11[] = {
	CG_FRAME( 0x80,    0,   5,           0,   407, 0x80,   0x20,    0,  0x00,       0x35,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x82,    0,   5, END_OF_DATA,   408, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl15_effect_11 ),
};

/* Char_Effect 12: shl15_01_t0_jump, shl15_01_t1_atck, shl15_01_end */
const Sint32 pl15_effect_12[] = {
	CG_FRAME(    0,    0,   6,           0,   409, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   410, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   410, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   409, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   410, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   410, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   413, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   416, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   417, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   418, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   419, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_effect_12_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   421, 0x80,   0x24,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_12_loop ),
};

/* Char_Effect 13: shl15_01_t0_jump, shl15_01_hold, shl15_01_t1_atck */
const Sint32 pl15_effect_13[] = {
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_effect_13_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_13_loop ),
};

/* Char_Effect 17 */
const Sint32 pl15_effect_17[] = {
	CG_FRAME(    0,    0,  12,           0,   399, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   399, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_17 ),
};

/* Char_Effect 14: shl15_02_init */
const Sint32 pl15_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   452, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   453, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl15_effect_14_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   455, 0x80,   0x00,  ATK,  0x00,       0x31,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl15_effect_14_loop ),
};

/* Char_Effect 15: shl15_02_back */
const Sint32 pl15_effect_15[] = {
	CG_FRAME(    0,    0,   4,           0,   456, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   451, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl15_effect_15 ),
};

const Sint32 pl15_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4, END_OF_DATA,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	0x00004e18,
};

const Sint32 pl15_unused_04[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4, END_OF_DATA,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	0x00004e30,
};

/* group 27 0: pls15_00_t2_main */
const Sint32 pl15_grp27_00[] = {
	EF_FRAME(    0,    0,   1,           0,   433 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   434 ),
	CG_NEXT( pl15_grp27_00 ),
};

/* group 27 1: pls15_03_wait */
const Sint32 pl15_grp27_01[] = {
	EF_FRAME(    0,    0,   4,           0,   356 ),
	EF_FRAME(    0,    0,   4,           0,   357 ),
	EF_FRAME(    0,    0,   4,           0,   355 ),
	EF_FRAME(    0,    0,   4,           0,   379 ),
	EF_FRAME(    0,    0,   4,           0,   380 ),
	EF_FRAME(    0,    0,   4,           0,   381 ),
	EF_FRAME(    0,    0,   4,           0,   382 ),
	EF_FRAME(    0,    0,   4,           0,   290 ),
	EF_FRAME(    0,    0,   4,           0,   291 ),
	EF_FRAME(    0,    0,   4,           0,   292 ),
	EF_FRAME(    0,    0,   4,           0,   293 ),
	EF_FRAME(    0,    0,   4,           0,   294 ),
	EF_FRAME(    0,    0,   4,           0,   295 ),
	EF_FRAME(    0,    0,   4,           0,   296 ),
	EF_FRAME(    0,    0,   4,           0,   297 ),
	EF_FRAME(    0,    0,   4,           0,   298 ),
	EF_FRAME(    0,    0,   4,           0,   467 ),
	EF_FRAME(    0,    0,   4,           0,   355 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   356 ),
	CG_NEXT( pl15_grp27_01 ),
};

/* group 27 2: pls15_01_t1_main */
const Sint32 pl15_grp27_02[] = {
	EF_FRAME(    0,    0,   4,           0,   368 ),
	EF_FRAME(    0,    0,   4,           0,   369 ),
	EF_FRAME(    0,    0,   4,           0,   370 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   371 ),
	CG_NEXT( pl15_grp27_02 ),
};

/* group 27 3: pls15_02_init */
const Sint32 pl15_grp27_03[] = {
	EF_FRAME(    0,    0,   4,           0,   476 ),
	EF_FRAME(    0,    0,   4,           0,   477 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   478 ),
	CG_NEXT( pl15_grp27_03 ),
};

/* group 27 4: pls15_01_t2_main */
const Sint32 pl15_grp27_04[] = {
	EF_FRAME(    0,    0,   6, END_OF_DATA,   355 ),
	CG_NEXT( pl15_grp27_04 ),
};

/* group 27 5: pls15_01_t3_main (5 + no) */
const Sint32 pl15_grp27_05[] = {
	EF_FRAME(    0,    0,   2,           0, 32794 ),
	EF_FRAME(    0,    0,   2,           0, 32795 ),
	EF_FRAME(    0,    0,   2,           0, 32796 ),
	EF_FRAME(    0,    0,   2,           0, 32797 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32798 ),
	CG_NEXT( pl15_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl15_grp27_06[] = {
	EF_FRAME(    0,    0,   2,           0, 32799 ),
	EF_FRAME(    0,    0,   2,           0, 32800 ),
	EF_FRAME(    0,    0,   2,           0, 32801 ),
	EF_FRAME(    0,    0,   2,           0, 32802 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32803 ),
	CG_NEXT( pl15_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl15_grp27_07[] = {
	EF_FRAME(    0,    0,   2,           0, 32804 ),
	EF_FRAME(    0,    0,   2,           0, 32805 ),
	EF_FRAME(    0,    0,   2,           0, 32806 ),
	EF_FRAME(    0,    0,   2,           0, 32807 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32808 ),
	CG_NEXT( pl15_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl15_grp27_08[] = {
	EF_FRAME(    0,    0,   2,           0, 32809 ),
	EF_FRAME(    0,    0,   2,           0, 32810 ),
	EF_FRAME(    0,    0,   2,           0, 32811 ),
	EF_FRAME(    0,    0,   2,           0, 32812 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32813 ),
	CG_NEXT( pl15_grp27_08 ),
};

/* group 27 9 */
const Sint32 pl15_grp27_09[] = {
	EF_FRAME(    0,    0,   2,           0, 32814 ),
	EF_FRAME(    0,    0,   2,           0, 32815 ),
	EF_FRAME(    0,    0,   2,           0, 32816 ),
	EF_FRAME(    0,    0,   2,           0, 32817 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32818 ),
	CG_NEXT( pl15_grp27_09 ),
};

/* group 27 10: pls15_01_t8_main */
const Sint32 pl15_grp27_10[] = {
	EF_FRAME(    0,    0,   4,           0,   534 ),
	EF_FRAME(    0,    0,   4,           0,   535 ),
	EF_FRAME(    0,    0,   4,           0,   536 ),
	EF_FRAME(    0,    0,   4,           0,   537 ),
	EF_FRAME(    0,    0,   4,           0,   538 ),
	EF_FRAME(    0,    0,   4,           0,   539 ),
	EF_FRAME(    0,    0,   4,           0,   540 ),
	EF_FRAME(    0,    0,   4,           0,   541 ),
	EF_FRAME(    0,    0,   4,           0,   542 ),
	EF_FRAME(    0,    0,   4,           0,   543 ),
	EF_FRAME(    0,    0,   4,           0,   544 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   545 ),
	CG_NEXT( pl15_grp27_10 ),
};
