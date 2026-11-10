/* Charlie (character 21): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_21      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_21     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl21.c) says which group
                   and number each move uses.
   pl21_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl21_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl21_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl21_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_21
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_21, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_21 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl21_stand_00[], pl21_stand_02[], pl21_stand_03[], pl21_jump_00[],
	pl21_jump_00_loop[], pl21_jump_01[], pl21_jump_01_loop[], pl21_jump_02[], pl21_jump_02_loop[],
	pl21_jump_03[], pl21_jump_06[], pl21_jump_09[], pl21_jump_07[], pl21_jump_07_loop[],
	pl21_jump_08[], pl21_jump_08_loop[], pl21_dash_00[], pl21_dash_00_loop[], pl21_dash_02[],
	pl21_dash_01[], pl21_dash_01_loop[], pl21_dash_03[], pl21_crouch_00[], pl21_crouch_01[],
	pl21_crouch_02[], pl21_turn_00[], pl21_turn_01[], pl21_guard_00[], pl21_guard_01[],
	pl21_guard_03[], pl21_guard_02[], pl21_guard_02_loop[], pl21_guard_04[], pl21_guard_05[],
	pl21_guard_07[], pl21_guard_06[], pl21_stand_p_00[], pl21_stand_p_01[], pl21_stand_p_02[],
	pl21_stand_p_03[], pl21_stand_k_00[], pl21_stand_k_01[], pl21_stand_k_03[], pl21_stand_k_02[],
	pl21_stand_k_04[], pl21_air_guard_00[], pl21_air_guard_01[], pl21_air_guard_02[],
	pl21_crouch_p_00[], pl21_crouch_p_01[], pl21_crouch_p_02[], pl21_crouch_k_00[],
	pl21_crouch_k_01[], pl21_crouch_k_02[], pl21_jump_p_00[], pl21_jump_p_01[], pl21_jump_p_02[],
	pl21_jump_k_00[], pl21_jump_k_03[], pl21_jump_k_01[], pl21_jump_k_02[], pl21_get_up_00[],
	pl21_get_up_01[], pl21_get_up_02[], pl21_get_up_03[], pl21_appear_00[], pl21_damage_00[],
	pl21_damage_01[], pl21_damage_02[], pl21_damage_03[], pl21_damage_04[], pl21_damage_05[],
	pl21_damage_06[], pl21_damage_07[], pl21_damage_08[], pl21_damage_10[], pl21_damage_11[],
	pl21_damage_12[], pl21_damage_13[], pl21_damage_13_loop[], pl21_damage_09[],
	pl21_damage_09_loop[], pl21_damage_16[], pl21_damage_17[], pl21_damage_18[], pl21_damage_19[],
	pl21_damage_20[], pl21_damage_21[], pl21_damage_22[], pl21_damage_23[], pl21_damage_24[],
	pl21_damage_25[], pl21_damage_26[], pl21_damage_27[], pl21_damage_30[], pl21_damage_32[],
	pl21_damage_33[], pl21_unused_00[], pl21_damage_28[], pl21_damage_29[], pl21_damage_31[],
	pl21_dizzy_00[], pl21_catch_00[], pl21_catch_01[], pl21_catch_02[], pl21_catch_03[],
	pl21_caught_00[], pl21_caught_01[], pl21_caught_02[], pl21_caught_03[], pl21_caught_04[],
	pl21_caught_05[], pl21_caught_06[], pl21_caught_07[], pl21_caught_08[], pl21_caught_09[],
	pl21_caught_10[], pl21_caught_11[], pl21_caught_12[], pl21_caught_13[], pl21_caught_14[],
	pl21_caught_15[], pl21_caught_16[], pl21_caught_17[], pl21_caught_18[], pl21_caught_19[],
	pl21_caught_20[], pl21_caught_21[], pl21_special_00[], pl21_special_01[], pl21_extra_00[],
	pl21_extra_00_loop[], pl21_extra_01[], pl21_extra_02[], pl21_extra_03[], pl21_extra_04[],
	pl21_extra_05[], pl21_extra_05_loop[], pl21_extra_06[], pl21_extra_07[], pl21_extra_07_loop[],
	pl21_extra_08[], pl21_effect_00[], pl21_special_02[], pl21_special_03[], pl21_special_05[],
	pl21_special_06[], pl21_effect_01[], pl21_special_25[], pl21_special_07[], pl21_special_08[],
	pl21_special_09[], pl21_special_10[], pl21_effect_03[], pl21_special_11[], pl21_special_12[],
	pl21_effect_04[], pl21_special_13[], pl21_special_14[], pl21_effect_05[], pl21_special_15[],
	pl21_special_21[], pl21_special_22[], pl21_super_00[], pl21_super_01[], pl21_super_02[],
	pl21_unused_01[], pl21_super_03[], pl21_special_18[], pl21_special_19[], pl21_special_20[],
	pl21_change_00[], pl21_change_01[], pl21_change_02[], pl21_change_02_loop[], pl21_change_03[],
	pl21_change_03_loop[], pl21_change_04[], pl21_win_00[], pl21_win_00_loop[], pl21_win_01[],
	pl21_win_01_loop[], pl21_win_02[], pl21_win_02_loop[], pl21_win_03[], pl21_win_03_loop[],
	pl21_win_04[], pl21_win_04_loop[], pl21_win_05[], pl21_win_05_loop[], pl21_special_24[],
	pl21_unused_02[];
extern const Sint32 *const pl21_stand_tbl[], *const pl21_jump_tbl[], *const pl21_dash_tbl[],
	*const pl21_crouch_tbl[], *const pl21_turn_tbl[], *const pl21_guard_tbl[],
	*const pl21_air_guard_tbl[], *const pl21_stand_p_tbl[], *const pl21_stand_k_tbl[],
	*const pl21_crouch_p_tbl[], *const pl21_crouch_k_tbl[], *const pl21_jump_p_tbl[],
	*const pl21_jump_k_tbl[], *const pl21_damage_tbl[], *const pl21_dizzy_tbl[],
	*const pl21_catch_tbl[], *const pl21_caught_tbl[], *const pl21_get_up_tbl[],
	*const pl21_appear_tbl[], *const pl21_win_tbl[], *const pl21_extra_tbl[],
	*const pl21_special_tbl[], *const pl21_super_tbl[], *const pl21_effect_tbl[],
	*const pl21_change_tbl[], *const pl21_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_21[16] = {
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
const Sint32 *const *const char_tbl_21[] = {
	pl21_stand_tbl, pl21_jump_tbl, pl21_dash_tbl, pl21_crouch_tbl, pl21_turn_tbl, pl21_guard_tbl,
	pl21_air_guard_tbl, pl21_stand_p_tbl, pl21_stand_k_tbl, pl21_crouch_p_tbl, pl21_crouch_k_tbl, pl21_jump_p_tbl,
	pl21_jump_k_tbl, pl21_damage_tbl, pl21_dizzy_tbl, pl21_catch_tbl, pl21_caught_tbl, pl21_get_up_tbl,
	pl21_appear_tbl, pl21_win_tbl, pl21_extra_tbl, pl21_special_tbl, pl21_super_tbl, pl21_effect_tbl,
	pl21_change_tbl, pl21_grp25_tbl, 0,
};

const Sint32 *const pl21_stand_tbl[] = {
	pl21_stand_00, pl21_stand_00, pl21_stand_02, pl21_stand_03, pl21_stand_00, pl21_stand_00,
	pl21_stand_02, pl21_stand_03, 0,
};

const Sint32 *const pl21_jump_tbl[] = {
	pl21_jump_00, pl21_jump_01, pl21_jump_02, pl21_jump_03, pl21_jump_03, pl21_jump_03,
	pl21_jump_06, pl21_jump_07, pl21_jump_08, pl21_jump_09, pl21_jump_09, pl21_jump_09,
	0,
};

const Sint32 *const pl21_dash_tbl[] = {
	pl21_dash_00, pl21_dash_01, pl21_dash_02, pl21_dash_03, 0,
};

const Sint32 *const pl21_crouch_tbl[] = {
	pl21_crouch_00, pl21_crouch_01, pl21_crouch_02, 0,
};

const Sint32 *const pl21_turn_tbl[] = {
	pl21_turn_00, pl21_turn_01, 0,
};

const Sint32 *const pl21_guard_tbl[] = {
	pl21_guard_00, pl21_guard_01, pl21_guard_02, pl21_guard_03, pl21_guard_04, pl21_guard_05,
	pl21_guard_06, pl21_guard_07, 0,
};

const Sint32 *const pl21_air_guard_tbl[] = {
	pl21_air_guard_00, pl21_air_guard_01, pl21_air_guard_02, 0,
};

const Sint32 *const pl21_stand_p_tbl[] = {
	pl21_stand_p_00, pl21_stand_p_01, pl21_stand_p_02, pl21_stand_p_03, 0,
};

const Sint32 *const pl21_stand_k_tbl[] = {
	pl21_stand_k_00, pl21_stand_k_01, pl21_stand_k_02, pl21_stand_k_03, pl21_stand_k_04, 0,
};

const Sint32 *const pl21_crouch_p_tbl[] = {
	pl21_crouch_p_00, pl21_crouch_p_01, pl21_crouch_p_02, 0,
};

const Sint32 *const pl21_crouch_k_tbl[] = {
	pl21_crouch_k_00, pl21_crouch_k_01, pl21_crouch_k_02, 0,
};

const Sint32 *const pl21_jump_p_tbl[] = {
	pl21_jump_p_00, pl21_jump_p_01, pl21_jump_p_02, pl21_jump_p_00, pl21_jump_p_01, pl21_jump_p_02,
	0,
};

const Sint32 *const pl21_jump_k_tbl[] = {
	pl21_jump_k_00, pl21_jump_k_01, pl21_jump_k_02, pl21_jump_k_03, pl21_jump_k_01, pl21_jump_k_02,
	0,
};

const Sint32 *const pl21_damage_tbl[] = {
	pl21_damage_00, pl21_damage_01, pl21_damage_02, pl21_damage_03, pl21_damage_04, pl21_damage_05,
	pl21_damage_06, pl21_damage_07, pl21_damage_08, pl21_damage_09, pl21_damage_10, pl21_damage_11,
	pl21_damage_12, pl21_damage_13, pl21_damage_13, pl21_damage_13, pl21_damage_16, pl21_damage_17,
	pl21_damage_18, pl21_damage_19, pl21_damage_20, pl21_damage_21, pl21_damage_22, pl21_damage_23,
	pl21_damage_24, pl21_damage_25, pl21_damage_26, pl21_damage_27, pl21_damage_28, pl21_damage_29,
	pl21_damage_30, pl21_damage_31, pl21_damage_32, pl21_damage_33, pl21_damage_13, 0,
};

const Sint32 *const pl21_dizzy_tbl[] = {
	pl21_dizzy_00, 0,
};

const Sint32 *const pl21_catch_tbl[] = {
	pl21_catch_00, pl21_catch_01, pl21_catch_02, pl21_catch_03, 0,
};

const Sint32 *const pl21_caught_tbl[] = {
	pl21_caught_00, pl21_caught_01, pl21_caught_02, pl21_caught_03, pl21_caught_04, pl21_caught_05,
	pl21_caught_06, pl21_caught_07, pl21_caught_08, pl21_caught_09, pl21_caught_10, pl21_caught_11,
	pl21_caught_12, pl21_caught_13, pl21_caught_14, pl21_caught_15, pl21_caught_16, pl21_caught_17,
	pl21_caught_18, pl21_caught_19, pl21_caught_20, pl21_caught_21, 0,
};

const Sint32 *const pl21_get_up_tbl[] = {
	pl21_get_up_00, pl21_get_up_01, pl21_get_up_02, pl21_get_up_03, 0,
};

const Sint32 *const pl21_appear_tbl[] = {
	pl21_appear_00, 0,
};

const Sint32 *const pl21_win_tbl[] = {
	pl21_win_00, pl21_win_01, pl21_win_02, pl21_win_03, pl21_win_04, pl21_win_05,
	0,
};

const Sint32 *const pl21_extra_tbl[] = {
	pl21_extra_00, pl21_extra_01, pl21_extra_02, pl21_extra_03, pl21_extra_04, pl21_extra_05,
	pl21_extra_06, pl21_extra_07, pl21_extra_08, 0,
};

const Sint32 *const pl21_special_tbl[] = {
	pl21_special_00, pl21_special_01, pl21_special_02, pl21_special_03, pl21_special_03, pl21_special_05,
	pl21_special_06, pl21_special_07, pl21_special_08, pl21_special_09, pl21_special_10, pl21_special_11,
	pl21_special_12, pl21_special_13, pl21_special_14, pl21_special_15, pl21_special_15, pl21_special_15,
	pl21_special_18, pl21_special_19, pl21_special_20, pl21_special_21, pl21_special_22, pl21_special_22,
	pl21_special_24, pl21_special_25, 0,
};

const Sint32 *const pl21_super_tbl[] = {
	pl21_super_00, pl21_super_01, pl21_super_02, pl21_super_03, 0,
};

const Sint32 *const pl21_effect_tbl[] = {
	pl21_effect_00, pl21_effect_01, pl21_effect_00, pl21_effect_03, pl21_effect_04, pl21_effect_05,
	pl21_special_15, 0,
};

const Sint32 *const pl21_change_tbl[] = {
	pl21_change_00, pl21_change_01, pl21_change_02, pl21_change_03, pl21_change_04, 0,
};

const Sint32 *const pl21_grp25_tbl[] = {
	pl21_stand_00, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0 */
