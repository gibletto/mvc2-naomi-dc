/* Hulk (character 0d): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_0d      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_0d     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl0d.c) says which group
                   and number each move uses.
   pl0d_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl0d_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl0d_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl0d_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_0d
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_0d, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_0d */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl0d_stand_00[], pl0d_stand_02[], pl0d_stand_03[], pl0d_stand_06[],
	pl0d_stand_07[], pl0d_jump_00[], pl0d_jump_00_loop[], pl0d_jump_01[], pl0d_jump_02[],
	pl0d_jump_03[], pl0d_jump_06[], pl0d_jump_07[], pl0d_jump_08[], pl0d_jump_09[], pl0d_dash_00[],
	pl0d_dash_01[], pl0d_dash_02[], pl0d_dash_03[], pl0d_crouch_00[], pl0d_crouch_01[],
	pl0d_crouch_02[], pl0d_turn_00[], pl0d_turn_01[], pl0d_guard_00[], pl0d_guard_01[],
	pl0d_guard_02[], pl0d_guard_03[], pl0d_guard_04[], pl0d_guard_05[], pl0d_guard_06[],
	pl0d_guard_07[], pl0d_air_guard_00[], pl0d_air_guard_01[], pl0d_air_guard_02[], pl0d_change_00[],
	pl0d_win_00[], pl0d_win_00_loop[], pl0d_win_02[], pl0d_win_02_loop[], pl0d_win_01[],
	pl0d_win_01_loop[], pl0d_extra_02[], pl0d_change_04[], pl0d_stand_p_00[], pl0d_stand_p_01[],
	pl0d_stand_p_02[], pl0d_stand_k_00[], pl0d_stand_k_01[], pl0d_stand_k_02[], pl0d_crouch_p_00[],
	pl0d_crouch_p_01[], pl0d_crouch_p_02[], pl0d_extra_03[], pl0d_crouch_k_00[], pl0d_crouch_k_01[],
	pl0d_crouch_k_02[], pl0d_extra_04[], pl0d_jump_p_00[], pl0d_jump_p_01[], pl0d_jump_p_02[],
	pl0d_jump_k_00[], pl0d_jump_k_01[], pl0d_jump_k_02[], pl0d_special_00[], pl0d_special_00_loop[],
	pl0d_special_01[], pl0d_special_01_loop[], pl0d_special_02[], pl0d_special_03[],
	pl0d_special_04[], pl0d_special_04_loop[], pl0d_special_06[], pl0d_special_06_loop[],
	pl0d_special_08[], pl0d_special_08_loop[], pl0d_special_10[], pl0d_special_12[],
	pl0d_special_13[], pl0d_extra_00[], pl0d_extra_01[], pl0d_change_01[], pl0d_change_03[],
	pl0d_change_03_loop[], pl0d_super_00[], pl0d_super_00_loop[], pl0d_super_01[], pl0d_super_02[],
	pl0d_super_03[], pl0d_super_04[], pl0d_super_04_loop[], pl0d_super_05[], pl0d_super_06[],
	pl0d_super_07[], pl0d_super_08[], pl0d_super_09[], pl0d_special_24[], pl0d_special_26[],
	pl0d_special_25[], pl0d_special_27[], pl0d_special_14[], pl0d_special_14_loop[],
	pl0d_special_16[], pl0d_special_16_loop[], pl0d_special_18[], pl0d_special_18_loop[],
	pl0d_special_20[], pl0d_special_20_loop[], pl0d_special_22[], pl0d_special_23[],
	pl0d_special_28[], pl0d_special_28_loop[], pl0d_super_10[], pl0d_super_11[],
	pl0d_super_11_loop[], pl0d_super_12[], pl0d_super_13[], pl0d_damage_00[], pl0d_damage_01[],
	pl0d_damage_02[], pl0d_damage_03[], pl0d_damage_04[], pl0d_damage_05[], pl0d_damage_06[],
	pl0d_damage_07[], pl0d_damage_08[], pl0d_damage_09[], pl0d_damage_09_loop[], pl0d_damage_10[],
	pl0d_damage_11[], pl0d_damage_12[], pl0d_damage_13[], pl0d_damage_16[], pl0d_damage_17[],
	pl0d_damage_18[], pl0d_damage_19[], pl0d_damage_20[], pl0d_damage_21[], pl0d_damage_22[],
	pl0d_damage_23[], pl0d_damage_24[], pl0d_damage_25[], pl0d_damage_26[], pl0d_damage_27[],
	pl0d_damage_30[], pl0d_damage_28[], pl0d_damage_29[], pl0d_damage_31[], pl0d_damage_32[],
	pl0d_dizzy_00[], pl0d_caught_00[], pl0d_caught_01[], pl0d_caught_02[], pl0d_caught_03[],
	pl0d_caught_04[], pl0d_caught_05[], pl0d_caught_06[], pl0d_caught_07[], pl0d_caught_08[],
	pl0d_caught_09[], pl0d_caught_10[], pl0d_caught_11[], pl0d_caught_12[], pl0d_caught_13[],
	pl0d_caught_14[], pl0d_caught_15[], pl0d_caught_16[], pl0d_caught_17[], pl0d_caught_18[],
	pl0d_caught_19[], pl0d_caught_20[], pl0d_caught_21[], pl0d_effect_00[], pl0d_effect_01[],
	pl0d_effect_02[], pl0d_effect_03[], pl0d_effect_05[], pl0d_effect_06[], pl0d_effect_07[],
	pl0d_appear_00[], pl0d_appear_01[], pl0d_appear_01_loop[], pl0d_catch_00[], pl0d_catch_01[],
	pl0d_effect_04[], pl0d_catch_02[], pl0d_catch_03[], pl0d_get_up_00[], pl0d_get_up_01[],
	pl0d_get_up_02[], pl0d_get_up_03[], pl0d_change_02[], pl0d_effect_08[], pl0d_effect_08_loop[],
	pl0d_effect_12[], pl0d_effect_12_loop[], pl0d_effect_13[], pl0d_effect_14[], pl0d_effect_15[],
	pl0d_effect_16[], pl0d_effect_17[], pl0d_effect_18[], pl0d_effect_21[], pl0d_effect_22[],
	pl0d_effect_23[], pl0d_effect_24[], pl0d_effect_24_loop[], pl0d_effect_25[],
	pl0d_effect_25_loop[], pl0d_effect_26[], pl0d_effect_27[], pl0d_effect_28[], pl0d_effect_29[],
	pl0d_effect_30[], pl0d_special_29[];
