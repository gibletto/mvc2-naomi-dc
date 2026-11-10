/* Akuma (character 1e): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_1e      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_1e     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl1e.c) says which group
                   and number each move uses.
   pl1e_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl1e_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl1e_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl1e_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_1e
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_1e, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_1e */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl1e_stand_00[], pl1e_stand_02[], pl1e_stand_03[], pl1e_jump_00[],
	pl1e_jump_00_loop[], pl1e_jump_01[], pl1e_jump_01_loop[], pl1e_jump_02[], pl1e_jump_02_loop[],
	pl1e_jump_03[], pl1e_jump_06[], pl1e_jump_09[], pl1e_jump_07[], pl1e_jump_10[], pl1e_jump_08[],
	pl1e_jump_11[], pl1e_dash_00[], pl1e_dash_00_loop[], pl1e_dash_02[], pl1e_crouch_00[],
	pl1e_crouch_01[], pl1e_crouch_02[], pl1e_turn_00[], pl1e_turn_01[], pl1e_guard_00[],
	pl1e_guard_01[], pl1e_guard_02[], pl1e_guard_03[], pl1e_guard_04[], pl1e_guard_05[],
	pl1e_guard_06[], pl1e_guard_07[], pl1e_stand_p_00[], pl1e_stand_p_01[], pl1e_stand_p_02[],
	pl1e_stand_k_00[], pl1e_stand_k_01[], pl1e_stand_k_02[], pl1e_air_guard_00[],
	pl1e_air_guard_01[], pl1e_crouch_p_00[], pl1e_crouch_p_01[], pl1e_crouch_p_02[], pl1e_extra_12[],
	pl1e_crouch_k_00[], pl1e_crouch_k_01[], pl1e_crouch_k_02[], pl1e_jump_p_00[], pl1e_jump_p_01[],
	pl1e_jump_p_02[], pl1e_jump_k_00[], pl1e_jump_k_01[], pl1e_jump_k_02[], pl1e_get_up_00[],
	pl1e_get_up_01[], pl1e_get_up_02[], pl1e_get_up_03[], pl1e_damage_00[], pl1e_damage_01[],
	pl1e_damage_02[], pl1e_damage_03[], pl1e_damage_04[], pl1e_damage_05[], pl1e_damage_06[],
	pl1e_damage_07[], pl1e_damage_08[], pl1e_damage_10[], pl1e_damage_11[], pl1e_damage_12[],
	pl1e_damage_13[], pl1e_damage_13_loop[], pl1e_damage_09[], pl1e_damage_09_loop[],
	pl1e_damage_16[], pl1e_damage_17[], pl1e_damage_18[], pl1e_damage_19[], pl1e_damage_20[],
	pl1e_damage_21[], pl1e_damage_22[], pl1e_damage_23[], pl1e_damage_24[], pl1e_damage_25[],
	pl1e_damage_26[], pl1e_damage_27[], pl1e_damage_31[], pl1e_damage_32[], pl1e_damage_28[],
	pl1e_damage_29[], pl1e_caught_00[], pl1e_caught_01[], pl1e_caught_02[], pl1e_caught_03[],
	pl1e_caught_04[], pl1e_caught_05[], pl1e_caught_06[], pl1e_caught_07[], pl1e_caught_08[],
	pl1e_caught_09[], pl1e_caught_10[], pl1e_caught_11[], pl1e_caught_12[], pl1e_caught_13[],
	pl1e_caught_14[], pl1e_caught_15[], pl1e_caught_16[], pl1e_caught_17[], pl1e_caught_18[],
	pl1e_caught_19[], pl1e_caught_20[], pl1e_caught_21[], pl1e_dizzy_00[], pl1e_win_00[],
	pl1e_win_00_loop[], pl1e_win_01[], pl1e_win_01_loop[], pl1e_win_02[], pl1e_win_02_loop[],
	pl1e_win_03[], pl1e_win_03_loop[], pl1e_win_04[], pl1e_win_04_loop[], pl1e_win_05[],
	pl1e_appear_00[], pl1e_appear_00_loop[], pl1e_extra_00[], pl1e_extra_11[], pl1e_extra_01[],
	pl1e_extra_01_loop[], pl1e_extra_03[], pl1e_extra_04[], pl1e_extra_05[], pl1e_extra_06[],
	pl1e_extra_06_loop[], pl1e_special_25[], pl1e_special_25_loop[], pl1e_special_26[],
	pl1e_special_26_loop[], pl1e_special_27[], pl1e_special_28[], pl1e_extra_08[], pl1e_extra_09[],
	pl1e_extra_10[], pl1e_change_00[], pl1e_change_00_loop[], pl1e_change_01[],
	pl1e_change_01_loop[], pl1e_catch_00[], pl1e_catch_01[], pl1e_catch_02[], pl1e_catch_02_loop[],
	pl1e_catch_03[], pl1e_catch_03_loop[], pl1e_catch_04[], pl1e_catch_05[], pl1e_catch_05_loop[],
	pl1e_catch_06[], pl1e_special_11[], pl1e_special_11_loop[], pl1e_special_12[],
	pl1e_special_12_loop[], pl1e_special_13[], pl1e_special_00[], pl1e_special_01[],
	pl1e_special_16[], pl1e_special_17[], pl1e_special_18[], pl1e_special_14[], pl1e_special_15[],
	pl1e_special_02[], pl1e_special_03[], pl1e_special_04[], pl1e_special_04_loop[],
	pl1e_special_06[], pl1e_special_05[], pl1e_special_07[], pl1e_special_08[],
	pl1e_special_08_loop[], pl1e_special_09[], pl1e_special_09_loop[], pl1e_special_10[],
	pl1e_special_19[], pl1e_special_21[], pl1e_special_21_loop[], pl1e_special_22[],
	pl1e_special_23[], pl1e_special_23_loop[], pl1e_special_24[], pl1e_super_00[], pl1e_super_07[],
	pl1e_super_01[], pl1e_super_08[], pl1e_super_08_loop[], pl1e_super_02[], pl1e_super_03[],
	pl1e_super_04[], pl1e_super_05[], pl1e_super_06[], pl1e_super_09[], pl1e_super_09_loop[],
	pl1e_super_10[], pl1e_super_10_loop[], pl1e_super_11[], pl1e_unused_00[], pl1e_grp25_00[],
	pl1e_grp25_01[], pl1e_grp25_02[], pl1e_grp25_03[], pl1e_grp25_04[], pl1e_grp25_05[],
	pl1e_grp25_06[], pl1e_grp25_07[], pl1e_grp25_08[], pl1e_grp25_09[], pl1e_grp25_10[],
	pl1e_grp25_11[], pl1e_grp25_12[], pl1e_grp25_13[], pl1e_grp25_14[], pl1e_grp25_15[],
	pl1e_grp25_16[], pl1e_grp25_17[], pl1e_grp25_18[], pl1e_grp25_19[], pl1e_grp25_20[],
	pl1e_grp25_21[], pl1e_grp25_22[], pl1e_grp25_23[], pl1e_effect_00[], pl1e_effect_00_loop[],
	pl1e_effect_02[], pl1e_effect_02_loop[], pl1e_effect_03[], pl1e_effect_04[], pl1e_effect_09[],
	pl1e_effect_10[], pl1e_effect_11[], pl1e_effect_11_loop[], pl1e_effect_12[],
	pl1e_effect_12_loop[], pl1e_effect_13[], pl1e_effect_14[], pl1e_effect_15[],
	pl1e_effect_15_loop[], pl1e_effect_16[], pl1e_effect_16_loop[], pl1e_effect_17[],
	pl1e_grp25_24[], pl1e_grp25_25[], pl1e_grp25_26[], pl1e_grp25_27[], pl1e_grp25_28[],
	pl1e_unused_01[], pl1e_unused_02[], pl1e_change_02[], pl1e_change_02_loop[], pl1e_change_03[],
	pl1e_change_03_loop[], pl1e_change_04[], pl1e_change_04_loop[];
