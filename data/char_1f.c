/* B.B. Hood (character 1f): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_1f      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_1f     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl1f.c) says which group
                   and number each move uses.
   pl1f_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl1f_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl1f_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl1f_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_1f
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_1f, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_1f */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl1f_stand_00[], pl1f_crouch_01[], pl1f_crouch_01_loop[], pl1f_stand_02[],
	pl1f_stand_03[], pl1f_special_11[], pl1f_special_11_loop[], pl1f_special_12[], pl1f_crouch_00[],
	pl1f_crouch_00_loop[], pl1f_crouch_02[], pl1f_crouch_02_loop[], pl1f_turn_00[], pl1f_turn_01[],
	pl1f_jump_03[], pl1f_jump_00[], pl1f_special_02_loop[], pl1f_jump_01[], pl1f_jump_01_loop[],
	pl1f_jump_02[], pl1f_jump_02_loop[], pl1f_jump_06[], pl1f_jump_09[], pl1f_jump_07[],
	pl1f_jump_08[], pl1f_guard_00[], pl1f_guard_01[], pl1f_guard_02[], pl1f_guard_04[],
	pl1f_guard_05[], pl1f_guard_06[], pl1f_air_guard_00[], pl1f_air_guard_01[], pl1f_unused_00[],
	pl1f_win_00[], pl1f_win_00_loop[], pl1f_win_01[], pl1f_win_01_loop[], pl1f_unused_01[],
	pl1f_win_03[], pl1f_win_03_loop[], pl1f_unused_02[], pl1f_unused_03[], pl1f_win_05[],
	pl1f_win_05_loop[], pl1f_change_00[], pl1f_change_02[], pl1f_change_03[], pl1f_change_03_loop[],
	pl1f_change_01[], pl1f_change_04[], pl1f_change_04_loop[], pl1f_extra_00[], pl1f_extra_00_loop[],
	pl1f_extra_01[], pl1f_extra_04[], pl1f_damage_00[], pl1f_damage_01[], pl1f_damage_02[],
	pl1f_damage_03[], pl1f_damage_04[], pl1f_damage_05[], pl1f_damage_06[], pl1f_damage_07[],
	pl1f_damage_08[], pl1f_damage_10[], pl1f_damage_11[], pl1f_damage_12[], pl1f_damage_09[],
	pl1f_damage_09_loop[], pl1f_guard_03[], pl1f_guard_07[], pl1f_air_guard_02[], pl1f_damage_28[],
	pl1f_damage_29[], pl1f_damage_31[], pl1f_damage_16[], pl1f_damage_17[], pl1f_damage_18[],
	pl1f_damage_19[], pl1f_damage_20[], pl1f_damage_21[], pl1f_damage_22[], pl1f_damage_23[],
	pl1f_damage_24[], pl1f_damage_25[], pl1f_damage_26[], pl1f_damage_13[], pl1f_damage_27[],
	pl1f_damage_30[], pl1f_dizzy_00[], pl1f_get_up_01[], pl1f_get_up_02[], pl1f_get_up_00[],
	pl1f_get_up_03[], pl1f_caught_00[], pl1f_caught_01[], pl1f_caught_02[], pl1f_caught_03[],
	pl1f_caught_04[], pl1f_caught_05[], pl1f_caught_06[], pl1f_caught_07[], pl1f_caught_08[],
	pl1f_caught_09[], pl1f_caught_10[], pl1f_caught_11[], pl1f_caught_12[], pl1f_caught_13[],
	pl1f_caught_14[], pl1f_caught_15[], pl1f_caught_16[], pl1f_caught_17[], pl1f_caught_18[],
	pl1f_caught_19[], pl1f_caught_20[], pl1f_caught_21[], pl1f_stand_p_00[], pl1f_stand_p_03[],
	pl1f_stand_p_01[], pl1f_stand_p_04[], pl1f_stand_p_02[], pl1f_stand_k_00[], pl1f_stand_k_03[],
	pl1f_stand_k_01[], pl1f_stand_k_02[], pl1f_crouch_p_00[], pl1f_crouch_p_01[], pl1f_crouch_p_02[],
	pl1f_crouch_k_00[], pl1f_crouch_k_01[], pl1f_crouch_k_02[], pl1f_jump_p_00[], pl1f_jump_p_01[],
	pl1f_jump_p_04[], pl1f_jump_p_02[], pl1f_jump_p_05[], pl1f_jump_k_00[], pl1f_jump_k_01[],
	pl1f_jump_k_04[], pl1f_jump_k_02[], pl1f_jump_k_02_loop[], pl1f_stand_p_06[], pl1f_stand_k_06[],
	pl1f_crouch_k_03[], pl1f_catch_00[], pl1f_catch_00_loop[], pl1f_catch_01[], pl1f_catch_01_loop[],
	pl1f_catch_02[], pl1f_dash_00[], pl1f_dash_00_loop[], pl1f_dash_01[], pl1f_dash_01_loop[],
	pl1f_special_00[], pl1f_special_01[], pl1f_special_02[], pl1f_special_03[],
	pl1f_special_03_loop[], pl1f_special_15[], pl1f_special_15_loop[], pl1f_special_04[],
	pl1f_special_04_loop[], pl1f_special_16[], pl1f_special_16_loop[], pl1f_special_13[],
	pl1f_special_13_loop[], pl1f_special_17[], pl1f_special_17_loop[], pl1f_special_14[],
	pl1f_special_14_loop[], pl1f_special_18[], pl1f_special_18_loop[], pl1f_special_05[],
	pl1f_special_08[], pl1f_special_08_loop[], pl1f_catch_11[], pl1f_catch_11_loop[],
	pl1f_super_17[], pl1f_super_18[], pl1f_super_18_loop[], pl1f_catch_12[], pl1f_catch_12_loop[],
	pl1f_catch_16[], pl1f_catch_13[], pl1f_catch_13_loop[], pl1f_catch_14[], pl1f_catch_14_loop[],
	pl1f_catch_15[], pl1f_super_16[], pl1f_super_16_loop[], pl1f_catch_03[], pl1f_catch_03_loop[],
	pl1f_catch_04[], pl1f_catch_05[], pl1f_catch_05_loop[], pl1f_catch_06[], pl1f_catch_07[],
	pl1f_catch_08[], pl1f_catch_08_loop[], pl1f_catch_09[], pl1f_catch_09_loop[], pl1f_catch_10[],
	pl1f_catch_10_loop[], pl1f_super_08[], pl1f_super_08_loop[], pl1f_super_09[], pl1f_extra_02[],
	pl1f_extra_05[], pl1f_extra_03[], pl1f_extra_03_loop[], pl1f_grp27_00[], pl1f_grp27_01[],
	pl1f_grp27_01_loop[], pl1f_grp27_02[], pl1f_grp27_02_loop[], pl1f_grp27_03[],
	pl1f_grp27_03_loop[], pl1f_grp27_04[], pl1f_grp27_04_loop[], pl1f_grp27_05[], pl1f_grp27_06[],
	pl1f_grp27_07[], pl1f_grp27_08[], pl1f_grp27_09[], pl1f_grp27_12[], pl1f_grp27_13[],
	pl1f_grp27_14[], pl1f_grp27_15[], pl1f_grp27_104[], pl1f_grp27_105[], pl1f_grp27_106[],
	pl1f_grp27_107[], pl1f_grp27_108[], pl1f_grp27_16[], pl1f_grp27_109[], pl1f_grp27_110[],
	pl1f_grp27_111[], pl1f_grp27_112[], pl1f_grp27_113[], pl1f_grp27_114[], pl1f_grp27_17[],
	pl1f_grp27_115[], pl1f_grp27_116[], pl1f_grp27_117[], pl1f_grp27_118[], pl1f_grp27_119[],
	pl1f_grp27_120[], pl1f_grp27_121[], pl1f_grp27_19[], pl1f_grp27_20[], pl1f_grp27_21[],
	pl1f_grp27_22[], pl1f_grp27_22_loop[], pl1f_grp27_23[], pl1f_grp27_23_loop[], pl1f_grp27_24[],
	pl1f_grp27_24_loop[], pl1f_grp27_28[], pl1f_grp27_29[], pl1f_grp27_30[], pl1f_grp27_31[],
	pl1f_grp27_31_loop[], pl1f_grp27_32[], pl1f_grp27_32_loop[], pl1f_grp27_33[],
	pl1f_grp27_33_loop[], pl1f_grp27_34[], pl1f_grp27_34_loop[], pl1f_grp27_35[],
	pl1f_grp27_35_loop[], pl1f_grp27_36[], pl1f_grp27_36_loop[], pl1f_grp27_37[],
	pl1f_grp27_37_loop[], pl1f_grp27_38[], pl1f_grp27_38_loop[], pl1f_grp27_39[],
	pl1f_grp27_39_loop[], pl1f_grp27_40[], pl1f_grp27_41[], pl1f_grp27_41_loop[], pl1f_grp27_42[],
	pl1f_grp27_42_loop[], pl1f_grp27_43[], pl1f_grp27_44[], pl1f_grp27_46[], pl1f_grp27_45[],
	pl1f_grp27_45_loop[], pl1f_grp27_47[], pl1f_grp27_47_loop[], pl1f_grp27_48[],
	pl1f_grp27_48_loop[], pl1f_grp27_55[], pl1f_grp27_55_loop[], pl1f_grp27_56[],
	pl1f_grp27_56_loop[], pl1f_grp27_57[], pl1f_grp27_57_loop[], pl1f_grp27_58[],
	pl1f_grp27_58_loop[], pl1f_grp27_49[], pl1f_grp27_49_loop[], pl1f_grp27_50[],
	pl1f_grp27_50_loop[], pl1f_grp27_51[], pl1f_grp27_51_loop[], pl1f_grp27_52[],
	pl1f_grp27_52_loop[], pl1f_grp27_53[], pl1f_grp27_53_loop[], pl1f_grp27_54[],
	pl1f_grp27_54_loop[], pl1f_grp27_59[], pl1f_grp27_59_loop[], pl1f_grp27_60[], pl1f_grp27_61[],
	pl1f_grp27_62[], pl1f_grp27_62_loop[], pl1f_grp27_63[], pl1f_grp27_63_loop[], pl1f_grp27_66[],
	pl1f_grp27_67[], pl1f_grp27_70[], pl1f_grp27_70_loop[], pl1f_grp27_79[], pl1f_grp27_79_loop[],
	pl1f_grp27_80[], pl1f_grp27_81[], pl1f_grp27_81_loop[], pl1f_grp27_82[], pl1f_grp27_83[],
	pl1f_grp27_84[], pl1f_grp27_84_loop[], pl1f_grp27_85[], pl1f_grp27_86[], pl1f_grp27_87[],
	pl1f_grp27_88[], pl1f_grp27_88_loop[], pl1f_grp27_89[], pl1f_grp27_89_loop[], pl1f_grp27_90[],
	pl1f_grp27_90_loop[], pl1f_grp27_91[], pl1f_grp27_91_loop[], pl1f_grp27_93[],
	pl1f_grp27_93_loop[], pl1f_grp27_94[], pl1f_grp27_94_loop[], pl1f_grp27_95[],
	pl1f_grp27_95_loop[], pl1f_grp27_96[], pl1f_grp27_96_loop[], pl1f_grp27_97[], pl1f_grp27_98[],
	pl1f_grp27_98_loop[], pl1f_grp27_99[], pl1f_grp27_99_loop[], pl1f_grp27_100[],
	pl1f_grp27_100_loop[], pl1f_grp27_101[], pl1f_grp27_101_loop[], pl1f_grp27_102[],
	pl1f_grp27_102_loop[], pl1f_grp27_103[], pl1f_grp27_103_loop[], pl1f_effect_00[],
	pl1f_effect_01[], pl1f_effect_01_loop[], pl1f_effect_02[], pl1f_effect_02_loop[],
	pl1f_effect_03[], pl1f_effect_03_loop[], pl1f_effect_04[], pl1f_effect_04_loop[],
	pl1f_effect_05[], pl1f_effect_05_loop[], pl1f_effect_08[], pl1f_effect_09[], pl1f_effect_16[],
	pl1f_effect_16_loop[], pl1f_effect_10[], pl1f_effect_10_loop[], pl1f_effect_11[],
	pl1f_effect_12[], pl1f_effect_13[], pl1f_effect_14[], pl1f_effect_15[], pl1f_effect_15_loop[],
	pl1f_effect_17[], pl1f_effect_18[], pl1f_effect_18_loop[], pl1f_effect_20[],
	pl1f_effect_20_loop[], pl1f_effect_19[], pl1f_effect_19_loop[], pl1f_effect_21[],
	pl1f_effect_21_loop[];
