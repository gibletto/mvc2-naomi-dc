/* Captain America (character 0b): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_0b      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_0b     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl0b.c) says which group
                   and number each move uses.
   pl0b_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl0b_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl0b_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl0b_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_0b
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_0b, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_0b */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl0b_stand_00[], pl0b_stand_02[], pl0b_stand_03[], pl0b_jump_00[],
	pl0b_jump_00_loop[], pl0b_jump_01[], pl0b_jump_01_loop[], pl0b_jump_02[], pl0b_jump_02_loop[],
	pl0b_jump_03[], pl0b_jump_06[], pl0b_jump_09[], pl0b_jump_07[], pl0b_jump_10[], pl0b_jump_08[],
	pl0b_jump_11[], pl0b_dash_00[], pl0b_dash_00_loop[], pl0b_dash_01[], pl0b_dash_01_loop[],
	pl0b_dash_02[], pl0b_dash_03[], pl0b_dash_03_loop[], pl0b_crouch_00[], pl0b_crouch_01[],
	pl0b_crouch_02[], pl0b_turn_00[], pl0b_turn_01[], pl0b_guard_00[], pl0b_guard_01[],
	pl0b_guard_02[], pl0b_guard_03[], pl0b_guard_04[], pl0b_guard_05[], pl0b_guard_06[],
	pl0b_guard_07[], pl0b_air_guard_00[], pl0b_air_guard_01[], pl0b_air_guard_02[],
	pl0b_stand_p_00[], pl0b_stand_p_03[], pl0b_stand_p_01[], pl0b_stand_p_01_loop[],
	pl0b_stand_p_02[], pl0b_stand_p_02_loop[], pl0b_stand_p_04[], pl0b_stand_p_04_loop[],
	pl0b_stand_k_00[], pl0b_stand_k_03[], pl0b_stand_k_01[], pl0b_stand_k_01_loop[],
	pl0b_stand_k_04[], pl0b_stand_k_02[], pl0b_unused_00[], pl0b_crouch_p_00[], pl0b_crouch_p_03[],
	pl0b_crouch_p_01[], pl0b_crouch_p_04[], pl0b_crouch_p_04_loop[], pl0b_crouch_p_02[],
	pl0b_crouch_p_05[], pl0b_crouch_p_05_loop[], pl0b_crouch_k_00[], pl0b_crouch_k_03[],
	pl0b_crouch_k_01[], pl0b_crouch_k_02[], pl0b_jump_p_00[], pl0b_jump_p_01[], pl0b_jump_p_06[],
	pl0b_jump_p_02[], pl0b_jump_p_07[], pl0b_jump_p_07_loop[], pl0b_jump_k_00[], pl0b_jump_k_01[],
	pl0b_extra_06[], pl0b_jump_k_02[], pl0b_jump_k_03[], pl0b_jump_k_04[], pl0b_damage_00[],
	pl0b_damage_01[], pl0b_damage_02[], pl0b_damage_03[], pl0b_damage_04[], pl0b_damage_05[],
	pl0b_damage_06[], pl0b_damage_07[], pl0b_damage_08[], pl0b_damage_09[], pl0b_damage_09_loop[],
	pl0b_damage_10[], pl0b_damage_11[], pl0b_damage_12[], pl0b_damage_13[], pl0b_damage_16[],
	pl0b_damage_17[], pl0b_damage_18[], pl0b_damage_19[], pl0b_damage_20[], pl0b_damage_21[],
	pl0b_damage_22[], pl0b_damage_23[], pl0b_damage_24[], pl0b_damage_25[], pl0b_damage_26[],
	pl0b_damage_27[], pl0b_damage_28[], pl0b_damage_29[], pl0b_damage_30[], pl0b_damage_31[],
	pl0b_damage_32[], pl0b_dizzy_00[], pl0b_catch_00[], pl0b_catch_00_loop[], pl0b_catch_01[],
	pl0b_catch_01_loop[], pl0b_catch_02[], pl0b_catch_02_loop[], pl0b_catch_03[], pl0b_catch_04[],
	pl0b_catch_04_loop[], pl0b_caught_00[], pl0b_caught_01[], pl0b_caught_02[], pl0b_caught_03[],
	pl0b_caught_04[], pl0b_caught_05[], pl0b_caught_06[], pl0b_caught_07[], pl0b_caught_08[],
	pl0b_caught_09[], pl0b_caught_10[], pl0b_caught_11[], pl0b_caught_12[], pl0b_caught_13[],
	pl0b_caught_14[], pl0b_caught_15[], pl0b_caught_16[], pl0b_caught_17[], pl0b_caught_18[],
	pl0b_caught_19[], pl0b_caught_20[], pl0b_caught_21[], pl0b_get_up_00[], pl0b_get_up_01[],
	pl0b_get_up_02[], pl0b_get_up_03[], pl0b_extra_00[], pl0b_extra_01[], pl0b_extra_03[],
	pl0b_extra_04[], pl0b_extra_05[], pl0b_extra_07[], pl0b_extra_08[], pl0b_extra_08_loop[],
	pl0b_extra_02[], pl0b_appear_03[], pl0b_appear_03_loop[], pl0b_appear_04[], pl0b_appear_00[],
	pl0b_appear_01[], pl0b_appear_01_loop[], pl0b_appear_02[], pl0b_win_00[], pl0b_win_00_loop[],
	pl0b_win_01[], pl0b_win_06[], pl0b_effect_29[], pl0b_win_05[], pl0b_win_04[], pl0b_win_04_loop[],
	pl0b_win_07[], pl0b_win_07_loop[], pl0b_win_08[], pl0b_win_09[], pl0b_win_10[],
	pl0b_win_10_loop[], pl0b_win_11[], pl0b_win_11_loop[], pl0b_effect_30[], pl0b_effect_31[],
	pl0b_effect_32[], pl0b_effect_33[], pl0b_effect_34[], pl0b_grp28_00[], pl0b_grp28_01[],
	pl0b_effect_35[], pl0b_effect_36[], pl0b_effect_37[], pl0b_effect_38[], pl0b_effect_39[],
	pl0b_effect_41[], pl0b_effect_41_loop[], pl0b_effect_42[], pl0b_effect_42_loop[],
	pl0b_effect_43[], pl0b_effect_40[], pl0b_win_03[], pl0b_win_03_loop[], pl0b_special_00[],
	pl0b_special_01[], pl0b_special_03[], pl0b_special_02[], pl0b_special_04[], pl0b_special_05[],
	pl0b_special_05_loop[], pl0b_special_06[], pl0b_special_06_loop[], pl0b_special_09[],
	pl0b_special_09_loop[], pl0b_special_10[], pl0b_special_10_loop[], pl0b_special_07[],
	pl0b_special_08[], pl0b_special_11[], pl0b_special_13[], pl0b_special_14[], pl0b_special_15[],
	pl0b_super_00[], pl0b_super_00_loop[], pl0b_super_01[], pl0b_super_02[], pl0b_super_02_loop[],
	pl0b_super_03[], pl0b_super_03_loop[], pl0b_super_04[], pl0b_catch_05[], pl0b_super_06[],
	pl0b_super_06_loop[], pl0b_super_07[], pl0b_super_07_loop[], pl0b_super_08[], pl0b_super_09[],
	pl0b_super_09_loop[], pl0b_super_10[], pl0b_super_11[], pl0b_super_12[], pl0b_super_13[],
	pl0b_unused_01[], pl0b_unused_02[], pl0b_unused_03[], pl0b_effect_00[], pl0b_effect_01[],
	pl0b_effect_02[], pl0b_effect_03[], pl0b_effect_04[], pl0b_effect_05[], pl0b_effect_06[],
	pl0b_effect_07[], pl0b_fly_05[], pl0b_fly_06[], pl0b_fly_07[], pl0b_effect_08[],
	pl0b_effect_09[], pl0b_effect_10[], pl0b_effect_11[], pl0b_effect_17[], pl0b_effect_18[],
	pl0b_effect_21[], pl0b_effect_21_loop[], pl0b_effect_22[], pl0b_effect_22_loop[],
	pl0b_effect_23[], pl0b_effect_23_loop[], pl0b_effect_12[], pl0b_effect_14[], pl0b_effect_15[],
	pl0b_effect_19[], pl0b_effect_13[], pl0b_effect_16[], pl0b_effect_26[], pl0b_effect_24[],
	pl0b_effect_25[], pl0b_effect_27[], pl0b_change_00[], pl0b_change_01[], pl0b_change_02[],
	pl0b_change_03[], pl0b_change_03_loop[], pl0b_change_04[], pl0b_grp27_00[], pl0b_grp27_02[],
	pl0b_grp27_06[];
