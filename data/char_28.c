/* Gambit (character 28): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_28      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_28     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl28.c) says which group
                   and number each move uses.
   pl28_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl28_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl28_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl28_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_28
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_28, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_28 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl28_stand_00[], pl28_stand_02[], pl28_stand_06[], pl28_stand_03[],
	pl28_stand_07[], pl28_jump_00[], pl28_jump_00_loop[], pl28_jump_01[], pl28_jump_01_loop[],
	pl28_jump_02[], pl28_jump_02_loop[], pl28_jump_03[], pl28_jump_04[], pl28_jump_06[],
	pl28_jump_06_loop[], pl28_jump_07[], pl28_jump_07_loop[], pl28_jump_08[], pl28_jump_08_loop[],
	pl28_jump_09[], pl28_dash_00[], pl28_dash_00_loop[], pl28_dash_02[], pl28_dash_01[],
	pl28_dash_01_loop[], pl28_dash_03[], pl28_crouch_00[], pl28_crouch_01[], pl28_crouch_02[],
	pl28_turn_00[], pl28_turn_01[], pl28_guard_00[], pl28_guard_01[], pl28_guard_02[],
	pl28_guard_02_loop[], pl28_guard_03[], pl28_guard_04[], pl28_guard_05[], pl28_guard_06[],
	pl28_guard_06_loop[], pl28_guard_07[], pl28_stand_p_00[], pl28_stand_p_01[], pl28_stand_p_02[],
	pl28_stand_k_00[], pl28_stand_k_01[], pl28_stand_k_02[], pl28_air_guard_00[],
	pl28_air_guard_01[], pl28_air_guard_02[], pl28_crouch_p_00[], pl28_crouch_p_01[],
	pl28_crouch_p_02[], pl28_extra_05[], pl28_crouch_k_00[], pl28_crouch_k_01[], pl28_crouch_k_02[],
	pl28_jump_p_00[], pl28_jump_p_01[], pl28_jump_p_02[], pl28_jump_k_00[], pl28_jump_k_01[],
	pl28_jump_k_02[], pl28_get_up_00[], pl28_get_up_01[], pl28_get_up_02[], pl28_get_up_03[],
	pl28_appear_00[], pl28_appear_01[], pl28_appear_02[], pl28_appear_03[], pl28_win_00[],
	pl28_win_00_loop[], pl28_win_01[], pl28_win_01_loop[], pl28_win_02[], pl28_win_02_loop[],
	pl28_win_03[], pl28_win_03_loop[], pl28_damage_00[], pl28_damage_01[], pl28_damage_02[],
	pl28_damage_03[], pl28_damage_04[], pl28_damage_05[], pl28_damage_06[], pl28_damage_07[],
	pl28_damage_08[], pl28_damage_10[], pl28_damage_11[], pl28_damage_12[], pl28_damage_13[],
	pl28_damage_09[], pl28_damage_09_loop[], pl28_damage_16[], pl28_damage_17[], pl28_damage_18[],
	pl28_damage_19[], pl28_damage_20[], pl28_damage_21[], pl28_damage_22[], pl28_damage_23[],
	pl28_damage_24[], pl28_damage_25[], pl28_damage_26[], pl28_unused_00[], pl28_damage_27[],
	pl28_damage_32[], pl28_damage_33[], pl28_damage_28[], pl28_damage_29[], pl28_damage_31[],
	pl28_dizzy_00[], pl28_catch_00[], pl28_catch_01[], pl28_catch_01_loop[], pl28_catch_02[],
	pl28_catch_03[], pl28_catch_03_loop[], pl28_catch_04[], pl28_catch_04_loop[], pl28_caught_00[],
	pl28_caught_01[], pl28_caught_02[], pl28_caught_03[], pl28_caught_04[], pl28_caught_05[],
	pl28_caught_06[], pl28_caught_07[], pl28_caught_08[], pl28_caught_09[], pl28_caught_10[],
	pl28_caught_11[], pl28_caught_12[], pl28_caught_13[], pl28_caught_14[], pl28_caught_15[],
	pl28_caught_16[], pl28_caught_17[], pl28_caught_18[], pl28_caught_19[], pl28_caught_20[],
	pl28_caught_21[], pl28_change_00[], pl28_change_02[], pl28_change_02_loop[], pl28_change_03[],
	pl28_change_03_loop[], pl28_change_04[], pl28_change_04_loop[], pl28_change_01[],
	pl28_extra_02[], pl28_extra_03[], pl28_special_00[], pl28_special_01[], pl28_effect_00[],
	pl28_special_03[], pl28_special_04[], pl28_effect_01[], pl28_special_06[], pl28_special_07[],
	pl28_effect_02[], pl28_special_09[], pl28_special_12[], pl28_special_13[], pl28_special_14[],
	pl28_special_15[], pl28_special_18[], pl28_super_06[], pl28_super_06_loop[], pl28_super_15[],
	pl28_super_15_loop[], pl28_special_16[], pl28_special_16_loop[], pl28_special_17[],
	pl28_special_19[], pl28_special_19_loop[], pl28_special_20[], pl28_special_22[], pl28_super_00[],
	pl28_super_16_loop[], pl28_super_16_loop_loop[], pl28_super_16[], pl28_super_01[],
	pl28_super_02[], pl28_super_03[], pl28_super_04[], pl28_super_05[], pl28_super_05_loop[],
	pl28_super_17[], pl28_super_07[], pl28_super_17_loop[], pl28_super_18[], pl28_super_14[],
	pl28_super_18_loop[], pl28_super_08[], pl28_super_09[], pl28_super_10[], pl28_super_11[],
	pl28_super_12[], pl28_super_12_loop[], pl28_super_13[], pl28_super_13_loop[], pl28_effect_03[],
	pl28_effect_04[], pl28_effect_05[], pl28_effect_06[], pl28_extra_00[], pl28_extra_01[],
	pl28_extra_04[], pl28_effect_07[], pl28_effect_08[], pl28_effect_09[], pl28_effect_10[],
	pl28_effect_11[], pl28_effect_12[], pl28_effect_13[], pl28_effect_14[], pl28_effect_15[],
	pl28_effect_16[], pl28_effect_17[], pl28_effect_18[], pl28_effect_19[], pl28_effect_20[],
	pl28_effect_20_loop[], pl28_effect_21[], pl28_effect_21_loop[];
extern const Sint32 *const pl28_stand_tbl[], *const pl28_jump_tbl[], *const pl28_dash_tbl[],
	*const pl28_crouch_tbl[], *const pl28_turn_tbl[], *const pl28_guard_tbl[],
	*const pl28_air_guard_tbl[], *const pl28_stand_p_tbl[], *const pl28_stand_k_tbl[],
	*const pl28_crouch_p_tbl[], *const pl28_crouch_k_tbl[], *const pl28_jump_p_tbl[],
	*const pl28_jump_k_tbl[], *const pl28_damage_tbl[], *const pl28_dizzy_tbl[],
	*const pl28_catch_tbl[], *const pl28_caught_tbl[], *const pl28_get_up_tbl[],
	*const pl28_appear_tbl[], *const pl28_win_tbl[], *const pl28_extra_tbl[],
	*const pl28_special_tbl[], *const pl28_super_tbl[], *const pl28_effect_tbl[],
	*const pl28_change_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_28[16] = {
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
const Sint32 *const *const char_tbl_28[] = {
	pl28_stand_tbl, pl28_jump_tbl, pl28_dash_tbl, pl28_crouch_tbl, pl28_turn_tbl, pl28_guard_tbl,
	pl28_air_guard_tbl, pl28_stand_p_tbl, pl28_stand_k_tbl, pl28_crouch_p_tbl, pl28_crouch_k_tbl, pl28_jump_p_tbl,
	pl28_jump_k_tbl, pl28_damage_tbl, pl28_dizzy_tbl, pl28_catch_tbl, pl28_caught_tbl, pl28_get_up_tbl,
	pl28_appear_tbl, pl28_win_tbl, pl28_extra_tbl, pl28_special_tbl, pl28_super_tbl, pl28_effect_tbl,
	pl28_change_tbl, pl28_change_tbl, 0,
};

const Sint32 *const pl28_stand_tbl[] = {
	pl28_stand_00, pl28_stand_00, pl28_stand_02, pl28_stand_03, pl28_stand_00, pl28_stand_00,
	pl28_stand_06, pl28_stand_07, pl28_stand_06, pl28_stand_07, 0,
};

const Sint32 *const pl28_jump_tbl[] = {
	pl28_jump_00, pl28_jump_01, pl28_jump_02, pl28_jump_03, pl28_jump_04, pl28_jump_04,
	pl28_jump_06, pl28_jump_07, pl28_jump_08, pl28_jump_09, pl28_jump_09, pl28_jump_09,
	0,
};

const Sint32 *const pl28_dash_tbl[] = {
	pl28_dash_00, pl28_dash_01, pl28_dash_02, pl28_dash_03, 0,
};

const Sint32 *const pl28_crouch_tbl[] = {
	pl28_crouch_00, pl28_crouch_01, pl28_crouch_02, 0,
};

const Sint32 *const pl28_turn_tbl[] = {
	pl28_turn_00, pl28_turn_01, 0,
};

const Sint32 *const pl28_guard_tbl[] = {
	pl28_guard_00, pl28_guard_01, pl28_guard_02, pl28_guard_03, pl28_guard_04, pl28_guard_05,
	pl28_guard_06, pl28_guard_07, 0,
};

const Sint32 *const pl28_air_guard_tbl[] = {
	pl28_air_guard_00, pl28_air_guard_01, pl28_air_guard_02, 0,
};

const Sint32 *const pl28_stand_p_tbl[] = {
	pl28_stand_p_00, pl28_stand_p_01, pl28_stand_p_02, 0,
};

const Sint32 *const pl28_stand_k_tbl[] = {
	pl28_stand_k_00, pl28_stand_k_01, pl28_stand_k_02, 0,
};

const Sint32 *const pl28_crouch_p_tbl[] = {
	pl28_crouch_p_00, pl28_crouch_p_01, pl28_crouch_p_02, 0,
};

const Sint32 *const pl28_crouch_k_tbl[] = {
	pl28_crouch_k_00, pl28_crouch_k_01, pl28_crouch_k_02, 0,
};

const Sint32 *const pl28_jump_p_tbl[] = {
	pl28_jump_p_00, pl28_jump_p_01, pl28_jump_p_02, pl28_jump_p_00, pl28_jump_p_01, pl28_jump_p_02,
	0,
};

const Sint32 *const pl28_jump_k_tbl[] = {
	pl28_jump_k_00, pl28_jump_k_01, pl28_jump_k_02, pl28_jump_k_00, pl28_jump_k_01, pl28_jump_k_02,
	0,
};

const Sint32 *const pl28_damage_tbl[] = {
	pl28_damage_00, pl28_damage_01, pl28_damage_02, pl28_damage_03, pl28_damage_04, pl28_damage_05,
	pl28_damage_06, pl28_damage_07, pl28_damage_08, pl28_damage_09, pl28_damage_10, pl28_damage_11,
	pl28_damage_12, pl28_damage_13, pl28_damage_13, pl28_damage_13, pl28_damage_16, pl28_damage_17,
	pl28_damage_18, pl28_damage_19, pl28_damage_20, pl28_damage_21, pl28_damage_22, pl28_damage_23,
	pl28_damage_24, pl28_damage_25, pl28_damage_26, pl28_damage_27, pl28_damage_28, pl28_damage_29,
	pl28_damage_27, pl28_damage_31, pl28_damage_32, pl28_damage_33, pl28_damage_13, 0,
};

const Sint32 *const pl28_dizzy_tbl[] = {
	pl28_dizzy_00, 0,
};

const Sint32 *const pl28_catch_tbl[] = {
	pl28_catch_00, pl28_catch_01, pl28_catch_02, pl28_catch_03, pl28_catch_04, 0,
};

const Sint32 *const pl28_caught_tbl[] = {
	pl28_caught_00, pl28_caught_01, pl28_caught_02, pl28_caught_03, pl28_caught_04, pl28_caught_05,
	pl28_caught_06, pl28_caught_07, pl28_caught_08, pl28_caught_09, pl28_caught_10, pl28_caught_11,
	pl28_caught_12, pl28_caught_13, pl28_caught_14, pl28_caught_15, pl28_caught_16, pl28_caught_17,
	pl28_caught_18, pl28_caught_19, pl28_caught_20, pl28_caught_21, 0,
};

const Sint32 *const pl28_get_up_tbl[] = {
	pl28_get_up_00, pl28_get_up_01, pl28_get_up_02, pl28_get_up_03, 0,
};

const Sint32 *const pl28_appear_tbl[] = {
	pl28_appear_00, pl28_appear_01, pl28_appear_02, pl28_appear_03, 0,
};

const Sint32 *const pl28_win_tbl[] = {
	pl28_win_00, pl28_win_01, pl28_win_02, pl28_win_03, 0,
};

const Sint32 *const pl28_extra_tbl[] = {
	pl28_extra_00, pl28_extra_01, pl28_extra_02, pl28_extra_03, pl28_extra_04, pl28_extra_05,
	0,
};

const Sint32 *const pl28_special_tbl[] = {
	pl28_special_00, pl28_special_01, pl28_special_01, pl28_special_03, pl28_special_04, pl28_special_04,
	pl28_special_06, pl28_special_07, pl28_special_07, pl28_special_09, pl28_special_09, pl28_special_09,
	pl28_special_12, pl28_special_13, pl28_special_14, pl28_special_15, pl28_special_16, pl28_special_17,
	pl28_special_18, pl28_special_19, pl28_special_20, pl28_crouch_p_02, pl28_special_22, 0,
};

const Sint32 *const pl28_super_tbl[] = {
	pl28_super_00, pl28_super_01, pl28_super_02, pl28_super_03, pl28_super_04, pl28_super_05,
	pl28_super_06, pl28_super_07, pl28_super_08, pl28_super_09, pl28_super_10, pl28_super_11,
	pl28_super_12, pl28_super_13, pl28_super_14, pl28_super_15, pl28_super_16, pl28_super_17,
	pl28_super_18, 0,
};

const Sint32 *const pl28_effect_tbl[] = {
	pl28_effect_00, pl28_effect_01, pl28_effect_02, pl28_effect_03, pl28_effect_04, pl28_effect_05,
	pl28_effect_06, pl28_effect_07, pl28_effect_08, pl28_effect_09, pl28_effect_10, pl28_effect_11,
	pl28_effect_12, pl28_effect_13, pl28_effect_14, pl28_effect_15, pl28_effect_16, pl28_effect_17,
	pl28_effect_18, pl28_effect_19, pl28_effect_20, pl28_effect_21, pl28_effect_05, 0,
};

const Sint32 *const pl28_change_tbl[] = {
	pl28_change_00, pl28_change_01, pl28_change_02, pl28_change_03, pl28_change_04, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_end, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl28_stand_00[] = {
	CG_FRAME(    0,    0,    3,           0,    48, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    49, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    50, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    51, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    52, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    54, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    55, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    56, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    57, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    58, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    59, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    60, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    61, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    62, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    64, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    65, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    66, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    67, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    69, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    70, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    71, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl28_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    73, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    74, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    75, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    77, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    84, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    87, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    88, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    89, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    90, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    92, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_stand_02 ),
};

/* Char_Stand 6
   Char_Stand 8 */
