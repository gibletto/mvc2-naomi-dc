/* Spiral (character 31): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_31      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_31     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl31.c) says which group
                   and number each move uses.
   pl31_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl31_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl31_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl31_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_31
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_31, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_31 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl31_stand_00[], pl31_stand_06[], pl31_stand_07[], pl31_stand_02[],
	pl31_stand_03[], pl31_jump_00[], pl31_jump_09[], pl31_jump_01[], pl31_jump_10[], pl31_jump_07[],
	pl31_jump_07_loop[], pl31_jump_02[], pl31_jump_11[], pl31_jump_08[], pl31_jump_08_loop[],
	pl31_jump_03[], pl31_dash_00[], pl31_dash_02[], pl31_dash_01[], pl31_dash_01_loop[],
	pl31_dash_03[], pl31_crouch_00[], pl31_crouch_02[], pl31_crouch_01[], pl31_turn_00[],
	pl31_turn_01[], pl31_guard_03[], pl31_guard_00[], pl31_guard_01[], pl31_guard_02[],
	pl31_guard_07[], pl31_guard_04[], pl31_guard_05[], pl31_guard_06[], pl31_air_guard_00[],
	pl31_air_guard_02[], pl31_air_guard_01[], pl31_air_guard_03[], pl31_air_guard_03_loop[],
	pl31_air_guard_04[], pl31_air_guard_04_loop[], pl31_air_guard_05[], pl31_air_guard_05_loop[],
	pl31_stand_p_00[], pl31_stand_p_01[], pl31_stand_p_02[], pl31_stand_k_00[], pl31_stand_k_01[],
	pl31_stand_k_02[], pl31_crouch_p_00[], pl31_crouch_p_01[], pl31_crouch_p_02[],
	pl31_crouch_k_00[], pl31_crouch_k_01[], pl31_crouch_k_02[], pl31_jump_p_00[], pl31_jump_p_02[],
	pl31_extra_07[], pl31_extra_06[], pl31_jump_p_01[], pl31_jump_k_00[], pl31_jump_k_01[],
	pl31_extra_08[], pl31_jump_k_02[], pl31_dizzy_00[], pl31_get_up_00[], pl31_get_up_01[],
	pl31_get_up_02[], pl31_get_up_03[], pl31_win_00[], pl31_win_00_loop[], pl31_win_02[],
	pl31_win_02_loop[], pl31_win_01[], pl31_appear_00[], pl31_appear_00_loop[], pl31_appear_01[],
	pl31_appear_01_loop[], pl31_appear_02[], pl31_change_00[], pl31_change_02[], pl31_change_03[],
	pl31_change_03_loop[], pl31_change_01[], pl31_extra_00[], pl31_extra_01[], pl31_extra_05[],
	pl31_damage_00[], pl31_damage_01[], pl31_damage_02[], pl31_damage_03[], pl31_damage_04[],
	pl31_damage_05[], pl31_damage_06[], pl31_damage_07[], pl31_damage_08[], pl31_damage_10[],
	pl31_damage_11[], pl31_damage_12[], pl31_damage_09[], pl31_damage_09_loop[], pl31_damage_13[],
	pl31_damage_14[], pl31_damage_15[], pl31_damage_16[], pl31_damage_17[], pl31_damage_18[],
	pl31_damage_19[], pl31_damage_20[], pl31_damage_21[], pl31_damage_22[], pl31_damage_23[],
	pl31_damage_24[], pl31_damage_25[], pl31_damage_26[], pl31_damage_26_loop[], pl31_damage_27[],
	pl31_damage_28[], pl31_damage_29[], pl31_damage_30[], pl31_damage_31[], pl31_caught_00[],
	pl31_caught_01[], pl31_caught_02[], pl31_caught_03[], pl31_caught_04[], pl31_caught_05[],
	pl31_caught_06[], pl31_caught_07[], pl31_caught_08[], pl31_caught_09[], pl31_caught_10[],
	pl31_caught_11[], pl31_caught_12[], pl31_caught_13[], pl31_caught_14[], pl31_caught_15[],
	pl31_caught_16[], pl31_caught_17[], pl31_caught_18[], pl31_caught_19[], pl31_caught_20[],
	pl31_caught_21[], pl31_special_00[], pl31_special_01[], pl31_special_10[], pl31_special_11[],
	pl31_super_11[], pl31_special_02[], pl31_special_02_loop[], pl31_special_03[], pl31_special_04[],
	pl31_special_05[], pl31_special_06[], pl31_special_07[], pl31_special_07_loop[],
	pl31_special_08[], pl31_special_09[], pl31_super_00[], pl31_effect_05[], pl31_super_07[],
	pl31_super_08[], pl31_super_09[], pl31_super_10[], pl31_super_01[], pl31_super_02[],
	pl31_super_03[], pl31_super_04[], pl31_super_05[], pl31_super_06[], pl31_extra_02[],
	pl31_extra_03[], pl31_change_04[], pl31_extra_04[], pl31_effect_00[], pl31_unused_00[],
	pl31_effect_01[], pl31_effect_02[], pl31_effect_02_loop[], pl31_effect_03[], pl31_effect_04[],
	pl31_effect_06[], pl31_catch_00[], pl31_catch_00_loop[], pl31_catch_06[], pl31_catch_06_loop[],
	pl31_catch_01[], pl31_catch_02[], pl31_catch_03[], pl31_catch_04[], pl31_catch_04_loop[],
	pl31_catch_05[], pl31_catch_05_loop[], pl31_catch_07[], pl31_catch_08[], pl31_catch_09[],
	pl31_effect_07[], pl31_effect_07_loop[], pl31_effect_08[], pl31_effect_09[], pl31_effect_10[];
extern const Sint32 *const pl31_stand_tbl[], *const pl31_jump_tbl[], *const pl31_dash_tbl[],
	*const pl31_crouch_tbl[], *const pl31_turn_tbl[], *const pl31_guard_tbl[],
	*const pl31_air_guard_tbl[], *const pl31_stand_p_tbl[], *const pl31_stand_k_tbl[],
	*const pl31_crouch_p_tbl[], *const pl31_crouch_k_tbl[], *const pl31_jump_p_tbl[],
	*const pl31_jump_k_tbl[], *const pl31_damage_tbl[], *const pl31_dizzy_tbl[],
	*const pl31_catch_tbl[], *const pl31_caught_tbl[], *const pl31_get_up_tbl[],
	*const pl31_appear_tbl[], *const pl31_win_tbl[], *const pl31_extra_tbl[],
	*const pl31_special_tbl[], *const pl31_super_tbl[], *const pl31_effect_tbl[],
	*const pl31_change_tbl[], *const pl31_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_31[16] = {
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
const Sint32 *const *const char_tbl_31[] = {
	pl31_stand_tbl, pl31_jump_tbl, pl31_dash_tbl, pl31_crouch_tbl, pl31_turn_tbl, pl31_guard_tbl,
	pl31_air_guard_tbl, pl31_stand_p_tbl, pl31_stand_k_tbl, pl31_crouch_p_tbl, pl31_crouch_k_tbl, pl31_jump_p_tbl,
	pl31_jump_k_tbl, pl31_damage_tbl, pl31_dizzy_tbl, pl31_catch_tbl, pl31_caught_tbl, pl31_get_up_tbl,
	pl31_appear_tbl, pl31_win_tbl, pl31_extra_tbl, pl31_special_tbl, pl31_super_tbl, pl31_effect_tbl,
	pl31_change_tbl, pl31_grp25_tbl, 0,
};

const Sint32 *const pl31_stand_tbl[] = {
	pl31_stand_00, pl31_stand_00, pl31_stand_02, pl31_stand_03, pl31_stand_00, pl31_stand_00,
	pl31_stand_06, pl31_stand_07, 0,
};

const Sint32 *const pl31_jump_tbl[] = {
	pl31_jump_00, pl31_jump_01, pl31_jump_02, pl31_jump_03, pl31_jump_03, pl31_jump_03,
	pl31_jump_00, pl31_jump_07, pl31_jump_08, pl31_jump_09, pl31_jump_10, pl31_jump_11,
	0,
};

const Sint32 *const pl31_dash_tbl[] = {
	pl31_dash_00, pl31_dash_01, pl31_dash_02, pl31_dash_03, 0,
};

const Sint32 *const pl31_crouch_tbl[] = {
	pl31_crouch_00, pl31_crouch_01, pl31_crouch_02, 0,
};

const Sint32 *const pl31_turn_tbl[] = {
	pl31_turn_00, pl31_turn_01, 0,
};

const Sint32 *const pl31_guard_tbl[] = {
	pl31_guard_00, pl31_guard_01, pl31_guard_02, pl31_guard_03, pl31_guard_04, pl31_guard_05,
	pl31_guard_06, pl31_guard_07, 0,
};

const Sint32 *const pl31_air_guard_tbl[] = {
	pl31_air_guard_00, pl31_air_guard_01, pl31_air_guard_02, pl31_air_guard_03, pl31_air_guard_04, pl31_air_guard_05,
	0,
};

const Sint32 *const pl31_stand_p_tbl[] = {
	pl31_stand_p_00, pl31_stand_p_01, pl31_stand_p_02, 0,
};

const Sint32 *const pl31_stand_k_tbl[] = {
	pl31_stand_k_00, pl31_stand_k_01, pl31_stand_k_02, 0,
};

const Sint32 *const pl31_crouch_p_tbl[] = {
	pl31_crouch_p_00, pl31_crouch_p_01, pl31_crouch_p_02, 0,
};

const Sint32 *const pl31_crouch_k_tbl[] = {
	pl31_crouch_k_00, pl31_crouch_k_01, pl31_crouch_k_02, 0,
};

const Sint32 *const pl31_jump_p_tbl[] = {
	pl31_jump_p_00, pl31_jump_p_01, pl31_jump_p_02, pl31_jump_p_00, pl31_jump_p_01, pl31_jump_p_02,
	0,
};

const Sint32 *const pl31_jump_k_tbl[] = {
	pl31_jump_k_00, pl31_jump_k_01, pl31_jump_k_02, pl31_jump_k_00, pl31_jump_k_01, pl31_jump_k_02,
	0,
};

const Sint32 *const pl31_damage_tbl[] = {
	pl31_damage_00, pl31_damage_01, pl31_damage_02, pl31_damage_03, pl31_damage_04, pl31_damage_05,
	pl31_damage_06, pl31_damage_07, pl31_damage_08, pl31_damage_09, pl31_damage_10, pl31_damage_11,
	pl31_damage_12, pl31_damage_13, pl31_damage_14, pl31_damage_15, pl31_damage_16, pl31_damage_17,
	pl31_damage_18, pl31_damage_19, pl31_damage_20, pl31_damage_21, pl31_damage_22, pl31_damage_23,
	pl31_damage_24, pl31_damage_25, pl31_damage_26, pl31_damage_27, pl31_damage_28, pl31_damage_29,
	pl31_damage_30, pl31_damage_31, pl31_damage_30, pl31_damage_27, pl31_damage_15, 0,
};

const Sint32 *const pl31_dizzy_tbl[] = {
	pl31_dizzy_00, 0,
};

const Sint32 *const pl31_catch_tbl[] = {
	pl31_catch_00, pl31_catch_01, pl31_catch_02, pl31_catch_03, pl31_catch_04, pl31_catch_05,
	pl31_catch_06, pl31_catch_07, pl31_catch_08, pl31_catch_09, 0,
};

const Sint32 *const pl31_caught_tbl[] = {
	pl31_caught_00, pl31_caught_01, pl31_caught_02, pl31_caught_03, pl31_caught_04, pl31_caught_05,
	pl31_caught_06, pl31_caught_07, pl31_caught_08, pl31_caught_09, pl31_caught_10, pl31_caught_11,
	pl31_caught_12, pl31_caught_13, pl31_caught_14, pl31_caught_15, pl31_caught_16, pl31_caught_17,
	pl31_caught_18, pl31_caught_19, pl31_caught_20, pl31_caught_21, 0,
};

const Sint32 *const pl31_get_up_tbl[] = {
	pl31_get_up_00, pl31_get_up_01, pl31_get_up_02, pl31_get_up_03, 0,
};

const Sint32 *const pl31_appear_tbl[] = {
	pl31_appear_00, pl31_appear_01, pl31_appear_02, 0,
};

const Sint32 *const pl31_win_tbl[] = {
	pl31_win_00, pl31_win_01, pl31_win_02, 0,
};

const Sint32 *const pl31_extra_tbl[] = {
	pl31_extra_00, pl31_extra_01, pl31_extra_02, pl31_extra_03, pl31_extra_04, pl31_extra_05,
	pl31_extra_06, pl31_extra_07, pl31_extra_08, 0,
};

const Sint32 *const pl31_special_tbl[] = {
	pl31_special_00, pl31_special_01, pl31_special_02, pl31_special_03, pl31_special_04, pl31_special_05,
	pl31_special_06, pl31_special_07, pl31_special_08, pl31_special_09, pl31_special_10, pl31_special_11,
	0,
};

const Sint32 *const pl31_super_tbl[] = {
	pl31_super_00, pl31_super_01, pl31_super_02, pl31_super_03, pl31_super_04, pl31_super_05,
	pl31_super_06, pl31_super_07, pl31_super_08, pl31_super_09, pl31_super_10, pl31_super_11,
	0,
};

const Sint32 *const pl31_effect_tbl[] = {
	pl31_effect_00, pl31_effect_01, pl31_effect_02, pl31_effect_03, pl31_effect_04, pl31_effect_05,
	pl31_effect_06, pl31_effect_07, pl31_effect_08, pl31_effect_09, pl31_effect_10, 0,
};

const Sint32 *const pl31_change_tbl[] = {
	pl31_change_00, pl31_change_01, pl31_change_02, pl31_change_03, pl31_change_04, 0,
};

const Sint32 *const pl31_grp25_tbl[] = {
	pl31_stand_00, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0 */