extern const Sint32 *const pl0b_stand_tbl[], *const pl0b_jump_tbl[], *const pl0b_dash_tbl[],
	*const pl0b_crouch_tbl[], *const pl0b_turn_tbl[], *const pl0b_guard_tbl[],
	*const pl0b_air_guard_tbl[], *const pl0b_stand_p_tbl[], *const pl0b_stand_k_tbl[],
	*const pl0b_crouch_p_tbl[], *const pl0b_crouch_k_tbl[], *const pl0b_jump_p_tbl[],
	*const pl0b_jump_k_tbl[], *const pl0b_damage_tbl[], *const pl0b_dizzy_tbl[],
	*const pl0b_catch_tbl[], *const pl0b_caught_tbl[], *const pl0b_get_up_tbl[],
	*const pl0b_appear_tbl[], *const pl0b_win_tbl[], *const pl0b_extra_tbl[],
	*const pl0b_special_tbl[], *const pl0b_super_tbl[], *const pl0b_effect_tbl[],
	*const pl0b_change_tbl[], *const pl0b_grp25_tbl[], *const pl0b_fly_tbl[],
	*const pl0b_grp27_tbl[], *const pl0b_grp28_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_0b[16] = {
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
const Sint32 *const *const char_tbl_0b[] = {
	pl0b_stand_tbl, pl0b_jump_tbl, pl0b_dash_tbl, pl0b_crouch_tbl, pl0b_turn_tbl, pl0b_guard_tbl,
	pl0b_air_guard_tbl, pl0b_stand_p_tbl, pl0b_stand_k_tbl, pl0b_crouch_p_tbl, pl0b_crouch_k_tbl, pl0b_jump_p_tbl,
	pl0b_jump_k_tbl, pl0b_damage_tbl, pl0b_dizzy_tbl, pl0b_catch_tbl, pl0b_caught_tbl, pl0b_get_up_tbl,
	pl0b_appear_tbl, pl0b_win_tbl, pl0b_extra_tbl, pl0b_special_tbl, pl0b_super_tbl, pl0b_effect_tbl,
	pl0b_change_tbl, pl0b_grp25_tbl, pl0b_fly_tbl, pl0b_grp27_tbl, pl0b_grp28_tbl, 0,
};

const Sint32 *const pl0b_stand_tbl[] = {
	pl0b_stand_00, pl0b_stand_00, pl0b_stand_02, pl0b_stand_03, pl0b_stand_00, pl0b_stand_00,
	pl0b_stand_02, pl0b_stand_03, pl0b_stand_02, pl0b_stand_03, 0,
};

const Sint32 *const pl0b_jump_tbl[] = {
	pl0b_jump_00, pl0b_jump_01, pl0b_jump_02, pl0b_jump_03, pl0b_jump_03, pl0b_jump_03,
	pl0b_jump_06, pl0b_jump_07, pl0b_jump_08, pl0b_jump_09, pl0b_jump_10, pl0b_jump_11,
	0,
};

const Sint32 *const pl0b_dash_tbl[] = {
	pl0b_dash_00, pl0b_dash_01, pl0b_dash_02, pl0b_dash_03, 0,
};

const Sint32 *const pl0b_crouch_tbl[] = {
	pl0b_crouch_00, pl0b_crouch_01, pl0b_crouch_02, 0,
};

const Sint32 *const pl0b_turn_tbl[] = {
	pl0b_turn_00, pl0b_turn_01, 0,
};

const Sint32 *const pl0b_guard_tbl[] = {
	pl0b_guard_00, pl0b_guard_01, pl0b_guard_02, pl0b_guard_03, pl0b_guard_04, pl0b_guard_05,
	pl0b_guard_06, pl0b_guard_07, 0,
};

const Sint32 *const pl0b_air_guard_tbl[] = {
	pl0b_air_guard_00, pl0b_air_guard_01, pl0b_air_guard_02, 0,
};

const Sint32 *const pl0b_stand_p_tbl[] = {
	pl0b_stand_p_00, pl0b_stand_p_01, pl0b_stand_p_02, pl0b_stand_p_03, pl0b_stand_p_04, 0,
};

const Sint32 *const pl0b_stand_k_tbl[] = {
	pl0b_stand_k_00, pl0b_stand_k_01, pl0b_stand_k_02, pl0b_stand_k_03, pl0b_stand_k_04, 0,
};

const Sint32 *const pl0b_crouch_p_tbl[] = {
	pl0b_crouch_p_00, pl0b_crouch_p_01, pl0b_crouch_p_02, pl0b_crouch_p_03, pl0b_crouch_p_04, pl0b_crouch_p_05,
	0,
};

const Sint32 *const pl0b_crouch_k_tbl[] = {
	pl0b_crouch_k_00, pl0b_crouch_k_01, pl0b_crouch_k_02, pl0b_crouch_k_03, 0,
};

const Sint32 *const pl0b_jump_p_tbl[] = {
	pl0b_jump_p_00, pl0b_jump_p_01, pl0b_jump_p_02, pl0b_jump_p_00, pl0b_jump_p_01, pl0b_jump_p_02,
	pl0b_jump_p_06, pl0b_jump_p_07, 0,
};

const Sint32 *const pl0b_jump_k_tbl[] = {
	pl0b_jump_k_00, pl0b_jump_k_01, pl0b_jump_k_02, pl0b_jump_k_03, pl0b_jump_k_04, 0,
};

const Sint32 *const pl0b_damage_tbl[] = {
	pl0b_damage_00, pl0b_damage_01, pl0b_damage_02, pl0b_damage_03, pl0b_damage_04, pl0b_damage_05,
	pl0b_damage_06, pl0b_damage_07, pl0b_damage_08, pl0b_damage_09, pl0b_damage_10, pl0b_damage_11,
	pl0b_damage_12, pl0b_damage_13, pl0b_damage_13, pl0b_damage_13, pl0b_damage_16, pl0b_damage_17,
	pl0b_damage_18, pl0b_damage_19, pl0b_damage_20, pl0b_damage_21, pl0b_damage_22, pl0b_damage_23,
	pl0b_damage_24, pl0b_damage_25, pl0b_damage_26, pl0b_damage_27, pl0b_damage_28, pl0b_damage_29,
	pl0b_damage_30, pl0b_damage_31, pl0b_damage_32, pl0b_damage_27, pl0b_damage_13, 0,
};

const Sint32 *const pl0b_dizzy_tbl[] = {
	pl0b_dizzy_00, 0,
};

const Sint32 *const pl0b_catch_tbl[] = {
	pl0b_catch_00, pl0b_catch_01, pl0b_catch_02, pl0b_catch_03, pl0b_catch_04, pl0b_catch_05,
	0,
};

const Sint32 *const pl0b_caught_tbl[] = {
	pl0b_caught_00, pl0b_caught_01, pl0b_caught_02, pl0b_caught_03, pl0b_caught_04, pl0b_caught_05,
	pl0b_caught_06, pl0b_caught_07, pl0b_caught_08, pl0b_caught_09, pl0b_caught_10, pl0b_caught_11,
	pl0b_caught_12, pl0b_caught_13, pl0b_caught_14, pl0b_caught_15, pl0b_caught_16, pl0b_caught_17,
	pl0b_caught_18, pl0b_caught_19, pl0b_caught_20, pl0b_caught_21, 0,
};

const Sint32 *const pl0b_get_up_tbl[] = {
	pl0b_get_up_00, pl0b_get_up_01, pl0b_get_up_02, pl0b_get_up_03, 0,
};

const Sint32 *const pl0b_appear_tbl[] = {
	pl0b_appear_00, pl0b_appear_01, pl0b_appear_02, pl0b_appear_03, pl0b_appear_04, 0,
};

const Sint32 *const pl0b_win_tbl[] = {
	pl0b_win_00, pl0b_win_01, pl0b_win_01, pl0b_win_03, pl0b_win_04, pl0b_win_05,
	pl0b_win_06, pl0b_win_07, pl0b_win_08, pl0b_win_09, pl0b_win_10, pl0b_win_11,
	0,
};

const Sint32 *const pl0b_extra_tbl[] = {
	pl0b_extra_00, pl0b_extra_01, pl0b_extra_02, pl0b_extra_03, pl0b_extra_04, pl0b_extra_05,
	pl0b_extra_06, pl0b_extra_07, pl0b_extra_08, 0,
};

const Sint32 *const pl0b_special_tbl[] = {
	pl0b_special_00, pl0b_special_01, pl0b_special_02, pl0b_special_03, pl0b_special_04, pl0b_special_05,
	pl0b_special_06, pl0b_special_07, pl0b_special_08, pl0b_special_09, pl0b_special_10, pl0b_special_11,
	pl0b_special_11, pl0b_special_13, pl0b_special_14, pl0b_special_15, 0,
};

const Sint32 *const pl0b_super_tbl[] = {
	pl0b_super_00, pl0b_super_01, pl0b_super_02, pl0b_super_03, pl0b_super_04, pl0b_catch_05,
	pl0b_super_06, pl0b_super_07, pl0b_super_08, pl0b_super_09, pl0b_super_10, pl0b_super_11,
	pl0b_super_12, pl0b_super_13, 0,
};

const Sint32 *const pl0b_effect_tbl[] = {
	pl0b_effect_00, pl0b_effect_01, pl0b_effect_02, pl0b_effect_03, pl0b_effect_04, pl0b_effect_05,
	pl0b_effect_06, pl0b_effect_07, pl0b_effect_08, pl0b_effect_09, pl0b_effect_10, pl0b_effect_11,
	pl0b_effect_12, pl0b_effect_13, pl0b_effect_14, pl0b_effect_15, pl0b_effect_16, pl0b_effect_17,
	pl0b_effect_18, pl0b_effect_19, pl0b_effect_19, pl0b_effect_21, pl0b_effect_22, pl0b_effect_23,
	pl0b_effect_24, pl0b_effect_25, pl0b_effect_26, pl0b_effect_27, pl0b_effect_26, pl0b_effect_29,
	pl0b_effect_30, pl0b_effect_31, pl0b_effect_32, pl0b_effect_33, pl0b_effect_34, pl0b_effect_35,
	pl0b_effect_36, pl0b_effect_37, pl0b_effect_38, pl0b_effect_39, pl0b_effect_40, pl0b_effect_41,
	pl0b_effect_42, pl0b_effect_43, 0,
};

const Sint32 *const pl0b_change_tbl[] = {
	pl0b_change_00, pl0b_change_01, pl0b_change_02, pl0b_change_03, pl0b_change_04, 0,
};

const Sint32 *const pl0b_grp25_tbl[] = {
	pl0b_stand_00, 0,
};

const Sint32 *const pl0b_fly_tbl[] = {
	pl0b_effect_03, pl0b_effect_04, pl0b_effect_05, pl0b_effect_06, pl0b_effect_07, pl0b_fly_05,
	pl0b_fly_06, pl0b_fly_07, pl0b_effect_08, 0,
};

const Sint32 *const pl0b_grp27_tbl[] = {
	pl0b_grp27_00, pl0b_grp27_00, pl0b_grp27_02, pl0b_grp27_00, pl0b_grp27_00, pl0b_grp27_02,
	pl0b_grp27_06, 0,
};

const Sint32 *const pl0b_grp28_tbl[] = {
	pl0b_grp28_00, pl0b_grp28_01, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0 */
const Sint32 pl0b_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    22, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01ac,   1 ),
	CG_FRAME(    0,    0,   4,           0,    23, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0262,   1 ),
	CG_FRAME(    0,    0,   4,           0,    24, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0263,   1 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0264,   1 ),
	CG_FRAME(    0,    0,   4,           0,    25, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0265,   1 ),
	CG_FRAME(    0,    0,   4,           0,    26, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0266,   1 ),
	CG_FRAME(    0,    0,   4,           0,    27, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0267,   1 ),
	CG_FRAME(    0,    0,   4,           0,    28, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0268,   1 ),
	CG_FRAME(    0,    0,   1,           0,    29, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0269,   1 ),
	CG_FRAME(    0,    0,   1,           0,    30, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x026a,   1 ),
	CG_FRAME(    0,    0,   1,           0,    31, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x026b,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    32, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x026c,   1 ),
	CG_NEXT( pl0b_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   Char_Stand 8 */
const Sint32 pl0b_stand_02[] = {
	CG_FRAME(    0,    0,   8,           0,    35, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x023b,   1 ),
	CG_FRAME(    0,    0,   8,           0,    36, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x023d,   1 ),
	CG_FRAME(    0,    0,   8,           0,    37, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x023f,   1 ),
	CG_FRAME(    0,    0,   8,           0,    38, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0241,   1 ),
	CG_FRAME(    0,    0,   8,           0,    39, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0243,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    34, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0245,   1 ),
	CG_NEXT( pl0b_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   Char_Stand 9 */
const Sint32 pl0b_stand_03[] = {
	CG_FRAME(    0,    0,   9,           0,    40, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0246,   1 ),
	CG_FRAME(    0,    0,   9,           0,    41, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0248,   1 ),
	CG_FRAME(    0,    0,   9,           0,    42, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x024a,   1 ),
	CG_FRAME(    0,    0,   9,           0,    43, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x024c,   1 ),
	CG_FRAME(    0,    0,   9,           0,    44, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x024e,   1 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,    45, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0250,   1 ),
	CG_NEXT( pl0b_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl0b_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0074,   3 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0075,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0076,   3 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0077,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0078,   3 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0079,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x007a,   3 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x007b,   3 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x007c,   3 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x007d,   3 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0212,   3 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x020f,   3 ),
};

const Sint32 pl0b_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0210,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0211,   3 ),
	CG_NEXT( pl0b_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl0b_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x007e,   3 ),
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x007f,   3 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0080,   3 ),
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0081,   3 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0080,   3 ),
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0081,   3 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0080,   3 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0213,   3 ),
	CG_FRAME(    0,    0,   2,           0,    73, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0082,   3 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0083,   3 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0084,   3 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0085,   3 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0086,   3 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0087,   3 ),
};

const Sint32 pl0b_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0088,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0089,   3 ),
	CG_NEXT( pl0b_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl0b_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x008a,   3 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008b,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008c,   3 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008b,   3 ),
	CG_FRAME(    0,    0,   2,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008c,   3 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008b,   3 ),
	CG_FRAME(    0,    0,   2,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008c,   3 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008d,   3 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008e,   3 ),
	CG_FRAME(    0,    0,   2,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008f,   3 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0090,   3 ),
	CG_FRAME(    0,    0,   2,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0091,   3 ),
	CG_FRAME(    0,    0,   2,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0092,   3 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0093,   3 ),
	CG_FRAME(    0,    0,   2,           0,    83, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0094,   3 ),
};

const Sint32 pl0b_jump_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0095,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0096,   3 ),
	CG_NEXT( pl0b_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl0b_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0097,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0098,   1 ),
	CG_NEXT( pl0b_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl0b_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0214,   3 ),
	CG_FRAME(    0,    0,  16,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0215,   3 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0216,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0215,   3 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0216,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0215,   3 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0216,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0217,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0218,   3 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0219,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021a,   3 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021b,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl0b_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021c,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021d,   3 ),
	CG_NEXT( pl0b_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl0b_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x021e,   3 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021f,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0220,   3 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021f,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0220,   3 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021f,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0220,   3 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x021f,   3 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0221,   3 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0222,   3 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0223,   3 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0224,   3 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0225,   3 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0226,   3 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0227,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl0b_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0228,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0229,   3 ),
	CG_NEXT( pl0b_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl0b_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x022a,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022b,   3 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022c,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022b,   3 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022c,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022b,   3 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022c,   3 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022d,   3 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022e,   3 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x022f,   3 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0230,   3 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0231,   3 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0232,   3 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0233,   3 ),
	CG_FRAME(    0,    0,   3,           0,    83, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0234,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl0b_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0235,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0236,   3 ),
	CG_NEXT( pl0b_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl0b_dash_00[] = {
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x80,          0,  0x00,    0,    0,   0,     0, 0x009f,  26 ),
};

const Sint32 pl0b_dash_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x80,          0,  0x00,    0,    0,   0,     0, 0x00a0,  26 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x80,          0,  0x00,    0,    0,   0,     0, 0x00a1,  26 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x80,       0x19,  0x00, 0x21,    0,   0,     0, 0x00a2,  26 ),
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x80,          0,  0x00,    0,    0,   0,     0, 0x00a3,  26 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x80,          0,  0x00,    0,    0,   0,     0, 0x00a4,  26 ),
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x80,          0,  0x00,    0,    0,   0,     0, 0x00a5,  26 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    53, 0x80,   0x80,       0x19,  0x00, 0x21,    0,   0,     0, 0x00a6,  26 ),
	CG_NEXT( pl0b_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl0b_dash_01[] = {
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00a7,   4 ),
	CG_FRAME(    0,    1,   3,           0,    60, 0x80,   0x01,       0x17,  0x00,    0,    0,   0,     0, 0x00a8,  87 ),
	CG_FRAME(    0, 0x80,   1,           0,    61, 0x80,   0x01,          5,  0x00, 0x03,    0,   0,     0, 0x00a9,  87 ),
	CG_FRAME(    0,    2,   3,           0,    61, 0x80,   0x1f,          0,  0x00,    0,    0,   0,     0, 0x00aa,  87 ),
	CG_FRAME(    0,    2,   1,           0,    62, 0x80,   0x1f,       0x18,  0x00,    0,    0,   0,     0, 0x00ab,  87 ),
};

const Sint32 pl0b_dash_01_loop[] = {
	CG_FRAME(    0,    2,   3, END_OF_DATA,    62, 0x80,   0x1f,          0,  0x00,    0,    0,   0,     0, 0x00ab,  87 ),
	CG_NEXT( pl0b_dash_01_loop ),
};

/* Char_Dash 2: dash_stop */
const Sint32 pl0b_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,    54, 0x80,   0x6c,          0,  0x00,    0,    0,   0,     0, 0x00ac,   1 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0x80,   0x4c,          0,  0x00,    0,    0,   0,     0, 0x00ad,   1 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x4d,          0,  0x00,    0,    0,   0,     0, 0x00ae,   1 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x4f,          0,  0x00,    0,    0,   0,     0, 0x00af,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    58, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00b0,   1 ),
	CG_NEXT( pl0b_dash_02 ),
};

/* Char_Dash 3: noki_fall */
const Sint32 pl0b_dash_03[] = {
	CG_FRAME(    0,    0,   6,           0,    63, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b1,   1 ),
};

const Sint32 pl0b_dash_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    64, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00b2,   1 ),
	CG_NEXT( pl0b_dash_03_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl0b_crouch_00[] = {
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b3,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    85, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00b4,   2 ),
	CG_NEXT( pl0b_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl0b_crouch_01[] = {
	CG_FRAME(    0, 0x80,   8, END_OF_DATA,    33, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0313,   2 ),
	CG_NEXT( pl0b_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl0b_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b3,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b5,   1 ),
	CG_NEXT( pl0b_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl0b_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b6,   1 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b7,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    89, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00b8,   1 ),
	CG_NEXT( pl0b_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl0b_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00b9,   2 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00ba,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00bb,   2 ),
	CG_NEXT( pl0b_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl0b_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00bc,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00bd,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl0b_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    94, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00be,   1 ),
	CG_NEXT( pl0b_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl0b_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    92, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00bc,   1 ),
	CG_NEXT( pl0b_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl0b_guard_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    94, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00be,   1 ),
	CG_NEXT( pl0b_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl0b_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00bf,   2 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00c0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl0b_guard_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    97, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00c1,   2 ),
	CG_NEXT( pl0b_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl0b_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    95, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00bf,   2 ),
	CG_NEXT( pl0b_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl0b_guard_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    97, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00c1,   2 ),
	CG_NEXT( pl0b_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl0b_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x00c2,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x00c3,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl0b_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x00c4,   3 ),
	CG_NEXT( pl0b_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl0b_air_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x00c4,   3 ),
	CG_NEXT( pl0b_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl0b_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   236, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ad,   4 ),
	CG_FRAME(    0,    0,   1,           0,   237, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ae,   4 ),
	CG_FRAME( 0x81,    0,   2,           0,   238, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01b0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   238, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01b0,  92 ),
	CG_FRAME(    0,    1,   5, END_OF_DATA,   237, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ae,   4 ),
	CG_NEXT( pl0b_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl0b_stand_p_03[] = {
	CG_FRAME(    0,    0,   4,           0,   236, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ad,   4 ),
	CG_FRAME(    0,    0,   1,           0,   237, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ae,   4 ),
	CG_FRAME( 0x81,    0,   2,           0,   238, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01b0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01b0,  92 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   237, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01ae,   4 ),
	CG_NEXT( pl0b_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl0b_stand_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   239, 0x90,   0x6d,        ATK,  0x00,    0,    0,   0,     0, 0x01b2, 101 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x90,   0x6d,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x01b2, 101 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x90,   0x6d,        ATK,  0x00,    0,    0,   0,     0, 0x01b3, 101 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x90,   0x6d,        ATK,  0x00,    0,    0,   0,     0, 0x01b5,   8 ),
	CG_FRAME(    0,    0,  10,           0,   241, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0, 0x01b5,  61 ),
	CG_FRAME(    0,    0,   5,           0,   242, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0, 0x01b6,   4 ),
};

const Sint32 pl0b_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   151, 0x90,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x01b7,   4 ),
	CG_NEXT( pl0b_stand_p_01_loop ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl0b_stand_p_02[] = {
	CG_FRAME(    0,    0,   5,           0,   253, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x02cd,  62 ),
	CG_FRAME(    0,    0,   1,           0,   253, 0x90,   0x00,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x02cd,  62 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x90,   0x70,        ATK,  0x00,    0,    0,   0,     0, 0x02ce,  63 ),
	CG_FRAME( 0x80,    0,   2,           0,   255, 0x90,   0x70,        ATK,  0x00,    0,    0,   0,     0, 0x02cf,   9 ),
	CG_FRAME( 0x80,    0,   2,           0,   256, 0x90,   0x70,        ATK,  0x00,    0,    0,   0,     0, 0x02d0,  25 ),
	CG_FRAME( 0x81,    0,   1,           0,   257, 0x90,   0x70,        ATK,  0x00,    0,    0,   0,     0, 0x02d1,  89 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x90,   0x70,        ATE,  0x00,    0,    0,   0,     0, 0x02d2,  89 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x90,   0x70,        ATE,  0x00,    0,    0,   0,     0, 0x02d3,  89 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x90,   0x70,        ATE,  0x00,    0,    0,   0,     0, 0x02d5,  89 ),
	CG_FRAME(    0,    0,   8,           0,   259, 0x90,   0x70,        ATE,  0x00,    0,    0,   0,     0, 0x02d7,  89 ),
	CG_FRAME(    0,    0,   8,           0,   260, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x02d9,   4 ),
};

const Sint32 pl0b_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   151, 0x90,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x02da,   4 ),
	CG_NEXT( pl0b_stand_p_02_loop ),
};

/* Char_Stand_P 4 */
const Sint32 pl0b_stand_p_04[] = {
	CG_FRAME(    0, 0x80,   1,           0,   243, 0x90,   0x6d,        ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0, 0x80,  10,           0,   243, 0x90,   0x6d, ATK | 0x16,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0, 0x81,   2,           0,   244, 0x90,   0x6d,    ATK | 7,  0x00, 0x05,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0, 0x82,   2,           0,   245, 0x90,   0x6d,        ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0, 0x83,   1,           0,   246, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x84,   1,           0,   247, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x85,   1,           0,   248, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x86,   1,           0,   249, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x87,   0,           0,   250, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x88,   0,           0,   251, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x87,  12,           0,   250, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x88,   8,           0,   251, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0, 0x89,   6,           0,   252, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0, 0x020d,   4 ),
};

const Sint32 pl0b_stand_p_04_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    86, 0x90,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x0238,   4 ),
	CG_NEXT( pl0b_stand_p_04_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl0b_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   261, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01b8,   4 ),
	CG_FRAME(    0,    0,   2,           0,   262, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ba,  10 ),
	CG_FRAME(    0,    0,   6,           0,   262, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ba,  90 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   263, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01bc,   4 ),
	CG_NEXT( pl0b_stand_k_00 ),
};

