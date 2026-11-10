/* Juggernaut (character 29): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_29      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_29     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl29.c) says which group
                   and number each move uses.
   pl29_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl29_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl29_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl29_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_29
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_29, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_29 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl29_stand_00[], pl29_stand_02[], pl29_stand_03[], pl29_stand_06[],
	pl29_stand_07[], pl29_jump_00[], pl29_jump_00_loop[], pl29_jump_01[], pl29_jump_01_loop[],
	pl29_jump_02[], pl29_jump_02_loop[], pl29_jump_03[], pl29_jump_09[], pl29_jump_10[],
	pl29_jump_11[], pl29_jump_06[], pl29_damage_30_loop[], pl29_jump_07[], pl29_jump_07_loop[],
	pl29_jump_08[], pl29_jump_08_loop[], pl29_dash_00[], pl29_dash_01[], pl29_dash_02[],
	pl29_dash_03[], pl29_crouch_00[], pl29_crouch_01[], pl29_crouch_02[], pl29_turn_00[],
	pl29_turn_01[], pl29_guard_00[], pl29_guard_01[], pl29_guard_02[], pl29_guard_03[],
	pl29_guard_04[], pl29_guard_05[], pl29_guard_06[], pl29_guard_07[], pl29_air_guard_00[],
	pl29_air_guard_01[], pl29_air_guard_04[], pl29_air_guard_04_loop[], pl29_air_guard_05[],
	pl29_air_guard_05_loop[], pl29_air_guard_06[], pl29_air_guard_06_loop[], pl29_stand_p_00[],
	pl29_extra_02[], pl29_stand_p_01[], pl29_stand_p_02[], pl29_stand_k_00[], pl29_unused_00[],
	pl29_stand_k_01[], pl29_stand_k_02[], pl29_crouch_p_00[], pl29_crouch_p_01[], pl29_crouch_p_02[],
	pl29_crouch_k_00[], pl29_crouch_k_01[], pl29_crouch_k_02[], pl29_jump_p_00[], pl29_jump_p_01[],
	pl29_jump_p_02[], pl29_jump_k_00[], pl29_jump_k_01[], pl29_jump_k_02[], pl29_catch_00[],
	pl29_catch_01[], pl29_catch_02[], pl29_catch_02_loop[], pl29_special_04[],
	pl29_special_04_loop[], pl29_unused_01[], pl29_extra_03[], pl29_effect_02[], pl29_special_00[],
	pl29_special_00_loop[], pl29_special_01[], pl29_special_01_loop[], pl29_special_03[],
	pl29_special_05[], pl29_special_05_loop[], pl29_special_08[], pl29_special_06[],
	pl29_special_06_loop[], pl29_special_10[], pl29_special_27[], pl29_special_27_loop[],
	pl29_special_28[], pl29_effect_03[], pl29_effect_04[], pl29_super_00[], pl29_super_00_loop[],
	pl29_super_01[], pl29_effect_01[], pl29_extra_04[], pl29_special_18[], pl29_special_21[],
	pl29_special_24[], pl29_special_19[], pl29_special_22[], pl29_special_25[], pl29_special_20[],
	pl29_special_23[], pl29_special_26[], pl29_special_29[], pl29_special_30[],
	pl29_special_30_loop[], pl29_special_31[], pl29_special_31_loop[], pl29_damage_00[],
	pl29_caught_00[], pl29_damage_01[], pl29_caught_01[], pl29_damage_02[], pl29_caught_02[],
	pl29_damage_03[], pl29_caught_03[], pl29_damage_04[], pl29_caught_04[], pl29_damage_05[],
	pl29_caught_05[], pl29_damage_06[], pl29_caught_06[], pl29_damage_07[], pl29_caught_07[],
	pl29_damage_08[], pl29_caught_08[], pl29_damage_10[], pl29_caught_09[], pl29_damage_11[],
	pl29_caught_10[], pl29_damage_12[], pl29_caught_11[], pl29_damage_13[], pl29_caught_12[],
	pl29_damage_09[], pl29_damage_09_loop[], pl29_damage_16[], pl29_caught_13[], pl29_caught_18[],
	pl29_damage_17[], pl29_caught_14[], pl29_damage_18[], pl29_caught_15[], pl29_damage_19[],
	pl29_caught_16[], pl29_damage_20[], pl29_caught_17[], pl29_damage_21[], pl29_damage_22[],
	pl29_damage_23[], pl29_damage_24[], pl29_damage_25[], pl29_damage_26[], pl29_damage_27[],
	pl29_damage_30[], pl29_damage_28[], pl29_caught_19[], pl29_damage_29[], pl29_dizzy_00[],
	pl29_special_11[], pl29_special_12[], pl29_win_00[], pl29_win_00_loop[], pl29_win_06[],
	pl29_win_06_loop[], pl29_win_07[], pl29_win_07_loop[], pl29_win_04[], pl29_win_04_loop[],
	pl29_effect_08[], pl29_win_02[], pl29_win_02_loop[], pl29_appear_00[], pl29_appear_00_loop[],
	pl29_get_up_00[], pl29_get_up_02[], pl29_get_up_01[], pl29_get_up_03[], pl29_effect_00[],
	pl29_super_02[], pl29_damage_31[], pl29_caught_20[], pl29_effect_05[], pl29_change_01[],
	pl29_extra_00[], pl29_extra_00_loop[], pl29_extra_01[], pl29_extra_01_loop[], pl29_change_00[],
	pl29_change_00_loop[], pl29_change_02[], pl29_change_02_loop[], pl29_change_03[],
	pl29_change_03_loop[], pl29_change_04[], pl29_change_04_loop[], pl29_extra_06[],
	pl29_caught_23[], pl29_unused_02[];
extern const Sint32 *const pl29_stand_tbl[], *const pl29_jump_tbl[], *const pl29_dash_tbl[],
	*const pl29_crouch_tbl[], *const pl29_turn_tbl[], *const pl29_guard_tbl[],
	*const pl29_air_guard_tbl[], *const pl29_stand_p_tbl[], *const pl29_stand_k_tbl[],
	*const pl29_damage_tbl[], *const pl29_dizzy_tbl[], *const pl29_catch_tbl[],
	*const pl29_caught_tbl[], *const pl29_crouch_p_tbl[], *const pl29_crouch_k_tbl[],
	*const pl29_jump_p_tbl[], *const pl29_jump_k_tbl[], *const pl29_get_up_tbl[],
	*const pl29_appear_tbl[], *const pl29_win_tbl[], *const pl29_extra_tbl[],
	*const pl29_special_tbl[], *const pl29_super_tbl[], *const pl29_effect_tbl[],
	*const pl29_change_tbl[], *const pl29_grp25_tbl[], *const pl29_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_29[16] = {
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
const Sint32 *const *const char_tbl_29[] = {
	pl29_stand_tbl, pl29_jump_tbl, pl29_dash_tbl, pl29_crouch_tbl, pl29_turn_tbl, pl29_guard_tbl,
	pl29_air_guard_tbl, pl29_stand_p_tbl, pl29_stand_k_tbl, pl29_crouch_p_tbl, pl29_crouch_k_tbl, pl29_jump_p_tbl,
	pl29_jump_k_tbl, pl29_damage_tbl, pl29_dizzy_tbl, pl29_catch_tbl, pl29_caught_tbl, pl29_get_up_tbl,
	pl29_appear_tbl, pl29_win_tbl, pl29_extra_tbl, pl29_special_tbl, pl29_super_tbl, pl29_effect_tbl,
	pl29_change_tbl, pl29_grp25_tbl, pl29_grp25_tbl, pl29_grp27_tbl, 0,
};

const Sint32 *const pl29_stand_tbl[] = {
	pl29_stand_00, pl29_stand_00, pl29_stand_02, pl29_stand_03, pl29_stand_00, pl29_stand_00,
	pl29_stand_06, pl29_stand_07, 0,
};

const Sint32 *const pl29_jump_tbl[] = {
	pl29_jump_00, pl29_jump_01, pl29_jump_02, pl29_jump_03, pl29_jump_03, pl29_jump_03,
	pl29_jump_06, pl29_jump_07, pl29_jump_08, pl29_jump_09, pl29_jump_10, pl29_jump_11,
	0,
};

const Sint32 *const pl29_dash_tbl[] = {
	pl29_dash_00, pl29_dash_01, pl29_dash_02, pl29_dash_03, 0,
};

const Sint32 *const pl29_crouch_tbl[] = {
	pl29_crouch_00, pl29_crouch_01, pl29_crouch_02, 0,
};

const Sint32 *const pl29_turn_tbl[] = {
	pl29_turn_00, pl29_turn_01, 0,
};

const Sint32 *const pl29_guard_tbl[] = {
	pl29_guard_00, pl29_guard_01, pl29_guard_02, pl29_guard_03, pl29_guard_04, pl29_guard_05,
	pl29_guard_06, pl29_guard_07, 0,
};

const Sint32 *const pl29_air_guard_tbl[] = {
	pl29_air_guard_00, pl29_air_guard_01, pl29_air_guard_01, 0, pl29_air_guard_04, pl29_air_guard_05,
	pl29_air_guard_06, 0,
};

const Sint32 *const pl29_stand_p_tbl[] = {
	pl29_stand_p_00, pl29_stand_p_01, pl29_stand_p_02, 0,
};

const Sint32 *const pl29_stand_k_tbl[] = {
	pl29_stand_k_00, pl29_stand_k_01, pl29_stand_k_02, 0,
};

const Sint32 *const pl29_damage_tbl[] = {
	pl29_damage_00, pl29_damage_01, pl29_damage_02, pl29_damage_03, pl29_damage_04, pl29_damage_05,
	pl29_damage_06, pl29_damage_07, pl29_damage_08, pl29_damage_09, pl29_damage_10, pl29_damage_11,
	pl29_damage_12, pl29_damage_13, pl29_damage_13, pl29_damage_13, pl29_damage_16, pl29_damage_17,
	pl29_damage_18, pl29_damage_19, pl29_damage_20, pl29_damage_21, pl29_damage_22, pl29_damage_23,
	pl29_damage_24, pl29_damage_25, pl29_damage_26, pl29_damage_27, pl29_damage_28, pl29_damage_29,
	pl29_damage_30, pl29_damage_31, pl29_damage_27, pl29_damage_27, pl29_damage_13, 0,
};

const Sint32 *const pl29_dizzy_tbl[] = {
	pl29_dizzy_00, pl29_stand_00, 0,
};

const Sint32 *const pl29_catch_tbl[] = {
	pl29_catch_00, pl29_catch_01, pl29_catch_02, 0,
};

const Sint32 *const pl29_caught_tbl[] = {
	pl29_caught_00, pl29_caught_01, pl29_caught_02, pl29_caught_03, pl29_caught_04, pl29_caught_05,
	pl29_caught_06, pl29_caught_07, pl29_caught_08, pl29_caught_09, pl29_caught_10, pl29_caught_11,
	pl29_caught_12, pl29_caught_13, pl29_caught_14, pl29_caught_15, pl29_caught_16, pl29_caught_17,
	pl29_caught_18, pl29_caught_19, pl29_caught_20, pl29_caught_11, 0, pl29_caught_23,
	pl29_caught_23, pl29_caught_23, pl29_caught_23, pl29_caught_23, pl29_caught_23, 0,
};

const Sint32 *const pl29_crouch_p_tbl[] = {
	pl29_crouch_p_00, pl29_crouch_p_01, pl29_crouch_p_02, 0,
};

const Sint32 *const pl29_crouch_k_tbl[] = {
	pl29_crouch_k_00, pl29_crouch_k_01, pl29_crouch_k_02, 0,
};

const Sint32 *const pl29_jump_p_tbl[] = {
	pl29_jump_p_00, pl29_jump_p_01, pl29_jump_p_02, pl29_jump_p_00, pl29_jump_p_01, pl29_jump_p_02,
	0,
};

const Sint32 *const pl29_jump_k_tbl[] = {
	pl29_jump_k_00, pl29_jump_k_01, pl29_jump_k_02, pl29_jump_k_00, pl29_jump_k_01, pl29_jump_k_02,
	0,
};

const Sint32 *const pl29_get_up_tbl[] = {
	pl29_get_up_00, pl29_get_up_01, pl29_get_up_02, pl29_get_up_03, 0,
};

const Sint32 *const pl29_appear_tbl[] = {
	pl29_appear_00, pl29_appear_00, 0,
};

const Sint32 *const pl29_win_tbl[] = {
	pl29_win_00, pl29_win_00, pl29_win_02, pl29_win_02, pl29_win_04, pl29_win_02,
	pl29_win_06, pl29_win_07, 0,
};

const Sint32 *const pl29_extra_tbl[] = {
	pl29_extra_00, pl29_extra_01, pl29_extra_02, pl29_extra_03, pl29_extra_04, pl29_caught_23,
	pl29_extra_06, 0,
};

const Sint32 *const pl29_special_tbl[] = {
	pl29_special_00, pl29_special_01, pl29_special_01, pl29_special_03, pl29_special_04, pl29_special_05,
	pl29_special_06, pl29_special_06, pl29_special_08, pl29_special_06, pl29_special_10, pl29_special_11,
	pl29_special_12, pl29_special_12, pl29_special_12, pl29_special_12, pl29_special_12, pl29_win_04,
	pl29_special_18, pl29_special_19, pl29_special_20, pl29_special_21, pl29_special_22, pl29_special_23,
	pl29_special_24, pl29_special_25, pl29_special_26, pl29_special_27, pl29_special_28, pl29_special_29,
	pl29_special_30, pl29_special_31, pl29_caught_23, 0,
};

const Sint32 *const pl29_super_tbl[] = {
	pl29_super_00, pl29_super_01, pl29_super_02, 0,
};

const Sint32 *const pl29_effect_tbl[] = {
	pl29_effect_00, pl29_effect_01, pl29_effect_02, pl29_effect_03, pl29_effect_04, pl29_effect_05,
	pl29_special_12, pl29_damage_31, pl29_effect_08, pl29_effect_05, pl29_win_02, pl29_win_02,
	pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02,
	pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02,
	pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02, pl29_win_02, 0,
};

const Sint32 *const pl29_change_tbl[] = {
	pl29_change_00, pl29_change_01, pl29_change_02, pl29_change_03, pl29_change_04, 0,
};

const Sint32 *const pl29_grp25_tbl[] = {
	pl29_damage_31, pl29_damage_31,
};

const Sint32 *const pl29_grp27_tbl[] = {
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Dizzy 1 */
const Sint32 pl29_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    22, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    23, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    24, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    26, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    27, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl29_stand_00 ),
};

