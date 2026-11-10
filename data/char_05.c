/* Strider Hiryu (character 05): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_05      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_05     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl05.c) says which group
                   and number each move uses.
   pl05_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl05_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl05_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl05_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_05
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_05, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_05 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl05_stand_00[], pl05_stand_02[], pl05_stand_06[], pl05_stand_03[],
	pl05_stand_07[], pl05_jump_00[], pl05_jump_00_loop[], pl05_jump_01[], pl05_jump_01_loop[],
	pl05_jump_02[], pl05_jump_02_loop[], pl05_jump_03[], pl05_jump_06[], pl05_jump_09[],
	pl05_jump_07[], pl05_jump_10[], pl05_jump_08[], pl05_jump_11[], pl05_dash_00[], pl05_dash_02[],
	pl05_dash_01[], pl05_dash_03[], pl05_crouch_00[], pl05_crouch_01[], pl05_crouch_02[],
	pl05_turn_00[], pl05_turn_01[], pl05_guard_00[], pl05_guard_01[], pl05_guard_02[],
	pl05_guard_03[], pl05_guard_04[], pl05_guard_05[], pl05_guard_06[], pl05_guard_07[],
	pl05_air_guard_00[], pl05_air_guard_01[], pl05_air_guard_02[], pl05_stand_p_00[],
	pl05_stand_p_01[], pl05_stand_p_02[], pl05_stand_k_00[], pl05_stand_k_01[], pl05_stand_k_02[],
	pl05_crouch_p_00[], pl05_crouch_p_01[], pl05_crouch_p_02[], pl05_crouch_k_00[],
	pl05_crouch_k_01[], pl05_crouch_k_02[], pl05_crouch_k_03[], pl05_crouch_k_03_loop[],
	pl05_crouch_k_04[], pl05_jump_p_00[], pl05_jump_p_01[], pl05_jump_p_02[], pl05_jump_k_00[],
	pl05_jump_k_01[], pl05_jump_k_02[], pl05_damage_00[], pl05_damage_01[], pl05_damage_02[],
	pl05_damage_03[], pl05_damage_04[], pl05_damage_05[], pl05_damage_06[], pl05_damage_07[],
	pl05_damage_08[], pl05_damage_09[], pl05_damage_09_loop[], pl05_damage_10[], pl05_damage_11[],
	pl05_damage_12[], pl05_damage_13[], pl05_damage_13_loop[], pl05_damage_16[], pl05_damage_17[],
	pl05_damage_18[], pl05_damage_19[], pl05_damage_20[], pl05_damage_21[], pl05_damage_22[],
	pl05_damage_23[], pl05_damage_24[], pl05_damage_25[], pl05_damage_26[], pl05_damage_27[],
	pl05_damage_28[], pl05_damage_29[], pl05_damage_30[], pl05_damage_31[], pl05_damage_32[],
	pl05_dizzy_00[], pl05_catch_00[], pl05_catch_00_loop[], pl05_catch_01[], pl05_catch_01_loop[],
	pl05_catch_02[], pl05_catch_03[], pl05_catch_03_loop[], pl05_catch_04[], pl05_catch_04_loop[],
	pl05_catch_05[], pl05_catch_05_loop[], pl05_catch_06[], pl05_catch_06_loop[], pl05_catch_07[],
	pl05_catch_08[], pl05_catch_08_loop[], pl05_get_up_00[], pl05_get_up_01[], pl05_get_up_02[],
	pl05_get_up_03[], pl05_appear_00[], pl05_appear_01[], pl05_appear_01_loop[], pl05_appear_02[],
	pl05_appear_03[], pl05_extra_00[], pl05_extra_00_loop[], pl05_extra_01[], pl05_jump_12[],
	pl05_extra_03[], pl05_extra_04[], pl05_extra_05[], pl05_extra_06[], pl05_extra_06_loop[],
	pl05_extra_07[], pl05_extra_08[], pl05_extra_09[], pl05_extra_10[], pl05_extra_10_loop[],
	pl05_extra_11[], pl05_extra_11_loop[], pl05_extra_12[], pl05_extra_12_loop[], pl05_extra_13[],
	pl05_extra_14[], pl05_extra_14_loop[], pl05_extra_15[], pl05_extra_15_loop[], pl05_extra_16[],
	pl05_extra_17[], pl05_win_00[], pl05_win_00_loop[], pl05_win_01[], pl05_win_01_loop[],
	pl05_win_02[], pl05_win_02_loop[], pl05_win_03[], pl05_win_04[], pl05_win_04_loop[],
	pl05_win_05[], pl05_win_05_loop[], pl05_change_00[], pl05_change_00_loop[], pl05_change_01[],
	pl05_change_01_loop[], pl05_change_03[], pl05_change_03_loop[], pl05_change_04[],
	pl05_change_02[], pl05_caught_00[], pl05_caught_01[], pl05_caught_02[], pl05_caught_03[],
	pl05_caught_04[], pl05_caught_05[], pl05_caught_06[], pl05_caught_07[], pl05_caught_08[],
	pl05_caught_09[], pl05_caught_10[], pl05_caught_11[], pl05_caught_12[], pl05_caught_13[],
	pl05_caught_14[], pl05_caught_15[], pl05_caught_16[], pl05_caught_17[], pl05_caught_18[],
	pl05_caught_19[], pl05_caught_20[], pl05_caught_21[], pl05_special_54[], pl05_special_00[],
	pl05_special_00_loop[], pl05_special_01[], pl05_special_02[], pl05_special_02_loop[],
	pl05_special_03[], pl05_special_04[], pl05_special_05[], pl05_special_05_loop[],
	pl05_special_09[], pl05_special_09_loop[], pl05_special_13[], pl05_special_13_loop[],
	pl05_special_06[], pl05_special_10[], pl05_special_14[], pl05_special_17[],
	pl05_special_17_loop[], pl05_special_18[], pl05_special_18_loop[], pl05_special_24[],
	pl05_special_24_loop[], pl05_special_25[], pl05_special_25_loop[], pl05_special_21[],
	pl05_special_21_loop[], pl05_special_22[], pl05_special_22_loop[], pl05_special_27[],
	pl05_special_53[], pl05_special_53_loop[], pl05_special_28[], pl05_special_28_loop[],
	pl05_special_29[], pl05_special_30[], pl05_special_31[], pl05_special_32[], pl05_special_33[],
	pl05_special_34[], pl05_special_35[], pl05_special_36[], pl05_special_37[],
	pl05_special_37_loop[], pl05_special_38[], pl05_special_38_loop[], pl05_special_39[],
	pl05_special_39_loop[], pl05_special_40[], pl05_special_40_loop[], pl05_special_41[],
	pl05_special_42[], pl05_special_43[], pl05_special_44[], pl05_special_45[], pl05_special_46[],
	pl05_special_47[], pl05_special_48[], pl05_special_49[], pl05_special_50[],
	pl05_special_50_loop[], pl05_special_51[], pl05_special_52[], pl05_super_00[],
	pl05_super_00_loop[], pl05_super_33[], pl05_super_02[], pl05_super_02_loop[], pl05_super_04[],
	pl05_super_04_loop[], pl05_super_06[], pl05_super_18[], pl05_super_29_loop[], pl05_super_07[],
	pl05_super_19[], pl05_super_08[], pl05_super_20[], pl05_super_09[], pl05_super_21[],
	pl05_super_10[], pl05_super_22[], pl05_super_11[], pl05_super_23[], pl05_super_12[],
	pl05_super_24[], pl05_super_13[], pl05_super_25[], pl05_super_14[], pl05_super_26[],
	pl05_super_15[], pl05_super_27[], pl05_super_16[], pl05_super_28[], pl05_super_17[],
	pl05_super_29[], pl05_super_30[], pl05_super_31[], pl05_super_32[], pl05_effect_00[],
	pl05_effect_01[], pl05_effect_01_loop[], pl05_effect_02[], pl05_effect_02_loop[],
	pl05_effect_03[], pl05_effect_04[], pl05_effect_05[], pl05_effect_06[], pl05_effect_07[],
	pl05_effect_07_loop[], pl05_effect_08[], pl05_effect_08_loop[], pl05_effect_12[],
	pl05_effect_13[], pl05_effect_14[], pl05_effect_15[], pl05_effect_16[], pl05_effect_17[],
	pl05_effect_18[], pl05_effect_19[], pl05_effect_20[], pl05_effect_21[], pl05_effect_22[],
	pl05_effect_22_loop[], pl05_effect_23[], pl05_effect_24[], pl05_effect_25[], pl05_effect_26[],
	pl05_effect_26_loop[], pl05_effect_27[], pl05_effect_28[], pl05_effect_30[], pl05_effect_31[],
	pl05_effect_32[], pl05_effect_33[], pl05_effect_34[], pl05_effect_35[], pl05_effect_36[],
	pl05_effect_36_loop[], pl05_effect_37[], pl05_effect_37_loop[], pl05_effect_38[];
extern const Sint32 *const pl05_stand_tbl[], *const pl05_jump_tbl[], *const pl05_dash_tbl[],
	*const pl05_crouch_tbl[], *const pl05_turn_tbl[], *const pl05_guard_tbl[],
	*const pl05_air_guard_tbl[], *const pl05_stand_p_tbl[], *const pl05_stand_k_tbl[],
	*const pl05_crouch_p_tbl[], *const pl05_crouch_k_tbl[], *const pl05_jump_p_tbl[],
	*const pl05_jump_k_tbl[], *const pl05_damage_tbl[], *const pl05_dizzy_tbl[],
	*const pl05_catch_tbl[], *const pl05_caught_tbl[], *const pl05_get_up_tbl[],
	*const pl05_appear_tbl[], *const pl05_win_tbl[], *const pl05_extra_tbl[],
	*const pl05_special_tbl[], *const pl05_super_tbl[], *const pl05_effect_tbl[],
	*const pl05_change_tbl[], *const pl05_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_05[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 6656, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 10752, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 14336, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 18432, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 25088, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 25088, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 18432, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 14336, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 10752, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 6656, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_05[] = {
	pl05_stand_tbl, pl05_jump_tbl, pl05_dash_tbl, pl05_crouch_tbl, pl05_turn_tbl, pl05_guard_tbl,
	pl05_air_guard_tbl, pl05_stand_p_tbl, pl05_stand_k_tbl, pl05_crouch_p_tbl, pl05_crouch_k_tbl, pl05_jump_p_tbl,
	pl05_jump_k_tbl, pl05_damage_tbl, pl05_dizzy_tbl, pl05_catch_tbl, pl05_caught_tbl, pl05_get_up_tbl,
	pl05_appear_tbl, pl05_win_tbl, pl05_extra_tbl, pl05_special_tbl, pl05_super_tbl, pl05_effect_tbl,
	pl05_change_tbl, pl05_grp25_tbl, 0,
};

const Sint32 *const pl05_stand_tbl[] = {
	pl05_stand_00, pl05_stand_00, pl05_stand_02, pl05_stand_03, pl05_stand_00, pl05_stand_00,
	pl05_stand_06, pl05_stand_07, pl05_stand_06, pl05_stand_07, 0,
};

const Sint32 *const pl05_jump_tbl[] = {
	pl05_jump_00, pl05_jump_01, pl05_jump_02, pl05_jump_03, pl05_jump_03, pl05_jump_03,
	pl05_jump_06, pl05_jump_07, pl05_jump_08, pl05_jump_09, pl05_jump_10, pl05_jump_11,
	pl05_jump_12, 0,
};

const Sint32 *const pl05_dash_tbl[] = {
	pl05_dash_00, pl05_dash_01, pl05_dash_02, pl05_dash_03, 0,
};

const Sint32 *const pl05_crouch_tbl[] = {
	pl05_crouch_00, pl05_crouch_01, pl05_crouch_02, 0,
};

const Sint32 *const pl05_turn_tbl[] = {
	pl05_turn_00, pl05_turn_01, 0,
};

const Sint32 *const pl05_guard_tbl[] = {
	pl05_guard_00, pl05_guard_01, pl05_guard_02, pl05_guard_03, pl05_guard_04, pl05_guard_05,
	pl05_guard_06, pl05_guard_07, 0,
};

const Sint32 *const pl05_air_guard_tbl[] = {
	pl05_air_guard_00, pl05_air_guard_01, pl05_air_guard_02, 0,
};

const Sint32 *const pl05_stand_p_tbl[] = {
	pl05_stand_p_00, pl05_stand_p_01, pl05_stand_p_02, 0,
};

const Sint32 *const pl05_stand_k_tbl[] = {
	pl05_stand_k_00, pl05_stand_k_01, pl05_stand_k_02, 0,
};

const Sint32 *const pl05_crouch_p_tbl[] = {
	pl05_crouch_p_00, pl05_crouch_p_01, pl05_crouch_p_02, 0,
};

const Sint32 *const pl05_crouch_k_tbl[] = {
	pl05_crouch_k_00, pl05_crouch_k_01, pl05_crouch_k_02, pl05_crouch_k_03, pl05_crouch_k_04, 0,
};

const Sint32 *const pl05_jump_p_tbl[] = {
	pl05_jump_p_00, pl05_jump_p_01, pl05_jump_p_02, pl05_jump_p_00, pl05_jump_p_01, pl05_jump_p_02,
	0,
};

const Sint32 *const pl05_jump_k_tbl[] = {
	pl05_jump_k_00, pl05_jump_k_01, pl05_jump_k_02, pl05_jump_k_00, pl05_jump_k_01, pl05_jump_k_02,
	0,
};

const Sint32 *const pl05_damage_tbl[] = {
	pl05_damage_00, pl05_damage_01, pl05_damage_02, pl05_damage_03, pl05_damage_04, pl05_damage_05,
	pl05_damage_06, pl05_damage_07, pl05_damage_08, pl05_damage_09, pl05_damage_10, pl05_damage_11,
	pl05_damage_12, pl05_damage_13, pl05_damage_13, pl05_damage_13, pl05_damage_16, pl05_damage_17,
	pl05_damage_18, pl05_damage_19, pl05_damage_20, pl05_damage_21, pl05_damage_22, pl05_damage_23,
	pl05_damage_24, pl05_damage_25, pl05_damage_26, pl05_damage_27, pl05_damage_28, pl05_damage_29,
	pl05_damage_30, pl05_damage_31, pl05_damage_32, pl05_damage_32, pl05_damage_13, 0,
};

const Sint32 *const pl05_dizzy_tbl[] = {
	pl05_dizzy_00, 0,
};

const Sint32 *const pl05_catch_tbl[] = {
	pl05_catch_00, pl05_catch_01, pl05_catch_02, pl05_catch_03, pl05_catch_04, pl05_catch_05,
	pl05_catch_06, pl05_catch_07, pl05_catch_08, 0,
};

const Sint32 *const pl05_caught_tbl[] = {
	pl05_caught_00, pl05_caught_01, pl05_caught_02, pl05_caught_03, pl05_caught_04, pl05_caught_05,
	pl05_caught_06, pl05_caught_07, pl05_caught_08, pl05_caught_09, pl05_caught_10, pl05_caught_11,
	pl05_caught_12, pl05_caught_13, pl05_caught_14, pl05_caught_15, pl05_caught_16, pl05_caught_17,
	pl05_caught_18, pl05_caught_19, pl05_caught_20, pl05_caught_21, 0,
};

const Sint32 *const pl05_get_up_tbl[] = {
	pl05_get_up_00, pl05_get_up_01, pl05_get_up_02, pl05_get_up_03, 0,
};

const Sint32 *const pl05_appear_tbl[] = {
	pl05_appear_00, pl05_appear_01, pl05_appear_02, pl05_appear_03, 0,
};

const Sint32 *const pl05_win_tbl[] = {
	pl05_win_00, pl05_win_01, pl05_win_02, pl05_win_03, pl05_win_04, pl05_win_05,
	0,
};

const Sint32 *const pl05_extra_tbl[] = {
	pl05_extra_00, pl05_extra_01, pl05_jump_12, pl05_extra_03, pl05_extra_04, pl05_extra_05,
	pl05_extra_06, pl05_extra_07, pl05_extra_08, pl05_extra_09, pl05_extra_10, pl05_extra_11,
	pl05_extra_12, pl05_extra_13, pl05_extra_14, pl05_extra_15, pl05_extra_16, pl05_extra_17,
	0,
};

const Sint32 *const pl05_special_tbl[] = {
	pl05_special_00, pl05_special_01, pl05_special_02, pl05_special_03, pl05_special_04, pl05_special_05,
	pl05_special_06, pl05_special_06, pl05_special_06, pl05_special_09, pl05_special_10, pl05_special_10,
	pl05_special_10, pl05_special_13, pl05_special_14, pl05_special_14, pl05_special_14, pl05_special_17,
	pl05_special_18, pl05_special_18, pl05_special_18, pl05_special_21, pl05_special_22, pl05_special_22,
	pl05_special_24, pl05_special_25, pl05_special_25, pl05_special_27, pl05_special_28, pl05_special_29,
	pl05_special_30, pl05_special_31, pl05_special_32, pl05_special_33, pl05_special_34, pl05_special_35,
	pl05_special_36, pl05_special_37, pl05_special_38, pl05_special_39, pl05_special_40, pl05_special_41,
	pl05_special_42, pl05_special_43, pl05_special_44, pl05_special_45, pl05_special_46, pl05_special_47,
	pl05_special_48, pl05_special_49, pl05_special_50, pl05_special_51, pl05_special_52, pl05_special_53,
	pl05_special_54, 0,
};

const Sint32 *const pl05_super_tbl[] = {
	pl05_super_00, pl05_super_00, pl05_super_02, pl05_super_02, pl05_super_04, pl05_super_04,
	pl05_super_06, pl05_super_07, pl05_super_08, pl05_super_09, pl05_super_10, pl05_super_11,
	pl05_super_12, pl05_super_13, pl05_super_14, pl05_super_15, pl05_super_16, pl05_super_17,
	pl05_super_18, pl05_super_19, pl05_super_20, pl05_super_21, pl05_super_22, pl05_super_23,
	pl05_super_24, pl05_super_25, pl05_super_26, pl05_super_27, pl05_super_28, pl05_super_29,
	pl05_super_30, pl05_super_31, pl05_super_32, pl05_super_33, 0,
};

const Sint32 *const pl05_effect_tbl[] = {
	pl05_effect_00, pl05_effect_01, pl05_effect_02, pl05_effect_03, pl05_effect_04, pl05_effect_05,
	pl05_effect_06, pl05_effect_07, pl05_effect_08, pl05_effect_08, pl05_effect_08, pl05_effect_08,
	pl05_effect_12, pl05_effect_13, pl05_effect_14, pl05_effect_15, pl05_effect_16, pl05_effect_17,
	pl05_effect_18, pl05_effect_19, pl05_effect_20, pl05_effect_21, pl05_effect_22, pl05_effect_23,
	pl05_effect_24, pl05_effect_25, pl05_effect_26, pl05_effect_27, pl05_effect_28, pl05_effect_28,
	pl05_effect_30, pl05_effect_31, pl05_effect_32, pl05_effect_33, pl05_effect_34, pl05_effect_35,
	pl05_effect_36, pl05_effect_37, pl05_effect_38, 0,
};

const Sint32 *const pl05_change_tbl[] = {
	pl05_change_00, pl05_change_01, pl05_change_02, pl05_change_03, pl05_change_04, 0,
};

const Sint32 *const pl05_grp25_tbl[] = {
	pl05_stand_00, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear1_init, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0 */