const Sint32 pl21_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl21_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl21_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl21_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl21_jump_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl21_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl21_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl21_jump_01_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl21_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl21_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl21_jump_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl21_jump_02_loop ),
};

/* Char_Jump 3: blitz_catch (Crossfire Blitz (hyper))
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl21_jump_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl21_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl21_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl21_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl21_jump_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl21_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl21_jump_08_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl21_dash_00[] = {
	CG_FRAME(    0,    1,    3,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   12,           0,    56, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl21_dash_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl21_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl21_dash_02[] = {
	CG_FRAME(    0,    0,    2,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl21_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl21_dash_01[] = {
	CG_FRAME(    0,    1,    4,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl21_dash_01_loop[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl21_dash_01_loop ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl21_dash_03[] = {
	CG_FRAME(    0,    1,    6,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    1,    4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    1,    3,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl21_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl21_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl21_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    43, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl21_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl21_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl21_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl21_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl21_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_guard_01 ),
};

/* Char_Guard 3 */
const Sint32 pl21_guard_03[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_guard_03 ),
};

/* Char_Guard 2 */
const Sint32 pl21_guard_02[] = {
	CG_FRAME(    0,    0,    2,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl21_guard_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_guard_02_loop ),
};

/* Char_Guard 4 */
const Sint32 pl21_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl21_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_guard_05 ),
};