extern const Sint32 *const pl1e_stand_tbl[], *const pl1e_jump_tbl[], *const pl1e_dash_tbl[],
	*const pl1e_crouch_tbl[], *const pl1e_turn_tbl[], *const pl1e_guard_tbl[],
	*const pl1e_air_guard_tbl[], *const pl1e_stand_p_tbl[], *const pl1e_stand_k_tbl[],
	*const pl1e_crouch_p_tbl[], *const pl1e_crouch_k_tbl[], *const pl1e_jump_p_tbl[],
	*const pl1e_jump_k_tbl[], *const pl1e_damage_tbl[], *const pl1e_dizzy_tbl[],
	*const pl1e_catch_tbl[], *const pl1e_caught_tbl[], *const pl1e_get_up_tbl[],
	*const pl1e_appear_tbl[], *const pl1e_win_tbl[], *const pl1e_extra_tbl[],
	*const pl1e_special_tbl[], *const pl1e_super_tbl[], *const pl1e_effect_tbl[],
	*const pl1e_change_tbl[], *const pl1e_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_1e[16] = {
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
const Sint32 *const *const char_tbl_1e[] = {
	pl1e_stand_tbl, pl1e_jump_tbl, pl1e_dash_tbl, pl1e_crouch_tbl, pl1e_turn_tbl, pl1e_guard_tbl,
	pl1e_air_guard_tbl, pl1e_stand_p_tbl, pl1e_stand_k_tbl, pl1e_crouch_p_tbl, pl1e_crouch_k_tbl, pl1e_jump_p_tbl,
	pl1e_jump_k_tbl, pl1e_damage_tbl, pl1e_dizzy_tbl, pl1e_catch_tbl, pl1e_caught_tbl, pl1e_get_up_tbl,
	pl1e_appear_tbl, pl1e_win_tbl, pl1e_extra_tbl, pl1e_special_tbl, pl1e_super_tbl, pl1e_effect_tbl,
	pl1e_change_tbl, pl1e_grp25_tbl, 0,
};

const Sint32 *const pl1e_stand_tbl[] = {
	pl1e_stand_00, pl1e_stand_00, pl1e_stand_02, pl1e_stand_03, pl1e_stand_00, pl1e_stand_00,
	pl1e_stand_02, pl1e_stand_03, 0,
};

const Sint32 *const pl1e_jump_tbl[] = {
	pl1e_jump_00, pl1e_jump_01, pl1e_jump_02, pl1e_jump_03, pl1e_jump_03, pl1e_jump_03,
	pl1e_jump_06, pl1e_jump_07, pl1e_jump_08, pl1e_jump_09, pl1e_jump_10, pl1e_jump_11,
	0,
};

const Sint32 *const pl1e_dash_tbl[] = {
	pl1e_dash_00, pl1e_dash_00, pl1e_dash_02, pl1e_dash_02, 0,
};

const Sint32 *const pl1e_crouch_tbl[] = {
	pl1e_crouch_00, pl1e_crouch_01, pl1e_crouch_02, 0,
};

const Sint32 *const pl1e_turn_tbl[] = {
	pl1e_turn_00, pl1e_turn_01, 0,
};

const Sint32 *const pl1e_guard_tbl[] = {
	pl1e_guard_00, pl1e_guard_01, pl1e_guard_02, pl1e_guard_03, pl1e_guard_04, pl1e_guard_05,
	pl1e_guard_06, pl1e_guard_07, 0,
};

const Sint32 *const pl1e_air_guard_tbl[] = {
	pl1e_air_guard_00, pl1e_air_guard_01, pl1e_air_guard_01, 0,
};

const Sint32 *const pl1e_stand_p_tbl[] = {
	pl1e_stand_p_00, pl1e_stand_p_01, pl1e_stand_p_02, 0,
};

const Sint32 *const pl1e_stand_k_tbl[] = {
	pl1e_stand_k_00, pl1e_stand_k_01, pl1e_stand_k_02, 0,
};

const Sint32 *const pl1e_crouch_p_tbl[] = {
	pl1e_crouch_p_00, pl1e_crouch_p_01, pl1e_crouch_p_02, 0,
};

const Sint32 *const pl1e_crouch_k_tbl[] = {
	pl1e_crouch_k_00, pl1e_crouch_k_01, pl1e_crouch_k_02, 0,
};

const Sint32 *const pl1e_jump_p_tbl[] = {
	pl1e_jump_p_00, pl1e_jump_p_01, pl1e_jump_p_02, pl1e_jump_p_00, pl1e_jump_p_01, pl1e_jump_p_02,
	0,
};

const Sint32 *const pl1e_jump_k_tbl[] = {
	pl1e_jump_k_00, pl1e_jump_k_01, pl1e_jump_k_02, pl1e_jump_k_00, pl1e_jump_k_01, pl1e_jump_k_02,
	0,
};

const Sint32 *const pl1e_damage_tbl[] = {
	pl1e_damage_00, pl1e_damage_01, pl1e_damage_02, pl1e_damage_03, pl1e_damage_04, pl1e_damage_05,
	pl1e_damage_06, pl1e_damage_07, pl1e_damage_08, pl1e_damage_09, pl1e_damage_10, pl1e_damage_11,
	pl1e_damage_12, pl1e_damage_13, pl1e_damage_13, pl1e_damage_13, pl1e_damage_16, pl1e_damage_17,
	pl1e_damage_18, pl1e_damage_19, pl1e_damage_20, pl1e_damage_21, pl1e_damage_22, pl1e_damage_23,
	pl1e_damage_24, pl1e_damage_25, pl1e_damage_26, pl1e_damage_27, pl1e_damage_28, pl1e_damage_29,
	pl1e_damage_27, pl1e_damage_31, pl1e_damage_32, pl1e_damage_32, pl1e_damage_13, 0,
};

const Sint32 *const pl1e_dizzy_tbl[] = {
	pl1e_dizzy_00, 0,
};

const Sint32 *const pl1e_catch_tbl[] = {
	pl1e_catch_00, pl1e_catch_01, pl1e_catch_02, pl1e_catch_03, pl1e_catch_04, pl1e_catch_05,
	pl1e_catch_06, 0,
};

const Sint32 *const pl1e_caught_tbl[] = {
	pl1e_caught_00, pl1e_caught_01, pl1e_caught_02, pl1e_caught_03, pl1e_caught_04, pl1e_caught_05,
	pl1e_caught_06, pl1e_caught_07, pl1e_caught_08, pl1e_caught_09, pl1e_caught_10, pl1e_caught_11,
	pl1e_caught_12, pl1e_caught_13, pl1e_caught_14, pl1e_caught_15, pl1e_caught_16, pl1e_caught_17,
	pl1e_caught_18, pl1e_caught_19, pl1e_caught_20, pl1e_caught_21, 0,
};

const Sint32 *const pl1e_get_up_tbl[] = {
	pl1e_get_up_00, pl1e_get_up_01, pl1e_get_up_02, pl1e_get_up_03, 0,
};

const Sint32 *const pl1e_appear_tbl[] = {
	pl1e_appear_00, 0,
};

const Sint32 *const pl1e_win_tbl[] = {
	pl1e_win_00, pl1e_win_01, pl1e_win_02, pl1e_win_03, pl1e_win_04, pl1e_win_05,
	0,
};

const Sint32 *const pl1e_extra_tbl[] = {
	pl1e_extra_00, pl1e_extra_01, pl1e_extra_01, pl1e_extra_03, pl1e_extra_04, pl1e_extra_05,
	pl1e_extra_06, pl1e_extra_06, pl1e_extra_08, pl1e_extra_09, pl1e_extra_10, pl1e_extra_11,
	pl1e_extra_12, 0,
};

const Sint32 *const pl1e_special_tbl[] = {
	pl1e_special_00, pl1e_special_01, pl1e_special_02, pl1e_special_03, pl1e_special_04, pl1e_special_05,
	pl1e_special_06, pl1e_special_07, pl1e_special_08, pl1e_special_09, pl1e_special_10, pl1e_special_11,
	pl1e_special_12, pl1e_special_13, pl1e_special_14, pl1e_special_15, pl1e_special_16, pl1e_special_17,
	pl1e_special_18, pl1e_special_19, pl1e_special_19, pl1e_special_21, pl1e_special_22, pl1e_special_23,
	pl1e_special_24, pl1e_special_25, pl1e_special_26, pl1e_special_27, pl1e_special_28, 0,
};

const Sint32 *const pl1e_super_tbl[] = {
	pl1e_super_00, pl1e_super_01, pl1e_super_02, pl1e_super_03, pl1e_super_04, pl1e_super_05,
	pl1e_super_06, pl1e_super_07, pl1e_super_08, pl1e_super_09, pl1e_super_10, pl1e_super_11,
	0,
};

const Sint32 *const pl1e_effect_tbl[] = {
	pl1e_effect_00, pl1e_effect_00, pl1e_effect_02, pl1e_effect_03, pl1e_effect_04, pl1e_effect_04,
	pl1e_effect_04, pl1e_effect_04, pl1e_effect_04, pl1e_effect_09, pl1e_effect_10, pl1e_effect_11,
	pl1e_effect_12, pl1e_effect_13, pl1e_effect_14, pl1e_effect_15, pl1e_effect_16, pl1e_effect_17,
	0,
};

const Sint32 *const pl1e_change_tbl[] = {
	pl1e_change_00, pl1e_change_01, pl1e_change_02, pl1e_change_03, pl1e_change_04, 0,
};

const Sint32 *const pl1e_grp25_tbl[] = {
	pl1e_grp25_00, pl1e_grp25_01, pl1e_grp25_02, pl1e_grp25_03, pl1e_grp25_04, pl1e_grp25_05,
	pl1e_grp25_06, pl1e_grp25_07, pl1e_grp25_08, pl1e_grp25_09, pl1e_grp25_10, pl1e_grp25_11,
	pl1e_grp25_12, pl1e_grp25_13, pl1e_grp25_14, pl1e_grp25_15, pl1e_grp25_16, pl1e_grp25_17,
	pl1e_grp25_18, pl1e_grp25_19, pl1e_grp25_20, pl1e_grp25_21, pl1e_grp25_22, pl1e_grp25_23,
	pl1e_grp25_24, pl1e_grp25_25, pl1e_grp25_26, pl1e_grp25_27, pl1e_grp25_28, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl1e_stand_00[] = {
	CG_FRAME(    0,    0,   3,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl1e_stand_02[] = {
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl1e_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl1e_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   6,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   6,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   6,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   6,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

const Sint32 pl1e_jump_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl1e_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl1e_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

const Sint32 pl1e_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl1e_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl1e_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

const Sint32 pl1e_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl1e_jump_02_loop ),
};

/* Char_Jump 3: jtatsumaki_spin (Tatsumaki Zankuukyaku in the air), jtatsumaki_fall (Tatsumaki Zankuukyaku in the air)
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl1e_jump_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl1e_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl1e_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl1e_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl1e_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl1e_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl1e_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_jump_11 ),
};

/* Char_Dash 0: act_dash
   Char_Dash 1: noki_init, act_back_dash */
const Sint32 pl1e_dash_00[] = {
	CG_FRAME(    0,    1,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_dash_00_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_dash_00_loop ),
};

/* Char_Dash 2: dash_move
   Char_Dash 3: noki_move */
const Sint32 pl1e_dash_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_dash_02 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl1e_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl1e_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl1e_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl1e_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl1e_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl1e_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl1e_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl1e_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl1e_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl1e_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl1e_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl1e_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl1e_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl1e_stand_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl1e_stand_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   6,           0,   162, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl1e_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   165, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   8,           0,   164, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl1e_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10,           0,   169, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl1e_stand_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10,           0,   170, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    1,   6,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl1e_stand_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   5,           0,   173, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   8,           0,   174, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   176, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl1e_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl1e_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl1e_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   177, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   177, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl1e_crouch_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   180, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl1e_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x00,          ATK | 3,  0x00,       0x05,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  11, END_OF_DATA,   183, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl1e_crouch_p_02 ),
};

/* Char_Extra 12: pl1e_upper */
const Sint32 pl1e_extra_12[] = {
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x00,          ATK | 3,  0x00,       0x05,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,  11, END_OF_DATA,   183, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl1e_extra_12 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl1e_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   186, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   185, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl1e_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   188, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   185, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl1e_crouch_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   190, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   191, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,  10,           0,   192, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   108, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl1e_jump_p_00[] = {
	CG_FRAME(    0,    0,   5,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   194, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_NEXT( pl1e_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl1e_jump_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   195, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   196, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl1e_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   196, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl1e_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  16,           0,   198, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   199, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl1e_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  12,           0,   198, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   199, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl1e_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   201, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   5,           0,   202, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   203, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl1e_get_up_00[] = {
	CG_FRAME(    0,    0,   6,           0,   120, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   121, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl1e_get_up_01[] = {
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl1e_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl1e_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl1e_damage_00[] = {
	CG_FRAME( 0x80,    0,   2,           0,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   5,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl1e_damage_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   8,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl1e_damage_02[] = {
	CG_FRAME( 0x80,    0,   2,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,  10,           0,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl1e_damage_03[] = {
	CG_FRAME( 0x80,    0,   2,           0,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   5,           0,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl1e_damage_04[] = {
	CG_FRAME( 0x80,    0,   2,           0,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   8,           0,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl1e_damage_05[] = {
	CG_FRAME( 0x80,    0,   2,           0,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,  10,           0,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl1e_damage_06[] = {
	CG_FRAME( 0x80,    0,   2,           0,   342, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   5,           0,   343, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   342, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl1e_damage_07[] = {
	CG_FRAME( 0x80,    0,   2,           0,   342, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,  10,           0,   343, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   342, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl1e_damage_08[] = {
	CG_FRAME( 0x80,    0,   2,           0,   342, 0x80,   0x50,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,  10,           0,   343, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,   4,           0,   344, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   4,           0,   343, 0x80,   0x52,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   342, 0x80,   0x50,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1e_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl1e_damage_10[] = {
	CG_FRAME( 0x80,    0,   1,           0,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,   8,           0,   346, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl1e_damage_11[] = {
	CG_FRAME( 0x80,    0,   1,           0,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   1,           0,   346, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,   6,           0,   347, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl1e_damage_12[] = {
	CG_FRAME( 0x80,    0,   1,           0,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   1,           0,   346, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   1,           0,   347, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,   4,           0,   348, 0x80,   0x36,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x35,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x32,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   345, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl1e_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1e_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   353, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1e_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl1e_damage_09[] = {
	CG_FRAME(    0,    0,   8,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   355, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   356, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x0e,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x80,   0x8d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x8d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   367, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl1e_damage_09_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   368, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1e_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl1e_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl1e_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   359, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl1e_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   360, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl1e_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   361, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl1e_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   362, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl1e_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl1e_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl1e_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   360, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl1e_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   361, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl1e_damage_25[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   362, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl1e_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   363, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x80,   0x8e,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x8e,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   367, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1e_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl1e_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl1e_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   368, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl1e_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl1e_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl1e_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   374, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl1e_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   380, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1e_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl1e_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl1e_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl1e_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl1e_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl1e_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl1e_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl1e_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl1e_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl1e_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl1e_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl1e_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl1e_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl1e_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl1e_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl1e_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl1e_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl1e_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl1e_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl1e_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   352, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl1e_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl1e_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   357, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl1e_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   359, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl1e_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   360, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl1e_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   361, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl1e_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   362, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl1e_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   369, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl1e_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   375, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1e_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl1e_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   368, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl1e_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl1e_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   354, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1e_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl1e_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   381, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   382, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   383, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   382, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_dizzy_00 ),
};

/* Char_Win 0 */
const Sint32 pl1e_win_00[] = {
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x54,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_win_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl1e_win_01[] = {
	CG_FRAME(    0,    0,   8,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   119, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_win_01_loop[] = {
	CG_FRAME(    0,    1,  -1, END_OF_DATA,   119, 0x80,   0x00,                7,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl1e_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   117, 0x80,   0x54,                8,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_win_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_win_02_loop ),
};

/* Char_Win 3 */
const Sint32 pl1e_win_03[] = {
	CG_FRAME(    0,    0,   6,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   117, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_win_03_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_win_03_loop ),
};

/* Char_Win 4 */
const Sint32 pl1e_win_04[] = {
	CG_FRAME(    0,    0,   1,           0,   119, 0x80,   0x00,             0x12,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_win_04_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   119, 0x80,   0x00,             0x12,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_win_04_loop ),
};

/* Char_Win 5 */
const Sint32 pl1e_win_05[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_win_05 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl1e_appear_00[] = {
	CG_FRAME(    0,    0,  24,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x55,                8,  0x00, 0x31 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   302, 0x80,   0x55,                6,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_appear_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   304, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   305, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   306, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   307, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   308, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   309, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   310, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   311, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_appear_00_loop ),
};

/* Char_Extra 0 */
const Sint32 pl1e_extra_00[] = {
	CG_FRAME(    0,    0,   6,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   227, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   228, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   4,           0,   229, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   4,           0,   230, 0x80,   0x00, ATK | CG_ATCK_NG | 0x16,  0x00, 0x15 | CSE,    0,   0,     0,      0,  80 ),
	CG_FRAME(    1,    0,   4,           0,   231, 0x80,   0x00, ATK | CG_ATCK_NG | 1,  0x00,       0x03,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,   4,           0,   232, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA,   232, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl1e_extra_00 ),
};

/* Char_Extra 11: snapback_init (Snap Back) */
const Sint32 pl1e_extra_11[] = {
	CG_FRAME(    0,    0,   6,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   227, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,   -1,   2,           0,   228, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   2,           0,   229, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   2,           0,   230, 0x80,   0x00, ATK | CG_ATCK_NG | 0x16,  0x00, 0x15 | CSE,    0,   0,     0,      0,  80 ),
	CG_FRAME(    1,    0,   4,           0,   231, 0x80,   0x00, ATK | CG_ATCK_NG | 1,  0x00,       0x03,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    1,   4,           0,   232, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    1,  16, END_OF_DATA,   232, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl1e_extra_11 ),
};

/* Char_Extra 1
   Char_Extra 2: fly_start5 */
const Sint32 pl1e_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   244, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00, ATK | CG_ATCK_NG | 0x16,  0x00, 0x15 | CSE,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x00, ATK | CG_ATCK_NG | 1,  0x00,       0x03,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl1e_extra_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   203, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl1e_extra_01_loop ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl1e_extra_03[] = {
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   207, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl1e_extra_03 ),
};

/* Char_Extra 4 */
const Sint32 pl1e_extra_04[] = {
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   7,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   209, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl1e_extra_05[] = {
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   209, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_extra_05 ),
};

/* Char_Extra 6: sp_chouhatsu (taunt)
   Char_Extra 7 */
const Sint32 pl1e_extra_06[] = {
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                5,  0x00,       0x06,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_extra_06_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_extra_06_loop ),
};

/* Char_Special 25: kuujin_init (Tenma Kuujinkyaku, 25 + atck_level) */
const Sint32 pl1e_special_25[] = {
	CG_FRAME(    0,    8,   8,           0,   208, 0x80,   0x00,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   4,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1e_special_25_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   279, 0x80,   0x00,              ATK,  0x15,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl1e_special_25_loop ),
};

/* Char_Special 26 */
const Sint32 pl1e_special_26[] = {
	CG_FRAME(    0,    1,  12,           0,   208, 0x80,   0x00,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   4,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1e_special_26_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   279, 0x80,   0x00,              ATK,  0x15,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl1e_special_26_loop ),
};

/* Char_Special 27: kuujin_move (Tenma Kuujinkyaku, 27 + atck_level) */
const Sint32 pl1e_special_27[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_special_27 ),
};

/* Char_Special 28 */
const Sint32 pl1e_special_28[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl1e_special_28 ),
};

/* Char_Extra 8: t_kick_init */
const Sint32 pl1e_extra_08[] = {
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  64, END_OF_DATA,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl1e_extra_08 ),
};

/* Char_Extra 9: t_kick_move */
const Sint32 pl1e_extra_09[] = {
	CG_FRAME(    0,    0,   8,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,       ATE | 0x10,  0x00,       0x0b,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   118, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   106, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   107, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_extra_09 ),
};

/* Char_Extra 10: stand_kick_move */
const Sint32 pl1e_extra_10[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    79, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_extra_10 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl1e_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,    80, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x30,   0x00,       ATK | 0x0f,  0x00,       0x10,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_change_00_loop[] = {
	CG_FRAME(    0,    1,  10, END_OF_DATA,   119, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl1e_change_01[] = {
	CG_FRAME(    0,    0,   2,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_change_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   277, 0x80,   0x55,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_change_01_loop ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl1e_catch_00[] = {
	CG_FRAME(    0,    0,  15,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,  10,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   6,           0,   213, 0x80,   0x1f,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,   6,           0,   214, 0x80,   0x1f,                3,  0x00,       0x05,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,  30,           0,   215, 0x80,   0x1f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_catch_00 ),
};

/* Char_Catch 1: nml_p_set */
const Sint32 pl1e_catch_01[] = {
	CG_FRAME(    0,    0,  15,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,  10,           0,   222, 0x80,   0x00,                4,  0x00,       0x07,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   6,           0,   223, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   5,           0,   224, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    1,  42,           0,   225, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_catch_01 ),
};

/* Char_Catch 2: shungoku_run (Shun Goku Satsu (hyper)) */
const Sint32 pl1e_catch_02[] = {
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x00,                2,  0x00,       0x04,    0,   0,     0, 0x0001,   0 ),
};

const Sint32 pl1e_catch_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_NEXT( pl1e_catch_02_loop ),
};

/* Char_Catch 3: shungoku_catch (Shun Goku Satsu (hyper)) */
const Sint32 pl1e_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0x00,                7,  0x00,       0x10,    0,   0,     0, 0x0001,   0 ),
};

const Sint32 pl1e_catch_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_NEXT( pl1e_catch_03_loop ),
};

/* Char_Catch 4: jump_k_set */
const Sint32 pl1e_catch_04[] = {
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   3,           0,   217, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_NEXT( pl1e_catch_04 ),
};

/* Char_Catch 5: jump_k_catch */
const Sint32 pl1e_catch_05[] = {
	CG_FRAME(    0,    0,  16,           0,   121, 0x80,   0x00,                2,  0x00,       0x04,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    1,   3,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_catch_05_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_catch_05_loop ),
};

/* Char_Catch 6: catch_com_set */
const Sint32 pl1e_catch_06[] = {
	CG_FRAME(    1,    0,   1,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    1,    0,  14,           0,   211, 0x80,   0x00,                1,  0x00,       0x03,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    1,    0,   6,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   8,           0,   217, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   7,           0,   218, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   6,           0,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   6,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   4,           0,   217, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x1f,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    1,    1,  32,           0,   214, 0x80,   0x1f,                3,  0x00,       0x05,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    1,    0,   2,           0,   215, 0x80,   0x1f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   6,           0,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   6, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_catch_06 ),
};

/* Char_Special 11 */
const Sint32 pl1e_special_11[] = {
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl1e_special_11_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1e_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl1e_special_12[] = {
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl1e_special_12_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1e_special_12_loop ),
};

/* Char_Special 13: shl1e_00_hadou_hit */
const Sint32 pl1e_special_13[] = {
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   420, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   422, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_special_13 ),
};

/* Char_Special 0 */
const Sint32 pl1e_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   236, 0x80,   0x4b,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x4b,       ATK | 0x0a,  0x00,       0x1e,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl1e_special_01[] = {
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   7,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   236, 0x80,   0x4b,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,       ATK | 0x0a,  0x00,       0x1e,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   5,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_01 ),
};

/* Char_Special 16: shl1e_02_init */
const Sint32 pl1e_special_16[] = {
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl1e_special_16 ),
};

/* Char_Special 17 */
const Sint32 pl1e_special_17[] = {
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl1e_special_17 ),
};

/* Char_Special 18: shl1e_00_zankuu_hit, shl1e_02_hit */
const Sint32 pl1e_special_18[] = {
	CG_FRAME(    0,    0,   4,           0, 32785, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32786, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32787, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32789, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32790, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_special_18 ),
};

/* Char_Special 14: zankuu_init (Zankuu Hadouken, 14 + atck_level) */
const Sint32 pl1e_special_14[] = {
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0, 0x80,   3,           0,   258, 0x80,   0x00,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   257, 0x80,   0x00,       ATK | 0x0a,  0x00,       0x1e,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl1e_special_15[] = {
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0, 0x80,   1,           0,   258, 0x80,   0x00,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,       ATK | 0x0a,  0x00,       0x1e,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   5,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_special_15 ),
};

/* Char_Special 2: shouryuu_init (Gou Shouryuuken, 2 + atck_level) */
const Sint32 pl1e_special_02[] = {
	CG_FRAME(    0, 0x80,   1,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0d00,   0 ),
	CG_FRAME(    0, 0x80,   3,           0,   238, 0x80,   0x00,       ATK | 0x11,  0x00,       0x14,    0,   0,     0, 0x0d00,   0 ),
	CG_FRAME(   51, 0x80,   4,           0,   239, 0x80,   0x00,       ATK | 0x15,  0x00,          0,    0,   0,     0, 0x0e00,  62 ),
	CG_FRAME(   52,    0,   6,           0,   240, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x0f00,  63 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00, 111 ),
	CG_FRAME(    0,    0,   6,           0,   241, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0, 0x80,   3,           0,   242, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   243, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   104, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl1e_special_03[] = {
	CG_FRAME(    0, 0x80,   1,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0d00,   0 ),
	CG_FRAME(   53, 0x80,   3,           0,   238, 0x80,   0x00,       ATK | 0x11,  0x00,       0x14,    0,   0,     0, 0x0d00,  64 ),
	CG_FRAME(   54, 0x80,   4,           0,   239, 0x80,   0x00,       ATK | 0x15,  0x00,          0,    0,   0,     0, 0x0e00,  62 ),
	CG_FRAME(   55,    0,  10,           0,   240, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x0f00,  63 ),
	CG_FRAME(    0,    0,   8,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00, 111 ),
	CG_FRAME(    0,    0,   6,           0,   241, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   242, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   243, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   104, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   9,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_03 ),
};

/* Char_Special 4: tatsumaki_init (Tatsumaki Zankuukyaku) */
const Sint32 pl1e_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,          ATK | 2,  0x00,       0x04,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
};

const Sint32 pl1e_special_04_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   247, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl1e_special_04_loop ),
};

/* Char_Special 6: tatsumaki_rise (Tatsumaki Zankuukyaku) */
const Sint32 pl1e_special_06[] = {
	CG_FRAME(    0,    1,   2,           0,   248, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    1,   2,           0,   250, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,   -1,   1, END_OF_DATA,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl1e_special_06 ),
};

/* Char_Special 5: tatsumaki_spin (Tatsumaki Zankuukyaku), gou_rasen_rise (Messatsu Gou Rasen (hyper)), gou_rasen_last (Messatsu Gou Rasen (hyper)) */
const Sint32 pl1e_special_05[] = {
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   255, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl1e_special_05 ),
};

/* Char_Special 7: tatsumaki_fall (Tatsumaki Zankuukyaku), gou_rasen_fall (Messatsu Gou Rasen (hyper)) */
const Sint32 pl1e_special_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_07 ),
};

/* Char_Special 8: jtatsumaki_init (Tatsumaki Zankuukyaku in the air) */
const Sint32 pl1e_special_08[] = {
	CG_FRAME(    0,    0,   6,           0,   247, 0x80,   0x00,          ATK | 2,  0x00,       0x04,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1e_special_08_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   248, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   250, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,   -1,   1, END_OF_DATA,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl1e_special_09[] = {
	CG_FRAME(    0,    0,   6,           0,   247, 0x80,   0x00,          ATK | 2,  0x00,       0x04,    0,   0,     0,      0,   7 ),
};

const Sint32 pl1e_special_09_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   248, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   250, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,   -1,   1, END_OF_DATA,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_special_09_loop ),
};

/* Char_Special 10: jtatsumaki_spin (Tatsumaki Zankuukyaku in the air) */
const Sint32 pl1e_special_10[] = {
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   255, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   252, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_special_10 ),
};

/* Char_Special 19
   Char_Special 20 */
const Sint32 pl1e_special_19[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_19 ),
};

/* Char_Special 21: ashura_init (Ashura Senkuu forward), ashura_b_init (Ashura Senkuu back) */
const Sint32 pl1e_special_21[] = {
	CG_FRAME(    2,    0,   3,           0, 32769, 0x80,   0x53,          ATE | 2,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    0,   2,           0, 32770, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,    0,   3,           0, 32771, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -2,    0,   3,           0, 32773, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -4,    0,   2,           0, 32774, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -2,    0,   3,           0, 32775, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   2,           0, 32768, 0x80,   0x00,                9,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_special_21_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_special_21_loop ),
};

/* Char_Special 22: ashura_stop (Ashura Senkuu forward), ashura_b_stop (Ashura Senkuu back) */
const Sint32 pl1e_special_22[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl1e_special_23[] = {
	CG_FRAME(    0,    0,   6,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl1e_special_23_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   127, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl1e_special_23_loop ),
};

/* Char_Special 24 */
const Sint32 pl1e_special_24[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_special_24 ),
};

/* Char_Super 0: gou_shouryuu_init (Messatsu Gou Shouryuu (hyper)) */
const Sint32 pl1e_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   260, 0x80,   0x00,          ATK | 5,  0x00,       0x06,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,   -1,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x88,    0,   2, END_OF_DATA,   260, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_super_00 ),
};

/* Char_Super 7: gou_shouryuu_start (Messatsu Gou Shouryuu (hyper)) */
const Sint32 pl1e_super_07[] = {
	CG_FRAME(    0,    0,   1,           0,   239, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0e00,  77 ),
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x00,          ATK | 2,  0x49,       0x04,    0,   0,     0, 0x0e00,  77 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,       ATK | 0x15,  0x4a,          0,    0,   0,     0, 0x0e00,  77 ),
	CG_FRAME(    0,    0,   3,           0,   240, 0x80,   0x00,              ATK,  0x4a, 0x4b | CSE,    0,   0,     0, 0x0f00,  76 ),
	CG_FRAME(    0,    0,   3,           0,   240, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0, 0x0f00,  76 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   1,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   2,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0d00, 101 ),
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x00,          ATK | 2,  0x4a,       0x04,    0,   0,     0, 0x0e00, 103 ),
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x00,       ATK | 0x15,  0x4a,          0,    0,   0,     0, 0x0e00, 103 ),
	CG_FRAME(    0,    0,   3,           0,   240, 0x80,   0x00,              ATK,  0x4a, 0x4b | CSE,    0,   0,     0, 0x0f00, 104 ),
	CG_FRAME(    0,    0,   3,           0,   240, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0, 0x0f00, 104 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    0,   1,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0d00, 101 ),
	CG_FRAME(    0,    1,   3,           0,   239, 0x80,   0x00,       ATK | 0x11,  0x4f,       0x14,    0,   0,     0, 0x0e00, 103 ),
	CG_FRAME(    0,    1,   3,           0,   239, 0x80,   0x00,       ATK | 0x15,  0x00,          0,    0,   0,     0, 0x0e00, 103 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x4f, 0x4b | CSE,    0,   0,     0, 0x0f00, 105 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00, 105 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0, 0x0f00, 105 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00, 105 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0f00,  65 ),
	CG_FRAME(    0,    0,   6,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    1,   2,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  65 ),
	CG_FRAME(    0,    1,   3,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   107, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_super_07 ),
};

/* Char_Super 1: gou_hadou_init (Messatsu Gou Hadou (hyper)) */
const Sint32 pl1e_super_01[] = {
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  21,           0,   303, 0x80,   0x4b,          ATK | 6,  0x00,       0x0a,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    1,   2,           0,   304, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   306, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   307, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   308, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   309, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   310, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x88,    0,   2, END_OF_DATA,   311, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_super_01 ),
};

/* Char_Super 8: gou_hadou_start (Messatsu Gou Hadou (hyper)) */
const Sint32 pl1e_super_08[] = {
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   235, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   237, 0x80,   0x4b,       ATK | 0x0b,  0x00, 0x48 | CSE,    0,   0,     0, 0x0300,  59 ),
	CG_FRAME(    0, 0x80,   1,           0,   237, 0x80,   0x4b,          ATK | 2,  0x00,       0x04,    0,   0,     0, 0x0300,  59 ),
};

const Sint32 pl1e_super_08_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  59 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   237, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  59 ),
	CG_NEXT( pl1e_super_08_loop ),
};

/* Char_Super 2: gou_hadou_beam (Messatsu Gou Hadou (hyper)) */
const Sint32 pl1e_super_02[] = {
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl1e_super_02 ),
};

/* Char_Super 3: tenma_init (Tenma Gou Zankuu (air hyper)) */
const Sint32 pl1e_super_03[] = {
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   269, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    1,   2,           0,   270, 0x80,   0x00,       ATK | 0x14,  0x00,       0x17,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   271, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x80,    0,   2,           0,   269, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x88,    0,   2, END_OF_DATA,   270, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl1e_super_03 ),
};

/* Char_Super 4: tenma_shot (Tenma Gou Zankuu (air hyper)) */
const Sint32 pl1e_super_04[] = {
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,       ATK | 0x0a,  0x00,       0x1e,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,          ATK | 1,  0x00,       0x03,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl1e_super_04 ),
};

/* Char_Super 5: tenma_beam (Tenma Gou Zankuu (air hyper)) */
const Sint32 pl1e_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl1e_super_05 ),
};

/* Char_Super 6: shungoku_init (Shun Goku Satsu (hyper)) */
const Sint32 pl1e_super_06[] = {
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   280, 0x80,   0x00,                5,  0x00,       0x06,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   281, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   282, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x80, 0x80,   2,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x87, 0x80,   2, END_OF_DATA,   280, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl1e_super_06 ),
};

/* Char_Super 9: shungoku_start (Shun Goku Satsu (hyper)) */
const Sint32 pl1e_super_09[] = {
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 108 ),
};

const Sint32 pl1e_super_09_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32768, 0x80,   0x00,                9,  0x00,       0x1f,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl1e_super_09_loop ),
};

/* Char_Super 10: gou_rasen_init (Messatsu Gou Rasen (hyper)) */
const Sint32 pl1e_super_10[] = {
	CG_FRAME(    0,    0,   6,           0,   145, 0x80,   0x00,                0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   306, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   307, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   308, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   309, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   310, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x88,    2,   2,           0,   311, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl1e_super_10_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   248, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    1,   2,           0,   249, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   2,           0,   250, 0x80,   0x00,       ATK | 0x0c,  0x00, 0x4f | CSE,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl1e_super_10_loop ),
};

/* Char_Super 11: gou_rasen_rise (Messatsu Gou Rasen (hyper)) */
const Sint32 pl1e_super_11[] = {
	CG_FRAME(    0,    0,   2,           0,   204, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   201, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   203, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl1e_super_11 ),
};

const Sint32 pl1e_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	0x000048e0,
};

/* group 25 0 */
const Sint32 pl1e_grp25_00[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_00 ),
};