/* Char_Stand_K 3 */
const Sint32 pl0b_stand_k_03[] = {
	CG_FRAME(    0,    0,   6,           0,   261, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01b8,   4 ),
	CG_FRAME( 0x81,    0,   2,           0,   262, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ba,  10 ),
	CG_FRAME(    0,    0,   4,           0,   262, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01ba,  90 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   263, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01bc,   4 ),
	CG_NEXT( pl0b_stand_k_03 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl0b_stand_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   264, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01bd,   4 ),
	CG_FRAME(    0,    0,   5,           0,   264, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01bd,   4 ),
	CG_FRAME(    0,    0,   1,           0,   265, 0x90,   0x00,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x01be,   4 ),
	CG_FRAME(    0,    0,   6,           0,   266, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x01c0,  11 ),
	CG_FRAME(    0,    1,   8,           0,   267, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01c3,  91 ),
	CG_FRAME(    0,    1,   8,           0,   270, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01c4,   4 ),
};

const Sint32 pl0b_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   151, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01c5,   4 ),
	CG_NEXT( pl0b_stand_k_01_loop ),
};

/* Char_Stand_K 4: stand_kick_move */
const Sint32 pl0b_stand_k_04[] = {
	CG_FRAME(    0,    0,   1,           0,   268, 0x90,   0x08,        ATK,  0x00,    0,    0,   0,     0, 0x01c6,   4 ),
	CG_FRAME(    0,    0,   7,           0,   268, 0x90,   0x08, ATK | 0x14,  0x00,    0,    0,   0,     0, 0x01c6,   4 ),
	CG_FRAME( 0x81,    0,   3,           0,   269, 0x90,   0xa1,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x0297,  28 ),
	CG_FRAME(    0,    0,   8,           0,   269, 0x90,   0xa1,        ATE,  0x00,    0,    0,   0,     0, 0x0297,  76 ),
	CG_FRAME(    0,    0,   6,           0,   268, 0x90,   0x08,        ATE,  0x00,    0,    0,   0,     0, 0x01c6,   4 ),
	CG_FRAME(    0,    0,   6,           0,   270, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01c4,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   151, 0x90,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x01c5,   4 ),
	CG_NEXT( pl0b_stand_k_04 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl0b_stand_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   271, 0x70,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x029a,   4 ),
	CG_FRAME(    0,    0,   8,           0,   272, 0x70,   0x6d,        ATK,  0x00,    0,    0,   0,     0, 0x029b,   4 ),
	CG_FRAME(    0,    0,   1,           0,   272, 0x70,   0x6d, ATK | 0x15,  0x00,    0,    0,   0,     0, 0x029b,   4 ),
	CG_FRAME( 0x80,    0,   1,           0,   273, 0x90,   0xe1,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x029f,  12 ),
	CG_FRAME( 0x81,    0,   3,           0,   273, 0x90,   0xe1,        ATK,  0x00,    0,    0,   0,     0, 0x029f,  12 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x90,   0xe1,        ATE,  0x00,    0,    0,   0,     0, 0x029f,  65 ),
	CG_FRAME(    0,    0,   2,           0,   274, 0x90,   0xe1,        ATE,  0x00,    0,    0,   0,     0, 0x027d,  65 ),
	CG_FRAME(    0,    0,   8,           0,   275, 0x90,   0x05,        ATE,  0x00,    0,    0,   0,     0, 0x027e,  66 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x90,   0x80,        ATE,  0x00,    0,    0,   0,     0, 0x0326,  66 ),
	CG_FRAME(    0,    0,   6,           0,   277, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0, 0x0327,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   149, 0x90,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x0328,   4 ),
	CG_NEXT( pl0b_stand_k_02 ),
};

const Sint32 pl0b_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2, END_OF_DATA,   151, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x0329,   4 ),
	0x000019a8,
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl0b_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   278, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032a,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   279, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032b,  13 ),
	CG_FRAME( 0x81,    0,   5,           0,   279, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032c,  93 ),
	CG_FRAME(    0,    0,   3,           0,   280, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032d,   5 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   281, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032e,   5 ),
	CG_NEXT( pl0b_crouch_p_00 ),
};

/* Char_Crouch_P 3 */
const Sint32 pl0b_crouch_p_03[] = {
	CG_FRAME(    0,    0,   3,           0,   278, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032a,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   279, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032b,  13 ),
	CG_FRAME( 0x81,    0,   2,           0,   279, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032b,  93 ),
	CG_FRAME(    0,    0,   3,           0,   280, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032d,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   281, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032e,   5 ),
	CG_NEXT( pl0b_crouch_p_03 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl0b_crouch_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   282, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x02db,   5 ),
	CG_FRAME(    0,    0,   2,           0,   282, 0x90,   0x4d,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x02db,   5 ),
	CG_FRAME(    0,    0,   1,           0,   283, 0x90,   0xa5,        ATK,  0x00,    0,    0,   0,     0, 0x02dc,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   288, 0x90,   0xa5,        ATK,  0x00,    0,    0,   0,     0, 0x02dd,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   289, 0x90,   0xa5,        ATK,  0x00,    0,    0,   0,     0, 0x02de,  14 ),
	CG_FRAME(    0,    0,   7,           0,   285, 0x90,   0xa5,        ATE,  0x00,    0,    0,   0,     0, 0x02e1,  67 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x02e2,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x02e3,   5 ),
	CG_NEXT( pl0b_crouch_p_01 ),
};

/* Char_Crouch_P 4 */
const Sint32 pl0b_crouch_p_04[] = {
	CG_FRAME(    0, 0x80,   1,           0,   282, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x032f,   5 ),
	CG_FRAME(    0, 0x80,   4,           0,   282, 0x90,   0x4d,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x032f,   5 ),
	CG_FRAME(    0, 0x81,   1,           0,   283, 0x90,   0xa5,        ATK,  0x00,    0,    0,   0,     0, 0x0330,   5 ),
	CG_FRAME( 0x80, 0x82,   2,           0,   284, 0x90,   0xa5,        ATK,  0x00,    0,    0,   0,     0, 0x0331,  29 ),
	CG_FRAME( 0x80, 0x83,   1,           0,   285, 0x90,   0xa5,        ATK,  0x00,    0,    0,   0,     0, 0x0332,  77 ),
	CG_FRAME(    0, 0x84,   0,           0,   286, 0x90,   0xa5,        ATE,  0x00,    0,    0,   0,     0, 0x0333,  77 ),
	CG_FRAME(    0, 0x83,   0,           0,   285, 0x90,   0xa5,        ATE,  0x00,    0,    0,   0,     0, 0x0332,  77 ),
	CG_FRAME(    0, 0x84,  12,           0,   286, 0x90,   0xa5,        ATE,  0x00,    0,    0,   0,     0, 0x0333,  77 ),
	CG_FRAME(    0, 0x85,   6,           0,   287, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0334,   5 ),
};

const Sint32 pl0b_crouch_p_04_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0335,   5 ),
	CG_NEXT( pl0b_crouch_p_04_loop ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl0b_crouch_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   296, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e4,  68 ),
	CG_FRAME(    0,    0,   6,           0,   296, 0x90,   0x8a,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x02e4,  68 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e5,  68 ),
	CG_FRAME( 0x80,    0,   1,           0,   298, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e6,  15 ),
	CG_FRAME( 0x80,    0,   2,           0,   299, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e7,  15 ),
	CG_FRAME( 0x81,    0,   2,           0,   300, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e8,  15 ),
	CG_FRAME(    0,    0,   5,           0,   301, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e9,  69 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02eb,  69 ),
	CG_FRAME(    0,    0,   5,           0,   293, 0x90,   0x8a,        ATE,  0x00,    0,    0,   0,     0, 0x02ec,  68 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x90,   0x6a,        ATE,  0x00,    0,    0,   0,     0, 0x02ed,   5 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x02ee,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x02ef,   5 ),
	CG_NEXT( pl0b_crouch_p_02 ),
};

/* Char_Crouch_P 5 */
const Sint32 pl0b_crouch_p_05[] = {
	CG_FRAME(    0, 0x80,   8,           0,   290, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0336,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   290, 0x90,   0x89, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0336,   5 ),
	CG_FRAME( 0x80, 0x82,   2,           0,   291, 0x90,   0x89,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x0337,  30 ),
	CG_FRAME( 0x81, 0x83,   2,           0,   292, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0338,  31 ),
	CG_FRAME(    1, 0x85,   1,           0,   292, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0338,  78 ),
	CG_FRAME(    1, 0x86,   3,           0,   291, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0337,  78 ),
	CG_FRAME(    1, 0x86,   8,           0,   291, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0337,  78 ),
	CG_FRAME(    1, 0x88,   5,           0,   293, 0x90,   0x6b,        ATK,  0x00,    0,    0,   0,     0, 0x0339,  78 ),
	CG_FRAME(    1, 0x89,   4,           0,   294, 0x90,   0x6b,        ATK,  0x00,    0,    0,   0,     0, 0x033a,   5 ),
	CG_FRAME(    1, 0x89,   2,           0,   295, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0040,   5 ),
};

const Sint32 pl0b_crouch_p_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0041,   5 ),
	CG_NEXT( pl0b_crouch_p_05_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl0b_crouch_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   302, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x0042,   5 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x90,   0x40,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0042,   5 ),
	CG_FRAME( 0x81,    0,   2,           0,   303, 0x90,   0xaa,        ATK,  0x00,    0,    0,   0,     0, 0x0044,  16 ),
	CG_FRAME(    0,    0,   5,           0,   303, 0x90,   0xaa,        ATE,  0x00,    0,    0,   0,     0, 0x0044,  88 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0045,   5 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   304, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0046,   5 ),
	CG_NEXT( pl0b_crouch_k_00 ),
};

/* Char_Crouch_K 3 */
const Sint32 pl0b_crouch_k_03[] = {
	CG_FRAME(    0,    0,   1,           0,   302, 0x90,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x0042,   5 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x90,   0x40,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0042,   5 ),
	CG_FRAME( 0x81,    0,   2,           0,   303, 0x90,   0xaa,        ATK,  0x00,    0,    0,   0,     0, 0x0044,  16 ),
	CG_FRAME(    0,    0,   5,           0,   303, 0x90,   0xaa,        ATE,  0x00,    0,    0,   0,     0, 0x0044,  88 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0045,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   304, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0046,   5 ),
	CG_NEXT( pl0b_crouch_k_03 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl0b_crouch_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   305, 0x90,   0x44,        ATK,  0x00,    0,    0,   0,     0, 0x0047,   5 ),
	CG_FRAME(    0,    0,   5,           0,   305, 0x90,   0x44,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x0047,   5 ),
	CG_FRAME( 0x81,    0,   2,           0,   306, 0x90,   0xe1,        ATK,  0x00,    0,    0,   0,     0, 0x0048,  17 ),
	CG_FRAME(    0,    0,   9,           0,   306, 0x90,   0xe1,        ATE,  0x00,    0,    0,   0,     0, 0x0048,  73 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x90,   0x45,        ATE,  0x00,    0,    0,   0,     0, 0x004a,   5 ),
	CG_FRAME(    0,    0,   3,           0,   308, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x004b,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x004c,   5 ),
	CG_NEXT( pl0b_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl0b_crouch_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   310, 0x70,   0x74,        ATK,  0x00,    0,    0,   0,     0, 0x004d,   5 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x70,   0x74,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x004d,   5 ),
	CG_FRAME(    0,    0,   5,           0,   311, 0x70,   0x61,        ATK,  0x00,    0,    0,   0,     0, 0x004e,   5 ),
	CG_FRAME( 0x81,    0,   8,           0,   312, 0x90,   0xb0,        ATK,  0x00,    0,    0,   0,     0, 0x004f,  18 ),
	CG_FRAME(    0,    0,   8,           0,   313, 0x90,   0x63,        ATE,  0x00,    0,    0,   0,     0, 0x0050,  94 ),
	CG_FRAME(    0,    0,   8,           0,   314, 0x90,   0x48,        ATE,  0x00,    0,    0,   0,     0, 0x0051,   5 ),
	CG_FRAME(    0,    0,   8,           0,   315, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0052,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x004c,   5 ),
	CG_NEXT( pl0b_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl0b_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   316, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0054,   6 ),
	CG_FRAME(    0,    0,   2,           0,   316, 0x90,   0x20,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0054,   6 ),
	CG_FRAME( 0x80,    0,   6,           0,   317, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0057,  19 ),
	CG_FRAME( 0x81,    0,   6,           0,   317, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0057,  70 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x005f,   6 ),
	CG_NEXT( pl0b_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl0b_jump_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   319, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02f1,   6 ),
	CG_FRAME(    0,    0,   6,           0,   319, 0x90,   0x20,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x02f1,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   323, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02f2,  20 ),
	CG_FRAME( 0x80,    0,   2,           0,   324, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02f3,  20 ),
	CG_FRAME( 0x81,    0,   4,           0,   325, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02f4,  71 ),
	CG_FRAME(    0,    0,   6,           0,   325, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02f4,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02f6,   6 ),
	CG_NEXT( pl0b_jump_p_01 ),
};

