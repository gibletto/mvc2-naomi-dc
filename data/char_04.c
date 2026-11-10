/* Anakaris (character 04): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_04      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_04     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl04.c) says which group
                   and number each move uses.
   pl04_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl04_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl04_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl04_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_04
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_04, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_04 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl04_stand_00[], pl04_crouch_01[], pl04_stand_02[], pl04_stand_03[],
	pl04_crouch_00[], pl04_crouch_00_loop[], pl04_crouch_02[], pl04_crouch_02_loop[], pl04_turn_00[],
	pl04_turn_01[], pl04_turn_02[], pl04_jump_03[], pl04_jump_04[], pl04_jump_00[],
	pl04_damage_30_loop[], pl04_jump_09[], pl04_jump_12[], pl04_guard_00[], pl04_guard_01[],
	pl04_guard_02[], pl04_guard_04[], pl04_guard_05[], pl04_guard_06[], pl04_air_guard_00[],
	pl04_appear_00[], pl04_appear_00_loop[], pl04_appear_01[], pl04_win_00[], pl04_win_02_loop[],
	pl04_win_02_loop_loop[], pl04_win_01[], pl04_win_01_loop[], pl04_win_02[], pl04_win_05[],
	pl04_win_05_loop[], pl04_damage_00[], pl04_damage_01[], pl04_damage_02[], pl04_damage_03[],
	pl04_damage_04[], pl04_damage_05[], pl04_damage_06[], pl04_damage_07[], pl04_damage_08[],
	pl04_damage_10[], pl04_damage_11[], pl04_damage_12[], pl04_damage_09[], pl04_damage_09_loop[],
	pl04_guard_03[], pl04_guard_07[], pl04_air_guard_02[], pl04_damage_28[], pl04_damage_29[],
	pl04_damage_31[], pl04_damage_16[], pl04_damage_17[], pl04_damage_18[], pl04_damage_19[],
	pl04_damage_20[], pl04_damage_21[], pl04_damage_22[], pl04_damage_23[], pl04_damage_24[],
	pl04_damage_25[], pl04_damage_26[], pl04_damage_13[], pl04_damage_27[], pl04_damage_30[],
	pl04_get_up_01[], pl04_get_up_02[], pl04_get_up_00[], pl04_dizzy_00[], pl04_caught_00[],
	pl04_caught_01[], pl04_caught_02[], pl04_caught_03[], pl04_caught_04[], pl04_caught_05[],
	pl04_caught_06[], pl04_caught_07[], pl04_caught_08[], pl04_caught_09[], pl04_caught_10[],
	pl04_caught_11[], pl04_caught_12[], pl04_caught_13[], pl04_caught_14[], pl04_caught_15[],
	pl04_caught_16[], pl04_caught_17[], pl04_caught_18[], pl04_caught_19[], pl04_caught_20[],
	pl04_caught_21[], pl04_effect_00[], pl04_effect_01[], pl04_effect_02[], pl04_effect_02_loop[],
	pl04_effect_03[], pl04_effect_04[], pl04_effect_05[], pl04_effect_06[], pl04_effect_07[],
	pl04_stand_p_00[], pl04_stand_p_01[], pl04_stand_p_02[], pl04_stand_k_00[], pl04_stand_k_01[],
	pl04_stand_k_02[], pl04_crouch_p_00[], pl04_crouch_p_01[], pl04_crouch_p_02[],
	pl04_crouch_k_00[], pl04_crouch_k_01[], pl04_crouch_k_02[], pl04_jump_p_00[],
	pl04_jump_p_00_loop[], pl04_jump_p_01[], pl04_jump_p_01_loop[], pl04_jump_p_02[],
	pl04_jump_p_02_loop[], pl04_jump_k_00[], pl04_jump_k_00_loop[], pl04_jump_k_01[],
	pl04_jump_k_01_loop[], pl04_jump_k_02[], pl04_jump_k_02_loop[], pl04_extra_08[],
	pl04_extra_08_loop[], pl04_extra_09[], pl04_extra_09_loop[], pl04_extra_10[],
	pl04_extra_10_loop[], pl04_change_00[], pl04_change_00_loop[], pl04_change_02[],
	pl04_change_03[], pl04_change_03_loop[], pl04_change_01[], pl04_extra_00[], pl04_extra_00_loop[],
	pl04_extra_01[], pl04_extra_07[], pl04_dash_00[], pl04_dash_02[], pl04_dash_01[], pl04_dash_03[],
	pl04_extra_04[], pl04_extra_05[], pl04_extra_11[], pl04_extra_14[], pl04_extra_14_loop[],
	pl04_extra_12[], pl04_extra_12_loop[], pl04_extra_13[], pl04_extra_13_loop[], pl04_extra_15[],
	pl04_super_06[], pl04_super_06_loop[], pl04_super_07[], pl04_special_01[], pl04_super_08[],
	pl04_super_08_loop[], pl04_super_09[], pl04_special_02[], pl04_special_03[], pl04_special_04[],
	pl04_special_05[], pl04_special_06[], pl04_special_07[], pl04_super_16[], pl04_super_16_loop[],
	pl04_super_17[], pl04_super_17_loop[], pl04_super_18[], pl04_super_19[], pl04_catch_00[],
	pl04_special_09[], pl04_special_10[], pl04_special_11[], pl04_special_11_loop[],
	pl04_special_21[], pl04_special_21_loop[], pl04_catch_01[], pl04_special_14[], pl04_special_15[],
	pl04_special_16[], pl04_special_16_loop[], pl04_catch_02[], pl04_super_20[],
	pl04_super_20_loop[], pl04_super_21[], pl04_super_22[], pl04_super_22_loop[], pl04_super_23[],
	pl04_super_24[], pl04_super_25[], pl04_super_26[], pl04_super_27[], pl04_grp27_14[],
	pl04_grp27_15[], pl04_super_00[], pl04_super_01[], pl04_super_02[], pl04_super_03[],
	pl04_super_04[], pl04_super_05[], pl04_extra_02[], pl04_extra_02_loop[], pl04_extra_03[],
	pl04_extra_03_loop[], pl04_change_04[], pl04_change_04_loop[], pl04_special_18[],
	pl04_special_18_loop[], pl04_super_10[], pl04_super_10_loop[], pl04_special_19[],
	pl04_special_19_loop[], pl04_special_20[], pl04_special_20_loop[], pl04_super_11[],
	pl04_super_13[], pl04_super_12[], pl04_super_14[], pl04_super_14_loop[], pl04_super_15[],
	pl04_super_15_loop[], pl04_extra_06[], pl04_grp27_00[], pl04_grp27_01[], pl04_grp27_02[],
	pl04_grp27_03[], pl04_grp27_03_loop[], pl04_grp27_04[], pl04_grp27_04_loop[], pl04_grp27_05[],
	pl04_grp27_16[], pl04_grp27_16_loop[], pl04_grp27_17[], pl04_grp27_17_loop[], pl04_grp27_18[],
	pl04_grp27_18_loop[], pl04_grp27_19[], pl04_grp27_19_loop[], pl04_grp27_06[],
	pl04_grp27_06_loop[], pl04_grp27_07[], pl04_grp27_07_loop[], pl04_grp27_08[],
	pl04_grp27_08_loop[], pl04_grp27_09[], pl04_grp27_09_loop[], pl04_grp27_10[],
	pl04_grp27_10_loop[], pl04_grp27_11[], pl04_grp27_11_loop[], pl04_grp27_12[], pl04_grp27_13[];
extern const Sint32 *const pl04_stand_tbl[], *const pl04_jump_tbl[], *const pl04_dash_tbl[],
	*const pl04_crouch_tbl[], *const pl04_turn_tbl[], *const pl04_guard_tbl[],
	*const pl04_air_guard_tbl[], *const pl04_stand_p_tbl[], *const pl04_stand_k_tbl[],
	*const pl04_crouch_p_tbl[], *const pl04_crouch_k_tbl[], *const pl04_jump_p_tbl[],
	*const pl04_jump_k_tbl[], *const pl04_damage_tbl[], *const pl04_dizzy_tbl[],
	*const pl04_catch_tbl[], *const pl04_caught_tbl[], *const pl04_get_up_tbl[],
	*const pl04_appear_tbl[], *const pl04_win_tbl[], *const pl04_extra_tbl[],
	*const pl04_special_tbl[], *const pl04_super_tbl[], *const pl04_effect_tbl[],
	*const pl04_change_tbl[], *const pl04_grp25_tbl[], *const pl04_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_04[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 4000, 4000, 16384, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_04[] = {
	pl04_stand_tbl, pl04_jump_tbl, pl04_dash_tbl, pl04_crouch_tbl, pl04_turn_tbl, pl04_guard_tbl,
	pl04_air_guard_tbl, pl04_stand_p_tbl, pl04_stand_k_tbl, pl04_crouch_p_tbl, pl04_crouch_k_tbl, pl04_jump_p_tbl,
	pl04_jump_k_tbl, pl04_damage_tbl, pl04_dizzy_tbl, pl04_catch_tbl, pl04_caught_tbl, pl04_get_up_tbl,
	pl04_appear_tbl, pl04_win_tbl, pl04_extra_tbl, pl04_special_tbl, pl04_super_tbl, pl04_effect_tbl,
	pl04_change_tbl, pl04_grp25_tbl, pl04_stand_tbl, pl04_grp27_tbl, 0,
};

const Sint32 *const pl04_stand_tbl[] = {
	pl04_stand_00, pl04_stand_00, pl04_stand_02, pl04_stand_03, pl04_stand_00, pl04_stand_00,
	pl04_stand_02, pl04_stand_03, 0,
};

const Sint32 *const pl04_jump_tbl[] = {
	pl04_jump_00, pl04_jump_00, pl04_jump_00, pl04_jump_03, pl04_jump_04, pl04_jump_04,
	pl04_jump_00, pl04_jump_00, pl04_jump_00, pl04_jump_09, pl04_jump_09, pl04_jump_09,
	pl04_jump_12, 0,
};

const Sint32 *const pl04_dash_tbl[] = {
	pl04_dash_00, pl04_dash_01, pl04_dash_02, pl04_dash_03, pl04_jump_12, 0,
};

const Sint32 *const pl04_crouch_tbl[] = {
	pl04_crouch_00, pl04_crouch_01, pl04_crouch_02, 0,
};

const Sint32 *const pl04_turn_tbl[] = {
	pl04_turn_00, pl04_turn_01, pl04_turn_02, 0,
};

const Sint32 *const pl04_guard_tbl[] = {
	pl04_guard_00, pl04_guard_01, pl04_guard_02, pl04_guard_03, pl04_guard_04, pl04_guard_05,
	pl04_guard_06, pl04_guard_07, 0,
};

const Sint32 *const pl04_air_guard_tbl[] = {
	pl04_air_guard_00, pl04_air_guard_00, pl04_air_guard_02, 0,
};

const Sint32 *const pl04_stand_p_tbl[] = {
	pl04_stand_p_00, pl04_stand_p_01, pl04_stand_p_02, 0,
};

const Sint32 *const pl04_stand_k_tbl[] = {
	pl04_stand_k_00, pl04_stand_k_01, pl04_stand_k_02, 0,
};

const Sint32 *const pl04_crouch_p_tbl[] = {
	pl04_crouch_p_00, pl04_crouch_p_01, pl04_crouch_p_02, 0,
};

const Sint32 *const pl04_crouch_k_tbl[] = {
	pl04_crouch_k_00, pl04_crouch_k_01, pl04_crouch_k_02, 0,
};

const Sint32 *const pl04_jump_p_tbl[] = {
	pl04_jump_p_00, pl04_jump_p_01, pl04_jump_p_02, pl04_jump_p_00, pl04_jump_p_01, pl04_jump_p_02,
	0,
};

const Sint32 *const pl04_jump_k_tbl[] = {
	pl04_jump_k_00, pl04_jump_k_01, pl04_jump_k_02, pl04_jump_k_00, pl04_jump_k_01, pl04_jump_k_02,
	0,
};

const Sint32 *const pl04_damage_tbl[] = {
	pl04_damage_00, pl04_damage_01, pl04_damage_02, pl04_damage_03, pl04_damage_04, pl04_damage_05,
	pl04_damage_06, pl04_damage_07, pl04_damage_08, pl04_damage_09, pl04_damage_10, pl04_damage_11,
	pl04_damage_12, pl04_damage_13, pl04_damage_13, pl04_damage_13, pl04_damage_16, pl04_damage_17,
	pl04_damage_18, pl04_damage_19, pl04_damage_20, pl04_damage_21, pl04_damage_22, pl04_damage_23,
	pl04_damage_24, pl04_damage_25, pl04_damage_26, pl04_damage_27, pl04_damage_28, pl04_damage_29,
	pl04_damage_30, pl04_damage_31, pl04_damage_30, pl04_damage_27, pl04_damage_13, 0,
};

const Sint32 *const pl04_dizzy_tbl[] = {
	pl04_dizzy_00, 0,
};

const Sint32 *const pl04_catch_tbl[] = {
	pl04_catch_00, pl04_catch_01, pl04_catch_02, 0,
};

const Sint32 *const pl04_caught_tbl[] = {
	pl04_caught_00, pl04_caught_01, pl04_caught_02, pl04_caught_03, pl04_caught_04, pl04_caught_05,
	pl04_caught_06, pl04_caught_07, pl04_caught_08, pl04_caught_09, pl04_caught_10, pl04_caught_11,
	pl04_caught_12, pl04_caught_13, pl04_caught_14, pl04_caught_15, pl04_caught_16, pl04_caught_17,
	pl04_caught_18, pl04_caught_19, pl04_caught_20, pl04_caught_21, 0,
};

const Sint32 *const pl04_get_up_tbl[] = {
	pl04_get_up_00, pl04_get_up_01, pl04_get_up_02, pl04_get_up_01, 0,
};

const Sint32 *const pl04_appear_tbl[] = {
	pl04_appear_00, pl04_appear_01, 0,
};

const Sint32 *const pl04_win_tbl[] = {
	pl04_win_00, pl04_win_01, pl04_win_02, pl04_win_01, pl04_win_00, pl04_win_05,
	pl04_win_05, 0,
};

const Sint32 *const pl04_extra_tbl[] = {
	pl04_extra_00, pl04_extra_01, pl04_extra_02, pl04_extra_03, pl04_extra_04, pl04_extra_05,
	pl04_extra_06, pl04_extra_07, pl04_extra_08, pl04_extra_09, pl04_extra_10, pl04_extra_11,
	pl04_extra_12, pl04_extra_13, pl04_extra_14, pl04_extra_15, 0,
};

const Sint32 *const pl04_special_tbl[] = {
	pl04_extra_15, pl04_special_01, pl04_special_02, pl04_special_03, pl04_special_04, pl04_special_05,
	pl04_special_06, pl04_special_07, pl04_catch_00, pl04_special_09, pl04_special_10, pl04_special_11,
	pl04_catch_01, pl04_catch_00, pl04_special_14, pl04_special_15, pl04_special_16, pl04_catch_02,
	pl04_special_18, pl04_special_19, pl04_special_20, pl04_special_21, 0,
};

const Sint32 *const pl04_super_tbl[] = {
	pl04_super_00, pl04_super_01, pl04_super_02, pl04_super_03, pl04_super_04, pl04_super_05,
	pl04_super_06, pl04_super_07, pl04_super_08, pl04_super_09, pl04_super_10, pl04_super_11,
	pl04_super_12, pl04_super_13, pl04_super_14, pl04_super_15, pl04_super_16, pl04_super_17,
	pl04_super_18, pl04_super_19, pl04_super_20, pl04_super_21, pl04_super_22, pl04_super_23,
	pl04_super_24, pl04_super_25, pl04_super_26, pl04_super_27, 0,
};

const Sint32 *const pl04_effect_tbl[] = {
	pl04_effect_00, pl04_effect_01, pl04_effect_02, pl04_effect_03, pl04_effect_04, pl04_effect_05,
	pl04_effect_06, pl04_effect_07, 0,
};

const Sint32 *const pl04_change_tbl[] = {
	pl04_change_00, pl04_change_01, pl04_change_02, pl04_change_03, pl04_change_04, 0,
};

const Sint32 *const pl04_grp25_tbl[] = {
	pl04_stand_00, 0,
};

const Sint32 *const pl04_grp27_tbl[] = {
	pl04_grp27_00, pl04_grp27_01, pl04_grp27_02, pl04_grp27_03, pl04_grp27_04, pl04_grp27_05,
	pl04_grp27_06, pl04_grp27_07, pl04_grp27_08, pl04_grp27_09, pl04_grp27_10, pl04_grp27_11,
	pl04_grp27_12, pl04_grp27_13, pl04_grp27_14, pl04_grp27_15, pl04_grp27_16, pl04_grp27_17,
	pl04_grp27_18, pl04_grp27_19, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_move, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   Char_Fly 0 */
