/* Shuma Gorath (character 2d): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_2d      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_2d     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl2d.c) says which group
                   and number each move uses.
   pl2d_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl2d_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl2d_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl2d_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_2d
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_2d, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_2d */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl2d_stand_00[], pl2d_stand_02[], pl2d_stand_03[], pl2d_stand_06[],
	pl2d_stand_07[], pl2d_dash_00[], pl2d_dash_00_loop[], pl2d_dash_01[], pl2d_dash_02[],
	pl2d_jump_00[], pl2d_jump_00_loop[], pl2d_jump_01[], pl2d_jump_01_loop[], pl2d_jump_02[],
	pl2d_jump_02_loop[], pl2d_jump_03[], pl2d_jump_06[], pl2d_jump_06_loop[], pl2d_jump_07[],
	pl2d_jump_07_loop[], pl2d_jump_08[], pl2d_jump_08_loop[], pl2d_jump_09[], pl2d_jump_10[],
	pl2d_jump_11[], pl2d_crouch_00[], pl2d_crouch_01[], pl2d_crouch_02[], pl2d_turn_00[],
	pl2d_turn_01[], pl2d_guard_00[], pl2d_guard_01[], pl2d_guard_02[], pl2d_guard_03[],
	pl2d_guard_04[], pl2d_guard_05[], pl2d_guard_06[], pl2d_guard_07[], pl2d_air_guard_00[],
	pl2d_air_guard_01[], pl2d_air_guard_05[], pl2d_air_guard_02[], pl2d_air_guard_02_loop[],
	pl2d_air_guard_03[], pl2d_air_guard_03_loop[], pl2d_air_guard_04[], pl2d_air_guard_04_loop[],
	pl2d_stand_p_00[], pl2d_stand_p_01[], pl2d_stand_p_02[], pl2d_stand_k_00[], pl2d_stand_k_01[],
	pl2d_stand_k_02[], pl2d_crouch_p_00[], pl2d_crouch_p_01[], pl2d_crouch_p_02[],
	pl2d_crouch_k_00[], pl2d_crouch_k_01[], pl2d_crouch_k_02[], pl2d_jump_p_00[], pl2d_jump_p_01[],
	pl2d_jump_p_02[], pl2d_jump_k_00[], pl2d_jump_k_01[], pl2d_jump_k_02[], pl2d_jump_p_05[],
	pl2d_jump_k_05[], pl2d_jump_k_05_loop[], pl2d_damage_00[], pl2d_damage_01[], pl2d_damage_02[],
	pl2d_damage_03[], pl2d_damage_04[], pl2d_damage_05[], pl2d_damage_06[], pl2d_damage_07[],
	pl2d_damage_08[], pl2d_damage_10[], pl2d_damage_11[], pl2d_damage_12[], pl2d_damage_13[],
	pl2d_damage_13_loop[], pl2d_damage_09[], pl2d_damage_09_loop[], pl2d_damage_16[],
	pl2d_damage_17[], pl2d_damage_18[], pl2d_damage_19[], pl2d_damage_20[], pl2d_damage_21[],
	pl2d_damage_22[], pl2d_damage_23[], pl2d_damage_24[], pl2d_damage_25[], pl2d_damage_26[],
	pl2d_damage_28[], pl2d_damage_29[], pl2d_damage_27[], pl2d_damage_30[], pl2d_damage_33[],
	pl2d_damage_31[], pl2d_dizzy_00[], pl2d_damage_34[], pl2d_catch_00[], pl2d_catch_02[],
	pl2d_catch_02_loop[], pl2d_catch_10[], pl2d_catch_11[], pl2d_catch_01[], pl2d_caught_00[],
	pl2d_caught_01[], pl2d_caught_02[], pl2d_caught_03[], pl2d_caught_04[], pl2d_caught_05[],
	pl2d_caught_06[], pl2d_caught_07[], pl2d_caught_08[], pl2d_caught_09[], pl2d_caught_10[],
	pl2d_caught_11[], pl2d_caught_12[], pl2d_caught_13[], pl2d_caught_18[], pl2d_caught_14[],
	pl2d_caught_15[], pl2d_caught_16[], pl2d_caught_17[], pl2d_unused_00[], pl2d_unused_01[],
	pl2d_unused_02[], pl2d_unused_03[], pl2d_unused_04[], pl2d_unused_05[], pl2d_caught_19[],
	pl2d_caught_20[], pl2d_get_up_01[], pl2d_get_up_01_loop[], pl2d_get_up_02[],
	pl2d_get_up_02_loop[], pl2d_get_up_03[], pl2d_get_up_00[], pl2d_win_02[], pl2d_win_02_loop[],
	pl2d_extra_00[], pl2d_change_04[], pl2d_win_00[], pl2d_win_00_loop[], pl2d_win_01[],
	pl2d_win_01_loop[], pl2d_change_03[], pl2d_change_03_loop[], pl2d_change_00[],
	pl2d_change_00_loop[], pl2d_extra_01[], pl2d_extra_02[], pl2d_appear_00[], pl2d_unused_06[],
	pl2d_unused_07[], pl2d_special_00[], pl2d_special_01[], pl2d_special_03[], pl2d_special_04[],
	pl2d_special_05[], pl2d_special_06[], pl2d_special_07[], pl2d_special_08[], pl2d_special_09[],
	pl2d_special_10[], pl2d_special_11[], pl2d_unused_08[], pl2d_special_12[], pl2d_catch_08[],
	pl2d_catch_08_loop[], pl2d_catch_09[], pl2d_special_15[], pl2d_special_16[], pl2d_special_17[],
	pl2d_super_00[], pl2d_super_01[], pl2d_super_02[], pl2d_super_03[], pl2d_catch_03[],
	pl2d_catch_04[], pl2d_catch_05[], pl2d_catch_06[], pl2d_catch_07[], pl2d_super_05[],
	pl2d_super_07[], pl2d_super_07_loop[], pl2d_super_08[], pl2d_super_09[], pl2d_special_18[],
	pl2d_special_19[], pl2d_special_19_loop[], pl2d_unused_09[], pl2d_special_20[], pl2d_effect_00[],
	pl2d_effect_01[], pl2d_effect_02[], pl2d_effect_02_loop[], pl2d_effect_03[],
	pl2d_effect_03_loop[], pl2d_effect_04[], pl2d_effect_05[], pl2d_effect_06[],
	pl2d_effect_06_loop[], pl2d_effect_07[], pl2d_effect_08[], pl2d_effect_09[], pl2d_effect_10[],
	pl2d_effect_11[], pl2d_effect_12[], pl2d_effect_13[], pl2d_effect_14[], pl2d_effect_15[],
	pl2d_effect_16[], pl2d_effect_17[], pl2d_effect_18[], pl2d_effect_19[], pl2d_effect_20[],
	pl2d_effect_21[], pl2d_effect_22[], pl2d_effect_23[], pl2d_effect_24[], pl2d_effect_25[],
	pl2d_effect_31[], pl2d_effect_31_loop[], pl2d_effect_26[], pl2d_effect_27[], pl2d_effect_28[],
	pl2d_effect_29[], pl2d_effect_30[], pl2d_unused_10[], pl2d_unused_11[];