/* Char_Jump_P 6 */
const Sint32 pl0b_jump_p_06[] = {
	CG_FRAME(    0, 0x80,   1,           0,   319, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   6,           0,   319, 0x90,   0x20,    ATK | 6,  0x00, 0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80, 0x81,   1,           0,   320, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x80, 0x82,   2,           0,   321, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x81, 0x83,   2,           0,   326, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0, 0x85,   4,           0,   322, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0, 0x85,   6,           0,   322, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x023a,   6 ),
	CG_NEXT( pl0b_jump_p_06 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl0b_jump_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   336, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02f7,   6 ),
	CG_FRAME(    0,    0,   3,           0,   337, 0x90,   0x20,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x02f8,   6 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02f9,  21 ),
	CG_FRAME(    0,    0,   2,           0,   341, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02fa,   6 ),
	CG_FRAME(    0,    0,   1,           0,   340, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02fb,   6 ),
	CG_FRAME(    0,    0,   3,           0,   339, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02fc,   6 ),
	CG_FRAME(    0,    0,   6,           0,   339, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02fe,   6 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x02ff,   6 ),
	CG_NEXT( pl0b_jump_p_02 ),
};

/* Char_Jump_P 7 */
const Sint32 pl0b_jump_p_07[] = {
	CG_FRAME(    0, 0x80,   1,           0,   327, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x80,   7,           0,   327, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x81,   2,           0,   328, 0x90,   0x20, ATK | 0x16,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0, 0x82,   2,           0,   329, 0x90,   0x20,    ATK | 7,  0x00, 0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80, 0x83,   2,           0,   330, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x81, 0x84,   2,           0,   331, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0, 0x85,   1,           0,   332, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0, 0x86,   1,           0,   333, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0, 0x87,   1,           0,   334, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0, 0x89,  12,           0,   335, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0, 0x89,   6,           0,   335, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
};

const Sint32 pl0b_jump_p_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x020e,   6 ),
	CG_NEXT( pl0b_jump_p_07_loop ),
};

/* Char_Jump_K 0 (light) */
const Sint32 pl0b_jump_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   342, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0060,   6 ),
	CG_FRAME(    0,    0,   3,           0,   342, 0x90,   0x20,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0060,   6 ),
	CG_FRAME( 0x81,    0,   9,           0,   343, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0062,  22 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0063,   6 ),
	CG_NEXT( pl0b_jump_k_00 ),
};

/* Char_Jump_K 1 (medium) */
const Sint32 pl0b_jump_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   344, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0064,   6 ),
	CG_FRAME(    0,    0,   6,           0,   344, 0x90,   0x20,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x0064,   6 ),
	CG_FRAME( 0x81,    0,   7,           0,   345, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0065,  23 ),
	CG_FRAME(    0,    0,   8,           0,   345, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0065,  72 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   344, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0067,   6 ),
	CG_NEXT( pl0b_jump_k_01 ),
};