/* group 25 1 */
const Sint32 pl1e_grp25_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32808, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_01 ),
};

/* group 25 2 */
const Sint32 pl1e_grp25_02[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32809, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_02 ),
};

/* group 25 3 */
const Sint32 pl1e_grp25_03[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32810, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_03 ),
};

/* group 25 4 */
const Sint32 pl1e_grp25_04[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32811, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_04 ),
};

/* group 25 5 */
const Sint32 pl1e_grp25_05[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32812, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_05 ),
};

/* group 25 6 */
const Sint32 pl1e_grp25_06[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32813, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_06 ),
};

/* group 25 7 */
const Sint32 pl1e_grp25_07[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32814, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_07 ),
};

/* group 25 8 */
const Sint32 pl1e_grp25_08[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32815, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_08 ),
};

/* group 25 9 */
const Sint32 pl1e_grp25_09[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32816, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_09 ),
};

/* group 25 10 */
const Sint32 pl1e_grp25_10[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32817, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_10 ),
};

/* group 25 11 */
const Sint32 pl1e_grp25_11[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32818, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_11 ),
};

/* group 25 12 */
const Sint32 pl1e_grp25_12[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32819, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_12 ),
};

/* group 25 13 */
const Sint32 pl1e_grp25_13[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32820, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_13 ),
};