/* Char_Stand 2: win_ko, act_walk (2 + no) */
const Sint32 pl29_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,    30, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    31, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    32, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    33, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    35, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl29_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl29_stand_03[] = {
	CG_FRAME(    0,    0,   7,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl29_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl29_stand_06[] = {
	CG_FRAME(    0,    0,  11,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  11,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  11,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  11, END_OF_DATA,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl29_stand_07[] = {
	CG_FRAME(    0,    0,  12,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl29_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl29_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    74, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl29_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl29_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl29_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    78, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl29_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl29_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl29_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl29_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl29_jump_03[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_jump_03 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl29_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    74, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl29_jump_09 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl29_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    78, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl29_jump_10 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl29_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl29_jump_11 ),
};

/* Char_Jump 6 */
const Sint32 pl29_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl29_damage_30_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    74, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl29_damage_30_loop ),
};

/* Char_Jump 7 */
const Sint32 pl29_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl29_jump_07_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    78, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl29_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl29_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl29_jump_08_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl29_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl29_dash_00[] = {
	CG_FRAME(    0,    1,   6,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl29_dash_00 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl29_dash_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl29_dash_01 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl29_dash_02[] = {
	CG_FRAME(    0,    0,   8,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  22,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,  10,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl29_dash_02 ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl29_dash_03[] = {
	CG_FRAME(    0,    0,  16,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl29_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl29_crouch_00[] = {
	CG_FRAME(    1,    0,   3,           0,    81, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    1,    0,   3,           0,    82, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    83, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl29_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl29_crouch_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    29, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl29_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl29_crouch_02[] = {
	CG_FRAME(    1,    0,   4,           0,    82, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    0,   4,           0,    81, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    84, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl29_turn_00[] = {
	CG_FRAME(    0,    0,   8,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl29_turn_01[] = {
	CG_FRAME(    0,    0,   8,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl29_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl29_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl29_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl29_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl29_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl29_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl29_guard_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl29_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl29_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl29_guard_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl29_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl29_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl29_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl29_guard_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl29_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl29_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl29_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    91, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl29_air_guard_01 ),
};

/* Char_Air_Guard 4 */
const Sint32 pl29_air_guard_04[] = {
	CG_FRAME(    0,    0,   6,           0,    89, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,  23,           0,    89, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl29_air_guard_04_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,    89, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl29_air_guard_04_loop ),
};

/* Char_Air_Guard 5 */
const Sint32 pl29_air_guard_05[] = {
	CG_FRAME(    0,    0,   6,           0,    90, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,  23,           0,    90, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl29_air_guard_05_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,    90, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl29_air_guard_05_loop ),
};

/* Char_Air_Guard 6 */
const Sint32 pl29_air_guard_06[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl29_air_guard_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    91, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl29_air_guard_06_loop ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl29_stand_p_00[] = {
	CG_FRAME(    0,    0,   7,           0,   128, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   128, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x81,    0,   3,           0,   129, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   129, 0x98,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    1,  14, END_OF_DATA,   130, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl29_stand_p_00 ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl29_extra_02[] = {
	CG_FRAME(    0,    0,   8,           0,   131, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   131, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x81,    0,   3,           0,   132, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   6,           0,   132, 0x98,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    2,  14, END_OF_DATA,   133, 0x98,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl29_extra_02 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl29_stand_p_01[] = {
	CG_FRAME(    0,    0,   7,           0,   134, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0xa0,   0xe1,  ATK,  0x00,       0x04,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   2,           0,   137, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x81,    0,   1,           0,   138, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,  14,           0,   138, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   7,           0,    61, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,    62, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl29_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl29_stand_p_02[] = {
	CG_FRAME(    0,    0,   8,           0,   139, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0xa0,   0xe1,  ATK,  0x00,       0x05,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   2,           0,   142, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x81,    0,   1,           0,   143, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,  32,           0,   143, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   144, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl29_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl29_stand_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,   145, 0xa0,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0,   146, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,   1,           0,   146, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x81,    0,   5,           0,   146, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   8,           0,   146, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,   8, END_OF_DATA,   144, 0x98,   0xc1,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_NEXT( pl29_stand_k_00 ),
};

const Sint32 pl29_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   145, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   8,           0,   146, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   146, 0x98,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	0x000014c0,
};

/* Char_Stand_K 1 (medium): win_ko */
const Sint32 pl29_stand_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   151, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   8,           0,   152, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    1,   2,           0,   154, 0xa0,   0xa9,  ATK,  0x00,       0x04,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0xa0,   0xa9,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0xa0,   0xa9,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0xa0,   0xa9,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0xa0,   0xa9,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0xa0,   0xa9,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0xa0,   0xa9,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0xa0,   0xa9,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x98,   0xa9,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   156, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl29_stand_k_02[] = {
	CG_FRAME(    0,    0,   8,           0,   147, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6,           0,   148, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   2,           0,   149, 0xa0,   0xe1,  ATK,  0x00,       0x05,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x81,    0,   1,           0,   149, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,  31,           0,   149, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   144, 0x98,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl29_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl29_crouch_p_00[] = {
	CG_FRAME(    0,    0,   7,           0,   157, 0xa0,   0x4b,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   158, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   158, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   159, 0x98,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl29_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl29_crouch_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   160, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   8,           0,   161, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   162, 0xa0,   0x41,  ATK,  0x00,       0x04,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    1,   4,           0,   163, 0xa0,   0x41,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   165, 0x98,   0x4a,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl29_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl29_crouch_p_02[] = {
	CG_FRAME(    0,    0,  10,           0,   166, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0xa0,   0x40,  ATK,  0x00,       0x05,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0xa0,   0xeb,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   2,           0,   169, 0xa0,   0xeb,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x81,    0,   1,           0,   170, 0xa0,   0xeb,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   170, 0xa0,   0xeb,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,  32,           0,   170, 0x98,   0xeb,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   171, 0x98,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl29_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl29_crouch_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,   174, 0xa0,   0xc3,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   175, 0xa0,   0xe1,  ATK,  0x00,       0x03,    0,   0,     0,      0,  64 ),
	CG_FRAME( 0x80,    0,   2,           0,   175, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x81,    0,   1,           0,   175, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   175, 0xa0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   174, 0x98,   0xc3,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl29_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl29_crouch_k_01[] = {
	CG_FRAME(    0,    0,   8,           0,   173, 0xa0,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0xa0,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,   3,           0,   173, 0xa0,   0xa1,  ATK,  0x00,       0x04,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x81,    0,   1,           0,   172, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,  24,           0,   172, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   173, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl29_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl29_crouch_k_02[] = {
	CG_FRAME(    0,    0,   8,           0,   176, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   6,           0,   177, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x80,    0,   1,           0,   178, 0xa0,   0xc1,  ATK,  0x00,       0x05,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x81,    0,   1,           0,   179, 0xa0,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0xa0,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,  26,           0,   179, 0x98,   0xc1,  ATE,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,  10,           0,   180, 0x98,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    82, 0x98,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl29_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl29_jump_p_00[] = {
	CG_FRAME(    0,    0,   6,           0,   181, 0xa0,   0x22,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    0,   2,           0,   182, 0xa0,   0xc2,  ATK,  0x00,       0x03,    0,   0,     0,      0,  77 ),
	CG_FRAME( 0x81,    0,   1,           0,   182, 0xa0,   0xc2,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,  11,           0,   182, 0xa0,   0xc2,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   183, 0x98,   0x22,  ATE,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl29_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl29_jump_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   184, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0xa0,   0xa8,  ATK,  0x00,       0x04,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   186, 0xa0,   0xa8,  ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x98,   0xa8,  ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,  10,           0,   188, 0x98,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x98,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   190, 0x98,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl29_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl29_jump_p_02[] = {
	CG_FRAME(    0,    0,  12,           0,   191, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME( 0x80,    0,   4,           0,   192, 0xa0,   0x20,  ATK,  0x00,       0x05,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x81,    0,   1,           0,   193, 0xa0,   0xa1,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   5,           0,   194, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   1,           0,   195, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x98,   0xa1,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   196, 0x98,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl29_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl29_jump_k_00[] = {
	CG_FRAME(    0,    0,   6,           0,   198, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME( 0x80,    0,   3,           0,   197, 0xa0,   0x41,  ATK,  0x00,       0x03,    0,   0,     0,      0,  89 ),
	CG_FRAME( 0x81,    0,   1,           0,   197, 0xa0,   0x41,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   197, 0xa0,   0x41,  ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   198, 0x98,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl29_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl29_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   201, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   7,           0,   202, 0xa0,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   3,           0,   203, 0xa0,   0xc1,  ATK,  0x00,       0x04,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x81,    0,   1,           0,   203, 0xa0,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   203, 0xa0,   0xc1,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   8,           0,   203, 0x98,   0xc1,  ATE,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   204, 0x98,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl29_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl29_jump_k_02[] = {
	CG_FRAME(    0,    0,  10,           0,   200, 0xa0,   0x22,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME( 0x80,    0,   2,           0,   200, 0xa0,   0x22,  ATK,  0x00,       0x05,    0,   0,     0, 0x6c00,  97 ),
	CG_FRAME( 0x81,    0,   1,           0,   199, 0xa0,   0xc2,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   5,           0,   199, 0xa0,   0xc2,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,  12,           0,   199, 0xa0,   0xc2,  ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   200, 0x98,   0x22,  ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl29_jump_k_02 ),
};

/* Char_Catch 0 */
const Sint32 pl29_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0044,   0 ),
	CG_FRAME(    0,    0,  10,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0046,   0 ),
	CG_FRAME(    0,    0,   8,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   3,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,  16,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,  38,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl29_catch_00 ),
};

/* Char_Catch 1 */
const Sint32 pl29_catch_01[] = {
	CG_FRAME(    0,    0,  12,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   8,           0,   208, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   8,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   8,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   8,           0,   208, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   8,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0, 0x80,   3,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,  22,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_NEXT( pl29_catch_01 ),
};

/* Char_Catch 2 */
const Sint32 pl29_catch_02[] = {
	CG_FRAME(    0,    0,  12,           0,   184, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x20,    0,  0x00,       0x05,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    1,  16,           0,   255, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
};

const Sint32 pl29_catch_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   190, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_NEXT( pl29_catch_02_loop ),
};

/* Char_Special 4: powerup_init (Cytorrak Power-up) */
const Sint32 pl29_special_04[] = {
	CG_FRAME(    0,    0,   1,           0,   210, 0x80,   0x00,    0,  0x00,       0x19,    0,   0,     0,      0, 100 ),
};

const Sint32 pl29_special_04_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl29_special_04_loop ),
};

const Sint32 pl29_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00002114,
};

/* Char_Extra 3: nml_atck, unused_init, fly_start5 */
const Sint32 pl29_extra_03[] = {
	CG_FRAME(    0,    0,   1,           0,   220, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   8,           0,   221, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0xb0,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   225, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl29_extra_03 ),
};

/* Char_Effect 2: pls29_00_b */
const Sint32 pl29_effect_02[] = {
	CG_FRAME(    0,    0,   4,           0,   226, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   227, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_02 ),
};

/* Char_Special 0 */
const Sint32 pl29_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   228, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   8,           0,   229, 0xb0,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0xb0,   0x01,  ATK,  0x00,       0x1f,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    1,   4,           0,   164, 0xb0,   0x01,  ATK,  0x00, 0x4d | CSE,    0,   0,     0,      0, 107 ),
};

const Sint32 pl29_special_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   164, 0x98,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl29_special_00_loop ),
};

/* Char_Special 1
   Char_Special 2 */
const Sint32 pl29_special_01[] = {
	CG_FRAME(    0,    0,   3,           0,   228, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,  12,           0,   229, 0xb0,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   230, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   2,           0,   162, 0xb0,   0x01,  ATK,  0x00,       0x1f,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    1,   4,           0,   164, 0xb0,   0x01,  ATK,  0x00, 0x4d | CSE,    0,   0,     0,      0, 107 ),
};

const Sint32 pl29_special_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   163, 0x98,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   164, 0x98,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl29_special_01_loop ),
};

/* Char_Special 3: quake_wait (Earthquake Punch) */
const Sint32 pl29_special_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   231, 0x98,   0x01,  ATE,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_NEXT( pl29_special_03 ),
};

/* Char_Special 5 */
const Sint32 pl29_special_05[] = {
	CG_FRAME(    0,    0,   1,           0,   232, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   8,           0,   233, 0xb0,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   2,           0,   234, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0,   235, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
};

const Sint32 pl29_special_05_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   236, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_NEXT( pl29_special_05_loop ),
};

/* Char_Special 8 */
const Sint32 pl29_special_08[] = {
	CG_FRAME(    0, 0x81,   4,           0,   236, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0, 0x82,   4,           0,   237, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    1, 0x83,   4,           0,   238, 0xb0,   0xe1,  ATK,  0x00,       0x1f,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x84,   7,           0,   240, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x80,   5,           0,   241, 0x98,   0xc6,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   242, 0x98,   0xab,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_special_08 ),
};

/* Char_Special 6
   Char_Special 7
   Char_Special 9 */
const Sint32 pl29_special_06[] = {
	CG_FRAME(    0,    0,   1,           0,   232, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   8,           0,   233, 0xb0,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   2,           0,   234, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0,   235, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
};

const Sint32 pl29_special_06_loop[] = {
	CG_FRAME(    0,    1,  10, END_OF_DATA,   236, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_NEXT( pl29_special_06_loop ),
};

/* Char_Special 10 */
const Sint32 pl29_special_10[] = {
	CG_FRAME(    0, 0x81,   1,           0,   236, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0, 0x82,   4,           0,   237, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    1, 0x83,   4,           0,   238, 0xb0,   0xe1,  ATK,  0x00,       0x1f,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   3,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   238, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   239, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x84,   8,           0,   240, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x80,   8,           0,   241, 0x98,   0xc6,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   242, 0x98,   0xab,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_special_10 ),
};

/* Char_Special 27 */
const Sint32 pl29_special_27[] = {
	CG_FRAME(    0,    0,   1,           0,   233, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   2,           0,   234, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0,   235, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
};

const Sint32 pl29_special_27_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   236, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_NEXT( pl29_special_27_loop ),
};

/* Char_Special 28 */
const Sint32 pl29_special_28[] = {
	CG_FRAME(    0, 0x81,   3,           0,   236, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0, 0x82,   3,           0,   237, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    1, 0x83,   3,           0,   238, 0xb0,   0xe5,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0, 0x83,   4,           0,   239, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   238, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   239, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   238, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   239, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   238, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   239, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x83,   4,           0,   239, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x84,   8,           0,   240, 0x98,   0xe5,  ATE,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0, 0x80,   6,           0,   241, 0x98,   0xc8,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   242, 0x98,   0xab,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_special_28 ),
};

/* Char_Effect 3: pls29_00_c */
const Sint32 pl29_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,   243, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   243, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   277, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    3,   1,           0,   246, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   1,           0,   247, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   248, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   249, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   250, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   251, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   252, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_03 ),
};

/* Char_Effect 4: pls29_00_d */
const Sint32 pl29_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   266, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   268, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   269, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   276, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_04 ),
};

/* Char_Super 0: headcrush_init (Juggernaut Headcrush (hyper)) */
const Sint32 pl29_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   228, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    2,   2,           0,   220, 0xc0,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    1,   1,           0, 32768, 0xc0,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   1,           0, 32768, 0xc0,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0, 118 ),
};

const Sint32 pl29_super_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_NEXT( pl29_super_00_loop ),
};

/* Char_Super 1: headcrush_rush (Juggernaut Headcrush (hyper)) */
const Sint32 pl29_super_01[] = {
	CG_FRAME(    0,    0,  20,           0,   257, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,  16,           0,    60, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,  14,           0,    61, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,  14, END_OF_DATA,    62, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 122 ),
	CG_NEXT( pl29_super_01 ),
};

/* Char_Effect 1: pls29_00_a */
const Sint32 pl29_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32770, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32771, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32776, 0xc0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_01 ),
};

/* Char_Extra 4 */
const Sint32 pl29_extra_04[] = {
	CG_FRAME(    0,    0,   8,           0,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,  16,           0,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   253, 0x30,   0x20,  ATE,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_NEXT( pl29_extra_04 ),
};

/* Char_Special 18 */
const Sint32 pl29_special_18[] = {
	CG_FRAME(    0,    1,   2,           0,   116, 0x90,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   8,           0,   253, 0x30,   0x20,  ATK,  0x00,       0x05,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,  96, END_OF_DATA,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl29_special_18 ),
};

/* Char_Special 21 */
const Sint32 pl29_special_21[] = {
	CG_FRAME(    0,    0,   8,           0,   116, 0x40,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8,           0,   117, 0x40,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8,           0,    81, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    82, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl29_special_21 ),
};

/* Char_Special 24 */
const Sint32 pl29_special_24[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   295, 0x40,   0xa0,  ATE,  0x00,       0x1f,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    1,   5,           0,   112, 0x40,   0xef,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,   5,           0,   113, 0x40,   0xef,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   325, 0x50,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl29_special_24 ),
};

/* Char_Special 19 */
const Sint32 pl29_special_19[] = {
	CG_FRAME(    0,    1,   4,           0,   116, 0x90,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   8,           0,   253, 0x30,   0x20,  ATK,  0x00,       0x05,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,  96, END_OF_DATA,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl29_special_19 ),
};

/* Char_Special 22 */
const Sint32 pl29_special_22[] = {
	CG_FRAME(    0,    0,   8,           0,   116, 0x40,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8,           0,   117, 0x40,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8,           0,    81, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,    82, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl29_special_22 ),
};

/* Char_Special 25 */
const Sint32 pl29_special_25[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   295, 0x40,   0xa0,  ATE,  0x00,       0x1f,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,   8,           0,   298, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    1,   5,           0,   112, 0x40,   0xef,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,   5,           0,   113, 0x40,   0xef,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   325, 0x50,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl29_special_25 ),
};

/* Char_Special 20 */
const Sint32 pl29_special_20[] = {
	CG_FRAME(    0,    1,   6,           0,   116, 0x90,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   8,           0,   253, 0x30,   0x20,  ATK,  0x00,       0x05,    0,   0,     0,      0, 123 ),
	CG_FRAME(    0,    0,  96, END_OF_DATA,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl29_special_20 ),
};

/* Char_Special 23 */
const Sint32 pl29_special_23[] = {
	CG_FRAME(    0,    0,   8,           0,   116, 0x40,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8,           0,   117, 0x40,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   8,           0,    81, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,    82, 0x80,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl29_special_23 ),
};

/* Char_Special 26 */
const Sint32 pl29_special_26[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   295, 0x40,   0xa0,  ATE,  0x00,       0x1f,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,  16,           0,   298, 0x40,   0xe0,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    1,   5,           0,   112, 0x40,   0xef,  ATE,  0x00,          0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,   5,           0,   113, 0x40,   0xef,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x40,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   325, 0x50,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl29_special_26 ),
};

/* Char_Special 29 */
const Sint32 pl29_special_29[] = {
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0044, 117 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0045, 117 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0044, 117 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0045, 117 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0044, 117 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0045, 117 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0044, 117 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0045, 117 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0044, 117 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0045, 117 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0044, 117 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0045, 117 ),
	CG_FRAME(    0,    0,   8,           0,   220, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0046, 117 ),
	CG_FRAME(    1,    1,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0, 0x0047, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0048, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0049, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0048, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0049, 117 ),
	CG_FRAME(    1,    0,   4,           0,   228, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004a, 117 ),
	CG_FRAME(    1,    0,   8,           0,   220, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004b, 117 ),
	CG_FRAME(    1,    1,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0, 0x004c, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004d, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004e, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004d, 117 ),
	CG_FRAME(    1,    0,   4,           0, 32768, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004e, 117 ),
	CG_FRAME(    1,    0,   4,           0,   134, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004f, 117 ),
	CG_FRAME(    1,    0,   4,           0,   135, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004f, 117 ),
	CG_FRAME(    1,    0,   4,           0,   136, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x004f, 117 ),
	CG_FRAME(    1,    1,   4,           0,   205, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x0018, 117 ),
	CG_FRAME(    1,    0,   4,           0,   206, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0019, 117 ),
	CG_FRAME(    1,    0,   4,           0,    61, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0018, 117 ),
	CG_FRAME(    1,    0,   6,           0,    62, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0019, 117 ),
	CG_FRAME(    1,    0,   4,           0,   166, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0018, 117 ),
	CG_FRAME(    1,    0,   4,           0,   167, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001a, 117 ),
	CG_FRAME(    0,    3,  10,           0,   168, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001b, 117 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001c, 117 ),
	CG_FRAME(    0,    0,  16,           0,   170, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001d, 117 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   171, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x001e, 117 ),
	CG_NEXT( pl29_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl29_special_30[] = {
	CG_FRAME(    0,    0,   4,           0,   253, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 151 ),
};

const Sint32 pl29_special_30_loop[] = {
	CG_FRAME(    0,    0,  48, END_OF_DATA,   253, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl29_special_30_loop ),
};

/* Char_Special 31 */
const Sint32 pl29_special_31[] = {
	CG_FRAME(    1,    0,   2,           0,    81, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    0,   8,           0,    82, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,  ATE,  0x00,       0x17,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
};

const Sint32 pl29_special_31_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_NEXT( pl29_special_31_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl29_damage_00[] = {
	CG_FRAME( 0x80,    0,   2,           0,   279, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x82,    0,   5,           0,   279, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   278, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl29_damage_00 ),
};

/* Char_Caught 0 */
const Sint32 pl29_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   278, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl29_caught_00 ),
};

/* Char_Damage 1 */
const Sint32 pl29_damage_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   280, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x82,    0,   8,           0,   280, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   278, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl29_damage_01 ),
};

/* Char_Caught 1 */
const Sint32 pl29_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   280, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl29_caught_01 ),
};

/* Char_Damage 2 */
const Sint32 pl29_damage_02[] = {
	CG_FRAME( 0x80,    0,   4,           0,   281, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x82,    0,   8,           0,   281, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   278, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl29_damage_02 ),
};

/* Char_Caught 2 */
const Sint32 pl29_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   281, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl29_caught_02 ),
};

/* Char_Damage 3 */
const Sint32 pl29_damage_03[] = {
	CG_FRAME( 0x80,    0,   2,           0,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x82,    0,   5,           0,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   282, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl29_damage_03 ),
};

/* Char_Caught 3 */
const Sint32 pl29_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 131 ),
	CG_NEXT( pl29_caught_03 ),
};

/* Char_Damage 4 */
const Sint32 pl29_damage_04[] = {
	CG_FRAME( 0x80,    0,   1,           0,   284, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x82,    0,   1,           0,   284, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   8,           0,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   282, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl29_damage_04 ),
};

/* Char_Caught 4 */
const Sint32 pl29_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   284, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 132 ),
	CG_NEXT( pl29_caught_04 ),
};

/* Char_Damage 5 */
const Sint32 pl29_damage_05[] = {
	CG_FRAME( 0x80,    0,   4,           0,   285, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME( 0x82,    0,   8,           0,   284, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   8,           0,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   282, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl29_damage_05 ),
};

/* Char_Caught 5 */
const Sint32 pl29_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   285, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 133 ),
	CG_NEXT( pl29_caught_05 ),
};

/* Char_Damage 6 */
const Sint32 pl29_damage_06[] = {
	CG_FRAME( 0x80,    0,   3,           0,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME( 0x82,    0,   5,           0,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_NEXT( pl29_damage_06 ),
};

/* Char_Caught 6 */
const Sint32 pl29_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_NEXT( pl29_caught_06 ),
};

/* Char_Damage 7 */
const Sint32 pl29_damage_07[] = {
	CG_FRAME( 0x80,    0,   2,           0,   287, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME( 0x82,    0,   8,           0,   287, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_NEXT( pl29_damage_07 ),
};

/* Char_Caught 7 */
const Sint32 pl29_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   287, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 135 ),
	CG_NEXT( pl29_caught_07 ),
};

/* Char_Damage 8 */
const Sint32 pl29_damage_08[] = {
	CG_FRAME( 0x80,    0,   4,           0,   288, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME( 0x82,    0,   8,           0,   288, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   286, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 134 ),
	CG_NEXT( pl29_damage_08 ),
};

/* Char_Caught 8 */
const Sint32 pl29_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   288, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_NEXT( pl29_caught_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl29_damage_10[] = {
	CG_FRAME( 0x80,    0,   5,           0,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x82,    0,   5, END_OF_DATA,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_10 ),
};

/* Char_Caught 9 */
const Sint32 pl29_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 138 ),
	CG_NEXT( pl29_caught_09 ),
};

/* Char_Damage 11 */
const Sint32 pl29_damage_11[] = {
	CG_FRAME( 0x80,    0,   5,           0,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_11 ),
};

/* Char_Caught 10 */
const Sint32 pl29_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   290, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 138 ),
	CG_NEXT( pl29_caught_10 ),
};

/* Char_Damage 12 */
const Sint32 pl29_damage_12[] = {
	CG_FRAME( 0x80,    0,   5,           0,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   289, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_12 ),
};

/* Char_Caught 11
   Char_Caught 21 */
const Sint32 pl29_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   291, 0x40,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 139 ),
	CG_NEXT( pl29_caught_11 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl29_damage_13[] = {
	CG_FRAME( 0x80,    0,   4,           0,   292, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME( 0x82,    0,   4, END_OF_DATA,   293, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl29_damage_13 ),
};

/* Char_Caught 12 */
const Sint32 pl29_caught_12[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   292, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 141 ),
	CG_NEXT( pl29_caught_12 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl29_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   283, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   294, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   295, 0x40,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   296, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
};

const Sint32 pl29_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   298, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl29_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl29_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   299, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   300, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_16 ),
};

/* Char_Caught 13 */
const Sint32 pl29_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   299, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl29_caught_13 ),
};

/* Char_Caught 18 */
const Sint32 pl29_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   300, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl29_caught_18 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl29_damage_17[] = {
	CG_FRAME(    0,    0,   4,           0,   301, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   301, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_17 ),
};

/* Char_Caught 14 */
const Sint32 pl29_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   301, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl29_caught_14 ),
};

/* Char_Damage 18 */
const Sint32 pl29_damage_18[] = {
	CG_FRAME(    0,    0,   4,           0,   302, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   302, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_18 ),
};

/* Char_Caught 15 */
const Sint32 pl29_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   302, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl29_caught_15 ),
};

/* Char_Damage 19 */
const Sint32 pl29_damage_19[] = {
	CG_FRAME(    0,    0,   4,           0,   303, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   303, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_19 ),
};

/* Char_Caught 16 */
const Sint32 pl29_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   303, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl29_caught_16 ),
};

/* Char_Damage 20 */
const Sint32 pl29_damage_20[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   304, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_20 ),
};