/* Char_Extra 6: t_kick_init */
const Sint32 pl0b_extra_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   189, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x030a,  64 ),
	CG_NEXT( pl0b_extra_06 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl0b_jump_k_02[] = {
	CG_FRAME(    0, 0x80,   2,           0,    98, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x00c2,   6 ),
	CG_FRAME(    0,    0,   2,           0,   344, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0064,   6 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x90,   0x20,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x0064,   6 ),
	CG_FRAME(    1,    0,   6,           0,   309, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x005a,  36 ),
	CG_FRAME(    1,    0,  12,           0,   309, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x005a,  79 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   344, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0067,   6 ),
	CG_NEXT( pl0b_jump_k_02 ),
};

/* Char_Jump_K 3 */
const Sint32 pl0b_jump_k_03[] = {
	CG_FRAME(    0,    0,   2,           0,   346, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0068,   3 ),
	CG_FRAME(    0,    0,   7,           0,   347, 0x90,   0x20,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x0069,   3 ),
	CG_FRAME( 0x81,    0,   5,           0,   348, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x006a,  37 ),
	CG_FRAME(    0,    0,  12,           0,   348, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x006c,  82 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   318, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x006d,   6 ),
	CG_NEXT( pl0b_jump_k_03 ),
};

/* Char_Jump_K 4 */
const Sint32 pl0b_jump_k_04[] = {
	CG_FRAME(    0,    0,   8,           0,   349, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x006e,   3 ),
	CG_FRAME(    0,    0,   2,           0,   350, 0x90,   0x20,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x006f,   3 ),
	CG_FRAME( 0x80,    0,   8,           0,   351, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0071,  24 ),
	CG_FRAME( 0x80,    0,   8,           0,   351, 0x90,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0071,  24 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0073,   6 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   344, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0067,   6 ),
	CG_NEXT( pl0b_jump_k_04 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl0b_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   474, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   475, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e1,   1 ),
	CG_NEXT( pl0b_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl0b_damage_01[] = {
	CG_FRAME(    0,    0,  10,           0,   476, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e5,   1 ),
	CG_FRAME(    0,    0,   2,           0,   477, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e6,   1 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e7,   1 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e8,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   480, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e9,   1 ),
	CG_NEXT( pl0b_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl0b_damage_02[] = {
	CG_FRAME(    0,    0,   8,           0,   481, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00ee,   1 ),
	CG_FRAME(    0,    0,   3,           0,   482, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00ef,   1 ),
	CG_FRAME(    0,    0,   3,           0,   483, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00f0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x00f1,   1 ),
	CG_FRAME(    0,    0,   2,           0,    46, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f3,   1 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f2,   1 ),
	CG_FRAME(    0,    0,   2,           0,    65, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f5,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    92, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f6,   1 ),
	CG_NEXT( pl0b_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl0b_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   486, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f8,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   487, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f9,   1 ),
	CG_NEXT( pl0b_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl0b_damage_04[] = {
	CG_FRAME(    0,    0,   8,           0,   488, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00fd,   1 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x55,          0,  0x00,    0,    0,   0,     0, 0x00ff,   1 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   2,           0,    65, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0158,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    92, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0101,   1 ),
	CG_NEXT( pl0b_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl0b_damage_05[] = {
	CG_FRAME(    0,    0,  12,           0,   489, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    92, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0109,   1 ),
	CG_NEXT( pl0b_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl0b_damage_06[] = {
	CG_FRAME( 0x81,    0,   7,           0,   493, 0x80,   0x56,          0,  0x00,    0,    0,   0,     0, 0x010b,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x010d,   2 ),
	CG_NEXT( pl0b_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl0b_damage_07[] = {
	CG_FRAME(    0,    0,  12,           0,   494, 0x80,   0x54,          0,  0x00,    0,    0,   0,     0, 0x010f,   2 ),
	CG_FRAME(    0,    0,   3,           0,   495, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0110,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   496, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0111,   2 ),
	CG_NEXT( pl0b_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl0b_damage_08[] = {
	CG_FRAME(    0,    0,  16,           0,   497, 0x80,   0x52,          0,  0x00,    0,    0,   0,     0, 0x0115,   2 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x52,          0,  0x00,    0,    0,   0,     0, 0x0116,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   496, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0117,   2 ),
	CG_NEXT( pl0b_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl0b_damage_09[] = {
	CG_FRAME(    0,    0,   3,           0,   486, 0x40,   0x13,          0,  0x00,    0,    0,   0,     0, 0x00f8,   0 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0127,   0 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0128,   0 ),
	CG_FRAME(    0,    0,   3,           0,   505, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0129,   0 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x012a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   507, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x012b,   0 ),
	CG_FRAME(    0,    0,   3,           0,   508, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x012c,   0 ),
};

const Sint32 pl0b_damage_09_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   515, 0x40,   0xec,          0,  0x00,    0,    0,   0,     0, 0x012d,  58 ),
	CG_NEXT( pl0b_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl0b_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   499, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x011c,   3 ),
	CG_NEXT( pl0b_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl0b_damage_11[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   500, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x011f,   3 ),
	CG_NEXT( pl0b_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl0b_damage_12[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   501, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0122,   3 ),
	CG_NEXT( pl0b_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl0b_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   502, 0x80,   0xa0,          0,  0x00,    0,    0,   0,     0, 0x0124,  58 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   515, 0x80,   0xe0,          0,  0x00,    0,    0,   0,     0, 0x012d,  58 ),
	CG_NEXT( pl0b_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl0b_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   509, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x012e,  59 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   510, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x012f,  59 ),
	CG_NEXT( pl0b_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl0b_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   511, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0130,  59 ),
	CG_NEXT( pl0b_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl0b_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   512, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0132,  59 ),
	CG_NEXT( pl0b_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl0b_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   513, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0134,  59 ),
	CG_NEXT( pl0b_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl0b_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   514, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0136,  59 ),
	CG_NEXT( pl0b_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl0b_damage_21[] = {
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x012e,  59 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x012f,  59 ),
};

/* Char_Damage 22 */
const Sint32 pl0b_damage_22[] = {
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0130,  59 ),
};

/* Char_Damage 23 */
const Sint32 pl0b_damage_23[] = {
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0132,  59 ),
};

/* Char_Damage 24 */
const Sint32 pl0b_damage_24[] = {
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0134,  59 ),
};

/* Char_Damage 25 */
const Sint32 pl0b_damage_25[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   514, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0136,  59 ),
	CG_NEXT( pl0b_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl0b_damage_26[] = {
	CG_FRAME(    0,    0,   6,           0,   515, 0x80,   0xed,          0,  0x00,    0,    0,   0,     0, 0x0138,  58 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   516, 0x80,   0xed,          0,  0x00,    0,    0,   0,     0, 0x0139,  58 ),
	CG_NEXT( pl0b_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl0b_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,    60, 0x40,   0x0c,          0,  0x00,    0,    0,   0,     0, 0x00a8,   3 ),
	CG_FRAME(    0,    0,   4,           0,    61, 0x40,   0x01,          0,  0x00,    0,    0,   0,     0, 0x00a9,   3 ),
	CG_FRAME(    0,    0,   4,           0,    62, 0x40,   0x5f,          0,  0x00,    0,    0,   0,     0, 0x00ab,   3 ),
	CG_FRAME(    0,    0,   4,           0,    63, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x00ac,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    64, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00ad,   3 ),
	CG_NEXT( pl0b_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl0b_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   522, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0140,   3 ),
	CG_FRAME(    0,    0,   4,           0,   523, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0141,   3 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0142,   3 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0143,   3 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0144,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   527, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0145,   3 ),
	CG_NEXT( pl0b_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl0b_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   528, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0147,   3 ),
	CG_FRAME(    0,    0,   4,           0,   529, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0148,   3 ),
	CG_FRAME(    0,    0,   4,           0,   530, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0149,   3 ),
	CG_FRAME(    0,    0,   4,           0,   531, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x014a,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   532, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x014b,   3 ),
	CG_NEXT( pl0b_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl0b_damage_30[] = {
	CG_FRAME(    0,    0,   3,           0,   517, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013a,   3 ),
	CG_FRAME(    0,    0,   3,           0,   518, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013b,   3 ),
	CG_FRAME(    0,    0,   3,           0,   519, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013c,   3 ),
	CG_FRAME(    0,    0,   4,           0,   520, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013d,   3 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013e,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   521, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013f,   3 ),
	CG_NEXT( pl0b_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl0b_damage_31[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   515, 0x80,   0xe0,          0,  0x00,    0,    0,   0,     0, 0x012d,  58 ),
	CG_NEXT( pl0b_damage_31 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl0b_damage_32[] = {
	CG_FRAME(    0,    0,   3,           0,   517, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013a,   3 ),
	CG_FRAME(    0,    0,   3,           0,   518, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013b,   3 ),
	CG_FRAME(    0,    0,   3,           0,   519, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013c,   3 ),
	CG_FRAME(    0,    0,   4,           0,   520, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013d,   3 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013e,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   521, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x013f,   3 ),
	CG_NEXT( pl0b_damage_32 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl0b_dizzy_00[] = {
	CG_FRAME(    0,    0,   7,           0,   533, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01df,   1 ),
	CG_FRAME(    0,    0,   7,           0,   534, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01dd,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   535, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01de,   1 ),
	CG_NEXT( pl0b_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl0b_catch_00[] = {
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x4e,          0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,  15,           0,   353, 0x80,   0x4e,          5,  0x00, 0x03,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   6,           0,   354, 0x80,   0x4e,          0,  0x00,    0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   2,           0,   355, 0x80,   0x4e,          0,  0x00,    0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   6,           0,   356, 0x80,   0x4e,          0,  0x00,    0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x40,          7,  0x00, 0x05,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    1,  20,           0,   357, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,   8,           0,   358, 0x80,   0x52,          0,  0x00,    0,    0,   0,     0, 0x0007,   1 ),
};

const Sint32 pl0b_catch_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   359, 0x80,   0x0f,          0,  0x00,    0,    0,   0,     0, 0x0008,   1 ),
	CG_NEXT( pl0b_catch_00_loop ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl0b_catch_01[] = {
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x6a,          0,  0x00,    0,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    0,  15,           0,   360, 0x80,   0x6a,          5,  0x00, 0x03,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0x6a,          0,  0x00,    0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000b,   1 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000b,   1 ),
	CG_FRAME(    0,    0,   8,           0,   363, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x000c,   1 ),
	CG_FRAME(    0,    1,   4,           0,   364, 0x80,   0x40,          6,  0x00, 0x04,    0,   0,     0, 0x000d,   1 ),
	CG_FRAME(    0,    0,  16,           0,   364, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x000d,   1 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x80,   0x56,          0,  0x00,    0,    0,   0,     0, 0x000e,   1 ),
	CG_FRAME(    0,    0,   3,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x000f,   1 ),
	CG_FRAME(    0, 0x80,   3,           0,    33, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x003e,   1 ),
};

const Sint32 pl0b_catch_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   150, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0013,   1 ),
	CG_NEXT( pl0b_catch_01_loop ),
};

/* Char_Catch 2: jump_p_set */
const Sint32 pl0b_catch_02[] = {
	CG_FRAME(    0,    0,   1,           0,   123, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0014,   3 ),
	CG_FRAME(    0,    0,   2,           0,   123, 0x80,   0x20,          5,  0x00, 0x03,    0,   0,     0, 0x0014,   3 ),
	CG_FRAME(    0,    0,  10,           0,   423, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0015,   3 ),
	CG_FRAME(    0,    0,   3,           0,   423, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0015,   3 ),
	CG_FRAME(    0,    0,   3,           0,   424, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0016,   3 ),
	CG_FRAME(    0,    1,   3,           0,   425, 0x80,   0x20,          6,  0x00, 0x04,    0,   0,     0, 0x0017,   3 ),
	CG_FRAME(    0,    0,  10,           0,   425, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0017,   3 ),
};

const Sint32 pl0b_catch_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   318, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0018,   3 ),
	CG_NEXT( pl0b_catch_02_loop ),
};

/* Char_Catch 3: justice_land_ck (Final Justice) */
const Sint32 pl0b_catch_03[] = {
	CG_FRAME(    0,    0,  16,           0,   242, 0xc0,   0x6e,        ATK,  0x00,    0,    0,   0,     0, 0x001a,   4 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0xc0,   0x6e,        ATK,  0x00,    0,    0,   0,     0, 0x001b,   4 ),
	CG_FRAME(    0,    0,   1,           0,   448, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x001c,   4 ),
	CG_FRAME(    0,    1,   6,           0,   243, 0xc0,   0x8b,        ATK,  0x00,    0,    0,   0,     0, 0x001d,   4 ),
	CG_FRAME(    0,    2,   4,           0,   244, 0xc0,   0x8b,        ATK,  0x00,    0,    0,   0,     0, 0x001e,   4 ),
	CG_FRAME(    0,    3,   2,           0,   244, 0xc0,   0x8b,        ATK,  0x00,    0,    0,   0,     0, 0x001f,   4 ),
	CG_FRAME(    0,    4,   1,           0,   291, 0xc0,   0x89,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x0020,   4 ),
	CG_FRAME(    0, 0x84,  10,           0,   292, 0xc0,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0021,   4 ),
	CG_FRAME(    1,    3,   2,           0,   448, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0022,   4 ),
	CG_FRAME(    0,    3,   2, END_OF_DATA,   449, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0023,   4 ),
	CG_NEXT( pl0b_catch_03 ),
};

/* Char_Catch 4: justice_hit_wait (Final Justice) */
const Sint32 pl0b_catch_04[] = {
	CG_FRAME(    0, 0x80,   8,           0,   365, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0024,   6 ),
	CG_FRAME(    0, 0x80,   8,           0,   427, 0xb0,   0x20,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0025,   6 ),
};

const Sint32 pl0b_catch_04_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   426, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0026,   6 ),
	CG_NEXT( pl0b_catch_04_loop ),
};

/* Char_Caught 0 */
const Sint32 pl0b_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   474, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e0,  38 ),
	CG_NEXT( pl0b_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl0b_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   476, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00e5,  38 ),
	CG_NEXT( pl0b_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl0b_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   481, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00ee,  38 ),
	CG_NEXT( pl0b_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl0b_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   486, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00f8,  38 ),
	CG_NEXT( pl0b_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl0b_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   488, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00fd,  38 ),
	CG_NEXT( pl0b_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl0b_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   489, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0103,  38 ),
	CG_NEXT( pl0b_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl0b_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   493, 0x40,   0x56,          0,  0x00,    0,    0,   0,     0, 0x010b,  38 ),
	CG_NEXT( pl0b_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl0b_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   494, 0x40,   0x56,          0,  0x00,    0,    0,   0,     0, 0x010f,  38 ),
	CG_NEXT( pl0b_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl0b_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   497, 0x40,   0x56,          0,  0x00,    0,    0,   0,     0, 0x0115,  38 ),
	CG_NEXT( pl0b_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl0b_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   499, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x011c,  38 ),
	CG_NEXT( pl0b_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl0b_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   500, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x011f,  38 ),
	CG_NEXT( pl0b_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl0b_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   501, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0122,  38 ),
	CG_NEXT( pl0b_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl0b_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   502, 0x40,   0xa0,          0,  0x00,    0,    0,   0,     0, 0x0124,  38 ),
	CG_NEXT( pl0b_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl0b_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   509, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x012e,  38 ),
	CG_NEXT( pl0b_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl0b_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   511, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0130,  38 ),
	CG_NEXT( pl0b_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl0b_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   512, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0132,  38 ),
	CG_NEXT( pl0b_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl0b_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   513, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0134,  38 ),
	CG_NEXT( pl0b_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl0b_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   514, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0136,  38 ),
	CG_NEXT( pl0b_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl0b_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   510, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x012e,  38 ),
	CG_NEXT( pl0b_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl0b_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   514, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0146,  38 ),
	CG_NEXT( pl0b_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl0b_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   515, 0x40,   0xed,          0,  0x00,    0,    0,   0,     0, 0x012d,  38 ),
	CG_NEXT( pl0b_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl0b_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   508, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0, 0x012c,  38 ),
	CG_NEXT( pl0b_caught_21 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl0b_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   145, 0x40,   0xa0,          0,  0x00,    0,    0,   0,     0, 0x00c5,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x00c6,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x00c7,   1 ),
	CG_FRAME(    0,    0,   6,           0,   147, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x00c8,   1 ),
	CG_FRAME(    0,    0,   6,           0,   148, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x00c9,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   149, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00ca,   1 ),
	CG_NEXT( pl0b_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl0b_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   145, 0x40,   0xa0,          0,  0x00,    0,    0,   0,     0, 0x00cd,   0 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0257,   0 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00cf,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d1,   0 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d2,   0 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x00d3,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   149, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00d4,   0 ),
	CG_NEXT( pl0b_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl0b_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   145, 0x40,   0xa0,          0,  0x00,    0,    0,   0,     0, 0x00cd,   0 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0257,   0 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00cf,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d1,   0 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d2,   0 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00cf,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d1,   0 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d2,   0 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00cf,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d1,   0 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d2,   0 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00cf,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d1,   0 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x00d3,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   149, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x00d4,   0 ),
	CG_NEXT( pl0b_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl0b_get_up_03[] = {
	CG_FRAME(    0,    0,   4,           0,   145, 0x40,   0xa0,          0,  0x00,    0,    0,   0,     0, 0x0259,   0 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d7,   0 ),
	CG_FRAME(    0,    0,   4,           0,   158, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d8,   0 ),
	CG_FRAME(    0,    0,   4,           0,   159, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00d9,   0 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00da,   0 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x40,   0x40,          0,  0x00,    0,    0,   0,     0, 0x00dc,   0 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x40,   0x51,          0,  0x00,    0,    0,   0,     0, 0x025c,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   149, 0x40,   0x11,          0,  0x00,    0,    0,   0,     0, 0x025e,   0 ),
	CG_NEXT( pl0b_get_up_03 ),
};

/* Char_Extra 0: pl0b_shield_look */
const Sint32 pl0b_extra_00[] = {
	CG_FRAME(    0,    0,   4,           0,   467, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x02c1,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02c2,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02c9,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02c3,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02ca,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02c4,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02cb,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02c5,   1 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x6e,          0,  0x00,    0,    0,   0,     0, 0x02cc,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    89, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x02c8,   1 ),
	CG_NEXT( pl0b_stand_00 ),
};

/* Char_Extra 1: pl0b_shield_look */
const Sint32 pl0b_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x02ba,   1 ),
	CG_FRAME(    0,    0,   4,           0,   367, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x02bb,   1 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x02bc,   1 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x02bd,   1 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x02be,   1 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x02bf,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   151, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x02c0,   1 ),
	CG_NEXT( pl0b_stand_00 ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl0b_extra_03[] = {
	CG_FRAME(    0,    0,   6,           0,   190, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x031f,   1 ),
	CG_FRAME(    0,    0,   7,           0,   191, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x0320,   1 ),
	CG_FRAME(    0,    0,   7,           0,   192, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x0321,   1 ),
	CG_FRAME(    0,    0,  32,           0,   193, 0x80,   0x14,          8,  0x00, 0x10,    0,   0,     0, 0x0322,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   192, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x0325,   1 ),
	CG_NEXT( pl0b_extra_03 ),
};

/* Char_Extra 4 */
const Sint32 pl0b_extra_04[] = {
	CG_FRAME(    0,    0,   6,           0,   190, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x031f,   1 ),
	CG_FRAME(    0,    0,   7,           0,   191, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x0320,   1 ),
	CG_FRAME(    0,    0,   7,           0,   192, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x0321,   1 ),
	CG_FRAME(    0,    0,  32,           0,   193, 0x80,   0x14,          9,  0x00, 0x11,    0,   0,     0, 0x0322,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   192, 0x80,   0x14,          0,  0x00,    0,    0,   0,     0, 0x0322,   1 ),
	CG_NEXT( pl0b_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl0b_extra_05[] = {
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x026e,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    86, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x026f,   1 ),
	CG_NEXT( pl0b_extra_05 ),
};

/* Char_Extra 7: pl0b_upper */
const Sint32 pl0b_extra_07[] = {
	CG_FRAME(    0,    0,   1,           0,   296, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e4, 107 ),
	CG_FRAME(    0,    0,   6,           0,   296, 0x90,   0x8a,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x02e4, 107 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e5, 107 ),
	CG_FRAME( 0x80,    0,   1,           0,   298, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e6, 109 ),
	CG_FRAME( 0x80,    0,   2,           0,   299, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e7, 109 ),
	CG_FRAME( 0x81,    0,   2,           0,   300, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e8, 109 ),
	CG_FRAME(    0,    0,   5,           0,   301, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02e9, 108 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x90,   0x8a,        ATK,  0x00,    0,    0,   0,     0, 0x02eb, 108 ),
	CG_FRAME(    0,    0,   5,           0,   293, 0x90,   0x8a,        ATE,  0x00,    0,    0,   0,     0, 0x02ec, 107 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x90,   0x6a,        ATE,  0x00,    0,    0,   0,     0, 0x02ed,   5 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x02ee,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x02ef,   5 ),
	CG_NEXT( pl0b_extra_07 ),
};

/* Char_Extra 8: pl0b_upper */
const Sint32 pl0b_extra_08[] = {
	CG_FRAME(    0, 0x80,   8,           0,   290, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0336,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   290, 0x90,   0x89, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0336,   5 ),
	CG_FRAME( 0x80, 0x82,   2,           0,   291, 0x90,   0x89,    ATK | 7,  0x00, 0x05,    0,   0,     0, 0x0337, 110 ),
	CG_FRAME( 0x81, 0x83,   2,           0,   292, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0338, 111 ),
	CG_FRAME(    1, 0x85,   1,           0,   292, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0338, 112 ),
	CG_FRAME(    1, 0x86,   3,           0,   291, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0337, 112 ),
	CG_FRAME(    1, 0x86,   8,           0,   291, 0x90,   0x89,        ATK,  0x00,    0,    0,   0,     0, 0x0337, 112 ),
	CG_FRAME(    1, 0x88,   5,           0,   293, 0x90,   0x6b,        ATK,  0x00,    0,    0,   0,     0, 0x0339, 112 ),
	CG_FRAME(    1, 0x89,   4,           0,   294, 0x90,   0x6b,        ATK,  0x00,    0,    0,   0,     0, 0x033a,   5 ),
	CG_FRAME(    1, 0x89,   2,           0,   295, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0040,   5 ),
};

const Sint32 pl0b_extra_08_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0, 0x0041,   5 ),
	CG_NEXT( pl0b_extra_08_loop ),
};

/* Char_Extra 2: t_kick_move, fly_start5 */
const Sint32 pl0b_extra_02[] = {
	CG_FRAME(    0,    0,   1,           0,   123, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x0256,   1 ),
	CG_FRAME(    0,    0,   5,           0,   123, 0x80,   0x6d,          5,  0x00, 0x03,    0,   0,     0, 0x0256,   1 ),
	CG_FRAME(    0,    0,   2,           0,   124, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x0253,   1 ),
	CG_FRAME(    0,    0,   2,           0,   125, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x0254,   1 ),
	CG_FRAME(    0,    0,  24,           0,   124, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x0253,   1 ),
	CG_FRAME(    0,    0,   2,           0,   125, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x0254,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   123, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x0256,   1 ),
	CG_NEXT( pl0b_extra_02 ),
};

/* Char_Appear 3: appear_jump_init */
const Sint32 pl0b_appear_03[] = {
	CG_FRAME(    0,    1,  20,           0,   127, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e0,   1 ),
	CG_FRAME(    0, 0x80,   4,           0,    73, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e1,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e2,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e3,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e4,   1 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e5,   1 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e6,   1 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e7,   1 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e8,   1 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01e9,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01ea,   1 ),
};

const Sint32 pl0b_appear_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01eb,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01ec,   1 ),
	CG_NEXT( pl0b_appear_03_loop ),
};

/* Char_Appear 4: appear_jump_fall */
const Sint32 pl0b_appear_04[] = {
	CG_FRAME(    0,    1,   1,           0,   149, 0x80,   0x51,          0,  0x00,    0,    0,   0,     0, 0x01ed,   0 ),
	CG_FRAME(    0,    1,   3,           0,   149, 0x80,   0x51,       0x0c,  0x00,    0,    0,   0,     0, 0x01ed,   0 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01ee,   0 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x51,          0,  0x00,    0,    0,   0,     0, 0x01ef,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   150, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01f0,   0 ),
	CG_NEXT( pl0b_appear_04 ),
};

/* Char_Appear 0: appear_shield_init */
const Sint32 pl0b_appear_00[] = {
	CG_FRAME(    0,    0,  16,           0,   178, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x01f1,   0 ),
	CG_FRAME(    0,    0,  12,           0,   179, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x01f2,   0 ),
	CG_FRAME(    0,    0,  16,           0,   180, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x01f3,   0 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   179, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x01f4,   0 ),
	CG_NEXT( pl0b_appear_00 ),
};

/* Char_Appear 1: appear_shield_keep */
const Sint32 pl0b_appear_01[] = {
	CG_FRAME(    0, 0x80,   2,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01f5,   1 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01f6,   1 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01f7,   1 ),
	CG_FRAME(    0,    0,   4,           0,   422, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0187,   1 ),
	CG_FRAME(    0,    0,   6,           0,   399, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x017b,   1 ),
	CG_FRAME(    0,    0,   4,           0,   419, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x017d,   1 ),
	CG_FRAME(    0,    0,   6,           0,   422, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x017e,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01fc,   1 ),
};

const Sint32 pl0b_appear_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01fd,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    70, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x01fe,   1 ),
	CG_NEXT( pl0b_appear_01_loop ),
};

/* Char_Appear 2: appear_shield_jump */
const Sint32 pl0b_appear_02[] = {
	CG_FRAME(    0,    0,   2,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01ff,   1 ),
	CG_FRAME(    0,    0,   2,           0,    65, 0x80,   0x00,          0,  0x00, 0x0a,    0,   0,     0, 0x01ff,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0200,   1 ),
	CG_NEXT( pl0b_appear_02 ),
};

/* Char_Win 0 */
const Sint32 pl0b_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01c7,   1 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01c8,   1 ),
	CG_FRAME(    0,    0,  48,           0,   102, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01c9,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01ca,   1 ),
	CG_FRAME(    0,    0,   8,           0,   104, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01cb,   1 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x11,          9,  0x00, 0x11,    0,   0,     0, 0x01cc,   1 ),
};

const Sint32 pl0b_win_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   106, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x01cd,   1 ),
	CG_NEXT( pl0b_win_00_loop ),
};

/* Char_Win 1
   Char_Win 2 */
const Sint32 pl0b_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01ce,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x15,          0,  0x00,    0,    0,   0,     0, 0x01cf,   1 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d0,   1 ),
};

/* Char_Win 6 */
const Sint32 pl0b_win_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   109, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d0,   1 ),
	CG_NEXT( pl0b_win_06 ),
};

/* Char_Effect 29: pls0b_00_win_cross_init */
const Sint32 pl0b_effect_29[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_29 ),
};

/* Char_Win 5 */
const Sint32 pl0b_win_05[] = {
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d1,   1 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d2,   1 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d3,   1 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d4,   1 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d5,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d6,   1 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d7,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   117, 0x80,   0x16,          0,  0x00,    0,    0,   0,     0, 0x01d8,   1 ),
	CG_NEXT( pl0b_win_05 ),
};

/* Char_Win 4 */
const Sint32 pl0b_win_04[] = {
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x01d9,   1 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x01da,   1 ),
	CG_FRAME(    0, 0x80,  60,           0,   125, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x01db,   1 ),
	CG_FRAME(    0, 0x80,   4,           0,   125, 0x80,   0x6d,       0x0a,  0x00, 0x12,    0,   0,     0, 0x01db,   1 ),
};

const Sint32 pl0b_win_04_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   125, 0x80,   0x6d,          0,  0x00,    0,    0,   0,     0, 0x01db,   1 ),
	CG_NEXT( pl0b_win_04_loop ),
};

