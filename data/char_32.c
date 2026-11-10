/* Colossus (character 32): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_32      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_32     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl32.c) says which group
                   and number each move uses.
   pl32_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl32_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl32_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl32_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_32
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_32, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_32 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl32_stand_00[], pl32_stand_02[], pl32_stand_03[], pl32_stand_06[],
	pl32_stand_07[], pl32_jump_00[], pl32_jump_09[], pl32_jump_01[], pl32_jump_10[], pl32_jump_02[],
	pl32_jump_11[], pl32_jump_03[], pl32_jump_04[], pl32_jump_05[], pl32_jump_06[],
	pl32_catch_15_loop_loop[], pl32_jump_07[], pl32_jump_07_loop[], pl32_jump_08[],
	pl32_jump_08_loop[], pl32_dash_00[], pl32_crouch_00[], pl32_crouch_01[], pl32_crouch_02[],
	pl32_turn_00[], pl32_turn_01[], pl32_guard_00[], pl32_guard_01[], pl32_guard_02[],
	pl32_guard_04[], pl32_guard_05[], pl32_guard_06[], pl32_appear_00[], pl32_appear_01[],
	pl32_appear_01_loop[], pl32_crouch_p_00[], pl32_extra_06[], pl32_crouch_p_01[],
	pl32_crouch_p_02[], pl32_stand_p_00[], pl32_extra_04[], pl32_extra_05[], pl32_stand_p_01[],
	pl32_stand_p_02[], pl32_jump_p_00[], pl32_extra_07[], pl32_jump_p_01[], pl32_jump_p_05[],
	pl32_jump_p_02[], pl32_dash_01[], pl32_dash_01_loop[], pl32_dash_03[], pl32_stand_k_00[],
	pl32_stand_k_01[], pl32_stand_k_02[], pl32_extra_03[], pl32_crouch_k_00[], pl32_crouch_k_01[],
	pl32_crouch_k_02[], pl32_jump_k_00[], pl32_jump_k_01[], pl32_jump_k_02[], pl32_unused_00[],
	pl32_unused_01[], pl32_get_up_00[], pl32_get_up_01[], pl32_get_up_02[], pl32_get_up_03[],
	pl32_air_guard_00[], pl32_air_guard_01[], pl32_damage_16[], pl32_damage_17[], pl32_damage_18[],
	pl32_damage_19[], pl32_damage_20[], pl32_damage_21[], pl32_damage_22[], pl32_damage_23[],
	pl32_damage_24[], pl32_damage_25[], pl32_damage_26[], pl32_damage_26_loop[], pl32_damage_27[],
	pl32_damage_30[], pl32_damage_30_loop[], pl32_damage_32[], pl32_damage_28[], pl32_damage_29[],
	pl32_damage_09[], pl32_damage_09_loop[], pl32_damage_31[], pl32_dizzy_00[], pl32_damage_00[],
	pl32_damage_01[], pl32_damage_02[], pl32_damage_03[], pl32_damage_04[], pl32_damage_05[],
	pl32_damage_06[], pl32_damage_07[], pl32_damage_08[], pl32_damage_13[], pl32_damage_10[],
	pl32_damage_11[], pl32_damage_12[], pl32_super_03[], pl32_super_03_loop[], pl32_win_00[],
	pl32_win_00_loop[], pl32_win_01[], pl32_win_01_loop[], pl32_win_04[], pl32_win_04_loop[],
	pl32_win_03[], pl32_win_03_loop[], pl32_win_02[], pl32_win_02_loop[], pl32_extra_02[],
	pl32_effect_04[], pl32_catch_03[], pl32_catch_23[], pl32_catch_04[], pl32_catch_05[],
	pl32_catch_06[], pl32_catch_24[], pl32_catch_07[], pl32_catch_07_loop[], pl32_catch_08[],
	pl32_catch_10[], pl32_catch_19[], pl32_catch_11[], pl32_catch_22[], pl32_catch_20[],
	pl32_catch_20_loop[], pl32_catch_21[], pl32_catch_16[], pl32_catch_16_loop[], pl32_super_01[],
	pl32_super_02[], pl32_special_00[], pl32_unused_02[], pl32_special_02[], pl32_special_03[],
	pl32_unused_03[], pl32_unused_04[], pl32_unused_05[], pl32_unused_06[], pl32_unused_07[],
	pl32_unused_08[], pl32_unused_09[], pl32_unused_10[], pl32_unused_11[], pl32_unused_12[],
	pl32_unused_13[], pl32_unused_14[], pl32_unused_15[], pl32_unused_16[], pl32_unused_17[],
	pl32_unused_18[], pl32_unused_19[], pl32_unused_20[], pl32_catch_18[], pl32_catch_12[],
	pl32_catch_13[], pl32_catch_14[], pl32_catch_15[], pl32_catch_15_loop[], pl32_special_04[],
	pl32_special_04_loop[], pl32_unused_21[], pl32_special_06[], pl32_catch_00[], pl32_catch_01[],
	pl32_catch_01_loop[], pl32_catch_02[], pl32_catch_02_loop[], pl32_special_11[],
	pl32_special_11_loop[], pl32_unused_22[], pl32_extra_08[], pl32_unused_23[], pl32_grp27_00[],
	pl32_grp27_01[], pl32_grp27_02[], pl32_grp27_03[], pl32_grp27_04[], pl32_grp27_05[],
	pl32_grp27_06[], pl32_grp27_07[], pl32_grp27_08[], pl32_caught_00[], pl32_caught_01[],
	pl32_caught_02[], pl32_caught_03[], pl32_caught_04[], pl32_caught_05[], pl32_caught_06[],
	pl32_caught_07[], pl32_caught_08[], pl32_caught_09[], pl32_caught_10[], pl32_caught_11[],
	pl32_caught_12[], pl32_caught_13[], pl32_caught_14[], pl32_caught_15[], pl32_caught_16[],
	pl32_caught_17[], pl32_caught_18[], pl32_caught_19[], pl32_caught_20[], pl32_caught_21[],
	pl32_extra_00[], pl32_extra_01[], pl32_extra_01_loop[], pl32_effect_01[], pl32_effect_03[],
	pl32_effect_00[], pl32_effect_00_loop[], pl32_effect_02[], pl32_special_08[];
extern const Sint32 *const pl32_stand_tbl[], *const pl32_jump_tbl[], *const pl32_dash_tbl[],
	*const pl32_crouch_tbl[], *const pl32_turn_tbl[], *const pl32_guard_tbl[],
	*const pl32_air_guard_tbl[], *const pl32_stand_p_tbl[], *const pl32_stand_k_tbl[],
	*const pl32_crouch_p_tbl[], *const pl32_crouch_k_tbl[], *const pl32_jump_p_tbl[],
	*const pl32_jump_k_tbl[], *const pl32_damage_tbl[], *const pl32_dizzy_tbl[],
	*const pl32_catch_tbl[], *const pl32_caught_tbl[], *const pl32_get_up_tbl[],
	*const pl32_appear_tbl[], *const pl32_win_tbl[], *const pl32_extra_tbl[],
	*const pl32_special_tbl[], *const pl32_super_tbl[], *const pl32_effect_tbl[],
	*const pl32_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_32[16] = {
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
const Sint32 *const *const char_tbl_32[] = {
	pl32_stand_tbl, pl32_jump_tbl, pl32_dash_tbl, pl32_crouch_tbl, pl32_turn_tbl, pl32_guard_tbl,
	pl32_air_guard_tbl, pl32_stand_p_tbl, pl32_stand_k_tbl, pl32_crouch_p_tbl, pl32_crouch_k_tbl, pl32_jump_p_tbl,
	pl32_jump_k_tbl, pl32_damage_tbl, pl32_dizzy_tbl, pl32_catch_tbl, pl32_caught_tbl, pl32_get_up_tbl,
	pl32_appear_tbl, pl32_win_tbl, pl32_extra_tbl, pl32_special_tbl, pl32_super_tbl, pl32_effect_tbl,
	pl32_effect_tbl, pl32_effect_tbl, pl32_stand_tbl, pl32_grp27_tbl, 0,
};

const Sint32 *const pl32_stand_tbl[] = {
	pl32_stand_00, pl32_stand_00, pl32_stand_02, pl32_stand_03, pl32_stand_00, pl32_stand_00,
	pl32_stand_06, pl32_stand_07, pl32_stand_02, pl32_stand_03, 0,
};

const Sint32 *const pl32_jump_tbl[] = {
	pl32_jump_00, pl32_jump_01, pl32_jump_02, pl32_jump_03, pl32_jump_04, pl32_jump_05,
	pl32_jump_06, pl32_jump_07, pl32_jump_08, pl32_jump_09, pl32_jump_10, pl32_jump_11,
	0,
};

const Sint32 *const pl32_dash_tbl[] = {
	pl32_dash_00, pl32_dash_01, pl32_dash_00, pl32_dash_03, 0,
};

const Sint32 *const pl32_crouch_tbl[] = {
	pl32_crouch_00, pl32_crouch_01, pl32_crouch_02, 0,
};

const Sint32 *const pl32_turn_tbl[] = {
	pl32_turn_00, pl32_turn_01, 0,
};

const Sint32 *const pl32_guard_tbl[] = {
	pl32_guard_00, pl32_guard_01, pl32_guard_02, pl32_guard_01, pl32_guard_04, pl32_guard_05,
	pl32_guard_06, pl32_guard_05, 0,
};

const Sint32 *const pl32_air_guard_tbl[] = {
	pl32_air_guard_00, pl32_air_guard_01, pl32_air_guard_01, 0,
};

const Sint32 *const pl32_stand_p_tbl[] = {
	pl32_stand_p_00, pl32_stand_p_01, pl32_stand_p_02, 0,
};

const Sint32 *const pl32_stand_k_tbl[] = {
	pl32_stand_k_00, pl32_stand_k_01, pl32_stand_k_02, 0,
};

const Sint32 *const pl32_crouch_p_tbl[] = {
	pl32_crouch_p_00, pl32_crouch_p_01, pl32_crouch_p_02, 0,
};

const Sint32 *const pl32_crouch_k_tbl[] = {
	pl32_crouch_k_00, pl32_crouch_k_01, pl32_crouch_k_02, 0,
};

const Sint32 *const pl32_jump_p_tbl[] = {
	pl32_jump_p_00, pl32_jump_p_01, pl32_jump_p_02, pl32_jump_p_00, pl32_jump_p_01, pl32_jump_p_05,
	0,
};

const Sint32 *const pl32_jump_k_tbl[] = {
	pl32_jump_k_00, pl32_jump_k_01, pl32_jump_k_02, pl32_jump_k_00, pl32_jump_k_01, pl32_jump_k_02,
	0,
};

const Sint32 *const pl32_damage_tbl[] = {
	pl32_damage_00, pl32_damage_01, pl32_damage_02, pl32_damage_03, pl32_damage_04, pl32_damage_05,
	pl32_damage_06, pl32_damage_07, pl32_damage_08, pl32_damage_09, pl32_damage_10, pl32_damage_11,
	pl32_damage_12, pl32_damage_13, pl32_damage_13, pl32_damage_13, pl32_damage_16, pl32_damage_17,
	pl32_damage_18, pl32_damage_19, pl32_damage_20, pl32_damage_21, pl32_damage_22, pl32_damage_23,
	pl32_damage_24, pl32_damage_25, pl32_damage_26, pl32_damage_27, pl32_damage_28, pl32_damage_29,
	pl32_damage_30, pl32_damage_31, pl32_damage_32, pl32_damage_27, pl32_damage_13, 0,
};

const Sint32 *const pl32_dizzy_tbl[] = {
	pl32_dizzy_00, 0,
};

const Sint32 *const pl32_catch_tbl[] = {
	pl32_catch_00, pl32_catch_01, pl32_catch_02, pl32_catch_03, pl32_catch_04, pl32_catch_05,
	pl32_catch_06, pl32_catch_07, pl32_catch_08, 0, pl32_catch_10, pl32_catch_11,
	pl32_catch_12, pl32_catch_13, pl32_catch_14, pl32_catch_15, pl32_catch_16, pl32_catch_16,
	pl32_catch_18, pl32_catch_19, pl32_catch_20, pl32_catch_21, pl32_catch_22, pl32_catch_23,
	pl32_catch_24, 0,
};

const Sint32 *const pl32_caught_tbl[] = {
	pl32_caught_00, pl32_caught_01, pl32_caught_02, pl32_caught_03, pl32_caught_04, pl32_caught_05,
	pl32_caught_06, pl32_caught_07, pl32_caught_08, pl32_caught_09, pl32_caught_10, pl32_caught_11,
	pl32_caught_12, pl32_caught_13, pl32_caught_14, pl32_caught_15, pl32_caught_16, pl32_caught_17,
	pl32_caught_18, pl32_caught_19, pl32_caught_20, pl32_caught_21, 0,
};

const Sint32 *const pl32_get_up_tbl[] = {
	pl32_get_up_00, pl32_get_up_01, pl32_get_up_02, pl32_get_up_03, 0,
};

const Sint32 *const pl32_appear_tbl[] = {
	pl32_appear_00, pl32_appear_01, 0,
};

const Sint32 *const pl32_win_tbl[] = {
	pl32_win_00, pl32_win_01, pl32_win_02, pl32_win_03, pl32_win_04, 0,
};

const Sint32 *const pl32_extra_tbl[] = {
	pl32_extra_00, pl32_extra_01, pl32_extra_02, pl32_extra_03, pl32_extra_04, pl32_extra_05,
	pl32_extra_06, pl32_extra_07, pl32_extra_08, 0,
};

const Sint32 *const pl32_special_tbl[] = {
	pl32_special_00, pl32_special_00, pl32_special_02, pl32_special_03, pl32_special_04, pl32_special_04,
	pl32_special_06, pl32_special_06, pl32_special_08, pl32_special_03, pl32_special_03, pl32_special_11,
	0,
};

const Sint32 *const pl32_super_tbl[] = {
	pl32_catch_16, pl32_super_01, pl32_super_02, pl32_super_03, 0,
};

const Sint32 *const pl32_effect_tbl[] = {
	pl32_effect_00, pl32_effect_01, pl32_effect_02, pl32_effect_03, pl32_effect_04, 0,
};

const Sint32 *const pl32_grp27_tbl[] = {
	pl32_grp27_00, pl32_grp27_01, pl32_grp27_02, pl32_grp27_03, pl32_grp27_04, pl32_grp27_05,
	pl32_grp27_06, pl32_grp27_07, pl32_grp27_08, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Fly 0
   Char_Fly 1: act_fly */
