/* Ken (character 27): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_27      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_27     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl27.c) says which group
                   and number each move uses.
   pl27_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl27_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl27_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl27_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_27
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_27, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_27 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl27_stand_00[], pl27_stand_02[], pl27_stand_03[], pl27_jump_00[],
	pl27_jump_00_loop[], pl27_jump_01[], pl27_jump_01_loop[], pl27_jump_02[], pl27_jump_02_loop[],
	pl27_jump_03[], pl27_jump_06[], pl27_jump_09[], pl27_jump_07[], pl27_jump_10[], pl27_jump_08[],
	pl27_jump_11[], pl27_dash_00[], pl27_dash_00_loop[], pl27_dash_01[], pl27_dash_01_loop[],
	pl27_dash_03[], pl27_dash_03_loop[], pl27_crouch_00[], pl27_crouch_01[], pl27_crouch_02[],
	pl27_turn_00[], pl27_turn_01[], pl27_guard_00[], pl27_guard_01[], pl27_guard_02[],
	pl27_guard_03[], pl27_guard_04[], pl27_guard_05[], pl27_guard_06[], pl27_guard_07[],
	pl27_stand_p_00[], pl27_stand_p_01[], pl27_stand_p_02[], pl27_stand_k_00[], pl27_stand_k_01[],
	pl27_stand_k_02[], pl27_air_guard_00[], pl27_air_guard_01[], pl27_crouch_p_00[],
	pl27_crouch_p_01[], pl27_crouch_p_02[], pl27_extra_08[], pl27_crouch_k_00[], pl27_crouch_k_01[],
	pl27_crouch_k_02[], pl27_jump_p_00[], pl27_jump_p_01[], pl27_jump_p_02[], pl27_jump_k_00[],
	pl27_jump_k_01[], pl27_jump_k_02[], pl27_get_up_00[], pl27_get_up_01[], pl27_get_up_02[],
	pl27_get_up_03[], pl27_damage_00[], pl27_damage_01[], pl27_damage_02[], pl27_damage_03[],
	pl27_damage_04[], pl27_damage_05[], pl27_damage_06[], pl27_damage_07[], pl27_damage_08[],
	pl27_damage_10[], pl27_damage_11[], pl27_damage_12[], pl27_damage_13[], pl27_damage_13_loop[],
	pl27_damage_09[], pl27_damage_09_loop[], pl27_damage_16[], pl27_damage_17[], pl27_damage_18[],
	pl27_damage_19[], pl27_damage_20[], pl27_damage_21[], pl27_damage_22[], pl27_damage_23[],
	pl27_damage_24[], pl27_damage_25[], pl27_damage_26[], pl27_damage_27[], pl27_damage_31[],
	pl27_damage_32[], pl27_damage_28[], pl27_damage_29[], pl27_caught_00[], pl27_caught_01[],
	pl27_caught_02[], pl27_caught_03[], pl27_caught_04[], pl27_caught_05[], pl27_caught_06[],
	pl27_caught_07[], pl27_caught_08[], pl27_caught_09[], pl27_caught_10[], pl27_caught_11[],
	pl27_caught_12[], pl27_caught_13[], pl27_caught_14[], pl27_caught_15[], pl27_caught_16[],
	pl27_caught_17[], pl27_caught_18[], pl27_caught_19[], pl27_caught_20[], pl27_caught_21[],
	pl27_dizzy_00[], pl27_win_00[], pl27_win_00_loop[], pl27_win_01[], pl27_win_01_loop[],
	pl27_win_02[], pl27_win_02_loop[], pl27_win_03[], pl27_win_05[], pl27_win_05_loop[],
	pl27_appear_00[], pl27_appear_00_loop[], pl27_extra_00[], pl27_special_28[], pl27_extra_01[],
	pl27_extra_02[], pl27_extra_03[], pl27_extra_05[], pl27_extra_04[], pl27_extra_06[],
	pl27_extra_07[], pl27_extra_07_loop[], pl27_change_00[], pl27_change_00_loop[], pl27_change_02[],
	pl27_change_03[], pl27_change_03_loop[], pl27_change_04[], pl27_catch_00[], pl27_catch_00_loop[],
	pl27_catch_03[], pl27_catch_03_loop[], pl27_catch_04[], pl27_catch_01[], pl27_catch_02[],
	pl27_catch_02_loop[], pl27_special_11[], pl27_special_13[], pl27_special_13_loop[],
	pl27_special_16[], pl27_special_18[], pl27_special_00[], pl27_special_01[], pl27_special_14[],
	pl27_special_15[], pl27_special_02[], pl27_special_03[], pl27_special_26[], pl27_special_04[],
	pl27_special_04_loop[], pl27_special_06[], pl27_special_05[], pl27_special_07[],
	pl27_special_08[], pl27_special_08_loop[], pl27_special_09[], pl27_special_09_loop[],
	pl27_special_10[], pl27_special_25[], pl27_special_19[], pl27_special_19_loop[],
	pl27_special_20[], pl27_super_00[], pl27_super_09[], pl27_super_01[], pl27_super_01_loop[],
	pl27_super_10[], pl27_super_10_loop[], pl27_super_02[], pl27_super_03[], pl27_super_03_loop[],
	pl27_super_04[], pl27_super_05[], pl27_super_11[], pl27_super_06[], pl27_super_07[],
	pl27_super_07_loop[], pl27_super_08[], pl27_super_12[], pl27_special_21[],
	pl27_special_21_loop[], pl27_special_22[], pl27_special_22_loop[], pl27_special_27[],
	pl27_special_27_loop[], pl27_special_23[], pl27_change_01[], pl27_change_01_loop[],
	pl27_effect_00[], pl27_effect_00_loop[], pl27_effect_01[], pl27_effect_02[], pl27_effect_03[],
	pl27_effect_04[], pl27_effect_04_loop[], pl27_effect_05[], pl27_effect_05_loop[],
	pl27_effect_06[], pl27_effect_07[], pl27_effect_08[], pl27_effect_09[], pl27_effect_10[],
	pl27_effect_11[], pl27_effect_12[], pl27_effect_13[], pl27_effect_14[], pl27_effect_15[],
	pl27_effect_16[], pl27_effect_17[], pl27_effect_18[], pl27_effect_19[], pl27_effect_20[],
	pl27_effect_21[], pl27_effect_22[], pl27_effect_23[], pl27_effect_23_loop[], pl27_effect_24[],
	pl27_effect_24_loop[], pl27_effect_25[], pl27_effect_25_loop[], pl27_effect_26[],
	pl27_effect_27[], pl27_effect_36[];
extern const Sint32 *const pl27_stand_tbl[], *const pl27_jump_tbl[], *const pl27_dash_tbl[],
	*const pl27_crouch_tbl[], *const pl27_turn_tbl[], *const pl27_guard_tbl[],
	*const pl27_air_guard_tbl[], *const pl27_stand_p_tbl[], *const pl27_stand_k_tbl[],
	*const pl27_crouch_p_tbl[], *const pl27_crouch_k_tbl[], *const pl27_jump_p_tbl[],
	*const pl27_jump_k_tbl[], *const pl27_damage_tbl[], *const pl27_dizzy_tbl[],
	*const pl27_catch_tbl[], *const pl27_caught_tbl[], *const pl27_get_up_tbl[],
	*const pl27_appear_tbl[], *const pl27_win_tbl[], *const pl27_extra_tbl[],
	*const pl27_special_tbl[], *const pl27_super_tbl[], *const pl27_effect_tbl[],
	*const pl27_change_tbl[], *const pl27_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_27[16] = {
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
const Sint32 *const *const char_tbl_27[] = {
	pl27_stand_tbl, pl27_jump_tbl, pl27_dash_tbl, pl27_crouch_tbl, pl27_turn_tbl, pl27_guard_tbl,
	pl27_air_guard_tbl, pl27_stand_p_tbl, pl27_stand_k_tbl, pl27_crouch_p_tbl, pl27_crouch_k_tbl, pl27_jump_p_tbl,
	pl27_jump_k_tbl, pl27_damage_tbl, pl27_dizzy_tbl, pl27_catch_tbl, pl27_caught_tbl, pl27_get_up_tbl,
	pl27_appear_tbl, pl27_win_tbl, pl27_extra_tbl, pl27_special_tbl, pl27_super_tbl, pl27_effect_tbl,
	pl27_change_tbl, pl27_grp25_tbl, 0,
};

const Sint32 *const pl27_stand_tbl[] = {
	pl27_stand_00, pl27_stand_00, pl27_stand_02, pl27_stand_03, pl27_stand_00, pl27_stand_00,
	pl27_stand_02, pl27_stand_03, 0,
};

const Sint32 *const pl27_jump_tbl[] = {
	pl27_jump_00, pl27_jump_01, pl27_jump_02, pl27_jump_03, pl27_jump_03, pl27_jump_03,
	pl27_jump_06, pl27_jump_07, pl27_jump_08, pl27_jump_09, pl27_jump_10, pl27_jump_11,
	0,
};

const Sint32 *const pl27_dash_tbl[] = {
	pl27_dash_00, pl27_dash_01, pl27_dash_00, pl27_dash_03, 0,
};

const Sint32 *const pl27_crouch_tbl[] = {
	pl27_crouch_00, pl27_crouch_01, pl27_crouch_02, 0,
};

const Sint32 *const pl27_turn_tbl[] = {
	pl27_turn_00, pl27_turn_01, 0,
};

const Sint32 *const pl27_guard_tbl[] = {
	pl27_guard_00, pl27_guard_01, pl27_guard_02, pl27_guard_03, pl27_guard_04, pl27_guard_05,
	pl27_guard_06, pl27_guard_07, 0,
};

const Sint32 *const pl27_air_guard_tbl[] = {
	pl27_air_guard_00, pl27_air_guard_01, pl27_air_guard_01, 0,
};

const Sint32 *const pl27_stand_p_tbl[] = {
	pl27_stand_p_00, pl27_stand_p_01, pl27_stand_p_02, 0,
};

const Sint32 *const pl27_stand_k_tbl[] = {
	pl27_stand_k_00, pl27_stand_k_01, pl27_stand_k_02, 0,
};

const Sint32 *const pl27_crouch_p_tbl[] = {
	pl27_crouch_p_00, pl27_crouch_p_01, pl27_crouch_p_02, 0,
};

const Sint32 *const pl27_crouch_k_tbl[] = {
	pl27_crouch_k_00, pl27_crouch_k_01, pl27_crouch_k_02, 0,
};

const Sint32 *const pl27_jump_p_tbl[] = {
	pl27_jump_p_00, pl27_jump_p_01, pl27_jump_p_02, pl27_jump_p_00, pl27_jump_p_01, pl27_jump_p_02,
	0,
};

const Sint32 *const pl27_jump_k_tbl[] = {
	pl27_jump_k_00, pl27_jump_k_01, pl27_jump_k_02, pl27_jump_k_00, pl27_jump_k_01, pl27_jump_k_02,
	0,
};

const Sint32 *const pl27_damage_tbl[] = {
	pl27_damage_00, pl27_damage_01, pl27_damage_02, pl27_damage_03, pl27_damage_04, pl27_damage_05,
	pl27_damage_06, pl27_damage_07, pl27_damage_08, pl27_damage_09, pl27_damage_10, pl27_damage_11,
	pl27_damage_12, pl27_damage_13, pl27_damage_13, pl27_damage_13, pl27_damage_16, pl27_damage_17,
	pl27_damage_18, pl27_damage_19, pl27_damage_20, pl27_damage_21, pl27_damage_22, pl27_damage_23,
	pl27_damage_24, pl27_damage_25, pl27_damage_26, pl27_damage_27, pl27_damage_28, pl27_damage_29,
	pl27_damage_27, pl27_damage_31, pl27_damage_32, pl27_damage_32, pl27_damage_13, 0,
};

const Sint32 *const pl27_dizzy_tbl[] = {
	pl27_dizzy_00, 0,
};

const Sint32 *const pl27_catch_tbl[] = {
	pl27_catch_00, pl27_catch_01, pl27_catch_02, pl27_catch_03, pl27_catch_04, 0,
};

const Sint32 *const pl27_caught_tbl[] = {
	pl27_caught_00, pl27_caught_01, pl27_caught_02, pl27_caught_03, pl27_caught_04, pl27_caught_05,
	pl27_caught_06, pl27_caught_07, pl27_caught_08, pl27_caught_09, pl27_caught_10, pl27_caught_11,
	pl27_caught_12, pl27_caught_13, pl27_caught_14, pl27_caught_15, pl27_caught_16, pl27_caught_17,
	pl27_caught_18, pl27_caught_19, pl27_caught_20, pl27_caught_21, 0,
};

const Sint32 *const pl27_get_up_tbl[] = {
	pl27_get_up_00, pl27_get_up_01, pl27_get_up_02, pl27_get_up_03, 0,
};

const Sint32 *const pl27_appear_tbl[] = {
	pl27_appear_00, 0,
};

const Sint32 *const pl27_win_tbl[] = {
	pl27_win_00, pl27_win_01, pl27_win_02, pl27_win_03, pl27_win_03, pl27_win_05,
	0,
};

const Sint32 *const pl27_extra_tbl[] = {
	pl27_extra_00, pl27_extra_01, pl27_extra_02, pl27_extra_03, pl27_extra_04, pl27_extra_05,
	pl27_extra_06, pl27_extra_07, pl27_extra_08, 0,
};

const Sint32 *const pl27_special_tbl[] = {
	pl27_special_00, pl27_special_01, pl27_special_02, pl27_special_03, pl27_special_04, pl27_special_05,
	pl27_special_06, pl27_special_07, pl27_special_08, pl27_special_09, pl27_special_10, pl27_special_11,
	pl27_special_11, pl27_special_13, pl27_special_14, pl27_special_15, pl27_special_16, pl27_special_16,
	pl27_special_18, pl27_special_19, pl27_special_20, pl27_special_21, pl27_special_22, pl27_special_23,
	pl27_special_23, pl27_special_25, pl27_special_26, pl27_special_27, pl27_special_28, 0,
};

const Sint32 *const pl27_super_tbl[] = {
	pl27_super_00, pl27_super_01, pl27_super_02, pl27_super_03, pl27_super_04, pl27_super_05,
	pl27_super_06, pl27_super_07, pl27_super_08, pl27_super_09, pl27_super_10, pl27_super_11,
	pl27_super_12, 0,
};

const Sint32 *const pl27_effect_tbl[] = {
	pl27_effect_00, pl27_effect_01, pl27_effect_02, pl27_effect_03, pl27_effect_04, pl27_effect_05,
	pl27_effect_06, pl27_effect_07, pl27_effect_08, pl27_effect_09, pl27_effect_10, pl27_effect_11,
	pl27_effect_12, pl27_effect_13, pl27_effect_14, pl27_effect_15, pl27_effect_16, pl27_effect_17,
	pl27_effect_18, pl27_effect_19, pl27_effect_20, pl27_effect_21, pl27_effect_22, pl27_effect_23,
	pl27_effect_24, pl27_effect_25, pl27_effect_26, pl27_effect_27, pl27_effect_00, pl27_effect_01,
	pl27_effect_02, pl27_effect_03, pl27_effect_04, pl27_effect_05, pl27_effect_06, pl27_effect_07,
	pl27_effect_36, 0,
};

const Sint32 *const pl27_change_tbl[] = {
	pl27_change_00, pl27_change_01, pl27_change_02, pl27_change_03, pl27_change_04, 0,
};

const Sint32 *const pl27_grp25_tbl[] = {
	pl27_effect_36, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl27_stand_00[] = {
	CG_FRAME(    0,    0,   3,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl27_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl27_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl27_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   6,           0,    67, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   6,           0,    68, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   6,           0,    69, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   6,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl27_jump_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl27_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl27_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl27_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl27_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl27_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,    68, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl27_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl27_jump_02_loop ),
};

/* Char_Jump 3: jtatsumaki_spin (Tatsumaki Senpuukyaku in the air), jtatsumaki_fall (Tatsumaki Senpuukyaku in the air), shippu_fall (Shippuu Jinraikyaku (hyper))
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl27_jump_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl27_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl27_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl27_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl27_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl27_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl27_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_jump_11 ),
};

/* Char_Dash 0: zenpou_init (Zenpou Tenshin (roll)), act_dash
   Char_Dash 2 */
