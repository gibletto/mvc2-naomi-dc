/* Megaman (character 1c): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_1c      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_1c     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl1c.c) says which group
                   and number each move uses.
   pl1c_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl1c_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl1c_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl1c_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_1c
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_1c, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_1c */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl1c_stand_00[], pl1c_stand_02[], pl1c_stand_03[], pl1c_stand_06[],
	pl1c_stand_07[], pl1c_jump_00[], pl1c_jump_00_loop[], pl1c_jump_03[], pl1c_jump_03_loop[],
	pl1c_jump_06[], pl1c_jump_09[], pl1c_jump_12[], pl1c_dash_00[], pl1c_dash_00_loop[],
	pl1c_dash_02[], pl1c_dash_02_loop[], pl1c_dash_01[], pl1c_dash_01_loop[], pl1c_dash_03[],
	pl1c_dash_03_loop[], pl1c_crouch_00[], pl1c_crouch_00_loop[], pl1c_crouch_01[], pl1c_crouch_02[],
	pl1c_crouch_02_loop[], pl1c_turn_00[], pl1c_turn_00_loop[], pl1c_turn_01[], pl1c_turn_01_loop[],
	pl1c_guard_00[], pl1c_guard_01[], pl1c_guard_02[], pl1c_guard_02_loop[], pl1c_guard_03[],
	pl1c_guard_03_loop[], pl1c_guard_04[], pl1c_guard_05[], pl1c_guard_06[], pl1c_guard_06_loop[],
	pl1c_guard_07[], pl1c_guard_07_loop[], pl1c_air_guard_00[], pl1c_air_guard_01[],
	pl1c_unused_00[], pl1c_air_guard_02[], pl1c_air_guard_02_loop[], pl1c_stand_p_00[],
	pl1c_stand_p_01[], pl1c_stand_p_02[], pl1c_special_00[], pl1c_special_09[], pl1c_special_10[],
	pl1c_special_11[], pl1c_special_13[], pl1c_stand_k_00[], pl1c_stand_k_01[], pl1c_stand_k_02[],
	pl1c_crouch_p_00[], pl1c_crouch_p_01[], pl1c_crouch_p_02[], pl1c_special_01[],
	pl1c_crouch_k_00[], pl1c_crouch_k_01[], pl1c_crouch_k_02[], pl1c_jump_p_00[], pl1c_jump_p_01[],
	pl1c_jump_p_02[], pl1c_special_02[], pl1c_special_14[], pl1c_special_15[], pl1c_jump_k_00[],
	pl1c_jump_k_01[], pl1c_jump_k_02[], pl1c_damage_00[], pl1c_damage_01[], pl1c_damage_02[],
	pl1c_damage_03[], pl1c_damage_04[], pl1c_damage_05[], pl1c_damage_06[], pl1c_damage_07[],
	pl1c_damage_08[], pl1c_damage_09[], pl1c_damage_09_loop[], pl1c_damage_10[], pl1c_damage_11[],
	pl1c_damage_12[], pl1c_damage_13[], pl1c_damage_13_loop[], pl1c_damage_16[], pl1c_damage_17[],
	pl1c_damage_18[], pl1c_damage_19[], pl1c_damage_20[], pl1c_damage_21[], pl1c_damage_22[],
	pl1c_damage_23[], pl1c_damage_24[], pl1c_damage_25[], pl1c_damage_26[], pl1c_damage_26_loop[],
	pl1c_damage_27[], pl1c_damage_28[], pl1c_damage_29[], pl1c_damage_30[], pl1c_damage_31[],
	pl1c_dizzy_00[], pl1c_damage_35[], pl1c_catch_00[], pl1c_catch_01[], pl1c_get_up_00[],
	pl1c_get_up_01[], pl1c_get_up_02[], pl1c_get_up_03[], pl1c_appear_00[], pl1c_appear_00_loop[],
	pl1c_appear_01[], pl1c_appear_01_loop[], pl1c_grp27_00[], pl1c_grp27_01[], pl1c_grp27_02[],
	pl1c_extra_02[], pl1c_extra_03[], pl1c_extra_04[], pl1c_extra_05[], pl1c_extra_06[],
	pl1c_special_03[], pl1c_special_03_loop[], pl1c_special_06[], pl1c_special_04[],
	pl1c_special_04_loop[], pl1c_special_07[], pl1c_special_12[], pl1c_special_16[],
	pl1c_special_16_loop[], pl1c_special_17[], pl1c_special_17_loop[], pl1c_super_00[],
	pl1c_super_01[], pl1c_super_02[], pl1c_super_03[], pl1c_super_03_loop[], pl1c_super_04[],
	pl1c_super_04_loop[], pl1c_super_05[], pl1c_super_06[], pl1c_super_06_loop[], pl1c_super_27[],
	pl1c_super_28[], pl1c_super_29[], pl1c_grp27_17[], pl1c_grp27_34[], pl1c_grp27_34_loop[],
	pl1c_grp27_18[], pl1c_grp27_19[], pl1c_grp27_20[], pl1c_grp27_21[], pl1c_grp27_22[],
	pl1c_grp27_23[], pl1c_grp27_24[], pl1c_grp27_25[], pl1c_grp27_26[], pl1c_grp27_27[],
	pl1c_grp27_28[], pl1c_grp27_29[], pl1c_grp27_30[], pl1c_grp27_31[], pl1c_grp27_32[],
	pl1c_grp27_33[], pl1c_effect_11[], pl1c_grp27_43[], pl1c_effect_13[], pl1c_effect_13_loop[],
	pl1c_effect_14[], pl1c_effect_15[], pl1c_effect_15_loop[], pl1c_effect_16[], pl1c_effect_17[],
	pl1c_effect_17_loop[], pl1c_effect_18[], pl1c_extra_00[], pl1c_extra_01[], pl1c_win_00[],
	pl1c_win_00_loop[], pl1c_win_01[], pl1c_win_01_loop[], pl1c_win_06[], pl1c_win_06_loop[],
	pl1c_win_07[], pl1c_win_07_loop[], pl1c_win_08[], pl1c_win_10[], pl1c_win_10_loop[],
	pl1c_win_09[], pl1c_win_09_loop[], pl1c_win_11[], pl1c_win_11_loop[], pl1c_win_12[],
	pl1c_win_12_loop[], pl1c_win_02[], pl1c_change_00[], pl1c_change_00_loop[], pl1c_change_01[],
	pl1c_change_01_loop[], pl1c_change_03[], pl1c_change_03_loop[], pl1c_change_02[],
	pl1c_change_02_loop[], pl1c_change_04[], pl1c_caught_00[], pl1c_caught_01[], pl1c_caught_02[],
	pl1c_caught_03[], pl1c_caught_04[], pl1c_caught_05[], pl1c_caught_06[], pl1c_caught_07[],
	pl1c_caught_08[], pl1c_caught_09[], pl1c_caught_10[], pl1c_caught_11[], pl1c_caught_12[],
	pl1c_caught_13[], pl1c_caught_14[], pl1c_caught_15[], pl1c_caught_16[], pl1c_caught_17[],
	pl1c_caught_18[], pl1c_caught_19[], pl1c_caught_20[], pl1c_caught_21[], pl1c_grp25_00[],
	pl1c_grp25_01[], pl1c_grp25_02[], pl1c_grp25_03[], pl1c_grp25_04[], pl1c_grp25_05[],
	pl1c_grp25_06[], pl1c_grp25_06_loop[], pl1c_grp25_07[], pl1c_grp25_07_loop[], pl1c_grp25_08[],
	pl1c_grp25_09[], pl1c_grp25_09_loop[], pl1c_grp25_10[], pl1c_grp25_10_loop[], pl1c_grp25_11[],
	pl1c_grp25_12[], pl1c_grp25_12_loop[], pl1c_grp25_15[], pl1c_grp25_15_loop[], pl1c_grp25_20[],
	pl1c_grp25_21[], pl1c_grp25_22[], pl1c_grp25_23[], pl1c_grp25_23_loop[], pl1c_grp25_24[],
	pl1c_grp25_24_loop[], pl1c_grp25_25[], pl1c_grp25_25_loop[], pl1c_grp27_08[], pl1c_grp27_09[],
	pl1c_grp27_10[], pl1c_grp27_11[], pl1c_grp27_12[], pl1c_grp27_13[], pl1c_grp27_14[],
	pl1c_grp27_15[], pl1c_grp27_35[], pl1c_grp27_05[], pl1c_effect_19[], pl1c_effect_20[],
	pl1c_effect_21[], pl1c_effect_22[], pl1c_effect_23[], pl1c_effect_24[], pl1c_effect_25[],
	pl1c_effect_04[], pl1c_effect_04_loop[], pl1c_effect_05[], pl1c_effect_05_loop[],
	pl1c_effect_06[], pl1c_effect_07[], pl1c_effect_08[], pl1c_effect_09[], pl1c_effect_10[],
	pl1c_grp27_16[], pl1c_grp27_06[], pl1c_grp27_07[], pl1c_grp27_07_loop[], pl1c_grp27_03[],
	pl1c_effect_00[], pl1c_effect_01[], pl1c_grp27_04[], pl1c_effect_02[], pl1c_effect_03[],
	pl1c_grp25_19[], pl1c_grp25_13[], pl1c_grp25_14[], pl1c_grp25_14_loop[], pl1c_grp25_18[],
	pl1c_grp25_18_loop[], pl1c_grp25_16[], pl1c_grp25_16_loop[], pl1c_grp25_17[], pl1c_super_17[],
	pl1c_super_33[], pl1c_super_33_loop[], pl1c_super_25[], pl1c_super_26[], pl1c_super_26_loop[],
	pl1c_super_18[], pl1c_super_19[], pl1c_super_20[], pl1c_super_21[], pl1c_super_22[],
	pl1c_super_23[], pl1c_super_24[], pl1c_super_30[], pl1c_super_31[], pl1c_super_32[],
	pl1c_super_07[], pl1c_super_34[], pl1c_super_34_loop[], pl1c_super_15[], pl1c_super_16[],
	pl1c_super_16_loop[], pl1c_super_08[], pl1c_super_09[], pl1c_super_11[], pl1c_super_12[],
	pl1c_super_38[], pl1c_super_39[], pl1c_super_13[], pl1c_super_36[], pl1c_super_35[],
	pl1c_super_37[], pl1c_super_14[], pl1c_super_10[], pl1c_super_40[], pl1c_super_40_loop[],
	pl1c_grp25_26[], pl1c_grp25_27[], pl1c_grp25_27_loop[], pl1c_grp25_30[], pl1c_super_41[],
	pl1c_grp27_36[], pl1c_grp27_37[], pl1c_grp27_42[];