const Sint32 pl04_stand_00[] = {
	CG_FRAME(    0,    0,    7,           0,    43, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    44, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    45, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    46, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    47, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    48, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    49, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    50, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    51, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    52, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    53, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    54, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    55, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    56, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    57, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    58, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    59, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    60, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    61, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    62, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    63, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    64, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    65, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,    66, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl04_crouch_01[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   148, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_crouch_01 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   Char_Fly 2
   Char_Fly 6 */
const Sint32 pl04_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    67, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    70, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    80, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   Char_Fly 3
   Char_Fly 7 */
const Sint32 pl04_stand_03[] = {
	CG_FRAME(    0,    0,    5,           0,    89, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    96, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    97, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    98, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   100, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   101, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   102, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    85, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    86, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    87, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    88, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl04_crouch_00[] = {
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   147, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl04_crouch_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   148, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_crouch_00_loop ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl04_crouch_02[] = {
	CG_FRAME(    0,    0,    3,           0,   150, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_crouch_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   152, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_crouch_02_loop ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl04_turn_00[] = {
	CG_FRAME(    0,    0,    3,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl04_turn_01[] = {
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   157, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   158, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_turn_01 ),
};

/* Char_Turn 2 */
const Sint32 pl04_turn_02[] = {
	CG_FRAME(    0,    0,    3,           0,   189, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   190, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   191, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   192, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   193, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_turn_02 ),
};

/* Char_Jump 3 */
const Sint32 pl04_jump_03[] = {
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_jump_03 ),
};

/* Char_Jump 4
   Char_Jump 5 */
const Sint32 pl04_jump_04[] = {
	CG_FRAME(    0,    0,    3,           0,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_jump_03 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2
   Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl04_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   115, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_damage_30_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   128, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   129, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   132, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   133, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: drop_fall, dive_fall, act_fall, act_air_guard_hold, ...
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl04_jump_09[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_09 ),
};

/* Char_Jump 12: act_wall_jump
   Char_Dash 4: fly_start_init, fly_start2_init */
const Sint32 pl04_jump_12[] = {
	CG_FRAME(    0,    0,    8,           0,   193, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   194, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   195, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   196, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   197, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   198, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   199, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_12 ),
};

/* Char_Guard 0 */
const Sint32 pl04_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   159, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl04_guard_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl04_guard_02[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl04_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl04_guard_05[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl04_guard_06[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard
   Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl04_air_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_air_guard_00 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl04_appear_00[] = {
	CG_FRAME(    0,    0,    3,           0,  1117, 0x80,   0x00,    0,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1118, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1119, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1120, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1121, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,  1117, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_appear_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    43, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_appear_00_loop ),
};

/* Char_Appear 1 */
const Sint32 pl04_appear_01[] = {
	CG_FRAME(    0,    0,   48,           0,   231, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   232, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   233, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   234, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   241, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   242, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   242, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_appear_01 ),
};

/* Char_Win 0: win_ko
   Char_Win 4: win_time */
const Sint32 pl04_win_00[] = {
	CG_FRAME(    0,    0,   19,           0,   231, 0x80,   0x00,    0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_win_02_loop[] = {
	CG_FRAME(    0,    0,   19,           0,   232, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   19,           0,   233, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   19,           0,   234, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_win_02_loop_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   235, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   236, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   237, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   238, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   239, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   240, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   241, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   242, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_win_02_loop_loop ),
};

/* Char_Win 1: win_ko
   Char_Win 3 */
const Sint32 pl04_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,    0,  0x00, 0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   243, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   244, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   245, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   246, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   247, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   248, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   249, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_win_01_loop[] = {
	CG_FRAME(    0,    0,    8,           0,   250, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   251, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl04_win_02[] = {
	CG_FRAME(    0,    0,   19, END_OF_DATA,   231, 0x80,   0x00,    0,  0x00, 0x11,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_win_02_loop ),
};

/* Char_Win 5: lose_time_ko
   Char_Win 6: draw_time */
const Sint32 pl04_win_05[] = {
	CG_FRAME(    0,    0,    7,           0,   200, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   201, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_win_05_loop[] = {
	CG_FRAME(    0,    0,    7,           0,   204, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   203, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   202, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   203, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   205, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   206, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   207, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   206, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_win_05_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl04_damage_00[] = {
	CG_FRAME(    0,    0,    7,           0,   802, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   801, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl04_damage_01[] = {
	CG_FRAME(    0,    0,   10,           0,   803, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   802, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   801, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl04_damage_02[] = {
	CG_FRAME(    0,    0,   12,           0,   804, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   803, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   802, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   801, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl04_damage_03[] = {
	CG_FRAME(    0,    0,    7,           0,   806, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   805, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl04_damage_04[] = {
	CG_FRAME(    0,    0,   10,           0,   807, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   806, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   805, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl04_damage_05[] = {
	CG_FRAME(    0,    0,   12,           0,   808, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   807, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   806, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   805, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl04_damage_06[] = {
	CG_FRAME(    0,    0,    8,           0,   809, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   809, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl04_damage_07[] = {
	CG_FRAME(    0,    0,   10,           0,   810, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   809, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   809, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl04_damage_08[] = {
	CG_FRAME(    0,    0,   12,           0,   811, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   810, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   810, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   809, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl04_damage_10[] = {
	CG_FRAME(    0,    0,    8,           0,   813, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   812, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl04_damage_11[] = {
	CG_FRAME(    0,    0,    6,           0,   814, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   812, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl04_damage_12[] = {
	CG_FRAME(    0,    0,    4,           0,   815, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   814, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   812, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_12 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl04_damage_09[] = {
	CG_FRAME(    0,    0,    4,           0,   818, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   819, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   820, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   830, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   831, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
};

const Sint32 pl04_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   832, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl04_damage_09_loop ),
};

/* Char_Guard 3 */
const Sint32 pl04_guard_03[] = {
	CG_FRAME(    0,    0,   20, END_OF_DATA,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_guard_03 ),
};

/* Char_Guard 7 */
const Sint32 pl04_guard_07[] = {
	CG_FRAME(    0,    0,   20, END_OF_DATA,   163, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_guard_07 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl04_air_guard_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_air_guard_02 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl04_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   840, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   841, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   842, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   843, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   844, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   845, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl04_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   846, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   848, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   849, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   850, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   851, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl04_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   832, 0x80,   0xc8,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl04_damage_31 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl04_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   821, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   822, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl04_damage_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   823, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl04_damage_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   824, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl04_damage_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   825, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl04_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   826, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl04_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   821, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   822, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl04_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   823, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl04_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   824, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl04_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   825, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl04_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   826, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl04_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   827, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   828, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   829, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   830, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   831, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   832, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl04_damage_26 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl04_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   816, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   817, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl04_damage_13 ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl04_damage_27[] = {
	CG_FRAME(    0,    0,    3,           0,   856, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   857, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   859, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   860, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   861, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   862, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl04_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,   857, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   859, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_damage_30_loop ),
};

/* Char_Get_Up 1
   Char_Get_Up 3 */
const Sint32 pl04_get_up_01[] = {
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   177, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   182, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   184, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   185, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl04_get_up_02[] = {
	CG_FRAME(    0,    0,    5,           0,   176, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   183, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   184, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   185, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   187, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_get_up_01 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl04_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   175, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_get_up_00 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl04_dizzy_00[] = {
	CG_FRAME(    0,    0,   12,           0,   852, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   853, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   854, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   855, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_dizzy_00 ),
};

/* Char_Caught 0 */
const Sint32 pl04_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   802, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl04_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   803, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl04_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   804, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl04_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   806, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl04_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   807, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl04_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl04_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   808, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl04_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl04_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   809, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl04_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl04_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   810, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl04_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl04_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   811, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl04_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl04_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   812, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl04_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   813, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl04_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   814, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl04_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   820, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl04_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl04_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   821, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl04_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl04_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   823, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl04_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl04_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   824, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl04_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl04_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   825, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl04_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   826, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl04_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl04_caught_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   840, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl04_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl04_caught_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   846, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl04_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl04_caught_20[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   832, 0x80,   0xc8,    0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl04_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl04_caught_21[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   819, 0x80,   0xc8,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl04_caught_21 ),
};

/* Char_Effect 0: shl04_04_init */
const Sint32 pl04_effect_00[] = {
	CG_FRAME(    0,    0,   10,           0,   607, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   607, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   607, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   607, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   607, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl04_effect_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   609, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl04_effect_02[] = {
	CG_FRAME(    0,    1,    4,           0,   612, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   614, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   611, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   614, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   611, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_effect_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   615, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   616, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_02_loop ),
};

/* Char_Effect 3 */
const Sint32 pl04_effect_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   612, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl04_effect_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   610, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl04_effect_05[] = {
	CG_FRAME(    0,    0,   11,           0,   617, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   620, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   11,           0,   621, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   618, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl04_effect_06[] = {
	CG_FRAME(    0,    0,   11,           0,   622, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   623, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   624, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   625, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   11,           0,   626, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   624, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   623, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl04_effect_07[] = {
	CG_FRAME(    0,    0,    4,           0,   611, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   612, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   613, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   614, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_effect_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl04_stand_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   262, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    6,           0,   263, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   261, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl04_stand_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   264, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   265, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    3,           0,   266, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    6,           0,   267, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    5,           0,   268, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   269, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl04_stand_p_02[] = {
	CG_FRAME(    0,    0,    1,           0,   270, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   271, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    3,           0,   273, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    6,           0,   274, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   10,           0,   275, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   276, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_NEXT( pl04_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl04_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   277, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   278, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   284, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl04_stand_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   280, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   281, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    7,           0,   282, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    6,           0,   283, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   284, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl04_stand_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   285, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   286, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,    1,           0,   287, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,    6,           0,   288, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME( 0x80,    0,    4,           0,   289, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0001,  27 ),
	CG_FRAME(    0,    0,    8,           0,   290, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,  28 ),
	CG_FRAME(    0,    0,    8,           0,   291, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,  28 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,  28 ),
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,    4,           0,   294, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,    4,           0,   295, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   297, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   298, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl04_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   299, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    6,           0,   301, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   302, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl04_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   303, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   304, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    1,    2,           0,   305, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   307, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    2,           0,   308, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   309, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   310, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   311, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   312, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   314, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   317, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl04_crouch_p_02[] = {
	CG_FRAME(    0,    0,    2,           0,   145, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,    1,           0,   319, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,  34 ),
	CG_FRAME(    0,    0,    1,           0,   320, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0300,  35 ),
	CG_FRAME(    0,    0,    1,           0,   321, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,  35 ),
	CG_FRAME(    0,    0,    1,           0,   322, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0500,  35 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0600,  36 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0700,  36 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0800,  36 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0900,  36 ),
	CG_FRAME(    0,    0,    3,           0,   327, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0a00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0b00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0c00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0d00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   331, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0e00,  36 ),
	CG_FRAME(    0,    0,    4,           0,   332, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0f00,  36 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   333, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x1000,   2 ),
	CG_NEXT( pl04_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl04_crouch_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   334, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   335, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    6,           0,   336, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   337, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   338, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl04_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   339, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   341, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   342, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   343, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   345, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    5,           0,   346, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   347, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl04_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl04_crouch_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   348, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   349, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   350, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   351, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   352, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   353, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   12,           0,   354, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    6,           0,   355, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    6,           0,   356, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   357, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl04_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl04_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   365, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   359, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   360, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   361, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   362, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   363, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   364, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    1,           0,   365, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   396, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   397, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   398, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl04_jump_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   366, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   367, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   368, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    3,           0,   369, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   370, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    4,           0,   371, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    4,           0,   372, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    4,           0,   373, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   374, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   393, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   396, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   397, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   398, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl04_jump_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   375, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   376, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   377, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   380, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    3,           0,   381, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    3,           0,   382, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    3,           0,   384, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   385, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   393, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   394, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   395, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   396, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   397, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   398, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl04_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   386, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   387, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0006,   3 ),
	CG_FRAME(    0,    1,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    0,    3,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x0002,   3 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0003,   3 ),
	CG_FRAME(    0,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0004,   3 ),
	CG_FRAME(    0,    0,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0005,   3 ),
	CG_FRAME(    0,    0,    3,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0006,   3 ),
	CG_FRAME(    0,    0,    1,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl04_jump_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   386, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0007,   3 ),
	CG_FRAME(    1,    0,    3,           0,   387, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0008,   3 ),
	CG_FRAME(    0,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x0009,   3 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x000a,   3 ),
	CG_FRAME(    0,    1,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x000b,   3 ),
	CG_FRAME(    1,    0,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000c,   3 ),
	CG_FRAME(    0,    0,    3,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x000d,   3 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x000e,   3 ),
	CG_FRAME(    0,    0,    3,           0,   390, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000f,   3 ),
	CG_FRAME(    0,    0,    3,           0,   391, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0010,   3 ),
	CG_FRAME(    0,    0,    3,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0011,   3 ),
	CG_FRAME(    0,    0,    1,           0,   392, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0012,   3 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0012,   3 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl04_jump_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   393, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9113,   3 ),
	CG_FRAME(    1,    0,    3,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9214,   3 ),
	CG_FRAME(    0,    0,    3,           0,   395, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x9315,   3 ),
	CG_FRAME(    0,    0,    3,           0,   396, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x9416,   3 ),
	CG_FRAME(    0,    0,    3,           0,   397, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     1, 0x9517,   3 ),
	CG_FRAME(    0,    0,    3,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9618,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9719,   3 ),
	CG_FRAME(    0,    0,    3,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x981a,   3 ),
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x991b,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9a1c,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9b1d,   3 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_jump_k_02_loop ),
};

/* Char_Extra 8 */
const Sint32 pl04_extra_08[] = {
	CG_FRAME(    0,    0,    3,           0,   403, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   404, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3,           0,   405, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3,           0,   406, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   407, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,    3,           0,   408, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   409, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   410, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   407, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   408, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   409, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1,           0,   410, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1,           0,   412, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_extra_08_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_extra_08_loop ),
};

/* Char_Extra 9 */
const Sint32 pl04_extra_09[] = {
	CG_FRAME(    0,    0,    3,           0,   413, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   414, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   415, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   416, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,    3,           0,   417, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   418, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   419, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   420, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   417, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   418, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   419, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   419, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   420, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   420, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  76 ),
};

const Sint32 pl04_extra_09_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_extra_09_loop ),
};

/* Char_Extra 10 */
const Sint32 pl04_extra_10[] = {
	CG_FRAME(    0,    0,    3,           0,   421, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   423, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3,           0,   424, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3,           0,   425, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   426, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   427, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   428, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   425, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   426, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    3,           0,   427, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    1,           0,   427, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    2,           0,   428, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    1,           0,   428, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  78 ),
};

const Sint32 pl04_extra_10_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_extra_10_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl04_change_00[] = {
	CG_FRAME(    0,    0,    3,           0,   155, 0x30,   0x00,    0,  0x00, 0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   153, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   252, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_change_00_loop[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   252, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl04_change_02[] = {
	CG_FRAME(    0,    0,    3,           0,   128, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   129, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   131, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   132, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   133, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl04_change_03[] = {
	CG_FRAME(    0,    0,    3,           0,   155, 0x30,   0x00,    0,  0x00, 0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   153, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   252, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_change_03_loop[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   252, 0x30,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_change_03_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl04_change_01[] = {
	CG_FRAME(    0,    0,    4,           0,   705, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,   705, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_change_01 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl04_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0,   590, 0xa0,   0x00,  ATK,  0x00, 0x14,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   591, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_extra_00_loop[] = {
	CG_FRAME( 0x80,    0,    3,           0,   592, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    3,           0,   593, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,    3,           0,   594, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x84,    0,    6, END_OF_DATA,   595, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl04_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl04_extra_01[] = {
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   144, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x30,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   154, 0x30,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x30,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   254, 0x30,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   253, 0x30,   0x00,  ATE,  0x00, 0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   252, 0x30,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    85, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    86, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    87, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    88, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_extra_01 ),
};

/* Char_Extra 7: pl04_upper */
const Sint32 pl04_extra_07[] = {
	CG_FRAME(    0,    0,    2,           0,   145, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0100,   2 ),
	CG_FRAME(    0,    0,    1,           0,   319, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0200,  34 ),
	CG_FRAME(    0,    0,    1,           0,   320, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0300,  35 ),
	CG_FRAME(    0,    0,    1,           0,   321, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0400,  35 ),
	CG_FRAME(    0,    0,    1,           0,   322, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0500,  35 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0600,  36 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0700,  36 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0800,  36 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0900,  36 ),
	CG_FRAME(    0,    0,    3,           0,   327, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0a00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0b00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0c00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0d00,  36 ),
	CG_FRAME(    0,    0,    3,           0,   331, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0e00,  36 ),
	CG_FRAME(    0,    0,    4,           0,   332, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0f00,  36 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   333, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x1000,   2 ),
	CG_NEXT( pl04_extra_07 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl04_dash_00[] = {
	CG_FRAME(    0,    0,    3,           0,   103, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   104, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   105, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   106, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   106, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_dash_00 ),
};

/* Char_Dash 2: dash_stop */
const Sint32 pl04_dash_02[] = {
	CG_FRAME(    0,    0,    3,           0,   107, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   107, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl04_dash_01[] = {
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_dash_01 ),
};

/* Char_Dash 3: noki_stop */
const Sint32 pl04_dash_03[] = {
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   113, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_dash_03 ),
};

/* Char_Extra 4 */
const Sint32 pl04_extra_04[] = {
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x00,    0,  0x00, 0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_extra_04 ),
};

/* Char_Extra 5: wrap_out */
const Sint32 pl04_extra_05[] = {
	CG_FRAME(    0,    0,    3,           0,   103, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   104, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   105, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   106, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   106, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_extra_05 ),
};

/* Char_Extra 11 */
const Sint32 pl04_extra_11[] = {
	CG_FRAME(    0,    0,    3,           0,   573, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   574, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   13,           0,   575, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   576, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   577, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,   578, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    3,           0,   579, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    3,           0,   580, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    3,           0,   581, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    3,           0,   582, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   583, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_extra_11 ),
};

/* Char_Extra 14: jump_kick */
const Sint32 pl04_extra_14[] = {
	CG_FRAME(    0,    0,    4,           0,   584, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   585, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_extra_14_loop[] = {
	CG_FRAME(    0,    1,    3,           0,   586, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   587, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0,   588, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   589, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl04_extra_14_loop ),
};

/* Char_Extra 12 */
const Sint32 pl04_extra_12[] = {
	CG_FRAME(    0,    0,    6,           0,   596, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   597, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_extra_12_loop[] = {
	CG_FRAME(    0,    1,   16, END_OF_DATA,   598, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl04_extra_12_loop ),
};

/* Char_Extra 13 */
const Sint32 pl04_extra_13[] = {
	CG_FRAME(    0,    0,    8,           0,   590, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   591, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_extra_13_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   592, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,   593, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,   594, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   595, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl04_extra_13_loop ),
};

/* Char_Extra 15
   Char_Special 0: sp_coffin (Coffin Dance) */
const Sint32 pl04_extra_15[] = {
	CG_FRAME(    0,    0,    4,           0,   627, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   628, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   629, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   630, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   631, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   633, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   633, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   634, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   635, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   636, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   637, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   638, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   639, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_extra_15 ),
};

/* Char_Super 6: coffins_init (Coffin Fall (hyper)) */
const Sint32 pl04_super_06[] = {
	CG_FRAME(    0,    2,    4,           0,   627, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_super_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   628, 0x80,   0x00,    0,  0x00, 0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   629, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   630, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   631, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   633, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   633, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   634, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_super_06_loop ),
};

/* Char_Super 7: coffins_shot (Coffin Fall (hyper)) */
const Sint32 pl04_super_07[] = {
	CG_FRAME(    0,    0,    7,           0,   634, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   635, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   636, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   637, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   638, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   639, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_super_07 ),
};

/* Char_Special 1: sp_coffin (Coffin Dance) */
const Sint32 pl04_special_01[] = {
	CG_FRAME(    0,    0,    4,           0,   646, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   647, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   648, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   650, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   651, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   651, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   652, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   653, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   654, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   655, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   652, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   653, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_special_01 ),
};

/* Char_Super 8: coffins_init (Coffin Fall (hyper)) */
const Sint32 pl04_super_08[] = {
	CG_FRAME(    0,    2,    4,           0,   656, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl04_super_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   646, 0x80,   0x00,    0,  0x00, 0x18,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   647, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   648, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   650, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   651, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    1,           0,   651, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   652, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_super_08_loop ),
};

/* Char_Super 9: coffins_shot (Coffin Fall (hyper)) */
const Sint32 pl04_super_09[] = {
	CG_FRAME(    0,    0,    7,           0,   653, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   654, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   655, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   652, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   653, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_super_09 ),
};

/* Char_Special 2: sp_judgement (Royal Judgement, 2 + atck_level) */
const Sint32 pl04_special_02[] = {
	CG_FRAME(    0,    0,    3,           0,   640, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   641, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   643, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    3,           0,   644, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   13,           0,   645, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   31, END_OF_DATA,   645, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl04_special_03[] = {
	CG_FRAME(    0,    0,    3,           0,   640, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   641, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   643, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    3,           0,   644, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   13,           0,   645, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   63, END_OF_DATA,   645, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_special_03 ),
};

/* Char_Special 4: sp_cobra (Cobra Blow, 4 + atck_level) */
const Sint32 pl04_special_04[] = {
	CG_FRAME(    0,    0,    3,           0,   661, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   662, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   663, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    5,           0,   674, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    3,           0,   675, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   676, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   677, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   678, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   679, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   661, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_special_04 ),
};

/* Char_Special 5 */
const Sint32 pl04_special_05[] = {
	CG_FRAME(    0,    0,    3,           0,   661, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   662, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   663, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   664, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6,           0,   665, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    5,           0,   666, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    3,           0,   667, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    2,           0,   668, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   669, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   670, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   671, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   672, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   673, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   674, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   675, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   676, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   677, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   678, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   679, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   661, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_special_05 ),
};

/* Char_Special 6: sp_cobra (Cobra Blow, 6 + atck_level) */
const Sint32 pl04_special_06[] = {
	CG_FRAME(    0,    0,    3,           0,   680, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   681, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   682, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   692, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    5,           0,   693, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   694, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   695, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   696, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   697, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   680, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_special_06 ),
};

/* Char_Special 7 */
const Sint32 pl04_special_07[] = {
	CG_FRAME(    0,    0,    3,           0,   680, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   681, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   682, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   683, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    6,           0,   684, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    5,           0,   685, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   686, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   687, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   688, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   689, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   690, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   691, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   692, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   693, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    2,           0,   694, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   695, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   696, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   697, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   680, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl04_special_07 ),
};

/* Char_Super 16: cobras_init (Pharaoh Cobra Throw (hyper)) */
const Sint32 pl04_super_16[] = {
	CG_FRAME(    0,    0,    3,           0,   767, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   770, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   704, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   705, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    1,    3,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   96,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   -1,   32,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   -2,    4,           0,   704, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_super_16_loop[] = {
	CG_FRAME(    0,   -2,    4, END_OF_DATA,   705, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_super_16_loop ),
};

/* Char_Super 17: shl04_03_head_init */
const Sint32 pl04_super_17[] = {
	CG_FRAME(    0,    0,    2,           0, 32775, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    3,           0, 32777, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
};

const Sint32 pl04_super_17_loop[] = {
	CG_FRAME(    0,    1,    2,           0, 32778, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    2,    2,           0, 32779, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    3,    2,           0, 32780, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    4,    2,           0, 32781, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    5,    2, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl04_super_17_loop ),
};

/* Char_Super 18: shl04_03_head_go */
const Sint32 pl04_super_18[] = {
	CG_FRAME(    0,    1,    2,           0, 32783, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    2,    2,           0, 32784, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    3,    2,           0, 32785, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    4,    2,           0, 32786, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    5,    2,           0, 32787, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,    2,           0, 32788, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    2,    2,           0, 32789, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    3,    2,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    4,    2,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    5,    2, END_OF_DATA, 32792, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl04_super_18 ),
};

/* Char_Super 19: shl04_03_body_move */
const Sint32 pl04_super_19[] = {
	CG_FRAME(    0,    0,    2,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0, 32794, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl04_super_19 ),
};

/* Char_Catch 0
   Char_Special 8
   Char_Special 13 */
const Sint32 pl04_catch_00[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   1 ),
	CG_NEXT( pl04_catch_00 ),
};

/* Char_Special 9: mummy_init (Mummy Drop, 9 + atck_level) */
const Sint32 pl04_special_09[] = {
	CG_FRAME(    0,    0,    3,           0,   485, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   1 ),
	CG_FRAME(    0,    0,    3,           0,   486, 0x80,   0x00,    0,  0x00, 0x1e,    0,   0,     0, 0x2401,   1 ),
	CG_FRAME(    0,    1,   23,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  79 ),
	CG_FRAME(    0,    0,    1,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  79 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   468, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   469, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   471, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   475, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   476, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2,           0,   477, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   478, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  80 ),
	CG_NEXT( pl04_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl04_special_10[] = {
	CG_FRAME(    0,    0,    3,           0,   485, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   1 ),
	CG_FRAME(    0,    0,    3,           0,   486, 0x80,   0x00,    0,  0x00, 0x1e,    0,   0,     0, 0x2401,   1 ),
	CG_FRAME(    0,    1,   23,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  81 ),
	CG_FRAME(    0,    0,    1,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  81 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   468, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   469, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   471, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   475, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   476, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2,           0,   477, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   478, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  82 ),
	CG_NEXT( pl04_special_10 ),
};

/* Char_Special 11: pl04_catch_set */
const Sint32 pl04_special_11[] = {
	CG_FRAME(    0,    0,    1,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   0 ),
	CG_FRAME(    0,    0,    3,           0,   451, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   452, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   453, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   454, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   455, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   456, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    8,           0,   458, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
};

const Sint32 pl04_special_11_loop[] = {
	CG_FRAME(    1,    0,    3,           0,   457, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   459, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   460, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   461, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,    8,           0,   462, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    1,    0,    3,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   461, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   460, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   459, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   457, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,   -1,    8, END_OF_DATA,   458, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_NEXT( pl04_special_11_loop ),
};

/* Char_Special 21: pl04_catch_set */
const Sint32 pl04_special_21[] = {
	CG_FRAME(    0,    0,    1,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   0 ),
	CG_FRAME(    0,    0,    3,           0,   451, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   452, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   453, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   454, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   455, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
};

const Sint32 pl04_special_21_loop[] = {
	CG_FRAME(    0,   -1,    3, END_OF_DATA,   456, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_NEXT( pl04_special_21_loop ),
};

/* Char_Catch 1
   Char_Special 12 */
const Sint32 pl04_catch_01[] = {
	CG_FRAME(    0,    0,    3,           0,   437, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   438, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   440, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   441, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,   64,    1,           0,   442, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,   66,   15,           0,   442, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    1,           0,   443, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0, 0x80,    2, END_OF_DATA,   443, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_NEXT( pl04_catch_01 ),
};

/* Char_Special 14: mummy_init (Mummy Drop, 14 + atck_level) */
const Sint32 pl04_special_14[] = {
	CG_FRAME(    0,    0,    3,           0,   557, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   3 ),
	CG_FRAME(    0,    0,    3,           0,   558, 0x80,   0x00,    0,  0x00, 0x1e,    0,   0,     0, 0x2401,   3 ),
	CG_FRAME(    0,    1,   23,           0,   559, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  83 ),
	CG_FRAME(    0,    0,    1,           0,   559, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  83 ),
	CG_FRAME(    0,    0,    2,           0,   538, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   539, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   540, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   541, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   542, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   543, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   544, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   545, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   546, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   547, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   548, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2,           0,   549, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   550, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  84 ),
	CG_NEXT( pl04_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl04_special_15[] = {
	CG_FRAME(    0,    0,    3,           0,   557, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   3 ),
	CG_FRAME(    0,    0,    3,           0,   558, 0x80,   0x00,    0,  0x00, 0x1e,    0,   0,     0, 0x2401,   3 ),
	CG_FRAME(    0,    1,   23,           0,   559, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  85 ),
	CG_FRAME(    0,    0,    1,           0,   559, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,  85 ),
	CG_FRAME(    0,    0,    2,           0,   538, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   539, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   540, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   541, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   542, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   543, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   544, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   545, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   546, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   547, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   548, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2,           0,   549, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   550, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2400,  86 ),
	CG_NEXT( pl04_special_15 ),
};

/* Char_Special 16: pl04_catch_set */
const Sint32 pl04_special_16[] = {
	CG_FRAME(    0,    0,    1,           0,   559, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x2401,   0 ),
	CG_FRAME(    0,    0,    3,           0,   523, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   524, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   525, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   526, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   527, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   528, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    8,           0,   530, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
};

const Sint32 pl04_special_16_loop[] = {
	CG_FRAME(    1,    0,    3,           0,   529, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   531, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   532, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   533, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   535, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,    8,           0,   534, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    1,    0,    3,           0,   535, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   533, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   532, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   531, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   529, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,   -1,    8, END_OF_DATA,   530, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_NEXT( pl04_special_16_loop ),
};

/* Char_Catch 2
   Char_Special 17 */
const Sint32 pl04_catch_02[] = {
	CG_FRAME(    0,    0,    3,           0,   509, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   510, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   511, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   512, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   513, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,   64,    1,           0,   514, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,   66,   15,           0,   514, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0, 0x80,    3, END_OF_DATA,   515, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_NEXT( pl04_catch_02 ),
};

/* Char_Super 20: illusion_init (Pharaoh Illusion (hyper)) */
const Sint32 pl04_super_20[] = {
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   704, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   705, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   706, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   707, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   708, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    1,    1,    2,           0,   705, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl04_super_20_loop[] = {
	CG_FRAME(    0,    1,    3, END_OF_DATA,   705, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl04_super_20_loop ),
};

/* Char_Super 21: illusion_rise (Pharaoh Illusion (hyper)) */
const Sint32 pl04_super_21[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,    -1, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x2300,  95 ),
	CG_NEXT( pl04_super_21 ),
};

/* Char_Super 22: illusion_fly (Pharaoh Illusion (hyper)) */
const Sint32 pl04_super_22[] = {
	CG_FRAME(    0,    0,    1,           0,  1117, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,  1118, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,  1119, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,  1120, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,  1121, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_super_22_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,  1117, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_super_22_loop ),
};

/* Char_Super 23 */
const Sint32 pl04_super_23[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   721, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl04_super_23 ),
};

/* Char_Super 24 */
const Sint32 pl04_super_24[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   722, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl04_super_24 ),
};

/* Char_Super 25: shl04_05_init */
const Sint32 pl04_super_25[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   724, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl04_super_25 ),
};

/* Char_Super 26 */
const Sint32 pl04_super_26[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   724, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl04_super_26 ),
};

/* Char_Super 27 */
const Sint32 pl04_super_27[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   724, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl04_super_27 ),
};

/* group 27 14 */
const Sint32 pl04_grp27_14[] = {
	EF_FRAME(    0,    0,    8, END_OF_DATA,   725 ),
	CG_NEXT( pl04_grp27_14 ),
};

/* group 27 15 */
const Sint32 pl04_grp27_15[] = {
	EF_FRAME(    0,    0,    8, END_OF_DATA,   726 ),
	CG_NEXT( pl04_grp27_15 ),
};

/* Char_Super 0: magic_init (Pharaoh Magic (hyper)) */
const Sint32 pl04_super_00[] = {
	CG_FRAME(    0,    2,    2,           0,   232, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9c00,   1 ),
	CG_FRAME(    0,    2,    2,           0,   233, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9d00,   1 ),
	CG_FRAME(    0,    2,    2,           0,   234, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9e00,   1 ),
	CG_FRAME(    0,    0,    2,           0,   235, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9f00,   1 ),
};

/* Char_Super 1 */
const Sint32 pl04_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   628, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0, 0xa000,   1 ),
	CG_FRAME(    0,    0,    4,           0,   629, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa100,   1 ),
	CG_FRAME(    0,    0,    4,           0,   630, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   1 ),
	CG_FRAME(    0,    0,    4,           0,   631, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   1 ),
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   1 ),
	CG_FRAME(    0,    0,    4,           0,   633, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   1 ),
	CG_FRAME(    0,    0,    3,           0,   634, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   1 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   634, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   1 ),
	CG_NEXT( pl04_super_01 ),
};

/* Char_Super 2: magic_wait (Pharaoh Magic (hyper)) */
const Sint32 pl04_super_02[] = {
	CG_FRAME(    0,    0,    5,           0,   635, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0xa000,   1 ),
	CG_FRAME(    0,    0,    5,           0,   636, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9f00,   1 ),
	CG_FRAME(    0,    0,    5,           0,   637, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9e00,   1 ),
	CG_FRAME(    0,    0,    5,           0,   638, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9d00,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   639, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9c00,   1 ),
	CG_NEXT( pl04_super_02 ),
};

/* Char_Super 3: magic_init (Pharaoh Magic (hyper)) */
const Sint32 pl04_super_03[] = {
	CG_FRAME(    0,    2,    4,           0,   656, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    2,    2,           0,   657, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9c00,   3 ),
	CG_FRAME(    0,    2,    2,           0,   658, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9d00,   3 ),
	CG_FRAME(    0,    2,    2,           0,   659, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9e00,   3 ),
	CG_FRAME(    0,    0,    2,           0,   660, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x9f00,   3 ),
};

/* Char_Super 4 */
const Sint32 pl04_super_04[] = {
	CG_FRAME(    0,    0,    4,           0,   646, 0x80,   0x00,  ATK,  0x00, 0x18,    0,   0,     0, 0xa000,   3 ),
	CG_FRAME(    0,    0,    4,           0,   647, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa100,   3 ),
	CG_FRAME(    0,    0,    4,           0,   648, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   3 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   3 ),
	CG_FRAME(    0,    0,    4,           0,   650, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   3 ),
	CG_FRAME(    0,    0,    4,           0,   651, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   3 ),
	CG_FRAME(    0,    0,    3,           0,   652, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   3 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   652, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0xa200,   3 ),
	CG_NEXT( pl04_super_04 ),
};

/* Char_Super 5: magic_wait (Pharaoh Magic (hyper)) */
const Sint32 pl04_super_05[] = {
	CG_FRAME(    0,    0,    5,           0,   653, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0xa000,   3 ),
	CG_FRAME(    0,    0,    5,           0,   654, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9f00,   3 ),
	CG_FRAME(    0,    0,    5,           0,   655, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9e00,   3 ),
	CG_FRAME(    0,    0,    5,           0,   652, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9d00,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   653, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x9c00,   3 ),
	CG_NEXT( pl04_super_05 ),
};

/* Char_Extra 2: snapback_init (The snapback), fly_start5 */
const Sint32 pl04_extra_02[] = {
	CG_FRAME(    0,    0,    8,           0,   270, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   271, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   273, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   274, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   10,           0,   275, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
};

const Sint32 pl04_extra_02_loop[] = {
	CG_FRAME(    0,    0,    7, END_OF_DATA,   276, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_NEXT( pl04_extra_02_loop ),
};

/* Char_Extra 3: sp_chouhatsu (The taunt), fly_start5 */
const Sint32 pl04_extra_03[] = {
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,    0,  0x00, 0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   36,           0,   251, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_extra_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_extra_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl04_change_04[] = {
	CG_FRAME(    0,    0,    1,           0,   155, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   244, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   246, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   247, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   249, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   250, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   251, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   154, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl04_change_04_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   155, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl04_change_04_loop ),
};

/* Char_Special 18: shl04_00_coffin_init */
const Sint32 pl04_special_18[] = {
	CG_FRAME(    0,    0,    1,           0, 32806, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6,           0, 32807, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32808, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32809, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32810, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_special_18_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32806, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_special_18_loop ),
};

/* Char_Super 10: shl04_00_coffins_init, shl04_01_magic_init (10 + type) */
const Sint32 pl04_super_10[] = {
	CG_FRAME(    0,    0,    1,           0, 32806, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6,           0, 32807, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32808, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32809, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32810, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_super_10_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32806, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_super_10_loop ),
};

/* Char_Special 19: shl04_01_judgement_init (19 + atck_level) */
const Sint32 pl04_special_19[] = {
	CG_FRAME(    0,    0,   20,           0,    -1, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_special_19_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,    -1, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl04_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl04_special_20[] = {
	CG_FRAME(    0,    0,   20,           0,    -1, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_special_20_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,    -1, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl04_special_20_loop ),
};

/* Char_Super 11 */
const Sint32 pl04_super_11[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,  1190, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl04_super_11 ),
};

/* Char_Super 13 */
const Sint32 pl04_super_13[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,  1191, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl04_super_13 ),
};

/* Char_Super 12 */
const Sint32 pl04_super_12[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,  1192, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl04_super_12 ),
};

/* Char_Super 14: shl04_00_magic_init */
const Sint32 pl04_super_14[] = {
	CG_FRAME(    0,    0,    1,           0, 32798, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    1,           0, 32798, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0, 32799, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0, 32800, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0, 32799, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0, 32801, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_super_14_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32798, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_super_14_loop ),
};

/* Char_Super 15 */
const Sint32 pl04_super_15[] = {
	CG_FRAME(    0,    0,    1,           0, 32798, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32798, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0, 32799, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0, 32800, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0, 32799, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0, 32801, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl04_super_15_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32798, 0x00,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_super_15_loop ),
};

/* Char_Extra 6: shl04_02_move */
const Sint32 pl04_extra_06[] = {
	CG_FRAME(    0,    0,   -1,           0,    -1, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1077, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1078, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   -1,           0,  1079, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1080, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1081, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1082, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1083, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1084, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1085, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   -1,           0,  1086, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   -1,           0,  1087, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   -1,           0,  1088, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1089, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   -1,           0,  1090, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   -1,           0,  1091, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1092, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1093, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1094, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1095, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1096, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1097, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   -1,           0,  1098, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   -1,           0,  1099, 0x00,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   -1,           0,  1100, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1101, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1102, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1103, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1,           0,  1104, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,  1105, 0x00,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl04_extra_06 ),
};

/* group 27 0 */
const Sint32 pl04_grp27_00[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,    -1 ),
	CG_NEXT( pl04_grp27_00 ),
};

/* group 27 1: pls04_00_frame_move (1 + dir) */
const Sint32 pl04_grp27_01[] = {
	EF_FRAME(    0,    0,    3,           0,    -1 ),
	EF_FRAME(    0,    1,    3,           0,  1045 ),
	EF_FRAME(    0,    2,    3,           0,  1046 ),
	EF_FRAME(    0,    3,    3,           0,  1047 ),
	EF_FRAME(    0,    4,    3,           0,  1048 ),
	EF_FRAME(    0,    5,    3,           0,  1049 ),
	EF_FRAME(    0,    6,    3,           0,  1050 ),
	EF_FRAME(    0,    7,    3,           0,  1051 ),
	EF_FRAME(    0,    8,    3,           0,  1052 ),
	EF_FRAME(    0,    9,    3,           0,  1053 ),
	EF_FRAME(    0,   10,    3,           0,  1054 ),
	EF_FRAME(    0,   11,    3,           0,  1055 ),
	EF_FRAME(    0,   12,    3,           0,  1056 ),
	EF_FRAME(    0,   13,    3,           0,  1057 ),
	EF_FRAME(    0,   14,    3,           0,  1058 ),
	EF_FRAME(    0,   15,    3,           0,  1059 ),
	EF_FRAME(    0,   16,    3,           0,  1060 ),
	EF_FRAME(    0,   17,    3,           0,    -1 ),
	EF_FRAME(    0,   18,    3,           0,    -1 ),
	EF_FRAME(    0,   19,    3,           0,    -1 ),
	EF_FRAME(    0,   20,    3,           0,    -1 ),
	EF_FRAME(    0,   21,    3,           0,    -1 ),
	EF_FRAME(    0,   22,    3,           0,    -1 ),
	EF_FRAME(    0,   23,    3,           0,    -1 ),
	EF_FRAME(    0,   24,    3,           0,    -1 ),
	EF_FRAME(    0,   25,    3,           0,    -1 ),
	EF_FRAME(    0,   26,    3,           0,    -1 ),
	EF_FRAME(    0,   27,    3,           0,    -1 ),
	EF_FRAME(    0,   28,    3,           0,   866 ),
	EF_FRAME(    0,   29,    3,           0,   867 ),
	EF_FRAME(    0,   30,    3,           0,   868 ),
	EF_FRAME(    0,   31,    3,           0,   869 ),
	EF_FRAME(    0,   32,    3,           0,   870 ),
	EF_FRAME(    0,   33,    3,           0,   871 ),
	EF_FRAME(    0,   34,    3,           0,   872 ),
	EF_FRAME(    0,   35,    3,           0,    -1 ),
	EF_FRAME(    0,   36,    3,           0,    -1 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,    -1 ),
	CG_NEXT( pl04_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl04_grp27_02[] = {
	EF_FRAME(    0,    0,    3,           0,    -1 ),
	EF_FRAME(    0,    1,    3,           0,  1061 ),
	EF_FRAME(    0,    2,    3,           0,  1062 ),
	EF_FRAME(    0,    3,    3,           0,  1063 ),
	EF_FRAME(    0,    4,    3,           0,  1064 ),
	EF_FRAME(    0,    5,    3,           0,  1065 ),
	EF_FRAME(    0,    6,    3,           0,  1066 ),
	EF_FRAME(    0,    7,    3,           0,  1067 ),
	EF_FRAME(    0,    8,    3,           0,  1068 ),
	EF_FRAME(    0,    9,    3,           0,  1069 ),
	EF_FRAME(    0,   10,    3,           0,  1070 ),
	EF_FRAME(    0,   11,    3,           0,  1071 ),
	EF_FRAME(    0,   12,    3,           0,  1072 ),
	EF_FRAME(    0,   13,    3,           0,  1073 ),
	EF_FRAME(    0,   14,    3,           0,  1074 ),
	EF_FRAME(    0,   15,    3,           0,  1075 ),
	EF_FRAME(    0,   16,    3,           0,  1076 ),
	EF_FRAME(    0,   17,    3,           0,  1106 ),
	EF_FRAME(    0,   18,    3,           0,  1107 ),
	EF_FRAME(    0,   19,    3,           0,  1108 ),
	EF_FRAME(    0,   20,    3,           0,  1109 ),
	EF_FRAME(    0,   21,    3,           0,  1110 ),
	EF_FRAME(    0,   22,    3,           0,  1111 ),
	EF_FRAME(    0,   23,    3,           0,  1112 ),
	EF_FRAME(    0,   24,    3,           0,  1113 ),
	EF_FRAME(    0,   25,    3,           0,  1114 ),
	EF_FRAME(    0,   26,    3,           0,  1115 ),
	EF_FRAME(    0,   27,    3,           0,  1116 ),
	EF_FRAME(    0,   28,    3,           0,    -1 ),
	EF_FRAME(    0,   29,    3,           0,    -1 ),
	EF_FRAME(    0,   30,    3,           0,    -1 ),
	EF_FRAME(    0,   31,    3,           0,    -1 ),
	EF_FRAME(    0,   32,    3,           0,    -1 ),
	EF_FRAME(    0,   33,    3,           0,    -1 ),
	EF_FRAME(    0,   34,    3,           0,    -1 ),
	EF_FRAME(    0,   35,    3,           0, 32770 ),
	EF_FRAME(    0,   36,    3,           0,    -1 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,    -1 ),
	CG_NEXT( pl04_grp27_02 ),
};

/* group 27 3: pls04_00_judgement_wait */
const Sint32 pl04_grp27_03[] = {
	EF_FRAME(    0,    0,    1,           0,  1122 ),
	EF_FRAME(    0,    0,    1,           0,  1153 ),
	EF_FRAME(    0,    0,    1,           0,  1123 ),
	EF_FRAME(    0,    0,    1,           0,  1154 ),
	EF_FRAME(    0,    0,    1,           0,  1124 ),
	EF_FRAME(    0,    0,    1,           0,  1155 ),
	EF_FRAME(    0,    0,    1,           0,  1125 ),
	EF_FRAME(    0,    0,    1,           0,  1156 ),
	EF_FRAME(    0,    0,    1,           0,  1126 ),
	EF_FRAME(    0,    0,    1,           0,  1157 ),
	EF_FRAME(    0,    0,    1,           0,  1127 ),
};

const Sint32 pl04_grp27_03_loop[] = {
	EF_FRAME(    0,    0,    1, END_OF_DATA,  1158 ),
	CG_NEXT( pl04_grp27_03_loop ),
};

/* group 27 4: pls04_00_judgement_wait, pls04_00_judgement_fly, pls04_00_judgement_ring, pls04_00_magic_ring */
const Sint32 pl04_grp27_04[] = {
	EF_FRAME(    0,    0,    3,           0,  1146 ),
	EF_FRAME(    0,    0,    3,           0,  1147 ),
	EF_FRAME(    0,    0,    3,           0,  1148 ),
	EF_FRAME(    0,    0,    3,           0,  1149 ),
	EF_FRAME(    0,    0,    3,           0,  1150 ),
	EF_FRAME(    0,    0,    3,           0,  1151 ),
};

const Sint32 pl04_grp27_04_loop[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA,  1152 ),
	CG_NEXT( pl04_grp27_04_loop ),
};

/* group 27 5: pls04_00_judgement_ring, pls04_00_magic_ring */
const Sint32 pl04_grp27_05[] = {
	EF_FRAME(    0,    0,    3,           0,  1129 ),
	EF_FRAME(    0,    0,    3,           0,  1130 ),
	EF_FRAME(    0,    0,    3,           0,  1131 ),
	EF_FRAME(    0,    0,    3,           0,  1132 ),
	EF_FRAME(    0,    0,    3,           0,  1133 ),
	EF_FRAME(    0,    0,    3,           0,  1145 ),
	EF_FRAME(    0,    0,    3,           0,  1144 ),
	EF_FRAME(    0,    0,    3,           0,  1143 ),
	EF_FRAME(    0,    0,    3,           0,  1137 ),
	EF_FRAME(    0,    0,    3,           0,  1138 ),
	EF_FRAME(    0,    0,    3,           0,  1139 ),
	EF_FRAME(    0,    0,    3,           0,  1140 ),
	EF_FRAME(    0,    0,    3,           0,  1141 ),
	EF_FRAME(    0,    0,    3,           0,  1142 ),
	EF_FRAME(    0,    0,    3,           0,  1136 ),
	EF_FRAME(    0,    0,    3,           0,  1135 ),
	EF_FRAME(    0,    0,    3,           0,  1160 ),
	EF_FRAME(    0,    0,    3,           0,  1161 ),
	EF_FRAME(    0,    0,    3,           0,  1162 ),
	EF_FRAME(    0,    0,    3,           0,  1163 ),
	EF_FRAME(    0,    0,    3,           0,  1164 ),
	EF_FRAME(    0,    0,    3,           0,  1176 ),
	EF_FRAME(    0,    0,    3,           0,  1175 ),
	EF_FRAME(    0,    0,    3,           0,  1174 ),
	EF_FRAME(    0,    0,    3,           0,  1168 ),
	EF_FRAME(    0,    0,    3,           0,  1169 ),
	EF_FRAME(    0,    0,    3,           0,  1170 ),
	EF_FRAME(    0,    0,    3,           0,  1171 ),
	EF_FRAME(    0,    0,    3,           0,  1172 ),
	EF_FRAME(    0,    0,    3,           0,  1173 ),
	EF_FRAME(    0,    0,    3,           0,  1167 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,  1166 ),
	CG_NEXT( pl04_grp27_05 ),
};

/* group 27 16 */
const Sint32 pl04_grp27_16[] = {
	EF_FRAME(    0,    0,   10,           0,   918 ),
	EF_FRAME(    0,    0,   10,           0,   919 ),
	EF_FRAME(    0,    0,   10,           0,   920 ),
	EF_FRAME(    0,    0,   10,           0,   921 ),
	EF_FRAME(    0,    0,    8,           0,   922 ),
};

const Sint32 pl04_grp27_16_loop[] = {
	EF_FRAME(    1,    0,    6,           0,   923 ),
	EF_FRAME(    1,    0,   18,           0,   924 ),
	EF_FRAME(    1,    0,   10,           0,   923 ),
	EF_FRAME(    1,    0,   20, END_OF_DATA,   922 ),
	CG_NEXT( pl04_grp27_16_loop ),
};

/* group 27 17 */
const Sint32 pl04_grp27_17[] = {
	EF_FRAME(    0,    0,    4,           0,   922 ),
	EF_FRAME(    0,    0,    4,           0,   921 ),
	EF_FRAME(    0,    0,    4,           0,   920 ),
	EF_FRAME(    0,    0,    4,           0,   919 ),
};

const Sint32 pl04_grp27_17_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   918 ),
	CG_NEXT( pl04_grp27_17_loop ),
};

/* group 27 18 */
const Sint32 pl04_grp27_18[] = {
	EF_FRAME(    0,    0,   10,           0,   925 ),
	EF_FRAME(    0,    0,   10,           0,   926 ),
	EF_FRAME(    0,    0,   10,           0,   927 ),
	EF_FRAME(    0,    0,   10,           0,   928 ),
	EF_FRAME(    0,    0,    8,           0,   929 ),
};

const Sint32 pl04_grp27_18_loop[] = {
	EF_FRAME(    1,    0,    6,           0,   930 ),
	EF_FRAME(    1,    0,   18,           0,   931 ),
	EF_FRAME(    1,    0,   10,           0,   930 ),
	EF_FRAME(    1,    0,   20, END_OF_DATA,   929 ),
	CG_NEXT( pl04_grp27_18_loop ),
};

/* group 27 19 */
const Sint32 pl04_grp27_19[] = {
	EF_FRAME(    0,    0,    4,           0,   929 ),
	EF_FRAME(    0,    0,    4,           0,   928 ),
	EF_FRAME(    0,    0,    4,           0,   927 ),
	EF_FRAME(    0,    0,    4,           0,   926 ),
};

const Sint32 pl04_grp27_19_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   925 ),
	CG_NEXT( pl04_grp27_19_loop ),
};

/* group 27 6: pls04_00_mummy_init (6 + atck_level) */
const Sint32 pl04_grp27_06[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    3,           0,   499 ),
	EF_FRAME(    0,    0,    3,           0,   500 ),
	EF_FRAME(    0,    0,    3,           0,   493 ),
	EF_FRAME(    0,    0,    3,           0,   494 ),
	EF_FRAME(    0,    0,    3,           0,   495 ),
	EF_FRAME(    0,    0,    3,           0,   496 ),
};

const Sint32 pl04_grp27_06_loop[] = {
	EF_FRAME(    0,    0,    2,           0,   484 ),
	EF_FRAME(    0,    0,    2,           0,   482 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   483 ),
	CG_NEXT( pl04_grp27_06_loop ),
};

/* group 27 7 */
const Sint32 pl04_grp27_07[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,   10,           0,   499 ),
	EF_FRAME(    0,    0,    3,           0,   500 ),
	EF_FRAME(    0,    0,    3,           0,   493 ),
	EF_FRAME(    0,    0,    3,           0,   494 ),
	EF_FRAME(    0,    0,    3,           0,   495 ),
	EF_FRAME(    0,    0,    3,           0,   496 ),
};

const Sint32 pl04_grp27_07_loop[] = {
	EF_FRAME(    0,    0,    2,           0,   484 ),
	EF_FRAME(    0,    0,    2,           0,   482 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   483 ),
	CG_NEXT( pl04_grp27_07_loop ),
};

/* group 27 8: pls04_00_mummy_set */
const Sint32 pl04_grp27_08[] = {
	EF_FRAME(    0,    0,    3,           0,   496 ),
};

const Sint32 pl04_grp27_08_loop[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,   496 ),
	CG_NEXT( pl04_grp27_08_loop ),
};

/* group 27 9: pls04_00_mummy_init (9 + atck_level) */
const Sint32 pl04_grp27_09[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    3,           0,   497 ),
	EF_FRAME(    0,    0,    3,           0,   498 ),
	EF_FRAME(    0,    0,    3,           0,   489 ),
	EF_FRAME(    0,    0,    3,           0,   490 ),
	EF_FRAME(    0,    0,    3,           0,   491 ),
	EF_FRAME(    0,    0,    3,           0,   492 ),
};

const Sint32 pl04_grp27_09_loop[] = {
	EF_FRAME(    0,    0,    2,           0,   479 ),
	EF_FRAME(    0,    0,    2,           0,   480 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   481 ),
	CG_NEXT( pl04_grp27_09_loop ),
};

/* group 27 10 */
const Sint32 pl04_grp27_10[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,   10,           0,   497 ),
	EF_FRAME(    0,    0,    3,           0,   498 ),
	EF_FRAME(    0,    0,    3,           0,   489 ),
	EF_FRAME(    0,    0,    3,           0,   490 ),
	EF_FRAME(    0,    0,    3,           0,   491 ),
	EF_FRAME(    0,    0,    3,           0,   492 ),
};

const Sint32 pl04_grp27_10_loop[] = {
	EF_FRAME(    0,    0,    2,           0,   479 ),
	EF_FRAME(    0,    0,    2,           0,   480 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA,   481 ),
	CG_NEXT( pl04_grp27_10_loop ),
};

/* group 27 11: pls04_00_mummy_set */
const Sint32 pl04_grp27_11[] = {
	EF_FRAME(    0,    0,    3,           0,   492 ),
};

const Sint32 pl04_grp27_11_loop[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,   492 ),
	CG_NEXT( pl04_grp27_11_loop ),
};

/* group 27 12 */
const Sint32 pl04_grp27_12[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,   519 ),
	CG_NEXT( pl04_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl04_grp27_13[] = {
	EF_FRAME(    0,    0,   -1, END_OF_DATA,   520 ),
	CG_NEXT( pl04_grp27_13 ),
};