const Sint32 pl27_dash_00[] = {
	CG_FRAME(    0,    1,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    59, 0x80,   0x32,                7,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    60, 0x80,   0x54,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    61, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_dash_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl27_dash_01[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_dash_01_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,    62, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl27_dash_03[] = {
	CG_FRAME(    0,    0,   5,           0,    63, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_dash_03_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_dash_03_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl27_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl27_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    47, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl27_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl27_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl27_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    85, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    86, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl27_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl27_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl27_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl27_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl27_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl27_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    90, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl27_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    89, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl27_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    90, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl27_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   140, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl27_stand_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   142, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   143, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   9,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl27_stand_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   140, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   146, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   9,           0,   146, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl27_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   7,           0,   150, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl27_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x00,          ATK | 7,  0x00,       0x03,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    1,   7,           0,   154, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   5,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   5,           0,   156, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   5,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl27_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl27_stand_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,          ATK | 9,  0x00,       0x05,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   6,           0,   161, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   158, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl27_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl27_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl27_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl27_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   5,           0,   163, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   162, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl27_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   164, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   165, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl27_crouch_p_02[] = {
	CG_FRAME(    0,    0,   9,           0,   167, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   168, 0x80,   0x40,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  10,           0,   169, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   167, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_crouch_p_02 ),
};

/* Char_Extra 8: pl27_upper */
const Sint32 pl27_extra_08[] = {
	CG_FRAME(    0,    0,   9,           0,   167, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   168, 0x80,   0x40,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  10,           0,   169, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   167, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_extra_08 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl27_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   170, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl27_crouch_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   170, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   173, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   9,           0,   173, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   170, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl27_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   174, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   8,           0,   175, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0,   176, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   8,           0,   177, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,    84, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl27_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl27_jump_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   179, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   178, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl27_jump_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   181, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   178, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl27_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   181, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   9,           0,   181, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   178, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl27_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   183, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   184, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl27_jump_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   182, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  12,           0,   183, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   8,           0,   183, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   184, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl27_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   190, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   186, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   5,           0,   187, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   188, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl27_get_up_00[] = {
	CG_FRAME(    0,    0,   6,           0,   106, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   107, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   108, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl27_get_up_01[] = {
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    47, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl27_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    47, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl27_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    47, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl27_damage_00[] = {
	CG_FRAME( 0x80,    0,   2,           0,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl27_damage_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   8,           0,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl27_damage_02[] = {
	CG_FRAME( 0x80,    0,   4,           0,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl27_damage_03[] = {
	CG_FRAME( 0x80,    0,   2,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   5,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl27_damage_04[] = {
	CG_FRAME( 0x80,    0,   2,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   8,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl27_damage_05[] = {
	CG_FRAME( 0x80,    0,   4,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   8,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl27_damage_06[] = {
	CG_FRAME(    0,    0,   2,           0,   291, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   5,           0,   292, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   291, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl27_damage_07[] = {
	CG_FRAME( 0x80,    0,   2,           0,   291, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,  10,           0,   292, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   291, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl27_damage_08[] = {
	CG_FRAME( 0x80,    0,   2,           0,   291, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   2,           0,   292, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,  10,           0,   293, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   292, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   291, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl27_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl27_damage_10[] = {
	CG_FRAME( 0x80,    0,   1,           0,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,   8,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl27_damage_11[] = {
	CG_FRAME( 0x80,    0,   1,           0,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   1,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,   6,           0,   296, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl27_damage_12[] = {
	CG_FRAME(    0,    0,   1,           0,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl27_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x71,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl27_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   308, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl27_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl27_damage_09[] = {
	CG_FRAME(    0,    0,   8,           0,   309, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   310, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   311, 0x80,   0x15,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   319, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   321, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   322, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl27_damage_09_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   323, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl27_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl27_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   313, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl27_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   314, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl27_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   315, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl27_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   316, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl27_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   317, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl27_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   313, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl27_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl27_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl27_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   316, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl27_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   317, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl27_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   321, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   322, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl27_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl27_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl27_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   323, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl27_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl27_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    47, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl27_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   329, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl27_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   330, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl27_caught_00[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   284, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl27_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl27_caught_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl27_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl27_caught_02[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl27_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl27_caught_03[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl27_caught_04[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl27_caught_05[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl27_caught_06[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl27_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl27_caught_07[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl27_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl27_caught_08[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   293, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl27_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl27_caught_09[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl27_caught_10[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl27_caught_11[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl27_caught_12[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl27_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl27_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl27_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl27_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl27_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl27_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl27_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   324, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl27_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl27_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl27_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   323, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl27_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl27_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl27_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl27_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_dizzy_00 ),
};

/* Char_Win 0 */
const Sint32 pl27_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_win_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl27_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,             0x0a,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_win_01_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl27_win_02[] = {
	CG_FRAME(    0,    0,   1,           0,   131, 0x80,   0x00,             0x0f,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_win_02_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_win_02_loop ),
};

/* Char_Win 3
   Char_Win 4 */
const Sint32 pl27_win_03[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_win_03 ),
};

/* Char_Win 5 */
const Sint32 pl27_win_05[] = {
	CG_FRAME(    0,    0,   2,           0,   132, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  18,           0,   133, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_win_05_loop[] = {
	CG_FRAME(    0,    0,  48, END_OF_DATA,   135, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_win_05_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl27_appear_00[] = {
	CG_FRAME(    0,    0,   2,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,             0x12,  0x00, 0x14 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,             0x12,  0x00, 0x14 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   144, 0x80,   0x00,             0x13,  0x00, 0x17 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   103, 0x80,   0x00,                4,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl27_appear_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_appear_00_loop ),
};

/* Char_Extra 0 */
const Sint32 pl27_extra_00[] = {
	CG_FRAME(    0,    0,   6,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   152, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   213, 0x80,   0x21, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x21, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   215, 0x80,   0x21, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    1,    0,   4,           0,   216, 0x80,   0x21, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    1,    0,   4,           0,   217, 0x80,   0x21, ATK | CG_ATCK_NG | 9,  0x00,       0x05,    0,   0,     0,      0,  54 ),
	CG_FRAME(    1,    1,   4,           0,   218, 0x80,   0x21, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x21, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   149, 0x80,   0x29, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl27_extra_00 ),
};

/* Char_Special 28: snapback_init (Snapback) */
const Sint32 pl27_special_28[] = {
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   3,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,          ATK | 9,  0x00,       0x05,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   6,           0,   161, 0x80,   0x00,              ATE,  0x00, 0x16 | CSE,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   158, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl27_special_28 ),
};

/* Char_Extra 1 */
const Sint32 pl27_extra_01[] = {
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   192, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   191, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl27_extra_01 ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl27_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   194, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   195, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   5,           0,   196, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   193, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_extra_02 ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl27_extra_03[] = {
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   199, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   8,           0,   199, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   198, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_extra_03 ),
};

/* Char_Extra 5 */
const Sint32 pl27_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_extra_05 ),
};

/* Char_Extra 4: sp_chouhatsu (Taunt) */
const Sint32 pl27_extra_04[] = {
	CG_FRAME(    0,    0,   6,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   101, 0x80,   0x00,             0x0a,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  36, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_extra_04 ),
};

/* Char_Extra 6: t_kick_init */
const Sint32 pl27_extra_06[] = {
	CG_FRAME(    0,    0,   2,           0,   197, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  64, END_OF_DATA,   199, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl27_extra_06 ),
};

/* Char_Extra 7: t_kick_move */
const Sint32 pl27_extra_07[] = {
	CG_FRAME(    0,    0,   6,           0,    81, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,       ATE | 0x11,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    97, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,    99, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
};

const Sint32 pl27_extra_07_loop[] = {
	CG_FRAME(    0,    0,  18, END_OF_DATA,   102, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl27_extra_07_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl27_change_00[] = {
	CG_FRAME(    0,    0,   3,           0,   132, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   133, 0x30,   0x00,       ATK | 0x10,  0x00,       0x0d,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   135, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_change_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   135, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl27_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl27_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl27_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x20,                0,  0x00,       0x0b,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl27_change_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   139, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl27_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  24, END_OF_DATA,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl27_change_04 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl27_catch_00[] = {
	CG_FRAME(    1,    0,  15,           0,   209, 0x80,   0x00,                7,  0x00,       0x03,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    1,    0,   6,           0,   200, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   8,           0,   205, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   7,           0,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   6,           0,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   5,           0,   208, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   3,           0,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x5f,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    1,    1,  41,           0,   202, 0x80,   0x5f,                8,  0x00,       0x04,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    1,    0,   2,           0,   203, 0x80,   0x5f,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,   6,           0,   204, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_catch_00_loop[] = {
	CG_FRAME(    1,    0,   6, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_catch_00_loop ),
};

/* Char_Catch 3: jump_k_set */
const Sint32 pl27_catch_03[] = {
	CG_FRAME(    0,    0,   6,           0,   200, 0x80,   0x20,                7,  0x00,       0x03,    0,   0,     0, 0x0002,   0 ),
};

const Sint32 pl27_catch_03_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   205, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   7,           0,   206, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   6,           0,   207, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   208, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl27_catch_03_loop ),
};

/* Char_Catch 4: jump_k_catch */
const Sint32 pl27_catch_04[] = {
	CG_FRAME(    0,    0,   5,           0,   201, 0x80,   0x5f,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    1,    1,  24,           0,   202, 0x80,   0x5f,                8,  0x00,       0x04,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    1,    0,   4,           0,   203, 0x80,   0x5f,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    1,    0,   4,           0,   204, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_catch_04 ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl27_catch_01[] = {
	CG_FRAME(    0,    0,  10,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   1,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    1,  12,           0,   211, 0x80,   0x00,                7,  0x00,       0x03,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_NEXT( pl27_catch_01 ),
};

/* Char_Catch 2: nml_k_release */
const Sint32 pl27_catch_02[] = {
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,  24,           0,   216, 0x80,   0x00,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_catch_02_loop[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_catch_02_loop ),
};

/* Char_Special 11
   Char_Special 12 */
const Sint32 pl27_special_11[] = {
	CG_FRAME(    0,    0,   1,           0,   379, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   380, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   381, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   382, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   383, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   384, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1,           0,   385, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   386, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl27_special_11 ),
};

/* Char_Special 13: shl27_00_hit */
const Sint32 pl27_special_13[] = {
	CG_FRAME(    0,    0,   2,           0,   387, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_special_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   392, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_special_13_loop ),
};

/* Char_Special 16
   Char_Special 17 */
const Sint32 pl27_special_16[] = {
	CG_FRAME(    0,    0,   1,           0,   393, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   396, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl27_special_16 ),
};

/* Char_Special 18: shl27_00_air_hit */
const Sint32 pl27_special_18[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   406, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl27_special_18 ),
};

/* Char_Special 0 */
const Sint32 pl27_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   7,           0,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   226, 0x80,   0x4a,          ATK | 1,  0x00,       0x16,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x4a,       ATK | 0x0c,  0x00,       0x1f,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl27_special_01[] = {
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   7,           0,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   226, 0x80,   0x4a,          ATK | 1,  0x00,       0x16,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x4a,       ATK | 0x0c,  0x00,       0x1f,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_01 ),
};

/* Char_Special 14: jhadou_init (Hadouken in the air, 14 + atck_level) */
const Sint32 pl27_special_14[] = {
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,   1,           0,   252, 0x80,   0x20,          ATK | 1,  0x00,       0x16,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,       ATK | 0x0c,  0x00,       0x1f,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   253, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   178, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl27_special_15[] = {
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,   1,           0,   252, 0x80,   0x20,          ATK | 1,  0x00,       0x16,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,       ATK | 0x0c,  0x00,       0x1f,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   253, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   5,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   5,           0,   253, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   178, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_special_15 ),
};

/* Char_Special 2: shouryuu_init (Shouryuuken, 2 + atck_level) */
const Sint32 pl27_special_02[] = {
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,          ATK | 2,  0x00,       0x15,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0, 0x80,   2,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(   51, 0x80,   4,           0,   230, 0x00,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  62 ),
	CG_FRAME(   52,    0,   8,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_FRAME(    0,    0,   6,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_FRAME(    0,    0,   6,           0,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   233, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   234, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   1,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,    47, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl27_special_03[] = {
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,          ATK | 2,  0x00,       0x15,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(   55, 0x80,   2,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0, 0x0200,  64 ),
	CG_FRAME(   56, 0x80,   4,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  62 ),
	CG_FRAME(   57,    0,   4,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_FRAME(    0,    0,   8,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_FRAME(    0,    0,  14,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_FRAME(    0,    0,   6,           0,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   233, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   234, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   1,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   9,           0,    47, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_03 ),
};

/* Char_Special 26 */
const Sint32 pl27_special_26[] = {
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,          ATK | 2,  0x00,       0x15,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(   55, 0x80,   2,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0, 0x0200,  99 ),
	CG_FRAME(   56, 0x80,   4,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300, 100 ),
	CG_FRAME(   57,    0,   4,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400, 101 ),
	CG_FRAME(    0,    0,  22,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400, 101 ),
	CG_FRAME(    0,    0,   6,           0,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   233, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   4,           0,   234, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_26 ),
};

/* Char_Special 4: tatsumaki_init (Tatsumaki Senpuukyaku) */
const Sint32 pl27_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
};

const Sint32 pl27_special_04_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl27_special_04_loop ),
};

/* Char_Special 6: tatsumaki_rise (Tatsumaki Senpuukyaku) */
const Sint32 pl27_special_06[] = {
	CG_FRAME(    0,    1,   2,           0,   239, 0x80,   0x00,       ATK | 0x13,  0x00, 0x17 | CSE,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    1,    0,   1, END_OF_DATA,   242, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl27_special_06 ),
};

/* Char_Special 5: tatsumaki_spin_end (Tatsumaki Senpuukyaku), shippu_end (Shippuu Jinraikyaku (hyper)) */
const Sint32 pl27_special_05[] = {
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   246, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_05 ),
};

/* Char_Special 7: tatsumaki_fall (Tatsumaki Senpuukyaku) */
const Sint32 pl27_special_07[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_07 ),
};

/* Char_Special 8: jtatsumaki_init (Tatsumaki Senpuukyaku in the air) */
const Sint32 pl27_special_08[] = {
	CG_FRAME(    0,    0,   6,           0,   238, 0x80,   0x20,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,   7 ),
};

const Sint32 pl27_special_08_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   239, 0x80,   0x20,       ATK | 0x13,  0x00, 0x17 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   2,           0,   241, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,   -1,   1, END_OF_DATA,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl27_special_09[] = {
	CG_FRAME(    0,    0,   6,           0,   238, 0x80,   0x20,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,   7 ),
};

const Sint32 pl27_special_09_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   239, 0x80,   0x20,       ATK | 0x13,  0x00, 0x17 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   2,           0,   241, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,   -1,   1, END_OF_DATA,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_special_09_loop ),
};

/* Char_Special 10: jtatsumaki_spin (Tatsumaki Senpuukyaku in the air) */
const Sint32 pl27_special_10[] = {
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   246, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   243, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_special_10 ),
};

/* Char_Special 25: tatsumaki_spin (Tatsumaki Senpuukyaku) */
const Sint32 pl27_special_25[] = {
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x20,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   251, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl27_special_25 ),
};

/* Char_Special 19 */
const Sint32 pl27_special_19[] = {
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0x26,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x22,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x01,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl27_special_19_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl27_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl27_special_20[] = {
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    79, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_20 ),
};

/* Char_Super 0: reppa_init (Shouryuu Reppa (hyper)) */
const Sint32 pl27_super_00[] = {
	CG_FRAME(    0,    0,   5,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   228, 0x80,   0x00,          ATK | 4,  0x00,       0x0a,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(   73,    0,   4,           0,   229, 0x80,   0x00,          ATK | 5,  0x49,       0x19,    0,   0,     0, 0x0200,  77 ),
	CG_FRAME(   74,    0,   4,           0,   230, 0x80,   0x00,       ATK | 0x0d,  0x49,       0x1e,    0,   0,     0, 0x0300,  75 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  76 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(   74,    0,   3,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x49,       0x1e,    0,   0,     0, 0x0200,  90 ),
	CG_FRAME(   74,    0,   3,           0,   230, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0300,  88 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(   74,    1,   3,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x4a,       0x1e,    0,   0,     0, 0x0200,  90 ),
	CG_FRAME(   75,    1,   3,           0,   230, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0, 0x0300,  88 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   92,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   75,    0,   4,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(    0,    0,   6,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    1,   2,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    1,   3,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_super_00 ),
};

/* Char_Super 9 */
const Sint32 pl27_super_09[] = {
	CG_FRAME(    0,    0,  13,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   228, 0x80,   0x00,          ATK | 4,  0x00,       0x0a,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(   73,    0,   4,           0,   229, 0x80,   0x00,          ATK | 5,  0x49,       0x19,    0,   0,     0, 0x0200,  77 ),
	CG_FRAME(   74,    0,   4,           0,   230, 0x80,   0x00,       ATK | 0x0d,  0x49,       0x1e,    0,   0,     0, 0x0300,  75 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  76 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  76 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(   74,    0,   3,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x49,       0x1e,    0,   0,     0, 0x0200,  90 ),
	CG_FRAME(   74,    0,   3,           0,   230, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0300,  88 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   74,    0,   3,           0,   231, 0x80,   0x00,              ATK,  0x49,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(   74,    1,   3,           0,   229, 0x80,   0x00,       ATK | 0x0d,  0x4a,       0x1e,    0,   0,     0, 0x0200,  90 ),
	CG_FRAME(   75,    1,   3,           0,   230, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0, 0x0300,  88 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0, 0x0400,  89 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   74,    0,   2,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(   75,    0,   4,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  65 ),
	CG_FRAME(    0,    0,   6,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    1,   2,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    1,   3,           0,   234, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,   5 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_super_09 ),
};

/* Char_Super 1: shinryuu_init (Shinryuuken (hyper)) */
const Sint32 pl27_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   3,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,       ATK | 0x14,  0x00,       0x0a,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   229, 0x80,   0x00,              ATK,  0x4c,          0,    0,   0,     0, 0x0200,  78 ),
	CG_FRAME(    0, 0x80,   4,           0,   230, 0x80,   0x00,          ATK | 6,  0x4d,       0x18,    0,   0,     0, 0x0300,  78 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,       ATK | 0x0b,  0x4d,          0,    0,   0,     0, 0x0400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d, 0x4c | CSE,    0,   0,     0, 0x0b00,  78 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1200,  78 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1300,  78 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1500,  78 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1200,  78 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1300,  78 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1500,  78 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
};

const Sint32 pl27_super_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1200,  78 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1300,  78 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1500,  78 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
	CG_NEXT( pl27_super_01_loop ),
};

/* Char_Super 10 */
const Sint32 pl27_super_10[] = {
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   2,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   3,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   8,           0,   228, 0x80,   0x00,       ATK | 0x14,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   1,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   229, 0x80,   0x00,              ATK,  0x4c,          0,    0,   0,     0, 0x0200,  78 ),
	CG_FRAME(    0, 0x80,   4,           0,   230, 0x80,   0x00,          ATK | 6,  0x4d,       0x18,    0,   0,     0, 0x0300,  78 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,       ATK | 0x0b,  0x4d,          0,    0,   0,     0, 0x0400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x0b00,  78 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1200,  78 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1300,  78 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1500,  78 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1200,  78 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1300,  78 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1500,  78 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
};

const Sint32 pl27_super_10_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1200,  78 ),
	CG_FRAME(    0,    0,   3,           0,   257, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1300,  78 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1400,  78 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1500,  78 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   255, 0x80,   0x00,              ATK,  0x4d,          0,    0,   0,     0, 0x1600,  78 ),
	CG_NEXT( pl27_super_10_loop ),
};

/* Char_Super 2: shinryuu_rise (Shinryuuken (hyper)) */
const Sint32 pl27_super_02[] = {
	CG_FRAME(    0,    0,  10,           0,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   2,           0,   233, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0, 0x80,   5,           0,   234, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl27_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  18,           0,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,   224, 0x80,   0x00,          ATK | 4,  0x00,       0x0a,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1f00,   5 ),
	CG_FRAME(    0, 0x80,   1,           0,   226, 0x80,   0x4a,          ATK | 1,  0x00,       0x16,    0,   0,     0, 0x2000,  59 ),
	CG_FRAME(    0,    0,   1,           0,   227, 0x80,   0x4a,       ATK | 0x0c,  0x00,       0x1f,    0,   0,     0, 0x2000,  59 ),
};

const Sint32 pl27_super_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0, 0x2000,  59 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   227, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0, 0x2000,  59 ),
	CG_NEXT( pl27_super_03_loop ),
};

/* Char_Super 4 */
const Sint32 pl27_super_04[] = {
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_super_04 ),
};

/* Char_Super 5: shippu_init (Shippuu Jinraikyaku (hyper)) */
const Sint32 pl27_super_05[] = {
	CG_FRAME(    0,    0,   6,           0,    49, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   152, 0x80,   0x00,          ATK | 4,  0x00,       0x1b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    0,   3,           0,   159, 0x80,   0x00,       ATK | 0x15,  0x00,       0x1a,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   153, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   153, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   2,           0,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   2,           0,   211, 0x80,   0x00,              ATE,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   212, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl27_super_05 ),
};

/* Char_Super 11 */
const Sint32 pl27_super_11[] = {
	CG_FRAME(    0,    0,   6,           0,    49, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  18,           0,   152, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    0,   3,           0,   159, 0x80,   0x00,       ATK | 0x15,  0x00,       0x1a,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   153, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   153, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x00,              ATK,  0x52,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    4,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,              ATK,  0x52, 0x4b | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   2,           0,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   2,           0,   211, 0x80,   0x00,              ATE,  0x00, 0x4b | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   212, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl27_super_11 ),
};

/* Char_Super 6: shippu_kick (Shippuu Jinraikyaku (hyper)) */
const Sint32 pl27_super_06[] = {
	CG_FRAME(    0,    0,  18,           0,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl27_super_06 ),
};

/* Char_Super 7: shippu_kick (Shippuu Jinraikyaku (hyper)) */
const Sint32 pl27_super_07[] = {
	CG_FRAME(    0,    0,   2,           0,   235, 0x80,   0x00,              ATK,  0x58,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
};

const Sint32 pl27_super_07_loop[] = {
	CG_FRAME(    1,    0,   2,           0,   239, 0x80,   0x20,              ATK,  0x59, 0x4b | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    2,    0,   2,           0,   241, 0x80,   0x20,              ATK,  0x59, 0x4b | CSE,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,   16,   1, END_OF_DATA,   242, 0x80,   0x20,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_super_07_loop ),
};

/* Char_Super 8: shippu_rush (Shippuu Jinraikyaku (hyper)) */
const Sint32 pl27_super_08[] = {
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x20,          ATK | 8,  0x5a,       0x04,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   251, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl27_super_08 ),
};

/* Char_Super 12: shippu_rush (Shippuu Jinraikyaku (hyper)) */
const Sint32 pl27_super_12[] = {
	CG_FRAME(    1,    0,   2,           0,   239, 0x80,   0x20,              ATK,  0x5d, 0x4b | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    2,    0,   2,           0,   241, 0x80,   0x20,              ATK,  0x5d, 0x4b | CSE,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,   16,   1, END_OF_DATA,   242, 0x80,   0x20,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl27_super_12 ),
};

/* Char_Special 21: jshouryuu_init (Shouryuuken in the air, 21 + atck_level) */
const Sint32 pl27_special_21[] = {
	CG_FRAME(    0,    1,   4,           0,   263, 0x00,   0x20,          ATK | 2,  0x00,       0x15,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,   4,           0,   264, 0x00,   0x20,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0, 0x0200,  64 ),
	CG_FRAME(    0,    1,   4,           0,   265, 0x00,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  62 ),
};

const Sint32 pl27_special_21_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   231, 0x00,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_NEXT( pl27_special_21_loop ),
};

/* Char_Special 22 */
const Sint32 pl27_special_22[] = {
	CG_FRAME(    0,    1,   4,           0,   263, 0x00,   0x20,          ATK | 2,  0x00,       0x15,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(   56,    1,   4,           0,   264, 0x00,   0x20,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0, 0x0200,  64 ),
	CG_FRAME(   57,    1,   2,           0,   265, 0x00,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  62 ),
	CG_FRAME(   57,    1,   2,           0,   265, 0x00,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  62 ),
};

const Sint32 pl27_special_22_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   231, 0x00,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  63 ),
	CG_NEXT( pl27_special_22_loop ),
};

/* Char_Special 27: jshouryuu_rise (Shouryuuken in the air) */
const Sint32 pl27_special_27[] = {
	CG_FRAME(    0,    0,   4,           0,   233, 0x00,   0x20,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
};

const Sint32 pl27_special_27_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   233, 0x00,   0x20,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_NEXT( pl27_special_27_loop ),
};

/* Char_Special 23: jshouryuu_fall (Shouryuuken in the air, 23 + atck_level)
   Char_Special 24 */
const Sint32 pl27_special_23[] = {
	CG_FRAME(    0,    0,   4,           0,   234, 0x00,   0x20,              ATE,  0x00,          0,    0,   0,     0, 0x0500,  65 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl27_special_27 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl27_change_01[] = {
	CG_FRAME(    0,    0,   2,           0,   259, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x00,   0x56,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_change_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   261, 0x00,   0x56,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_change_01_loop ),
};

/* Char_Effect 0
   Char_Effect 28 */
const Sint32 pl27_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0, 32785, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_effect_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32790, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32791, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_00_loop ),
};

/* Char_Effect 1
   Char_Effect 29 */
const Sint32 pl27_effect_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   339, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_01 ),
};

/* Char_Effect 2
   Char_Effect 30 */
const Sint32 pl27_effect_02[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   340, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_02 ),
};

/* Char_Effect 3
   Char_Effect 31 */
const Sint32 pl27_effect_03[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   341, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_03 ),
};

/* Char_Effect 4
   Char_Effect 32 */
const Sint32 pl27_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   342, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_effect_04_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   343, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   344, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   345, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   348, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   349, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   350, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_04_loop ),
};

/* Char_Effect 5
   Char_Effect 33 */
const Sint32 pl27_effect_05[] = {
	CG_FRAME(    0,    0,   2,           0,   351, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   352, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   353, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   354, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   355, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   356, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_effect_05_loop[] = {
	CG_FRAME(    0,    0,  -1,        0x8f,    -1, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_05_loop ),
};

/* Char_Effect 6
   Char_Effect 34 */
const Sint32 pl27_effect_06[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32768, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_06 ),
};

/* Char_Effect 7
   Char_Effect 35 */
const Sint32 pl27_effect_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32769, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl27_effect_08[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32770, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl27_effect_09[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32771, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl27_effect_10[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32772, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl27_effect_11[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32773, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl27_effect_12[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32774, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl27_effect_13[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32775, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl27_effect_14[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl27_effect_15[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl27_effect_16[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32778, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl27_effect_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32779, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl27_effect_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32780, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl27_effect_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32781, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl27_effect_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32782, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl27_effect_21[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl27_effect_22[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32784, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl27_effect_23[] = {
	CG_FRAME(    0,    0,   2,           0, 32792, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32794, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_effect_23_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32797, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32798, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_23_loop ),
};

/* Char_Effect 24 */
const Sint32 pl27_effect_24[] = {
	CG_FRAME(    0,    0,   2,           0, 32791, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32790, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32785, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_effect_24_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    -1, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_24_loop ),
};

/* Char_Effect 25 */
const Sint32 pl27_effect_25[] = {
	CG_FRAME(    0,    0,   2,           0, 32798, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32797, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32794, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32792, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl27_effect_25_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    -1, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_25_loop ),
};

/* Char_Effect 26 */
const Sint32 pl27_effect_26[] = {
	CG_FRAME(    0,    0,   2,           0,   375, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   376, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_26 ),
};

/* Char_Effect 27 */
const Sint32 pl27_effect_27[] = {
	CG_FRAME(    0,    0,   2,           0,   377, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   378, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_27 ),
};

/* Char_Effect 36: pls27_02_init
   group 25 0 */
const Sint32 pl27_effect_36[] = {
	CG_FRAME(    0,    0,   2,           0, 32799, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32800, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32801, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32802, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32803, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32804, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32805, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32806, 0x00,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl27_effect_36 ),
};
