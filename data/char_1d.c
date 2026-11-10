/* Roll (character 1d): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_1d      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_1d     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl1d.c) says which group
                   and number each move uses.
   pl1d_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl1d_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl1d_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl1d_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_1d
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_1d, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_1d */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl1d_stand_00[], pl1d_stand_02[], pl1d_stand_06[], pl1d_stand_03[],
	pl1d_stand_07[], pl1d_jump_00[], pl1d_jump_00_loop[], pl1d_jump_03[], pl1d_jump_03_loop[],
	pl1d_jump_06[], pl1d_jump_09[], pl1d_dash_00[], pl1d_dash_00_loop[], pl1d_dash_02[],
	pl1d_dash_02_loop[], pl1d_dash_01[], pl1d_dash_01_loop[], pl1d_dash_03[], pl1d_dash_03_loop[],
	pl1d_crouch_00[], pl1d_crouch_00_loop[], pl1d_crouch_01[], pl1d_crouch_02[],
	pl1d_crouch_02_loop[], pl1d_turn_00[], pl1d_turn_00_loop[], pl1d_turn_01[], pl1d_turn_01_loop[],
	pl1d_guard_00[], pl1d_guard_01[], pl1d_guard_04[], pl1d_guard_05[], pl1d_air_guard_00[],
	pl1d_air_guard_02[], pl1d_air_guard_01[], pl1d_stand_p_00[], pl1d_stand_p_01[],
	pl1d_stand_p_02[], pl1d_special_00[], pl1d_special_09[], pl1d_special_10[], pl1d_special_11[],
	pl1d_special_13[], pl1d_stand_k_00[], pl1d_stand_k_01[], pl1d_stand_k_02[], pl1d_crouch_p_00[],
	pl1d_crouch_p_01[], pl1d_crouch_p_02[], pl1d_crouch_k_00[], pl1d_crouch_k_01[],
	pl1d_crouch_k_02[], pl1d_jump_p_00[], pl1d_jump_p_01[], pl1d_jump_p_02[], pl1d_special_02[],
	pl1d_special_14[], pl1d_special_15[], pl1d_jump_k_00[], pl1d_jump_k_01[], pl1d_jump_k_02[],
	pl1d_jump_k_06[], pl1d_damage_00[], pl1d_damage_01[], pl1d_damage_02[], pl1d_damage_06[],
	pl1d_damage_07[], pl1d_damage_08[], pl1d_damage_09[], pl1d_damage_09_loop[], pl1d_damage_10[],
	pl1d_damage_13[], pl1d_damage_16[], pl1d_damage_17[], pl1d_damage_18[], pl1d_damage_19[],
	pl1d_damage_20[], pl1d_damage_21[], pl1d_damage_22[], pl1d_damage_23[], pl1d_damage_24[],
	pl1d_damage_25[], pl1d_damage_26[], pl1d_damage_26_loop[], pl1d_damage_27[], pl1d_damage_28[],
	pl1d_damage_29[], pl1d_damage_30[], pl1d_damage_31[], pl1d_dizzy_00[], pl1d_damage_35[],
	pl1d_caught_00[], pl1d_caught_01[], pl1d_caught_02[], pl1d_caught_03[], pl1d_caught_04[],
	pl1d_caught_05[], pl1d_caught_06[], pl1d_caught_07[], pl1d_caught_08[], pl1d_caught_09[],
	pl1d_caught_10[], pl1d_caught_11[], pl1d_caught_12[], pl1d_caught_13[], pl1d_caught_14[],
	pl1d_caught_15[], pl1d_caught_16[], pl1d_caught_17[], pl1d_caught_18[], pl1d_caught_19[],
	pl1d_caught_20[], pl1d_caught_21[], pl1d_catch_00[], pl1d_catch_01[], pl1d_get_up_00[],
	pl1d_get_up_01[], pl1d_get_up_02[], pl1d_get_up_03[], pl1d_appear_00[], pl1d_appear_02[],
	pl1d_appear_02_loop[], pl1d_appear_03[], pl1d_appear_03_loop[], pl1d_appear_01[],
	pl1d_appear_04[], pl1d_appear_05[], pl1d_appear_06[], pl1d_appear_06_loop[], pl1d_appear_07[],
	pl1d_appear_07_loop[], pl1d_appear_08[], pl1d_extra_02[], pl1d_extra_03[], pl1d_special_03[],
	pl1d_special_05[], pl1d_extra_00[], pl1d_extra_01[], pl1d_extra_01_loop[], pl1d_win_00[],
	pl1d_win_00_loop[], pl1d_win_01[], pl1d_win_01_loop[], pl1d_win_06[], pl1d_win_06_loop[],
	pl1d_win_07[], pl1d_win_07_loop[], pl1d_win_08[], pl1d_win_08_loop[], pl1d_win_02[],
	pl1d_win_02_loop[], pl1d_change_00[], pl1d_change_00_loop[], pl1d_change_01[],
	pl1d_change_01_loop[], pl1d_change_03[], pl1d_change_03_loop[], pl1d_change_04[],
	pl1d_special_12[], pl1d_change_02[], pl1d_change_02_loop[], pl1d_super_00[], pl1d_super_01[],
	pl1d_super_02[], pl1d_super_03[], pl1d_super_03_loop[], pl1d_super_29[], pl1d_super_29_loop[],
	pl1d_super_41[], pl1d_super_42[], pl1d_super_04[], pl1d_super_04_loop[], pl1d_super_05[],
	pl1d_super_06[], pl1d_super_06_loop[], pl1d_super_27[], pl1d_super_28[], pl1d_super_17[],
	pl1d_super_33[], pl1d_super_33_loop[], pl1d_super_25[], pl1d_super_26[], pl1d_super_26_loop[],
	pl1d_super_18[], pl1d_super_19[], pl1d_super_20[], pl1d_super_21[], pl1d_super_22[],
	pl1d_super_23[], pl1d_super_24[], pl1d_super_30[], pl1d_super_31[], pl1d_super_32[],
	pl1d_super_40[], pl1d_super_40_loop[], pl1d_super_07[], pl1d_super_34[], pl1d_super_34_loop[],
	pl1d_super_15[], pl1d_super_16[], pl1d_super_16_loop[], pl1d_super_08[], pl1d_super_09[],
	pl1d_super_11[], pl1d_super_12[], pl1d_super_38[], pl1d_super_39[], pl1d_super_13[],
	pl1d_super_36[], pl1d_super_35[], pl1d_super_37[], pl1d_super_14[], pl1d_super_10[],
	pl1d_effect_00[], pl1d_effect_01[], pl1d_effect_02[], pl1d_effect_03[], pl1d_effect_09[],
	pl1d_effect_10[], pl1d_effect_19[], pl1d_effect_20[], pl1d_effect_21[], pl1d_effect_22[],
	pl1d_effect_23[], pl1d_effect_24[], pl1d_effect_25[], pl1d_effect_26[], pl1d_effect_04[],
	pl1d_effect_04_loop[], pl1d_effect_05[], pl1d_effect_05_loop[], pl1d_effect_06[],
	pl1d_effect_07[], pl1d_effect_08[], pl1d_effect_11[], pl1d_grp27_43[], pl1d_effect_13[],
	pl1d_effect_13_loop[], pl1d_effect_14[], pl1d_effect_15[], pl1d_effect_15_loop[],
	pl1d_effect_16[], pl1d_effect_17[], pl1d_effect_17_loop[], pl1d_effect_18[], pl1d_unused_00[],
	pl1d_grp25_00[], pl1d_grp25_01[], pl1d_grp25_02[], pl1d_grp25_03[], pl1d_grp25_04[],
	pl1d_grp25_05[], pl1d_grp25_06[], pl1d_grp25_06_loop[], pl1d_grp25_07[], pl1d_grp25_07_loop[],
	pl1d_grp25_08[], pl1d_grp25_09[], pl1d_grp25_09_loop[], pl1d_grp25_10[], pl1d_grp25_10_loop[],
	pl1d_grp25_11[], pl1d_grp25_12[], pl1d_grp25_12_loop[], pl1d_grp25_15[], pl1d_grp25_15_loop[],
	pl1d_grp25_20[], pl1d_grp25_21[], pl1d_grp25_22[], pl1d_grp25_23[], pl1d_grp25_23_loop[],
	pl1d_grp25_24[], pl1d_grp25_24_loop[], pl1d_grp25_19[], pl1d_grp25_13[], pl1d_grp25_14[],
	pl1d_grp25_14_loop[], pl1d_grp25_18[], pl1d_grp25_18_loop[], pl1d_grp25_28[], pl1d_grp25_29[],
	pl1d_grp25_29_loop[], pl1d_grp25_30[], pl1d_grp25_31[], pl1d_grp25_16[], pl1d_grp25_16_loop[],
	pl1d_grp25_17[], pl1d_grp25_25[], pl1d_grp25_25_loop[], pl1d_grp25_27[], pl1d_grp25_32[],
	pl1d_grp27_03[], pl1d_grp27_04[], pl1d_grp27_06[], pl1d_grp27_07[], pl1d_grp27_07_loop[],
	pl1d_grp27_08[], pl1d_grp27_09[], pl1d_grp27_11[], pl1d_grp27_10[], pl1d_grp27_12[],
	pl1d_grp27_13[], pl1d_grp27_14[], pl1d_grp27_15[], pl1d_grp27_16[], pl1d_grp27_17[],
	pl1d_grp27_34[], pl1d_grp27_34_loop[], pl1d_grp27_18[], pl1d_grp27_19[], pl1d_grp27_20[],
	pl1d_grp27_21[], pl1d_grp27_22[], pl1d_grp27_23[], pl1d_grp27_24[], pl1d_grp27_25[],
	pl1d_grp27_26[], pl1d_grp27_27[], pl1d_grp27_28[], pl1d_grp27_29[], pl1d_grp27_30[],
	pl1d_grp27_31[], pl1d_grp27_32[], pl1d_grp27_33[], pl1d_grp27_35[], pl1d_grp27_36[],
	pl1d_grp27_37[], pl1d_grp27_38[], pl1d_grp27_39[], pl1d_grp27_40[], pl1d_grp27_41[];