const Sint32 pl32_stand_00[] = {
	CG_FRAME(   23,    0,  10,           0,    13, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   24,    0,  10,           0,    14, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   25,    0,  10,           0,    15, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   26,    0,  10,           0,    16, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   27,    0,  10,           0,    17, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   28,    0,  10, END_OF_DATA,    18, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 8
   Char_Fly 2
   Char_Fly 8 */
const Sint32 pl32_stand_02[] = {
	CG_FRAME(    0,    0,   5,           0,    20, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    22, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    23, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    24, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   5,           0,    26, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    27, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    30, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 9
   Char_Fly 3
   Char_Fly 9 */
const Sint32 pl32_stand_03[] = {
	CG_FRAME(    0,    0,   5,           0,    31, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    32, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    33, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    35, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_stand_03 ),
};

/* Char_Stand 6
   Char_Fly 6 */
const Sint32 pl32_stand_06[] = {
	CG_FRAME(    0,    0,   8,           0,    18, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  14,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_stand_06 ),
};

/* Char_Stand 7
   Char_Fly 7 */
const Sint32 pl32_stand_07[] = {
	CG_FRAME(    0,    0,   8,           0,    18, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  14,           0,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl32_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl32_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl32_jump_01[] = {
	CG_FRAME(    0,    1,   2,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl32_jump_10[] = {
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_jump_10 ),
};

/* Char_Jump 2 */
const Sint32 pl32_jump_02[] = {
	CG_FRAME(    0,    1,   2,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl32_jump_11[] = {
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_jump_11 ),
};

/* Char_Jump 3 */
const Sint32 pl32_jump_03[] = {
	CG_FRAME(    0,    0,   1,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   1,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl32_jump_04[] = {
	CG_FRAME(    0,    0,   1,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   1,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl32_jump_05[] = {
	CG_FRAME(    0,    0,   1,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   1,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_jump_05 ),
};

/* Char_Jump 6 */
const Sint32 pl32_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl32_catch_15_loop_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_catch_15_loop_loop ),
};

/* Char_Jump 7 */
const Sint32 pl32_jump_07[] = {
	CG_FRAME(    0,    1,   2,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl32_jump_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl32_jump_08[] = {
	CG_FRAME(    0,    1,   2,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl32_jump_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_jump_08_loop ),
};

/* Char_Dash 0: act_dash
   Char_Dash 2 */
const Sint32 pl32_dash_00[] = {
	CG_FRAME(    0,    1,   4,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl32_dash_00 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl32_crouch_00[] = {
	CG_FRAME(    3,    0,   2,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    2,    0,   2,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,    0,   2,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl32_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl32_crouch_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    19, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl32_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl32_crouch_02[] = {
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   2,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   2,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   2, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl32_turn_00[] = {
	CG_FRAME(    0,    0,   6,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl32_turn_01[] = {
	CG_FRAME(    0,    0,   6,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl32_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl32_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl32_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl32_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl32_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl32_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl32_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl32_guard_06[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl32_guard_06 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl32_appear_00[] = {
	CG_FRAME(    0,    0,   3,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_appear_00 ),
};

/* Char_Appear 1: appear_fall */
const Sint32 pl32_appear_01[] = {
	CG_FRAME(    0,    0,  16,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_appear_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_appear_01_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl32_crouch_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   195, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    1,   3,           0,   197, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   198, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_crouch_p_00 ),
};

/* Char_Extra 6 */
const Sint32 pl32_extra_06[] = {
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_extra_06 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl32_crouch_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   199, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   200, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   203, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   203, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   203, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   7,           0,   204, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   205, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl32_crouch_p_02[] = {
	CG_FRAME(    0,    0,  10,           0,   206, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   208, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   209, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   210, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,  10,           0,   212, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   213, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_crouch_p_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl32_stand_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   148, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   1,           0,   149, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    1,   3,           0,   154, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   155, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_stand_p_00 ),
};

/* Char_Extra 4 */
const Sint32 pl32_extra_04[] = {
	CG_FRAME(    0,    0,   1,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl32_extra_05[] = {
	CG_FRAME(    0,    0,   1,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_extra_05 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl32_stand_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   156, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   159, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   1,           0,   160, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   1,           0,   160, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   7,           0,   162, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   163, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl32_stand_p_02[] = {
	CG_FRAME(    1,    0,   1,           0,   165, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    2,    0,   2,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    1,    0,   1,           0,   165, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    2,    0,   1,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    1,    0,   2,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    2,    0,   1,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    1,    0,   2,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   1,           0,   168, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   173, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   1,           0,   169, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    4,    0,   2,           0,   170, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    5,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    6,    0,   2,           0,   170, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    7,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    8,    0,   2,           0,   170, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    9,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   8,           0,   172, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   168, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_stand_p_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl32_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   232, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   233, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    1,   1,           0,   235, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    1,  16,           0,   236, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    1,   1,           0,   237, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   2,           0,   238, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,   2,           0,   239, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   1,           0,   240, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   1,           0,   241, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   242, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_p_00 ),
};

/* Char_Extra 7 */
const Sint32 pl32_extra_07[] = {
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   239, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_extra_07 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl32_jump_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   243, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   244, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x95,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   5,           0,   250, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   252, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_p_01 ),
};

/* Char_Jump_P 5 */
const Sint32 pl32_jump_p_05[] = {
	CG_FRAME(    0,    0,   1,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   292, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x00,  ATE,  0x00, 0x16 | CSE,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   1,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3,           0,   296, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3,           0,   297, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   300, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   301, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   302, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   303, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_p_05 ),
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl32_jump_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   253, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   254, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   255, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   256, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   263, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   4,           0,   257, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   264, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   260, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   261, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   262, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_p_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl32_dash_01[] = {
	CG_FRAME(    0,    1,   2,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl32_dash_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl32_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl32_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_dash_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl32_stand_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   175, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   176, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   177, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl32_stand_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   179, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,   1,           0,   180, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   5,           0,   182, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   8,           0,   179, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   183, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy): stand_hk_init (standing heavy kick (gamma assist and counter)) */
const Sint32 pl32_stand_k_02[] = {
	CG_FRAME(    0,    0,  10,           0,   184, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   1,           0,   186, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   1,           0,   192, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,   1,           0,   193, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,  10,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   190, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_stand_k_02 ),
};

/* Char_Extra 3: pl32_upper, fly_start5 */
const Sint32 pl32_extra_03[] = {
	CG_FRAME(    0,    0,  10,           0,   184, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   1,           0,   186, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   1,           0,   192, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,   1,           0,   193, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,  10,           0,   189, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   190, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_extra_03 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl32_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   215, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   5,           0,   216, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   217, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl32_crouch_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   222, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   218, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   219, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0,   223, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0,   223, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   8,           0,   221, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   222, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl32_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   226, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   227, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   7,           0,   228, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   229, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   230, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_crouch_k_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl32_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   268, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   269, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl32_jump_k_01[] = {
	CG_FRAME(    0,    0,   5,           0,   270, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   271, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   275, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   276, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl32_jump_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   282, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   285, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,  10,           0,   288, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   289, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_jump_k_02 ),
};

const Sint32 pl32_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   284, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00002680,
};

const Sint32 pl32_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   288, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   289, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00002724,
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl32_get_up_00[] = {
	CG_FRAME(    0,    0,   5,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl32_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl32_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl32_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_get_up_03 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl32_air_guard_00[] = {
	CG_FRAME(    0,    0,  10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl32_air_guard_01[] = {
	CG_FRAME(    0,    0,  14, END_OF_DATA,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_air_guard_01 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl32_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   507, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl32_damage_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   508, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl32_damage_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   509, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl32_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl32_damage_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   511, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl32_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

/* Char_Damage 22 */
const Sint32 pl32_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   508, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

/* Char_Damage 23 */
const Sint32 pl32_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   509, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

/* Char_Damage 24 */
const Sint32 pl32_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

/* Char_Damage 25 */
const Sint32 pl32_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   511, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl32_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl32_damage_26_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   515, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   516, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl32_damage_26_loop ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl32_damage_27[] = {
	CG_FRAME(    0,    0,   5,           0,   512, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   7,           0,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl32_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   518, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   520, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   521, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   523, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl32_damage_30_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   525, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_30_loop ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl32_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   518, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   520, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   521, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   523, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   524, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl32_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   529, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   530, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   531, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl32_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   532, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   533, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   534, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   535, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   536, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   537, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_29 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl32_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl32_damage_09_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl32_damage_09_loop ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl32_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl32_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl32_dizzy_00[] = {
	CG_FRAME(    0,    0,   9,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_dizzy_00 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl32_damage_00[] = {
	CG_FRAME( 0x81,    0,   5,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl32_damage_01[] = {
	CG_FRAME( 0x81,    0,   9,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl32_damage_02[] = {
	CG_FRAME(    0,    0,  14,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   473, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl32_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   474, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl32_damage_04[] = {
	CG_FRAME( 0x81,    0,  14,           0,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   474, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl32_damage_05[] = {
	CG_FRAME(    0,    0,  14,           0,   477, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   482, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl32_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   483, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl32_damage_07[] = {
	CG_FRAME(    0,    0,  18, END_OF_DATA,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl32_damage_08[] = {
	CG_FRAME(    0,    0,  14,           0,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   486, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   487, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   488, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_damage_08 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl32_damage_13[] = {
	CG_FRAME( 0x80,    0,   2,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x82,    0,   2,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl32_damage_13 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl32_damage_10[] = {
	CG_FRAME( 0x81,    0,  10, END_OF_DATA,   491, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl32_damage_11[] = {
	CG_FRAME( 0x81,    0,  10, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl32_damage_12[] = {
	CG_FRAME( 0x81,    0,  10, END_OF_DATA,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_damage_12 ),
};

/* Char_Super 3: sp_armor_init (Super Armor) */
const Sint32 pl32_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   454, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   454, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0,      0,   1 ),
};

const Sint32 pl32_super_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   455, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   2, END_OF_DATA,   456, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_super_03_loop ),
};

/* Char_Win 0: win_time */
const Sint32 pl32_win_00[] = {
	CG_FRAME(    0,    0,  16,           0,   454, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   456, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_win_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   456, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl32_win_01[] = {
	CG_FRAME(    0,    0,   8,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   146, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_win_01_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_win_01_loop ),
};

/* Char_Win 4: draw_time */
const Sint32 pl32_win_04[] = {
	CG_FRAME(    0,    0,   6,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_win_04_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_win_04_loop ),
};

/* Char_Win 3 */
const Sint32 pl32_win_03[] = {
	CG_FRAME(    0,    0,   8,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_win_03_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_win_03_loop ),
};

/* Char_Win 2 */
const Sint32 pl32_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_win_02_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_win_02_loop ),
};

/* Char_Extra 2: chouhatsu_init (the taunt), fly_start5 */
const Sint32 pl32_extra_02[] = {
	CG_FRAME(    0,    0,   6,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   138, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_extra_02 ),
};

/* Char_Effect 4
   Char_Change 4: act_change_assist_end
   group 25 4 */
const Sint32 pl32_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_effect_04 ),
};

/* Char_Catch 3: nml_k_set */
const Sint32 pl32_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   6,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   2,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
};

/* Char_Catch 23 */
const Sint32 pl32_catch_23[] = {
	CG_FRAME(   16,    0,   1,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(   17,    1,  24,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_NEXT( pl32_catch_03 ),
};

/* Char_Catch 4: nml_k_fall */
const Sint32 pl32_catch_04[] = {
	CG_FRAME(    0,    0,  48,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_04 ),
};

/* Char_Catch 5: pls32_00_nml_k_init, pls32_00_nml_k_frame */
const Sint32 pl32_catch_05[] = {
	CG_FRAME(    0,    0,   8,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_05 ),
};

/* Char_Catch 6: nml_p_set */
const Sint32 pl32_catch_06[] = {
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   7,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
};

/* Char_Catch 24 */
const Sint32 pl32_catch_24[] = {
	CG_FRAME(    0,    0,   4,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   2,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   1,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,    0,  0x00,       0x06,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,   2,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    1,  14,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_06 ),
};

/* Char_Catch 7: jump_p_set */
const Sint32 pl32_catch_07[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
};

const Sint32 pl32_catch_07_loop[] = {
	CG_FRAME(    0,    1,  -1, END_OF_DATA,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_NEXT( pl32_catch_07_loop ),
};

/* Char_Catch 8: jump_p_fall */
const Sint32 pl32_catch_08[] = {
	CG_FRAME(    0,    0,  48,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_08 ),
};

/* Char_Catch 10 */
const Sint32 pl32_catch_10[] = {
	CG_FRAME(    0,    0,   3,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   7,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Catch 19 */
const Sint32 pl32_catch_19[] = {
	CG_FRAME(    0,    0,   6,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_10 ),
};

/* Char_Catch 11 */
const Sint32 pl32_catch_11[] = {
	CG_FRAME(    0,    0,   3,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   373, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   5,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Catch 22 */
const Sint32 pl32_catch_22[] = {
	CG_FRAME(    0,    0,   8,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   380, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_11 ),
};

/* Char_Catch 20 */
const Sint32 pl32_catch_20[] = {
	CG_FRAME(    0,    0,  16,           0,   405, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_catch_20_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   406, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   409, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_20_loop ),
};

/* Char_Catch 21 */
const Sint32 pl32_catch_21[] = {
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   413, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   414, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  18,           0,   415, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  14,           0,   416, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   418, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_21 ),
};

/* Char_Catch 16
   Char_Catch 17
   Char_Super 0: dive_init (Super Dive) */
const Sint32 pl32_catch_16[] = {
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   253, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl32_catch_16_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   458, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl32_catch_16_loop ),
};

/* Char_Super 1: dive_rise (Super Dive) */
const Sint32 pl32_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl32_super_01 ),
};

/* Char_Super 2: dive_dive (Super Dive) */
const Sint32 pl32_super_02[] = {
	CG_FRAME(    0,    0,  40,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   100, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_super_02 ),
};

/* Char_Special 0
   Char_Special 1 */
const Sint32 pl32_special_00[] = {
	CG_FRAME(    0,    0,   7,           0,   421, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0,   422, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,  62 ),
	CG_FRAME(  -53,    1,   2,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(  -58,    0,  24, END_OF_DATA, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl32_special_00 ),
};

const Sint32 pl32_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32768, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  63 ),
	0x00004140,
};

/* Char_Special 2 */
const Sint32 pl32_special_02[] = {
	CG_FRAME(    0,    0,  14,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   6,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    2,    0,   5,           0,   101, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    3,    0,   4, END_OF_DATA,   100, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl32_special_02 ),
};

/* Char_Special 3
   Char_Special 9
   Char_Special 10 */
const Sint32 pl32_special_03[] = {
	CG_FRAME(    0,    0,   3,           0,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   407, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   406, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   409, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl32_special_03 ),
};

const Sint32 pl32_unused_03[] = {	/* nothing reaches it */
	0x80280000,
};

const Sint32 pl32_unused_04[] = {
	0x000001aa,
};

const Sint32 pl32_unused_05[] = {
	0x00000080,
};

const Sint32 pl32_unused_06[] = {
	0x00000000,
};

const Sint32 pl32_unused_07[] = {
	0x00000000,
};

const Sint32 pl32_unused_08[] = {
	0x00004228,
};

const Sint32 pl32_unused_09[] = {
	0x80280000,
};

const Sint32 pl32_unused_10[] = {
	0x000001ab,
};

const Sint32 pl32_unused_11[] = {
	0x00000080,
};

const Sint32 pl32_unused_12[] = {
	0x00000000,
};

const Sint32 pl32_unused_13[] = {
	0x00000000,
};

const Sint32 pl32_unused_14[] = {
	0x00004240,
};

const Sint32 pl32_unused_15[] = {
	0x80280000,
};

const Sint32 pl32_unused_16[] = {
	0x000001ac,
};

const Sint32 pl32_unused_17[] = {
	0x00000080,
};

const Sint32 pl32_unused_18[] = {
	0x00000000,
};

const Sint32 pl32_unused_19[] = {
	0x00000000,
};

const Sint32 pl32_unused_20[] = {
	0x00004258,
};

/* Char_Catch 18 */
const Sint32 pl32_catch_18[] = {
	0x00040000,
};

/* Char_Catch 12 */
const Sint32 pl32_catch_12[] = {
	CG_FRAME(    0,    0,   8,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   395, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_12 ),
};

/* Char_Catch 13 */
const Sint32 pl32_catch_13[] = {
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   399, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_15_loop ),
};

/* Char_Catch 14 */
const Sint32 pl32_catch_14[] = {
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_15_loop ),
};

/* Char_Catch 15 */
const Sint32 pl32_catch_15[] = {
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   401, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_15_loop ),
};

const Sint32 pl32_catch_15_loop[] = {
	CG_FRAME(    0,    0,   8,           0,   402, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   403, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   404, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_catch_15_loop_loop ),
};

/* Char_Special 4: swing_init (Giant Swing, 4 + atck_level)
   Char_Special 5 */
const Sint32 pl32_special_04[] = {
	CG_FRAME(    0,    1,  10,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   6,           0,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   6,           0,   430, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  65 ),
};

const Sint32 pl32_special_04_loop[] = {
	CG_FRAME(  -58,    0,  -1, END_OF_DATA,   453, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl32_special_04_loop ),
};

const Sint32 pl32_unused_21[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4, END_OF_DATA,   453, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl32_special_04_loop ),
};

/* Char_Special 6
   Char_Special 7 */
const Sint32 pl32_special_06[] = {
	CG_FRAME(    0,    0,   9,           0,   430, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   431, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl32_special_06 ),
};

/* Char_Catch 0: rush_set, rush_hold */
const Sint32 pl32_catch_00[] = {
	CG_FRAME(    0,    0,  16,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   436, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0, 0x0002,   0 ),
	CG_NEXT( pl32_catch_00 ),
};

/* Char_Catch 1: rush_air_set, rush_hold, rush_rise, rush_air_hold */
const Sint32 pl32_catch_01[] = {
	CG_FRAME(    0,    0,   6,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
};

const Sint32 pl32_catch_01_loop[] = {
	CG_FRAME(    0,    0,  16,           0,   443, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   445, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_NEXT( pl32_catch_01_loop ),
};

/* Char_Catch 2: rush_rise, rush_air_hold */
const Sint32 pl32_catch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   447, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   4,           0,   449, 0x80,   0x00,    0,  0x00, 0x16 | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   450, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_catch_02_loop[] = {
	CG_FRAME(    0,    2,   2,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl32_catch_02_loop ),
};

/* Char_Special 11 */
const Sint32 pl32_special_11[] = {
	CG_FRAME(    0,    0,   1,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_special_11_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_special_11_loop ),
};

const Sint32 pl32_unused_22[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   8,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000472c,
};

/* Char_Extra 8 */
const Sint32 pl32_extra_08[] = {
	CG_FRAME(    0,    0,   5,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_extra_08 ),
};

const Sint32 pl32_unused_23[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   8, END_OF_DATA,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00004888,
};

/* group 27 0: pls32_00_punch_init */
const Sint32 pl32_grp27_00[] = {
	EF_FRAME(    0,    0,   3,           0,   577 ),
	EF_FRAME(    0,    0,   3,           0,   572 ),
	EF_FRAME(    0,    0,   3,           0,   578 ),
	EF_FRAME(    0,    0,   3,           0,   573 ),
	EF_FRAME(    0,    0,   3,           0,   579 ),
	EF_FRAME(    0,    0,   3,           0,   574 ),
	EF_FRAME(    0,    0,   3,           0,   580 ),
	EF_FRAME(    0,    0,   3,           0,   575 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   576 ),
	CG_NEXT( pl32_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl32_grp27_01[] = {
	EF_FRAME(    0,    0,   6,           0,   548 ),
	EF_FRAME(    0,    0,   6,           0,   549 ),
	EF_FRAME(    0,    0,   6,           0,   550 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   551 ),
	CG_NEXT( pl32_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl32_grp27_02[] = {
	EF_FRAME(    0,    0,   6,           0,   552 ),
	EF_FRAME(    0,    0,   6,           0,   553 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   554 ),
	CG_NEXT( pl32_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl32_grp27_03[] = {
	EF_FRAME(    0,    0,   6,           0,   555 ),
	EF_FRAME(    0,    0,   6,           0,   556 ),
	EF_FRAME(    0,    0,   6,           0,   557 ),
	EF_FRAME(    0,    0,   6,           0,   558 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   559 ),
	CG_NEXT( pl32_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl32_grp27_04[] = {
	EF_FRAME(    0,    0,   4,           0,   560 ),
	EF_FRAME(    0,    0,   4,           0,   561 ),
	EF_FRAME(    0,    0,   4,           0,   562 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   563 ),
	CG_NEXT( pl32_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl32_grp27_05[] = {
	EF_FRAME(    0,    0,   6,           0,   564 ),
	EF_FRAME(    0,    0,   6,           0,   565 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   566 ),
	CG_NEXT( pl32_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl32_grp27_06[] = {
	EF_FRAME(    0,    0,   6,           0,   567 ),
	EF_FRAME(    0,    0,   6,           0,   568 ),
	EF_FRAME(    0,    0,   6,           0,   569 ),
	EF_FRAME(    0,    0,   6,           0,   570 ),
	EF_FRAME(    0,    0,   6, END_OF_DATA,   571 ),
	CG_NEXT( pl32_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl32_grp27_07[] = {
	EF_FRAME(    0,    0,   2,           0, 32769 ),
	EF_FRAME(    0,    0,   2,           0, 32770 ),
	EF_FRAME(    0,    0,   2,           0, 32771 ),
	EF_FRAME(    0,    0,   2,           0, 32772 ),
	EF_FRAME(    0,    0,   2,           0, 32773 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32774 ),
	CG_NEXT( pl32_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl32_grp27_08[] = {
	EF_FRAME(    0,    0,   2,           0, 32775 ),
	EF_FRAME(    0,    0,   2,           0, 32776 ),
	EF_FRAME(    0,    0,   2,           0, 32777 ),
	EF_FRAME(    0,    0,   2,           0, 32778 ),
	EF_FRAME(    0,    0,   2,           0, 32779 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA, 32780 ),
	CG_NEXT( pl32_grp27_08 ),
};

/* Char_Caught 0 */
const Sint32 pl32_caught_00[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl32_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl32_caught_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl32_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl32_caught_02[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl32_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl32_caught_03[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl32_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl32_caught_04[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl32_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl32_caught_05[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   477, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl32_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl32_caught_06[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   483, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl32_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl32_caught_07[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl32_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl32_caught_08[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl32_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl32_caught_09[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   491, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl32_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl32_caught_10[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl32_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl32_caught_11[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl32_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl32_caught_12[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl32_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl32_caught_13[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl32_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl32_caught_14[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   508, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl32_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl32_caught_15[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   509, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl32_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl32_caught_16[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl32_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl32_caught_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   511, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl32_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl32_caught_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   526, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl32_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl32_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   532, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl32_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl32_caught_20[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_NEXT( pl32_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl32_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl32_caught_21 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl32_extra_00[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl32_extra_00 ),
};

/* Char_Extra 1: t_kick_jump */
const Sint32 pl32_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,  ATE,  0x00,       0x06,    0,   0,     0,      0,   4 ),
};

const Sint32 pl32_extra_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   136, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_extra_01_loop ),
};

/* Char_Effect 1
   Char_Change 1: act_change_vcounter_init
   group 25 1 */
const Sint32 pl32_effect_01[] = {
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_effect_01 ),
};

/* Char_Effect 3
   Char_Change 3: act_stand, act_change_call
   group 25 3 */
const Sint32 pl32_effect_03[] = {
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,    0,  0x00,       0x07,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl32_effect_03 ),
};

/* Char_Effect 0
   Char_Change 0: act_change_tag_init
   group 25 0 */
const Sint32 pl32_effect_00[] = {
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,    0,  0x00,       0x07,    0,   0,     0,      0,   0 ),
};

const Sint32 pl32_effect_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_effect_00_loop ),
};

/* Char_Effect 2
   Char_Change 2: act_change_out_bound
   group 25 2 */
const Sint32 pl32_effect_02[] = {
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl32_effect_02 ),
};

/* Char_Special 8: snapback_init (the snapback) */
const Sint32 pl32_special_08[] = {
	CG_FRAME(    1,    0,   2,           0,   165, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    2,    0,   2,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    1,    0,   1,           0,   165, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    2,    0,   1,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    1,    0,   2,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    2,    1,   1,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    1,    0,   2,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   1,           0,   168, 0x80,   0x00,  ATK,  0x00,       0x06,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   173, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   169, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    4,    0,   2,           0,   170, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    5,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    6,    0,   2,           0,   170, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    7,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    8,    0,   2,           0,   170, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    9,    0,   2,           0,   171, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   8,           0,   172, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   168, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl32_special_08 ),
};
