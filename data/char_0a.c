/* Rogue (character 0a): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_0a      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_0a     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl0a.c) says which group
                   and number each move uses.
   pl0a_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl0a_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl0a_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl0a_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_0a
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_0a, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_0a */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl0a_stand_00[], pl0a_stand_53[], pl0a_stand_54[], pl0a_stand_55[],
	pl0a_stand_56[], pl0a_stand_57[], pl0a_stand_58[], pl0a_stand_59[], pl0a_stand_60[],
	pl0a_stand_61[], pl0a_stand_10[], pl0a_stand_11[], pl0a_stand_01[], pl0a_stand_02[],
	pl0a_stand_13[], pl0a_stand_14[], pl0a_stand_15[], pl0a_stand_16[], pl0a_stand_17[],
	pl0a_stand_18[], pl0a_stand_19[], pl0a_stand_20[], pl0a_stand_21[], pl0a_stand_22[],
	pl0a_stand_23[], pl0a_stand_24[], pl0a_stand_25[], pl0a_stand_26[], pl0a_stand_27[],
	pl0a_stand_28[], pl0a_stand_29[], pl0a_stand_30[], pl0a_stand_31[], pl0a_stand_08[],
	pl0a_stand_03[], pl0a_stand_33[], pl0a_stand_34[], pl0a_stand_35[], pl0a_stand_36[],
	pl0a_stand_37[], pl0a_stand_38[], pl0a_stand_39[], pl0a_stand_40[], pl0a_stand_41[],
	pl0a_stand_42[], pl0a_stand_43[], pl0a_stand_44[], pl0a_stand_45[], pl0a_stand_46[],
	pl0a_stand_47[], pl0a_stand_48[], pl0a_stand_49[], pl0a_stand_50[], pl0a_stand_51[],
	pl0a_stand_09[], pl0a_stand_06[], pl0a_stand_07[], pl0a_jump_00[], pl0a_jump_00_loop[],
	pl0a_jump_01[], pl0a_jump_01_loop[], pl0a_jump_02[], pl0a_jump_02_loop[], pl0a_jump_03[],
	pl0a_jump_06[], pl0a_jump_09[], pl0a_jump_07[], pl0a_jump_07_loop[], pl0a_jump_08[],
	pl0a_jump_08_loop[], pl0a_dash_00[], pl0a_dash_00_loop[], pl0a_dash_02[], pl0a_dash_04[],
	pl0a_dash_04_loop[], pl0a_dash_01[], pl0a_dash_01_loop[], pl0a_dash_03[], pl0a_crouch_00[],
	pl0a_crouch_01[], pl0a_crouch_02[], pl0a_turn_00[], pl0a_turn_01[], pl0a_guard_00[],
	pl0a_guard_01[], pl0a_guard_02[], pl0a_guard_03[], pl0a_guard_03_loop[], pl0a_guard_04[],
	pl0a_guard_05[], pl0a_guard_06[], pl0a_guard_07[], pl0a_guard_07_loop[], pl0a_air_guard_00[],
	pl0a_air_guard_02[], pl0a_air_guard_01[], pl0a_stand_p_00[], pl0a_stand_p_00_loop[],
	pl0a_stand_p_01[], pl0a_stand_p_01_loop[], pl0a_stand_p_02[], pl0a_stand_p_02_loop[],
	pl0a_super_08[], pl0a_super_08_loop[], pl0a_stand_k_00[], pl0a_stand_k_00_loop[],
	pl0a_stand_k_01[], pl0a_stand_k_01_loop[], pl0a_stand_k_02[], pl0a_stand_k_02_loop[],
	pl0a_stand_k_03[], pl0a_stand_k_03_loop[], pl0a_extra_23[], pl0a_extra_23_loop[],
	pl0a_extra_24[], pl0a_extra_24_loop[], pl0a_crouch_p_00[], pl0a_crouch_p_00_loop[],
	pl0a_crouch_p_01[], pl0a_crouch_p_01_loop[], pl0a_crouch_p_02[], pl0a_crouch_p_02_loop[],
	pl0a_crouch_k_00[], pl0a_crouch_k_00_loop[], pl0a_crouch_k_01[], pl0a_crouch_k_01_loop[],
	pl0a_crouch_k_02[], pl0a_crouch_k_02_loop[], pl0a_jump_p_00[], pl0a_jump_p_00_loop[],
	pl0a_jump_p_01[], pl0a_jump_p_01_loop[], pl0a_jump_p_02[], pl0a_jump_p_02_loop[],
	pl0a_jump_k_00[], pl0a_jump_k_00_loop[], pl0a_jump_k_01[], pl0a_jump_k_01_loop[],
	pl0a_jump_k_02[], pl0a_jump_k_02_loop[], pl0a_jump_k_05[], pl0a_jump_k_05_loop[],
	pl0a_get_up_00[], pl0a_get_up_01[], pl0a_get_up_02[], pl0a_get_up_03[], pl0a_damage_00[],
	pl0a_damage_01[], pl0a_damage_02[], pl0a_damage_03[], pl0a_damage_04[], pl0a_damage_05[],
	pl0a_damage_06[], pl0a_damage_07[], pl0a_damage_08[], pl0a_damage_10[], pl0a_damage_11[],
	pl0a_damage_12[], pl0a_damage_13[], pl0a_damage_34[], pl0a_damage_09[], pl0a_damage_09_loop[],
	pl0a_damage_16[], pl0a_damage_17[], pl0a_damage_18[], pl0a_damage_19[], pl0a_damage_20[],
	pl0a_damage_21[], pl0a_damage_22[], pl0a_damage_23[], pl0a_damage_24[], pl0a_damage_25[],
	pl0a_damage_26[], pl0a_unused_00[], pl0a_damage_27[], pl0a_damage_32[], pl0a_damage_28[],
	pl0a_damage_29[], pl0a_damage_31[], pl0a_dizzy_00[], pl0a_catch_00[], pl0a_catch_00_loop[],
	pl0a_catch_01[], pl0a_catch_01_loop[], pl0a_catch_02[], pl0a_catch_03[], pl0a_catch_04[],
	pl0a_catch_05[], pl0a_catch_06[], pl0a_catch_07[], pl0a_catch_07_loop[], pl0a_catch_08[],
	pl0a_catch_09[], pl0a_catch_10[], pl0a_catch_10_loop[], pl0a_appear_00[], pl0a_appear_01[],
	pl0a_appear_01_loop[], pl0a_caught_00[], pl0a_caught_01[], pl0a_caught_02[], pl0a_caught_03[],
	pl0a_caught_04[], pl0a_caught_05[], pl0a_caught_06[], pl0a_caught_07[], pl0a_caught_08[],
	pl0a_caught_09[], pl0a_caught_10[], pl0a_caught_11[], pl0a_caught_12[], pl0a_caught_13[],
	pl0a_caught_14[], pl0a_caught_15[], pl0a_caught_16[], pl0a_caught_17[], pl0a_caught_18[],
	pl0a_caught_19[], pl0a_caught_20[], pl0a_caught_21[], pl0a_win_00[], pl0a_win_00_loop[],
	pl0a_win_01[], pl0a_win_01_loop[], pl0a_win_02[], pl0a_win_02_loop[], pl0a_win_03[],
	pl0a_win_03_loop[], pl0a_win_04[], pl0a_win_04_loop[], pl0a_win_05[], pl0a_win_05_loop[],
	pl0a_win_06[], pl0a_win_06_loop[], pl0a_win_07[], pl0a_win_07_loop[], pl0a_win_08[],
	pl0a_win_08_loop[], pl0a_win_09[], pl0a_win_09_loop[], pl0a_extra_22[], pl0a_extra_22_loop[],
	pl0a_extra_00[], pl0a_extra_00_loop[], pl0a_extra_01[], pl0a_extra_01_loop[], pl0a_change_00[],
	pl0a_change_00_loop[], pl0a_change_02[], pl0a_change_01[], pl0a_special_00[],
	pl0a_special_00_loop[], pl0a_special_03[], pl0a_special_03_loop[], pl0a_special_01[],
	pl0a_special_01_loop[], pl0a_special_04[], pl0a_special_04_loop[], pl0a_special_02[],
	pl0a_special_02_loop[], pl0a_special_05[], pl0a_special_05_loop[], pl0a_special_06[],
	pl0a_special_06_loop[], pl0a_special_51[], pl0a_special_51_loop[], pl0a_special_52[],
	pl0a_special_52_loop[], pl0a_special_07[], pl0a_special_07_loop[], pl0a_special_08[],
	pl0a_special_08_loop[], pl0a_special_09[], pl0a_special_09_loop[], pl0a_special_12[],
	pl0a_special_12_loop[], pl0a_special_13[], pl0a_special_13_loop[], pl0a_special_14[],
	pl0a_special_14_loop[], pl0a_special_15[], pl0a_special_15_loop[], pl0a_special_16[],
	pl0a_special_16_loop[], pl0a_special_17[], pl0a_special_17_loop[], pl0a_special_18[],
	pl0a_special_18_loop[], pl0a_special_19[], pl0a_special_19_loop[], pl0a_special_20[],
	pl0a_special_20_loop[], pl0a_special_21[], pl0a_special_21_loop[], pl0a_special_22[],
	pl0a_special_22_loop[], pl0a_special_23[], pl0a_special_23_loop[], pl0a_special_28[],
	pl0a_special_28_loop[], pl0a_special_30[], pl0a_special_30_loop[], pl0a_special_24[],
	pl0a_special_24_loop[], pl0a_special_25[], pl0a_special_25_loop[], pl0a_special_26[],
	pl0a_special_27[], pl0a_special_31[], pl0a_special_31_loop[], pl0a_special_32[],
	pl0a_special_33[], pl0a_special_33_loop[], pl0a_special_34[], pl0a_special_35[],
	pl0a_special_35_loop[], pl0a_special_36[], pl0a_special_48[], pl0a_special_48_loop[],
	pl0a_special_49[], pl0a_special_38[], pl0a_special_37[], pl0a_special_39[], pl0a_special_41[],
	pl0a_special_40[], pl0a_special_53[], pl0a_special_42[], pl0a_special_42_loop[],
	pl0a_special_43[], pl0a_special_43_loop[], pl0a_special_44[], pl0a_special_44_loop[],
	pl0a_special_45[], pl0a_special_46[], pl0a_special_46_loop[], pl0a_special_47[], pl0a_super_00[],
	pl0a_super_00_loop[], pl0a_super_01[], pl0a_super_01_loop[], pl0a_super_02[],
	pl0a_super_02_loop[], pl0a_super_03[], pl0a_super_04[], pl0a_super_05[], pl0a_super_05_loop[],
	pl0a_super_06[], pl0a_super_07[], pl0a_super_10[], pl0a_super_11[], pl0a_super_12[],
	pl0a_super_13[], pl0a_super_15[], pl0a_super_15_loop[], pl0a_grp27_00[], pl0a_grp27_02[],
	pl0a_grp27_03[], pl0a_grp27_04[], pl0a_grp27_05[], pl0a_grp27_13[], pl0a_grp27_14[],
	pl0a_grp27_15[], pl0a_grp27_15_loop[], pl0a_grp27_16[], pl0a_grp27_16_loop[], pl0a_grp27_17[],
	pl0a_effect_00[], pl0a_grp27_19[], pl0a_effect_03[], pl0a_effect_04[], pl0a_effect_05[],
	pl0a_effect_06[], pl0a_effect_08[], pl0a_effect_09[], pl0a_effect_11[], pl0a_effect_12[],
	pl0a_effect_01[], pl0a_effect_02[], pl0a_extra_02[], pl0a_grp25_02[], pl0a_grp25_03[],
	pl0a_change_03[], pl0a_change_03_loop[], pl0a_change_04[], pl0a_change_04_loop[],
	pl0a_grp27_27[], pl0a_stand_p_03[], pl0a_stand_p_03_loop[], pl0a_stand_p_04[],
	pl0a_stand_p_04_loop[], pl0a_grp28_00[], pl0a_grp28_01[], pl0a_grp28_02[], pl0a_grp28_03[],
	pl0a_grp28_04[], pl0a_grp28_05[], pl0a_grp28_06[], pl0a_grp28_07[], pl0a_grp28_08[],
	pl0a_grp28_09[], pl0a_grp28_10[], pl0a_grp28_11[], pl0a_grp28_12[], pl0a_grp28_13[],
	pl0a_grp28_14[];
