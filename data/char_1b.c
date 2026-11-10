/* Chun Li (character 1b): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_1b      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_1b     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl1b.c) says which group
                   and number each move uses.
   pl1b_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl1b_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl1b_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl1b_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_1b
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_1b, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_1b */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl1b_stand_00[], pl1b_stand_02[], pl1b_stand_03[], pl1b_stand_06[],
	pl1b_stand_07[], pl1b_jump_00[], pl1b_jump_00_loop[], pl1b_jump_01[], pl1b_jump_01_loop[],
	pl1b_jump_02[], pl1b_jump_02_loop[], pl1b_jump_03[], pl1b_jump_06[], pl1b_jump_09[],
	pl1b_dash_00[], pl1b_dash_04[], pl1b_dash_04_loop[], pl1b_dash_02[], pl1b_dash_01[],
	pl1b_dash_03[], pl1b_crouch_00[], pl1b_crouch_01[], pl1b_crouch_02[], pl1b_turn_00[],
	pl1b_turn_01[], pl1b_guard_00[], pl1b_guard_01[], pl1b_guard_02[], pl1b_guard_04[],
	pl1b_guard_05[], pl1b_guard_06[], pl1b_stand_p_00[], pl1b_stand_p_01[], pl1b_stand_p_02[],
	pl1b_stand_k_00[], pl1b_stand_k_01[], pl1b_stand_k_02[], pl1b_air_guard_00[],
	pl1b_air_guard_01[], pl1b_crouch_p_00[], pl1b_crouch_p_01[], pl1b_crouch_p_02[],
	pl1b_crouch_k_00[], pl1b_crouch_k_01[], pl1b_crouch_k_02[], pl1b_crouch_k_03[],
	pl1b_crouch_k_03_loop[], pl1b_unused_00[], pl1b_crouch_k_04[], pl1b_jump_p_00[],
	pl1b_jump_p_01[], pl1b_jump_p_02[], pl1b_jump_k_00[], pl1b_jump_k_03[], pl1b_unused_01[],
	pl1b_jump_12[], pl1b_jump_13[], pl1b_jump_13_loop[], pl1b_jump_14[], pl1b_jump_14_loop[],
	pl1b_jump_15[], pl1b_jump_15_loop[], pl1b_jump_k_01[], pl1b_jump_k_02[], pl1b_jump_k_05[],
	pl1b_get_up_00[], pl1b_get_up_01[], pl1b_get_up_02[], pl1b_get_up_03[], pl1b_damage_00[],
	pl1b_damage_01[], pl1b_damage_02[], pl1b_damage_03[], pl1b_damage_04[], pl1b_damage_05[],
	pl1b_damage_06[], pl1b_damage_07[], pl1b_damage_08[], pl1b_damage_10[], pl1b_damage_11[],
	pl1b_damage_12[], pl1b_damage_13[], pl1b_damage_09[], pl1b_damage_09_loop[], pl1b_damage_16[],
	pl1b_damage_17[], pl1b_damage_18[], pl1b_damage_19[], pl1b_damage_20[], pl1b_damage_21[],
	pl1b_damage_22[], pl1b_damage_23[], pl1b_damage_24[], pl1b_damage_25[], pl1b_damage_26[],
	pl1b_damage_27[], pl1b_damage_30[], pl1b_damage_32[], pl1b_damage_33[], pl1b_unused_02[],
	pl1b_damage_28[], pl1b_damage_29[], pl1b_damage_31[], pl1b_dizzy_00[], pl1b_special_00[],
	pl1b_special_25[], pl1b_extra_00[], pl1b_extra_00_loop[], pl1b_extra_01[], pl1b_special_01[],
	pl1b_special_01_loop[], pl1b_special_02[], pl1b_special_02_loop[], pl1b_special_26[],
	pl1b_special_04[], pl1b_unused_03[], pl1b_special_05[], pl1b_unused_04[], pl1b_special_07[],
	pl1b_special_07_loop[], pl1b_special_10[], pl1b_special_08[], pl1b_special_08_loop[],
	pl1b_special_12[], pl1b_special_13[], pl1b_special_13_loop[], pl1b_special_14[],
	pl1b_special_14_loop[], pl1b_special_16[], pl1b_special_17[], pl1b_special_19[],
	pl1b_special_19_loop[], pl1b_special_22[], pl1b_super_00[], pl1b_super_01[], pl1b_super_04[],
	pl1b_extra_02[], pl1b_extra_03[], pl1b_unused_05[], pl1b_extra_04[], pl1b_effect_00[],
	pl1b_effect_01[], pl1b_effect_02[], pl1b_effect_03[], pl1b_extra_08[], pl1b_extra_09[],
	pl1b_extra_11[], pl1b_extra_12[], pl1b_extra_13[], pl1b_extra_05[], pl1b_effect_04[],
	pl1b_effect_05[], pl1b_effect_06[], pl1b_effect_07[], pl1b_effect_08[], pl1b_effect_08_loop[],
	pl1b_super_05[], pl1b_super_05_loop[], pl1b_super_06[], pl1b_super_08[], pl1b_super_09[],
	pl1b_super_09_loop[], pl1b_catch_02[], pl1b_change_01[], pl1b_change_02[], pl1b_change_03[],
	pl1b_change_03_loop[], pl1b_change_04[], pl1b_win_00[], pl1b_win_00_loop[], pl1b_win_01[],
	pl1b_win_01_loop[], pl1b_win_02[], pl1b_win_02_loop[], pl1b_win_03[], pl1b_win_03_loop[],
	pl1b_win_05[], pl1b_win_05_loop[], pl1b_win_04[], pl1b_win_04_loop[], pl1b_extra_07[],
	pl1b_catch_00[], pl1b_catch_01[], pl1b_caught_00[], pl1b_caught_01[], pl1b_caught_02[],
	pl1b_caught_03[], pl1b_caught_04[], pl1b_caught_05[], pl1b_caught_06[], pl1b_caught_07[],
	pl1b_caught_08[], pl1b_caught_09[], pl1b_caught_10[], pl1b_caught_11[], pl1b_caught_12[],
	pl1b_caught_13[], pl1b_caught_14[], pl1b_caught_15[], pl1b_caught_16[], pl1b_caught_17[],
	pl1b_caught_18[], pl1b_caught_19[], pl1b_caught_20[], pl1b_caught_21[], pl1b_appear_00[],
	pl1b_extra_06[], pl1b_special_27[], pl1b_special_27_loop[], pl1b_special_28[];