extern const Sint32 *const pl2d_stand_tbl[], *const pl2d_jump_tbl[], *const pl2d_dash_tbl[],
	*const pl2d_crouch_tbl[], *const pl2d_turn_tbl[], *const pl2d_guard_tbl[],
	*const pl2d_air_guard_tbl[], *const pl2d_stand_p_tbl[], *const pl2d_stand_k_tbl[],
	*const pl2d_crouch_p_tbl[], *const pl2d_crouch_k_tbl[], *const pl2d_jump_p_tbl[],
	*const pl2d_jump_k_tbl[], *const pl2d_damage_tbl[], *const pl2d_dizzy_tbl[],
	*const pl2d_catch_tbl[], *const pl2d_caught_tbl[], *const pl2d_get_up_tbl[],
	*const pl2d_appear_tbl[], *const pl2d_win_tbl[], *const pl2d_extra_tbl[],
	*const pl2d_special_tbl[], *const pl2d_super_tbl[], *const pl2d_effect_tbl[],
	*const pl2d_change_tbl[], *const pl2d_grp25_tbl[], *const pl2d_fly_tbl[],
	*const pl2d_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_2d[16] = {
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
const Sint32 *const *const char_tbl_2d[] = {
	pl2d_stand_tbl, pl2d_jump_tbl, pl2d_dash_tbl, pl2d_crouch_tbl, pl2d_turn_tbl, pl2d_guard_tbl,
	pl2d_air_guard_tbl, pl2d_stand_p_tbl, pl2d_stand_k_tbl, pl2d_crouch_p_tbl, pl2d_crouch_k_tbl, pl2d_jump_p_tbl,
	pl2d_jump_k_tbl, pl2d_damage_tbl, pl2d_dizzy_tbl, pl2d_catch_tbl, pl2d_caught_tbl, pl2d_get_up_tbl,
	pl2d_appear_tbl, pl2d_win_tbl, pl2d_extra_tbl, pl2d_special_tbl, pl2d_super_tbl, pl2d_effect_tbl,
	pl2d_change_tbl, pl2d_grp25_tbl, pl2d_fly_tbl, pl2d_grp27_tbl, 0,
};

const Sint32 *const pl2d_stand_tbl[] = {
	pl2d_stand_00, pl2d_stand_00, pl2d_stand_02, pl2d_stand_03, pl2d_stand_00, pl2d_stand_00,
	pl2d_stand_06, pl2d_stand_07, 0,
};

const Sint32 *const pl2d_jump_tbl[] = {
	pl2d_jump_00, pl2d_jump_01, pl2d_jump_02, pl2d_jump_03, pl2d_jump_03, pl2d_jump_03,
	pl2d_jump_06, pl2d_jump_07, pl2d_jump_08, pl2d_jump_09, pl2d_jump_10, pl2d_jump_11,
	0,
};

const Sint32 *const pl2d_dash_tbl[] = {
	pl2d_dash_00, pl2d_dash_01, pl2d_dash_02, pl2d_stand_00, 0,
};

const Sint32 *const pl2d_crouch_tbl[] = {
	pl2d_crouch_00, pl2d_crouch_01, pl2d_crouch_02, 0,
};

const Sint32 *const pl2d_turn_tbl[] = {
	pl2d_turn_00, pl2d_turn_01, 0,
};

const Sint32 *const pl2d_guard_tbl[] = {
	pl2d_guard_00, pl2d_guard_01, pl2d_guard_02, pl2d_guard_03, pl2d_guard_04, pl2d_guard_05,
	pl2d_guard_06, pl2d_guard_07, 0,
};

const Sint32 *const pl2d_air_guard_tbl[] = {
	pl2d_air_guard_00, pl2d_air_guard_01, pl2d_air_guard_02, pl2d_air_guard_03, pl2d_air_guard_04, pl2d_air_guard_05,
	0,
};

const Sint32 *const pl2d_stand_p_tbl[] = {
	pl2d_stand_p_00, pl2d_stand_p_01, pl2d_stand_p_02, 0,
};

const Sint32 *const pl2d_stand_k_tbl[] = {
	pl2d_stand_k_00, pl2d_stand_k_01, pl2d_stand_k_02, 0,
};

const Sint32 *const pl2d_crouch_p_tbl[] = {
	pl2d_crouch_p_00, pl2d_crouch_p_01, pl2d_crouch_p_02, 0,
};

const Sint32 *const pl2d_crouch_k_tbl[] = {
	pl2d_crouch_k_00, pl2d_crouch_k_01, pl2d_crouch_k_02, 0,
};

const Sint32 *const pl2d_jump_p_tbl[] = {
	pl2d_jump_p_00, pl2d_jump_p_01, pl2d_jump_p_02, pl2d_jump_p_00, pl2d_jump_p_01, pl2d_jump_p_05,
	0,
};

const Sint32 *const pl2d_jump_k_tbl[] = {
	pl2d_jump_k_00, pl2d_jump_k_01, pl2d_jump_k_02, pl2d_jump_k_00, pl2d_jump_k_01, pl2d_jump_k_05,
	0,
};

const Sint32 *const pl2d_damage_tbl[] = {
	pl2d_damage_00, pl2d_damage_01, pl2d_damage_02, pl2d_damage_03, pl2d_damage_04, pl2d_damage_05,
	pl2d_damage_06, pl2d_damage_07, pl2d_damage_08, pl2d_damage_09, pl2d_damage_10, pl2d_damage_11,
	pl2d_damage_12, pl2d_damage_13, pl2d_damage_13, pl2d_damage_13, pl2d_damage_16, pl2d_damage_17,
	pl2d_damage_18, pl2d_damage_19, pl2d_damage_20, pl2d_damage_21, pl2d_damage_22, pl2d_damage_23,
	pl2d_damage_24, pl2d_damage_25, pl2d_damage_26, pl2d_damage_27, pl2d_damage_28, pl2d_damage_29,
	pl2d_damage_30, pl2d_damage_31, pl2d_damage_30, pl2d_damage_33, pl2d_damage_34, 0,
};

const Sint32 *const pl2d_dizzy_tbl[] = {
	pl2d_dizzy_00, pl2d_stand_00, 0,
};

const Sint32 *const pl2d_catch_tbl[] = {
	pl2d_catch_00, pl2d_catch_01, pl2d_catch_02, pl2d_catch_03, pl2d_catch_04, pl2d_catch_05,
	pl2d_catch_06, pl2d_catch_07, pl2d_catch_08, pl2d_catch_09, pl2d_catch_10, pl2d_catch_11,
	0,
};

const Sint32 *const pl2d_caught_tbl[] = {
	pl2d_caught_00, pl2d_caught_01, pl2d_caught_02, pl2d_caught_03, pl2d_caught_04, pl2d_caught_05,
	pl2d_caught_06, pl2d_caught_07, pl2d_caught_08, pl2d_caught_09, pl2d_caught_10, pl2d_caught_11,
	pl2d_caught_12, pl2d_caught_13, pl2d_caught_14, pl2d_caught_15, pl2d_caught_16, pl2d_caught_17,
	pl2d_caught_18, pl2d_caught_19, pl2d_caught_20, pl2d_caught_09, 0,
};

const Sint32 *const pl2d_get_up_tbl[] = {
	pl2d_get_up_00, pl2d_get_up_01, pl2d_get_up_02, pl2d_get_up_03, 0,
};

const Sint32 *const pl2d_appear_tbl[] = {
	pl2d_appear_00, 0,
};

const Sint32 *const pl2d_win_tbl[] = {
	pl2d_win_00, pl2d_win_01, pl2d_win_02, 0,
};

const Sint32 *const pl2d_extra_tbl[] = {
	pl2d_extra_00, pl2d_extra_01, pl2d_extra_02, 0,
};

const Sint32 *const pl2d_special_tbl[] = {
	pl2d_special_00, pl2d_special_01, pl2d_special_01, pl2d_special_03, pl2d_special_04, pl2d_special_05,
	pl2d_special_06, pl2d_special_07, pl2d_special_08, pl2d_special_09, pl2d_special_10, pl2d_special_11,
	pl2d_special_12, pl2d_catch_08, pl2d_catch_09, pl2d_special_15, pl2d_special_16, pl2d_special_17,
	pl2d_special_18, pl2d_special_19, pl2d_special_20, 0,
};

const Sint32 *const pl2d_super_tbl[] = {
	pl2d_super_00, pl2d_super_01, pl2d_super_02, pl2d_super_03, pl2d_super_00, pl2d_super_05,
	pl2d_super_05, pl2d_super_07, pl2d_super_08, pl2d_super_09, 0,
};

const Sint32 *const pl2d_effect_tbl[] = {
	pl2d_effect_00, pl2d_effect_01, pl2d_effect_02, pl2d_effect_03, pl2d_effect_04, pl2d_effect_05,
	pl2d_effect_06, pl2d_effect_07, pl2d_effect_08, pl2d_effect_09, pl2d_effect_10, pl2d_effect_11,
	pl2d_effect_12, pl2d_effect_13, pl2d_effect_14, pl2d_effect_15, pl2d_effect_16, pl2d_effect_17,
	pl2d_effect_18, pl2d_effect_19, pl2d_effect_20, pl2d_effect_21, pl2d_effect_22, pl2d_effect_23,
	pl2d_effect_24, pl2d_effect_25, pl2d_effect_26, pl2d_effect_27, pl2d_effect_28, pl2d_effect_29,
	pl2d_effect_30, pl2d_effect_31, 0,
};

const Sint32 *const pl2d_change_tbl[] = {
	pl2d_change_00, pl2d_stand_00, pl2d_jump_11, pl2d_change_03, pl2d_change_04, 0,
};

const Sint32 *const pl2d_grp25_tbl[] = {
	pl2d_stand_00, 0,
};

const Sint32 *const pl2d_fly_tbl[] = {
	pl2d_stand_00, 0,
};

const Sint32 *const pl2d_grp27_tbl[] = {
	pl2d_stand_00, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_end, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Dash 3
   Char_Dizzy 1 */
const Sint32 pl2d_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,    32, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    33, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl2d_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl2d_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl2d_stand_06[] = {
	CG_FRAME(    0,    0,   8,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl2d_stand_07[] = {
	CG_FRAME(    0,    0,   8,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_stand_07 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl2d_dash_00[] = {
	CG_FRAME(    0,    1,   2,           0,    65, 0x80,   0x63,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl2d_dash_00_loop[] = {
	CG_FRAME(    0,    1,   2,           0,    66, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,    67, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,    68, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,    69, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    71, 0x80,   0x4d,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2d_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl2d_dash_01[] = {
	CG_FRAME(    0,    0,   1,           0,    75, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   658, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  10,           0,   659, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   660, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_dash_01 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl2d_dash_02[] = {
	CG_FRAME(    0,    1,   3,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2d_dash_02 ),
};

/* Char_Jump 0 */
const Sint32 pl2d_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    83, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2d_jump_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl2d_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2d_jump_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   102, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   103, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl2d_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2d_jump_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   114, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   115, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_jump_02_loop ),
};

/* Char_Jump 3: appear_move
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl2d_jump_03[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl2d_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    77, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    80, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    81, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    83, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl2d_jump_06_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2d_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl2d_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    94, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    95, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl2d_jump_07_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2d_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl2d_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   107, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   108, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   109, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   110, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl2d_jump_08_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2d_jump_08_loop ),
};

/* Char_Jump 9: chaos_catch_appear (Chaos Dimension), act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl2d_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_jump_09 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl2d_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,   102, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   103, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_jump_10 ),
};

/* Char_Jump 11: act_change_out_recover
   Char_Change 2: act_change_out_bound */
const Sint32 pl2d_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,   114, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_jump_11 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl2d_crouch_00[] = {
	CG_FRAME(    0,    2,   3,           0,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    2,   3,           0,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   3,           0,   118, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl2d_crouch_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    48, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl2d_crouch_02[] = {
	CG_FRAME(    0,    2,   4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl2d_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl2d_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   130, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl2d_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   134, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl2d_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2d_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl2d_guard_02[] = {
	CG_FRAME(    0,    1,   6, END_OF_DATA,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2d_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl2d_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2d_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl2d_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   135, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl2d_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   135, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2d_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl2d_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   136, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2d_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl2d_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   135, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2d_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl2d_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   138, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   140, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl2d_air_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   138, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_air_guard_01 ),
};

/* Char_Air_Guard 5 */
const Sint32 pl2d_air_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   138, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_air_guard_05 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl2d_air_guard_02[] = {
	CG_FRAME(    0,    1,   5,           0,   139, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2d_air_guard_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_air_guard_02_loop ),
};

/* Char_Air_Guard 3 */
const Sint32 pl2d_air_guard_03[] = {
	CG_FRAME(    0,    1,   4,           0,   139, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2d_air_guard_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   102, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   103, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_air_guard_03_loop ),
};

/* Char_Air_Guard 4 */
const Sint32 pl2d_air_guard_04[] = {
	CG_FRAME(    0,    1,   4,           0,   139, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2d_air_guard_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   114, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_air_guard_04_loop ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl2d_stand_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   247, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   250, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl2d_stand_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   251, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,   4,           0,   252, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0xc3,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,  10,           0,   253, 0x80,   0xc3,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x88,              ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x4f,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   256, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl2d_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   262, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATK,  0x00,       0x23,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   1,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   279, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   280, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,  16,           0,   281, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   116, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl2d_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   282, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   284, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3,           0,   284, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   285, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_NEXT( pl2d_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl2d_stand_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   5,           0,   287, 0x80,   0x22,              ATK,  0x00,       0x04,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x22,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   6,           0,   289, 0x80,   0x22,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   6,           0,   290, 0x80,   0x22,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   291, 0x80,   0x2a,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl2d_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   296, 0x80,   0x26,              ATK,  0x01, 0x15 | CSE,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   4,           0,   297, 0x80,   0x26,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   4,           0,   298, 0x80,   0x26,              ATK,  0x01, 0x15 | CSE,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    1,   4,           0,   299, 0x80,   0x26,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    1,   4,           0,   300, 0x80,   0x26,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,   7,           0,   301, 0x80,   0x26,              ATK,  0x01, 0x15 | CSE,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   8,           0,   302, 0x80,   0x26,              ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   8,           0,   303, 0x80,   0x26,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   6,           0,   304, 0x80,   0x26,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   306, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl2d_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   307, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   309, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl2d_crouch_p_01[] = {
	CG_FRAME(    0,    0,   7,           0,   310, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   311, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,   3,           0,   312, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,  10,           0,   312, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   345, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl2d_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   313, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   316, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   317, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   318, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   319, 0x80,   0x20,              ATK,  0x00,       0x23,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   1,           0,   320, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   321, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   322, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   328, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   1,           0,   329, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   1,           0,   330, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   1,           0,   337, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   319, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,  16,           0,   338, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   6,           0,   116, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   117, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl2d_crouch_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   339, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5,           0,   340, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   341, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl2d_crouch_k_01[] = {
	CG_FRAME(    0,    0,   7,           0,   342, 0x80,   0x28,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   6,           0,   343, 0x80,   0x28,              ATK,  0x00,       0x05,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   7,           0,   343, 0x80,   0x28,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   9,           0,   344, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   345, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl2d_crouch_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   347, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   348, 0x80,   0x20,              ATK,  0x00,       0x03,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   348, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   348, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   349, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   350, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   117, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2d_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl2d_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   351, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x81,    0,   8,           0,   352, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   353, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl2d_jump_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   354, 0x80,   0x20,              ATK,  0x00,       0x05,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   2,           0,   355, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x81,    0,   6,           0,   356, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   357, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   358, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl2d_jump_p_02[] = {
	CG_FRAME(    0,    0,   5,           0,   359, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   361, 0x80,   0x20,              ATK,  0x00,       0x23,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   1,           0,   362, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   363, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   1,           0,   364, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1,           0,   365, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   366, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   0,           0,   370, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   372, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,  16,           0,   374, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   353, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl2d_jump_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   358, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   375, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   9,           0,   376, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl2d_jump_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   358, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    1,   2,           0,   383, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    1,   2,           0,   384, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    1,   2,           0,   385, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    1,   2,           0,   383, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x82,    1,   2,           0,   384, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   358, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl2d_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   358, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6,           0,   403, 0x80,   0x20,              ATK,  0x00,       0x03,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    1,   1,           0,   404, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,   8,           0,   405, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   406, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   8,           0,   407, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   358, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_k_02 ),
};

/* Char_Jump_P 5 */
const Sint32 pl2d_jump_p_05[] = {
	CG_FRAME(    0,    0,   5,           0,   387, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   388, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   389, 0x80,   0x20,              ATK,  0x00,       0x23,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   1,           0,   390, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   1,           0,   391, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x80,    0,   1,           0,   392, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   1,           0,   393, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   1,           0,   394, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   1,           0,   400, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,  16,           0,   402, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_jump_p_05 ),
};

/* Char_Jump_K 5 */
const Sint32 pl2d_jump_k_05[] = {
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,       0x03,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   7,           0,   382, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   382, 0x80,   0x20, ATK | CG_ATCK_NG,  0x01,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   378, 0x80,   0x60, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0x80,   0x60, ATE | CG_ATCK_NG | 0x19,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x60, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,  12,           0,   378, 0x80,   0x60, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    1,   4,           0,   378, 0x80,   0x60, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    2,   4,           0,   380, 0x80,   0x60, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    3,   4,           0,   380, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl2d_jump_k_05_loop[] = {
	CG_FRAME(    0,    4,   4, END_OF_DATA,   381, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_jump_k_05_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl2d_damage_00[] = {
	CG_FRAME(    0,    0,   8,           0,   547, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   548, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2d_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl2d_damage_01[] = {
	CG_FRAME(    0,    0,  10,           0,   549, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2d_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl2d_damage_02[] = {
	CG_FRAME(    0,    0,   3,           0,   550, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   551, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2d_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl2d_damage_03[] = {
	CG_FRAME(    0,    0,   8,           0,   552, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   122, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2d_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl2d_damage_04[] = {
	CG_FRAME(    0,    0,  10,           0,   553, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   553, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2d_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl2d_damage_05[] = {
	CG_FRAME(    0,    0,   3,           0,   554, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   555, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl2d_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl2d_damage_06[] = {
	CG_FRAME(    0,    0,   2,           0,   556, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0,   557, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0,   558, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   131, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl2d_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl2d_damage_07[] = {
	CG_FRAME(    0,    0,   5,           0,   559, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   5,           0,   557, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   131, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl2d_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl2d_damage_08[] = {
	CG_FRAME(    0,    0,   3,           0,   560, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0,   561, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0,   562, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0,   563, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   564, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   565, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   131, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl2d_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl2d_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   566, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl2d_damage_11[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   567, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl2d_damage_12[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   568, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl2d_damage_13[] = {
	CG_FRAME(    0,    0,   3,           0,   569, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl2d_damage_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   192, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl2d_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl2d_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   570, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   572, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   581, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   582, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl2d_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   584, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl2d_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl2d_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   575, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   576, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl2d_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   577, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl2d_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   578, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl2d_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   579, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl2d_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   580, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl2d_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   575, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   576, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Damage 22 */
const Sint32 pl2d_damage_22[] = {
	CG_FRAME(    0,    0,   8,           0,   577, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Damage 23 */
const Sint32 pl2d_damage_23[] = {
	CG_FRAME(    0,    0,   8,           0,   578, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Damage 24 */
const Sint32 pl2d_damage_24[] = {
	CG_FRAME(    0,    0,   8,           0,   579, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Damage 25 */
const Sint32 pl2d_damage_25[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   580, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl2d_damage_26[] = {
	CG_FRAME(    0,    0,   4,           0,   581, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   3,           0,   582, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0,   584, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   585, 0x40,   0xf1,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl2d_damage_26 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl2d_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   590, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   591, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   592, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   593, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   594, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   595, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   596, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   597, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_28 ),
};

/* Char_Damage 29: hold_power_start, dm_away_fly, dm_bound_hop, dm_bound, ... */
const Sint32 pl2d_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   598, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   600, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   601, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   602, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   603, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   605, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2d_damage_29 ),
};

/* Char_Damage 27 */
const Sint32 pl2d_damage_27[] = {
	CG_FRAME(    0,    0,   1,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl2d_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   615, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   618, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   113, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_damage_30 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl2d_damage_33[] = {
	CG_FRAME(    0,    0,   3,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   192, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_damage_33 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl2d_damage_31[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   585, 0x60,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl2d_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl2d_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   606, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   607, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   608, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   609, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   6,           0,   610, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   611, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   612, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   613, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   614, 0x50,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_dizzy_00 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl2d_damage_34[] = {
	CG_FRAME(    0,    0,   6,           0,   615, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6,           0,   616, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6,           0,   617, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   618, 0x40,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl2d_damage_34 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl2d_catch_00[] = {
	CG_FRAME(    0,    0,   2,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,  15,           0,   427, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   5,           0,   428, 0x80,   0x4a,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   5,           0,   429, 0x80,   0x5a,                0,  0x00,       0x05,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    1,  13,           0,   430, 0x80,   0x9b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   431, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   9,           0,   432, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   433, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   434, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_catch_00 ),
};

/* Char_Catch 2: k_set */
const Sint32 pl2d_catch_02[] = {
	CG_FRAME(    0,    0,   7,           0,   435, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   1,           0,   435, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    1,   1,           0,   436, 0xb0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
};

const Sint32 pl2d_catch_02_loop[] = {
	CG_FRAME(    0,    1,   6,           0,   436, 0xb0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   6,           0,   437, 0xb0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   6,           0,   438, 0xb0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   439, 0xb0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_NEXT( pl2d_catch_02_loop ),
};

/* Char_Catch 10: k_catch_end */
const Sint32 pl2d_catch_10[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_catch_10 ),
};

/* Char_Catch 11: k_catch_move */
const Sint32 pl2d_catch_11[] = {
	CG_FRAME(    0,    0,   4,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x90,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2d_catch_11 ),
};

/* Char_Catch 1: jump_p_set */
const Sint32 pl2d_catch_01[] = {
	CG_FRAME(    0,    0,   1,           0,   375, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   5,           0,   375, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   5,           0,   473, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   5,           0,   474, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   5,           0,   475, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   5,           0,   476, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   5,           0,   477, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   5,           0,   478, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    1,   5,           0,   479, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   375, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_catch_01 ),
};

/* Char_Caught 0 */
const Sint32 pl2d_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   547, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl2d_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl2d_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   549, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl2d_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl2d_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   550, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl2d_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl2d_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   552, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl2d_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl2d_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   553, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl2d_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl2d_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   554, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl2d_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl2d_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   556, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl2d_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl2d_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   559, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl2d_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl2d_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   560, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2d_caught_08 ),
};

/* Char_Caught 9
   Char_Caught 21 */
const Sint32 pl2d_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   566, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl2d_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl2d_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   567, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2d_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl2d_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   568, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl2d_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl2d_caught_12[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   569, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2d_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl2d_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   575, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_13 ),
};

/* Char_Caught 18 */
const Sint32 pl2d_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   576, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_18 ),
};

/* Char_Caught 14 */
const Sint32 pl2d_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   577, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl2d_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   578, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl2d_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   579, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl2d_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   580, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_17 ),
};

const Sint32 pl2d_unused_00[] = {	/* nothing reaches it */
	0x00040000,
};

const Sint32 pl2d_unused_01[] = {
	0x0000024e,
};

const Sint32 pl2d_unused_02[] = {
	0x00000040,
};

const Sint32 pl2d_unused_03[] = {
	0x00000000,
};

const Sint32 pl2d_unused_04[] = {
	0x004d0000,
};

const Sint32 pl2d_unused_05[] = {
	0x00003258,
};

/* Char_Caught 19 */
const Sint32 pl2d_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   598, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2d_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl2d_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   585, 0x60,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2d_damage_31 ),
};

/* Char_Get_Up 1 */
const Sint32 pl2d_get_up_01[] = {
	CG_FRAME(    0,    0,   1,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   192, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   193, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2d_get_up_01_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_get_up_01_loop ),
};

/* Char_Get_Up 2 */
const Sint32 pl2d_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   192, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2d_get_up_02_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_get_up_02_loop ),
};

/* Char_Get_Up 3 */
const Sint32 pl2d_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   192, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_get_up_03 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl2d_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   192, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x40,   0xe0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x40,   0xc0,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   8,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_get_up_00 ),
};

/* Char_Win 2: win_init */
const Sint32 pl2d_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   197, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x40,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2d_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   193, 0x40,   0x80,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_win_02_loop ),
};

/* Char_Extra 0: chouhatsu_init (Taunt) */
const Sint32 pl2d_extra_00[] = {
	CG_FRAME(    0,    0,   1,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  20,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  34, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_extra_00 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl2d_change_04[] = {
	CG_FRAME(    0,    0,   1,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_change_04 ),
};

/* Char_Win 0 */
const Sint32 pl2d_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   142, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  40,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2d_win_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl2d_win_01[] = {
	CG_FRAME(    0,    0,   5,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   126, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   5,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  46,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2d_win_01_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_win_01_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl2d_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   243, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2d_change_03_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_change_03_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl2d_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2d_change_00_loop[] = {
	CG_FRAME(    0,    1,  14, END_OF_DATA,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_change_00_loop ),
};

/* Char_Extra 1 */
const Sint32 pl2d_extra_01[] = {
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   220, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   223, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   224, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_extra_01 ),
};

/* Char_Extra 2: pl2d_upper, fly_start5 */
const Sint32 pl2d_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   5,           0,   287, 0x80,   0x22,              ATK,  0x00,       0x04,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   5,           0,   288, 0x80,   0x22,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   6,           0,   289, 0x80,   0x22,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   6,           0,   290, 0x80,   0x22,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   5,           0,   291, 0x80,   0x2a,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_extra_02 ),
};

/* Char_Appear 0: appear_wait, chaos_catch_appear (Chaos Dimension) */
const Sint32 pl2d_appear_00[] = {
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   162, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   170, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   171, 0x80,   0xff,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   172, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   173, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   174, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_appear_00 ),
};

const Sint32 pl2d_unused_06[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   528, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   529, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   530, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   531, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   532, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   532, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   533, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003dc8,
};

const Sint32 pl2d_unused_07[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   531, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   530, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   529, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   528, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003df4,
};

/* Char_Special 0 */
const Sint32 pl2d_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   460, 0xb0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   6,           0,   461, 0xb0,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   6,           0,   462, 0xb0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   4,           0,   463, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   8,           0,   464, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   5,           0,   465, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   7,           0,   466, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   660, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_special_00 ),
};

/* Char_Special 1
   Char_Special 2 */
const Sint32 pl2d_special_01[] = {
	CG_FRAME(    0,    0,   6,           0,   460, 0xb0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    1,   6,           0,   461, 0xb0,   0x00,              ATK,  0x00,       0x14,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   6,           0,   462, 0xb0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   4,           0,   463, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   9,           0,   464, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   465, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   8,           0,   466, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   660, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_special_01 ),
};

/* Char_Special 3: smash_init (Mystic Smash, 3 + smash_type) */
const Sint32 pl2d_special_03[] = {
	CG_FRAME(    0,    0,   1,           0,   448, 0xb0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0,   449, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   454, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_03 ),
};

/* Char_Special 4 */
const Sint32 pl2d_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   448, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   449, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2d_special_04 ),
};

/* Char_Special 5: smash_homing (Mystic Smash) */
const Sint32 pl2d_special_05[] = {
	CG_FRAME(    0,    0,   5,           0,   458, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   459, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     1,      0,  87 ),
	CG_NEXT( pl2d_special_05 ),
};

/* Char_Special 6: smash_start (Mystic Smash) */
const Sint32 pl2d_special_06[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   454, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_10 ),
};

/* Char_Special 7: smash_fly (Mystic Smash) */
const Sint32 pl2d_special_07[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   455, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_10 ),
};

/* Char_Special 8 */
const Sint32 pl2d_special_08[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   457, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_10 ),
};

/* Char_Special 9: smash_fly (Mystic Smash), smash_fall (Mystic Smash), smash_homing (Mystic Smash) */
const Sint32 pl2d_special_09[] = {
	CG_FRAME(    0,    0,   4,           0,   450, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0,   451, 0xb0,   0x20,              ATK,  0x01,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0,   452, 0xb0,   0x20,              ATK,  0x02,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   453, 0xb0,   0x20,              ATK,  0x03,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl2d_special_10[] = {
	CG_FRAME(    0,    0,   4,           0,   450, 0xb0,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0,   451, 0xb0,   0x20,              ATE,  0x01,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0,   452, 0xb0,   0x20,              ATE,  0x02,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   453, 0xb0,   0x20,              ATE,  0x03,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_10 ),
};

/* Char_Special 11: smash_fly (Mystic Smash), smash_fall (Mystic Smash) */
const Sint32 pl2d_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   454, 0xb0,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   4,           0,   450, 0xb0,   0x20,              ATE,  0x01,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   4,           0,   451, 0xb0,   0x20,              ATE,  0x02,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    2,   4,           0,   449, 0xb0,   0x20,              ATE,  0x03,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4,           0,   448, 0xb0,   0x00,              ATE,  0x04,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,              ATE,  0x05,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_special_11 ),
};

const Sint32 pl2d_unused_08[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   2, END_OF_DATA,   454, 0xb0,   0x20,              ATK,  0x01,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_special_10 ),
};

/* Char_Special 12: devital_init (Devitalization) */
const Sint32 pl2d_special_12[] = {
	CG_FRAME(    0,    0,   6,           0,    74, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   408, 0xb0,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2d_special_12 ),
};

/* Char_Catch 8
   Char_Special 13: devital_catch (Devitalization) */
const Sint32 pl2d_catch_08[] = {
	CG_FRAME(    0,    0,   1,           0,   408, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,  12,           0,   408, 0x80,   0x0b,                0,  0x00,       0x16,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    1,   4,           0,   411, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
};

const Sint32 pl2d_catch_08_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   412, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   413, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_NEXT( pl2d_catch_08_loop ),
};

/* Char_Catch 9
   Char_Special 14: devital_rise (Devitalization) */
const Sint32 pl2d_catch_09[] = {
	CG_FRAME(    0,    0,   8,           0,   414, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    1,  12, END_OF_DATA,   416, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_catch_09 ),
};

/* Char_Special 15: devital_throw (Devitalization) */
const Sint32 pl2d_special_15[] = {
	CG_FRAME(    0,    1,   1,           0,   417, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   418, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl2d_special_15 ),
};

/* Char_Special 16: devital_fall (Devitalization) */
const Sint32 pl2d_special_16[] = {
	CG_FRAME(    0,    0,   3,           0,   285, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  21, END_OF_DATA,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_special_16 ),
};

/* Char_Special 17: devital_catch (Devitalization) */
const Sint32 pl2d_special_17[] = {
	CG_FRAME(    0,    0,   7,           0,   408, 0xb0,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   8,           0,    73, 0xb0,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    74, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_special_17 ),
};

/* Char_Super 0: chaos_init (Chaos Dimension)
   Char_Super 4 */
const Sint32 pl2d_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   251, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,   4,           0,   251, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    2,  20,           0,   252, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   255, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl2d_super_00 ),
};

/* Char_Super 1 */
const Sint32 pl2d_super_01[] = {
	CG_FRAME(    0,    0,   5,           0,   507, 0xc0,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   508, 0xc0,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4,           0,   509, 0xc0,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   512, 0xc0,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   513, 0xc0,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl2d_super_02[] = {
	CG_FRAME(    0,    0,   5,           0,   339, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   4,           0,   515, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   341, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl2d_super_02 ),
};

/* Char_Super 3: power_jump_atck */
const Sint32 pl2d_super_03[] = {
	CG_FRAME(    0,    0,   5,           0,   443, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    1,   4,           0,   518, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   8,           0,   519, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,   7, END_OF_DATA,   358, 0xc0,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl2d_super_03 ),
};

/* Char_Catch 3: power_catch (3 + pl_stance) */
const Sint32 pl2d_catch_03[] = {
	CG_FRAME(    0,    0,  12,           0,   509, 0xc0,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    0,   4,           0,   510, 0xc0,   0x0d,                0,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   511, 0xc0,   0x01,                0,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   480, 0xc0,   0x20,                0,  0x00,       0x21,    0,   0,     0, 0x0024,   0 ),
	CG_NEXT( pl2d_catch_06 ),
};

/* Char_Catch 4 */
const Sint32 pl2d_catch_04[] = {
	CG_FRAME(    0,    0,  12,           0,   515, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x001e,  23 ),
	CG_FRAME(    0,    0,   4,           0,   516, 0xc0,   0x2d,                0,  0x00,          0,    0,   0,     0, 0x001f,  24 ),
	CG_FRAME(    0,    0,   4,           0,   517, 0xc0,   0x21,                0,  0x00,          0,    0,   0,     0, 0x0020,  25 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   480, 0xc0,   0x20,                0,  0x00,       0x21,    0,   0,     0, 0x0024,   0 ),
	CG_NEXT( pl2d_catch_06 ),
};

/* Char_Catch 5 */
const Sint32 pl2d_catch_05[] = {
	CG_FRAME(    0,    0,  12,           0,   519, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0021,   0 ),
	CG_FRAME(    0,    0,   4,           0,   520, 0xc0,   0x2d,                0,  0x00,          0,    0,   0,     0, 0x0022,   0 ),
	CG_FRAME(    0,    0,   4,           0,   521, 0xc0,   0x21,                0,  0x00,          0,    0,   0,     0, 0x0023,   0 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   480, 0xc0,   0x20,                0,  0x00,       0x21,    0,   0,     0, 0x0024,   0 ),
	CG_NEXT( pl2d_catch_06 ),
};

/* Char_Catch 6 */
const Sint32 pl2d_catch_06[] = {
	CG_FRAME(    0,    0,   1,           0,   480, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0025,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   481, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0025,   0 ),
	CG_NEXT( pl2d_catch_06 ),
};

/* Char_Catch 7: chaos_catch_up (Chaos Dimension) */
const Sint32 pl2d_catch_07[] = {
	CG_FRAME(    0,    0,   1,           0,   482, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0027,   0 ),
	CG_FRAME(    0,    0,   8,           0,   482, 0xc0,   0x20,                0,  0x00,       0x17,    0,   0,     0, 0x0027,   0 ),
	CG_FRAME(    0,    0,   8,           0,   483, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x0027,   0 ),
	CG_FRAME(    0,    0,   7,           0,   484, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
	CG_FRAME(    0,    1,   6,           0,   485, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x002a,   0 ),
	CG_FRAME(    0,    0,   5,           0,   486, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x002c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0xc0,   0x20,                0,  0x00,          0,    0,   0,     0, 0x002d,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32780, 0xc0,   0x20,                0,  0x00,       0x22,    0,   0,     0, 0x002e,   0 ),
	CG_NEXT( pl2d_catch_07 ),
};

/* Char_Super 5
   Char_Super 6 */
const Sint32 pl2d_super_05[] = {
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   8,           0,   467, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   468, 0xc0,   0x1b,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   470, 0xc0,   0x1e,              ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0xc0,   0x1e,              ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    4,   4,           0,   125, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4,           0,    36, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    32, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2d_super_05 ),
};

/* Char_Super 7: hsmash_init (Hyper Mystic Smash) */
const Sint32 pl2d_super_07[] = {
	CG_FRAME(    0,    0,   6,           0,   124, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   6,           0,   148, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   1,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
};

const Sint32 pl2d_super_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   467, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   468, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_super_07_loop ),
};

/* Char_Super 8 */
const Sint32 pl2d_super_08[] = {
	CG_FRAME(    0,    0,   4,           0, 32774, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   4,           0, 32770, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,   4,           0, 32771, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    2,   4,           0, 32769, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4,           0, 32768, 0xb0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_super_08 ),
};

/* Char_Super 9: hsmash_move (Hyper Mystic Smash) */
const Sint32 pl2d_super_09[] = {
	CG_FRAME(    0,    0,   1,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  24, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_super_09 ),
};

/* Char_Special 18: t_kick_init */
const Sint32 pl2d_special_18[] = {
	CG_FRAME( 0x80,    1,   2,           0,   383, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    1,   2,           0,   384, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    1,   2,           0,   385, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    1,   2,           0,   383, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   1,           0,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x82,    1,   2,           0,   384, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   386, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl2d_special_18 ),
};

/* Char_Special 19: t_kick_jump */
const Sint32 pl2d_special_19[] = {
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  13,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2d_special_19_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2d_special_19_loop ),
};

const Sint32 pl2d_unused_09[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   172, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   173, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   174, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00004b60,
};

/* Char_Special 20: snapback_init (Snapback) */
const Sint32 pl2d_special_20[] = {
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,   260, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x23,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   1,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   279, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   0,           0,   280, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,  16,           0,   281, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   116, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2d_special_20 ),
};

/* Char_Effect 0: pls2d_00_t0_init */
const Sint32 pl2d_effect_00[] = {
	CG_FRAME(    0,    0,   5,           0,   653, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   654, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   655, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   655, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   654, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   656, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   657, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   657, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_00 ),
};

/* Char_Effect 1: pls2d_00_t1_init */
const Sint32 pl2d_effect_01[] = {
	CG_FRAME(    0,    1,   4,           0,   175, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   176, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   178, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   180, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   181, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   182, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   191, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_01 ),
};

/* Char_Effect 2: pls2d_00_t2_init */
const Sint32 pl2d_effect_02[] = {
	CG_FRAME(    0,    0,   8,           0,   441, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2d_effect_02_loop[] = {
	CG_FRAME(    0,    1,   6,           0,   440, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   441, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   442, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   441, 0xb0,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_02_loop ),
};

/* Char_Effect 3: shl2d_00_init */
const Sint32 pl2d_effect_03[] = {
	CG_FRAME(    0,    9,   1,           0,   534, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   1,           0,   535, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   1,           0,   536, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   1,           0,   537, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   1,           0,   538, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   1,           0,   539, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   1,           0,   540, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   1,           0,   541, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   542, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2d_effect_03_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   652, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl2d_effect_03_loop ),
};

/* Char_Effect 4: shl2d_00_stick */
const Sint32 pl2d_effect_04[] = {
	CG_FRAME(    0,    1,   1,           0,   619, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   620, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   621, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   622, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   623, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   624, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   625, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   626, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   627, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   628, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   629, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   630, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   631, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   632, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   633, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   634, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   635, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   635, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   636, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   637, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   638, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   639, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   640, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   641, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   642, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   643, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   644, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   645, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   646, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   647, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   648, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   649, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1,           0,   650, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   651, 0xc0,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl2d_effect_04 ),
};

/* Char_Effect 5: shl2d_00_appear, shl2d_00_fly, shl2d_00_keep, shl2d_00_atck */
const Sint32 pl2d_effect_05[] = {
	CG_FRAME(    0,    0,   2,           0,   652, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   542, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   541, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   538, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   537, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   535, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   534, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_05 ),
};

/* Char_Effect 6: pls2d_00_t3_init */
const Sint32 pl2d_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0,   409, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2d_effect_06_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   410, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_06_loop ),
};

/* Char_Effect 7: pls2d_00_t4_init */
const Sint32 pl2d_effect_07[] = {
	CG_FRAME(    0,    0,   4,           0,   426, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   424, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   423, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   422, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   421, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   420, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   419, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_07 ),
};

/* Char_Effect 8: pls2d_00_t5_init */
const Sint32 pl2d_effect_08[] = {
	CG_FRAME(    0,    0,   2,           0,   522, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   523, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_08 ),
};

/* Char_Effect 9: pls2d_00_t6_init */
const Sint32 pl2d_effect_09[] = {
	CG_FRAME(    0,    0,   6,           0,   488, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   489, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32781, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl2d_effect_10[] = {
	CG_FRAME(    0,    6,   4,           0, 32782, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32783, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   4, END_OF_DATA, 32796, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl2d_effect_11[] = {
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   4,           0, 32796, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32782, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32783, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl2d_effect_12[] = {
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl2d_effect_13[] = {
	CG_FRAME(    0,    0,   4,           0, 32788, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4, END_OF_DATA, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl2d_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    2,   4, END_OF_DATA, 32783, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl2d_effect_15[] = {
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    4,   4,           0, 32783, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4, END_OF_DATA, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl2d_effect_16[] = {
	CG_FRAME(    0,    4,   4,           0, 32783, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    6,   4, END_OF_DATA, 32782, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl2d_effect_17[] = {
	CG_FRAME(    0,    6,   4,           0, 32786, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32787, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    6,   4,           0, 32782, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0, 32783, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4, END_OF_DATA, 32785, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl2d_effect_18[] = {
	CG_FRAME(    0,    8,   4,           0, 32789, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32790, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4, END_OF_DATA, 32798, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl2d_effect_19[] = {
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32789, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32790, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4, END_OF_DATA, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl2d_effect_20[] = {
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32795, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4, END_OF_DATA, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl2d_effect_21[] = {
	CG_FRAME(    0,    8,   4,           0, 32795, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4, END_OF_DATA, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl2d_effect_22[] = {
	CG_FRAME(    0,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32795, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32795, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4, END_OF_DATA, 32790, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl2d_effect_23[] = {
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32795, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32790, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32790, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4, END_OF_DATA, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl2d_effect_24[] = {
	CG_FRAME(    0,    8,   4,           0, 32790, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4, END_OF_DATA, 32789, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl2d_effect_25[] = {
	CG_FRAME(    0,    8,   4,           0, 32793, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32794, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32789, 0xc0,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    8,   4,           0, 32789, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32790, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4,           0, 32791, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   4, END_OF_DATA, 32792, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_25 ),
};

/* Char_Effect 31 */
const Sint32 pl2d_effect_31[] = {
	CG_FRAME(    0,    0,   4,           0,   469, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   471, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   126, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4,           0,    32, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
};

const Sint32 pl2d_effect_31_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    32, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2d_effect_31_loop ),
};

/* Char_Effect 26: shl2d_01_init */
const Sint32 pl2d_effect_26[] = {
	CG_FRAME(    0,    0,   1,           0, 32768, 0xb0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0, 32769, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32774, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_effect_26 ),
};

/* Char_Effect 27: shl2d_01_body_ck */
const Sint32 pl2d_effect_27[] = {
	CG_FRAME(    0,    0,   5,           0, 32778, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA, 32779, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     1,      0,  87 ),
	CG_NEXT( pl2d_effect_27 ),
};

/* Char_Effect 28: shl2d_01_start, shl2d_01_land, shl2d_01_body_ck */
const Sint32 pl2d_effect_28[] = {
	CG_FRAME(    0,    0,   4,           0, 32770, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0, 32771, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32773, 0xb0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_effect_28 ),
};

/* Char_Effect 29: shl2d_01_fall */
const Sint32 pl2d_effect_29[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32774, 0xb0,   0x20,              ATK,  0x01,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2d_effect_29 ),
};

/* Char_Effect 30: shl2d_01_fall, shl2d_01_land */
const Sint32 pl2d_effect_30[] = {
	CG_FRAME(    0,    0,   2,           0,   542, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   541, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   539, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   538, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   537, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   535, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   534, 0xc0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2d_effect_30 ),
};

const Sint32 pl2d_unused_10[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   1,           0,   203, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   215, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   218, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000609c,
};

const Sint32 pl2d_unused_11[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   1,           0,   225, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   226, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   227, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   232, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   239, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000060b4,
};