extern const Sint32 *const pl1f_stand_tbl[], *const pl1f_jump_tbl[], *const pl1f_dash_tbl[],
	*const pl1f_crouch_tbl[], *const pl1f_turn_tbl[], *const pl1f_guard_tbl[],
	*const pl1f_air_guard_tbl[], *const pl1f_stand_p_tbl[], *const pl1f_stand_k_tbl[],
	*const pl1f_crouch_p_tbl[], *const pl1f_crouch_k_tbl[], *const pl1f_jump_p_tbl[],
	*const pl1f_jump_k_tbl[], *const pl1f_damage_tbl[], *const pl1f_dizzy_tbl[],
	*const pl1f_catch_tbl[], *const pl1f_caught_tbl[], *const pl1f_get_up_tbl[],
	*const pl1f_appear_tbl[], *const pl1f_win_tbl[], *const pl1f_extra_tbl[],
	*const pl1f_special_tbl[], *const pl1f_super_tbl[], *const pl1f_effect_tbl[],
	*const pl1f_change_tbl[], *const pl1f_grp25_tbl[], *const pl1f_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_1f[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 500, 300, -6144, 0, 0, 0, 0, 0 },
	{ 500, 300, -4096, 0, 0, 0, 0, 0 },
	{ 500, 300, -2048, 0, 0, 0, 0, 0 },
	{ 500, 300, 0, 0, 0, 0, 0, 0 },
	{ 500, 300, 9216, 0, 0, 0, 0, 0 },
	{ 500, 300, 6144, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 12, 65528 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 65504 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_1f[] = {
	pl1f_stand_tbl, pl1f_jump_tbl, pl1f_dash_tbl, pl1f_crouch_tbl, pl1f_turn_tbl, pl1f_guard_tbl,
	pl1f_air_guard_tbl, pl1f_stand_p_tbl, pl1f_stand_k_tbl, pl1f_crouch_p_tbl, pl1f_crouch_k_tbl, pl1f_jump_p_tbl,
	pl1f_jump_k_tbl, pl1f_damage_tbl, pl1f_dizzy_tbl, pl1f_catch_tbl, pl1f_caught_tbl, pl1f_get_up_tbl,
	pl1f_appear_tbl, pl1f_win_tbl, pl1f_extra_tbl, pl1f_special_tbl, pl1f_super_tbl, pl1f_effect_tbl,
	pl1f_change_tbl, pl1f_grp25_tbl, pl1f_stand_tbl, pl1f_grp27_tbl, 0,
};

const Sint32 *const pl1f_stand_tbl[] = {
	pl1f_stand_00, pl1f_stand_00, pl1f_stand_02, pl1f_stand_03, pl1f_stand_00, pl1f_stand_00,
	pl1f_stand_03, pl1f_stand_03, 0,
};

const Sint32 *const pl1f_jump_tbl[] = {
	pl1f_jump_00, pl1f_jump_01, pl1f_jump_02, pl1f_jump_03, pl1f_jump_03, pl1f_jump_03,
	pl1f_jump_06, pl1f_jump_07, pl1f_jump_08, pl1f_jump_09, pl1f_jump_09, pl1f_jump_09,
	0,
};

const Sint32 *const pl1f_dash_tbl[] = {
	pl1f_dash_00, pl1f_dash_01, pl1f_dash_00, pl1f_dash_01, 0,
};

const Sint32 *const pl1f_crouch_tbl[] = {
	pl1f_crouch_00, pl1f_crouch_01, pl1f_crouch_02, 0,
};

const Sint32 *const pl1f_turn_tbl[] = {
	pl1f_turn_00, pl1f_turn_01, 0,
};

const Sint32 *const pl1f_guard_tbl[] = {
	pl1f_guard_00, pl1f_guard_01, pl1f_guard_02, pl1f_guard_03, pl1f_guard_04, pl1f_guard_05,
	pl1f_guard_06, pl1f_guard_07, 0,
};

const Sint32 *const pl1f_air_guard_tbl[] = {
	pl1f_air_guard_00, pl1f_air_guard_01, pl1f_air_guard_02, 0,
};

const Sint32 *const pl1f_stand_p_tbl[] = {
	pl1f_stand_p_00, pl1f_stand_p_01, pl1f_stand_p_02, pl1f_stand_p_03, pl1f_stand_p_04, pl1f_stand_p_02,
	pl1f_stand_p_06, 0,
};

const Sint32 *const pl1f_stand_k_tbl[] = {
	pl1f_stand_k_00, pl1f_stand_k_01, pl1f_stand_k_02, pl1f_stand_k_03, pl1f_stand_k_01, pl1f_stand_k_02,
	pl1f_stand_k_06, 0,
};

const Sint32 *const pl1f_crouch_p_tbl[] = {
	pl1f_crouch_p_00, pl1f_crouch_p_01, pl1f_crouch_p_02, 0,
};

const Sint32 *const pl1f_crouch_k_tbl[] = {
	pl1f_crouch_k_00, pl1f_crouch_k_01, pl1f_crouch_k_02, pl1f_crouch_k_03, 0,
};

const Sint32 *const pl1f_jump_p_tbl[] = {
	pl1f_jump_p_00, pl1f_jump_p_01, pl1f_jump_p_02, pl1f_jump_p_00, pl1f_jump_p_04, pl1f_jump_p_05,
	0,
};

const Sint32 *const pl1f_jump_k_tbl[] = {
	pl1f_jump_k_00, pl1f_jump_k_01, pl1f_jump_k_02, pl1f_jump_k_00, pl1f_jump_k_04, pl1f_jump_k_02,
	0,
};

const Sint32 *const pl1f_damage_tbl[] = {
	pl1f_damage_00, pl1f_damage_01, pl1f_damage_02, pl1f_damage_03, pl1f_damage_04, pl1f_damage_05,
	pl1f_damage_06, pl1f_damage_07, pl1f_damage_08, pl1f_damage_09, pl1f_damage_10, pl1f_damage_11,
	pl1f_damage_12, pl1f_damage_13, pl1f_damage_13, pl1f_damage_13, pl1f_damage_16, pl1f_damage_17,
	pl1f_damage_18, pl1f_damage_19, pl1f_damage_20, pl1f_damage_21, pl1f_damage_22, pl1f_damage_23,
	pl1f_damage_24, pl1f_damage_25, pl1f_damage_26, pl1f_damage_27, pl1f_damage_28, pl1f_damage_29,
	pl1f_damage_30, pl1f_damage_31, pl1f_damage_30, pl1f_damage_27, pl1f_damage_13, 0,
};

const Sint32 *const pl1f_dizzy_tbl[] = {
	pl1f_dizzy_00, 0,
};

const Sint32 *const pl1f_catch_tbl[] = {
	pl1f_catch_00, pl1f_catch_01, pl1f_catch_02, pl1f_catch_03, pl1f_catch_04, pl1f_catch_05,
	pl1f_catch_06, pl1f_catch_07, pl1f_catch_08, pl1f_catch_09, pl1f_catch_10, pl1f_catch_11,
	pl1f_catch_12, pl1f_catch_13, pl1f_catch_14, pl1f_catch_15, pl1f_catch_16, 0,
};

const Sint32 *const pl1f_caught_tbl[] = {
	pl1f_caught_00, pl1f_caught_01, pl1f_caught_02, pl1f_caught_03, pl1f_caught_04, pl1f_caught_05,
	pl1f_caught_06, pl1f_caught_07, pl1f_caught_08, pl1f_caught_09, pl1f_caught_10, pl1f_caught_11,
	pl1f_caught_12, pl1f_caught_13, pl1f_caught_14, pl1f_caught_15, pl1f_caught_16, pl1f_caught_17,
	pl1f_caught_18, pl1f_caught_19, pl1f_caught_20, pl1f_caught_21, 0,
};

const Sint32 *const pl1f_get_up_tbl[] = {
	pl1f_get_up_00, pl1f_get_up_01, pl1f_get_up_02, pl1f_get_up_03, 0,
};

const Sint32 *const pl1f_appear_tbl[] = {
	pl1f_stand_02, 0,
};

const Sint32 *const pl1f_win_tbl[] = {
	pl1f_win_00, pl1f_win_01, pl1f_win_00, pl1f_win_03, pl1f_win_03, pl1f_win_05,
	pl1f_win_05, 0,
};

const Sint32 *const pl1f_extra_tbl[] = {
	pl1f_extra_00, pl1f_extra_01, pl1f_extra_02, pl1f_extra_03, pl1f_extra_04, pl1f_extra_05,
	0,
};

const Sint32 *const pl1f_special_tbl[] = {
	pl1f_special_00, pl1f_special_01, pl1f_special_02, pl1f_special_03, pl1f_special_04, pl1f_special_05,
	pl1f_special_05, pl1f_special_05, pl1f_special_08, pl1f_special_08, pl1f_special_08, pl1f_special_11,
	pl1f_special_12, pl1f_special_13, pl1f_special_14, pl1f_special_15, pl1f_special_16, pl1f_special_17,
	pl1f_special_18, 0,
};

const Sint32 *const pl1f_super_tbl[] = {
	pl1f_catch_03, pl1f_catch_04, pl1f_catch_05, pl1f_catch_06, pl1f_catch_07, pl1f_catch_08,
	pl1f_catch_09, pl1f_catch_10, pl1f_super_08, pl1f_super_09, pl1f_catch_11, pl1f_catch_12,
	pl1f_catch_13, pl1f_catch_14, pl1f_catch_15, pl1f_catch_16, pl1f_super_16, pl1f_super_17,
	pl1f_super_18, 0,
};

const Sint32 *const pl1f_effect_tbl[] = {
	pl1f_effect_00, pl1f_effect_01, pl1f_effect_02, pl1f_effect_03, pl1f_effect_04, pl1f_effect_05,
	pl1f_effect_04, pl1f_effect_05, pl1f_effect_08, pl1f_effect_09, pl1f_effect_10, pl1f_effect_11,
	pl1f_effect_12, pl1f_effect_13, pl1f_effect_14, pl1f_effect_15, pl1f_effect_16, pl1f_effect_17,
	pl1f_effect_18, pl1f_effect_19, pl1f_effect_20, pl1f_effect_21, 0,
};

const Sint32 *const pl1f_change_tbl[] = {
	pl1f_change_00, pl1f_change_01, pl1f_change_02, pl1f_change_03, pl1f_change_04, 0,
};

const Sint32 *const pl1f_grp25_tbl[] = {
	pl1f_stand_00, 0,
};

const Sint32 *const pl1f_grp27_tbl[] = {
	pl1f_grp27_00, pl1f_grp27_01, pl1f_grp27_02, pl1f_grp27_03, pl1f_grp27_04, pl1f_grp27_05,
	pl1f_grp27_06, pl1f_grp27_07, pl1f_grp27_08, pl1f_grp27_09, pl1f_grp27_09, pl1f_grp27_09,
	pl1f_grp27_12, pl1f_grp27_13, pl1f_grp27_14, pl1f_grp27_15, pl1f_grp27_16, pl1f_grp27_17,
	pl1f_grp27_17, pl1f_grp27_19, pl1f_grp27_20, pl1f_grp27_21, pl1f_grp27_22, pl1f_grp27_23,
	pl1f_grp27_24, pl1f_grp27_24, pl1f_grp27_24, pl1f_grp27_24, pl1f_grp27_28, pl1f_grp27_29,
	pl1f_grp27_30, pl1f_grp27_31, pl1f_grp27_32, pl1f_grp27_33, pl1f_grp27_34, pl1f_grp27_35,
	pl1f_grp27_36, pl1f_grp27_37, pl1f_grp27_38, pl1f_grp27_39, pl1f_grp27_40, pl1f_grp27_41,
	pl1f_grp27_42, pl1f_grp27_43, pl1f_grp27_44, pl1f_grp27_45, pl1f_grp27_46, pl1f_grp27_47,
	pl1f_grp27_48, pl1f_grp27_49, pl1f_grp27_50, pl1f_grp27_51, pl1f_grp27_52, pl1f_grp27_53,
	pl1f_grp27_54, pl1f_grp27_55, pl1f_grp27_56, pl1f_grp27_57, pl1f_grp27_58, pl1f_grp27_59,
	pl1f_grp27_60, pl1f_grp27_61, pl1f_grp27_62, pl1f_grp27_63, pl1f_grp27_63, pl1f_grp27_63,
	pl1f_grp27_66, pl1f_grp27_67, pl1f_grp27_63, pl1f_grp27_63, pl1f_grp27_70, pl1f_grp27_70,
	pl1f_grp27_70, pl1f_grp27_70, pl1f_grp27_70, pl1f_grp27_70, pl1f_grp27_70, pl1f_grp27_70,
	pl1f_grp27_70, pl1f_grp27_79, pl1f_grp27_80, pl1f_grp27_81, pl1f_grp27_82, pl1f_grp27_83,
	pl1f_grp27_84, pl1f_grp27_85, pl1f_grp27_86, pl1f_grp27_87, pl1f_grp27_88, pl1f_grp27_89,
	pl1f_grp27_90, pl1f_grp27_91, pl1f_grp27_89, pl1f_grp27_93, pl1f_grp27_94, pl1f_grp27_95,
	pl1f_grp27_96, pl1f_grp27_97, pl1f_grp27_98, pl1f_grp27_99, pl1f_grp27_100, pl1f_grp27_101,
	pl1f_grp27_102, pl1f_grp27_103, pl1f_grp27_104, pl1f_grp27_105, pl1f_grp27_106, pl1f_grp27_107,
	pl1f_grp27_108, pl1f_grp27_109, pl1f_grp27_110, pl1f_grp27_111, pl1f_grp27_112, pl1f_grp27_113,
	pl1f_grp27_114, pl1f_grp27_115, pl1f_grp27_116, pl1f_grp27_117, pl1f_grp27_118, pl1f_grp27_119,
	pl1f_grp27_120, pl1f_grp27_121, pl1f_grp27_09, 0, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_move, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl1f_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_stand_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl1f_crouch_01[] = {
	CG_FRAME(    0,    0,   59,           0,    96, 0x80,   0x40,    0,  0x18,          0,    0,   0,    26,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,    96, 0x80,   0x40,    0,  0x18,       0x1c,    0,   0,    26,      0,   2 ),
};

const Sint32 pl1f_crouch_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x40,    0,  0x18,          0,    0,   0,    26, 0x0100,   2 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x40,    0,  0x18,          0,    0,   0,    26, 0x0100,   2 ),
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x40,    0,  0x18,          0,    0,   0,    26, 0x0100,   2 ),
	CG_FRAME(    0,    0,   64, END_OF_DATA,    97, 0x80,   0x40,    0,  0x18,          0,    0,   0,    26, 0x0100,   2 ),
	CG_NEXT( pl1f_crouch_01_loop ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Appear 0: appear_init
   Char_Fly 2 */
const Sint32 pl1f_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   101, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 6
   Char_Stand 7
   Char_Fly 3
   Char_Fly 6
   Char_Fly 7 */
const Sint32 pl1f_stand_03[] = {
	CG_FRAME(    0,    0,    5,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_stand_03 ),
};

/* Char_Special 11: tellme_init (Tell Me Why) */
const Sint32 pl1f_special_11[] = {
	CG_FRAME(    0,    0,    3,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
};

const Sint32 pl1f_special_11_loop[] = {
	CG_FRAME(    0,    0,    8,           0,   480, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    8,           0,   481, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    8,           0,   482, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   483, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl1f_special_11_loop ),
};

/* Char_Special 12: tellme_move (Tell Me Why) */
const Sint32 pl1f_special_12[] = {
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_special_12 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl1f_crouch_00[] = {
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,    0,  0x18,          0,    0,   0,    22,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x40,    0,  0x18,          0,    0,   0,    24,      0,   2 ),
};

const Sint32 pl1f_crouch_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x40,    0,  0x18,          0,    0,   0,    26,      0,   2 ),
	CG_NEXT( pl1f_crouch_00_loop ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl1f_crouch_02[] = {
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x40,    0,  0x00,          0,    0,   0,    26,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   162, 0x80,   0x40,    0,  0x00,          0,    0,   0,    24,      0,   1 ),
};

const Sint32 pl1f_crouch_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,    22,      0,   1 ),
	CG_NEXT( pl1f_crouch_02_loop ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl1f_turn_00[] = {
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl1f_turn_01[] = {
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x40,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x40,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   169, 0x80,   0x40,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
	CG_NEXT( pl1f_turn_01 ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl1f_jump_03[] = {
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_jump_03 ),
};

/* Char_Jump 0 */
const Sint32 pl1f_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1f_special_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   137, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_special_02_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl1f_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1f_jump_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   147, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl1f_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   150, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl1f_jump_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   157, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_jump_02_loop ),
};

/* Char_Jump 6 */
const Sint32 pl1f_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl1f_jump_09[] = {
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   137, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl1f_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   147, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_jump_09 ),
};

/* Char_Jump 8 */
const Sint32 pl1f_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   150, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   157, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_jump_09 ),
};

/* Char_Guard 0 */
const Sint32 pl1f_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl1f_guard_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl1f_guard_02[] = {
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl1f_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x40,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl1f_guard_05[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   173, 0x80,   0x40,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
	CG_NEXT( pl1f_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl1f_guard_06[] = {
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,    26,      0,   2 ),
	CG_NEXT( pl1f_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl1f_air_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl1f_air_guard_01[] = {
	CG_FRAME(    0,    0,    3,           0,   176, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   177, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_air_guard_01 ),
};

const Sint32 pl1f_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   175, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	0x000014fc,
};

/* Char_Win 0
   Char_Win 2 */
const Sint32 pl1f_win_00[] = {
	CG_FRAME(    0,    0,    8,           0,   223, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   224, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   226, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   227, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   228, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   229, 0x80,   0x0c,    0,  0x2a,       0x0f,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   230, 0x80,   0x0c,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   231, 0x80,   0x0c,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
};

const Sint32 pl1f_win_00_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   232, 0x80,   0x0c,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_NEXT( pl1f_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl1f_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   32,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   40,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   219, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1f_win_01_loop[] = {
	CG_FRAME(    0,    0,    5,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_win_01_loop ),
};

const Sint32 pl1f_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   40,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   237, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   239, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   241, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   243, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000174c,
};

/* Char_Win 3
   Char_Win 4: win_time */
const Sint32 pl1f_win_03[] = {
	CG_FRAME(    0,    0,    8,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   226, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   227, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   228, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   229, 0x80,   0x0c,    0,  0x2a,       0x0f,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   230, 0x80,   0x0c,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   231, 0x80,   0x0c,    0,  0x2c,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   232, 0x80,   0x0c,    0,  0x2c,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   226, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   227, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   228, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   229, 0x80,   0x0c,    0,  0x2a,       0x0f,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   230, 0x80,   0x0c,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   231, 0x80,   0x0c,    0,  0x2c,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   232, 0x80,   0x0c,    0,  0x2c,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   226, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   227, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   228, 0x80,   0x00,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   229, 0x80,   0x0c,    0,  0x2a,       0x0f,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   230, 0x80,   0x0c,    0,  0x2a,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   231, 0x80,   0x0c,    0,  0x2c,          0,    0,   0,    14,      0,   0 ),
	CG_FRAME(    0,    0,   32,           0,   232, 0x80,   0x0c,    0,  0x2c,          0,    0,   0,    14,      0,   0 ),
};

const Sint32 pl1f_win_03_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   232, 0x80,   0x0c,    0,  0x00,          0,    0,   0,    14,      0,   0 ),
	CG_NEXT( pl1f_win_03_loop ),
};

const Sint32 pl1f_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   12,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00001b00,
};

const Sint32 pl1f_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    8,           0,   208, 0x80,   0x00,    0,  0x26,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   211, 0x80,   0x00,    0,  0x26,          0,    0,   0,     0,      0,   0 ),
	0x00001b40,
};

/* Char_Win 5: lose_time_ko
   Char_Win 6: draw_time */