extern const Sint32 *const pl0a_stand_tbl[], *const pl0a_jump_tbl[], *const pl0a_dash_tbl[],
	*const pl0a_crouch_tbl[], *const pl0a_turn_tbl[], *const pl0a_guard_tbl[],
	*const pl0a_air_guard_tbl[], *const pl0a_stand_p_tbl[], *const pl0a_stand_k_tbl[],
	*const pl0a_crouch_p_tbl[], *const pl0a_crouch_k_tbl[], *const pl0a_jump_p_tbl[],
	*const pl0a_jump_k_tbl[], *const pl0a_damage_tbl[], *const pl0a_dizzy_tbl[],
	*const pl0a_catch_tbl[], *const pl0a_caught_tbl[], *const pl0a_get_up_tbl[],
	*const pl0a_appear_tbl[], *const pl0a_win_tbl[], *const pl0a_extra_tbl[],
	*const pl0a_special_tbl[], *const pl0a_super_tbl[], *const pl0a_effect_tbl[],
	*const pl0a_change_tbl[], *const pl0a_grp25_tbl[], *const pl0a_grp27_tbl[],
	*const pl0a_grp28_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_0a[16] = {
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
const Sint32 *const *const char_tbl_0a[] = {
	pl0a_stand_tbl, pl0a_jump_tbl, pl0a_dash_tbl, pl0a_crouch_tbl, pl0a_turn_tbl, pl0a_guard_tbl,
	pl0a_air_guard_tbl, pl0a_stand_p_tbl, pl0a_stand_k_tbl, pl0a_crouch_p_tbl, pl0a_crouch_k_tbl, pl0a_jump_p_tbl,
	pl0a_jump_k_tbl, pl0a_damage_tbl, pl0a_dizzy_tbl, pl0a_catch_tbl, pl0a_caught_tbl, pl0a_get_up_tbl,
	pl0a_appear_tbl, pl0a_win_tbl, pl0a_extra_tbl, pl0a_special_tbl, pl0a_super_tbl, pl0a_effect_tbl,
	pl0a_change_tbl, pl0a_grp25_tbl, pl0a_grp25_tbl, pl0a_grp27_tbl, pl0a_grp28_tbl, 0,
};

const Sint32 *const pl0a_stand_tbl[] = {
	pl0a_stand_00, pl0a_stand_01, pl0a_stand_02, pl0a_stand_03, pl0a_stand_00, pl0a_stand_00,
	pl0a_stand_06, pl0a_stand_07, pl0a_stand_08, pl0a_stand_09, pl0a_stand_10, pl0a_stand_11,
	pl0a_stand_02, pl0a_stand_13, pl0a_stand_14, pl0a_stand_15, pl0a_stand_16, pl0a_stand_17,
	pl0a_stand_18, pl0a_stand_19, pl0a_stand_20, pl0a_stand_21, pl0a_stand_22, pl0a_stand_23,
	pl0a_stand_24, pl0a_stand_25, pl0a_stand_26, pl0a_stand_27, pl0a_stand_28, pl0a_stand_29,
	pl0a_stand_30, pl0a_stand_31, pl0a_stand_03, pl0a_stand_33, pl0a_stand_34, pl0a_stand_35,
	pl0a_stand_36, pl0a_stand_37, pl0a_stand_38, pl0a_stand_39, pl0a_stand_40, pl0a_stand_41,
	pl0a_stand_42, pl0a_stand_43, pl0a_stand_44, pl0a_stand_45, pl0a_stand_46, pl0a_stand_47,
	pl0a_stand_48, pl0a_stand_49, pl0a_stand_50, pl0a_stand_51, pl0a_stand_00, pl0a_stand_53,
	pl0a_stand_54, pl0a_stand_55, pl0a_stand_56, pl0a_stand_57, pl0a_stand_58, pl0a_stand_59,
	pl0a_stand_60, pl0a_stand_61, 0,
};

const Sint32 *const pl0a_jump_tbl[] = {
	pl0a_jump_00, pl0a_jump_01, pl0a_jump_02, pl0a_jump_03, pl0a_jump_03, pl0a_jump_03,
	pl0a_jump_06, pl0a_jump_07, pl0a_jump_08, pl0a_jump_09, pl0a_jump_09, pl0a_jump_09,
	0,
};

const Sint32 *const pl0a_dash_tbl[] = {
	pl0a_dash_00, pl0a_dash_01, pl0a_dash_02, pl0a_dash_03, pl0a_dash_04, 0,
};

const Sint32 *const pl0a_crouch_tbl[] = {
	pl0a_crouch_00, pl0a_crouch_01, pl0a_crouch_02, 0,
};

const Sint32 *const pl0a_turn_tbl[] = {
	pl0a_turn_00, pl0a_turn_01, 0,
};

const Sint32 *const pl0a_guard_tbl[] = {
	pl0a_guard_00, pl0a_guard_01, pl0a_guard_02, pl0a_guard_03, pl0a_guard_04, pl0a_guard_05,
	pl0a_guard_06, pl0a_guard_07, 0,
};

const Sint32 *const pl0a_air_guard_tbl[] = {
	pl0a_air_guard_00, pl0a_air_guard_01, pl0a_air_guard_02, 0,
};

const Sint32 *const pl0a_stand_p_tbl[] = {
	pl0a_stand_p_00, pl0a_stand_p_01, pl0a_stand_p_02, pl0a_stand_p_03, pl0a_stand_p_04, 0,
};

const Sint32 *const pl0a_stand_k_tbl[] = {
	pl0a_stand_k_00, pl0a_stand_k_01, pl0a_stand_k_02, pl0a_stand_k_03, 0,
};

const Sint32 *const pl0a_crouch_p_tbl[] = {
	pl0a_crouch_p_00, pl0a_crouch_p_01, pl0a_crouch_p_02, 0,
};

const Sint32 *const pl0a_crouch_k_tbl[] = {
	pl0a_crouch_k_00, pl0a_crouch_k_01, pl0a_crouch_k_02, 0,
};

const Sint32 *const pl0a_jump_p_tbl[] = {
	pl0a_jump_p_00, pl0a_jump_p_01, pl0a_jump_p_02, pl0a_jump_p_00, pl0a_jump_p_01, pl0a_jump_p_02,
	0,
};

const Sint32 *const pl0a_jump_k_tbl[] = {
	pl0a_jump_k_00, pl0a_jump_k_01, pl0a_jump_k_02, pl0a_jump_k_00, pl0a_jump_k_01, pl0a_jump_k_05,
	0,
};

const Sint32 *const pl0a_damage_tbl[] = {
	pl0a_damage_00, pl0a_damage_01, pl0a_damage_02, pl0a_damage_03, pl0a_damage_04, pl0a_damage_05,
	pl0a_damage_06, pl0a_damage_07, pl0a_damage_08, pl0a_damage_09, pl0a_damage_10, pl0a_damage_11,
	pl0a_damage_12, pl0a_damage_13, pl0a_damage_13, pl0a_damage_13, pl0a_damage_16, pl0a_damage_17,
	pl0a_damage_18, pl0a_damage_19, pl0a_damage_20, pl0a_damage_21, pl0a_damage_22, pl0a_damage_23,
	pl0a_damage_24, pl0a_damage_25, pl0a_damage_26, pl0a_damage_27, pl0a_damage_28, pl0a_damage_29,
	pl0a_damage_27, pl0a_damage_31, pl0a_damage_32, pl0a_damage_27, pl0a_damage_34, 0,
};

const Sint32 *const pl0a_dizzy_tbl[] = {
	pl0a_dizzy_00, 0,
};

const Sint32 *const pl0a_catch_tbl[] = {
	pl0a_catch_00, pl0a_catch_01, pl0a_catch_02, pl0a_catch_03, pl0a_catch_04, pl0a_catch_05,
	pl0a_catch_06, pl0a_catch_07, pl0a_catch_08, pl0a_catch_09, pl0a_catch_10, 0,
};

const Sint32 *const pl0a_caught_tbl[] = {
	pl0a_caught_00, pl0a_caught_01, pl0a_caught_02, pl0a_caught_03, pl0a_caught_04, pl0a_caught_05,
	pl0a_caught_06, pl0a_caught_07, pl0a_caught_08, pl0a_caught_09, pl0a_caught_10, pl0a_caught_11,
	pl0a_caught_12, pl0a_caught_13, pl0a_caught_14, pl0a_caught_15, pl0a_caught_16, pl0a_caught_17,
	pl0a_caught_18, pl0a_caught_19, pl0a_caught_20, pl0a_caught_21, 0,
};

const Sint32 *const pl0a_get_up_tbl[] = {
	pl0a_get_up_00, pl0a_get_up_01, pl0a_get_up_02, pl0a_get_up_03, 0,
};

const Sint32 *const pl0a_appear_tbl[] = {
	pl0a_appear_00, pl0a_appear_01, 0,
};

const Sint32 *const pl0a_win_tbl[] = {
	pl0a_win_00, pl0a_win_01, pl0a_win_02, pl0a_win_03, pl0a_win_04, pl0a_win_05,
	pl0a_win_06, pl0a_win_07, pl0a_win_08, pl0a_win_09, 0,
};

const Sint32 *const pl0a_extra_tbl[] = {
	pl0a_extra_00, pl0a_extra_01, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02,
	pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02,
	pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02,
	pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_02, pl0a_extra_22, pl0a_extra_23,
	pl0a_extra_24, 0,
};

const Sint32 *const pl0a_special_tbl[] = {
	pl0a_special_00, pl0a_special_01, pl0a_special_02, pl0a_special_03, pl0a_special_04, pl0a_special_05,
	pl0a_special_06, pl0a_special_07, pl0a_special_08, pl0a_special_09, pl0a_special_09, pl0a_special_09,
	pl0a_special_12, pl0a_special_13, pl0a_special_14, pl0a_special_15, pl0a_special_16, pl0a_special_17,
	pl0a_special_18, pl0a_special_19, pl0a_special_20, pl0a_special_21, pl0a_special_22, pl0a_special_23,
	pl0a_special_24, pl0a_special_25, pl0a_special_26, pl0a_special_27, pl0a_special_28, pl0a_special_26,
	pl0a_special_30, pl0a_special_31, pl0a_special_32, pl0a_special_33, pl0a_special_34, pl0a_special_35,
	pl0a_special_36, pl0a_special_37, pl0a_special_38, pl0a_special_39, pl0a_special_40, pl0a_special_41,
	pl0a_special_42, pl0a_special_43, pl0a_special_44, pl0a_special_45, pl0a_special_46, pl0a_special_47,
	pl0a_special_48, pl0a_special_49, pl0a_special_06, pl0a_special_51, pl0a_special_52, pl0a_special_53,
	0,
};

const Sint32 *const pl0a_super_tbl[] = {
	pl0a_super_00, pl0a_super_01, pl0a_super_02, pl0a_super_03, pl0a_super_04, pl0a_super_05,
	pl0a_super_06, pl0a_super_07, pl0a_super_08, pl0a_super_08, pl0a_super_10, pl0a_super_11,
	pl0a_super_12, pl0a_super_13, pl0a_super_13, pl0a_super_15, 0,
};

const Sint32 *const pl0a_effect_tbl[] = {
	pl0a_effect_00, pl0a_effect_01, pl0a_effect_02, pl0a_effect_03, pl0a_effect_04, pl0a_effect_05,
	pl0a_effect_06, pl0a_effect_03, pl0a_effect_08, pl0a_effect_09, pl0a_effect_06, pl0a_effect_11,
	pl0a_effect_12, 0,
};

const Sint32 *const pl0a_change_tbl[] = {
	pl0a_change_00, pl0a_change_01, pl0a_change_02, pl0a_change_03, pl0a_change_04, 0,
};

const Sint32 *const pl0a_grp25_tbl[] = {
	pl0a_stand_00, pl0a_extra_02, pl0a_grp25_02, pl0a_grp25_03, 0,
};

const Sint32 *const pl0a_grp27_tbl[] = {
	pl0a_grp27_00, pl0a_grp27_00, pl0a_grp27_02, pl0a_grp27_03, pl0a_grp27_04, pl0a_grp27_05,
	pl0a_grp27_05, pl0a_grp27_05, pl0a_grp27_05, pl0a_grp27_05, pl0a_grp27_05, pl0a_grp27_05,
	pl0a_grp27_05, pl0a_grp27_13, pl0a_grp27_14, pl0a_grp27_15, pl0a_grp27_16, pl0a_grp27_17,
	pl0a_grp27_00, pl0a_grp27_19, pl0a_grp27_00, pl0a_grp27_00, pl0a_grp27_00, pl0a_grp27_00,
	pl0a_grp27_00, pl0a_grp27_00, pl0a_grp27_19, pl0a_grp27_27, 0,
};

const Sint32 *const pl0a_grp28_tbl[] = {
	pl0a_grp28_00, pl0a_grp28_01, pl0a_grp28_02, pl0a_grp28_03, pl0a_grp28_04, pl0a_grp28_05,
	pl0a_grp28_06, pl0a_grp28_07, pl0a_grp28_08, pl0a_grp28_09, pl0a_grp28_10, pl0a_grp28_11,
	pl0a_grp28_12, pl0a_grp28_13, pl0a_grp28_14, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 4
   Char_Stand 5
   Char_Stand 52
   group 25 0
   Char_Fly 0 */
const Sint32 pl0a_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    15, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 53 */
const Sint32 pl0a_stand_53[] = {
	CG_FRAME(    0,    0,   4,           0,    16, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 54 */
const Sint32 pl0a_stand_54[] = {
	CG_FRAME(    0,    0,   4,           0,    17, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 55 */
const Sint32 pl0a_stand_55[] = {
	CG_FRAME(    0,    0,   4,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 56 */
const Sint32 pl0a_stand_56[] = {
	CG_FRAME(    0,    0,   4,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 57 */
const Sint32 pl0a_stand_57[] = {
	CG_FRAME(    0,    0,   4,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 58 */
const Sint32 pl0a_stand_58[] = {
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 59 */
const Sint32 pl0a_stand_59[] = {
	CG_FRAME(    0,    0,   4,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 60 */
const Sint32 pl0a_stand_60[] = {
	CG_FRAME(    0,    0,   4,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 61 */
const Sint32 pl0a_stand_61[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_00 ),
};

/* Char_Stand 10 */
const Sint32 pl0a_stand_10[] = {
	CG_FRAME(    0,    0,   4,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_00 ),
};

/* Char_Stand 11 */
const Sint32 pl0a_stand_11[] = {
	CG_FRAME(    0,    0,   4,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    28, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    29, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    30, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    31, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    32, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    33, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_00 ),
};

/* Char_Stand 1 */
const Sint32 pl0a_stand_01[] = {
	CG_FRAME(    0,    0,   8,           0,    15, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7400,   1 ),
	CG_FRAME(    0,    0,   8,           0,    16, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7500,   1 ),
	CG_FRAME(    0,    0,   8,           0,    17, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7600,   1 ),
	CG_FRAME(    0,    0,   8,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7700,   1 ),
	CG_FRAME(    0,    0,   8,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7800,   1 ),
	CG_FRAME(    0,    0,   8,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7900,   1 ),
	CG_FRAME(    0,    0,   8,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7a00,   1 ),
	CG_FRAME(    0,    0,   8,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7b00,   1 ),
	CG_FRAME(    0,    0,   8,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7c00,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7d00,   1 ),
	CG_NEXT( pl0a_stand_01 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 12 */
const Sint32 pl0a_stand_02[] = {
	CG_FRAME(    0,    0,   3,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 13 */
const Sint32 pl0a_stand_13[] = {
	CG_FRAME(    0,    0,   3,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 14 */
const Sint32 pl0a_stand_14[] = {
	CG_FRAME(    0,    0,   3,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 15 */
const Sint32 pl0a_stand_15[] = {
	CG_FRAME(    0,    0,   3,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 16 */
const Sint32 pl0a_stand_16[] = {
	CG_FRAME(    0,    0,   3,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 17 */
const Sint32 pl0a_stand_17[] = {
	CG_FRAME(    0,    0,   3,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 18 */
const Sint32 pl0a_stand_18[] = {
	CG_FRAME(    0,    0,   3,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 19 */
const Sint32 pl0a_stand_19[] = {
	CG_FRAME(    0,    0,   3,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 20 */
const Sint32 pl0a_stand_20[] = {
	CG_FRAME(    0,    0,   3,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 21 */
const Sint32 pl0a_stand_21[] = {
	CG_FRAME(    0,    0,   3,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 22 */
const Sint32 pl0a_stand_22[] = {
	CG_FRAME(    0,    0,   3,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 23 */
const Sint32 pl0a_stand_23[] = {
	CG_FRAME(    0,    0,   3,           0,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 24 */
const Sint32 pl0a_stand_24[] = {
	CG_FRAME(    0,    0,   3,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 25 */
const Sint32 pl0a_stand_25[] = {
	CG_FRAME(    0,    0,   3,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 26 */
const Sint32 pl0a_stand_26[] = {
	CG_FRAME(    0,    0,   3,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 27 */
const Sint32 pl0a_stand_27[] = {
	CG_FRAME(    0,    0,   3,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 28 */
const Sint32 pl0a_stand_28[] = {
	CG_FRAME(    0,    0,   3,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 29 */
const Sint32 pl0a_stand_29[] = {
	CG_FRAME(    0,    0,   3,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 30 */
const Sint32 pl0a_stand_30[] = {
	CG_FRAME(    0,    0,   3,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 31 */
const Sint32 pl0a_stand_31[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_02 ),
};

/* Char_Stand 8 */
const Sint32 pl0a_stand_08[] = {
	CG_FRAME(    0,    0,   3,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x4c00,   1 ),
	CG_FRAME(    0,    1,   8,           0,    37, 0x80,   0x00,                0,  0x01, 0x2f | CSE,    0,   0,     0, 0x4d00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x4e00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x4f00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5000,   1 ),
	CG_FRAME(    0,    0,   3,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5100,   1 ),
	CG_FRAME(    0,    0,   3,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5200,   1 ),
	CG_FRAME(    0,    0,   3,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5300,   1 ),
	CG_FRAME(    0,    0,   3,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5400,   1 ),
	CG_FRAME(    0,    0,   3,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5500,   1 ),
	CG_FRAME(    0,    0,   3,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5600,   1 ),
	CG_FRAME(    0,    0,   3,           0,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5700,   1 ),
	CG_FRAME(    0,    1,   8,           0,    48, 0x80,   0x00,                0,  0x01, 0x2f | CSE,    0,   0,     0, 0x5800,   1 ),
	CG_FRAME(    0,    0,   3,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5900,   1 ),
	CG_FRAME(    0,    0,   3,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5a00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5b00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5c00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5d00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5e00,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x5f00,   1 ),
	CG_NEXT( pl0a_stand_08 ),
};

/* Char_Stand 3
   Char_Stand 32 */
const Sint32 pl0a_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 33 */
const Sint32 pl0a_stand_33[] = {
	CG_FRAME(    0,    0,   3,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 34 */
const Sint32 pl0a_stand_34[] = {
	CG_FRAME(    0,    0,   3,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 35 */
const Sint32 pl0a_stand_35[] = {
	CG_FRAME(    0,    0,   3,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 36 */
const Sint32 pl0a_stand_36[] = {
	CG_FRAME(    0,    0,   3,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 37 */
const Sint32 pl0a_stand_37[] = {
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 38 */
const Sint32 pl0a_stand_38[] = {
	CG_FRAME(    0,    0,   3,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 39 */
const Sint32 pl0a_stand_39[] = {
	CG_FRAME(    0,    0,   3,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 40 */
const Sint32 pl0a_stand_40[] = {
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 41 */
const Sint32 pl0a_stand_41[] = {
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 42 */
const Sint32 pl0a_stand_42[] = {
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 43 */
const Sint32 pl0a_stand_43[] = {
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 44 */
const Sint32 pl0a_stand_44[] = {
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 45 */
const Sint32 pl0a_stand_45[] = {
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 46 */
const Sint32 pl0a_stand_46[] = {
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 47 */
const Sint32 pl0a_stand_47[] = {
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 48 */
const Sint32 pl0a_stand_48[] = {
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 49 */
const Sint32 pl0a_stand_49[] = {
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 50 */
const Sint32 pl0a_stand_50[] = {
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 51 */
const Sint32 pl0a_stand_51[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_03 ),
};

/* Char_Stand 9 */
const Sint32 pl0a_stand_09[] = {
	CG_FRAME(    0,    0,   3,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6000,   1 ),
	CG_FRAME(    0,    0,   3,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6100,   1 ),
	CG_FRAME(    0,    0,   3,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6200,   1 ),
	CG_FRAME(    0,    0,   3,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6300,   1 ),
	CG_FRAME(    0,    0,   3,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6400,   1 ),
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6500,   1 ),
	CG_FRAME(    0,    0,   3,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6600,   1 ),
	CG_FRAME(    0,    1,   8,           0,    63, 0x80,   0x00,                0,  0x01, 0x2f | CSE,    0,   0,     0, 0x6700,   1 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6800,   1 ),
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6900,   1 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6a00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6b00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6c00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6d00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6e00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x6f00,   1 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7000,   1 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7100,   1 ),
	CG_FRAME(    0,    1,   8,           0,    74, 0x80,   0x00,                0,  0x01, 0x2f | CSE,    0,   0,     0, 0x7200,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x7300,   1 ),
	CG_NEXT( pl0a_stand_09 ),
};

/* Char_Stand 6 */
const Sint32 pl0a_stand_06[] = {
	CG_FRAME(    0,    0,   5,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    57, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl0a_stand_07[] = {
	CG_FRAME(    0,    0,   5,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    74, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl0a_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
};

const Sint32 pl0a_jump_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl0a_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl0a_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
};

const Sint32 pl0a_jump_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl0a_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl0a_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
};

const Sint32 pl0a_jump_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   121, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl0a_jump_02_loop ),
};

/* Char_Jump 3: jump_dive_fall
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl0a_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl0a_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: drain_move (Power Drain), act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl0a_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl0a_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_jump_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl0a_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_jump_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl0a_dash_00[] = {
	CG_FRAME(    0,    1,   2,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2000,   1 ),
};

const Sint32 pl0a_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2100,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2200,   1 ),
	CG_NEXT( pl0a_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl0a_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2300,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2400,   1 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2500,   1 ),
	CG_FRAME(    0,    1,   3,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    1,   3,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2700,   1 ),
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_dash_02 ),
};

/* Char_Dash 4: fly_start_init, fly_start2_init */
const Sint32 pl0a_dash_04[] = {
	CG_FRAME(    0,    1,   7,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2000,   1 ),
};

const Sint32 pl0a_dash_04_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2100,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2200,   1 ),
	CG_NEXT( pl0a_dash_04_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl0a_dash_01[] = {
	CG_FRAME(    0,    1,   2,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_dash_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl0a_dash_03[] = {
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl0a_crouch_00[] = {
	CG_FRAME(    0,    3,   3,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    2,   3,           0,   123, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   3,           0,   124, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   124, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl0a_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    35, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl0a_crouch_02[] = {
	CG_FRAME(    0,    3,   3,           0,   124, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,   3,           0,   123, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    16, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl0a_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl0a_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl0a_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl0a_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl0a_guard_02[] = {
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl0a_guard_03[] = {
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_guard_03_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_guard_03_loop ),
};

/* Char_Guard 4 */
const Sint32 pl0a_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl0a_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   135, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl0a_guard_06[] = {
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   124, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl0a_guard_07[] = {
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl0a_guard_07_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   134, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_guard_07_loop ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl0a_air_guard_00[] = {
	CG_FRAME(    0,    0,   1,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl0a_air_guard_02[] = {
	CG_FRAME(    0,    0,   2,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl0a_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl0a_stand_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_p_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_p_00_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl0a_stand_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   3,           0,   234, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,   2,           0,   235, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,  10,           0,   236, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   237, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
};

const Sint32 pl0a_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_p_01_loop ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl0a_stand_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   242, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,   2,           0,   243, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   244, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   245, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x82,    0,  12,           0,   246, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_p_02_loop ),
};

/* Char_Super 8
   Char_Super 9: snapback_init (Snapback) */
const Sint32 pl0a_super_08[] = {
	CG_FRAME(    0,    0,   8,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   239, 0x80,   0x00,              ATK,  0x00,       0x0a,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   245, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x82,    0,  12,           0,   246, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_super_08_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_super_08_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl0a_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   250, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_k_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_k_00_loop ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl0a_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   255, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x82,    0,   3,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   8,           0,   252, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_k_01_loop ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl0a_stand_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   261, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(   -3, 0x80,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0, 0x0100,   4 ),
	CG_FRAME(   -8, 0x80,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0200,   4 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0300,   4 ),
	CG_FRAME(    0,    4,   3,           0,   265, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0400,   4 ),
	CG_FRAME(    0,    8,   6,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   16,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_k_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_k_02_loop ),
};

/* Char_Stand_K 3 */
const Sint32 pl0a_stand_k_03[] = {
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x82,    0,   2,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    4,   3,           0,   265, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    8,   6,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   16,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_k_03_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_k_03_loop ),
};

/* Char_Extra 23: pl0a_upper */
const Sint32 pl0a_extra_23[] = {
	CG_FRAME(    0,    0,   6,           0,   261, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(   -3, 0x80,   3,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0, 0x0100, 119 ),
	CG_FRAME(   -8, 0x80,   1,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0200,   4 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0300,   4 ),
	CG_FRAME(    8,    0,   3,           0,   265, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0400,   4 ),
	CG_FRAME(    8,    0,   6,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(   48,    0,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_extra_23_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_extra_23_loop ),
};

/* Char_Extra 24 */
const Sint32 pl0a_extra_24[] = {
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x82,    0,   2,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    8,    0,   3,           0,   265, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    8,    0,   6,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(   48,    0,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_extra_24_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_extra_24_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl0a_crouch_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x40,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   274, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0a_crouch_p_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0a_crouch_p_00_loop ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl0a_crouch_p_01[] = {
	CG_FRAME(    0,    0,   7,           0,   277, 0x80,   0x40,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   278, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  10,           0,   279, 0x80,   0x40,              ATK,  0x00,       0x04,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   2,           0,   282, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0a_crouch_p_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0a_crouch_p_01_loop ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl0a_crouch_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x40,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   284, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   285, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   2,           0,   286, 0x80,   0x40,              ATK,  0x00,       0x03,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   8,           0,   288, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,   123, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0a_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0a_crouch_p_02_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl0a_crouch_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x40,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   6,           0,   294, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6,           0,   295, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0a_crouch_k_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0a_crouch_k_00_loop ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl0a_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   124, 0x80,   0x40,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x40,              ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   4,           0,   298, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x82,    0,  12,           0,   299, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0a_crouch_k_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0a_crouch_k_01_loop ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl0a_crouch_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x40,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   302, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x40,              ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   3,           0,   304, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x82,    0,  16,           0,   305, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,  10,           0,   306, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   8,           0,   307, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   308, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl0a_crouch_k_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0a_crouch_k_02_loop ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl0a_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   309, 0x80,   0x20,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   310, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   311, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
};

const Sint32 pl0a_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   312, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl0a_jump_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   313, 0x80,   0x20,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   314, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x80,    0,   1,           0,   315, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   316, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
};

const Sint32 pl0a_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   317, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl0a_jump_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   318, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   319, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   320, 0x80,   0x20,              ATK,  0x00,       0x03,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   321, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   2,           0,   325, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl0a_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   324, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl0a_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   309, 0x80,   0x20,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   326, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  10,           0,   327, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
};

const Sint32 pl0a_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   328, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl0a_jump_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   309, 0x80,   0x20,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   329, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   330, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x82,    0,   4,           0,   331, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   6,           0,   332, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl0a_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   328, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl0a_jump_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   334, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   336, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   337, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x82,    0,   2,           0,   338, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6,           0,   339, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl0a_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   340, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_jump_k_02_loop ),
};

/* Char_Jump_K 5: jump_atck */
const Sint32 pl0a_jump_k_05[] = {
	CG_FRAME(    0,    0,   3,           0,   333, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   334, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   335, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   3,           0,   341, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0,      0,   6 ),
};

const Sint32 pl0a_jump_k_05_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   342, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   343, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl0a_jump_k_05_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl0a_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0xa0,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x0a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl0a_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x0a,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl0a_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x0a,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl0a_get_up_03[] = {
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x0a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl0a_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl0a_damage_01[] = {
	CG_FRAME(    0,    0,   8,           0,   551, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl0a_damage_02[] = {
	CG_FRAME( 0x80,    0,   2,           0,   555, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   4,           0,   556, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   557, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   559, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   560, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl0a_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   564, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl0a_damage_04[] = {
	CG_FRAME(    0,    0,   8,           0,   565, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   564, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl0a_damage_05[] = {
	CG_FRAME( 0x80,    0,   2,           0,   568, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,  10,           0,   569, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   570, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl0a_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   572, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   571, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    35, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl0a_damage_07[] = {
	CG_FRAME(    0,    0,   8,           0,   573, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   572, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   571, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    35, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl0a_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0,   579, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   580, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   581, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   582, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   573, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   572, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   571, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    35, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl0a_damage_10[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   583, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl0a_damage_11[] = {
	CG_FRAME(    0,    0,   2,           0,   566, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   566, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   567, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl0a_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   588, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   589, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   590, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl0a_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   591, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   593, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   594, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   596, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0a_damage_13 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl0a_damage_34[] = {
	CG_FRAME(    0,    0,   6,           0,   591, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   594, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0a_damage_34 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl0a_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   598, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   608, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x6c,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   614, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   616, 0x80,   0x8b,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   615, 0x80,   0x8b,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
};

const Sint32 pl0a_damage_09_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   616, 0x80,   0x8b,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0a_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl0a_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   601, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl0a_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   602, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl0a_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl0a_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl0a_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   609, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl0a_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   601, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl0a_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl0a_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl0a_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl0a_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   608, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl0a_damage_26[] = {
	CG_FRAME(    0,    0,   1,           0,   610, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   613, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   614, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   615, 0x80,   0xab,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   594, 0x80,   0xab,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   616, 0x80,   0xab,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0,   615, 0x80,   0xab,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   616, 0x80,   0xab,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0a_damage_26 ),
};

const Sint32 pl0a_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   610, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_27 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 33: act_escape */
const Sint32 pl0a_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_27 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl0a_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl0a_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   618, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   619, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   623, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl0a_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   624, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   625, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   626, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   627, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   628, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   629, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl0a_damage_31[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   596, 0x80,   0xcb,                0,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl0a_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl0a_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   647, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   646, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   645, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   644, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   643, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   642, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   643, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   644, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   645, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   646, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_dizzy_00 ),
};

/* Char_Catch 0: sugah_rise (Goodnight Sugah (hyper)), drain_set */
const Sint32 pl0a_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   4,           0,   515, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,   4,           0,   516, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   517, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   4,           0,   518, 0x80,   0x00,                0,  0x00,       0x1e,    0,   0,     0, 0x0007,   0 ),
};

const Sint32 pl0a_catch_00_loop[] = {
	CG_FRAME(    1,    0,   4,           0,   519, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    1,    0,   4,           0,   520, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA,   521, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_NEXT( pl0a_catch_00_loop ),
};

/* Char_Catch 1: sugah_drain (Goodnight Sugah (hyper)), drain_main */
const Sint32 pl0a_catch_01[] = {
	CG_FRAME(    0,    0,   1,           0,   522, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   5,           0,   522, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   6,           0,   523, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    1,   6,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0a_catch_01_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_catch_01_loop ),
};

/* Char_Catch 2: sugah_fall (Goodnight Sugah (hyper)), drain_fall */
const Sint32 pl0a_catch_02[] = {
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_catch_02 ),
};

/* Char_Catch 3: screw_catch_set */
const Sint32 pl0a_catch_03[] = {
	CG_FRAME(    0,    0,   3,           0,   403, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,  69 ),
	CG_FRAME(    0,    0,   3,           0,   404, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0, 0x000f,  69 ),
	CG_FRAME(    0,    1,   3,           0,   405, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   3,           0,   406, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   407, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_NEXT( pl0a_catch_03 ),
};

/* Char_Catch 4: screw_throw_hold */
const Sint32 pl0a_catch_04[] = {
	CG_FRAME(    0,    0,   2,           0,   408, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   412, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_NEXT( pl0a_catch_04 ),
};

/* Char_Catch 5: screw_throw_jump */
const Sint32 pl0a_catch_05[] = {
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   416, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_NEXT( pl0a_catch_05 ),
};

/* Char_Catch 6: screw_throw_fly */
const Sint32 pl0a_catch_06[] = {
	CG_FRAME(    0,    0,  12,           0,   417, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    1,   1,           0,   417, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    1,    1,   4, END_OF_DATA,   418, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001b,   0 ),
	CG_NEXT( pl0a_catch_06 ),
};

/* Char_Catch 7: nml_catch_set */
const Sint32 pl0a_catch_07[] = {
	CG_FRAME(    0,    0,   4,           0,   507, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    1,   4,           0,   506, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   507, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   507, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0, 0x0026,   0 ),
	CG_FRAME(    0,    0,   4,           0,   508, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
};

const Sint32 pl0a_catch_07_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   508, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   509, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
	CG_NEXT( pl0a_catch_07_loop ),
};

/* Char_Catch 8: nml_catch_rise */
const Sint32 pl0a_catch_08[] = {
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0021,   0 ),
	CG_FRAME(    0,    0,   2,           0,   318, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0022,   6 ),
	CG_FRAME(    0,    0,   2,           0,   319, 0x80,   0x00,              ATK,  0x6e,       0x03,    0,   0,     0, 0x0022,   6 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0024, 114 ),
	CG_FRAME(    0,    1,   2,           0,   321, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   2,           0,   325, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   324, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_catch_08 ),
};

/* Char_Catch 9: shungoku_catch_set (Shun Goku Satsu) */
const Sint32 pl0a_catch_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   405, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_NEXT( pl0a_catch_09 ),
};

/* Char_Catch 10: shungoku_hold (Shun Goku Satsu) */
const Sint32 pl0a_catch_10[] = {
	CG_FRAME(    0,    0,   4,           0,   405, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
};

const Sint32 pl0a_catch_10_loop[] = {
	CG_FRAME(    0,    0,  64, END_OF_DATA,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0025,   0 ),
	CG_NEXT( pl0a_catch_10_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl0a_appear_00[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   178, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_appear_00 ),
};

/* Char_Appear 1: appear_fly */
const Sint32 pl0a_appear_01[] = {
	CG_FRAME(    0,    0,   8,           0,   165, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   166, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   167, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   173, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   176, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   177, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   181, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0a_appear_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_appear_01_loop ),
};

/* Char_Caught 0 */
const Sint32 pl0a_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl0a_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   553, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl0a_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   555, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl0a_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl0a_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   566, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl0a_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   568, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl0a_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   571, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0a_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl0a_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   574, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0a_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl0a_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   579, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0a_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl0a_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   583, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl0a_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   586, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl0a_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   588, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl0a_caught_12[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   591, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0a_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl0a_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl0a_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   602, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl0a_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl0a_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0a_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl0a_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   608, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl0a_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl0a_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   618, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl0a_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   624, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl0a_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   615, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl0a_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl0a_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl0a_caught_21 ),
};

/* Char_Win 0 */
const Sint32 pl0a_win_00[] = {
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   184, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   186, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   189, 0x80,   0x4d,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_win_00_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   192, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl0a_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x4d,                0,  0x00,       0x15,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0a_win_01_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   198, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl0a_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,   199, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   200, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   201, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   202, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   203, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   204, 0x80,   0x00,                0,  0x00,       0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   205, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   206, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   207, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   208, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   209, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   210, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   211, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   212, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0a_win_02_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   213, 0x80,   0x34,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_win_02_loop ),
};

/* Char_Win 3 */
const Sint32 pl0a_win_03[] = {
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_win_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   216, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   214, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   215, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_win_03_loop ),
};

/* Char_Win 4 */
const Sint32 pl0a_win_04[] = {
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_win_04_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_win_04_loop ),
};

/* Char_Win 5 */
const Sint32 pl0a_win_05[] = {
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_win_05_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   221, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_win_05_loop ),
};

/* Char_Win 6 */
const Sint32 pl0a_win_06[] = {
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_win_06_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   223, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_win_06_loop ),
};

/* Char_Win 7 */
const Sint32 pl0a_win_07[] = {
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_win_07_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_win_07_loop ),
};

/* Char_Win 8 */
const Sint32 pl0a_win_08[] = {
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_win_08_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_win_08_loop ),
};

/* Char_Win 9 */
const Sint32 pl0a_win_09[] = {
	CG_FRAME(    0,    0,   4,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   643, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   644, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   645, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   646, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_win_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   646, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_win_09_loop ),
};

/* Char_Extra 22: sp_chouhatsu (Taunt) */
const Sint32 pl0a_extra_22[] = {
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   184, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   186, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   189, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x00,              ATK,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_extra_22_loop[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,   192, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_extra_22_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl0a_extra_00[] = {
	CG_FRAME(    0,    0,   2,           0,   511, 0xa0,   0x00,              ATK,  0x30,          0,    0,   0,     0, 0x2800,  47 ),
};

const Sint32 pl0a_extra_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   511, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2800,  47 ),
	CG_NEXT( pl0a_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl0a_extra_01[] = {
	CG_FRAME(    0,    1,   3,           0,   124, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   124, 0x80,   0x00,              ATE,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   195, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   196, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_extra_01_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   198, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_extra_01_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl0a_change_00[] = {
	CG_FRAME(    0,    0,   2,           0,   184, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   186, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x30,   0x4d,              ATK,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   2,           0,   191, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0a_change_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   192, 0x30,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl0a_change_02[] = {
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl0a_change_02 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl0a_change_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   122, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   123, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   129, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0, 127, END_OF_DATA,   151, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_change_01 ),
};

/* Char_Special 0 */
const Sint32 pl0a_special_00[] = {
	CG_FRAME(    0,    1,   4,           0,    76, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,   4 ),
	CG_FRAME(    0,    0,   4,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x37,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x38,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   537, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   538, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0x80,   0x00,              ATK,  0x39,       0x05,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    1,   1,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8,           0,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8,           0,   288, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl0a_special_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_special_00_loop ),
};

/* Char_Special 3: repeat_fall (Repeating Punches, 3 + atck_level) */
const Sint32 pl0a_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_03_loop ),
};

/* Char_Special 1 */
const Sint32 pl0a_special_01[] = {
	CG_FRAME(    0,    1,   4,           0,    76, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,   4 ),
	CG_FRAME(    0,    0,   4,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x3a,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x3b,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x3c,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   537, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   538, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0x80,   0x00,              ATK,  0x3d,       0x05,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    1,   1,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8,           0,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8,           0,   288, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl0a_special_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_special_01_loop ),
};

/* Char_Special 4 */
const Sint32 pl0a_special_04[] = {
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_04_loop ),
};

/* Char_Special 2 */
const Sint32 pl0a_special_02[] = {
	CG_FRAME(    0,    1,   4,           0,    76, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,   4 ),
	CG_FRAME(    0,    0,   4,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x3e,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x3f,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x40,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x41,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   537, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   538, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0x80,   0x00,              ATK,  0x42,       0x05,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    1,   1,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8,           0,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8,           0,   288, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   289, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   290, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6,           0,   291, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl0a_special_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_special_02_loop ),
};

/* Char_Special 5 */
const Sint32 pl0a_special_05[] = {
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_05_loop ),
};

/* Char_Special 6
   Char_Special 50 */
const Sint32 pl0a_special_06[] = {
	CG_FRAME(    0,    1,   3,           0,   540, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   2,           0,   541, 0x80,   0x00,              ATK,  0x55,       0x04,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    1,   1,           0,   542, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   8,           0,   543, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    1,   1,           0,   545, 0x80,   0x00,              ATK,  0x56,       0x04,    0,   0,     0,      0,  77 ),
};

const Sint32 pl0a_special_06_loop[] = {
	CG_FRAME(    0,   -1,   8, END_OF_DATA,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl0a_special_06_loop ),
};

/* Char_Special 51 */
const Sint32 pl0a_special_51[] = {
	CG_FRAME(    0,    1,   5,           0,   540, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   2,           0,   541, 0x80,   0x00,              ATK,  0x55,       0x04,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    1,   1,           0,   542, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   8,           0,   543, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    1,   1,           0,   545, 0x80,   0x00,              ATK,  0x56,       0x04,    0,   0,     0,      0,  77 ),
};

const Sint32 pl0a_special_51_loop[] = {
	CG_FRAME(    0,   -1,   8, END_OF_DATA,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl0a_special_51_loop ),
};

/* Char_Special 52 */
const Sint32 pl0a_special_52[] = {
	CG_FRAME(    0,    1,  12,           0,   540, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   2,           0,   541, 0x80,   0x00,              ATK,  0x55,       0x04,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    1,   1,           0,   542, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   8,           0,   543, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    1,   1,           0,   545, 0x80,   0x00,              ATK,  0x56,       0x04,    0,   0,     0,      0,  77 ),
};

const Sint32 pl0a_special_52_loop[] = {
	CG_FRAME(    0,   -1,   8, END_OF_DATA,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl0a_special_52_loop ),
};

/* Char_Special 7: rising_move (Rising Repeating Punches, 7 + cg_prm) */
const Sint32 pl0a_special_07[] = {
	CG_FRAME( 0x80,    1,   1,           0,   542, 0x80,   0x00,              ATK,  0x74,       0x04,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   543, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl0a_special_07_loop[] = {
	CG_FRAME(    1,   -1,   6, END_OF_DATA,   543, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl0a_special_07_loop ),
};

/* Char_Special 8 */
const Sint32 pl0a_special_08[] = {
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    1,   1,           0,   545, 0x80,   0x00,              ATK,  0x74,       0x04,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl0a_special_08_loop[] = {
	CG_FRAME(    0,   -1,   6, END_OF_DATA,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl0a_special_08_loop ),
};

/* Char_Special 9
   Char_Special 10
   Char_Special 11 */
const Sint32 pl0a_special_09[] = {
	CG_FRAME(    0,    0,   2,           0,   548, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   319, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x00,              ATK,  0x57,       0x03,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   321, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   322, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   324, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,    97, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl0a_special_09_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   101, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_special_09_loop ),
};

/* Char_Special 12 */
const Sint32 pl0a_special_12[] = {
	CG_FRAME(    0,    0,   1,           0,   344, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   345, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   348, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   349, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   1,           0,   349, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_12_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_12_loop ),
};

/* Char_Special 13 */
const Sint32 pl0a_special_13[] = {
	CG_FRAME(    0,    0,   4,           0,   344, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   345, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   346, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   347, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   348, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   349, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   349, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   353, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_13_loop ),
};

/* Char_Special 14 */
const Sint32 pl0a_special_14[] = {
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   355, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   356, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   358, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   3,           0,   358, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   360, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   360, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   361, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
};

const Sint32 pl0a_special_14_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   362, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl0a_special_14_loop ),
};

/* Char_Special 15 */
const Sint32 pl0a_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    1,   1,           0,   366, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   367, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_special_15_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_special_15_loop ),
};

/* Char_Special 16 */
const Sint32 pl0a_special_16[] = {
	CG_FRAME(    0,    0,  24,           0,   370, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    1,   1,           0,   373, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   373, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_special_16_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_special_16_loop ),
};

/* Char_Special 17 */
const Sint32 pl0a_special_17[] = {
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   384, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   1,           0,   392, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   392, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,  16,           0,   394, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   395, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
};

const Sint32 pl0a_special_17_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   396, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_17_loop ),
};

/* Char_Special 18 */
const Sint32 pl0a_special_18[] = {
	CG_FRAME(    0,    1,   2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   476, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   477, 0x80,   0x00,              ATK,  0x4f,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,  14,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    1,   2,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   6,           0,   481, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_18_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_18_loop ),
};

/* Char_Special 19 */
const Sint32 pl0a_special_19[] = {
	CG_FRAME(    0,    1,   2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   476, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   477, 0x80,   0x00,              ATK,  0x50,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,  16,           0,   478, 0x80,   0x00,              ATK,  0x51,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    1,   2,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   6,           0,   481, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_19_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl0a_special_20[] = {
	CG_FRAME(    0,    1,   2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    1,   2,           0,   476, 0x80,   0x00,              ATK,  0x52,       0x05,    0,   0,     0, 0x0d00,  62 ),
	CG_FRAME(    0,    1,   4,           0,   477, 0x80,   0x00,              ATK,  0x53, 0x4b | CSE,    0,   0,     0, 0x0e00,  63 ),
	CG_FRAME(    0,    0,  32,           0,   478, 0x80,   0x00,              ATK,  0x54,          0,    0,   0,     0, 0x0f00,  64 ),
	CG_FRAME(    0,    0,   6,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    1,   2,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   6,           0,   481, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_20_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_20_loop ),
};

/* Char_Special 21 */
const Sint32 pl0a_special_21[] = {
	CG_FRAME(    0,    0,   8,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   1,           0,   422, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   422, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   8,           0,   423, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,  24,           0,   424, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl0a_special_21_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   425, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0a_special_21_loop ),
};

/* Char_Special 22: drain_init (Power Drain) */
const Sint32 pl0a_special_22[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2300,   4 ),
	CG_FRAME(    0,    1,   6,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2000, 115 ),
};

const Sint32 pl0a_special_22_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x3100,  75 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2200,  75 ),
	CG_NEXT( pl0a_special_22_loop ),
};

/* Char_Special 23: drain_move (Power Drain) */
const Sint32 pl0a_special_23[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2300,   4 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2400,   4 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2500,   4 ),
	CG_FRAME(    0,    1,   3,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2600,   4 ),
	CG_FRAME(    0,    1,   3,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2700,   4 ),
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_23_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_23_loop ),
};

/* Char_Special 28 */
const Sint32 pl0a_special_28[] = {
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   400, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_28_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl0a_special_28_loop ),
};

/* Char_Special 30 */
const Sint32 pl0a_special_30[] = {
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
};

const Sint32 pl0a_special_30_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   402, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_30_loop ),
};

/* Char_Special 24 */
const Sint32 pl0a_special_24[] = {
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  10,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   401, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
};

const Sint32 pl0a_special_24_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl0a_special_24_loop ),
};

/* Char_Special 25 */
const Sint32 pl0a_special_25[] = {
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   402, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl0a_special_25_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   402, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_25_loop ),
};

/* Char_Special 26: screw_init (Screw Piledriver)
   Char_Special 29 */
const Sint32 pl0a_special_26[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl0a_special_26 ),
};

/* Char_Special 27: screw_catch (Screw Piledriver) */
const Sint32 pl0a_special_27[] = {
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   403, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   404, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   419, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   420, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   421, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   237, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_special_27 ),
};

/* Char_Special 31 */
const Sint32 pl0a_special_31[] = {
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   428, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   1,           0,   429, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    1,   1,           0,   429, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   3,           0,   429, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
};

const Sint32 pl0a_special_31_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   430, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   432, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0a_special_31_loop ),
};

/* Char_Special 32 */
const Sint32 pl0a_special_32[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_32 ),
};

/* Char_Special 33 */
const Sint32 pl0a_special_33[] = {
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   468, 0x80,   0x00,              ATK,  0x6d,       0x05,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0,   468, 0x80,   0x00,              ATK,  0x6d, 0x4b | CSE,    0,   0,     0,      0, 112 ),
};

const Sint32 pl0a_special_33_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl0a_special_33_loop ),
};

/* Char_Special 34 */
const Sint32 pl0a_special_34[] = {
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0,   129, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   123, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   122, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_34 ),
};

/* Char_Special 35 */
const Sint32 pl0a_special_35[] = {
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   467, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   1,           0,   467, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   2 ),
};

const Sint32 pl0a_special_35_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0a_special_35_loop ),
};

/* Char_Special 36 */
const Sint32 pl0a_special_36[] = {
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_36 ),
};

/* Char_Special 48 */
const Sint32 pl0a_special_48[] = {
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   497, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1,           0,   498, 0x80,   0x00,              ATK,  0x6d,       0x05,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0,   498, 0x80,   0x00,              ATK,  0x6d, 0x4b | CSE,    0,   0,     0,      0, 112 ),
};

const Sint32 pl0a_special_48_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   499, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   503, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl0a_special_48_loop ),
};

/* Char_Special 49 */
const Sint32 pl0a_special_49[] = {
	CG_FRAME(    0,    0,   4,           0,   504, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0,   129, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   123, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   122, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_48 ),
};

/* Char_Special 38 */
const Sint32 pl0a_special_38[] = {
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_FRAME(    0,    1,   2,           0,   433, 0x80,   0x00,              ATK,  0x62,       0x04,    0,   0,     0, 0x1100,  97 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_NEXT( pl0a_special_38 ),
};

/* Char_Special 37: barrage_init (Berserker Barrage, Wolverine's power) */
const Sint32 pl0a_special_37[] = {
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_FRAME(    0,    3,   2,           0,   434, 0x80,   0x00,              ATK,  0x79,       0x04,    0,   0,     0, 0x1100,  98 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_NEXT( pl0a_special_37 ),
};

/* Char_Special 39 */
const Sint32 pl0a_special_39[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2300,   4 ),
	CG_FRAME(    0,    0,   1,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2400,   4 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2500,   4 ),
	CG_FRAME(    0,    1,   3,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2600,   4 ),
	CG_FRAME(    0,    1,   3,           0,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2700,   4 ),
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_39 ),
};

/* Char_Special 41 */
const Sint32 pl0a_special_41[] = {
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_FRAME(    0,    1,   2,           0,   433, 0x80,   0x00,              ATK,  0x7d,       0x04,    0,   0,     0, 0x1100,  97 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_NEXT( pl0a_special_41 ),
};

/* Char_Special 40 */
const Sint32 pl0a_special_40[] = {
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_FRAME(    0,    3,   2,           0,   434, 0x80,   0x00,              ATK,  0x7b,       0x04,    0,   0,     0, 0x1100,  98 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_NEXT( pl0a_special_40 ),
};

/* Char_Special 53: barragex_move (Berserker Barrage X, Wolverine's power (team hyper)) */
const Sint32 pl0a_special_53[] = {
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_special_53 ),
};

/* Char_Special 42 */
const Sint32 pl0a_special_42[] = {
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   4,           0,   400, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_special_42_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   401, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl0a_special_42_loop ),
};

/* Char_Special 43 */
const Sint32 pl0a_special_43[] = {
	CG_FRAME(    0,    0,   4,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
};

const Sint32 pl0a_special_43_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   402, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_special_43_loop ),
};

/* Char_Special 44: dive_init (Dive Punch) */
const Sint32 pl0a_special_44[] = {
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0a_special_44_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   100, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_special_44_loop ),
};

/* Char_Special 45: dive_rise (Dive Punch) */
const Sint32 pl0a_special_45[] = {
	CG_FRAME(    0,    0,   2,           0,   216, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   214, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0a_special_45 ),
};

/* Char_Special 46: dive_turn (Dive Punch) */
const Sint32 pl0a_special_46[] = {
	CG_FRAME(    0,    0,   6,           0,   511, 0x80,   0x00,              ATK,  0x6f,       0x05,    0,   0,     0, 0x2800,  47 ),
};

const Sint32 pl0a_special_46_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   511, 0x80,   0x00,              ATK,  0x6f,          0,    0,   0,     0, 0x2800,  47 ),
	CG_NEXT( pl0a_special_46_loop ),
};

/* Char_Special 47: dive_dive (Dive Punch) */
const Sint32 pl0a_special_47[] = {
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_special_47 ),
};

/* Char_Super 0: sugah_init (Goodnight Sugah (hyper)) */
const Sint32 pl0a_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,              ATK,  0x00,       0x0a,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_super_00_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_super_00_loop ),
};

/* Char_Super 1: sugah_dash_init (Goodnight Sugah (hyper)) */
const Sint32 pl0a_super_01[] = {
	CG_FRAME(    0,    1,   4,           0,   399, 0x80,   0x00,              ATK,  0x65,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,   0 ),
};

const Sint32 pl0a_super_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2100, 100 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2200, 100 ),
	CG_NEXT( pl0a_super_01_loop ),
};

/* Char_Super 2: sugah_dash (Goodnight Sugah (hyper)), sugah_catch (Goodnight Sugah (hyper)) */
const Sint32 pl0a_super_02[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2300,   4 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2400,   4 ),
	CG_FRAME(    0,    0,  12,           0,    81, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2500,   4 ),
	CG_FRAME(    0,    1,   3,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2600,   4 ),
	CG_FRAME(    0,    1,   3,           0,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x2700,   4 ),
	CG_FRAME(    0,    1,   2,           0,    90, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_super_02_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_super_02_loop ),
};

/* Char_Super 3: sugah_dash (Goodnight Sugah (hyper)) */
const Sint32 pl0a_super_03[] = {
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,              ATK,  0x67,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   2,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,              ATK,  0x6a,       0x04,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   297, 0x80,   0x00,              ATK,  0x67,       0x05,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   3,           0,   298, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   234, 0x80,   0x00,              ATK,  0x6a,       0x03,    0,   0,     0,      0, 104 ),
	CG_FRAME( 0x80,    0,   2,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   336, 0x80,   0x00,              ATK,  0x67,       0x04,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   318, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x00,              ATK,  0x6a,       0x05,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2,           0,   321, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME( 0x80,    0,   1,           0,   244, 0x80,   0x00,              ATK,  0x67,       0x03,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   3,           0,   304, 0x80,   0x00,              ATK,  0x6a,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x00,              ATK,  0x67,       0x04,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   291, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   262, 0x80,   0x00,              ATK,  0x6c,       0x05,    0,   0,     0, 0x0100, 110 ),
	CG_FRAME(    0,    0,   2,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0300,   0 ),
	CG_FRAME(    0,    4,   2,           0,   265, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    8,   2,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,   2, END_OF_DATA,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_super_03 ),
};

/* Char_Super 4: sugah_catch (Goodnight Sugah (hyper)) */
const Sint32 pl0a_super_04[] = {
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_super_04 ),
};

/* Char_Super 5: shungoku_init (Shun Goku Satsu) */
const Sint32 pl0a_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,   507, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   507, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_super_05_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   507, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_super_05_loop ),
};

/* Char_Super 6: team_init (the team hyper combo) */
const Sint32 pl0a_super_06[] = {
	CG_FRAME(    1,    0,   4,           0,   133, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    1,    0,   2,           0,   184, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    1,    1,   6,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    1,    0,   2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x70,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x71,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x70,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x71,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x70,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x71,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   534, 0x80,   0x00,              ATK,  0x70,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   530, 0x80,   0x00,              ATK,  0x72,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   1,           0,   531, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,  20, END_OF_DATA,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0a_super_06 ),
};

/* Char_Super 7: team_move (the team hyper combo) */
const Sint32 pl0a_super_07[] = {
	CG_FRAME(    0,    0,   6,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2300,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2400,   1 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2500,   1 ),
	CG_FRAME(    0,    1,   4,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    1,   4,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x2700,   1 ),
	CG_FRAME(    0,    1,   3,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_super_07 ),
};

/* Char_Super 10: barragex_init (Berserker Barrage X, Wolverine's power (team hyper)) */
const Sint32 pl0a_super_10[] = {
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x00,              ATK,  0x00,       0x0a,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_super_10 ),
};

/* Char_Super 11: barragex_start (Berserker Barrage X, Wolverine's power (team hyper)) */
const Sint32 pl0a_super_11[] = {
	CG_FRAME(    0,    0,   1,           0,   529, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_FRAME(    0,    1,   1,           0,   433, 0x80,   0x00,              ATK,  0x63,          0,    0,   0,     0, 0x1100,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   532, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_NEXT( pl0a_super_11 ),
};

/* Char_Super 12 */
const Sint32 pl0a_super_12[] = {
	CG_FRAME(    0,    0,   1,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_FRAME(    0,    3,   1,           0,   434, 0x80,   0x00,              ATK,  0x7a,          0,    0,   0,     0, 0x1100,  98 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1100,  50 ),
	CG_NEXT( pl0a_super_12 ),
};

/* Char_Super 13
   Char_Super 14 */
const Sint32 pl0a_super_13[] = {
	CG_FRAME(    0,    0,   2,           0,   419, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   420, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  48, END_OF_DATA,   421, 0x80,   0x00,              ATK,  0x7c,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_super_13 ),
};

/* Char_Super 15: reppa_init (Shoryu-Reppa, Ken's power (team hyper)) */
const Sint32 pl0a_super_15[] = {
	CG_FRAME(    0,    0,   5,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   475, 0x80,   0x00,              ATK,  0x00,       0x0a,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x80,   0x00,              ATK,  0x27,       0x04,    0,   0,     0, 0x0d00,  62 ),
	CG_FRAME(    0,    0,   4,           0,   477, 0x80,   0x00,              ATK,  0x28, 0x4b | CSE,    0,   0,     0, 0x0e00,  63 ),
	CG_FRAME(    0,    0,   3,           0,   478, 0x80,   0x00,              ATK,  0x28,          0,    0,   0,     0, 0x0f00,  64 ),
	CG_FRAME(    0,    0,   3,           0,   478, 0x80,   0x00,              ATK,  0x28,          0,    0,   0,     0, 0x0f00,  64 ),
	CG_FRAME(    0,    0,   3,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   3,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   1,           0,   481, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,   2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   476, 0x80,   0x00,              ATK,  0x28,       0x04,    0,   0,     0, 0x0d00, 128 ),
	CG_FRAME(    0,    0,   3,           0,   477, 0x80,   0x00,              ATK,  0x28, 0x4b | CSE,    0,   0,     0, 0x0e00, 129 ),
	CG_FRAME(    0,    0,   3,           0,   478, 0x80,   0x00,              ATK,  0x28,          0,    0,   0,     0, 0x0f00, 130 ),
	CG_FRAME(    0,    0,   3,           0,   478, 0x80,   0x00,              ATK,  0x28,          0,    0,   0,     0, 0x0f00, 130 ),
	CG_FRAME(    0,    0,   3,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   3,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   1,           0,   481, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,   476, 0x80,   0x00,              ATK,  0x2a,       0x05,    0,   0,     0, 0x0d00, 128 ),
	CG_FRAME(    0,    1,   3,           0,   477, 0x80,   0x00,              ATK,  0x29, 0x4b | CSE,    0,   0,     0, 0x0e00, 129 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00, 130 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00, 130 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   6,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    1,   2,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    1,   3,           0,   481, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_super_15_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_super_15_loop ),
};

/* group 27 0
   group 27 1
   group 27 18
   group 27 20
   group 27 21
   group 27 22 */
const Sint32 pl0a_grp27_00[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   268 ),
	CG_NEXT( pl0a_grp27_00 ),
};

/* group 27 2 */
const Sint32 pl0a_grp27_02[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   269 ),
	CG_NEXT( pl0a_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl0a_grp27_03[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   270 ),
	CG_NEXT( pl0a_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl0a_grp27_04[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   271 ),
	CG_NEXT( pl0a_grp27_04 ),
};

/* group 27 5
   group 27 6
   group 27 7
   group 27 8
   group 27 9
   group 27 10 */
const Sint32 pl0a_grp27_05[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   482 ),
	CG_NEXT( pl0a_grp27_05 ),
};

/* group 27 13 */
const Sint32 pl0a_grp27_13[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   483 ),
	CG_NEXT( pl0a_grp27_13 ),
};

/* group 27 14 */
const Sint32 pl0a_grp27_14[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   484 ),
	CG_NEXT( pl0a_grp27_14 ),
};

/* group 27 15 */
const Sint32 pl0a_grp27_15[] = {
	EF_FRAME(    0,    0,   4,           0,   485 ),
};

const Sint32 pl0a_grp27_15_loop[] = {
	EF_FRAME(    0,    0,   2,           0,   486 ),
	EF_FRAME(    0,    0,   2,           0,   487 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   488 ),
	CG_NEXT( pl0a_grp27_15_loop ),
};

/* group 27 16 */
const Sint32 pl0a_grp27_16[] = {
	EF_FRAME(    0,    0,   2,           0,   489 ),
	EF_FRAME(    0,    0,   2,           0,   490 ),
	EF_FRAME(    0,    0,   2,           0,   491 ),
	EF_FRAME(    0,    0,   2,           0,   492 ),
	EF_FRAME(    0,    0,   2,           0,   493 ),
	EF_FRAME(    0,    0,   2,           0,   494 ),
	EF_FRAME(    0,    0,   2,           0,   495 ),
};

const Sint32 pl0a_grp27_16_loop[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,    -1 ),
	CG_NEXT( pl0a_grp27_16_loop ),
};

/* group 27 17 */
const Sint32 pl0a_grp27_17[] = {
	EF_FRAME(    0,    0,   2,           0,   435 ),
	EF_FRAME(    0,    0,   2,           0,   436 ),
	EF_FRAME(    0,    0,   2,           0,   437 ),
	EF_FRAME(    0,    0,   2,           0,   438 ),
	EF_FRAME(    0,    0,   2,           0,   439 ),
	EF_FRAME(    0,    0,   2,           0,   440 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   441 ),
	CG_NEXT( pl0a_grp27_17 ),
};

/* Char_Effect 0: shl0a_00_init */
const Sint32 pl0a_effect_00[] = {
	CG_FRAME(    0,    0,   6,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_NEXT( pl0a_effect_00 ),
};

/* group 27 19
   group 27 26: pls0a_00_init (26 + type) */
const Sint32 pl0a_grp27_19[] = {
	EF_FRAME(    0,    0,   5,           0,   648 ),
	EF_FRAME(    0,    0,   5,           0,   649 ),
	EF_FRAME(    0,    0,   5,           0,   650 ),
	EF_FRAME(    0,    0,   5,           0,   651 ),
	EF_FRAME(    0,    0,   5,           0,   652 ),
	EF_FRAME(    0,    0,   5,           0,   653 ),
	EF_FRAME(    0,    0,   5,           0,   654 ),
	EF_FRAME(    0,    0,   5,           0,   655 ),
	EF_FRAME(    0,    0,   5,           0,   656 ),
	EF_FRAME(    0,    0,   5,           0,   657 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   658 ),
	CG_NEXT( pl0a_grp27_19 ),
};

/* Char_Effect 3
   Char_Effect 7 */
const Sint32 pl0a_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,   458, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    0,   1,           0,   459, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    0,   1,           0,   460, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl0a_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl0a_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   464, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl0a_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   1,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   1,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   1,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   1,           0,   450, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   452, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   453, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_05 ),
};

/* Char_Effect 6
   Char_Effect 10 */
const Sint32 pl0a_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0,   446, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   447, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   448, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   449, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_06 ),
};

/* Char_Effect 8 */
const Sint32 pl0a_effect_08[] = {
	CG_FRAME(    0,    0,   1,           0,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl0a_effect_09[] = {
	CG_FRAME(    0,    0,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_09 ),
};

/* Char_Effect 11: pls0a_03_move (11 + sub_type) */
const Sint32 pl0a_effect_11[] = {
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   464, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl0a_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   451, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   452, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   453, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_effect_12 ),
};

/* Char_Effect 1 */
const Sint32 pl0a_effect_01[] = {
	CG_FRAME(    0,    0,   6,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_NEXT( pl0a_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl0a_effect_02[] = {
	CG_FRAME(    0,    0,   6,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_NEXT( pl0a_effect_02 ),
};

/* Char_Extra 2: fly_start5
   Char_Extra 3: fly_start5
   Char_Extra 4
   Char_Extra 5
   Char_Extra 6
   Char_Extra 7 */
const Sint32 pl0a_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   528, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_extra_02 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl0a_grp25_02[] = {
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   497, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   500, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   501, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   504, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_grp25_02 ),
};

/* group 25 3
   Char_Fly 3 */
const Sint32 pl0a_grp25_03[] = {
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   497, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   500, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   501, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   504, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0a_grp25_03 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl0a_change_03[] = {
	CG_FRAME(    0,    0,   8,           0,   193, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_change_03_loop[] = {
	CG_FRAME(    0,    0,  17, END_OF_DATA,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl0a_change_04[] = {
	CG_FRAME(    0,    0,   8,           0,   193, 0x80,   0x00,                0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0a_change_04_loop[] = {
	CG_FRAME(    0,    0,  17, END_OF_DATA,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0a_change_04_loop ),
};

/* group 27 27 */
const Sint32 pl0a_grp27_27[] = {
	EF_FRAME(    0,    0,   1,           0,   659 ),
	EF_FRAME(    0,    0,   1,           0,   660 ),
	EF_FRAME(    0,    0,   1,           0,   661 ),
	EF_FRAME(    0,    0,   1,           0,   662 ),
	EF_FRAME(    0,    0,   1,           0,   663 ),
	EF_FRAME(    0,    0,   1,           0,   664 ),
	EF_FRAME(    0,    0,   1,           0,   665 ),
	EF_FRAME(    0,    0,   1,           0,   666 ),
	EF_FRAME(    0,    0,   1,           0,   667 ),
	EF_FRAME(    0,    0,   1,           0,   668 ),
	EF_FRAME(    0,    0,   1,           0,   669 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   670 ),
	CG_NEXT( pl0a_grp27_27 ),
};

/* Char_Stand_P 3: nml_atck */
const Sint32 pl0a_stand_p_03[] = {
	CG_FRAME(    0,    0,  16,           0,   426, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x00, ATK | CG_ATCK_NG,  0x18,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    1,    0,   1,           0,   429, 0x80,   0x00, ATK | CG_ATCK_NG,  0x1f,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    1,   1,           0,   429, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,       0x05,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   1,           0,   429, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0, 125 ),
};

const Sint32 pl0a_stand_p_03_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   430, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   2,           0,   431, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   432, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl0a_stand_p_03_loop ),
};

/* Char_Stand_P 4: nml_move_charge */
const Sint32 pl0a_stand_p_04[] = {
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl0a_stand_p_04_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0a_stand_p_04_loop ),
};

/* group 28 0 */
const Sint32 pl0a_grp28_00[] = {
	EF_FRAME(    0,    0,   1,           0, 32768 ),
};

/* group 28 1 */
const Sint32 pl0a_grp28_01[] = {
	EF_FRAME(    1,    0,   1,           0, 32769 ),
};

/* group 28 2 */
const Sint32 pl0a_grp28_02[] = {
	EF_FRAME(    2,    0,   1,           0, 32770 ),
};

/* group 28 3 */
const Sint32 pl0a_grp28_03[] = {
	EF_FRAME(    3,    0,   1,           0, 32771 ),
};

/* group 28 4 */
const Sint32 pl0a_grp28_04[] = {
	EF_FRAME(    4,    0,   1,           0, 32772 ),
};

/* group 28 5 */
const Sint32 pl0a_grp28_05[] = {
	EF_FRAME(    5,    0,   1,           0, 32773 ),
};

/* group 28 6 */
const Sint32 pl0a_grp28_06[] = {
	EF_FRAME(    6,    0,   1,           0, 32774 ),
};

/* group 28 7 */
const Sint32 pl0a_grp28_07[] = {
	EF_FRAME(    7,    0,   1,           0, 32775 ),
};

/* group 28 8 */
const Sint32 pl0a_grp28_08[] = {
	EF_FRAME(    8,    0,   1,           0, 32776 ),
};

/* group 28 9 */
const Sint32 pl0a_grp28_09[] = {
	EF_FRAME(    9,    0,   1,           0, 32777 ),
};

/* group 28 10 */
const Sint32 pl0a_grp28_10[] = {
	EF_FRAME(   10,    0,   1,           0, 32778 ),
};

/* group 28 11 */
const Sint32 pl0a_grp28_11[] = {
	EF_FRAME(   11,    0,   1,           0, 32779 ),
};

/* group 28 12 */
const Sint32 pl0a_grp28_12[] = {
	EF_FRAME(   12,    0,   1,           0, 32780 ),
};

/* group 28 13 */
const Sint32 pl0a_grp28_13[] = {
	EF_FRAME(   13,    0,   1,           0, 32781 ),
};

/* group 28 14 */
const Sint32 pl0a_grp28_14[] = {
	EF_FRAME(   14,    0,   1, END_OF_DATA, 32782 ),
	CG_NEXT( pl0a_grp28_00 ),
};