/* Char_Caught 17 */
const Sint32 pl29_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   304, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 142 ),
	CG_NEXT( pl29_caught_17 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl29_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   299, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
};

/* Char_Damage 22 */
const Sint32 pl29_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   301, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
};

/* Char_Damage 23 */
const Sint32 pl29_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   302, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
};

/* Char_Damage 24 */
const Sint32 pl29_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   303, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
};

/* Char_Damage 25 */
const Sint32 pl29_damage_25[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   304, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl29_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   305, 0x40,   0x1c,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   3,           0,   306, 0x40,   0x1c,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   3,           0,   307, 0x40,   0xf9,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   2,           0,   308, 0x40,   0xc0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   1,           0,   309, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   310, 0x40,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl29_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl29_damage_27[] = {
	CG_FRAME(    0,    0,   5,           0,   124, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   6,           0,   125, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   6,           0,   126, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   127, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_30_loop ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl29_damage_30[] = {
	CG_FRAME(    0,    0,   5,           0,   127, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   5,           0,   125, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,  14, END_OF_DATA,    71, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_30_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl29_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   311, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   312, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   316, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_28 ),
};

/* Char_Caught 19 */
const Sint32 pl29_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   317, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_caught_19 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl29_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   317, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   320, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   322, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl29_damage_29 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl29_dizzy_00[] = {
	CG_FRAME(    0,    0,   7,           0,   108, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   109, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   110, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   111, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   110, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   109, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_dizzy_00 ),
};

/* Char_Special 11 */
const Sint32 pl29_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    21, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_special_11 ),
};

/* Char_Special 12
   Char_Special 13
   Char_Special 14
   Char_Special 15
   Char_Special 16
   Char_Effect 6 */
const Sint32 pl29_special_12[] = {
	CG_FRAME(    0,    0,   7, END_OF_DATA,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 146 ),
	CG_NEXT( pl29_special_12 ),
};

/* Char_Win 0
   Char_Win 1: win_ko */
const Sint32 pl29_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl29_win_00_loop[] = {
	CG_FRAME(    0,    0,  10,           0,    94, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,    95, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,    96, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,    95, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_win_00_loop ),
};

/* Char_Win 6: win_pose */
const Sint32 pl29_win_06[] = {
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x00,    0,  0x00,       0x18,    0,   0,     0,      0,   0 ),
};