const Sint32 pl05_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl05_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_stand_02 ),
};

/* Char_Stand 6
   Char_Stand 8 */
const Sint32 pl05_stand_06[] = {
	CG_FRAME(    0,    0,   6,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_stand_06 ),
};

/* Char_Stand 3 */
const Sint32 pl05_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_stand_03 ),
};

/* Char_Stand 7
   Char_Stand 9 */
const Sint32 pl05_stand_07[] = {
	CG_FRAME(    0,    0,   6,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl05_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl05_jump_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_jump_00_loop ),
};

/* Char_Jump 1: stick_off_init (Kabe Hari Tsuki), act_wall_jump, act_change_gattai_init, act_change_zero_init, ... */
const Sint32 pl05_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl05_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   231, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl05_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   235, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl05_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   242, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_jump_02_loop ),
};

/* Char_Jump 3: appear0_fall, appear2_in, warp_fall (Warp), stick_off_fall (Kabe Hari Tsuki), ...
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl05_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl05_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl05_jump_09[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl05_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl05_jump_10[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_jump_10 ),
};

/* Char_Jump 8: kabe_init (Kabe Hari Tsuki) */
const Sint32 pl05_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl05_jump_11[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl05_dash_00[] = {
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_dash_00 ),
};

/* Char_Dash 2 */
const Sint32 pl05_dash_02[] = {
	CG_FRAME(    0,    1,   3,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,  20,           0,   122, 0x80,   0x00,                0,  0x00, 0x29 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_dash_02 ),
};

