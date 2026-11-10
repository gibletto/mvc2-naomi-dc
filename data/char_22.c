/* Sakura (character 22): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_22      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_22     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl22.c) says which group
                   and number each move uses.
   pl22_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl22_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl22_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl22_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_22
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_22, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_22 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl22_stand_00[], pl22_stand_02[], pl22_stand_03[], pl22_jump_00[],
	pl22_jump_09[], pl22_jump_01[], pl22_jump_01_loop[], pl22_jump_02[], pl22_jump_02_loop[],
	pl22_jump_03[], pl22_jump_06[], pl22_jump_06_loop[], pl22_jump_07[], pl22_jump_07_loop[],
	pl22_jump_08[], pl22_jump_08_loop[], pl22_dash_04[], pl22_dash_04_loop[], pl22_dash_06[],
	pl22_dash_00[], pl22_dash_01[], pl22_dash_02[], pl22_crouch_00[], pl22_crouch_01[],
	pl22_crouch_02[], pl22_turn_00[], pl22_turn_01[], pl22_guard_00[], pl22_guard_01[],
	pl22_guard_02[], pl22_guard_03[], pl22_guard_04[], pl22_guard_05[], pl22_guard_06[],
	pl22_guard_07[], pl22_air_guard_00[], pl22_air_guard_01[], pl22_air_guard_02[],
	pl22_stand_p_00[], pl22_stand_p_01[], pl22_stand_p_02[], pl22_stand_k_00[], pl22_stand_k_01[],
	pl22_stand_k_04[], pl22_stand_k_02[], pl22_crouch_p_00[], pl22_crouch_p_01[], pl22_crouch_p_02[],
	pl22_crouch_k_00[], pl22_crouch_k_01[], pl22_crouch_k_02[], pl22_jump_p_00[], pl22_jump_p_01[],
	pl22_jump_p_02[], pl22_jump_k_00[], pl22_jump_k_01[], pl22_jump_k_02[], pl22_damage_00[],
	pl22_damage_01[], pl22_damage_02[], pl22_damage_03[], pl22_damage_04[], pl22_damage_05[],
	pl22_damage_06[], pl22_damage_07[], pl22_damage_08[], pl22_damage_09[], pl22_damage_09_loop[],
	pl22_damage_10[], pl22_damage_11[], pl22_damage_12[], pl22_damage_13[], pl22_damage_16[],
	pl22_damage_17[], pl22_damage_18[], pl22_damage_19[], pl22_damage_20[], pl22_damage_21[],
	pl22_damage_22[], pl22_damage_23[], pl22_damage_24[], pl22_damage_25[], pl22_damage_26[],
	pl22_damage_27[], pl22_damage_28[], pl22_damage_29[], pl22_damage_30[], pl22_damage_31[],
	pl22_damage_32[], pl22_damage_33[], pl22_dizzy_00[], pl22_catch_00[], pl22_catch_01[],
	pl22_catch_01_loop[], pl22_catch_02[], pl22_catch_03[], pl22_catch_04[], pl22_catch_05[],
	pl22_get_up_00[], pl22_get_up_01[], pl22_get_up_02[], pl22_get_up_03[], pl22_appear_00[],
	pl22_appear_01[], pl22_appear_01_loop[], pl22_extra_00[], pl22_extra_01[], pl22_extra_02[],
	pl22_extra_03[], pl22_extra_04[], pl22_appear_02[], pl22_appear_03[], pl22_appear_04[],
	pl22_appear_04_loop[], pl22_win_00[], pl22_win_00_loop[], pl22_win_01[], pl22_win_01_loop[],
	pl22_win_02[], pl22_win_02_loop[], pl22_win_03[], pl22_win_03_loop[], pl22_win_04[],
	pl22_win_04_loop[], pl22_win_08[], pl22_win_09[], pl22_win_09_loop[], pl22_win_10[],
	pl22_win_11[], pl22_win_11_loop[], pl22_win_12[], pl22_win_12_loop[], pl22_win_13[],
	pl22_win_13_loop[], pl22_win_05[], pl22_win_06[], pl22_win_07[], pl22_win_07_loop[],
	pl22_special_00[], pl22_special_00_loop[], pl22_special_01[], pl22_special_01_loop[],
	pl22_special_02[], pl22_special_02_loop[], pl22_super_02[], pl22_super_02_loop[],
	pl22_special_03[], pl22_special_04[], pl22_special_04_loop[], pl22_special_05[],
	pl22_special_41[], pl22_special_06[], pl22_special_07[], pl22_special_08[],
	pl22_special_08_loop[], pl22_special_09[], pl22_special_09_loop[], pl22_special_10[],
	pl22_special_11[], pl22_effect_25[], pl22_effect_25_loop[], pl22_effect_26[], pl22_special_29[],
	pl22_special_29_loop[], pl22_special_30[], pl22_special_30_loop[], pl22_special_31[],
	pl22_special_31_loop[], pl22_special_32[], pl22_special_32_loop[], pl22_special_33[],
	pl22_special_33_loop[], pl22_special_12[], pl22_special_14[], pl22_special_13[],
	pl22_special_13_loop[], pl22_special_15[], pl22_special_17[], pl22_special_17_loop[],
	pl22_special_45[], pl22_special_45_loop[], pl22_special_19[], pl22_special_20[],
	pl22_special_47[], pl22_special_16[], pl22_special_16_loop[], pl22_super_01[],
	pl22_super_01_loop[], pl22_super_00[], pl22_super_06[], pl22_super_07[], pl22_super_07_loop[],
	pl22_super_08[], pl22_special_21[], pl22_special_21_loop[], pl22_special_22[],
	pl22_special_22_loop[], pl22_special_23[], pl22_super_09[], pl22_super_09_loop[],
	pl22_super_10[], pl22_super_12[], pl22_super_13[], pl22_super_13_loop[], pl22_super_14[],
	pl22_super_11[], pl22_super_15[], pl22_super_15_loop[], pl22_super_16[], pl22_special_25[],
	pl22_special_25_loop[], pl22_special_26[], pl22_special_26_loop[], pl22_special_27[],
	pl22_unused_00[], pl22_unused_01[], pl22_special_34[], pl22_effect_06[], pl22_effect_06_loop[],
	pl22_effect_10[], pl22_effect_27[], pl22_effect_27_loop[], pl22_effect_11[], pl22_effect_07[],
	pl22_effect_08[], pl22_effect_08_loop[], pl22_effect_09[], pl22_effect_13[], pl22_effect_14[],
	pl22_effect_15[], pl22_effect_16[], pl22_effect_17[], pl22_effect_18[], pl22_effect_19[],
	pl22_effect_20[], pl22_effect_21[], pl22_effect_22[], pl22_effect_23[], pl22_effect_24[],
	pl22_change_00[], pl22_change_01[], pl22_change_01_loop[], pl22_change_02[],
	pl22_change_02_loop[], pl22_special_42[], pl22_change_03[], pl22_change_03_loop[],
	pl22_change_04[], pl22_change_04_loop[], pl22_caught_00[], pl22_caught_01[], pl22_caught_02[],
	pl22_caught_03[], pl22_caught_04[], pl22_caught_05[], pl22_caught_06[], pl22_caught_07[],
	pl22_caught_08[], pl22_caught_09[], pl22_caught_10[], pl22_caught_11[], pl22_caught_12[],
	pl22_caught_13[], pl22_caught_14[], pl22_caught_15[], pl22_caught_16[], pl22_caught_17[],
	pl22_caught_18[], pl22_caught_19[], pl22_caught_20[], pl22_caught_21[], pl22_effect_00[],
	pl22_effect_00_loop[], pl22_effect_02[], pl22_effect_02_loop[], pl22_effect_03[],
	pl22_effect_03_loop[], pl22_effect_04[], pl22_effect_05[], pl22_effect_05_loop[],
	pl22_grp25_00[], pl22_grp25_01[], pl22_grp25_02[], pl22_grp25_03[], pl22_grp25_04[],
	pl22_grp25_05[], pl22_grp25_06[], pl22_grp25_07[], pl22_grp25_08[], pl22_grp25_09[],
	pl22_grp25_10[], pl22_grp25_11[], pl22_grp25_12[], pl22_grp25_13[], pl22_grp25_14[],
	pl22_grp25_15[], pl22_grp25_16[], pl22_grp25_17[], pl22_grp25_18[], pl22_grp25_19[],
	pl22_grp25_20[], pl22_grp25_21[], pl22_grp25_22[], pl22_grp25_23[], pl22_grp27_00[],
	pl22_grp27_00_loop[], pl22_grp27_02[], pl22_effect_28[], pl22_effect_29[], pl22_effect_29_loop[],
	pl22_effect_30[], pl22_effect_31[], pl22_special_43[], pl22_special_43_loop[], pl22_special_44[],
	pl22_super_18[], pl22_super_18_loop[], pl22_catch_06[], pl22_catch_07[], pl22_catch_07_loop[],
	pl22_super_19[], pl22_super_19_loop[], pl22_grp27_01[], pl22_grp27_01_loop[];
extern const Sint32 *const pl22_stand_tbl[], *const pl22_jump_tbl[], *const pl22_dash_tbl[],
	*const pl22_crouch_tbl[], *const pl22_turn_tbl[], *const pl22_guard_tbl[],
	*const pl22_air_guard_tbl[], *const pl22_stand_p_tbl[], *const pl22_stand_k_tbl[],
	*const pl22_crouch_p_tbl[], *const pl22_crouch_k_tbl[], *const pl22_jump_p_tbl[],
	*const pl22_jump_k_tbl[], *const pl22_damage_tbl[], *const pl22_dizzy_tbl[],
	*const pl22_catch_tbl[], *const pl22_caught_tbl[], *const pl22_get_up_tbl[],
	*const pl22_appear_tbl[], *const pl22_win_tbl[], *const pl22_extra_tbl[],
	*const pl22_special_tbl[], *const pl22_super_tbl[], *const pl22_effect_tbl[],
	*const pl22_change_tbl[], *const pl22_grp25_tbl[], *const pl22_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_22[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
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
const Sint32 *const *const char_tbl_22[] = {
	pl22_stand_tbl, pl22_jump_tbl, pl22_dash_tbl, pl22_crouch_tbl, pl22_turn_tbl, pl22_guard_tbl,
	pl22_air_guard_tbl, pl22_stand_p_tbl, pl22_stand_k_tbl, pl22_crouch_p_tbl, pl22_crouch_k_tbl, pl22_jump_p_tbl,
	pl22_jump_k_tbl, pl22_damage_tbl, pl22_dizzy_tbl, pl22_catch_tbl, pl22_caught_tbl, pl22_get_up_tbl,
	pl22_appear_tbl, pl22_win_tbl, pl22_extra_tbl, pl22_special_tbl, pl22_super_tbl, pl22_effect_tbl,
	pl22_change_tbl, pl22_grp25_tbl, pl22_grp25_tbl, pl22_grp27_tbl, 0,
};

const Sint32 *const pl22_stand_tbl[] = {
	pl22_stand_00, pl22_stand_00, pl22_stand_02, pl22_stand_03, pl22_stand_00, pl22_stand_00,
	pl22_stand_02, pl22_stand_03, 0,
};

const Sint32 *const pl22_jump_tbl[] = {
	pl22_jump_00, pl22_jump_01, pl22_jump_02, pl22_jump_03, pl22_jump_03, pl22_jump_03,
	pl22_jump_06, pl22_jump_07, pl22_jump_08, pl22_jump_09, pl22_jump_09, pl22_jump_09,
	0,
};

const Sint32 *const pl22_dash_tbl[] = {
	pl22_dash_00, pl22_dash_01, pl22_dash_02, pl22_dash_01, pl22_dash_04, pl22_dash_04,
	pl22_dash_06, pl22_dash_06, 0,
};

const Sint32 *const pl22_crouch_tbl[] = {
	pl22_crouch_00, pl22_crouch_01, pl22_crouch_02, 0,
};

const Sint32 *const pl22_turn_tbl[] = {
	pl22_turn_00, pl22_turn_01, 0,
};

const Sint32 *const pl22_guard_tbl[] = {
	pl22_guard_00, pl22_guard_01, pl22_guard_02, pl22_guard_03, pl22_guard_04, pl22_guard_05,
	pl22_guard_06, pl22_guard_07, 0,
};

const Sint32 *const pl22_air_guard_tbl[] = {
	pl22_air_guard_00, pl22_air_guard_01, pl22_air_guard_02, 0,
};

const Sint32 *const pl22_stand_p_tbl[] = {
	pl22_stand_p_00, pl22_stand_p_01, pl22_stand_p_02, 0,
};

const Sint32 *const pl22_stand_k_tbl[] = {
	pl22_stand_k_00, pl22_stand_k_01, pl22_stand_k_02, pl22_stand_k_00, pl22_stand_k_04, pl22_stand_k_02,
	0,
};

const Sint32 *const pl22_crouch_p_tbl[] = {
	pl22_crouch_p_00, pl22_crouch_p_01, pl22_crouch_p_02, 0,
};

const Sint32 *const pl22_crouch_k_tbl[] = {
	pl22_crouch_k_00, pl22_crouch_k_01, pl22_crouch_k_02, 0,
};

const Sint32 *const pl22_jump_p_tbl[] = {
	pl22_jump_p_00, pl22_jump_p_01, pl22_jump_p_02, pl22_jump_p_00, pl22_jump_p_01, pl22_jump_p_02,
	0,
};

const Sint32 *const pl22_jump_k_tbl[] = {
	pl22_jump_k_00, pl22_jump_k_01, pl22_jump_k_02, pl22_jump_k_00, pl22_jump_k_01, pl22_jump_k_02,
	0,
};

const Sint32 *const pl22_damage_tbl[] = {
	pl22_damage_00, pl22_damage_01, pl22_damage_02, pl22_damage_03, pl22_damage_04, pl22_damage_05,
	pl22_damage_06, pl22_damage_07, pl22_damage_08, pl22_damage_09, pl22_damage_10, pl22_damage_11,
	pl22_damage_12, pl22_damage_13, pl22_damage_13, pl22_damage_13, pl22_damage_16, pl22_damage_17,
	pl22_damage_18, pl22_damage_19, pl22_damage_20, pl22_damage_21, pl22_damage_22, pl22_damage_23,
	pl22_damage_24, pl22_damage_25, pl22_damage_26, pl22_damage_27, pl22_damage_28, pl22_damage_29,
	pl22_damage_30, pl22_damage_31, pl22_damage_32, pl22_damage_33, pl22_damage_13, 0,
};

const Sint32 *const pl22_dizzy_tbl[] = {
	pl22_dizzy_00, 0,
};

const Sint32 *const pl22_catch_tbl[] = {
	pl22_catch_00, pl22_catch_01, pl22_catch_02, pl22_catch_03, pl22_catch_04, pl22_catch_05,
	pl22_catch_06, pl22_catch_07, 0,
};

const Sint32 *const pl22_caught_tbl[] = {
	pl22_caught_00, pl22_caught_01, pl22_caught_02, pl22_caught_03, pl22_caught_04, pl22_caught_05,
	pl22_caught_06, pl22_caught_07, pl22_caught_08, pl22_caught_09, pl22_caught_10, pl22_caught_11,
	pl22_caught_12, pl22_caught_13, pl22_caught_14, pl22_caught_15, pl22_caught_16, pl22_caught_17,
	pl22_caught_18, pl22_caught_19, pl22_caught_20, pl22_caught_21, 0,
};

const Sint32 *const pl22_get_up_tbl[] = {
	pl22_get_up_00, pl22_get_up_01, pl22_get_up_02, pl22_get_up_03, 0,
};

const Sint32 *const pl22_appear_tbl[] = {
	pl22_appear_00, pl22_appear_01, pl22_appear_02, pl22_appear_03, pl22_appear_04, 0,
};

const Sint32 *const pl22_win_tbl[] = {
	pl22_win_00, pl22_win_01, pl22_win_02, pl22_win_03, pl22_win_04, pl22_win_05,
	pl22_win_06, pl22_win_07, pl22_win_08, pl22_win_09, pl22_win_10, pl22_win_11,
	pl22_win_12, pl22_win_13, 0,
};

const Sint32 *const pl22_extra_tbl[] = {
	pl22_extra_00, pl22_extra_01, pl22_extra_02, pl22_extra_03, pl22_extra_04, 0,
};

const Sint32 *const pl22_special_tbl[] = {
	pl22_special_00, pl22_special_01, pl22_special_02, pl22_special_03, pl22_special_04, pl22_special_05,
	pl22_special_06, pl22_special_07, pl22_special_08, pl22_special_09, pl22_special_10, pl22_special_11,
	pl22_special_12, pl22_special_13, pl22_special_14, pl22_special_15, pl22_special_16, pl22_special_17,
	pl22_special_17, pl22_special_19, pl22_special_20, pl22_special_21, pl22_special_22, pl22_special_23,
	pl22_special_23, pl22_special_25, pl22_special_26, pl22_special_27, pl22_special_27, pl22_special_29,
	pl22_special_30, pl22_special_31, pl22_special_32, pl22_special_33, pl22_special_34, pl22_special_17,
	pl22_special_17, pl22_special_17, pl22_special_17, pl22_special_17, pl22_special_17, pl22_special_41,
	pl22_special_42, pl22_special_43, pl22_special_44, pl22_special_45, pl22_special_45, pl22_special_47,
	pl22_special_47, 0,
};

const Sint32 *const pl22_super_tbl[] = {
	pl22_super_00, pl22_super_01, pl22_super_02, pl22_special_03, pl22_special_04, pl22_special_05,
	pl22_super_06, pl22_super_07, pl22_super_08, pl22_super_09, pl22_super_10, pl22_super_11,
	pl22_super_12, pl22_super_13, pl22_super_14, pl22_super_15, pl22_super_16, pl22_special_16,
	pl22_super_18, pl22_super_19, 0,
};

const Sint32 *const pl22_effect_tbl[] = {
	pl22_effect_00, pl22_effect_00, pl22_effect_02, pl22_effect_03, pl22_effect_04, pl22_effect_05,
	pl22_effect_06, pl22_effect_07, pl22_effect_08, pl22_effect_09, pl22_effect_10, pl22_effect_11,
	pl22_effect_07, pl22_effect_13, pl22_effect_14, pl22_effect_15, pl22_effect_16, pl22_effect_17,
	pl22_effect_18, pl22_effect_19, pl22_effect_20, pl22_effect_21, pl22_effect_22, pl22_effect_23,
	pl22_effect_24, pl22_effect_25, pl22_effect_26, pl22_effect_27, pl22_effect_28, pl22_effect_29,
	pl22_effect_30, pl22_effect_31, 0,
};

const Sint32 *const pl22_change_tbl[] = {
	pl22_change_00, pl22_change_01, pl22_change_02, pl22_change_03, pl22_change_04, 0,
};

const Sint32 *const pl22_grp25_tbl[] = {
	pl22_grp25_00, pl22_grp25_01, pl22_grp25_02, pl22_grp25_03, pl22_grp25_04, pl22_grp25_05,
	pl22_grp25_06, pl22_grp25_07, pl22_grp25_08, pl22_grp25_09, pl22_grp25_10, pl22_grp25_11,
	pl22_grp25_12, pl22_grp25_13, pl22_grp25_14, pl22_grp25_15, pl22_grp25_16, pl22_grp25_17,
	pl22_grp25_18, pl22_grp25_19, pl22_grp25_20, pl22_grp25_21, pl22_grp25_22, pl22_grp25_23,
	0,
};

const Sint32 *const pl22_grp27_tbl[] = {
	pl22_grp27_00, pl22_grp27_01, pl22_grp27_02, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl22_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl22_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl22_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl22_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: haru_up (Haru Ichiban (hyper)), act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl22_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl22_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   103, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   104, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   105, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_jump_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   106, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   107, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl22_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_jump_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl22_jump_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl22_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_jump_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl22_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   103, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   104, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   105, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_jump_07_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl22_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_jump_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_jump_08_loop ),
};

/* Char_Dash 4: fly_start_init, fly_start2_init
   Char_Dash 5: noki_init */