/* Char_Guard 7 */
const Sint32 pl21_guard_07[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_guard_07 ),
};

/* Char_Guard 6 */
const Sint32 pl21_guard_06[] = {
	CG_FRAME(    0,    0,    2,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_guard_06 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl21_stand_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   157, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl21_stand_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    5,           0,   160, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   162, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl21_stand_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   163, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   164, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    8,           0,   165, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    5,           0,   164, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_p_02 ),
};

/* Char_Stand_P 3 */
const Sint32 pl21_stand_p_03[] = {
	CG_FRAME(    0,    0,   10,           0,   233, 0x80,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0x81,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x81,              ATK,  0x00,       0x04,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    8,           0,   235, 0x80,   0x81,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   236, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_p_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl21_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   168, 0x80,   0x08,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   169, 0x80,   0x02,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x02,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x08,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl21_stand_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   170, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   172, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    6,           0,   172, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_k_01 ),
};

/* Char_Stand_K 3 */
const Sint32 pl21_stand_k_03[] = {
	CG_FRAME(    0,    0,   12,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   10,           0,   237, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   10,           0,   238, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    2,           0,   239, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   241, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,    84, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    83, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_k_03 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl21_stand_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5,           0,   173, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   174, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2,           0,   175, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    6,           0,   175, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    6,           0,   176, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    6,           0,   168, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_k_02 ),
};