const Sint32 pl29_win_06_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_win_06_loop ),
};

/* Char_Win 7: win_pose_ef */
const Sint32 pl29_win_07[] = {
	CG_FRAME(    0,    0,   1,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  48,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   101, 0x80,   0x00,    0,  0x00, 0x50 | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl29_win_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_win_07_loop ),
};

/* Char_Win 4
   Char_Special 17: sp_chouhatsu (Taunt) */
const Sint32 pl29_win_04[] = {
	CG_FRAME(    0,    0,   5,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    0,   5,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    1,   2,           0,   101, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,    0,  0x00, 0x50 | CSE,    0,   0,     0,      0, 147 ),
};

const Sint32 pl29_win_04_loop[] = {
	CG_FRAME(    0,    0,  35, END_OF_DATA,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 147 ),
	CG_NEXT( pl29_win_04_loop ),
};

/* Char_Effect 8: pls29_00_e */
const Sint32 pl29_effect_08[] = {
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_08 ),
};

/* Char_Win 2: pl29_win
   Char_Win 3
   Char_Win 5: pl29_win
   Char_Effect 10
   Char_Effect 11
   Char_Effect 12 */
const Sint32 pl29_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   110, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl29_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   108, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl29_win_02_loop ),
};

/* Char_Appear 0: appear_init
   Char_Appear 1 */