const Sint32 pl22_dash_04[] = {
	CG_FRAME(    0,    1,    3,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl22_dash_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_dash_04_loop ),
};

/* Char_Dash 6: dash_move
   Char_Dash 7: noki_move */
const Sint32 pl22_dash_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_dash_06 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl22_dash_00[] = {
	CG_FRAME(    0,    1,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_dash_00 ),
};

/* Char_Dash 1: act_back_dash
   Char_Dash 3 */
const Sint32 pl22_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    1,    1,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    5,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    9,           0,    57, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    1,    4,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl22_dash_01 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl22_dash_02[] = {
	CG_FRAME(    0,    0,    3,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    77, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    78, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    77, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,    78, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    4,           0,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    4,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_dash_02 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl22_crouch_00[] = {
	CG_FRAME(    0,    0,    6,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl22_crouch_01[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    57, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl22_crouch_02[] = {
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl22_turn_00[] = {
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl22_turn_01[] = {
	CG_FRAME(    0,    0,    6,           0,   122, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   123, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   124, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl22_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl22_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl22_guard_02[] = {
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl22_guard_03[] = {
	CG_FRAME( 0x80,    0,    2,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    6,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    6, END_OF_DATA,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_guard_01 ),
};

/* Char_Guard 4 */
const Sint32 pl22_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl22_guard_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl22_guard_06[] = {
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   132, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl22_guard_07[] = {
	CG_FRAME( 0x80,    0,    2,           0,   133, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    2,           0,   134, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    2,           0,   135, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    6,           0,   136, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    2,           0,   137, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    6, END_OF_DATA,   138, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_guard_05 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl22_air_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl22_air_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   140, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl22_air_guard_02[] = {
	CG_FRAME( 0x80,    0,    2,           0,   140, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    2,           0,   141, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    2,           0,   142, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    6,           0,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    2,           0,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    6, END_OF_DATA,   145, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl22_stand_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   294, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   297, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl22_stand_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   294, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   298, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    1,           0,   299, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    8,           0,   300, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl22_stand_p_02[] = {
	CG_FRAME(    0,    0,    7,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   304, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   307, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    8,           0,   308, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_stand_p_02 ),
};

/* Char_Stand_K 0 (light)
   Char_Stand_K 3 */
const Sint32 pl22_stand_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   311, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   312, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   312, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   314, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl22_stand_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   315, 0x80,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   316, 0x80,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    1,           0,   317, 0x80,   0x01,              ATK,  0x00,       0x04,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   318, 0x80,   0x01,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   318, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   319, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x05,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   321, 0x80,   0x0c,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_stand_k_01 ),
};

/* Char_Stand_K 4: nml_atck */
const Sint32 pl22_stand_k_04[] = {
	CG_FRAME(    0,    0,    7,           0,   420, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   421, 0x80,   0x0a, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   422, 0x80,   0x06, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    7,           0,   423, 0x80,   0x06, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   424, 0x80,   0x01, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    3,           0,   425, 0x80,   0x01, ATK | CG_ATCK_NG,  0x13,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,   426, 0x80,   0x01, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   428, 0x80,   0x0e, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   121, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_stand_k_04 ),
};

/* Char_Stand_K 2 (heavy)
   Char_Stand_K 5 */
const Sint32 pl22_stand_k_02[] = {
	CG_FRAME(    0,    0,    7,           0,   322, 0x80,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    1,           0,   324, 0x80,   0x01,              ATK,  0x00,       0x05,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    8,           0,   325, 0x80,   0x01,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   326, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   327, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x0a,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl22_crouch_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   328, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   331, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl22_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl22_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   332, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   333, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   334, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    6,           0,   336, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   338, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl22_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl22_crouch_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   339, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x40,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    1,           0,   340, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    5,           0,   341, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   342, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   343, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    5,           0,   344, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    5,           0,   340, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   338, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl22_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl22_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   346, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    3,           0,   346, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    3,           0,   345, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   124, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl22_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl22_crouch_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   347, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   348, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   349, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   347, 0x80,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   352, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl22_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl22_crouch_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   353, 0x70,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   354, 0x70,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   355, 0x70,   0x87,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   356, 0x80,   0x87,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   357, 0x90,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   359, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   352, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl22_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl22_jump_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   361, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    2,           0,   363, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    2,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl22_jump_p_01[] = {
	CG_FRAME(    0,    0,    6,           0,   364, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   365, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   366, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   366, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    2,           0,   364, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   364, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl22_jump_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   369, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   373, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    6,           0,   372, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl22_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   376, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   377, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   378, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   377, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl22_jump_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   379, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   380, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    1,           0,   379, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl22_jump_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   384, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    6,           0,   387, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    6,           0,   388, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    6,           0,   389, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl22_damage_00[] = {
	CG_FRAME(    0,    0,    7,           0,   539, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   538, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl22_damage_01[] = {
	CG_FRAME(    0,    0,    4,           0,   540, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   539, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   538, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl22_damage_02[] = {
	CG_FRAME(    0,    0,    6,           0,   541, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   540, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   539, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   538, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl22_damage_03[] = {
	CG_FRAME(    0,    0,    7,           0,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   542, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl22_damage_04[] = {
	CG_FRAME(    0,    0,    6,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   542, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl22_damage_05[] = {
	CG_FRAME(    0,    0,    6,           0,   545, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   542, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl22_damage_06[] = {
	CG_FRAME(    0,    0,    7,           0,   546, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   546, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl22_damage_07[] = {
	CG_FRAME(    0,    0,    5,           0,   547, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   546, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   547, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   546, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl22_damage_08[] = {
	CG_FRAME(    0,    0,    5,           0,   548, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   547, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   548, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   547, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   546, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl22_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl22_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   564, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   565, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   566, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   567, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   568, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl22_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   569, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl22_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl22_damage_10[] = {
	CG_FRAME(    0,    0,    6,           0,   550, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   550, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl22_damage_11[] = {
	CG_FRAME(    0,    0,    4,           0,   551, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   550, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   549, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl22_damage_12[] = {
	CG_FRAME(    0,    0,    3,           0,   552, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   551, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   550, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   549, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl22_damage_13[] = {
	CG_FRAME(    0,    0,    2,           0,   557, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   558, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   559, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   560, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   568, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   569, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl22_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl22_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   570, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   571, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl22_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   572, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl22_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   573, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl22_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   574, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl22_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   575, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl22_damage_21[] = {
	CG_FRAME(    0,    0,    4,           0,   570, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   571, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

/* Char_Damage 22 */
const Sint32 pl22_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   572, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

/* Char_Damage 23 */
const Sint32 pl22_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   573, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

/* Char_Damage 24 */
const Sint32 pl22_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   574, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

/* Char_Damage 25 */
const Sint32 pl22_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   575, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl22_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   576, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   565, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   566, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   567, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   568, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   569, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl22_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl22_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl22_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   577, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   578, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   579, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   580, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   581, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   582, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl22_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   583, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   584, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   585, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   586, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   587, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   588, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl22_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl22_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   569, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl22_damage_31 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl22_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl22_damage_33[] = {
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl22_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   589, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   590, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   591, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_dizzy_00 ),
};

/* Char_Catch 0 */
const Sint32 pl22_catch_00[] = {
	CG_FRAME(    0,    0,    4,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,                0,  0x00, 0x0b | CSE,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    1,    0,    4,           0,   395, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    1,   -1,    4,           0,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,                0,  0xff, 0x0b | CSE,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,    4,           0,   395, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    1,   -1,    4,           0,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,    4,           0,   395, 0x80,   0x00,                0,  0x00, 0x0b | CSE,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    1,   -1,    4,           0,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,    4,           0,   395, 0x80,   0x00,                0,  0x00, 0x0b | CSE,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    1,   -1,    4,           0,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,                0,  0xff, 0x0b | CSE,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,    4,           0,   395, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    1,   -1,    4,           0,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,                0,  0xff, 0x0b | CSE,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,    4,           0,   395, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    1,   -1,    4,           0,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   0 ),
	CG_FRAME(    1,    0,    4,           0,   396, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0206,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0009,   0 ),
};

/* Char_Catch 1: nml_p_catch */
const Sint32 pl22_catch_01[] = {
	CG_FRAME(    0,    0,    4,           0,   398, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    4,           0,   399, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    4,           0,   400, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,                0,  0x01,       0x05,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    1,    1,           0,   401, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0, 0x000d,   0 ),
};

const Sint32 pl22_catch_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   402, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_catch_01_loop ),
};

/* Char_Catch 2: catch_fall */
const Sint32 pl22_catch_02[] = {
	CG_FRAME(    0,   10,    6,           0,   403, 0x80,   0x00,                0,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,    6,           0,   404, 0x80,   0x00,                0,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,    6, END_OF_DATA,   405, 0x80,   0x00,                0,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl22_catch_02 ),
};

/* Char_Catch 3 */
const Sint32 pl22_catch_03[] = {
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,    6,           0,   407, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,    6,           0,   408, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   16,           0,   409, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x80,   0x20,                0,  0x01,       0x0a,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    1,    1,           0,   410, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   412, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   413, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   414, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   415, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_catch_03 ),
};

/* Char_Catch 4: catch_fall */
const Sint32 pl22_catch_04[] = {
	CG_FRAME(    0,    0,    6,           0,   416, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   417, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   418, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   419, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_catch_04 ),
};

/* Char_Catch 5 */
const Sint32 pl22_catch_05[] = {
	CG_FRAME(    0,    0,    2,           0,   524, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,    4,           0,   525, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,    4,           0,   526, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   14,           0,   527, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,    4,           0,   528, 0x80,   0x20,                0,  0x01,       0x0a,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    1,    1,           0,   528, 0x80,   0x20,                0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   529, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   528, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   529, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   530, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   531, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   532, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_catch_05 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl22_get_up_00[] = {
	CG_FRAME(    0,    0,    6,           0,   194, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   195, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   196, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl22_get_up_01[] = {
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl22_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl22_get_up_03[] = {
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_get_up_03 ),
};

/* Char_Appear 0: appear_wait, appear_late_init */
const Sint32 pl22_appear_00[] = {
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   187, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_appear_00 ),
};

/* Char_Appear 1: appear_run */
const Sint32 pl22_appear_01[] = {
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,                0,  0x00, 0x34 | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_appear_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   190, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_appear_01_loop ),
};

/* Char_Extra 0: sp_chouhatsu (Taunt) */
const Sint32 pl22_extra_00[] = {
	CG_FRAME(    0,    0,    4,           0,   491, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   492, 0x80,   0x00,              ATK,  0x00,       0x1c,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   493, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   495, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   496, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   497, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   497, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   491, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_extra_00 ),
};

/* Char_Extra 1: t_kick_init */
const Sint32 pl22_extra_01[] = {
	CG_FRAME(    0,    0,    2,           0,   380, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   381, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl22_extra_01 ),
};

/* Char_Extra 2: t_kick_move, fly_start5 */
const Sint32 pl22_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   270, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   271, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,                0,  0x00,       0x19,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   279, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_extra_02 ),
};

/* Char_Extra 3: pl22_upper, fly_start5 */
const Sint32 pl22_extra_03[] = {
	CG_FRAME(    0,    0,    5,           0,   339, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   340, 0x80,   0x40,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   341, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   342, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   343, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    5,           0,   344, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    5,           0,   340, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   338, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl22_extra_03 ),
};

/* Char_Extra 4: pl22_upper */
const Sint32 pl22_extra_04[] = {
	CG_FRAME(    0,    0,    6,           0,   315, 0x80,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   316, 0x80,   0x09,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    1,           0,   317, 0x80,   0x01,              ATK,  0x00,       0x04,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   318, 0x80,   0x01,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   318, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   319, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x05,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   321, 0x80,   0x0c,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_extra_04 ),
};

/* Char_Appear 2: appear_stop */
const Sint32 pl22_appear_02[] = {
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_appear_02 ),
};

/* Char_Appear 3: appear_init */
const Sint32 pl22_appear_03[] = {
	CG_FRAME(    0,    0,    6,           0,   753, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   754, 0x80,   0x00,                0,  0x00,       0x2a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   755, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   756, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   757, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_appear_03 ),
};

/* Char_Appear 4: pls22_00_t2 */
const Sint32 pl22_appear_04[] = {
	CG_FRAME(    0,    0,   30,           0,   726, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0x80,   0x00,                0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   729, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   730, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   731, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   732, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   24,           0,   733, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   24,           0,   734, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   735, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   736, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   737, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   738, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   739, 0x80,   0x00,                0,  0x00,       0x2a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   740, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   741, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   742, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   743, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   744, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   745, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   32,           0,   746, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   752, 0x80,   0x00,                0,  0x00,       0x0d,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_appear_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   747, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   748, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   749, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   750, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   751, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   752, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_appear_04_loop ),
};

/* Char_Win 0 */
const Sint32 pl22_win_00[] = {
	CG_FRAME(    0,    0,    6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   148, 0x80,   0x40,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   149, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_win_00_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   150, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   151, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   152, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl22_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   312, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   313, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   153, 0x80,   0x01,                0,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x08,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x80,   0x08,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   159, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_win_01_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl22_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   312, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   313, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   317, 0x80,   0x01,                0,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x08,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x08,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_win_02_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   279, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_win_02_loop ),
};

/* Char_Win 3: pl22_win */
const Sint32 pl22_win_03[] = {
	CG_FRAME(    0,    0,    6,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_win_03_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   231, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   232, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   234, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   239, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_win_03_loop ),
};

/* Char_Win 4 */
const Sint32 pl22_win_04[] = {
	CG_FRAME(    0,    0,    4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,       0x1b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_win_04_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_04_loop ),
};

/* Char_Win 8: pls22_00_t4 */
const Sint32 pl22_win_08[] = {
	CG_FRAME(    0,    0,    4,           0,   776, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   777, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   778, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_08 ),
};

/* Char_Win 9: pls22_00_t4 */
const Sint32 pl22_win_09[] = {
	CG_FRAME(    0,    0,    6,           0,   758, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   759, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_win_09_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   760, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   761, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   762, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   763, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   764, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   765, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   766, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   767, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   769, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_09_loop ),
};

/* Char_Win 10: pls22_00_t5 */
const Sint32 pl22_win_10[] = {
	CG_FRAME(    0,    0,    4,           0,   782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   786, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   787, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_10 ),
};

/* Char_Win 11: pls22_00_t5 */
const Sint32 pl22_win_11[] = {
	CG_FRAME(    0,    0,    4,           0,   788, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   789, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   790, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   791, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_win_11_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_11_loop ),
};

/* Char_Win 12: pl22_win */
const Sint32 pl22_win_12[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   797, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   798, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_win_12_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_12_loop ),
};

/* Char_Win 13: pl22_win */
const Sint32 pl22_win_13[] = {
	CG_FRAME(    0,    0,    4,           0,   800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   802, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   803, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   804, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   805, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   806, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   807, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   808, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   809, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_win_13_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   810, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_13_loop ),
};

/* Char_Win 5 */
const Sint32 pl22_win_05[] = {
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   181, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_05 ),
};

/* Char_Win 6: shl22_00_init_fall, shl22_00_init_bounce */
const Sint32 pl22_win_06[] = {
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl22_win_06 ),
};

/* Char_Win 7: shl22_00_land */
const Sint32 pl22_win_07[] = {
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_win_07_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   174, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_win_07_loop ),
};

/* Char_Special 0 */
const Sint32 pl22_special_00[] = {
	CG_FRAME(    0,    1,    3,           0,   430, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,    1,           0,   430, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  54 ),
};

const Sint32 pl22_special_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   447, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl22_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl22_special_01[] = {
	CG_FRAME(    0,    1,    7,           0,   430, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,    1,           0,   430, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  54 ),
};

const Sint32 pl22_special_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   447, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl22_special_01_loop ),
};

/* Char_Special 2: shouou_run (Shououken) */
const Sint32 pl22_special_02[] = {
	CG_FRAME(    0,    0,    4,           0,    76, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl22_special_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    78, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_special_02_loop ),
};

/* Char_Super 2: midare_run (Midare Zakura (hyper)) */
const Sint32 pl22_super_02[] = {
	CG_FRAME(    0,    0,    2,           0,    76, 0x80,   0x00,              ATE,  0x00,       0x0a,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,    76, 0x80,   0x00,              ATE,  0x00, 0x34 | CSE,    0,   0,     0,      0,   8 ),
};

const Sint32 pl22_super_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    78, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_super_02_loop ),
};

/* Char_Special 3: shouou_stop (Shououken)
   Char_Super 3: midare_stop (Midare Zakura (hyper)) */
const Sint32 pl22_special_03[] = {
	CG_FRAME(    0,    1,    4,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    81, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl22_special_03 ),
};

/* Char_Special 4: shouou_run (Shououken), jshouou_run (Shououken in the air)
   Char_Super 4: midare_run (Midare Zakura (hyper)) */
const Sint32 pl22_special_04[] = {
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_special_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_04_loop ),
};

/* Char_Special 5: shouou_guard (Shououken), jshouou_guard (Shououken in the air)
   Char_Super 5: midare_guard (Midare Zakura (hyper)) */
const Sint32 pl22_special_05[] = {
	CG_FRAME(    0,    0,    5,           0,   117, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   449, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   450, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_05 ),
};

/* Char_Special 41 */
const Sint32 pl22_special_41[] = {
	CG_FRAME(    0,    0,    3,           0,   197, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   198, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   199, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl22_special_41 ),
};

/* Char_Special 6: shouou_run (Shououken, 6 + atck_level) */
const Sint32 pl22_special_06[] = {
	CG_FRAME(    0,    1,    2,           0,   433, 0x80,   0x00,              ATK,  0x30,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    2,           0,   433, 0x80,   0x00,              ATK,  0x30,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    2,           0,   434, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   434, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   435, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,    2,           0,   435, 0x80,   0x00,              ATK,  0x33,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   436, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl22_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl22_special_07[] = {
	CG_FRAME(    0,    1,    2,           0,   433, 0x80,   0x00,              ATK,  0x38,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    2,           0,   433, 0x80,   0x00,              ATK,  0x38,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    2,           0,   434, 0x80,   0x00,              ATK,  0x39,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   434, 0x80,   0x00,              ATK,  0x39,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   435, 0x80,   0x00,              ATK,  0x3a,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,    2,           0,   435, 0x80,   0x00,              ATK,  0x3b,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   436, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl22_special_07 ),
};

/* Char_Special 8: shouou_up (Shououken, 8 + atck_level), jshouou_up (Shououken in the air, 8 + atck_level) */
const Sint32 pl22_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,   437, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   439, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_special_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   440, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl22_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   437, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   439, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_special_09_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   440, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_09_loop ),
};

/* Char_Special 10: shouou_fall (Shououken, 10 + atck_level), jshouou_fall (Shououken in the air, 10 + atck_level), haru_fall (Haru Ichiban (hyper)) */
const Sint32 pl22_special_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   441, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_10 ),
};

/* Char_Special 11 */
const Sint32 pl22_special_11[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   441, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_11 ),
};

/* Char_Effect 25: pls22_00_armor */
const Sint32 pl22_effect_25[] = {
	CG_FRAME(    0,    0,    2,           0, 32810, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32813, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_25_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32815, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_25_loop ),
};

/* Char_Effect 26: pls22_00_armor */
const Sint32 pl22_effect_26[] = {
	CG_FRAME(    0,    0,    4,           0,   879, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   880, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   881, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   882, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   883, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   884, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_26 ),
};

/* Char_Special 29: jshouou_init (Shououken in the air, 29 + atck_level) */
const Sint32 pl22_special_29[] = {
	CG_FRAME(    0,    1,    7,           0,   512, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   512, 0x80,   0x20,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_special_29_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   513, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   514, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   515, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl22_special_29_loop ),
};

/* Char_Special 30 */
const Sint32 pl22_special_30[] = {
	CG_FRAME(    0,    1,   19,           0,   512, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   512, 0x80,   0x20,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_special_30_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   513, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   514, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   515, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl22_special_30_loop ),
};

/* Char_Special 31: jshouou_run (Shououken in the air) */
const Sint32 pl22_special_31[] = {
	CG_FRAME(    0,    0,    8,           0,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_special_31_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_31_loop ),
};

/* Char_Special 32: jshouou_run (Shououken in the air, 32 + atck_level) */
const Sint32 pl22_special_32[] = {
	CG_FRAME(    0,    0,    2,           0,   516, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2,           0,   517, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,    2,           0,   518, 0x80,   0x20,              ATK,  0x5f,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   518, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   518, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   518, 0x80,   0x20,              ATK,  0x60,          0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl22_special_32_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   519, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_32_loop ),
};

/* Char_Special 33 */
const Sint32 pl22_special_33[] = {
	CG_FRAME(    0,    0,    2,           0,   516, 0x80,   0x20,              ATK,  0x5e,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2,           0,   517, 0x80,   0x20,              ATK,  0x5e,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,    2,           0,   518, 0x80,   0x20,              ATK,  0x5f,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   518, 0x80,   0x20,              ATK,  0x5f,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   518, 0x80,   0x20,              ATK,  0x5f,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   518, 0x80,   0x20,              ATK,  0x60,          0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl22_special_33_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   519, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_33_loop ),
};

/* Char_Special 12: shunpu_init (Shunpukyaku, 12 + atck_level) */
const Sint32 pl22_special_12[] = {
	CG_FRAME(    0,    1,    1,           0,   117, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   473, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   474, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    5,           0,   475, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    5,           0,   476, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    8,           0,   477, 0x80,   0x20,              ATK,  0x4b,          0,    0,   0,     0,      0,  59 ),
};

/* Char_Special 14: shunpu_jump (Shunpukyaku, 14 + atck_level) */
const Sint32 pl22_special_14[] = {
	CG_FRAME(    0,    0,    8,           0,   314, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_14 ),
};

/* Char_Special 13 */
const Sint32 pl22_special_13[] = {
	CG_FRAME(    0,    1,    1,           0,   117, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   473, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   474, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   475, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl22_special_13_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   476, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    3,           0,   477, 0x80,   0x20,              ATK,  0x4d,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   478, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   479, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_special_13_loop ),
};

/* Char_Special 15 */
const Sint32 pl22_special_15[] = {
	CG_FRAME(    0,    0,    8,           0,   314, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_15 ),
};

/* Char_Special 17
   Char_Special 18
   Char_Special 35
   Char_Special 36
   Char_Special 37
   Char_Special 38 */
const Sint32 pl22_special_17[] = {
	CG_FRAME(    0,    0,    1,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   460, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   464, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0,      0,  65 ),
};

const Sint32 pl22_special_17_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3,           0,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   467, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl22_special_17_loop ),
};

/* Char_Special 45
   Char_Special 46 */
const Sint32 pl22_special_45[] = {
	CG_FRAME(    0,    0,    1,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   453, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   456, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0,      0,  85 ),
};

const Sint32 pl22_special_45_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl22_special_45_loop ),
};

/* Char_Special 19 */
const Sint32 pl22_special_19[] = {
	CG_FRAME(    0,    0,    4,           0,   465, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3,           0,   468, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_19 ),
};

/* Char_Special 20 */
const Sint32 pl22_special_20[] = {
	CG_FRAME(    0,    0,   10,           0,   465, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3,           0,   468, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_20 ),
};

/* Char_Special 47
   Char_Special 48 */
const Sint32 pl22_special_47[] = {
	CG_FRAME(    0,    0,    4,           0,   459, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_47 ),
};

/* Char_Special 16
   Char_Super 17: midare_run (Midare Zakura (hyper)) */
const Sint32 pl22_special_16[] = {
	CG_FRAME(    0,    0,   16,           0,   442, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
};

const Sint32 pl22_special_16_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   442, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl22_special_16_loop ),
};

/* Char_Super 1: midare_init (Midare Zakura (hyper)) */
const Sint32 pl22_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   294, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   16,           0,   430, 0x80,   0x00,              ATK,  0x00,       0x18,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0, 0x81,    6,           0,   430, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl22_super_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   447, 0x80,   0x00,              ATK,  0x20,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl22_super_01_loop ),
};

/* Char_Super 0: midare_catch (Midare Zakura (hyper)) */
const Sint32 pl22_super_00[] = {
	CG_FRAME(    0,    0,    2,           0,   315, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   317, 0x80,   0x00,              ATK,  0x48,       0x03,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   318, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   298, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   299, 0x80,   0x00,              ATK,  0x49,       0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    6,           0,   300, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   295, 0x80,   0x00,              ATK,  0x48,       0x05,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    6,           0,   296, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   310, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   312, 0x80,   0x00,              ATK,  0x49,       0x04,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   315, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   317, 0x80,   0x00,              ATK,  0x48,       0x03,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   318, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   310, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   312, 0x80,   0x00,              ATK,  0x49,       0x05,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   475, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   477, 0x80,   0x00,              ATK,  0x48,       0x03,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   479, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   477, 0x80,   0x00,              ATK,  0x49,       0x05,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   322, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    1,           0,   324, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   325, 0x80,   0x00,              ATK,  0x49,       0x04,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   359, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_00 ),
};

/* Char_Super 6: midare_hit (Midare Zakura (hyper)) */
const Sint32 pl22_super_06[] = {
	CG_FRAME(    0,    1,    2,           0,   433, 0x80,   0x00,              ATK,  0x54,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    2,           0,   433, 0x80,   0x00,              ATK,  0x54,       0x14,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    2,           0,   434, 0x80,   0x00,              ATK,  0x57, 0x14 | CSE,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   434, 0x80,   0x00,              ATK,  0x57,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    2,           0,   435, 0x80,   0x00,              ATK,  0x58,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,    2,           0,   435, 0x80,   0x00,              ATK,  0x59,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   436, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl22_super_06 ),
};

/* Char_Super 7: midare_up (Midare Zakura (hyper)) */
const Sint32 pl22_super_07[] = {
	CG_FRAME(    0,    0,    4,           0,   437, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   439, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_super_07_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   440, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_07_loop ),
};

/* Char_Super 8: midare_fall (Midare Zakura (hyper)) */
const Sint32 pl22_super_08[] = {
	CG_FRAME(    0,    0,    2,           0,    98, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    50, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   253, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   254, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0x00,              ATE,  0x00,       0x1b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   256, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   259, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   260, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_08 ),
};

/* Char_Special 21: jhadou_init (Hadouken in the air, 21 + atck_level) */
const Sint32 pl22_special_21[] = {
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   361, 0x80,   0x20,              ATK,  0x00,       0x16,    0,   0,     0,      0,   7 ),
};

const Sint32 pl22_special_21_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   363, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_special_21_loop ),
};

/* Char_Special 22 */
const Sint32 pl22_special_22[] = {
	CG_FRAME(    0,    0,    8,           0,   360, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   361, 0x80,   0x20,              ATK,  0x00,       0x16,    0,   0,     0,      0,   7 ),
};

const Sint32 pl22_special_22_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   363, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_special_22_loop ),
};

/* Char_Special 23: jhadou_shot (Hadouken in the air, 23 + atck_level)
   Char_Special 24 */
const Sint32 pl22_special_23[] = {
	CG_FRAME(    0,    0,    2,           0,   360, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_special_23 ),
};

/* Char_Super 9: shinkuu_init (Shinkuu Hadouken (hyper)) */
const Sint32 pl22_super_09[] = {
	CG_FRAME(    0,    0,    3,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    1,           0,   460, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    3,           0,   460, 0x80,   0x00,              ATK,  0x00,       0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   464, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_super_09_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   467, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_09_loop ),
};

/* Char_Super 10: shinkuu_end_init (Shinkuu Hadouken (hyper)) */
const Sint32 pl22_super_10[] = {
	CG_FRAME(    0,    0,   12,           0,   468, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_10 ),
};

/* Char_Super 12: dshinkuu_init (Shinkuu Hadouken, dark form (hyper)) */
const Sint32 pl22_super_12[] = {
	CG_FRAME(    0,    0,    3,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   452, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,    1,           0,   453, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME( 0x80,    1,    1,           0,   454, 0x80,   0x00,              ATK,  0x00,       0x17,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   452, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   453, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
};

/* Char_Super 13 */
const Sint32 pl22_super_13[] = {
	CG_FRAME(    0,    0,    4,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,   1 ),
	CG_FRAME(    0, 0x80,    2,           0,   456, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0, 0x0500,   1 ),
};

const Sint32 pl22_super_13_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_NEXT( pl22_super_13_loop ),
};

/* Char_Super 14: dshinkuu_keep (Shinkuu Hadouken, dark form (hyper)) */
const Sint32 pl22_super_14[] = {
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   456, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   456, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   456, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   456, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   459, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_14 ),
};

/* Char_Super 11 */
const Sint32 pl22_super_11[] = {
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   481, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    2,           0,   485, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   481, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x00,              ATK,  0x53,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    3,           0,   327, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    3,           0,   314, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   302, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_super_11 ),
};

/* Char_Super 15: haru_init (Haru Ichiban (hyper)) */
const Sint32 pl22_super_15[] = {
	CG_FRAME(    0,    0,    4,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x81,    4,           0,   472, 0x80,   0x00,              ATK,  0x00,       0x19,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,    2,           0,   473, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,    2,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   475, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl22_super_15_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   476, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   477, 0x80,   0x20,              ATK,  0x61,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   478, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   479, 0x80,   0x20,              ATK,  0x61,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl22_super_15_loop ),
};

/* Char_Super 16: haru_up (Haru Ichiban (hyper)) */
const Sint32 pl22_super_16[] = {
	CG_FRAME(    0,    0,    2,           0,   364, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   365, 0x80,   0x20,              ATK,  0x63,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2,           0,   366, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2,           0,   360, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   361, 0x80,   0x20,              ATK,  0x63,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    4,           0,   379, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   380, 0x80,   0x20,              ATK,  0x66,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    6,           0,   369, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x20,              ATK,  0x64,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3,           0,   373, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    6,           0,   372, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl22_super_16 ),
};

/* Char_Special 25: jshunpu_init (Shunpukyaku in the air, 25 + atck_level) */
const Sint32 pl22_special_25[] = {
	CG_FRAME(    0,    0,    2,           0,   499, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x20,              ATK,  0x00,       0x15,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   501, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x20,              ATK,  0x5a,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   503, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_special_25_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   504, 0x80,   0x20,              ATK,  0x5a, 0x16 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    3,           0,   501, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x20,              ATK,  0x5a,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    1,    1,           0,   503, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   503, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_25_loop ),
};

/* Char_Special 26 */
const Sint32 pl22_special_26[] = {
	CG_FRAME(    0,    0,    3,           0,   499, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   500, 0x80,   0x20,              ATK,  0x00,       0x15,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   501, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x20,              ATK,  0x5a,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   503, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_special_26_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   504, 0x80,   0x20,              ATK,  0x5a, 0x16 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    3,           0,   501, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x20,              ATK,  0x5a,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    1,    1,           0,   503, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   503, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_26_loop ),
};

/* Char_Special 27
   Char_Special 28 */
const Sint32 pl22_special_27[] = {
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_27 ),
};

const Sint32 pl22_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   505, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   506, 0x80,   0x40,              ATK,  0x5d,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    4,           0,   507, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   508, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   509, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   510, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   511, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_27 ),
};

const Sint32 pl22_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   121, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_special_27 ),
};

/* Char_Special 34: jshunpu_air (Shunpukyaku in the air) */
const Sint32 pl22_special_34[] = {
	CG_FRAME(    0,    0,    3,           0,   440, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   448, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_special_34 ),
};

/* Char_Effect 6 */
const Sint32 pl22_effect_06[] = {
	CG_FRAME(    0,    0,    1,           0,   629, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   630, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   631, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   632, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   615, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   616, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_06_loop[] = {
	CG_FRAME(    0,    1,    2,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl22_effect_06_loop ),
};

/* Char_Effect 10: shl22_00_init_follow */
const Sint32 pl22_effect_10[] = {
	CG_FRAME(    0,    0,    2,           0,   629, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   630, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   631, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_10 ),
};

/* Char_Effect 27: shl22_00_move_follow */
const Sint32 pl22_effect_27[] = {
	CG_FRAME(    0,    0,    5,           0,   629, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   632, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   633, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_27_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   634, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    6, END_OF_DATA,   633, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_27_loop ),
};

/* Char_Effect 11: shl22_00_init_spark */
const Sint32 pl22_effect_11[] = {
	CG_FRAME(    0,    0,    2,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl22_effect_11 ),
};

/* Char_Effect 7: shl22_00_shot_fly
   Char_Effect 12: shl22_00_move_spark */
const Sint32 pl22_effect_07[] = {
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32775, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32777, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32778, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl22_effect_08[] = {
	CG_FRAME(    0,    0,    2,           0,   712, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   713, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   714, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   715, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_08_loop[] = {
	CG_FRAME(    0,    1,    1,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    1,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    1,           0,   718, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl22_effect_08_loop ),
};

/* Char_Effect 9: shl22_00_shot_fly */
const Sint32 pl22_effect_09[] = {
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32786, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32787, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32788, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32789, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_09 ),
};

/* Char_Effect 13 */
const Sint32 pl22_effect_13[] = {
	CG_FRAME(    0,    0,    6,           0,   664, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   665, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   666, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   667, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl22_effect_14[] = {
	CG_FRAME(    0,    0,    6,           0,   668, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   669, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   670, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   671, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl22_effect_15[] = {
	CG_FRAME(    0,    0,    6,           0,   672, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   674, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   675, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl22_effect_16[] = {
	CG_FRAME(    0,    0,    6,           0,   676, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   677, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   678, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   679, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl22_effect_17[] = {
	CG_FRAME(    0,    0,    6,           0,   680, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   681, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   682, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   683, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl22_effect_18[] = {
	CG_FRAME(    0,    0,    6,           0,   684, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   685, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   686, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   687, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl22_effect_19[] = {
	CG_FRAME(    0,    0,    6,           0,   688, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   689, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   690, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   691, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl22_effect_20[] = {
	CG_FRAME(    0,    0,    6,           0,   692, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   693, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   694, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   695, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl22_effect_21[] = {
	CG_FRAME(    0,    0,    6,           0,   696, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   697, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   698, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   699, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl22_effect_22[] = {
	CG_FRAME(    0,    0,    6,           0,   700, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   701, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   702, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   703, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_23 ),
};

/* Char_Effect 23 */
const Sint32 pl22_effect_23[] = {
	CG_FRAME(    0,    0,    6,           0,   704, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   705, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   706, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   707, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl22_effect_24[] = {
	CG_FRAME(    0,    0,    6,           0,   708, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   709, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   710, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   711, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_24 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl22_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,                0,  0x00,       0x0e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   265, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   269, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl22_change_01[] = {
	CG_FRAME( 0x80,    0,    6,           0,   489, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_change_01_loop[] = {
	CG_FRAME( 0x80,    1, -128, END_OF_DATA,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_change_01_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl22_change_02[] = {
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl22_change_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl22_change_02_loop ),
};

/* Char_Special 42: snapback_init (Snapback) */
const Sint32 pl22_special_42[] = {
	CG_FRAME(    0,    0,    6,           0,   322, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   323, 0x80,   0x0a, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   324, 0x80,   0x01, ATK | CG_ATCK_NG,  0x00,       0x05,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    8,           0,   325, 0x80,   0x01, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   326, 0x80,   0x01, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   327, 0x80,   0x01, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x0a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   302, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl22_special_42 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl22_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   280, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   281, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,                0,  0x00,       0x1b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   42,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_change_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   280, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl22_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   492, 0x80,   0x00,                0,  0x00,       0x1c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   493, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   495, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   496, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   497, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   497, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   498, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_change_04_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_change_04_loop ),
};

/* Char_Caught 0 */
const Sint32 pl22_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   539, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl22_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   540, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl22_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   541, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl22_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   543, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl22_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl22_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   545, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl22_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl22_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   547, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl22_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   548, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl22_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl22_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   551, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl22_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   552, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl22_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   558, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl22_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl22_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   570, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl22_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   572, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl22_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   573, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl22_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   574, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl22_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   575, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl22_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   579, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl22_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   585, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl22_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   569, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl22_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl22_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl22_caught_21 ),
};

/* Char_Effect 0
   Char_Effect 1 */
const Sint32 pl22_effect_00[] = {
	CG_FRAME(    0,    0,    4,           0,   819, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   820, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   821, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   822, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   823, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   824, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_00_loop ),
};

/* Char_Effect 2 */
const Sint32 pl22_effect_02[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   827, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   828, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   829, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   830, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   831, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   832, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   833, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_02_loop ),
};

/* Char_Effect 3 */
const Sint32 pl22_effect_03[] = {
	CG_FRAME(    0,    0,    2,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   813, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_03_loop ),
};

/* Char_Effect 4 */
const Sint32 pl22_effect_04[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   815, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl22_effect_05[] = {
	CG_FRAME(    0,    0,    2,           0,   816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   859, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   860, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   861, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   862, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   863, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   864, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   865, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   866, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   867, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   868, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   869, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   870, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_05_loop ),
};

/* group 25 0
   Char_Fly 0 */
const Sint32 pl22_grp25_00[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   835, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_00 ),
};

/* group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl22_grp25_01[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_01 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl22_grp25_02[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   837, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_02 ),
};

/* group 25 3
   Char_Fly 3 */
const Sint32 pl22_grp25_03[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   838, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_03 ),
};

/* group 25 4
   Char_Fly 4 */
const Sint32 pl22_grp25_04[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_04 ),
};

/* group 25 5
   Char_Fly 5 */
const Sint32 pl22_grp25_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   840, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_05 ),
};

/* group 25 6
   Char_Fly 6 */
const Sint32 pl22_grp25_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32792, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_06 ),
};

/* group 25 7
   Char_Fly 7 */
const Sint32 pl22_grp25_07[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32793, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_07 ),
};

/* group 25 8
   Char_Fly 8 */
const Sint32 pl22_grp25_08[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32794, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_08 ),
};

/* group 25 9
   Char_Fly 9 */
const Sint32 pl22_grp25_09[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32795, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_09 ),
};

/* group 25 10
   Char_Fly 10 */
const Sint32 pl22_grp25_10[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_10 ),
};

/* group 25 11
   Char_Fly 11 */
const Sint32 pl22_grp25_11[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_11 ),
};

/* group 25 12
   Char_Fly 12 */
const Sint32 pl22_grp25_12[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_12 ),
};

/* group 25 13
   Char_Fly 13 */
const Sint32 pl22_grp25_13[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_13 ),
};

/* group 25 14
   Char_Fly 14 */
const Sint32 pl22_grp25_14[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_14 ),
};

/* group 25 15
   Char_Fly 15 */
const Sint32 pl22_grp25_15[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_15 ),
};

/* group 25 16
   Char_Fly 16 */
const Sint32 pl22_grp25_16[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32802, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_16 ),
};

/* group 25 17
   Char_Fly 17 */
const Sint32 pl22_grp25_17[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_17 ),
};

/* group 25 18
   Char_Fly 18 */
const Sint32 pl22_grp25_18[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_18 ),
};

/* group 25 19
   Char_Fly 19 */
const Sint32 pl22_grp25_19[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32805, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_19 ),
};

/* group 25 20
   Char_Fly 20 */
const Sint32 pl22_grp25_20[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32806, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_20 ),
};

/* group 25 21
   Char_Fly 21 */
const Sint32 pl22_grp25_21[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl22_grp25_21 ),
};

/* group 25 22
   Char_Fly 22 */
const Sint32 pl22_grp25_22[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32808, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_22 ),
};

/* group 25 23
   Char_Fly 23 */
const Sint32 pl22_grp25_23[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32809, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl22_grp25_23 ),
};

/* group 27 0 */
const Sint32 pl22_grp27_00[] = {
	EF_FRAME(    0,    0,   30,           0,   726 ),
	EF_FRAME(    0,    0,    4,           0,   727 ),
	EF_FRAME(    0,    0,    4,           0,   728 ),
	EF_FRAME(    0,    0,    4,           0,   727 ),
	EF_FRAME(    0,    0,    4,           0,   728 ),
	EF_FRAME(    0,    0,    4,           0,   727 ),
	EF_FRAME(    0,    0,    4,           0,   728 ),
	EF_FRAME(    0,    0,    4,           0,   729 ),
	EF_FRAME(    0,    0,    4,           0,   730 ),
	EF_FRAME(    0,    0,    4,           0,   731 ),
	EF_FRAME(    0,    0,    4,           0,   732 ),
	EF_FRAME(    0,    0,   24,           0,   733 ),
	EF_FRAME(    0,    0,   24,           0,   734 ),
	EF_FRAME(    0,    0,    6,           0,   735 ),
	EF_FRAME(    0,    0,    6,           0,   736 ),
	EF_FRAME(    0,    0,    6,           0,   737 ),
	EF_FRAME(    0,    0,    6,           0,   738 ),
	EF_FRAME(    0,    0,    6,           0,   739 ),
	EF_FRAME(    0,    0,    6,           0,   740 ),
	EF_FRAME(    0,    0,    6,           0,   741 ),
	EF_FRAME(    0,    0,    6,           0,   742 ),
	EF_FRAME(    0,    0,    6,           0,   743 ),
	EF_FRAME(    0,    0,    6,           0,   744 ),
	EF_FRAME(    0,    0,    6,           0,   745 ),
	EF_FRAME(    0,    0,   32,           0,   746 ),
	EF_FRAME(    0,    1,    4,           0,   752 ),
};

const Sint32 pl22_grp27_00_loop[] = {
	EF_FRAME(    0,    0,    4,           0,   747 ),
	EF_FRAME(    0,    0,    4,           0,   748 ),
	EF_FRAME(    0,    0,    4,           0,   749 ),
	EF_FRAME(    0,    0,    4,           0,   750 ),
	EF_FRAME(    0,    0,    4,           0,   751 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   752 ),
	CG_NEXT( pl22_grp27_00_loop ),
};

/* group 27 2 */
const Sint32 pl22_grp27_02[] = {
	EF_FRAME(    0,    0,    4,           0,   647 ),
	EF_FRAME(    0,    0,    4,           0,   648 ),
	EF_FRAME(    0,    0,    4,           0,   649 ),
	EF_FRAME(    0,    0,    4,           0,   650 ),
	EF_FRAME(    0,    0,    4,           0,   651 ),
	EF_FRAME(    0,    0,    4,           0,   652 ),
	EF_FRAME(    0,    0,    4,           0, 32779 ),
	EF_FRAME(    0,    0,    4,           0, 32780 ),
	EF_FRAME(    0,    0,    4,           0, 32781 ),
	EF_FRAME(    0,    0,    4,           0,   656 ),
	EF_FRAME(    0,    0,    4,           0,   657 ),
	EF_FRAME(    0,    0,    4,           0,   658 ),
	EF_FRAME(    0,    0,    4,           0,   659 ),
	EF_FRAME(    0,    0,    4,           0,   660 ),
	EF_FRAME(    0,    0,    4,           0,   661 ),
	EF_FRAME(    0,    0,    4,           0,   662 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   663 ),
	CG_NEXT( pl22_grp27_02 ),
};

/* Char_Effect 28 */
const Sint32 pl22_effect_28[] = {
	CG_FRAME(    0,    0,    1,           0,   892, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   893, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   894, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   895, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   896, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   897, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   898, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   899, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl22_effect_28 ),
};

/* Char_Effect 29: shl22_00_fire_fly */
const Sint32 pl22_effect_29[] = {
	CG_FRAME(    0,    0,    2,           0,   900, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   901, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   902, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   903, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   904, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_effect_29_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   905, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_29_loop ),
};

/* Char_Effect 30 */
const Sint32 pl22_effect_30[] = {
	CG_FRAME(    0,    0,    1,           0,   906, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1,           0,   907, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1,           0,   908, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1,           0,   909, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1,           0,   910, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1,           0,   911, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1,           0,   912, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   913, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl22_effect_30 ),
};

/* Char_Effect 31: shl22_00_fire_fly */
const Sint32 pl22_effect_31[] = {
	CG_FRAME(    0,    0,    2,           0,   914, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   915, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   916, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   917, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   918, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   919, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_effect_31 ),
};

/* Char_Special 43: ashura_f_init */
const Sint32 pl22_special_43[] = {
	CG_FRAME(    2,    0,    3,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    0,    2,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,    3,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -2,    0,    3,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -4,    0,    2,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -2,    0,    3,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,    2,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_special_43_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_special_43_loop ),
};

/* Char_Special 44: ashura_f_move */
const Sint32 pl22_special_44[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_special_44 ),
};

/* Char_Super 18: shungoku_init (Shun Goku Satsu (level 3 hyper)) */
const Sint32 pl22_super_18[] = {
	CG_FRAME(    0,    0,    6,           0,   652, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   653, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,             0x1a,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl22_super_18_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl22_super_18_loop ),
};

/* Char_Catch 6: shungoku_dash (Shun Goku Satsu (level 3 hyper)) */
const Sint32 pl22_catch_06[] = {
	CG_FRAME(    0,    0,    4,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl22_catch_06 ),
};

/* Char_Catch 7: shungoku_hold (Shun Goku Satsu (level 3 hyper)) */
const Sint32 pl22_catch_07[] = {
	CG_FRAME(    0,    0,   48,           0,   656, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl22_catch_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl22_catch_07_loop ),
};

/* Char_Super 19: sp_dark (the change to and from the dark form) */
const Sint32 pl22_super_19[] = {
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   656, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   656, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   656, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl22_super_19_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl22_super_19_loop ),
};

/* group 27 1 */
const Sint32 pl22_grp27_01[] = {
	EF_FRAME(    0,    0,    8,           0,   657 ),
	EF_FRAME(    0,    0,    2,           0,   658 ),
	EF_FRAME(    0,    0,    2,           0,   659 ),
	EF_FRAME(    0,    0,    2,           0,   660 ),
	EF_FRAME(    0,    0,    2,           0,   661 ),
	EF_FRAME(    0,    0,    2,           0,   662 ),
};

const Sint32 pl22_grp27_01_loop[] = {
	EF_FRAME(    0,    0,   28, END_OF_DATA,   663 ),
	CG_NEXT( pl22_grp27_01_loop ),
};