/* group 25 14 */
const Sint32 pl1e_grp25_14[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32821, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_14 ),
};

/* group 25 15 */
const Sint32 pl1e_grp25_15[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32822, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_15 ),
};

/* group 25 16 */
const Sint32 pl1e_grp25_16[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32823, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_16 ),
};

/* group 25 17 */
const Sint32 pl1e_grp25_17[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32824, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_17 ),
};

/* group 25 18 */
const Sint32 pl1e_grp25_18[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32825, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_18 ),
};

/* group 25 19 */
const Sint32 pl1e_grp25_19[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32826, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_19 ),
};

/* group 25 20 */
const Sint32 pl1e_grp25_20[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32827, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_20 ),
};

/* group 25 21 */
const Sint32 pl1e_grp25_21[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32828, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl1e_grp25_21 ),
};

/* group 25 22 */
const Sint32 pl1e_grp25_22[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32829, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_22 ),
};

/* group 25 23 */
const Sint32 pl1e_grp25_23[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32830, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl1e_grp25_23 ),
};

/* Char_Effect 0
   Char_Effect 1 */
const Sint32 pl1e_effect_00[] = {
	CG_FRAME(    0,    0,   4,           0, 32791, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_effect_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32796, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_00_loop ),
};

/* Char_Effect 2 */
const Sint32 pl1e_effect_02[] = {
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_effect_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32806, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_02_loop ),
};