const Sint32 pl29_appear_00[] = {
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl29_appear_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_appear_00_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl29_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   112, 0x40,   0xef,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x40,   0xef,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_get_up_00 ),
};

/* Char_Get_Up 2 */
const Sint32 pl29_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x40,   0xc0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x40,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x40,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   120, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   117, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_get_up_02 ),
};

/* Char_Get_Up 1 */
const Sint32 pl29_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   118, 0x40,   0xc0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x40,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   122, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   116, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   117, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    84, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_get_up_01 ),
};

/* Char_Get_Up 3 */
const Sint32 pl29_get_up_03[] = {
	CG_FRAME(    0,    0,   4,           0,   123, 0x40,   0xc0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   124, 0x40,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   125, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   126, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   127, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    83, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_get_up_03 ),
};

/* Char_Effect 0: shl29_00_ride */
const Sint32 pl29_effect_00[] = {
	CG_FRAME(    0,    0,   4,           0,   327, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   330, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   334, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_00 ),
};

/* Char_Super 2: shl29_00_row */
const Sint32 pl29_super_02[] = {
	CG_FRAME(    0,    0,   2,           0, 32777, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32779, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32780, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 148 ),
	CG_FRAME(    0,    0,   3,           0, 32781, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 148 ),
	CG_FRAME( 0x80,    0,   3,           0, 32782, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 149 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32785, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32786, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32787, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32788, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_super_02 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down
   Char_Effect 7
   group 25 0
   group 25 1
   Char_Fly 0
   Char_Fly 1: act_fly */
const Sint32 pl29_damage_31[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   326, 0x60,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl29_damage_31 ),
};

/* Char_Caught 20 */
const Sint32 pl29_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   326, 0x60,   0xe0,    0,  0x00,          0,    0,   0,     0,      0, 141 ),
	CG_NEXT( pl29_caught_20 ),
};