/* Char_Stand_K 4 */
const Sint32 pl21_stand_k_04[] = {
	CG_FRAME(    0,    0,    7,           0,   170, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   171, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   242, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,       0x04,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    6,           0,   242, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   157, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_stand_k_04 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl21_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl21_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl21_air_guard_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_air_guard_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl21_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   178, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   177, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl21_crouch_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   179, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    6,           0,   180, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   177, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl21_crouch_p_02[] = {
	CG_FRAME(    0,    0,    8,           0,   181, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   182, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    8,           0,   183, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    5,           0,   182, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    5,           0,   181, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   177, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl21_crouch_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   184, 0x80,   0x46,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x46,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   177, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl21_crouch_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   186, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0xa1,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    6,           0,   188, 0x80,   0xa1,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    6,           0,   187, 0x80,   0x64,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   185, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl21_crouch_k_02[] = {
	CG_FRAME(    0,    0,    7,           0,   189, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x81,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x81,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    6,           0,   191, 0x80,   0x81,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    5,           0,   187, 0x80,   0x46,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    5,           0,   186, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   185, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl21_jump_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   10,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   192, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl21_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   196, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    6,           0,   196, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   192, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl21_jump_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   194, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    4,           0,   196, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   10,           0,   196, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   192, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_p_02 ),
};

/* Char_Jump_K 0 (light) */
const Sint32 pl21_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   10,           0,   204, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   203, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_k_00 ),
};

/* Char_Jump_K 3 */
const Sint32 pl21_jump_k_03[] = {
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   197, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_k_03 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl21_jump_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   201, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    6,           0,   201, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    76, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl21_jump_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    3,           0,   202, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    8,           0,   202, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6,           0,   204, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    76, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl21_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl21_get_up_00[] = {
	CG_FRAME(    0,    0,    6,           0,   126, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   127, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl21_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   126, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl21_get_up_02[] = {
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl21_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl21_appear_00[] = {
	CG_FRAME(    0,    0,   10,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   121, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_appear_00 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl21_damage_00[] = {
	CG_FRAME( 0x80,    0,    2,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    6,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl21_damage_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl21_damage_02[] = {
	CG_FRAME( 0x80,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl21_damage_03[] = {
	CG_FRAME( 0x80,    0,    2,           0,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    6,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl21_damage_04[] = {
	CG_FRAME( 0x80,    0,    2,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl21_damage_05[] = {
	CG_FRAME( 0x80,    0,    4,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   262, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl21_damage_06[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   263, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl21_damage_07[] = {
	CG_FRAME( 0x80,    0,    3,           0,   263, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    9,           0,   264, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   263, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl21_damage_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   263, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   264, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   10,           0,   265, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    6,           0,   264, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    4, END_OF_DATA,   263, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl21_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl21_damage_10[] = {
	CG_FRAME( 0x80,    0,    1,           0,   266, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    8,           0,   267, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   266, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl21_damage_11[] = {
	CG_FRAME( 0x80,    0,    1,           0,   266, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    1,           0,   267, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    6,           0,   268, 0x80,   0x38,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   267, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   266, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl21_damage_12[] = {
	CG_FRAME( 0x80,    0,    1,           0,   266, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    1,           0,   267, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    1,           0,   268, 0x80,   0x38,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    4,           0,   269, 0x80,   0x3c,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   268, 0x80,   0x38,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   267, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   266, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl21_damage_13[] = {
	CG_FRAME( 0x80,    0,    2,           0,   273, 0x80,   0x6d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x82,    0,    2,           0,   274, 0x80,   0x6d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   275, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl21_damage_13_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   276, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl21_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl21_damage_09[] = {
	CG_FRAME(    0,    0,    6,           0,   277, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   278, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   279, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x8e,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,    2,           0,   273, 0x80,   0x6d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x82,    0,    2,           0,   274, 0x80,   0x6d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl21_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   281, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl21_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl21_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   284, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl21_damage_17[] = {
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   286, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl21_damage_18[] = {
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   288, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl21_damage_19[] = {
	CG_FRAME(    0,    0,    2,           0,   289, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   290, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl21_damage_20[] = {
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   292, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl21_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl21_damage_22[] = {
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl21_damage_23[] = {
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl21_damage_24[] = {
	CG_FRAME(    0,    0,    2,           0,   289, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl21_damage_25[] = {
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   292, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl21_damage_26[] = {
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   294, 0x80,   0xcd,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   280, 0x80,   0x6d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   281, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl21_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl21_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x2b,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl21_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x2b,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_30 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl21_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl21_damage_33[] = {
	CG_FRAME(    0,    0,    6,           0,   310, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    43, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_damage_33 ),
};

const Sint32 pl21_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	0x00002600,
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl21_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   296, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   297, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   298, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   300, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl21_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   303, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   304, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   306, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl21_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl21_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   282, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl21_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl21_dizzy_00[] = {
	CG_FRAME(    0,    0,    8,           0,   270, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   271, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl21_catch_00[] = {
	CG_FRAME(    0,    0,    2,           0,   205, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    8,           0,   206, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    6,           0,   207, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    8,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,    8,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    8,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    84, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    83, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_catch_00 ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl21_catch_01[] = {
	CG_FRAME(    0,    0,   16,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,   -1,   16,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   16,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,   -1,   16,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   16,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,   -1,   16,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   16,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,   -1,    1,           0,   173, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,   15,           0,   173, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   174, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   175, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   176, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_catch_01 ),
};

/* Char_Catch 2: jump_p_set */
const Sint32 pl21_catch_02[] = {
	CG_FRAME(    0,    0,    6,           0,   225, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    6,           0,   225, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   225, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_NEXT( pl21_catch_02 ),
};

/* Char_Catch 3: jump_p_catch */
const Sint32 pl21_catch_03[] = {
	CG_FRAME(    0,   -1,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    0,    4,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   1 ),
	CG_FRAME(    0,    1,    1,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_catch_03 ),
};

/* Char_Caught 0 */
const Sint32 pl21_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl21_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl21_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl21_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl21_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl21_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   262, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl21_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   263, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl21_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   264, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl21_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   265, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl21_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   267, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl21_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   268, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl21_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   269, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl21_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl21_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   283, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl21_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   285, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl21_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl21_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl21_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl21_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl21_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl21_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   282, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl21_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   277, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl21_caught_21 ),
};

/* Char_Special 0: sonic_init (Sonic Boom) */
const Sint32 pl21_special_00[] = {
	CG_FRAME(    0,    0,    5,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   211, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  59 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  59 ),
	CG_FRAME(    0,    0,    8,           0,   213, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  59 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   213, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0xfe00,  59 ),
	CG_NEXT( pl21_special_00 ),
};

/* Char_Special 1: sonic_move (Sonic Boom) */
const Sint32 pl21_special_01[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_special_01 ),
};

/* Char_Extra 0: shl21_00_sonic_init, shl21_00_break_init */
const Sint32 pl21_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
};

const Sint32 pl21_extra_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl21_extra_00_loop ),
};

/* Char_Extra 1: shl21_00_sonic_hit, shl21_00_break_hit */
const Sint32 pl21_extra_01[] = {
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32786, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32787, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32788, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_extra_01 ),
};

/* Char_Extra 2: sp_chouhatsu (Taunt), fly_start5 */
const Sint32 pl21_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_extra_02 ),
};

/* Char_Extra 3: t_kick_init, fly_start5 */
const Sint32 pl21_extra_03[] = {
	CG_FRAME(    0,    0,    2,           0,   203, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   204, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  127, END_OF_DATA,   202, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl21_extra_03 ),
};

/* Char_Extra 4: t_kick_move */
const Sint32 pl21_extra_04[] = {
	CG_FRAME(    0,    0,    2,           0,    84, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,    43, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   100, 0x80,   0x00,              ATE,  0x00,       0x0a,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   101, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   150, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   151, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   12,           0,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,    85, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,    86, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    87, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_extra_04 ),
};

/* Char_Extra 5: shl21_00_smr_init */
const Sint32 pl21_extra_05[] = {
	CG_FRAME(    0,    0,    1,           0, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl21_extra_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl21_extra_05_loop ),
};

/* Char_Extra 6: shl21_00_land */
const Sint32 pl21_extra_06[] = {
	CG_FRAME(    0,    0,    4,           0, 32792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_extra_06 ),
};

/* Char_Extra 7: shl21_00_justice_init */
const Sint32 pl21_extra_07[] = {
	CG_FRAME(    0,    0,    2,           0, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl21_extra_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    2,           0, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl21_extra_07_loop ),
};

/* Char_Extra 8: pl21_upper */
const Sint32 pl21_extra_08[] = {
	CG_FRAME(    0,    0,    6,           0,   181, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   182, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    6,           0,   183, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    6,           0,   183, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    6,           0,   181, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   177, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl21_extra_08 ),
};

/* Char_Effect 0
   Char_Effect 2 */
const Sint32 pl21_effect_00[] = {
	CG_FRAME(    0,    0,    1,           0, 32774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32776, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32777, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_NEXT( pl21_effect_00 ),
};

/* Char_Special 2 */
const Sint32 pl21_special_02[] = {
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    3,           0,   216, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  61 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  61 ),
	CG_FRAME(    0,    0,    3,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    3,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xff00,  63 ),
	CG_NEXT( pl21_special_02 ),
};

/* Char_Special 3
   Char_Special 4 */
const Sint32 pl21_special_03[] = {
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    3,           0,   216, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0, 0x0200,  70 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  70 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  70 ),
	CG_FRAME(    0,    0,    3,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    2,    3,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  71 ),
	CG_FRAME(    0,    0,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  71 ),
	CG_FRAME(    0,    0,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0c00,  63 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xff00,  63 ),
	CG_NEXT( pl21_special_03 ),
};

/* Char_Special 5: smr_jump (Somersault Shell), justice_air (Somersault Justice (hyper)) */
const Sint32 pl21_special_05[] = {
	CG_FRAME(    0,    0,    3,           0,   222, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   223, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   224, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0xfe00,  64 ),
	CG_NEXT( pl21_special_05 ),
};

/* Char_Special 6: smr_fall (Somersault Shell), justice_fall (Somersault Justice (hyper)) */
const Sint32 pl21_special_06[] = {
	CG_FRAME(    0,    0,    8,           0,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    87, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_special_06 ),
};

/* Char_Effect 1 */
const Sint32 pl21_effect_01[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_NEXT( pl21_effect_01 ),
};

/* Char_Special 25: break_init (Sonic Break (hyper)) */
const Sint32 pl21_special_25[] = {
	CG_FRAME(    0,    0,   24,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 7 */
const Sint32 pl21_special_07[] = {
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   213, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,   1 ),
	CG_NEXT( pl21_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl21_special_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x8000,   1 ),
	CG_NEXT( pl21_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl21_special_09[] = {
	CG_FRAME(    0,    0,    1,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,   1 ),
	CG_NEXT( pl21_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl21_special_10[] = {
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x8000,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   162, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_special_10 ),
};

/* Char_Effect 3 */
const Sint32 pl21_effect_03[] = {
	CG_FRAME(    0,    0,    1,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_NEXT( pl21_effect_03 ),
};

/* Char_Special 11 */
const Sint32 pl21_special_11[] = {
	CG_FRAME(    0,    0,    1,           0,   160, 0x80,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x0c,              ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x81,              ATK,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x81,              ATK,  0x00,       0x1e,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x81,              ATK,  0x00,          0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   235, 0x80,   0x81,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,   1 ),
	CG_NEXT( pl21_special_11 ),
};

/* Char_Special 12 */
const Sint32 pl21_special_12[] = {
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x81,              ATE,  0x00,          0,    0,   0,     0, 0x8000,   1 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_special_12 ),
};

/* Char_Effect 4 */
const Sint32 pl21_effect_04[] = {
	CG_FRAME(    0,    0,    1,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_NEXT( pl21_effect_04 ),
};

/* Char_Special 13 */
const Sint32 pl21_special_13[] = {
	CG_FRAME(    0,    0,    1,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x8000,   1 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0x00,              ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x00,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,   1 ),
	CG_NEXT( pl21_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl21_special_14[] = {
	CG_FRAME(    0,    1,    1,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    84, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    83, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_special_14 ),
};

/* Char_Effect 5 */
const Sint32 pl21_effect_05[] = {
	CG_FRAME(    0,    0,    1,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_NEXT( pl21_effect_05 ),
};

/* Char_Special 15
   Char_Special 16
   Char_Special 17
   Char_Effect 6 */
const Sint32 pl21_special_15[] = {
	CG_FRAME(    0,    0,    1,           0, 32796, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32797, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32798, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   0 ),
	CG_NEXT( pl21_special_15 ),
};

/* Char_Special 21 */
const Sint32 pl21_special_21[] = {
	CG_FRAME(    0,    1,    4,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   244, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   245, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   246, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   247, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0, 0x0200,  87 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  87 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  88 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   248, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl21_special_21 ),
};

/* Char_Special 22
   Char_Special 23 */
const Sint32 pl21_special_22[] = {
	CG_FRAME(    0,    1,    4,           0,   243, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    4,           0,   244, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    6,           0,   245, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   246, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   247, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0, 0x0200,  87 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,  87 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  88 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   248, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl21_special_22 ),
};

/* Char_Super 0: blitz_init (Crossfire Blitz (hyper)) */
const Sint32 pl21_super_00[] = {
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    87, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    60, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,    65, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl21_super_00 ),
};

/* Char_Super 1: blitz_dash (Crossfire Blitz (hyper)) */
const Sint32 pl21_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,    59, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,    61, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    62, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl21_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl21_super_02[] = {
	CG_FRAME(    0,    0,    3,           0,   168, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   169, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   242, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x00,              ATK,  0x4b,       0x03,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   157, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   164, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    3,           0,   164, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    6,           0,   136, 0x80,   0x00,              ATK,  0x4c,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    6,           0,   134, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   135, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl21_super_02 ),
};

const Sint32 pl21_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    6,           0,   203, 0x80,   0x00,              ATK,  0x4c,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   202, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl21_super_02 ),
};

/* Char_Super 3: blitz_dash (Crossfire Blitz (hyper)) */
const Sint32 pl21_super_03[] = {
	CG_FRAME(    0,    0,    2,           0,   167, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    2,           0,   168, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,              ATK,  0x4b,       0x04,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2,           0,   170, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,   170, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x00,              ATK,  0x4b,       0x04,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   237, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,              ATK,  0x4b,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   189, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,    56, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,    58, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   134, 0x80,   0x00,              ATK,  0x4c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   135, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    2,           0,   136, 0x80,   0x00,              ATK,  0x4c,       0x05,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   233, 0x80,   0x00,              ATK,  0x4e,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   10,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    2,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   241, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,    84, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    83, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_super_03 ),
};

/* Char_Special 18: justice_init (Somersault Justice (hyper)) */
const Sint32 pl21_special_18[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    1,           0,    95, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Special 19: justice_move (Somersault Justice (hyper)) */
const Sint32 pl21_special_19[] = {
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0, 0x0200,  82 ),
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0, 0x0500,  82 ),
	CG_FRAME(    0,    2,    2,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0700,  82 ),
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  83 ),
	CG_FRAME(    0,    2,    2,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0900,  84 ),
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0b00,  84 ),
	CG_FRAME(    0,    0,    1,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0d00,  83 ),
	CG_FRAME(    0,    0,    1,           0,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   222, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1,           0,   223, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  83 ),
	CG_NEXT( pl21_special_18 ),
};

/* Char_Special 20: justice_move (Somersault Justice (hyper)) */
const Sint32 pl21_special_20[] = {
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   214, 0x80,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   216, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0, 0x0200,  82 ),
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  82 ),
	CG_FRAME(    0,   10,    3,           0,   217, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  82 ),
	CG_FRAME(    0,   12,    3,           0,   218, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    3,           0,   219, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  32 ),
	CG_FRAME(    0,   12,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0a00,  84 ),
	CG_FRAME(    0,    0,    3,           0,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0c00,  84 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   221, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  84 ),
	CG_NEXT( pl21_special_19 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl21_change_00[] = {
	CG_FRAME(    0,    0,    1,           0,   168, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   103, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   104, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   105, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   106, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   107, 0x30,   0x00,              ATK,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   10, END_OF_DATA,   108, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl21_change_01[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   227, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl21_change_02[] = {
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl21_change_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl21_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl21_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   149, 0x80,   0x00,              ATK,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl21_change_03_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl21_change_04[] = {
	CG_FRAME(    0,    0,    2,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_change_04 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl21_win_00[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl21_win_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl21_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   103, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl21_win_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_win_01_loop ),
};

/* Char_Win 2: win_ko_init */
const Sint32 pl21_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
};

const Sint32 pl21_win_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_win_02_loop ),
};

/* Char_Win 3: win_ko_init, win_time */
const Sint32 pl21_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl21_win_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl21_win_03_loop ),
};

/* Char_Win 4: lose_time_ko */
const Sint32 pl21_win_04[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl21_win_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_win_04_loop ),
};

/* Char_Win 5: draw_time */
const Sint32 pl21_win_05[] = {
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl21_win_05_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl21_win_05_loop ),
};

/* Char_Special 24: snapback_init (Snapback) */
const Sint32 pl21_special_24[] = {
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   233, 0x80,   0x0c,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0x81,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x80,   0x81,              ATK,  0x00,       0x04,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    8,           0,   235, 0x80,   0x81,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   236, 0x80,   0x48,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_special_24 ),
};

const Sint32 pl21_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   12,           0,   233, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   10,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   10,           0,   238, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    2,           0,   239, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   241, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    8,           0,    84, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    83, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl21_special_24 ),
};