/* Char_Effect 3 */
const Sint32 pl1e_effect_03[] = {
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   473, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   477, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   481, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_03 ),
};

/* Char_Effect 4
   Char_Effect 5
   Char_Effect 6
   Char_Effect 7
   Char_Effect 8: shl1e_02_init */
const Sint32 pl1e_effect_04[] = {
	CG_FRAME(    0,    0,   2,           0,   494, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   495, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   496, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32831, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32832, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32833, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_04 ),
};

/* Char_Effect 9: shl1e_02_init */
const Sint32 pl1e_effect_09[] = {
	CG_FRAME(    0,    0,   2,           0,   500, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   501, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32835, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32836, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl1e_effect_10[] = {
	CG_FRAME(    0,    0,   2,           0,   442, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   449, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   452, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   457, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl1e_effect_11[] = {
	CG_FRAME(    0,    0,   2,           0,   435, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   436, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   437, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_effect_11_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   441, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_11_loop ),
};

/* Char_Effect 12 */
const Sint32 pl1e_effect_12[] = {
	CG_FRAME(    0,    0,  16,           0,   441, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   437, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   436, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_effect_12_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   435, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_12_loop ),
};

/* Char_Effect 13 */
const Sint32 pl1e_effect_13[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   506, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl1e_effect_14[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   507, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl1e_effect_15[] = {
	CG_FRAME(    0,    0,   2,           0,   508, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_effect_15_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   511, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   515, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_15_loop ),
};

/* Char_Effect 16 */
const Sint32 pl1e_effect_16[] = {
	CG_FRAME(    0,    0,   2,           0,   517, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   518, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   520, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   521, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   522, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   523, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   524, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl1e_effect_16_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_16_loop ),
};

/* Char_Effect 17: pls1e_04_tatsumaki_init, pls1e_04_gou_rasen_init */
const Sint32 pl1e_effect_17[] = {
	CG_FRAME(   -1,   -1,   2,           0,   458, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -1,   -1,   2,           0,   459, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -1,   -1,   2,           0,   460, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -1,   -1,   2,           0,   461, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -1,   -1,   2,           0,   462, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(   -1,   -1,   2,           0,   463, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   464, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   465, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   466, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   467, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   468, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   4, END_OF_DATA,   469, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_effect_17 ),
};

/* group 25 24 */
const Sint32 pl1e_grp25_24[] = {
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_grp25_24 ),
};

/* group 25 25 */
const Sint32 pl1e_grp25_25[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   404, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   408, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_grp25_25 ),
};

/* group 25 26 */
const Sint32 pl1e_grp25_26[] = {
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   431, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   432, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   434, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_grp25_26 ),
};

/* group 25 27 */
const Sint32 pl1e_grp25_27[] = {
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   465, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   469, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_grp25_27 ),
};

/* group 25 28 */
const Sint32 pl1e_grp25_28[] = {
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   486, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   487, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   488, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   493, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1e_grp25_28 ),
};

const Sint32 pl1e_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000057ec,
};

const Sint32 pl1e_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000058b8,
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl1e_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

const Sint32 pl1e_change_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl1e_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl1e_change_03[] = {
	CG_FRAME(    2,    0,   3,           0, 32769, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    4,    0,   2,           0, 32770, 0x80,   0x53,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   1,           0, 32771, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    1,   2,           0, 32771, 0x80,   0x53,                0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_change_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0, 32773, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0, 32775, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   3,           0, 32769, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    4,    0,   2,           0, 32770, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   3, END_OF_DATA, 32771, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl1e_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                5,  0x00,       0x06,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1e_change_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  34, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1e_change_04_loop ),
};