const Sint32 pl1f_win_05[] = {
	CG_FRAME(    0,    0,    8,           0,   202, 0x80,   0x40,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   161, 0x80,   0x40,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
};

const Sint32 pl1f_win_05_loop[] = {
	CG_FRAME(    0,    0,   10,           0,   201, 0x80,   0x40,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   200, 0x80,   0x60,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   198, 0x80,   0x60,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   199, 0x80,   0x60,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   200, 0x80,   0x60,    0,  0x2e,          0,    0,   0,    18,      0,   0 ),
	CG_NEXT( pl1f_win_05_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl1f_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   208, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x30,   0x00,  ATK,  0x00,       0x11,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   209, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl1f_change_02[] = {
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl1f_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   191, 0x30,   0x00,    0,  0x00,       0x1d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x30,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1f_change_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   194, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x30,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   197, 0x30,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_change_03_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl1f_change_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,    84, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,    84, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1f_change_01 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl1f_change_04[] = {
	CG_FRAME(    0,    0,    5,           0,   158, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   203, 0x80,   0x40,    0,  0x00,       0x2b,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   204, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   205, 0x80,   0x40,    0,  0x00,       0x11,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
};

const Sint32 pl1f_change_04_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,    16,      0,   1 ),
	CG_NEXT( pl1f_change_04_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl1f_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0,   377, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl1f_extra_00_loop[] = {
	CG_FRAME( 0x84,    0,    6, END_OF_DATA,   377, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl1f_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl1f_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x1700,   2 ),
	CG_FRAME(    0,    0,    1,           0,   225, 0x30,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   223, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   224, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   225, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x00,  ATE,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   228, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   229, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   230, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x00,  ATE,  0x00,          0,   12,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   232, 0x80,   0x0c,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   164, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   165, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   166, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_extra_01 ),
};

/* Char_Extra 4: pl1f_upper */
const Sint32 pl1f_extra_04[] = {
	CG_FRAME(    0,    0,    5,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3,           0,   280, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   282, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1f_extra_04 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl1f_damage_00[] = {
	CG_FRAME(    0,    0,    7,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_NEXT( pl1f_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl1f_damage_01[] = {
	CG_FRAME(    0,    0,   10,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_NEXT( pl1f_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl1f_damage_02[] = {
	CG_FRAME(    0,    0,   12,           0,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_NEXT( pl1f_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl1f_damage_03[] = {
	CG_FRAME(    0,    0,    7,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_NEXT( pl1f_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl1f_damage_04[] = {
	CG_FRAME(    0,    0,   10,           0,   682, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_NEXT( pl1f_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl1f_damage_05[] = {
	CG_FRAME(    0,    0,   12,           0,   683, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   1 ),
	CG_NEXT( pl1f_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl1f_damage_06[] = {
	CG_FRAME(    0,    0,    7,           0,   684, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   684, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_NEXT( pl1f_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl1f_damage_07[] = {
	CG_FRAME(    0,    0,   10,           0,   685, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   684, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   684, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_NEXT( pl1f_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl1f_damage_08[] = {
	CG_FRAME(    0,    0,   12,           0,   686, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   685, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   685, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   684, 0x80,   0x40,    0,  0x00,          0,    0,   0,     2,      0,   2 ),
	CG_NEXT( pl1f_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl1f_damage_10[] = {
	CG_FRAME(    0,    0,    6,           0,   689, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   688, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_NEXT( pl1f_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl1f_damage_11[] = {
	CG_FRAME(    0,    0,    2,           0,   690, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   689, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   688, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_NEXT( pl1f_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl1f_damage_12[] = {
	CG_FRAME(    0,    0,    1,           0,   691, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   690, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   689, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   688, 0x80,   0x20,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_NEXT( pl1f_damage_12 ),
};

/* Char_Damage 9: apple_grab (Apple for You), dm_sweep */
const Sint32 pl1f_damage_09[] = {
	CG_FRAME(    0,    0,    4,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   705, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   706, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   705, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,  14 ),
};

const Sint32 pl1f_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,  14 ),
	CG_NEXT( pl1f_damage_09_loop ),
};

/* Char_Guard 3 */
const Sint32 pl1f_guard_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_guard_03 ),
};

/* Char_Guard 7 */
const Sint32 pl1f_guard_07[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   174, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1f_guard_07 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl1f_air_guard_02[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   177, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_air_guard_02 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl1f_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   711, 0x80,   0x00,    0,  0x00,          0,    0,   0,     4,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   712, 0x80,   0x00,    0,  0x00,          0,    0,   0,     4,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   713, 0x80,   0x00,    0,  0x00,          0,    0,   0,     4,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   714, 0x80,   0x00,    0,  0x00,          0,    0,   0,     4,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   715, 0x80,   0x00,    0,  0x00,          0,    0,   0,     4,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   716, 0x80,   0x00,    0,  0x00,          0,    0,   0,     4,      0,   3 ),
	CG_NEXT( pl1f_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl1f_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   719, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   720, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   721, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   722, 0x80,   0x00,    0,  0x00,          0,    0,   0,     2,      0,   3 ),
	CG_NEXT( pl1f_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl1f_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   710, 0x80,   0x40,    0,  0x00,          0,    0,   0,     4,      0,  14 ),
	CG_NEXT( pl1f_damage_31 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl1f_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl1f_damage_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl1f_damage_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl1f_damage_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   702, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl1f_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl1f_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl1f_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl1f_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl1f_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   702, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl1f_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl1f_damage_26[] = {
	CG_FRAME(    0,    0,    1,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    1,           0,   705, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   706, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   710, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl1f_damage_26 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl1f_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   693, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl1f_damage_13 ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl1f_damage_27[] = {
	CG_FRAME(    0,    0,    3,           0,   727, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   729, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl1f_damage_30[] = {
	CG_FRAME(    0,    0,    6,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1f_special_02_loop ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl1f_dizzy_00[] = {
	CG_FRAME(    0,    0,    4,           0,   723, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   724, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_dizzy_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl1f_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   657, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   658, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   660, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl1f_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   656, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   657, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   658, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   659, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   660, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   661, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   654, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   655, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_get_up_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl1f_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_get_up_00 ),
};

/* Char_Get_Up 3 */
const Sint32 pl1f_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_get_up_03 ),
};

/* Char_Caught 0 */
const Sint32 pl1f_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl1f_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl1f_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl1f_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl1f_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   682, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1f_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl1f_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   683, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1f_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl1f_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   684, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1f_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl1f_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   685, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1f_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl1f_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   686, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1f_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl1f_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   689, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1f_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl1f_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl1f_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl1f_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl1f_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl1f_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1f_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl1f_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl1f_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1f_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl1f_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   702, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1f_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl1f_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1f_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl1f_caught_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   711, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1f_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl1f_caught_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1f_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl1f_caught_20[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   710, 0x80,   0xc8,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl1f_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl1f_caught_21[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   695, 0x80,   0xc8,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl1f_caught_21 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl1f_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   248, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,  ATE,  0x02,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   251, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1f_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl1f_stand_p_03[] = {
	CG_FRAME(    0,    0,    3,           0,   398, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   399, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     6,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   400, 0x80,   0x00,  ATE,  0x02,          0,    0,   0,     6,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,  ATE,  0x02,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   402, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     6,      0,  11 ),
	CG_NEXT( pl1f_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl1f_stand_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   252, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    5,           0,   254, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3,           0,   252, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    3,           0,   256, 0x80,   0x00,  ATE,  0x06,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x00,  ATE,  0x06,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    7,           0,   258, 0x80,   0x00,  ATE,  0x06,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1f_stand_p_01 ),
};

/* Char_Stand_P 4 */
const Sint32 pl1f_stand_p_04[] = {
	CG_FRAME(    0,    0,    6,           0,   403, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    3,           0,   404, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     6,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   405, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     6,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   406, 0x80,   0x00,  ATE,  0x06,          0,    0,   0,     6,      0,  24 ),
	CG_FRAME(    0,    0,    6,           0,   407, 0x80,   0x00,  ATE,  0x06,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     6,      0,  11 ),
	CG_NEXT( pl1f_stand_p_04 ),
};

/* Char_Stand_P 2 (heavy)
   Char_Stand_P 5 */
const Sint32 pl1f_stand_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   261, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    1,           0,   262, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0x00,  ATK,  0x0a,       0x1e,    0,   0,     6,      0,  20 ),
	CG_FRAME(    0,    1,    2,           0,   263, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0200,  56 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0200,  20 ),
	CG_FRAME(    0,    2,    2,           0,   265, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0300,  56 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0300,  20 ),
	CG_FRAME(    0,    1,    2,           0,   267, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0400,  56 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0400,  20 ),
	CG_FRAME(    0,    2,    2,           0,   269, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0500,  56 ),
	CG_FRAME(    0,    0,    4,           0,   270, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0500,  20 ),
	CG_FRAME(    0,    1,    2,           0,   267, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0400,  56 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0400,  20 ),
	CG_FRAME(    0,    2,    2,           0,   265, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0300,  56 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0300,  20 ),
	CG_FRAME(    0,    1,    2,           0,   263, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6, 0x0200,  56 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     6, 0x0200,  20 ),
	CG_FRAME(    0,    0,   12,           0,   264, 0x80,   0x00,  ATE,  0x0a,       0x2b,    0,   0,     6,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   271, 0x80,   0x00,  ATE,  0x0a,       0x2b,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    3,           0,   272, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   273, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     6,      0,  11 ),
	CG_NEXT( pl1f_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl1f_stand_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3,           0,   275, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0, 0x0600,  25 ),
	CG_FRAME(    0,    0,    2,           0,   275, 0x80,   0x00,  ATE,  0x0e,          0,    0,   0,     0, 0x0600,  26 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x00,  ATE,  0x0e,          0,    0,   0,     0, 0x0600,  26 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   277, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0600,  11 ),
	CG_NEXT( pl1f_stand_k_00 ),
};

/* Char_Stand_K 3 */
const Sint32 pl1f_stand_k_03[] = {
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3,           0,   410, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   411, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0, 0x0700,  29 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x0e,          0,    0,   0,     0, 0x0700,  30 ),
	CG_FRAME(    0,    0,    2,           0,   413, 0x80,   0x00,  ATE,  0x0e,          0,    0,   0,     0, 0x0700,  11 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  11 ),
	CG_NEXT( pl1f_stand_k_03 ),
};

/* Char_Stand_K 1 (medium)
   Char_Stand_K 4 */
const Sint32 pl1f_stand_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   278, 0x80,   0x00,  ATK,  0x12,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x00,  ATK,  0x12,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3,           0,   280, 0x80,   0x00,  ATE,  0x12,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x00,  ATE,  0x12,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   282, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1f_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy)
   Char_Stand_K 5 */
const Sint32 pl1f_stand_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   283, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    5,           0,   284, 0x80,   0x00,  ATK,  0x0a,       0x20,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    1,           0,   285, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    1,           0,   285, 0x80,   0x00,  ATK,  0x0a,       0x04,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,   20,           0,   287, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    5,           0,   288, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   289, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     6,      0,  11 ),
	CG_NEXT( pl1f_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl1f_crouch_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   290, 0x80,   0x40,  ATK,  0x04,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   291, 0x80,   0x40,  ATK,  0x04,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x40,  ATE,  0x04,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0x40,  ATE,  0x04,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   293, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl1f_crouch_p_01[] = {
	CG_FRAME(    0,    0,    6,           0,   294, 0x80,   0x40,  ATK,  0x08,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x40,  ATK,  0x08,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   296, 0x80,   0x40,  ATK,  0x08,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   297, 0x80,   0x40,  ATE,  0x08,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    6,           0,   298, 0x80,   0x40,  ATE,  0x08,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   299, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl1f_crouch_p_02[] = {
	CG_FRAME(    0,    0,    1,           0,   300, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   303, 0x80,   0x40,  ATK,  0x0c,       0x1e,    0,   0,     6,      0,  35 ),
	CG_FRAME(    0,    1,    2,           0,   303, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0700,  57 ),
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0700,  35 ),
	CG_FRAME(    0,    2,    2,           0,   305, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0800,  57 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0800,  35 ),
	CG_FRAME(    0,    1,    2,           0,   307, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0900,  57 ),
	CG_FRAME(    0,    0,    4,           0,   308, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0900,  35 ),
	CG_FRAME(    0,    2,    2,           0,   309, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0a00,  57 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0a00,  35 ),
	CG_FRAME(    0,    1,    2,           0,   307, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0900,  57 ),
	CG_FRAME(    0,    0,    4,           0,   308, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0900,  35 ),
	CG_FRAME(    0,    2,    2,           0,   305, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0800,  57 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0800,  35 ),
	CG_FRAME(    0,    1,    2,           0,   303, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6, 0x0700,  57 ),
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x40,  ATE,  0x0c,          0,    0,   0,     6, 0x0700,  35 ),
	CG_FRAME(    0,    0,   14,           0,   304, 0x80,   0x40,  ATE,  0x0c,       0x2b,    0,   0,     6,      0,  35 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x40,  ATE,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   312, 0x80,   0x40,  ATE,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   313, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     6,      0,  12 ),
	CG_NEXT( pl1f_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl1f_crouch_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   314, 0x80,   0x40,  ATK,  0x10,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x40,  ATK,  0x10,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x40,  ATE,  0x10,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    3,           0,   317, 0x80,   0x40,  ATE,  0x10,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   317, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl1f_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x40,  ATK,  0x14,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   319, 0x80,   0x40,  ATK,  0x14,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    5,           0,   320, 0x80,   0x40,  ATE,  0x14,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0,   321, 0x80,   0x40,  ATE,  0x14,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   321, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl1f_crouch_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   322, 0x80,   0x40,  ATK,  0x16,       0x2b,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   323, 0x80,   0x40,  ATK,  0x16,       0x10,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x40,  ATK,  0x16,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   10,           0,   325, 0x80,   0x40,  ATE,  0x16,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   323, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3: jump_punch (3 + no) */
const Sint32 pl1f_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   10,           0,   327, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   328, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   329, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    3,    0,    3, END_OF_DATA,   326, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1f_jump_p_00 ),
};

/* Char_Jump_P 1 (medium) */
const Sint32 pl1f_jump_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   340, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   342, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   343, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    1,           0,   345, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    3,           0,   346, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   347, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    3,    0,    3, END_OF_DATA,   348, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1f_jump_p_01 ),
};

/* Char_Jump_P 4 */
const Sint32 pl1f_jump_p_04[] = {
	CG_FRAME(    0,    0,    6,           0,   380, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    1,           0,   382, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    4,           0,   383, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    3,    0,    3, END_OF_DATA,   384, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1f_jump_p_04 ),
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl1f_jump_p_02[] = {
	CG_FRAME(    0,    0,    2,           0,   349, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   350, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   351, 0x80,   0x20,  ATK,  0x00,       0x1e,    0,   0,     6,      0,  46 ),
	CG_FRAME(    0,    0,    2,           0,   351, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0b00,  58 ),
	CG_FRAME(    0,    0,    4,           0,   352, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0b00,  46 ),
	CG_FRAME(    0,    1,    2,           0,   351, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0b00,  58 ),
	CG_FRAME(    0,    0,    4,           0,   352, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0b00,  46 ),
	CG_FRAME(    0,    1,    2,           0,   351, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0b00,  58 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   352, 0x80,   0x20,  ATE,  0x00,       0x2b,    0,   0,     6,      0,  13 ),
	CG_NEXT( pl1f_jump_p_02 ),
};

/* Char_Jump_P 5 */
const Sint32 pl1f_jump_p_05[] = {
	CG_FRAME(    0,    0,    6,           0,   349, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   357, 0x80,   0x20,  ATK,  0x00,       0x1e,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   357, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0c00,  59 ),
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0c00,  49 ),
	CG_FRAME(    0,    1,    2,           0,   357, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0c00,  59 ),
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0c00,  49 ),
	CG_FRAME(    0,    1,    2,           0,   357, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6, 0x0c00,  59 ),
	CG_FRAME(    0,    0,   12,           0,   358, 0x80,   0x20,  ATE,  0x00,       0x2b,    0,   0,     6,      0,  49 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   357, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_NEXT( pl1f_jump_p_05 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3: jump_kick (3 + no) */
const Sint32 pl1f_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   359, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   360, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    8,           0,   361, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    1,           0,   361, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    3,    0,    3, END_OF_DATA,   359, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1f_jump_k_00 ),
};

/* Char_Jump_K 1 (medium) */
const Sint32 pl1f_jump_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   363, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   364, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   365, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  52 ),
	CG_FRAME(    0,    0,    3,           0,   366, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  52 ),
	CG_FRAME(    0,    0,    1,           0,   367, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0, 0x0d00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   366, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0, 0x0d00,  13 ),
	CG_FRAME(    0,    0,    1,           0,   368, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0, 0x0d00,  13 ),
	CG_FRAME(    3,    0,    1, END_OF_DATA,   369, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0, 0x0d00,  13 ),
	CG_NEXT( pl1f_jump_k_01 ),
};

/* Char_Jump_K 4 */
const Sint32 pl1f_jump_k_04[] = {
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   394, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    3,           0,   396, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    3,           0,   397, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    3,    0,    3, END_OF_DATA,   393, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl1f_jump_k_04 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl1f_jump_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   371, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   372, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   373, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   371, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   372, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   373, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   374, 0x80,   0x20,  ATK,  0x00,       0x1a,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   375, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   376, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,   10,           0,   377, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   378, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    3,    0,    4,           0,   379, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
};

const Sint32 pl1f_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_FRAME(    3,    0,    3, END_OF_DATA,   379, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     6,      0,  13 ),
	CG_NEXT( pl1f_jump_k_02_loop ),
};

/* Char_Stand_P 6: stand_punch */
const Sint32 pl1f_stand_p_06[] = {
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,    2,           0,   340, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   341, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   342, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   343, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    4,           0,   344, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,    1,           0,   345, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,    4,           0,   346, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,    4,           0,   347, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    3,           0,   348, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   158, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1f_stand_p_06 ),
};

/* Char_Stand_K 6 */
const Sint32 pl1f_stand_k_06[] = {
	CG_FRAME(    0,    0,    2,           0,   415, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,   -1,    2,           0,   416, 0x80,   0x00,  ATK,  0x16,       0x1d,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,   -1,    3,           0,   417, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    2,    2,           0,   418, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,    2,           0,   419, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,    2,           0,   418, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,    2,           0,   419, 0x80,   0x00,  ATE,  0x16,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   418, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   419, 0x80,   0x00,  ATE,  0x16,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    2,           0,   418, 0x80,   0x00,  ATK,  0x16,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,    1,           0,   420, 0x80,   0x00,  ATE,  0x16,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    5,           0,   421, 0x80,   0x00,  ATE,  0x16,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   422, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl1f_stand_k_06 ),
};

/* Char_Crouch_K 3 */
const Sint32 pl1f_crouch_k_03[] = {
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    8,           0,   424, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   425, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   426, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  63 ),
	CG_FRAME(    0,    0,   17,           0,   427, 0x80,   0x40,  ATK,  0x0c,          0,    0,   0,     6,      0,  63 ),
	CG_FRAME(    0,    0,    8,           0,   428, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     6,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   318, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     6,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   318, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     6,      0,   2 ),
	CG_NEXT( pl1f_crouch_k_03 ),
};

/* Char_Catch 0: pl1f_catch_set */
const Sint32 pl1f_catch_00[] = {
	CG_FRAME(    0,    0,    2,           0,   460, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    2,           0,   461, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    2,           0,   462, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,    4,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    4,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    4,           0,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,    4,           0,   442, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   24,           0,   444, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    2,   16,           0,   444, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
};

const Sint32 pl1f_catch_00_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   444, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_NEXT( pl1f_catch_00_loop ),
};

/* Char_Catch 1: catch_throw */
const Sint32 pl1f_catch_01[] = {
	CG_FRAME(    0,    0,   12,           0,   449, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   450, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1f_catch_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   456, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   455, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   454, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   459, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   458, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   457, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_catch_01_loop ),
};

/* Char_Catch 2 */
const Sint32 pl1f_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   464, 0x80,   0x00,    0,  0x00,       0x2b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24, END_OF_DATA,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_catch_02 ),
};

/* Char_Dash 0: act_dash
   Char_Dash 2 */
const Sint32 pl1f_dash_00[] = {
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x0d,    0,  0x00,       0x19,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1f_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x65,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x65,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x65,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x65,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x65,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   118, 0x80,   0x65,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl1f_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash
   Char_Dash 3 */
const Sint32 pl1f_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,    0,  0x00,       0x1d,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1f_dash_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_dash_01_loop ),
};

/* Char_Special 0: sp_missile (Smile & Missile) */
const Sint32 pl1f_special_00[] = {
	CG_FRAME(    0,    0,    1,           0,   503, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   504, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   505, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   506, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   507, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   508, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0, 0x81,    2,           0,   509, 0x80,   0x00,  ATK,  0x1c,       0x1b,    0,   0,     8, 0x0001,   1 ),
	CG_FRAME(    0,    2,    2,           0,   510, 0x80,   0x00,  ATK,  0x1c,       0x1f,    0,   0,     8, 0x0002,   1 ),
	CG_FRAME(    0,    0,    2,           0,   511, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   507, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    9,           0,   508, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   512, 0x80,   0x00,  ATE,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   513, 0x80,   0x00,  ATE,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   514, 0x80,   0x00,  ATE,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   515, 0x80,   0x00,  ATE,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   516, 0x80,   0x00,  ATE,  0x1c,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   162, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_00 ),
};

/* Char_Special 1: sp_happy (Smile & Missile (low; Happy & Missile)) */
const Sint32 pl1f_special_01[] = {
	CG_FRAME(    0,    0,    1,           0,   517, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   518, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   519, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   520, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   521, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   522, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0, 0x83,    2,           0,   523, 0x80,   0x00,  ATK,  0x0a,       0x1b,    0,   0,     8, 0x0003,   1 ),
	CG_FRAME(    0,    4,    2,           0,   524, 0x80,   0x00,  ATK,  0x0a,       0x1f,    0,   0,     8, 0x0004,   1 ),
	CG_FRAME(    0,    0,    2,           0,   525, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   521, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    9,           0,   522, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   526, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   527, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   528, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   529, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_01 ),
};

/* Char_Special 2: missile_j_init (Smile & Missile (jumping)) */
const Sint32 pl1f_special_02[] = {
	CG_FRAME(    0,    1,    3,           0,   158, 0x80,   0x00,  ATK,  0x1c,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   530, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   531, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   530, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   531, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   530, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   531, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   532, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,   -1,    5,           0,   533, 0x80,   0x20,  ATK,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0, 0x85,    1,           0,   533, 0x80,   0x20,  ATK,  0x1c,       0x1b,    0,   0,     8, 0x0005,   3 ),
	CG_FRAME(    0,    6,    3,           0,   534, 0x80,   0x20,  ATK,  0x1c,       0x1f,    0,   0,     8, 0x0006,   3 ),
	CG_FRAME(    0,    0,    4,           0,   535, 0x80,   0x20,  ATE,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   536, 0x80,   0x20,  ATE,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   537, 0x80,   0x20,  ATE,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   538, 0x80,   0x20,  ATE,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   537, 0x80,   0x20,  ATE,  0x1c,          0,    0,   0,     8,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   538, 0x80,   0x20,  ATE,  0x00,          0,    0,   0,     8,      0,   3 ),
	CG_NEXT( pl1f_special_02_loop ),
};

/* Char_Special 3: cheer_init (3 + atck_level) */
const Sint32 pl1f_special_03[] = {
	CG_FRAME(    0,   -1,    2,           0,   161, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    2,           0,   162, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    1,           0,   484, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   485, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   486, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   487, 0x80,   0x00,  ATK,  0x1e,       0x1d,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    2,    3,           0,   494, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_03_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   495, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   494, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  71 ),
	CG_NEXT( pl1f_special_03_loop ),
};

/* Char_Special 15: cheer_keep (15 + atck_level) */
const Sint32 pl1f_special_15[] = {
	CG_FRAME(    0,    0,    4,           0,   495, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   499, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   500, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_15_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    84, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_15_loop ),
};

/* Char_Special 4 */
const Sint32 pl1f_special_04[] = {
	CG_FRAME(    0,   -1,    2,           0,   161, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    2,           0,   162, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    2,           0,   484, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    2,           0,   485, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   486, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   487, 0x80,   0x00,  ATK,  0x1e,       0x1d,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    2,    3,           0,   494, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_04_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   495, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  71 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   494, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  71 ),
	CG_NEXT( pl1f_special_04_loop ),
};

/* Char_Special 16 */
const Sint32 pl1f_special_16[] = {
	CG_FRAME(    0,    0,    6,           0,   495, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   498, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   499, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   500, 0x80,   0x00,    0,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_16_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    84, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_16_loop ),
};

/* Char_Special 13: cheer_init (13 + atck_level) */
const Sint32 pl1f_special_13[] = {
	CG_FRAME(    0,   -1,    2,           0,   161, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    2,           0,   162, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    1,           0,   484, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   485, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   486, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   487, 0x80,   0x00,  ATK,  0x1e,       0x1d,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    2,    3,           0,   501, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_13_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  84 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   501, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  84 ),
	CG_NEXT( pl1f_special_13_loop ),
};

/* Char_Special 17: cheer_keep (17 + atck_level) */
const Sint32 pl1f_special_17[] = {
	CG_FRAME(    0,    0,    4,           0,   502, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   499, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_17_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    84, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_17_loop ),
};

/* Char_Special 14 */
const Sint32 pl1f_special_14[] = {
	CG_FRAME(    0,   -1,    2,           0,   161, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    2,           0,   162, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   2 ),
	CG_FRAME(    0,   -1,    2,           0,   484, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    2,           0,   485, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   486, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,   -1,    1,           0,   487, 0x80,   0x00,  ATK,  0x1e,       0x1d,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    2,    3,           0,   501, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_14_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  84 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   501, 0x80,   0x00,  ATK,  0x1e,          0,    0,   0,     8,      0,  84 ),
	CG_NEXT( pl1f_special_14_loop ),
};

/* Char_Special 18 */
const Sint32 pl1f_special_18[] = {
	CG_FRAME(    0,    0,    6,           0,   502, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   498, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   499, 0x80,   0x00,  ATE,  0x1e,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   500, 0x80,   0x00,    0,  0x1e,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_18_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    84, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_18_loop ),
};

/* Char_Special 5: shyness_init (Shyness & Strike)
   Char_Special 6
   Char_Special 7 */
const Sint32 pl1f_special_05[] = {
	CG_FRAME(    0,    1,    3,           0,   539, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x1000,   1 ),
	CG_FRAME(    0,    1,    3,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x1000,   1 ),
	CG_FRAME(    0,    1,    2,           0,   541, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x1000,   1 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   541, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x1000,   1 ),
	CG_NEXT( pl1f_special_05 ),
};

/* Char_Special 8
   Char_Special 9
   Char_Special 10: shyness_spd_set (Shyness & Strike) */
const Sint32 pl1f_special_08[] = {
	CG_FRAME(    0,    1,    4,           0,   542, 0x80,   0x00,  ATK,  0x00,       0x12,    0,   0,     8, 0x1300,   1 ),
	CG_FRAME(    0,    1,    4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x1300,   1 ),
	CG_FRAME(    0,    0,    4,           0,   543, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x1f00,  79 ),
	CG_FRAME(    0,    0,    4,           0,   544, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8, 0x2000,  80 ),
	CG_FRAME(    0,    0,    4,           0,   545, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8, 0x2100,  81 ),
	CG_FRAME(    0,    0,    5,           0,   546, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8, 0x2200,  81 ),
	CG_FRAME(    0,    0,    5,           0,   547, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8, 0x2300,  81 ),
	CG_FRAME(    0,    0,    5,           0,   548, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8, 0x2400,  81 ),
	CG_FRAME(    0,    0,    5,           0,   548, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8, 0x2500,  81 ),
	CG_FRAME(    0,    0,    5,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   550, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_special_08_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   551, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_special_08_loop ),
};

/* Char_Catch 11
   Char_Super 10: apple_init (Apple for You) */
const Sint32 pl1f_catch_11[] = {
	CG_FRAME(    0,    0,    4,           0,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    2,           0,   644, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   646, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x83,    0,    2,           0,   647, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   648, 0x80,   0x00,    0,  0x00,       0x1d,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_catch_11_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  75 ),
	CG_NEXT( pl1f_catch_11_loop ),
};

/* Char_Super 17: apple_dash (Apple for You) */
const Sint32 pl1f_super_17[] = {
	CG_FRAME(    0,    1,    4,           0,   651, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8,      0,  75 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   650, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8,      0,  75 ),
	CG_NEXT( pl1f_super_17 ),
};

/* Char_Super 18: apple_miss (Apple for You) */
const Sint32 pl1f_super_18[] = {
	CG_FRAME(    0,    0,    8,           0,   652, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,   32,           0,   653, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3,           0,   162, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl1f_super_18_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   163, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_super_18_loop ),
};

/* Char_Catch 12
   Char_Super 11: apple_hit_jump (Apple for You) */
const Sint32 pl1f_catch_12[] = {
	CG_FRAME(    0,    0,    2,           0,   654, 0x80,   0x00,    0,  0x00,       0x19,    0,   0,     8,      0,   0 ),
};

const Sint32 pl1f_catch_12_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   655, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   654, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_NEXT( pl1f_catch_12_loop ),
};

/* Char_Catch 16
   Char_Super 15: apple_hit_hop (Apple for You) */
const Sint32 pl1f_catch_16[] = {
	CG_FRAME(    0,   -1,    3,           0,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0021,   0 ),
	CG_FRAME(    0,   -1,    3,           0,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0021,   0 ),
	CG_FRAME(    0,    1,    3,           0,   664, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     8, 0x3b22,   0 ),
	CG_FRAME(    0,    0,    3,           0,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3b21,   0 ),
	CG_FRAME(    0,    1,    3,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3c22,   0 ),
	CG_FRAME(    0,    0,    3,           0,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3c21,   0 ),
	CG_FRAME(    0,    1,    3,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3d22,   0 ),
	CG_FRAME(    0,    0,    3,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3d21,   0 ),
	CG_FRAME(    0,    1,    3,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3c22,   0 ),
	CG_FRAME(    0,    0,    3,           0,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3c21,   0 ),
	CG_FRAME(    0,    1,    3,           0,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3b22,   0 ),
	CG_FRAME(    0,    0,    3,           0,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3b21,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3b22,   0 ),
	CG_NEXT( pl1f_catch_16 ),
};

/* Char_Catch 13
   Char_Super 12: apple_hit_fall (Apple for You) */
const Sint32 pl1f_catch_13[] = {
	CG_FRAME(    0,    1,    1,           0,   669, 0x80,   0x00,    0,  0x00,       0x2b,    0,   0,     8, 0x3d20,   0 ),
};

const Sint32 pl1f_catch_13_loop[] = {
	CG_FRAME(    0,    3,    4,           0,   670, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     8, 0x3822,   0 ),
	CG_FRAME(    0,    2,    4,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3821,   0 ),
	CG_FRAME(    0,    3,    4,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3922,   0 ),
	CG_FRAME(    0,    2,    4,           0,   673, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3921,   0 ),
	CG_FRAME(    0,    3,    4,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3a22,   0 ),
	CG_FRAME(    0,    2,    4,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3a21,   0 ),
	CG_FRAME(    0,    3,    4,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3822,   0 ),
	CG_FRAME(    0,    2,    4,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3821,   0 ),
	CG_FRAME(    0,    3,    4,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3922,   0 ),
	CG_FRAME(    0,    2,    4,           0,   673, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3921,   0 ),
	CG_FRAME(    0,    3,    4,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3a22,   0 ),
	CG_FRAME(    0,    2,    4, END_OF_DATA,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x3a21,   0 ),
	CG_NEXT( pl1f_catch_13_loop ),
};

/* Char_Catch 14
   Char_Super 13: apple_hit_keep (Apple for You) */
const Sint32 pl1f_catch_14[] = {
	CG_FRAME(    0,    0,   24,           0,   631, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,    0,  0x00,       0x2b,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,   633, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
};

const Sint32 pl1f_catch_14_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    1,    2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    1,    2,           0,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    1,    2,           0,   655, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   654, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,  77 ),
	CG_NEXT( pl1f_catch_14_loop ),
};

/* Char_Catch 15
   Char_Super 14: apple_dish (Apple for You) */
const Sint32 pl1f_catch_15[] = {
	CG_FRAME(    0,    0,   24, END_OF_DATA,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_catch_15 ),
};

/* Char_Super 16: apple_grab (Apple for You) */
const Sint32 pl1f_super_16[] = {
	CG_FRAME(    0,    1,    8,           0,   651, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   727, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   728, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   729, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
};

const Sint32 pl1f_super_16_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   654, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   655, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_NEXT( pl1f_super_16_loop ),
};

/* Char_Catch 3
   Char_Super 0: memory_init (Beautiful Memory) */
const Sint32 pl1f_catch_03[] = {
	CG_FRAME(    0,    1,    4,           0,   576, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1f_catch_03_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   577, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   578, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   579, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   576, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_catch_03_loop ),
};

/* Char_Catch 4
   Char_Super 1: memory_wait (Beautiful Memory) */
const Sint32 pl1f_catch_04[] = {
	CG_FRAME(    0,    0,    3,           0,   580, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8,      0,  73 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   581, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     8,      0,  73 ),
	CG_NEXT( pl1f_catch_04 ),
};

/* Char_Catch 5
   Char_Super 2: memory_dash (Beautiful Memory) */
const Sint32 pl1f_catch_05[] = {
	CG_FRAME(    0,    0,   16,           0,   631, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    4,           0,   633, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
};

const Sint32 pl1f_catch_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   639, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   638, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   637, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   636, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   635, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   634, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   641, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   640, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,  74 ),
	CG_NEXT( pl1f_catch_05_loop ),
};

/* Char_Catch 6
   Char_Super 3: memory_dash (Beautiful Memory) */
const Sint32 pl1f_catch_06[] = {
	CG_FRAME(    0,    0,    4,           0,   464, 0x80,   0x00,  ATE,  0x00,       0x2b,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   465, 0x80,   0x00,  ATE,  0x00,       0x13,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   466, 0x80,   0x00,  ATE,  0x00,       0x21,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,   24, END_OF_DATA,   467, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_catch_06 ),
};

/* Char_Catch 7
   Char_Super 4: memory_dash (Beautiful Memory) */
const Sint32 pl1f_catch_07[] = {
	CG_FRAME(    0,    0,    3,           0,   582, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x000e,   0 ),
	CG_FRAME(    0,    0,    3,           0,   583, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x000f,   0 ),
	CG_FRAME(    0,    1,    3,           0,   584, 0x80,   0x00,    0,  0x0a, 0x05 | CSE,    0,   0,     8, 0x0010,   0 ),
	CG_FRAME(    0,    0,    3,           0,   585, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x000e,   0 ),
	CG_FRAME(    0,    0,    3,           0,   586, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x000f,   0 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   587, 0x80,   0x00,    0,  0x0a, 0x05 | CSE,    0,   0,     8, 0x0010,   0 ),
	CG_NEXT( pl1f_catch_07 ),
};

/* Char_Catch 8
   Char_Super 5: memory_catch (Beautiful Memory) */
const Sint32 pl1f_catch_08[] = {
	CG_FRAME(    0,    0,    1,           0,   588, 0x80,   0x00,    0,  0x0a,       0x17,    0,   0,     8, 0x0011,   0 ),
};

const Sint32 pl1f_catch_08_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   588, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x0011,   0 ),
	CG_FRAME(    0,    0,    3,           0,   589, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x0011,   0 ),
	CG_FRAME(    0,    0,    3,           0,   611, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x0011,   0 ),
	CG_FRAME(    0,    0,    3,           0,   590, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x0011,   0 ),
	CG_FRAME(    0,    1,    3,           0,   591, 0x80,   0x00,    0,  0x0a, 0x02 | CSE,    0,   0,     8, 0x0012,   0 ),
	CG_FRAME(    0,    0,    3,           0,   592, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x0011,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   593, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     8, 0x0011,   0 ),
	CG_NEXT( pl1f_catch_08_loop ),
};

/* Char_Catch 9
   Char_Super 6: memory_catch (Beautiful Memory) */
const Sint32 pl1f_catch_09[] = {
	CG_FRAME(    0,    0,    6,           0,   594, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0013,   0 ),
	CG_FRAME(    0,    0,    6,           0,   595, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0014,   0 ),
	CG_FRAME(    0,    0,   32,           0,   596, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0015,   0 ),
	CG_FRAME(    0,    0,    6,           0,   597, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0016,   0 ),
	CG_FRAME(    0,    1,   32,           0,   598, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0017,   0 ),
	CG_FRAME(    0,    1,    4,           0,   599, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8, 0x0018,   0 ),
	CG_FRAME(    0, 0x80,    4,           0,   600, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     8,      0,   0 ),
};

const Sint32 pl1f_catch_09_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   601, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   602, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   0 ),
	CG_NEXT( pl1f_catch_09_loop ),
};

/* Char_Catch 10
   Char_Super 7: memory_fall (Beautiful Memory), apple_dish (Apple for You) */
const Sint32 pl1f_catch_10[] = {
	CG_FRAME(    0,    0,    5,           0,   158, 0x80,   0x00,    0,  0x00,       0x18,    0,   0,     8,      0,   1 ),
};

const Sint32 pl1f_catch_10_loop[] = {
	CG_FRAME(    0,    0,    5,           0,   603, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   604, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   605, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   606, 0x80,   0x00,    0,  0x00,       0x2b,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   607, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   608, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   609, 0x80,   0x00,    0,  0x00,          0,    0,   0,     8,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   610, 0x80,   0x00,    0,  0x00,       0x2e,    0,   0,     8,      0,   1 ),
	CG_NEXT( pl1f_catch_10_loop ),
};

/* Char_Super 8: hunting_init (Cool Hunting) */
const Sint32 pl1f_super_08[] = {
	CG_FRAME(    0,    0,    1,           0,   612, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   613, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   614, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   615, 0x80,   0x00,  ATK,  0x0a,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   616, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   10,           0,   616, 0x80,   0x00,  ATK,  0x0a,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   617, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1f_super_08_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,       0x1e,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0x00,  ATK,  0x0a,       0x1f,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    2,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    1,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    2,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,       0x1e,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0x00,  ATK,  0x0a,       0x1f,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    1,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    2,    2,           0,   618, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   619, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x2600,   1 ),
	CG_NEXT( pl1f_super_08_loop ),
};

/* Char_Super 9: hunting_shot (Cool Hunting) */
const Sint32 pl1f_super_09[] = {
	CG_FRAME(    0,    0,    3,           0,   620, 0x80,   0x00,  ATE,  0x0a,       0x2b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   621, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   622, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   623, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   624, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   620, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   621, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   625, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   626, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   627, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   628, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   629, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   630, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1f_super_09 ),
};

/* Char_Extra 2: snapback_init (Snap Back), fly_start5 */
const Sint32 pl1f_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x11,  ATK,  0x00,       0x17,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,    4,           0,   432, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   433, 0x80,   0x11,  ATK,  0x00, 0x17 | CSE,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4,           0,   434, 0x80,   0x6c,  ATE,  0x00, 0x2c | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    4,           0,   435, 0x80,   0x6c,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    8,           0,   436, 0x80,   0x6c,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   179, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_extra_02 ),
};

/* Char_Extra 5: nml_atck_dash */
const Sint32 pl1f_extra_05[] = {
	CG_FRAME(    0,    1,    2,           0,   429, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   430, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   431, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2,           0,   432, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2,           0,   433, 0x80,   0x11,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   434, 0x80,   0x6c,  ATE,  0x00, 0x2c | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    4,           0,   435, 0x80,   0x6c,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   12,           0,   436, 0x80,   0x6c,  ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    6,           0,   178, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   179, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl1f_extra_05 ),
};

/* Char_Extra 3: sp_chouhatsu (taunt), fly_start5 */
const Sint32 pl1f_extra_03[] = {
	CG_FRAME(    0,    0,    5,           0,   158, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   203, 0x80,   0x40,    0,  0x00,       0x2b,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   204, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   205, 0x80,   0x40,    0,  0x00,       0x11,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   206, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   205, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   206, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   207, 0x80,   0x40,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
};

const Sint32 pl1f_extra_03_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,    16,      0,   2 ),
	CG_NEXT( pl1f_extra_03_loop ),
};

/* group 27 0: pls1f_00_t0_init */
const Sint32 pl1f_grp27_00[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,    -1 ),
	CG_NEXT( pl1f_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl1f_grp27_01[] = {
	EF_FRAME(    0,    0,    2,           0,   861 ),
	EF_FRAME(    0,    0,    2,           0,   862 ),
	EF_FRAME(    0,    0,    2,           0,   863 ),
	EF_FRAME(    0,    0,    2,           0,   864 ),
	EF_FRAME(    0,    0,    2,           0,   865 ),
	EF_FRAME(    0,    0,    2,           0,   866 ),
	EF_FRAME(    0,    0,    2,           0,   867 ),
	EF_FRAME(    0,    0,    2,           0,   868 ),
	EF_FRAME(    0,    0,    2,           0,   869 ),
	EF_FRAME(    0,    0,    2,           0,   870 ),
	EF_FRAME(    0,    0,    2,           0,   871 ),
};

const Sint32 pl1f_grp27_01_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,   872 ),
	CG_NEXT( pl1f_grp27_01_loop ),
};

/* group 27 2 */
const Sint32 pl1f_grp27_02[] = {
	EF_FRAME(    0,    0,    2,           0,   873 ),
	EF_FRAME(    0,    0,    2,           0,   874 ),
	EF_FRAME(    0,    0,    2,           0,   875 ),
	EF_FRAME(    0,    0,    2,           0,   876 ),
	EF_FRAME(    0,    0,    2,           0,   877 ),
	EF_FRAME(    0,    0,    2,           0,   878 ),
	EF_FRAME(    0,    0,    2,           0,   879 ),
	EF_FRAME(    0,    0,    2,           0,   880 ),
	EF_FRAME(    0,    0,    2,           0,   881 ),
	EF_FRAME(    0,    0,    2,           0,   882 ),
	EF_FRAME(    0,    0,    2,           0,   883 ),
	EF_FRAME(    0,    0,    2,           0,   884 ),
};

const Sint32 pl1f_grp27_02_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,   885 ),
	CG_NEXT( pl1f_grp27_02_loop ),
};

/* group 27 3 */
const Sint32 pl1f_grp27_03[] = {
	EF_FRAME(    0,    0,    2,           0,   886 ),
	EF_FRAME(    0,    0,    2,           0,   887 ),
	EF_FRAME(    0,    0,    2,           0,   888 ),
	EF_FRAME(    0,    0,    2,           0,   889 ),
	EF_FRAME(    0,    0,    2,           0,   890 ),
	EF_FRAME(    0,    0,    2,           0,   891 ),
	EF_FRAME(    0,    0,    2,           0,   892 ),
	EF_FRAME(    0,    0,    2,           0,   893 ),
	EF_FRAME(    0,    0,    2,           0,   894 ),
	EF_FRAME(    0,    0,    2,           0,   895 ),
	EF_FRAME(    0,    0,    2,           0,   896 ),
};

const Sint32 pl1f_grp27_03_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,   897 ),
	CG_NEXT( pl1f_grp27_03_loop ),
};

/* group 27 4 */
const Sint32 pl1f_grp27_04[] = {
	EF_FRAME(    0,    0,    2,           0,   898 ),
	EF_FRAME(    0,    0,    2,           0,   899 ),
	EF_FRAME(    0,    0,    2,           0,   900 ),
	EF_FRAME(    0,    0,    2,           0,   901 ),
	EF_FRAME(    0,    0,    2,           0,   902 ),
	EF_FRAME(    0,    0,    2,           0,   903 ),
	EF_FRAME(    0,    0,    2,           0,   904 ),
	EF_FRAME(    0,    0,    2,           0,   905 ),
	EF_FRAME(    0,    0,    2,           0,   906 ),
	EF_FRAME(    0,    0,    2,           0,   907 ),
	EF_FRAME(    0,    0,    2,           0,   908 ),
	EF_FRAME(    0,    0,    2,           0,   909 ),
};

const Sint32 pl1f_grp27_04_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,   910 ),
	CG_NEXT( pl1f_grp27_04_loop ),
};

/* group 27 5 */
const Sint32 pl1f_grp27_05[] = {
	EF_FRAME(    0,    0,    2,           0,   767 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   768 ),
	CG_NEXT( pl1f_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl1f_grp27_06[] = {
	EF_FRAME(    0,    0,    2,           0,   769 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   770 ),
	CG_NEXT( pl1f_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl1f_grp27_07[] = {
	EF_FRAME(    0,    0,    2,           0,   771 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   772 ),
	CG_NEXT( pl1f_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl1f_grp27_08[] = {
	EF_FRAME(    0,    0,    2,           0,   773 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   774 ),
	CG_NEXT( pl1f_grp27_08 ),
};

/* group 27 9
   group 27 10: pls1f_00_t4_init
   group 27 11
   group 27 122: pls1f_00_t4_init */
const Sint32 pl1f_grp27_09[] = {
	EF_FRAME(    0,    0,    4,           0,   936 ),
	EF_FRAME(    0,    0,    4,           0,   937 ),
	EF_FRAME(    0,    1,    4, END_OF_DATA,   938 ),
	CG_NEXT( pl1f_grp27_09 ),
};

/* group 27 12 */
const Sint32 pl1f_grp27_12[] = {
	EF_FRAME(    0,    0,    4,           0,   945 ),
	EF_FRAME(    0,    0,    4,           0,   946 ),
	EF_FRAME(    0,    1,    4, END_OF_DATA,   947 ),
	CG_NEXT( pl1f_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl1f_grp27_13[] = {
	EF_FRAME(    0,    0,    4,           0,   954 ),
	EF_FRAME(    0,    0,    4,           0,   955 ),
	EF_FRAME(    0,    1,    4, END_OF_DATA,   956 ),
	CG_NEXT( pl1f_grp27_13 ),
};

/* group 27 14 */
const Sint32 pl1f_grp27_14[] = {
	EF_FRAME(    0,    0,    4,           0,   936 ),
	EF_FRAME(    0,    0,    4,           0,   946 ),
	EF_FRAME(    0,    1,    4, END_OF_DATA,   956 ),
	CG_NEXT( pl1f_grp27_14 ),
};

/* group 27 15 */
const Sint32 pl1f_grp27_15[] = {
	EF_FRAME(    0,    0,    8, END_OF_DATA,   939 ),
	CG_NEXT( pl1f_grp27_15 ),
};

/* group 27 104 */
const Sint32 pl1f_grp27_104[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   940 ),
	CG_NEXT( pl1f_grp27_104 ),
};

/* group 27 105 */
const Sint32 pl1f_grp27_105[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   941 ),
	CG_NEXT( pl1f_grp27_105 ),
};

/* group 27 106 */
const Sint32 pl1f_grp27_106[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   942 ),
	CG_NEXT( pl1f_grp27_106 ),
};

/* group 27 107 */
const Sint32 pl1f_grp27_107[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   943 ),
	CG_NEXT( pl1f_grp27_107 ),
};

/* group 27 108 */
const Sint32 pl1f_grp27_108[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   944 ),
	CG_NEXT( pl1f_grp27_108 ),
};

/* group 27 16 */
const Sint32 pl1f_grp27_16[] = {
	EF_FRAME(    0,    0,    8, END_OF_DATA,   948 ),
	CG_NEXT( pl1f_grp27_16 ),
};

/* group 27 109 */
const Sint32 pl1f_grp27_109[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   949 ),
	CG_NEXT( pl1f_grp27_109 ),
};

/* group 27 110 */
const Sint32 pl1f_grp27_110[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   950 ),
	CG_NEXT( pl1f_grp27_110 ),
};

/* group 27 111 */
const Sint32 pl1f_grp27_111[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   951 ),
	CG_NEXT( pl1f_grp27_111 ),
};

/* group 27 112 */
const Sint32 pl1f_grp27_112[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   952 ),
	CG_NEXT( pl1f_grp27_112 ),
};

/* group 27 113 */
const Sint32 pl1f_grp27_113[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   953 ),
	CG_NEXT( pl1f_grp27_113 ),
};

/* group 27 114 */
const Sint32 pl1f_grp27_114[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   944 ),
	CG_NEXT( pl1f_grp27_114 ),
};

/* group 27 17
   group 27 18 */
const Sint32 pl1f_grp27_17[] = {
	EF_FRAME(    0,    0,    8, END_OF_DATA,   957 ),
	CG_NEXT( pl1f_grp27_17 ),
};

/* group 27 115 */
const Sint32 pl1f_grp27_115[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   958 ),
	CG_NEXT( pl1f_grp27_115 ),
};

/* group 27 116 */
const Sint32 pl1f_grp27_116[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   959 ),
	CG_NEXT( pl1f_grp27_116 ),
};

/* group 27 117 */
const Sint32 pl1f_grp27_117[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   960 ),
	CG_NEXT( pl1f_grp27_117 ),
};

/* group 27 118 */
const Sint32 pl1f_grp27_118[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   961 ),
	CG_NEXT( pl1f_grp27_118 ),
};

/* group 27 119 */
const Sint32 pl1f_grp27_119[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   962 ),
	CG_NEXT( pl1f_grp27_119 ),
};

/* group 27 120 */
const Sint32 pl1f_grp27_120[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   953 ),
	CG_NEXT( pl1f_grp27_120 ),
};

/* group 27 121 */
const Sint32 pl1f_grp27_121[] = {
	EF_FRAME(    0,    1,    4, END_OF_DATA,   944 ),
	CG_NEXT( pl1f_grp27_121 ),
};

/* group 27 19: pls1f_01_dir_anm (19 + routine_3) */
const Sint32 pl1f_grp27_19[] = {
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32813 ),
	EF_FRAME(    0,    0,    4,           0, 32814 ),
	EF_FRAME(    0,    0,    4,           0, 32815 ),
	EF_FRAME(    0,    0,    4,           0, 32816 ),
	EF_FRAME(    0,    0,    4,           0, 32817 ),
	EF_FRAME(    0,    0,    4,           0, 32818 ),
	EF_FRAME(    0,    0,    4,           0, 32819 ),
	EF_FRAME(    0,    0,    4,           0, 32820 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32821 ),
	CG_NEXT( pl1f_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl1f_grp27_20[] = {
	EF_FRAME(    0,    0,    4,           0, 32802 ),
	EF_FRAME(    0,    0,    4,           0, 32803 ),
	EF_FRAME(    0,    0,    4,           0, 32804 ),
	EF_FRAME(    0,    0,    4,           0, 32805 ),
	EF_FRAME(    0,    0,    4,           0, 32806 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32809 ),
	EF_FRAME(    0,    0,    4,           0, 32810 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32811 ),
	CG_NEXT( pl1f_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl1f_grp27_21[] = {
	EF_FRAME(    0,    0,    4,           0, 32792 ),
	EF_FRAME(    0,    0,    4,           0, 32793 ),
	EF_FRAME(    0,    0,    4,           0, 32794 ),
	EF_FRAME(    0,    0,    4,           0, 32795 ),
	EF_FRAME(    0,    0,    4,           0, 32796 ),
	EF_FRAME(    0,    0,    4,           0, 32797 ),
	EF_FRAME(    0,    0,    4,           0, 32798 ),
	EF_FRAME(    0,    0,    4,           0, 32799 ),
	EF_FRAME(    0,    0,    4,           0, 32800 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32801 ),
	CG_NEXT( pl1f_grp27_21 ),
};

/* group 27 22 */
const Sint32 pl1f_grp27_22[] = {
	EF_FRAME(    0,    0,    4,           0, 32782 ),
	EF_FRAME(    0,    0,    4,           0, 32783 ),
	EF_FRAME(    0,    0,    4,           0, 32784 ),
	EF_FRAME(    0,    0,    4,           0, 32785 ),
	EF_FRAME(    0,    0,    4,           0, 32786 ),
	EF_FRAME(    0,    0,    4,           0, 32787 ),
	EF_FRAME(    0,    0,    4,           0, 32788 ),
	EF_FRAME(    0,    0,    4,           0, 32789 ),
	EF_FRAME(    0,    0,    4,           0, 32790 ),
	EF_FRAME(    0,    0,    4,           0, 32791 ),
};

const Sint32 pl1f_grp27_22_loop[] = {
	EF_FRAME(    0,    0,    4,           0, 32802 ),
	EF_FRAME(    0,    0,    4,           0, 32803 ),
	EF_FRAME(    0,    0,    4,           0, 32804 ),
	EF_FRAME(    0,    0,    4,           0, 32805 ),
	EF_FRAME(    0,    0,    4,           0, 32806 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32809 ),
	EF_FRAME(    0,    0,    4,           0, 32810 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32811 ),
	CG_NEXT( pl1f_grp27_22_loop ),
};

/* group 27 23 */
const Sint32 pl1f_grp27_23[] = {
	EF_FRAME(    0,    0,    4,           0, 32772 ),
	EF_FRAME(    0,    0,    4,           0, 32773 ),
	EF_FRAME(    0,    0,    4,           0, 32774 ),
	EF_FRAME(    0,    0,    4,           0, 32775 ),
	EF_FRAME(    0,    0,    4,           0, 32776 ),
	EF_FRAME(    0,    0,    4,           0, 32777 ),
	EF_FRAME(    0,    0,    4,           0, 32778 ),
	EF_FRAME(    0,    0,    4,           0, 32779 ),
	EF_FRAME(    0,    0,    4,           0, 32780 ),
	EF_FRAME(    0,    0,    4,           0, 32781 ),
};

const Sint32 pl1f_grp27_23_loop[] = {
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32813 ),
	EF_FRAME(    0,    0,    4,           0, 32814 ),
	EF_FRAME(    0,    0,    4,           0, 32815 ),
	EF_FRAME(    0,    0,    4,           0, 32816 ),
	EF_FRAME(    0,    0,    4,           0, 32817 ),
	EF_FRAME(    0,    0,    4,           0, 32818 ),
	EF_FRAME(    0,    0,    4,           0, 32819 ),
	EF_FRAME(    0,    0,    4,           0, 32820 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32821 ),
	CG_NEXT( pl1f_grp27_23_loop ),
};

/* group 27 24: pls1f_03_t2
   group 27 25
   group 27 26
   group 27 27 */
const Sint32 pl1f_grp27_24[] = {
	EF_FRAME(    0,    0,    4,           0,   443 ),
	EF_FRAME(    0,    0,   24,           0,   445 ),
	EF_FRAME(    0,    0,    4,           0,   446 ),
	EF_FRAME(    0,    0,    4,           0,   447 ),
};

const Sint32 pl1f_grp27_24_loop[] = {
	EF_FRAME(    0,    0,    9, END_OF_DATA,   448 ),
	CG_NEXT( pl1f_grp27_24_loop ),
};

/* group 27 28 */
const Sint32 pl1f_grp27_28[] = {
	EF_FRAME(    0,    0,    2,           0,   775 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   776 ),
	CG_NEXT( pl1f_grp27_28 ),
};

/* group 27 29 */
const Sint32 pl1f_grp27_29[] = {
	EF_FRAME(    0,    0,    2,           0,   777 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   778 ),
	CG_NEXT( pl1f_grp27_29 ),
};

/* group 27 30 */
const Sint32 pl1f_grp27_30[] = {
	EF_FRAME(    0,    0,    2,           0,   779 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   780 ),
	CG_NEXT( pl1f_grp27_30 ),
};

/* group 27 31 */
const Sint32 pl1f_grp27_31[] = {
	EF_FRAME(    0,    0,    3,           0,   911 ),
	EF_FRAME(    0,    0,    3,           0,   912 ),
	EF_FRAME(    0,    0,    3,           0,   913 ),
	EF_FRAME(    0,    0,    3,           0,   914 ),
	EF_FRAME(    0,    0,    3,           0,   915 ),
	EF_FRAME(    0,    0,    3,           0,   916 ),
	EF_FRAME(    0,    0,    3,           0,   917 ),
	EF_FRAME(    0,    0,    3,           0,   918 ),
	EF_FRAME(    0,    0,    3,           0,   919 ),
	EF_FRAME(    0,    0,    3,           0,   920 ),
	EF_FRAME(    0,    0,    3,           0,   921 ),
};

const Sint32 pl1f_grp27_31_loop[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA,   922 ),
	CG_NEXT( pl1f_grp27_31_loop ),
};

/* group 27 32 */
const Sint32 pl1f_grp27_32[] = {
	EF_FRAME(    0,    0,    3,           0,   923 ),
	EF_FRAME(    0,    0,    3,           0,   924 ),
	EF_FRAME(    0,    0,    3,           0,   925 ),
	EF_FRAME(    0,    0,    3,           0,   926 ),
	EF_FRAME(    0,    0,    3,           0,   927 ),
	EF_FRAME(    0,    0,    3,           0,   928 ),
	EF_FRAME(    0,    0,    3,           0,   929 ),
	EF_FRAME(    0,    0,    3,           0,   930 ),
	EF_FRAME(    0,    0,    3,           0,   931 ),
	EF_FRAME(    0,    0,    3,           0,   932 ),
	EF_FRAME(    0,    0,    3,           0,   933 ),
	EF_FRAME(    0,    0,    3,           0,   934 ),
};

const Sint32 pl1f_grp27_32_loop[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA,   935 ),
	CG_NEXT( pl1f_grp27_32_loop ),
};

/* group 27 33: pls1f_03_t3 */
const Sint32 pl1f_grp27_33[] = {
	EF_FRAME(    0,    0,    2,           0,  1001 ),
	EF_FRAME(    0,    0,    2,           0,  1002 ),
	EF_FRAME(    0,    0,    2,           0,  1003 ),
	EF_FRAME(    0,    0,    2,           0,  1004 ),
	EF_FRAME(    0,    0,    2,           0,  1005 ),
	EF_FRAME(    0,    0,    2,           0,  1006 ),
	EF_FRAME(    0,    0,    2,           0,  1007 ),
	EF_FRAME(    0,    0,    2,           0,  1008 ),
	EF_FRAME(    0,    0,    2,           0,  1009 ),
	EF_FRAME(    0,    0,    2,           0,  1010 ),
	EF_FRAME(    0,    0,    2,           0,  1011 ),
	EF_FRAME(    0,    0,    2,           0,  1012 ),
	EF_FRAME(    0,    0,    2,           0,  1013 ),
	EF_FRAME(    0,    0,    2,           0,  1014 ),
	EF_FRAME(    0,    0,    2,           0,  1015 ),
	EF_FRAME(    0,    0,    2,           0,  1016 ),
	EF_FRAME(    0,    0,    2,           0,  1017 ),
	EF_FRAME(    0,    0,    2,           0,  1018 ),
	EF_FRAME(    0,    0,    2,           0,  1019 ),
	EF_FRAME(    0,    0,    2,           0,  1020 ),
	EF_FRAME(    0,    0,    2,           0,  1021 ),
};

const Sint32 pl1f_grp27_33_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1022 ),
	EF_FRAME(    0,    0,    2,           0,  1023 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1024 ),
	CG_NEXT( pl1f_grp27_33_loop ),
};

/* group 27 34 */
const Sint32 pl1f_grp27_34[] = {
	EF_FRAME(    0,    0,    3,           0,  1025 ),
	EF_FRAME(    0,    0,    3,           0,  1026 ),
	EF_FRAME(    0,    0,    3,           0,  1027 ),
};

const Sint32 pl1f_grp27_34_loop[] = {
	EF_FRAME(    0,    0,    3,           0,  1028 ),
	EF_FRAME(    0,    0,    3,           0,  1029 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,  1030 ),
	CG_NEXT( pl1f_grp27_34_loop ),
};

/* group 27 35 */
const Sint32 pl1f_grp27_35[] = {
	EF_FRAME(    0,    0,    3,           0,  1031 ),
	EF_FRAME(    0,    0,   16,           0,  1032 ),
	EF_FRAME(    0,    1,    3,           0,  1033 ),
	EF_FRAME(    0,    0,    3,           0,  1034 ),
	EF_FRAME(    0,    0,    3,           0,  1035 ),
};

const Sint32 pl1f_grp27_35_loop[] = {
	EF_FRAME(    0,    0,    3,           0,  1036 ),
	EF_FRAME(    0,    0,    3,           0,  1037 ),
	EF_FRAME(    0,    0,    3,           0,  1038 ),
	EF_FRAME(    0,    0,    3,           0,  1039 ),
	EF_FRAME(    0,    0,    3,           0,  1040 ),
	EF_FRAME(    0,    0,    3,           0,  1041 ),
	EF_FRAME(    0,    0,    3,           0,  1042 ),
	EF_FRAME(    0,    0,    3,           0,  1043 ),
	EF_FRAME(    0,    0,    3,           0,  1044 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,  1045 ),
	CG_NEXT( pl1f_grp27_35_loop ),
};

/* group 27 36: pls1f_03_t3 */
const Sint32 pl1f_grp27_36[] = {
	EF_FRAME(    0,    0,    3,           0,  1046 ),
	EF_FRAME(    0,    0,    3,           0,  1047 ),
	EF_FRAME(    0,    0,    3,           0,  1048 ),
	EF_FRAME(    0,    0,    3,           0,  1049 ),
	EF_FRAME(    0,    0,    3,           0,  1050 ),
	EF_FRAME(    0,    0,    3,           0,  1051 ),
	EF_FRAME(    0,    0,    3,           0,  1052 ),
	EF_FRAME(    0,    0,    3,           0,  1053 ),
	EF_FRAME(    0,    0,    3,           0,  1054 ),
	EF_FRAME(    0,    0,    3,           0,  1055 ),
	EF_FRAME(    0,    0,    3,           0,  1056 ),
	EF_FRAME(    0,    0,    3,           0,  1057 ),
};

const Sint32 pl1f_grp27_36_loop[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA,  1058 ),
	CG_NEXT( pl1f_grp27_36_loop ),
};