/* Char_Win 7 */
const Sint32 pl0b_win_07[] = {
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0314,   1 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0315,   1 ),
	CG_FRAME(    0,    0,  16,           0,   169, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0316,   1 ),
	CG_FRAME(    0,    0,   6,           0,   170, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0317,   1 ),
	CG_FRAME(    0,    0,   6,           0,   171, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0318,   1 ),
	CG_FRAME(    0,    0,   6,           0,   172, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0319,   1 ),
	CG_FRAME(    0,    0,  44,           0,   173, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x031a,   1 ),
	CG_FRAME(    0,    0,   6,           0,   174, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x031b,   1 ),
	CG_FRAME(    0,    0,   8,           0,   175, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x031c,   1 ),
	CG_FRAME(    0,    0,   8,           0,   176, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x031d,   1 ),
	CG_FRAME(    0,    0,  16,           0,   177, 0x80,   0x11,       0x0b,  0x00, 0x0f,    0,   0,     0, 0x031e,   1 ),
};

const Sint32 pl0b_win_07_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   177, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x031e,   1 ),
	CG_NEXT( pl0b_win_07_loop ),
};

/* Char_Win 8: pls0b_00_win_join_init */
const Sint32 pl0b_win_08[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   934, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_win_08 ),
};

/* Char_Win 9: pls0b_00_win_join_fly */
const Sint32 pl0b_win_09[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA,   935, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_win_09 ),
};

/* Char_Win 10: pls0b_00_win_join_glide */
const Sint32 pl0b_win_10[] = {
	CG_FRAME(    0,    0,   6,           0,   936, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0b_win_10_loop[] = {
	CG_FRAME(    0,    0,  10,           0,   937, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   938, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   939, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   940, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   941, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_win_10_loop ),
};

/* Char_Win 11: pls0b_00_win_join_brake */
const Sint32 pl0b_win_11[] = {
	CG_FRAME(    0,    0,   6,           0,   942, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   943, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   944, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   943, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   945, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   943, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   946, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   947, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   948, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   949, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   950, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0b_win_11_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   951, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_win_11_loop ),
};

/* Char_Effect 30: pls0b_00_win_shield_init */
const Sint32 pl0b_effect_30[] = {
	CG_FRAME(    0,    0,   2,           0,   849, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   850, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   851, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   852, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   853, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   854, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   855, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   856, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   857, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_30 ),
};

/* Char_Effect 31 */
const Sint32 pl0b_effect_31[] = {
	CG_FRAME(    0,    0,   4,           0,   867, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   868, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   867, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   869, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   867, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   870, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_31 ),
};

/* Char_Effect 32 */
const Sint32 pl0b_effect_32[] = {
	CG_FRAME(    0,    0,   2,           0,   871, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   872, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   873, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_32 ),
};

/* Char_Effect 33 */
const Sint32 pl0b_effect_33[] = {
	CG_FRAME(    0,    0,   2,           0,   874, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   875, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   876, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl0b_effect_34[] = {
	CG_FRAME(    0,    0,   2,           0,   877, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   878, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   879, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_34 ),
};

/* group 28 0 */
const Sint32 pl0b_grp28_00[] = {
	CG_FRAME(    0,    0,   4,           0,   666, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   667, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   668, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   669, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   670, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   671, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   672, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   673, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   674, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   675, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   676, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   677, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   679, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   680, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   681, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   682, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   683, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   684, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   685, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   686, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   687, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   688, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   689, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_grp28_00 ),
};

/* group 28 1 */
const Sint32 pl0b_grp28_01[] = {
	CG_FRAME(    0,    0,   4,           0,   785, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   785, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   786, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   787, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   788, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   789, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4, END_OF_DATA,   790, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_grp28_01 ),
};

/* Char_Effect 35 */
const Sint32 pl0b_effect_35[] = {
	CG_FRAME(    0,    0,   4,           0,   880, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   881, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   882, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   883, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   884, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   885, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   884, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   883, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   882, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   881, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_35 ),
};

/* Char_Effect 36 */
const Sint32 pl0b_effect_36[] = {
	CG_FRAME(    0,    0,   4,           0,   893, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   894, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   895, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   896, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   897, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   898, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   897, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   896, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   895, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   894, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_36 ),
};

/* Char_Effect 37 */
const Sint32 pl0b_effect_37[] = {
	CG_FRAME(    0,    0,   4,           0,   886, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   887, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   888, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   889, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   890, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   891, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   892, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   891, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   890, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   889, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   888, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   887, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_37 ),
};

/* Char_Effect 38 */
const Sint32 pl0b_effect_38[] = {
	CG_FRAME(    0,    0,   4,           0,   899, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   900, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   901, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   902, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   903, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   904, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   903, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   902, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   901, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   900, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_38 ),
};

/* Char_Effect 39: pls0b_00_charge_init */
const Sint32 pl0b_effect_39[] = {
	CG_FRAME(    1,    0,   1,           0,   845, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   845, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   846, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   847, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   848, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_39 ),
};

/* Char_Effect 41 */
const Sint32 pl0b_effect_41[] = {
	CG_FRAME(    0,    0,   1,           0,   915, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   913, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0b_effect_41_loop[] = {
	CG_FRAME(    0,    0,   1,           0, 32780, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32777, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32778, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32779, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_41_loop ),
};

/* Char_Effect 42 */
const Sint32 pl0b_effect_42[] = {
	CG_FRAME(    0,    0,   1,           0,   916, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   914, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0b_effect_42_loop[] = {
	CG_FRAME(    0,    0,   1,           0, 32784, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32781, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32782, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32783, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_42_loop ),
};

/* Char_Effect 43: pls0b_00_hcharge_follow */
const Sint32 pl0b_effect_43[] = {
	CG_FRAME(    0,    0,   4,           0, 32785, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32786, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32787, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   921, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_43 ),
};

/* Char_Effect 40: pls0b_00_hstars_init */
const Sint32 pl0b_effect_40[] = {
	CG_FRAME(    0,    0,   1,           0, 32772, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    1,   1,           0, 32773, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    1,   1,           0, 32774, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    8,    1,   1,           0, 32775, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32776, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_40 ),
};

/* Char_Win 3 */
const Sint32 pl0b_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,   126, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01dc,   1 ),
	CG_FRAME(    0,    0,   4,           0,   534, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01dd,   1 ),
	CG_FRAME(    0,    0,   4,           0,   535, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01de,   1 ),
};

const Sint32 pl0b_win_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   533, 0x40,   0x52,          0,  0x00,    0,    0,   0,     0, 0x01df,   1 ),
	CG_NEXT( pl0b_win_03_loop ),
};

/* Char_Special 0 */
const Sint32 pl0b_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   378, 0xb0,   0x6c,        ATK,  0x00,    0,    0,   0,     0, 0x0159,   4 ),
	CG_FRAME(    0,    0,   6,           0,   379, 0xb0,   0x6c,    ATK | 1,  0x00, 0x16,    0,   0,     0, 0x015a,   4 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x015b,   4 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x015c,   4 ),
	CG_FRAME(    0,    0,   5,           0,   382, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x015d,   4 ),
	CG_FRAME(    0,    0,   5,           0,   383, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x015e,  44 ),
	CG_FRAME(    1,    0,   2,           0,   384, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x015f,   4 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x0161,   4 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x0161,   4 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x0161,   4 ),
	CG_FRAME(    0,    0,   8,           0,   388, 0xb0,   0x8c,        ATK,  0x00,    0,    0,   0,     0, 0x0161,   4 ),
	CG_FRAME(    0,    0,   8,           0,   388, 0xb0,   0x8c,        ATE,  0x00,    0,    0,   0,     0, 0x0161,   4 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0xb0,   0x4e,        ATE,  0x00,    0,    0,   0,     0, 0x0308,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   150, 0xb0,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x0309,   4 ),
	CG_NEXT( pl0b_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl0b_special_01[] = {
	CG_FRAME(    0,    0,   3,           0,   438, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0169,   4 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0xb0,   0x00,    ATK | 1,  0x00, 0x16,    0,   0,     0, 0x0169,   4 ),
	CG_FRAME(    0,    0,   3,           0,   440, 0xb0,   0x74,        ATK,  0x00,    0,    0,   0,     0, 0x016a,   4 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016b,   4 ),
	CG_FRAME(    1,    0,   2,           0,   442, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016c,   4 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016d,   4 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016d,   4 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016d,   4 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016d,   4 ),
	CG_FRAME(    0,    0,   6,           0,   447, 0xb0,   0x73,        ATK,  0x00,    0,    0,   0,     0, 0x016d,   4 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0xb0,   0x73,        ATE,  0x00,    0,    0,   0,     0, 0x016d,   4 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x016e,   4 ),
	CG_FRAME(    0,    0,   4,           0,   449, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x016f,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0xb0,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x0170,   4 ),
	CG_NEXT( pl0b_special_01 ),
};

/* Char_Special 3 */
const Sint32 pl0b_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   401, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0180,   6 ),
	CG_FRAME(    0,    0,   4,           0,   402, 0xb0,   0x20,    ATK | 1,  0x00, 0x16,    0,   0,     0, 0x0180,   6 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0181,   6 ),
	CG_FRAME(    0,    0,   2,           0,   404, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0182,   6 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0183,  51 ),
	CG_FRAME(    1,    0,   1,           0,   406, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   1,           0,   407, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   1,           0,   408, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   1,           0,   409, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   4,           0,   410, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   8,           0,   411, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   8,           0,   411, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0184,   6 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0186,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   422, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0187,   6 ),
	CG_NEXT( pl0b_special_03 ),
};

/* Char_Special 2 */
const Sint32 pl0b_special_02[] = {
	CG_FRAME(    0,    0,   4,           0,   401, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0188,   6 ),
	CG_FRAME(    0,    0,   4,           0,   402, 0xb0,   0x20,    ATK | 1,  0x00, 0x16,    0,   0,     0, 0x0188,   6 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0189,   6 ),
	CG_FRAME(    0,    0,   2,           0,   404, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018a,   6 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018b,  52 ),
	CG_FRAME(    1,    0,   2,           0,   413, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,   2,           0,   416, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,   4,           0,   417, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,   4,           0,   418, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,  12,           0,   418, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x018c,   6 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x018d,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   422, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x018f,   6 ),
	CG_NEXT( pl0b_special_02 ),
};

/* Char_Special 4 */
const Sint32 pl0b_special_04[] = {
	CG_FRAME(    0,    0,   1,           0,    59, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01a2,  96 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x50,   0x00,          7,  0x00, 0x05,    0,   0,     0, 0x01a2,  96 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01a3,  96 ),
	CG_FRAME(    0,    0,   3,           0,    61, 0x50,   0x41,          0,  0x00,    0,    0,   0,     0, 0x01a4,  96 ),
	CG_FRAME(    0,    0,   3,           0,   461, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01a5,  96 ),
	CG_FRAME(    0,    0,   3,           0,   462, 0x50,   0x1c,          0,  0x00,    0,    0,   0,     0, 0x01a6,  96 ),
	CG_FRAME(    0,    0,   3,           0,   463, 0x50,   0x5f,          0,  0x00,    0,    0,   0,     0, 0x01a7,  96 ),
	CG_FRAME(    0,    0,   4,           0,   464, 0x50,   0x01,          0,  0x00,    0,    0,   0,     0, 0x01a8,  96 ),
	CG_FRAME(    0,    0,   5,           0,   465, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0, 0x01a9,  96 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   466, 0x50,   0x4e,          0,  0x00,    0,    0,   0,     0, 0x01aa,   1 ),
	CG_NEXT( pl0b_special_04 ),
};

/* Char_Special 5 */
const Sint32 pl0b_special_05[] = {
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52,        ATK,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    1,    9,   4,           0,   371, 0xb0,   0x40,    ATK | 2,  0x00, 0x17,    0,   0,     0, 0x0191,  97 ),
	CG_FRAME(    2,   10,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192,  98 ),
	CG_FRAME(    3, 0x8b,   2,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  99 ),
	CG_FRAME(    3, 0x8c,   8,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  99 ),
	CG_FRAME(    3,    0,   2,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0194, 102 ),
	CG_FRAME(    4,    0,   5,           0,   374, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0195, 102 ),
	CG_FRAME(    5,    0,   4,           0,   375, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0196, 102 ),
};

const Sint32 pl0b_special_05_loop[] = {
	CG_FRAME(    6,    0,   2,           0,   376, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0197, 102 ),
	CG_FRAME(    7,    0,   2, END_OF_DATA,   377, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0198, 102 ),
	CG_NEXT( pl0b_special_05_loop ),
};

/* Char_Special 6 */
const Sint32 pl0b_special_06[] = {
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52,        ATK,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    1,   13,   4,           0,   371, 0xb0,   0x40,    ATK | 2,  0x00, 0x17,    0,   0,     0, 0x0191,  97 ),
	CG_FRAME(    2,   14,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192,  98 ),
	CG_FRAME(    3, 0x8f,   2,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  99 ),
	CG_FRAME(    3, 0x90,  16,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  99 ),
	CG_FRAME(    3,    0,   2,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0194, 102 ),
	CG_FRAME(    4,    0,   6,           0,   374, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0195, 102 ),
	CG_FRAME(    5,    0,   4,           0,   375, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0196, 102 ),
};

const Sint32 pl0b_special_06_loop[] = {
	CG_FRAME(    6,    0,   2,           0,   376, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0197, 102 ),
	CG_FRAME(    7,    0,   2, END_OF_DATA,   377, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0198, 102 ),
	CG_NEXT( pl0b_special_06_loop ),
};

/* Char_Special 9 */
const Sint32 pl0b_special_09[] = {
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52,        ATK,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    1,    1,   4,           0,   371, 0xb0,   0x40,    ATK | 2,  0x00, 0x17,    0,   0,     0, 0x0191,  53 ),
	CG_FRAME(    2,    2,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192,  54 ),
	CG_FRAME(    3, 0x83,   2,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  55 ),
	CG_FRAME(    3, 0x84,   8,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  55 ),
	CG_FRAME(    3,    0,   2,           0,   373, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0194, 102 ),
	CG_FRAME(    4,    0,   5,           0,   374, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0195, 102 ),
	CG_FRAME(    5,    0,   4,           0,   375, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0196, 102 ),
};

