/* Marrow (character 16): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_16      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_16     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl16.c) says which group
                   and number each move uses.
   pl16_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl16_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl16_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl16_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_16
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_16, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_16 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl16_change_01[], pl16_stand_00[], pl16_stand_02[], pl16_stand_03[],
	pl16_jump_00[], pl16_jump_00_loop[], pl16_jump_01[], pl16_jump_01_loop[], pl16_jump_02[],
	pl16_jump_02_loop[], pl16_jump_03[], pl16_jump_06[], pl16_jump_09[], pl16_jump_07[],
	pl16_jump_10[], pl16_jump_08[], pl16_jump_11[], pl16_jump_12[], pl16_jump_12_loop[],
	pl16_dash_00[], pl16_dash_00_loop[], pl16_dash_01[], pl16_dash_01_loop[], pl16_dash_02[],
	pl16_dash_03[], pl16_crouch_00[], pl16_crouch_01[], pl16_crouch_02[], pl16_turn_00[],
	pl16_turn_01[], pl16_guard_00[], pl16_guard_01[], pl16_guard_02[], pl16_guard_04[],
	pl16_guard_05[], pl16_guard_06[], pl16_air_guard_00[], pl16_air_guard_01[], pl16_stand_p_00[],
	pl16_stand_p_01[], pl16_stand_p_02[], pl16_stand_p_03[], pl16_stand_p_04[], pl16_stand_k_00[],
	pl16_stand_k_01[], pl16_stand_k_02[], pl16_stand_k_03[], pl16_crouch_p_00[], pl16_crouch_p_01[],
	pl16_crouch_p_02[], pl16_crouch_k_00[], pl16_crouch_k_01[], pl16_crouch_k_02[],
	pl16_crouch_k_03[], pl16_jump_p_00[], pl16_jump_p_01[], pl16_jump_p_02[], pl16_jump_k_00[],
	pl16_jump_k_01[], pl16_jump_k_02[], pl16_extra_02[], pl16_extra_03[], pl16_extra_04[],
	pl16_special_00[], pl16_special_00_loop[], pl16_special_01[], pl16_special_01_loop[],
	pl16_special_02[], pl16_special_02_loop[], pl16_special_03[], pl16_special_03_loop[],
	pl16_special_04[], pl16_special_04_loop[], pl16_special_05[], pl16_special_05_loop[],
	pl16_special_06[], pl16_special_06_loop[], pl16_special_07[], pl16_special_07_loop[],
	pl16_special_08[], pl16_special_08_loop[], pl16_super_00[], pl16_super_00_loop[],
	pl16_super_01[], pl16_super_01_loop[], pl16_super_02[], pl16_effect_00[], pl16_effect_01[],
	pl16_effect_02[], pl16_effect_03[], pl16_effect_04[], pl16_get_up_00[], pl16_get_up_01[],
	pl16_get_up_02[], pl16_get_up_03[], pl16_damage_00[], pl16_damage_01[], pl16_damage_02[],
	pl16_damage_03[], pl16_damage_04[], pl16_damage_05[], pl16_damage_06[], pl16_damage_07[],
	pl16_damage_08[], pl16_damage_10[], pl16_damage_11[], pl16_damage_12[], pl16_damage_13[],
	pl16_damage_09[], pl16_damage_09_loop[], pl16_special_09[], pl16_special_10[], pl16_special_11[],
	pl16_special_12[], pl16_special_12_loop[], pl16_special_14[], pl16_special_14_loop[],
	pl16_unused_00[], pl16_special_16[], pl16_special_16_loop[], pl16_special_18[], pl16_extra_00[],
	pl16_extra_01[], pl16_damage_16[], pl16_damage_17[], pl16_damage_18[], pl16_damage_19[],
	pl16_damage_20[], pl16_damage_21[], pl16_damage_22[], pl16_damage_23[], pl16_damage_24[],
	pl16_damage_25[], pl16_damage_26[], pl16_damage_27[], pl16_damage_31[], pl16_damage_28[],
	pl16_damage_29[], pl16_caught_00[], pl16_caught_01[], pl16_caught_02[], pl16_caught_03[],
	pl16_caught_04[], pl16_caught_05[], pl16_caught_06[], pl16_caught_07[], pl16_caught_08[],
	pl16_caught_09[], pl16_caught_10[], pl16_caught_11[], pl16_caught_12[], pl16_caught_13[],
	pl16_caught_14[], pl16_caught_15[], pl16_caught_16[], pl16_caught_17[], pl16_caught_18[],
	pl16_caught_19[], pl16_caught_20[], pl16_caught_21[], pl16_dizzy_00[], pl16_catch_00[],
	pl16_catch_01[], pl16_special_20[], pl16_special_21_loop[], pl16_special_21_loop_loop[],
	pl16_special_21[], pl16_special_22[], pl16_catch_02[], pl16_catch_02_loop[], pl16_win_00[],
	pl16_win_00_loop[], pl16_unused_01[], pl16_win_01[], pl16_win_01_loop[], pl16_win_02[],
	pl16_win_02_loop[], pl16_appear_00[], pl16_appear_00_loop[], pl16_appear_01[],
	pl16_appear_01_loop[], pl16_grp27_04[], pl16_grp27_04_loop[], pl16_grp27_05[], pl16_grp27_06[],
	pl16_grp27_06_loop[], pl16_unused_02[], pl16_unused_03[], pl16_unused_04[], pl16_change_00[],
	pl16_change_02[], pl16_change_03[], pl16_change_03_loop[], pl16_change_04[], pl16_grp27_00[],
	pl16_grp27_01[], pl16_grp27_01_loop[], pl16_grp27_02[], pl16_grp27_02_loop[], pl16_grp27_03[],
	pl16_grp27_03_loop[], pl16_grp27_07[], pl16_grp27_08[], pl16_grp27_09[], pl16_grp27_10[],
	pl16_grp27_11[], pl16_grp27_12[], pl16_grp27_12_loop[], pl16_grp27_13[], pl16_grp27_14[],
	pl16_grp27_15[], pl16_grp27_16[], pl16_grp27_17[], pl16_grp27_18[], pl16_grp27_19[];
extern const Sint32 *const pl16_stand_tbl[], *const pl16_jump_tbl[], *const pl16_dash_tbl[],
	*const pl16_crouch_tbl[], *const pl16_turn_tbl[], *const pl16_guard_tbl[],
	*const pl16_air_guard_tbl[], *const pl16_stand_p_tbl[], *const pl16_stand_k_tbl[],
	*const pl16_crouch_p_tbl[], *const pl16_crouch_k_tbl[], *const pl16_jump_p_tbl[],
	*const pl16_jump_k_tbl[], *const pl16_damage_tbl[], *const pl16_dizzy_tbl[],
	*const pl16_catch_tbl[], *const pl16_caught_tbl[], *const pl16_get_up_tbl[],
	*const pl16_appear_tbl[], *const pl16_win_tbl[], *const pl16_extra_tbl[],
	*const pl16_special_tbl[], *const pl16_super_tbl[], *const pl16_effect_tbl[],
	*const pl16_change_tbl[], *const pl16_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_16[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, -64, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_16[] = {
	pl16_stand_tbl, pl16_jump_tbl, pl16_dash_tbl, pl16_crouch_tbl, pl16_turn_tbl, pl16_guard_tbl,
	pl16_air_guard_tbl, pl16_stand_p_tbl, pl16_stand_k_tbl, pl16_crouch_p_tbl, pl16_crouch_k_tbl, pl16_jump_p_tbl,
	pl16_jump_k_tbl, pl16_damage_tbl, pl16_dizzy_tbl, pl16_catch_tbl, pl16_caught_tbl, pl16_get_up_tbl,
	pl16_appear_tbl, pl16_win_tbl, pl16_extra_tbl, pl16_special_tbl, pl16_super_tbl, pl16_effect_tbl,
	pl16_change_tbl, pl16_stand_tbl, pl16_stand_tbl, pl16_grp27_tbl, 0,
};

const Sint32 *const pl16_stand_tbl[] = {
	pl16_stand_00, pl16_stand_00, pl16_stand_02, pl16_stand_03, pl16_stand_00, pl16_stand_00,
	pl16_stand_02, pl16_stand_03, 0,
};

const Sint32 *const pl16_jump_tbl[] = {
	pl16_jump_00, pl16_jump_01, pl16_jump_02, pl16_jump_03, pl16_jump_03, pl16_jump_03,
	pl16_jump_06, pl16_jump_07, pl16_jump_08, pl16_jump_09, pl16_jump_10, pl16_jump_11,
	pl16_jump_12, 0,
};

const Sint32 *const pl16_dash_tbl[] = {
	pl16_dash_00, pl16_dash_01, pl16_dash_02, pl16_dash_03, 0,
};

const Sint32 *const pl16_crouch_tbl[] = {
	pl16_crouch_00, pl16_crouch_01, pl16_crouch_02, 0,
};

const Sint32 *const pl16_turn_tbl[] = {
	pl16_turn_00, pl16_turn_01, 0,
};

const Sint32 *const pl16_guard_tbl[] = {
	pl16_guard_00, pl16_guard_01, pl16_guard_02, pl16_guard_01, pl16_guard_04, pl16_guard_05,
	pl16_guard_06, pl16_guard_05, 0,
};

const Sint32 *const pl16_air_guard_tbl[] = {
	pl16_air_guard_00, pl16_air_guard_01, pl16_air_guard_01, 0,
};

const Sint32 *const pl16_stand_p_tbl[] = {
	pl16_stand_p_00, pl16_stand_p_01, pl16_stand_p_02, pl16_stand_p_03, pl16_stand_p_04, 0,
};

const Sint32 *const pl16_stand_k_tbl[] = {
	pl16_stand_k_00, pl16_stand_k_01, pl16_stand_k_02, pl16_stand_k_03, 0,
};

const Sint32 *const pl16_crouch_p_tbl[] = {
	pl16_crouch_p_00, pl16_crouch_p_01, pl16_crouch_p_02, 0,
};

const Sint32 *const pl16_crouch_k_tbl[] = {
	pl16_crouch_k_00, pl16_crouch_k_01, pl16_crouch_k_02, pl16_crouch_k_03, 0,
};

const Sint32 *const pl16_jump_p_tbl[] = {
	pl16_jump_p_00, pl16_jump_p_01, pl16_jump_p_02, pl16_jump_p_00, pl16_jump_p_01, pl16_jump_p_02,
	0,
};

const Sint32 *const pl16_jump_k_tbl[] = {
	pl16_jump_k_00, pl16_jump_k_01, pl16_jump_k_02, pl16_jump_k_00, pl16_jump_k_01, pl16_jump_k_02,
	0,
};

const Sint32 *const pl16_damage_tbl[] = {
	pl16_damage_00, pl16_damage_01, pl16_damage_02, pl16_damage_03, pl16_damage_04, pl16_damage_05,
	pl16_damage_06, pl16_damage_07, pl16_damage_08, pl16_damage_09, pl16_damage_10, pl16_damage_11,
	pl16_damage_12, pl16_damage_13, pl16_damage_13, pl16_damage_13, pl16_damage_16, pl16_damage_17,
	pl16_damage_18, pl16_damage_19, pl16_damage_20, pl16_damage_21, pl16_damage_22, pl16_damage_23,
	pl16_damage_24, pl16_damage_25, pl16_damage_26, pl16_damage_27, pl16_damage_28, pl16_damage_29,
	pl16_damage_27, pl16_damage_31, pl16_damage_27, pl16_damage_27, pl16_damage_13, 0,
};

const Sint32 *const pl16_dizzy_tbl[] = {
	pl16_dizzy_00, 0,
};

const Sint32 *const pl16_catch_tbl[] = {
	pl16_catch_00, pl16_catch_01, pl16_catch_02, 0,
};

const Sint32 *const pl16_caught_tbl[] = {
	pl16_caught_00, pl16_caught_01, pl16_caught_02, pl16_caught_03, pl16_caught_04, pl16_caught_05,
	pl16_caught_06, pl16_caught_07, pl16_caught_08, pl16_caught_09, pl16_caught_10, pl16_caught_11,
	pl16_caught_12, pl16_caught_13, pl16_caught_14, pl16_caught_15, pl16_caught_16, pl16_caught_17,
	pl16_caught_18, pl16_caught_19, pl16_caught_20, pl16_caught_21, 0,
};

const Sint32 *const pl16_get_up_tbl[] = {
	pl16_get_up_00, pl16_get_up_01, pl16_get_up_02, pl16_get_up_03, 0,
};

const Sint32 *const pl16_appear_tbl[] = {
	pl16_appear_00, pl16_appear_01, 0,
};

const Sint32 *const pl16_win_tbl[] = {
	pl16_win_00, pl16_win_01, pl16_win_02, 0,
};

const Sint32 *const pl16_extra_tbl[] = {
	pl16_extra_00, pl16_extra_01, pl16_extra_02, pl16_extra_03, pl16_extra_04, 0,
};

const Sint32 *const pl16_special_tbl[] = {
	pl16_special_00, pl16_special_01, pl16_special_02, pl16_special_03, pl16_special_04, pl16_special_05,
	pl16_special_06, pl16_special_07, pl16_special_08, pl16_special_09, pl16_special_10, pl16_special_11,
	pl16_special_12, pl16_special_12, pl16_special_14, pl16_special_14, pl16_special_16, pl16_special_16,
	pl16_special_18, pl16_special_18, pl16_special_20, pl16_special_21, pl16_special_22, 0,
};

const Sint32 *const pl16_super_tbl[] = {
	pl16_super_00, pl16_super_01, pl16_super_02, 0,
};

const Sint32 *const pl16_effect_tbl[] = {
	pl16_effect_00, pl16_effect_01, pl16_effect_02, pl16_effect_03, pl16_effect_04, 0,
};

const Sint32 *const pl16_change_tbl[] = {
	pl16_change_00, pl16_change_01, pl16_change_02, pl16_change_03, pl16_change_04, 0,
};

const Sint32 *const pl16_grp27_tbl[] = {
	pl16_grp27_00, pl16_grp27_01, pl16_grp27_02, pl16_grp27_03, pl16_grp27_04, pl16_grp27_05,
	pl16_grp27_06, pl16_grp27_07, pl16_grp27_08, pl16_grp27_09, pl16_grp27_10, pl16_grp27_11,
	pl16_grp27_12, pl16_grp27_13, pl16_grp27_14, pl16_grp27_15, pl16_grp27_16, pl16_grp27_17,
	pl16_grp27_18, pl16_grp27_19, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl16_change_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_change_01 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   group 25 1 */