extern const Sint32 *const pl1d_stand_tbl[], *const pl1d_jump_tbl[], *const pl1d_dash_tbl[],
	*const pl1d_crouch_tbl[], *const pl1d_turn_tbl[], *const pl1d_guard_tbl[],
	*const pl1d_air_guard_tbl[], *const pl1d_stand_p_tbl[], *const pl1d_stand_k_tbl[],
	*const pl1d_crouch_p_tbl[], *const pl1d_crouch_k_tbl[], *const pl1d_jump_p_tbl[],
	*const pl1d_jump_k_tbl[], *const pl1d_damage_tbl[], *const pl1d_dizzy_tbl[],
	*const pl1d_catch_tbl[], *const pl1d_caught_tbl[], *const pl1d_get_up_tbl[],
	*const pl1d_appear_tbl[], *const pl1d_win_tbl[], *const pl1d_extra_tbl[],
	*const pl1d_special_tbl[], *const pl1d_super_tbl[], *const pl1d_effect_tbl[],
	*const pl1d_change_tbl[], *const pl1d_grp25_tbl[], *const pl1d_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_1d[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 1, 0, 0 },
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
const Sint32 *const *const char_tbl_1d[] = {
	pl1d_stand_tbl, pl1d_jump_tbl, pl1d_dash_tbl, pl1d_crouch_tbl, pl1d_turn_tbl, pl1d_guard_tbl,
	pl1d_air_guard_tbl, pl1d_stand_p_tbl, pl1d_stand_k_tbl, pl1d_crouch_p_tbl, pl1d_crouch_k_tbl, pl1d_jump_p_tbl,
	pl1d_jump_k_tbl, pl1d_damage_tbl, pl1d_dizzy_tbl, pl1d_catch_tbl, pl1d_caught_tbl, pl1d_get_up_tbl,
	pl1d_appear_tbl, pl1d_win_tbl, pl1d_extra_tbl, pl1d_special_tbl, pl1d_super_tbl, pl1d_effect_tbl,
	pl1d_change_tbl, pl1d_grp25_tbl, pl1d_grp25_tbl, pl1d_grp27_tbl, 0,
};

const Sint32 *const pl1d_stand_tbl[] = {
	pl1d_stand_00, pl1d_stand_00, pl1d_stand_02, pl1d_stand_03, pl1d_stand_00, pl1d_stand_00,
	pl1d_stand_06, pl1d_stand_07, pl1d_stand_02, pl1d_stand_03, 0,
};

const Sint32 *const pl1d_jump_tbl[] = {
	pl1d_jump_00, pl1d_jump_00, pl1d_jump_00, pl1d_jump_03, pl1d_jump_03, pl1d_jump_03,
	pl1d_jump_06, pl1d_jump_06, pl1d_jump_06, pl1d_jump_09, pl1d_jump_09, pl1d_jump_09,
	0,
};

const Sint32 *const pl1d_dash_tbl[] = {
	pl1d_dash_00, pl1d_dash_01, pl1d_dash_02, pl1d_dash_03, 0,
};

const Sint32 *const pl1d_crouch_tbl[] = {
	pl1d_crouch_00, pl1d_crouch_01, pl1d_crouch_02, 0,
};

const Sint32 *const pl1d_turn_tbl[] = {
	pl1d_turn_00, pl1d_turn_01, 0,
};

const Sint32 *const pl1d_guard_tbl[] = {
	pl1d_guard_00, pl1d_guard_01, pl1d_guard_01, pl1d_guard_00, pl1d_guard_04, pl1d_guard_05,
	pl1d_guard_05, pl1d_guard_04, 0,
};

const Sint32 *const pl1d_air_guard_tbl[] = {
	pl1d_air_guard_00, pl1d_air_guard_01, pl1d_air_guard_02, 0,
};

const Sint32 *const pl1d_stand_p_tbl[] = {
	pl1d_stand_p_00, pl1d_stand_p_01, pl1d_stand_p_02, 0,
};

const Sint32 *const pl1d_stand_k_tbl[] = {
	pl1d_stand_k_00, pl1d_stand_k_01, pl1d_stand_k_02, 0,
};

const Sint32 *const pl1d_crouch_p_tbl[] = {
	pl1d_crouch_p_00, pl1d_crouch_p_01, pl1d_crouch_p_02, 0,
};

const Sint32 *const pl1d_crouch_k_tbl[] = {
	pl1d_crouch_k_00, pl1d_crouch_k_01, pl1d_crouch_k_02, 0,
};

const Sint32 *const pl1d_jump_p_tbl[] = {
	pl1d_jump_p_00, pl1d_jump_p_01, pl1d_jump_p_02, pl1d_jump_p_00, pl1d_jump_p_01, pl1d_jump_p_02,
	0,
};

const Sint32 *const pl1d_jump_k_tbl[] = {
	pl1d_jump_k_00, pl1d_jump_k_01, pl1d_jump_k_02, pl1d_jump_k_00, pl1d_jump_k_01, pl1d_jump_k_02,
	pl1d_jump_k_06, 0,
};

const Sint32 *const pl1d_damage_tbl[] = {
	pl1d_damage_00, pl1d_damage_01, pl1d_damage_02, pl1d_damage_00, pl1d_damage_01, pl1d_damage_02,
	pl1d_damage_06, pl1d_damage_07, pl1d_damage_08, pl1d_damage_09, pl1d_damage_10, pl1d_damage_10,
	pl1d_damage_10, pl1d_damage_13, pl1d_damage_13, pl1d_damage_13, pl1d_damage_16, pl1d_damage_17,
	pl1d_damage_18, pl1d_damage_19, pl1d_damage_20, pl1d_damage_21, pl1d_damage_22, pl1d_damage_23,
	pl1d_damage_24, pl1d_damage_25, pl1d_damage_26, pl1d_damage_27, pl1d_damage_28, pl1d_damage_29,
	pl1d_damage_30, pl1d_damage_31, pl1d_damage_30, pl1d_damage_27, pl1d_damage_13, pl1d_damage_35,
	0,
};

const Sint32 *const pl1d_dizzy_tbl[] = {
	pl1d_dizzy_00, 0,
};

const Sint32 *const pl1d_catch_tbl[] = {
	pl1d_catch_00, pl1d_catch_01, 0,
};

const Sint32 *const pl1d_caught_tbl[] = {
	pl1d_caught_00, pl1d_caught_01, pl1d_caught_02, pl1d_caught_03, pl1d_caught_04, pl1d_caught_05,
	pl1d_caught_06, pl1d_caught_07, pl1d_caught_08, pl1d_caught_09, pl1d_caught_10, pl1d_caught_11,
	pl1d_caught_12, pl1d_caught_13, pl1d_caught_14, pl1d_caught_15, pl1d_caught_16, pl1d_caught_17,
	pl1d_caught_18, pl1d_caught_19, pl1d_caught_20, pl1d_caught_21, 0,
};

const Sint32 *const pl1d_get_up_tbl[] = {
	pl1d_get_up_00, pl1d_get_up_01, pl1d_get_up_02, pl1d_get_up_03, 0,
};

const Sint32 *const pl1d_appear_tbl[] = {
	pl1d_appear_00, pl1d_appear_01, pl1d_appear_02, pl1d_appear_03, pl1d_appear_04, pl1d_appear_05,
	pl1d_appear_06, pl1d_appear_07, pl1d_appear_08, 0,
};

const Sint32 *const pl1d_win_tbl[] = {
	pl1d_win_00, pl1d_win_01, pl1d_win_02, pl1d_win_02, pl1d_win_01, pl1d_win_02,
	pl1d_win_06, pl1d_win_07, pl1d_win_08, 0,
};

const Sint32 *const pl1d_extra_tbl[] = {
	pl1d_extra_00, pl1d_extra_01, pl1d_extra_02, pl1d_extra_03, 0,
};

const Sint32 *const pl1d_special_tbl[] = {
	pl1d_special_00, pl1d_special_00, pl1d_special_02, pl1d_special_03, pl1d_special_00, pl1d_special_05,
	pl1d_special_00, pl1d_special_00, pl1d_special_00, pl1d_special_09, pl1d_special_10, pl1d_special_11,
	pl1d_special_12, pl1d_special_13, pl1d_special_14, pl1d_special_15, 0,
};

const Sint32 *const pl1d_super_tbl[] = {
	pl1d_super_00, pl1d_super_01, pl1d_super_02, pl1d_super_03, pl1d_super_04, pl1d_super_05,
	pl1d_super_06, pl1d_super_07, pl1d_super_08, pl1d_super_09, pl1d_super_10, pl1d_super_11,
	pl1d_super_12, pl1d_super_13, pl1d_super_14, pl1d_super_15, pl1d_super_16, pl1d_super_17,
	pl1d_super_18, pl1d_super_19, pl1d_super_20, pl1d_super_21, pl1d_super_22, pl1d_super_23,
	pl1d_super_24, pl1d_super_25, pl1d_super_26, pl1d_super_27, pl1d_super_28, pl1d_super_29,
	pl1d_super_30, pl1d_super_31, pl1d_super_32, pl1d_super_33, pl1d_super_34, pl1d_super_35,
	pl1d_super_36, pl1d_super_37, pl1d_super_38, pl1d_super_39, pl1d_super_40, pl1d_super_41,
	pl1d_super_42, 0,
};

const Sint32 *const pl1d_effect_tbl[] = {
	pl1d_effect_00, pl1d_effect_01, pl1d_effect_02, pl1d_effect_03, pl1d_effect_04, pl1d_effect_05,
	pl1d_effect_06, pl1d_effect_07, pl1d_effect_08, pl1d_effect_09, pl1d_effect_10, pl1d_effect_11,
	pl1d_effect_11, pl1d_effect_13, pl1d_effect_14, pl1d_effect_15, pl1d_effect_16, pl1d_effect_17,
	pl1d_effect_18, pl1d_effect_19, pl1d_effect_20, pl1d_effect_21, pl1d_effect_22, pl1d_effect_23,
	pl1d_effect_24, pl1d_effect_25, pl1d_effect_26, 0,
};

const Sint32 *const pl1d_change_tbl[] = {
	pl1d_change_00, pl1d_change_01, pl1d_change_02, pl1d_change_03, pl1d_change_04, 0,
};

const Sint32 *const pl1d_grp25_tbl[] = {
	pl1d_grp25_00, pl1d_grp25_01, pl1d_grp25_02, pl1d_grp25_03, pl1d_grp25_04, pl1d_grp25_05,
	pl1d_grp25_06, pl1d_grp25_07, pl1d_grp25_08, pl1d_grp25_09, pl1d_grp25_10, pl1d_grp25_11,
	pl1d_grp25_12, pl1d_grp25_13, pl1d_grp25_14, pl1d_grp25_15, pl1d_grp25_16, pl1d_grp25_17,
	pl1d_grp25_18, pl1d_grp25_19, pl1d_grp25_20, pl1d_grp25_21, pl1d_grp25_22, pl1d_grp25_23,
	pl1d_grp25_24, pl1d_grp25_25, pl1d_stand_02, pl1d_grp25_27, pl1d_grp25_28, pl1d_grp25_29,
	pl1d_grp25_30, pl1d_grp25_31, pl1d_grp25_32, 0,
};