extern const Sint32 *const pl0d_stand_tbl[], *const pl0d_jump_tbl[], *const pl0d_dash_tbl[],
	*const pl0d_crouch_tbl[], *const pl0d_turn_tbl[], *const pl0d_guard_tbl[],
	*const pl0d_air_guard_tbl[], *const pl0d_stand_p_tbl[], *const pl0d_stand_k_tbl[],
	*const pl0d_crouch_p_tbl[], *const pl0d_crouch_k_tbl[], *const pl0d_jump_p_tbl[],
	*const pl0d_jump_k_tbl[], *const pl0d_damage_tbl[], *const pl0d_dizzy_tbl[],
	*const pl0d_catch_tbl[], *const pl0d_caught_tbl[], *const pl0d_get_up_tbl[],
	*const pl0d_appear_tbl[], *const pl0d_win_tbl[], *const pl0d_extra_tbl[],
	*const pl0d_special_tbl[], *const pl0d_super_tbl[], *const pl0d_effect_tbl[],
	*const pl0d_change_tbl[], *const pl0d_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_0d[16] = {
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
const Sint32 *const *const char_tbl_0d[] = {
	pl0d_stand_tbl, pl0d_jump_tbl, pl0d_dash_tbl, pl0d_crouch_tbl, pl0d_turn_tbl, pl0d_guard_tbl,
	pl0d_air_guard_tbl, pl0d_stand_p_tbl, pl0d_stand_k_tbl, pl0d_crouch_p_tbl, pl0d_crouch_k_tbl, pl0d_jump_p_tbl,
	pl0d_jump_k_tbl, pl0d_damage_tbl, pl0d_dizzy_tbl, pl0d_catch_tbl, pl0d_caught_tbl, pl0d_get_up_tbl,
	pl0d_appear_tbl, pl0d_win_tbl, pl0d_extra_tbl, pl0d_special_tbl, pl0d_super_tbl, pl0d_effect_tbl,
	pl0d_change_tbl, pl0d_grp25_tbl, 0,
};

const Sint32 *const pl0d_stand_tbl[] = {
	pl0d_stand_00, pl0d_stand_00, pl0d_stand_02, pl0d_stand_03, pl0d_stand_00, pl0d_stand_00,
	pl0d_stand_06, pl0d_stand_07, pl0d_stand_00, pl0d_stand_00, 0,
};

const Sint32 *const pl0d_jump_tbl[] = {
	pl0d_jump_00, pl0d_jump_01, pl0d_jump_02, pl0d_jump_03, pl0d_jump_03, pl0d_jump_03,
	pl0d_jump_06, pl0d_jump_07, pl0d_jump_08, pl0d_jump_09, pl0d_jump_09, pl0d_jump_09,
	0,
};

const Sint32 *const pl0d_dash_tbl[] = {
	pl0d_dash_00, pl0d_dash_01, pl0d_dash_02, pl0d_dash_03, 0,
};

const Sint32 *const pl0d_crouch_tbl[] = {
	pl0d_crouch_00, pl0d_crouch_01, pl0d_crouch_02, 0,
};

const Sint32 *const pl0d_turn_tbl[] = {
	pl0d_turn_00, pl0d_turn_01, 0,
};

const Sint32 *const pl0d_guard_tbl[] = {
	pl0d_guard_00, pl0d_guard_01, pl0d_guard_02, pl0d_guard_03, pl0d_guard_04, pl0d_guard_05,
	pl0d_guard_06, pl0d_guard_07, 0,
};

const Sint32 *const pl0d_air_guard_tbl[] = {
	pl0d_air_guard_00, pl0d_air_guard_01, pl0d_air_guard_02, 0,
};

const Sint32 *const pl0d_stand_p_tbl[] = {
	pl0d_stand_p_00, pl0d_stand_p_01, pl0d_stand_p_02, 0,
};

const Sint32 *const pl0d_stand_k_tbl[] = {
	pl0d_stand_k_00, pl0d_stand_k_01, pl0d_stand_k_02, 0,
};

const Sint32 *const pl0d_crouch_p_tbl[] = {
	pl0d_crouch_p_00, pl0d_crouch_p_01, pl0d_crouch_p_02, 0,
};

const Sint32 *const pl0d_crouch_k_tbl[] = {
	pl0d_crouch_k_00, pl0d_crouch_k_01, pl0d_crouch_k_02, 0,
};

const Sint32 *const pl0d_jump_p_tbl[] = {
	pl0d_jump_p_00, pl0d_jump_p_01, pl0d_jump_p_02, pl0d_jump_p_00, pl0d_jump_p_01, pl0d_jump_p_02,
	0,
};

const Sint32 *const pl0d_jump_k_tbl[] = {
	pl0d_jump_k_00, pl0d_jump_k_01, pl0d_jump_k_02, pl0d_jump_k_00, pl0d_jump_k_01, pl0d_jump_k_02,
	0,
};

const Sint32 *const pl0d_damage_tbl[] = {
	pl0d_damage_00, pl0d_damage_01, pl0d_damage_02, pl0d_damage_03, pl0d_damage_04, pl0d_damage_05,
	pl0d_damage_06, pl0d_damage_07, pl0d_damage_08, pl0d_damage_09, pl0d_damage_10, pl0d_damage_11,
	pl0d_damage_12, pl0d_damage_13, pl0d_damage_13, pl0d_damage_13, pl0d_damage_16, pl0d_damage_17,
	pl0d_damage_18, pl0d_damage_19, pl0d_damage_20, pl0d_damage_21, pl0d_damage_22, pl0d_damage_23,
	pl0d_damage_24, pl0d_damage_25, pl0d_damage_26, pl0d_damage_27, pl0d_damage_28, pl0d_damage_29,
	pl0d_damage_30, pl0d_damage_31, pl0d_damage_32, pl0d_damage_32, pl0d_damage_13, 0,
};

const Sint32 *const pl0d_dizzy_tbl[] = {
	pl0d_dizzy_00, 0,
};

const Sint32 *const pl0d_catch_tbl[] = {
	pl0d_catch_00, pl0d_catch_01, pl0d_catch_02, pl0d_catch_03, 0,
};

const Sint32 *const pl0d_caught_tbl[] = {
	pl0d_caught_00, pl0d_caught_01, pl0d_caught_02, pl0d_caught_03, pl0d_caught_04, pl0d_caught_05,
	pl0d_caught_06, pl0d_caught_07, pl0d_caught_08, pl0d_caught_09, pl0d_caught_10, pl0d_caught_11,
	pl0d_caught_12, pl0d_caught_13, pl0d_caught_14, pl0d_caught_15, pl0d_caught_16, pl0d_caught_17,
	pl0d_caught_18, pl0d_caught_19, pl0d_caught_20, pl0d_caught_21, 0,
};

const Sint32 *const pl0d_get_up_tbl[] = {
	pl0d_get_up_00, pl0d_get_up_01, pl0d_get_up_02, pl0d_get_up_03, 0,
};

const Sint32 *const pl0d_appear_tbl[] = {
	pl0d_appear_00, pl0d_appear_01, 0,
};

const Sint32 *const pl0d_win_tbl[] = {
	pl0d_win_00, pl0d_win_01, pl0d_win_02, 0,
};

const Sint32 *const pl0d_extra_tbl[] = {
	pl0d_extra_00, pl0d_extra_01, pl0d_extra_02, pl0d_extra_03, pl0d_extra_04, 0,
};

const Sint32 *const pl0d_special_tbl[] = {
	pl0d_special_00, pl0d_special_01, pl0d_special_02, pl0d_special_03, pl0d_special_04, pl0d_special_04,
	pl0d_special_06, pl0d_special_06, pl0d_special_08, pl0d_special_08, pl0d_special_10, pl0d_special_10,
	pl0d_special_12, pl0d_special_13, pl0d_special_14, pl0d_special_14, pl0d_special_16, pl0d_special_16,
	pl0d_special_18, pl0d_special_18, pl0d_special_20, pl0d_special_20, pl0d_special_22, pl0d_special_23,
	pl0d_special_24, pl0d_special_25, pl0d_special_26, pl0d_special_27, pl0d_special_28, pl0d_special_29,
	0,
};

const Sint32 *const pl0d_super_tbl[] = {
	pl0d_super_00, pl0d_super_01, pl0d_super_02, pl0d_super_03, pl0d_super_04, pl0d_super_05,
	pl0d_super_06, pl0d_super_07, pl0d_super_08, pl0d_super_09, pl0d_super_10, pl0d_super_11,
	pl0d_super_12, pl0d_super_13, 0,
};

const Sint32 *const pl0d_effect_tbl[] = {
	pl0d_effect_00, pl0d_effect_01, pl0d_effect_02, pl0d_effect_03, pl0d_effect_04, pl0d_effect_05,
	pl0d_effect_06, pl0d_effect_07, pl0d_effect_08, pl0d_effect_08, pl0d_effect_08, pl0d_effect_08,
	pl0d_effect_12, pl0d_effect_13, pl0d_effect_14, pl0d_effect_15, pl0d_effect_16, pl0d_effect_17,
	pl0d_effect_18, pl0d_effect_08, pl0d_effect_08, pl0d_effect_21, pl0d_effect_22, pl0d_effect_23,
	pl0d_effect_24, pl0d_effect_25, pl0d_effect_26, pl0d_effect_27, pl0d_effect_28, pl0d_effect_29,
	pl0d_effect_30, 0,
};

const Sint32 *const pl0d_change_tbl[] = {
	pl0d_change_00, pl0d_change_01, pl0d_change_02, pl0d_change_03, pl0d_change_04, 0,
};

const Sint32 *const pl0d_grp25_tbl[] = {
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl0d_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    52, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    53, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    54, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    55, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    56, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    57, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    58, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    59, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl0d_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    62, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    64, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    66, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    68, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    72, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    74, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    76, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl0d_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    87, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    85, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    82, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    80, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    79, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    77, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl0d_stand_06[] = {
	CG_FRAME(    0,    0,   8,           0,    68, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    71, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl0d_stand_07[] = {
	CG_FRAME(    0,    0,   8,           0,    71, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    70, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    69, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    68, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl0d_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   101, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   101, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   103, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   104, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   105, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   106, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   105, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   104, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl0d_jump_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   107, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   108, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl0d_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   110, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   112, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   115, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   114, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA,   116, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_01 ),
};

/* Char_Jump 2 */
const Sint32 pl0d_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   117, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   120, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   121, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   120, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA,   122, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_02 ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl0d_jump_03[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl0d_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   101, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   107, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl0d_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   110, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_09 ),
};

/* Char_Jump 8 */
const Sint32 pl0d_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   117, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_09 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl0d_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   108, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl0d_dash_00[] = {
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  18,           0,    94, 0x80,   0x60,          4,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,    95, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_dash_00 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl0d_dash_01[] = {
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,       0x19,  0x00, 0x34 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    99, 0x80,   0x00,          0,  0x00, 0x34 | CSE,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0d_dash_01 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl0d_dash_02[] = {
	CG_FRAME(    0,    0,  16,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_dash_02 ),
};

/* Char_Dash 3 */
const Sint32 pl0d_dash_03[] = {
	CG_FRAME(    0,    1,   4,           0,   100, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl0d_crouch_00[] = {
	CG_FRAME(    0,    4,   3,           0,   123, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   3,           0,   124, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   3,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl0d_crouch_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    60, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl0d_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   126, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,   3,           0,   127, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    3,   3,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    4,   3,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    4,   3, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl0d_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl0d_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl0d_guard_00[] = {
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl0d_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl0d_guard_02[] = {
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl0d_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl0d_guard_04[] = {
	CG_FRAME(    0,    0,   6,           0,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl0d_guard_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl0d_guard_06[] = {
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl0d_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl0d_air_guard_00[] = {
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl0d_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   143, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl0d_air_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   142, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_air_guard_02 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl0d_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x00,          3,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   199, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  26, END_OF_DATA,   199, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_change_00 ),
};

/* Char_Win 0: win_time */
const Sint32 pl0d_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x00,          7,  0x00,       0x11,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   159, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   160, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   158, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0d_win_00_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   160, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_win_00_loop ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl0d_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   428, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   426, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   164, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   165, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_win_02_loop ),
};

/* Char_Win 1 */
const Sint32 pl0d_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0x80,   0xa0,          6,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0d_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   186, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_win_01_loop ),
};

/* Char_Extra 2: chouhatsu_init (Taunt), fly_start5 */
const Sint32 pl0d_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0x80,   0xa0,          6,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   186, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   152, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_extra_02 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl0d_change_04[] = {
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   177, 0x80,   0xa0,          6,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   180, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   183, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   177, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_change_04 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl0d_stand_p_00[] = {
	CG_FRAME(    0,    0,   6,           0,   203, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   204, 0x80,   0xa4, ATK | 0x13,  0x00, 0x15 | CSE,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   5,           0,   205, 0x80,   0xa4,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   8,           0,   206, 0x80,   0xc3,        ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0xc3,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   207, 0x80,   0xc3,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   5,           0,   201, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl0d_stand_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   128, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x00,    ATK | 3,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   7,           0,   202, 0x80,   0x09,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  13,           0,   202, 0x80,   0x09,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   201, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl0d_stand_p_02[] = {
	CG_FRAME(    0,    0,  10,           0,   208, 0x80,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   1,           0,   208, 0x80,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0xa8, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   2,           0,   209, 0x80,   0xa1,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0xa1,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x01,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,  22,           0,   210, 0x80,   0x01,        ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   7,           0,   211, 0x80,   0x01,        ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   7,           0,   109, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl0d_stand_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   129, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   212, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0xe8,    ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   8,           0,   213, 0x80,   0xe8,        ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   6,           0,   128, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl0d_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   214, 0x80,   0x67,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   215, 0x80,   0x67,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   215, 0x80,   0x67,        ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,   4,           0,   216, 0x80,   0x67,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,  16,           0,   216, 0x80,   0x67,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   7,           0,   217, 0x80,   0x67,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl0d_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0xbc,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   221, 0x80,   0xbc, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0xbc,        ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   223, 0x80,   0xb3,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    1,   4,           0,   222, 0x80,   0xb3,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME( 0x80,    0,   4,           0,   223, 0x80,   0xb3,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,  16,           0,   222, 0x80,   0xb3,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   5,           0,   224, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl0d_crouch_p_00[] = {
	CG_FRAME(    0,    0,   6,           0,   227, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0,   227, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,   2,           0,   228, 0x80,   0x00,    ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,  16,           0,   229, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   230, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl0d_crouch_p_01[] = {
	CG_FRAME(    0,    0,   9,           0,   231, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0xa7, ATK | 0x13,  0x00, 0x15 | CSE,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0xa7,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,  16,           0,   234, 0x80,   0xc4,        ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6,           0,   235, 0x80,   0xc4,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   236, 0x80,   0x0c,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl0d_crouch_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   237, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   238, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   238, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x00, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   2,           0,   240, 0x80,   0x00,    ATK | 5,  0x1a,       0x05,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,  21,           0,   242, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_crouch_p_02 ),
};

/* Char_Extra 3: upper_init, fly_start5 */
const Sint32 pl0d_extra_03[] = {
	CG_FRAME(    0,    0,   6,           0,   237, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   238, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   238, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x00, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   2,           0,   240, 0x80,   0x00,    ATK | 5,  0x1a,       0x05,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,  21,           0,   242, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_extra_03 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl0d_crouch_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   134, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   245, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0xea,    ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  10,           0,   246, 0x80,   0xea,        ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   6,           0,   133, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl0d_crouch_k_01[] = {
	CG_FRAME(    0,    0,   8,           0,   247, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,        ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6,           0,   248, 0x80,   0x00,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,  14,           0,   248, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6,           0,   249, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl0d_crouch_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   247, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,        ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   4,           0,   250, 0x80,   0x00,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,  21,           0,   251, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   6,           0,   249, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_crouch_k_02 ),
};

/* Char_Extra 4: upper_init */
const Sint32 pl0d_extra_04[] = {
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0xbc,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   221, 0x80,   0xbc, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0xbc,        ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0xb3,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0xb3,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0xb3,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,  16,           0,   222, 0x80,   0xb3,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   5,           0,   224, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_extra_04 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl0d_jump_p_00[] = {
	CG_FRAME(    0,    0,   5,           0,   103, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   103, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,  12,           0,   255, 0x80,   0x60,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl0d_jump_p_01[] = {
	CG_FRAME(    0,    0,   8,           0,   252, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x80,    0,   6,           0,   254, 0x80,   0x60,    ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   8,           0,   254, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   113, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl0d_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x60, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    1,   1,           0,   257, 0x80,   0x60,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   8,           0,   257, 0x80,   0x60, ATK | 0x0f,  0x00, 0x03 | CSE,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x82,    0,  16,           0,   258, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl0d_jump_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   260, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,  10,           0,   261, 0x80,   0x60,    ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   259, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl0d_jump_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   104, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   262, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,  10,           0,   263, 0x80,   0x60,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   6,           0,   263, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl0d_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,  10,           0,   264, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   1,           0,   265, 0x80,   0x60, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   7,           0,   265, 0x80,   0x60,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x81,    0,   4,           0,   265, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   104, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_jump_k_02 ),
};

/* Char_Special 0 */
const Sint32 pl0d_special_00[] = {
	CG_FRAME(    0,    0,   6,           0,   505, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,   1,           0,   291, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_special_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   291, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl0d_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl0d_special_01[] = {
	CG_FRAME(    0,    0,   6,           0,   505, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,   1,           0,   291, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_special_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   291, 0x80,   0xcc,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl0d_special_01_loop ),
};

/* Char_Special 2 */
const Sint32 pl0d_special_02[] = {
	CG_FRAME(    0,    0,  20,           0,   109, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,   4,           0,   132, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl0d_special_03[] = {
	CG_FRAME(    0,    0,  32,           0,   109, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,   4,           0,   132, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   131, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   132, 0x98,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_special_03 ),
};

/* Char_Special 4
   Char_Special 5 */
const Sint32 pl0d_special_04[] = {
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   299, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   300, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
};

const Sint32 pl0d_special_04_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   297, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   298, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl0d_special_04_loop ),
};

/* Char_Special 6
   Char_Special 7 */
const Sint32 pl0d_special_06[] = {
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
};

const Sint32 pl0d_special_06_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   295, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   296, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl0d_special_06_loop ),
};

/* Char_Special 8
   Char_Special 9 */
const Sint32 pl0d_special_08[] = {
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   299, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   300, 0x80,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
};

const Sint32 pl0d_special_08_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   301, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   302, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl0d_special_08_loop ),
};

/* Char_Special 10
   Char_Special 11 */
const Sint32 pl0d_special_10[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   303, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_special_10 ),
};

/* Char_Special 12 */
const Sint32 pl0d_special_12[] = {
	CG_FRAME(    0,    1,   2,           0,   166, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   167, 0x80,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl0d_special_13[] = {
	CG_FRAME(    0,    0,   8,           0,   109, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x80,          0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_special_13 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl0d_extra_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   261, 0x80,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl0d_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl0d_extra_01[] = {
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x80,   0x80,    ATE | 5,  0x00,       0x05,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   188, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,  14,           0,   192, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   195, 0x80,   0x80,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_extra_01 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl0d_change_01[] = {
	CG_FRAME(    0,    0,   4,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_change_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl0d_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,          3,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl0d_change_03_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_change_03_loop ),
};

/* Char_Super 0 */
const Sint32 pl0d_super_00[] = {
	CG_FRAME(    0,    0,   1,           0,   144, 0xc0,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1,           0,   145, 0xc0,   0xa0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0xc0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1,           0,   147, 0xc0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1,           0,   148, 0xc0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1,           0,   149, 0xc0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0xc0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    2,   1,           0,   150, 0xc0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1,           0,   151, 0xc0,   0xc0,    ATK | 1,  0x00,       0x16,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1,           0,   299, 0xb0,   0xc0,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
};

const Sint32 pl0d_super_00_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   342, 0xc0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   343, 0xc0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl0d_super_00_loop ),
};

/* Char_Super 1 */
const Sint32 pl0d_super_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   344, 0xc0,   0x60,        ATK,  0x00,          0,    0,   0,     0, 0x0600,  58 ),
	CG_NEXT( pl0d_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl0d_super_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   345, 0xc0,   0x60,        ATK,  0x00,          0,    0,   0,     0, 0x0700,  58 ),
	CG_NEXT( pl0d_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl0d_super_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   346, 0xc0,   0x60,        ATK,  0x00,          0,    0,   0,     0, 0x0900,  58 ),
	CG_NEXT( pl0d_super_03 ),
};

/* Char_Super 4 */
const Sint32 pl0d_super_04[] = {
	CG_FRAME(    0,    0,   8,           0,   394, 0xc0,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   8,           0,    95, 0xc0,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl0d_super_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   166, 0x98,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   167, 0x98,   0x60,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_super_04_loop ),
};

/* Char_Super 5 */
const Sint32 pl0d_super_05[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   109, 0x90,   0x80,          0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_super_05 ),
};

/* Char_Super 6: shl0d_00_atck_init (6 + type) */
const Sint32 pl0d_super_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32776, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl0d_super_06 ),
};

/* Char_Super 7 */
const Sint32 pl0d_super_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32777, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl0d_super_07 ),
};

/* Char_Super 8 */
const Sint32 pl0d_super_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32810, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl0d_super_08 ),
};

/* Char_Super 9 */
const Sint32 pl0d_super_09[] = {
	CG_FRAME(    0,    0,   6,           0,   475, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   476, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   477, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   478, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   479, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   480, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   481, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   482, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   483, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   484, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_super_09 ),
};

/* Char_Special 24: tornado_init (Gamma Tornado, 24 + atck_level) */
const Sint32 pl0d_special_24[] = {
	CG_FRAME(    0,    0,   8,           0,   125, 0x30,   0xab,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   5,           0,   126, 0x30,   0xab,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x30,   0xab,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x30,   0xe5,        ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   283, 0x30,   0xe5,        ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl0d_special_24 ),
};

/* Char_Special 26: tornado_move (Gamma Tornado, 26 + atck_level) */
const Sint32 pl0d_special_26[] = {
	CG_FRAME(    0,    0,   4,           0,   283, 0x30,   0xe5,        ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,  10,           0,   207, 0x80,   0xe5,        ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0xab,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x80,   0xab,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_special_26 ),
};

/* Char_Special 25 */
const Sint32 pl0d_special_25[] = {
	CG_FRAME(    0,    0,  12,           0,   125, 0x30,   0xab,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   5,           0,   126, 0x30,   0xab,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x30,   0xab,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x30,   0xe5,        ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   283, 0x30,   0xe5,        ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl0d_special_25 ),
};

/* Char_Special 27 */
const Sint32 pl0d_special_27[] = {
	CG_FRAME(    0,    0,   4,           0,   283, 0x30,   0xe5,        ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,  20,           0,   207, 0x80,   0xe5,        ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0xab,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x80,   0xab,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_special_27 ),
};

/* Char_Special 14: ucharge_g_init (Gamma Charge, upward, 14 + atck_level)
   Char_Special 15 */
const Sint32 pl0d_special_14[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
};

const Sint32 pl0d_special_14_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   297, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   298, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl0d_special_14_loop ),
};

/* Char_Special 16
   Char_Special 17 */
const Sint32 pl0d_special_16[] = {
	CG_FRAME(    0,    0,   4,           0,   290, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl0d_special_16_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   291, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   292, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl0d_special_16_loop ),
};

/* Char_Special 18
   Char_Special 19 */
const Sint32 pl0d_special_18[] = {
	CG_FRAME(    0,    0,   4,           0,   109, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl0d_special_18_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   301, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   302, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl0d_special_18_loop ),
};

/* Char_Special 20
   Char_Special 21 */
const Sint32 pl0d_special_20[] = {
	CG_FRAME(    0,    0,   4,           0,   293, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl0d_special_20_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   297, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   298, 0xb0,   0x60,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl0d_special_20_loop ),
};

/* Char_Special 22 */
const Sint32 pl0d_special_22[] = {
	CG_FRAME(    0,    0,   2,           0,   503, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   504, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   2,           0,   505, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    4,   2,           0,   508, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    2,   2,           0,   509, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,  10,           0,   510, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,  18,           0,   242, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   5,           0,   243, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl0d_special_23[] = {
	CG_FRAME(    0,    0,   6,           0,   503, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,  12,           0,   504, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   2,           0,   505, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    4,   2,           0,   508, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    2,   2,           0,   509, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,  10,           0,   510, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,  36,           0,   242, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   5,           0,   243, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   5,           0,   244, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   137, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_special_23 ),
};

/* Char_Special 28: shl0d_01_slam_rock_init */
const Sint32 pl0d_special_28[] = {
	CG_FRAME(    0,    0,  17,           0, 32811, 0x80,   0x00, ATK | 0x0e,  0x00,       0x1e,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   8,           0, 32812, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0, 32813, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   8,           0, 32814, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0, 32815, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_special_28_loop[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA, 32816, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_special_28_loop ),
};

/* Char_Super 10: wave_init (Gamma Wave (hyper)) */
const Sint32 pl0d_super_10[] = {
	CG_FRAME(    0,    0,   8,           0,   503, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,   1,           0,   504, 0x80,   0x00,    ATK | 6,  0x00,       0x0f,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    8,   1,           0,   505, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    4,   3,           0,   508, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   8,           0,   509, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    2,   3,           0,   510, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,  26,           0,   242, 0x80,   0x00,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   5,           0,   243, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   137, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0d_super_10 ),
};

/* Char_Super 11: shl0d_01_wave_rock_init */
const Sint32 pl0d_super_11[] = {
	CG_FRAME(    0,    0,  24,           0, 32811, 0x80,   0x00, ATK | 0x0e,  0x00,       0x1e,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   8,           0, 32812, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0, 32813, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   8,           0, 32814, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0, 32815, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_super_11_loop[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA, 32816, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_super_11_loop ),
};

/* Char_Super 12 */
const Sint32 pl0d_super_12[] = {
	CG_FRAME(    0,    0,   8,           0,   498, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   499, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   500, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   501, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   502, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_super_12 ),
};

/* Char_Super 13: quake_init (Gamma Quake) */
const Sint32 pl0d_super_13[] = {
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   353, 0x80,   0x00,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    1,   2,           0,   355, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x82,    0,   2,           0,   356, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   347, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    2,   4,           0,   348, 0x80,   0x00,    ATK | 4,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   4,           0,   349, 0x80,   0x00, ATK | 0x18,  0x00,       0x03,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,  64,           0,   350, 0x80,   0x00, ATK | 0x17,  0x00,       0x1e,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   353, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   354, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_super_13 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl0d_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   376, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   375, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl0d_damage_01[] = {
	CG_FRAME(    0,    0,  14,           0,   377, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   375, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl0d_damage_02[] = {
	CG_FRAME(    0,    0,  16,           0,   379, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   380, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl0d_damage_03[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   381, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl0d_damage_04[] = {
	CG_FRAME(    0,    0,  18, END_OF_DATA,   382, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl0d_damage_05[] = {
	CG_FRAME(    0,    0,  12,           0,   383, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   385, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   386, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl0d_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   388, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   387, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl0d_damage_07[] = {
	CG_FRAME(    0,    0,  14,           0,   389, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   387, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl0d_damage_08[] = {
	CG_FRAME(    0,    0,  20,           0,   390, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   391, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0d_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl0d_damage_09[] = {
	CG_FRAME(    0,    0,   5,           0,   398, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   399, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   406, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   408, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl0d_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   397, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl0d_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl0d_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   392, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl0d_damage_11[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   393, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl0d_damage_12[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   394, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl0d_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   395, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   396, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   397, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl0d_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl0d_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   402, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl0d_damage_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   403, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl0d_damage_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   404, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl0d_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   405, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl0d_damage_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   406, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl0d_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 22 */
const Sint32 pl0d_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 23 */
const Sint32 pl0d_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   404, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 24 */
const Sint32 pl0d_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   405, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 25 */
const Sint32 pl0d_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   406, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl0d_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   397, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   408, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   397, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl0d_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl0d_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   430, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl0d_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl0d_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   410, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   411, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   412, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   394, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl0d_damage_30 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl0d_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   414, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   419, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl0d_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   420, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   422, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   423, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   424, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   425, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0d_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl0d_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   397, 0x80,   0xe0,          0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl0d_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl0d_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   430, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_damage_32 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl0d_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   426, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   427, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   428, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0d_dizzy_00 ),
};

/* Char_Caught 0 */
const Sint32 pl0d_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   375, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0d_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl0d_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   377, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0d_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl0d_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   378, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0d_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl0d_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   382, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0d_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl0d_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   381, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0d_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl0d_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   383, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl0d_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl0d_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   387, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0d_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl0d_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   389, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0d_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl0d_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   390, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0d_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl0d_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   393, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl0d_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   392, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl0d_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   394, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl0d_caught_12[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   395, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0d_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl0d_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   401, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl0d_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   403, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl0d_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   404, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl0d_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   405, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl0d_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   406, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl0d_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   414, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl0d_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   420, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0d_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl0d_caught_20[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   409, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl0d_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl0d_caught_21[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   407, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl0d_caught_21 ),
};

/* Char_Effect 0 */
const Sint32 pl0d_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0,   431, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   432, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   434, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   435, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   436, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   437, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   438, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   442, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl0d_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   449, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   452, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   454, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl0d_effect_02[] = {
	CG_FRAME(    0,    0,   3,           0,   455, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   456, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   457, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   458, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   459, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   460, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   461, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   462, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   463, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   464, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   465, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl0d_effect_03[] = {
	CG_FRAME(    0,    0,   3,           0,   466, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   467, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   468, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   469, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   470, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   471, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   472, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   473, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   474, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_03 ),
};

/* Char_Effect 5 */
const Sint32 pl0d_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32769, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32770, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32771, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl0d_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0, 32772, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32774, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32775, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   489, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl0d_effect_07[] = {
	CG_FRAME(    0,    0,   1,           0,   490, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   491, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   492, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   493, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_07 ),
};

/* Char_Appear 0 */
const Sint32 pl0d_appear_00[] = {
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   167, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_appear_00 ),
};

/* Char_Appear 1 */
const Sint32 pl0d_appear_01[] = {
	CG_FRAME(    0,    0,  12,           0,   168, 0x80,   0x00,       0x16,  0x00, 0x35 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,          5,  0x00,       0x05,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_appear_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_appear_01_loop ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl0d_catch_00[] = {
	CG_FRAME(    0,    0,  16,           0,   283, 0x80,   0xe4,          0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,          5,  0x00,       0x05,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0, 0x80,  16,           0,   287, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   288, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   289, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_catch_00 ),
};

/* Char_Catch 1: jump_p_set */
const Sint32 pl0d_catch_01[] = {
	CG_FRAME(    0,    0,  12,           0,   339, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   8,           0,   336, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x60,          5,  0x00,       0x05,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA,   338, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_catch_01 ),
};

/* Char_Effect 4: pls0d_00_c_init */
const Sint32 pl0d_effect_04[] = {
	CG_FRAME(    0,    0,   9,           0,   340, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   341, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_04 ),
};

/* Char_Catch 2: tornado_set (2 + atck_level) */
const Sint32 pl0d_catch_02[] = {
	CG_FRAME(    0,    0,   8,           0,   283, 0x80,   0xe4,          0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   266, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   4,           0,   268, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   269, 0x80,   0xc3,       0x15,  0x00, 0x16 | CSE,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x80,   0xcb,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    2,   4,           0,   275, 0x80,   0xcb,          5,  0x00,       0x05,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0xcb,       0x15,  0x00, 0x16 | CSE,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0, 0x81,   8,           0,   277, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   6,           0,   278, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   6,           0,   279, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   280, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   281, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4, END_OF_DATA,   282, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_catch_02 ),
};

/* Char_Catch 3 */
const Sint32 pl0d_catch_03[] = {
	CG_FRAME(    0,    0,   8,           0,   283, 0x80,   0xe4,          0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   266, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   5,           0,   269, 0x80,   0xc3,       0x15,  0x00, 0x16 | CSE,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   5,           0,   270, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   6,           0,   271, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   5,           0,   272, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   4,           0,   268, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   269, 0x80,   0xc3,       0x15,  0x00, 0x16 | CSE,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   5,           0,   271, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   5,           0,   273, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   5,           0,   274, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   3,           0,   269, 0x80,   0xc3,       0x15,  0x00, 0x16 | CSE,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   3,           0,   272, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x80,   0xc3,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0xcb,          0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    2,   4,           0,   275, 0x80,   0xcb,          5,  0x00,       0x05,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0xcb,       0x15,  0x00, 0x16 | CSE,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0, 0x81,   4,           0,   277, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   278, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   279, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   280, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4,           0,   281, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,   4, END_OF_DATA,   282, 0x80,   0xa0,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_catch_03 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl0d_get_up_00[] = {
	CG_FRAME(    0,    0,   8,           0,   169, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   170, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   171, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl0d_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   170, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   171, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   130, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   131, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   132, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl0d_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   170, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   171, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   130, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   131, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   132, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl0d_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   170, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   171, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   130, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   131, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   132, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_get_up_03 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl0d_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_change_02 ),
};

/* Char_Effect 8
   Char_Effect 9
   Char_Effect 10
   Char_Effect 11
   Char_Effect 19: shl0d_00_trail_init
   Char_Effect 20 */
const Sint32 pl0d_effect_08[] = {
	CG_FRAME(    0,    0,   8,           0, 32779, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_effect_08_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32780, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32781, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32778, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32779, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_08_loop ),
};

/* Char_Effect 12 */
const Sint32 pl0d_effect_12[] = {
	CG_FRAME(    0,    0,   8,           0, 32784, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_effect_12_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32782, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32785, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32784, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_12_loop ),
};

/* Char_Effect 13 */
const Sint32 pl0d_effect_13[] = {
	CG_FRAME(    0,    0,   1,           0, 32786, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32787, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32788, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32789, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl0d_effect_14[] = {
	CG_FRAME(    0,    0,   1,           0, 32790, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32793, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl0d_effect_15[] = {
	CG_FRAME(    0,    0,   1,           0, 32794, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32797, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl0d_effect_16[] = {
	CG_FRAME(    0,    0,   1,           0, 32798, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32799, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32800, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32801, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl0d_effect_17[] = {
	CG_FRAME(    0,    0,   1,           0, 32802, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32803, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32804, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32805, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl0d_effect_18[] = {
	CG_FRAME(    0,    0,   1,           0, 32806, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32807, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32808, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32809, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_18 ),
};

/* Char_Effect 21 */
const Sint32 pl0d_effect_21[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32817, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl0d_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl0d_effect_22[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32818, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl0d_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl0d_effect_23[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32819, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl0d_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl0d_effect_24[] = {
	CG_FRAME(    0,    0,   8,           0, 32779, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_effect_24_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32780, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32781, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32778, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32779, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_24_loop ),
};

/* Char_Effect 25 */
const Sint32 pl0d_effect_25[] = {
	CG_FRAME(    0,    0,   8,           0, 32784, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0d_effect_25_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32782, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32785, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32784, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_25_loop ),
};

/* Char_Effect 26 */
const Sint32 pl0d_effect_26[] = {
	CG_FRAME(    0,    0,   2,           0, 32786, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32789, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_26 ),
};

/* Char_Effect 27 */
const Sint32 pl0d_effect_27[] = {
	CG_FRAME(    0,    0,   2,           0, 32790, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32791, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32792, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32793, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_27 ),
};

/* Char_Effect 28 */
const Sint32 pl0d_effect_28[] = {
	CG_FRAME(    0,    0,   2,           0, 32794, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32797, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_28 ),
};

/* Char_Effect 29 */
const Sint32 pl0d_effect_29[] = {
	CG_FRAME(    0,    0,   2,           0, 32798, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32799, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32800, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32801, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_29 ),
};

/* Char_Effect 30 */
const Sint32 pl0d_effect_30[] = {
	CG_FRAME(    0,    0,   2,           0, 32802, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32803, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32804, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32805, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0d_effect_30 ),
};

/* Char_Special 29: snapback_init (Snapback) */
const Sint32 pl0d_special_29[] = {
	CG_FRAME(    0,    0,  10,           0,   208, 0x80,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    1,   1,           0,   208, 0x80,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0xa8, ATK | 0x14,  0x00, 0x16 | CSE,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   2,           0,   209, 0x80,   0xa1,    ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0xa1,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x01,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,  22,           0,   210, 0x80,   0x01,        ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   7,           0,   211, 0x80,   0x01,        ATE,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   7,           0,   109, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   130, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   131, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   132, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl0d_stand_p_02 ),
};