extern const Sint32 *const pl1b_stand_tbl[], *const pl1b_jump_tbl[], *const pl1b_dash_tbl[],
	*const pl1b_crouch_tbl[], *const pl1b_turn_tbl[], *const pl1b_guard_tbl[],
	*const pl1b_air_guard_tbl[], *const pl1b_stand_p_tbl[], *const pl1b_stand_k_tbl[],
	*const pl1b_crouch_p_tbl[], *const pl1b_crouch_k_tbl[], *const pl1b_jump_p_tbl[],
	*const pl1b_jump_k_tbl[], *const pl1b_damage_tbl[], *const pl1b_dizzy_tbl[],
	*const pl1b_catch_tbl[], *const pl1b_caught_tbl[], *const pl1b_get_up_tbl[],
	*const pl1b_appear_tbl[], *const pl1b_win_tbl[], *const pl1b_extra_tbl[],
	*const pl1b_special_tbl[], *const pl1b_super_tbl[], *const pl1b_effect_tbl[],
	*const pl1b_change_tbl[], *const pl1b_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_1b[16] = {
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
const Sint32 *const *const char_tbl_1b[] = {
	pl1b_stand_tbl, pl1b_jump_tbl, pl1b_dash_tbl, pl1b_crouch_tbl, pl1b_turn_tbl, pl1b_guard_tbl,
	pl1b_air_guard_tbl, pl1b_stand_p_tbl, pl1b_stand_k_tbl, pl1b_crouch_p_tbl, pl1b_crouch_k_tbl, pl1b_jump_p_tbl,
	pl1b_jump_k_tbl, pl1b_damage_tbl, pl1b_dizzy_tbl, pl1b_catch_tbl, pl1b_caught_tbl, pl1b_get_up_tbl,
	pl1b_appear_tbl, pl1b_win_tbl, pl1b_extra_tbl, pl1b_special_tbl, pl1b_super_tbl, pl1b_effect_tbl,
	pl1b_change_tbl, pl1b_grp25_tbl, 0,
};

const Sint32 *const pl1b_stand_tbl[] = {
	pl1b_stand_00, pl1b_stand_00, pl1b_stand_02, pl1b_stand_03, pl1b_stand_00, pl1b_stand_00,
	pl1b_stand_06, pl1b_stand_07, pl1b_stand_02, pl1b_stand_03, 0,
};

const Sint32 *const pl1b_jump_tbl[] = {
	pl1b_jump_00, pl1b_jump_01, pl1b_jump_02, pl1b_jump_03, pl1b_jump_03, pl1b_jump_03,
	pl1b_jump_06, pl1b_jump_06, pl1b_jump_06, pl1b_jump_09, pl1b_jump_09, pl1b_jump_09,
	pl1b_jump_12, pl1b_jump_13, pl1b_jump_14, pl1b_jump_15, 0,
};

const Sint32 *const pl1b_dash_tbl[] = {
	pl1b_dash_00, pl1b_dash_01, pl1b_dash_02, pl1b_dash_03, pl1b_dash_04, 0,
};

const Sint32 *const pl1b_crouch_tbl[] = {
	pl1b_crouch_00, pl1b_crouch_01, pl1b_crouch_02, 0,
};

const Sint32 *const pl1b_turn_tbl[] = {
	pl1b_turn_00, pl1b_turn_01, 0,
};

const Sint32 *const pl1b_guard_tbl[] = {
	pl1b_guard_00, pl1b_guard_01, pl1b_guard_02, pl1b_guard_01, pl1b_guard_04, pl1b_guard_05,
	pl1b_guard_06, pl1b_guard_05, 0,
};

const Sint32 *const pl1b_air_guard_tbl[] = {
	pl1b_air_guard_00, pl1b_air_guard_01, pl1b_air_guard_01, 0,
};

const Sint32 *const pl1b_stand_p_tbl[] = {
	pl1b_stand_p_00, pl1b_stand_p_01, pl1b_stand_p_02, 0,
};

const Sint32 *const pl1b_stand_k_tbl[] = {
	pl1b_stand_k_00, pl1b_stand_k_01, pl1b_stand_k_02, 0,
};

const Sint32 *const pl1b_crouch_p_tbl[] = {
	pl1b_crouch_p_00, pl1b_crouch_p_01, pl1b_crouch_p_02, 0,
};

const Sint32 *const pl1b_crouch_k_tbl[] = {
	pl1b_crouch_k_00, pl1b_crouch_k_01, pl1b_crouch_k_02, pl1b_crouch_k_03, pl1b_crouch_k_04, 0,
};

const Sint32 *const pl1b_jump_p_tbl[] = {
	pl1b_jump_p_00, pl1b_jump_p_01, pl1b_jump_p_02, pl1b_jump_p_00, pl1b_jump_p_01, pl1b_jump_p_02,
	0,
};

const Sint32 *const pl1b_jump_k_tbl[] = {
	pl1b_jump_k_00, pl1b_jump_k_01, pl1b_jump_k_02, pl1b_jump_k_03, pl1b_jump_k_01, pl1b_jump_k_05,
	pl1b_jump_k_03, 0,
};

const Sint32 *const pl1b_damage_tbl[] = {
	pl1b_damage_00, pl1b_damage_01, pl1b_damage_02, pl1b_damage_03, pl1b_damage_04, pl1b_damage_05,
	pl1b_damage_06, pl1b_damage_07, pl1b_damage_08, pl1b_damage_09, pl1b_damage_10, pl1b_damage_11,
	pl1b_damage_12, pl1b_damage_13, pl1b_damage_13, pl1b_damage_13, pl1b_damage_16, pl1b_damage_17,
	pl1b_damage_18, pl1b_damage_19, pl1b_damage_20, pl1b_damage_21, pl1b_damage_22, pl1b_damage_23,
	pl1b_damage_24, pl1b_damage_25, pl1b_damage_26, pl1b_damage_27, pl1b_damage_28, pl1b_damage_29,
	pl1b_damage_30, pl1b_damage_31, pl1b_damage_32, pl1b_damage_33, pl1b_damage_13, 0,
};

const Sint32 *const pl1b_dizzy_tbl[] = {
	pl1b_dizzy_00, 0,
};

const Sint32 *const pl1b_catch_tbl[] = {
	pl1b_catch_00, pl1b_catch_01, pl1b_catch_02, 0,
};

const Sint32 *const pl1b_caught_tbl[] = {
	pl1b_caught_00, pl1b_caught_01, pl1b_caught_02, pl1b_caught_03, pl1b_caught_04, pl1b_caught_05,
	pl1b_caught_06, pl1b_caught_07, pl1b_caught_08, pl1b_caught_09, pl1b_caught_10, pl1b_caught_11,
	pl1b_caught_12, pl1b_caught_13, pl1b_caught_14, pl1b_caught_15, pl1b_caught_16, pl1b_caught_17,
	pl1b_caught_18, pl1b_caught_19, pl1b_caught_20, pl1b_caught_21, 0,
};

const Sint32 *const pl1b_get_up_tbl[] = {
	pl1b_get_up_00, pl1b_get_up_01, pl1b_get_up_02, pl1b_get_up_03, 0,
};

const Sint32 *const pl1b_appear_tbl[] = {
	pl1b_appear_00, 0,
};

const Sint32 *const pl1b_win_tbl[] = {
	pl1b_win_00, pl1b_win_01, pl1b_win_02, pl1b_win_03, pl1b_win_04, pl1b_win_05,
	0,
};

const Sint32 *const pl1b_extra_tbl[] = {
	pl1b_extra_00, pl1b_extra_01, pl1b_extra_02, pl1b_extra_03, pl1b_extra_04, pl1b_extra_05,
	pl1b_extra_06, pl1b_extra_07, pl1b_extra_08, pl1b_extra_09, pl1b_win_05, pl1b_extra_11,
	pl1b_extra_12, pl1b_extra_13, 0,
};

const Sint32 *const pl1b_special_tbl[] = {
	pl1b_special_00, pl1b_special_01, pl1b_special_02, pl1b_special_02, pl1b_special_04, pl1b_special_05,
	pl1b_special_05, pl1b_special_07, pl1b_special_08, pl1b_special_08, pl1b_special_10, pl1b_special_08,
	pl1b_special_12, pl1b_special_13, pl1b_special_14, pl1b_special_14, pl1b_special_16, pl1b_special_17,
	pl1b_special_17, pl1b_special_19, pl1b_special_19, pl1b_special_19, pl1b_special_22, pl1b_special_22,
	pl1b_special_22, pl1b_special_25, pl1b_special_26, pl1b_special_27, pl1b_special_28, 0,
};

const Sint32 *const pl1b_super_tbl[] = {
	pl1b_super_00, pl1b_super_01, pl1b_super_01, pl1b_super_01, pl1b_super_04, pl1b_super_05,
	pl1b_super_06, pl1b_super_06, pl1b_super_08, pl1b_super_09, pl1b_catch_02, pl1b_catch_02,
	pl1b_catch_02, pl1b_catch_02, pl1b_catch_02, pl1b_catch_02, 0,
};

const Sint32 *const pl1b_effect_tbl[] = {
	pl1b_effect_00, pl1b_effect_01, pl1b_effect_02, pl1b_effect_03, pl1b_effect_04, pl1b_effect_05,
	pl1b_effect_06, pl1b_effect_07, pl1b_effect_08, pl1b_appear_00, pl1b_appear_00, pl1b_appear_00,
	pl1b_appear_00, pl1b_appear_00, pl1b_appear_00, pl1b_appear_00, pl1b_appear_00, 0,
};

const Sint32 *const pl1b_change_tbl[] = {
	pl1b_catch_02, pl1b_change_01, pl1b_change_02, pl1b_change_03, pl1b_change_04, 0,
};

const Sint32 *const pl1b_grp25_tbl[] = {
	pl1b_stand_00, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0 */
const Sint32 pl1b_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,     8, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,     9, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    10, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    11, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    12, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    13, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    14, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    15, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,     9, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    16, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,     9, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    10, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    11, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    12, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    13, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    14, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    15, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,     9, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 8 */
const Sint32 pl1b_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,    17, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 9 */
const Sint32 pl1b_stand_03[] = {
	CG_FRAME(    0,    0,    4,           0,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    28, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    29, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    30, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    29, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    28, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl1b_stand_06[] = {
	CG_FRAME(    0,    0,    8,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    13, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl1b_stand_07[] = {
	CG_FRAME(    0,    0,    8,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    13, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl1b_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl1b_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1b_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl1b_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl1b_jump_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1b_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl1b_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl1b_jump_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl1b_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl1b_jump_03[] = {
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl1b_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: shichisei_throw (Shichisei Ranka (air hyper)), change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl1b_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl1b_dash_00[] = {
	CG_FRAME(    0,    0,    1,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,    31, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,    32, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,    31, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,    32, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0,    33, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_dash_00 ),
};

/* Char_Dash 4: fly_start_init, fly_start2_init */
const Sint32 pl1b_dash_04[] = {
	CG_FRAME(    0,    0,    1,           0,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3,           0,   133, 0x80,   0x20,                0,  0x00,       0x03,    0,   0,     0,      0,  77 ),
};

const Sint32 pl1b_dash_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl1b_dash_04_loop ),
};

/* Char_Dash 2: dash_stop */
const Sint32 pl1b_dash_02[] = {
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl1b_dash_01[] = {
	CG_FRAME(    0,    1,    2,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    1,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    4,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    5,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    5,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    2,           0,    42, 0x80,   0x00,                0,  0x00, 0x34 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_dash_01 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl1b_dash_03[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl1b_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl1b_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl1b_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl1b_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl1b_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,    72, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,    73, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    74, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl1b_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl1b_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl1b_guard_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl1b_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,    77, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl1b_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    78, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl1b_guard_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    77, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_guard_06 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl1b_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   147, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    3,           0,   148, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   147, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl1b_stand_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    5,           0,   151, 0x80,   0x4d,              ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   150, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl1b_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl1b_stand_p_02[] = {
	CG_FRAME(    0,    0,    7,           0,   152, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    7,           0,   154, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   152, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl1b_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl1b_stand_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   158, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl1b_stand_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0200,  56 ),
	CG_FRAME( 0x80,    0,    2,           0,   162, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,    3,           0,   163, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,   9 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00, 103 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    6,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   12,           0,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   167, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl1b_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl1b_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl1b_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   168, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   169, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   168, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1b_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl1b_crouch_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   171, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   172, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   171, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1b_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl1b_crouch_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   171, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   174, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   10,           0,   176, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    1,           0,   175, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   174, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1b_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl1b_crouch_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    1,           0,   178, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   179, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   177, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1b_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl1b_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0x80,   0x42,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    4,           0,   182, 0x80,   0x41,              ATK,  0x00,       0x05,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    6,           0,   182, 0x80,   0x41,              ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    6,           0,   183, 0x80,   0x42,              ATE,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   178, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl1b_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl1b_crouch_k_02[] = {
	CG_FRAME(    0,    0,    6,           0,   184, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   185, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    9,           0,   186, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   188, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   189, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl1b_crouch_k_02 ),
};

/* Char_Crouch_K 3: crouch_kick */
const Sint32 pl1b_crouch_k_03[] = {
	CG_FRAME(    0,    0,    8,           0,    48, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(  -48,    0,    4,           0,   278, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
};

const Sint32 pl1b_crouch_k_03_loop[] = {
	CG_FRAME(  -53,    0,    4,           0,   279, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(  -58,    0,    4, END_OF_DATA,   279, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl1b_crouch_k_03_loop ),
};

const Sint32 pl1b_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   279, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	0x00001728,
};

/* Char_Crouch_K 4: lever_atck1_jump */
const Sint32 pl1b_crouch_k_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   280, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_crouch_k_04 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl1b_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   190, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    9,           0,   192, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   190, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl1b_jump_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   193, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    6,           0,   195, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   194, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   193, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl1b_jump_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   193, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_p_02 ),
};

/* Char_Jump_K 0 (light) */
const Sint32 pl1b_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   196, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   198, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   196, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_k_00 ),
};

/* Char_Jump_K 3
   Char_Jump_K 6 */
const Sint32 pl1b_jump_k_03[] = {
	CG_FRAME(  -58,    0,   16, END_OF_DATA,   212, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl1b_jump_k_03 ),
};

const Sint32 pl1b_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   213, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  45 ),
	0x00001910,
};

/* Char_Jump 12: act_wall_jump */
const Sint32 pl1b_jump_12[] = {
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   214, 0x80,   0x20,              ATK,  0x00, 0x23 | CSE,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   215, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_jump_12 ),
};

/* Char_Jump 13: lever_atck2_bound */
const Sint32 pl1b_jump_13[] = {
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    52, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl1b_jump_13_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    56, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_13_loop ),
};

/* Char_Jump 14: lever_atck2_bound */
const Sint32 pl1b_jump_14[] = {
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl1b_jump_14_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_14_loop ),
};

/* Char_Jump 15: lever_atck2_bound */
const Sint32 pl1b_jump_15[] = {
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl1b_jump_15_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_jump_15_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl1b_jump_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    1,           0,   203, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   10,           0,   204, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   202, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl1b_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl1b_jump_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    1,           0,   203, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,    2,           0,   203, 0x80,   0x20,              ATK,  0x19,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,    3,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    1,           0,   205, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    5,           0,   206, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    5,           0,   205, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   202, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl1b_jump_k_02 ),
};

/* Char_Jump_K 5 */
const Sint32 pl1b_jump_k_05[] = {
	CG_FRAME(    0,    0,    5,           0,   199, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    8,           0,   201, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   201, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   191, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl1b_jump_k_05 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl1b_get_up_00[] = {
	CG_FRAME(    0,    0,    8,           0,   107, 0x80,   0x92,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl1b_get_up_01[] = {
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl1b_get_up_02[] = {
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl1b_get_up_03[] = {
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl1b_damage_00[] = {
	CG_FRAME( 0x80,    0,    1,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl1b_damage_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   10,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl1b_damage_02[] = {
	CG_FRAME( 0x80,    0,    2,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   14,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl1b_damage_03[] = {
	CG_FRAME(    0,    0,    1,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl1b_damage_04[] = {
	CG_FRAME( 0x80,    0,    2,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   10,           0,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl1b_damage_05[] = {
	CG_FRAME( 0x80,    0,    2,           0,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   14,           0,   293, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl1b_damage_06[] = {
	CG_FRAME( 0x80,    0,    1,           0,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl1b_damage_07[] = {
	CG_FRAME( 0x80,    0,    2,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   10,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl1b_damage_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   14,           0,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl1b_damage_10[] = {
	CG_FRAME( 0x80,    0,    3,           0,   298, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    4,           0,   299, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   298, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl1b_damage_11[] = {
	CG_FRAME( 0x80,    0,    1,           0,   298, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   299, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    4,           0,   300, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   298, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl1b_damage_12[] = {
	CG_FRAME( 0x80,    0,    1,           0,   299, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   300, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    4,           0,   301, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   300, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   298, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl1b_damage_13[] = {
	CG_FRAME( 0x80,    0,    2,           0,   306, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME( 0x82,    0,    2,           0,   307, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   308, 0x80,   0x91,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   309, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1b_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl1b_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   310, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   14,           0,   311, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   312, 0x80,   0x17,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   312, 0x80,   0x17,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl1b_damage_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   314, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1b_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl1b_damage_16[] = {
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   316, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl1b_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   317, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl1b_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   318, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl1b_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   319, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl1b_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   320, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl1b_damage_21[] = {
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl1b_damage_22[] = {
	CG_FRAME(    0,    0,    8,           0,   317, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl1b_damage_23[] = {
	CG_FRAME(    0,    0,    8,           0,   318, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl1b_damage_24[] = {
	CG_FRAME(    0,    0,    8,           0,   319, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl1b_damage_25[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   320, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl1b_damage_26[] = {
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x71,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4,           0,   313, 0x80,   0x71,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   314, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1b_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl1b_damage_27[] = {
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl1b_damage_30[] = {
	CG_FRAME(    0,    0,    2,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_30 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl1b_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl1b_damage_33[] = {
	CG_FRAME(    0,    0,    4,           0,    43, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    44, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    45, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    46, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_33 ),
};

const Sint32 pl1b_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3, END_OF_DATA,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00002840,
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl1b_damage_28[] = {
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   327, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl1b_damage_29[] = {
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   331, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   332, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   333, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl1b_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   314, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl1b_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl1b_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_dizzy_00 ),
};

/* Char_Special 0: kikou_init (Kikou Ken) */
const Sint32 pl1b_special_00[] = {
	CG_FRAME(    0,    0,    2,           0,   252, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   253, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    8,           0,   254, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,   257, 0x80,   0x48,              ATK,  0x00,       0x17,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    2,           0,   257, 0x80,   0x48,              ATK,  0x00,       0x1e,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    6,           0,   258, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  94 ),
	CG_FRAME(    0,    0,    6,           0,   259, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   16,           0,   260, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   261, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_00 ),
};

/* Char_Special 25 */
const Sint32 pl1b_special_25[] = {
	CG_FRAME(    0,    0,    2,           0,   252, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   253, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    8,           0,   254, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,   257, 0x80,   0x48,              ATK,  0x00,       0x17,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    2,           0,   257, 0x80,   0x48,              ATK,  0x00,       0x1e,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    6,           0,   258, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  94 ),
	CG_FRAME(    0,    0,    6,           0,   259, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   32,           0,   260, 0x80,   0x48,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_25 ),
};

/* Char_Extra 0: shl1b_00_kikou_init */
const Sint32 pl1b_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0,   375, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
};

const Sint32 pl1b_extra_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   366, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   374, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   375, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_NEXT( pl1b_extra_00_loop ),
};

/* Char_Extra 1: shl1b_00_kikou_move */
const Sint32 pl1b_extra_01[] = {
	CG_FRAME(    0,    0,    2,           0,   376, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   377, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   378, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   379, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   380, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   381, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_extra_01 ),
};

/* Char_Special 1 */
const Sint32 pl1b_special_01[] = {
	CG_FRAME(    0,    1,    2,           0,    66, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  23 ),
	CG_FRAME( 0x80,    2,    1,           0,   224, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0500,  25 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  25 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  25 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   228, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  23 ),
	CG_FRAME( 0x80,    4,    1,           0,   229, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00,  26 ),
	CG_FRAME(    0,    0,    6,           0,   230, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  26 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  26 ),
	CG_FRAME(    0,    0,    3,           0,   232, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  23 ),
	CG_FRAME(    0,    0,    1,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl1b_special_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_special_01_loop ),
};

/* Char_Special 2
   Char_Special 3 */
const Sint32 pl1b_special_02[] = {
	CG_FRAME(    0,    1,    2,           0,    66, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  23 ),
	CG_FRAME( 0x80,   10,    1,           0,   224, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0500,  25 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  25 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  25 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  23 ),
	CG_FRAME( 0x80,   10,    1,           0,   229, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00,  26 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  26 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  26 ),
	CG_FRAME(    0,    0,    1,           0,   232, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  23 ),
	CG_FRAME( 0x80,   10,    1,           0,   224, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0400,  25 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  25 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  25 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  23 ),
	CG_FRAME( 0x80,   12,    1,           0,   229, 0x80,   0x20,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00,  26 ),
	CG_FRAME(    0,    0,    6,           0,   230, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  26 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  26 ),
	CG_FRAME(    0,    0,    3,           0,   232, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  23 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl1b_special_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_special_02_loop ),
};

/* Char_Special 26: tenshou_move (Tenshoukyaku), hazan_air (Hazan Tenshoukyaku (hyper)) */
const Sint32 pl1b_special_26[] = {
	CG_FRAME(    0,    0,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_special_26 ),
};

/* Char_Special 4 */
const Sint32 pl1b_special_04[] = {
	CG_FRAME(    0,    0,    1,           0,    59, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,    60, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,   -1,    6,           0,   236, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,   -1,   18,           0,   237, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,   -1,    6,           0,   238, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(  -28,   -1,    1,           0,   239, 0x80,   0x20,              ATK,  0x01, 0x16 | CSE,    0,   0,     0, 0x0200,  28 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  86 ),
	CG_FRAME(    0,    0,    8,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  87 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    3,           0,   243, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    70, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_04 ),
};

const Sint32 pl1b_unused_03[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,   -1,    1,           0,   240, 0x80,   0x20,              ATK,  0x01,          0,    0,   0,     0, 0xfe00,  27 ),
	CG_FRAME(    0,   -1,    1,           0,   240, 0x80,   0x20,              ATK,  0x01,       0x03,    0,   0,     0, 0xfe00,  27 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,    8,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  87 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    3,           0,   243, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    70, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	0x000031ac,
};

/* Char_Special 5
   Char_Special 6 */
const Sint32 pl1b_special_05[] = {
	CG_FRAME(    0,    0,    1,           0,    59, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,    60, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,   -1,    4,           0,   236, 0x80,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,   -1,    8,           0,   237, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,   -1,    2,           0,   238, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(  -28,   -1,    1,           0,   239, 0x80,   0x20,              ATK,  0x01, 0x16 | CSE,    0,   0,     0, 0x0200,  28 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  86 ),
	CG_FRAME(    0,    0,   12,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  87 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    3,           0,   243, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    70, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_05 ),
};

const Sint32 pl1b_unused_04[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,   -1,    1,           0,   240, 0x80,   0x20,              ATK,  0x01,          0,    0,   0,     0, 0xfe00,  27 ),
	CG_FRAME(    0,   -1,    1,           0,   240, 0x80,   0x20,              ATK,  0x01,       0x03,    0,   0,     0, 0xfe00,  27 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   12,           0,   241, 0x80,   0x4b,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  87 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    3,           0,   243, 0x80,   0x4b,              ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    70, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	0x00003380,
};

/* Char_Special 7 */
const Sint32 pl1b_special_07[] = {
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    2,    3,           0,   216, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0200,  41 ),
};

const Sint32 pl1b_special_07_loop[] = {
	CG_FRAME(    0,    4,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  92 ),
	CG_FRAME(    0,    6,    3,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  42 ),
	CG_FRAME(    0,    8,    3,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0b00,  92 ),
	CG_FRAME(    0,   10,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0c00,  43 ),
	CG_FRAME(    0,   12,    3,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  92 ),
	CG_FRAME(    0,   14,    3,           0,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  44 ),
	CG_FRAME(    0,   16,    3,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1300,  92 ),
	CG_FRAME(    0,    2,    3, END_OF_DATA,   216, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0400,  41 ),
	CG_NEXT( pl1b_special_07_loop ),
};

/* Char_Special 10 */
const Sint32 pl1b_special_10[] = {
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,   129, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    1,    1,           0,   130, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    1,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   12,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_10 ),
};

/* Char_Special 8
   Char_Special 9
   Char_Special 11 */
const Sint32 pl1b_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    2,    1,           0,   216, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0200,  41 ),
};

const Sint32 pl1b_special_08_loop[] = {
	CG_FRAME(    0,    4,    1,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  92 ),
	CG_FRAME(    0,    6,    1,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  42 ),
	CG_FRAME(    0,    8,    1,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0b00,  92 ),
	CG_FRAME(    0,   10,    1,           0,   220, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0c00,  43 ),
	CG_FRAME(    0,   12,    1,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  92 ),
	CG_FRAME(    0,   14,    1,           0,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  44 ),
	CG_FRAME(    0,   16,    1,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1300,  92 ),
	CG_FRAME(    0,    2,    1, END_OF_DATA,   216, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0400,  41 ),
	CG_NEXT( pl1b_special_08_loop ),
};

/* Char_Special 12 */
const Sint32 pl1b_special_12[] = {
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,   129, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    1,    1,           0,   130, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    1,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   16,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl1b_special_13[] = {
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    2,    3,           0,   246, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0, 0x0200,  47 ),
};

const Sint32 pl1b_special_13_loop[] = {
	CG_FRAME(    0,    4,    3,           0,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  95 ),
	CG_FRAME(    0,    6,    3,           0,   248, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  48 ),
	CG_FRAME(    0,    8,    3,           0,   249, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0b00,  95 ),
	CG_FRAME(    0,   10,    3,           0,   250, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0, 0x0c00,  49 ),
	CG_FRAME(    0,   12,    3,           0,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  95 ),
	CG_FRAME(    0,   14,    3,           0,   251, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  50 ),
	CG_FRAME(    0,   16,    3,           0,   249, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1300,  95 ),
	CG_FRAME(    0,    2,    3, END_OF_DATA,   246, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0, 0x0400,  47 ),
	CG_NEXT( pl1b_special_13_loop ),
};

/* Char_Special 14
   Char_Special 15 */
const Sint32 pl1b_special_14[] = {
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    2,    1,           0,   246, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0, 0x0200,  47 ),
};

const Sint32 pl1b_special_14_loop[] = {
	CG_FRAME(    0,    4,    1,           0,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  95 ),
	CG_FRAME(    0,    6,    1,           0,   248, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  48 ),
	CG_FRAME(    0,    8,    1,           0,   249, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0b00,  95 ),
	CG_FRAME(    0,   10,    1,           0,   250, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0, 0x0c00,  49 ),
	CG_FRAME(    0,   12,    1,           0,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  58 ),
	CG_FRAME(    0,   14,    1,           0,   251, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  50 ),
	CG_FRAME(    0,   16,    1,           0,   249, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x1300,  95 ),
	CG_FRAME(    0,    2,    1, END_OF_DATA,   246, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0, 0x0400,  47 ),
	CG_NEXT( pl1b_special_14_loop ),
};

/* Char_Special 16 */
const Sint32 pl1b_special_16[] = {
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   244, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_special_16 ),
};

/* Char_Special 17
   Char_Special 18 */
const Sint32 pl1b_special_17[] = {
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   244, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_special_17 ),
};

/* Char_Special 19
   Char_Special 20
   Char_Special 21: stand_punch */
const Sint32 pl1b_special_19[] = {
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x4c, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0x0200,  56 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0, 0x0400,  98 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0x0400,  98 ),
	CG_FRAME(    0,    1,    1,           0,   271, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,       0x20,    0,   0,     0, 0xff00,  98 ),
	CG_FRAME(    0,    0,    1,           0,   272, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0xff00,  98 ),
};

const Sint32 pl1b_special_19_loop[] = {
	CG_FRAME(    0,    1,    1,           0,   271, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0xff00,  98 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   272, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0xff00,  98 ),
	CG_NEXT( pl1b_special_19_loop ),
};

/* Char_Special 22
   Char_Special 23
   Char_Special 24: lever_atck0_set */
const Sint32 pl1b_special_22[] = {
	CG_FRAME(    0,    0,    2,           0,   271, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,   10,    3,           0,   271, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,   12,    3,           0,   272, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    66, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_22 ),
};

/* Char_Super 0: kikoushou_init (Kikoushou (hyper)) */
const Sint32 pl1b_super_00[] = {
	CG_FRAME(    0,    0,    2,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  97 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  97 ),
	CG_FRAME(    0,    0,    2,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  97 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,              ATK,  0x00,       0x14,    0,   0,     0, 0x0400,  97 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  97 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  97 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  97 ),
	CG_FRAME(    0,    1,    1,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0c00,  97 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  56 ),
	CG_FRAME(    0,    0,    1,           0,   270, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  98 ),
	CG_FRAME(    0,    0,    1,           0,   270, 0x80,   0x49,              ATK,  0x00,       0x18,    0,   0,     0, 0x1000,  98 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   270, 0x80,   0x49,              ATK,  0x00,       0x1f,    0,   0,     0, 0x1100,  98 ),
	CG_NEXT( pl1b_super_00 ),
};

/* Char_Super 1
   Char_Super 2
   Char_Super 3: kikoushou_shot (Kikoushou (hyper)) */
const Sint32 pl1b_super_01[] = {
	CG_FRAME(    0,    0,    1,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  99 ),
	CG_FRAME(    0,   10,    1,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  99 ),
	CG_FRAME(    0,   10,    1,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  99 ),
	CG_FRAME(    0,   10,    1,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1500,  99 ),
	CG_FRAME(    0,   10,    1,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  99 ),
	CG_FRAME(    0,   10,    1,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  99 ),
	CG_FRAME(    0,   10,    1,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  99 ),
	CG_FRAME(    0,   10,    1,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  99 ),
	CG_FRAME(    0,   10,    1,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  99 ),
	CG_FRAME(    0,   12,    1,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  99 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  99 ),
	CG_NEXT( pl1b_super_01 ),
};

/* Char_Super 4: kikoushou_shot (Kikoushou (hyper)), kikoushou_hold (Kikoushou (hyper)) */
const Sint32 pl1b_super_04[] = {
	CG_FRAME(    0,   10,    2,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  98 ),
	CG_FRAME(    0,   12,    2,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  98 ),
	CG_FRAME(    0,   10,    3,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  98 ),
	CG_FRAME(    0,   12,    3,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  98 ),
	CG_FRAME(    0,   10,    4,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  98 ),
	CG_FRAME(    0,   12,    4,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  98 ),
	CG_FRAME(    0,   10,    5,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  98 ),
	CG_FRAME(    0,   12,    5,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  98 ),
	CG_FRAME(    0,   10,    6,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  98 ),
	CG_FRAME(    0,   12,    6,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  98 ),
	CG_FRAME(    0,   10,    6,           0,   271, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  98 ),
	CG_FRAME(    0,   12,    6,           0,   272, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x1400,  98 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    66, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_super_04 ),
};

/* Char_Extra 2: shl1b_00_kikoushou_init, fly_start5 */
const Sint32 pl1b_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   417, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   418, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,   419, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,   -1,    4,           0,   420, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl1b_extra_03[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,   -1,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,   -1,    2, END_OF_DATA, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl1b_extra_03 ),
};

const Sint32 pl1b_unused_05[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    1,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,   -1,   -1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,   -1,   -1, END_OF_DATA,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl1b_extra_03 ),
};

/* Char_Extra 4: shl1b_00_kikoushou_move */
const Sint32 pl1b_extra_04[] = {
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   432, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_extra_04 ),
};

/* Char_Effect 0: shl1b_00_kikoushou_pic_init */
const Sint32 pl1b_effect_00[] = {
	CG_FRAME(    0,    0,    1,           0,   382, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   383, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   384, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   385, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   386, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   387, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   388, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1000,   0 ),
	CG_NEXT( pl1b_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl1b_effect_01[] = {
	CG_FRAME(    0,    0,    1,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1200,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1400,   0 ),
	CG_NEXT( pl1b_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl1b_effect_02[] = {
	CG_FRAME(    0,    0,    1,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   393, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_effect_02 ),
};

/* Char_Effect 3: shl1b_00_3_init */
const Sint32 pl1b_effect_03[] = {
	CG_FRAME(    0,    0,    6,           0,   413, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   414, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   415, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   414, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_effect_03 ),
};

/* Char_Extra 8: shl1b_00_ball_init */
const Sint32 pl1b_extra_08[] = {
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   396, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   402, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
};

/* Char_Extra 9 */
const Sint32 pl1b_extra_09[] = {
	CG_FRAME(    0,    0,    1,           0,   403, 0x80,   0x00,              ATK,  0x5a,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   405, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   406, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   407, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   408, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl1b_extra_08 ),
};

/* Char_Extra 11: shl1b_00_11_init */
const Sint32 pl1b_extra_11[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,    4,           0,    -1, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Extra 12 */
const Sint32 pl1b_extra_12[] = {
	CG_FRAME(    0,    0,    2,           0,   459, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   460, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,    2,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   464, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,    2, END_OF_DATA,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_extra_12 ),
};

/* Char_Extra 13: pl1b_upper */
const Sint32 pl1b_extra_13[] = {
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0200,  56 ),
	CG_FRAME( 0x80,    0,    2,           0,   162, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0, 117 ),
	CG_FRAME( 0x80,    0,    3,           0,   163, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400, 117 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00, 118 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    6,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   12,           0,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   167, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_extra_13 ),
};

/* Char_Extra 5: t_kick_init */
const Sint32 pl1b_extra_05[] = {
	CG_FRAME(    0,    0,    2,           0,   196, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   32, END_OF_DATA,   198, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl1b_extra_05 ),
};

/* Char_Effect 4 */
const Sint32 pl1b_effect_04[] = {
	CG_FRAME(    0,    0,    1,           0,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_NEXT( pl1b_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl1b_effect_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   365, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_NEXT( pl1b_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl1b_effect_06[] = {
	CG_FRAME(    0,    0,    1,           0,   357, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   358, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   359, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   360, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   361, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   362, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   363, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   364, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1000,   0 ),
	CG_NEXT( pl1b_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl1b_effect_07[] = {
	CG_FRAME(    0,    0,    1,           0,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   350, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0e00,   0 ),
	CG_FRAME(    0,    0,    1,           0,   351, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1000,   0 ),
	CG_FRAME(    0,    0,    1,           0,   352, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   353, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   354, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   355, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1800,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   356, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1a00,   0 ),
	CG_NEXT( pl1b_effect_07 ),
};

/* Char_Effect 8: shl1b_00_ball_pic_init */
const Sint32 pl1b_effect_08[] = {
	CG_FRAME(    0,    0,    1,           0,   387, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   388, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
};

const Sint32 pl1b_effect_08_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_effect_08_loop ),
};

/* Char_Super 5: senretsu_init (Senretsukyaku (hyper)) */
const Sint32 pl1b_super_05[] = {
	CG_FRAME(    0,    0,    6,           0,   129, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   130, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   131, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   132, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   132, 0x80,   0x00,              ATK,  0x00,       0x19,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  92 ),
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x00,              ATK,  0x43, 0x15 | CSE,    0,   0,     0, 0x0800,  42 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  92 ),
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x00,              ATK,  0x44, 0x15 | CSE,    0,   0,     0, 0x0c00,  43 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  92 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x00,              ATK,  0x43, 0x15 | CSE,    0,   0,     0, 0x1000,  44 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  92 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,              ATK,  0x44, 0x15 | CSE,    0,   0,     0, 0x0400,  41 ),
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  92 ),
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x00,              ATK,  0x43, 0x15 | CSE,    0,   0,     0, 0x0800,  42 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  92 ),
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x00,              ATK,  0x44, 0x15 | CSE,    0,   0,     0, 0x0c00,  43 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  92 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x00,              ATK,  0x43, 0x15 | CSE,    0,   0,     0, 0x1000,  44 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  92 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,              ATK,  0x44, 0x15 | CSE,    0,   0,     0, 0x0400,  41 ),
};

const Sint32 pl1b_super_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  92 ),
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x00,              ATK,  0x43, 0x15 | CSE,    0,   0,     0, 0x0800,  42 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  92 ),
	CG_FRAME(    0,    0,    1,           0,   220, 0x80,   0x00,              ATK,  0x44,       0x05,    0,   0,     0, 0x0c00,  43 ),
	CG_FRAME(    0,    0,    1,           0,   220, 0x80,   0x00,              ATK,  0x00, 0x15 | CSE,    0,   0,     0, 0x0c00,  43 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  92 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x00,              ATK,  0x43, 0x15 | CSE,    0,   0,     0, 0x1000,  44 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1200,  92 ),
	CG_FRAME(    0,    0,    1,           0,   216, 0x80,   0x00,              ATK,  0x44,       0x05,    0,   0,     0, 0x0400,  41 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   216, 0x80,   0x00,              ATK,  0x00, 0x15 | CSE,    0,   0,     0, 0x0400,  41 ),
	CG_NEXT( pl1b_super_05_loop ),
};

/* Char_Super 6
   Char_Super 7: senretsu_kick (Senretsukyaku (hyper)) */
const Sint32 pl1b_super_06[] = {
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,              ATK,  0x46,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   201, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   191, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl1b_super_06 ),
};

/* Char_Super 8: senretsu_stop (Senretsukyaku (hyper)) */
const Sint32 pl1b_super_08[] = {
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    1,    4,           0,   130, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    4,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_super_08 ),
};

/* Char_Super 9: hazan_init (Hazan Tenshoukyaku (hyper)) */
const Sint32 pl1b_super_09[] = {
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    1,           0,    73, 0x80,   0x00,              ATK,  0x00,       0x1b,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   15,    2,           0,   222, 0x80,   0x00,              ATK,  0x00,       0x1a,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  88 ),
	CG_FRAME( 0x80,   14,    1,           0,   224, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0400,  90 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  90 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  90 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  88 ),
	CG_FRAME( 0x80,   14,    1,           0,   229, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00,  91 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  91 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  91 ),
	CG_FRAME(    0,    0,    1,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  23 ),
	CG_FRAME( 0x80,   10,    1,           0,   224, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0400,  90 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  90 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  90 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  89 ),
	CG_FRAME( 0x80,   10,    1,           0,   229, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00,  91 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  91 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  91 ),
	CG_FRAME(    0,    0,    1,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  89 ),
	CG_FRAME( 0x80,   10,    1,           0,   224, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0400,  90 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  90 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  90 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  89 ),
	CG_FRAME( 0x80,   10,    1,           0,   229, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00,  91 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00,  91 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000,  91 ),
	CG_FRAME(    0,    0,    1,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  89 ),
	CG_FRAME( 0x80,   10,    1,           0,   224, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0400,  90 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  90 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  90 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  89 ),
	CG_FRAME( 0x80,   12,    1,           0,   229, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0c00, 107 ),
	CG_FRAME(    0,    0,    3,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0e00, 107 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x1000, 107 ),
	CG_FRAME(    0,    0,    3,           0,   232, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  89 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl1b_super_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    48, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl1b_super_09_loop ),
};

/* Char_Catch 2
   Char_Super 10
   Char_Super 11: shichisei_init (Shichisei Ranka (air hyper))
   Char_Super 12
   Char_Super 13: shichisei_dash (Shichisei Ranka (air hyper))
   Char_Super 14: shichisei_catch (Shichisei Ranka (air hyper)) */
const Sint32 pl1b_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   126, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x30,   0x00,              ATK,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   10, END_OF_DATA,   128, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_catch_02 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl1b_change_01[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl1b_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl1b_change_02[] = {
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   125, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl1b_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl1b_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   100, 0x80,   0x00,                0,  0x00,       0x0e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_change_03_loop[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl1b_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_change_04 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl1b_win_00[] = {
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x00,                0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_win_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl1b_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,                0,  0x00,       0x11,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x20,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_win_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_win_01_loop ),
};

/* Char_Win 2: win_ko_init, win_time */
const Sint32 pl1b_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   128, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_win_02_loop[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_win_02_loop ),
};

/* Char_Win 3: win_ko_init */
const Sint32 pl1b_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_win_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_win_03_loop ),
};

/* Char_Win 5
   Char_Extra 10 */
const Sint32 pl1b_win_05[] = {
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_win_05_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_win_05_loop ),
};

/* Char_Win 4: lose_time_ko */
const Sint32 pl1b_win_04[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    96, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    97, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    98, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    97, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    98, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    97, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    97, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl1b_win_04_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    98, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_win_04_loop ),
};

/* Char_Extra 7: sp_chouhatsu (taunt) */
const Sint32 pl1b_extra_07[] = {
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   128, 0x80,   0x00,              ATK,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   16,           0,   128, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    71, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_extra_07 ),
};

/* Char_Catch 0: nml_set */
const Sint32 pl1b_catch_00[] = {
	CG_FRAME(    0,    0,    6,           0,   207, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    6,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,    6,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    1,   14,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   211, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_catch_00 ),
};

/* Char_Catch 1: jump_set */
const Sint32 pl1b_catch_01[] = {
	CG_FRAME(    0,    0,    6,           0,   207, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    0,    6,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0007,   1 ),
	CG_FRAME(    0,    0,    2,           0,   210, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0, 0x0008,   1 ),
	CG_FRAME(    0,    1,   14,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_catch_01 ),
};

/* Char_Caught 0 */
const Sint32 pl1b_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl1b_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl1b_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl1b_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl1b_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   292, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl1b_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   293, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl1b_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl1b_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl1b_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl1b_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl1b_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl1b_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl1b_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl1b_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl1b_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl1b_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl1b_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl1b_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl1b_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   324, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl1b_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl1b_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl1b_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl1b_caught_21 ),
};

/* Char_Appear 0: appear_init
   Char_Effect 9
   Char_Effect 10
   Char_Effect 11
   Char_Effect 12
   Char_Effect 13 */
const Sint32 pl1b_appear_00[] = {
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   105, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   13,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl1b_appear_00 ),
};

/* Char_Extra 6: t_kick_move */
const Sint32 pl1b_extra_06[] = {
	CG_FRAME(    0,    0,    1,           0,    68, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    5,           0,    68, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    1,           0,    67, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    1,           0,    66, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,    92, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,              ATE,  0x00,       0x0d,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   18,           0,    94, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    85, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl1b_extra_06 ),
};

/* Char_Special 27: snapback_init (Snap Back) */
const Sint32 pl1b_special_27[] = {
	CG_FRAME(    0,    1,    8,           0,   268, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    2,           0,   268, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x4c, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0x0200,  56 ),
	CG_FRAME(    0,    0,    3,           0,   270, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0, 0x0400,  98 ),
	CG_FRAME(    0,    1,    1,           0,   271, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,       0x20,    0,   0,     0, 0xff00,  98 ),
	CG_FRAME(    0,    0,    1,           0,   272, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0xff00,  98 ),
};

const Sint32 pl1b_special_27_loop[] = {
	CG_FRAME(    0,    1,    1,           0,   271, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0xff00,  98 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   272, 0x80,   0x49, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0xff00,  98 ),
	CG_NEXT( pl1b_special_27_loop ),
};

/* Char_Special 28: snapback_wait (Snap Back) */
const Sint32 pl1b_special_28[] = {
	CG_FRAME(    0,    0,    2,           0,   271, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,   10,    3,           0,   271, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,   12,    3,           0,   272, 0x80,   0x49, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    66, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl1b_special_28 ),
};