const Sint32 *const pl1d_grp27_tbl[] = {
	pl1d_grp25_27, pl1d_grp25_27, pl1d_grp25_27, pl1d_grp27_03, pl1d_grp27_04, pl1d_grp25_27,
	pl1d_grp27_06, pl1d_grp27_07, pl1d_grp27_08, pl1d_grp27_09, pl1d_grp27_10, pl1d_grp27_11,
	pl1d_grp27_12, pl1d_grp27_13, pl1d_grp27_14, pl1d_grp27_15, pl1d_grp27_16, pl1d_grp27_17,
	pl1d_grp27_18, pl1d_grp27_19, pl1d_grp27_20, pl1d_grp27_21, pl1d_grp27_22, pl1d_grp27_23,
	pl1d_grp27_24, pl1d_grp27_25, pl1d_grp27_26, pl1d_grp27_27, pl1d_grp27_28, pl1d_grp27_29,
	pl1d_grp27_30, pl1d_grp27_31, pl1d_grp27_32, pl1d_grp27_33, pl1d_grp27_34, pl1d_grp27_35,
	pl1d_grp27_36, pl1d_grp27_37, pl1d_grp27_38, pl1d_grp27_39, pl1d_grp27_40, pl1d_grp27_41,
	pl1d_grp27_41, pl1d_grp27_43, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_stand_init, win_turn_init, win_shine_init, win_shine_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl1d_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,    31, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    32, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    33, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    34, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    35, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    31, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    32, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    33, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    34, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    35, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    36, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    37, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    38, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    40, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 8
   group 25 26
   Char_Fly 26 */
const Sint32 pl1d_stand_02[] = {
	CG_FRAME(    0,    0,   5,           0,    44, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    45, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    46, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    47, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    48, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    49, 0x80,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_stand_02 ),
};

/* Char_Stand 6 */
const Sint32 pl1d_stand_06[] = {
	CG_FRAME(    0,    0,   6,           0,   241, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   240, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   190, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    31, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_stand_06 ),
};

/* Char_Stand 3
   Char_Stand 9 */
const Sint32 pl1d_stand_03[] = {
	CG_FRAME(    0,    0,   5,           0,    50, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    51, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    52, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    53, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    54, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    55, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_stand_03 ),
};

/* Char_Stand 7 */
const Sint32 pl1d_stand_07[] = {
	CG_FRAME(    0,    0,  10,           0,   190, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   240, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   241, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    31, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_stand_07 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl1d_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    78, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1d_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_jump_00_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl1d_jump_03[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_jump_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    90, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_jump_03_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl1d_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    78, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x00,          0,  0x00,       0x18,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl1d_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl1d_dash_00[] = {
	CG_FRAME(    0,    0,   1,           0,    56, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   8,           0,    57, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,    58, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   0,           0,    59, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,    60, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   6,           0,    61, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,    58, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   0,           0,    59, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,    60, 0x80,   0x00,          0,  0x00,       0x04,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   6,           0,    61, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   6,           0,    62, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl1d_dash_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    63, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl1d_dash_00_loop ),
};

/* Char_Dash 2: dash_jump */
const Sint32 pl1d_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_dash_02_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    66, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl1d_dash_01[] = {
	CG_FRAME(    0,    0,   1,           0,    67, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    1,   8,           0,    68, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   1,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   0,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   1,           0,    71, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   6,           0,    72, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   1,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   0,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   1,           0,    71, 0x80,   0x00,          0,  0x00,       0x04,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   6,           0,    72, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   6,           0,    73, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl1d_dash_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    74, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl1d_dash_01_loop ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl1d_dash_03[] = {
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_dash_03_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    77, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_dash_03_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl1d_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1d_crouch_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    88, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_crouch_00_loop ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl1d_crouch_01[] = {
	CG_FRAME(    0,    0,  64,           0,    41, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    43, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl1d_crouch_02[] = {
	CG_FRAME(    0,    0,   6,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_crouch_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    90, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_crouch_02_loop ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl1d_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_turn_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    93, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_turn_00_loop ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl1d_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1d_turn_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    96, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_turn_01_loop ),
};

/* Char_Guard 0
   Char_Guard 3 */
const Sint32 pl1d_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 2 */
const Sint32 pl1d_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   101, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_guard_01 ),
};

/* Char_Guard 4
   Char_Guard 7 */
const Sint32 pl1d_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 6 */
const Sint32 pl1d_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   106, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_guard_05 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl1d_air_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl1d_air_guard_02[] = {
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl1d_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   111, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl1d_stand_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   217, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   218, 0x90,   0x0d,        ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   222, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl1d_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   223, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    1,   2,           0,   224, 0x90,   0x0d,        ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   230, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl1d_stand_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   232, 0x90,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   233, 0x90,   0x0e,        ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   234, 0x90,   0x0d,        ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    1,   2,           0,   235, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    1,  10,           0,   236, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   237, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_stand_p_02 ),
};

/* Char_Special 0: buster_stand_init (Roll Buster)
   Char_Special 1
   Char_Special 4
   Char_Special 6
   Char_Special 7
   Char_Special 8 */
const Sint32 pl1d_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   372, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x90,   0x00,        ATK,  0x00,       0x17,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   8,           0,   379, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   384, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   385, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   386, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   387, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   388, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_special_00 ),
};

/* Char_Special 9: item_ball_init (Weapon Activation) */
const Sint32 pl1d_special_09[] = {
	CG_FRAME(    0,    0,   2,           0,   372, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x90,   0x00,        ATE,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,   379, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   384, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   385, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   387, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   388, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_special_09 ),
};

/* Char_Special 10: item_tornado_init (Weapon Activation) */
const Sint32 pl1d_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   372, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   8,           0,   379, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   381, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   382, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   381, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   382, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   384, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   385, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   386, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   387, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   388, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_special_10 ),
};

/* Char_Special 11: item_shield_init (Weapon Activation) */
const Sint32 pl1d_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   372, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   373, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   374, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   6,           0,   379, 0x90,   0x00,        ATE,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   381, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   382, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   381, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   382, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   384, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   387, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   388, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_special_11 ),
};

/* Char_Special 13: item_throw_init (Weapon Activation) */
const Sint32 pl1d_special_13[] = {
	CG_FRAME(    0,    0,   2,           0,   372, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   5,           0,   379, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   384, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   384, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   385, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   387, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   388, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_special_13 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl1d_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   242, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   2,           0,   243, 0x90,   0x0a,        ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   8,           0,   244, 0x90,   0x0a,        ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   246, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl1d_stand_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   247, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   249, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   255, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy): appear_stand_wait */
const Sint32 pl1d_stand_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   256, 0x90,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x90,   0x0e,        ATK,  0x00,       0x04,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   259, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   260, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,   2,           0,   261, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   262, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   263, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   5,           0,   264, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   267, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl1d_crouch_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   268, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   269, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   1,           0,   270, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   1,           0,   271, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   8,           0,   272, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   273, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl1d_crouch_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   274, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    1,   1,           0,   275, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   1,           0,   276, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   1,           0,   277, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,  10,           0,   278, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   279, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl1d_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   280, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   281, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   282, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   1,           0,   283, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   1,           0,   284, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,  10,           0,   285, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   287, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl1d_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   289, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   1,           0,   290, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   8,           0,   292, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   294, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl1d_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   302, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   309, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl1d_crouch_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   301, 0x90,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   295, 0x90,   0x0d,        ATK,  0x00,       0x04,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,   1,           0,   296, 0x90,   0x2a,        ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x90,   0x2a,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x90,   0x2a,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x90,   0x2a,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x90,   0x2a,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x90,   0x2a,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   6,           0,   299, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   8,           0,   300, 0x90,   0x0f,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   301, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl1d_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   310, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   311, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   312, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   313, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,  16,           0,   314, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   315, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl1d_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl1d_jump_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   316, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   317, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   1,           0,   318, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   1,           0,   319, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   8,           0,   320, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   322, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl1d_jump_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   323, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   323, 0x90,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   324, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   325, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   326, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,  10,           0,   327, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   328, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_jump_p_02 ),
};

/* Char_Special 2: buster_air_init (Roll Buster) */
const Sint32 pl1d_special_02[] = {
	CG_FRAME(    0,    0,   2,           0,   397, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x90,   0x00,        ATK,  0x00,       0x17,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   399, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   4,           0,   404, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   405, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   407, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   405, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   407, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   408, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   409, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   410, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   411, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   412, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   413, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_special_02 ),
};

/* Char_Special 14: item_air_throw_init (Weapon Activation) */
const Sint32 pl1d_special_14[] = {
	CG_FRAME(    0,    0,   2,           0,   397, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   399, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   8,           0,   404, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   408, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   409, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   410, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   411, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   7,           0,   412, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   413, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_special_14 ),
};