const Sint32 pl28_stand_06[] = {
	CG_FRAME(    0,    0,    4,           0,    74, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    75, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    76, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_stand_06 ),
};

/* Char_Stand 3 */
const Sint32 pl28_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,    93, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    94, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    95, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    96, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    97, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    98, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    99, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   100, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   101, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   102, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   103, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   104, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   105, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   106, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   107, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   108, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   109, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   110, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   111, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   112, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_stand_03 ),
};

/* Char_Stand 7
   Char_Stand 9 */
const Sint32 pl28_stand_07[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl28_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   156, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   132, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   133, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl28_jump_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   136, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   137, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl28_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl28_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl28_jump_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   147, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   148, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl28_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl28_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl28_jump_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   154, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   155, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl28_jump_02_loop ),
};

/* Char_Jump 3 */
const Sint32 pl28_jump_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   160, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_jump_03 ),
};

/* Char_Jump 4: strike_land_set (Cajun Strike), escape_jump (Cajun Escape), escape_dive (Cajun Escape), explosion_jump
   Char_Jump 5 */
const Sint32 pl28_jump_04[] = {
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   150, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   157, 0x80,   0x0c,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x0c,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x0e,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_jump_04 ),
};

/* Char_Jump 6 */
const Sint32 pl28_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   156, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl28_jump_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   218, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl28_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl28_jump_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   147, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   220, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl28_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl28_jump_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   154, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   222, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_jump_08_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl28_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   218, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl28_dash_00[] = {
	CG_FRAME(    0,    1,    1,           0,   113, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   114, 0x80,   0x11,    0,  0x00, 0x05,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   115, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_dash_00_loop ),
};