/* Char_Dash 1: jump_p_catch_land, act_back_dash */
const Sint32 pl05_dash_01[] = {
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   430, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   428, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_dash_01 ),
};

/* Char_Dash 3: jump_p_catch_back */
const Sint32 pl05_dash_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl05_crouch_00[] = {
	CG_FRAME(    0,    1,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl05_crouch_01[] = {
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl05_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   4,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl05_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl05_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl05_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl05_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   158, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl05_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl05_guard_03[] = {
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   158, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl05_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,   159, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl05_guard_05[] = {
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl05_guard_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl05_guard_07[] = {
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl05_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl05_air_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl05_air_guard_02[] = {
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl05_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   278, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   280, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   281, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl05_stand_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   282, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x90,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   284, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,   3,           0,   285, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,   2,           0,   286, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   292, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   288, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   291, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl05_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl05_stand_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   293, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,   36,   2,           0,   294, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x90,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   3,           0,   296, 0x90,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   297, 0x90,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME( 0x80,    0,   2,           0,   298, 0x90,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x90,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x90,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x90,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x90,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x90,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,  -36,   4,           0,   302, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   290, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl05_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   304, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   305, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   3,           0,   305, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   306, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   307, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   281, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl05_stand_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   308, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   309, 0x90,   0x00,              ATK,  0x00,       0x06,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   310, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   311, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   312, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   313, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   316, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   313, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   316, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   291, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl05_stand_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   317, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   318, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,   32,   2,           0,   319, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    4,   2,           0,   320, 0x90,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   321, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   2,           0,   322, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   2,           0,   323, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   324, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   326, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   290, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl05_crouch_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   327, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   3,           0,   327, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   330, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl05_crouch_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   331, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   332, 0x90,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   333, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   2,           0,   334, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   2,           0,   335, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   336, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   341, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   336, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   337, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   338, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   340, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl05_crouch_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   342, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   343, 0x90,   0x09,              ATK,  0x00,       0x05,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   344, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   345, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   2,           0,   346, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   347, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   351, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   351, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x90,   0x09,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   350, 0x90,   0x09,              ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   363, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   340, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl05_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   352, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   353, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   3,           0,   353, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   354, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   352, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium): appear2_init */
const Sint32 pl05_crouch_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   356, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x90,   0x00,              ATK,  0x00,       0x06,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   358, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2,           0,   359, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   340, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl05_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   365, 0x90,   0x60,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x90,   0x60,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x90,   0x60,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   368, 0x90,   0x60,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   2,           0,   369, 0x90,   0x60,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   2,           0,   370, 0x90,   0x60,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   3,           0,   371, 0x90,   0x60,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   3,           0,   374, 0x90,   0x60,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4,           0,   371, 0x90,   0x60,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4,           0,   374, 0x90,   0x60,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x90,   0x60,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   3,           0,   373, 0x90,   0x60,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   339, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_k_02 ),
};

/* Char_Crouch_K 3 */
const Sint32 pl05_crouch_k_03[] = {
	CG_FRAME(    0,    0,   4,           0,   428, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   429, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   430, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00, 0x29 | CSE,    0,   0,     0,      0,   7 ),
};

const Sint32 pl05_crouch_k_03_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   432, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   433, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl05_crouch_k_03_loop ),
};

/* Char_Crouch_K 4: slide_slide */
const Sint32 pl05_crouch_k_04[] = {
	CG_FRAME(    0,    0,   4,           0,   206, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   147, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_crouch_k_04 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl05_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl05_jump_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   378, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0x90,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   3,           0,   380, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   381, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   382, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   385, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl05_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   404, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0x90,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   3,           0,   389, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   2,           0,   390, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   2,           0,   391, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   392, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   394, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   392, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   394, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   392, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   394, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   2,           0,   393, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   385, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl05_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   395, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   2,           0,   396, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   397, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   396, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   397, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   396, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4,           0,   395, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl05_jump_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   398, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   399, 0x90,   0x00,              ATK,  0x00,       0x06,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   3,           0,   400, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   401, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   402, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   404, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   405, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   131, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   132, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl05_jump_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   407, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   408, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x90,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   3,           0,   410, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,   2,           0,   411, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,   2,           0,   412, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   413, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   413, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   413, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   405, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl05_damage_00[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl05_damage_01[] = {
	CG_FRAME(    0,    0,   2,           0,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl05_damage_02[] = {
	CG_FRAME(    0,    0,   2,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   545, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl05_damage_03[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl05_damage_04[] = {
	CG_FRAME(    0,    0,   2,           0,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  14,           0,   547, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl05_damage_05[] = {
	CG_FRAME(    0,    0,   2,           0,   547, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   548, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl05_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl05_damage_07[] = {
	CG_FRAME(    0,    0,   2,           0,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  14,           0,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl05_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  16,           0,   551, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl05_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   558, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   559, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   560, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   571, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   557, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl05_damage_09_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   555, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl05_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl05_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   552, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl05_damage_11[] = {
	CG_FRAME(    0,    0,   2,           0,   552, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   553, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl05_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   553, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   554, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl05_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   555, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   557, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl05_damage_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   555, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl05_damage_13_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl05_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   564, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   565, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl05_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   566, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl05_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   567, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl05_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   568, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl05_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   569, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl05_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   564, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 22 */
const Sint32 pl05_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   566, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 23 */
const Sint32 pl05_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   567, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 24 */
const Sint32 pl05_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   568, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 25 */
const Sint32 pl05_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   569, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl05_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   557, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   555, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl05_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl05_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   585, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   586, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl05_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   572, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   575, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   576, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   577, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl05_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   578, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   579, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   581, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   582, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   583, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl05_damage_30[] = {
	CG_FRAME(    0,    0,   2,           0,   140, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl05_damage_31[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   555, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl05_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl05_damage_32[] = {
	CG_FRAME(    0,    0,   2,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   140, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_damage_32 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl05_dizzy_00[] = {
	CG_FRAME(    0,    0,   8,           0,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_dizzy_00 ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl05_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
};

const Sint32 pl05_catch_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_NEXT( pl05_catch_00_loop ),
};

/* Char_Catch 1: nml_k_catch_start */
const Sint32 pl05_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
};

const Sint32 pl05_catch_01_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   422, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_NEXT( pl05_catch_01_loop ),
};

/* Char_Catch 2: nml_k_catch_jump */
const Sint32 pl05_catch_02[] = {
	CG_FRAME(    0,    0,   1,           0,   422, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0x80,   0x4e,                0,  0x00,       0x05,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   4,           0,   423, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   4,           0,   424, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  20,           0,   425, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   281, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_catch_02 ),
};

/* Char_Catch 3: jump_p_set */
const Sint32 pl05_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   481, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   4,           0,   482, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   4,           0,   485, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
};

const Sint32 pl05_catch_03_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   486, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_NEXT( pl05_catch_03_loop ),
};

/* Char_Catch 4: jump_p_catch_fall */
const Sint32 pl05_catch_04[] = {
	CG_FRAME(    0,    0,   6,           0,   487, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
};

const Sint32 pl05_catch_04_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   488, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_NEXT( pl05_catch_04_loop ),
};

/* Char_Catch 5: ragnarok_dash (Ragnarok), nml_p_set */
const Sint32 pl05_catch_05[] = {
	CG_FRAME(    0,    0,   1,           0,   489, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,  11,           0,   489, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    1,   3,           0,   491, 0x80,   0x20,                0,  0x00, 0x32 | CSE,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   3,           0,   492, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
};

const Sint32 pl05_catch_05_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   491, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   492, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_NEXT( pl05_catch_05_loop ),
};

/* Char_Catch 6: nml_p_catch_up */
const Sint32 pl05_catch_06[] = {
	CG_FRAME(    0,    0,   6,           0,   416, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   6,           0,   417, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    1,   6,           0,   418, 0x80,   0x20,                0,  0x00,       0x04,    0,   0,     0, 0x0015,   0 ),
};

const Sint32 pl05_catch_06_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   418, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_NEXT( pl05_catch_06_loop ),
};

/* Char_Catch 7: nml_p_catch_fall */
const Sint32 pl05_catch_07[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   419, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_NEXT( pl05_catch_07 ),
};

/* Char_Catch 8: nml_p_catch_land */
const Sint32 pl05_catch_08[] = {
	CG_FRAME(    0,    0,   8,           0,   429, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   430, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   431, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   7,           0,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_catch_08_loop[] = {
	CG_FRAME(    0,    1,   7, END_OF_DATA,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_catch_08_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl05_get_up_00[] = {
	CG_FRAME(    0,    0,   7,           0,   205, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   206, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   207, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   208, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   148, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl05_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   214, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl05_get_up_02[] = {
	CG_FRAME(    0,    0,   5,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   214, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl05_get_up_03[] = {
	CG_FRAME(    0,    0,   5,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_get_up_03 ),
};

/* Char_Appear 0: appear0_init */
const Sint32 pl05_appear_00[] = {
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x72,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x72,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   199, 0x80,   0x72,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   200, 0x80,   0x72,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_appear_00 ),
};

/* Char_Appear 1: appear0_fly */
const Sint32 pl05_appear_01[] = {
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_appear_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_appear_01_loop ),
};

/* Char_Appear 2: shl05_03_init1 */
const Sint32 pl05_appear_02[] = {
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   204, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_appear_02 ),
};

/* Char_Appear 3: appear1_wait */
const Sint32 pl05_appear_03[] = {
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   331, 0x90,   0x00,                0,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   332, 0x90,   0x00,                0,  0x00,       0x1e,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   333, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   334, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   335, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   341, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   337, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   338, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   339, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   3,           0,   340, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_appear_03 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl05_extra_00[] = {
	CG_FRAME(    0,    0,   2,           0,   395, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl05_extra_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   817, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   818, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl05_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl05_extra_01[] = {
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   175, 0x80,   0x00,                0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   176, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   177, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   180, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   183, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_extra_01 ),
};

/* Char_Jump 12: act_wall_jump
   Char_Extra 2: fly_start5 */
const Sint32 pl05_jump_12[] = {
	CG_FRAME(    0,    0,   1,           0,   434, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   434, 0x80,   0x00,                0,  0x00, 0x23 | CSE,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   435, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_jump_12 ),
};

/* Char_Extra 3: stick_wait_init (Kabe Hari Tsuki), fly_start5 */
const Sint32 pl05_extra_03[] = {
	CG_FRAME(    0,    0,   4,           0,   790, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   791, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_03 ),
};

/* Char_Extra 4: stick_up_init (Kabe Hari Tsuki) */
const Sint32 pl05_extra_04[] = {
	CG_FRAME(    0,    0,   4,           0,   796, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   797, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   798, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_04 ),
};

/* Char_Extra 5: stick_down_init (Kabe Hari Tsuki) */
const Sint32 pl05_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0,   801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   802, 0x80,   0x00,                0,  0x00, 0x29 | CSE,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   802, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_05 ),
};

/* Char_Extra 6: kabe_jump (Kabe Hari Tsuki), stick_dash_jump (Kabe Hari Tsuki) */
const Sint32 pl05_extra_06[] = {
	CG_FRAME(    0,    0,   4,           0,   803, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   804, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   800, 0x80,   0x00,                0,  0x00, 0x23 | CSE,    0,   0,     0,      0,  66 ),
};

const Sint32 pl05_extra_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_06_loop ),
};

/* Char_Extra 7: stick_dash_init (Kabe Hari Tsuki) */
const Sint32 pl05_extra_07[] = {
	CG_FRAME(    0,    0,   4,           0,   801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   804, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   803, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_07 ),
};

/* Char_Extra 8: stick_dash_start (Kabe Hari Tsuki) */
const Sint32 pl05_extra_08[] = {
	CG_FRAME(    0,    0,   4,           0,   805, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   806, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl05_extra_08 ),
};

/* Char_Extra 9: stick_punch_init (Kabe Hari Tsuki) */
const Sint32 pl05_extra_09[] = {
	CG_FRAME(    0,    0,   4,           0,   807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   808, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   2,           0,   809, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    0,   2,           0,   810, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   811, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   812, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   813, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   814, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   815, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_09 ),
};

/* Char_Extra 10 */
const Sint32 pl05_extra_10[] = {
	CG_FRAME(    0,    1,   3,           0,   801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   816, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  66 ),
};

const Sint32 pl05_extra_10_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   819, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   820, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl05_extra_10_loop ),
};

/* Char_Extra 11 */
const Sint32 pl05_extra_11[] = {
	CG_FRAME(    0,    1,   3,           0,   801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   816, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  66 ),
};

const Sint32 pl05_extra_11_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   817, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   818, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl05_extra_11_loop ),
};

/* Char_Extra 12 */
const Sint32 pl05_extra_12[] = {
	CG_FRAME(    0,    1,   3,           0,   801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   3,           0,   803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   816, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  66 ),
};

const Sint32 pl05_extra_12_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   817, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   818, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl05_extra_12_loop ),
};

/* Char_Extra 13: stick_kick_jump (Kabe Hari Tsuki) */
const Sint32 pl05_extra_13[] = {
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_extra_13 ),
};

/* Char_Extra 14: stick_slide_init (Kabe Hari Tsuki) */
const Sint32 pl05_extra_14[] = {
	CG_FRAME(    0,    1,   4,           0,   821, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    1,   4,           0,   822, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl05_extra_14_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   823, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   824, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl05_extra_14_loop ),
};

/* Char_Extra 15: stick_slide_move (Kabe Hari Tsuki) */
const Sint32 pl05_extra_15[] = {
	CG_FRAME(    0,    0,  10,           0,   822, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl05_extra_15_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   801, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl05_extra_15_loop ),
};

/* Char_Extra 16: sp_chouhatsu (The taunt) */
const Sint32 pl05_extra_16[] = {
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_extra_16 ),
};

/* Char_Extra 17: pl05_upper */
const Sint32 pl05_extra_17[] = {
	CG_FRAME(    0,    0,   3,           0,   342, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   343, 0x90,   0x09,              ATK,  0x00,       0x05,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   344, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   2,           0,   345, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,   2,           0,   346, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME( 0x80,    0,   2,           0,   347, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   351, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   351, 0x90,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x90,   0x09,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   3,           0,   350, 0x90,   0x09,              ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   363, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   340, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_extra_17 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl05_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl05_win_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   176, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   183, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl05_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl05_win_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   196, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_win_01_loop ),
};

/* Char_Win 2: win_ko_init, win_time_init */
const Sint32 pl05_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl05_win_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_win_02_loop ),
};

/* Char_Win 3: lose_time_init, lose_init */
const Sint32 pl05_win_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    -1, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_win_03 ),
};

/* Char_Win 4: win_ko_init */
const Sint32 pl05_win_04[] = {
	CG_FRAME(    0,    1,   4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   3,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl05_win_04_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_win_04_loop ),
};

/* Char_Win 5: win_ko3_turn */
const Sint32 pl05_win_05[] = {
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl05_win_05_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   262, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   263, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   268, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_win_05_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl05_change_00[] = {
	CG_FRAME(    0,    0,   3,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   269, 0x80,   0x53,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   270, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   270, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_change_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   271, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl05_change_01[] = {
	CG_FRAME( 0x80,    0,   4,           0,   448, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_change_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   541, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_change_01_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl05_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   150, 0x80,   0x00,                0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   277, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl05_change_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   281, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl05_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_change_04 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl05_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl05_change_02 ),
};

/* Char_Caught 0 */
const Sint32 pl05_caught_00[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl05_caught_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl05_caught_02[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   545, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl05_caught_03[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl05_caught_04[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   547, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl05_caught_05[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   548, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl05_caught_06[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl05_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl05_caught_07[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl05_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl05_caught_08[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   551, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl05_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl05_caught_09[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   552, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl05_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl05_caught_10[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   553, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl05_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl05_caught_11[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   554, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl05_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl05_caught_12[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   556, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl05_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl05_caught_13[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   564, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl05_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl05_caught_14[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   566, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl05_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl05_caught_15[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   567, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl05_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl05_caught_16[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   568, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl05_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl05_caught_17[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   569, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl05_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl05_caught_18[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   572, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl05_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl05_caught_19[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   578, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl05_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl05_caught_20[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   555, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl05_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl05_caught_21[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl05_caught_21 ),
};

/* Char_Special 54: snapback_init (The snapback) */
const Sint32 pl05_special_54[] = {
	CG_FRAME(    0,    0,   4,           0,   317, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,   32,   2,           0,   319, 0x90,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    4,   2,           0,   320, 0x90,   0x00,              ATK,  0x02,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   321, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   2,           0,   322, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   2,           0,   323, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   324, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   326, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   290, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_54 ),
};

/* Char_Special 0: murakumo_init (Ame no Murakumo) */
const Sint32 pl05_special_00[] = {
	CG_FRAME(    0,    1,   4,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl05_special_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   121, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl05_special_00_loop ),
};

/* Char_Special 1: murakumo_dash (Ame no Murakumo) */
const Sint32 pl05_special_01[] = {
	CG_FRAME(    0,    1,   3,           0,   317, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   318, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   319, 0x90,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   320, 0x90,   0x00,              ATK,  0x00,       0x06,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,   321, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    1,   2,           0,   322, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    1,   3,           0,   323, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,  16,           0,   324, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   325, 0x90,   0x6e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   289, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   290, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   291, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_01 ),
};

/* Char_Special 2: appear2_ride, warp_init (Warp) */
const Sint32 pl05_special_02[] = {
	CG_FRAME(    0,    0,   3,           0,   436, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   437, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_special_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   442, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_02_loop ),
};

/* Char_Special 3: appear2_out, warp_out (Warp) */
const Sint32 pl05_special_03[] = {
	CG_FRAME(    0,    0,   3,           0,   443, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   444, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   445, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_03 ),
};

/* Char_Special 4: warp_in (Warp) */
const Sint32 pl05_special_04[] = {
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   447, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl05_special_04 ),
};

/* Char_Special 5: gram_init (Gram) */
const Sint32 pl05_special_05[] = {
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl05_special_05_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   450, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   458, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_05_loop ),
};

/* Char_Special 9: lgram_init (Low Gram) */
const Sint32 pl05_special_09[] = {
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl05_special_09_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_special_09_loop ),
};

/* Char_Special 13: jgram_init (Gram (in the air)) */
const Sint32 pl05_special_13[] = {
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl05_special_13_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_13_loop ),
};

/* Char_Special 6
   Char_Special 7
   Char_Special 8 */
const Sint32 pl05_special_06[] = {
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   451, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   452, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   453, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   2,           0,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   2,           0,   455, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   456, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   291, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_06 ),
};

/* Char_Special 10
   Char_Special 11
   Char_Special 12 */
const Sint32 pl05_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   462, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   463, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   464, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME( 0x80,    0,   2,           0,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME( 0x80,    0,   2,           0,   466, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   2,           0,   467, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   340, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl05_special_10 ),
};

/* Char_Special 14
   Char_Special 15
   Char_Special 16 */
const Sint32 pl05_special_14[] = {
	CG_FRAME(    0,    0,   1,           0,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   473, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,   2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME( 0x80,    0,   2,           0,   476, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME( 0x80,    0,   2,           0,   477, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   3,           0,   478, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   386, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   386, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   385, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_14 ),
};

/* Char_Special 17: formation_a_init (Formation A) */
const Sint32 pl05_special_17[] = {
	CG_FRAME(    0,    0,   7,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   3,           0,   275, 0x80,   0x00,              ATK,  0x00,       0x0c,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl05_special_17_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   281, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_17_loop ),
};

/* Char_Special 18
   Char_Special 19
   Char_Special 20 */
const Sint32 pl05_special_18[] = {
	CG_FRAME(    0,    0,   2,           0,   732, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   733, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   734, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   735, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   736, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   737, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   738, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl05_special_18_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   739, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_18_loop ),
};

/* Char_Special 24 */
const Sint32 pl05_special_24[] = {
	CG_FRAME(    0,    1,   2,           0,   740, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1d00,  84 ),
	CG_FRAME(    0,    0,   2,           0,   741, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1d00,  84 ),
};

const Sint32 pl05_special_24_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   740, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1d00,  84 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   741, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1d00,  84 ),
	CG_NEXT( pl05_special_24_loop ),
};

/* Char_Special 25
   Char_Special 26 */
const Sint32 pl05_special_25[] = {
	CG_FRAME(    0,    1,   2,           0,   744, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1f00,  86 ),
	CG_FRAME(    0,    0,   2,           0,   745, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1f00,  86 ),
};

const Sint32 pl05_special_25_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   744, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1f00,  86 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   745, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1f00,  86 ),
	CG_NEXT( pl05_special_25_loop ),
};

/* Char_Special 21 */
const Sint32 pl05_special_21[] = {
	CG_FRAME(    0,    1,   2,           0,   750, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2200,  87 ),
	CG_FRAME(    0,    0,   2,           0,   751, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2200,  87 ),
};

const Sint32 pl05_special_21_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   750, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2200,  87 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   751, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2200,  87 ),
	CG_NEXT( pl05_special_21_loop ),
};

/* Char_Special 22
   Char_Special 23 */
const Sint32 pl05_special_22[] = {
	CG_FRAME(    0,    1,   2,           0,   746, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,  89 ),
	CG_FRAME(    0,    0,   2,           0,   747, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,  89 ),
};

const Sint32 pl05_special_22_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   746, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,  89 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   747, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x2000,  89 ),
	CG_NEXT( pl05_special_22_loop ),
};

/* Char_Special 27: jexcalibur_rush (Excalibur (in the air)), jexcalibur_fall (Excalibur (in the air)) */
const Sint32 pl05_special_27[] = {
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_27 ),
};

/* Char_Special 53: jexcalibur_rush (Excalibur (in the air)) */
const Sint32 pl05_special_53[] = {
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl05_special_53_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   134, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_53_loop ),
};

/* Char_Special 28: vajra_init (Vajra) */
const Sint32 pl05_special_28[] = {
	CG_FRAME(    0,    0,   3,           0,   681, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   682, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl05_special_28_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   683, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_28_loop ),
};

/* Char_Special 29: shl05_04_init (29 + type), shl05_05_up_init (29 + sub_type) */
const Sint32 pl05_special_29[] = {
	CG_FRAME(    0,    0,   3,           0,   684, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   685, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl05_special_30[] = {
	CG_FRAME(    0,    0,   3,           0,   689, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   690, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_30 ),
};

/* Char_Special 31: vajra_start (Vajra) */
const Sint32 pl05_special_31[] = {
	CG_FRAME(    0,    0,   3,           0,   694, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   695, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_31 ),
};

/* Char_Special 32 */
const Sint32 pl05_special_32[] = {
	CG_FRAME(    0,    0,   3,           0,   699, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_32 ),
};

/* Char_Special 33: shl05_04_out (33 + type), shl05_05_up_leap (33 + sub_type) */
const Sint32 pl05_special_33[] = {
	CG_FRAME(    0,    0,   4,           0,   686, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   687, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   688, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_33 ),
};

/* Char_Special 34 */
const Sint32 pl05_special_34[] = {
	CG_FRAME(    0,    0,   4,           0,   691, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   692, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   693, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_34 ),
};

/* Char_Special 35: vajra_jump (Vajra) */
const Sint32 pl05_special_35[] = {
	CG_FRAME(    0,    0,   4,           0,   696, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   697, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   698, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_35 ),
};

/* Char_Special 36 */
const Sint32 pl05_special_36[] = {
	CG_FRAME(    0,    0,   4,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   702, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   703, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_36 ),
};

/* Char_Special 37: shl05_04_wait (37 + type) */
const Sint32 pl05_special_37[] = {
	CG_FRAME(    0,    0,   2,           0,   704, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl05_special_37_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   705, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   706, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   707, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   704, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl05_special_37_loop ),
};

/* Char_Special 38 */
const Sint32 pl05_special_38[] = {
	CG_FRAME(    0,    0,   2,           0,   711, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
};

const Sint32 pl05_special_38_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   712, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   711, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl05_special_38_loop ),
};

/* Char_Special 39: vajra_in (Vajra) */
const Sint32 pl05_special_39[] = {
	CG_FRAME(    0,    0,   2,           0,   718, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl05_special_39_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   719, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   720, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   721, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   718, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl05_special_39_loop ),
};

/* Char_Special 40 */
const Sint32 pl05_special_40[] = {
	CG_FRAME(    0,    0,   2,           0,   725, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl05_special_40_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   726, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0,   727, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0,   728, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   725, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl05_special_40_loop ),
};

/* Char_Special 41: shl05_04_last (41 + type), shl05_05_down_wait (41 + sub_type) */
const Sint32 pl05_special_41[] = {
	CG_FRAME(    0,    0,   4,           0,   708, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   709, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   710, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_41 ),
};

/* Char_Special 42 */
const Sint32 pl05_special_42[] = {
	CG_FRAME(    0,    0,   4,           0,   715, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   716, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   717, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_42 ),
};

/* Char_Special 43 */
const Sint32 pl05_special_43[] = {
	CG_FRAME(    0,    0,   4,           0,   722, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   723, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   724, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_43 ),
};

/* Char_Special 44 */
const Sint32 pl05_special_44[] = {
	CG_FRAME(    0,    0,   4,           0,   729, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   730, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   731, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_special_44 ),
};

/* Char_Special 45: shl05_05_down_init (45 + sub_type) */
const Sint32 pl05_special_45[] = {
	CG_FRAME(    0,    0,   4,           0,   710, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   709, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   708, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_45 ),
};

/* Char_Special 46 */
const Sint32 pl05_special_46[] = {
	CG_FRAME(    0,    0,   4,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   715, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_46 ),
};

/* Char_Special 47: vajra_out (Vajra) */
const Sint32 pl05_special_47[] = {
	CG_FRAME(    0,    0,   4,           0,   724, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   723, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   722, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_47 ),
};

/* Char_Special 48 */
const Sint32 pl05_special_48[] = {
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   730, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   729, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_48 ),
};

/* Char_Special 49: formation_c_init (Formation C) */
const Sint32 pl05_special_49[] = {
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   777, 0x80,   0x00,              ATK,  0x00,       0x0d,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   281, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_49 ),
};

/* Char_Special 50: formation_b_init (Formation B) */
const Sint32 pl05_special_50[] = {
	CG_FRAME(    0,    0,   3,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   269, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   3,           0,   270, 0x80,   0x53,                0,  0x00,       0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl05_special_50_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_special_50_loop ),
};

/* Char_Special 51 */
const Sint32 pl05_special_51[] = {
	CG_FRAME(    0,    0,   4,           0,   783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   785, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   4,           0,   786, 0x80,   0x00,              ATK,  0x00,       0x0b,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   785, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_special_51 ),
};

/* Char_Special 52 */
const Sint32 pl05_special_52[] = {
	CG_FRAME(    0,    0,   4,           0,   778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   4,           0,   788, 0x80,   0x00,              ATK,  0x00,       0x0b,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl05_special_52 ),
};

/* Char_Super 0: legion_init (Legion)
   Char_Super 1 */
const Sint32 pl05_super_00[] = {
	CG_FRAME(    0,    0,   8,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   8,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   275, 0x80,   0x00,              ATK,  0x00,       0x0c,    0,   0,     0,      0,   6 ),
};

const Sint32 pl05_super_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_super_00_loop ),
};

/* Char_Super 33: legion_keep (Legion) */
const Sint32 pl05_super_33[] = {
	CG_FRAME(    0,    0,   3,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   281, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_super_33 ),
};

/* Char_Super 2: ouroboros_init (Ouroboros)
   Char_Super 3 */
const Sint32 pl05_super_02[] = {
	CG_FRAME(    0,    0,   3,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   307, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   306, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8,           0,   269, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,  12,           0,   269, 0x80,   0x53,              ATK,  0x00,       0x05,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   3,           0,   270, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl05_super_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   271, 0x80,   0x53,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_super_02_loop ),
};

/* Char_Super 4: ragnarok_init (Ragnarok)
   Char_Super 5 */
const Sint32 pl05_super_04[] = {
	CG_FRAME(    0,    0,   3,           0,   291, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   290, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   172, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   3,           0,   173, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl05_super_04_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   174, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_super_04_loop ),
};

/* Char_Super 6 */
const Sint32 pl05_super_06[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_06 ),
};

/* Char_Super 18: shl05_04_pass (18 + cg) */
const Sint32 pl05_super_18[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

const Sint32 pl05_super_29_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 7 */
const Sint32 pl05_super_07[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_07 ),
};

/* Char_Super 19 */
const Sint32 pl05_super_19[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 8 */
const Sint32 pl05_super_08[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_08 ),
};

/* Char_Super 20 */
const Sint32 pl05_super_20[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 9 */
const Sint32 pl05_super_09[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    3,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    3,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_09 ),
};

/* Char_Super 21 */
const Sint32 pl05_super_21[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 10 */
const Sint32 pl05_super_10[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    4,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    4,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_10 ),
};

/* Char_Super 22 */
const Sint32 pl05_super_22[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 11 */
const Sint32 pl05_super_11[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    5,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    5,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_11 ),
};

/* Char_Super 23 */
const Sint32 pl05_super_23[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    5,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 12 */
const Sint32 pl05_super_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    6,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    6,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_12 ),
};

/* Char_Super 24 */
const Sint32 pl05_super_24[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    6,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 13 */
const Sint32 pl05_super_13[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    7,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    7,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_13 ),
};

/* Char_Super 25 */
const Sint32 pl05_super_25[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    7,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 14 */
const Sint32 pl05_super_14[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    8,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    8,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_14 ),
};

/* Char_Super 26 */
const Sint32 pl05_super_26[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 15 */
const Sint32 pl05_super_15[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    9,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    9,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_15 ),
};

/* Char_Super 27 */
const Sint32 pl05_super_27[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,    9,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 16 */
const Sint32 pl05_super_16[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,   10,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,   10,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_16 ),
};

/* Char_Super 28 */
const Sint32 pl05_super_28[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,   10,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,   10,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 17 */
const Sint32 pl05_super_17[] = {
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,   11,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,   11,   0,     0,      0,  79 ),
	CG_NEXT( pl05_super_17 ),
};

/* Char_Super 29 */
const Sint32 pl05_super_29[] = {
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x80,   0x00,              ATK,  0x00,          0,   11,   0,     0,      0,   0 ),
	CG_NEXT( pl05_super_29_loop ),
};

/* Char_Super 30: ragnarok_start (Ragnarok) */
const Sint32 pl05_super_30[] = {
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   121, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl05_super_30 ),
};

/* Char_Super 31: ragnarok_dash (Ragnarok) */
const Sint32 pl05_super_31[] = {
	CG_FRAME(    0,    1,   3,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,  12,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl05_super_31 ),
};

/* Char_Super 32: ragnarok_hide (Ragnarok) */
const Sint32 pl05_super_32[] = {
	CG_FRAME(    0,    0,   6,           0,   443, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   444, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   445, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl05_super_32 ),
};

/* Char_Effect 0: shl05_03_bird_init */
const Sint32 pl05_effect_00[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   607, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_00 ),
};

/* Char_Effect 1: shl05_03_bird_fly, shl05_03_5_fly */
const Sint32 pl05_effect_01[] = {
	CG_FRAME(    0,    0,   8,           0,   608, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_effect_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   609, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_01_loop ),
};

/* Char_Effect 2 */
const Sint32 pl05_effect_02[] = {
	CG_FRAME(    0,    0,   4,           0,   595, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   594, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   593, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_effect_02_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_02_loop ),
};

/* Char_Effect 3: shl05_00_circle */
const Sint32 pl05_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,   596, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   598, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_03 ),
};

/* Char_Effect 4: shl05_00_land, shl05_03_3_appear */
const Sint32 pl05_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   601, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   603, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   605, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl05_effect_05[] = {
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   605, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   603, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   602, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   601, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_05 ),
};

/* Char_Effect 6: shl05_00_char_set */
const Sint32 pl05_effect_06[] = {
	CG_FRAME(    0,    0,   4,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32785, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32792, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32793, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32794, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32795, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0, 32808, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32809, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl05_effect_06 ),
};

/* Char_Effect 7: shl05_01_flash_init */
const Sint32 pl05_effect_07[] = {
	CG_FRAME(    0,    0,   4,           0,   643, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   641, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_effect_07_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   642, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_07_loop ),
};

/* Char_Effect 8: shl05_01_shot_init
   Char_Effect 9
   Char_Effect 10
   Char_Effect 11 */
const Sint32 pl05_effect_08[] = {
	CG_FRAME(    0,    0,   3,           0,   637, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   638, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   639, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl05_effect_08_loop[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA, 32822, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl05_effect_08_loop ),
};

/* Char_Effect 12: pls05_01_init0 */
const Sint32 pl05_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0,   636, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   619, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   621, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   623, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   624, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_12 ),
};

/* Char_Effect 13: pls05_01_init1 */
const Sint32 pl05_effect_13[] = {
	CG_FRAME(    0,    0,   4,           0, 32811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32813, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32815, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_13 ),
};

/* Char_Effect 14: pls05_01_init2 */
const Sint32 pl05_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0, 32816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_14 ),
};

/* Char_Effect 15: pls05_01_init3 */
const Sint32 pl05_effect_15[] = {
	CG_FRAME(    0,    0,   4,           0, 32819, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32820, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32821, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_15 ),
};

/* Char_Effect 16: shl05_02_init0, shl05_02_init5, shl05_02_1_jump_land */
const Sint32 pl05_effect_16[] = {
	CG_FRAME(    0,    0,   4,           0, 32823, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    2,    0,   4,           0, 32824, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    4,    0,   4,           0, 32825, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    6,    0,   4,           0, 32826, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    8,    0,   4,           0, 32827, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(   10,    0,   4, END_OF_DATA, 32828, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl05_effect_16 ),
};

/* Char_Effect 17: shl05_02_init1, shl05_02_0_run, shl05_02_hit */
const Sint32 pl05_effect_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32829, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl05_effect_17 ),
};

/* Char_Effect 18: shl05_02_1_wait, shl05_02_1_jump */
const Sint32 pl05_effect_18[] = {
	CG_FRAME(    0,    0,   4,           0, 32831, 0x80,   0x6a,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32832, 0x80,   0x6a,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl05_effect_18 ),
};

/* Char_Effect 19: shl05_02_1_land */
const Sint32 pl05_effect_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32830, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl05_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl05_effect_20[] = {
	CG_FRAME(    0,    0,   4,           0,   650, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   651, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32832, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   650, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   651, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32832, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   650, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   651, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32832, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   648, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   649, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   648, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   649, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_20 ),
};

/* Char_Effect 21: shl05_02_0_run, shl05_02_catch */
const Sint32 pl05_effect_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32833, 0x80,   0x6a,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_21 ),
};

/* Char_Effect 22: shl05_03_init3 */
const Sint32 pl05_effect_22[] = {
	CG_FRAME(    0,    0,   1,           0,   679, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_effect_22_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   680, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_22_loop ),
};

/* Char_Effect 23: shl05_02_4_wait */
const Sint32 pl05_effect_23[] = {
	CG_FRAME(    0,    0,   2,           0,   675, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   677, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   678, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl05_effect_23 ),
};

/* Char_Effect 24: pls05_00_init, shl05_03_init4 */
const Sint32 pl05_effect_24[] = {
	CG_FRAME(    0,    0,   1,           0,   653, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   654, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   655, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   656, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   657, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   658, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   659, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   660, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   661, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   662, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   663, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   664, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl05_effect_25[] = {
	CG_FRAME(    0,    0,   3,           0,   672, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   673, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   674, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   665, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   666, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   667, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   668, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   669, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   670, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   671, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl05_effect_25 ),
};

/* Char_Effect 26: shl05_03_bomb_init */
const Sint32 pl05_effect_26[] = {
	CG_FRAME(    0,    0,   2,           0,   610, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   612, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    1,   4,           0,   613, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
};

const Sint32 pl05_effect_26_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   613, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl05_effect_26_loop ),
};

/* Char_Effect 27: shl05_02_init4 */
const Sint32 pl05_effect_27[] = {
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32790, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32791, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32792, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32794, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32797, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32798, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32799, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32800, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32801, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32802, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32803, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32804, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32805, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32806, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32807, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32808, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32809, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_27 ),
};

/* Char_Effect 28
   Char_Effect 29 */
const Sint32 pl05_effect_28[] = {
	CG_FRAME(    0,    0,   2,           0,   752, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   753, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   754, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   755, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_28 ),
};

/* Char_Effect 30 */
const Sint32 pl05_effect_30[] = {
	CG_FRAME(    0,    0,   2,           0,   756, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   757, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   758, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   759, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_30 ),
};

/* Char_Effect 31 */
const Sint32 pl05_effect_31[] = {
	CG_FRAME(    0,    0,   2,           0,   760, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   762, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   763, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_31 ),
};

/* Char_Effect 32 */
const Sint32 pl05_effect_32[] = {
	CG_FRAME(    0,    0,   2,           0,   764, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   765, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   766, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   767, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_32 ),
};

/* Char_Effect 33 */
const Sint32 pl05_effect_33[] = {
	CG_FRAME(    0,    0,   2,           0,   768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   769, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_33 ),
};

/* Char_Effect 34 */
const Sint32 pl05_effect_34[] = {
	CG_FRAME(    0,    0,   2,           0,   772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_34 ),
};

/* Char_Effect 35: shl05_02_init2, shl05_02_init6, shl05_03_init5 */
const Sint32 pl05_effect_35[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32810, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl05_effect_35 ),
};

/* Char_Effect 36: shl05_03_init6 */
const Sint32 pl05_effect_36[] = {
	CG_FRAME(    0,    0,  16,           0, 32829, 0x80,   0x93,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_effect_36_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32830, 0x80,   0xa0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_36_loop ),
};

/* Char_Effect 37: shl05_03_6_fall */
const Sint32 pl05_effect_37[] = {
	CG_FRAME(    0,    0,   6,           0, 32831, 0x80,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0, 32832, 0x80,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   648, 0x80,   0xc0,                0,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   649, 0x80,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   650, 0x80,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl05_effect_37_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   651, 0x80,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl05_effect_37_loop ),
};

/* Char_Effect 38: shl05_03_bomb_land */
const Sint32 pl05_effect_38[] = {
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  27, END_OF_DATA,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl05_effect_38 ),
};