const Sint32 pl0b_special_09_loop[] = {
	CG_FRAME(    6,    0,   2,           0,   376, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0197, 102 ),
	CG_FRAME(    7,    0,   2, END_OF_DATA,   377, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0198, 102 ),
	CG_NEXT( pl0b_special_09_loop ),
};

/* Char_Special 10 */
const Sint32 pl0b_special_10[] = {
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52,        ATK,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_FRAME(    1,    5,   4,           0,   371, 0xb0,   0x40,    ATK | 2,  0x00, 0x17,    0,   0,     0, 0x0191,  53 ),
	CG_FRAME(    2,    6,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192,  54 ),
	CG_FRAME(    3, 0x87,   2,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  55 ),
	CG_FRAME(    3, 0x88,  16,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  55 ),
	CG_FRAME(    3,    0,   2,           0,   373, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0194, 102 ),
	CG_FRAME(    4,    0,   6,           0,   374, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0195, 102 ),
	CG_FRAME(    5,    0,   4,           0,   375, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0196, 102 ),
};

const Sint32 pl0b_special_10_loop[] = {
	CG_FRAME(    6,    0,   2,           0,   376, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0197, 102 ),
	CG_FRAME(    7,    0,   2, END_OF_DATA,   377, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0198, 102 ),
	CG_NEXT( pl0b_special_10_loop ),
};

/* Char_Special 7 */
const Sint32 pl0b_special_07[] = {
	CG_FRAME(    0,    1,   6,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0097,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0098,   1 ),
	CG_NEXT( pl0b_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl0b_special_08[] = {
	CG_FRAME(    0,    1,   6,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0097,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0098,   1 ),
	CG_NEXT( pl0b_special_08 ),
};

/* Char_Special 11
   Char_Special 12 */
const Sint32 pl0b_special_11[] = {
	CG_FRAME(    0,    1,   6,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0097,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0098,   1 ),
	CG_NEXT( pl0b_special_11 ),
};

/* Char_Special 13 */
const Sint32 pl0b_special_13[] = {
	CG_FRAME(    0,    0,   3,           0,   433, 0xb0,   0x53,        ATK,  0x00,    0,    0,   0,     0, 0x0199,   1 ),
	CG_FRAME(    0,    0,   3,           0,   434, 0xb0,   0x55, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x019a,   5 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0xb0,   0x6f,    ATK | 3,  0x00, 0x14,    0,   0,     0, 0x019b,   5 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0xb0,   0xac,        ATK,  0x00,    0,    0,   0,     0, 0x019c, 103 ),
	CG_FRAME(    1,    1,  12,           0,   437, 0xb0,   0x51,        ATK,  0x00,    0,    0,   0,     0, 0x019d,  56 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0xb0,   0x0c,        ATE,  0x00,    0,    0,   0,     0, 0x019e,   4 ),
	CG_FRAME(    0,    0,  20,           0,    56, 0xb0,   0x0c,        ATE,  0x00,    0,    0,   0,     0, 0x019f,   4 ),
	CG_FRAME(    0,    0,   6,           0,    57, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01a0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    58, 0xb0,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x01a1,   4 ),
	CG_NEXT( pl0b_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl0b_special_14[] = {
	CG_FRAME(    0,    0,   3,           0,   433, 0xb0,   0x53,        ATK,  0x00,    0,    0,   0,     0, 0x0199,   1 ),
	CG_FRAME(    0,    0,   5,           0,   434, 0xb0,   0x55, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x019a,   5 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0xb0,   0x6f,    ATK | 3,  0x00, 0x14,    0,   0,     0, 0x019b,   5 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0xb0,   0xac,        ATK,  0x00,    0,    0,   0,     0, 0x019c, 103 ),
	CG_FRAME(    1,    1,  24,           0,   437, 0xb0,   0x51,        ATK,  0x00,    0,    0,   0,     0, 0x019d,  56 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0xb0,   0x0c,        ATE,  0x00,    0,    0,   0,     0, 0x019e,   4 ),
	CG_FRAME(    0,    0,  24,           0,    56, 0xb0,   0x0c,        ATE,  0x00,    0,    0,   0,     0, 0x019f,   4 ),
	CG_FRAME(    0,    0,   6,           0,    57, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01a0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    58, 0xb0,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x01a1,   4 ),
	CG_NEXT( pl0b_special_14 ),
};

/* Char_Special 15: snapback_init (Snapback) */
const Sint32 pl0b_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,   271, 0x70,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x029a,   4 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x70,   0x6d,        ATK,  0x00,    0,    0,   0,     0, 0x029b,   4 ),
	CG_FRAME(    0,    1,   4,           0,   272, 0x70,   0x6d,        ATK,  0x00,    0,    0,   0,     0, 0x029b,   4 ),
	CG_FRAME(    0,    0,   1,           0,   272, 0x70,   0x6d, ATK | 0x15,  0x01,    0,    0,   0,     0, 0x029b,   4 ),
	CG_FRAME( 0x80,    0,   1,           0,   273, 0x90,   0xe1,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x029f,  12 ),
	CG_FRAME( 0x81,    0,   3,           0,   273, 0x90,   0xe1,        ATK,  0x00,    0,    0,   0,     0, 0x029f,  12 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x90,   0xe1,        ATE,  0x00,    0,    0,   0,     0, 0x029f,  65 ),
	CG_FRAME(    0,    0,   2,           0,   274, 0x90,   0xe1,        ATE,  0x00,    0,    0,   0,     0, 0x027d,  65 ),
	CG_FRAME(    0,    0,   8,           0,   275, 0x90,   0x05,        ATE,  0x00,    0,    0,   0,     0, 0x027e,  66 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x90,   0x80,        ATE,  0x00,    0,    0,   0,     0, 0x0326,  66 ),
	CG_FRAME(    0,    0,   6,           0,   277, 0x90,   0x6d,        ATE,  0x00,    0,    0,   0,     0, 0x0327,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   149, 0x90,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x0328,   4 ),
	CG_NEXT( pl0b_special_15 ),
};

/* Char_Super 0 */
const Sint32 pl0b_super_00[] = {
	CG_FRAME(    0,    0,   1,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,  35 ),
	CG_FRAME(    2,   17,   4,           0,   371, 0xb0,   0x40,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x0191, 104 ),
	CG_FRAME(    2,   33,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192, 105 ),
	CG_FRAME(    2,   49,   2,           0,   373, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  86 ),
	CG_FRAME(    2,    0,   5,           0,   374, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0195,   6 ),
	CG_FRAME(    2,    0,   4,           0,   375, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0196,   6 ),
	CG_FRAME(    2,    0,   2,           0,   376, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0197,   6 ),
	CG_FRAME(    1,    0,   2,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,   4 ),
	CG_FRAME(    2,   17,   4,           0,   371, 0xb0,   0x40,    ATK | 6,  0x00, 0x04,    0,   0,     0, 0x0191,  84 ),
	CG_FRAME(    2,   33,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192,  85 ),
	CG_FRAME(    2,   49,   2,           0,   373, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  86 ),
	CG_FRAME(    2,    0,   5,           0,   374, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0195,   6 ),
	CG_FRAME(    2,    0,   4,           0,   375, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0196,   6 ),
	CG_FRAME(    2,    0,   2,           0,   376, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0197,   6 ),
	CG_FRAME(    2,    0,   2,           0,   370, 0xb0,   0x52, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x0190,   4 ),
	CG_FRAME(    2,   17,   4,           0,   371, 0xb0,   0x40,    ATK | 2,  0x00, 0x17,    0,   0,     0, 0x0191,  84 ),
	CG_FRAME(    2,   33,   2,           0,   372, 0xb0,   0x4d,        ATK,  0x00,    0,    0,   0,     0, 0x0192,  85 ),
	CG_FRAME(    3,   50,  16,           0,   373, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0193,  86 ),
	CG_FRAME(    3,    0,   6,           0,   374, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0195,   6 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0196,   6 ),
};

const Sint32 pl0b_super_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   376, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0197,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   377, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0, 0x0198,   6 ),
	CG_NEXT( pl0b_super_00_loop ),
};

/* Char_Super 1 */
const Sint32 pl0b_super_01[] = {
	CG_FRAME(    0,    2,   6,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0097,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0098,   1 ),
	CG_NEXT( pl0b_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl0b_super_02[] = {
	CG_FRAME(    0, 0x80,   3,           0,   433, 0xc0,   0x53,        ATK,  0x00,    0,    0,   0,     0, 0x028c,   4 ),
	CG_FRAME(    0, 0x80,   3,           0,   107, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x028d,   4 ),
	CG_FRAME(    0, 0x80,   3,           0,   108, 0xc0,   0x16,        ATK,  0x00,    0,    0,   0,     0, 0x028e, 106 ),
	CG_FRAME(    0, 0x80,  12,           0,   109, 0xc0,   0x17,        ATK,  0x00,    0,    0,   0,     0, 0x028f, 106 ),
};

const Sint32 pl0b_super_02_loop[] = {
	CG_FRAME(    0, 0x80,   3, END_OF_DATA,   109, 0xc0,   0x17,        ATK,  0x00,    0,    0,   0,     0, 0x028f, 106 ),
	CG_NEXT( pl0b_super_02_loop ),
};

/* Char_Super 3 */
const Sint32 pl0b_super_03[] = {
	CG_FRAME(    0, 0x80,   4,           0,    46, 0xc0,   0x00,    ATK | 4,  0x00, 0x15,    0,   0,     0, 0x0290,   4 ),
};

const Sint32 pl0b_super_03_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   437, 0xc0,   0x18,        ATK,  0x00,    0,    0,   0,     0, 0x0291,  60 ),
	CG_NEXT( pl0b_super_03_loop ),
};

/* Char_Super 4: justice_throw_init (Final Justice) */
const Sint32 pl0b_super_04[] = {
	CG_FRAME(    0, 0x80,   4,           0,    55, 0xc0,   0x0d,        ATK,  0x00,    0,    0,   0,     0, 0x0292,   4 ),
	CG_FRAME(    0, 0x80,   4,           0,    56, 0xc0,   0x0d,        ATK,  0x00,    0,    0,   0,     0, 0x0293,   4 ),
	CG_FRAME(    0, 0x80,  12,           0,    57, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0294,   4 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,    58, 0xc0,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x0295,   4 ),
	CG_NEXT( pl0b_super_04 ),
};

/* Char_Catch 5
   Char_Super 5: justice_hit_init (Final Justice) */
const Sint32 pl0b_catch_05[] = {
	CG_FRAME(    0,    0,   1,           0,   496, 0xc0,   0x40,        ATK,  0x00,    0,    0,   0,     0, 0x0027,   4 ),
	CG_FRAME(    0,    0,   2,           0,   496, 0xc0,   0x40,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0027,   4 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0xa0,   0x6c,        ATK,  0x00,    0,    0,   0,     0, 0x0028,   4 ),
	CG_FRAME(    1,    0,   1,           0,   241, 0xa0,   0x6c,        ATK,  0x02,    0,    0,   0,     0, 0x0029,  83 ),
	CG_FRAME(    0,    0,   6,           0,   241, 0xa0,   0x6c,        ATK,  0x00,    0,    0,   0,     0, 0x0029,   4 ),
	CG_FRAME(    0,    0,   3,           0,   265, 0xa0,   0x01,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0030,   4 ),
	CG_FRAME(    2,    0,   1,           0,   266, 0xa0,   0x01,        ATK,  0x02,    0,    0,   0,     0, 0x0031,  83 ),
	CG_FRAME(    0,    0,   6,           0,   266, 0xa0,   0x01,        ATK,  0x00,    0,    0,   0,     0, 0x0031,   4 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0xa0,   0x01,        ATK,  0x00,    0,    0,   0,     0, 0x0032,   4 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0xa0,   0x01,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0033,   4 ),
	CG_FRAME(    1,    0,   1,           0,   269, 0xa0,   0x01,        ATK,  0x02,    0,    0,   0,     0, 0x0034,  83 ),
	CG_FRAME(    0,    0,   6,           0,   269, 0xa0,   0x01,        ATK,  0x00,    0,    0,   0,     0, 0x0034,   4 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0xa0,   0x01,        ATK,  0x00,    0,    0,   0,     0, 0x003f,   4 ),
	CG_FRAME(    0,    0,   2,           0,   270, 0xa0,   0x0c,        ATK,  0x00,    0,    0,   0,     0, 0x0035,   4 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0xa0,   0x01,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0036,   4 ),
	CG_FRAME(    1,    0,   1,           0,   262, 0xa0,   0x01,        ATK,  0x01,    0,    0,   0,     0, 0x0036,  83 ),
	CG_FRAME(    0,    0,   6,           0,   262, 0xa0,   0x01,        ATK,  0x00,    0,    0,   0,     0, 0x0036,   4 ),
	CG_FRAME(    0,    1,   2,           0,   261, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0037,   4 ),
	CG_FRAME(    0,    0,   2,           0,   342, 0xa0,   0x00,    ATK | 5,  0x00, 0x03,    0,   0,     0, 0x0038,   6 ),
	CG_FRAME(    2,    0,   1,           0,   343, 0xa0,   0x00,        ATK,  0x01,    0,    0,   0,     0, 0x0039,  83 ),
	CG_FRAME(    0,    0,   6,           0,   343, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0039,   4 ),
	CG_FRAME(    0,    0,   2,           0,   128, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x003a,   4 ),
	CG_FRAME(    0,    0,   2,           0,   129, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x003a,   4 ),
	CG_FRAME(    0, 0x80,   3,           0,   319, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x003b,   6 ),
	CG_FRAME(    0, 0x81,   1,           0,   320, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x003c,   4 ),
	CG_FRAME(    2, 0x82,   1,           0,   321, 0xa0,   0x00,        ATK,  0x02,    0,    0,   0,     0, 0x003c,  83 ),
	CG_FRAME(    0, 0x83,   4,           0,   326, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x003c,   6 ),
	CG_FRAME(    0, 0x85,   4,           0,   322, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0012,   6 ),
	CG_FRAME(    0, 0x85,   4,           0,   322, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x0012,   6 ),
	CG_FRAME(    0,    1,   2,           0,    65, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x003d,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x11,        ATK,  0x00,    0,    0,   0,     0, 0x003e,   4 ),
	CG_NEXT( pl0b_catch_05 ),
};

/* Char_Super 6: justice_hop_init (Final Justice), justice_down_init (Final Justice) */
const Sint32 pl0b_super_06[] = {
	CG_FRAME(    0, 0x80,   4,           0,   150, 0x40,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x02a0,   1 ),
	CG_FRAME(    0, 0x80,   4,           0,   149, 0x40,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x02a1,   1 ),
	CG_FRAME(    0, 0x80,   4,           0,   148, 0x40,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x02a2,   1 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0xc0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02a3,   1 ),
};

const Sint32 pl0b_super_06_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    66, 0xc0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02a4,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    70, 0xc0,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x02a5,   1 ),
	CG_NEXT( pl0b_super_06_loop ),
};

/* Char_Super 7: justice_let_go (Final Justice) */
const Sint32 pl0b_super_07[] = {
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x027f,   6 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0280,   6 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0281,   6 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0282,   6 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0283,   6 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0284,   6 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0285,   6 ),
};