/* Char_Dash 2: dash_jump */
const Sint32 pl28_dash_02[] = {
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x16,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x14,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   118, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl28_dash_01[] = {
	CG_FRAME(    0,    1,    1,           0,   113, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x14,    0,  0x00, 0x03,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_dash_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x14,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   121, 0x80,   0x14,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_dash_01_loop ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl28_dash_03[] = {
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x08,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x08,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x08,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x0b,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x0d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   127, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl28_crouch_00[] = {
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   157, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   158, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl28_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    72, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl28_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   159, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   156, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl28_turn_00[] = {
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   163, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl28_turn_01[] = {
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl28_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   167, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl28_guard_01[] = {
	CG_FRAME(    0,    0,    2,           0,   168, 0x80,   0x13,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   169, 0x80,   0x13,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl28_guard_02[] = {
	CG_FRAME(    0,    0,    2,           0,   170, 0x80,   0x13,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_guard_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   167, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_guard_02_loop ),
};

/* Char_Guard 3 */
const Sint32 pl28_guard_03[] = {
	CG_FRAME( 0x80,    0,    2,           0,   168, 0x80,   0x13,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    2, END_OF_DATA,   169, 0x80,   0x13,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl28_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   171, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl28_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   172, 0x80,   0x4f,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl28_guard_06[] = {
	CG_FRAME(    0,    0,    2,           0,   173, 0x80,   0x4f,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl28_guard_06_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   171, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_guard_06_loop ),
};

/* Char_Guard 7 */
const Sint32 pl28_guard_07[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   172, 0x80,   0x4f,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl28_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   224, 0x80,   0x0f,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   225, 0x80,   0x0f,  ATK,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   226, 0x80,   0x0f,  ATE,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl28_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl28_stand_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   227, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   228, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   229, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    1,           0,   229, 0x80,   0x00,  ATK,  0x00, 0x20,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    2,           0,   230, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    8,           0,   231, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    5,           0,   232, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   233, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl28_stand_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   235, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   236, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   237, 0x80,   0x01,  ATK,  0x00, 0x06,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    1,           0,   237, 0x80,   0x01,  ATK,  0x00, 0x20,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    2,           0,   238, 0x80,   0x01,  ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    2,           0,   239, 0x80,   0x01,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   15,           0,   240, 0x80,   0x01,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   241, 0x80,   0x04,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   242, 0x80,   0x08,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   233, 0x80,   0x0d,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl28_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0x0e,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    3,           0,   244, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   244, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   245, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl28_stand_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   246, 0x80,   0x0e,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    1,           0,   243, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x81,    0,    4,           0,   247, 0x80,   0x08,  ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    5,           0,   247, 0x80,   0x08,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   245, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl28_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl28_stand_k_02[] = {
	CG_FRAME(    0,    0,    6,           0,   248, 0x70,   0x0d,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   249, 0x70,   0x0a,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    1,           0,   250, 0x80,   0x01,  ATK,  0x00, 0x03,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x01,  ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    6,           0,   251, 0x80,   0x01,  ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   252, 0x80,   0x01,  ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   253, 0x80,   0x0a,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl28_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   174, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl28_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   175, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl28_air_guard_02[] = {
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   176, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_air_guard_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl28_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   254, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    5,           0,   255, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   256, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl28_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl28_crouch_p_01[] = {
	CG_FRAME(    0,    0,    6,           0,   257, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    1,           0,   258, 0x80,   0x4d,  ATK,  0x00, 0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   258, 0x80,   0x4d,  ATK,  0x00, 0x20,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,    2,           0,   259, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   10,           0,   260, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x82,    0,    4,           0,   261, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   262, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl28_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy)
   Char_Special 21: crouch_hp_init (crouching heavy punch launcher: the gamma assist and the gamma variable counter) */
const Sint32 pl28_crouch_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   507, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   263, 0x80,   0x4a,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    1,           0,   264, 0x80,   0x07,  ATK,  0x00, 0x05,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,    2,           0,   264, 0x80,   0x07,  ATK,  0x00, 0x20,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,    3,           0,   265, 0x80,   0x07,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   10,           0,   266, 0x80,   0x07,  ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    7,           0,   267, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   268, 0x80,   0x0a,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_crouch_p_02 ),
};

/* Char_Extra 5: pl28_upper */
const Sint32 pl28_extra_05[] = {
	CG_FRAME(    0,    0,    3,           0,   507, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   263, 0x80,   0x4a,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    1,           0,   264, 0x80,   0x07,  ATK,  0x00, 0x05,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x80,    0,    2,           0,   264, 0x80,   0x07,  ATK,  0x00, 0x20,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x80,    0,    3,           0,   265, 0x80,   0x07,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   266, 0x80,   0x07,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   267, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   268, 0x80,   0x0a,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl28_crouch_k_00[] = {
	CG_FRAME(    0,    0,    5,           0,   269, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   270, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,    2,           0,   271, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    7,           0,   271, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   272, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl28_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl28_crouch_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   273, 0x80,   0x48,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x45,  ATK,  0x00, 0x03,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    6,           0,   274, 0x80,   0x45,  ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    6,           0,   275, 0x80,   0x48,  ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    5,           0,   276, 0x80,   0x4c,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   277, 0x80,   0x4d,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl28_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl28_crouch_k_02[] = {
	CG_FRAME(    0,    0,    7,           0,   278, 0x80,   0x4a,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   279, 0x80,   0x4a,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   280, 0x90,   0x44,  ATK,  0x00, 0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x44,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x44,  ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   10,           0,   282, 0x90,   0x46,  ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    7,           0,   283, 0x90,   0x4a,  ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   277, 0x80,   0x4c,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl28_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl28_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   284, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    7,           0,   285, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   285, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   286, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl28_jump_p_01[] = {
	CG_FRAME(    0,    0,    7,           0,   287, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    1,           0,   288, 0x80,   0x20,  ATK,  0x00, 0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   288, 0x80,   0x20,  ATK,  0x00, 0x20,    0,   0,     0,      0,  88 ),
	CG_FRAME( 0x80,    0,    2,           0,   289, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,    2,           0,   290, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   290, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    8,           0,   291, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   292, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl28_jump_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    8,           0,   294, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   295, 0x80,   0x20,  ATK,  0x00, 0x06,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,    1,           0,   295, 0x80,   0x20,  ATK,  0x00, 0x20,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,    2,           0,   296, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   12,           0,   297, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    4,           0,   298, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   299, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl28_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   300, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   301, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    6,           0,   302, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    6,           0,   302, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   303, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl28_jump_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   304, 0x90,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x90,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   306, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,    3,           0,   307, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,    2,           0,   307, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    9,           0,   307, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   308, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl28_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl28_jump_k_02[] = {
	CG_FRAME(    0,    0,    8,           0,   309, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   310, 0x80,   0x20,  ATK,  0x00, 0x04,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    2,           0,   312, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    4,           0,   311, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    8,           0,   311, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   308, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl28_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl28_get_up_00[] = {
	CG_FRAME(    0,    0,    8,           0,   505, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   506, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   507, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   508, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   509, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl28_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   505, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   506, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   205, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   206, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   207, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   507, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   508, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   509, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl28_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   505, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   506, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   208, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   205, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   208, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   507, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   508, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   509, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl28_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   505, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   506, 0x80,   0x4e,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   208, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   207, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   205, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   507, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   508, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   509, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl28_appear_00[] = {
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_appear_00 ),
};

/* Char_Appear 1: appear_jump */
const Sint32 pl28_appear_01[] = {
	CG_FRAME(    0,    0,    3,           0,   147, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   148, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_appear_01 ),
};

/* Char_Appear 2 */
const Sint32 pl28_appear_02[] = {
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x68,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x68,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   508, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   509, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   431, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   431, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_appear_02 ),
};

/* Char_Appear 3: appear_jump */
const Sint32 pl28_appear_03[] = {
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x68,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x68,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   441, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   507, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00, 0x0b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   431, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   431, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_appear_03 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl28_win_00[] = {
	CG_FRAME(    0,    0,    1,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x00,    0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_win_00_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl28_win_01[] = {
	CG_FRAME(    0,    0,    1,           0,   182, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x00,    0,  0x00, 0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   185, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x02,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x02,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x02,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x02,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x02,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x02,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_win_01_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   197, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_win_01_loop ),
};

/* Char_Win 2: win_time_init */
const Sint32 pl28_win_02[] = {
	CG_FRAME(    0,    0,    1,           0,   198, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   198, 0x80,   0x0a,    0,  0x00, 0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   199, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   200, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   20,           0,   201, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    0,           0,   202, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    0,           0,   203, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_win_02_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   199, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_win_02_loop ),
};

/* Char_Win 3: lose_init */
const Sint32 pl28_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   510, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_win_03_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_win_03_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl28_damage_00[] = {
	CG_FRAME(    0,    0,    6,           0,   452, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   457, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl28_damage_01[] = {
	CG_FRAME(    0,    0,   10,           0,   453, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   456, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   457, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl28_damage_02[] = {
	CG_FRAME(    0,    0,   12,           0,   454, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   455, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   456, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   457, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl28_damage_03[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   458, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl28_damage_04[] = {
	CG_FRAME(    0,    0,   10,           0,   459, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   461, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   156, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl28_damage_05[] = {
	CG_FRAME(    0,    0,    2,           0,   459, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   460, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   459, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   461, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   156, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl28_damage_06[] = {
	CG_FRAME(    0,    0,    6,           0,   462, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   463, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl28_damage_07[] = {
	CG_FRAME(    0,    0,   10,           0,   464, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   465, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   463, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl28_damage_08[] = {
	CG_FRAME(    0,    0,   12,           0,   466, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   467, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   468, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   465, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   463, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl28_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl28_damage_10[] = {
	CG_FRAME( 0x80,    0,   10, END_OF_DATA,   469, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl28_damage_11[] = {
	CG_FRAME( 0x80,    0,   10, END_OF_DATA,   470, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl28_damage_12[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   471, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl28_damage_13[] = {
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x8b,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x8b,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   492, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl28_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl28_damage_09[] = {
	CG_FRAME(    0,    0,    6,           0,   475, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   476, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   477, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   478, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   490, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   491, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl28_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   492, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl28_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl28_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   479, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   480, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl28_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   481, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl28_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   482, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl28_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   483, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl28_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   484, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl28_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   479, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   480, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl28_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   481, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl28_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   482, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl28_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   483, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl28_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   484, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl28_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   488, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   489, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   490, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   491, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   492, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   492, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl28_damage_26 ),
};

const Sint32 pl28_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   125, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   124, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   122, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   126, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   127, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	0x00002f0c,
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl28_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_27 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl28_damage_32[] = {
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl28_damage_33[] = {
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl28_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   495, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   497, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   498, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl28_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   499, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   502, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   503, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   504, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl28_damage_31[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   492, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl28_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl28_dizzy_00[] = {
	CG_FRAME(    0,    0,    5,           0,   510, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    5,           0,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    5,           0,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_NEXT( pl28_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl28_catch_00[] = {
	CG_FRAME(    0,    0,    1,           0,   313, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    3,           0,   313, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x00,    0,  0x00, 0x1a,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    1,    0,    2,           0,   316, 0x80,   0x00,    0,  0x01, 0x1d,    0,   0,     0, 0x0203,   0 ),
	CG_FRAME(    0,    0,    2,           0,   316, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0404,   0 ),
	CG_FRAME(    1,    0,    2,           0,   317, 0x80,   0x00,    0,  0x02, 0x1d,    0,   0,     0, 0x0603,   0 ),
	CG_FRAME(    0,    0,    2,           0,   317, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0804,   0 ),
	CG_FRAME(    1,    0,    2,           0,   318, 0x80,   0x00,    0,  0x03, 0x1d,    0,   0,     0, 0x0a03,   0 ),
	CG_FRAME(    0,    0,    2,           0,   318, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c04,   0 ),
	CG_FRAME(    1,    0,    2,           0,   319, 0x80,   0x00,    0,  0x01, 0x1d,    0,   0,     0, 0x0e03,   0 ),
	CG_FRAME(    0,    0,    2,           0,   319, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1004,   0 ),
	CG_FRAME(    1,    0,    2,           0,   316, 0x80,   0x00,    0,  0x02, 0x1d,    0,   0,     0, 0x0203,   0 ),
	CG_FRAME(    0,    0,    2,           0,   316, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0404,   0 ),
	CG_FRAME(    1,    0,    2,           0,   317, 0x80,   0x00,    0,  0x03, 0x1d,    0,   0,     0, 0x0603,   0 ),
	CG_FRAME(    0,    0,    2,           0,   317, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0804,   0 ),
	CG_FRAME(    1,    0,    2,           0,   318, 0x80,   0x00,    0,  0x01, 0x1d,    0,   0,     0, 0x0a03,   0 ),
	CG_FRAME(    0,    0,    2,           0,   318, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c04,   0 ),
	CG_FRAME(    1,    0,    2,           0,   319, 0x80,   0x00,    0,  0x02, 0x1d,    0,   0,     0, 0x0e03,   0 ),
	CG_FRAME(    0,    0,    2,           0,   319, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0xfe05,   0 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,   -1,    2,           0,   322, 0x80,   0x00,    0,  0x00, 0x05,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   10,           0,   323, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   324, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   325, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   326, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_catch_00 ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl28_catch_01[] = {
	CG_FRAME(    0,    0,    8,           0,   327, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    8,           0,   328, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0009,   0 ),
};

const Sint32 pl28_catch_01_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   329, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000a,   0 ),
	CG_NEXT( pl28_catch_01_loop ),
};

/* Char_Catch 2: nml_k_land */
const Sint32 pl28_catch_02[] = {
	CG_FRAME(    0,    0,    8,           0,   330, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,   -1,   10,           0,   331, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    8,           0,   330, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,   -1,   10,           0,   331, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    8,           0,   330, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,   -1,   10,           0,   331, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    8,           0,   330, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,   -1,   10,           0,   331, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    8,           0,   330, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,   -1,   10,           0,   331, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   332, 0x80,   0x00,    0,  0x01,    0,    0,   0,     0, 0x000b,   0 ),
	CG_NEXT( pl28_catch_02 ),
};

/* Char_Catch 3: nml_k_throw */
const Sint32 pl28_catch_03[] = {
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00, 0x06,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl28_catch_03_loop[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   154, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_catch_03_loop ),
};

/* Char_Catch 4: jump_k_set */
const Sint32 pl28_catch_04[] = {
	CG_FRAME(    0,    0,   12,           0,   328, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,    6,           0,   397, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    1,   10,           0,   398, 0x80,   0x20,    0,  0x00, 0x04,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,    6,           0,   399, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl28_catch_04_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   154, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   155, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_catch_04_loop ),
};

/* Char_Caught 0 */
const Sint32 pl28_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   452, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl28_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   453, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl28_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   454, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl28_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   461, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl28_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   459, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl28_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   460, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl28_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   462, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl28_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl28_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   466, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl28_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   458, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl28_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   459, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl28_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   460, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl28_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl28_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   472, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl28_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl28_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   480, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl28_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl28_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   481, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl28_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl28_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl28_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl28_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   483, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl28_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl28_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   484, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl28_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl28_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   498, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl28_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl28_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   504, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl28_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl28_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl28_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl28_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl28_caught_21 ),
};

/* Char_Change 0: act_change_tag_init
   group 25 0 */
const Sint32 pl28_change_00[] = {
	CG_FRAME(    0,    0,    8,           0,   163, 0x30,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   198, 0x30,   0x20,  ATK,  0x00, 0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    8, END_OF_DATA,   198, 0x30,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_change_00 ),
};

/* Char_Change 2: act_change_out_bound
   group 25 2 */
const Sint32 pl28_change_02[] = {
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl28_change_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl28_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call
   group 25 3 */
const Sint32 pl28_change_03[] = {
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x00,    0,  0x00, 0x0c,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    1,    3,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0400,   1 ),
};

const Sint32 pl28_change_03_loop[] = {
	CG_FRAME(    0,    0,   19, END_OF_DATA,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0xfe00,   1 ),
	CG_NEXT( pl28_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end
   group 25 4 */
const Sint32 pl28_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   431, 0x80,   0x00,    0,  0x00, 0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   431, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_change_04_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_change_04_loop ),
};

/* Char_Change 1: act_change_vcounter_init
   group 25 1 */
const Sint32 pl28_change_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   426, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   427, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    0,           0,   428, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    0,           0,   429, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0, -128, END_OF_DATA,   430, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl28_change_01 ),
};

/* Char_Extra 2: t_kick_init, sp_t_kick_init, fly_start5 */
const Sint32 pl28_extra_02[] = {
	CG_FRAME(    0,    0,    2,           0,   309, 0xa0,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   310, 0xa0,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME( 0x80,    0,   48,           0,   311, 0xa0,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME( 0x82,    0,  127, END_OF_DATA,   312, 0xa0,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl28_extra_02 ),
};

/* Char_Extra 3: t_kick_jump, sp_t_kick_jump, fly_start5 */
const Sint32 pl28_extra_03[] = {
	CG_FRAME(    0,    0,    2,           0,   157, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,  ATE,  0x00, 0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   160, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   179, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   180, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   431, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   432, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   431, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   432, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   11, END_OF_DATA,   431, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_extra_03 ),
};

/* Char_Special 0: card_init */
const Sint32 pl28_special_00[] = {
	CG_FRAME(    0,    0,    6,           0,   376, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   363, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,   10,           0,   364, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   5 ),
	CG_FRAME(    0,    0,    1,           0,   365, 0x80,   0x0e,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(    0,    1,    1,           0,   366, 0x80,   0x0c,  ATK,  0x00, 0x14,    0,   0,     0, 0x0800,  52 ),
	CG_FRAME(    0,    0,    1,           0,   366, 0x80,   0x0c,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0800,  52 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0a00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0c00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0e00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1000,  52 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1200,  52 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1400,  52 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1600,  52 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0x1800,  52 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0xfe00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   374, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   375, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_00 ),
};

/* Char_Special 1
   Char_Special 2: card_init */
const Sint32 pl28_special_01[] = {
	CG_FRAME(    0,    0,    6,           0,   376, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   363, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,   18,           0,   364, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   5 ),
	CG_FRAME(    0,    0,    1,           0,   365, 0x80,   0x0e,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(    0,    1,    1,           0,   366, 0x80,   0x0c,  ATK,  0x00, 0x14,    0,   0,     0, 0x0800,  52 ),
	CG_FRAME(    0,    0,    1,           0,   366, 0x80,   0x0c,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0800,  52 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0a00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0c00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0e00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1000,  52 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1200,  52 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1400,  52 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1600,  52 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0x1800,  52 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0xfe00,  52 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   374, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   375, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_01 ),
};

/* Char_Effect 0 */
const Sint32 pl28_effect_00[] = {
	CG_FRAME(    0,    0,    1,           0,   522, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   523, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   524, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   525, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   526, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   527, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   528, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   529, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1000,   0 ),
	CG_FRAME(    0,    0,    1,           0,   530, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   531, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   532, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1600,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   533, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1800,   0 ),
	CG_NEXT( pl28_effect_00 ),
};

/* Char_Special 3: card_init */
const Sint32 pl28_special_03[] = {
	CG_FRAME(    0,    0,    4,           0,   389, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,    8,           0,   390, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   7 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0800,  53 ),
	CG_FRAME(    0,    0,    8,           0,   393, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0a00,  53 ),
	CG_FRAME(    0,    1,    1,           0,   394, 0x80,   0x20,  ATK,  0x00, 0x14,    0,   0,     0, 0x0c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00, 0x1e,    0,   0,     0, 0x0e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1000,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1200,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1400,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1600,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1800,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1a00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x2000,  53 ),
	CG_FRAME(    0,    0,   20,           0,   395, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   396, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl28_special_03 ),
};

/* Char_Special 4
   Char_Special 5: card_init */
const Sint32 pl28_special_04[] = {
	CG_FRAME(    0,    0,    6,           0,   389, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,   10,           0,   390, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   7 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0800,  53 ),
	CG_FRAME(    0,    0,   10,           0,   393, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0a00,  53 ),
	CG_FRAME(    0,    1,    1,           0,   394, 0x80,   0x20,  ATK,  0x00, 0x14,    0,   0,     0, 0x0c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00, 0x1e,    0,   0,     0, 0x0e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1000,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1200,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1400,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1600,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1800,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1a00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x1e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0, 0x2000,  53 ),
	CG_FRAME(    0,    0,   20,           0,   395, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   396, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl28_special_04 ),
};

/* Char_Effect 1 */
const Sint32 pl28_effect_01[] = {
	CG_FRAME(    0,    0,    1,           0,   585, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   586, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   587, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   588, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   589, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   590, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   591, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   592, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1000,   0 ),
	CG_FRAME(    0,    0,    1,           0,   593, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   594, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   595, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   596, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   597, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   599, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1e00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   600, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2000,   0 ),
	CG_NEXT( pl28_effect_01 ),
};

/* Char_Special 6 */
const Sint32 pl28_special_06[] = {
	CG_FRAME(    0,    0,    6,           0,   376, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   377, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   379, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,   10,           0,   380, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   5 ),
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x0e,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(    0,    1,    2,           0,   382, 0x80,   0x0c,  ATK,  0x00, 0x15,    0,   0,     0, 0x0800,   5 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x0c,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0a00,  54 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0c00,  54 ),
	CG_FRAME(    0,    0,    2,           0,   385, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0e00,  54 ),
	CG_FRAME(    0,    0,    2,           0,   386, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1000,  54 ),
	CG_FRAME(    0,    0,    2,           0,   386, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1200,  54 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1400,  54 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1600,  54 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0x1800,  54 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0xfe00,  54 ),
	CG_FRAME(    0,    0,    4,           0,   373, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   375, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_06 ),
};

/* Char_Special 7
   Char_Special 8 */
const Sint32 pl28_special_07[] = {
	CG_FRAME(    0,    0,    6,           0,   376, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   377, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   379, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,   18,           0,   380, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   5 ),
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x0e,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   5 ),
	CG_FRAME(    0,    1,    2,           0,   382, 0x80,   0x0c,  ATK,  0x00, 0x15,    0,   0,     0, 0x0800,   5 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x0c,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0a00,  54 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0c00,  54 ),
	CG_FRAME(    0,    0,    2,           0,   385, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x0e00,  54 ),
	CG_FRAME(    0,    0,    2,           0,   386, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1000,  54 ),
	CG_FRAME(    0,    0,    2,           0,   386, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1200,  54 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1400,  54 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0x80,   0x0c,  ATK,  0x00,    0,    0,   0,     0, 0x1600,  54 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0x1800,  54 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0x80,   0x0c,  ATE,  0x00,    0,    0,   0,     0, 0xfe00,  54 ),
	CG_FRAME(    0,    0,    4,           0,   373, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   374, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   375, 0x80,   0x0e,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_07 ),
};

/* Char_Effect 2 */
const Sint32 pl28_effect_02[] = {
	CG_FRAME(    0,    0,    1,           0,   534, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   535, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   536, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   537, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   538, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   539, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   540, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   541, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1000,   0 ),
	CG_FRAME(    0,    0,    1,           0,   542, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   543, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   544, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1600,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   545, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1800,   0 ),
	CG_NEXT( pl28_effect_02 ),
};

/* Char_Special 9
   Char_Special 10: slash_init
   Char_Special 11 */
const Sint32 pl28_special_09[] = {
	CG_FRAME(    0,    1,    2,           0,   156, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   132, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   133, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   293, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   294, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   295, 0x80,   0x20,  ATK,  0x00, 0x20,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    4,           0,   296, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    4,           0,   297, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x82,    0,    3,           0,   298, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   299, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl28_special_09 ),
};

/* Char_Special 12: slash_init */
const Sint32 pl28_special_12[] = {
	CG_FRAME(    0,    1,    7,           0,   333, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   334, 0x80,   0x91,  ATK,  0x00, 0x20,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x91,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   336, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    2,   20,           0,   337, 0x80,   0x16,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    3,    8, END_OF_DATA,   338, 0x80,   0x11,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl28_special_12 ),
};

/* Char_Special 13: slash_jump */
const Sint32 pl28_special_13[] = {
	CG_FRAME(    0,    1,    1,           0,   333, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   339, 0x80,   0x16,  ATK,  0x02,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   340, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x0f,  ATK,  0x00, 0x20,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    2,    2,           0,   342, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,    4,           0,   343, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x0e,  ATK,  0x03,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   345, 0x80,   0x0b,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME( 0x80,    2,    2,           0,   346, 0x80,   0x89,  ATK,  0x00, 0x20,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    2,           0,   347, 0x80,   0x89,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    3,   11,           0,   348, 0x80,   0x89,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   349, 0x80,   0x4b,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   350, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_13 ),
};

/* Char_Special 14: slash_init */
const Sint32 pl28_special_14[] = {
	CG_FRAME(    0,    1,    6,           0,   333, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,    2,           0,   340, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x80,    0,    1,           0,   341, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x80,    0,    2,           0,   341, 0x80,   0x00,  ATK,  0x00, 0x20,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x80,    3,    2,           0,   342, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    1,    6,           0,   343, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x16,  ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   352, 0x80,   0x4c,  ATK,  0x05,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x80,    2,    2,           0,   353, 0x80,   0x4c,  ATK,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    2,           0,   353, 0x80,   0x4c,  ATK,  0x00, 0x20,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    2,           0,   354, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    3,    6,           0,   355, 0x80,   0x4d,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   356, 0x80,   0x00,  ATK,  0x06,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   357, 0x80,   0x92,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,    4,           0,   357, 0x80,   0x92,  ATK,  0x00, 0x20,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,    4,           0,   358, 0x80,   0x92,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x81,    2,   26,           0,   359, 0x80,   0x92,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x0a,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,    2, END_OF_DATA,   361, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_14 ),
};

/* Char_Special 15: strike_jump_set */
const Sint32 pl28_special_15[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

/* Char_Special 18: strike_wall (Cajun Strike), escape_wall (Cajun Escape) */
const Sint32 pl28_special_18[] = {
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_special_18 ),
};

/* Char_Super 6 */
const Sint32 pl28_super_06[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl28_super_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_super_06_loop ),
};

/* Char_Super 15 */
const Sint32 pl28_super_15[] = {
	CG_FRAME(    0,    1,    6,           0,   139, 0x80,   0x0f,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl28_super_15_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   144, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   145, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   146, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_super_15_loop ),
};

/* Char_Special 16: escape_dive (Cajun Escape) */
const Sint32 pl28_special_16[] = {
	CG_FRAME(    0,    0,    3,           0,   433, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    1,           0,   434, 0x80,   0x20,  ATK,  0x00, 0x06,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    0,    2,           0,   434, 0x80,   0x20,  ATK,  0x00, 0x20,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x81,    0,    3,           0,   435, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
};

const Sint32 pl28_special_16_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   443, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   444, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   445, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   446, 0x80,   0x20,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_special_16_loop ),
};

/* Char_Special 17: strike_jump (Cajun Strike), escape_jump (Cajun Escape) */
const Sint32 pl28_special_17[] = {
	CG_FRAME(    0,    0,    4,           0,   436, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   437, 0x80,   0x20,    0,  0x00, 0x1d,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl28_special_17 ),
};

/* Char_Special 19: strike_wall (Cajun Strike) */
const Sint32 pl28_special_19[] = {
	CG_FRAME(    0,    0,    6,           0,   303, 0x80,   0x20,  ATK,  0x00, 0x17,    0,   0,     0,      0,  39 ),
};

const Sint32 pl28_special_19_loop[] = {
	CG_FRAME( 0x80,    0,    3,           0,   438, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x82,    0,    3, END_OF_DATA,   439, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl28_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl28_special_20[] = {
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3,           0,   440, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3,           0,   441, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3,           0,   442, 0x80,   0x0a,    0,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x0c,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   160, 0x80,   0x0e,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_20 ),
};

/* Char_Special 22: snapback_init (Snapback) */
const Sint32 pl28_special_22[] = {
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   235, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   236, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   237, 0x80,   0x01,  ATK,  0x00, 0x06,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    1,           0,   237, 0x80,   0x01,  ATK,  0x00, 0x20,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    2,           0,   238, 0x80,   0x01,  ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    2,           0,   239, 0x80,   0x01,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   15,           0,   240, 0x80,   0x01,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   241, 0x80,   0x04,  ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   242, 0x80,   0x08,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   233, 0x80,   0x0d,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl28_special_22 ),
};

/* Char_Super 0: royal_init (Royal Flush) */
const Sint32 pl28_super_00[] = {
	CG_FRAME(    0,    0,    4,           0,   400, 0x80,   0x52,  ATK,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   405, 0x80,   0x52,  ATK,  0xff,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   407, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   408, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   409, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_super_16_loop[] = {
	CG_FRAME(    0,    1,    1,           0,   409, 0x80,   0x52,  ATK,  0x00, 0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   409, 0x80,   0x52,  ATK,  0x00, 0x19,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_super_16_loop_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   410, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x80,   0x0a,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0400,   1 ),
	CG_FRAME(    0,    0,    3,           0,   412, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,    3,           0,   413, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0, 0x0800,   1 ),
	CG_FRAME(    0,    0,    3,           0,   414, 0x80,   0x0a,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,    3,           0,   415, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0, 0x0c00,   1 ),
	CG_FRAME(    0,    0,    3,           0,   416, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0, 0x0e00,   1 ),
	CG_FRAME(    0,    0,    1,           0,   417, 0x80,   0x0a,  ATK,  0x00, 0x1e,    0,   0,     0, 0x1000,   1 ),
	CG_FRAME(    0,    0,    1,           0,   418, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0, 0x1200,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   419, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_super_16_loop_loop ),
};

/* Char_Super 16 */
const Sint32 pl28_super_16[] = {
	CG_FRAME(    0,    0,    4,           0,   400, 0x80,   0x52,  ATK,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   405, 0x80,   0x52,  ATK,  0xff,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   407, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   408, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   14, END_OF_DATA,   409, 0x80,   0x52,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_super_16_loop ),
};

/* Char_Super 1: royal_move (Royal Flush) */
const Sint32 pl28_super_01[] = {
	CG_FRAME(    0,    0, -128,           0,   420, 0x80,   0x0c,  ATK,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x11,  ATK,  0xff,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   421, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   423, 0x80,   0x00,  ATE,  0x00, 0x21,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    5,           0,   423, 0x80,   0x00,  ATE,  0x00, 0x11,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    6,           0,   424, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0400,   1 ),
	CG_FRAME(    0,    0,   32, END_OF_DATA,   425, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0xfe00,   1 ),
	CG_NEXT( pl28_super_01 ),
};

/* Char_Super 2: shl28_00_t9_init */
const Sint32 pl28_super_02[] = {
	CG_FRAME(    0,    0,    1,           0,   402, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

/* Char_Super 3 */
const Sint32 pl28_super_03[] = {
	CG_FRAME(    0,    0,    4,           0,   403, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   404, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   401, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   402, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl28_super_03 ),
};

/* Char_Super 4: shl28_00_t9_fall */
const Sint32 pl28_super_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   404, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl28_super_05[] = {
	CG_FRAME(    0,    0,    1,           0,   182, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x20,    0,  0x00, 0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   185, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,    1,           0,   185, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl28_super_05_loop[] = {
	CG_FRAME( 0x80,    0,    2,           0,   186, 0x80,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   187, 0x80,   0x20,    0,  0x40,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x83,    0,    2, END_OF_DATA,   188, 0x80,   0x20,    0,  0x20,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_super_05_loop ),
};

/* Char_Super 17 */
const Sint32 pl28_super_17[] = {
	CG_FRAME(    0,    0,    2,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
	CG_NEXT( pl28_super_17_loop ),
};

/* Char_Super 7: explosion_jump */
const Sint32 pl28_super_07[] = {
	CG_FRAME(    0,    0,    2,           0,   389, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,    1,           0,   390, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
};

const Sint32 pl28_super_17_loop[] = {
	CG_FRAME(    0,    1,    1,           0,   390, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
	CG_FRAME(    0,    0,    0,           0,   391, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   7 ),
	CG_FRAME(    0,    0,    0,           0,   392, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0800,  53 ),
	CG_FRAME(    0,    0,    0,           0,   393, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x0a00,  53 ),
	CG_FRAME(    0,    1,    1,           0,   394, 0x80,   0x20,  ATK,  0x00, 0x14,    0,   0,     0, 0x0c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1000,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1200,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1400,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1600,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1800,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1a00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x1e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0, 0x2000,  53 ),
	CG_FRAME(    0,    0,   20,           0,   395, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   396, 0x80,   0x20,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl28_super_07 ),
};

/* Char_Super 18 */
const Sint32 pl28_super_18[] = {
	CG_FRAME(    0,    0,    2,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
	CG_NEXT( pl28_super_18_loop ),
};

/* Char_Super 14: explosion_jump */
const Sint32 pl28_super_14[] = {
	CG_FRAME(    0,    0,    2,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,    1,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
};

const Sint32 pl28_super_18_loop[] = {
	CG_FRAME(    0,    1,    1,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,   7 ),
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0600,   7 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0800,  53 ),
	CG_FRAME(    0,    0,    7,           0,   393, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0a00,  53 ),
	CG_FRAME(    0,    1,    1,           0,   394, 0x80,   0x00,  ATK,  0x00, 0x14,    0,   0,     0, 0x0c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00, 0x1e,    0,   0,     0, 0x0e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1000,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1200,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1400,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1600,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1800,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1a00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1c00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x1e00,  53 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x2000,  53 ),
	CG_FRAME(    0,    0,   20,           0,   395, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   396, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl28_super_14 ),
};

/* Char_Super 8: shl28_01_init */
const Sint32 pl28_super_08[] = {
	CG_FRAME(    0,    0,    4,           0, 32792, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32799, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32801, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32805, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_08 ),
};

/* Char_Super 9: shl28_01_init */
const Sint32 pl28_super_09[] = {
	CG_FRAME(    0,    0,    4,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32800, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32804, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_09 ),
};

/* Char_Super 10: shl28_01_init */
const Sint32 pl28_super_10[] = {
	CG_FRAME(    0,    0,    4,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32800, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32804, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_10 ),
};

/* Char_Super 11: shl28_01_2_move */
const Sint32 pl28_super_11[] = {
	CG_FRAME(    0,    0,    2,           0, 32806, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32807, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32808, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32809, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0, 32810, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32811, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32812, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32813, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32814, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_11 ),
};

/* Char_Super 12: shl28_01_2_init */
const Sint32 pl28_super_12[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_super_12_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_12_loop ),
};

/* Char_Super 13: shl28_01_2_init */
const Sint32 pl28_super_13[] = {
	CG_FRAME(    0,    0,    1,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_super_13_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32773, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_super_13_loop ),
};

/* Char_Effect 3 */
const Sint32 pl28_effect_03[] = {
	CG_FRAME(    0,    0,    1,           0,   514, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   515, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   516, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   517, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   518, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   519, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   520, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   521, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1000,   0 ),
	CG_NEXT( pl28_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl28_effect_04[] = {
	CG_FRAME(    0,    0,    1,           0, 32774, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32776, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32777, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32778, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32779, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32780, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32781, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1000,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32782, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x1200,   0 ),
	CG_NEXT( pl28_effect_04 ),
};

/* Char_Effect 5
   Char_Effect 22: shl28_00_t10_init */
const Sint32 pl28_effect_05[] = {
	CG_FRAME(    0,    0,    2,           0, 32787, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32788, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32789, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32790, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl28_effect_06[] = {
	CG_FRAME(    0,    0,    2,           0, 32783, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32786, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_06 ),
};

/* Char_Extra 0: shl28_00_t5_init, shl28_00_t6_init, shl28_00_t8_init */
const Sint32 pl28_extra_00[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl28_extra_00 ),
};

/* Char_Extra 1: shl28_00_t6_move */
const Sint32 pl28_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0, 32768, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_extra_01 ),
};

/* Char_Extra 4: sp_chouhatsu (Taunt) */
const Sint32 pl28_extra_04[] = {
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   431, 0x80,   0x00,    0,  0x00, 0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   431, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   432, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl28_extra_04 ),
};

/* Char_Effect 7: pls28_00_init */
const Sint32 pl28_effect_07[] = {
	CG_FRAME(    0,    0,    4,           0,   546, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   547, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   548, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl28_effect_08[] = {
	CG_FRAME(    0,    0,    4,           0,   549, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   550, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   551, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl28_effect_09[] = {
	CG_FRAME(    0,    0,    4,           0,   552, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   553, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   554, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl28_effect_10[] = {
	CG_FRAME(    0,    0,    4,           0,   555, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   556, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   557, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl28_effect_11[] = {
	CG_FRAME(    0,    0,    4,           0,   558, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   559, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   560, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl28_effect_12[] = {
	CG_FRAME(    0,    0,    4,           0,   561, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   562, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   563, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl28_effect_13[] = {
	CG_FRAME(    0,    0,    4,           0,   564, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   565, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   566, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl28_effect_14[] = {
	CG_FRAME(    0,    0,    4,           0,   567, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   568, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   569, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl28_effect_15[] = {
	CG_FRAME(    0,    0,    4,           0,   570, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   571, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   572, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl28_effect_16[] = {
	CG_FRAME(    0,    0,    4,           0,   573, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   574, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   575, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl28_effect_17[] = {
	CG_FRAME(    0,    0,    4,           0,   576, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   577, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   578, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl28_effect_18[] = {
	CG_FRAME(    0,    0,    4,           0,   579, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   580, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   581, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl28_effect_19[] = {
	CG_FRAME(    0,    0,    4,           0,   582, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   583, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   584, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_19 ),
};

/* Char_Effect 20: shl28_00_t1_init */
const Sint32 pl28_effect_20[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32769, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_effect_20_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32770, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_20_loop ),
};

/* Char_Effect 21 */
const Sint32 pl28_effect_21[] = {
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl28_effect_21_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32773, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl28_effect_21_loop ),
};
