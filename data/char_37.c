/* Jin (character 37): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_37      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_37     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl37.c) says which group
                   and number each move uses.
   pl37_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl37_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl37_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl37_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_37
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_37, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_37 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl37_stand_00[], pl37_stand_02[], pl37_stand_03[], pl37_stand_06[],
	pl37_stand_07[], pl37_jump_00[], pl37_jump_00_loop[], pl37_jump_01[], pl37_jump_01_loop[],
	pl37_jump_02[], pl37_jump_02_loop[], pl37_jump_03[], pl37_jump_06[], pl37_jump_09[],
	pl37_jump_07[], pl37_jump_10[], pl37_jump_08[], pl37_jump_11[], pl37_dash_00[],
	pl37_dash_00_loop[], pl37_dash_02[], pl37_dash_01[], pl37_dash_01_loop[], pl37_dash_03[],
	pl37_crouch_00[], pl37_crouch_01[], pl37_crouch_02[], pl37_turn_00[], pl37_turn_01[],
	pl37_guard_00[], pl37_guard_01[], pl37_guard_02[], pl37_guard_04[], pl37_guard_05[],
	pl37_guard_06[], pl37_air_guard_00[], pl37_air_guard_01[], pl37_stand_p_00[], pl37_stand_p_03[],
	pl37_stand_p_01[], pl37_extra_09[], pl37_stand_p_02[], pl37_stand_k_00[], pl37_stand_k_01[],
	pl37_stand_k_01_loop[], pl37_stand_k_02[], pl37_stand_k_02_loop[], pl37_stand_k_03[],
	pl37_crouch_p_00[], pl37_crouch_p_03[], pl37_crouch_p_01[], pl37_crouch_p_02[],
	pl37_special_10[], pl37_crouch_k_00[], pl37_crouch_k_01[], pl37_crouch_k_01_loop[],
	pl37_extra_10[], pl37_extra_10_loop[], pl37_crouch_k_02[], pl37_crouch_k_02_loop[],
	pl37_crouch_k_03[], pl37_jump_p_00[], pl37_jump_p_01[], pl37_jump_p_02[], pl37_jump_k_00[],
	pl37_jump_k_01[], pl37_jump_k_02[], pl37_extra_02[], pl37_extra_02_loop[], pl37_effect_26[],
	pl37_effect_27[], pl37_effect_28[], pl37_effect_29[], pl37_catch_00[], pl37_catch_00_loop[],
	pl37_catch_01[], pl37_catch_01_loop[], pl37_catch_02[], pl37_catch_03[], pl37_catch_03_loop[],
	pl37_catch_04[], pl37_catch_04_loop[], pl37_catch_05[], pl37_catch_06[], pl37_catch_07[],
	pl37_catch_08[], pl37_catch_09[], pl37_catch_10[], pl37_catch_11[], pl37_catch_12[],
	pl37_catch_13[], pl37_catch_14[], pl37_catch_15[], pl37_catch_15_loop[], pl37_catch_16[],
	pl37_catch_17[], pl37_effect_22[], pl37_effect_23[], pl37_effect_24[], pl37_effect_25[],
	pl37_catch_18[], pl37_catch_18_loop[], pl37_special_06[], pl37_special_06_loop[],
	pl37_special_08[], pl37_effect_01[], pl37_effect_02[], pl37_effect_21[], pl37_effect_11[],
	pl37_effect_12[], pl37_effect_13[], pl37_effect_14[], pl37_effect_15[], pl37_effect_16[],
	pl37_effect_17[], pl37_effect_18[], pl37_effect_19[], pl37_effect_20[], pl37_special_00[],
	pl37_special_00_loop[], pl37_special_01[], pl37_special_01_loop[], pl37_special_02[],
	pl37_special_02_loop[], pl37_special_03[], pl37_special_03_loop[], pl37_special_04[],
	pl37_special_05[], pl37_effect_00[], pl37_super_10[], pl37_super_08[], pl37_super_08_loop[],
	pl37_super_09[], pl37_super_09_loop[], pl37_super_18[], pl37_super_00[], pl37_super_00_loop[],
	pl37_super_01[], pl37_super_02[], pl37_super_03[], pl37_super_04[], pl37_super_05[],
	pl37_super_28[], pl37_super_06[], pl37_super_15[], pl37_super_16[], pl37_super_17[],
	pl37_super_19[], pl37_super_20[], pl37_super_21[], pl37_super_22[], pl37_super_23[],
	pl37_super_24[], pl37_super_25[], pl37_super_26[], pl37_super_27[], pl37_super_29[],
	pl37_super_11[], pl37_super_12[], pl37_super_12_loop[], pl37_super_13[], pl37_super_30[],
	pl37_effect_05[], pl37_effect_06[], pl37_effect_07[], pl37_damage_00[], pl37_damage_01[],
	pl37_damage_02[], pl37_damage_03[], pl37_damage_04[], pl37_damage_05[], pl37_damage_06[],
	pl37_damage_07[], pl37_damage_08[], pl37_damage_10[], pl37_damage_11[], pl37_damage_12[],
	pl37_damage_13[], pl37_damage_09[], pl37_damage_09_loop[], pl37_damage_16[], pl37_damage_17[],
	pl37_damage_18[], pl37_damage_19[], pl37_damage_20[], pl37_damage_21[], pl37_damage_22[],
	pl37_damage_23[], pl37_damage_24[], pl37_damage_25[], pl37_damage_26[], pl37_damage_27[],
	pl37_damage_28[], pl37_damage_29[], pl37_damage_30[], pl37_damage_31[], pl37_damage_32[],
	pl37_dizzy_00[], pl37_unused_00[], pl37_get_up_00[], pl37_get_up_01[], pl37_unused_01[],
	pl37_get_up_02[], pl37_get_up_03[], pl37_appear_00[], pl37_appear_01[], pl37_appear_01_loop[],
	pl37_appear_02[], pl37_extra_04[], pl37_extra_05[], pl37_extra_06[], pl37_extra_07[],
	pl37_extra_08[], pl37_change_04[], pl37_change_04_loop[], pl37_effect_09[], pl37_extra_00[],
	pl37_extra_01[], pl37_effect_08[], pl37_extra_03[], pl37_win_00[], pl37_win_00_loop[],
	pl37_win_03[], pl37_win_03_loop[], pl37_effect_10[], pl37_win_01[], pl37_win_01_loop[],
	pl37_win_02[], pl37_win_02_loop[], pl37_change_00[], pl37_change_00_loop[], pl37_change_02[],
	pl37_change_01[], pl37_change_03[], pl37_caught_00[], pl37_caught_01[], pl37_caught_02[],
	pl37_caught_03[], pl37_caught_04[], pl37_caught_05[], pl37_caught_06[], pl37_caught_07[],
	pl37_caught_08[], pl37_caught_09[], pl37_caught_10[], pl37_caught_11[], pl37_caught_12[],
	pl37_caught_13[], pl37_caught_14[], pl37_caught_15[], pl37_caught_16[], pl37_caught_17[],
	pl37_caught_18[], pl37_caught_19[], pl37_caught_20[], pl37_caught_21[], pl37_effect_03[];
extern const Sint32 *const pl37_stand_tbl[], *const pl37_jump_tbl[], *const pl37_dash_tbl[],
	*const pl37_crouch_tbl[], *const pl37_turn_tbl[], *const pl37_guard_tbl[],
	*const pl37_air_guard_tbl[], *const pl37_stand_p_tbl[], *const pl37_stand_k_tbl[],
	*const pl37_crouch_p_tbl[], *const pl37_crouch_k_tbl[], *const pl37_jump_p_tbl[],
	*const pl37_jump_k_tbl[], *const pl37_damage_tbl[], *const pl37_dizzy_tbl[],
	*const pl37_catch_tbl[], *const pl37_caught_tbl[], *const pl37_get_up_tbl[],
	*const pl37_appear_tbl[], *const pl37_win_tbl[], *const pl37_extra_tbl[],
	*const pl37_special_tbl[], *const pl37_super_tbl[], *const pl37_effect_tbl[],
	*const pl37_change_tbl[], *const pl37_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_37[16] = {
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
const Sint32 *const *const char_tbl_37[] = {
	pl37_stand_tbl, pl37_jump_tbl, pl37_dash_tbl, pl37_crouch_tbl, pl37_turn_tbl, pl37_guard_tbl,
	pl37_air_guard_tbl, pl37_stand_p_tbl, pl37_stand_k_tbl, pl37_crouch_p_tbl, pl37_crouch_k_tbl, pl37_jump_p_tbl,
	pl37_jump_k_tbl, pl37_damage_tbl, pl37_dizzy_tbl, pl37_catch_tbl, pl37_caught_tbl, pl37_get_up_tbl,
	pl37_appear_tbl, pl37_win_tbl, pl37_extra_tbl, pl37_special_tbl, pl37_super_tbl, pl37_effect_tbl,
	pl37_change_tbl, pl37_grp25_tbl, 0,
};

const Sint32 *const pl37_stand_tbl[] = {
	pl37_stand_00, pl37_stand_00, pl37_stand_02, pl37_stand_03, pl37_stand_00, pl37_stand_00,
	pl37_stand_06, pl37_stand_07, pl37_stand_00, pl37_stand_00, 0,
};

const Sint32 *const pl37_jump_tbl[] = {
	pl37_jump_00, pl37_jump_01, pl37_jump_02, pl37_jump_03, pl37_jump_03, pl37_jump_03,
	pl37_jump_06, pl37_jump_07, pl37_jump_08, pl37_jump_09, pl37_jump_10, pl37_jump_11,
	0,
};

const Sint32 *const pl37_dash_tbl[] = {
	pl37_dash_00, pl37_dash_01, pl37_dash_02, pl37_dash_03, 0,
};

const Sint32 *const pl37_crouch_tbl[] = {
	pl37_crouch_00, pl37_crouch_01, pl37_crouch_02, 0,
};

const Sint32 *const pl37_turn_tbl[] = {
	pl37_turn_00, pl37_turn_01, 0,
};

const Sint32 *const pl37_guard_tbl[] = {
	pl37_guard_00, pl37_guard_01, pl37_guard_02, pl37_guard_01, pl37_guard_04, pl37_guard_05,
	pl37_guard_06, pl37_guard_05, 0,
};

const Sint32 *const pl37_air_guard_tbl[] = {
	pl37_air_guard_00, pl37_air_guard_01, pl37_air_guard_01, 0,
};

const Sint32 *const pl37_stand_p_tbl[] = {
	pl37_stand_p_00, pl37_stand_p_01, pl37_stand_p_02, pl37_stand_p_03, 0,
};

const Sint32 *const pl37_stand_k_tbl[] = {
	pl37_stand_k_00, pl37_stand_k_01, pl37_stand_k_02, pl37_stand_k_03, 0,
};

const Sint32 *const pl37_crouch_p_tbl[] = {
	pl37_crouch_p_00, pl37_crouch_p_01, pl37_crouch_p_02, pl37_crouch_p_03, 0,
};

const Sint32 *const pl37_crouch_k_tbl[] = {
	pl37_crouch_k_00, pl37_crouch_k_01, pl37_crouch_k_02, pl37_crouch_k_03, 0,
};

const Sint32 *const pl37_jump_p_tbl[] = {
	pl37_jump_p_00, pl37_jump_p_01, pl37_jump_p_02, pl37_jump_p_00, pl37_jump_p_01, pl37_jump_p_02,
	0,
};

const Sint32 *const pl37_jump_k_tbl[] = {
	pl37_jump_k_00, pl37_jump_k_01, pl37_jump_k_02, pl37_jump_k_00, pl37_jump_k_01, pl37_jump_k_02,
	0,
};

const Sint32 *const pl37_damage_tbl[] = {
	pl37_damage_00, pl37_damage_01, pl37_damage_02, pl37_damage_03, pl37_damage_04, pl37_damage_05,
	pl37_damage_06, pl37_damage_07, pl37_damage_08, pl37_damage_09, pl37_damage_10, pl37_damage_11,
	pl37_damage_12, pl37_damage_13, pl37_damage_13, pl37_damage_13, pl37_damage_16, pl37_damage_17,
	pl37_damage_18, pl37_damage_19, pl37_damage_20, pl37_damage_21, pl37_damage_22, pl37_damage_23,
	pl37_damage_24, pl37_damage_25, pl37_damage_26, pl37_damage_27, pl37_damage_28, pl37_damage_29,
	pl37_damage_30, pl37_damage_31, pl37_damage_32, pl37_damage_32, pl37_damage_13, 0,
};

const Sint32 *const pl37_dizzy_tbl[] = {
	pl37_dizzy_00, 0,
};

const Sint32 *const pl37_catch_tbl[] = {
	pl37_catch_00, pl37_catch_01, pl37_catch_02, pl37_catch_03, pl37_catch_04, pl37_catch_05,
	pl37_catch_06, pl37_catch_07, pl37_catch_08, pl37_catch_09, pl37_catch_10, pl37_catch_11,
	pl37_catch_12, pl37_catch_13, pl37_catch_14, pl37_catch_15, pl37_catch_16, pl37_catch_17,
	pl37_catch_18, pl37_catch_04, pl37_catch_05, 0,
};

const Sint32 *const pl37_caught_tbl[] = {
	pl37_caught_00, pl37_caught_01, pl37_caught_02, pl37_caught_03, pl37_caught_04, pl37_caught_05,
	pl37_caught_06, pl37_caught_07, pl37_caught_08, pl37_caught_09, pl37_caught_10, pl37_caught_11,
	pl37_caught_12, pl37_caught_13, pl37_caught_14, pl37_caught_15, pl37_caught_16, pl37_caught_17,
	pl37_caught_18, pl37_caught_19, pl37_caught_20, pl37_caught_21, 0,
};

const Sint32 *const pl37_get_up_tbl[] = {
	pl37_get_up_00, pl37_get_up_01, pl37_get_up_02, pl37_get_up_03, 0,
};

const Sint32 *const pl37_appear_tbl[] = {
	pl37_appear_00, pl37_appear_01, pl37_appear_02, 0,
};

const Sint32 *const pl37_win_tbl[] = {
	pl37_win_00, pl37_win_01, pl37_win_02, pl37_win_03, 0,
};

const Sint32 *const pl37_extra_tbl[] = {
	pl37_extra_00, pl37_extra_01, pl37_extra_02, pl37_extra_03, pl37_extra_04, pl37_extra_05,
	pl37_extra_06, pl37_extra_07, pl37_extra_08, pl37_extra_09, pl37_extra_10, 0,
};

const Sint32 *const pl37_special_tbl[] = {
	pl37_special_00, pl37_special_01, pl37_special_02, pl37_special_03, pl37_special_04, pl37_special_05,
	pl37_special_06, pl37_special_06, pl37_special_08, pl37_special_08, pl37_special_10, 0,
};

const Sint32 *const pl37_super_tbl[] = {
	pl37_super_00, pl37_super_01, pl37_super_02, pl37_super_03, pl37_super_04, pl37_super_05,
	pl37_super_06, pl37_super_06, pl37_super_08, pl37_super_09, pl37_super_10, pl37_super_11,
	pl37_super_12, pl37_super_13, pl37_super_06, pl37_super_15, pl37_super_16, pl37_super_17,
	pl37_super_18, pl37_super_19, pl37_super_20, pl37_super_21, pl37_super_22, pl37_super_23,
	pl37_super_24, pl37_super_25, pl37_super_26, pl37_super_27, pl37_super_28, pl37_super_29,
	pl37_super_30, 0,
};

const Sint32 *const pl37_effect_tbl[] = {
	pl37_effect_00, pl37_effect_01, pl37_effect_02, pl37_effect_03, pl37_effect_03, pl37_effect_05,
	pl37_effect_06, pl37_effect_07, pl37_effect_08, pl37_effect_09, pl37_effect_10, pl37_effect_11,
	pl37_effect_12, pl37_effect_13, pl37_effect_14, pl37_effect_15, pl37_effect_16, pl37_effect_17,
	pl37_effect_18, pl37_effect_19, pl37_effect_20, pl37_effect_21, pl37_effect_22, pl37_effect_23,
	pl37_effect_24, pl37_effect_25, pl37_effect_26, pl37_effect_27, pl37_effect_28, pl37_effect_29,
	0,
};

const Sint32 *const pl37_change_tbl[] = {
	pl37_change_00, pl37_change_01, pl37_change_02, pl37_change_03, pl37_change_04, 0,
};

const Sint32 *const pl37_grp25_tbl[] = {
	pl37_stand_00, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl37_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_stand_00 ),
};

/* Char_Stand 2: win_ko_init, act_walk (2 + no) */
const Sint32 pl37_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl37_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl37_stand_06[] = {
	CG_FRAME(    0,    0,   5,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl37_stand_07[] = {
	CG_FRAME(    0,    0,   6,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl37_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl37_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   106, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl37_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   107, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   108, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl37_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl37_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl37_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   124, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5: appear_jump */
const Sint32 pl37_jump_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl37_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   275, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   277, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl37_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   279, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl37_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   282, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   283, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   284, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   285, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   286, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl37_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   288, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl37_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   291, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   292, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   293, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   294, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   295, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl37_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   297, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl37_dash_00[] = {
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x00,             0x1f,  0x00,       0x03,    0,   0,     0,      0,   4 ),
};

const Sint32 pl37_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl37_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl37_dash_02[] = {
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl37_dash_01[] = {
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x20,             0x1f,  0x00,       0x03,    0,   0,     0,      0,   4 ),
};

const Sint32 pl37_dash_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl37_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl37_dash_03[] = {
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl37_crouch_00[] = {
	CG_FRAME(    0,    0,   2,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl37_crouch_01[] = {
	CG_FRAME(    0,    0,   3,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl37_crouch_02[] = {
	CG_FRAME(    0,    0,   6,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl37_turn_00[] = {
	CG_FRAME(    0,    0,   6,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl37_turn_01[] = {
	CG_FRAME(    0,    0,   6,           0,   134, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   135, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   136, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl37_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl37_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl37_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   137, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl37_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl37_guard_05[] = {
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl37_guard_06[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl37_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   159, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   162, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl37_air_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   162, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   165, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl37_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   342, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   1,           0,   343, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   344, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   7,           0,   344, 0x90,   0x6d,              ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   345, 0x90,   0x6d,              ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl37_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl37_stand_p_03[] = {
	CG_FRAME(    0,    1,   3,           0,   345, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   1,           0,   343, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   344, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   345, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl37_stand_p_03 ),
};

/* Char_Stand_P 1 (medium): punch_init (Standing medium punch (assist)) */
const Sint32 pl37_stand_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   347, 0x90,   0x73,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   347, 0x90,   0x73,       ATK | 0x16,  0x00,       0x05,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   348, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   349, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   350, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   351, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   352, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   353, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   354, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   355, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   356, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   366, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_NEXT( pl37_stand_p_01 ),
};

/* Char_Extra 9: upper_init */
const Sint32 pl37_extra_09[] = {
	CG_FRAME(    0,    0,   1,           0,   347, 0x90,   0x73,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0,   347, 0x90,   0x73,       ATK | 0x16,  0x00,       0x05,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   348, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   349, 0x90,   0x0f,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   350, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   351, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   352, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   353, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   354, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   355, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   356, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   366, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_NEXT( pl37_extra_09 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl37_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   367, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0,   368, 0x90,   0x00,       ATK | 0x17,  0x00,       0x07,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x90,   0xa7,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    1,   6,           0,   370, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   3,           0,   371, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   3,           0,   372, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    2,   2,           0,   374, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   380, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   382, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   383, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_NEXT( pl37_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl37_stand_k_00[] = {
	CG_FRAME(    0,   16,   6,           0,   384, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   32,   2,           0,   385, 0x90,   0x00,       ATK | 0x1f,  0x00,       0x03,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,   12,   2,           0,   386, 0x90,   0x15,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME( 0x80,    0,   2,           0,   387, 0x90,   0x15,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME( 0x80,    0,   4,           0,   386, 0x90,   0x15,              ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   6,           0,   387, 0x90,   0x15,              ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,  -24,   8,           0,   388, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,   -4,   4,           0,   389, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   -2,   2,           0,   390, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   -2,   0,           0,   391, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   392, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl37_stand_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   393, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,   393, 0x90,   0x00,       ATK | 0x1c,  0x00,       0x04,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   395, 0x90,   0x20, ATK | CG_ATCK_NG | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,  81 ),
	CG_FRAME( 0x80,    2,   2,           0,   396, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x80,    1,   2,           0,   397, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,   2,           0,   398, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   0,           0,   399, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   0,           0,   400, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   0,           0,   399, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   0,           0,   400, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    4,   2,           0,   401, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   403, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    8,   2,           0,   404, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   405, 0x90,   0x20, ATK | CG_ATCK_NG | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   2,           0,   406, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   407, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   408, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    4,   4,           0,   409, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4,           0,   410, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   0,           0,   409, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   0,           0,   410, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   412, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl37_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   486, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   487, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl37_stand_k_01_loop ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl37_stand_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   413, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4,           0,   416, 0x90,   0x20, ATK | CG_ATCK_NG | 0x0e,  0x00,       0x1c,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4,           0,   417, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   0,           0,   416, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   0,           0,   417, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  30 ),
};

const Sint32 pl37_stand_k_02_loop[] = {
	CG_FRAME( 0x80,    1,   2,           0,   418, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2,           0,   419, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2,           0,   420, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   421, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_NEXT( pl37_stand_k_02_loop ),
};

/* Char_Stand_K 3: stand_hk_move */
const Sint32 pl37_stand_k_03[] = {
	CG_FRAME(    0,    0,   2,           0,   422, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   423, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   423, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_stand_k_03 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl37_crouch_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   424, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   425, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   1,           0,   426, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   8,           0,   426, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   424, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_NEXT( pl37_crouch_p_00 ),
};

/* Char_Crouch_P 3 */
const Sint32 pl37_crouch_p_03[] = {
	CG_FRAME(    0,    0,   2,           0,   424, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   425, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   426, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   8,           0,   426, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   427, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_NEXT( pl37_crouch_p_03 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl37_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   430, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   431, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   432, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   433, 0x90,   0x00,          ATK | 2,  0x00,       0x03,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x80,    0,   2,           0,   434, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   435, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   436, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   437, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   438, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   439, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   441, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   0,           0,   442, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   443, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl37_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl37_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   444, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   445, 0x90,   0x6d,       ATK | 0x19,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   446, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   447, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   448, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   449, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   450, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,   2,           0,   451, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,   2,           0,   452, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   456, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl37_crouch_p_02 ),
};

/* Char_Special 10: snapback_init (Snapback) */
const Sint32 pl37_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   444, 0x90,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   445, 0x90,   0x6d,       ATK | 0x19,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   445, 0x90,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   446, 0x90,   0x8a,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   447, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   448, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   449, 0x90,   0x8a,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   450, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,   2,           0,   451, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,   2,           0,   452, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   456, 0x90,   0x8a,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl37_special_10 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl37_crouch_k_00[] = {
	CG_FRAME(    0,    0,   8,           0,   462, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   464, 0x90,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   4,           0,   465, 0x90,   0x83,       ATK | 0x1e,  0x00,       0x06,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   4,           0,   466, 0x90,   0x83,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   4,           0,   465, 0x90,   0x83,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x90,   0x83,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,  10,           0,   467, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   6,           0,   468, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   469, 0x90,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl37_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl37_crouch_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   470, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   472, 0x90,   0x20, ATK | CG_ATCK_NG | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    1,   1,           0,   473, 0x90,   0x20, ATK | CG_ATCK_NG | 0x1d,  0x00,       0x06,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   474, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   475, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   488, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    2,   2,           0,   476, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   477, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   476, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   477, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    8,   1,           0,   480, 0x90,   0x20, ATK | CG_ATCK_NG | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   481, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   2,           0,   482, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   2,           0,   489, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    4,   4,           0,   483, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   0,           0,   483, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   484, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
};

const Sint32 pl37_crouch_k_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   486, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   487, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl37_crouch_k_01_loop ),
};

/* Char_Extra 10: upper_init */
const Sint32 pl37_extra_10[] = {
	CG_FRAME(    0,    0,   6,           0,   470, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   472, 0x90,   0x20, ATK | CG_ATCK_NG | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    1,   1,           0,   473, 0x90,   0x20, ATK | CG_ATCK_NG | 0x1d,  0x00,       0x06,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   474, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   475, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   488, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    2,   2,           0,   476, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   477, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   476, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   477, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    8,   1,           0,   480, 0x90,   0x20, ATK | CG_ATCK_NG | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   481, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   2,           0,   482, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,   2,           0,   489, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    4,   4,           0,   483, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   0,           0,   483, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   0,           0,   484, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
};

const Sint32 pl37_extra_10_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   486, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   487, 0x90,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl37_extra_10_loop ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl37_crouch_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   490, 0x90,   0x00,       ATK | 0x0e,  0x00,       0x1c,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   491, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   6,           0,   492, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl37_crouch_k_02_loop[] = {
	CG_FRAME( 0x80,    0,   2,           0,   495, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME( 0x80,    0,   2,           0,   496, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME( 0x80,    0,   2,           0,   497, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   498, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl37_crouch_k_02_loop ),
};

/* Char_Crouch_K 3: crouch_hk_slide */
const Sint32 pl37_crouch_k_03[] = {
	CG_FRAME(    0,    0,   2,           0,   499, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   499, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   500, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl37_crouch_k_03 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl37_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   501, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   504, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl37_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl37_jump_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   505, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   508, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   509, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   510, 0x90,   0x20,       ATK | 0x1b,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   511, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   512, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   513, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   2,           0,   515, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   2,           0,   516, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   2,           0,   517, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   514, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl37_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl37_jump_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   518, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   7,           0,   519, 0x90,   0x20,       ATK | 0x18,  0x00,       0x08,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   520, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   521, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   522, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   523, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   524, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   525, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   0,           0,   529, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   529, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   0,           0,   530, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   531, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   532, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   533, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   534, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   535, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   536, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   526, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   527, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   528, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl37_jump_p_02 ),
};

/* Char_Jump_K 0 (light): jump_kick
   Char_Jump_K 3 */
const Sint32 pl37_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   537, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   538, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   539, 0x90,   0x20,       ATK | 0x1e,  0x00,       0x06,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   540, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   539, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   6,           0,   540, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   541, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl37_jump_k_00 ),
};

/* Char_Jump_K 1 (medium): jump_kick
   Char_Jump_K 4 */
const Sint32 pl37_jump_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   479, 0x90,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   480, 0x90,   0x20,       ATK | 0x1f,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   481, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    0,   2,           0,   482, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    2,   4,           0,   483, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   0,           0,   483, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   0,           0,   484, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   485, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl37_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy): jump_kick
   Char_Jump_K 5 */
const Sint32 pl37_jump_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   542, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   543, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   544, 0x90,   0x20,          ATK | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   1,           0,   545, 0x90,   0x20,       ATK | 0x1c,  0x00,       0x04,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   2,           0,   546, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   2,           0,   547, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   2,           0,   548, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    2,   2,           0,   560, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    0,   0,           0,   561, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   0,           0,   549, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   550, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   551, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   552, 0x90,   0x20,          ATK | 7,  0x00, 0x15 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    8,   1,           0,   553, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   2,           0,   554, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   2,           0,   555, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    4,   2,           0,   562, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   563, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   557, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   559, 0x90,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl37_jump_k_02 ),
};

/* Char_Extra 2: jump_kick, fly_start5 */
const Sint32 pl37_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   564, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x90,   0x20,       ATK | 0x0e,  0x00,       0x1c,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,   570, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl37_extra_02_loop[] = {
	CG_FRAME( 0x80,    2,   2,           0,   565, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME( 0x80,    0,   2,           0,   566, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME( 0x80,    0,   2,           0,   567, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   568, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl37_extra_02_loop ),
};

/* Char_Effect 26 */
const Sint32 pl37_effect_26[] = {
	CG_FRAME(    0,    0,   2,           0,   743, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   744, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   745, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   746, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   747, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   748, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_26 ),
};

/* Char_Effect 27 */
const Sint32 pl37_effect_27[] = {
	CG_FRAME(    0,    0,   2,           0,   749, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   750, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   751, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   752, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   753, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   754, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_27 ),
};

/* Char_Effect 28 */
const Sint32 pl37_effect_28[] = {
	CG_FRAME(    0,    0,   2,           0,   758, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   759, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   760, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   762, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   763, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_28 ),
};

/* Char_Effect 29: pls37_00_init13 */
const Sint32 pl37_effect_29[] = {
	CG_FRAME(    0,    0,   2,           0,   755, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   756, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   757, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_29 ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl37_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   571, 0x80,   0x00,                2,  0x00,       0x03,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   572, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   4,           0,   573, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   4,           0,   574, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0005,   0 ),
};

const Sint32 pl37_catch_00_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   575, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   1,           0,   576, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME( 0x80,    1,   4,           0,   577, 0x80,   0x00,             0x1b,  0xff,       0x04,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   578, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME( 0x82,    0,   4,           0,   577, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   6,           0,   579, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   1,           0,   580, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME( 0x80,    1,   4,           0,   581, 0x80,   0x00,             0x1b,  0xff,       0x04,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   582, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME( 0x82,    0,   4, END_OF_DATA,   581, 0x80,   0x00,                0,  0xff,          0,    0,   0,     0, 0x000b,   0 ),
	CG_NEXT( pl37_catch_00_loop ),
};

/* Char_Catch 1: nml_k_mash */
const Sint32 pl37_catch_01[] = {
	CG_FRAME(    0,    1,   4,           0,   583, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   584, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl37_catch_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_catch_01_loop ),
};

/* Char_Catch 2: nml_k_fly */
const Sint32 pl37_catch_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_catch_02 ),
};

/* Char_Catch 3: nml_p_set */
const Sint32 pl37_catch_03[] = {
	CG_FRAME(    0,    1,   2,           0,   585, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    1,   8,           0,   586, 0x80,   0x00,             0x1a,  0x00,       0x03,    0,   0,     0, 0x000d,   0 ),
};

const Sint32 pl37_catch_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   587, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   588, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_NEXT( pl37_catch_03_loop ),
};

/* Char_Catch 4: nml_p_jump
   Char_Catch 19: jump_p_jump */
const Sint32 pl37_catch_04[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl37_catch_04_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_catch_04_loop ),
};

/* Char_Catch 5: nml_p_fall
   Char_Catch 20 */
const Sint32 pl37_catch_05[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_catch_05 ),
};

/* Char_Catch 6: crush_c_set */
const Sint32 pl37_catch_06[] = {
	CG_FRAME(    0,    0,   6,           0,   589, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   2,           0,   590, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   2,           0,   591, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   8,           0,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    1,   3,           0,   594, 0x80,   0x00,             0x1a,  0x00,       0x03,    0,   0,     0, 0x0015,   0 ),
};

/* Char_Catch 7: crush_c_down */
const Sint32 pl37_catch_07[] = {
	CG_FRAME(    0,    0,   3,           0,   593, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   594, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_NEXT( pl37_catch_07 ),
};

/* Char_Catch 8: crush_c_stop */
const Sint32 pl37_catch_08[] = {
	CG_FRAME(    0,    0,   2,           0,   595, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   596, 0x80,   0x00,             0x1b,  0x00,       0x04,    0,   0,     0, 0x0017,   0 ),
	CG_NEXT( pl37_catch_08 ),
};

/* Char_Catch 9: crush_c_run */
const Sint32 pl37_catch_09[] = {
	CG_FRAME(    0,    0,   3,           0,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   598, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_NEXT( pl37_catch_09 ),
};

/* Char_Catch 10 */
const Sint32 pl37_catch_10[] = {
	CG_FRAME(    0,    0,   2,           0,   599, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001b,   0 ),
	CG_NEXT( pl37_catch_10 ),
};

/* Char_Catch 11: crush_c_up */
const Sint32 pl37_catch_11[] = {
	CG_FRAME(    0,    0,   3,           0,   601, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   602, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_NEXT( pl37_catch_11 ),
};

/* Char_Catch 12 */
const Sint32 pl37_catch_12[] = {
	CG_FRAME(    0,    0,   2,           0,   603, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x001f,   0 ),
	CG_NEXT( pl37_catch_12 ),
};

/* Char_Catch 13: crush_c_back */
const Sint32 pl37_catch_13[] = {
	CG_FRAME(    0,    0,   3,           0,   605, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0021,   0 ),
	CG_NEXT( pl37_catch_13 ),
};

/* Char_Catch 14 */
const Sint32 pl37_catch_14[] = {
	CG_FRAME(    0,    0,   2,           0,   607, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0022,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   608, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0023,   0 ),
	CG_NEXT( pl37_catch_14 ),
};

/* Char_Catch 15: crush_c_throw */
const Sint32 pl37_catch_15[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl37_catch_15_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_catch_15_loop ),
};

/* Char_Catch 16: crush_c_fall */
const Sint32 pl37_catch_16[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_catch_16 ),
};

/* Char_Catch 17: crush_init (Saotome Crash) */
const Sint32 pl37_catch_17[] = {
	CG_FRAME(    0,    1,   1,           0,    84, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    1,  13,           0,    84, 0x80,   0x00,       ATK | 0x10,  0x00,       0x15,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,  10,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,  10,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    2,   5,           0,   704, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   5,           0,   705, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   5,           0,   704, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   5,           0,   705, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   706, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl37_catch_17 ),
};

/* Char_Effect 22: pls37_00_init8 (22 + sub_type) */
const Sint32 pl37_effect_22[] = {
	CG_FRAME(    0,    0,   2,           0,   719, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   720, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   721, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   722, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   723, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   724, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl37_effect_23[] = {
	CG_FRAME(    0,    0,   2,           0,   725, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   726, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   727, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   728, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   729, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   730, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl37_effect_24[] = {
	CG_FRAME(    0,    0,   2,           0,   731, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   732, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   733, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   734, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   735, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   736, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl37_effect_25[] = {
	CG_FRAME(    0,    0,   2,           0,   737, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   738, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   739, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   740, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   741, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   742, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_25 ),
};

/* Char_Catch 18: jump_p_set */
const Sint32 pl37_catch_18[] = {
	CG_FRAME(    0,    1,   4,           0,   631, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0024,   0 ),
	CG_FRAME(    0,    1,   4,           0,   632, 0x80,   0x00,             0x1a,  0x00,       0x03,    0,   0,     0, 0x0025,   0 ),
};

const Sint32 pl37_catch_18_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   633, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0026,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   634, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0027,   0 ),
	CG_NEXT( pl37_catch_18_loop ),
};

/* Char_Special 6: dynamite_init (Saotome Dynamite, 6 + atck_level)
   Char_Special 7 */
const Sint32 pl37_special_06[] = {
	CG_FRAME(    0,    0,   1,           0,   609, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   610, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   613, 0x90,   0x00,       ATK | 0x0c,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,   4,           0,   613, 0x90,   0x00,          ATK | 9,  0x00, 0x4a | CSE,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   613, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  18,           0,   613, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   614, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    8,   3,           0,   616, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl37_special_06_loop[] = {
	CG_FRAME(    0,    4,   3,           0,   615, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    4,   3, END_OF_DATA,   616, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl37_special_06_loop ),
};

/* Char_Special 8: dynamite_move (Saotome Dynamite, 8 + atck_level)
   Char_Special 9 */
const Sint32 pl37_special_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   615, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl37_special_08 ),
};

/* Char_Effect 1: pls37_00_init2 */
const Sint32 pl37_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   650, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   651, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_01 ),
};

/* Char_Effect 2: pls37_00_init3 */
const Sint32 pl37_effect_02[] = {
	CG_FRAME(    0,    0,   3,           0,   652, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   653, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   654, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   655, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   656, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   657, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   658, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32769, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   660, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   661, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   662, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_02 ),
};

/* Char_Effect 21: pls37_00_init6 */
const Sint32 pl37_effect_21[] = {
	CG_FRAME(    0,    0,   2,           0,   711, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   712, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   715, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   716, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   717, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   718, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_21 ),
};

/* Char_Effect 11 */
const Sint32 pl37_effect_11[] = {
	CG_FRAME(    0,    0,   2,           0, 32782, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32783, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl37_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl37_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32784, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32785, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl37_effect_13[] = {
	CG_FRAME(    0,    0,   2,           0, 32786, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32787, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl37_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl37_effect_14[] = {
	CG_FRAME(    0,    0,   2,           0, 32788, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32789, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl37_effect_15[] = {
	CG_FRAME(    0,    0,   2,           0, 32790, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32791, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl37_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl37_effect_16[] = {
	CG_FRAME(    0,    0,   2,           0, 32792, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32793, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl37_effect_17[] = {
	CG_FRAME(    0,    0,   2,           0, 32794, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32795, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl37_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl37_effect_18[] = {
	CG_FRAME(    0,    0,   2,           0, 32796, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32797, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl37_effect_19[] = {
	CG_FRAME(    0,    0,   2,           0, 32798, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32799, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl37_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl37_effect_20[] = {
	CG_FRAME(    0,    0,   2,           0, 32800, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32801, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl37_effect_20 ),
};

/* Char_Special 0 */
const Sint32 pl37_special_00[] = {
	CG_FRAME(    0,    6,   2,           0,   619, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   623, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl37_special_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   624, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   625, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   629, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   630, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl37_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl37_special_01[] = {
	CG_FRAME(    0,   10,   2,           0,   619, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   622, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   623, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl37_special_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   624, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   625, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   629, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   630, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl37_special_01_loop ),
};

/* Char_Special 2: typhoon_wait (Saotome Typhoon, 2 + atck_level) */
const Sint32 pl37_special_02[] = {
	CG_FRAME(    0,   32,   1,           0,   626, 0x90,   0x00,       ATK | 0x0d,  0x00,       0x1b,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   1,           0,   627, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   3,           0,   628, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   2,           0,   680, 0x90,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  61 ),
};

const Sint32 pl37_special_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   681, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   685, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   686, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   687, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   680, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl37_special_02_loop ),
};

/* Char_Special 3 */
const Sint32 pl37_special_03[] = {
	CG_FRAME(    0,   56,   1,           0,   626, 0x90,   0x00,       ATK | 0x0d,  0x00,       0x1b,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   1,           0,   627, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   3,           0,   628, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   2,           0,   680, 0x90,   0x00,              ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  61 ),
};

const Sint32 pl37_special_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   681, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   685, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   686, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   687, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   680, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl37_special_03_loop ),
};

/* Char_Special 4: typhoon_dash (Saotome Typhoon, 4 + atck_level) */
const Sint32 pl37_special_04[] = {
	CG_FRAME(    0,    0,   6,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_special_04 ),
};

/* Char_Special 5 */
const Sint32 pl37_special_05[] = {
	CG_FRAME(    0,    0,   6,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  22,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_special_05 ),
};

/* Char_Effect 0: pls37_00_init1 */
const Sint32 pl37_effect_00[] = {
	CG_FRAME(    0,    0,   4,           0,   688, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   689, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   690, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   691, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_00 ),
};

/* Char_Super 10: pls37_00_init15, shl37_02_init */
const Sint32 pl37_super_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32768, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl37_super_10 ),
};

/* Char_Super 8: blodia_init (Blodia Punch (hyper)) */
const Sint32 pl37_super_08[] = {
	CG_FRAME(    0,    0,   8,           0,   764, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x90,   0x00,       ATK | 0x13,  0x00,       0x16,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   767, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   768, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   765, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl37_super_08_loop[] = {
	CG_FRAME( 0x80,    0,   2,           0,   766, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   767, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   768, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x82,    0,   2, END_OF_DATA,   765, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_super_08_loop ),
};

/* Char_Super 9: blodia_wait (Blodia Punch (hyper)) */
const Sint32 pl37_super_09[] = {
	CG_FRAME(    0,    0,   1,           0,   367, 0x90,   0x00,       ATK | 0x11,  0x00,       0x17,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   369, 0x90,   0xa7,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    1,   6,           0,   370, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x80,    0,   3,           0,   371, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x80,    0,   3,           0,   372, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x80,    0,   3,           0,   373, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    2,   3,           0,   374, 0x90,   0x03,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
};

const Sint32 pl37_super_09_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   377, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   382, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_NEXT( pl37_super_09_loop ),
};

/* Char_Super 18: blodia_hold (Blodia Punch (hyper)) */
const Sint32 pl37_super_18[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   383, 0x90,   0x03,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_super_18 ),
};

/* Char_Super 0: vulcan_init (Blodia Vulcan (hyper)) */
const Sint32 pl37_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   663, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   664, 0x90,   0x00,       ATK | 0x12,  0x00,       0x04,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   665, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   666, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   667, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,   4,           0,   668, 0x90,   0x00,       ATK | 0x15,  0x00,       0x20,    0,   0,     0,      0,   4 ),
};

const Sint32 pl37_super_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   669, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   670, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   671, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   672, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_super_00_loop ),
};

/* Char_Super 1 */
const Sint32 pl37_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   671, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   672, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_super_01 ),
};

/* Char_Super 2: shl37_01_0_init, shl37_01_0_ride */
const Sint32 pl37_super_02[] = {
	CG_FRAME(    0,    0,   1,           0,   635, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   636, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   637, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_02 ),
};

/* Char_Super 3: shl37_01_1_init */
const Sint32 pl37_super_03[] = {
	CG_FRAME(    0,    0,   2,           0, 32770, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32771, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32777, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_03 ),
};

/* Char_Super 4: shl37_01_2_init */
const Sint32 pl37_super_04[] = {
	CG_FRAME(    0,    0,   2,           0,   673, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   674, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   675, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   676, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_04 ),
};

/* Char_Super 5: shl37_01_3_init */
const Sint32 pl37_super_05[] = {
	CG_FRAME(    0,    0,   1,           0,   678, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   679, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl37_super_05 ),
};

/* Char_Super 28: shl37_01_7_init */
const Sint32 pl37_super_28[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   679, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl37_super_28 ),
};

/* Char_Super 6: shl37_01_3_fly, shl37_01_7_wait
   Char_Super 7: shl37_01_3_fly
   Char_Super 14: shl37_01_4_init */
const Sint32 pl37_super_06[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32802, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_06 ),
};

/* Char_Super 15: shl37_01_5_init */
const Sint32 pl37_super_15[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32803, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_15 ),
};

/* Char_Super 16 */
const Sint32 pl37_super_16[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   805, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_16 ),
};

/* Char_Super 17 */
const Sint32 pl37_super_17[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   806, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_17 ),
};

/* Char_Super 19 */
const Sint32 pl37_super_19[] = {
	CG_FRAME(    0,    0,   1,           0,   769, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   770, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   771, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   772, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_19 ),
};

/* Char_Super 20 */
const Sint32 pl37_super_20[] = {
	CG_FRAME(    0,    0,   1,           0,   773, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   774, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   775, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   776, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_20 ),
};

/* Char_Super 21: vulcan_rise (Blodia Vulcan (hyper)) */
const Sint32 pl37_super_21[] = {
	CG_FRAME(    0,    1,   6,           0,   125, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   777, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   778, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   779, 0x90,   0xff,       ATK | 0x11,  0x00,       0x17,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_21 ),
};

/* Char_Super 22: vulcan_up (Blodia Vulcan (hyper)) */
const Sint32 pl37_super_22[] = {
	CG_FRAME(    0,    0,   2,           0,   783, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   784, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   785, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   786, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   787, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   788, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   789, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   790, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   791, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   792, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   793, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   794, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   795, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   796, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   797, 0x90,   0xff,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_22 ),
};

/* Char_Super 23: shl37_01_0_wait */
const Sint32 pl37_super_23[] = {
	CG_FRAME(    0,    0,   1,           0,   638, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   639, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   640, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_23 ),
};

/* Char_Super 24: shl37_01_9_init */
const Sint32 pl37_super_24[] = {
	CG_FRAME(    0,    0,   1,           0,   800, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   801, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   802, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   803, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   802, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   801, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   800, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_24 ),
};

/* Char_Super 25: vulcan_fire (Blodia Vulcan (hyper)), vulcan_burn (Blodia Vulcan (hyper)) */
const Sint32 pl37_super_25[] = {
	CG_FRAME(    0,    0,   4,           0,   798, 0x90,   0xff,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   799, 0x90,   0xff,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_25 ),
};

/* Char_Super 26: vulcan_fall (Blodia Vulcan (hyper)) */
const Sint32 pl37_super_26[] = {
	CG_FRAME(    0,    0,   2,           0,   780, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   781, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   782, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   0,           0,   125, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   215, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   219, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   220, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   221, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   222, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   225, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   226, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   227, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   228, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   229, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   241, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_super_26 ),
};

/* Char_Super 27: shl37_01_9_show */
const Sint32 pl37_super_27[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   804, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_super_27 ),
};

/* Char_Super 29: vulcan_burn_fall (Blodia Vulcan (hyper)) */
const Sint32 pl37_super_29[] = {
	CG_FRAME(    0,    0,   2,           0,   780, 0x90,   0xff,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   781, 0x90,   0xff,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   782, 0x90,   0xff,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   4,           0,   105, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   125, 0x90,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_super_29 ),
};

/* Char_Super 11: cyclone_init (Saotome Cyclone (hyper)) */
const Sint32 pl37_super_11[] = {
	CG_FRAME(    0,    0,   4,           0,   619, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   620, 0x90,   0x00,       ATK | 0x0b,  0x00,       0x19,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   621, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   622, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   3,           0,   623, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    1,   3,           0,   624, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x83,    0,   3, END_OF_DATA,   625, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl37_super_11 ),
};

/* Char_Super 12: cyclone_wait (Saotome Cyclone (hyper)) */
const Sint32 pl37_super_12[] = {
	CG_FRAME(    0,   48,   1,           0,   626, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   627, 0x90,   0x00,              ATK,  0x00, 0x4c | CSE,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   628, 0x90,   0x00,       ATK | 0x0f,  0x00,       0x18,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   680, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   681, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   682, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   683, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   684, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   685, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   2,           0,   686, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   2,           0,   687, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
};

const Sint32 pl37_super_12_loop[] = {
	CG_FRAME(    0,    1,   3,           0,   680, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   681, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   682, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   683, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   684, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   685, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2,           0,   686, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   687, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl37_super_12_loop ),
};

/* Char_Super 13: cyclone_spin_move (Saotome Cyclone (hyper)) */
const Sint32 pl37_super_13[] = {
	CG_FRAME(    0,    0,   6,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_super_13 ),
};

/* Char_Super 30: cyclone_hold (Saotome Cyclone (hyper)) */
const Sint32 pl37_super_30[] = {
	CG_FRAME(    0,    0,   6,           0,   131, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  80,           0,   132, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_super_30 ),
};

/* Char_Effect 5: shl37_03_0_init */
const Sint32 pl37_effect_05[] = {
	CG_FRAME(    0,    0,   2,           0, 32778, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   693, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   695, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   699, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_05 ),
};

/* Char_Effect 6: shl37_03_0_bob */
const Sint32 pl37_effect_06[] = {
	CG_FRAME(    0,    0,   4,           0,   700, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   701, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   702, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   703, 0x90,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_06 ),
};

/* Char_Effect 7: pls37_00_init7, shl37_03_1_init */
const Sint32 pl37_effect_07[] = {
	CG_FRAME(    0,    0,   2,           0,   707, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   708, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   709, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   710, 0x90,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_07 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl37_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl37_damage_01[] = {
	CG_FRAME(    0,    0,   1,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   813, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl37_damage_02[] = {
	CG_FRAME(    0,    0,   1,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   3,           0,   814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   3,           0,   815, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   9,           0,   816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl37_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   819, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   820, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl37_damage_04[] = {
	CG_FRAME(    0,    0,   1,           0,   825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   821, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl37_damage_05[] = {
	CG_FRAME(    0,    0,   1,           0,   824, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,   6,           0,   822, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   6,           0,   823, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   824, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl37_damage_06[] = {
	CG_FRAME(    0,    0,   8,           0,   827, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   828, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl37_damage_07[] = {
	CG_FRAME(    0,    0,   1,           0,   827, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   9,           0,   829, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   827, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   828, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl37_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0,   832, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   9,           0,   830, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,   4,           0,   831, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   832, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   833, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl37_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl37_damage_10[] = {
	CG_FRAME(    0,    0,   6,           0,   835, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   836, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl37_damage_11[] = {
	CG_FRAME(    0,    0,   2,           0,   841, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   837, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   841, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   841, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl37_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   840, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   838, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x81,    0,   2,           0,   839, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   840, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   841, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   842, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl37_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   843, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   844, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   845, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   846, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl37_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl37_damage_09[] = {
	CG_FRAME(    0,    0,   5,           0,   847, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   848, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   849, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   852, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   850, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   851, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl37_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   234, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl37_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl37_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   853, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   854, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl37_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   855, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl37_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   856, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl37_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   857, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl37_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   858, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl37_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   853, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   854, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl37_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   855, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl37_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   856, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl37_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   857, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl37_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   858, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl37_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   860, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   861, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   862, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   851, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl37_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl37_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_27 ),
};

/* Char_Damage 28: cyclone_em_init, dm_away_init, dm_bound_fly, dm_piyo, ... */
const Sint32 pl37_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   867, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   868, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   869, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   870, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   871, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   872, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl37_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl37_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   873, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   874, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   875, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   876, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   877, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   878, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl37_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl37_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   863, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   864, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   865, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   866, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl37_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl37_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   234, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl37_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl37_damage_32[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   879, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_damage_32 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl37_dizzy_00[] = {
	CG_FRAME(    0,    0,   4,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_dizzy_00 ),
};

const Sint32 pl37_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   6, END_OF_DATA,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00005d9c,
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl37_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl37_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_get_up_01 ),
};

const Sint32 pl37_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00005f38,
};

/* Char_Get_Up 2 */
const Sint32 pl37_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl37_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl37_appear_00[] = {
	CG_FRAME(    0,    0,   3,           0,   230, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   231, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_appear_00 ),
};

/* Char_Appear 1: appear_wait */
const Sint32 pl37_appear_01[] = {
	CG_FRAME(    0,    1,  10,           0,   232, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   116, 0x80,   0x20,                1,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl37_appear_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   124, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_appear_01_loop ),
};

/* Char_Appear 2: pls37_00_init0 */
const Sint32 pl37_appear_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_appear_02 ),
};

/* Char_Extra 4: chouhatsu_init (Taunt, which charges him up), chouhatsu_charge (Taunt, which charges him up, 4 + charge_level) */
const Sint32 pl37_extra_04[] = {
	CG_FRAME(    0,    0,   8,           0,   641, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   642, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   643, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   642, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl37_extra_05[] = {
	CG_FRAME(    0,    0,   5,           0,   644, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   645, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   646, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   645, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_extra_05 ),
};

/* Char_Extra 6 */
const Sint32 pl37_extra_06[] = {
	CG_FRAME(    0,    0,   2,           0,   647, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   648, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   649, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   648, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_extra_06 ),
};

/* Char_Extra 7 */
const Sint32 pl37_extra_07[] = {
	CG_FRAME(    0,    1,   2,           0,   647, 0x80,   0x73,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   648, 0x80,   0x73,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   649, 0x80,   0x73,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   648, 0x80,   0x73,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl37_extra_07 ),
};

/* Char_Extra 8: chouhatsu_release (Taunt, which charges him up) */
const Sint32 pl37_extra_08[] = {
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   616, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   618, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_extra_08 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl37_change_04[] = {
	CG_FRAME(    0,    0,   1,           0,   641, 0x80,   0x73,                0,  0x00,       0x04,    0,   0,     0,      0,   1 ),
};

const Sint32 pl37_change_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   641, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   642, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   643, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   642, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_change_04_loop ),
};

/* Char_Effect 9 */
const Sint32 pl37_effect_09[] = {
	CG_FRAME(    0,    0,   4,           0,   647, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   648, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   649, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_09 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl37_extra_00[] = {
	CG_FRAME( 0x80,    0,   3,           0,   539, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x82,    0,   3, END_OF_DATA,   540, 0x90,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl37_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl37_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,          ATE | 5,  0x00,       0x0c,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   316, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  20, END_OF_DATA,   317, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_extra_01 ),
};

/* Char_Effect 8: pls37_00_init5 */
const Sint32 pl37_effect_08[] = {
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_08 ),
};

/* Char_Extra 3: shine_init (Saotome Shine (at low vitality)), fly_start5 */
const Sint32 pl37_extra_03[] = {
	CG_FRAME( 0x80,    1,   2,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   323, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   324, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x84,    0,   2, END_OF_DATA,   325, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_extra_03 ),
};

/* Char_Win 0: win_ko_init, win_time */
const Sint32 pl37_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   168, 0x80,   0x53,             0x14,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl37_win_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   178, 0x80,   0x53,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_win_00_loop ),
};

/* Char_Win 3: win_ko_init */
const Sint32 pl37_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   180, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   181, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   182, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   8,           0,   183, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   192, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   195, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,  96,           0,   204, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0x0b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   208, 0x80,   0x20,                8,  0x00,       0x03,    0,   0,     0,      0,   1 ),
};

const Sint32 pl37_win_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   208, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_win_03_loop ),
};

/* Char_Effect 10 */
const Sint32 pl37_effect_10[] = {
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl37_effect_10 ),
};

/* Char_Win 1: win2_move */
const Sint32 pl37_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   298, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl37_win_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_win_01_loop ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl37_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x4b,             0x0a,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   215, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   217, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   221, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl37_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   229, 0x80,   0x4b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_win_02_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl37_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,                4,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl37_change_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl37_change_02[] = {
	CG_FRAME(    0,    0,   2,           0,   116, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   117, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_change_02 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl37_change_01[] = {
	CG_FRAME(    0,    0,   2,           0,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_change_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl37_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   326, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   329, 0x80,   0x00,                3,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   0,           0,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl37_change_03 ),
};

/* Char_Caught 0 */
const Sint32 pl37_caught_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl37_caught_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl37_caught_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl37_caught_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl37_caught_04[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   824, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl37_caught_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   823, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl37_caught_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl37_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl37_caught_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   833, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl37_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl37_caught_08[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   832, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl37_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl37_caught_09[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   841, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl37_caught_10[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   840, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl37_caught_11[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   839, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl37_caught_12[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   845, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl37_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   853, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl37_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   855, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl37_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   856, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl37_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   857, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl37_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   858, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl37_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   867, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl37_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   873, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl37_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl37_caught_20[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   234, 0x80,   0xae,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl37_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl37_caught_21[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   847, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl37_caught_21 ),
};

/* Char_Effect 3
   Char_Effect 4 */
const Sint32 pl37_effect_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl37_effect_03 ),
};