const Sint32 pl16_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    27, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    30, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    31, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    32, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    33, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    35, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    35, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    33, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    32, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    31, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    30, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   group 25 2
   group 25 6
   Char_Fly 2
   Char_Fly 6 */
const Sint32 pl16_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   group 25 3
   group 25 7
   Char_Fly 3
   Char_Fly 7 */
const Sint32 pl16_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl16_jump_00[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl16_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl16_jump_01[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl16_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl16_jump_02[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl16_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_jump_02_loop ),
};

/* Char_Jump 3: ride_fly (Ride and Slash), ride_bound (Ride and Slash), burst_fall (Bone Burst (hyper)), ride_catch_fall
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl16_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl16_jump_06[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: burst_wait (Bone Burst (hyper)), act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl16_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl16_jump_07[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl16_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl16_jump_08[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl16_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_jump_11 ),
};

/* Char_Jump 12: act_wall_jump */
const Sint32 pl16_jump_12[] = {
	CG_FRAME(    0,    0,   1,           0,   593, 0x80,   0x00,  ATK,  0x00, 0x23 | CSE,    1,   0,     0,      0,  69 ),
};

const Sint32 pl16_jump_12_loop[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   593, 0x80,   0x00,  ATK,  0x00,          0,    1,   0,     0,      0,  69 ),
	CG_NEXT( pl16_jump_12_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl16_dash_00[] = {
	CG_FRAME(    0,    1,   1,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl16_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl16_dash_01[] = {
	CG_FRAME(    0,    1,   1,           0,    85, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,    86, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl16_dash_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    88, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_dash_01_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl16_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    82, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    83, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    84, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_dash_02 ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl16_dash_03[] = {
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl16_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl16_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl16_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl16_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl16_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl16_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl16_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl16_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl16_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl16_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl16_guard_06[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl16_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl16_air_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl16_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   289, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   4,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   292, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl16_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   296, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   297, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   6,           0,   298, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   300, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl16_stand_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,   5,           0,   303, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x0300,   5 ),
	CG_FRAME(  -13,    0,   1,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1600,  16 ),
	CG_FRAME(    0,    1,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,       0x23,    0,   0,     0, 0x0600,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0800,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0900,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0a00,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0b00,  88 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0c00,  88 ),
	CG_NEXT( pl16_stand_p_02 ),
};

/* Char_Stand_P 3 */
const Sint32 pl16_stand_p_03[] = {
	CG_FRAME(    0,    1,   6,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0e00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0, 0x0e00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0f00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1000,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1100,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1200,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1300,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1400,  88 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1500,  88 ),
	CG_NEXT( pl16_stand_p_03 ),
};

/* Char_Stand_P 4: stand_hp_start */
const Sint32 pl16_stand_p_04[] = {
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_p_04 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl16_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   336, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   337, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   338, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   340, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl16_stand_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   341, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   342, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   343, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    0,   2,           0,   344, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   9,           0,   345, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   346, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   347, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl16_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   348, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   349, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   351, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   352, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   3,           0,   353, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(  -43,    1,   3,           0,   354, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,   3,           0,   355, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,  12,           0,   356, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   357, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_k_02 ),
};

/* Char_Stand_K 3 */
const Sint32 pl16_stand_k_03[] = {
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   355, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,  12,           0,   356, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   357, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_stand_k_03 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl16_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   360, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   361, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6,           0,   362, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   363, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   364, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl16_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   367, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   6,           0,   368, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   5,           0,   369, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   365, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl16_crouch_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   371, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   2,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,   2,           0,   374, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,  18,           0,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   3,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   379, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl16_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   380, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   381, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4,           0,   382, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   383, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   384, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl16_crouch_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   7,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   390, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   391, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl16_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   393, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(  -28,    0,   3,           0,   394, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   395, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   398, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   399, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_k_02 ),
};

/* Char_Crouch_K 3 */
const Sint32 pl16_crouch_k_03[] = {
	CG_FRAME(    0,    0,   1,           0,   401, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   3,           0,   395, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   398, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   399, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   400, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_crouch_k_03 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl16_jump_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   405, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   7,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl16_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   3,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl16_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   418, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    1,   2,           0,   419, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   8,           0,   420, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   408, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl16_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   424, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   6,           0,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   6,           0,   425, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   426, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl16_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,  10,           0,   430, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   431, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl16_jump_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   435, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   436, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   437, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   439, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   440, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   441, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,  10,           0,   442, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   438, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_k_02 ),
};

/* Char_Extra 2: pl16_upper, fly_start5 */
const Sint32 pl16_extra_02[] = {
	CG_FRAME(    0,    0,   6,           0,   371, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   2,           0,   373, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   2,           0,   374, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,  18,           0,   375, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   3,           0,   378, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   379, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_extra_02 ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl16_extra_03[] = {
	CG_FRAME(    0,    0,   3,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME( 0x80,    0,   2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   7,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   390, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   391, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl16_extra_03 ),
};

/* Char_Extra 4: sp_chouhatsu (the taunt) */
const Sint32 pl16_extra_04[] = {
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,    0,  0x00,       0x0e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_extra_04 ),
};

/* Char_Special 0: bone_init (Bone-merang) */
const Sint32 pl16_special_00[] = {
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   511, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   513, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   514, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   515, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,  12,           0,   517, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   6,           0,   518, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   519, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl16_special_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_special_00_loop ),
};

/* Char_Special 1: bone_init (Bone-merang) */
const Sint32 pl16_special_01[] = {
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   523, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   525, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   526, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,  12,           0,   529, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   6,           0,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl16_special_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_special_01_loop ),
};

/* Char_Special 2: bone_init (Bone-merang) */
const Sint32 pl16_special_02[] = {
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   534, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   535, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   537, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   6,           0,   539, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   6,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl16_special_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_special_02_loop ),
};

/* Char_Special 3: bone_init (Bone-merang) */
const Sint32 pl16_special_03[] = {
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   534, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   535, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   536, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   537, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   3,           0,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   549, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl16_special_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_special_03_loop ),
};

/* Char_Special 4: spine_init (Towering Spine) */
const Sint32 pl16_special_04[] = {
	CG_FRAME(    0,    0,   2,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   553, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   554, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   6,           0,   555, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   1,           0,   556, 0x80,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,   4,           0,   557, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0, 0x0100,  63 ),
	CG_FRAME(    0,    2,   4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  64 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  65 ),
	CG_FRAME(    0,    3,   4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  65 ),
	CG_FRAME(    0,    4,   4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  65 ),
	CG_FRAME(    0,    5,   4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  65 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,  66 ),
	CG_FRAME(    0,    0,   5,           0,   558, 0x80,   0x00,  ATE,  0x00,       0x23,    0,   0,     0, 0x0400,  66 ),
	CG_FRAME(    0,    0,   5,           0,   558, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0500,  67 ),
	CG_FRAME(    0,    0,   4,           0,   562, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0600,  67 ),
	CG_FRAME(    0,    0,   3,           0,   562, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  67 ),
	CG_FRAME(    0,    0,   3,           0,   562, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0800,  68 ),
	CG_FRAME(    0,    0,   3,           0,   559, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0900,   6 ),
};

const Sint32 pl16_special_04_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   560, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0a00,   6 ),
	CG_NEXT( pl16_special_04_loop ),
};

/* Char_Special 5: shl16_02_move_0 */
const Sint32 pl16_special_05[] = {
	CG_FRAME(    0,    0,   1,           0,   577, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   1,           0,   578, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,   1,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0900,   0 ),
};

const Sint32 pl16_special_05_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_NEXT( pl16_special_05_loop ),
};

/* Char_Special 6: shl16_02_move_1 */
const Sint32 pl16_special_06[] = {
	CG_FRAME(    0,    0,   1,           0,   563, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   1,           0,   564, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0700,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_FRAME(    0,    0,   1,           0,   571, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0900,   0 ),
};

const Sint32 pl16_special_06_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   572, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0a00,   0 ),
	CG_NEXT( pl16_special_06_loop ),
};