const Sint32 pl0b_super_07_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0286,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    79, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0, 0x0287,   6 ),
	CG_NEXT( pl0b_super_07_loop ),
};

/* Char_Super 8: justice_land (Final Justice) */
const Sint32 pl0b_super_08[] = {
	CG_FRAME(    0,    0,  16,           0,   148, 0x40,   0x11,        ATK,  0x00,    0,    0,   0,     0, 0x0288,   4 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x40,   0x11,        ATK,  0x00,    0,    0,   0,     0, 0x0289,   4 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x40,   0x11,        ATK,  0x00,    0,    0,   0,     0, 0x028a,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   151, 0x40,   0x11,        ATK,  0x00,    0,    0,   0,     0, 0x028b,   4 ),
	CG_NEXT( pl0b_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl0b_super_09[] = {
	CG_FRAME(    0,    0,   3,           0,   434, 0xb0,   0x55,        ATK,  0x00,    0,    0,   0,     0, 0x019a,   5 ),
	CG_FRAME(    0,    0,   1,           0,   435, 0xb0,   0x6f, ATK | 0x16,  0x00,    0,    0,   0,     0, 0x019b,   5 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0xb0,   0xac,    ATK | 3,  0x00, 0x14,    0,   0,     0, 0x019c, 100 ),
};

const Sint32 pl0b_super_09_loop[] = {
	CG_FRAME(    1,    1,   1,           0,   437, 0xb0,   0x51,        ATK,  0x00,    0,    0,   0,     0, 0x019d, 100 ),
	CG_FRAME(    2,    1,   1, END_OF_DATA,   437, 0xb0,   0x51,        ATK,  0x00,    0,    0,   0,     0, 0x019d, 100 ),
	CG_NEXT( pl0b_super_09_loop ),
};

/* Char_Super 10 */
const Sint32 pl0b_super_10[] = {
	CG_FRAME(    3,    0,   6, END_OF_DATA,   437, 0xb0,   0x51,        ATK,  0x00,    0,    0,   0,     0, 0x019d, 100 ),
	CG_NEXT( pl0b_super_10 ),
};

/* Char_Super 11 */
const Sint32 pl0b_super_11[] = {
	CG_FRAME(    0,    0,   4,           0,    55, 0xb0,   0x0c,        ATE,  0x00,    0,    0,   0,     0, 0x019e,   4 ),
	CG_FRAME(    0,    0,   8,           0,    56, 0xb0,   0x0c,        ATE,  0x00,    0,    0,   0,     0, 0x019f,   4 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0, 0x01a0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    58, 0xb0,   0x11,        ATE,  0x00,    0,    0,   0,     0, 0x01a1,   4 ),
	CG_NEXT( pl0b_super_11 ),
};

/* Char_Super 12 */
const Sint32 pl0b_super_12[] = {
	CG_FRAME(    0,    0,  20, END_OF_DATA,   433, 0xb0,   0x53,        ATK,  0x00,    0,    0,   0,     0, 0x0199,   1 ),
	CG_NEXT( pl0b_super_12 ),
};

/* Char_Super 13 */
const Sint32 pl0b_super_13[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   370, 0xb0,   0x52,        ATK,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	CG_NEXT( pl0b_super_13 ),
};

const Sint32 pl0b_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,  19, END_OF_DATA,   370, 0xb0,   0x52,        ATK,  0x00,    0,    0,   0,     0, 0x0190,   5 ),
	0x00006034,
};

const Sint32 pl0b_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0, 0x80,   3,           0,   433, 0xc0,   0x53,        ATK,  0x00,    0,    0,   0,     0, 0x028c,   4 ),
	CG_FRAME(    0, 0x80,   3,           0,   107, 0xc0,   0x00,        ATK,  0x00,    0,    0,   0,     0, 0x028d,   4 ),
	CG_FRAME(    0, 0x80,   3,           0,   108, 0xc0,   0x16,        ATK,  0x00,    0,    0,   0,     0, 0x028e, 106 ),
	CG_FRAME(    0, 0x80,  12,           0,   109, 0xc0,   0x17,        ATK,  0x00,    0,    0,   0,     0, 0x028f, 106 ),
	CG_FRAME(    0, 0x80,  11, END_OF_DATA,   109, 0xc0,   0x17,        ATK,  0x00,    0,    0,   0,     0, 0x028f, 106 ),
	0x0000609c,
};

const Sint32 pl0b_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,  28, END_OF_DATA,   433, 0xb0,   0x53,        ATK,  0x00,    0,    0,   0,     0, 0x0199,   1 ),
	0x000060b4,
};

/* Char_Effect 0: pls0b_01_trail_init, pls0b_01_trail_move */
const Sint32 pl0b_effect_00[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32770, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32771, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_00 ),
};

/* Char_Effect 1: pls0b_01_head_init, pls0b_01_t2_init */
const Sint32 pl0b_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   136, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   138, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   140, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl0b_effect_02[] = {
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_02 ),
};

/* Char_Effect 3
   Char_Fly 0 */
const Sint32 pl0b_effect_03[] = {
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   545, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   546, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   548, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   549, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   550, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   553, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   554, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   555, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   540, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   541, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   542, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   543, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_03 ),
};

/* Char_Effect 4
   Char_Fly 1: act_fly */
const Sint32 pl0b_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   560, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   561, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   562, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   563, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   564, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   565, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   566, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   567, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   568, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   569, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   570, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   556, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   557, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   559, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_04 ),
};

/* Char_Effect 5
   Char_Fly 2 */
const Sint32 pl0b_effect_05[] = {
	CG_FRAME(    0,    0,   4,           0,   576, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   577, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   578, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   579, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   581, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   582, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   583, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   585, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   586, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   572, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   575, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_05 ),
};

/* Char_Effect 6
   Char_Fly 3 */
const Sint32 pl0b_effect_06[] = {
	CG_FRAME(    0,    0,   4,           0,   592, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   593, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   594, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   595, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   596, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   597, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   598, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   601, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   603, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   588, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   589, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   590, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   591, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_06 ),
};

/* Char_Effect 7
   Char_Fly 4 */
const Sint32 pl0b_effect_07[] = {
	CG_FRAME(    0,    0,   4,           0,   608, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   609, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   610, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   611, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   613, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   618, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   619, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   605, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   607, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_07 ),
};

/* Char_Fly 5 */
const Sint32 pl0b_fly_05[] = {
	CG_FRAME(    0,    0,   4,           0,   694, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   695, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   696, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   697, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   698, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   699, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   700, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   701, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   702, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   703, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   704, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   705, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   690, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   691, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   692, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   693, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_fly_05 ),
};

/* Char_Fly 6 */
const Sint32 pl0b_fly_06[] = {
	CG_FRAME(    0,    0,   4,           0,   710, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   711, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   712, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   713, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   714, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   715, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   716, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   717, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   718, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   719, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   720, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   721, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   706, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   707, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   708, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   709, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_fly_06 ),
};

/* Char_Fly 7 */
const Sint32 pl0b_fly_07[] = {
	CG_FRAME(    0,    0,   4,           0,   726, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   727, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   728, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   729, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   730, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   732, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   733, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   734, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   735, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   736, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   737, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   722, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   723, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   724, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   725, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_fly_07 ),
};

/* Char_Effect 8
   Char_Fly 8 */
const Sint32 pl0b_effect_08[] = {
	CG_FRAME(    0,    0,   4,           0,   620, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   623, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   624, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   625, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   626, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   627, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   628, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   629, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   630, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   631, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   632, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   633, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   634, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   635, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   636, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   637, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   638, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   639, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   640, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   641, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   642, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   643, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   644, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   645, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   646, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   647, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   648, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   649, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   650, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl0b_effect_09[] = {
	CG_FRAME(    0,    0,   4,           0,   651, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   652, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   653, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl0b_effect_10[] = {
	CG_FRAME(    0,    0,   4,           0,   654, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   655, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   656, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl0b_effect_11[] = {
	CG_FRAME(    0,    0,   4,           0,   657, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   658, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   659, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_11 ),
};

/* Char_Effect 17 */
const Sint32 pl0b_effect_17[] = {
	CG_FRAME(    0,    0,   4,           0,   660, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   661, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   662, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl0b_effect_18[] = {
	CG_FRAME(    0,    0,   4,           0,   663, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   664, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   665, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_18 ),
};

/* Char_Effect 21 */
const Sint32 pl0b_effect_21[] = {
	CG_FRAME(    0,    0,   4,           0,   797, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   798, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   799, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
};

const Sint32 pl0b_effect_21_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   800, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   801, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   802, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl0b_effect_21_loop ),
};

/* Char_Effect 22 */
const Sint32 pl0b_effect_22[] = {
	CG_FRAME(    0,    0,   4,           0,   791, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1,           0,   792, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1,           0,   793, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
};

const Sint32 pl0b_effect_22_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   794, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1,           0,   795, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   796, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_NEXT( pl0b_effect_22_loop ),
};

/* Char_Effect 23 */
const Sint32 pl0b_effect_23[] = {
	CG_FRAME(    0,    0,   4,           0,   803, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   1,           0,   804, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   1,           0,   805, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
};

const Sint32 pl0b_effect_23_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   806, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   1,           0,   807, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   808, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl0b_effect_23_loop ),
};

/* Char_Effect 12 */
const Sint32 pl0b_effect_12[] = {
	CG_FRAME(    0,    0,   4,           0,   738, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   739, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   740, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   741, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   742, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   743, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   744, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   745, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   746, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4, END_OF_DATA,   747, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_12 ),
};

/* Char_Effect 14 */
const Sint32 pl0b_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0,   748, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   749, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   750, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   751, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   752, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   753, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl0b_effect_15[] = {
	CG_FRAME(    0,    0,   4,           0,   754, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   755, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   756, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   757, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   758, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   759, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   760, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   761, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   762, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   763, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   764, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_15 ),
};

/* Char_Effect 19
   Char_Effect 20 */
const Sint32 pl0b_effect_19[] = {
	CG_FRAME(    0, 0x80,   4,           0,   779, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   780, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   781, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   782, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   783, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0, 0x80,   4, END_OF_DATA,   784, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl0b_effect_19 ),
};

/* Char_Effect 13 */
const Sint32 pl0b_effect_13[] = {
	CG_FRAME(    0, 0x80,   4,           0,   765, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   766, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   767, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   769, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   770, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   771, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   772, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   773, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4, END_OF_DATA,   774, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_13 ),
};

/* Char_Effect 16 */
const Sint32 pl0b_effect_16[] = {
	CG_FRAME(    0,    0,   4,           0,   775, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   778, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_16 ),
};

/* Char_Effect 26
   Char_Effect 28 */
const Sint32 pl0b_effect_26[] = {
	CG_FRAME(    0,    0,   3,           0,   819, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   820, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   821, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   822, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   823, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   824, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   825, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   826, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   827, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   828, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   829, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   830, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   831, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   832, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   833, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   834, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   835, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   836, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   837, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   838, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   839, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   840, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   831, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   832, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   833, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   834, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   835, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   836, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   837, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   838, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   839, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   840, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   841, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   842, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   841, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_26 ),
};

/* Char_Effect 24 */
const Sint32 pl0b_effect_24[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   817, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl0b_effect_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   818, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0b_effect_25 ),
};

/* Char_Effect 27: pls0b_00_ground_init, pls0b_00_ground_wait */
const Sint32 pl0b_effect_27[] = {
	CG_FRAME(    0,    0,   4,           0,   858, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   859, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   860, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   861, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   862, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   863, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   864, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   865, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   866, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0b_effect_27 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl0b_change_00[] = {
	CG_FRAME(    0,    0,   1,           0,   181, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x030b,   0 ),
	CG_FRAME(    0,    0,   1,           0,   182, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x030c,   0 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x0b,          0,  0x00,    0,    0,   0,     0, 0x030d,   0 ),
	CG_FRAME(    0,    0,   2,           0,   184, 0x80,   0x09,          0,  0x00,    0,    0,   0,     0, 0x030e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x09,          0,  0x00,    0,    0,   0,     0, 0x030f,   0 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x09,       0x0f,  0x00, 0x20,    0,   0,     0, 0x0310,   0 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x80,   0x09,          0,  0x00,    0,    0,   0,     0, 0x0311,   0 ),
	CG_FRAME(    0,    1,   9, END_OF_DATA,   188, 0x80,   0x09,          0,  0x00,    0,    0,   0,     0, 0x0312,   0 ),
	CG_NEXT( pl0b_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl0b_change_01[] = {
	CG_FRAME( 0x80,    0,   4,           0,   428, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0323,   0 ),
	CG_FRAME( 0x80,    0,   4, END_OF_DATA,   428, 0x80,   0x11,          0,  0x00,    0,    0,   0,     0, 0x0324,   0 ),
	CG_NEXT( pl0b_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl0b_change_02[] = {
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008b,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x008c,   3 ),
	CG_NEXT( pl0b_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl0b_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01c7,   1 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01c9,   1 ),
	CG_FRAME(    0,    0,   1,           0,   103, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01ca,   1 ),
	CG_FRAME(    0,    1,   3,           0,   103, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01ca,   1 ),
	CG_FRAME(    0,    0,   7,           0,   104, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01cb,   1 ),
	CG_FRAME(    0,    0,   1,           0,   106, 0x80,   0x12,          0,  0x00, 0x0f,    0,   0,     0, 0x01cd,   1 ),
};

const Sint32 pl0b_change_03_loop[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   106, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01cd,   1 ),
	CG_NEXT( pl0b_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl0b_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01c7,   1 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01c9,   1 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01ca,   1 ),
	CG_FRAME(    0,    1,   3,           0,   103, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01ca,   1 ),
	CG_FRAME(    0,    0,   7,           0,   104, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01cb,   1 ),
	CG_FRAME(    0,    0,  14,           0,   106, 0x80,   0x12,          0,  0x00, 0x0f,    0,   0,     0, 0x01cd,   1 ),
	CG_FRAME(    0,    0,   5,           0,   103, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01ca,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   102, 0x80,   0x12,          0,  0x00,    0,    0,   0,     0, 0x01c9,   1 ),
	CG_NEXT( pl0b_change_04 ),
};

/* group 27 0
   group 27 1
   group 27 3
   group 27 4 */
const Sint32 pl0b_grp27_00[] = {
	EF_FRAME(    0,    0,   4,           0,   922 ),
	EF_FRAME(    0,    0,   4,           0,   923 ),
	EF_FRAME(    0,    0,   4,           0,   924 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   925 ),
	CG_NEXT( pl0b_grp27_00 ),
};

/* group 27 2
   group 27 5 */
const Sint32 pl0b_grp27_02[] = {
	EF_FRAME(    0,    0,   4,           0,   926 ),
	EF_FRAME(    0,    0,   4,           0,   927 ),
	EF_FRAME(    0,    0,   4,           0,   928 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   929 ),
	CG_NEXT( pl0b_grp27_02 ),
};

/* group 27 6 */
const Sint32 pl0b_grp27_06[] = {
	EF_FRAME(    0,    0,   4,           0,   930 ),
	EF_FRAME(    0,    0,   4,           0,   931 ),
	EF_FRAME(    0,    0,   4,           0,   932 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   933 ),
	CG_NEXT( pl0b_grp27_06 ),
};