const Sint32 pl31_stand_00[] = {
	CG_FRAME(    0,    0,    4,           0,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_stand_00 ),
};

/* Char_Stand 6 */
const Sint32 pl31_stand_06[] = {
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Stand 7 */
const Sint32 pl31_stand_07[] = {
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_stand_06 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl31_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl31_stand_03[] = {
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_stand_03 ),
};

/* Char_Jump 0
   Char_Jump 6 */
const Sint32 pl31_jump_00[] = {
	CG_FRAME(    0,    1,    2,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl31_jump_09[] = {
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl31_jump_01[] = {
	CG_FRAME(    0,    1,    2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl31_jump_10[] = {
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_jump_10 ),
};

/* Char_Jump 7 */
const Sint32 pl31_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl31_jump_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_jump_07_loop ),
};

/* Char_Jump 2 */
const Sint32 pl31_jump_02[] = {
	CG_FRAME(    0,    1,    2,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl31_jump_11[] = {
	CG_FRAME(    0,    0,    2,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_jump_11 ),
};

/* Char_Jump 8 */
const Sint32 pl31_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl31_jump_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_jump_08_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl31_jump_03[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_jump_03 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl31_dash_00[] = {
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    3,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl31_dash_00 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl31_dash_02[] = {
	CG_FRAME(    0,    0,    8,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl31_dash_01[] = {
	CG_FRAME(    0,    1,    2,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl31_dash_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl31_dash_03[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl31_crouch_00[] = {
	CG_FRAME(    2,    0,    2,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,    0,    2,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_crouch_00 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl31_crouch_02[] = {
	CG_FRAME(    2,    0,    2,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,    2,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_crouch_02 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl31_crouch_01[] = {
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_crouch_01 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl31_turn_00[] = {
	CG_FRAME(    0,    0,    8,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl31_turn_01[] = {
	CG_FRAME(    0,    0,    8,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_turn_01 ),
};

/* Char_Guard 3 */
const Sint32 pl31_guard_03[] = {
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_guard_01 ),
};

/* Char_Guard 0 */
const Sint32 pl31_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl31_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl31_guard_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_guard_02 ),
};

/* Char_Guard 7 */
const Sint32 pl31_guard_07[] = {
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_guard_05 ),
};

/* Char_Guard 4 */
const Sint32 pl31_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl31_guard_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl31_guard_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl31_air_guard_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_air_guard_00 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl31_air_guard_02[] = {
	CG_FRAME(    0,    0,    4,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl31_air_guard_01[] = {
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_air_guard_01 ),
};

/* Char_Air_Guard 3 */
const Sint32 pl31_air_guard_03[] = {
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl31_air_guard_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_air_guard_03_loop ),
};

/* Char_Air_Guard 4 */
const Sint32 pl31_air_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl31_air_guard_04_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_air_guard_04_loop ),
};

/* Char_Air_Guard 5 */
const Sint32 pl31_air_guard_05[] = {
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl31_air_guard_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_air_guard_05_loop ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl31_stand_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   262, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    1,           0,   263, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   264, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   265, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl31_stand_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   268, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   270, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    1,           0,   271, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    3,           0,   273, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   274, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy): stand_hp_init (standing heavy punch (gamma assist)) */
const Sint32 pl31_stand_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   277, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    2,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    2,    0,    3,           0,   281, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   282, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    3,    3,           0,   283, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    4,    0,    4,           0,   284, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    5,    4,           0,   285, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    5,           0,   286, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    6,           0,   287, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    7,           0,   288, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   289, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl31_stand_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl31_stand_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   296, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    1,           0,   297, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   298, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    3,           0,   299, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    2,           0,   300, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   302, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl31_stand_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   303, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   306, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   307, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   308, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    3,           0,   310, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   311, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    1,           0,   312, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   313, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   315, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl31_crouch_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   317, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    1,           0,   318, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    1,    0,    2,           0,   318, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    1,    0,    3, END_OF_DATA,   319, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl31_crouch_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   320, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   323, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0,   324, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   326, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy): crouch_hp_init (crouching heavy punch (gamma counter)) */
const Sint32 pl31_crouch_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   327, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   328, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   329, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   331, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6,           0,   332, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    5,           0,   333, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   334, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl31_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   335, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   336, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   338, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl31_crouch_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   340, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   341, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    7,           0,   342, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   343, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl31_crouch_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   344, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   345, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   346, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   347, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   350, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   351, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    5,           0,   352, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   353, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   355, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl31_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   356, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   16,           0,   357, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    1,           0,   358, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   359, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0,   360, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   361, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_jump_p_00 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl31_jump_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   356, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   363, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    8,           0,   364, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    6,           0,   365, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    5,           0,   366, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4,           0,   367, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   368, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_jump_p_02 ),
};

/* Char_Extra 7 */
const Sint32 pl31_extra_07[] = {
	CG_FRAME(    0,    0,    1,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   371, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   372, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    5,           0,   374, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    7,           0,   375, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    8,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_extra_07 ),
};

/* Char_Extra 6 */
const Sint32 pl31_extra_06[] = {
	CG_FRAME(    0,    0,    1,           0,   396, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   399, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   400, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   402, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_extra_06 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl31_jump_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   403, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   405, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   407, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   409, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_jump_p_01 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl31_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   16,           0,   379, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    1,           0,   380, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    4,           0,   382, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   383, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl31_jump_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   410, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    1,           0,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    5,           0,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_jump_k_01 ),
};

/* Char_Extra 8 */
const Sint32 pl31_extra_08[] = {
	CG_FRAME(    0,    0,    6,           0,   384, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   385, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    5,           0,   386, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    5,           0,   387, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   388, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_extra_08 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl31_jump_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   392, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    6,           0,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   395, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_jump_k_02 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl31_dizzy_00[] = {
	CG_FRAME(    0,    0,    4,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   685, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   686, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   687, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   683, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   684, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_dizzy_00 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl31_get_up_00[] = {
	CG_FRAME(    0,    0,    3,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl31_get_up_01[] = {
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl31_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl31_get_up_03[] = {
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_get_up_03 ),
};

/* Char_Win 0: win_ko */
const Sint32 pl31_win_00[] = {
	CG_FRAME(    0,    0,    8,           0,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   245, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_win_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_win_00_loop ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl31_win_02[] = {
	CG_FRAME(    0,    0,    8,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_win_02_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_win_02_loop ),
};

/* Char_Win 1: win_time */
const Sint32 pl31_win_01[] = {
	CG_FRAME(    0,    1,    8,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_win_01 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl31_appear_00[] = {
	CG_FRAME(    0,    0,    8,           0,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_appear_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_appear_00_loop ),
};

/* Char_Appear 1: appear_wait */
const Sint32 pl31_appear_01[] = {
	CG_FRAME(    0,    0,    3,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_appear_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_appear_01_loop ),
};

/* Char_Appear 2: appear_fall */
const Sint32 pl31_appear_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   243, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_appear_02 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl31_change_00[] = {
	CG_FRAME(    0,    0,    6,           0,   244, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   245, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   246, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   247, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   247, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl31_change_02[] = {
	CG_FRAME(    0,    0,    1,           0,   130, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   131, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl31_change_03[] = {
	CG_FRAME(    0,    0,    6,           0,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   245, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl31_change_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_change_03_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl31_change_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,    36, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,    36, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,    36, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_change_01 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl31_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   413, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl31_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl31_extra_01[] = {
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   244, 0x80,   0x00,  ATE,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   245, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   246, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   247, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   148, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_extra_01 ),
};

/* Char_Extra 5: pl31_upper */
const Sint32 pl31_extra_05[] = {
	CG_FRAME(    0,    0,    1,           0,   344, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   345, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   346, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   347, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   350, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   351, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    5,           0,   352, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   353, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   355, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_extra_05 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl31_damage_00[] = {
	CG_FRAME(    0,    0,    5,           0,   568, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   569, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   570, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl31_damage_01[] = {
	CG_FRAME(    0,    0,    6,           0,   572, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   573, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   574, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   575, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl31_damage_02[] = {
	CG_FRAME(    0,    0,    4,           0,   577, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   578, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   579, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   580, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   581, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   582, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   583, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl31_damage_03[] = {
	CG_FRAME(    0,    0,    2,           0,   585, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   586, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   587, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl31_damage_04[] = {
	CG_FRAME(    0,    0,    2,           0,   589, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   590, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   591, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   592, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl31_damage_05[] = {
	CG_FRAME( 0x80,    0,    4,           0,   594, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   10,           0,   595, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   596, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   597, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   598, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   599, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   600, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl31_damage_06[] = {
	CG_FRAME(    0,    0,    3,           0,   602, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   603, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   604, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl31_damage_07[] = {
	CG_FRAME(    0,    0,    2,           0,   606, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   10,           0,   607, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   608, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   609, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl31_damage_08[] = {
	CG_FRAME(    0,    0,    4,           0,   611, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   10,           0,   612, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   613, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   614, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   615, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   616, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   617, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl31_damage_10[] = {
	CG_FRAME(    0,    0,    3,           0,   619, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   620, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   621, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl31_damage_11[] = {
	CG_FRAME(    0,    0,    3,           0,   623, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   624, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   625, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl31_damage_12[] = {
	CG_FRAME(    0,    0,    3,           0,   627, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   628, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   629, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_12 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl31_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   633, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   634, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   635, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   639, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl31_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl31_damage_09_loop ),
};

/* Char_Damage 13: dm_snap_fly, dm_air */
const Sint32 pl31_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   631, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   632, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl31_damage_13 ),
};

/* Char_Damage 14 */
const Sint32 pl31_damage_14[] = {
	CG_FRAME(    0,    0,    4,           0,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   704, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl31_damage_14 ),
};

/* Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl31_damage_15[] = {
	CG_FRAME(    0,    0,    4,           0,   706, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl31_damage_15 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl31_damage_16[] = {
	CG_FRAME(    0,    0,    4,           0,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl31_damage_17[] = {
	CG_FRAME(    0,    0,    4,           0,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   644, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl31_damage_18[] = {
	CG_FRAME(    0,    0,    4,           0,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   646, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl31_damage_19[] = {
	CG_FRAME(    0,    0,    4,           0,   647, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   648, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl31_damage_20[] = {
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl31_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl31_damage_22[] = {
	CG_FRAME(    0,    0,    2,           0,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   644, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl31_damage_23[] = {
	CG_FRAME(    0,    0,    2,           0,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   646, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl31_damage_24[] = {
	CG_FRAME(    0,    0,    2,           0,   647, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   648, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl31_damage_25[] = {
	CG_FRAME(    0,    0,    2,           0,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl31_damage_26[] = {
	CG_FRAME(    0,    0,    1,           0,   651, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   652, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   653, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   654, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   655, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl31_damage_26_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl31_damage_26_loop ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl31_damage_27[] = {
	CG_FRAME(    0,    0,    6,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl31_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl31_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   673, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl31_damage_30[] = {
	CG_FRAME(    0,    0,    2,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl31_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl31_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl31_damage_31 ),
};

/* Char_Caught 0 */
const Sint32 pl31_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   569, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl31_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   573, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl31_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   578, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl31_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   586, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl31_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   590, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl31_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   595, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl31_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   603, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl31_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl31_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   607, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl31_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl31_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   612, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl31_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl31_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   620, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl31_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   624, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl31_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   628, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl31_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   632, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl31_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl31_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl31_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl31_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl31_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl31_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl31_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl31_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   647, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl31_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl31_caught_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl31_caught_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl31_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl31_caught_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl31_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl31_caught_21[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl31_caught_21 ),
};

/* Char_Special 0: sword_init (Dancing Sword) */
const Sint32 pl31_special_00[] = {
	CG_FRAME(    0,    0,    6,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8,           0,   495, 0x80,   0x00,    0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   32, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_00 ),
};

/* Char_Special 1: tobashi_init (Tsurugi Tobashi) */
const Sint32 pl31_special_01[] = {
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   15,           0,   498, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   499, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_01 ),
};

/* Char_Special 10: swords_init (the swords out and fired at once (alpha and beta assist and counter)) */
const Sint32 pl31_special_10[] = {
	CG_FRAME(    0,    0,    6,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8,           0,   495, 0x80,   0x00,    0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_10 ),
};

/* Char_Special 11: swords_move (the swords out and fired at once (alpha and beta assist and counter)) */
const Sint32 pl31_special_11[] = {
	CG_FRAME(    0,    0,    1,           0,   494, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   499, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_11 ),
};

/* Char_Super 11: stampede_init (Stampede Sword (hyper)) */
const Sint32 pl31_super_11[] = {
	CG_FRAME(    0,    0,    6,           0,   495, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   495, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   495, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0, -128,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   496, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   499, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_super_11 ),
};

/* Char_Special 2: tele_init (Teleport Dance) */
const Sint32 pl31_special_02[] = {
	CG_FRAME(    0,    0,    2,           0,   481, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   488, 0x80,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_special_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_special_02_loop ),
};

/* Char_Special 3: tele_wait (Teleport Dance) */
const Sint32 pl31_special_03[] = {
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   487, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    5, END_OF_DATA,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_03 ),
};

/* Char_Special 4: tele_land (Teleport Dance) */
const Sint32 pl31_special_04[] = {
	CG_FRAME(    0,    0,    2,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   470, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_04 ),
};

/* Char_Special 5: sp_power (Power Dance (hyper)) */
const Sint32 pl31_special_05[] = {
	CG_FRAME(    0,    0,    8,           0,   456, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   457, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   458, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   12, END_OF_DATA,   459, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_05 ),
};

/* Char_Special 6: sp_speed (Speed Dance (hyper)) */
const Sint32 pl31_special_06[] = {
	CG_FRAME(    0,    0,    8,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   12,           0,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   470, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_special_06 ),
};

/* Char_Special 7: grapple_init (Six-hand Grapple) */
const Sint32 pl31_special_07[] = {
	CG_FRAME(    0,    0,    1,           0,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl31_special_07_loop[] = {
	CG_FRAME(    0,    1,   10,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,   12,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,   10,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,   12,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,   10,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,   12, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl31_special_07_loop ),
};

/* Char_Special 8 */
const Sint32 pl31_special_08[] = {
	CG_FRAME(    0,    1,    4,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl31_special_08 ),
};

/* Char_Special 9: grapple_move (Six-hand Grapple) */
const Sint32 pl31_special_09[] = {
	CG_FRAME(    0,    0,    5,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_special_09 ),
};

/* Char_Super 0: metamor_maxspeed_init (Metamorphose (level 3 hyper): the speed up, then a punch's hyper throw) */
const Sint32 pl31_super_00[] = {
	CG_FRAME(    0,    0,    2,           0,   507, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   509, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3,           0,   509, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   509, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   510, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_super_00 ),
};

/* Char_Effect 5: pls31_00_metamor_init */
const Sint32 pl31_effect_05[] = {
	CG_FRAME(    0,    0,    4,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_05 ),
};

/* Char_Super 7 */
const Sint32 pl31_super_07[] = {
	CG_FRAME(    0,    0,    1,           0,   419, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    2,    4,           0,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    3,    1,    4,           0,   517, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    5,    2,    4,           0,   518, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    6,    1,    4,           0,   519, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    7,    2,    4,           0,   520, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    8,    1,    4,           0,   521, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    9,    2,    4,           0,   522, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   10,    1,    4,           0,   523, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   11,    2,    4,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   13,    1,    4,           0,   525, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   15,    2,    4,           0,   526, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   17,    1,    4,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   18,    2,    4,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   19,    1,    4,           0,   529, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   21,    2,    4,           0,   530, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   22,    1,    4,           0,   531, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   24,    2,    4,           0,   532, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   25,    1,    4,           0,   533, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   26,    2,    4,           0,   534, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   27,    1,    4,           0,   535, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   28,    2,    4,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   29,    1,    4,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   30,    2,    4,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   32,    1,    4,           0,   539, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   33,    1,    4,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    3,   16, END_OF_DATA,   515, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl31_super_07 ),
};

/* Char_Super 8 */
const Sint32 pl31_super_08[] = {
	CG_FRAME(    0,    0,    1,           0,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    4,           0,   541, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    3,    1,    4,           0,   543, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    5,    2,    4,           0,   544, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    6,    1,    4,           0,   545, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    7,    2,    4,           0,   546, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    8,    1,    4,           0,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    9,    2,    4,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   10,    1,    4,           0,   549, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   11,    2,    4,           0,   550, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   13,    1,    4,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   15,    2,    4,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   17,    1,    4,           0,   553, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   18,    2,    4,           0,   554, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   19,    1,    4,           0,   555, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   21,    2,    4,           0,   556, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   22,    1,    4,           0,   557, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   24,    2,    4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   25,    1,    4,           0,   559, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   26,    2,    4,           0,   560, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   27,    1,    4,           0,   561, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   28,    2,    4,           0,   562, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   29,    1,    4,           0,   563, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   30,    2,    4,           0,   564, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(   32,    1,    4,           0,   565, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(   33,    1,    4,           0,   566, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    1,    3,   16, END_OF_DATA,   542, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl31_super_08 ),
};

/* Char_Super 9: hmetamor_init (Hyper Metamorphose: the gamma type's team hyper (this command is not checked)) */
const Sint32 pl31_super_09[] = {
	CG_FRAME(    0,    0,    2,           0,   507, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   509, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   509, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   509, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   510, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    2,    4,           0,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    3,    1,    4,           0,   517, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    5,    2,    4,           0,   518, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    6,    1,    4,           0,   519, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    7,    2,    4,           0,   520, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    8,    1,    4,           0,   521, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    9,    2,    4,           0,   522, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   10,    1,    4,           0,   523, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   11,    2,    4,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   13,    1,    4,           0,   525, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   15,    2,    4,           0,   526, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   17,    1,    4,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   18,    2,    4,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   19,    1,    4,           0,   529, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   21,    2,    4,           0,   530, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   22,    1,    4,           0,   531, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   24,    2,    4,           0,   532, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   25,    1,    4,           0,   533, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   26,    2,    4,           0,   534, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(   27,    1,    4,           0,   535, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   28,    2,    4,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   29,    1,    4,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(   30,    2,    4,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   32,    1,    4,           0,   539, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(   33,    1,    4,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    3,   16, END_OF_DATA,   515, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl31_super_09 ),
};

/* Char_Super 10: hmetamor_hit (Hyper Metamorphose: the gamma type's team hyper (this command is not checked)) */
const Sint32 pl31_super_10[] = {
	CG_FRAME(    0,    0,    8,           0,   450, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   451, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   452, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_super_10 ),
};

/* Char_Super 1 */
const Sint32 pl31_super_01[] = {
	CG_FRAME(    0,    0,    3,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   418, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    3,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl31_super_02[] = {
	CG_FRAME(    0,    0,    6,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   10,           0,   418, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    6,           0,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    6,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl31_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl31_super_03[] = {
	CG_FRAME(    0,    0,    3,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   424, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    3,           0,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    3,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   428, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_super_03 ),
};

/* Char_Super 4 */
const Sint32 pl31_super_04[] = {
	CG_FRAME(    0,    0,    6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   10,           0,   424, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    6,           0,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    6,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   428, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl31_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl31_super_05[] = {
	CG_FRAME(    0,    0,    3,           0,   429, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    5,           0,   430, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    3,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_super_05 ),
};

/* Char_Super 6 */
const Sint32 pl31_super_06[] = {
	CG_FRAME(    0,    0,    6,           0,   429, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   10,           0,   430, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    6,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    6,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl31_super_06 ),
};

/* Char_Extra 2: snapback_init (the snapback), fly_start5 */
const Sint32 pl31_extra_02[] = {
	CG_FRAME(    0,    0,    5,           0,   327, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    6,           0,   329, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   331, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6,           0,   332, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    5,           0,   333, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   334, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl31_extra_02 ),
};

/* Char_Extra 3: sp_chouhatsu (the taunt), fly_start5 */
const Sint32 pl31_extra_03[] = {
	CG_FRAME(    0,    0,    8,           0,   450, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   451, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   452, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8, END_OF_DATA,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_extra_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl31_change_04[] = {
	CG_FRAME(    0,    0,    8,           0,   450, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   451, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   452, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   453, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_change_04 ),
};

/* Char_Extra 4 */
const Sint32 pl31_extra_04[] = {
	CG_FRAME(    0,    0,    4,           0,   472, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   473, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   474, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   477, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   478, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   479, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   479, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl31_extra_04 ),
};

/* Char_Effect 0 */
const Sint32 pl31_effect_00[] = {
	CG_FRAME(    0,    0,    2,           0,   713, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   714, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   715, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   716, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   719, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   720, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   721, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   722, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   723, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   724, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_00 ),
};

const Sint32 pl31_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   31,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   499, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00004e60,
};

/* Char_Effect 1: shl31_00_init, shl31_01_init */
const Sint32 pl31_effect_01[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_01 ),
};

/* Char_Effect 2: shl31_00_stick, shl31_01_stick */
const Sint32 pl31_effect_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32769, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_effect_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32768, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_02_loop ),
};

/* Char_Effect 3: shl31_00_out, shl31_00_fire_front, shl31_00_fire_orbit, shl31_00_fire_curve, ... */
const Sint32 pl31_effect_03[] = {
	CG_FRAME(    0,    0,    2,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl31_effect_03 ),
};

/* Char_Effect 4: shl31_00_shake, shl31_01_shake */
const Sint32 pl31_effect_04[] = {
	CG_FRAME(    0,    0,    4,           0, 32792, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32799, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32800, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32801, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32788, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32789, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32790, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32791, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_04 ),
};

/* Char_Effect 6: pls31_00_win_init */
const Sint32 pl31_effect_06[] = {
	CG_FRAME(    0,    0,    4,           0,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   710, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   711, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   712, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_06 ),
};

/* Char_Catch 0: jump_p_set */
const Sint32 pl31_catch_00[] = {
	CG_FRAME(    0,    0,   16,           0,   434, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    6,           0,   435, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    8,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,    2,           0,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
};

const Sint32 pl31_catch_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_NEXT( pl31_catch_00_loop ),
};

/* Char_Catch 6: grapple_set */
const Sint32 pl31_catch_06[] = {
	CG_FRAME(    0,    0,   16,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,    6,           0,   435, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    8,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,    2,           0,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
};

const Sint32 pl31_catch_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   437, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_NEXT( pl31_catch_06_loop ),
};

/* Char_Catch 1 */
const Sint32 pl31_catch_01[] = {
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    1,   16,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   443, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_jump_09 ),
};

/* Char_Catch 2 */
const Sint32 pl31_catch_02[] = {
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    1,   16,           0,   442, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   443, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_jump_09 ),
};

/* Char_Catch 3 */
const Sint32 pl31_catch_03[] = {
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    1,   16,           0,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   443, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_jump_09 ),
};

/* Char_Catch 4: nml_p_set */
const Sint32 pl31_catch_04[] = {
	CG_FRAME(    6,    0,   16,           0,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
};

const Sint32 pl31_catch_04_loop[] = {
	CG_FRAME(    0,    0,   10,           0,   444, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,   10,           0,   445, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   10,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,   10,           0,   447, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   10,           0,   448, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   10, END_OF_DATA,   449, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_NEXT( pl31_catch_04_loop ),
};

/* Char_Catch 5: jump_k_set */
const Sint32 pl31_catch_05[] = {
	CG_FRAME(    0,    0,   12,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    1,    1,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,    2,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_catch_05_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_catch_05_loop ),
};

/* Char_Catch 7 */
const Sint32 pl31_catch_07[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,  80 ),
	CG_NEXT( pl31_catch_07 ),
};

/* Char_Catch 8 */
const Sint32 pl31_catch_08[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0017,  80 ),
	CG_NEXT( pl31_catch_08 ),
};

/* Char_Catch 9 */
const Sint32 pl31_catch_09[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0018,  80 ),
	CG_NEXT( pl31_catch_09 ),
};

/* Char_Effect 7: pls31_00_appear_init */
const Sint32 pl31_effect_07[] = {
	CG_FRAME(    0,    0,    4,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl31_effect_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   729, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_07_loop ),
};

/* Char_Effect 8 */
const Sint32 pl31_effect_08[] = {
	CG_FRAME(    0,    0,    2,           0,   730, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   731, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   732, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   733, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl31_effect_09[] = {
	CG_FRAME(    0,    0,    2,           0,   734, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   736, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   737, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_09 ),
};

/* Char_Effect 10: pls31_00_appear_wait */
const Sint32 pl31_effect_10[] = {
	CG_FRAME(    0,    0,    4,           0,   729, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl31_effect_10 ),
};