/* group 27 37: pls1f_03_t4 */
const Sint32 pl1f_grp27_37[] = {
	EF_FRAME(    0,    0,    2,           0,  1382 ),
	EF_FRAME(    0,    0,    2,           0,  1383 ),
	EF_FRAME(    0,    0,    2,           0,  1384 ),
	EF_FRAME(    0,    0,    2,           0,  1385 ),
	EF_FRAME(    0,    0,    2,           0,  1386 ),
	EF_FRAME(    0,    0,    2,           0,  1387 ),
	EF_FRAME(    0,    1,    2,           0,  1388 ),
	EF_FRAME(    0,    1,    2,           0,  1389 ),
	EF_FRAME(    0,    1,    2,           0,  1390 ),
	EF_FRAME(    0,    1,    2,           0,  1391 ),
	EF_FRAME(    0,    1,    2,           0,  1388 ),
	EF_FRAME(    0,    1,    2,           0,  1389 ),
	EF_FRAME(    0,    1,    2,           0,  1390 ),
	EF_FRAME(    0,    1,    2,           0,  1391 ),
	EF_FRAME(    0,    1,    2,           0,  1388 ),
	EF_FRAME(    0,    1,    2,           0,  1389 ),
	EF_FRAME(    0,    1,    2,           0,  1390 ),
	EF_FRAME(    0,    1,    2,           0,  1391 ),
	EF_FRAME(    0,    0,    2,           0,  1387 ),
	EF_FRAME(    0,    0,    2,           0,  1386 ),
	EF_FRAME(    0,    0,    2,           0,  1385 ),
	EF_FRAME(    0,    0,    2,           0,  1384 ),
	EF_FRAME(    0,    0,    2,           0,  1383 ),
};