/* Char_Special 7: shl16_02_move_2 */
const Sint32 pl16_special_07[] = {
	CG_FRAME(    0,    0,   1,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   1,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,   0 ),
};

const Sint32 pl16_special_07_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_NEXT( pl16_special_07_loop ),
};

/* Char_Special 8: shl16_02_move_3 */
const Sint32 pl16_special_08[] = {
	CG_FRAME(    0,    0,   1,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,   1,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,   0 ),
};

const Sint32 pl16_special_08_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_NEXT( pl16_special_08_loop ),
};

/* Char_Super 0: burst_init (Bone Burst (hyper)), burst_j_init (Bone Burst (hyper)) */
const Sint32 pl16_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   635, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_super_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   636, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_super_00_loop ),
};

/* Char_Super 1: burst_rise (Bone Burst (hyper)), burst_j_rise (Bone Burst (hyper)) */
const Sint32 pl16_super_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   637, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   638, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   639, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   640, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   641, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   642, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   642, 0x80,   0x00,  ATK,  0x00,       0x18,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   2,           0,   643, 0x80,   0x00,  ATK,  0x00,       0x25,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   644, 0x80,   0x00,  ATK,  0x00,       0x26,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   645, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   646, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   647, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   648, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   649, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_super_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   650, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_super_01_loop ),
};