/* Char_Effect 5
   Char_Effect 9 */
const Sint32 pl29_effect_05[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_effect_05 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl29_change_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_change_01 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl29_extra_00[] = {
	CG_FRAME(    0,    0,   4,           0,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 151 ),
};

const Sint32 pl29_extra_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   253, 0x30,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0, 152 ),
	CG_NEXT( pl29_extra_00_loop ),
};

/* Char_Extra 1: t_kick_jump */
const Sint32 pl29_extra_01[] = {
	CG_FRAME(    1,    0,   2,           0,    81, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    0,   8,           0,    82, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   4,           0,   211, 0x80,   0x00,  ATE,  0x00,       0x17,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
};

const Sint32 pl29_extra_01_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   211, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 153 ),
	CG_NEXT( pl29_extra_01_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl29_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,    97, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x30,   0x0d,  ATK,  0x00,       0x17,    0,   0,     0,      0,   0 ),
};

const Sint32 pl29_change_00_loop[] = {
	CG_FRAME(    0,    1,  10, END_OF_DATA,   102, 0x30,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl29_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl29_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,    77, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl29_change_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    80, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl29_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl29_change_03[] = {
	CG_FRAME(    1,    0,   2,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   8,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   211, 0x80,   0x00,    0,  0x00,       0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl29_change_03_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl29_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl29_change_04[] = {
	CG_FRAME(    0,    0,   5,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   101, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl29_change_04_loop[] = {
	CG_FRAME(    0,    0,  14, END_OF_DATA,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl29_change_04_loop ),
};

/* Char_Extra 6: pl29_upper */
const Sint32 pl29_extra_06[] = {
	CG_FRAME(    0,    0,  10,           0,   166, 0xa0,   0x40,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0xa0,   0x40,  ATK,  0x00,       0x05,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0xa0,   0xe9,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   2,           0,   169, 0xa0,   0xe9,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x81,    0,   1,           0,   170, 0xa0,   0xe9,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   170, 0xa0,   0xe9,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,  32,           0,   170, 0x98,   0xe9,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   171, 0x98,   0x40,  ATE,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl29_extra_06 ),
};

/* Char_Caught 23
   Char_Caught 24
   Char_Caught 25
   Char_Caught 26
   Char_Caught 27
   Char_Caught 28 */
const Sint32 pl29_caught_23[] = {
	CG_FRAME(    0,    0,   1,           0,   220, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   8,           0,   221, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME( 0x80,    1,   4,           0,   220, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME( 0x81,    0,   4,           0,   221, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0xb0,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0xb0,   0xe1,  ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   223, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    2,   2,           0,   224, 0x98,   0xe1,  ATE,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   225, 0x98,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl29_caught_23 ),
};

const Sint32 pl29_unused_02[] = {	/* nothing reaches it */
	CG_NEXT( pl29_caught_23 ),
};