const Sint32 pl1f_grp27_37_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1382 ),
	CG_NEXT( pl1f_grp27_37_loop ),
};

/* group 27 38: pls1f_03_t5 */
const Sint32 pl1f_grp27_38[] = {
	EF_FRAME(    0,    0,    2,           0,  1138 ),
	EF_FRAME(    0,    0,    2,           0,  1139 ),
	EF_FRAME(    0,    0,    2,           0,  1140 ),
	EF_FRAME(    0,    0,    2,           0,  1141 ),
	EF_FRAME(    0,    0,    2,           0,  1142 ),
	EF_FRAME(    0,    0,    2,           0,  1143 ),
	EF_FRAME(    0,    0,    2,           0,  1144 ),
	EF_FRAME(    0,    0,    2,           0,  1145 ),
	EF_FRAME(    0,    0,    2,           0,  1146 ),
	EF_FRAME(    0,    0,    2,           0,  1147 ),
	EF_FRAME(    0,    0,    2,           0,  1148 ),
	EF_FRAME(    0,    0,    2,           0,  1149 ),
	EF_FRAME(    0,    0,    2,           0,  1150 ),
	EF_FRAME(    0,    0,    2,           0,  1151 ),
	EF_FRAME(    0,    0,    2,           0,  1152 ),
};