/* Char_Super 2: stinger_init (Stinger Bones (hyper)) */
const Sint32 pl16_super_02[] = {
	CG_FRAME(    0,    0,   4,           0,   671, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   672, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   673, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   674, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    1,   4,           0,   675, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   676, 0x80,   0x00,  ATK,  0x00,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    1,   3,           0,   677, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   678, 0x80,   0x00,  ATK,  0x02,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    2,   2,           0,   678, 0x80,   0x00,  ATK,  0x03,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x04,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x05,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x06,       0x27,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x07,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x08,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x08,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x09,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x09,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    3,   2,           0,   678, 0x80,   0x00,  ATK,  0x09,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    4,   4,           0,   678, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  12,           0,   678, 0x80,   0x00,  ATE,  0x0a,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x0b,       0x23,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x0c,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x0d,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x0e,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x0f,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x10,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x11,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x12,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATE,  0x13,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,   -1,   4,           0,   679, 0x80,   0x00,  ATE,  0xff,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   680, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_super_02 ),
};

/* Char_Effect 0: shl16_00_init */
const Sint32 pl16_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0,   780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   781, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl16_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl16_effect_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32792, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl16_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl16_effect_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32793, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl16_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl16_effect_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32794, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl16_effect_03 ),
};

/* Char_Effect 4: shl16_03_init */
const Sint32 pl16_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   651, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   652, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    2,   4,           0,   653, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    3,   4,           0,   654, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    2,   4,           0,   655, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    3,   4,           0,   656, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    2,   4,           0, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    3,   4,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,  82 ),
	CG_FRAME( 0x80,    4,   4,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0400,  83 ),
	CG_FRAME(    0,    0,   4,           0, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   661, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   662, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   663, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   664, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   665, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   666, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   667, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   668, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   669, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   670, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_effect_04 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl16_get_up_00[] = {
	CG_FRAME(    0,    0,   1,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl16_get_up_01[] = {
	CG_FRAME(    0,    0,   1,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl16_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl16_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl16_damage_00[] = {
	CG_FRAME(    0,    0,   6,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl16_damage_01[] = {
	CG_FRAME(    0,    0,  10,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl16_damage_02[] = {
	CG_FRAME(    0,    0,  10,           0,   706, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl16_damage_03[] = {
	CG_FRAME(    0,    0,   6,           0,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl16_damage_04[] = {
	CG_FRAME(    0,    0,  10,           0,   716, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl16_damage_05[] = {
	CG_FRAME(    0,    0,  10,           0,   715, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   716, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl16_damage_06[] = {
	CG_FRAME(    0,    0,   6,           0,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   727, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl16_damage_07[] = {
	CG_FRAME(    0,    0,  10,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   727, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl16_damage_08[] = {
	CG_FRAME(    0,    0,  10,           0,   724, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   727, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl16_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl16_damage_10[] = {
	CG_FRAME(    0,    0,   5,           0,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   736, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl16_damage_11[] = {
	CG_FRAME(    0,    0,   3,           0,   734, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   736, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl16_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   733, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   734, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   736, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl16_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl16_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl16_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   739, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   740, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   741, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   742, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   743, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   744, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   758, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl16_damage_09_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   763, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl16_damage_09_loop ),
};

/* Char_Special 9: snapback_init (the snapback) */
const Sint32 pl16_special_09[] = {
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   5 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0300,   5 ),
	CG_FRAME(    0,   -1,   1,           0,   303, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x0300,   5 ),
	CG_FRAME(  -13,    0,   1,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1600,  16 ),
	CG_FRAME(    0,    1,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0, 0x0400,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0500,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,       0x23,    0,   0,     0, 0x0600,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0700,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0800,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0900,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0a00,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0b00,  88 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   304, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0c00,  88 ),
	CG_NEXT( pl16_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl16_special_10[] = {
	CG_FRAME(    0,    1,   6,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0d00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0e00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   305, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0, 0x0e00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0f00,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1000,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1100,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1200,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1300,  88 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1400,  88 ),
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   304, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x1500,  88 ),
	CG_NEXT( pl16_special_10 ),
};

/* Char_Special 11: snapback_move (the snapback) */
const Sint32 pl16_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_special_11 ),
};

/* Char_Special 12: ricochet_init (Ricochet Slash, 12 + atck_level)
   Char_Special 13 */
const Sint32 pl16_special_12[] = {
	CG_FRAME(    0,    1,   2,           0,    95, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   2,           0,    96, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_special_12_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_special_12_loop ),
};

/* Char_Special 14: ricochet_rise (Ricochet Slash, 14 + atck_level)
   Char_Special 15 */
const Sint32 pl16_special_14[] = {
	CG_FRAME(    0,    1,   4,           0,   591, 0x80,   0x00,  ATK,  0x00, 0x34 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    1,   4,           0,   592, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    1,   4,           0,   593, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   594, 0x80,   0x00,  ATK,  0x00, 0x32 | CSE,    0,   0,     0,      0,  70 ),
};

const Sint32 pl16_special_14_loop[] = {
	CG_FRAME(  -58,    1,   2, END_OF_DATA,   594, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl16_special_14_loop ),
};

const Sint32 pl16_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   2, END_OF_DATA,   595, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	0x00003db8,
};

/* Char_Special 16: ricochet_atck (Ricochet Slash, 16 + atck_level)
   Char_Special 17 */
const Sint32 pl16_special_16[] = {
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_special_16_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_special_16_loop ),
};

/* Char_Special 18: ricochet_land (Ricochet Slash, 18 + atck_level)
   Char_Special 19 */
const Sint32 pl16_special_18[] = {
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   108, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl16_special_18 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl16_extra_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   442, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl16_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl16_extra_01[] = {
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   201, 0x80,   0x00,  ATE,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   202, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  11,           0,   203, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   204, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   718, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_extra_01 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl16_damage_16[] = {
	CG_FRAME(    0,    0,   3,           0,   751, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl16_damage_17[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl16_damage_18[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl16_damage_19[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   755, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl16_damage_20[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl16_damage_21[] = {
	CG_FRAME(    0,    0,   3,           0,   751, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl16_damage_22[] = {
	CG_FRAME(    0,    0,   3,           0,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl16_damage_23[] = {
	CG_FRAME(    0,    0,   3,           0,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl16_damage_24[] = {
	CG_FRAME(    0,    0,   3,           0,   755, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl16_damage_25[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl16_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   758, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   1,           0,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2,           0,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl16_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl16_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl16_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl16_damage_31 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl16_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl16_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl16_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl16_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl16_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   706, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl16_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   717, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl16_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   716, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl16_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   715, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl16_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl16_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl16_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl16_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl16_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   724, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl16_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl16_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl16_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   734, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl16_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   733, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl16_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl16_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl16_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   751, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl16_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl16_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl16_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   755, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl16_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl16_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl16_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl16_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl16_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl16_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl16_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   739, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl16_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl16_dizzy_00[] = {
	CG_FRAME(    0,    0,   5,           0,   751, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl16_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    1,  14,           0,   460, 0x80,   0x00,    0,  0x02,       0x05,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,  14,           0,   461, 0x80,   0x00,    0,  0x03,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,  14,           0,   462, 0x80,   0x00,    0,  0x04,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    1,   4,           0,   463, 0x80,   0x00,    0,  0x05,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    1,   4,           0,   464, 0x80,   0x00,    0,  0x06,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,    0,  0x07,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,    0,  0x08,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x09,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x0b,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x0c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x0d,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x0e,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,    0,  0x0f,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00,    0,  0xff,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   469, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_catch_00 ),
};

/* Char_Catch 1: jump_p_set */
const Sint32 pl16_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,  14,           0,   485, 0x80,   0x00,    0,  0x02,       0x05,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,  14,           0,   486, 0x80,   0x00,    0,  0x03,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,  14,           0,   487, 0x80,   0x00,    0,  0x04,          0,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    1,   4,           0,   488, 0x80,   0x00,    0,  0x05,          0,    0,   0,     0, 0x0007,   1 ),
	CG_FRAME(    0,    1,   4,           0,   489, 0x80,   0x00,    0,  0x06,          0,    0,   0,     0, 0x0008,   1 ),
	CG_FRAME(    0,    1,   4,           0,   490, 0x80,   0x00,    0,  0x07,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,    0,  0x08,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x09,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x0a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x0b,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x0c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x0d,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x0e,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   492, 0x80,   0x00,    0,  0x0f,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x00,    0,  0xff,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_catch_01 ),
};

/* Char_Special 20: ride_init (Ride and Slash) */
const Sint32 pl16_special_20[] = {
	CG_FRAME(    0,    1,   4,           0,    95, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,    96, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_special_21_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   605, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,   1,           0,   606, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_special_21_loop_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   607, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl16_special_21_loop_loop ),
};

/* Char_Special 21: ride_j_init (Ride and Slash) */
const Sint32 pl16_special_21[] = {
	CG_FRAME(    0,    1,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   109, 0x80,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_special_21_loop ),
};

/* Char_Special 22: ride_fly (Ride and Slash) */
const Sint32 pl16_special_22[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   607, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_jump_09 ),
};

/* Char_Catch 2: ride_set */
const Sint32 pl16_catch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   607, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,   4,           0,   608, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   4,           0,   609, 0x80,   0x00,  ATK,  0x03,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    2,   4,           0,   610, 0x80,   0x00,  ATK,  0x04,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   611, 0x80,   0x00,  ATK,  0x05,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   613, 0x80,   0x00,  ATK,  0x07,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   614, 0x80,   0x00,  ATK,  0x08,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   615, 0x80,   0x00,  ATK,  0x09,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    3,   4,           0,   616, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   4,           0,   617, 0x80,   0x00,  ATK,  0x0b,       0x23,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   4,           0,   618, 0x80,   0x00,  ATK,  0x0c,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   619, 0x80,   0x00,  ATK,  0x0d,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   3,           0,   620, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     0, 0x0010,   7 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0x80,   0x00,  ATK,  0x0f,          0,    0,   0,     0, 0x0010,   7 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0x80,   0x00,  ATK,  0x10,          0,    0,   0,     0, 0x0010,   7 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x00,  ATK,  0xff,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl16_catch_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl16_catch_02_loop ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl16_win_00[] = {
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl16_win_00_loop[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_win_00_loop ),
};

const Sint32 pl16_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3, END_OF_DATA,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl16_win_01[] = {
	CG_FRAME(    0,    0,   3,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   260, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl16_win_01_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_win_01_loop ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl16_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl16_win_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_win_02_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl16_appear_00[] = {
	CG_FRAME(    0,    0,  16,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl16_appear_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_appear_00_loop ),
};

/* Char_Appear 1: appear_move */
const Sint32 pl16_appear_01[] = {
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl16_appear_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_appear_01_loop ),
};

/* group 27 4: pls16_00_lid_init */
const Sint32 pl16_grp27_04[] = {
	EF_FRAME(    0,    0,  12,           0,   247 ),
	EF_FRAME(    0,    0,   4,           0,   248 ),
	EF_FRAME(    0,    0,   4,           0,   249 ),
	EF_FRAME(    0,    0,   8,           0,   250 ),
	EF_FRAME(    0,    0,   4,           0,   249 ),
	EF_FRAME(    0,    0,   4,           0,   248 ),
	EF_FRAME(    0,    0,   8,           0,   247 ),
};

const Sint32 pl16_grp27_04_loop[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   253 ),
	CG_NEXT( pl16_grp27_04_loop ),
};

/* group 27 5: pls16_00_lid_move */
const Sint32 pl16_grp27_05[] = {
	EF_FRAME(    0,    0,   1,           0,   219 ),
	EF_FRAME(    0,    0,   1,           0,   220 ),
	EF_FRAME(    0,    0,   1,           0,   221 ),
	EF_FRAME(    0,    0,   1,           0,   222 ),
	EF_FRAME(    0,    0,   1,           0,   223 ),
	EF_FRAME(    0,    0,   1,           0,   224 ),
	EF_FRAME(    0,    0,   1,           0,   225 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   226 ),
	CG_NEXT( pl16_grp27_05 ),
};

/* group 27 6: pls16_00_lid_land */
const Sint32 pl16_grp27_06[] = {
	EF_FRAME(    0,    0,   2,           0,   227 ),
	EF_FRAME(    0,    0,   2,           0,   228 ),
	EF_FRAME(    0,    0,   2,           0,   229 ),
	EF_FRAME(    0,    0,   2,           0,   230 ),
	EF_FRAME(    0,    0,   2,           0,   231 ),
	EF_FRAME(    0,    0,   2,           0,   232 ),
	EF_FRAME(    0,    0,   2,           0,   233 ),
	EF_FRAME(    0,    0,   2,           0,   234 ),
	EF_FRAME(    0,    0,   2,           0,   235 ),
	EF_FRAME(    0,    0,   2,           0,   236 ),
	EF_FRAME(    0,    0,   2,           0,   237 ),
	EF_FRAME(    0,    0,   2,           0,   238 ),
	EF_FRAME(    0,    0,   2,           0,   239 ),
	EF_FRAME(    0,    0,   2,           0,   240 ),
	EF_FRAME(    0,    0,   2,           0,   241 ),
	EF_FRAME(    0,    0,   2,           0,   242 ),
	EF_FRAME(    0,    0,   2,           0,   243 ),
	EF_FRAME(    0,    0,   2,           0,   244 ),
	EF_FRAME(    0,    0,   2,           0,   245 ),
	EF_FRAME(    0,    0,   2,           0,   246 ),
};

const Sint32 pl16_grp27_06_loop[] = {
	EF_FRAME(    0,    0,   2, END_OF_DATA,   247 ),
	CG_NEXT( pl16_grp27_06_loop ),
};

const Sint32 pl16_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    76, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00004ee0,
};

const Sint32 pl16_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,   4,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  53 ),
	0x00004fac,
};

const Sint32 pl16_unused_04[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   4,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00005014,
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl16_change_00[] = {
	CG_FRAME(    0,    0,   7,           0,   199, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   198, 0x80,   0x00,  ATK,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   199, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   199, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl16_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl16_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl16_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl16_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,    0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl16_change_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl16_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   201, 0x80,   0x00,    0,  0x00,       0x0e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  11,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl16_change_04 ),
};

/* group 27 0: pls16_01_t0_init */
const Sint32 pl16_grp27_00[] = {
	EF_FRAME(    0,    1,   3, END_OF_DATA,   329 ),
	CG_NEXT( pl16_grp27_00 ),
};

/* group 27 1: pls16_01_t1_move */
const Sint32 pl16_grp27_01[] = {
	EF_FRAME(   13,    0,   1,           0,   330 ),
	EF_FRAME(   14,    0,   1,           0,   331 ),
	EF_FRAME(   15,    0,   1,           0,   332 ),
	EF_FRAME(   16,    0,   1,           0,   333 ),
	EF_FRAME(   17,    0,   1,           0,   334 ),
};

const Sint32 pl16_grp27_01_loop[] = {
	EF_FRAME(   18,    0,   1, END_OF_DATA,   335 ),
	CG_NEXT( pl16_grp27_01_loop ),
};

/* group 27 2: pls16_01_t2_move */
const Sint32 pl16_grp27_02[] = {
	EF_FRAME(    1,    0,   1,           0,   310 ),
	EF_FRAME(    2,    0,   1,           0,   311 ),
	EF_FRAME(    3,    0,   1,           0,   312 ),
	EF_FRAME(   22,    0,   1,           0,    -1 ),
	EF_FRAME(    4,    0,   1,           0,   313 ),
	EF_FRAME(    5,    0,   1,           0,   313 ),
	EF_FRAME(    6,    0,   1,           0,   314 ),
	EF_FRAME(    7,    0,   1,           0,   315 ),
	EF_FRAME(    8,    0,   1,           0,   316 ),
	EF_FRAME(    9,    0,   1,           0,   317 ),
	EF_FRAME(   10,    0,   1,           0,   318 ),
	EF_FRAME(   11,    0,   1,           0,   319 ),
};

const Sint32 pl16_grp27_02_loop[] = {
	EF_FRAME(   12,    0,   1, END_OF_DATA,   320 ),
	CG_NEXT( pl16_grp27_02_loop ),
};

/* group 27 3: pls16_01_t2_move */
const Sint32 pl16_grp27_03[] = {
	EF_FRAME(    1,    0,   1,           0,   310 ),
	EF_FRAME(    2,    0,   1,           0,   311 ),
	EF_FRAME(    3,    0,   1,           0,   312 ),
	EF_FRAME(   22,    0,   1,           0,    -1 ),
	EF_FRAME(   13,    0,   1,           0,   321 ),
	EF_FRAME(   14,    0,   1,           0,   321 ),
	EF_FRAME(   15,    0,   1,           0,   322 ),
	EF_FRAME(   16,    0,   1,           0,   323 ),
	EF_FRAME(   17,    0,   1,           0,   324 ),
	EF_FRAME(   18,    0,   1,           0,   325 ),
	EF_FRAME(   19,    0,   1,           0,   326 ),
	EF_FRAME(   20,    0,   1,           0,   327 ),
};

const Sint32 pl16_grp27_03_loop[] = {
	EF_FRAME(   21,    0,   1, END_OF_DATA,   328 ),
	CG_NEXT( pl16_grp27_03_loop ),
};

/* group 27 7: pls16_03_nml_p_move */
const Sint32 pl16_grp27_07[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   470 ),
	EF_FRAME(    0,    0,   1,           0,   471 ),
	EF_FRAME(    0,    0,   1,           0,   472 ),
	EF_FRAME(    0,    0,   1,           0,   473 ),
	EF_FRAME(    0,    0,   1,           0,   474 ),
	EF_FRAME(    0,    0,   1,           0,   475 ),
	EF_FRAME(    0,    0,   1,           0,   476 ),
	EF_FRAME(    0,    0,   1,           0,   477 ),
	EF_FRAME(    0,    0,   1,           0,   478 ),
	EF_FRAME(    0,    0,   1,           0,   479 ),
	EF_FRAME(    0,    0,   1,           0,   480 ),
	EF_FRAME(    0,    0,   1,           0,   481 ),
	EF_FRAME(    0,    0,   1,           0,   482 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   483 ),
	CG_NEXT( pl16_grp27_07 ),
};

/* group 27 8: pls16_03_jump_p_move */
const Sint32 pl16_grp27_08[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,   495 ),
	EF_FRAME(    0,    0,   1,           0,   496 ),
	EF_FRAME(    0,    0,   1,           0,   497 ),
	EF_FRAME(    0,    0,   1,           0,   498 ),
	EF_FRAME(    0,    0,   1,           0,   499 ),
	EF_FRAME(    0,    0,   1,           0,   500 ),
	EF_FRAME(    0,    0,   1,           0,   501 ),
	EF_FRAME(    0,    0,   1,           0,   502 ),
	EF_FRAME(    0,    0,   1,           0,   503 ),
	EF_FRAME(    0,    0,   1,           0,   504 ),
	EF_FRAME(    0,    0,   1,           0,   505 ),
	EF_FRAME(    0,    0,   1,           0,   506 ),
	EF_FRAME(    0,    0,   1,           0,   507 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   508 ),
	CG_NEXT( pl16_grp27_08 ),
};

/* group 27 9: pls16_02_dive_init */
const Sint32 pl16_grp27_09[] = {
	EF_FRAME(    0,    0,   1,           0,   597 ),
	EF_FRAME(    0,    0,   1,           0,   598 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   599 ),
	CG_NEXT( pl16_grp27_09 ),
};

/* group 27 10: pls16_02_drop_init */
const Sint32 pl16_grp27_10[] = {
	EF_FRAME(    0,    0,   5,           0,   600 ),
	EF_FRAME(    0,    0,   5,           0,   601 ),
	EF_FRAME(    0,    0,   5,           0,   602 ),
	EF_FRAME(    0,    0,   5,           0,   603 ),
	EF_FRAME(    0,    0,   5, END_OF_DATA,   604 ),
	CG_NEXT( pl16_grp27_10 ),
};

/* group 27 11: pls16_05_init, pls16_05_move */
const Sint32 pl16_grp27_11[] = {
	EF_FRAME(    0,    0,   4,           0,    -1 ),
	EF_FRAME(    0,    0,   4,           0,   631 ),
	EF_FRAME(    0,    0,   4,           0,   632 ),
	EF_FRAME(    0,    0,   4,           0,   633 ),
	EF_FRAME(    0,    0,   4,           0,   634 ),
	EF_FRAME(    0,    0,   4,           0,    -1 ),
	EF_FRAME(    0,    0,   4,           0,    -1 ),
	EF_FRAME(    0,    0,   4,           0,   622 ),
	EF_FRAME(    0,    0,   4,           0,   623 ),
	EF_FRAME(    0,    0,   4,           0,   624 ),
	EF_FRAME(    0,    0,   4,           0,   625 ),
	EF_FRAME(    0,    0,   4,           0,   626 ),
	EF_FRAME(    0,    0,   4,           0,   627 ),
	EF_FRAME(    0,    0,   4,           0,   628 ),
	EF_FRAME(    0,    0,   4,           0,   629 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   630 ),
	CG_NEXT( pl16_grp27_11 ),
};

/* group 27 12: pls16_04_init */
const Sint32 pl16_grp27_12[] = {
	EF_FRAME(    0,    0,  16,           0,    -1 ),
	EF_FRAME(    0,    0,   8,           0,   681 ),
	EF_FRAME(    0,    0,   8,           0,   682 ),
	EF_FRAME(    0,    0,   8,           0,   683 ),
	EF_FRAME(    0,    0,   8,           0,   684 ),
};

const Sint32 pl16_grp27_12_loop[] = {
	EF_FRAME(    0,    0,   8, END_OF_DATA,   685 ),
	CG_NEXT( pl16_grp27_12_loop ),
};

/* group 27 13: pls16_06_wait */
const Sint32 pl16_grp27_13[] = {
	EF_FRAME(    0,    0,   1,           0,   691 ),
	EF_FRAME(    0,    0,   1,           0,   692 ),
	EF_FRAME(    0,    0,   1,           0,   693 ),
	EF_FRAME(    0,    0,   1,           0,   694 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   695 ),
	CG_NEXT( pl16_grp27_13 ),
};

/* group 27 14: pls16_06_step0 */
const Sint32 pl16_grp27_14[] = {
	EF_FRAME(    0,    0,   1,           0,   686 ),
	EF_FRAME(    0,    0,   1,           0,   687 ),
	EF_FRAME(    0,    0,   1,           0,   688 ),
	EF_FRAME(    0,    0,   1,           0,   689 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   690 ),
	CG_NEXT( pl16_grp27_14 ),
};

/* group 27 15: pls16_06_step1 */
const Sint32 pl16_grp27_15[] = {
	EF_FRAME(    0,    0,   1,           0,   696 ),
	EF_FRAME(    0,    0,   1,           0,   697 ),
	EF_FRAME(    0,    0,   1,           0,   698 ),
	EF_FRAME(    0,    0,   1,           0,   699 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   700 ),
	CG_NEXT( pl16_grp27_15 ),
};

/* group 27 16: pls16_08_init */
const Sint32 pl16_grp27_16[] = {
	EF_FRAME(    0,    0,   1, END_OF_DATA,   278 ),
	CG_NEXT( pl16_grp27_16 ),
};

/* group 27 17: pls16_08_fly */
const Sint32 pl16_grp27_17[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   279 ),
	CG_NEXT( pl16_grp27_17 ),
};

/* group 27 18: pls16_07_init */
const Sint32 pl16_grp27_18[] = {
	EF_FRAME(    0,    0,   2,           0,   266 ),
	EF_FRAME(    0,    0,   2,           0,   267 ),
	EF_FRAME(    0,    0,   2,           0,   268 ),
	EF_FRAME(    0,    0,   2,           0,   269 ),
	EF_FRAME(    0,    0,   2,           0,   270 ),
	EF_FRAME(    0,    0,   2,           0,   271 ),
	EF_FRAME(    0,    0,   2,           0,   272 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   273 ),
	CG_NEXT( pl16_grp27_18 ),
};

/* group 27 19: pls16_00_hole_init */
const Sint32 pl16_grp27_19[] = {
	EF_FRAME(    0,    0,   8, END_OF_DATA,   253 ),
	CG_NEXT( pl16_grp27_19 ),
};
