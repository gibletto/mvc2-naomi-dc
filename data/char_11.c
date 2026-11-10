/* Jill (character 11): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_11      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_11     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl11.c) says which group
                   and number each move uses.
   pl11_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl11_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl11_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl11_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_11
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_11, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_11 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl11_stand_00[], pl11_stand_00_loop[], pl11_stand_02[], pl11_stand_03[],
	pl11_stand_06[], pl11_jump_00[], pl11_jump_00_loop[], pl11_jump_02[], pl11_jump_02_loop[],
	pl11_jump_03[], pl11_jump_03_loop[], pl11_jump_06[], pl11_jump_09[], pl11_dash_00[],
	pl11_dash_02[], pl11_dash_02_loop[], pl11_dash_01[], pl11_dash_03[], pl11_crouch_00[],
	pl11_crouch_01[], pl11_crouch_02[], pl11_turn_00[], pl11_turn_01[], pl11_guard_00[],
	pl11_guard_01[], pl11_guard_02[], pl11_guard_04[], pl11_guard_05[], pl11_guard_06[],
	pl11_air_guard_00[], pl11_air_guard_01[], pl11_stand_p_00[], pl11_stand_p_01[],
	pl11_stand_p_02[], pl11_stand_k_00[], pl11_stand_k_01[], pl11_stand_k_02[], pl11_crouch_p_00[],
	pl11_crouch_p_01[], pl11_crouch_p_02[], pl11_crouch_k_00[], pl11_crouch_k_01[],
	pl11_crouch_k_02[], pl11_jump_p_00[], pl11_jump_p_01[], pl11_jump_p_02[], pl11_jump_k_00[],
	pl11_jump_k_01[], pl11_jump_k_02[], pl11_damage_00[], pl11_damage_01[], pl11_damage_02[],
	pl11_damage_03[], pl11_damage_04[], pl11_damage_05[], pl11_damage_06[], pl11_damage_07[],
	pl11_damage_08[], pl11_damage_10[], pl11_damage_11[], pl11_damage_12[], pl11_damage_09[],
	pl11_damage_09_loop[], pl11_damage_13[], pl11_damage_16[], pl11_damage_17[], pl11_damage_18[],
	pl11_damage_19[], pl11_damage_20[], pl11_damage_21[], pl11_damage_22[], pl11_damage_23[],
	pl11_damage_24[], pl11_damage_25[], pl11_damage_26[], pl11_damage_27[], pl11_damage_27_loop[],
	pl11_damage_28[], pl11_damage_29[], pl11_damage_33[], pl11_dizzy_00[], pl11_damage_31[],
	pl11_get_up_00[], pl11_get_up_01[], pl11_get_up_02[], pl11_get_up_03[], pl11_appear_00[],
	pl11_appear_01[], pl11_win_00[], pl11_win_00_loop[], pl11_win_01[], pl11_win_01_loop[],
	pl11_win_02[], pl11_win_02_loop[], pl11_win_03[], pl11_win_03_loop[], pl11_catch_00[],
	pl11_catch_00_loop[], pl11_special_00[], pl11_special_00_loop[], pl11_special_01[],
	pl11_special_02[], pl11_special_02_loop[], pl11_special_03[], pl11_special_04[],
	pl11_special_04_loop[], pl11_effect_00[], pl11_effect_01[], pl11_effect_01_loop[],
	pl11_effect_02[], pl11_effect_03[], pl11_effect_03_loop[], pl11_effect_04[],
	pl11_effect_04_loop[], pl11_effect_05[], pl11_effect_05_loop[], pl11_effect_06[],
	pl11_effect_07[], pl11_effect_07_loop[], pl11_effect_08[], pl11_effect_08_loop[],
	pl11_special_05[], pl11_effect_09[], pl11_effect_10[], pl11_effect_11[], pl11_effect_12[],
	pl11_effect_12_loop[], pl11_effect_13[], pl11_effect_13_loop[], pl11_effect_14[],
	pl11_effect_14_loop[], pl11_effect_15[], pl11_effect_15_loop[], pl11_effect_16[],
	pl11_effect_17[], pl11_catch_06[], pl11_catch_07[], pl11_effect_19[], pl11_effect_19_loop[],
	pl11_special_06[], pl11_catch_01[], pl11_catch_01_loop[], pl11_catch_02[], pl11_catch_11[],
	pl11_catch_11_loop[], pl11_catch_03[], pl11_catch_03_loop[], pl11_catch_04[], pl11_catch_05[],
	pl11_special_07[], pl11_special_07_loop[], pl11_special_08[], pl11_special_08_loop[],
	pl11_special_09[], pl11_special_11[], pl11_special_11_loop[], pl11_special_12[],
	pl11_special_12_loop[], pl11_special_13[], pl11_special_13_loop[], pl11_special_15[],
	pl11_special_15_loop[], pl11_special_17[], pl11_special_16[], pl11_special_16_loop[],
	pl11_special_18[], pl11_special_19[], pl11_special_19_loop[], pl11_special_20[],
	pl11_special_20_loop[], pl11_special_21[], pl11_special_21_loop[], pl11_special_23[],
	pl11_special_24[], pl11_special_25[], pl11_special_26[], pl11_super_00[], pl11_super_00_loop[],
	pl11_super_01[], pl11_super_02[], pl11_super_02_loop[], pl11_super_03[], pl11_super_03_loop[],
	pl11_super_04[], pl11_super_04_loop[], pl11_super_05[], pl11_super_05_loop[], pl11_super_06[],
	pl11_super_06_loop[], pl11_super_07[], pl11_super_07_loop[], pl11_super_08[], pl11_super_09[],
	pl11_super_10[], pl11_super_10_loop[], pl11_super_11[], pl11_super_11_loop[], pl11_super_12[],
	pl11_super_12_loop[], pl11_super_13[], pl11_effect_20[], pl11_effect_21[], pl11_effect_22[],
	pl11_effect_23[], pl11_effect_24[], pl11_effect_25[], pl11_effect_25_loop[], pl11_catch_08[],
	pl11_catch_08_loop[], pl11_catch_09[], pl11_catch_09_loop[], pl11_catch_10[],
	pl11_catch_10_loop[], pl11_effect_29[], pl11_effect_29_loop[], pl11_effect_30[],
	pl11_effect_30_loop[], pl11_effect_31[], pl11_effect_32[], pl11_change_00[], pl11_change_02[],
	pl11_change_03[], pl11_change_03_loop[], pl11_change_04[], pl11_change_01[], pl11_extra_00[],
	pl11_extra_00_loop[], pl11_extra_01[], pl11_extra_02[], pl11_extra_03[], pl11_caught_00[],
	pl11_caught_01[], pl11_caught_02[], pl11_caught_03[], pl11_caught_04[], pl11_caught_05[],
	pl11_caught_06[], pl11_caught_07[], pl11_caught_08[], pl11_caught_09[], pl11_caught_10[],
	pl11_caught_11[], pl11_caught_12[], pl11_caught_13[], pl11_caught_14[], pl11_caught_15[],
	pl11_caught_16[], pl11_caught_17[], pl11_caught_18[], pl11_caught_19[], pl11_caught_20[],
	pl11_caught_21[], pl11_grp27_00[], pl11_grp27_01[], pl11_grp27_02[], pl11_grp27_03[],
	pl11_grp27_04[], pl11_grp27_05[], pl11_grp27_06[], pl11_grp27_07[], pl11_grp27_08[],
	pl11_grp27_09[], pl11_grp27_10[], pl11_grp27_10_loop[], pl11_grp27_11[], pl11_grp27_11_loop[],
	pl11_grp27_12[], pl11_grp27_13[], pl11_grp27_14[], pl11_grp27_15[], pl11_grp27_16[],
	pl11_grp27_17[], pl11_grp27_18[], pl11_grp27_19[], pl11_grp27_20[], pl11_grp27_21[],
	pl11_grp27_22[], pl11_grp27_23[], pl11_grp27_24[], pl11_grp27_25[], pl11_grp27_26[],
	pl11_grp27_27[], pl11_grp27_28[], pl11_grp27_29[], pl11_grp27_30[];
extern const Sint32 *const pl11_stand_tbl[], *const pl11_jump_tbl[], *const pl11_dash_tbl[],
	*const pl11_crouch_tbl[], *const pl11_turn_tbl[], *const pl11_guard_tbl[],
	*const pl11_air_guard_tbl[], *const pl11_stand_p_tbl[], *const pl11_stand_k_tbl[],
	*const pl11_crouch_p_tbl[], *const pl11_crouch_k_tbl[], *const pl11_jump_p_tbl[],
	*const pl11_jump_k_tbl[], *const pl11_damage_tbl[], *const pl11_dizzy_tbl[],
	*const pl11_catch_tbl[], *const pl11_caught_tbl[], *const pl11_get_up_tbl[],
	*const pl11_appear_tbl[], *const pl11_win_tbl[], *const pl11_extra_tbl[],
	*const pl11_special_tbl[], *const pl11_super_tbl[], *const pl11_effect_tbl[],
	*const pl11_change_tbl[], *const pl11_grp25_tbl[], *const pl11_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_11[16] = {
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
const Sint32 *const *const char_tbl_11[] = {
	pl11_stand_tbl, pl11_jump_tbl, pl11_dash_tbl, pl11_crouch_tbl, pl11_turn_tbl, pl11_guard_tbl,
	pl11_air_guard_tbl, pl11_stand_p_tbl, pl11_stand_k_tbl, pl11_crouch_p_tbl, pl11_crouch_k_tbl, pl11_jump_p_tbl,
	pl11_jump_k_tbl, pl11_damage_tbl, pl11_dizzy_tbl, pl11_catch_tbl, pl11_caught_tbl, pl11_get_up_tbl,
	pl11_appear_tbl, pl11_win_tbl, pl11_extra_tbl, pl11_special_tbl, pl11_super_tbl, pl11_effect_tbl,
	pl11_change_tbl, pl11_grp25_tbl, pl11_grp25_tbl, pl11_grp27_tbl, 0,
};

const Sint32 *const pl11_stand_tbl[] = {
	pl11_stand_00, pl11_stand_00, pl11_stand_02, pl11_stand_03, pl11_stand_00, pl11_stand_00,
	pl11_stand_06, pl11_stand_06, pl11_stand_00, pl11_stand_00, 0,
};

const Sint32 *const pl11_jump_tbl[] = {
	pl11_jump_00, pl11_jump_00, pl11_jump_02, pl11_jump_03, pl11_jump_03, pl11_jump_03,
	pl11_jump_06, pl11_jump_06, pl11_jump_06, pl11_jump_09, pl11_jump_09, pl11_jump_09,
	0,
};

const Sint32 *const pl11_dash_tbl[] = {
	pl11_dash_00, pl11_dash_01, pl11_dash_02, pl11_dash_03, 0,
};

const Sint32 *const pl11_crouch_tbl[] = {
	pl11_crouch_00, pl11_crouch_01, pl11_crouch_02, 0,
};

const Sint32 *const pl11_turn_tbl[] = {
	pl11_turn_00, pl11_turn_01, 0,
};

const Sint32 *const pl11_guard_tbl[] = {
	pl11_guard_00, pl11_guard_01, pl11_guard_02, pl11_guard_01, pl11_guard_04, pl11_guard_05,
	pl11_guard_06, pl11_guard_05, 0,
};

const Sint32 *const pl11_air_guard_tbl[] = {
	pl11_air_guard_00, pl11_air_guard_01, pl11_air_guard_01, 0,
};

const Sint32 *const pl11_stand_p_tbl[] = {
	pl11_stand_p_00, pl11_stand_p_01, pl11_stand_p_02, 0,
};

const Sint32 *const pl11_stand_k_tbl[] = {
	pl11_stand_k_00, pl11_stand_k_01, pl11_stand_k_02, 0,
};

const Sint32 *const pl11_crouch_p_tbl[] = {
	pl11_crouch_p_00, pl11_crouch_p_01, pl11_crouch_p_02, 0,
};

const Sint32 *const pl11_crouch_k_tbl[] = {
	pl11_crouch_k_00, pl11_crouch_k_01, pl11_crouch_k_02, 0,
};

const Sint32 *const pl11_jump_p_tbl[] = {
	pl11_jump_p_00, pl11_jump_p_01, pl11_jump_p_02, pl11_jump_p_00, pl11_jump_p_01, pl11_jump_p_02,
	0,
};

const Sint32 *const pl11_jump_k_tbl[] = {
	pl11_jump_k_00, pl11_jump_k_01, pl11_jump_k_02, pl11_jump_k_00, pl11_jump_k_01, pl11_jump_k_02,
	0,
};

const Sint32 *const pl11_damage_tbl[] = {
	pl11_damage_00, pl11_damage_01, pl11_damage_02, pl11_damage_03, pl11_damage_04, pl11_damage_05,
	pl11_damage_06, pl11_damage_07, pl11_damage_08, pl11_damage_09, pl11_damage_10, pl11_damage_11,
	pl11_damage_12, pl11_damage_13, pl11_damage_13, pl11_damage_13, pl11_damage_16, pl11_damage_17,
	pl11_damage_18, pl11_damage_19, pl11_damage_20, pl11_damage_21, pl11_damage_22, pl11_damage_23,
	pl11_damage_24, pl11_damage_25, pl11_damage_26, pl11_damage_27, pl11_damage_28, pl11_damage_29,
	pl11_damage_27, pl11_damage_31, pl11_damage_27, pl11_damage_33, pl11_damage_13, 0,
};

const Sint32 *const pl11_dizzy_tbl[] = {
	pl11_dizzy_00, 0,
};

const Sint32 *const pl11_catch_tbl[] = {
	pl11_catch_00, pl11_catch_01, pl11_catch_02, pl11_catch_03, pl11_catch_04, pl11_catch_05,
	pl11_catch_06, pl11_catch_07, pl11_catch_08, pl11_catch_09, pl11_catch_10, pl11_catch_11,
	0,
};

const Sint32 *const pl11_caught_tbl[] = {
	pl11_caught_00, pl11_caught_01, pl11_caught_02, pl11_caught_03, pl11_caught_04, pl11_caught_05,
	pl11_caught_06, pl11_caught_07, pl11_caught_08, pl11_caught_09, pl11_caught_10, pl11_caught_11,
	pl11_caught_12, pl11_caught_13, pl11_caught_14, pl11_caught_15, pl11_caught_16, pl11_caught_17,
	pl11_caught_18, pl11_caught_19, pl11_caught_20, pl11_caught_21, 0,
};

const Sint32 *const pl11_get_up_tbl[] = {
	pl11_get_up_00, pl11_get_up_01, pl11_get_up_02, pl11_get_up_03, 0,
};

const Sint32 *const pl11_appear_tbl[] = {
	pl11_appear_00, pl11_appear_01, 0,
};

const Sint32 *const pl11_win_tbl[] = {
	pl11_win_00, pl11_win_01, pl11_win_02, pl11_win_03, 0,
};

const Sint32 *const pl11_extra_tbl[] = {
	pl11_extra_00, pl11_extra_01, pl11_extra_02, pl11_extra_03, 0,
};

const Sint32 *const pl11_special_tbl[] = {
	pl11_special_00, pl11_special_01, pl11_special_02, pl11_special_03, pl11_special_04, pl11_special_05,
	pl11_special_06, pl11_special_07, pl11_special_08, pl11_special_09, pl11_special_09, pl11_special_11,
	pl11_special_12, pl11_special_13, pl11_special_13, pl11_special_15, pl11_special_16, pl11_special_17,
	pl11_special_18, pl11_special_19, pl11_special_20, pl11_special_21, pl11_special_21, pl11_special_23,
	pl11_special_24, pl11_special_25, pl11_special_26, 0,
};

const Sint32 *const pl11_super_tbl[] = {
	pl11_super_00, pl11_super_01, pl11_super_02, pl11_super_03, pl11_super_04, pl11_super_05,
	pl11_super_06, pl11_super_07, pl11_super_08, pl11_super_09, pl11_super_10, pl11_super_11,
	pl11_super_12, pl11_super_13, 0,
};

const Sint32 *const pl11_effect_tbl[] = {
	pl11_effect_00, pl11_effect_01, pl11_effect_02, pl11_effect_03, pl11_effect_04, pl11_effect_05,
	pl11_effect_06, pl11_effect_07, pl11_effect_08, pl11_effect_09, pl11_effect_10, pl11_effect_11,
	pl11_effect_12, pl11_effect_13, pl11_effect_14, pl11_effect_15, pl11_effect_16, pl11_effect_17,
	pl11_effect_15, pl11_effect_19, pl11_effect_20, pl11_effect_21, pl11_effect_22, pl11_effect_23,
	pl11_effect_24, pl11_effect_25, pl11_catch_08, pl11_catch_09, pl11_catch_10, pl11_effect_29,
	pl11_effect_30, pl11_effect_31, pl11_effect_32, 0,
};

const Sint32 *const pl11_change_tbl[] = {
	pl11_change_00, pl11_change_01, pl11_change_02, pl11_change_03, pl11_change_04, 0,
};

const Sint32 *const pl11_grp25_tbl[] = {
	pl11_stand_00, 0,
};

const Sint32 *const pl11_grp27_tbl[] = {
	pl11_grp27_00, pl11_grp27_01, pl11_grp27_02, pl11_grp27_03, pl11_grp27_04, pl11_grp27_05,
	pl11_grp27_06, pl11_grp27_07, pl11_grp27_08, pl11_grp27_09, pl11_grp27_10, pl11_grp27_11,
	pl11_grp27_12, pl11_grp27_13, pl11_grp27_14, pl11_grp27_15, pl11_grp27_16, pl11_grp27_17,
	pl11_grp27_18, pl11_grp27_19, pl11_grp27_20, pl11_grp27_21, pl11_grp27_22, pl11_grp27_23,
	pl11_grp27_24, pl11_grp27_25, pl11_grp27_26, pl11_grp27_27, pl11_grp27_28, pl11_grp27_29,
	pl11_grp27_30, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl11_stand_00[] = {
	CG_FRAME(    0,    0,    5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_stand_00_loop[] = {
	CG_FRAME(    0,    0,    5,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_00_loop ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl11_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl11_stand_03[] = {
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_03 ),
};

/* Char_Stand 6
   Char_Stand 7 */