const Sint32 pl1f_grp27_38_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1153 ),
	CG_NEXT( pl1f_grp27_38_loop ),
};

/* group 27 39 */
const Sint32 pl1f_grp27_39[] = {
	EF_FRAME(    0,    0,    2,           0,   788 ),
	EF_FRAME(    0,    0,    2,           0,   789 ),
	EF_FRAME(    0,    0,    2,           0,   790 ),
	EF_FRAME(    0,    0,    2,           0,   791 ),
	EF_FRAME(    0,    0,    2,           0,   792 ),
	EF_FRAME(    0,    0,    2,           0,   793 ),
	EF_FRAME(    0,    0,    2,           0,   794 ),
	EF_FRAME(    0,    0,    2,           0,   795 ),
	EF_FRAME(    0,    0,    2,           0,   796 ),
	EF_FRAME(    0,    0,    2,           0,   797 ),
	EF_FRAME(    0,    0,    2,           0,   798 ),
};

const Sint32 pl1f_grp27_39_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,   799 ),
	CG_NEXT( pl1f_grp27_39_loop ),
};

/* group 27 40 */
const Sint32 pl1f_grp27_40[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,   627 ),
	CG_NEXT( pl1f_grp27_40 ),
};

/* group 27 41: pls1f_00_t5_init */
const Sint32 pl1f_grp27_41[] = {
	EF_FRAME(    0,    0,    6,           0,    -1 ),
	EF_FRAME(    0,    0,    2,           0,  1105 ),
	EF_FRAME(    0,    0,    2,           0,  1106 ),
	EF_FRAME(    0,    0,    4,           0,  1107 ),
	EF_FRAME(    0,    0,    6,           0,  1108 ),
	EF_FRAME(    0,    0,    2,           0,  1102 ),
	EF_FRAME(    0,    0,    2,           0, 32830 ),
	EF_FRAME(    0,    0,    2,           0, 32831 ),
};

const Sint32 pl1f_grp27_41_loop[] = {
	EF_FRAME(    0,    1,    6,           0, 32832 ),
	EF_FRAME(    0,    1,    3,           0, 32833 ),
	EF_FRAME(    0,    1,    3, END_OF_DATA, 32831 ),
	CG_NEXT( pl1f_grp27_41_loop ),
};

/* group 27 42: pls1f_00_t5_move */
const Sint32 pl1f_grp27_42[] = {
	EF_FRAME(    0,   -1,    6,           0, 32834 ),
	EF_FRAME(    0,   -1,    6,           0, 32835 ),
};

const Sint32 pl1f_grp27_42_loop[] = {
	EF_FRAME(    0,   -1,   10, END_OF_DATA,  1102 ),
	CG_NEXT( pl1f_grp27_42_loop ),
};

/* group 27 43: pls1f_00_t6_init, pls1f_00_t7_init */
const Sint32 pl1f_grp27_43[] = {
	EF_FRAME(    0,    0,    8,           0, 32836 ),
	EF_FRAME(    0,    0,    8, END_OF_DATA, 32837 ),
	CG_NEXT( pl1f_grp27_43 ),
};

/* group 27 44: pls1f_00_t8_init */
const Sint32 pl1f_grp27_44[] = {
	EF_FRAME(    0,    0,    2,           0,  1111 ),
	EF_FRAME(    0,    0,    2,           0,  1112 ),
	EF_FRAME(    0,    0,    2,           0,  1113 ),
	EF_FRAME(    0,    0,    2,           0,  1114 ),
	EF_FRAME(    0,    0,    2,           0,  1115 ),
	EF_FRAME(    0,    0,    1,           0,  1116 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,  1117 ),
	CG_NEXT( pl1f_grp27_44 ),
};

/* group 27 46 */
const Sint32 pl1f_grp27_46[] = {
	EF_FRAME(    0,    0,   16,           0,    -1 ),
};

/* group 27 45 */
const Sint32 pl1f_grp27_45[] = {
	EF_FRAME(    0,    0,    2,           0,  1079 ),
	EF_FRAME(    0,    0,    2,           0,  1080 ),
	EF_FRAME(    0,    0,    2,           0,  1081 ),
	EF_FRAME(    0,    0,    2,           0,  1076 ),
	EF_FRAME(    0,    0,    2,           0,  1077 ),
	EF_FRAME(    0,    0,    2,           0,  1078 ),
	EF_FRAME(    0,    0,    2,           0,  1073 ),
	EF_FRAME(    0,    0,    2,           0,  1074 ),
	EF_FRAME(    0,    0,    4,           0,  1075 ),
};

const Sint32 pl1f_grp27_45_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1070 ),
	EF_FRAME(    0,    0,    2,           0,  1071 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1072 ),
	CG_NEXT( pl1f_grp27_45_loop ),
};

/* group 27 47 */
const Sint32 pl1f_grp27_47[] = {
	EF_FRAME(    0,    0,   32,           0,    -1 ),
	EF_FRAME(    0,    0,    2,           0,  1079 ),
	EF_FRAME(    0,    0,    2,           0,  1080 ),
	EF_FRAME(    0,    0,    2,           0,  1081 ),
	EF_FRAME(    0,    0,    4,           0,  1076 ),
	EF_FRAME(    0,    0,    4,           0,  1077 ),
	EF_FRAME(    0,    0,    4,           0,  1078 ),
};

const Sint32 pl1f_grp27_47_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1073 ),
	EF_FRAME(    0,    0,    2,           0,  1074 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1075 ),
	CG_NEXT( pl1f_grp27_47_loop ),
};

/* group 27 48 */
const Sint32 pl1f_grp27_48[] = {
	EF_FRAME(    0,    0,   56,           0,    -1 ),
};

const Sint32 pl1f_grp27_48_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1076 ),
	EF_FRAME(    0,    0,    2,           0,  1077 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1078 ),
	CG_NEXT( pl1f_grp27_48_loop ),
};

/* group 27 55 */
const Sint32 pl1f_grp27_55[] = {
	EF_FRAME(    0,    0,   56,           0,    -1 ),
};

const Sint32 pl1f_grp27_55_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1079 ),
	EF_FRAME(    0,    0,    2,           0,  1080 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1081 ),
	CG_NEXT( pl1f_grp27_55_loop ),
};

/* group 27 56 */
const Sint32 pl1f_grp27_56[] = {
	EF_FRAME(    0,    0,   56,           0,    -1 ),
	EF_FRAME(    0,    0,    2,           0,  1079 ),
	EF_FRAME(    0,    0,    2,           0,  1080 ),
	EF_FRAME(    0,    0,    2,           0,  1081 ),
};

const Sint32 pl1f_grp27_56_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1076 ),
	EF_FRAME(    0,    0,    2,           0,  1077 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1078 ),
	CG_NEXT( pl1f_grp27_56_loop ),
};

/* group 27 57 */
const Sint32 pl1f_grp27_57[] = {
	EF_FRAME(    0,    0,   56,           0,    -1 ),
	EF_FRAME(    0,    0,    2,           0,  1079 ),
	EF_FRAME(    0,    0,    2,           0,  1080 ),
	EF_FRAME(    0,    0,    2,           0,  1081 ),
	EF_FRAME(    0,    0,    2,           0,  1076 ),
	EF_FRAME(    0,    0,    2,           0,  1077 ),
	EF_FRAME(    0,    0,    2,           0,  1078 ),
};

const Sint32 pl1f_grp27_57_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1073 ),
	EF_FRAME(    0,    0,    2,           0,  1074 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1075 ),
	CG_NEXT( pl1f_grp27_57_loop ),
};

/* group 27 58 */
const Sint32 pl1f_grp27_58[] = {
	EF_FRAME(    0,    0,   56,           0,    -1 ),
	EF_FRAME(    0,    0,    2,           0,  1079 ),
	EF_FRAME(    0,    0,    2,           0,  1080 ),
	EF_FRAME(    0,    0,    2,           0,  1081 ),
	EF_FRAME(    0,    0,    2,           0,  1076 ),
	EF_FRAME(    0,    0,    2,           0,  1077 ),
	EF_FRAME(    0,    0,    2,           0,  1078 ),
	EF_FRAME(    0,    0,    2,           0,  1073 ),
	EF_FRAME(    0,    0,    2,           0,  1074 ),
	EF_FRAME(    0,    0,    2,           0,  1075 ),
};

const Sint32 pl1f_grp27_58_loop[] = {
	EF_FRAME(    0,    0,    2,           0,  1070 ),
	EF_FRAME(    0,    0,    2,           0,  1071 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1072 ),
	CG_NEXT( pl1f_grp27_58_loop ),
};

/* group 27 49 */
const Sint32 pl1f_grp27_49[] = {
	EF_FRAME(    0,    0,    4,           0,  1090 ),
	EF_FRAME(    0,    0,    4,           0,  1091 ),
	EF_FRAME(    0,    0,    4,           0,  1092 ),
};

const Sint32 pl1f_grp27_49_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1093 ),
	CG_NEXT( pl1f_grp27_49_loop ),
};

/* group 27 50 */
const Sint32 pl1f_grp27_50[] = {
	EF_FRAME(    0,    0,    4,           0,  1088 ),
	EF_FRAME(    0,    0,    4,           0,  1089 ),
	EF_FRAME(    0,    0,    4,           0,  1090 ),
	EF_FRAME(    0,    0,    4,           0,  1091 ),
	EF_FRAME(    0,    0,    4,           0,  1092 ),
};

const Sint32 pl1f_grp27_50_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1093 ),
	CG_NEXT( pl1f_grp27_50_loop ),
};