/* Char_Special 15: snapback_init (Snap Back) */
const Sint32 pl1d_special_15[] = {
	CG_FRAME(    0,    0,   5,           0,   256, 0x90,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   5,           0,   257, 0x90,   0x0e,        ATK,  0x00,       0x04,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,   3,           0,   258, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   259, 0x90,   0x0e,        ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   260, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,   2,           0,   261, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   262, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   263, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   5,           0,   264, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   267, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_special_15 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl1d_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   335, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   0,           0,   329, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   330, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   1,           0,   331, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   1,           0,   332, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,  10,           0,   333, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl1d_jump_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   336, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   337, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   338, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   339, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   340, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME( 0x80,    0,   1,           0,   341, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   3,           0,   342, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   343, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   344, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   345, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   346, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   347, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl1d_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   348, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   349, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   350, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME( 0x80,    0,   1,           0,   351, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   352, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   353, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   354, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_jump_k_02 ),
};

/* Char_Jump_K 6 */
const Sint32 pl1d_jump_k_06[] = {
	CG_FRAME(    0,    0,   4,           0,   355, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   356, 0x90,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   357, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   2,           0,   358, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   2,           0,   359, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   8,           0,   360, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   361, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_jump_k_06 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm
   Char_Damage 3 */
const Sint32 pl1d_damage_00[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   435, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_damage_00 ),
};

/* Char_Damage 1
   Char_Damage 4 */
const Sint32 pl1d_damage_01[] = {
	CG_FRAME(    0,    0,  18, END_OF_DATA,   435, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_damage_01 ),
};

/* Char_Damage 2
   Char_Damage 5 */
const Sint32 pl1d_damage_02[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   435, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_damage_02 ),
};

/* Char_Damage 6 */
const Sint32 pl1d_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   436, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl1d_damage_07[] = {
	CG_FRAME(    0,    0,  18, END_OF_DATA,   436, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl1d_damage_08[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   436, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1d_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl1d_damage_09[] = {
	CG_FRAME(    0,    0,   3,           0,   438, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   439, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   440, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   443, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1d_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   454, 0x80,   0x2e,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1d_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow
   Char_Damage 11
   Char_Damage 12 */
const Sint32 pl1d_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   435, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_10 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl1d_damage_13[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   437, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1d_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl1d_damage_16[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   446, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl1d_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   447, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl1d_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   448, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl1d_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   449, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl1d_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   450, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl1d_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   446, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl1d_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl1d_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl1d_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl1d_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   450, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl1d_damage_26[] = {
	CG_FRAME(    0,    0,   3,           0,   451, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   452, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   453, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1d_damage_26_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   454, 0x80,   0x2e,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1d_damage_26_loop ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl1d_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   176, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   175, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   159, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    88, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl1d_damage_28[] = {
	CG_FRAME(    0,    0,   6,           0,   455, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   456, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl1d_damage_29[] = {
	CG_FRAME(    0,    0,   6,           0,   457, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   458, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl1d_damage_30[] = {
	CG_FRAME(    0,    0,   3,           0,   176, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   175, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl1d_damage_31[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   454, 0x40,   0x2e,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1d_damage_31 ),
};

/* Char_Dizzy 0: win_roll_2_wait, act_dizzy */
const Sint32 pl1d_dizzy_00[] = {
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   461, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   462, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   463, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   464, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   466, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_dizzy_00 ),
};

/* Char_Damage 35 */
const Sint32 pl1d_damage_35[] = {
	CG_FRAME(    0,    0,   3,           0,   446, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   455, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_damage_35 ),
};

/* Char_Caught 0 */
const Sint32 pl1d_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl1d_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl1d_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl1d_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl1d_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl1d_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl1d_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   436, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1d_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl1d_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   436, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1d_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl1d_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   436, 0x40,   0x14,          0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1d_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl1d_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl1d_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl1d_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   435, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl1d_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   437, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1d_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl1d_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   446, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1d_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl1d_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   447, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl1d_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   448, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1d_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl1d_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   449, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl1d_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   450, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl1d_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   455, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl1d_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   457, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl1d_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   454, 0x40,   0x2e,          0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1d_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl1d_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   447, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1d_caught_21 ),
};

/* Char_Catch 0: nml_set */
const Sint32 pl1d_catch_00[] = {
	CG_FRAME(    0,    0,   1,           0,   362, 0x80,   0x0f,          0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x0e,          0,  0x00,       0x04,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   6,           0,   364, 0x80,   0x0e,          0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    1,   4,           0,   365, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   366, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   367, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   368, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   369, 0x80,   0x0e,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   370, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   371, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_catch_00 ),
};

/* Char_Catch 1: jump_set */
const Sint32 pl1d_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   414, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x80,   0x00,          0,  0x00,       0x04,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    0,   4,           0,   416, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    1,   4,           0,   417, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   418, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   419, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_catch_01 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl1d_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   158, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   159, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   160, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   161, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl1d_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   169, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   170, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   173, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl1d_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   172, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   173, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl1d_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   184, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_get_up_03 ),
};

/* Char_Appear 0: appear_ride_init */
const Sint32 pl1d_appear_00[] = {
	CG_FRAME(    0,    1,   6,           0,   421, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   422, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_appear_00 ),
};

/* Char_Appear 2: appear_ride_wait */
const Sint32 pl1d_appear_02[] = {
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl1d_appear_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    74, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl1d_appear_02_loop ),
};

/* Char_Appear 3: appear_ride_fall */
const Sint32 pl1d_appear_03[] = {
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,          0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_appear_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    77, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_appear_03_loop ),
};

/* Char_Appear 1 */
const Sint32 pl1d_appear_01[] = {
	CG_FRAME(    0,    0,   6,           0,   467, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   6,           0,   468, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,   6,           0,   469, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,   6,           0,   470, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    0,   6,           0,   471, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    5,    0,   6,           0,   472, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    6,    0,   6,           0,   473, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    7,    0,   6, END_OF_DATA,   474, 0x80,   0x31,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_appear_01 ),
};

/* Char_Appear 4: appear_drop_init */
const Sint32 pl1d_appear_04[] = {
	CG_FRAME(    0,    1,   6,           0,    83, 0x80,   0x00,          0,  0x00,          0,    1,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,    84, 0x80,   0x00,          0,  0x00,          0,    1,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_appear_04 ),
};

/* Char_Appear 5: appear_drop_wait */
const Sint32 pl1d_appear_05[] = {
	CG_FRAME(    0,    0,   6,           0,   167, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_appear_05 ),
};

/* Char_Appear 6: appear_drop_move */
const Sint32 pl1d_appear_06[] = {
	CG_FRAME(    0,    0,   6,           0,    74, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl1d_appear_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_appear_06_loop ),
};

/* Char_Appear 7: appear_drop_fall */
const Sint32 pl1d_appear_07[] = {
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,          0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_appear_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    77, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_appear_07_loop ),
};

/* Char_Appear 8 */
const Sint32 pl1d_appear_08[] = {
	CG_FRAME(    0,    0,   6,           0,   950, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   945, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_appear_08 ),
};

/* Char_Extra 2: appear_stand_move, chouhatsu_init (taunt), fly_start5 */
const Sint32 pl1d_extra_02[] = {
	CG_FRAME(    0,    0,   6,           0,   423, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   424, 0x80,   0x00,          0,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   425, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   425, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   428, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   429, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_extra_02 ),
};

/* Char_Extra 3: pl1d_upper, fly_start5 */
const Sint32 pl1d_extra_03[] = {
	CG_FRAME(    0,    0,   1,           0,   256, 0x90,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x90,   0x0e,        ATK,  0x00,       0x04,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   259, 0x90,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   260, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,   2,           0,   261, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   262, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   263, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   5,           0,   264, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x90,   0x0b,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x90,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   267, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1d_extra_03 ),
};

/* Char_Special 3: flower_init (Flowers) */
const Sint32 pl1d_special_03[] = {
	CG_FRAME(    0,    1,   3,           0,   372, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,   373, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  16,           0,   232, 0x90,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   2,           0,   233, 0x90,   0x0e,        ATK,  0x00,       0x19,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   234, 0x90,   0x0d,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x90,   0x0d,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  12,           0,   236, 0x90,   0x0d,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   237, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_special_03 ),
};

/* Char_Special 5: flower_air_init (Flowers) */
const Sint32 pl1d_special_05[] = {
	CG_FRAME(    0,    1,   3,           0,   397, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   398, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    2,    0,  16,           0,   323, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    3,    0,   2,           0,   324, 0x90,   0x00, ATK | 0x11,  0x00,       0x19,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   2,           0,   325, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   326, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  12,           0,   327, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   328, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1d_special_05 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl1d_extra_00[] = {
	CG_FRAME( 0x80,    0,   1,           0,   296, 0x90,   0x0a,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x80,    0,   1,           0,   297, 0x90,   0x0a,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   298, 0x90,   0x0a,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl1d_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl1d_extra_01[] = {
	CG_FRAME(    0,    0,   3,           0,   299, 0x90,   0x0d,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   300, 0x90,   0x0f,        ATE,  0x00,       0x0c,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,    41, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   138, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_extra_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   139, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_extra_01_loop ),
};

/* Char_Win 0: win_turn_wait */
const Sint32 pl1d_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,    86, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,    87, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,    88, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   6,           0,   112, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   113, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   114, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   115, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   116, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   117, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   118, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   119, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   120, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   121, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   122, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_win_00_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   123, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   124, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   125, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   126, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   127, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   128, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   129, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   130, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   131, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   127, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   128, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   129, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   130, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   131, 0x80,   0x2d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_win_00_loop ),
};

/* Char_Win 1: win_held_init, win_shine_pose_wait
   Char_Win 4: win_ko_init */
const Sint32 pl1d_win_01[] = {
	CG_FRAME(    0,    0,   6,           0,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   133, 0x80,   0x00,          0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   134, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_win_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_win_01_loop ),
};

/* Char_Win 6: win_shine_wait */
const Sint32 pl1d_win_06[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_win_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   147, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_win_06_loop ),
};

/* Char_Win 7: win_pose_init */
const Sint32 pl1d_win_07[] = {
	CG_FRAME(    0,    0,   6,           0,    86, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    89, 0x80,   0x00,          0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_win_07_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   202, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_win_07_loop ),
};

/* Char_Win 8: win_roll_wait */
const Sint32 pl1d_win_08[] = {
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_win_08_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_win_08_loop ),
};

/* Char_Win 2
   Char_Win 3
   Char_Win 5: lose_init */
const Sint32 pl1d_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,   148, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   149, 0x80,   0x00,          8,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_win_02_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   150, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   151, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_win_02_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl1d_change_00[] = {
	CG_FRAME(    0,    0,   2,           0,   204, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   210, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_change_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   208, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl1d_change_01[] = {
	CG_FRAME( 0x80,    0,   6,           0,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   133, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   134, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   6,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_change_01_loop[] = {
	CG_FRAME( 0x80,    0,   6, END_OF_DATA,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_change_01_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl1d_change_03[] = {
	CG_FRAME(    0,    0,   6,           0,   389, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   390, 0x80,   0x00,          0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   392, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   393, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   394, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   395, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1d_change_03_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   396, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl1d_change_04[] = {
	CG_FRAME(    0,    0,   3,           0,   423, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   424, 0x80,   0x00,          0,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   427, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   428, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   429, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_change_04 ),
};

/* Char_Special 12: item_sel_init (Summon power-up: Rock Ball) */
const Sint32 pl1d_special_12[] = {
	CG_FRAME(    0,    0,   6,           0,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,          0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   0,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_special_12 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl1d_change_02[] = {
	CG_FRAME(    0,    0,   1,           0,    67, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl1d_change_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    72, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl1d_change_02_loop ),
};

/* Char_Super 0: hyper_roll_init (Hyper Roll (hyper)) */
const Sint32 pl1d_super_00[] = {
	CG_FRAME(    0,    0,   3,           0,   211, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x90,   0x00,        ATK,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   214, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   215, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   132, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_00 ),
};

/* Char_Super 1 */
const Sint32 pl1d_super_01[] = {
	CG_FRAME(    0,    0,   1,           0,   420, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   8,           0,   583, 0x90,   0x33,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   8,           0,   584, 0x90,   0x53,        ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    1,  24,           0,   574, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   576, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   577, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1d_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl1d_super_02[] = {
	CG_FRAME(    0,    0,   2,           0,   420, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   8,           0,   583, 0x90,   0x33,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   584, 0x90,   0x53,        ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    1,  24,           0,   574, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   576, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   577, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1d_super_02 ),
};

/* Char_Super 3: hyper_roll_main_init (Hyper Roll (hyper)) */
const Sint32 pl1d_super_03[] = {
	CG_FRAME(    0,    0,   2,           0,   578, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl1d_super_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   579, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   580, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0,   581, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   578, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1d_super_03_loop ),
};

/* Char_Super 29: shl1d_05_init */
const Sint32 pl1d_super_29[] = {
	CG_FRAME(    0,    1,   4,           0, 32777, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   4,           0, 32778, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
};

const Sint32 pl1d_super_29_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32779, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    1,    0,   4,           0, 32780, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    2,    0,   4,           0, 32779, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    3,    0,   4, END_OF_DATA, 32780, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl1d_super_29_loop ),
};

/* Char_Super 41: shl1d_05_init, shl1d_05_char_set */
const Sint32 pl1d_super_41[] = {
	CG_FRAME(    0,    0,   4,           0, 32781, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0, 32782, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32784, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl1d_super_41 ),
};

/* Char_Super 42: shl1d_05_init, shl1d_05_char_set */
const Sint32 pl1d_super_42[] = {
	CG_FRAME(    0,    0,   4,           0, 32785, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32786, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl1d_super_42 ),
};

/* Char_Super 4: hyper_roll_main (Hyper Roll (hyper)) */
const Sint32 pl1d_super_04[] = {
	CG_FRAME(    0,    0,   6,           0,   578, 0x90,   0x73,        ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   577, 0x90,   0x73,        ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   576, 0x90,   0x73,        ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   9,           0,   574, 0x90,   0x73,        ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   584, 0x90,   0x53,        ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl1d_super_04_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   583, 0x90,   0x33,        ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1d_super_04_loop ),
};

/* Char_Super 5: hyper_roll_end_init (Hyper Roll (hyper)) */
const Sint32 pl1d_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,   204, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   210, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_super_05 ),
};

/* Char_Super 6: hyper_roll_end_init (Hyper Roll (hyper)) */
const Sint32 pl1d_super_06[] = {
	CG_FRAME(    0,    0,  22,           0,    82, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1d_super_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_super_06_loop ),
};

/* Char_Super 27 */
const Sint32 pl1d_super_27[] = {
	CG_FRAME(    0,    0,   3,           0,   211, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   214, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   215, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   216, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   132, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   420, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   583, 0x90,   0x33,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   584, 0x90,   0x53,        ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    1,  30,           0,   574, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,  48, END_OF_DATA,   576, 0x90,   0x73,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1d_super_27 ),
};

/* Char_Super 28 */
const Sint32 pl1d_super_28[] = {
	CG_FRAME(    0,    1,   6,           0,   574, 0x90,   0x73,        ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   584, 0x90,   0x53,        ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   6,           0,   583, 0x90,   0x33,        ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    1,  12, END_OF_DATA,   420, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_super_28 ),
};

/* Char_Super 17: beat_init (Beat Plane) */
const Sint32 pl1d_super_17[] = {
	CG_FRAME(    0,    1,   4,           0,   211, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   212, 0x90,   0x00,        ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   213, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   214, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   215, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   216, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   132, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   133, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   134, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   139, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_17 ),
};

/* Char_Super 33: beat_main (Beat Plane) */
const Sint32 pl1d_super_33[] = {
	CG_FRAME(    0,    1,  32,           0,   558, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
};

const Sint32 pl1d_super_33_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    -1, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1d_super_33_loop ),
};

/* Char_Super 25: beat_end_init (Beat Plane) */
const Sint32 pl1d_super_25[] = {
	CG_FRAME(    0,    0,   4,           0,   204, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   210, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_super_25 ),
};

/* Char_Super 26: beat_end_init (Beat Plane) */
const Sint32 pl1d_super_26[] = {
	CG_FRAME(    0,    0,  22,           0,    82, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1d_super_26_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_super_26_loop ),
};

/* Char_Super 18: beat_charge (Beat Plane), beat_main (Beat Plane) */
const Sint32 pl1d_super_18[] = {
	CG_FRAME(    0,    0,   6,           0,   558, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   6,           0,   559, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   6,           0,   560, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   6,           0,   561, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   562, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1d_super_18 ),
};

/* Char_Super 19 */
const Sint32 pl1d_super_19[] = {
	CG_FRAME(    0,    0,   1,           0,   551, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   552, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   553, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   554, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_19 ),
};

/* Char_Super 20: beat_main (Beat Plane) */
const Sint32 pl1d_super_20[] = {
	CG_FRAME(    0,    0,   4,           0,   572, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   573, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1d_super_20 ),
};

/* Char_Super 21 */
const Sint32 pl1d_super_21[] = {
	CG_FRAME(    0,    0,  10,           0,   555, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   556, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   557, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   556, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_21 ),
};

/* Char_Super 22 */
const Sint32 pl1d_super_22[] = {
	CG_FRAME(    0,    0,   8,           0,   563, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   564, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   565, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_22 ),
};

/* Char_Super 23 */
const Sint32 pl1d_super_23[] = {
	CG_FRAME(    0,    0,   4,           0,   566, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   567, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   568, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   569, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_23 ),
};

/* Char_Super 24 */
const Sint32 pl1d_super_24[] = {
	CG_FRAME(    0,    0,   1,           0,    -1, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   570, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6,           0,   571, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   570, 0x40,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    -1, 0x40,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_24 ),
};

/* Char_Super 30 */
const Sint32 pl1d_super_30[] = {
	CG_FRAME(    0,    0,   4,           0,  1008, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,  1009, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,  1010, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,  1011, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,  1012, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,  1013, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,  1014, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  1015, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl1d_super_30 ),
};

/* Char_Super 31 */
const Sint32 pl1d_super_31[] = {
	CG_FRAME(    0,    0,   4,           0,  1016, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1017, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1018, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1019, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1020, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  1021, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_31 ),
};

/* Char_Super 32 */
const Sint32 pl1d_super_32[] = {
	CG_FRAME(    0,    0,   4,           0,  1022, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,  1023, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  1024, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl1d_super_32 ),
};

/* Char_Super 40 */
const Sint32 pl1d_super_40[] = {
	CG_FRAME(    0,    0,   6,           0,   838, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   837, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   838, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   839, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   840, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   841, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_super_40_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   842, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_40_loop ),
};

/* Char_Super 7: drill_init (Rush Drill (hyper)) */
const Sint32 pl1d_super_07[] = {
	CG_FRAME(    0,    1,   4,           0,   211, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   212, 0x90,   0x00,        ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   213, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   214, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   215, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   216, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   132, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   133, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   134, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   139, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_07 ),
};

/* Char_Super 34: drill_main (Rush Drill (hyper)) */
const Sint32 pl1d_super_34[] = {
	CG_FRAME(    0,    1,  32,           0,    -1, 0x80,   0x52,        ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
};

const Sint32 pl1d_super_34_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    -1, 0x80,   0xff,        ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl1d_super_34_loop ),
};

/* Char_Super 15: drill_end_init (Rush Drill (hyper)) */
const Sint32 pl1d_super_15[] = {
	CG_FRAME(    0,    0,   4,           0,   204, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   210, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1d_super_15 ),
};

/* Char_Super 16: drill_end_init (Rush Drill (hyper)) */
const Sint32 pl1d_super_16[] = {
	CG_FRAME(    0,    0,  22,           0,    82, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1d_super_16_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1d_super_16_loop ),
};

/* Char_Super 8: drill_charge (Rush Drill (hyper)), drill_turn (Rush Drill (hyper)), drill_walk (Rush Drill (hyper)), drill_back (Rush Drill (hyper)), ... */
const Sint32 pl1d_super_08[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    -1, 0x90,   0x52,        ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl1d_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl1d_super_09[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   537, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_09 ),
};

/* Char_Super 11 */
const Sint32 pl1d_super_11[] = {
	CG_FRAME(    0,    1,   2,           0,   542, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0,   543, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0,   544, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   545, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl1d_super_11 ),
};

/* Char_Super 12 */
const Sint32 pl1d_super_12[] = {
	CG_FRAME(    0,    0,   3,           0,   546, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   547, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   548, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_12 ),
};

/* Char_Super 38 */
const Sint32 pl1d_super_38[] = {
	CG_FRAME(    0,    0,   3,           0,  1000, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1001, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1002, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  1003, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_38 ),
};

/* Char_Super 39 */
const Sint32 pl1d_super_39[] = {
	CG_FRAME(    0,    0,   3,           0,  1004, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1005, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1006, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,  1007, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_39 ),
};

/* Char_Super 13 */
const Sint32 pl1d_super_13[] = {
	CG_FRAME(    3,    0,   3,           0,   538, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   3,           0,   539, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   3,           0,   540, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   3, END_OF_DATA,   541, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_13 ),
};

/* Char_Super 36 */
const Sint32 pl1d_super_36[] = {
	CG_FRAME(    3,    0,   1,           0,   538, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   1,           0,   539, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   1,           0,   540, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   1, END_OF_DATA,   541, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_36 ),
};

/* Char_Super 35 */
const Sint32 pl1d_super_35[] = {
	CG_FRAME(    0,    3,   3,           0,   541, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   3,           0,   540, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   3,           0,   539, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   538, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_35 ),
};

/* Char_Super 37 */
const Sint32 pl1d_super_37[] = {
	CG_FRAME(    0,    3,   1,           0,   541, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   1,           0,   540, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   1,           0,   539, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,   1, END_OF_DATA,   538, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_37 ),
};

/* Char_Super 14 */
const Sint32 pl1d_super_14[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   549, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_super_14 ),
};

/* Char_Super 10: drill_turn (Rush Drill (hyper)) */
const Sint32 pl1d_super_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   550, 0x80,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl1d_super_10 ),
};

/* Char_Effect 0 */
const Sint32 pl1d_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0,   488, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   490, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   491, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   493, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl1d_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl1d_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   494, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   495, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   496, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   497, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   499, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl1d_effect_02[] = {
	CG_FRAME(    0,    0,   2,           0,   506, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   508, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   509, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   505, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1d_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl1d_effect_03[] = {
	CG_FRAME(    0,    0,   2,           0,   511, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_03 ),
};

/* Char_Effect 9 */
const Sint32 pl1d_effect_09[] = {
	CG_FRAME(    0,    0,   3,           0, 32791, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32792, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32793, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32794, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32795, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32796, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32797, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32798, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl1d_effect_10[] = {
	CG_FRAME(    0,    0,   3,           0, 32791, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32792, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32793, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32794, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32795, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32796, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32797, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32798, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl1d_effect_10 ),
};

/* Char_Effect 19 */
const Sint32 pl1d_effect_19[] = {
	CG_FRAME(    0,    0,   3,           0,   965, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   966, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   967, 0x40,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl1d_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl1d_effect_20[] = {
	CG_FRAME(    0,    0,   3,           0,   965, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   966, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   967, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl1d_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl1d_effect_21[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   968, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl1d_effect_22[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   969, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl1d_effect_23[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   970, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl1d_effect_24[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   971, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl1d_effect_25[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   972, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_25 ),
};

/* Char_Effect 26 */
const Sint32 pl1d_effect_26[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1025, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl1d_effect_26 ),
};

/* Char_Effect 4 */
const Sint32 pl1d_effect_04[] = {
	CG_FRAME(    0,    0,   6,           0,   973, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   974, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   975, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_effect_04_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   976, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_04_loop ),
};

/* Char_Effect 5 */
const Sint32 pl1d_effect_05[] = {
	CG_FRAME(    0,    0,   4,           0,   977, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   978, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   979, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,   4,           0,   977, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   978, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   979, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,   3,           0,   977, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   978, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   979, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   977, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   978, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   979, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   977, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
};

const Sint32 pl1d_effect_05_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   978, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   979, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   977, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl1d_effect_05_loop ),
};

/* Char_Effect 6 */
const Sint32 pl1d_effect_06[] = {
	CG_FRAME(    0,    0,   3,           0,   983, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   984, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   985, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   986, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl1d_effect_07[] = {
	CG_FRAME(    0,    0,   3,           0,   987, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   988, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl1d_effect_08[] = {
	CG_FRAME(    0,    0,   3,           0,   989, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   990, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   991, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   992, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   993, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   994, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   995, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   996, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_08 ),
};

/* Char_Effect 11
   Char_Effect 12 */
const Sint32 pl1d_effect_11[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32775, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl1d_effect_11 ),
};

/* group 27 43 */
const Sint32 pl1d_grp27_43[] = {
	EF_FRAME(    0,    0,   6, END_OF_DATA, 32775 ),
	CG_NEXT( pl1d_grp27_43 ),
};

/* Char_Effect 13 */
const Sint32 pl1d_effect_13[] = {
	CG_FRAME(    0,    0,   4,           0,   787, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   784, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   785, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   786, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   781, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl1d_effect_13_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   769, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   770, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   771, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   772, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   773, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   774, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl1d_effect_13_loop ),
};

/* Char_Effect 14 */
const Sint32 pl1d_effect_14[] = {
	CG_FRAME(    0,    0,   3,           0,   775, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   776, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   777, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   778, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   779, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   780, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl1d_effect_15[] = {
	CG_FRAME(    0,    0,   4,           0,   824, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   822, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   823, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   820, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   821, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   822, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   823, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
};

const Sint32 pl1d_effect_15_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   802, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   803, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   804, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   805, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   806, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   807, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   808, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   809, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   810, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   811, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   812, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   813, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl1d_effect_15_loop ),
};

/* Char_Effect 16 */
const Sint32 pl1d_effect_16[] = {
	CG_FRAME(    0,    0,   3,           0,   814, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   815, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   816, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   817, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   818, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   819, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl1d_effect_17[] = {
	CG_FRAME(    0,    0,   4,           0,   801, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   800, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
};

const Sint32 pl1d_effect_17_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   788, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   789, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   790, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   791, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   792, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   793, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl1d_effect_17_loop ),
};

/* Char_Effect 18 */
const Sint32 pl1d_effect_18[] = {
	CG_FRAME(    0,    0,   3,           0,   794, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   795, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   796, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   797, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   798, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   799, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_effect_18 ),
};

const Sint32 pl1d_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   6,           0,   420, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   582, 0x90,   0x13,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   583, 0x90,   0x33,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   584, 0x90,   0x53,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   574, 0x90,   0x73,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   574, 0x90,   0x73,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  48, END_OF_DATA,   576, 0x90,   0x73,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00006370,
};

/* group 25 0
   Char_Fly 0 */
const Sint32 pl1d_grp25_00[] = {
	CG_FRAME(    0,    0,   5,           0,   887, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   888, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   889, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   890, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   889, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   888, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_00 ),
};

/* group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl1d_grp25_01[] = {
	CG_FRAME(    0,    0,   6,           0,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_01 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl1d_grp25_02[] = {
	CG_FRAME(    0,    0,   6,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_02 ),
};

/* group 25 3
   Char_Fly 3 */
const Sint32 pl1d_grp25_03[] = {
	CG_FRAME(    0,    0,   5,           0,   893, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   894, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   895, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   896, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   895, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   894, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_03 ),
};

/* group 25 4
   Char_Fly 4 */
const Sint32 pl1d_grp25_04[] = {
	CG_FRAME(    0,    0,   4,           0,   897, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   898, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   899, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   900, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   901, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   902, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   903, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   904, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_04 ),
};

/* group 25 5
   Char_Fly 5 */
const Sint32 pl1d_grp25_05[] = {
	CG_FRAME(    0,    0,   3,           0,   905, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   906, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   907, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   908, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   909, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   910, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   911, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   912, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_05 ),
};

/* group 25 6
   Char_Fly 6 */
const Sint32 pl1d_grp25_06[] = {
	CG_FRAME(    0,    0,   4,           0,   913, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   914, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_06_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   915, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_06_loop ),
};

/* group 25 7
   Char_Fly 7 */
const Sint32 pl1d_grp25_07[] = {
	CG_FRAME(    0,    0,   6,           0,   887, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_07_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   916, 0x40,   0x03,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_07_loop ),
};

/* group 25 8
   Char_Fly 8 */
const Sint32 pl1d_grp25_08[] = {
	CG_FRAME(    0,    0,   6,           0,   893, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   921, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   922, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   923, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_08 ),
};

/* group 25 9
   Char_Fly 9 */
const Sint32 pl1d_grp25_09[] = {
	CG_FRAME(    0,    0,   3,           0,   924, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   925, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_09_loop ),
};

/* group 25 10
   Char_Fly 10 */
const Sint32 pl1d_grp25_10[] = {
	CG_FRAME(    0,    0,   3,           0,   931, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_10_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   932, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   933, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   934, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   935, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   934, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   933, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_10_loop ),
};

/* group 25 11
   Char_Fly 11 */
const Sint32 pl1d_grp25_11[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   931, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_11 ),
};

/* group 25 12
   Char_Fly 12 */
const Sint32 pl1d_grp25_12[] = {
	CG_FRAME(    0,    0,   1,           0,   936, 0x40,   0xff,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_12_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_12_loop ),
};

/* group 25 15
   Char_Fly 15 */
const Sint32 pl1d_grp25_15[] = {
	CG_FRAME(    0,    0,   3,           0,   894, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_15_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   937, 0x40,   0xff,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_15_loop ),
};

/* group 25 20
   Char_Fly 20 */
const Sint32 pl1d_grp25_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   945, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_20 ),
};

/* group 25 21
   Char_Fly 21 */
const Sint32 pl1d_grp25_21[] = {
	CG_FRAME(    0,    0,   8,           0,   946, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   947, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   948, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   949, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_21 ),
};

/* group 25 22
   Char_Fly 22 */
const Sint32 pl1d_grp25_22[] = {
	CG_FRAME(    0,    0,   4,           0,   950, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   951, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   952, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   953, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   954, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16,           0,   955, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   954, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   952, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   950, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_22 ),
};

/* group 25 23
   Char_Fly 23 */
const Sint32 pl1d_grp25_23[] = {
	CG_FRAME(    0,    0,   1,           0,   956, 0x40,   0xff,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   950, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_23_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   945, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_23_loop ),
};

/* group 25 24
   Char_Fly 24 */
const Sint32 pl1d_grp25_24[] = {
	CG_FRAME(    0,    0,   3,           0,   945, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   950, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_24_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   937, 0x40,   0xff,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_24_loop ),
};

/* group 25 19
   Char_Fly 19 */
const Sint32 pl1d_grp25_19[] = {
	CG_FRAME(    0,    0,   4,           0,   475, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   477, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   480, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   481, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   482, 0x40,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_19 ),
};

/* group 25 13
   Char_Fly 13 */
const Sint32 pl1d_grp25_13[] = {
	CG_FRAME(    0,    0,   1,           0,   847, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   848, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_13 ),
};

/* group 25 14
   Char_Fly 14 */
const Sint32 pl1d_grp25_14[] = {
	CG_FRAME(    0,    0,   4,           0,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   921, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   922, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   923, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   938, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   939, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   940, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   941, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   942, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_14_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   943, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   944, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_14_loop ),
};

/* group 25 18
   Char_Fly 18 */
const Sint32 pl1d_grp25_18[] = {
	CG_FRAME(    0,    0,   1,           0,   936, 0x40,   0xff,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   921, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   922, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   923, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   938, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   939, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   940, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   941, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   942, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_18_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   943, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   944, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_18_loop ),
};

/* group 25 28
   Char_Fly 28 */
const Sint32 pl1d_grp25_28[] = {
	CG_FRAME(    0,    0,   3,           0,   834, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   835, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   836, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   825, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   826, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   827, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   828, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   829, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   830, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   831, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   832, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   833, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_28 ),
};

/* group 25 29
   Char_Fly 29 */
const Sint32 pl1d_grp25_29[] = {
	CG_FRAME(    0,    0,   5,           0,   877, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   878, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   879, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   880, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   881, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   882, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   883, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   884, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  15,           0,   885, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_29_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   886, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_29_loop ),
};

/* group 25 30
   Char_Fly 30 */
const Sint32 pl1d_grp25_30[] = {
	CG_FRAME(    0,    0,   6,           0,   185, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   186, 0x80,   0x0d,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_grp25_30 ),
};

/* group 25 31
   Char_Fly 31 */
const Sint32 pl1d_grp25_31[] = {
	CG_FRAME(    0,    0,   6,           0,   150, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   151, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_grp25_31 ),
};

/* group 25 16
   Char_Fly 16 */
const Sint32 pl1d_grp25_16[] = {
	CG_FRAME(    0,    0,   1,           0,   909, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   910, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   911, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   912, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   887, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_16_loop[] = {
	CG_FRAME(    0,    0,   5,           0,   893, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   894, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   895, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   896, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   895, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   894, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_16_loop ),
};

/* group 25 17
   Char_Fly 17 */
const Sint32 pl1d_grp25_17[] = {
	CG_FRAME(    0,    0,   1,           0,   893, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   892, 0x40,   0x12,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   891, 0x40,   0x11,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   887, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   913, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   914, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   915, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_17 ),
};

/* group 25 25
   Char_Fly 25 */
const Sint32 pl1d_grp25_25[] = {
	CG_FRAME(    0,    0,   6,           0,   869, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   870, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   871, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   872, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   873, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1d_grp25_25_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   874, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   875, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   876, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_25_loop ),
};

/* group 25 27
   Char_Fly 27
   group 27 0
   group 27 1
   group 27 2
   group 27 5 */
const Sint32 pl1d_grp25_27[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    31, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1d_grp25_27 ),
};

/* group 25 32
   Char_Fly 32 */
const Sint32 pl1d_grp25_32[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   239, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1d_grp25_32 ),
};

/* group 27 3 */
const Sint32 pl1d_grp27_03[] = {
	EF_FRAME(    0,    0,   2,           0,   483 ),
	EF_FRAME(    0,    0,   2,           0,   484 ),
	EF_FRAME(    0,    0,   2,           0,   485 ),
	EF_FRAME(    0,    0,   2,           0,   486 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   487 ),
	CG_NEXT( pl1d_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl1d_grp27_04[] = {
	EF_FRAME(    0,    0,   2,           0,   500 ),
	EF_FRAME(    0,    0,   2,           0,   501 ),
	EF_FRAME(    0,    0,   2,           0,   502 ),
	EF_FRAME(    0,    0,   2,           0,   503 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   504 ),
	CG_NEXT( pl1d_grp27_04 ),
};

/* group 27 6 */
const Sint32 pl1d_grp27_06[] = {
	EF_FRAME(    0,    0,   3,           0,   517 ),
	EF_FRAME(    0,    0,   3,           0,   518 ),
	EF_FRAME(    0,    0,   3,           0,   519 ),
	EF_FRAME(    0,    0,   3,           0,   520 ),
	EF_FRAME(    0,    0,   3,           0,   521 ),
	EF_FRAME(    0,    0,   3,           0,   522 ),
	EF_FRAME(    0,    0,   3,           0,   523 ),
	EF_FRAME(    0,    0,   3,           0,   524 ),
	EF_FRAME(    0,    0,   3,           0,   525 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   526 ),
	CG_NEXT( pl1d_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl1d_grp27_07[] = {
	EF_FRAME(    0,    0,   3,           0,   527 ),
	EF_FRAME(    0,    0,   3,           0,   528 ),
	EF_FRAME(    0,    0,   3,           0,   529 ),
	EF_FRAME(    0,    0,   3,           0,   530 ),
	EF_FRAME(    0,    0,   3,           0,   531 ),
	EF_FRAME(    0,    0,   3,           0,   532 ),
	EF_FRAME(    0,    0,   3,           0,   533 ),
	EF_FRAME(    0,    0,   3,           0,   534 ),
	EF_FRAME(    0,    0,   3,           0,   535 ),
	EF_FRAME(    0,    0,   3,           0,   536 ),
	EF_FRAME(    0,    0,   3,           0,   527 ),
	EF_FRAME(    0,    0,   3,           0,   528 ),
	EF_FRAME(    0,    0,   3,           0,   529 ),
	EF_FRAME(    0,    0,   3,           0,   530 ),
	EF_FRAME(    0,    0,   3,           0,   531 ),
	EF_FRAME(    0,    0,   3,           0,   532 ),
	EF_FRAME(    0,    0,   3,           0,   533 ),
	EF_FRAME(    0,    0,   3,           0,   534 ),
	EF_FRAME(    0,    0,   3,           0,   535 ),
	EF_FRAME(    0,    0,   3,           0,   536 ),
	EF_FRAME(    0,    0,   2,           0,   527 ),
	EF_FRAME(    0,    0,   2,           0,   528 ),
	EF_FRAME(    0,    0,   2,           0,   529 ),
	EF_FRAME(    0,    0,   2,           0,   530 ),
	EF_FRAME(    0,    0,   2,           0,   531 ),
	EF_FRAME(    0,    0,   2,           0,   532 ),
	EF_FRAME(    0,    0,   2,           0,   533 ),
	EF_FRAME(    0,    0,   2,           0,   534 ),
	EF_FRAME(    0,    0,   2,           0,   535 ),
	EF_FRAME(    0,    0,   2,           0,   536 ),
	EF_FRAME(    0,    0,   2,           0,   527 ),
	EF_FRAME(    0,    0,   2,           0,   528 ),
	EF_FRAME(    0,    0,   2,           0,   529 ),
	EF_FRAME(    0,    0,   2,           0,   530 ),
	EF_FRAME(    0,    0,   2,           0,   531 ),
	EF_FRAME(    0,    0,   2,           0,   532 ),
	EF_FRAME(    0,    0,   2,           0,   533 ),
	EF_FRAME(    0,    0,   2,           0,   534 ),
	EF_FRAME(    0,    0,   2,           0,   535 ),
	EF_FRAME(    0,    0,   2,           0,   536 ),
};

const Sint32 pl1d_grp27_07_loop[] = {
	EF_FRAME(    0,    0,   1,           0,   527 ),
	EF_FRAME(    0,    0,   1,           0,   528 ),
	EF_FRAME(    0,    0,   1,           0,   529 ),
	EF_FRAME(    0,    0,   1,           0,   530 ),
	EF_FRAME(    0,    0,   1,           0,   531 ),
	EF_FRAME(    0,    0,   1,           0,   532 ),
	EF_FRAME(    0,    0,   1,           0,   533 ),
	EF_FRAME(    0,    0,   1,           0,   534 ),
	EF_FRAME(    0,    0,   1,           0,   535 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   536 ),
	CG_NEXT( pl1d_grp27_07_loop ),
};

/* group 27 8 */
const Sint32 pl1d_grp27_08[] = {
	EF_FRAME(    0,    0,  80,           0,   957 ),
	EF_FRAME(    0,    0,   2,           0,   958 ),
	EF_FRAME(    0,    0,   2,           0,   959 ),
	EF_FRAME(    0,    0,   2,           0,   960 ),
	EF_FRAME(    0,    0,   2,           0,   961 ),
	EF_FRAME(    0,    0,   2,           0,   962 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   963 ),
	CG_NEXT( pl1d_grp27_08 ),
};

/* group 27 9 */
const Sint32 pl1d_grp27_09[] = {
	EF_FRAME(    0,    0,  -1, END_OF_DATA,   964 ),
	CG_NEXT( pl1d_grp27_09 ),
};

/* group 27 11 */
const Sint32 pl1d_grp27_11[] = {
	EF_FRAME(    0,    0,   3,           0,   965 ),
	EF_FRAME(    0,    0,   3,           0,   966 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   967 ),
	CG_NEXT( pl1d_grp27_11 ),
};

/* group 27 10 */
const Sint32 pl1d_grp27_10[] = {
	EF_FRAME(    0,    0,   3,           0, 32791 ),
	EF_FRAME(    0,    0,   3,           0, 32792 ),
	EF_FRAME(    0,    0,   3,           0, 32793 ),
	EF_FRAME(    0,    0,   3,           0, 32794 ),
	EF_FRAME(    0,    0,   3,           0, 32795 ),
	EF_FRAME(    0,    0,   3,           0, 32796 ),
	EF_FRAME(    0,    0,   3,           0, 32797 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA, 32798 ),
	CG_NEXT( pl1d_grp27_10 ),
};

/* group 27 12 */
const Sint32 pl1d_grp27_12[] = {
	EF_FRAME(    0,    0,   3,           0,   980 ),
	EF_FRAME(    0,    0,   3,           0,   981 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   982 ),
	CG_NEXT( pl1d_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl1d_grp27_13[] = {
	EF_FRAME(    0,    0,  32, END_OF_DATA,   997 ),
	CG_NEXT( pl1d_grp27_13 ),
};

/* group 27 14 */
const Sint32 pl1d_grp27_14[] = {
	EF_FRAME(    0,    0,  32, END_OF_DATA,   999 ),
	CG_NEXT( pl1d_grp27_14 ),
};

/* group 27 15 */
const Sint32 pl1d_grp27_15[] = {
	EF_FRAME(    0,    0,  32, END_OF_DATA,   998 ),
	CG_NEXT( pl1d_grp27_15 ),
};

/* group 27 16 */
const Sint32 pl1d_grp27_16[] = {
	EF_FRAME(    0,    0,   6,           0, 32787 ),
	EF_FRAME(    0,    0,   6,           0, 32788 ),
	EF_FRAME(    0,    0,   6,           0, 32789 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA, 32790 ),
	CG_NEXT( pl1d_grp27_16 ),
};

/* group 27 17 */
const Sint32 pl1d_grp27_17[] = {
	EF_FRAME(    0,    0,  -1, END_OF_DATA,   575 ),
	CG_NEXT( pl1d_grp27_17 ),
};

/* group 27 34 */
const Sint32 pl1d_grp27_34[] = {
	EF_FRAME(    0,    0,   3,           0,   761 ),
	EF_FRAME(    0,    0,   3,           0,   762 ),
	EF_FRAME(    0,    0,   3,           0,   763 ),
	EF_FRAME(    0,    0,   3,           0,   764 ),
	EF_FRAME(    0,    0,   3,           0,   765 ),
	EF_FRAME(    0,    0,   3,           0,   766 ),
	EF_FRAME(    0,    0,   3,           0,   767 ),
};

const Sint32 pl1d_grp27_34_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   768 ),
	CG_NEXT( pl1d_grp27_34_loop ),
};

/* group 27 18 */
const Sint32 pl1d_grp27_18[] = {
	EF_FRAME(    0,    0,   2,           0,   585 ),
	EF_FRAME(    0,    0,   2,           0,   586 ),
	EF_FRAME(    0,    0,   2,           0,   587 ),
	EF_FRAME(    0,    0,   2,           0,   588 ),
	EF_FRAME(    0,    0,   2,           0,   589 ),
	EF_FRAME(    0,    0,   2,           0,   590 ),
	EF_FRAME(    0,    0,   2,           0,   591 ),
	EF_FRAME(    0,    0,   2,           0,   592 ),
	EF_FRAME(    0,    0,   2,           0,   593 ),
	EF_FRAME(    0,    0,   2,           0,   594 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   595 ),
	CG_NEXT( pl1d_grp27_18 ),
};

/* group 27 19 */
const Sint32 pl1d_grp27_19[] = {
	EF_FRAME(    0,    0,   2,           0,   596 ),
	EF_FRAME(    0,    0,   2,           0,   597 ),
	EF_FRAME(    0,    0,   2,           0,   598 ),
	EF_FRAME(    0,    0,   2,           0,   599 ),
	EF_FRAME(    0,    0,   2,           0,   600 ),
	EF_FRAME(    0,    0,   2,           0,   601 ),
	EF_FRAME(    0,    0,   2,           0,   602 ),
	EF_FRAME(    0,    0,   2,           0,   603 ),
	EF_FRAME(    0,    0,   2,           0,   604 ),
	EF_FRAME(    0,    0,   2,           0,   605 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   606 ),
	CG_NEXT( pl1d_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl1d_grp27_20[] = {
	EF_FRAME(    0,    0,   2,           0,   607 ),
	EF_FRAME(    0,    0,   2,           0,   608 ),
	EF_FRAME(    0,    0,   2,           0,   609 ),
	EF_FRAME(    0,    0,   2,           0,   610 ),
	EF_FRAME(    0,    0,   2,           0,   611 ),
	EF_FRAME(    0,    0,   2,           0,   612 ),
	EF_FRAME(    0,    0,   2,           0,   613 ),
	EF_FRAME(    0,    0,   2,           0,   614 ),
	EF_FRAME(    0,    0,   2,           0,   615 ),
	EF_FRAME(    0,    0,   2,           0,   616 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   617 ),
	CG_NEXT( pl1d_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl1d_grp27_21[] = {
	EF_FRAME(    0,    0,   2,           0,   618 ),
	EF_FRAME(    0,    0,   2,           0,   619 ),
	EF_FRAME(    0,    0,   2,           0,   620 ),
	EF_FRAME(    0,    0,   2,           0,   621 ),
	EF_FRAME(    0,    0,   2,           0,   622 ),
	EF_FRAME(    0,    0,   2,           0,   623 ),
	EF_FRAME(    0,    0,   2,           0,   624 ),
	EF_FRAME(    0,    0,   2,           0,   625 ),
	EF_FRAME(    0,    0,   2,           0,   626 ),
	EF_FRAME(    0,    0,   2,           0,   627 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   628 ),
	CG_NEXT( pl1d_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl1d_grp27_22[] = {
	EF_FRAME(    0,    0,   2,           0,   629 ),
	EF_FRAME(    0,    0,   2,           0,   630 ),
	EF_FRAME(    0,    0,   2,           0,   631 ),
	EF_FRAME(    0,    0,   2,           0,   632 ),
	EF_FRAME(    0,    0,   2,           0,   633 ),
	EF_FRAME(    0,    0,   2,           0,   634 ),
	EF_FRAME(    0,    0,   2,           0,   635 ),
	EF_FRAME(    0,    0,   2,           0,   636 ),
	EF_FRAME(    0,    0,   2,           0,   637 ),
	EF_FRAME(    0,    0,   2,           0,   638 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   639 ),
	CG_NEXT( pl1d_grp27_22 ),
};

/* group 27 23 */
const Sint32 pl1d_grp27_23[] = {
	EF_FRAME(    0,    0,   2,           0,   640 ),
	EF_FRAME(    0,    0,   2,           0,   641 ),
	EF_FRAME(    0,    0,   2,           0,   642 ),
	EF_FRAME(    0,    0,   2,           0,   643 ),
	EF_FRAME(    0,    0,   2,           0,   644 ),
	EF_FRAME(    0,    0,   2,           0,   645 ),
	EF_FRAME(    0,    0,   2,           0,   646 ),
	EF_FRAME(    0,    0,   2,           0,   647 ),
	EF_FRAME(    0,    0,   2,           0,   648 ),
	EF_FRAME(    0,    0,   2,           0,   649 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   650 ),
	CG_NEXT( pl1d_grp27_23 ),
};

/* group 27 24 */
const Sint32 pl1d_grp27_24[] = {
	EF_FRAME(    0,    0,   2,           0,   651 ),
	EF_FRAME(    0,    0,   2,           0,   652 ),
	EF_FRAME(    0,    0,   2,           0,   653 ),
	EF_FRAME(    0,    0,   2,           0,   654 ),
	EF_FRAME(    0,    0,   2,           0,   655 ),
	EF_FRAME(    0,    0,   2,           0,   656 ),
	EF_FRAME(    0,    0,   2,           0,   657 ),
	EF_FRAME(    0,    0,   2,           0,   658 ),
	EF_FRAME(    0,    0,   2,           0,   659 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   661 ),
	CG_NEXT( pl1d_grp27_24 ),
};

/* group 27 25 */
const Sint32 pl1d_grp27_25[] = {
	EF_FRAME(    0,    0,   2,           0,   662 ),
	EF_FRAME(    0,    0,   2,           0,   663 ),
	EF_FRAME(    0,    0,   2,           0,   664 ),
	EF_FRAME(    0,    0,   2,           0,   665 ),
	EF_FRAME(    0,    0,   2,           0,   666 ),
	EF_FRAME(    0,    0,   2,           0,   667 ),
	EF_FRAME(    0,    0,   2,           0,   668 ),
	EF_FRAME(    0,    0,   2,           0,   669 ),
	EF_FRAME(    0,    0,   2,           0,   670 ),
	EF_FRAME(    0,    0,   2,           0,   671 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   672 ),
	CG_NEXT( pl1d_grp27_25 ),
};

/* group 27 26 */
const Sint32 pl1d_grp27_26[] = {
	EF_FRAME(    0,    0,   2,           0,   673 ),
	EF_FRAME(    0,    0,   2,           0,   674 ),
	EF_FRAME(    0,    0,   2,           0,   675 ),
	EF_FRAME(    0,    0,   2,           0,   676 ),
	EF_FRAME(    0,    0,   2,           0,   677 ),
	EF_FRAME(    0,    0,   2,           0,   678 ),
	EF_FRAME(    0,    0,   2,           0,   679 ),
	EF_FRAME(    0,    0,   2,           0,   680 ),
	EF_FRAME(    0,    0,   2,           0,   681 ),
	EF_FRAME(    0,    0,   2,           0,   682 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   683 ),
	CG_NEXT( pl1d_grp27_26 ),
};

/* group 27 27 */
const Sint32 pl1d_grp27_27[] = {
	EF_FRAME(    0,    0,   2,           0,   684 ),
	EF_FRAME(    0,    0,   2,           0,   685 ),
	EF_FRAME(    0,    0,   2,           0,   686 ),
	EF_FRAME(    0,    0,   2,           0,   687 ),
	EF_FRAME(    0,    0,   2,           0,   688 ),
	EF_FRAME(    0,    0,   2,           0,   689 ),
	EF_FRAME(    0,    0,   2,           0,   690 ),
	EF_FRAME(    0,    0,   2,           0,   691 ),
	EF_FRAME(    0,    0,   2,           0,   692 ),
	EF_FRAME(    0,    0,   2,           0,   693 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   694 ),
	CG_NEXT( pl1d_grp27_27 ),
};

/* group 27 28 */
const Sint32 pl1d_grp27_28[] = {
	EF_FRAME(    0,    0,   2,           0,   695 ),
	EF_FRAME(    0,    0,   2,           0,   696 ),
	EF_FRAME(    0,    0,   2,           0,   697 ),
	EF_FRAME(    0,    0,   2,           0,   698 ),
	EF_FRAME(    0,    0,   2,           0,   699 ),
	EF_FRAME(    0,    0,   2,           0,   700 ),
	EF_FRAME(    0,    0,   2,           0,   701 ),
	EF_FRAME(    0,    0,   2,           0,   702 ),
	EF_FRAME(    0,    0,   2,           0,   703 ),
	EF_FRAME(    0,    0,   2,           0,   704 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   705 ),
	CG_NEXT( pl1d_grp27_28 ),
};

/* group 27 29 */
const Sint32 pl1d_grp27_29[] = {
	EF_FRAME(    0,    0,   2,           0,   706 ),
	EF_FRAME(    0,    0,   2,           0,   707 ),
	EF_FRAME(    0,    0,   2,           0,   708 ),
	EF_FRAME(    0,    0,   2,           0,   709 ),
	EF_FRAME(    0,    0,   2,           0,   710 ),
	EF_FRAME(    0,    0,   2,           0,   711 ),
	EF_FRAME(    0,    0,   2,           0,   712 ),
	EF_FRAME(    0,    0,   2,           0,   713 ),
	EF_FRAME(    0,    0,   2,           0,   714 ),
	EF_FRAME(    0,    0,   2,           0,   715 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   716 ),
	CG_NEXT( pl1d_grp27_29 ),
};

/* group 27 30 */
const Sint32 pl1d_grp27_30[] = {
	EF_FRAME(    0,    0,   2,           0,   717 ),
	EF_FRAME(    0,    0,   2,           0,   718 ),
	EF_FRAME(    0,    0,   2,           0,   719 ),
	EF_FRAME(    0,    0,   2,           0,   720 ),
	EF_FRAME(    0,    0,   2,           0,   721 ),
	EF_FRAME(    0,    0,   2,           0,   722 ),
	EF_FRAME(    0,    0,   2,           0,   723 ),
	EF_FRAME(    0,    0,   2,           0,   724 ),
	EF_FRAME(    0,    0,   2,           0,   725 ),
	EF_FRAME(    0,    0,   2,           0,   726 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   727 ),
	CG_NEXT( pl1d_grp27_30 ),
};

/* group 27 31 */
const Sint32 pl1d_grp27_31[] = {
	EF_FRAME(    0,    0,   2,           0,   728 ),
	EF_FRAME(    0,    0,   2,           0,   729 ),
	EF_FRAME(    0,    0,   2,           0,   730 ),
	EF_FRAME(    0,    0,   2,           0,   731 ),
	EF_FRAME(    0,    0,   2,           0,   732 ),
	EF_FRAME(    0,    0,   2,           0,   733 ),
	EF_FRAME(    0,    0,   2,           0,   734 ),
	EF_FRAME(    0,    0,   2,           0,   735 ),
	EF_FRAME(    0,    0,   2,           0,   736 ),
	EF_FRAME(    0,    0,   2,           0,   737 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   738 ),
	CG_NEXT( pl1d_grp27_31 ),
};

/* group 27 32 */
const Sint32 pl1d_grp27_32[] = {
	EF_FRAME(    0,    0,   2,           0,   739 ),
	EF_FRAME(    0,    0,   2,           0,   740 ),
	EF_FRAME(    0,    0,   2,           0,   741 ),
	EF_FRAME(    0,    0,   2,           0,   742 ),
	EF_FRAME(    0,    0,   2,           0,   743 ),
	EF_FRAME(    0,    0,   2,           0,   744 ),
	EF_FRAME(    0,    0,   2,           0,   745 ),
	EF_FRAME(    0,    0,   2,           0,   746 ),
	EF_FRAME(    0,    0,   2,           0,   747 ),
	EF_FRAME(    0,    0,   2,           0,   748 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   749 ),
	CG_NEXT( pl1d_grp27_32 ),
};

/* group 27 33 */
const Sint32 pl1d_grp27_33[] = {
	EF_FRAME(    0,    0,   2,           0,   750 ),
	EF_FRAME(    0,    0,   2,           0,   751 ),
	EF_FRAME(    0,    0,   2,           0,   752 ),
	EF_FRAME(    0,    0,   2,           0,   753 ),
	EF_FRAME(    0,    0,   2,           0,   754 ),
	EF_FRAME(    0,    0,   2,           0,   755 ),
	EF_FRAME(    0,    0,   2,           0,   756 ),
	EF_FRAME(    0,    0,   2,           0,   757 ),
	EF_FRAME(    0,    0,   2,           0,   758 ),
	EF_FRAME(    0,    0,   2,           0,   759 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   760 ),
	CG_NEXT( pl1d_grp27_33 ),
};

/* group 27 35 */
const Sint32 pl1d_grp27_35[] = {
	EF_FRAME(    0,    0,   6,           0,   926 ),
	EF_FRAME(    0,    0,   6,           0,   927 ),
	EF_FRAME(    0,    0,   6,           0,   928 ),
	EF_FRAME(    0,    0,   6,           0,   929 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   930 ),
	CG_NEXT( pl1d_grp27_35 ),
};

/* group 27 36 */
const Sint32 pl1d_grp27_36[] = {
	EF_FRAME(    0,    0,   2,           0,   849 ),
	EF_FRAME(    0,    0,   2,           0,   850 ),
	EF_FRAME(    0,    0,   2,           0,   851 ),
	EF_FRAME(    0,    0,   2,           0,   852 ),
	EF_FRAME(    0,    0,   2,           0,   853 ),
	EF_FRAME(    0,    0,   2,           0,   854 ),
	EF_FRAME(    0,    0,   2,           0,   855 ),
	EF_FRAME(    0,    0,   2,           0,   856 ),
	EF_FRAME(    0,    0,   2,           0,   857 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   858 ),
	CG_NEXT( pl1d_grp27_36 ),
};

/* group 27 37 */
const Sint32 pl1d_grp27_37[] = {
	EF_FRAME(    0,    0,   2,           0,   859 ),
	EF_FRAME(    0,    0,   2,           0,   860 ),
	EF_FRAME(    0,    0,   2,           0,   861 ),
	EF_FRAME(    0,    0,   2,           0,   862 ),
	EF_FRAME(    0,    0,   2,           0,   863 ),
	EF_FRAME(    0,    0,   2,           0,   864 ),
	EF_FRAME(    0,    0,   2,           0,   865 ),
	EF_FRAME(    0,    0,   2,           0,   866 ),
	EF_FRAME(    0,    0,   2,           0,   867 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   868 ),
	CG_NEXT( pl1d_grp27_37 ),
};

/* group 27 38 */
const Sint32 pl1d_grp27_38[] = {
	EF_FRAME(    0,    0,   6,           0,   843 ),
	EF_FRAME(    0,    0,   6,           0,   844 ),
	EF_FRAME(    0,    0,   6,           0,   845 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   846 ),
	CG_NEXT( pl1d_grp27_38 ),
};

/* group 27 39 */
const Sint32 pl1d_grp27_39[] = {
	EF_FRAME(    0,    0,   8,           0,   563 ),
	EF_FRAME(    0,    0,   8,           0,   564 ),
	EF_FRAME(    0,    0,   8, END_OF_DATA,   565 ),
	CG_NEXT( pl1d_grp27_39 ),
};

/* group 27 40 */
const Sint32 pl1d_grp27_40[] = {
	EF_FRAME(    0,    0,  10,           0,   555 ),
	EF_FRAME(    0,    0,  10,           0,   556 ),
	EF_FRAME(    0,    0,  10,           0,   557 ),
	EF_FRAME(    0,    0,  10, END_OF_DATA,   556 ),
	CG_NEXT( pl1d_grp27_40 ),
};

/* group 27 41
   group 27 42 */
const Sint32 pl1d_grp27_41[] = {
	EF_FRAME(    0,    0,   1,           0,   551 ),
	EF_FRAME(    0,    0,   1,           0,   552 ),
	EF_FRAME(    0,    1,   2,           0,   553 ),
	EF_FRAME(    0,    1,   2, END_OF_DATA,   554 ),
	CG_NEXT( pl1d_grp27_41 ),
};