const Sint32 pl11_stand_06[] = {
	CG_FRAME(    0,    0,    4,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_06 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl11_jump_00[] = {
	CG_FRAME(    0,    1,    2,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   105, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   106, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   107, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   108, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   109, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   110, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   111, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl11_jump_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_00_loop ),
};

/* Char_Jump 2 */
const Sint32 pl11_jump_02[] = {
	CG_FRAME(    0,    1,    2,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   117, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   125, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl11_jump_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   112, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_02_loop ),
};

/* Char_Jump 3: stars_j_fall (Charging Stars), hstars_j_fall (Hyper Charging Stars (hyper))
   Char_Jump 4
   Char_Jump 5: nml_catch_fall */
const Sint32 pl11_jump_03[] = {
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_jump_03_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_jump_03_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl11_jump_06[] = {
	CG_FRAME(    0,    1,    2,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   105, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   105, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   105, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   106, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   107, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   108, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   109, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   110, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   111, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl11_jump_09[] = {
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   143, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl11_dash_00[] = {
	CG_FRAME(    0,    1,    3, END_OF_DATA,    96, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl11_dash_00 ),
};

/* Char_Dash 2 */
const Sint32 pl11_dash_02[] = {
	CG_FRAME(    1,    0,    3,           0,    97, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,    3,           0,    98, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,    3,           0,    99, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_dash_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl11_dash_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   101, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_dash_01 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl11_dash_03[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl11_crouch_00[] = {
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl11_crouch_01[] = {
	CG_FRAME(    0,    0,    5,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl11_crouch_02[] = {
	CG_FRAME(    0,    0,    3,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl11_turn_00[] = {
	CG_FRAME(    0,    0,    3,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl11_turn_01[] = {
	CG_FRAME(    0,    0,    3,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl11_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl11_guard_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl11_guard_02[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl11_guard_04[] = {
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl11_guard_05[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl11_guard_06[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl11_air_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl11_air_guard_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   141, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl11_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   238, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   237, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl11_stand_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   241, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    5,           0,   242, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    5,           0,   243, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   244, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl11_stand_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   245, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   246, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   247, 0x80,   0x4d,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    5,           0,   248, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   250, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   251, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   237, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl11_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   252, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   253, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    5,           0,   254, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    3,           0,   255, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   256, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl11_stand_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   243, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   257, 0x80,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    5,           0,   258, 0x80,   0x41,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   259, 0x80,   0x41,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    5,           0,   260, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   244, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl11_stand_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   261, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   262, 0x80,   0x46,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   263, 0x80,   0x46,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   264, 0x80,   0x70,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   265, 0x80,   0x70,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   266, 0x80,   0x46,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   267, 0x80,   0x46,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   268, 0x80,   0x09,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   269, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl11_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   270, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    5,           0,   272, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   270, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl11_crouch_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    5,           0,   275, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    5,           0,   276, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   277, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl11_crouch_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    1,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,   16,    1,           0,   281, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,    1,           0,   282, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   283, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    3,           0,   284, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,   16,    3,           0,   285, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   128, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl11_crouch_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   286, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   287, 0x80,   0x6a,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0x6a,  ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    3,           0,   286, 0x80,   0x4d,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   133, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl11_crouch_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   289, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   290, 0x80,   0x85,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    5,           0,   291, 0x80,   0x85,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    5,           0,   292, 0x80,   0x4b,  ATE,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   277, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl11_crouch_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x4e,  ATK,  0x00,       0x03,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x4e,  ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    9,           0,   296, 0x80,   0xac,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    1,    6,           0,   297, 0x80,   0xac,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    8,           0,   298, 0x80,   0xac,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   299, 0x80,   0x4e,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl11_jump_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    3,           0,   303, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl11_jump_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   306, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    5,           0,   306, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   307, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl11_jump_p_02[] = {
	CG_FRAME(    0,    0,    8,           0,   308, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   309, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   310, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,    2,           0,   311, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,    1,           0,   312, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   313, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    3,           0,   314, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   303, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl11_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   317, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   318, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    5,           0,   318, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   319, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl11_jump_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   320, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    5,           0,   322, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl11_jump_k_02[] = {
	CG_FRAME(    0,    0,    8,           0,   324, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   326, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    9,           0,   327, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl11_damage_00[] = {
	CG_FRAME(    0,    0,    5,           0,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl11_damage_01[] = {
	CG_FRAME(    0,    0,    6,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl11_damage_02[] = {
	CG_FRAME(    0,    0,    6,           0,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl11_damage_03[] = {
	CG_FRAME(    0,    0,    5,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl11_damage_04[] = {
	CG_FRAME(    0,    0,    6,           0,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_damage_04 ),
};

/* Char_Damage 5: shl11_03_b_reach, shl11_03_b_near_hit, shl11_03_b_far_hit, shl11_03_b_last_hit */
const Sint32 pl11_damage_05[] = {
	CG_FRAME(    0,    0,    6,           0,   470, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl11_damage_06[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   471, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl11_damage_07[] = {
	CG_FRAME(    0,    0,    9,           0,   472, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   471, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl11_damage_08[] = {
	CG_FRAME(    0,    0,    8,           0,   473, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   472, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   471, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl11_damage_10[] = {
	CG_FRAME(    0,    0,    5,           0,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   474, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl11_damage_11[] = {
	CG_FRAME(    0,    0,    3,           0,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl11_damage_12[] = {
	CG_FRAME(    0,    0,    2,           0,   477, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   474, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_12 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl11_damage_09[] = {
	CG_FRAME(    0,    0,    4,           0,   479, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   480, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   481, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   482, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   483, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl11_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl11_damage_09_loop ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl11_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   478, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   484, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl11_damage_13 ),
};

/* Char_Damage 16: shl11_03_held_wait, dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl11_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   486, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl11_damage_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   487, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl11_damage_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   488, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl11_damage_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl11_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_20 ),
};

/* Char_Damage 21: shl11_03_held_up, dm_fall_char_set (21 + no) */
const Sint32 pl11_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   486, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl11_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   487, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl11_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   488, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl11_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl11_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl11_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   491, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    3,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl11_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl11_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   488, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl11_damage_27_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_27_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl11_damage_28[] = {
	CG_FRAME(    0,    0,    3,           0,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   499, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   501, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl11_damage_29[] = {
	CG_FRAME(    0,    0,    3,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   507, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_29 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl11_damage_33[] = {
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl11_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   467, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_dizzy_00 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl11_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl11_damage_31 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl11_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl11_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl11_get_up_02[] = {
	CG_FRAME(    0,    0,    3,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl11_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_get_up_02 ),
};

/* Char_Appear 0 */
const Sint32 pl11_appear_00[] = {
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   24,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_appear_00 ),
};

/* Char_Appear 1 */
const Sint32 pl11_appear_01[] = {
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   24,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_appear_01 ),
};

/* Char_Win 0 */
const Sint32 pl11_win_00[] = {
	CG_FRAME(    0,    0,    4,           0,   201, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_win_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl11_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   219, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   48,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    4,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   20,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_win_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   226, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   230, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   233, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl11_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   380, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   396, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_win_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_win_02_loop ),
};

/* Char_Win 3: lose_time_ko */
const Sint32 pl11_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   463, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_win_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_win_03_loop ),
};

/* Char_Catch 0: nml_p_set, jump_p_set */
const Sint32 pl11_catch_00[] = {
	CG_FRAME(    0,    0,    8,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    1,    6,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,    8,           0,   333, 0x80,   0x00,    0,  0x00,       0x05,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,    4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,    4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    1,    4,           0,   336, 0x80,   0x00,    0,  0x00, 0x19 | CSE,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_catch_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_catch_00_loop ),
};

/* Char_Special 0: cerberus_init (Cerberus Escape) */
const Sint32 pl11_special_00[] = {
	CG_FRAME(    0,    1,    3,           0,   352, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   355, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_00_loop ),
};

/* Char_Special 1: crow_shot (Crow Escape) */
const Sint32 pl11_special_01[] = {
	CG_FRAME(    0,    0,    3,           0,   364, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   365, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    1,           0,   366, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   367, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_special_01 ),
};

/* Char_Special 2: cerberus_shot (Cerberus Escape) */
const Sint32 pl11_special_02[] = {
	CG_FRAME(    0,    1,    3,           0,   356, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   357, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   359, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   125, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   112, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_02_loop ),
};

/* Char_Special 3: cerberus_fall (Cerberus Escape) */
const Sint32 pl11_special_03[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   126, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_03 ),
};

/* Char_Special 4: crow_init (Crow Escape) */
const Sint32 pl11_special_04[] = {
	CG_FRAME(    0,    1,    3,           0,   352, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   361, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   362, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_04_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   363, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_04_loop ),
};

/* Char_Effect 0: shl11_00_a_init */
const Sint32 pl11_effect_00[] = {
	CG_FRAME(    0,    0,    3,           0,   508, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   509, 0x80,   0x80,  ATK,  0x00,       0x33,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    6,           0,   510, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   511, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   512, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   513, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl11_effect_00 ),
};

/* Char_Effect 1: shl11_00_a_arc */
const Sint32 pl11_effect_01[] = {
	CG_FRAME(    0,    0,    5,           0,   514, 0x80,   0xa0,  ATK,  0x00,       0x34,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    6,           0,   515, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   10,           0,   516, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
};

const Sint32 pl11_effect_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   517, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl11_effect_01_loop ),
};

/* Char_Effect 2: shl11_00_a_land */
const Sint32 pl11_effect_02[] = {
	CG_FRAME(    0,    1,    4,           0,   511, 0x80,   0x80,  ATK,  0x00,       0x33,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   512, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   513, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl11_effect_00 ),
};

/* Char_Effect 3: shl11_00_a_hit_init */
const Sint32 pl11_effect_03[] = {
	CG_FRAME(    0,    0,    3,           0,   518, 0x80,   0x00,  ATE,  0x00,       0x32,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   519, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_03_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   520, 0x80,   0xa0,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_03_loop ),
};

/* Char_Effect 4: shl11_00_a_hit_fall */
const Sint32 pl11_effect_04[] = {
	CG_FRAME(    0,    0,    3,           0,   521, 0x80,   0x80,  ATE,  0x00, 0x2c | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_04_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   522, 0x80,   0x80,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_04_loop ),
};

/* Char_Effect 5: shl11_00_b_init */
const Sint32 pl11_effect_05[] = {
	CG_FRAME(    0,    0,    3,           0,   523, 0x80,   0x00,  ATK,  0x00,       0x2f,    0,   0,     0,      0,  52 ),
};

const Sint32 pl11_effect_05_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   523, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl11_effect_05_loop ),
};

/* Char_Effect 6: shl11_00_b_fly */
const Sint32 pl11_effect_06[] = {
	CG_FRAME(    0,    0,    7,           0,   524, 0x80,   0x00,  ATK,  0x00,       0x2d,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    5,           0,   525, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    5,           0,   526, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl11_effect_06 ),
};

/* Char_Effect 7: shl11_00_b_hit_init */
const Sint32 pl11_effect_07[] = {
	CG_FRAME(    0,    0,    3,           0,   528, 0x80,   0x00,  ATE,  0x00,       0x2e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_07_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_07_loop ),
};

/* Char_Effect 8: shl11_00_b_hit_fall */
const Sint32 pl11_effect_08[] = {
	CG_FRAME(    0,    0,    3,           0,   531, 0x80,   0x00,  ATE,  0x00, 0x2c | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_08_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   532, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_08_loop ),
};

/* Char_Special 5: zombie_init (Zombie Escape) */
const Sint32 pl11_special_05[] = {
	CG_FRAME(    0,    0,    4,           0,   352, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   368, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   369, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   370, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   130, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_05 ),
};

/* Char_Effect 9: shl11_01_leave */
const Sint32 pl11_effect_09[] = {
	CG_FRAME(    0,    0,    8,           0,   533, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   534, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   535, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   536, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   537, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   538, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   539, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   540, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   541, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   542, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8,           0,   543, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   544, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl11_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl11_effect_10[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   551, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl11_effect_11[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   545, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_11 ),
};

/* Char_Effect 12: shl11_01_c_fly */
const Sint32 pl11_effect_12[] = {
	CG_FRAME(    0,    0,    3,           0,   551, 0x80,   0x00,    0,  0x03,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   552, 0x80,   0x00,    0,  0x04,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   553, 0x80,   0x00,    0,  0x05,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_12_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   554, 0x80,   0x80,    0,  0x06,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_12_loop ),
};

/* Char_Effect 13: shl11_01_c_fall */
const Sint32 pl11_effect_13[] = {
	CG_FRAME(    0,    0,    3,           0,   555, 0x80,   0x80,    0,  0x07, 0x2c | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_13_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   556, 0x80,   0x80,    0,  0x08,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_13_loop ),
};

/* Char_Effect 14 */
const Sint32 pl11_effect_14[] = {
	CG_FRAME(    0,    0,    3,           0,   545, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   546, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   547, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_14_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   548, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_14_loop ),
};

/* Char_Effect 15
   Char_Effect 18 */
const Sint32 pl11_effect_15[] = {
	CG_FRAME(    0,    0,    3,           0,   549, 0x80,   0xc0,    0,  0x00, 0x2c | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_15_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   550, 0x80,   0xc0,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_15_loop ),
};

/* Char_Effect 16 */
const Sint32 pl11_effect_16[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   556, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl11_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl11_effect_17[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   550, 0x80,   0xc0,    0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl11_effect_17 ),
};

/* Char_Catch 6: shl11_01_a_seek */
const Sint32 pl11_catch_06[] = {
	CG_FRAME(    0,    0,    8,           0,   557, 0x80,   0x00,    0,  0x00, 0x0c | CSE,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,    6,           0,   558, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   559, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl11_catch_06 ),
};

/* Char_Catch 7: shl11_01_a_down */
const Sint32 pl11_catch_07[] = {
	CG_FRAME(    0,    0,    8,           0,   560, 0x80,   0x00,    0,  0x00, 0x0c | CSE,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    6,           0,   561, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   562, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_NEXT( pl11_catch_07 ),
};

/* Char_Effect 19: shl11_01_a_down_held */
const Sint32 pl11_effect_19[] = {
	CG_FRAME(    0,    0,    3,           0,   548, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   549, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_19_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   550, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_19_loop ),
};

/* Char_Special 6: return_flash (Return Fire) */
const Sint32 pl11_special_06[] = {
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    1,    3,           0,   374, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   375, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_special_06 ),
};

/* Char_Catch 1: sp_catch_set */
const Sint32 pl11_catch_01[] = {
	CG_FRAME(    0,    0,    3,           0,   375, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    3,           0,   376, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    3,           0,   377, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    3,           0,   378, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    3,           0,   380, 0x40,   0x00,    0,  0x00,       0x15,    0,   0,     0, 0x0005,   0 ),
};

const Sint32 pl11_catch_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   381, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_NEXT( pl11_catch_01_loop ),
};

/* Char_Catch 2: sp_catch_pull */
const Sint32 pl11_catch_02[] = {
	CG_FRAME(    0,    0,    1,           0,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   382, 0x80,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_02 ),
};

/* Char_Catch 11: sp_catch_hit */
const Sint32 pl11_catch_11[] = {
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_catch_11_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_11_loop ),
};

/* Char_Catch 3: sp_catch_next */
const Sint32 pl11_catch_03[] = {
	CG_FRAME(    0,    1,    3,           0,   382, 0x80,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_catch_03_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_03_loop ),
};

/* Char_Catch 4: sp_catch_release */
const Sint32 pl11_catch_04[] = {
	CG_FRAME(    0,    0,    6,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   399, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_04 ),
};

/* Char_Catch 5: sp_catch_next */
const Sint32 pl11_catch_05[] = {
	CG_FRAME(    0,    1,    3,           0,   382, 0x80,   0x00,    0,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    6,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   395, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   399, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_05 ),
};

/* Char_Special 7: stars_init (Charging Stars, 7 + atck_level) */
const Sint32 pl11_special_07[] = {
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    4,           0,   402, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  73 ),
};

const Sint32 pl11_special_07_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl11_special_07_loop ),
};

/* Char_Special 8 */
const Sint32 pl11_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,   401, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    8,           0,   402, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    4,           0,   403, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  73 ),
};

const Sint32 pl11_special_08_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl11_special_08_loop ),
};

/* Char_Special 9: stars_rush (Charging Stars, 9 + atck_level)
   Char_Special 10 */
const Sint32 pl11_special_09[] = {
	CG_FRAME(    0,    1,    2,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,   405, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   10,           0,   406, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   407, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl11_special_09 ),
};

/* Char_Special 11: stars_j_init (Charging Stars, 11 + atck_level) */
const Sint32 pl11_special_11[] = {
	CG_FRAME(    0,    0,    2,           0,   422, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   8 ),
};

const Sint32 pl11_special_11_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl11_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl11_special_12[] = {
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   424, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   8 ),
};

const Sint32 pl11_special_12_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl11_special_12_loop ),
};

/* Char_Special 13: stars_j_rush (Charging Stars, 13 + atck_level)
   Char_Special 14 */
const Sint32 pl11_special_13[] = {
	CG_FRAME(    0,    1,    4,           0,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    6,           0,   307, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl11_special_13_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl11_special_13_loop ),
};

/* Char_Special 15: grenade_init (Grenade Launcher, 15 + atck_level) */
const Sint32 pl11_special_15[] = {
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   411, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   15,           0,   412, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   412, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   413, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   414, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   415, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_15_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_15_loop ),
};

/* Char_Special 17: grenade_wait (Grenade Launcher, 17 + atck_level) */
const Sint32 pl11_special_17[] = {
	CG_FRAME(    0,    0,    4,           0,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_17 ),
};

/* Char_Special 16 */
const Sint32 pl11_special_16[] = {
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   15,           0,   418, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   418, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   419, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   420, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   421, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   421, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_16_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   418, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_16_loop ),
};

/* Char_Special 18 */
const Sint32 pl11_special_18[] = {
	CG_FRAME(    0,    0,    4,           0,   417, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   416, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_18 ),
};

/* Char_Special 19: heal_init (the heal for the partner on the point (assist), 19 + timer1) */
const Sint32 pl11_special_19[] = {
	CG_FRAME(    0,    0,    4,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_19_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl11_special_19_loop ),
};

/* Char_Special 20 */
const Sint32 pl11_special_20[] = {
	CG_FRAME(    0,    0,    4,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   347, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_20_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl11_special_20_loop ),
};

/* Char_Special 21: heal_hit (the heal for the partner on the point (assist), 21 + timer1)
   Char_Special 22 */
const Sint32 pl11_special_21[] = {
	CG_FRAME(    0,    0,   32,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_special_21_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_21_loop ),
};

/* Char_Special 23: heal_hit (the heal for the partner on the point (assist), 23 + timer1) */
const Sint32 pl11_special_23[] = {
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_23 ),
};

/* Char_Special 24 */
const Sint32 pl11_special_24[] = {
	CG_FRAME(    0,    0,    4,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_24 ),
};

/* Char_Special 25: snapback_init (the snapback) */
const Sint32 pl11_special_25[] = {
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   262, 0x80,   0x46,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    3,           0,   263, 0x80,   0x46,  ATK,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   264, 0x80,   0x70,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   265, 0x80,   0x70,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   266, 0x80,   0x46,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   267, 0x80,   0x46,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   268, 0x80,   0x09,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   269, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_25 ),
};

/* Char_Special 26: return_init (Return Fire) */
const Sint32 pl11_special_26[] = {
	CG_FRAME(    0,    0,    6,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   32,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    8,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_special_26 ),
};

/* Char_Super 0: hstars_init (Hyper Charging Stars (hyper)) */
const Sint32 pl11_super_00[] = {
	CG_FRAME(    0,    0,    4,           0,   401, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    8,           0,   402, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    1,    1,           0,   402, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    4,           0,   403, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0,      0,  73 ),
};

const Sint32 pl11_super_00_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl11_super_00_loop ),
};

/* Char_Super 1: hstars_rush (Hyper Charging Stars (hyper)) */
const Sint32 pl11_super_01[] = {
	CG_FRAME(    0,    0,    3,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   405, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   16,           0,   406, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    5,           0,   407, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl11_super_01 ),
};

/* Char_Super 2: hstars_j_init (Hyper Charging Stars (hyper)) */
const Sint32 pl11_super_02[] = {
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    1,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   424, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0,      0,   8 ),
};

const Sint32 pl11_super_02_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl11_super_02_loop ),
};

/* Char_Super 3: hstars_j_rush (Hyper Charging Stars (hyper)) */
const Sint32 pl11_super_03[] = {
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   14,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl11_super_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl11_super_03_loop ),
};

/* Char_Super 4: rocket_init (Rocket Launcher (hyper)) */
const Sint32 pl11_super_04[] = {
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   428, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_super_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   435, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   436, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_super_04_loop ),
};

/* Char_Super 5: rocket_shot_end (Rocket Launcher (hyper)) */
const Sint32 pl11_super_05[] = {
	CG_FRAME(    0,    0,    4,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   435, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   437, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   438, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   439, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl11_super_05_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   440, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_super_05_loop ),
};

/* Char_Super 6: rocket_fall */
const Sint32 pl11_super_06[] = {
	CG_FRAME(    0,    0,    1,           0,   491, 0x80,   0x00,  ATK,  0x00, 0x2c | CSE,    0,   0,     0,      0,   4 ),
};

const Sint32 pl11_super_06_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   491, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl11_super_06_loop ),
};

/* Char_Super 7: rocket_land */
const Sint32 pl11_super_07[] = {
	CG_FRAME(    0,    0,    6,           0,   492, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl11_super_07_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   493, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl11_super_07_loop ),
};

/* Char_Super 8: rocket_hop */
const Sint32 pl11_super_08[] = {
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   144, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_super_08 ),
};

/* Char_Super 9: tyrant_init (Tyrant Escape (hyper)) */
const Sint32 pl11_super_09[] = {
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   223, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    4,           0,   224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    4,           0,   224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   225, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_super_09 ),
};

/* Char_Super 10: tyrant_catch_ck (Tyrant Escape (hyper)) */
const Sint32 pl11_super_10[] = {
	CG_FRAME(    0,    0,    1,           0,   224, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   225, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_super_10_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   445, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   446, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   447, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   448, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   449, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   450, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   451, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   452, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   453, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   455, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   456, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_super_10_loop ),
};

/* Char_Super 11: tyrant_slide (Tyrant Escape (hyper)) */
const Sint32 pl11_super_11[] = {
	CG_FRAME(    0,    0,    4,           0,   457, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   458, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_super_11_loop[] = {
	CG_FRAME(    0,    0,   32, END_OF_DATA,   461, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_super_11_loop ),
};

/* Char_Super 12: tyrant_com_ck (Tyrant Escape (hyper)) */
const Sint32 pl11_super_12[] = {
	CG_FRAME(    0,    0,    3,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   144, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    1,    4,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,    4,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   428, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   430, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    4,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,    1,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   435, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   437, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   438, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   439, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl11_super_12_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   440, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_super_12_loop ),
};

/* Char_Super 13: tyrant_hit_ck (Tyrant Escape (hyper)), tyrant_com_ck (Tyrant Escape (hyper)) */
const Sint32 pl11_super_13[] = {
	CG_FRAME(    0,    0,    4,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   144, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_super_13 ),
};

/* Char_Effect 20: shl11_02_a_shot */
const Sint32 pl11_effect_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   646, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl11_effect_20 ),
};

/* Char_Effect 21: shl11_02_a_fly, shl11_02_b_fly */
const Sint32 pl11_effect_21[] = {
	CG_FRAME(    0,    0,    4,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32792, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32798, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_21 ),
};

/* Char_Effect 22: shl11_02_b_init, shl11_02_c_init */
const Sint32 pl11_effect_22[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl11_effect_22 ),
};

/* Char_Effect 23: shl11_03_a_fly */
const Sint32 pl11_effect_23[] = {
	CG_FRAME(    0,    0,    4,           0,   694, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   695, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   698, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   699, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   700, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   701, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   702, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   703, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   704, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   705, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   706, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   707, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   708, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   709, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   710, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   711, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   712, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   714, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   715, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   716, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   717, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_23 ),
};

/* Char_Effect 24: shl11_03_b_wait */
const Sint32 pl11_effect_24[] = {
	CG_FRAME(    0,    0,   20,           0,    -1, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   661, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   60,           0,   661, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   672, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   673, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   675, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   676, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_24 ),
};

/* Char_Effect 25: shl11_03_b_reach */
const Sint32 pl11_effect_25[] = {
	CG_FRAME(    0,    0,    4,           0,   661, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   662, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   663, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   664, 0x80,   0xff,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   665, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   666, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   667, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   668, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   669, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   670, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   671, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_25_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   677, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_25_loop ),
};

/* Char_Catch 8
   Char_Effect 26: shl11_03_b_near_wait */
const Sint32 pl11_catch_08[] = {
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   679, 0x80,   0xab,  ATK,  0x01,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,    4,           0,   680, 0x80,   0xab,  ATK,  0x00,       0x25,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    1,    4,           0,   681, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
};

const Sint32 pl11_catch_08_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   681, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_08_loop ),
};

/* Char_Catch 9
   Char_Effect 27: shl11_03_b_far_wait */
const Sint32 pl11_catch_09[] = {
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   683, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   684, 0x80,   0xa0,  ATK,  0x01,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    1,    4,           0,   685, 0x80,   0xa0,  ATK,  0x00,       0x25,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    1,    4,           0,   686, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
};

const Sint32 pl11_catch_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   686, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_09_loop ),
};

/* Char_Catch 10
   Char_Effect 28: shl11_03_b_last_wait */
const Sint32 pl11_catch_10[] = {
	CG_FRAME(    0,    0,    4,           0,   687, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   679, 0x80,   0xab,  ATK,  0x01,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,    4,           0,   680, 0x80,   0xab,  ATK,  0x00,       0x25,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    1,    4,           0,   681, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
};

const Sint32 pl11_catch_10_loop[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   681, 0x80,   0xab,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_catch_10_loop ),
};

/* Char_Effect 29: shl11_03_b_last_hit */
const Sint32 pl11_effect_29[] = {
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4,           0,   688, 0x80,   0x80,  ATK,  0x00,       0x26,    0,   0,     0,      0,  59 ),
};

const Sint32 pl11_effect_29_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   689, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4,           0,   690, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4,           0,   691, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   692, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl11_effect_29_loop ),
};

/* Char_Effect 30: shl11_03_fail */
const Sint32 pl11_effect_30[] = {
	CG_FRAME(    0,    1,    4,           0,   688, 0x80,   0x60,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   677, 0x80,   0x60,  ATK,  0x00, 0x32 | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl11_effect_30_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   693, 0x80,   0x60,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_effect_30_loop ),
};

/* Char_Effect 31: shl11_03_b_shot_wait */
const Sint32 pl11_effect_31[] = {
	CG_FRAME(    0,    0,    4,           0,   689, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4,           0,   690, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4,           0,   691, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   692, 0x80,   0xa0,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl11_effect_31 ),
};

/* Char_Effect 32: shl11_01_c_init */
const Sint32 pl11_effect_32[] = {
	CG_FRAME(    0,    0,    8,           0,   533, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   534, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   535, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   536, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   537, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   538, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   539, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   540, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   541, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   542, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   543, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   544, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl11_effect_32 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl11_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x80,   0x00,  ATK,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   10, END_OF_DATA,   159, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl11_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl11_change_02[] = {
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl11_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl11_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl11_change_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl11_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_change_04 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl11_change_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,   154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,   154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_change_01 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl11_extra_00[] = {
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   317, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
};

const Sint32 pl11_extra_00_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   318, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl11_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl11_extra_01[] = {
	CG_FRAME(    0,    0,    6,           0,   160, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   161, 0x80,   0x00,  ATE,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   162, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   161, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   160, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_extra_01 ),
};

/* Char_Extra 2: sp_chouhatsu (the taunt), fly_start5 */
const Sint32 pl11_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl11_extra_02 ),
};

/* Char_Extra 3: pl11_upper, fly_start5 */
const Sint32 pl11_extra_03[] = {
	CG_FRAME(    0,    0,    3,           0,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    1,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,   16,    1,           0,   281, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,    1,           0,   282, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    5,           0,   283, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   284, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,   16,    3,           0,   285, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   126, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   128, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl11_extra_03 ),
};

/* Char_Caught 0 */
const Sint32 pl11_caught_00[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   464, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl11_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl11_caught_01[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   465, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl11_caught_01 ),
};

/* Char_Caught 2: shl11_01_hold */
const Sint32 pl11_caught_02[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   466, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl11_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl11_caught_03[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   468, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl11_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl11_caught_04[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl11_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl11_caught_05[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   470, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl11_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl11_caught_06[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   471, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl11_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl11_caught_07[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   472, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl11_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl11_caught_08[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   473, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl11_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl11_caught_09[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   475, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl11_caught_10[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   476, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_10 ),
};

/* Char_Caught 11: shl11_01_hold */
const Sint32 pl11_caught_11[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   477, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl11_caught_12[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   478, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl11_caught_12 ),
};

/* Char_Caught 13: shl11_01_hold */
const Sint32 pl11_caught_13[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   485, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl11_caught_14[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   487, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl11_caught_15[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   488, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl11_caught_16[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   489, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl11_caught_17[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   490, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_17 ),
};

/* Char_Caught 18: shl11_01_hold */
const Sint32 pl11_caught_18[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl11_caught_19[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl11_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl11_caught_20[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl11_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl11_caught_21[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl11_caught_21 ),
};

/* group 27 0: pls11_00_shell_init */
const Sint32 pl11_grp27_00[] = {
	EF_FRAME(    0,    0,    3,           0,   563 ),
	EF_FRAME(    0,    0,    3,           0,   564 ),
	EF_FRAME(    0,    0,    3,           0,   565 ),
	EF_FRAME(    0,    0,    3,           0,   566 ),
	EF_FRAME(    0,    0,    3,           0,   567 ),
	EF_FRAME(    0,    0,    3,           0,   568 ),
	EF_FRAME(    0,    0,    3,           0,   569 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   570 ),
	CG_NEXT( pl11_grp27_00 ),
};

/* group 27 1: pls11_00_t1_init */
const Sint32 pl11_grp27_01[] = {
	EF_FRAME(    0,    0,    3,           0,   571 ),
	EF_FRAME(    0,    0,    3,           0,   572 ),
	EF_FRAME(    0,    0,    3,           0,   573 ),
	EF_FRAME(    0,    0,    3,           0,   574 ),
	EF_FRAME(    0,    0,    3,           0,   575 ),
	EF_FRAME(    0,    0,    3,           0,   576 ),
	EF_FRAME(    0,    0,    3,           0,   577 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   578 ),
	CG_NEXT( pl11_grp27_01 ),
};

/* group 27 2: pls11_00_flash_init */
const Sint32 pl11_grp27_02[] = {
	EF_FRAME(    0,    0,    3,           0,   752 ),
	EF_FRAME(    0,    0,    3,           0,   753 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   754 ),
	CG_NEXT( pl11_grp27_02 ),
};

/* group 27 3: pls11_00_appear_init (3 + sub_type) */
const Sint32 pl11_grp27_03[] = {
	EF_FRAME(    0,    0,    3,           0,   593 ),
	EF_FRAME(    0,    0,    3,           0,   594 ),
	EF_FRAME(    0,    0,    3,           0,   595 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   596 ),
	CG_NEXT( pl11_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl11_grp27_04[] = {
	EF_FRAME(    0,    0,    3,           0,   597 ),
	EF_FRAME(    0,    0,    3,           0,   598 ),
	EF_FRAME(    0,    0,    3,           0,   599 ),
	EF_FRAME(    0,    0,    3,           0,   600 ),
	EF_FRAME(    0,    0,    3,           0,   601 ),
	EF_FRAME(    0,    0,    3,           0,   602 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   603 ),
	CG_NEXT( pl11_grp27_04 ),
};

/* group 27 5: pls11_00_stars_init (5 + sub_type) */
const Sint32 pl11_grp27_05[] = {
	EF_FRAME(    0,    0,    3,           0, 32768 ),
	EF_FRAME(    0,    0,    3,           0, 32769 ),
	EF_FRAME(    0,    0,    3,           0, 32770 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA, 32771 ),
	CG_NEXT( pl11_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl11_grp27_06[] = {
	EF_FRAME(    0,    0,    3,           0,   579 ),
	EF_FRAME(    0,    0,    3,           0,   580 ),
	EF_FRAME(    0,    0,    3,           0,   581 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   582 ),
	CG_NEXT( pl11_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl11_grp27_07[] = {
	EF_FRAME(    0,    0,    3,           0,   734 ),
	EF_FRAME(    0,    0,    3,           0,   735 ),
	EF_FRAME(    0,    0,    3,           0,   736 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   737 ),
	CG_NEXT( pl11_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl11_grp27_08[] = {
	EF_FRAME(    0,    0,    3,           0,   738 ),
	EF_FRAME(    0,    0,    3,           0,   739 ),
	EF_FRAME(    0,    0,    3,           0,   740 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   741 ),
	CG_NEXT( pl11_grp27_08 ),
};

/* group 27 9: pls11_00_trail_init, pls11_00_rocket_trail_init */
const Sint32 pl11_grp27_09[] = {
	EF_FRAME(    0,    0,    3,           0, 32772 ),
	EF_FRAME(    0,    0,    3,           0, 32773 ),
	EF_FRAME(    0,    0,    3,           0, 32774 ),
	EF_FRAME(    0,    0,    3,           0, 32775 ),
	EF_FRAME(    0,    0,    3,           0, 32776 ),
	EF_FRAME(    0,    0,    3,           0, 32777 ),
	EF_FRAME(    0,    0,    3,           0, 32778 ),
	EF_FRAME(    0,    0,    3,           0, 32779 ),
	EF_FRAME(    0,    0,    3,           0, 32780 ),
	EF_FRAME(    0,    0,    3,           0, 32781 ),
	EF_FRAME(    0,    0,    3,           0, 32782 ),
	EF_FRAME(    0,    0,    3,           0, 32783 ),
	EF_FRAME(    0,    0,    3,           0, 32784 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA, 32785 ),
	CG_NEXT( pl11_grp27_09 ),
};

/* group 27 10: pls11_00_hstars_init (10 + sub_type) */
const Sint32 pl11_grp27_10[] = {
	EF_FRAME(    0,    0,    1,           0,   583 ),
};

const Sint32 pl11_grp27_10_loop[] = {
	EF_FRAME(    0,    0,    1,           0,   584 ),
	EF_FRAME(    0,    0,    1,           0,   585 ),
	EF_FRAME(    0,    0,    1,           0,   586 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   587 ),
	CG_NEXT( pl11_grp27_10_loop ),
};

/* group 27 11 */
const Sint32 pl11_grp27_11[] = {
	EF_FRAME(    0,    0,    1,           0,   742 ),
};

const Sint32 pl11_grp27_11_loop[] = {
	EF_FRAME(    0,    0,    1,           0,   743 ),
	EF_FRAME(    0,    0,    1,           0,   744 ),
	EF_FRAME(    0,    0,    1,           0,   745 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   746 ),
	CG_NEXT( pl11_grp27_11_loop ),
};

/* group 27 12: pls11_00_hstars_move (12 + sub_type) */
const Sint32 pl11_grp27_12[] = {
	EF_FRAME(    0,    0,    1,           0,   588 ),
	EF_FRAME(    0,    0,    1,           0,   589 ),
	EF_FRAME(    0,    0,    1,           0,   590 ),
	EF_FRAME(    0,    0,    1,           0,   591 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   592 ),
	CG_NEXT( pl11_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl11_grp27_13[] = {
	EF_FRAME(    0,    0,    1,           0,   747 ),
	EF_FRAME(    0,    0,    1,           0,   748 ),
	EF_FRAME(    0,    0,    1,           0,   749 ),
	EF_FRAME(    0,    0,    1,           0,   750 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   751 ),
	CG_NEXT( pl11_grp27_13 ),
};

/* group 27 14: pls11_00_rocket_flash_init */
const Sint32 pl11_grp27_14[] = {
	EF_FRAME(    0,    0,    3,           0,   654 ),
	EF_FRAME(    0,    0,    3,           0,   655 ),
	EF_FRAME(    0,    0,    3,           0,   656 ),
	EF_FRAME(    0,    0,    3,           0,   657 ),
	EF_FRAME(    0,    0,    3,           0,   658 ),
	EF_FRAME(    0,    0,    3,           0,   659 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   660 ),
	CG_NEXT( pl11_grp27_14 ),
};

/* group 27 15: pls11_00_backblast_init */
const Sint32 pl11_grp27_15[] = {
	EF_FRAME(    0,    0,    3,           0,   647 ),
	EF_FRAME(    0,    0,    3,           0,   648 ),
	EF_FRAME(    0,    0,    3,           0,   649 ),
	EF_FRAME(    0,    0,    3,           0,   650 ),
	EF_FRAME(    0,    0,    3,           0,   651 ),
	EF_FRAME(    0,    0,    3,           0,   652 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   653 ),
	CG_NEXT( pl11_grp27_15 ),
};

/* group 27 16: pls11_00_heal_init (16 + sub_type) */
const Sint32 pl11_grp27_16[] = {
	EF_FRAME(    0,    0,    3,           0,   643 ),
	EF_FRAME(    0,    0,    3,           0,   644 ),
	EF_FRAME(    0,    0,    2,           0,   645 ),
	EF_FRAME(    0,    0,   16,           0,   608 ),
	EF_FRAME(    0,    0,    2,           0,   610 ),
	EF_FRAME(    0,    0,    2,           0,   611 ),
	EF_FRAME(    0,    0,    2,           0,   612 ),
	EF_FRAME(    0,    0,    2,           0,   613 ),
	EF_FRAME(    0,    0,    2,           0,   614 ),
	EF_FRAME(    0,    0,    2,           0,   615 ),
	EF_FRAME(    0,    0,    2,           0,   616 ),
	EF_FRAME(    0,    0,    2,           0,   617 ),
	EF_FRAME(    0,    0,    2,           0,   618 ),
	EF_FRAME(    0,    0,    1,           0,   619 ),
	EF_FRAME(    0,    0,    1,           0,   620 ),
	EF_FRAME(    0,    0,    1,           0,   621 ),
	EF_FRAME(    0,    0,    1,           0,   622 ),
	EF_FRAME(    0,    0,    1,           0,   623 ),
	EF_FRAME(    0,    0,    1,           0,   624 ),
	EF_FRAME(    0,    0,    1,           0,   625 ),
	EF_FRAME(    0,    0,    1,           0,   626 ),
	EF_FRAME(    0,    0,    1,           0,   627 ),
	EF_FRAME(    0,    0,    1,           0,   628 ),
	EF_FRAME(    0,    0,    1,           0,   629 ),
	EF_FRAME(    0,    0,    1,           0,   630 ),
	EF_FRAME(    0,    0,    1,           0,   631 ),
	EF_FRAME(    0,    0,    1,           0,   632 ),
	EF_FRAME(    0,    0,    1,           0,   633 ),
	EF_FRAME(    0,    0,    1,           0,   634 ),
	EF_FRAME(    0,    0,    1,           0,   635 ),
	EF_FRAME(    0,    0,    1,           0,   636 ),
	EF_FRAME(    0,    0,    1,           0,   637 ),
	EF_FRAME(    0,    0,    1,           0,   638 ),
	EF_FRAME(    0,    0,    1,           0,   639 ),
	EF_FRAME(    0,    0,    1,           0,   640 ),
	EF_FRAME(    0,    0,    1,           0,   641 ),
	EF_FRAME(    0,    0,    1,           0,   642 ),
	EF_FRAME(    0,    0,    2,           0,   645 ),
	EF_FRAME(    0,    0,    3,           0,   644 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   643 ),
	CG_NEXT( pl11_grp27_16 ),
};

/* group 27 17 */
const Sint32 pl11_grp27_17[] = {
	EF_FRAME(    0,    0,    3,           0,   643 ),
	EF_FRAME(    0,    0,    3,           0,   644 ),
	EF_FRAME(    0,    0,    2,           0,   645 ),
	EF_FRAME(    0,    0,   16,           0,   609 ),
	EF_FRAME(    0,    0,    2,           0,   610 ),
	EF_FRAME(    0,    0,    2,           0,   611 ),
	EF_FRAME(    0,    0,    2,           0,   612 ),
	EF_FRAME(    0,    0,    2,           0,   613 ),
	EF_FRAME(    0,    0,    2,           0,   614 ),
	EF_FRAME(    0,    0,    2,           0,   615 ),
	EF_FRAME(    0,    0,    2,           0,   616 ),
	EF_FRAME(    0,    0,    2,           0,   617 ),
	EF_FRAME(    0,    0,    2,           0,   618 ),
	EF_FRAME(    0,    0,    1,           0,   619 ),
	EF_FRAME(    0,    0,    1,           0,   620 ),
	EF_FRAME(    0,    0,    1,           0,   621 ),
	EF_FRAME(    0,    0,    1,           0,   622 ),
	EF_FRAME(    0,    0,    1,           0,   623 ),
	EF_FRAME(    0,    0,    1,           0,   624 ),
	EF_FRAME(    0,    0,    1,           0,   625 ),
	EF_FRAME(    0,    0,    1,           0,   626 ),
	EF_FRAME(    0,    0,    1,           0,   627 ),
	EF_FRAME(    0,    0,    1,           0,   628 ),
	EF_FRAME(    0,    0,    1,           0,   629 ),
	EF_FRAME(    0,    0,    1,           0,   630 ),
	EF_FRAME(    0,    0,    1,           0,   631 ),
	EF_FRAME(    0,    0,    1,           0,   632 ),
	EF_FRAME(    0,    0,    1,           0,   633 ),
	EF_FRAME(    0,    0,    1,           0,   634 ),
	EF_FRAME(    0,    0,    1,           0,   635 ),
	EF_FRAME(    0,    0,    1,           0,   636 ),
	EF_FRAME(    0,    0,    1,           0,   637 ),
	EF_FRAME(    0,    0,    1,           0,   638 ),
	EF_FRAME(    0,    0,    1,           0,   639 ),
	EF_FRAME(    0,    0,    1,           0,   640 ),
	EF_FRAME(    0,    0,    1,           0,   641 ),
	EF_FRAME(    0,    0,    1,           0,   642 ),
	EF_FRAME(    0,    0,    2,           0,   645 ),
	EF_FRAME(    0,    0,    3,           0,   644 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   643 ),
	CG_NEXT( pl11_grp27_17 ),
};

/* group 27 18: pls11_00_win_shot_init */
const Sint32 pl11_grp27_18[] = {
	EF_FRAME(    0,    0,    3,           0,   605 ),
	EF_FRAME(    0,    0,    3,           0,   606 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   607 ),
	CG_NEXT( pl11_grp27_18 ),
};

/* group 27 19: pls11_00_win_init */
const Sint32 pl11_grp27_19[] = {
	EF_FRAME(    0,    0,    3, END_OF_DATA,   236 ),
	CG_NEXT( pl11_grp27_19 ),
};

/* group 27 20: pls11_00_tyrant_atck_init (20 + sub_type) */
const Sint32 pl11_grp27_20[] = {
	EF_FRAME(    0,    0,    3,           0,   718 ),
	EF_FRAME(    0,    0,    3,           0,   719 ),
	EF_FRAME(    0,    0,    3,           0,   720 ),
	EF_FRAME(    0,    0,    3,           0,   721 ),
	EF_FRAME(    0,    0,    3,           0,   722 ),
	EF_FRAME(    0,    0,    3,           0,   723 ),
	EF_FRAME(    0,    0,    3,           0,   724 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   725 ),
	CG_NEXT( pl11_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl11_grp27_21[] = {
	EF_FRAME(    0,    0,    3,           0,   726 ),
	EF_FRAME(    0,    0,    3,           0,   727 ),
	EF_FRAME(    0,    0,    3,           0,   728 ),
	EF_FRAME(    0,    0,    3,           0,   729 ),
	EF_FRAME(    0,    0,    3,           0,   730 ),
	EF_FRAME(    0,    0,    3,           0,   731 ),
	EF_FRAME(    0,    0,    3,           0,   732 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   733 ),
	CG_NEXT( pl11_grp27_21 ),
};

/* group 27 22: pls11_00_charge_init */
const Sint32 pl11_grp27_22[] = {
	EF_FRAME(    0,    0,    4,           0,   760 ),
	EF_FRAME(    0,    0,    4,           0,   761 ),
	EF_FRAME(    0,    0,    4,           0,   762 ),
	EF_FRAME(    0,    0,    4,           0,   763 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   764 ),
	CG_NEXT( pl11_grp27_22 ),
};

/* group 27 23: pls11_00_rocket_jump_init */
const Sint32 pl11_grp27_23[] = {
	EF_FRAME(    0,    0,    4,           0,   755 ),
	EF_FRAME(    0,    0,    4,           0,   756 ),
	EF_FRAME(    0,    0,    4,           0,   757 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   758 ),
	CG_NEXT( pl11_grp27_23 ),
};

/* group 27 24: pls11_00_zombie_fire_init, shl11_01_d_init */
const Sint32 pl11_grp27_24[] = {
	EF_FRAME(    0,    0,    4,           0, 32799 ),
	EF_FRAME(    0,    0,    4,           0, 32800 ),
	EF_FRAME(    0,    0,    4,           0, 32801 ),
	EF_FRAME(    0,    0,    4,           0, 32802 ),
	EF_FRAME(    0,    0,    4,           0, 32803 ),
	EF_FRAME(    0,    0,    4,           0, 32804 ),
	EF_FRAME(    0,    0,    4,           0, 32805 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32806 ),
	CG_NEXT( pl11_grp27_24 ),
};

/* group 27 25: pls11_00_feather_init */
const Sint32 pl11_grp27_25[] = {
	EF_FRAME(    0,    0,    6,           0,   765 ),
	EF_FRAME(    0,    0,   10,           0,   766 ),
	EF_FRAME(    0,    0,   12,           0,   767 ),
	EF_FRAME(    0,    0,    6,           0,   766 ),
	EF_FRAME(    0,    0,    6,           0,   765 ),
	EF_FRAME(    0,    0,    6,           0,   768 ),
	EF_FRAME(    0,    0,   10,           0,   769 ),
	EF_FRAME(    0,    0,   12,           0,   770 ),
	EF_FRAME(    0,    0,    6,           0,   769 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,   768 ),
	CG_NEXT( pl11_grp27_25 ),
};

/* group 27 26: pls11_00_t18_init */
const Sint32 pl11_grp27_26[] = {
	EF_FRAME(    0,    0,   12,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   771 ),
	EF_FRAME(    0,    0,    4,           0,   772 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    2,           0,   775 ),
	EF_FRAME(    0,    0,    2,           0,   776 ),
	EF_FRAME(    0,    0,    2,           0,   777 ),
	EF_FRAME(    0,    0,    2,           0,   778 ),
	EF_FRAME(    0,    0,    2,           0,   779 ),
	EF_FRAME(    0,    0,    2,           0,   780 ),
	EF_FRAME(    0,    0,    2,           0, 32809 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32810 ),
	CG_NEXT( pl11_grp27_26 ),
};

/* group 27 27: pls11_00_t18_init */
const Sint32 pl11_grp27_27[] = {
	EF_FRAME(    0,    0,   20,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    2,           0, 32813 ),
	EF_FRAME(    0,    0,    2,           0, 32814 ),
	EF_FRAME(    0,    0,    2,           0, 32815 ),
	EF_FRAME(    0,    0,    2,           0, 32816 ),
	EF_FRAME(    0,    0,    2,           0, 32817 ),
	EF_FRAME(    0,    0,    2,           0, 32818 ),
	EF_FRAME(    0,    0,    2,           0, 32819 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32820 ),
	CG_NEXT( pl11_grp27_27 ),
};

/* group 27 28: pls11_00_t18_move */
const Sint32 pl11_grp27_28[] = {
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    2,           0,   775 ),
	EF_FRAME(    0,    0,    2,           0,   776 ),
	EF_FRAME(    0,    0,    2,           0,   777 ),
	EF_FRAME(    0,    0,    2,           0,   778 ),
	EF_FRAME(    0,    0,    2,           0,   779 ),
	EF_FRAME(    0,    0,    2,           0,   780 ),
	EF_FRAME(    0,    0,    2,           0, 32809 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32810 ),
	CG_NEXT( pl11_grp27_28 ),
};

/* group 27 29: pls11_00_t18_move */
const Sint32 pl11_grp27_29[] = {
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    2,           0, 32813 ),
	EF_FRAME(    0,    0,    2,           0, 32814 ),
	EF_FRAME(    0,    0,    2,           0, 32815 ),
	EF_FRAME(    0,    0,    2,           0, 32816 ),
	EF_FRAME(    0,    0,    2,           0, 32817 ),
	EF_FRAME(    0,    0,    2,           0, 32818 ),
	EF_FRAME(    0,    0,    2,           0, 32819 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32820 ),
	CG_NEXT( pl11_grp27_29 ),
};

/* group 27 30: pls11_00_rocket_fall_init */
const Sint32 pl11_grp27_30[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   441 ),
	CG_NEXT( pl11_grp27_30 ),
};
