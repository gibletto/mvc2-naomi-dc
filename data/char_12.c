/* Hayato (character 12): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_12      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_12     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl12.c) says which group
                   and number each move uses.
   pl12_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl12_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl12_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl12_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_12
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_12, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_12 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl12_stand_00[], pl12_stand_02[], pl12_stand_03[], pl12_jump_00[],
	pl12_jump_00_loop[], pl12_jump_02[], pl12_jump_02_loop[], pl12_jump_03[], pl12_jump_06[],
	pl12_jump_06_loop[], pl12_jump_09[], pl12_jump_09_loop[], pl12_dash_00[], pl12_dash_02[],
	pl12_dash_01[], pl12_dash_03[], pl12_crouch_00[], pl12_crouch_01[], pl12_crouch_02[],
	pl12_turn_00[], pl12_turn_01[], pl12_guard_00[], pl12_guard_01[], pl12_guard_02[],
	pl12_guard_04[], pl12_guard_05[], pl12_guard_06[], pl12_air_guard_00[], pl12_air_guard_01[],
	pl12_stand_p_00[], pl12_stand_p_01[], pl12_stand_p_02[], pl12_stand_k_00[], pl12_stand_k_01[],
	pl12_stand_k_02[], pl12_crouch_p_00[], pl12_crouch_p_01[], pl12_crouch_p_02[],
	pl12_crouch_k_00[], pl12_crouch_k_01[], pl12_crouch_k_02[], pl12_jump_p_00[],
	pl12_jump_p_00_loop[], pl12_jump_p_01[], pl12_jump_p_01_loop[], pl12_jump_p_02[],
	pl12_jump_p_02_loop[], pl12_jump_k_00[], pl12_jump_k_00_loop[], pl12_jump_k_01[],
	pl12_jump_k_01_loop[], pl12_jump_k_02[], pl12_jump_k_02_loop[], pl12_jump_k_06[],
	pl12_jump_k_06_loop[], pl12_extra_20[], pl12_extra_20_loop[], pl12_extra_21[], pl12_stand_p_03[],
	pl12_extra_00[], pl12_extra_01[], pl12_extra_01_loop[], pl12_extra_02[], pl12_extra_03[],
	pl12_extra_04[], pl12_extra_05[], pl12_extra_06[], pl12_extra_07[], pl12_extra_08[],
	pl12_extra_09[], pl12_extra_10[], pl12_extra_11[], pl12_extra_12[], pl12_extra_13[],
	pl12_extra_14[], pl12_extra_15[], pl12_extra_16[], pl12_extra_17[], pl12_extra_18[],
	pl12_extra_19[], pl12_special_00[], pl12_special_01[], pl12_special_02[], pl12_special_03[],
	pl12_special_06[], pl12_special_04[], pl12_catch_03[], pl12_special_05[], pl12_special_07[],
	pl12_special_07_loop[], pl12_change_04[], pl12_change_04_loop[], pl12_super_00[],
	pl12_super_01[], pl12_super_02[], pl12_super_02_loop[], pl12_super_03[], pl12_super_04[],
	pl12_super_04_loop[], pl12_super_05[], pl12_super_06[], pl12_super_06_loop[], pl12_super_07[],
	pl12_catch_04[], pl12_super_09[], pl12_super_10[], pl12_catch_00[], pl12_catch_01[],
	pl12_get_up_00[], pl12_get_up_01[], pl12_get_up_02[], pl12_get_up_03[], pl12_damage_02[],
	pl12_damage_01[], pl12_damage_00[], pl12_damage_05[], pl12_damage_04[], pl12_damage_03[],
	pl12_damage_08[], pl12_damage_07[], pl12_damage_06[], pl12_damage_12[], pl12_damage_11[],
	pl12_damage_10[], pl12_damage_13[], pl12_damage_13_loop[], pl12_damage_09[],
	pl12_damage_09_loop[], pl12_damage_16[], pl12_damage_17[], pl12_damage_18[], pl12_damage_19[],
	pl12_damage_20[], pl12_damage_21[], pl12_damage_22[], pl12_damage_23[], pl12_damage_24[],
	pl12_damage_25[], pl12_damage_26[], pl12_damage_31[], pl12_damage_27[], pl12_damage_28[],
	pl12_damage_29[], pl12_damage_34[], pl12_caught_00[], pl12_caught_01[], pl12_caught_02[],
	pl12_caught_03[], pl12_caught_04[], pl12_caught_05[], pl12_caught_06[], pl12_caught_07[],
	pl12_caught_08[], pl12_caught_09[], pl12_caught_10[], pl12_caught_11[], pl12_caught_12[],
	pl12_caught_13[], pl12_caught_14[], pl12_caught_15[], pl12_caught_16[], pl12_caught_17[],
	pl12_caught_18[], pl12_caught_19[], pl12_caught_20[], pl12_caught_21[], pl12_dizzy_00[],
	pl12_win_00[], pl12_win_00_loop[], pl12_win_01[], pl12_win_02[], pl12_win_02_loop[],
	pl12_win_03[], pl12_win_04[], pl12_win_04_loop[], pl12_win_05[], pl12_win_05_loop[],
	pl12_win_06[], pl12_win_06_loop[], pl12_change_00[], pl12_change_00_loop[], pl12_change_02[],
	pl12_change_02_loop[], pl12_change_01[], pl12_change_03[], pl12_unused_00[], pl12_appear_00[],
	pl12_appear_01[], pl12_appear_01_loop[], pl12_appear_02[], pl12_appear_02_loop[],
	pl12_appear_03[], pl12_appear_03_loop[], pl12_effect_01[], pl12_effect_02[], pl12_effect_07[],
	pl12_effect_07_loop[], pl12_effect_08[], pl12_effect_09[], pl12_effect_00[], pl12_effect_10[],
	pl12_effect_10_loop[], pl12_effect_03[], pl12_effect_04[], pl12_effect_05[], pl12_effect_06[],
	pl12_grp27_16[], pl12_grp27_00[], pl12_grp27_18[], pl12_grp27_19[], pl12_grp27_20[],
	pl12_grp27_21[], pl12_grp27_22[], pl12_grp27_01[], pl12_grp27_02[], pl12_grp27_03[],
	pl12_grp27_04[], pl12_grp27_05[], pl12_grp27_06[], pl12_grp27_07[], pl12_grp27_08[],
	pl12_grp27_09[], pl12_grp27_10[], pl12_grp27_11[], pl12_grp27_11_loop[], pl12_grp27_12[],
	pl12_grp27_12_loop[], pl12_grp27_13[], pl12_grp27_13_loop[], pl12_grp27_14[];
extern const Sint32 *const pl12_stand_tbl[], *const pl12_jump_tbl[], *const pl12_dash_tbl[],
	*const pl12_crouch_tbl[], *const pl12_turn_tbl[], *const pl12_guard_tbl[],
	*const pl12_air_guard_tbl[], *const pl12_stand_p_tbl[], *const pl12_stand_k_tbl[],
	*const pl12_crouch_p_tbl[], *const pl12_crouch_k_tbl[], *const pl12_jump_p_tbl[],
	*const pl12_jump_k_tbl[], *const pl12_damage_tbl[], *const pl12_dizzy_tbl[],
	*const pl12_catch_tbl[], *const pl12_caught_tbl[], *const pl12_get_up_tbl[],
	*const pl12_appear_tbl[], *const pl12_win_tbl[], *const pl12_extra_tbl[],
	*const pl12_special_tbl[], *const pl12_super_tbl[], *const pl12_effect_tbl[],
	*const pl12_change_tbl[], *const pl12_grp25_tbl[], *const pl12_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_12[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -16384, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 40 },
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
const Sint32 *const *const char_tbl_12[] = {
	pl12_stand_tbl, pl12_jump_tbl, pl12_dash_tbl, pl12_crouch_tbl, pl12_turn_tbl, pl12_guard_tbl,
	pl12_air_guard_tbl, pl12_stand_p_tbl, pl12_stand_k_tbl, pl12_crouch_p_tbl, pl12_crouch_k_tbl, pl12_jump_p_tbl,
	pl12_jump_k_tbl, pl12_damage_tbl, pl12_dizzy_tbl, pl12_catch_tbl, pl12_caught_tbl, pl12_get_up_tbl,
	pl12_appear_tbl, pl12_win_tbl, pl12_extra_tbl, pl12_special_tbl, pl12_super_tbl, pl12_effect_tbl,
	pl12_change_tbl, pl12_grp25_tbl, pl12_grp25_tbl, pl12_grp27_tbl, pl12_grp27_tbl, pl12_grp27_tbl,
	pl12_grp27_tbl, pl12_grp27_tbl, pl12_grp27_tbl, 0,
};

const Sint32 *const pl12_stand_tbl[] = {
	pl12_stand_00, pl12_stand_00, pl12_stand_02, pl12_stand_03, pl12_stand_00, pl12_stand_00,
	pl12_stand_02, pl12_stand_03, 0,
};

const Sint32 *const pl12_jump_tbl[] = {
	pl12_jump_00, pl12_jump_00, pl12_jump_02, pl12_jump_03, pl12_jump_03, pl12_jump_03,
	pl12_jump_06, pl12_jump_06, pl12_jump_06, pl12_jump_09, pl12_jump_09, pl12_jump_09,
	0,
};

const Sint32 *const pl12_dash_tbl[] = {
	pl12_dash_00, pl12_dash_01, pl12_dash_02, pl12_dash_03, 0,
};

const Sint32 *const pl12_crouch_tbl[] = {
	pl12_crouch_00, pl12_crouch_01, pl12_crouch_02, 0,
};

const Sint32 *const pl12_turn_tbl[] = {
	pl12_turn_00, pl12_turn_01, 0,
};

const Sint32 *const pl12_guard_tbl[] = {
	pl12_guard_00, pl12_guard_01, pl12_guard_02, pl12_guard_01, pl12_guard_04, pl12_guard_05,
	pl12_guard_06, pl12_guard_05, 0,
};

const Sint32 *const pl12_air_guard_tbl[] = {
	pl12_air_guard_00, pl12_air_guard_01, pl12_air_guard_01, 0,
};

const Sint32 *const pl12_stand_p_tbl[] = {
	pl12_stand_p_00, pl12_stand_p_01, pl12_stand_p_02, pl12_stand_p_03, 0,
};

const Sint32 *const pl12_stand_k_tbl[] = {
	pl12_stand_k_00, pl12_stand_k_01, pl12_stand_k_02, 0,
};

const Sint32 *const pl12_crouch_p_tbl[] = {
	pl12_crouch_p_00, pl12_crouch_p_01, pl12_crouch_p_02, 0,
};

const Sint32 *const pl12_crouch_k_tbl[] = {
	pl12_crouch_k_00, pl12_crouch_k_01, pl12_crouch_k_02, 0,
};

const Sint32 *const pl12_jump_p_tbl[] = {
	pl12_jump_p_00, pl12_jump_p_01, pl12_jump_p_02, pl12_jump_p_00, pl12_jump_p_01, pl12_jump_p_02,
	0,
};

const Sint32 *const pl12_jump_k_tbl[] = {
	pl12_jump_k_00, pl12_jump_k_01, pl12_jump_k_02, pl12_jump_k_00, pl12_jump_k_01, pl12_jump_k_02,
	pl12_jump_k_06, 0,
};

const Sint32 *const pl12_damage_tbl[] = {
	pl12_damage_00, pl12_damage_01, pl12_damage_02, pl12_damage_03, pl12_damage_04, pl12_damage_05,
	pl12_damage_06, pl12_damage_07, pl12_damage_08, pl12_damage_09, pl12_damage_10, pl12_damage_11,
	pl12_damage_12, pl12_damage_13, pl12_damage_13, pl12_damage_13, pl12_damage_16, pl12_damage_17,
	pl12_damage_18, pl12_damage_19, pl12_damage_20, pl12_damage_21, pl12_damage_22, pl12_damage_23,
	pl12_damage_24, pl12_damage_25, pl12_damage_26, pl12_damage_27, pl12_damage_28, pl12_damage_29,
	pl12_damage_27, pl12_damage_31, pl12_damage_27, pl12_damage_27, pl12_damage_34, 0,
};

const Sint32 *const pl12_dizzy_tbl[] = {
	pl12_dizzy_00, 0,
};

const Sint32 *const pl12_catch_tbl[] = {
	pl12_catch_00, pl12_catch_01, pl12_catch_01, pl12_catch_03, pl12_catch_04, 0,
};

const Sint32 *const pl12_caught_tbl[] = {
	pl12_caught_00, pl12_caught_01, pl12_caught_02, pl12_caught_03, pl12_caught_04, pl12_caught_05,
	pl12_caught_06, pl12_caught_07, pl12_caught_08, pl12_caught_09, pl12_caught_10, pl12_caught_11,
	pl12_caught_12, pl12_caught_13, pl12_caught_14, pl12_caught_15, pl12_caught_16, pl12_caught_17,
	pl12_caught_18, pl12_caught_19, pl12_caught_20, pl12_caught_21, 0,
};

const Sint32 *const pl12_get_up_tbl[] = {
	pl12_get_up_00, pl12_get_up_01, pl12_get_up_02, pl12_get_up_03, 0,
};

const Sint32 *const pl12_appear_tbl[] = {
	pl12_appear_00, pl12_appear_01, pl12_appear_02, pl12_appear_03, 0,
};

const Sint32 *const pl12_win_tbl[] = {
	pl12_win_00, pl12_win_01, pl12_win_02, pl12_win_03, pl12_win_04, pl12_win_05,
	pl12_win_06, 0,
};

const Sint32 *const pl12_extra_tbl[] = {
	pl12_extra_00, pl12_extra_01, pl12_extra_02, pl12_extra_03, pl12_extra_04, pl12_extra_05,
	pl12_extra_06, pl12_extra_07, pl12_extra_08, pl12_extra_09, pl12_extra_10, pl12_extra_11,
	pl12_extra_12, pl12_extra_13, pl12_extra_14, pl12_extra_15, pl12_extra_16, pl12_extra_17,
	pl12_extra_18, pl12_extra_19, pl12_extra_20, pl12_extra_21, 0,
};

const Sint32 *const pl12_special_tbl[] = {
	pl12_special_00, pl12_special_01, pl12_special_02, pl12_special_03, pl12_special_04, pl12_special_05,
	pl12_special_06, pl12_special_07, 0,
};

const Sint32 *const pl12_super_tbl[] = {
	pl12_super_00, pl12_super_01, pl12_super_02, pl12_super_03, pl12_super_04, pl12_super_05,
	pl12_super_06, pl12_super_07, pl12_catch_04, pl12_super_09, pl12_super_10, 0,
};

const Sint32 *const pl12_effect_tbl[] = {
	pl12_effect_00, pl12_effect_01, pl12_effect_02, pl12_effect_03, pl12_effect_04, pl12_effect_05,
	pl12_effect_06, pl12_effect_07, pl12_effect_08, pl12_effect_09, pl12_effect_10, 0,
};

const Sint32 *const pl12_change_tbl[] = {
	pl12_change_00, pl12_change_01, pl12_change_02, pl12_change_03, pl12_change_04, 0,
};

const Sint32 *const pl12_grp25_tbl[] = {
	pl12_stand_00, 0,
};

const Sint32 *const pl12_grp27_tbl[] = {
	pl12_grp27_00, pl12_grp27_01, pl12_grp27_02, pl12_grp27_03, pl12_grp27_04, pl12_grp27_05,
	pl12_grp27_06, pl12_grp27_07, pl12_grp27_08, pl12_grp27_09, pl12_grp27_10, pl12_grp27_11,
	pl12_grp27_12, pl12_grp27_13, pl12_grp27_14, pl12_grp27_14, pl12_grp27_16, pl12_grp27_00,
	pl12_grp27_18, pl12_grp27_19, pl12_grp27_20, pl12_grp27_21, pl12_grp27_22, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl12_stand_00[] = {
	CG_FRAME(    0,    1,   7,           0,    24, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    1,   7,           0,    25, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    26, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    27, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    28, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    29, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    30, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    31, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    32, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    33, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    34, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,    35, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    35, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl12_stand_02[] = {
	CG_FRAME(    0,    1,   6,           0,    37, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    38, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    40, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    41, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    46, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    46, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl12_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,    46, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    41, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    40, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    38, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   5,           0,    37, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    37, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_stand_03 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl12_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    24, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   8,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    61, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    62, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    63, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl12_jump_00_loop[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_jump_00_loop ),
};

/* Char_Jump 2 */
const Sint32 pl12_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    24, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   8,           0,    68, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    69, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl12_jump_02_loop[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl12_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl12_jump_06[] = {
	CG_FRAME(    0,    1,   1,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    1,   3,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    62, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl12_jump_06_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_jump_06_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl12_jump_09[] = {
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
};

const Sint32 pl12_jump_09_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_jump_09_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl12_dash_00[] = {
	CG_FRAME(    0,    0,   2,           0,    47, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    1,   1,           0,    48, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  11,           0,    48, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,    50, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Dash 2 */
const Sint32 pl12_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,    51, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    52, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl12_dash_01[] = {
	CG_FRAME(    0,    0,   4,           0,    53, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    1,   1,           0,    53, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    54, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    55, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    36, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x00,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Dash 3 */
const Sint32 pl12_dash_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl12_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl12_crouch_00[] = {
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl12_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl12_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    36, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_NEXT( pl12_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl12_crouch_02[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl12_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    80, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl12_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,    81, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl12_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl12_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl12_guard_01[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
};

/* Char_Guard 2 */
const Sint32 pl12_guard_02[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    85, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl12_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl12_guard_04[] = {
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl12_guard_05[] = {
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
};

/* Char_Guard 6 */
const Sint32 pl12_guard_06[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    87, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_NEXT( pl12_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl12_air_guard_00[] = {
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl12_air_guard_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_NEXT( pl12_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl12_stand_p_00[] = {
	CG_FRAME(    0,   80,   1,           0,   152, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,   80,   4,           0,   152, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,   80,   2,           0,   153, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   154, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  16 ),
	CG_FRAME( 0x80,    0,   3,           0,   155, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  74 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  74 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   158, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl12_stand_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   159, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,              ATK,  0x00, 0x24,    0,   0,     0, 0x0600, 134 ),
	CG_FRAME( 0x80,    0,   4,           0,   161, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  17 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  75 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  75 ),
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   165, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   167, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   172, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   173, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl12_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   8,           0,   175, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   176, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 135 ),
	CG_FRAME( 0x80,    0,   3,           0,   178, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  18 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  76 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  76 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   186, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   190, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl12_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   191, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   1,           0,   192, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   193, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   194, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   197, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl12_stand_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   198, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   5,           0,   199, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   200, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   8,           0,   201, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   6,           0,   202, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   204, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl12_stand_k_02[] = {
	CG_FRAME(    0,    0,   8,           0,   205, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   1,           0,   206, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0900, 136 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  21 ),
	CG_FRAME(    0,    1,   2,           0,   209, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  79 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  79 ),
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 136 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    2,    0,   1,           0,   214, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   215, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   216, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   217, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   218, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   1,           0,   221, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  22 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0900,  80 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,              ATK,  0x00, 0x03,    0,   0,     0, 0x0900,  80 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   229, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   233, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl12_crouch_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  23 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  81 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  81 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   240, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl12_crouch_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   241, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   5,           0,   241, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x00,              ATK,  0x00, 0x24,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   1,           0,   243, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME( 0x80,    0,   3,           0,   244, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  24 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  82 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  82 ),
	CG_FRAME(    0,    0,   8,           0,   247, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  82 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   251, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   257, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl12_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   260, 0x80,   0x00,              ATK,  0x00, 0x03,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  25 ),
	CG_FRAME(    0,    0,   3,           0,   262, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  83 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  83 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 139 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   2,           0,   266, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   1,           0,   271, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   1,           0,   272, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   274, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl12_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   275, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   275, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  84 ),
	CG_NEXT( pl12_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl12_crouch_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   277, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   8,           0,   280, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   282, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl12_crouch_k_02[] = {
	CG_FRAME(    0,    0,   8,           0,   283, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   1,           0,   284, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0900,  13 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x00,              ATK,  0x00, 0x04,    0,   0,     0, 0x0900, 138 ),
	CG_FRAME( 0x80,    0,   4,           0,   286, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  28 ),
	CG_FRAME( 0x80,    0,   4,           0,   287, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900,  86 ),
	CG_FRAME( 0x80,    0,   4,           0,   288, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900,  86 ),
	CG_FRAME( 0x80,    0,   4,           0,   289, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0300, 138 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100, 138 ),
	CG_FRAME(    0,    0,   2,           0,   291, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0300, 138 ),
	CG_FRAME(    0,    0,   2,           0,   292, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200, 138 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3,           0,   295, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3,           0,   296, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3,           0,   297, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3,           0,   298, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   299, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl12_jump_p_00[] = {
	CG_FRAME(    0,    0,   5,           0,   300, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   301, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   8,           0,   302, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3,           0,   303, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  87 ),
};

const Sint32 pl12_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   304, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl12_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl12_jump_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   305, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    0,   1,           0,   306, 0x80,   0x00,              ATK,  0x00, 0x24,    0,   0,     0, 0x0600,  14 ),
	CG_FRAME(    0,    0,   2,           0,   307, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  14 ),
	CG_FRAME( 0x80,    0,   4,           0,   308, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  30 ),
	CG_FRAME( 0x80,    0,   4,           0,   309, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  88 ),
	CG_FRAME(    0,    0,   3,           0,   310, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  88 ),
	CG_FRAME(    0,    0,   3,           0,   311, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  14 ),
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  14 ),
	CG_FRAME(    0,    0,   1,           0,   313, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  14 ),
};

const Sint32 pl12_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   314, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_NEXT( pl12_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl12_jump_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    0,   1,           0,   316, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   317, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   318, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  14 ),
	CG_FRAME(    0,    0,   1,           0,   319, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  14 ),
	CG_FRAME(    0,    0,   2,           0,   320, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  14 ),
	CG_FRAME( 0x80,    0,   6,           0,   321, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  31 ),
	CG_FRAME( 0x80,    0,   4,           0,   322, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  89 ),
	CG_FRAME( 0x80,    0,   4,           0,   323, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  89 ),
	CG_FRAME(    0,    0,   1,           0,   324, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  14 ),
	CG_FRAME(    0,    0,   1,           0,   325, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  14 ),
	CG_FRAME(    0,    0,   1,           0,   326, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  14 ),
};

const Sint32 pl12_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   327, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_NEXT( pl12_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl12_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    0,  10,           0,   329, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
};

const Sint32 pl12_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   329, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl12_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl12_jump_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   330, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    0,   2,           0,   331, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   8,           0,   332, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   8,           0,   333, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
};

const Sint32 pl12_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   334, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl12_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl12_jump_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   335, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    0,   1,           0,   336, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0b00,  14 ),
	CG_FRAME(    0,    0,   1,           0,   337, 0x80,   0x00,              ATK,  0x00, 0x04,    0,   0,     0, 0x0b00,  14 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0b00,  34 ),
	CG_FRAME(    0,    0,   2,           0,   339, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0b00,  92 ),
	CG_FRAME(    0,    0,   3,           0,   340, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0b00,  92 ),
	CG_FRAME(    0,    0,   3,           0,   341, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0b00,  14 ),
	CG_FRAME(    0,    0,   3,           0,   342, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0b00,  14 ),
};

const Sint32 pl12_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   343, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0b00,  14 ),
	CG_NEXT( pl12_jump_k_02_loop ),
};

/* Char_Jump_K 6 */
const Sint32 pl12_jump_k_06[] = {
	CG_FRAME(    0,    1,   8,           0,   363, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    1,   6,           0,   364, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
};

const Sint32 pl12_jump_k_06_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   368, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl12_jump_k_06_loop ),
};

/* Char_Extra 20: dive_move */
const Sint32 pl12_extra_20[] = {
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl12_extra_20_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_extra_20_loop ),
};

/* Char_Extra 21: dive_move, dive_bound */
const Sint32 pl12_extra_21[] = {
	CG_FRAME(    0,    0,   6,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_extra_21 ),
};

/* Char_Stand_P 3 */
const Sint32 pl12_stand_p_03[] = {
	CG_FRAME(    0,    0,   3,           0,   413, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x25,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,    0,   1,           0,   413, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x04,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,    0,   3,           0,   414, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x25,    0,   0,     0, 0x0200, 133 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200, 133 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200, 133 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200,  38 ),
	CG_FRAME(    0,    0,   2,           0,   418, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200,  96 ),
	CG_FRAME(    0,    0,   2,           0,   419, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200,  96 ),
	CG_FRAME(    0,    1,   2,           0,   420, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200, 133 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   422, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_stand_p_03 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl12_extra_00[] = {
	CG_FRAME(    0,    0,   3,           0,   333, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   333, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl12_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl12_extra_01[] = {
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   1,           0,   134, 0x00,   0x00,                0,  0x00, 0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   134, 0x80,   0x00,                0,  0x00, 0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x00,                0,  0x00, 0x20,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl12_extra_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl12_extra_01_loop ),
};

/* Char_Extra 2: snapback_init (the snapback), fly_start5 */
const Sint32 pl12_extra_02[] = {
	CG_FRAME(    0,    0,   1,           0,   218, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   9,           0,   218, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x0f,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    1,   2,           0,   218, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x25,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  37 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  95 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900,  95 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00, 137 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   233, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_02 ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl12_extra_03[] = {
	CG_FRAME(    0,    0,   6,           0,   159, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x25,    0,   0,     0, 0x1000, 134 ),
	CG_FRAME(    0,    0,   1,           0,   160, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x04,    0,   0,     0, 0x1000, 134 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x1000,  36 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x1000,  94 ),
	CG_FRAME(    0,    0,  10,           0,   163, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x1000,  94 ),
	CG_FRAME(    0,    1,   3,           0,   164, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   168, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   169, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   170, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    2,   1,           0,   171, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   172, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,  16,           0,   173, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   173, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_03 ),
};

/* Char_Extra 4: rush_move */
const Sint32 pl12_extra_04[] = {
	CG_FRAME(    0,    1,   2,           0,   212, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    1,   2,           0,   213, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x25,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    1,   2,           0,   214, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x05,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    1,   2,           0,   215, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    1,   2,           0,   216, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    1,   2,           0,   217, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    1,   2,           0,   218, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    2,   2,           0,   219, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  37 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0900,  95 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900,  95 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00, 137 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   233, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl12_extra_05[] = {
	CG_FRAME(    0,    0,   8,           0,   423, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   2,           0,   424, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   425, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   426, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  39 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  97 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  97 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   431, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   432, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   434, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   435, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   436, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   437, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_05 ),
};

/* Char_Extra 6 */
const Sint32 pl12_extra_06[] = {
	CG_FRAME(    0,    6,   2,           0,   438, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    6,   2,           0,   454, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    6,   2,           0,   455, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0c00,  40 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0c00,  98 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0c00,  98 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0c00,  98 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0c00,  98 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   452, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   453, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_06 ),
};

/* Char_Extra 7 */
const Sint32 pl12_extra_07[] = {
	CG_FRAME(    0,    6,   2,           0,   258, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    6,   2,           0,   259, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    6,   2,           0,   260, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  41 ),
	CG_FRAME(    0,    0,   2,           0,   262, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  99 ),
	CG_FRAME(    0,    0,   2,           0,   263, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  99 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 139 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   2,           0,   266, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   269, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   270, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   271, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   274, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_extra_07 ),
};

/* Char_Extra 8 */
const Sint32 pl12_extra_08[] = {
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    6,   8,           0,   175, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    6,   2,           0,   176, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 135 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  42 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 100 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 100 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   186, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   1,           0,   189, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   190, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_08 ),
};

/* Char_Extra 9 */
const Sint32 pl12_extra_09[] = {
	CG_FRAME(    0,    6,   6,           0,   241, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    6,   2,           0,   242, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    6,   2,           0,   243, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   244, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  43 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 101 ),
	CG_FRAME(    0,    0,   6,           0,   246, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 101 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 101 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   251, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   257, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_extra_09 ),
};

/* Char_Extra 10 */
const Sint32 pl12_extra_10[] = {
	CG_FRAME(    0,    6,   4,           0,   277, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    6,   3,           0,   278, 0x80,   0x00,              ATK,  0x00, 0x03,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   279, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   3,           0,   280, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   281, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   282, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl12_extra_10 ),
};

/* Char_Extra 11 */
const Sint32 pl12_extra_11[] = {
	CG_FRAME(    0,    0,   2,           0,   438, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,              ATK,  0x00, 0x04,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  45 ),
	CG_FRAME(    0,    1,   2,           0,   442, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 103 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 103 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 103 ),
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 103 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   449, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   450, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   451, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   452, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   453, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_11 ),
};

/* Char_Extra 12 */
const Sint32 pl12_extra_12[] = {
	CG_FRAME(    0,    0,  16,           0,    59, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x17,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   467, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x25,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   317, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   469, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   4,           0,   470, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   471, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   182, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   190, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_12 ),
};

/* Char_Extra 13 */
const Sint32 pl12_extra_13[] = {
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   154, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  47 ),
	CG_FRAME( 0x80,    0,   3,           0,   155, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200, 105 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0200,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   158, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_13 ),
};

/* Char_Extra 14 */
const Sint32 pl12_extra_14[] = {
	CG_FRAME(    0,    6,   3,           0,   159, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    6,   3,           0,   160, 0x80,   0x00,              ATK,  0x00, 0x23,    0,   0,     0, 0x0600, 134 ),
	CG_FRAME( 0x80,    0,   3,           0,   161, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  48 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 106 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 106 ),
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   165, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   167, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   172, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   173, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_14 ),
};

/* Char_Extra 15 */
const Sint32 pl12_extra_15[] = {
	CG_FRAME(    0,    6,   3,           0,   205, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    6,   3,           0,   206, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    6,   3,           0,   207, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 136 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  49 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 107 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 107 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 136 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   212, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100, 136 ),
	CG_NEXT( pl12_extra_15 ),
};

/* Char_Extra 16 */
const Sint32 pl12_extra_16[] = {
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   1,           0,   214, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    8,   1,           0,   215, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    8,   1,           0,   216, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    8,   1,           0,   217, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    8,   1,           0,   218, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    8,   2,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    8,   2,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  50 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 108 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 108 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0a00,  12 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   233, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_16 ),
};

/* Char_Extra 17 */
const Sint32 pl12_extra_17[] = {
	CG_FRAME(    0,    8,   4,           0,   277, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    8,   4,           0,   278, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   282, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_extra_17 ),
};

/* Char_Extra 18: chain12_init */
const Sint32 pl12_extra_18[] = {
	CG_FRAME(    0,    9,   2,           0,    48, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    8,   1,           0,    48, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x1a,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    8,   1,           0,    49, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00, 0x21,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   1,           0,   475, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,   1,           0,   476, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   6,           0,   477, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   4,           0,   480, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    1,   1,           0,   480, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   482, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_extra_18 ),
};

/* Char_Extra 19: pl12_upper */
const Sint32 pl12_extra_19[] = {
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   259, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   260, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  55 ),
	CG_FRAME(    0,    0,   3,           0,   262, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 113 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 113 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0600, 139 ),
	CG_FRAME(    0,    0,   3,           0,   265, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   3,           0,   266, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   3,           0,   267, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600, 139 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   269, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  12 ),
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   272, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3,           0,   273, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x3600,  13 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   274, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  13 ),
	CG_NEXT( pl12_extra_19 ),
};

/* Char_Special 0 */
const Sint32 pl12_special_00[] = {
	CG_FRAME(    0,    2,   1,           0,   159, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    2,   1,           0,   159, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   1,           0,   159, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,              ATK,  0x00, 0x03,    0,   0,     0, 0x0e00, 134 ),
	CG_FRAME(    0,    2,   2,           0,   160, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 134 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  56 ),
	CG_FRAME(    0,    0,   1,           0,   162, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 114 ),
	CG_FRAME(    0,    0,   1,           0,   163, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 114 ),
	CG_FRAME(    0,    0,   1,           0,   483, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   483, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   484, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   484, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   485, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   486, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   487, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    1,   1,           0,   216, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   216, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   217, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   217, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   218, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   218, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    2,   1,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  57 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 115 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 115 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   233, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl12_special_01[] = {
	CG_FRAME(    0,    2,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   2,           0,   160, 0x80,   0x00,              ATK,  0x00, 0x03,    0,   0,     0, 0x0e00, 134 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 134 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  58 ),
	CG_FRAME(    0,    0,   1,           0,   162, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 116 ),
	CG_FRAME(    0,    0,   1,           0,   163, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 116 ),
	CG_FRAME(    0,    0,   1,           0,   483, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   483, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   484, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   484, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   485, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   486, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   487, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x3c00,  12 ),
	CG_FRAME(    0,    1,   1,           0,   216, 0x80,   0x00,              ATK,  0x00, 0x25,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   216, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   217, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   217, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   218, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   218, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    2,   1,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    2,   1,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0e00,  59 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 117 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 117 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00, 137 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0e00,  12 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   233, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_special_01 ),
};

/* Char_Special 2: guren_init (Guren, 2 + atck_level) */
const Sint32 pl12_special_02[] = {
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x00,              ATK,  0x00, 0x21,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   1,           0,   475, 0x80,   0x00,              ATK,  0x00, 0x19,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   1,           0,   476, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   6,           0,   477, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   4,           0,   480, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    1,   1,           0,   480, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   482, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl12_special_03[] = {
	CG_FRAME(    0,    0,   3,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   3,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    49, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,              ATK,  0x00, 0x21,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   473, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   1,           0,   475, 0x80,   0x00,              ATK,  0x00, 0x19,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   1,           0,   476, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   6,           0,   477, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   6,           0,   478, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   6,           0,   479, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   6,           0,   480, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    1,   1,           0,   480, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   482, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_special_03 ),
};

/* Char_Special 6: counter_init (Guren (variable counter)) */
const Sint32 pl12_special_06[] = {
	CG_FRAME(    0,    0,   3,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   3,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,    49, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   473, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   488, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0x80,   0x00,              ATK,  0x00, 0x21,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   1,           0,   490, 0x80,   0x00,              ATK,  0x00, 0x19,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,   1,           0,   491, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   6,           0,   493, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   6,           0,   494, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,  16,           0,   495, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   2,           0,   496, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   2,           0,   497, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    1,   1,           0,   480, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   482, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_special_06 ),
};

/* Char_Special 4: byakko_init (Byakkou Hou) */
const Sint32 pl12_special_04[] = {
	CG_FRAME(    0,    0,  10,           0,   400, 0x80,   0x00,                0,  0x00, 0x03,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   1,           0,   401, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100, 132 ),
	CG_FRAME(    0,    1,   1,           0,   401, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   401, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_special_04 ),
};

/* Char_Catch 3: jump_k_set */
const Sint32 pl12_catch_03[] = {
	CG_FRAME(    0,    0,  16,           0,   402, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0x00,                0,  0x00, 0x18,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,  12,           0,   404, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0007,   1 ),
	CG_FRAME(    0,    0,   3,           0,   405, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0008,   1 ),
	CG_FRAME(    0,    1,   6,           0,   406, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   407, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   408, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   409, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   410, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   412, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   412, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_catch_03 ),
};

/* Char_Special 5: byakko_move (Byakkou Hou) */
const Sint32 pl12_special_05[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,  16,           0,   411, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   412, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_special_05 ),
};

/* Char_Special 7: chouhatsu_init (the taunt) */
const Sint32 pl12_special_07[] = {
	CG_FRAME(    0,    1,   4,           0,   124, 0x80,   0x00,                0,  0x00, 0x23,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,                0,  0x00, 0x14,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,  32,           0,   126, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl12_special_07_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_special_07_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl12_change_04[] = {
	CG_FRAME(    0,    1,   4,           0,   124, 0x80,   0x00,                0,  0x00, 0x23,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,                0,  0x00, 0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  36,           0,   126, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl12_change_04_loop[] = {
	CG_FRAME(    0,    0,  64, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_change_04_loop ),
};

/* Char_Super 0: engetsu_init (Engetsu (hyper)) */
const Sint32 pl12_super_00[] = {
	CG_FRAME(    0,    0,   3,           0,   498, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME( 0x80,    1,   3,           0,   499, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   500, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   501, 0x80,   0x00,              ATK,  0x00, 0x1b,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   502, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   503, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   504, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   505, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   506, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   507, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   508, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   509, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   510, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   511, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   512, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   513, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   514, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   515, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x00,              ATK,  0x00, 0x20,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   517, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   518, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   519, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   520, 0x1e,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   521, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   1,           0,   522, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME( 0x80,    0,   1,           0,   523, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME( 0x80,    0,   1,           0,   524, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME( 0x80,    0,   2,           0,   525, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME( 0x80,    0,   2,           0,   526, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    2,   2,           0,   527, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,  48,           0,   528, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   2,           0,   530, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   3,           0,   532, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   3,           0,   533, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    3,   3,           0,   534, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   535, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   536, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   537, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   538, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   539, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   540, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   541, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   542, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   543, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   544, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   545, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   546, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   547, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   548, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_super_00 ),
};

/* Char_Super 1: rasetsu_init (Rasetsu Zan (hyper)) */
const Sint32 pl12_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   549, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   1,           0,   549, 0x80,   0x00,              ATK,  0x00, 0x15,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    1,   2,           0,   549, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,  18,           0,   549, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   550, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   551, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   4,           0,   552, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   4,           0,   553, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   8,           0,   554, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   555, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   556, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   555, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   556, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   555, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   556, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   555, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   8,           0,   556, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    2,   2,           0,   438, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0c00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0c00,  63 ),
	CG_FRAME(    0,    0,   2,           0,   442, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0c00, 121 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100, 121 ),
	CG_FRAME(    0,    2,   1,           0,   205, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   1,           0,   205, 0x80,   0x00,              ATK,  0x00, 0x03,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 136 ),
	CG_FRAME(    0,    0,   2,           0,   208, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  64 ),
	CG_FRAME(    0,    0,   2,           0,   209, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 122 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 122 ),
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 136 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100, 136 ),
	CG_FRAME(    0,    2,   2,           0,   159, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0600, 134 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 134 ),
	CG_FRAME( 0x80,    0,   2,           0,   161, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  65 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 123 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100, 123 ),
	CG_FRAME(    0,    2,   1,           0,   174, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x00,              ATK,  0x00, 0x05,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 135 ),
	CG_FRAME(    0,    0,   3,           0,   180, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  66 ),
	CG_FRAME(    0,    2,   1,           0,   283, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0900,  13 ),
	CG_FRAME(    0,    0,   1,           0,   284, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  13 ),
	CG_FRAME(    0,    0,   1,           0,   285, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 138 ),
	CG_FRAME(    0,    0,   2,           0,   286, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  67 ),
	CG_FRAME(    0,    0,   2,           0,   287, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 125 ),
	CG_FRAME(    0,    0,   2,           0,   288, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 125 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100, 138 ),
	CG_FRAME(    0,    2,   1,           0,   258, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,              ATK,  0x00, 0x04,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  12 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  68 ),
	CG_FRAME(    0,    0,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 126 ),
	CG_FRAME(    0,    0,   2,           0,   263, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0600, 126 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100, 139 ),
	CG_FRAME(    0,    2,   6,           0,   218, 0x80,   0x00,              ATK,  0x00, 0x1e,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  12 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0900,  69 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 127 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 127 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0900, 137 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100, 137 ),
	CG_FRAME(    0,    3,   2,           0,   557, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   558, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   559, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   560, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   562, 0x80,   0x00,              ATE,  0x00, 0x16,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    4,   2,           0,   564, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   565, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   566, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   2,           0,   567, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   568, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   569, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   570, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   3,           0,   571, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0f00,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   571, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_NEXT( pl12_super_01 ),
};

/* Char_Super 2: field_init (Plasma Field (hyper)) */
const Sint32 pl12_super_02[] = {
	CG_FRAME(    0,    0,   8,           0,   572, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
	CG_FRAME(    0,    1,   2,           0,   572, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x80,   0x00,              ATK,  0x00, 0x19,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    2,   4,           0,   574, 0x80,   0x00,              ATK,  0x00, 0x1f,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   575, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
};

const Sint32 pl12_super_02_loop[] = {
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    3,   2,           0,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   576, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl12_super_02_loop ),
};

/* Char_Super 3: field_keep (Plasma Field (hyper)) */
const Sint32 pl12_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   481, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   482, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_super_03 ),
};

/* Char_Super 4: field_keep (Plasma Field (hyper)) */
const Sint32 pl12_super_04[] = {
	CG_FRAME(    0,    0,   3,           0,    65, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  14 ),
};

const Sint32 pl12_super_04_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl12_super_04_loop ),
};

/* Char_Super 5: field_fall (Plasma Field (hyper)) */
const Sint32 pl12_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    75, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_super_05 ),
};

/* Char_Super 6: black_init (Black Hayato (hyper)) */
const Sint32 pl12_super_06[] = {
	CG_FRAME(    0,    0,   3,           0,   578, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0101,  12 ),
	CG_FRAME(    0,    0,   1,           0,   578, 0x80,   0x00,              ATK,  0x00, 0x14,    0,   0,     0, 0x0001,  12 ),
	CG_FRAME(    0,    0,   2,           0,   578, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_FRAME(    0,    1,   2,           0,   578, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_FRAME(    0,    0,   3,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001, 130 ),
	CG_FRAME(    0,    0,  10,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001, 130 ),
	CG_FRAME( 0x80,    0,   1,           0,    48, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  72 ),
};

const Sint32 pl12_super_06_loop[] = {
	CG_FRAME( 0x80,    0,   1, END_OF_DATA,    49, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001, 130 ),
	CG_NEXT( pl12_super_06_loop ),
};

/* Char_Super 7: black_move (Black Hayato (hyper)) */
const Sint32 pl12_super_07[] = {
	CG_FRAME(    0,    1,   3,           0,   369, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0101,  12 ),
	CG_FRAME(    0,    0,   3,           0,   370, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_FRAME( 0x80,    0,   3,           0,   371, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_FRAME(    0,    0,   5,           0,   372, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   372, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_NEXT( pl12_super_07 ),
};

/* Char_Catch 4
   Char_Super 8: black_cut (Black Hayato (hyper)) */
const Sint32 pl12_catch_04[] = {
	CG_FRAME(    0,    0,   2,           0,   579, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0109,  12 ),
	CG_FRAME(    0,    0,   1,           0,   580, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x000a,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   580, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x000a,  12 ),
	CG_NEXT( pl12_catch_04 ),
};

/* Char_Super 9 */
const Sint32 pl12_super_09[] = {
	CG_FRAME(    0,    0,   4,           0,   581, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0101,  12 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   581, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0001,  12 ),
	CG_NEXT( pl12_super_09 ),
};

/* Char_Super 10: black_move (Black Hayato (hyper)), black_turn (Black Hayato (hyper)) */
const Sint32 pl12_super_10[] = {
	CG_FRAME(    0,    1,   6,           0,    49, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,   6,           0,    50, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6,           0,    51, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    52, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl12_super_10 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl12_catch_00[] = {
	CG_FRAME(    0,    1,   6,           0,   369, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0101,   1 ),
	CG_FRAME(    0,    2,   6,           0,   370, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    3,   6,           0,   371, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    4,   6,           0,   372, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    5,   6,           0,   373, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b01,   1 ),
	CG_FRAME(    0,    6,   1,           0,   374, 0x80,   0x00,                0,  0x00, 0x05,    0,   0,     0, 0x0b01,   1 ),
	CG_FRAME(    0,    7,   3,           0,   374, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b01,   1 ),
	CG_FRAME(    0,    8,   4,           0,   375, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    9,   1,           0,   376, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    9,   3,           0,   376, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,  12,           0,   380, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   384, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   387, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x3d00,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   387, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl12_catch_00 ),
};

/* Char_Catch 1: nml_k_set
   Char_Catch 2: jump_p_set */
const Sint32 pl12_catch_01[] = {
	CG_FRAME(    0,    0,  16,           0,   398, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0102,   3 ),
	CG_FRAME(    0,    1,   1,           0,   399, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0004,   3 ),
	CG_FRAME(    0,    1,   4,           0,    55, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,    57, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,    58, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    2,   6, END_OF_DATA,    36, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl12_catch_01 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl12_get_up_00[] = {
	CG_FRAME(    0,    0,   8,           0,    90, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   8,           0,    91, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    92, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    93, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl12_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    96, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    97, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    36, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl12_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    97, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    36, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl12_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,    94, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   3,           0,    55, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    56, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    57, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    58, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    36, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_get_up_03 ),
};

/* Char_Damage 2 */
const Sint32 pl12_damage_02[] = {
	CG_FRAME(    0,    0,  10,           0,   590, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,   589, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   588, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_damage_02 ),
};

/* Char_Damage 1 */
const Sint32 pl12_damage_01[] = {
	CG_FRAME(    0,    0,  10,           0,   589, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   4,           0,   588, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_damage_01 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl12_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   588, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_damage_00 ),
};

/* Char_Damage 5: nml_p_set */
const Sint32 pl12_damage_05[] = {
	CG_FRAME(    0,    0,  10,           0,   594, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   4,           0,   593, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   592, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_damage_05 ),
};

/* Char_Damage 4 */
const Sint32 pl12_damage_04[] = {
	CG_FRAME(    0,    0,   8,           0,   593, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   5,           0,   592, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   592, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_damage_04 ),
};

/* Char_Damage 3 */
const Sint32 pl12_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   592, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   3,           0,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_damage_03 ),
};

/* Char_Damage 8 */
const Sint32 pl12_damage_08[] = {
	CG_FRAME(    0,    0,  10,           0,   597, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,   6,           0,   596, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   596, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl12_damage_08 ),
};

/* Char_Damage 7 */
const Sint32 pl12_damage_07[] = {
	CG_FRAME(    0,    0,  10,           0,   596, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,   7,           0,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl12_damage_07 ),
};

/* Char_Damage 6 */
const Sint32 pl12_damage_06[] = {
	CG_FRAME(    0,    0,   8,           0,   596, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl12_damage_06 ),
};

/* Char_Damage 12 */
const Sint32 pl12_damage_12[] = {
	CG_FRAME(    0,    0,   5,           0,   600, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3,           0,   599, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   598, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   598, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_12 ),
};

/* Char_Damage 11 */
const Sint32 pl12_damage_11[] = {
	CG_FRAME(    0,    0,   7,           0,   599, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   2,           0,   598, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   598, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_11 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl12_damage_10[] = {
	CG_FRAME(    0,    0,  15,           0,   598, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   598, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_10 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl12_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,  11 ),
	CG_FRAME(    0,    0,   3,           0,   602, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
};

const Sint32 pl12_damage_13_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   602, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl12_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl12_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   603, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   2,           0,   604, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   605, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   606, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   607, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   608, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   609, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   610, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   612, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   7,           0,   613, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
};

const Sint32 pl12_damage_09_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   613, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl12_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl12_damage_16[] = {
	CG_FRAME(    0,    0,   3,           0,   615, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   616, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl12_damage_17[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   617, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_NEXT( pl12_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl12_damage_18[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   618, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_NEXT( pl12_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl12_damage_19[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   619, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_NEXT( pl12_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl12_damage_20[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   620, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_NEXT( pl12_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl12_damage_21[] = {
	CG_FRAME(    0,    0,   3,           0,   615, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3,           0,   616, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl12_damage_22[] = {
	CG_FRAME(    0,    0,   3,           0,   617, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl12_damage_23[] = {
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl12_damage_24[] = {
	CG_FRAME(    0,    0,   3,           0,   619, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl12_damage_25[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   620, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_NEXT( pl12_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl12_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   621, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,  11 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   623, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   624, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   625, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   626, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl12_damage_26 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl12_damage_31[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   602, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,  11 ),
	CG_NEXT( pl12_damage_31 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl12_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   617, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3,           0,   618, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   641, 0x80,   0x00,                0,  0x00,    0,    2,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   642, 0x80,   0x00,                0,  0x00,    0,    2,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl12_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   627, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3,           0,   628, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   629, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   630, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   631, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   632, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl12_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   633, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3,           0,   634, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   635, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   636, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   637, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   638, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_29 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl12_damage_34[] = {
	CG_FRAME(    0,    0,   3,           0,   610, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   3,           0,   611, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   612, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   612, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_damage_34 ),
};

/* Char_Caught 0 */
const Sint32 pl12_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   588, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   5 ),
	CG_NEXT( pl12_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl12_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   589, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   5 ),
	CG_NEXT( pl12_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl12_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   590, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   5 ),
	CG_NEXT( pl12_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl12_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   592, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   5 ),
	CG_NEXT( pl12_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl12_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   593, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl12_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl12_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   594, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl12_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl12_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   595, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl12_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl12_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   596, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl12_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl12_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   597, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl12_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl12_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   599, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   7 ),
	CG_NEXT( pl12_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl12_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   600, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   7 ),
	CG_NEXT( pl12_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl12_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   601, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   7 ),
	CG_NEXT( pl12_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl12_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   625, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   9 ),
	CG_NEXT( pl12_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl12_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   615, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl12_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl12_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   617, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl12_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl12_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   618, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl12_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl12_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   619, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl12_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl12_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   620, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,  10 ),
	CG_NEXT( pl12_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl12_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   630, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,  10 ),
	CG_NEXT( pl12_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl12_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   636, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   8 ),
	CG_NEXT( pl12_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl12_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   639, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   9 ),
	CG_NEXT( pl12_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl12_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   602, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   9 ),
	CG_NEXT( pl12_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl12_dizzy_00[] = {
	CG_FRAME(    0,    0,   7,           0,   587, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   7,           0,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   592, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   591, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_dizzy_00 ),
};

/* Char_Win 0: win_p3_run_init */
const Sint32 pl12_win_00[] = {
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_win_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    68, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_win_00_loop ),
};

/* Char_Win 1: win_p3_run */
const Sint32 pl12_win_01[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   112, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl12_win_01 ),
};

/* Char_Win 2 */
const Sint32 pl12_win_02[] = {
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_win_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    68, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_win_02_loop ),
};

/* Char_Win 3 */
const Sint32 pl12_win_03[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   142, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl12_win_03 ),
};

/* Char_Win 4: win_ko_init, win_p1_init, win_p2_init, win_p3_init */
const Sint32 pl12_win_04[] = {
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,                0,  0x00, 0x20,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   1,           0,   143, 0x80,   0x00,                0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   144, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   145, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_win_04_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   149, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_win_04_loop ),
};

/* Char_Win 5: win_p1_move, win_p2_move, win_p3_move */
const Sint32 pl12_win_05[] = {
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,  48,           0,   151, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_win_05_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   151, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_win_05_loop ),
};

/* Char_Win 6: lose_ko_init */
const Sint32 pl12_win_06[] = {
	CG_FRAME(    0,    0,   3,           0,   108, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_win_06_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   111, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_win_06_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl12_change_00[] = {
	CG_FRAME(    0,    0,   1,           0,   100, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   1,           0,   100, 0x80,   0x00,                0,  0x00, 0x23,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x00,                0,  0x00, 0x03,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl12_change_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl12_change_02[] = {
	CG_FRAME(    0,    0,   8,           0,    68, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   3 ),
	CG_FRAME(    0,    0,   6,           0,    69, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl12_change_02_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    69, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl12_change_02_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl12_change_01[] = {
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_change_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl12_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,                0,  0x00, 0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   107, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl12_change_03 ),
};

const Sint32 pl12_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3, END_OF_DATA,    24, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	0x00006700,
};

/* Char_Appear 0: appear_a_init */
const Sint32 pl12_appear_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   112, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl12_appear_00 ),
};

/* Char_Appear 1: appear_a_move */
const Sint32 pl12_appear_01[] = {
	CG_FRAME(    0,    0,   2,           0,   113, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_appear_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_appear_01_loop ),
};

/* Char_Appear 2: appear_a_fall */
const Sint32 pl12_appear_02[] = {
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   134, 0x80,   0x00,                0,  0x00, 0x20,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   134, 0x80,   0x00,                0,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   139, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   140, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_appear_02_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_appear_02_loop ),
};

/* Char_Appear 3: appear_b_init */
const Sint32 pl12_appear_03[] = {
	CG_FRAME(    0,    0,   1,           0,   129, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   1,           0,   129, 0x80,   0x00,                0,  0x00, 0x20,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   129, 0x80,   0x00,                0,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   5,           0,   133, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   5,           0,   134, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   135, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   136, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   137, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   138, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   139, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   140, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_appear_03_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_appear_03_loop ),
};

/* Char_Effect 1: ef01_init */
const Sint32 pl12_effect_01[] = {
	CG_FRAME(    0,    0,   3,           0, 32768, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32769, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32770, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32771, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32775, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32776, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32777, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_01 ),
};

/* Char_Effect 2: ef02_init */
const Sint32 pl12_effect_02[] = {
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32778, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32779, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32772, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32773, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32774, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32780, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32781, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_02 ),
};

/* Char_Effect 7: engetsu_init */
const Sint32 pl12_effect_07[] = {
	CG_FRAME(    0,    0,   3,           0,    -1, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  36,           0, 32783, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
};

const Sint32 pl12_effect_07_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32783, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl12_effect_07_loop ),
};

/* Char_Effect 8: engetsu_part_init */
const Sint32 pl12_effect_08[] = {
	CG_FRAME(    0,    0,   3,           0, 32784, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32785, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32786, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32787, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32787, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_08 ),
};

/* Char_Effect 9: engetsu_part_widen */
const Sint32 pl12_effect_09[] = {
	CG_FRAME(    0,    0,   3,           0, 32788, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32789, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32790, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32791, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32791, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_09 ),
};

/* Char_Effect 0: field_init */
const Sint32 pl12_effect_00[] = {
	CG_FRAME(    0,    0,  10,           0, 32782, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32782, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  37,           0, 32782, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32782, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_00 ),
};

/* Char_Effect 10: field_right_init, field_left_init */
const Sint32 pl12_effect_10[] = {
	CG_FRAME(    0,    0,   1,           0,   644, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   645, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   646, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   647, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   648, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   649, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   650, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   651, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   652, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   653, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   654, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl12_effect_10_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   655, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_10_loop ),
};

/* Char_Effect 3: ef03_init */
const Sint32 pl12_effect_03[] = {
	CG_FRAME(    0,    0,   2,           0,   656, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   657, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   658, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   659, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   660, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   660, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_03 ),
};

/* Char_Effect 4: ef04_init */
const Sint32 pl12_effect_04[] = {
	CG_FRAME(    0,    0,   8,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   661, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   662, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   663, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   664, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   665, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   666, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   667, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_04 ),
};

/* Char_Effect 5: ef05_init */
const Sint32 pl12_effect_05[] = {
	CG_FRAME(    0,    0,   2,           0,   656, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   657, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   658, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   659, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   660, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   660, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_05 ),
};

/* Char_Effect 6: ef06_init */
const Sint32 pl12_effect_06[] = {
	CG_FRAME(    0,    0,   8,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   661, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   662, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   663, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   664, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   665, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   666, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   667, 0x80,   0x00,                0,  0x00,    0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl12_effect_06 ),
};

/* group 27 16: pls12_00_catch_init
   group 28 16
   group 29 16
   group 30 16
   group 31 16
   group 32 16 */
const Sint32 pl12_grp27_16[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   388 ),
	CG_NEXT( pl12_grp27_16 ),
};

/* group 27 0
   group 27 17: pls12_00_catch_char
   group 28 0
   group 28 17
   group 29 0
   group 29 17 */
const Sint32 pl12_grp27_00[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   389 ),
	CG_NEXT( pl12_grp27_00 ),
};

/* group 27 18: pls12_00_catch_char
   group 28 18
   group 29 18
   group 30 18
   group 31 18
   group 32 18 */
const Sint32 pl12_grp27_18[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   390 ),
	CG_NEXT( pl12_grp27_18 ),
};

/* group 27 19: pls12_00_catch_char
   group 28 19
   group 29 19
   group 30 19
   group 31 19
   group 32 19 */
const Sint32 pl12_grp27_19[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   391 ),
	CG_NEXT( pl12_grp27_19 ),
};

/* group 27 20: pls12_00_catch_char
   group 28 20
   group 29 20
   group 30 20
   group 31 20
   group 32 20 */
const Sint32 pl12_grp27_20[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   392 ),
	CG_NEXT( pl12_grp27_20 ),
};

/* group 27 21: pls12_00_catch_char
   group 28 21
   group 29 21
   group 30 21
   group 31 21
   group 32 21 */
const Sint32 pl12_grp27_21[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   393 ),
	CG_NEXT( pl12_grp27_21 ),
};

/* group 27 22: pls12_00_catch_char
   group 28 22
   group 29 22
   group 30 22
   group 31 22
   group 32 22 */
const Sint32 pl12_grp27_22[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   394 ),
	CG_NEXT( pl12_grp27_22 ),
};

/* group 27 1
   group 28 1
   group 29 1
   group 30 1
   group 31 1
   group 32 1 */
const Sint32 pl12_grp27_01[] = {
	EF_FRAME(    0,    0,   5,           0,    -1 ),
	EF_FRAME(    0,    0,   4,           0,   668 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   668 ),
	CG_NEXT( pl12_grp27_01 ),
};

/* group 27 2
   group 28 2
   group 29 2
   group 30 2
   group 31 2
   group 32 2 */
const Sint32 pl12_grp27_02[] = {
	EF_FRAME(    0,    0,   5,           0,    -1 ),
	EF_FRAME(    0,    0,   4,           0,   669 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   669 ),
	CG_NEXT( pl12_grp27_02 ),
};

/* group 27 3
   group 28 3
   group 29 3
   group 30 3
   group 31 3
   group 32 3 */
const Sint32 pl12_grp27_03[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   2,           0,   670 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   670 ),
	CG_NEXT( pl12_grp27_03 ),
};

/* group 27 4
   group 28 4
   group 29 4
   group 30 4
   group 31 4
   group 32 4 */
const Sint32 pl12_grp27_04[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   2,           0,   671 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   671 ),
	CG_NEXT( pl12_grp27_04 ),
};

/* group 27 5
   group 28 5
   group 29 5
   group 30 5
   group 31 5
   group 32 5 */
const Sint32 pl12_grp27_05[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   2,           0,   672 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   672 ),
	CG_NEXT( pl12_grp27_05 ),
};

/* group 27 6
   group 28 6
   group 29 6
   group 30 6
   group 31 6
   group 32 6 */
const Sint32 pl12_grp27_06[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   2,           0,   673 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   673 ),
	CG_NEXT( pl12_grp27_06 ),
};

/* group 27 7
   group 28 7
   group 29 7
   group 30 7
   group 31 7
   group 32 7 */
const Sint32 pl12_grp27_07[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   2,           0,   674 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   674 ),
	CG_NEXT( pl12_grp27_07 ),
};

/* group 27 8
   group 28 8
   group 29 8
   group 30 8
   group 31 8
   group 32 8 */
const Sint32 pl12_grp27_08[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   675 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   675 ),
	CG_NEXT( pl12_grp27_08 ),
};

/* group 27 9: pls12_00_bike_init
   group 28 9
   group 29 9
   group 30 9
   group 31 9
   group 32 9 */
const Sint32 pl12_grp27_09[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   694 ),
	CG_NEXT( pl12_grp27_09 ),
};

/* group 27 10: pls12_00_wheel_f_init, pls12_00_wheel_r_init
   group 28 10
   group 29 10
   group 30 10
   group 31 10
   group 32 10 */
const Sint32 pl12_grp27_10[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   695 ),
	CG_NEXT( pl12_grp27_10 ),
};

/* group 27 11: pls12_00_t6_init
   group 28 11
   group 29 11
   group 30 11
   group 31 11
   group 32 11 */
const Sint32 pl12_grp27_11[] = {
	EF_FRAME(    0,    0,   4,           0,   696 ),
	EF_FRAME(    0,    0,   4,           0,   697 ),
	EF_FRAME(    0,    0,   4,           0,   698 ),
	EF_FRAME(    0,    0,   4,           0,   699 ),
	EF_FRAME(    0,    0,   4,           0,   700 ),
	EF_FRAME(    0,    0,   4,           0,   701 ),
	EF_FRAME(    0,    0,  12,           0,   702 ),
	EF_FRAME(    0,    0,   3,           0,   703 ),
	EF_FRAME(    0,    0,   3,           0,   704 ),
	EF_FRAME(    0,    0,   3,           0,   705 ),
	EF_FRAME(    0,    0,   2,           0,   706 ),
};

const Sint32 pl12_grp27_11_loop[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   706 ),
	CG_NEXT( pl12_grp27_11_loop ),
};

/* group 27 12: pls12_00_t7_init
   group 28 12
   group 29 12
   group 30 12
   group 31 12
   group 32 12 */
const Sint32 pl12_grp27_12[] = {
	EF_FRAME(    0,    0,   3,           0,   718 ),
	EF_FRAME(    0,    0,   3,           0,   719 ),
	EF_FRAME(    0,    0,   2,           0,   720 ),
};

const Sint32 pl12_grp27_12_loop[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   720 ),
	CG_NEXT( pl12_grp27_12_loop ),
};

/* group 27 13: pls12_00_t8_init
   group 28 13
   group 29 13
   group 30 13
   group 31 13
   group 32 13 */
const Sint32 pl12_grp27_13[] = {
	EF_FRAME(    0,    0,   3,           0,   707 ),
	EF_FRAME(    0,    0,   3,           0,   708 ),
	EF_FRAME(    0,    0,   3,           0,   709 ),
	EF_FRAME(    0,    0,   3,           0,   710 ),
	EF_FRAME(    0,    0,   3,           0,   711 ),
	EF_FRAME(    0,    0,   3,           0,   712 ),
	EF_FRAME(    0,    0,   3,           0,   713 ),
	EF_FRAME(    0,    0,   3,           0,   714 ),
	EF_FRAME(    0,    0,   3,           0,   715 ),
};

const Sint32 pl12_grp27_13_loop[] = {
	EF_FRAME(    0,    0,   3,           0,   716 ),
	EF_FRAME(    0,    0,   2,           0,   717 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   717 ),
	CG_NEXT( pl12_grp27_13_loop ),
};

/* group 27 14: pls12_00_fade_init, pls12_00_fade_win_init
   group 27 15
   group 28 14
   group 28 15
   group 29 14
   group 29 15 */
const Sint32 pl12_grp27_14[] = {
	EF_FRAME(    0,    0,   1,           0,   676 ),
	EF_FRAME(    0,    0,   1,           0,   677 ),
	EF_FRAME(    0,    0,   1,           0,   678 ),
	EF_FRAME(    0,    0,   1,           0,   679 ),
	EF_FRAME(    0,    0,   2,           0,   680 ),
	EF_FRAME(    0,    0,   2,           0,   681 ),
	EF_FRAME(    0,    0,   2,           0,   682 ),
	EF_FRAME(    0,    0,   2,           0,   683 ),
	EF_FRAME(    0,    0,   2,           0,   684 ),
	EF_FRAME(    0,    0,   2,           0,   685 ),
	EF_FRAME(    0,    0,   3,           0,   686 ),
	EF_FRAME(    0,    0,   3,           0,   687 ),
	EF_FRAME(    0,    0,   3,           0,   688 ),
	EF_FRAME(    0,    0,   3,           0,   689 ),
	EF_FRAME(    0,    0,   3,           0,   690 ),
	EF_FRAME(    0,    0,   4,           0,   691 ),
	EF_FRAME(    0,    0,   4,           0,   692 ),
	EF_FRAME(    0,    0,   4,           0,   693 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   693 ),
	CG_NEXT( pl12_grp27_14 ),
};