/* group 27 51 */
const Sint32 pl1f_grp27_51[] = {
	EF_FRAME(    0,    0,    4,           0,  1084 ),
	EF_FRAME(    0,    0,    4,           0,  1085 ),
	EF_FRAME(    0,    0,    4,           0,  1086 ),
};

const Sint32 pl1f_grp27_51_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1087 ),
	CG_NEXT( pl1f_grp27_51_loop ),
};

/* group 27 52 */
const Sint32 pl1f_grp27_52[] = {
	EF_FRAME(    0,    0,    4,           0,  1082 ),
	EF_FRAME(    0,    0,    4,           0,  1083 ),
	EF_FRAME(    0,    0,    4,           0,  1084 ),
	EF_FRAME(    0,    0,    4,           0,  1085 ),
	EF_FRAME(    0,    0,    4,           0,  1086 ),
};

const Sint32 pl1f_grp27_52_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1087 ),
	CG_NEXT( pl1f_grp27_52_loop ),
};

/* group 27 53: pls1f_03_t14 */
const Sint32 pl1f_grp27_53[] = {
	EF_FRAME(    0,    0,    4,           0,  1155 ),
	EF_FRAME(    0,    0,    4,           0,  1156 ),
	EF_FRAME(    0,    0,    4,           0,  1155 ),
	EF_FRAME(    0,    0,    4,           0,  1154 ),
	EF_FRAME(    0,    0,    4,           0,  1155 ),
	EF_FRAME(    0,    0,    4,           0,  1156 ),
	EF_FRAME(    0,    0,    4,           0,  1155 ),
	EF_FRAME(    0,    0,    4,           0,  1154 ),
	EF_FRAME(    0,    0,   12,           0,  1157 ),
	EF_FRAME(    0,    0,    8,           0,  1158 ),
	EF_FRAME(    0,    0,    8,           0,  1159 ),
	EF_FRAME(    0,    0,    8,           0,  1160 ),
	EF_FRAME(    0,    0,   20,           0,  1161 ),
};

const Sint32 pl1f_grp27_53_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1162 ),
	CG_NEXT( pl1f_grp27_53_loop ),
};

/* group 27 54 */
const Sint32 pl1f_grp27_54[] = {
	EF_FRAME(    0,    0,    1,           0,  1163 ),
	EF_FRAME(    0,    0,    1,           0,  1164 ),
	EF_FRAME(    0,    0,    1,           0,  1165 ),
	EF_FRAME(    0,    0,    1,           0,  1166 ),
	EF_FRAME(    0,    0,    1,           0,  1167 ),
	EF_FRAME(    0,    0,    1,           0,  1168 ),
	EF_FRAME(    0,    0,    1,           0,  1169 ),
	EF_FRAME(    0,    0,    1,           0,  1170 ),
	EF_FRAME(    0,    0,    1,           0,  1171 ),
	EF_FRAME(    0,    0,    1,           0,  1172 ),
	EF_FRAME(    0,    0,    1,           0,  1173 ),
	EF_FRAME(    0,    0,   48,           0,    -1 ),
	EF_FRAME(    0,    0,    6,           0,  1094 ),
	EF_FRAME(    0,    0,    6,           0,  1095 ),
	EF_FRAME(    0,    0,    6,           0,  1096 ),
};

const Sint32 pl1f_grp27_54_loop[] = {
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1097 ),
	CG_NEXT( pl1f_grp27_54_loop ),
};

/* group 27 59: pls1f_00_t3_init */
const Sint32 pl1f_grp27_59[] = {
	EF_FRAME(    0,    0,    2,           0,    -1 ),
	EF_FRAME(    0,    0,    3,           0, 32842 ),
	EF_FRAME(    0,    0,    3,           0, 32843 ),
	EF_FRAME(    0,    0,    3,           0, 32844 ),
	EF_FRAME(    0,    0,    3,           0, 32845 ),
	EF_FRAME(    0,    0,    3,           0, 32846 ),
	EF_FRAME(    0,    0,    3,           0, 32847 ),
	EF_FRAME(    0,    0,    3,           0, 32848 ),
	EF_FRAME(    0,    0,    3,           0, 32849 ),
	EF_FRAME(    0,    0,    3,           0, 32850 ),
};

const Sint32 pl1f_grp27_59_loop[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA, 32851 ),
	CG_NEXT( pl1f_grp27_59_loop ),
};

/* group 27 60 */
const Sint32 pl1f_grp27_60[] = {
	EF_FRAME(    0,    0,    3,           0,  1345 ),
	EF_FRAME(    0,    0,    3,           0,  1346 ),
	EF_FRAME(    0,    0,    3,           0,  1347 ),
	EF_FRAME(    0,    0,    3,           0,  1348 ),
};

/* group 27 61 */
const Sint32 pl1f_grp27_61[] = {
	EF_FRAME(    0,    0,    6,           0,  1349 ),
	EF_FRAME(    0,    0,    6,           0,  1350 ),
	EF_FRAME(    0,    0,    6,           0,  1351 ),
	EF_FRAME(    0,    0,    6,           0,  1352 ),
	EF_FRAME(    0,    0,    6,           0,  1353 ),
	EF_FRAME(    0,    0,    6,           0,  1352 ),
	EF_FRAME(    0,    0,    6,           0,  1351 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1350 ),
	CG_NEXT( pl1f_grp27_61 ),
};

/* group 27 62 */
const Sint32 pl1f_grp27_62[] = {
	EF_FRAME(    0,    0,    2,           0,  1348 ),
	EF_FRAME(    0,    0,    2,           0,  1347 ),
	EF_FRAME(    0,    0,    2,           0,  1346 ),
	EF_FRAME(    0,    0,    2,           0,  1345 ),
};

const Sint32 pl1f_grp27_62_loop[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,    -1 ),
	CG_NEXT( pl1f_grp27_62_loop ),
};

/* group 27 63
   group 27 64
   group 27 65
   group 27 68
   group 27 69 */
const Sint32 pl1f_grp27_63[] = {
	EF_FRAME(    0,    0,    1,           0,   781 ),
	EF_FRAME(    0,    0,    1,           0,   782 ),
	EF_FRAME(    0,    0,    1,           0,   783 ),
	EF_FRAME(    0,    0,    1,           0,   784 ),
	EF_FRAME(    0,    0,    1,           0,   785 ),
	EF_FRAME(    0,    0,    1,           0,   786 ),
	EF_FRAME(    0,    0,    1,           0,   787 ),
};

const Sint32 pl1f_grp27_63_loop[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,    -1 ),
	CG_NEXT( pl1f_grp27_63_loop ),
};

/* group 27 66 */
const Sint32 pl1f_grp27_66[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA,    -1 ),
	CG_NEXT( pl1f_grp27_66 ),
};

/* group 27 67: pls1f_00_t2_init */
const Sint32 pl1f_grp27_67[] = {
	EF_FRAME(    0,    0,    3,           0,  1136 ),
	EF_FRAME(    0,    1,    3, END_OF_DATA,  1137 ),
	CG_NEXT( pl1f_grp27_67 ),
};

/* group 27 70
   group 27 71
   group 27 72
   group 27 73
   group 27 74
   group 27 75 */
const Sint32 pl1f_grp27_70[] = {
	EF_FRAME(    0,    0,    6,           0,  1180 ),
	EF_FRAME(    0,    0,    6,           0,  1181 ),
	EF_FRAME(    0,    0,    6,           0,  1182 ),
	EF_FRAME(    0,    0,    6,           0,  1183 ),
	EF_FRAME(    0,    0,    6,           0,  1184 ),
	EF_FRAME(    0,    0,    6,           0,  1185 ),
	EF_FRAME(    0,    0,    6,           0,  1186 ),
	EF_FRAME(    0,    0,    6,           0,  1187 ),
	EF_FRAME(    0,    0,    6,           0,  1188 ),
	EF_FRAME(    0,    0,    6,           0,  1189 ),
};

const Sint32 pl1f_grp27_70_loop[] = {
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1190 ),
	CG_NEXT( pl1f_grp27_70_loop ),
};

/* group 27 79 */
const Sint32 pl1f_grp27_79[] = {
	EF_FRAME(    0,    0,    4,           0,  1324 ),
};

const Sint32 pl1f_grp27_79_loop[] = {
	EF_FRAME(    0,    0,    4,           0,  1325 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1326 ),
	CG_NEXT( pl1f_grp27_79_loop ),
};

/* group 27 80 */
const Sint32 pl1f_grp27_80[] = {
	EF_FRAME(    0,    0,    7,           0,  1198 ),
	EF_FRAME(    0,    0,    7,           0,  1199 ),
	EF_FRAME(    0,    0,    7,           0,  1200 ),
	EF_FRAME(    0,    0,    7,           0,  1201 ),
	EF_FRAME(    0,    0,    8,           0,  1202 ),
	EF_FRAME(    0,    0,    8,           0,  1203 ),
	EF_FRAME(    0,    0,    7,           0,  1204 ),
	EF_FRAME(    0,    0,    7,           0,  1205 ),
	EF_FRAME(    0,    0,    7,           0,  1206 ),
	EF_FRAME(    0,    0,    8,           0,  1207 ),
	EF_FRAME(    0,    0,    8,           0,  1208 ),
	EF_FRAME(    0,    0,    8,           0,  1209 ),
	EF_FRAME(    0,    0,    6,           0,  1210 ),
	EF_FRAME(    0,    1,    1, END_OF_DATA,  1210 ),
	CG_NEXT( pl1f_grp27_80 ),
};

/* group 27 81 */
const Sint32 pl1f_grp27_81[] = {
	EF_FRAME(    0,    0,    6,           0,  1211 ),
	EF_FRAME(    0,    0,    6,           0,  1212 ),
	EF_FRAME(    0,    0,    6,           0,  1213 ),
	EF_FRAME(    0,    0,    6,           0,  1214 ),
	EF_FRAME(    0,    0,    6,           0,  1215 ),
	EF_FRAME(    0,    0,    6,           0,  1216 ),
	EF_FRAME(    0,    0,    6,           0,  1215 ),
	EF_FRAME(    0,    0,    6,           0,  1216 ),
	EF_FRAME(    0,    0,    6,           0,  1215 ),
	EF_FRAME(    0,    0,    6,           0,  1216 ),
	EF_FRAME(    0,    0,    6,           0,  1215 ),
	EF_FRAME(    0,    0,    6,           0,  1216 ),
	EF_FRAME(    0,    0,    6,           0,  1215 ),
	EF_FRAME(    0,    0,    6,           0,  1216 ),
	EF_FRAME(    0,    0,    6,           0,  1215 ),
	EF_FRAME(    0,    0,    5,           0,  1216 ),
	EF_FRAME(    0,    1,    1,           0,  1216 ),
};

const Sint32 pl1f_grp27_81_loop[] = {
	EF_FRAME(    0,    0,    7,           0,  1198 ),
	EF_FRAME(    0,    0,    7,           0,  1199 ),
	EF_FRAME(    0,    0,    7,           0,  1200 ),
	EF_FRAME(    0,    0,    7,           0,  1201 ),
	EF_FRAME(    0,    0,    8,           0,  1202 ),
	EF_FRAME(    0,    0,    8,           0,  1203 ),
	EF_FRAME(    0,    0,    7,           0,  1204 ),
	EF_FRAME(    0,    0,    7,           0,  1205 ),
	EF_FRAME(    0,    0,    7,           0,  1206 ),
	EF_FRAME(    0,    0,    8,           0,  1207 ),
	EF_FRAME(    0,    0,    8,           0,  1208 ),
	EF_FRAME(    0,    0,    8,           0,  1209 ),
	EF_FRAME(    0,    0,    6,           0,  1210 ),
	EF_FRAME(    0,    1,    1, END_OF_DATA,  1210 ),
	CG_NEXT( pl1f_grp27_81_loop ),
};

/* group 27 82 */
const Sint32 pl1f_grp27_82[] = {
	EF_FRAME(    0,    0,    4,           0,  1217 ),
	EF_FRAME(    0,    0,    4,           0,  1218 ),
	EF_FRAME(    0,    0,    4,           0,  1219 ),
	EF_FRAME(    0,    0,    4,           0,  1220 ),
	EF_FRAME(    0,    0,    4,           0,  1221 ),
	EF_FRAME(    0,    0,    4,           0,  1222 ),
	EF_FRAME(    0,    0,    4,           0,  1223 ),
	EF_FRAME(    0,    0,    4,           0,  1224 ),
	EF_FRAME(    0,    0,    4,           0,  1217 ),
	EF_FRAME(    0,    0,    4,           0,  1218 ),
	EF_FRAME(    0,    0,    4,           0,  1219 ),
	EF_FRAME(    0,    0,    4,           0,  1220 ),
	EF_FRAME(    0,    0,    4,           0,  1221 ),
	EF_FRAME(    0,    0,    4,           0,  1222 ),
	EF_FRAME(    0,    0,    4,           0,  1223 ),
	EF_FRAME(    0,    0,    4,           0,  1224 ),
	EF_FRAME(    0,    0,    4,           0,  1225 ),
	EF_FRAME(    0,    0,    4,           0,  1226 ),
	EF_FRAME(    0,    0,    4,           0,  1227 ),
	EF_FRAME(    0,    0,    4,           0,  1228 ),
	EF_FRAME(    0,    0,    4,           0,  1229 ),
	EF_FRAME(    0,    0,    4,           0,  1230 ),
	EF_FRAME(    0,    0,    4,           0,  1231 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1232 ),
	CG_NEXT( pl1f_grp27_82 ),
};

/* group 27 83 */
const Sint32 pl1f_grp27_83[] = {
	EF_FRAME(    0,    0,    6,           0,  1233 ),
	EF_FRAME(    0,    0,    6,           0,  1234 ),
	EF_FRAME(    0,    0,    6,           0,  1235 ),
	EF_FRAME(    0,    0,    6,           0,  1236 ),
	EF_FRAME(    0,    0,    6,           0,  1237 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1238 ),
	CG_NEXT( pl1f_grp27_83 ),
};

/* group 27 84 */
const Sint32 pl1f_grp27_84[] = {
	EF_FRAME(    0,    0,    4,           0,  1239 ),
	EF_FRAME(    0,    0,    4,           0,  1240 ),
};

const Sint32 pl1f_grp27_84_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1241 ),
	CG_NEXT( pl1f_grp27_84_loop ),
};

/* group 27 85 */
const Sint32 pl1f_grp27_85[] = {
	EF_FRAME(    0,    0,   16,           0,  1242 ),
};

/* group 27 86 */
const Sint32 pl1f_grp27_86[] = {
	EF_FRAME(    0,    0,   16,           0,  1243 ),
};

/* group 27 87 */
const Sint32 pl1f_grp27_87[] = {
	EF_FRAME(    0,    0,   16, END_OF_DATA,  1244 ),
	CG_NEXT( pl1f_grp27_87 ),
};

/* group 27 88 */
const Sint32 pl1f_grp27_88[] = {
	EF_FRAME(    0,    0,    8,           0,  1245 ),
};

const Sint32 pl1f_grp27_88_loop[] = {
	EF_FRAME(    0,    0,    6,           0,  1246 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1247 ),
	CG_NEXT( pl1f_grp27_88_loop ),
};

/* group 27 89
   group 27 92 */
const Sint32 pl1f_grp27_89[] = {
	EF_FRAME(    0,    0,    4,           0,  1248 ),
	EF_FRAME(    0,    0,    4,           0,  1249 ),
	EF_FRAME(    0,    0,    4,           0,  1250 ),
	EF_FRAME(    0,    0,    4,           0,  1251 ),
	EF_FRAME(    0,    0,    4,           0,  1252 ),
	EF_FRAME(    0,    0,    4,           0,  1253 ),
	EF_FRAME(    0,    0,    4,           0,  1254 ),
	EF_FRAME(    0,    0,    4,           0,  1255 ),
	EF_FRAME(    0,    0,    4,           0,  1256 ),
};

const Sint32 pl1f_grp27_89_loop[] = {
	EF_FRAME(    0,    0,    6,           0,  1257 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1258 ),
	CG_NEXT( pl1f_grp27_89_loop ),
};

/* group 27 90 */
const Sint32 pl1f_grp27_90[] = {
	EF_FRAME(    0,    0,    6,           0,  1259 ),
	EF_FRAME(    0,    0,    6,           0,  1260 ),
	EF_FRAME(    0,    0,    6,           0,  1261 ),
	EF_FRAME(    0,    0,    6,           0,  1262 ),
	EF_FRAME(    0,    0,    6,           0,  1263 ),
	EF_FRAME(    0,    0,    6,           0,  1264 ),
	EF_FRAME(    0,    0,    6,           0,  1265 ),
	EF_FRAME(    0,    0,    6,           0,  1266 ),
	EF_FRAME(    0,    0,    6,           0,  1267 ),
	EF_FRAME(    0,    0,    6,           0,  1268 ),
	EF_FRAME(    0,    0,    6,           0,  1269 ),
	EF_FRAME(    0,    0,    6,           0,  1270 ),
	EF_FRAME(    0,    0,    6,           0,  1271 ),
	EF_FRAME(    0,    0,    6,           0,  1272 ),
};

const Sint32 pl1f_grp27_90_loop[] = {
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1264 ),
	CG_NEXT( pl1f_grp27_90_loop ),
};

/* group 27 91 */
const Sint32 pl1f_grp27_91[] = {
	EF_FRAME(    0,    0,    6,           0,  1274 ),
	EF_FRAME(    0,    0,    6,           0,  1275 ),
	EF_FRAME(    0,    0,    6,           0,  1276 ),
	EF_FRAME(    0,    0,    6,           0,  1277 ),
	EF_FRAME(    0,    0,    6,           0,  1278 ),
	EF_FRAME(    0,    0,    6,           0,  1279 ),
	EF_FRAME(    0,    0,    6,           0,  1280 ),
	EF_FRAME(    0,    0,    6,           0,  1281 ),
	EF_FRAME(    0,    0,    6,           0,  1282 ),
	EF_FRAME(    0,    0,    6,           0,  1283 ),
	EF_FRAME(    0,    0,    6,           0,  1284 ),
	EF_FRAME(    0,    0,    6,           0,  1285 ),
	EF_FRAME(    0,    0,    6,           0,  1286 ),
	EF_FRAME(    0,    0,    6,           0,  1287 ),
	EF_FRAME(    0,    0,    6,           0,  1288 ),
	EF_FRAME(    0,    0,    6,           0,  1289 ),
	EF_FRAME(    0,    0,    6,           0,  1290 ),
	EF_FRAME(    0,    0,    6,           0,  1291 ),
	EF_FRAME(    0,    0,    6,           0,  1292 ),
};