extern const Sint32 *const pl1c_stand_tbl[], *const pl1c_jump_tbl[], *const pl1c_dash_tbl[],
	*const pl1c_crouch_tbl[], *const pl1c_turn_tbl[], *const pl1c_guard_tbl[],
	*const pl1c_air_guard_tbl[], *const pl1c_stand_p_tbl[], *const pl1c_stand_k_tbl[],
	*const pl1c_crouch_p_tbl[], *const pl1c_crouch_k_tbl[], *const pl1c_jump_p_tbl[],
	*const pl1c_jump_k_tbl[], *const pl1c_damage_tbl[], *const pl1c_dizzy_tbl[],
	*const pl1c_catch_tbl[], *const pl1c_caught_tbl[], *const pl1c_get_up_tbl[],
	*const pl1c_appear_tbl[], *const pl1c_win_tbl[], *const pl1c_extra_tbl[],
	*const pl1c_special_tbl[], *const pl1c_super_tbl[], *const pl1c_effect_tbl[],
	*const pl1c_change_tbl[], *const pl1c_grp25_tbl[], *const pl1c_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_1c[16] = {
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
const Sint32 *const *const char_tbl_1c[] = {
	pl1c_stand_tbl, pl1c_jump_tbl, pl1c_dash_tbl, pl1c_crouch_tbl, pl1c_turn_tbl, pl1c_guard_tbl,
	pl1c_air_guard_tbl, pl1c_stand_p_tbl, pl1c_stand_k_tbl, pl1c_crouch_p_tbl, pl1c_crouch_k_tbl, pl1c_jump_p_tbl,
	pl1c_jump_k_tbl, pl1c_damage_tbl, pl1c_dizzy_tbl, pl1c_catch_tbl, pl1c_caught_tbl, pl1c_get_up_tbl,
	pl1c_appear_tbl, pl1c_win_tbl, pl1c_extra_tbl, pl1c_special_tbl, pl1c_super_tbl, pl1c_effect_tbl,
	pl1c_change_tbl, pl1c_grp25_tbl, pl1c_grp25_tbl, pl1c_grp27_tbl, 0,
};

const Sint32 *const pl1c_stand_tbl[] = {
	pl1c_stand_00, pl1c_stand_00, pl1c_stand_02, pl1c_stand_03, pl1c_stand_00, pl1c_stand_00,
	pl1c_stand_06, pl1c_stand_07, pl1c_stand_02, pl1c_stand_03, 0,
};

const Sint32 *const pl1c_jump_tbl[] = {
	pl1c_jump_00, pl1c_jump_00, pl1c_jump_00, pl1c_jump_03, pl1c_jump_03, pl1c_jump_03,
	pl1c_jump_06, pl1c_jump_06, pl1c_jump_06, pl1c_jump_09, pl1c_jump_09, pl1c_jump_09,
	pl1c_jump_12, 0,
};

const Sint32 *const pl1c_dash_tbl[] = {
	pl1c_dash_00, pl1c_dash_01, pl1c_dash_02, pl1c_dash_03, 0,
};

const Sint32 *const pl1c_crouch_tbl[] = {
	pl1c_crouch_00, pl1c_crouch_01, pl1c_crouch_02, 0,
};

const Sint32 *const pl1c_turn_tbl[] = {
	pl1c_turn_00, pl1c_turn_01, 0,
};

const Sint32 *const pl1c_guard_tbl[] = {
	pl1c_guard_00, pl1c_guard_01, pl1c_guard_02, pl1c_guard_03, pl1c_guard_04, pl1c_guard_05,
	pl1c_guard_06, pl1c_guard_07, 0,
};

const Sint32 *const pl1c_air_guard_tbl[] = {
	pl1c_air_guard_00, pl1c_air_guard_01, pl1c_air_guard_02, 0,
};

const Sint32 *const pl1c_stand_p_tbl[] = {
	pl1c_stand_p_00, pl1c_stand_p_01, pl1c_stand_p_02, 0,
};

const Sint32 *const pl1c_stand_k_tbl[] = {
	pl1c_stand_k_00, pl1c_stand_k_01, pl1c_stand_k_02, 0,
};

const Sint32 *const pl1c_crouch_p_tbl[] = {
	pl1c_crouch_p_00, pl1c_crouch_p_01, pl1c_crouch_p_02, 0,
};

const Sint32 *const pl1c_crouch_k_tbl[] = {
	pl1c_crouch_k_00, pl1c_crouch_k_01, pl1c_crouch_k_02, 0,
};

const Sint32 *const pl1c_jump_p_tbl[] = {
	pl1c_jump_p_00, pl1c_jump_p_01, pl1c_jump_p_02, pl1c_jump_p_00, pl1c_jump_p_01, pl1c_jump_p_02,
	0,
};

const Sint32 *const pl1c_jump_k_tbl[] = {
	pl1c_jump_k_00, pl1c_jump_k_01, pl1c_jump_k_02, pl1c_jump_k_00, pl1c_jump_k_01, pl1c_jump_k_02,
	0,
};

const Sint32 *const pl1c_damage_tbl[] = {
	pl1c_damage_00, pl1c_damage_01, pl1c_damage_02, pl1c_damage_03, pl1c_damage_04, pl1c_damage_05,
	pl1c_damage_06, pl1c_damage_07, pl1c_damage_08, pl1c_damage_09, pl1c_damage_10, pl1c_damage_11,
	pl1c_damage_12, pl1c_damage_13, pl1c_damage_13, pl1c_damage_13, pl1c_damage_16, pl1c_damage_17,
	pl1c_damage_18, pl1c_damage_19, pl1c_damage_20, pl1c_damage_21, pl1c_damage_22, pl1c_damage_23,
	pl1c_damage_24, pl1c_damage_25, pl1c_damage_26, pl1c_damage_27, pl1c_damage_28, pl1c_damage_29,
	pl1c_damage_30, pl1c_damage_31, pl1c_damage_30, pl1c_damage_27, pl1c_damage_13, pl1c_damage_35,
	0,
};

const Sint32 *const pl1c_dizzy_tbl[] = {
	pl1c_dizzy_00, 0,
};

const Sint32 *const pl1c_catch_tbl[] = {
	pl1c_catch_00, pl1c_catch_01, 0,
};

const Sint32 *const pl1c_caught_tbl[] = {
	pl1c_caught_00, pl1c_caught_01, pl1c_caught_02, pl1c_caught_03, pl1c_caught_04, pl1c_caught_05,
	pl1c_caught_06, pl1c_caught_07, pl1c_caught_08, pl1c_caught_09, pl1c_caught_10, pl1c_caught_11,
	pl1c_caught_12, pl1c_caught_13, pl1c_caught_14, pl1c_caught_15, pl1c_caught_16, pl1c_caught_17,
	pl1c_caught_18, pl1c_caught_19, pl1c_caught_20, pl1c_caught_21, 0,
};

const Sint32 *const pl1c_get_up_tbl[] = {
	pl1c_get_up_00, pl1c_get_up_01, pl1c_get_up_02, pl1c_get_up_03, 0,
};

const Sint32 *const pl1c_appear_tbl[] = {
	pl1c_appear_00, pl1c_appear_01, 0,
};

const Sint32 *const pl1c_win_tbl[] = {
	pl1c_win_00, pl1c_win_01, pl1c_win_02, pl1c_win_02, pl1c_win_00, pl1c_win_02,
	pl1c_win_06, pl1c_win_07, pl1c_win_08, pl1c_win_09, pl1c_win_10, pl1c_win_11,
	pl1c_win_12, 0,
};

const Sint32 *const pl1c_extra_tbl[] = {
	pl1c_extra_00, pl1c_extra_01, pl1c_extra_02, pl1c_extra_03, pl1c_extra_04, pl1c_extra_05,
	pl1c_extra_06, 0,
};

const Sint32 *const pl1c_special_tbl[] = {
	pl1c_special_00, pl1c_special_01, pl1c_special_02, pl1c_special_03, pl1c_special_04, pl1c_special_04,
	pl1c_special_06, pl1c_special_07, pl1c_special_07, pl1c_special_09, pl1c_special_10, pl1c_special_11,
	pl1c_special_12, pl1c_special_13, pl1c_special_14, pl1c_special_15, pl1c_special_16, pl1c_special_17,
	0,
};

const Sint32 *const pl1c_super_tbl[] = {
	pl1c_super_00, pl1c_super_01, pl1c_super_02, pl1c_super_03, pl1c_super_04, pl1c_super_05,
	pl1c_super_06, pl1c_super_07, pl1c_super_08, pl1c_super_09, pl1c_super_10, pl1c_super_11,
	pl1c_super_12, pl1c_super_13, pl1c_super_14, pl1c_super_15, pl1c_super_16, pl1c_super_17,
	pl1c_super_18, pl1c_super_19, pl1c_super_20, pl1c_super_21, pl1c_super_22, pl1c_super_23,
	pl1c_super_24, pl1c_super_25, pl1c_super_26, pl1c_super_27, pl1c_super_28, pl1c_super_29,
	pl1c_super_30, pl1c_super_31, pl1c_super_32, pl1c_super_33, pl1c_super_34, pl1c_super_35,
	pl1c_super_36, pl1c_super_37, pl1c_super_38, pl1c_super_39, pl1c_super_40, pl1c_super_41,
	pl1c_super_41, 0,
};

const Sint32 *const pl1c_effect_tbl[] = {
	pl1c_effect_00, pl1c_effect_01, pl1c_effect_02, pl1c_effect_03, pl1c_effect_04, pl1c_effect_05,
	pl1c_effect_06, pl1c_effect_07, pl1c_effect_08, pl1c_effect_09, pl1c_effect_10, pl1c_effect_11,
	pl1c_effect_11, pl1c_effect_13, pl1c_effect_14, pl1c_effect_15, pl1c_effect_16, pl1c_effect_17,
	pl1c_effect_18, pl1c_effect_19, pl1c_effect_20, pl1c_effect_21, pl1c_effect_22, pl1c_effect_23,
	pl1c_effect_24, pl1c_effect_25, pl1c_super_41, 0,
};

const Sint32 *const pl1c_change_tbl[] = {
	pl1c_change_00, pl1c_change_01, pl1c_change_02, pl1c_change_03, pl1c_change_04, 0,
};

const Sint32 *const pl1c_grp25_tbl[] = {
	pl1c_grp25_00, pl1c_grp25_01, pl1c_grp25_02, pl1c_grp25_03, pl1c_grp25_04, pl1c_grp25_05,
	pl1c_grp25_06, pl1c_grp25_07, pl1c_grp25_08, pl1c_grp25_09, pl1c_grp25_10, pl1c_grp25_11,
	pl1c_grp25_12, pl1c_grp25_13, pl1c_grp25_14, pl1c_grp25_15, pl1c_grp25_16, pl1c_grp25_17,
	pl1c_grp25_18, pl1c_grp25_19, pl1c_grp25_20, pl1c_grp25_21, pl1c_grp25_22, pl1c_grp25_23,
	pl1c_grp25_24, pl1c_grp25_25, pl1c_grp25_26, pl1c_grp25_27, pl1c_stand_02, pl1c_extra_02,
	pl1c_grp25_30, 0,
};

const Sint32 *const pl1c_grp27_tbl[] = {
	pl1c_grp27_00, pl1c_grp27_01, pl1c_grp27_02, pl1c_grp27_03, pl1c_grp27_04, pl1c_grp27_05,
	pl1c_grp27_06, pl1c_grp27_07, pl1c_grp27_08, pl1c_grp27_09, pl1c_grp27_10, pl1c_grp27_11,
	pl1c_grp27_12, pl1c_grp27_13, pl1c_grp27_14, pl1c_grp27_15, pl1c_grp27_16, pl1c_grp27_17,
	pl1c_grp27_18, pl1c_grp27_19, pl1c_grp27_20, pl1c_grp27_21, pl1c_grp27_22, pl1c_grp27_23,
	pl1c_grp27_24, pl1c_grp27_25, pl1c_grp27_26, pl1c_grp27_27, pl1c_grp27_28, pl1c_grp27_29,
	pl1c_grp27_30, pl1c_grp27_31, pl1c_grp27_32, pl1c_grp27_33, pl1c_grp27_34, pl1c_grp27_35,
	pl1c_grp27_36, pl1c_grp27_37, pl1c_super_41, pl1c_super_41, pl1c_super_41, pl1c_super_41,
	pl1c_grp27_42, pl1c_grp27_43, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: win_turn_init, win_turn_back, win_roll_init, win_roll_2_init, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl1c_stand_00[] = {
	CG_FRAME(    0,    0,  60,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 8
   group 25 28: pls1c_07_t10_init
   Char_Fly 28 */
const Sint32 pl1c_stand_02[] = {
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    93, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 9 */
const Sint32 pl1c_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl1c_stand_06[] = {
	CG_FRAME(    0,    0,   6,           0,   434, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   433, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   433, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl1c_stand_07[] = {
	CG_FRAME(    0,    0,   6,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   332, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   333, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   332, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_stand_07 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl1c_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1c_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_jump_00_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl1c_jump_03[] = {
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_jump_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_jump_03_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl1c_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   121, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  16,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   124, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl1c_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_jump_09 ),
};

/* Char_Jump 12: act_wall_jump */
const Sint32 pl1c_jump_12[] = {
	CG_FRAME(    0,    0,   1,           0,   302, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   302, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_jump_12 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl1c_dash_00[] = {
	CG_FRAME(    0,    0,   1,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,  16,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl1c_dash_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl1c_dash_00_loop ),
};

/* Char_Dash 2: dash_jump */
const Sint32 pl1c_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_dash_02_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl1c_dash_01[] = {
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl1c_dash_01_loop[] = {
	CG_FRAME(    0,    1,  16, END_OF_DATA,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl1c_dash_01_loop ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl1c_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   8,           0,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_dash_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_dash_03_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl1c_crouch_00[] = {
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1c_crouch_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_crouch_00_loop ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl1c_crouch_01[] = {
	CG_FRAME(    0,    0,  60,           0,    81, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    82, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl1c_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_crouch_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_crouch_02_loop ),
};

/* Char_Turn 0: win_turn_wait, act_turn */
const Sint32 pl1c_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_turn_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_turn_00_loop ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl1c_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1c_turn_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_turn_01_loop ),
};

/* Char_Guard 0 */
const Sint32 pl1c_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl1c_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   137, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl1c_guard_02[] = {
	CG_FRAME(    0,    0,   3,           0,   138, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_guard_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_guard_02_loop ),
};

/* Char_Guard 3 */
const Sint32 pl1c_guard_03[] = {
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_guard_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   137, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_guard_03_loop ),
};

/* Char_Guard 4 */
const Sint32 pl1c_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl1c_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl1c_guard_06[] = {
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1c_guard_06_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_guard_06_loop ),
};

/* Char_Guard 7 */
const Sint32 pl1c_guard_07[] = {
	CG_FRAME(    0,    0,   3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1c_guard_07_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_guard_07_loop ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl1c_air_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl1c_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_air_guard_01 ),
};

const Sint32 pl1c_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	0x00001098,
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl1c_air_guard_02[] = {
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1c_air_guard_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_air_guard_02_loop ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl1c_stand_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   329, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   330, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4,           0,   330, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   331, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl1c_stand_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   332, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   333, 0x90,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    1,   1,           0,   334, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   335, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,  12,           0,   336, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   338, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl1c_stand_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   118, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   340, 0x90,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   341, 0x90,   0x53, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  18,           0,   342, 0x90,   0x53, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x90,   0x53, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x90,   0x53, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   345, 0x90,   0x53, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   346, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_stand_p_02 ),
};

/* Char_Special 0: buster_stand_init (charged Mega Buster) */
const Sint32 pl1c_special_00[] = {
	CG_FRAME(    0,    0,   1,           0,   118, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   340, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   341, 0x90,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  16,           0,   342, 0x90,   0x53,              ATK,  0x00,       0x18,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   8,           0,   342, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   345, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   346, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_00 ),
};

/* Char_Special 9: item_ball_init (Weapon Activation) */
const Sint32 pl1c_special_09[] = {
	CG_FRAME(    0,    0,   2,           0,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x90,   0x00,              ATE,  0x00,       0x1b,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  16,           0,   342, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   344, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   345, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   346, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_09 ),
};

/* Char_Special 10: item_tornado_init (Weapon Activation) */
const Sint32 pl1c_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   118, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x90,   0x00,              ATK,  0x00,       0x19,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   341, 0x90,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  16,           0,   342, 0x90,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,  18,           0,   342, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10,           0,   345, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   346, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_10 ),
};

/* Char_Special 11: item_shield_init (Weapon Activation) */
const Sint32 pl1c_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x90,   0x00,              ATE,  0x00,       0x1c,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10,           0,   341, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  24,           0,   342, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6,           0,   343, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6,           0,   344, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   345, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   346, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_11 ),
};

/* Char_Special 13: item_throw_init (Weapon Activation) */
const Sint32 pl1c_special_13[] = {
	CG_FRAME(    0,    0,   2,           0,   118, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x90,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  16,           0,   342, 0x90,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x90,   0x53,              ATE,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   345, 0x90,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   346, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_13 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl1c_stand_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   347, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   348, 0x90,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   348, 0x90,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   349, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   350, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl1c_stand_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   118, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   351, 0x90,   0x0b,       ATK | 0x10,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x90,   0x07,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   353, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,   -1,   2,           0,   354, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  10,           0,   355, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   356, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl1c_stand_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   357, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x90,   0x0f,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   362, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME( 0x80,    0,   1,           0,   363, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   6,           0,   364, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl1c_crouch_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   368, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   368, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl1c_crouch_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   371, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   372, 0x90,   0x0d,              ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    1,   1,           0,   373, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   2,           0,   374, 0x90,   0x15,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,  12,           0,   375, 0x90,   0x15,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x15,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x90,   0x15,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   129, 0x90,   0x0f,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl1c_crouch_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   378, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   379, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,       0x04,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    1,  18,           0,   380, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   382, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0,   384, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x90,   0x0e, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_crouch_p_02 ),
};

/* Char_Special 1: buster_crouch_init (charged Mega Buster) */
const Sint32 pl1c_special_01[] = {
	CG_FRAME(    0,    0,   3,           0,   378, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   379, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    1,  16,           0,   380, 0x90,   0x00,              ATK,  0x00,       0x18,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0,   380, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   382, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_special_01 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl1c_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   385, 0x90,   0x29,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x90,   0x01,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4,           0,   386, 0x90,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0x90,   0x02,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   6,           0,   388, 0x90,   0x2b,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl1c_crouch_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   130, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   390, 0x90,   0x2d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   391, 0x90,   0x68,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   6,           0,   392, 0x90,   0x68,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x90,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   6,           0,   394, 0x90,   0x2e,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl1c_crouch_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   130, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   395, 0x90,   0x0e,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   1,           0,   396, 0x90,   0x0e,              ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  16,           0,   397, 0x90,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   8,           0,   398, 0x90,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   6,           0,   399, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1c_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl1c_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   401, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  16,           0,   402, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   403, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl1c_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl1c_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   404, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   405, 0x90,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   2,           0,   406, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,  10,           0,   407, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   408, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   409, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1c_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl1c_jump_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   9,           0,   411, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    1,   8,           0,   412, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  16,           0,   414, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   415, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1c_jump_p_02 ),
};

/* Char_Special 2: buster_air_init (charged Mega Buster) */
const Sint32 pl1c_special_02[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    1,   8,           0,   412, 0x90,   0x20,              ATK,  0x00,       0x18,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,  24,           0,   414, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   415, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1c_special_02 ),
};

/* Char_Special 14: item_air_throw_init (Weapon Activation) */
const Sint32 pl1c_special_14[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    1,   8,           0,   412, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x90,   0x20,              ATK,  0x00,       0x03,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,  24,           0,   414, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   415, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1c_special_14 ),
};

/* Char_Special 15: snapback_init (Snap Back) */
const Sint32 pl1c_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   358, 0x90,   0x0f,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   359, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   3,           0,   360, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x90,   0x0d,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   362, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME( 0x80,    0,   1,           0,   363, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   6,           0,   364, 0x90,   0x0c,              ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x90,   0x0d,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0x90,   0x0f,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_15 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl1c_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   416, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  16,           0,   417, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   2,           0,   418, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   419, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1c_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl1c_jump_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   420, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   421, 0x90,   0x20,       ATK | 0x1b,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   422, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,  12,           0,   423, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   424, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   124, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1c_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl1c_jump_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   426, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   426, 0x90,   0x20,       ATK | 0x10,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   427, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,  10,           0,   428, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   430, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl1c_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl1c_damage_00[] = {
	CG_FRAME(    0,    0,   9,           0,   472, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   471, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl1c_damage_01[] = {
	CG_FRAME(    0,    0,   1,           0,   472, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   9,           0,   473, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   472, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   471, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl1c_damage_02[] = {
	CG_FRAME(    0,    0,   1,           0,   473, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   474, 0x80,   0x17,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   473, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   471, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl1c_damage_03[] = {
	CG_FRAME(    0,    0,   9,           0,   475, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl1c_damage_04[] = {
	CG_FRAME(    0,    0,   2,           0,   475, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   476, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl1c_damage_05[] = {
	CG_FRAME(    0,    0,   1,           0,   476, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   477, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   476, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl1c_damage_06[] = {
	CG_FRAME(    0,    0,   9,           0,   479, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   478, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl1c_damage_07[] = {
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0,   480, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   478, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl1c_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  12,           0,   481, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   480, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   478, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1c_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl1c_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   488, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   497, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   498, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   497, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   499, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   500, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   501, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   503, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   504, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   505, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1c_damage_09_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   506, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1c_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl1c_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   482, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl1c_damage_11[] = {
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   483, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   482, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl1c_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   482, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl1c_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   486, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1c_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   487, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1c_damage_13_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl1c_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   507, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl1c_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   493, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl1c_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   494, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl1c_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   495, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl1c_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   496, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl1c_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl1c_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl1c_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   494, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl1c_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   495, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl1c_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   496, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl1c_damage_26[] = {
	CG_FRAME(    0,    0,   1,           0,   497, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   497, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   499, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   500, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   501, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   502, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   503, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   504, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   505, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1c_damage_26_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   506, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1c_damage_26_loop ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl1c_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x2f,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl1c_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   508, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   513, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl1c_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   514, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   515, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   516, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   517, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   518, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   519, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl1c_damage_30[] = {
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   124, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl1c_damage_31[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   506, 0x40,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1c_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl1c_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   471, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   472, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   475, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_dizzy_00 ),
};

/* Char_Damage 35 */
const Sint32 pl1c_damage_35[] = {
	CG_FRAME(    0,    0,   3,           0,   483, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   482, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_damage_35 ),
};

/* Char_Catch 0: nml_set */
const Sint32 pl1c_catch_00[] = {
	CG_FRAME(    0,    0,   6,           0,   433, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   6,           0,   434, 0x80,   0x0e,                0,  0x00,       0x04,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   3,           0,   435, 0x80,   0x2b,                0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   6,           0,   436, 0x80,   0x2b,                0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   3,           0,   437, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    0,   6,           0,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    1,   3,           0,   439, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   440, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_catch_00 ),
};

/* Char_Catch 1: jump_set */
const Sint32 pl1c_catch_01[] = {
	CG_FRAME(    0,    0,   6,           0,   434, 0x80,   0x00,                0,  0x00,       0x04,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,   6,           0,   435, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,   6,           0,   436, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   6,           0,   437, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    0,   6,           0,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    1,   6,           0,   439, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   440, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_catch_01 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl1c_get_up_00[] = {
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl1c_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x2e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   223, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   225, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   226, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   227, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl1c_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x2e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x2e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x2e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl1c_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   234, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   237, 0x80,   0x2f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl1c_appear_00[] = {
	CG_FRAME(    0,    0,   1,           0,   216, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_appear_00_loop[] = {
	CG_FRAME(    0,    1,   1,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   1, END_OF_DATA,    -1, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_appear_00_loop ),
};

/* Char_Appear 1: pls1c_07_t11_init, appear_land */
const Sint32 pl1c_appear_01[] = {
	CG_FRAME(    0,    0,   1,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x0e,                0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_appear_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_appear_01_loop ),
};

/* group 27 0 */
const Sint32 pl1c_grp27_00[] = {
	EF_FRAME(    0,    0,   3,           0,   210 ),
	EF_FRAME(    0,    0,   3,           0,   211 ),
	EF_FRAME(    0,    0,   3,           0,   212 ),
	EF_FRAME(    0,    0,   3,           0,   213 ),
	EF_FRAME(    0,    0,   3,           0,   214 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   215 ),
	CG_NEXT( pl1c_grp27_00 ),
};

/* group 27 1: pls1c_01_init */
const Sint32 pl1c_grp27_01[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   217 ),
	CG_NEXT( pl1c_grp27_01 ),
};

/* group 27 2: pls1c_01_init */
const Sint32 pl1c_grp27_02[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   218 ),
	CG_NEXT( pl1c_grp27_02 ),
};

/* Char_Extra 2: chouhatsu_init (taunt), fly_start5
   group 25 29: pls1c_07_t10_run
   Char_Fly 29 */
const Sint32 pl1c_extra_02[] = {
	CG_FRAME(    0,    0,   5,           0,   456, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   457, 0x80,   0x00,                0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   458, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   459, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   460, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   461, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   462, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   463, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   464, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   465, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_extra_02 ),
};

/* Char_Extra 3: pl1c_upper, fly_start5 */
const Sint32 pl1c_extra_03[] = {
	CG_FRAME(    0,    0,   1,           0,   357, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x90,   0x0f,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   362, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME( 0x80,    0,   1,           0,   363, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   6,           0,   364, 0x90,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x90,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   118, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_extra_03 ),
};

/* Char_Extra 4 */
const Sint32 pl1c_extra_04[] = {
	CG_FRAME(    0,    0,   6,           0,   304, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   305, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   306, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   307, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   308, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl1c_extra_05[] = {
	CG_FRAME(    0,    0,   6,           0,   309, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   310, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   311, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   312, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   313, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_extra_05 ),
};

/* Char_Extra 6 */
const Sint32 pl1c_extra_06[] = {
	CG_FRAME(    0,    0,   6,           0,   314, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   315, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   316, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   317, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   318, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_extra_06 ),
};

/* Char_Special 3: upper_init (Mega Uppercut, 3 + atck_level) */
const Sint32 pl1c_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   4,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   52,    1,  15,           0,   446, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   449, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1c_special_03_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   451, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_03_loop ),
};

/* Char_Special 6: upper_fall (Mega Uppercut, 6 + atck_level), upper_air_fall (Mega Uppercut, 6 + atck_level) */
const Sint32 pl1c_special_06[] = {
	CG_FRAME(    0,    0,   6,           0,   130, 0x80,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_06 ),
};

/* Char_Special 4
   Char_Special 5 */
const Sint32 pl1c_special_04[] = {
	CG_FRAME(    0,    0,   1,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   4,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   56,    1,  29,           0,   446, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   450, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1c_special_04_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   451, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_04_loop ),
};

/* Char_Special 7
   Char_Special 8 */
const Sint32 pl1c_special_07[] = {
	CG_FRAME(    0,    0,   6,           0,   130, 0x80,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_07 ),
};

/* Char_Special 12: item_sel_init (Summon power-up: Rock Ball) */
const Sint32 pl1c_special_12[] = {
	CG_FRAME(    0,    0,   1,           0,   248, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   248, 0x90,   0x00,              ATE,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x90,   0x0f,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA,   251, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_12 ),
};

/* Char_Special 16: upper_air_init (Mega Uppercut, 16 + atck_level) */
const Sint32 pl1c_special_16[] = {
	CG_FRAME(    0,    0,   1,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,   444, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   52,    1,  15,           0,   446, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   449, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   450, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1c_special_16_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   451, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_16_loop ),
};

/* Char_Special 17 */
const Sint32 pl1c_special_17[] = {
	CG_FRAME(    0,    0,   1,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,   444, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   56,    1,  29,           0,   446, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   450, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1c_special_17_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   451, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_special_17_loop ),
};

/* Char_Super 0: hyper_rock_init (Hyper Mega Man (hyper)) */
const Sint32 pl1c_super_00[] = {
	CG_FRAME(    0,    0,   1,           0,   248, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   248, 0x90,   0x00,              ATK,  0x00,       0x17,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   328, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   250, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_00 ),
};

/* Char_Super 1 */
const Sint32 pl1c_super_01[] = {
	CG_FRAME(    0,    0,   1,           0,   251, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   767, 0x90,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0x90,   0x80,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   769, 0x90,   0xa0,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    1,  24,           0, 32780, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32782, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1c_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl1c_super_02[] = {
	CG_FRAME(    0,    0,   2,           0,   251, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   767, 0x90,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0x90,   0x80,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   4,           0,   769, 0x90,   0xa0,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    1,  24,           0, 32780, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32782, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1c_super_02 ),
};

/* Char_Super 3: hyper_rock_main_init (Hyper Mega Man (hyper)) */
const Sint32 pl1c_super_03[] = {
	CG_FRAME(    0,    0,   2,           0, 32783, 0x90,   0xc0,              ATK,  0x00,       0x14,    0,   0,     0,      0,  83 ),
};

const Sint32 pl1c_super_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32784, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0, 32785, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0, 32790, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA, 32783, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1c_super_03_loop ),
};

/* Char_Super 4: hyper_rock_main (Hyper Mega Man (hyper)) */
const Sint32 pl1c_super_04[] = {
	CG_FRAME(    0,    0,   6,           0, 32783, 0x90,   0xc0,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0, 32782, 0x90,   0xc0,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0, 32781, 0x90,   0xc0,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0, 32780, 0x90,   0xc0,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   769, 0x90,   0xa0,              ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   6,           0,   768, 0x90,   0x80,              ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
};

const Sint32 pl1c_super_04_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   767, 0x90,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl1c_super_04_loop ),
};

/* Char_Super 5: hyper_rock_end_init (Hyper Mega Man (hyper)) */
const Sint32 pl1c_super_05[] = {
	CG_FRAME(    0,    0,  22,           0,   251, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_super_05 ),
};

/* Char_Super 6: hyper_rock_end_init (Hyper Mega Man (hyper)) */
const Sint32 pl1c_super_06[] = {
	CG_FRAME(    0,    0,  36,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1c_super_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   126, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_super_06_loop ),
};

/* Char_Super 27 */
const Sint32 pl1c_super_27[] = {
	CG_FRAME(    0,    0,   8,           0,   248, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   328, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   250, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   251, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6,           0,    -1, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   767, 0x90,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0x90,   0x80,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   769, 0x90,   0xa0,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    1,  30,           0, 32780, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,  48, END_OF_DATA, 32781, 0x90,   0xc0,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1c_super_27 ),
};

/* Char_Super 28 */
const Sint32 pl1c_super_28[] = {
	CG_FRAME(    0,    1,   6,           0, 32780, 0x90,   0xc0,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   769, 0x90,   0xa0,              ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   6,           0,   768, 0x90,   0x80,              ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   6,           0,   767, 0x90,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   251, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_super_28 ),
};

/* Char_Super 29: shl1c_05_head_char, shl1c_05_part_char */
const Sint32 pl1c_super_29[] = {
	CG_FRAME(    0,    0,   4,           0, 32839, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32840, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32841, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0, 32842, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0, 32843, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32844, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl1c_super_29 ),
};

/* group 27 17 */
const Sint32 pl1c_grp27_17[] = {
	EF_FRAME(    0,    0,  -1, END_OF_DATA,   756 ),
	CG_NEXT( pl1c_grp27_17 ),
};

/* group 27 34 */
const Sint32 pl1c_grp27_34[] = {
	EF_FRAME(    0,    0,   3,           0,   949 ),
	EF_FRAME(    0,    0,   3,           0,   950 ),
	EF_FRAME(    0,    0,   3,           0,   951 ),
	EF_FRAME(    0,    0,   3,           0,   952 ),
	EF_FRAME(    0,    0,   3,           0,   953 ),
	EF_FRAME(    0,    0,   3,           0,   954 ),
	EF_FRAME(    0,    0,   3,           0,   955 ),
};

const Sint32 pl1c_grp27_34_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   956 ),
	CG_NEXT( pl1c_grp27_34_loop ),
};

/* group 27 18 */
const Sint32 pl1c_grp27_18[] = {
	EF_FRAME(    0,    0,   2,           0,   770 ),
	EF_FRAME(    0,    0,   2,           0,   771 ),
	EF_FRAME(    0,    0,   2,           0,   772 ),
	EF_FRAME(    0,    0,   2,           0,   773 ),
	EF_FRAME(    0,    0,   2,           0,   774 ),
	EF_FRAME(    0,    0,   2,           0,   775 ),
	EF_FRAME(    0,    0,   2,           0,   776 ),
	EF_FRAME(    0,    0,   2,           0,   777 ),
	EF_FRAME(    0,    0,   2,           0,   778 ),
	EF_FRAME(    0,    0,   2,           0,   779 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   780 ),
	CG_NEXT( pl1c_grp27_18 ),
};

/* group 27 19 */
const Sint32 pl1c_grp27_19[] = {
	EF_FRAME(    0,    0,   2,           0,   781 ),
	EF_FRAME(    0,    0,   2,           0,   782 ),
	EF_FRAME(    0,    0,   2,           0,   783 ),
	EF_FRAME(    0,    0,   2,           0,   784 ),
	EF_FRAME(    0,    0,   2,           0,   785 ),
	EF_FRAME(    0,    0,   2,           0,   786 ),
	EF_FRAME(    0,    0,   2,           0,   787 ),
	EF_FRAME(    0,    0,   2,           0,   788 ),
	EF_FRAME(    0,    0,   2,           0,   789 ),
	EF_FRAME(    0,    0,   2,           0,   790 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   791 ),
	CG_NEXT( pl1c_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl1c_grp27_20[] = {
	EF_FRAME(    0,    0,   2,           0,   792 ),
	EF_FRAME(    0,    0,   2,           0,   793 ),
	EF_FRAME(    0,    0,   2,           0,   794 ),
	EF_FRAME(    0,    0,   2,           0,   795 ),
	EF_FRAME(    0,    0,   2,           0,   796 ),
	EF_FRAME(    0,    0,   2,           0,   797 ),
	EF_FRAME(    0,    0,   2,           0,   798 ),
	EF_FRAME(    0,    0,   2,           0,   799 ),
	EF_FRAME(    0,    0,   2,           0,   800 ),
	EF_FRAME(    0,    0,   2,           0,   801 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   802 ),
	CG_NEXT( pl1c_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl1c_grp27_21[] = {
	EF_FRAME(    0,    0,   2,           0,   803 ),
	EF_FRAME(    0,    0,   2,           0,   804 ),
	EF_FRAME(    0,    0,   2,           0,   805 ),
	EF_FRAME(    0,    0,   2,           0,   806 ),
	EF_FRAME(    0,    0,   2,           0,   807 ),
	EF_FRAME(    0,    0,   2,           0,   808 ),
	EF_FRAME(    0,    0,   2,           0,   809 ),
	EF_FRAME(    0,    0,   2,           0,   810 ),
	EF_FRAME(    0,    0,   2,           0,   811 ),
	EF_FRAME(    0,    0,   2,           0,   812 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   813 ),
	CG_NEXT( pl1c_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl1c_grp27_22[] = {
	EF_FRAME(    0,    0,   2,           0,   814 ),
	EF_FRAME(    0,    0,   2,           0,   815 ),
	EF_FRAME(    0,    0,   2,           0,   816 ),
	EF_FRAME(    0,    0,   2,           0,   817 ),
	EF_FRAME(    0,    0,   2,           0,   818 ),
	EF_FRAME(    0,    0,   2,           0,   819 ),
	EF_FRAME(    0,    0,   2,           0,   820 ),
	EF_FRAME(    0,    0,   2,           0,   821 ),
	EF_FRAME(    0,    0,   2,           0,   822 ),
	EF_FRAME(    0,    0,   2,           0,   823 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   824 ),
	CG_NEXT( pl1c_grp27_22 ),
};

/* group 27 23 */
const Sint32 pl1c_grp27_23[] = {
	EF_FRAME(    0,    0,   2,           0,   825 ),
	EF_FRAME(    0,    0,   2,           0,   826 ),
	EF_FRAME(    0,    0,   2,           0,   827 ),
	EF_FRAME(    0,    0,   2,           0,   828 ),
	EF_FRAME(    0,    0,   2,           0,   829 ),
	EF_FRAME(    0,    0,   2,           0,   830 ),
	EF_FRAME(    0,    0,   2,           0,   831 ),
	EF_FRAME(    0,    0,   2,           0,   832 ),
	EF_FRAME(    0,    0,   2,           0,   833 ),
	EF_FRAME(    0,    0,   2,           0,   834 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   835 ),
	CG_NEXT( pl1c_grp27_23 ),
};

/* group 27 24 */
const Sint32 pl1c_grp27_24[] = {
	EF_FRAME(    0,    0,   2,           0,   836 ),
	EF_FRAME(    0,    0,   2,           0,   837 ),
	EF_FRAME(    0,    0,   2,           0,   838 ),
	EF_FRAME(    0,    0,   2,           0,   839 ),
	EF_FRAME(    0,    0,   2,           0,   840 ),
	EF_FRAME(    0,    0,   2,           0,   841 ),
	EF_FRAME(    0,    0,   2,           0,   842 ),
	EF_FRAME(    0,    0,   2,           0,   843 ),
	EF_FRAME(    0,    0,   2,           0,   844 ),
	EF_FRAME(    0,    0,   2,           0,   845 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   846 ),
	CG_NEXT( pl1c_grp27_24 ),
};

/* group 27 25 */
const Sint32 pl1c_grp27_25[] = {
	EF_FRAME(    0,    0,   2,           0,   847 ),
	EF_FRAME(    0,    0,   2,           0,   848 ),
	EF_FRAME(    0,    0,   2,           0,   849 ),
	EF_FRAME(    0,    0,   2,           0,   850 ),
	EF_FRAME(    0,    0,   2,           0,   851 ),
	EF_FRAME(    0,    0,   2,           0,   852 ),
	EF_FRAME(    0,    0,   2,           0,   853 ),
	EF_FRAME(    0,    0,   2,           0,   854 ),
	EF_FRAME(    0,    0,   2,           0,   855 ),
	EF_FRAME(    0,    0,   2,           0,   856 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   857 ),
	CG_NEXT( pl1c_grp27_25 ),
};

/* group 27 26 */
const Sint32 pl1c_grp27_26[] = {
	EF_FRAME(    0,    0,   2,           0,   858 ),
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
	CG_NEXT( pl1c_grp27_26 ),
};

/* group 27 27 */
const Sint32 pl1c_grp27_27[] = {
	EF_FRAME(    0,    0,   2,           0,   869 ),
	EF_FRAME(    0,    0,   2,           0,   870 ),
	EF_FRAME(    0,    0,   2,           0,   871 ),
	EF_FRAME(    0,    0,   2,           0,   872 ),
	EF_FRAME(    0,    0,   2,           0,   873 ),
	EF_FRAME(    0,    0,   2,           0,   874 ),
	EF_FRAME(    0,    0,   2,           0,   875 ),
	EF_FRAME(    0,    0,   2,           0,   876 ),
	EF_FRAME(    0,    0,   2,           0,   877 ),
	EF_FRAME(    0,    0,   2,           0,   878 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   879 ),
	CG_NEXT( pl1c_grp27_27 ),
};

/* group 27 28 */
const Sint32 pl1c_grp27_28[] = {
	EF_FRAME(    0,    0,   2,           0,   880 ),
	EF_FRAME(    0,    0,   2,           0,   881 ),
	EF_FRAME(    0,    0,   2,           0,   882 ),
	EF_FRAME(    0,    0,   2,           0,   883 ),
	EF_FRAME(    0,    0,   2,           0,   884 ),
	EF_FRAME(    0,    0,   2,           0,   885 ),
	EF_FRAME(    0,    0,   2,           0,   886 ),
	EF_FRAME(    0,    0,   2,           0,   887 ),
	EF_FRAME(    0,    0,   2,           0,   888 ),
	EF_FRAME(    0,    0,   2,           0,   889 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   890 ),
	CG_NEXT( pl1c_grp27_28 ),
};

/* group 27 29 */
const Sint32 pl1c_grp27_29[] = {
	EF_FRAME(    0,    0,   2,           0,   891 ),
	EF_FRAME(    0,    0,   2,           0,   892 ),
	EF_FRAME(    0,    0,   2,           0,   893 ),
	EF_FRAME(    0,    0,   2,           0,   894 ),
	EF_FRAME(    0,    0,   2,           0,   895 ),
	EF_FRAME(    0,    0,   2,           0,   896 ),
	EF_FRAME(    0,    0,   2,           0,   897 ),
	EF_FRAME(    0,    0,   2,           0,   898 ),
	EF_FRAME(    0,    0,   2,           0,   899 ),
	EF_FRAME(    0,    0,   2,           0,   900 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   901 ),
	CG_NEXT( pl1c_grp27_29 ),
};

/* group 27 30 */
const Sint32 pl1c_grp27_30[] = {
	EF_FRAME(    0,    0,   2,           0,   902 ),
	EF_FRAME(    0,    0,   2,           0,   903 ),
	EF_FRAME(    0,    0,   2,           0,   904 ),
	EF_FRAME(    0,    0,   2,           0,   905 ),
	EF_FRAME(    0,    0,   2,           0,   906 ),
	EF_FRAME(    0,    0,   2,           0,   907 ),
	EF_FRAME(    0,    0,   2,           0,   908 ),
	EF_FRAME(    0,    0,   2,           0,   909 ),
	EF_FRAME(    0,    0,   2,           0,   910 ),
	EF_FRAME(    0,    0,   2,           0,   911 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   912 ),
	CG_NEXT( pl1c_grp27_30 ),
};

/* group 27 31 */
const Sint32 pl1c_grp27_31[] = {
	EF_FRAME(    0,    0,   2,           0,   913 ),
	EF_FRAME(    0,    0,   2,           0,   914 ),
	EF_FRAME(    0,    0,   2,           0,   915 ),
	EF_FRAME(    0,    0,   2,           0,   916 ),
	EF_FRAME(    0,    0,   2,           0,   917 ),
	EF_FRAME(    0,    0,   2,           0,   918 ),
	EF_FRAME(    0,    0,   2,           0,   919 ),
	EF_FRAME(    0,    0,   2,           0,   920 ),
	EF_FRAME(    0,    0,   2,           0,   921 ),
	EF_FRAME(    0,    0,   2,           0,   922 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   923 ),
	CG_NEXT( pl1c_grp27_31 ),
};

/* group 27 32 */
const Sint32 pl1c_grp27_32[] = {
	EF_FRAME(    0,    0,   2,           0,   924 ),
	EF_FRAME(    0,    0,   2,           0,   925 ),
	EF_FRAME(    0,    0,   2,           0,   926 ),
	EF_FRAME(    0,    0,   2,           0,   927 ),
	EF_FRAME(    0,    0,   2,           0,   928 ),
	EF_FRAME(    0,    0,   2,           0,   929 ),
	EF_FRAME(    0,    0,   2,           0,   930 ),
	EF_FRAME(    0,    0,   2,           0,   931 ),
	EF_FRAME(    0,    0,   2,           0,   932 ),
	EF_FRAME(    0,    0,   2,           0,   933 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   934 ),
	CG_NEXT( pl1c_grp27_32 ),
};

/* group 27 33 */
const Sint32 pl1c_grp27_33[] = {
	EF_FRAME(    0,    0,   2,           0,   935 ),
	EF_FRAME(    0,    0,   2,           0,   936 ),
	EF_FRAME(    0,    0,   2,           0,   937 ),
	EF_FRAME(    0,    0,   2,           0,   938 ),
	EF_FRAME(    0,    0,   2,           0,   939 ),
	EF_FRAME(    0,    0,   2,           0,   940 ),
	EF_FRAME(    0,    0,   2,           0,   941 ),
	EF_FRAME(    0,    0,   2,           0,   942 ),
	EF_FRAME(    0,    0,   2,           0,   943 ),
	EF_FRAME(    0,    0,   2,           0,   944 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   945 ),
	CG_NEXT( pl1c_grp27_33 ),
};

/* Char_Effect 11: shl1c_06_missile_init, shl1c_06_missile_move
   Char_Effect 12 */
const Sint32 pl1c_effect_11[] = {
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    2,    0,   1,           0, 32806, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    4,    0,   1,           0, 32805, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    6,    0,   1,           0, 32804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    8,    0,   1,           0, 32803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   10,    0,   1,           0, 32802, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   12,    0,   1,           0, 32801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   14,    0,   1,           0, 32800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   16,    0,   1,           0, 32799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   18,    0,   1,           0, 32798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   20,    0,   1,           0, 32797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   22,    0,   1,           0, 32796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   24,    0,   1,           0, 32795, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   26,    0,   1,           0, 32794, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   28,    0,   1,           0, 32793, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(   30,    0,   1, END_OF_DATA, 32792, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl1c_effect_11 ),
};

/* group 27 43: shl1c_06_flame_init, shl1c_06_flame_move */
const Sint32 pl1c_grp27_43[] = {
	EF_FRAME(    0,    0,   2,           0, 32807 ),
	EF_FRAME(    0,    0,   2,           0, 32808 ),
	EF_FRAME(    2,    0,   2,           0, 32837 ),
	EF_FRAME(    2,    0,   2,           0, 32838 ),
	EF_FRAME(    4,    0,   2,           0, 32835 ),
	EF_FRAME(    4,    0,   2,           0, 32836 ),
	EF_FRAME(    6,    0,   2,           0, 32833 ),
	EF_FRAME(    6,    0,   2,           0, 32834 ),
	EF_FRAME(    8,    0,   2,           0, 32831 ),
	EF_FRAME(    8,    0,   2,           0, 32832 ),
	EF_FRAME(   10,    0,   2,           0, 32829 ),
	EF_FRAME(   10,    0,   2,           0, 32830 ),
	EF_FRAME(   12,    0,   2,           0, 32827 ),
	EF_FRAME(   12,    0,   2,           0, 32828 ),
	EF_FRAME(   14,    0,   2,           0, 32825 ),
	EF_FRAME(   14,    0,   2,           0, 32826 ),
	EF_FRAME(   16,    0,   2,           0, 32823 ),
	EF_FRAME(   16,    0,   2,           0, 32824 ),
	EF_FRAME(   18,    0,   2,           0, 32821 ),
	EF_FRAME(   18,    0,   2,           0, 32822 ),
	EF_FRAME(   20,    0,   2,           0, 32819 ),
	EF_FRAME(   20,    0,   2,           0, 32820 ),
	EF_FRAME(   22,    0,   2,           0, 32817 ),
	EF_FRAME(   22,    0,   2,           0, 32818 ),
	EF_FRAME(   24,    0,   2,           0, 32815 ),
	EF_FRAME(   24,    0,   2,           0, 32816 ),
	EF_FRAME(   26,    0,   2,           0, 32813 ),
	EF_FRAME(   26,    0,   2,           0, 32814 ),
	EF_FRAME(   28,    0,   2,           0, 32811 ),
	EF_FRAME(   28,    0,   2,           0, 32812 ),
	EF_FRAME(   30,    0,   2,           0, 32809 ),
	EF_FRAME(   30,    0,   2, END_OF_DATA, 32810 ),
	CG_NEXT( pl1c_grp27_43 ),
};

/* Char_Effect 13 */
const Sint32 pl1c_effect_13[] = {
	CG_FRAME(    0,    1,   4,           0,   975, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   972, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   973, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   974, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   969, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl1c_effect_13_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   958, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   959, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   960, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   961, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   962, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   957, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl1c_effect_13_loop ),
};

/* Char_Effect 14 */
const Sint32 pl1c_effect_14[] = {
	CG_FRAME(    0,    0,   3,           0,   963, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   964, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   965, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   966, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   967, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   968, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl1c_effect_15[] = {
	CG_FRAME(    0,    1,   4,           0,  1012, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1010, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,  1011, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,  1008, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,  1009, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,  1010, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,  1011, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
};

const Sint32 pl1c_effect_15_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   990, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   991, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   992, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   993, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   994, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   995, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   996, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   997, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   998, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   999, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,  1000, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1001, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl1c_effect_15_loop ),
};

/* Char_Effect 16 */
const Sint32 pl1c_effect_16[] = {
	CG_FRAME(    0,    0,   3,           0,  1002, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,  1003, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1004, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1005, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1006, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,  1007, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl1c_effect_17[] = {
	CG_FRAME(    0,    1,   4,           0,   989, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   988, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
};

const Sint32 pl1c_effect_17_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   976, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   977, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   978, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   979, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   980, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   981, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl1c_effect_17_loop ),
};

/* Char_Effect 18 */
const Sint32 pl1c_effect_18[] = {
	CG_FRAME(    0,    0,   3,           0,   982, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   983, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   984, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   985, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   986, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   987, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_18 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl1c_extra_00[] = {
	CG_FRAME(    0,    0,   4,           0,   431, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   432, 0x90,   0x20,          ATK | 6,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl1c_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl1c_extra_01[] = {
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   113, 0x80,   0x00,              ATE,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   252, 0x80,   0x11,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x0f,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   256, 0x80,   0x0f,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_extra_01 ),
};

/* Char_Win 0: win_held_init
   Char_Win 4: win_ko_init */
const Sint32 pl1c_win_00[] = {
	CG_FRAME(    0,    0,   5,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   148, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   149, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   150, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   151, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   152, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   153, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   154, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   155, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   156, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   157, 0x80,   0x12,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   159, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   158, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   160, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   161, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   159, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   162, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   163, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   164, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   163, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   164, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   163, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   164, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   163, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   164, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   163, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_win_00_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   164, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_win_00_loop ),
};

/* Char_Win 1: win_turn_wait, win_turn_back, win_turn_back_wait */
const Sint32 pl1c_win_01[] = {
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   169, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   170, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   171, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   172, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   171, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   172, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   173, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   174, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   175, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   176, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   177, 0x80,   0x6a,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   179, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   181, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   182, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   183, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   184, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   185, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   186, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   187, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_win_01_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   188, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   190, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   191, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   192, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   193, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_win_01_loop ),
};

/* Char_Win 6: win_shine_init, win_roll_wait */
const Sint32 pl1c_win_06[] = {
	CG_FRAME(    0,    0,   6,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   251, 0x80,   0x0e,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   252, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   9,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_win_06_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_win_06_loop ),
};

/* Char_Win 7: win_shine_init */
const Sint32 pl1c_win_07[] = {
	CG_FRAME(    0,    0,   6,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   251, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   252, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   9,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_win_07_loop[] = {
	CG_FRAME(    0,   -1,  -1, END_OF_DATA,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_win_07_loop ),
};

/* Char_Win 8: win_pose_init */
const Sint32 pl1c_win_08[] = {
	CG_FRAME(    0,    0,   6,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   251, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  48,           0,   252, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_win_08 ),
};

/* Char_Win 10 */
const Sint32 pl1c_win_10[] = {
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   6,           0,   306, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   308, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  20,           0,   305, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_win_10_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_win_10_loop ),
};

/* Char_Win 9 */
const Sint32 pl1c_win_09[] = {
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  32,           0,   311, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_win_09_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   259, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_win_09_loop ),
};

/* Char_Win 11 */
const Sint32 pl1c_win_11[] = {
	CG_FRAME(    0,    0,   3,           0,   260, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    1,   1,           0,   262, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_win_11_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   262, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_win_11_loop ),
};

/* Char_Win 12: win_roll_2_wait */
const Sint32 pl1c_win_12[] = {
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  24,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  15,           0,   251, 0x80,   0x0e,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   252, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   9,           0,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_win_12_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   256, 0x80,   0x0f,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_win_12_loop ),
};

/* Char_Win 2
   Char_Win 3
   Char_Win 5: lose_init */
const Sint32 pl1c_win_02[] = {
	CG_FRAME(    0,    0,   1,           0,   208, 0x80,   0x00,             0x0f,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    -1, 0x00,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_win_02 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl1c_change_00[] = {
	CG_FRAME(    0,    0,   1,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   146, 0x80,   0x00,                5,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_change_00_loop[] = {
	CG_FRAME(    0,    1,  24, END_OF_DATA,   152, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl1c_change_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   252, 0x80,   0x11,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   253, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   254, 0x80,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   256, 0x80,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   255, 0x80,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_change_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   256, 0x80,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_change_01_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl1c_change_03[] = {
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x0f,                3,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,  16,           0,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_change_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   251, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_change_03_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl1c_change_02[] = {
	CG_FRAME(    0,    0,   1,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1c_change_02_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_change_02_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl1c_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   456, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x00,                0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   458, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   461, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   462, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   463, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   464, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   465, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_change_04 ),
};

/* Char_Caught 0 */
const Sint32 pl1c_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   472, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl1c_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   473, 0x40,   0x13,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl1c_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   474, 0x40,   0x17,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl1c_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   475, 0x40,   0x12,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl1c_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   476, 0x40,   0x12,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl1c_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   477, 0x40,   0x12,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl1c_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   479, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1c_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl1c_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   480, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1c_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl1c_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   481, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1c_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl1c_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   482, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1c_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl1c_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   483, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1c_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl1c_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   484, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1c_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl1c_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   485, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl1c_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl1c_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   492, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1c_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl1c_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   493, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1c_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl1c_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   494, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1c_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl1c_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   495, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1c_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl1c_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   496, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl1c_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   508, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1c_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl1c_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   514, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1c_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl1c_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   506, 0x40,   0x53,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl1c_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl1c_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   504, 0x40,   0x51,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1c_caught_21 ),
};

/* group 25 0: pls1c_03_init, pls1c_03_hide, pls1c_03_stand_init, pls1c_03_join_init
   Char_Fly 0 */
const Sint32 pl1c_grp25_00[] = {
	CG_FRAME(    0,    0,   5,           0,   520, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   521, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   522, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   523, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   522, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   521, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_00 ),
};

/* group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl1c_grp25_01[] = {
	CG_FRAME(    0,    0,   6,           0,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_01 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl1c_grp25_02[] = {
	CG_FRAME(    0,    0,   6,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_02 ),
};

/* group 25 3: pls1c_03_t6_init
   Char_Fly 3 */
const Sint32 pl1c_grp25_03[] = {
	CG_FRAME(    0,    0,   5,           0,   526, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   527, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   528, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   529, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   528, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   527, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_03 ),
};

/* group 25 4: pls1c_03_walk_init
   Char_Fly 4 */
const Sint32 pl1c_grp25_04[] = {
	CG_FRAME(    0,    0,   4,           0,   530, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   531, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   533, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   534, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   535, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   536, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   537, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_04 ),
};

/* group 25 5: pls1c_03_run_init, pls1c_03_win_join_init, pls1c_03_win_join_wait, pls1c_07_rush_turn_end
   Char_Fly 5 */
const Sint32 pl1c_grp25_05[] = {
	CG_FRAME(    0,    0,   3,           0,   538, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   539, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   540, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   541, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   542, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   543, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   544, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   545, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_05 ),
};

/* group 25 6: pls1c_03_turn_init
   Char_Fly 6 */
const Sint32 pl1c_grp25_06[] = {
	CG_FRAME(    0,    0,   4,           0,   546, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_06_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   548, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_06_loop ),
};

/* group 25 7: pls1c_03_win_join_near
   Char_Fly 7 */
const Sint32 pl1c_grp25_07[] = {
	CG_FRAME(    0,    0,   6,           0,   520, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_07_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   549, 0x40,   0x03,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_07_loop ),
};

/* group 25 8: pls1c_03_t7_init
   Char_Fly 8 */
const Sint32 pl1c_grp25_08[] = {
	CG_FRAME(    0,    0,   6,           0,   526, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   554, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   555, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   556, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_08 ),
};

/* group 25 9: pls1c_03_damage_init
   Char_Fly 9 */
const Sint32 pl1c_grp25_09[] = {
	CG_FRAME(    0,    0,   3,           0,   557, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   558, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_09_loop ),
};

/* group 25 10: pls1c_03_charge_init
   Char_Fly 10 */
const Sint32 pl1c_grp25_10[] = {
	CG_FRAME(    0,    0,   3,           0,   564, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_10_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   565, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   566, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   567, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   568, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   567, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   566, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_10_loop ),
};

/* group 25 11
   Char_Fly 11 */
const Sint32 pl1c_grp25_11[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   564, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_11 ),
};

/* group 25 12: pls1c_03_appear_init, pls1c_03_away_wait
   Char_Fly 12 */
const Sint32 pl1c_grp25_12[] = {
	CG_FRAME(    0,    0,   1,           0,   569, 0x40,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_12_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_12_loop ),
};

/* group 25 15: pls1c_03_leave_init, pls1c_03_away_init
   Char_Fly 15 */
const Sint32 pl1c_grp25_15[] = {
	CG_FRAME(    0,    0,   3,           0,   527, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_15_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   570, 0x40,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_15_loop ),
};

/* group 25 20: pls1c_04_turn_init, pls1c_07_eddie_init, pls1c_07_t6_near, pls1c_07_t13_wait, ...
   Char_Fly 20 */
const Sint32 pl1c_grp25_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   586, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_20 ),
};

/* group 25 21: pls1c_04_run_init, pls1c_07_t5_init, pls1c_07_t6_init, pls1c_07_t6_wait, ...
   Char_Fly 21 */
const Sint32 pl1c_grp25_21[] = {
	CG_FRAME(    0,    0,   8,           0,   587, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   588, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   589, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   590, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_21 ),
};

/* group 25 22: pls1c_04_throw_init, pls1c_07_t13_init
   Char_Fly 22 */
const Sint32 pl1c_grp25_22[] = {
	CG_FRAME(    0,    0,   4,           0,   591, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   592, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   593, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   594, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   595, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16,           0,   596, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   595, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   593, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   591, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_22 ),
};

/* group 25 23: pls1c_04_drop_init
   Char_Fly 23 */
const Sint32 pl1c_grp25_23[] = {
	CG_FRAME(    0,    0,   1,           0,   597, 0x40,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   591, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_23_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   586, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_23_loop ),
};

/* group 25 24: pls1c_04_jump_init
   Char_Fly 24 */
const Sint32 pl1c_grp25_24[] = {
	CG_FRAME(    0,    0,   3,           0,   586, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   591, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_24_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   570, 0x40,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_24_loop ),
};

/* group 25 25: pls1c_07_t5_run
   Char_Fly 25 */
const Sint32 pl1c_grp25_25[] = {
	CG_FRAME(    0,    0,   6,           0,   319, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   320, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   321, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   322, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   323, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_25_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   324, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   325, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   326, 0x80,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_25_loop ),
};

/* group 27 8: pls1c_05_b_init */
const Sint32 pl1c_grp27_08[] = {
	EF_FRAME(    0,    0,  80,           0,   598 ),
	EF_FRAME(    0,    0,   2,           0,   599 ),
	EF_FRAME(    0,    0,   2,           0,   600 ),
	EF_FRAME(    0,    0,   2,           0,   601 ),
	EF_FRAME(    0,    0,   2,           0,   602 ),
	EF_FRAME(    0,    0,   2,           0,   603 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   604 ),
	CG_NEXT( pl1c_grp27_08 ),
};

/* group 27 9: pls1c_05_c_init */
const Sint32 pl1c_grp27_09[] = {
	EF_FRAME(    0,    0,  -1, END_OF_DATA,   605 ),
	CG_NEXT( pl1c_grp27_09 ),
};

/* group 27 10 */
const Sint32 pl1c_grp27_10[] = {
	EF_FRAME(    0,    0,   3,           0, 32772 ),
	EF_FRAME(    0,    0,   3,           0, 32773 ),
	EF_FRAME(    0,    0,   3,           0, 32774 ),
	EF_FRAME(    0,    0,   3,           0, 32775 ),
	EF_FRAME(    0,    0,   3,           0, 32776 ),
	EF_FRAME(    0,    0,   3,           0, 32777 ),
	EF_FRAME(    0,    0,   3,           0, 32778 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA, 32779 ),
	CG_NEXT( pl1c_grp27_10 ),
};

/* group 27 11 */
const Sint32 pl1c_grp27_11[] = {
	EF_FRAME(    0,    0,   3,           0,   606 ),
	EF_FRAME(    0,    0,   3,           0,   607 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   608 ),
	CG_NEXT( pl1c_grp27_11 ),
};

/* group 27 12 */
const Sint32 pl1c_grp27_12[] = {
	EF_FRAME(    0,    0,   3,           0,   621 ),
	EF_FRAME(    0,    0,   3,           0,   622 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   623 ),
	CG_NEXT( pl1c_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl1c_grp27_13[] = {
	EF_FRAME(    0,    0,  32, END_OF_DATA,   638 ),
	CG_NEXT( pl1c_grp27_13 ),
};

/* group 27 14 */
const Sint32 pl1c_grp27_14[] = {
	EF_FRAME(    0,    0,  32, END_OF_DATA,   640 ),
	CG_NEXT( pl1c_grp27_14 ),
};

/* group 27 15 */
const Sint32 pl1c_grp27_15[] = {
	EF_FRAME(    0,    0,  32, END_OF_DATA,   639 ),
	CG_NEXT( pl1c_grp27_15 ),
};

/* group 27 35 */
const Sint32 pl1c_grp27_35[] = {
	EF_FRAME(    0,    0,   6,           0,   559 ),
	EF_FRAME(    0,    0,   6,           0,   560 ),
	EF_FRAME(    0,    0,   6,           0,   561 ),
	EF_FRAME(    0,    0,   6,           0,   562 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   563 ),
	CG_NEXT( pl1c_grp27_35 ),
};

/* group 27 5 */
const Sint32 pl1c_grp27_05[] = {
	EF_FRAME(    0,    0,   2,           0,   452 ),
	EF_FRAME(    0,    0,   2,           0,   453 ),
	EF_FRAME(    0,    0,   2,           0,   454 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   455 ),
	CG_NEXT( pl1c_grp27_05 ),
};

/* Char_Effect 19: shl1c_02_a_wait */
const Sint32 pl1c_effect_19[] = {
	CG_FRAME(    0,    0,   3,           0,   606, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   607, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   608, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl1c_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl1c_effect_20[] = {
	CG_FRAME(    0,    0,   3,           0,   606, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   607, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   608, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl1c_effect_21[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   609, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl1c_effect_22[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   610, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl1c_effect_23[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   611, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl1c_effect_24[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   612, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl1c_effect_25[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   613, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_25 ),
};

/* Char_Effect 4 */
const Sint32 pl1c_effect_04[] = {
	CG_FRAME(    0,    0,   6,           0,   614, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   615, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   616, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_effect_04_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_04_loop ),
};

/* Char_Effect 5: shl1c_03_a_fly */
const Sint32 pl1c_effect_05[] = {
	CG_FRAME(    0,    0,   4,           0,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   619, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   620, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,   4,           0,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   619, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   620, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,   3,           0,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   619, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   620, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   619, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   620, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
};

const Sint32 pl1c_effect_05_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   619, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   620, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl1c_effect_05_loop ),
};

/* Char_Effect 6 */
const Sint32 pl1c_effect_06[] = {
	CG_FRAME(    0,    0,   3,           0,   624, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   625, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   626, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   627, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_06 ),
};

/* Char_Effect 7: shl1c_03_c_keep */
const Sint32 pl1c_effect_07[] = {
	CG_FRAME(    0,    0,   3,           0,   628, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   629, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl1c_effect_08[] = {
	CG_FRAME(    0,    0,   3,           0,   630, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   631, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   632, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   633, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   634, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   635, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   636, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   637, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_08 ),
};

/* Char_Effect 9: shl1c_04_b_init, shl1c_04_b_fall_init */
const Sint32 pl1c_effect_09[] = {
	CG_FRAME(    0,    0,   3,           0, 32772, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32773, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32774, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32775, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32776, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32777, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32778, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32779, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_09 ),
};

/* Char_Effect 10: shl1c_04_b_keep */
const Sint32 pl1c_effect_10[] = {
	CG_FRAME(    0,    0,   3,           0, 32772, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32773, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32774, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32775, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32776, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32777, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0, 32778, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32779, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl1c_effect_10 ),
};

/* group 27 16: pls1c_06_init */
const Sint32 pl1c_grp27_16[] = {
	EF_FRAME(    0,    0,   6,           0, 32768 ),
	EF_FRAME(    0,    0,   6,           0, 32769 ),
	EF_FRAME(    0,    0,   6,           0, 32770 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA, 32771 ),
	CG_NEXT( pl1c_grp27_16 ),
};

/* group 27 6: pls1c_02_wait */
const Sint32 pl1c_grp27_06[] = {
	EF_FRAME(    0,    0,   3,           0,   675 ),
	EF_FRAME(    0,    0,   3,           0,   676 ),
	EF_FRAME(    0,    0,   3,           0,   677 ),
	EF_FRAME(    0,    0,   3,           0,   678 ),
	EF_FRAME(    0,    0,   3,           0,   679 ),
	EF_FRAME(    0,    0,   3,           0,   680 ),
	EF_FRAME(    0,    0,   3,           0,   681 ),
	EF_FRAME(    0,    0,   3,           0,   682 ),
	EF_FRAME(    0,    0,   3,           0,   683 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   684 ),
	CG_NEXT( pl1c_grp27_06 ),
};

/* group 27 7: pls1c_02_hold */
const Sint32 pl1c_grp27_07[] = {
	EF_FRAME(    0,    0,   3,           0,   685 ),
	EF_FRAME(    0,    0,   3,           0,   686 ),
	EF_FRAME(    0,    0,   3,           0,   687 ),
	EF_FRAME(    0,    0,   3,           0,   688 ),
	EF_FRAME(    0,    0,   3,           0,   689 ),
	EF_FRAME(    0,    0,   3,           0,   690 ),
	EF_FRAME(    0,    0,   3,           0,   691 ),
	EF_FRAME(    0,    0,   3,           0,   692 ),
	EF_FRAME(    0,    0,   3,           0,   693 ),
	EF_FRAME(    0,    0,   3,           0,   694 ),
	EF_FRAME(    0,    0,   3,           0,   685 ),
	EF_FRAME(    0,    0,   3,           0,   686 ),
	EF_FRAME(    0,    0,   3,           0,   687 ),
	EF_FRAME(    0,    0,   3,           0,   688 ),
	EF_FRAME(    0,    0,   3,           0,   689 ),
	EF_FRAME(    0,    0,   3,           0,   690 ),
	EF_FRAME(    0,    0,   3,           0,   691 ),
	EF_FRAME(    0,    0,   3,           0,   692 ),
	EF_FRAME(    0,    0,   3,           0,   693 ),
	EF_FRAME(    0,    0,   3,           0,   694 ),
	EF_FRAME(    0,    0,   2,           0,   685 ),
	EF_FRAME(    0,    0,   2,           0,   686 ),
	EF_FRAME(    0,    0,   2,           0,   687 ),
	EF_FRAME(    0,    0,   2,           0,   688 ),
	EF_FRAME(    0,    0,   2,           0,   689 ),
	EF_FRAME(    0,    0,   2,           0,   690 ),
	EF_FRAME(    0,    0,   2,           0,   691 ),
	EF_FRAME(    0,    0,   2,           0,   692 ),
	EF_FRAME(    0,    0,   2,           0,   693 ),
	EF_FRAME(    0,    0,   2,           0,   694 ),
	EF_FRAME(    0,    0,   2,           0,   685 ),
	EF_FRAME(    0,    0,   2,           0,   686 ),
	EF_FRAME(    0,    0,   2,           0,   687 ),
	EF_FRAME(    0,    0,   2,           0,   688 ),
	EF_FRAME(    0,    0,   2,           0,   689 ),
	EF_FRAME(    0,    0,   2,           0,   690 ),
	EF_FRAME(    0,    0,   2,           0,   691 ),
	EF_FRAME(    0,    0,   2,           0,   692 ),
	EF_FRAME(    0,    0,   2,           0,   693 ),
	EF_FRAME(    0,    0,   2,           0,   694 ),
};

const Sint32 pl1c_grp27_07_loop[] = {
	EF_FRAME(    0,    0,   1,           0,   685 ),
	EF_FRAME(    0,    0,   1,           0,   686 ),
	EF_FRAME(    0,    0,   1,           0,   687 ),
	EF_FRAME(    0,    0,   1,           0,   688 ),
	EF_FRAME(    0,    0,   1,           0,   689 ),
	EF_FRAME(    0,    0,   1,           0,   690 ),
	EF_FRAME(    0,    0,   1,           0,   691 ),
	EF_FRAME(    0,    0,   1,           0,   692 ),
	EF_FRAME(    0,    0,   1,           0,   693 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   694 ),
	CG_NEXT( pl1c_grp27_07_loop ),
};

/* group 27 3 */
const Sint32 pl1c_grp27_03[] = {
	EF_FRAME(    0,    0,   2,           0,   641 ),
	EF_FRAME(    0,    0,   2,           0,   642 ),
	EF_FRAME(    0,    0,   2,           0,   643 ),
	EF_FRAME(    0,    0,   2,           0,   644 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   645 ),
	CG_NEXT( pl1c_grp27_03 ),
};

/* Char_Effect 0 */
const Sint32 pl1c_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0,   646, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0,   647, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0,   648, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0,   649, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0,   650, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   651, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl1c_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl1c_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   652, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   653, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   654, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   655, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   656, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   657, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_01 ),
};

/* group 27 4 */
const Sint32 pl1c_grp27_04[] = {
	EF_FRAME(    0,    0,   2,           0,   658 ),
	EF_FRAME(    0,    0,   2,           0,   659 ),
	EF_FRAME(    0,    0,   2,           0,   660 ),
	EF_FRAME(    0,    0,   2,           0,   661 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   662 ),
	CG_NEXT( pl1c_grp27_04 ),
};

/* Char_Effect 2 */
const Sint32 pl1c_effect_02[] = {
	CG_FRAME(    0,    0,   2,           0,   664, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   665, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   666, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   667, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   668, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   663, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1c_effect_02 ),
};

/* Char_Effect 3: shl1c_01_fly */
const Sint32 pl1c_effect_03[] = {
	CG_FRAME(    0,    0,   2,           0,   669, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   670, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   671, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   672, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   673, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   674, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_effect_03 ),
};

/* group 25 19: pls1c_07_beat_init, pls1c_07_t12_init
   Char_Fly 19 */
const Sint32 pl1c_grp25_19[] = {
	CG_FRAME(    0,    0,   4,           0,   578, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   579, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   581, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   582, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   583, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   584, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   585, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_19 ),
};

/* group 25 13
   Char_Fly 13 */
const Sint32 pl1c_grp25_13[] = {
	CG_FRAME(    0,    0,   1,           0,   263, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   264, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_13 ),
};

/* group 25 14: pls1c_03_win_init, pls1c_03_t16_init
   Char_Fly 14 */
const Sint32 pl1c_grp25_14[] = {
	CG_FRAME(    0,    0,   4,           0,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   554, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   555, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   556, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   572, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   575, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_14_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   576, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   577, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_14_loop ),
};

/* group 25 18: pls1c_07_t8_init
   Char_Fly 18 */
const Sint32 pl1c_grp25_18[] = {
	CG_FRAME(    0,    0,   1,           0,   569, 0x40,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   554, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   555, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   556, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   572, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   575, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_18_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   576, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   577, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_18_loop ),
};

/* group 25 16: pls1c_07_rush_init
   Char_Fly 16 */
const Sint32 pl1c_grp25_16[] = {
	CG_FRAME(    0,    0,   1,           0,   542, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   543, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   544, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   545, 0x40,   0x11,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   520, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_16_loop[] = {
	CG_FRAME(    0,    0,   5,           0,   526, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   527, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   528, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   529, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   528, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   527, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_16_loop ),
};

/* group 25 17: pls1c_07_rush_join, pls1c_07_rush_ride
   Char_Fly 17 */
const Sint32 pl1c_grp25_17[] = {
	CG_FRAME(    0,    0,   1,           0,   526, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   525, 0x40,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   524, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   520, 0x40,   0x31,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   546, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   548, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_17 ),
};

/* Char_Super 17: beat_init (Beat Plane) */
const Sint32 pl1c_super_17[] = {
	CG_FRAME(    0,    1,   1,           0,   248, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  12,           0,   249, 0x90,   0x0f,              ATK,  0x00,       0x15,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   250, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   251, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_super_17 ),
};

/* Char_Super 33: beat_main (Beat Plane) */
const Sint32 pl1c_super_33[] = {
	CG_FRAME(    0,    1,  32,           0,   724, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
};

const Sint32 pl1c_super_33_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    -1, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1c_super_33_loop ),
};

/* Char_Super 25: beat_end_init (Beat Plane) */
const Sint32 pl1c_super_25[] = {
	CG_FRAME(    0,    0,  22,           0,   251, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_super_25 ),
};

/* Char_Super 26: beat_end_init (Beat Plane) */
const Sint32 pl1c_super_26[] = {
	CG_FRAME(    0,    0,  36,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1c_super_26_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   126, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_super_26_loop ),
};

/* Char_Super 18: beat_charge (Beat Plane), beat_main (Beat Plane) */
const Sint32 pl1c_super_18[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   724, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1c_super_18 ),
};

/* Char_Super 19 */
const Sint32 pl1c_super_19[] = {
	CG_FRAME(    0,    0,   1,           0,   717, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   718, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   719, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   720, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_19 ),
};

/* Char_Super 20: beat_main (Beat Plane) */
const Sint32 pl1c_super_20[] = {
	CG_FRAME(    0,    0,   4,           0,   734, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   735, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1c_super_20 ),
};

/* Char_Super 21 */
const Sint32 pl1c_super_21[] = {
	CG_FRAME(    0,    0,  10,           0,   721, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   722, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   723, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   722, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_21 ),
};

/* Char_Super 22 */
const Sint32 pl1c_super_22[] = {
	CG_FRAME(    0,    0,   8,           0,   725, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   726, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   727, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_22 ),
};

/* Char_Super 23 */
const Sint32 pl1c_super_23[] = {
	CG_FRAME(    0,    0,   4,           0,   728, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   729, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   730, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   731, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_23 ),
};

/* Char_Super 24 */
const Sint32 pl1c_super_24[] = {
	CG_FRAME(    0,    0,   1,           0,    -1, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   732, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6,           0,   733, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   732, 0x40,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    -1, 0x40,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_24 ),
};

/* Char_Super 30 */
const Sint32 pl1c_super_30[] = {
	CG_FRAME(    0,    0,   4,           0,   736, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   737, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   738, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   739, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   740, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   741, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   742, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   743, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl1c_super_30 ),
};

/* Char_Super 31: shl1c_09_shot_fly */
const Sint32 pl1c_super_31[] = {
	CG_FRAME(    0,    0,   4,           0,   744, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   745, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   746, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   747, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   748, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   749, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_31 ),
};

/* Char_Super 32 */
const Sint32 pl1c_super_32[] = {
	CG_FRAME(    0,    0,   4,           0,  1013, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,  1014, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  1015, 0x40,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl1c_super_32 ),
};

/* Char_Super 7: drill_init (Rush Drill (hyper)) */
const Sint32 pl1c_super_07[] = {
	CG_FRAME(    0,    1,   1,           0,   248, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  16,           0,   248, 0x90,   0x00,              ATK,  0x00,       0x16,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   249, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   250, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   251, 0x90,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_super_07 ),
};

/* Char_Super 34: drill_main (Rush Drill (hyper)) */
const Sint32 pl1c_super_34[] = {
	CG_FRAME(    0,    1,  32,           0,    -1, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
};

const Sint32 pl1c_super_34_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    -1, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl1c_super_34_loop ),
};

/* Char_Super 15: drill_end_init (Rush Drill (hyper)) */
const Sint32 pl1c_super_15[] = {
	CG_FRAME(    0,    0,  22,           0,   251, 0x90,   0x0e,              ATE,  0x00,          0,    0,   0,     0, 0x000e,   5 ),
	CG_FRAME(    0,    0,   2,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1c_super_15 ),
};

/* Char_Super 16: drill_end_init (Rush Drill (hyper)) */
const Sint32 pl1c_super_16[] = {
	CG_FRAME(    0,    0,  36,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1c_super_16_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   126, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1c_super_16_loop ),
};

/* Char_Super 8: drill_charge (Rush Drill (hyper)), drill_turn (Rush Drill (hyper)), drill_walk (Rush Drill (hyper)), drill_back (Rush Drill (hyper)), ... */
const Sint32 pl1c_super_08[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    -1, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl1c_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl1c_super_09[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   695, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_09 ),
};

/* Char_Super 11 */
const Sint32 pl1c_super_11[] = {
	CG_FRAME(    0,    1,   2,           0,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0,   702, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   703, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl1c_super_11 ),
};

/* Char_Super 12 */
const Sint32 pl1c_super_12[] = {
	CG_FRAME(    0,    0,   3,           0,   704, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   705, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   706, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_12 ),
};

/* Char_Super 38 */
const Sint32 pl1c_super_38[] = {
	CG_FRAME(    0,    0,   3,           0,   709, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   710, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   711, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   712, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_38 ),
};

/* Char_Super 39 */
const Sint32 pl1c_super_39[] = {
	CG_FRAME(    0,    0,   3,           0,   713, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   714, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   715, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   716, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_39 ),
};

/* Char_Super 13: shl1c_07_4_walk_init */
const Sint32 pl1c_super_13[] = {
	CG_FRAME(    3,    0,   3,           0,   696, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   3,           0,   697, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   3,           0,   698, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   3, END_OF_DATA,   699, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_13 ),
};

/* Char_Super 36: shl1c_07_4_shot_init */
const Sint32 pl1c_super_36[] = {
	CG_FRAME(    3,    0,   1,           0,   696, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   1,           0,   697, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   1,           0,   698, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   1, END_OF_DATA,   699, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_36 ),
};

/* Char_Super 35: shl1c_07_4_back_init */
const Sint32 pl1c_super_35[] = {
	CG_FRAME(    0,    3,   3,           0,   699, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   3,           0,   698, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   3,           0,   697, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,   3, END_OF_DATA,   696, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_35 ),
};

/* Char_Super 37 */
const Sint32 pl1c_super_37[] = {
	CG_FRAME(    0,    3,   1,           0,   699, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   1,           0,   698, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    1,   1,           0,   697, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,    0,   1, END_OF_DATA,   696, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_37 ),
};

/* Char_Super 14 */
const Sint32 pl1c_super_14[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   707, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_14 ),
};

/* Char_Super 10: drill_turn (Rush Drill (hyper)) */
const Sint32 pl1c_super_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   708, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl1c_super_10 ),
};

/* Char_Super 40: pls1c_07_t9_init */
const Sint32 pl1c_super_40[] = {
	CG_FRAME(    0,    0,   6,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   194, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   196, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_super_40_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   199, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_super_40_loop ),
};

/* group 25 26: pls1c_07_t7_init
   Char_Fly 26 */
const Sint32 pl1c_grp25_26[] = {
	CG_FRAME(    0,    1,   5,           0,  1016, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    1,   5,           0,  1017, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    1,   5,           0,  1018, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    1,   5,           0,  1019, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    4,    1,   5,           0,  1020, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    5,    1,   5, END_OF_DATA,  1021, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_grp25_26 ),
};

/* group 25 27: pls1c_07_t7_run
   Char_Fly 27 */
const Sint32 pl1c_grp25_27[] = {
	CG_FRAME(    0,    0,   6,           0,   271, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   277, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   278, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   279, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   280, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   281, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1c_grp25_27_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   272, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   275, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   276, 0x80,   0x21,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_27_loop ),
};

/* group 25 30
   Char_Fly 30 */
const Sint32 pl1c_grp25_30[] = {
	CG_FRAME(    0,    0,   6,           0,   750, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   751, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   752, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   753, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   754, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   946, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   947, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   948, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1c_grp25_30 ),
};

/* Char_Super 41
   Char_Super 42
   Char_Effect 26
   group 27 38
   group 27 39
   group 27 40 */
const Sint32 pl1c_super_41[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1c_super_41 ),
};

/* group 27 36 */
const Sint32 pl1c_grp27_36[] = {
	EF_FRAME(    0,    0,   2,           0,   282 ),
	EF_FRAME(    0,    0,   2,           0,   283 ),
	EF_FRAME(    0,    0,   2,           0,   284 ),
	EF_FRAME(    0,    0,   2,           0,   285 ),
	EF_FRAME(    0,    0,   2,           0,   286 ),
	EF_FRAME(    0,    0,   2,           0,   287 ),
	EF_FRAME(    0,    0,   2,           0,   288 ),
	EF_FRAME(    0,    0,   2,           0,   289 ),
	EF_FRAME(    0,    0,   2,           0,   290 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   291 ),
	CG_NEXT( pl1c_grp27_36 ),
};

/* group 27 37 */
const Sint32 pl1c_grp27_37[] = {
	EF_FRAME(    0,    0,   2,           0,   292 ),
	EF_FRAME(    0,    0,   2,           0,   293 ),
	EF_FRAME(    0,    0,   2,           0,   294 ),
	EF_FRAME(    0,    0,   2,           0,   295 ),
	EF_FRAME(    0,    0,   2,           0,   296 ),
	EF_FRAME(    0,    0,   2,           0,   297 ),
	EF_FRAME(    0,    0,   2,           0,   298 ),
	EF_FRAME(    0,    0,   2,           0,   299 ),
	EF_FRAME(    0,    0,   2,           0,   300 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   301 ),
	CG_NEXT( pl1c_grp27_37 ),
};

/* group 27 42: pls1c_00_t8 */
const Sint32 pl1c_grp27_42[] = {
	EF_FRAME(    0,    0,   6,           0,   265 ),
	EF_FRAME(    0,    0,   6,           0,   266 ),
	EF_FRAME(    0,    0,   6,           0,   267 ),
	EF_FRAME(    0,    0,   6,           0,   268 ),
	EF_FRAME(    0,    0,   6,           0,   269 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   270 ),
	CG_NEXT( pl1c_grp27_42 ),
};