const Sint32 pl1f_grp27_91_loop[] = {
	EF_FRAME(    0,    0,    4,           0,  1293 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1294 ),
	CG_NEXT( pl1f_grp27_91_loop ),
};

/* group 27 93 */
const Sint32 pl1f_grp27_93[] = {
	EF_FRAME(    0,    0,    4,           0,  1295 ),
	EF_FRAME(    0,    0,    4,           0,  1296 ),
	EF_FRAME(    0,    0,    4,           0,  1297 ),
	EF_FRAME(    0,    0,    4,           0,  1298 ),
	EF_FRAME(    0,    0,    4,           0,  1299 ),
	EF_FRAME(    0,    0,    4,           0,  1300 ),
	EF_FRAME(    0,    0,    4,           0,  1301 ),
	EF_FRAME(    0,    0,    4,           0,  1302 ),
	EF_FRAME(    0,    1,    4,           0,  1303 ),
};

const Sint32 pl1f_grp27_93_loop[] = {
	EF_FRAME(    0,    0,    4,           0,  1304 ),
	EF_FRAME(    0,    0,    4,           0,  1305 ),
	EF_FRAME(    0,    0,    4,           0,  1306 ),
	EF_FRAME(    0,    0,    4,           0,  1307 ),
	EF_FRAME(    0,    0,    4,           0,  1308 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1309 ),
	CG_NEXT( pl1f_grp27_93_loop ),
};

/* group 27 94 */
const Sint32 pl1f_grp27_94[] = {
	EF_FRAME(    0,    0,    6,           0,  1310 ),
	EF_FRAME(    0,    0,    6,           0,  1311 ),
	EF_FRAME(    0,    0,    6,           0,  1312 ),
};

const Sint32 pl1f_grp27_94_loop[] = {
	EF_FRAME(    0,    0,    6,           0,  1313 ),
	EF_FRAME(    0,    0,    6,           0,  1314 ),
	EF_FRAME(    0,    0,    6,           0,  1315 ),
	EF_FRAME(    0,    0,    6,           0,  1316 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1317 ),
	CG_NEXT( pl1f_grp27_94_loop ),
};

/* group 27 95 */
const Sint32 pl1f_grp27_95[] = {
	EF_FRAME(    0,    0,    6,           0,  1318 ),
	EF_FRAME(    0,    0,    4,           0,  1319 ),
	EF_FRAME(    0,    0,    4,           0,  1320 ),
	EF_FRAME(    0,    0,    4,           0,  1321 ),
	EF_FRAME(    0,    0,    4,           0,  1322 ),
	EF_FRAME(    0,    0,    4,           0,  1323 ),
	EF_FRAME(    0,    0,    4,           0,  1324 ),
};

const Sint32 pl1f_grp27_95_loop[] = {
	EF_FRAME(    0,    0,    4,           0,  1325 ),
	EF_FRAME(    0,    0,    4,           0,  1326 ),
	EF_FRAME(    0,    0,    4,           0,  1325 ),
	EF_FRAME(    0,    0,    4,           0,  1326 ),
	EF_FRAME(    0,    0,   48, END_OF_DATA,  1325 ),
	CG_NEXT( pl1f_grp27_95_loop ),
};

/* group 27 96 */
const Sint32 pl1f_grp27_96[] = {
	EF_FRAME(    0,    0,    8,           0,  1327 ),
	EF_FRAME(    0,    0,    8,           0,  1328 ),
	EF_FRAME(    0,    0,    8,           0,  1329 ),
	EF_FRAME(    0,    0,    8,           0,  1330 ),
	EF_FRAME(    0,    0,    8,           0,  1331 ),
	EF_FRAME(    0,    0,    8,           0,  1332 ),
	EF_FRAME(    0,    0,    8,           0,  1333 ),
	EF_FRAME(    0,    0,    8,           0,  1334 ),
};

const Sint32 pl1f_grp27_96_loop[] = {
	EF_FRAME(    0,    0,    8, END_OF_DATA,  1335 ),
	CG_NEXT( pl1f_grp27_96_loop ),
};

/* group 27 97 */
const Sint32 pl1f_grp27_97[] = {
	EF_FRAME(    0,    0,    3,           0,  1336 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,  1337 ),
	CG_NEXT( pl1f_grp27_97 ),
};

/* group 27 98 */
const Sint32 pl1f_grp27_98[] = {
	EF_FRAME(    0,    0,    4,           0,  1338 ),
	EF_FRAME(    0,    0,    4,           0,  1339 ),
	EF_FRAME(    0,    0,    4,           0,  1340 ),
	EF_FRAME(    0,    0,    4,           0,  1341 ),
	EF_FRAME(    0,    0,    4,           0,  1342 ),
	EF_FRAME(    0,    0,    4,           0,  1343 ),
};

const Sint32 pl1f_grp27_98_loop[] = {
	EF_FRAME(    0,    0,    6,           0,  1257 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1258 ),
	CG_NEXT( pl1f_grp27_98_loop ),
};

/* group 27 99: pls1f_03_t22 */
const Sint32 pl1f_grp27_99[] = {
	EF_FRAME(    0,    0,    3,           0,  1372 ),
	EF_FRAME(    0,    0,    3,           0,  1373 ),
	EF_FRAME(    0,    0,    3,           0,  1372 ),
	EF_FRAME(    0,    0,    3,           0,  1373 ),
	EF_FRAME(    0,    0,    3,           0,  1372 ),
	EF_FRAME(    0,    0,    3,           0,  1373 ),
	EF_FRAME(    0,    0,    4,           0,  1374 ),
	EF_FRAME(    0,    0,    4,           0,  1375 ),
	EF_FRAME(    0,    0,    4,           0,  1376 ),
	EF_FRAME(    0,    0,    2,           0,  1377 ),
	EF_FRAME(    0,    0,    2,           0,  1378 ),
	EF_FRAME(    0,    0,    2,           0,  1377 ),
	EF_FRAME(    0,    0,    2,           0,  1378 ),
	EF_FRAME(    0,    0,    4,           0,  1379 ),
	EF_FRAME(    0,    0,    4,           0,  1380 ),
	EF_FRAME(    0,    0,    4,           0,  1381 ),
	EF_FRAME(    0,    0,    2,           0,  1372 ),
	EF_FRAME(    0,    0,    2,           0,  1373 ),
	EF_FRAME(    0,    0,    2,           0,  1372 ),
};

const Sint32 pl1f_grp27_99_loop[] = {
	EF_FRAME(    0,    0,    2, END_OF_DATA,  1373 ),
	CG_NEXT( pl1f_grp27_99_loop ),
};

/* group 27 100 */
const Sint32 pl1f_grp27_100[] = {
	EF_FRAME(    0,    0,   64,           0,  1191 ),
};

const Sint32 pl1f_grp27_100_loop[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,  1191 ),
	CG_NEXT( pl1f_grp27_100_loop ),
};

/* group 27 101 */
const Sint32 pl1f_grp27_101[] = {
	EF_FRAME(    0,    0,   48,           0,    -1 ),
	EF_FRAME(    0,    0,    6,           0,  1354 ),
	EF_FRAME(    0,    0,    6,           0,  1355 ),
	EF_FRAME(    0,    0,    6,           0,  1356 ),
	EF_FRAME(    0,    0,    6,           0,  1357 ),
};

const Sint32 pl1f_grp27_101_loop[] = {
	EF_FRAME(    0,    0,    6,           0,  1358 ),
	EF_FRAME(    0,    0,    6,           0,  1359 ),
	EF_FRAME(    0,    0,    6,           0,  1360 ),
	EF_FRAME(    0,    0,    6,           0,  1361 ),
	EF_FRAME(    0,    0,    6,           0,  1362 ),
	EF_FRAME(    0,    0,    6,           0,  1361 ),
	EF_FRAME(    0,    0,    6,           0,  1360 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1359 ),
	CG_NEXT( pl1f_grp27_101_loop ),
};

/* group 27 102 */
const Sint32 pl1f_grp27_102[] = {
	EF_FRAME(    0,    0,   48,           0,    -1 ),
	EF_FRAME(    0,    0,    6,           0,  1363 ),
	EF_FRAME(    0,    0,    6,           0,  1364 ),
	EF_FRAME(    0,    0,    6,           0,  1365 ),
	EF_FRAME(    0,    0,    6,           0,  1366 ),
};

const Sint32 pl1f_grp27_102_loop[] = {
	EF_FRAME(    0,    0,    6,           0,  1367 ),
	EF_FRAME(    0,    0,    6,           0,  1368 ),
	EF_FRAME(    0,    0,    6,           0,  1369 ),
	EF_FRAME(    0,    0,    6,           0,  1370 ),
	EF_FRAME(    0,    0,    6,           0,  1371 ),
	EF_FRAME(    0,    0,    6,           0,  1370 ),
	EF_FRAME(    0,    0,    6,           0,  1369 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,  1368 ),
	CG_NEXT( pl1f_grp27_102_loop ),
};

/* group 27 103 */
const Sint32 pl1f_grp27_103[] = {
	EF_FRAME(    0,    0,    4,           0,  1361 ),
	EF_FRAME(    0,    0,    4,           0,  1362 ),
	EF_FRAME(    0,    0,    4,           0,  1363 ),
	EF_FRAME(    0,    0,    4,           0,  1364 ),
	EF_FRAME(    0,    0,    4,           0,  1365 ),
	EF_FRAME(    0,    0,    4,           0,  1366 ),
	EF_FRAME(    0,    0,    4,           0,  1367 ),
	EF_FRAME(    0,    0,    4,           0,  1368 ),
	EF_FRAME(    0,    0,    4,           0,  1369 ),
	EF_FRAME(    0,    0,    4,           0,  1368 ),
	EF_FRAME(    0,    0,    4,           0,  1367 ),
	EF_FRAME(    0,    0,    4,           0,  1366 ),
	EF_FRAME(    0,    0,    4,           0,  1364 ),
	EF_FRAME(    0,    0,    4,           0,  1363 ),
	EF_FRAME(    0,    0,    4,           0,  1362 ),
};

const Sint32 pl1f_grp27_103_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1361 ),
	CG_NEXT( pl1f_grp27_103_loop ),
};

/* Char_Effect 0 */
const Sint32 pl1f_effect_00[] = {
	CG_FRAME(    0,    0,   12, END_OF_DATA,    -1, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_effect_00 ),
};

/* Char_Effect 1: shl1f_00_init (1 + type) */
const Sint32 pl1f_effect_01[] = {
	CG_FRAME(    0,    0,    1,           0,   730, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   736, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   737, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   738, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   739, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   740, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   741, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   742, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   743, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   742, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,   13,           0,   742, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   749, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   755, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   756, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   749, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   750, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    6,           0,   749, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
};

const Sint32 pl1f_effect_01_loop[] = {
	CG_FRAME(    0,    1,   16, END_OF_DATA,   749, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl1f_effect_01_loop ),
};

/* Char_Effect 2 */
const Sint32 pl1f_effect_02[] = {
	CG_FRAME(    0,   -1,    6,           0,    -1, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    8,           0,   756, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   756, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    6,           0,   749, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    8,           0,   750, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,    1,           0,   739, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   740, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   741, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   742, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   743, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   744, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   745, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   746, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   747, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   748, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
};

const Sint32 pl1f_effect_02_loop[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   749, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl1f_effect_02_loop ),
};

/* Char_Effect 3 */
const Sint32 pl1f_effect_03[] = {
	CG_FRAME(    0,    0,    1,           0,   757, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   758, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   759, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   760, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   761, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   762, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   763, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   764, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   765, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   766, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   757, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   758, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   759, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
};

const Sint32 pl1f_effect_03_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   760, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl1f_effect_03_loop ),
};

/* Char_Effect 4: shl1f_01_init (4 + type)
   Char_Effect 6 */
const Sint32 pl1f_effect_04[] = {
	CG_FRAME(    0,    1,    7,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    1,   0,     0, 0x0100,  65 ),
	CG_FRAME(    0,    1,    7,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    2,   0,     0, 0x0200,  65 ),
	CG_FRAME(    0,    1,    7,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    3,   0,     0, 0x0300,  65 ),
};

const Sint32 pl1f_effect_04_loop[] = {
	CG_FRAME(    0,    0,    6,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    4,   0,     0, 0x0400,  65 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA, 32838, 0x00,   0x00,  ATK,  0x00,          0,    4,   0,     0, 0x0400,  65 ),
	CG_NEXT( pl1f_effect_04_loop ),
};

/* Char_Effect 5
   Char_Effect 7 */
const Sint32 pl1f_effect_05[] = {
	CG_FRAME(    0,    1,    5,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    1,   0,     0, 0x0100,  65 ),
	CG_FRAME(    0,    1,    5,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    2,   0,     0, 0x0200,  65 ),
	CG_FRAME(    0,    1,    5,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    3,   0,     0, 0x0300,  65 ),
};

const Sint32 pl1f_effect_05_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    4,   0,     0, 0x0400,  65 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA, 32838, 0x00,   0x00,  ATK,  0x00,          0,    4,   0,     0, 0x0400,  65 ),
	CG_NEXT( pl1f_effect_05_loop ),
};

/* Char_Effect 8 */
const Sint32 pl1f_effect_08[] = {
	CG_FRAME(    0,    0,    4,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    5,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA, 32838, 0x00,   0x00,  ATK,  0x00,          0,    5,   0,     0, 0x0500,  65 ),
	CG_NEXT( pl1f_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl1f_effect_09[] = {
	CG_FRAME(    0,    0,    4,           0, 32838, 0x00,   0x00,  ATK,  0x00,          0,    6,   0,     0, 0x0800,  65 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA, 32838, 0x00,   0x00,  ATK,  0x00,          0,    6,   0,     0, 0x0800,  65 ),
	CG_NEXT( pl1f_effect_09 ),
};

/* Char_Effect 16: shl1f_02_init */
const Sint32 pl1f_effect_16[] = {
	CG_FRAME(    0,    1,    7,           0, 32841, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  72 ),
	CG_FRAME(    0,    1,    7,           0, 32841, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  72 ),
	CG_FRAME(    0,    1,    7,           0, 32841, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  72 ),
};

const Sint32 pl1f_effect_16_loop[] = {
	CG_FRAME(    0,    0,    6,           0, 32841, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,  72 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA, 32841, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,  72 ),
	CG_NEXT( pl1f_effect_16_loop ),
};

/* Char_Effect 10: shl1f_00_move, shl1f_00_move_hit_end, shl1f_00_move_02_1, shl1f_01_hit_end */
const Sint32 pl1f_effect_10[] = {
	CG_FRAME(    0,    0,    2,           0,   788, 0x00,   0x00,    0,  0x00, 0x07 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   790, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   792, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   793, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   794, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   795, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   796, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   797, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   798, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1f_effect_10_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   799, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_effect_10_loop ),
};

/* Char_Effect 11: shl1f_02_init (11 + type) */
const Sint32 pl1f_effect_11[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA, 32838, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl1f_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl1f_effect_12[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA, 32838, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl1f_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl1f_effect_13[] = {
	CG_FRAME(    0,    0,    8,           0,   963, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   964, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   965, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   966, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   967, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   968, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   969, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   970, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   971, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   972, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8,           0,   973, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   974, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl1f_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl1f_effect_14[] = {
	CG_FRAME(    0,    0,    4,           0,   975, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   976, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   977, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   978, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl1f_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl1f_effect_15[] = {
	CG_FRAME(    0,    0,    2,           0,   979, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   980, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   981, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   982, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   983, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   984, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   985, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   986, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   987, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   988, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   989, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   990, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   991, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   992, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   993, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   994, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   995, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   996, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
};

const Sint32 pl1f_effect_15_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   997, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl1f_effect_15_loop ),
};

/* Char_Effect 17: shl1f_03_init, shl1f_03_scrap_init */
const Sint32 pl1f_effect_17[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32769, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32771, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl1f_effect_17 ),
};

/* Char_Effect 18: shl1f_00_spark_main (18 + type) */
const Sint32 pl1f_effect_18[] = {
	CG_FRAME(    0,    0,    1,           0,   812, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   813, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   814, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    1,           0,   815, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
};

const Sint32 pl1f_effect_18_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   816, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    1,           0,   817, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    1,           0,   818, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    1,           0,   819, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    1,           0,   820, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   821, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl1f_effect_18_loop ),
};

/* Char_Effect 20: shl1f_00_spark_main (20 + type) */
const Sint32 pl1f_effect_20[] = {
	CG_FRAME(    0,    0,    1,           0,   822, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   823, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   824, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   825, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   826, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1f_effect_20_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   827, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_effect_20_loop ),
};

/* Char_Effect 19 */
const Sint32 pl1f_effect_19[] = {
	CG_FRAME(    0,    0,    1,           0,   828, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   829, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   830, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    1,           0,   831, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
};

const Sint32 pl1f_effect_19_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   832, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    1,           0,   833, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    1,           0,   834, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    1,           0,   835, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    1,           0,   836, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   837, 0x00,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl1f_effect_19_loop ),
};

/* Char_Effect 21 */
const Sint32 pl1f_effect_21[] = {
	CG_FRAME(    0,    0,    1,           0,   838, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   839, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   840, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   841, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   842, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   843, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1f_effect_21_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   844, 0x00,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1f_effect_21_loop ),
};
