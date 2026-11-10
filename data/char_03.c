/* Morrigan (character 03): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_03      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_03     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl03.c) says which group
                   and number each move uses.
   pl03_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl03_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl03_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl03_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_03
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_03, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_03 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl03_stand_04[], pl03_stand_00[], pl03_stand_02[], pl03_stand_03[],
	pl03_jump_00[], pl03_jump_00_loop[], pl03_jump_02[], pl03_jump_02_loop[], pl03_jump_03[],
	pl03_jump_06[], pl03_jump_09[], pl03_jump_08[], pl03_jump_11[], pl03_dash_00[],
	pl03_dash_00_loop[], pl03_dash_04[], pl03_dash_05[], pl03_dash_07[], pl03_dash_07_loop[],
	pl03_dash_06[], pl03_dash_06_loop[], pl03_dash_02[], pl03_dash_01[], pl03_dash_01_loop[],
	pl03_dash_03[], pl03_crouch_00[], pl03_crouch_01[], pl03_crouch_02[], pl03_turn_00[],
	pl03_turn_01[], pl03_guard_00[], pl03_guard_01[], pl03_guard_02[], pl03_guard_03[],
	pl03_guard_04[], pl03_guard_05[], pl03_guard_06[], pl03_guard_07[], pl03_air_guard_00[],
	pl03_air_guard_01[], pl03_air_guard_02[], pl03_stand_p_00[], pl03_stand_p_03[],
	pl03_stand_p_01[], pl03_stand_p_02[], pl03_stand_k_00[], pl03_stand_k_01[], pl03_stand_k_02[],
	pl03_stand_k_03[], pl03_stand_k_03_loop[], pl03_stand_k_04[], pl03_stand_k_04_loop[],
	pl03_stand_k_05[], pl03_stand_k_05_loop[], pl03_crouch_p_00[], pl03_crouch_p_01[],
	pl03_crouch_p_02[], pl03_crouch_p_03[], pl03_crouch_k_00[], pl03_crouch_k_01[],
	pl03_crouch_k_02[], pl03_jump_p_00[], pl03_jump_p_01[], pl03_jump_p_02[], pl03_jump_k_00[],
	pl03_jump_k_01[], pl03_jump_k_02[], pl03_jump_k_05[], pl03_jump_k_05_loop[], pl03_jump_k_04[],
	pl03_jump_k_04_loop[], pl03_damage_00[], pl03_damage_01[], pl03_damage_02[], pl03_damage_03[],
	pl03_damage_04[], pl03_damage_05[], pl03_damage_06[], pl03_damage_07[], pl03_damage_08[],
	pl03_damage_09[], pl03_damage_09_loop[], pl03_damage_10[], pl03_damage_11[], pl03_damage_12[],
	pl03_damage_13[], pl03_damage_16[], pl03_damage_17[], pl03_damage_18[], pl03_damage_19[],
	pl03_damage_20[], pl03_damage_26[], pl03_damage_21[], pl03_damage_22[], pl03_damage_23[],
	pl03_damage_24[], pl03_damage_25[], pl03_damage_28[], pl03_damage_29[], pl03_damage_27[],
	pl03_damage_31[], pl03_damage_33[], pl03_dizzy_00[], pl03_catch_00[], pl03_catch_01[],
	pl03_catch_01_loop[], pl03_catch_02[], pl03_catch_02_loop[], pl03_catch_03[], pl03_catch_04[],
	pl03_catch_04_loop[], pl03_catch_05[], pl03_catch_06[], pl03_catch_07[], pl03_catch_07_loop[],
	pl03_catch_08[], pl03_catch_08_loop[], pl03_catch_09[], pl03_catch_09_loop[], pl03_get_up_00[],
	pl03_get_up_01[], pl03_get_up_02[], pl03_get_up_03[], pl03_appear_00[], pl03_appear_01[],
	pl03_appear_01_loop[], pl03_appear_02[], pl03_appear_03[], pl03_appear_04[],
	pl03_appear_04_loop[], pl03_appear_05[], pl03_appear_05_loop[], pl03_extra_05[],
	pl03_extra_05_loop[], pl03_extra_06[], pl03_extra_06_loop[], pl03_extra_07[], pl03_extra_08[],
	pl03_extra_08_loop[], pl03_extra_09[], pl03_extra_02[], pl03_extra_03[], pl03_special_00[],
	pl03_special_02[], pl03_special_04[], pl03_special_05[], pl03_special_06[], pl03_special_07[],
	pl03_special_08[], pl03_special_09[], pl03_special_10[], pl03_special_11[], pl03_special_12[],
	pl03_special_13[], pl03_special_14[], pl03_special_15[], pl03_special_16[], pl03_special_17[],
	pl03_super_00[], pl03_super_01[], pl03_super_02[], pl03_super_03[], pl03_super_03_loop[],
	pl03_super_04[], pl03_super_05[], pl03_super_06[], pl03_super_06_loop[], pl03_super_07[],
	pl03_super_07_loop[], pl03_super_08[], pl03_super_09[], pl03_catch_10[], pl03_catch_10_loop[],
	pl03_super_10[], pl03_super_11[], pl03_super_11_loop[], pl03_super_12[], pl03_super_13[],
	pl03_super_13_loop[], pl03_catch_11[], pl03_catch_11_loop[], pl03_catch_12[], pl03_super_18[],
	pl03_super_18_loop[], pl03_super_19[], pl03_super_20[], pl03_super_14[], pl03_catch_13[],
	pl03_catch_14[], pl03_catch_15[], pl03_catch_15_loop[], pl03_catch_16[], pl03_super_25[],
	pl03_super_26[], pl03_super_26_loop[], pl03_super_27[], pl03_super_27_loop[], pl03_super_28[],
	pl03_super_29[], pl03_super_29_loop[], pl03_super_30[], pl03_super_31[], pl03_super_32[],
	pl03_super_33[], pl03_super_34[], pl03_super_35[], pl03_super_35_loop[], pl03_super_36[],
	pl03_super_37[], pl03_super_38[], pl03_super_39[], pl03_super_40[], pl03_super_41[],
	pl03_super_42[], pl03_super_43[], pl03_super_44[], pl03_super_45[], pl03_super_46[],
	pl03_extra_00[], pl03_extra_01[], pl03_win_00[], pl03_win_00_loop[], pl03_win_01[],
	pl03_win_01_loop[], pl03_win_02[], pl03_win_02_loop[], pl03_win_03[], pl03_win_03_loop[],
	pl03_win_04[], pl03_win_04_loop[], pl03_win_05[], pl03_win_05_loop[], pl03_win_06[],
	pl03_win_06_loop[], pl03_win_07[], pl03_win_07_loop[], pl03_win_08[], pl03_win_08_loop[],
	pl03_effect_00[], pl03_effect_00_loop[], pl03_effect_01[], pl03_effect_03[], pl03_effect_04[],
	pl03_effect_02[], pl03_caught_00[], pl03_caught_01[], pl03_caught_02[], pl03_caught_03[],
	pl03_caught_04[], pl03_caught_05[], pl03_caught_06[], pl03_caught_07[], pl03_caught_08[],
	pl03_caught_09[], pl03_caught_10[], pl03_caught_11[], pl03_caught_12[], pl03_caught_13[],
	pl03_caught_14[], pl03_caught_15[], pl03_caught_16[], pl03_caught_17[], pl03_caught_18[],
	pl03_caught_19[], pl03_caught_20[], pl03_caught_21[], pl03_unused_00[], pl03_grp27_00[],
	pl03_grp27_01[], pl03_grp27_02[], pl03_grp27_03[], pl03_grp27_04[], pl03_grp27_05[],
	pl03_grp27_06[], pl03_grp27_07[], pl03_grp27_08[], pl03_grp27_09[], pl03_grp27_10[],
	pl03_grp27_10_loop[], pl03_grp27_11[], pl03_grp27_12[], pl03_grp27_13[], pl03_grp27_14[],
	pl03_grp27_15[], pl03_grp27_16[], pl03_grp27_17[], pl03_grp27_18[], pl03_grp27_19[],
	pl03_grp27_20[], pl03_grp27_21[], pl03_grp27_21_loop[], pl03_grp27_22[], pl03_grp27_22_loop[],
	pl03_grp27_23[], pl03_grp27_24[], pl03_grp27_25[];
extern const Sint32 *const pl03_stand_tbl[], *const pl03_jump_tbl[], *const pl03_dash_tbl[],
	*const pl03_crouch_tbl[], *const pl03_turn_tbl[], *const pl03_guard_tbl[],
	*const pl03_air_guard_tbl[], *const pl03_stand_p_tbl[], *const pl03_stand_k_tbl[],
	*const pl03_crouch_p_tbl[], *const pl03_crouch_k_tbl[], *const pl03_jump_p_tbl[],
	*const pl03_jump_k_tbl[], *const pl03_damage_tbl[], *const pl03_dizzy_tbl[],
	*const pl03_catch_tbl[], *const pl03_caught_tbl[], *const pl03_get_up_tbl[],
	*const pl03_appear_tbl[], *const pl03_win_tbl[], *const pl03_extra_tbl[],
	*const pl03_special_tbl[], *const pl03_super_tbl[], *const pl03_effect_tbl[],
	*const pl03_grp25_tbl[], *const pl03_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_03[16] = {
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
const Sint32 *const *const char_tbl_03[] = {
	pl03_stand_tbl, pl03_jump_tbl, pl03_dash_tbl, pl03_crouch_tbl, pl03_turn_tbl, pl03_guard_tbl,
	pl03_air_guard_tbl, pl03_stand_p_tbl, pl03_stand_k_tbl, pl03_crouch_p_tbl, pl03_crouch_k_tbl, pl03_jump_p_tbl,
	pl03_jump_k_tbl, pl03_damage_tbl, pl03_dizzy_tbl, pl03_catch_tbl, pl03_caught_tbl, pl03_get_up_tbl,
	pl03_appear_tbl, pl03_win_tbl, pl03_extra_tbl, pl03_special_tbl, pl03_super_tbl, pl03_effect_tbl,
	pl03_effect_tbl, pl03_grp25_tbl, pl03_stand_tbl, pl03_grp27_tbl, 0,
};

const Sint32 *const pl03_stand_tbl[] = {
	pl03_stand_00, pl03_stand_00, pl03_stand_02, pl03_stand_03, pl03_stand_04, pl03_stand_04,
	pl03_stand_02, pl03_stand_03, pl03_stand_02, pl03_stand_03, 0,
};

const Sint32 *const pl03_jump_tbl[] = {
	pl03_jump_00, pl03_jump_00, pl03_jump_02, pl03_jump_03, pl03_jump_03, pl03_jump_03,
	pl03_jump_06, pl03_jump_06, pl03_jump_08, pl03_jump_09, pl03_jump_09, pl03_jump_11,
	0,
};

const Sint32 *const pl03_dash_tbl[] = {
	pl03_dash_00, pl03_dash_01, pl03_dash_02, pl03_dash_03, pl03_dash_04, pl03_dash_05,
	pl03_dash_06, pl03_dash_07, 0,
};

const Sint32 *const pl03_crouch_tbl[] = {
	pl03_crouch_00, pl03_crouch_01, pl03_crouch_02, 0,
};

const Sint32 *const pl03_turn_tbl[] = {
	pl03_turn_00, pl03_turn_01, 0,
};

const Sint32 *const pl03_guard_tbl[] = {
	pl03_guard_00, pl03_guard_01, pl03_guard_02, pl03_guard_03, pl03_guard_04, pl03_guard_05,
	pl03_guard_06, pl03_guard_07, 0,
};

const Sint32 *const pl03_air_guard_tbl[] = {
	pl03_air_guard_00, pl03_air_guard_01, pl03_air_guard_02, 0,
};

const Sint32 *const pl03_stand_p_tbl[] = {
	pl03_stand_p_00, pl03_stand_p_01, pl03_stand_p_02, pl03_stand_p_03, 0,
};

const Sint32 *const pl03_stand_k_tbl[] = {
	pl03_stand_k_00, pl03_stand_k_01, pl03_stand_k_02, pl03_stand_k_03, pl03_stand_k_04, pl03_stand_k_05,
	0,
};

const Sint32 *const pl03_crouch_p_tbl[] = {
	pl03_crouch_p_00, pl03_crouch_p_01, pl03_crouch_p_02, pl03_crouch_p_03, 0,
};

const Sint32 *const pl03_crouch_k_tbl[] = {
	pl03_crouch_k_00, pl03_crouch_k_01, pl03_crouch_k_02, 0,
};

const Sint32 *const pl03_jump_p_tbl[] = {
	pl03_jump_p_00, pl03_jump_p_01, pl03_jump_p_02, pl03_jump_p_00, pl03_jump_p_01, pl03_jump_p_02,
	0,
};

const Sint32 *const pl03_jump_k_tbl[] = {
	pl03_jump_k_00, pl03_jump_k_01, pl03_jump_k_02, pl03_jump_k_00, pl03_jump_k_04, pl03_jump_k_05,
	0,
};

const Sint32 *const pl03_damage_tbl[] = {
	pl03_damage_00, pl03_damage_01, pl03_damage_02, pl03_damage_03, pl03_damage_04, pl03_damage_05,
	pl03_damage_06, pl03_damage_07, pl03_damage_08, pl03_damage_09, pl03_damage_10, pl03_damage_11,
	pl03_damage_12, pl03_damage_13, pl03_damage_13, pl03_damage_13, pl03_damage_16, pl03_damage_17,
	pl03_damage_18, pl03_damage_19, pl03_damage_20, pl03_damage_21, pl03_damage_22, pl03_damage_23,
	pl03_damage_24, pl03_damage_25, pl03_damage_26, pl03_damage_27, pl03_damage_28, pl03_damage_29,
	pl03_damage_27, pl03_damage_31, pl03_damage_27, pl03_damage_33, pl03_damage_13, 0,
};

const Sint32 *const pl03_dizzy_tbl[] = {
	pl03_dizzy_00, 0,
};

const Sint32 *const pl03_catch_tbl[] = {
	pl03_catch_00, pl03_catch_01, pl03_catch_02, pl03_catch_03, pl03_catch_04, pl03_catch_05,
	pl03_catch_06, pl03_catch_07, pl03_catch_08, pl03_catch_09, pl03_catch_10, pl03_catch_11,
	pl03_catch_12, pl03_catch_13, pl03_catch_14, pl03_catch_15, pl03_catch_16, 0,
};

const Sint32 *const pl03_caught_tbl[] = {
	pl03_caught_00, pl03_caught_01, pl03_caught_02, pl03_caught_03, pl03_caught_04, pl03_caught_05,
	pl03_caught_06, pl03_caught_07, pl03_caught_08, pl03_caught_09, pl03_caught_10, pl03_caught_11,
	pl03_caught_12, pl03_caught_13, pl03_caught_14, pl03_caught_15, pl03_caught_16, pl03_caught_17,
	pl03_caught_18, pl03_caught_19, pl03_caught_20, pl03_caught_21, 0,
};

const Sint32 *const pl03_get_up_tbl[] = {
	pl03_get_up_00, pl03_get_up_01, pl03_get_up_02, pl03_get_up_03, 0,
};

const Sint32 *const pl03_appear_tbl[] = {
	pl03_appear_00, pl03_appear_01, pl03_appear_02, pl03_appear_03, pl03_appear_04, pl03_appear_05,
	0,
};

const Sint32 *const pl03_win_tbl[] = {
	pl03_win_00, pl03_win_01, pl03_win_02, pl03_win_03, pl03_win_04, pl03_win_05,
	pl03_win_06, pl03_win_07, pl03_win_08, 0,
};

const Sint32 *const pl03_extra_tbl[] = {
	pl03_extra_00, pl03_extra_01, pl03_extra_02, pl03_extra_03, pl03_extra_02, pl03_extra_05,
	pl03_extra_06, pl03_extra_07, pl03_extra_08, pl03_extra_09, 0,
};

const Sint32 *const pl03_special_tbl[] = {
	pl03_special_00, pl03_special_00, pl03_special_02, pl03_special_02, pl03_special_04, pl03_special_05,
	pl03_special_06, pl03_special_07, pl03_special_08, pl03_special_09, pl03_special_10, pl03_special_11,
	pl03_special_12, pl03_special_13, pl03_special_14, pl03_special_15, pl03_special_16, pl03_special_17,
	0,
};

const Sint32 *const pl03_super_tbl[] = {
	pl03_super_00, pl03_super_01, pl03_super_02, pl03_super_03, pl03_super_04, pl03_super_05,
	pl03_super_06, pl03_super_07, pl03_super_08, pl03_super_09, pl03_super_10, pl03_super_11,
	pl03_super_12, pl03_super_13, pl03_super_14, pl03_catch_10, pl03_catch_11, pl03_catch_12,
	pl03_super_18, pl03_super_19, pl03_super_20, pl03_catch_13, pl03_catch_14, pl03_catch_15,
	pl03_catch_16, pl03_super_25, pl03_super_26, pl03_super_27, pl03_super_28, pl03_super_29,
	pl03_super_30, pl03_super_31, pl03_super_32, pl03_super_33, pl03_super_34, pl03_super_35,
	pl03_super_36, pl03_super_37, pl03_super_38, pl03_super_39, pl03_super_40, pl03_super_41,
	pl03_super_42, pl03_super_43, pl03_super_44, pl03_super_45, pl03_super_46, 0,
};

const Sint32 *const pl03_effect_tbl[] = {
	pl03_effect_00, pl03_effect_01, pl03_effect_02, pl03_effect_03, pl03_effect_04, 0,
};

const Sint32 *const pl03_grp25_tbl[] = {
	pl03_stand_00, 0,
};

const Sint32 *const pl03_grp27_tbl[] = {
	pl03_grp27_00, pl03_grp27_01, pl03_grp27_02, pl03_grp27_03, pl03_grp27_04, pl03_grp27_05,
	pl03_grp27_06, pl03_grp27_07, pl03_grp27_08, pl03_grp27_09, pl03_grp27_10, pl03_grp27_11,
	pl03_grp27_12, pl03_grp27_13, pl03_grp27_14, pl03_grp27_15, pl03_grp27_16, pl03_grp27_17,
	pl03_grp27_18, pl03_grp27_19, pl03_grp27_20, pl03_grp27_21, pl03_grp27_22, pl03_grp27_23,
	pl03_grp27_24, pl03_grp27_25, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 4
   Char_Stand 5
   Char_Fly 4
   Char_Fly 5 */
const Sint32 pl03_stand_04[] = {
	CG_FRAME(    0,    0,    6,           0,   322, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   group 25 0
   Char_Fly 0
   Char_Fly 1: act_fly */
const Sint32 pl03_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    72, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    73, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    74, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    84, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   Char_Stand 8
   Char_Fly 2
   Char_Fly 6
   Char_Fly 8 */
const Sint32 pl03_stand_02[] = {
	CG_FRAME(    0,    0,    3,           0,    93, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    94, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    95, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    96, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    97, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    98, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    99, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   100, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   101, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   102, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   103, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   104, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   Char_Stand 9
   Char_Fly 3
   Char_Fly 7
   Char_Fly 9 */
const Sint32 pl03_stand_03[] = {
	CG_FRAME(    0,    0,    3,           0,   105, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   106, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   107, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   108, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   109, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   111, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   114, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   115, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_stand_03 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl03_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_jump_00_loop ),
};

/* Char_Jump 2: jump_p_catch_throw */
const Sint32 pl03_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   137, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   138, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   139, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   140, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   141, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   142, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   143, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_jump_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_jump_02_loop ),
};

/* Char_Jump 3: jump_hk_dive, chouhatsu_fall (the taunt)
   Char_Jump 4
   Char_Jump 5: jump_p_catch_land */
const Sint32 pl03_jump_03[] = {
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7 */
const Sint32 pl03_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: chouhatsu_move (the taunt), jillusion_rush (Darkness Illusion (hyper)), act_fall, act_air_guard_hold, ...
   Char_Jump 10: change_in */
const Sint32 pl03_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_jump_09 ),
};

/* Char_Jump 8 */
const Sint32 pl03_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   137, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   138, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   139, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   140, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   144, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl03_jump_11[] = {
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl03_dash_00[] = {
	CG_FRAME(    0,    0,    2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_dash_00_loop ),
};

/* Char_Dash 4: air_dash_init, fly_start_init, fly_start2_init */
const Sint32 pl03_dash_04[] = {
	CG_FRAME(    0,    1,    6, END_OF_DATA,   127, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_dash_04 ),
};

/* Char_Dash 5: air_dash_start (5 + no) */
const Sint32 pl03_dash_05[] = {
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   118, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_dash_05 ),
};

/* Char_Dash 7 */
const Sint32 pl03_dash_07[] = {
	CG_FRAME(    0,    0,    4,           0,   240, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   241, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   240, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_dash_07_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   683, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_dash_07_loop ),
};

/* Char_Dash 6 */
const Sint32 pl03_dash_06[] = {
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   683, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   683, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_dash_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   118, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_dash_06_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl03_dash_02[] = {
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl03_dash_01[] = {
	CG_FRAME(    0,    1,    2,           0,   137, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_dash_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl03_dash_03[] = {
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   147, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl03_crouch_00[] = {
	CG_FRAME(    0,    0,    3,           0,   148, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   150, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl03_crouch_01[] = {
	CG_FRAME(    0,    0,    3,           0,    85, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    86, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    87, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    88, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    89, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    90, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    91, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    92, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl03_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   152, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl03_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,   154, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   156, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl03_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,   157, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   158, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   160, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl03_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl03_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   164, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl03_guard_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   161, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl03_guard_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   164, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl03_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl03_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   168, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl03_guard_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl03_guard_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   168, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl03_air_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl03_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   170, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl03_air_guard_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   170, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl03_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   325, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    2,           0,   327, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   336, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl03_stand_p_03[] = {
	CG_FRAME(    0,    0,    2,           0,   325, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   327, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   336, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl03_stand_p_01[] = {
	CG_FRAME(    0,    0,    2,           0,   328, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   329, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME( 0x80,    0,    2,           0,   331, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    2,           0,   332, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x83,    0,    2,           0,   333, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   331, 0x90,   0x47,              ATE,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x90,   0x47,              ATE,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   333, 0x90,   0x47,              ATE,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    3,           0,   334, 0x90,   0x47,              ATE,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   335, 0x90,   0x47,              ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   336, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl03_stand_p_02[] = {
	CG_FRAME(    0,    0,    2,           0,   337, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   338, 0x90,   0x46,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   340, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   341, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x81,    0,    4,           0,   342, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   14,           0,   342, 0x90,   0x43,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    4,           0,   343, 0x90,   0x46,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   344, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl03_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   345, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   346, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   346, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   347, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   345, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl03_stand_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   348, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   350, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    3,           0,   351, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x81,    0,   12,           0,   352, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    4,           0,   348, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   353, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl03_stand_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   354, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   355, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   356, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   357, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   358, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    2,           0,   359, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    1,           0,   360, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   16,           0,   361, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    6,           0,   362, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   363, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_stand_k_02 ),
};

/* Char_Stand_K 3 */
const Sint32 pl03_stand_k_03[] = {
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   379, 0x90,   0x40, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   455, 0x90,   0x00, ATK | CG_ATCK_NG | 1,  0x00, 0x04,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_stand_k_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   455, 0x90,   0x00, ATK | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_stand_k_03_loop ),
};

/* Char_Stand_K 4: stand_hk_start */
const Sint32 pl03_stand_k_04[] = {
	CG_FRAME(    0,    0,    8,           0,   456, 0x90,   0x00, ATK | CG_ATCK_NG,  0x01,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,   20,    3,           0,   457, 0x90,   0x00, ATK | CG_ATCK_NG,  0x01,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,   21,    3,           0,   458, 0x90,   0x00, ATK | CG_ATCK_NG,  0x01,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,   22,    7,           0,   459, 0x90,   0x00, ATK | CG_ATCK_NG,  0x01,    0,    0,   0,     0,      0,  87 ),
};

const Sint32 pl03_stand_k_04_loop[] = {
	CG_FRAME(    0,    0,    5, END_OF_DATA,   460, 0x90,   0x40, ATK | CG_ATCK_NG,  0x01,    0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl03_stand_k_04_loop ),
};

/* Char_Stand_K 5: stand_hk_jump */
const Sint32 pl03_stand_k_05[] = {
	CG_FRAME(    0,    0,    4,           0,   149, 0x90,   0x40, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   462, 0x90,   0x40, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   463, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_stand_k_05_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   464, 0x90,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_stand_k_05_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl03_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   366, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   364, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    1,    2,           0,   365, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    2,           0,   365, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   366, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl03_crouch_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   367, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    3,           0,   368, 0x90,   0x8a,              ATK,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    9,           0,   369, 0x90,   0x8a,              ATE,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   370, 0x90,   0x8a,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   393, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl03_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl03_crouch_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   465, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x90,   0x40,          ATK | 2,  0x00, 0x03,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   468, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    8,           0,   468, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   468, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   469, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4,           0,   470, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   471, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_crouch_p_02 ),
};

/* Char_Crouch_P 3 */
const Sint32 pl03_crouch_p_03[] = {
	CG_FRAME(    0,    0,    2,           0,   371, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    4,           0,   374, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x81,    0,    4,           0,   375, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    6,           0,   376, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   377, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   378, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl03_crouch_p_03 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl03_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   379, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x81,    0,    3,           0,   380, 0x90,   0xa8,              ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    8,           0,   381, 0x90,   0xa8,              ATE,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   379, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl03_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl03_crouch_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   382, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    4,           0,   383, 0x90,   0xc6,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    9,           0,   384, 0x90,   0xc6,              ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    5,           0,   385, 0x90,   0x8b,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   382, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl03_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl03_crouch_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   386, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   387, 0x90,   0x76,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0x90,   0x4b,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   389, 0x90,   0xa1,              ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    3,           0,   389, 0x90,   0xa1,              ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    5,           0,   390, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    6,           0,   391, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   392, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   393, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl03_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl03_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   394, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   395, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,    2,           0,   396, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,    2,           0,   395, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x81,    0,    2,           0,   396, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    8,           0,   397, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   394, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl03_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl03_jump_p_01[] = {
	CG_FRAME(    0,    0,    2,           0,   398, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   399, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   400, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   401, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    8,           0,   401, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   409, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl03_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl03_jump_p_02[] = {
	CG_FRAME(    0,    0,    2,           0,   402, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   404, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   405, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    4,           0,   407, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   15,           0,   408, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   405, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl03_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl03_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   409, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   410, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   412, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   412, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   409, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl03_jump_k_00 ),
};

/* Char_Jump_K 1 (medium) */
const Sint32 pl03_jump_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   413, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   414, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    4,           0,   415, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   10,           0,   416, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   413, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl03_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl03_jump_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   417, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   418, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    4,           0,   419, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   15,           0,   420, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   421, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   422, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl03_jump_k_02 ),
};

/* Char_Jump_K 5 */
const Sint32 pl03_jump_k_05[] = {
	CG_FRAME(    0,    0,    4,           0,   546, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   547, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   548, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    1,    4,           0,   549, 0x90,   0x20,          ATK | 1,  0x00, 0x04,    0,   0,     0,      0,  67 ),
};

const Sint32 pl03_jump_k_05_loop[] = {
	CG_FRAME( 0x80,    1,    3,           0,   550, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    1,    3,           0,   551, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    1,    3, END_OF_DATA,   552, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl03_jump_k_05_loop ),
};

/* Char_Jump_K 4 */
const Sint32 pl03_jump_k_04[] = {
	CG_FRAME(    0,    0,    3,           0,   423, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   424, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   425, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   426, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl03_jump_k_04_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   427, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl03_jump_k_04_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl03_damage_00[] = {
	CG_FRAME(    0,    0,    9,           0,   691, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   336, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl03_damage_01[] = {
	CG_FRAME(    0,    0,    2,           0,   691, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   692, 0x80,   0x55,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   691, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   336, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl03_damage_02[] = {
	CG_FRAME(    0,    0,    2,           0,   693, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   694, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   695, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   696, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   697, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   699, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   700, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   701, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   702, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl03_damage_03[] = {
	CG_FRAME(    0,    0,    9,           0,   703, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   336, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl03_damage_04[] = {
	CG_FRAME(    0,    0,    2,           0,   703, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   704, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   703, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   336, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl03_damage_05[] = {
	CG_FRAME(    0,    0,    1,           0,   705, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   706, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   14,           0,   707, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   708, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl03_damage_06[] = {
	CG_FRAME(    0,    0,    9,           0,   710, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   709, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl03_damage_07[] = {
	CG_FRAME(    0,    0,    1,           0,   712, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   13,           0,   711, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   712, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl03_damage_08[] = {
	CG_FRAME(    0,    0,    2,           0,   711, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   713, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   14,           0,   714, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   712, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl03_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl03_damage_09[] = {
	CG_FRAME(    0,    0,    4,           0,   722, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   723, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   724, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   734, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   725, 0x80,   0xad,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl03_damage_09_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl03_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl03_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   715, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl03_damage_11[] = {
	CG_FRAME(    0,    0,    2,           0,   715, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   716, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl03_damage_12[] = {
	CG_FRAME(    0,    0,    3,           0,   717, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   718, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   719, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl03_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   720, 0x80,   0x80,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   721, 0x80,   0x80,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl03_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl03_damage_16[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   726, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl03_damage_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   727, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl03_damage_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   728, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl03_damage_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   729, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl03_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   730, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_20 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl03_damage_26[] = {
	CG_FRAME(    0,    0,    1,           0,   731, 0x80,   0xd2,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   732, 0x80,   0xb2,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   733, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   734, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   735, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   736, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   737, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl03_damage_26 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl03_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   726, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl03_damage_22[] = {
	CG_FRAME(    0,    0,    2,           0,   727, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl03_damage_23[] = {
	CG_FRAME(    0,    0,    2,           0,   728, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl03_damage_24[] = {
	CG_FRAME(    0,    0,    2,           0,   729, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl03_damage_25[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   730, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_25 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl03_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   743, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   744, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   745, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   746, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   747, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   748, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl03_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   749, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   750, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   752, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   753, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   754, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_29 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl03_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   739, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   740, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   741, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   742, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl03_damage_31[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   737, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl03_damage_31 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl03_damage_33[] = {
	CG_FRAME(    0,    0,    2,           0,   732, 0x80,   0xb3,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   738, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   739, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   740, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   741, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   742, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_damage_33 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl03_dizzy_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   708, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl03_catch_00[] = {
	CG_FRAME(    0,    0,    6,           0,   430, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    5,           0,   431, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,    5,           0,   432, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    0,    5,           0,   433, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,    5,           0,   434, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,   16,           0,   435, 0x80,   0x4b,                3,  0x00, 0x05,    0,   0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,    4,           0,   436, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   437, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_catch_00 ),
};

/* Char_Catch 1: vector_set */
const Sint32 pl03_catch_01[] = {
	CG_FRAME(    0,    1,    4,           0,   430, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    1,    4,           0,   431, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    1,    4,           0,   438, 0x80,   0x00,             0x1b,  0x00, 0x1f,    0,   0,     0, 0x0009,   1 ),
};

const Sint32 pl03_catch_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   439, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   441, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000b,   1 ),
	CG_NEXT( pl03_catch_01_loop ),
};

/* Char_Catch 2: vector_catch_fly */
const Sint32 pl03_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   442, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000c,   1 ),
	CG_FRAME(    0,    0,    4,           0,   443, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000d,   1 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x80,   0x20,             0x0f,  0x00, 0x18,    0,   0,     0, 0x000e,   1 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000f,   1 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    0,    4,           0,   447, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0011,   1 ),
	CG_FRAME(    0,    0,    4,           0,   448, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,    4,           0,   449, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0013,   1 ),
};

const Sint32 pl03_catch_02_loop[] = {
	CG_FRAME(    0,    1,    3,           0,   450, 0x80,   0x20,             0x1c,  0x00,    0,    0,   0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    1,    3,           0,   451, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0015,   1 ),
	CG_FRAME(    0,    1,    3,           0,   452, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0016,   1 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   453, 0x80,   0x20,                0,  0x01,    0,    0,   0,     0, 0x0017,   1 ),
	CG_NEXT( pl03_catch_02_loop ),
};

/* Char_Catch 3: vector_catch_fall */
const Sint32 pl03_catch_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   451, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0018,   1 ),
	CG_NEXT( pl03_catch_03 ),
};

/* Char_Catch 4: vector_catch_leave */
const Sint32 pl03_catch_04[] = {
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0018,   3 ),
	CG_FRAME(    0,    0,    4,           0,   138, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   139, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   140, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_catch_04_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   144, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_catch_04_loop ),
};

/* Char_Catch 5 */
const Sint32 pl03_catch_05[] = {
	CG_FRAME(    0,    0,    2,           0,   439, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   441, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x000b,   1 ),
	CG_NEXT( pl03_catch_05 ),
};

/* Char_Catch 6: pls03_01_a_init */
const Sint32 pl03_catch_06[] = {
	CG_FRAME(    0,    0,    4,           0,   892, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   892, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x001a,   0 ),
	CG_NEXT( pl03_catch_06 ),
};

/* Char_Catch 7: nml_k_set */
const Sint32 pl03_catch_07[] = {
	CG_FRAME(    0,    0,    4,           0,   590, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    0,    4,           0,   591, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   592, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,    4,           0,   593, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,    4,           0,   594, 0x80,   0x6d,                1,  0x00, 0x04,    0,   0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    0,    1,           0,   595, 0x80,   0x6d,                0,  0x01,    0,    0,   0,     0, 0x0020,   0 ),
};

const Sint32 pl03_catch_07_loop[] = {
	CG_FRAME(    1,    0,    4,           0,   595, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0, 0x0020,   0 ),
	CG_FRAME(    1,    0,    4, END_OF_DATA,   596, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0, 0x0020,   0 ),
	CG_NEXT( pl03_catch_07_loop ),
};

/* Char_Catch 8: jump_p_set */
const Sint32 pl03_catch_08[] = {
	CG_FRAME(    0,    0,    4,           0,   672, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x014c,   0 ),
	CG_FRAME(    0,    1,    4,           0,   673, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x014d,   0 ),
	CG_FRAME(    0,    0,    4,           0,   674, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x014e,   0 ),
	CG_FRAME(    0,    0,    4,           0,   675, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x014f,   0 ),
	CG_FRAME(    0,    0,    4,           0,   676, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0150,   0 ),
};

const Sint32 pl03_catch_08_loop[] = {
	CG_FRAME(    1,    0,    4,           0,   677, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0151,   0 ),
	CG_FRAME(    1,    0,    4, END_OF_DATA,   678, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x0152,   0 ),
	CG_NEXT( pl03_catch_08_loop ),
};

/* Char_Catch 9: jump_p_catch_drop */
const Sint32 pl03_catch_09[] = {
	CG_FRAME(    0,    0,    1,           0,   679, 0x80,   0x00,                1,  0x00, 0x04,    0,   0,     0, 0x0153,   0 ),
};

const Sint32 pl03_catch_09_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   680, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0153,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   681, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0153,   0 ),
	CG_NEXT( pl03_catch_09_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl03_get_up_00[] = {
	CG_FRAME(    0,    0,    5,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,    5,           0,   224, 0x80,   0x80,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,    5,           0,   225, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    3,    5,           0,   226, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    4,    5,           0,   227, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,    5, END_OF_DATA,   228, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl03_get_up_01[] = {
	CG_FRAME(    0,    5,    3,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   224, 0x80,   0x80,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    3,           0,   225, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    3,           0,   226, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,    3,           0,   227, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,    3, END_OF_DATA,   228, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl03_get_up_02[] = {
	CG_FRAME(    0,    5,    5,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,    5,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,    5,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   12,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   224, 0x80,   0x80,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    3,           0,   225, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    3,           0,   226, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,    3,           0,   227, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,    3, END_OF_DATA,   228, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl03_get_up_03[] = {
	CG_FRAME(    0,    5,    4,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,    4,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,    4,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,    4,           0,   223, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   224, 0x80,   0x80,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    3,           0,   225, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    3,           0,   226, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,    3,           0,   227, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,    3, END_OF_DATA,   228, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl03_appear_00[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   201, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   205, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   16,           0,   205, 0x80,   0x00,             0x0e,  0x00, 0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   206, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_appear_00 ),
};

/* Char_Appear 1: appear_wait */
const Sint32 pl03_appear_01[] = {
	CG_FRAME(    0,    0,   16,           0,   215, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   216, 0x80,   0x00,                6,  0x00, 0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_appear_01_loop[] = {
	CG_FRAME(    1,    0,    2,           0,   219, 0x80,   0x00,                0,  0x02,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,    2, END_OF_DATA,   219, 0x80,   0x00,                0,  0x03,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_appear_01_loop ),
};

/* Char_Appear 2: appear_change */
const Sint32 pl03_appear_02[] = {
	CG_FRAME(    5,    0,    4,           0,   220, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    6,    0,    4,           0,   220, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    7,    0,    4,           0,   220, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    8,    0,    4,           0,   220, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   96,    0,    4,           0,   220, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   221, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   222, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_appear_02 ),
};

/* Char_Appear 3: pls03_03_appear_init */
const Sint32 pl03_appear_03[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   207, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_appear_03 ),
};

/* Char_Appear 4: pls03_03_appear_fall */
const Sint32 pl03_appear_04[] = {
	CG_FRAME(    0,    0,    6,           0,   208, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_appear_04_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   209, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_appear_04_loop ),
};

/* Char_Appear 5: pls03_03_appear_wait */
const Sint32 pl03_appear_05[] = {
	CG_FRAME(    0,    0,   16,           0,   210, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_appear_05_loop[] = {
	CG_FRAME(    1,    0,    2,           0,   214, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,    2, END_OF_DATA,   214, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_appear_05_loop ),
};

/* Char_Extra 5: chouhatsu_init (the taunt) */
const Sint32 pl03_extra_05[] = {
	CG_FRAME(    0,    0,    4,           0,   507, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   508, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   509, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   510, 0x80,   0x00,             0x0c,  0x01, 0x0a,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_extra_05_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   511, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   512, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   513, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   514, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   515, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   516, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   517, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_extra_05_loop ),
};

/* Char_Extra 6: pls03_03_chouhatsu_init */
const Sint32 pl03_extra_06[] = {
	CG_FRAME(    0,    0,    1,           0,    -1, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   518, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   519, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   520, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   521, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   522, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl03_extra_06_loop[] = {
	CG_FRAME(    1,    0,    4,           0,   523, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    4,           0,   524, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    4, END_OF_DATA,   525, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_extra_06_loop ),
};

/* Char_Extra 7: pls03_03_chouhatsu_fall */
const Sint32 pl03_extra_07[] = {
	CG_FRAME(    0,    0,    2,           0,   526, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   527, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   528, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   529, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   530, 0x80,   0x0b,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   531, 0x80,   0x09,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   28, END_OF_DATA,   532, 0x80,   0x09,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_extra_07 ),
};

/* Char_Extra 8: pls03_03_chouhatsu_land */
const Sint32 pl03_extra_08[] = {
	CG_FRAME(    0,    0,    4,           0,   533, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl03_extra_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   534, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   535, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_extra_08_loop ),
};

/* Char_Extra 9: pl03_upper */
const Sint32 pl03_extra_09[] = {
	CG_FRAME(    0,    0,    4,           0,   465, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x90,   0x40,          ATK | 2,  0x00, 0x03,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   468, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    8,           0,   468, 0x90,   0x40,              ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   468, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   469, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4,           0,   470, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   471, 0x90,   0x40,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_extra_09 ),
};

/* Char_Extra 2: nml_k_catch_miss, fly_start5
   Char_Extra 4 */
const Sint32 pl03_extra_02[] = {
	CG_FRAME(    0,    0,   28,           0,   597, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   598, 0x80,   0x6d,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   599, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   600, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_extra_02 ),
};

/* Char_Extra 3: pls03_01_b_init, fly_start5 */
const Sint32 pl03_extra_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   893, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_extra_03 ),
};

/* Char_Special 0
   Char_Special 1 */
const Sint32 pl03_special_00[] = {
	CG_FRAME(    0,    0,    4,           0,   575, 0x90,   0x15,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    6,           0,   576, 0x90,   0x59,       ATK | 0x11,  0x00, 0x14,    0,   0,     0, 0x0100,  49 ),
	CG_FRAME(    0,    0,    4,           0,   577, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  49 ),
	CG_FRAME(    0,    1,    3,           0,   578, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0, 0x0300,  49 ),
	CG_FRAME(    0,    0,    3,           0,   578, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0, 0x0400,  49 ),
	CG_FRAME(    0,    0,    3,           0,   578, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0, 0x0500,  49 ),
	CG_FRAME(    0,    0,    6,           0,   579, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0, 0x0600,  49 ),
	CG_FRAME(    0,    0,    6,           0,   580, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0, 0x0700,  49 ),
	CG_FRAME(    0,    0,    8,           0,   581, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0, 0x0800,  49 ),
	CG_FRAME(    0,    0,    4,           0,   323, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0, 0x0800,  49 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   324, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0800,  49 ),
	CG_NEXT( pl03_special_00 ),
};

/* Char_Special 2
   Char_Special 3 */
const Sint32 pl03_special_02[] = {
	CG_FRAME(    0,    0,    1,           0,   128, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   582, 0x90,   0x20,       ATK | 0x11,  0x00, 0x14,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   583, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   584, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0100,  50 ),
	CG_FRAME(    0,    0,    2,           0,   585, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0200,  50 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   586, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0300,  50 ),
	CG_NEXT( pl03_special_02 ),
};

/* Char_Special 4: jsoul_move (Soul Fist) */
const Sint32 pl03_special_04[] = {
	CG_FRAME(    0,    0,    4,           0,   587, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0400,  50 ),
	CG_FRAME(    0,    0,    4,           0,   588, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   32, END_OF_DATA,   589, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl03_special_04 ),
};

/* Char_Special 5 */
const Sint32 pl03_special_05[] = {
	CG_FRAME(    0,    0,    1,           0,   486, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x90,   0x00,       ATK | 0x10,  0x00, 0x15,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   488, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,   10,           0,   489, 0x90,   0x2a,       ATK | 0x19,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   489, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl03_special_05 ),
};

/* Char_Special 6 */
const Sint32 pl03_special_06[] = {
	CG_FRAME(    0,    0,    1,           0,   486, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x90,   0x00,       ATK | 0x10,  0x00, 0x15,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   488, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,       ATK | 0x19,  0x01,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x01,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x01,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x02,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   489, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   489, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl03_special_06 ),
};

/* Char_Special 7: jblade_init (Shadow Blade, 7 + atck_level) */
const Sint32 pl03_special_07[] = {
	CG_FRAME(    0,    0,    4,           0,   553, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   554, 0x90,   0x20,       ATK | 0x10,  0x00, 0x15,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   555, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,   10,           0,   556, 0x90,   0x2a,       ATK | 0x19,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   556, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   556, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl03_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl03_special_08[] = {
	CG_FRAME(    0,    0,    1,           0,   553, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   554, 0x90,   0x20,       ATK | 0x10,  0x00, 0x15,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   555, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    1,   16,           0,   556, 0x90,   0x2a,       ATK | 0x19,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    4,           0,   556, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   556, 0x90,   0x2a,              ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl03_special_08 ),
};

/* Char_Special 9: blade_move (Shadow Blade), jblade_move (Shadow Blade) */
const Sint32 pl03_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   490, 0x80,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   491, 0x80,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   492, 0x80,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_special_09 ),
};

/* Char_Special 10: blade_fall (Shadow Blade), jblade_fall (Shadow Blade) */
const Sint32 pl03_special_10[] = {
	CG_FRAME(    0,    0,    2,           0,   493, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   495, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   497, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   498, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   499, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_special_10 ),
};

/* Char_Special 11 */
const Sint32 pl03_special_11[] = {
	CG_FRAME(    0,    0,    2,           0, 32797, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2,           0, 32800, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2,           0, 32798, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2,           0, 32801, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32799, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl03_special_11 ),
};

/* Char_Special 12 */
const Sint32 pl03_special_12[] = {
	CG_FRAME(    0,    0,    4,           0, 32802, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32804, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32805, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32806, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl03_special_13[] = {
	CG_FRAME(    0,    0,    2,           0,   844, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   845, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2,           0,   848, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   846, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl03_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl03_special_14[] = {
	CG_FRAME(    0,    0,    4,           0,   849, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   850, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   851, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   852, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   853, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   854, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   855, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_special_14 ),
};

/* Char_Special 15: vector_init (Vector Drain) */
const Sint32 pl03_special_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   430, 0x80,   0x4b,                0,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl03_special_15 ),
};

/* Char_Special 16: vector_move (Vector Drain) */
const Sint32 pl03_special_16[] = {
	CG_FRAME(    0,    0,    4,           0,   536, 0x80,   0x0a,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   537, 0x80,   0x01,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   538, 0x80,   0x01,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   537, 0x80,   0x01,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   538, 0x80,   0x01,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   537, 0x80,   0x01,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   538, 0x80,   0x01,                0,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl03_special_16 ),
};

/* Char_Special 17: snapback_init (the snapback) */
const Sint32 pl03_special_17[] = {
	CG_FRAME(    0,    0,    2,           0,   337, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   338, 0x90,   0x46,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   340, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   341, 0x90,   0x43,              ATK,  0x01,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x81,    0,    4,           0,   342, 0x90,   0x43,              ATK,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   14,           0,   342, 0x90,   0x43,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    4,           0,   343, 0x90,   0x46,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   344, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_special_17 ),
};

/* Char_Super 0 */
const Sint32 pl03_super_00[] = {
	CG_FRAME(    0,    0,    6,           0,   502, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   503, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   504, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   505, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   505, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_00 ),
};

/* Char_Super 1 */
const Sint32 pl03_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   505, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   64,           0,   505, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   505, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl03_super_02[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   506, 0x90,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl03_super_03[] = {
	CG_FRAME(    0,    0,    1,           0,   900, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   901, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   902, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   894, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   895, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   896, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   897, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   898, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   899, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl03_super_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   818, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   819, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   820, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   821, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl03_super_03_loop ),
};

/* Char_Super 4 */
const Sint32 pl03_super_04[] = {
	CG_FRAME(    0,    0,    4,           0,   857, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   859, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   860, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   861, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   862, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_04 ),
};

/* Char_Super 5: illusion_init (Darkness Illusion (hyper)) */
const Sint32 pl03_super_05[] = {
	CG_FRAME(    0,    0,    6,           0,   122, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   127, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   13, END_OF_DATA,   128, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_super_05 ),
};

/* Char_Super 6: illusion_wait (Darkness Illusion (hyper)) */
const Sint32 pl03_super_06[] = {
	CG_FRAME(    0,    0,    1,           0,   128, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   128, 0x80,   0x20,       ATK | 0x0a,  0x00, 0x19,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x90,   0x20,       ATK | 0x1a,  0x00, 0x1f,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl03_super_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   500, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   501, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl03_super_06_loop ),
};

/* Char_Super 7: jillusion_wait (Darkness Illusion (hyper)) */
const Sint32 pl03_super_07[] = {
	CG_FRAME(    0,    0,    1,           0,   128, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   128, 0x80,   0x20,       ATK | 0x0a,  0x00, 0x19,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x90,   0x20,       ATK | 0x1a,  0x00, 0x1f,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl03_super_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   500, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   501, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl03_super_07_loop ),
};

/* Char_Super 8: shl03_03_illusion_init */
const Sint32 pl03_super_08[] = {
	CG_FRAME(    0,    0,    1,           0,   557, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   558, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   559, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   560, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   561, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   562, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   563, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   564, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   565, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   566, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   567, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   568, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   569, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   570, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   571, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   572, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   573, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   574, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_08 ),
};

/* Char_Super 9: shl03_03_jillusion_init */
const Sint32 pl03_super_09[] = {
	CG_FRAME(    0,    0,    1,           0,   557, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   558, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   559, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   560, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   561, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   562, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   563, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   564, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   565, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   566, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   567, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   568, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   569, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   570, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   571, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_09 ),
};

/* Char_Catch 10
   Char_Super 15: illusion_rush (Darkness Illusion (hyper)) */
const Sint32 pl03_catch_10[] = {
	CG_FRAME(    0,    0,    4,           0,   319, 0x80,   0x4b,              ATK,  0x00,    0,    0,   0,     0, 0x0038,   1 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x4b,              ATK,  0x00,    0,    0,   0,     0, 0x0038,   1 ),
};

const Sint32 pl03_catch_10_loop[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   321, 0x80,   0x4b,              ATK,  0x00,    0,    0,   0,     0, 0x0038,   1 ),
	CG_NEXT( pl03_catch_10_loop ),
};

/* Char_Super 10: illusion_rush (Darkness Illusion (hyper)) */
const Sint32 pl03_super_10[] = {
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   132, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_10 ),
};

/* Char_Super 11: illusion_rush (Darkness Illusion (hyper)), jillusion_rush (Darkness Illusion (hyper)) */
const Sint32 pl03_super_11[] = {
	CG_FRAME(    0,    0,    7,           0,   138, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   139, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   140, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   141, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,   142, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   143, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_super_11_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   144, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_super_11_loop ),
};

/* Char_Super 12: illusion_bound (Darkness Illusion (hyper)), jillusion_bound (Darkness Illusion (hyper)) */
const Sint32 pl03_super_12[] = {
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_12 ),
};

/* Char_Super 13: illusion_hits (Darkness Illusion (hyper)), shl03_03_illusion_hit */
const Sint32 pl03_super_13[] = {
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl03_super_13_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   129, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_super_13_loop ),
};

/* Char_Catch 11
   Char_Super 16: illusion_hold (Darkness Illusion (hyper)), shl03_03_illusion_appear */
const Sint32 pl03_catch_11[] = {
	CG_FRAME(    0,    0,    2,           0,   325, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0038,   5 ),
	CG_FRAME(    0,   60,    2,           0,   326, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0, 0x0038,  60 ),
	CG_FRAME(    0,    0,    4,           0,   326, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  60 ),
	CG_FRAME(    0,    0,    1,           0,   327, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   9 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   5 ),
	CG_FRAME(    0,   61,    2,           0,   346, 0x90,   0x00,              ATK,  0x02,    0,    0,   0,     0, 0x003a,  61 ),
	CG_FRAME(    0,    0,    4,           0,   346, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  61 ),
	CG_FRAME(    0,    0,    1,           0,   347, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  15 ),
	CG_FRAME(    0,    0,    2,           0,   325, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   5 ),
	CG_FRAME(    0,   60,    2,           0,   326, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0, 0x0039,  60 ),
	CG_FRAME(    0,    0,    4,           0,   326, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  60 ),
	CG_FRAME(    0,    0,    1,           0,   327, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   9 ),
	CG_FRAME(    0,    0,    1,           0,   328, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  44 ),
	CG_FRAME(    0,    0,    1,           0,   329, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  44 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  44 ),
	CG_FRAME(    0,   61,    2,           0,   331, 0x90,   0x47,              ATK,  0x02,    0,    0,   0,     0, 0x003a,  61 ),
	CG_FRAME(    0,    0,    6,           0,   331, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  61 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  61 ),
	CG_FRAME(    0,    0,    1,           0,   335, 0x90,   0x47,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  44 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   5 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   5 ),
	CG_FRAME(    0,    0,    1,           0,   350, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   5 ),
	CG_FRAME(    0,   60,    2,           0,   351, 0x90,   0x00,              ATK,  0x02,    0,    0,   0,     0, 0x0039,  60 ),
	CG_FRAME(    0,    0,    6,           0,   351, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  60 ),
	CG_FRAME(    0,    0,    2,           0,   352, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  17 ),
	CG_FRAME(    0,    0,    2,           0,   454, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    4,           0,   455, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,   60,    2,           0,   456, 0x90,   0x00,              ATK,  0x03,    0,    0,   0,     0, 0x0039,  60 ),
	CG_FRAME(    0,    0,    4,           0,   456, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  60 ),
	CG_FRAME(    0,   61,    2,           0,   457, 0x90,   0x00,              ATK,  0x03,    0,    0,   0,     0, 0x003a,  61 ),
	CG_FRAME(    0,    0,    4,           0,   457, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  61 ),
	CG_FRAME(    0,   60,    2,           0,   458, 0x90,   0x00,              ATK,  0x03,    0,    0,   0,     0, 0x0039,  60 ),
	CG_FRAME(    0,    0,    4,           0,   458, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,  60 ),
	CG_FRAME(    0,   61,    2,           0,   459, 0x90,   0x00,              ATK,  0x03,    0,    0,   0,     0, 0x003a,  61 ),
	CG_FRAME(    0,    0,    4,           0,   459, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  61 ),
	CG_FRAME(    0,    0,    4,           0,   460, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    2,           0,   461, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    1,           0,   462, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    1,           0,   463, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    2,           0,   464, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x0039,  60 ),
	CG_FRAME(    0,   62,    2,           0,   140, 0x80,   0x00,              ATK,  0x04,    0,    0,   0,     0, 0x0039,  60 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   3 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   3 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   3 ),
};

const Sint32 pl03_catch_11_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   143, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0, 0x003a,   3 ),
	CG_NEXT( pl03_catch_11_loop ),
};

/* Char_Catch 12
   Char_Super 17: illusion_catch_up (Darkness Illusion (hyper)), shl03_03_illusion_hold */
const Sint32 pl03_catch_12[] = {
	CG_FRAME(    0,    0,    2,           0,   125, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   3 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   3 ),
	CG_FRAME(    0,   60,    2,           0,   410, 0x90,   0x20,              ATK,  0x01,    0,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  60 ),
	CG_FRAME(    0,    0,    1,           0,   411, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  60 ),
	CG_FRAME(    0,    0,    1,           0,   412, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  39 ),
	CG_FRAME(    0,   61,    2,           0,   412, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x003c,   3 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   7 ),
	CG_FRAME(    0,    0,    2,           0,   395, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,    0,    1,           0,   396, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,    0,    2,           0,   397, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,  33 ),
	CG_FRAME(    0,    0,    2,           0,   413, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   7 ),
	CG_FRAME(    0,   60,    1,           0,   414, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x003b,   7 ),
	CG_FRAME(    0,    0,    4,           0,   415, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  60 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  41 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   7 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   7 ),
	CG_FRAME(    0,   61,    2,           0,   400, 0x90,   0x20,              ATK,  0x01,    0,    0,   0,     0, 0x003c,  60 ),
	CG_FRAME(    0,    0,    4,           0,   400, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,  35 ),
	CG_FRAME(    0,    0,    2,           0,   417, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   7 ),
	CG_FRAME(    0,    0,    1,           0,   418, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   7 ),
	CG_FRAME(    0,   60,    2,           0,   419, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,    0,    6,           0,   419, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  60 ),
	CG_FRAME(    0,    0,    2,           0,   420, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,  43 ),
	CG_FRAME(    0,    0,    1,           0,   421, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   7 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   7 ),
	CG_FRAME(    0,    0,    1,           0,   539, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   3 ),
	CG_FRAME(    0,    0,    1,           0,   539, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   3 ),
	CG_FRAME(    0,    0,    2,           0,   540, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   0 ),
	CG_FRAME(    0,    0,    4,           0,   541, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003c,   3 ),
	CG_FRAME(    0,    0,    6,           0,   542, 0x80,   0x20,          ATK | 1,  0x00, 0x04,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,   63,    4, END_OF_DATA,   542, 0x80,   0x20,              ATK,  0x04,    0,    0,   0,     0, 0x003b,  60 ),
	CG_NEXT( pl03_catch_12 ),
};

/* Char_Super 18: illusion_air_hits (Darkness Illusion (hyper)), shl03_03_illusion_drop */
const Sint32 pl03_super_18[] = {
	CG_FRAME(    0,    0,    4,           0,   542, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,  60 ),
	CG_FRAME(    0,    0,    4,           0,   543, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   3 ),
	CG_FRAME(    0,    0,    4,           0,   544, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   3 ),
};

const Sint32 pl03_super_18_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   545, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003b,   3 ),
	CG_NEXT( pl03_super_18_loop ),
};

/* Char_Super 19: illusion_finish (Darkness Illusion (hyper)), jillusion_finish (Darkness Illusion (hyper)), shl03_03_illusion_land, shl03_03_jillusion_land */
const Sint32 pl03_super_19[] = {
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_super_19 ),
};

/* Char_Super 20: illusion_fall (Darkness Illusion (hyper)), jillusion_fall (Darkness Illusion (hyper)) */
const Sint32 pl03_super_20[] = {
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_20 ),
};

/* Char_Super 14: jillusion_init (Darkness Illusion (hyper)) */
const Sint32 pl03_super_14[] = {
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   13, END_OF_DATA,   128, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl03_super_14 ),
};

/* Char_Catch 13
   Char_Super 21: jillusion_rush (Darkness Illusion (hyper)) */
const Sint32 pl03_catch_13[] = {
	CG_FRAME(    0,    0,   68, END_OF_DATA,   128, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x004b,   3 ),
	CG_NEXT( pl03_catch_13 ),
};

/* Char_Catch 14
   Char_Super 22: jillusion_hold (Darkness Illusion (hyper)), shl03_03_jillusion_appear */
const Sint32 pl03_catch_14[] = {
	CG_FRAME(    0,    0,    2,           0,   413, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x004a,   7 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x004a,   7 ),
	CG_FRAME(    0,   64,    2,           0,   415, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x004a,  60 ),
	CG_FRAME(    0,    0,    4,           0,   415, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    1,           0,   416, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  41 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,    0,    1,           0,   399, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,   65,    2,           0,   400, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    4,           0,   400, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  35 ),
	CG_FRAME(    0,    0,    2,           0,   125, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   3 ),
	CG_FRAME(    0,    0,    1,           0,   409, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   3 ),
	CG_FRAME(    0,   64,    2,           0,   410, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    4,           0,   410, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    1,           0,   411, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    1,           0,   412, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  39 ),
	CG_FRAME(    0,    0,    2,           0,   394, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,   65,    2,           0,   395, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    4,           0,   395, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    1,           0,   396, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    1,           0,   397, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  33 ),
	CG_FRAME(    0,    0,    2,           0,   413, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   7 ),
	CG_FRAME(    0,    0,    1,           0,   414, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   7 ),
	CG_FRAME(    0,   64,    2,           0,   415, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    4,           0,   415, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  41 ),
	CG_FRAME(    0,    0,    1,           0,   402, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,    0,    1,           0,   403, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,    0,    1,           0,   404, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,    0,    2,           0,   405, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   7 ),
	CG_FRAME(    0,   65,    2,           0,   407, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003e,  36 ),
	CG_FRAME(    0,    0,    6,           0,   407, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  36 ),
	CG_FRAME(    0,    0,    2,           0,   454, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   3 ),
	CG_FRAME(    0,    0,    2,           0,   455, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   3 ),
	CG_FRAME(    0,   64,    1,           0,   456, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    3,           0,   456, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,   65,    1,           0,   457, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003e,  61 ),
	CG_FRAME(    0,    0,    3,           0,   457, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  61 ),
	CG_FRAME(    0,   64,    1,           0,   458, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,    0,    3,           0,   458, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,   65,    1,           0,   459, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x003e,  61 ),
	CG_FRAME(    0,    0,    3,           0,   459, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  61 ),
	CG_FRAME(    0,    0,    2,           0,   460, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,   3 ),
	CG_FRAME(    0,    0,    1,           0,   139, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003d,  60 ),
	CG_FRAME(    0,   66,    3,           0,   139, 0x80,   0x20,              ATK,  0x04,    0,    0,   0,     0, 0x003e,  60 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    2,           0,   144, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    1,           0,   125, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    1,           0,   126, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   127, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0, 0x003e,   3 ),
	CG_NEXT( pl03_catch_14 ),
};

/* Char_Catch 15
   Char_Super 23: jillusion_hits (Darkness Illusion (hyper)), shl03_03_jillusion_hit */
const Sint32 pl03_catch_15[] = {
	CG_FRAME(    1,    0,   24,           0,   423, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003f,   7 ),
	CG_FRAME(    1,    0,    5,           0,   424, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003f,   7 ),
	CG_FRAME(    1,    0,    5,           0,   425, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003f,   7 ),
	CG_FRAME(    1,    0,    4,           0,   426, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003f,   7 ),
	CG_FRAME(    1,    0,    4,           0,   427, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x003f,  62 ),
	CG_FRAME(    0,   67,    2,           0,   428, 0x90,   0x20,              ATK,  0x03,    0,    0,   0,     0, 0x013f,  62 ),
	CG_FRAME(    1,    0,    1,           0,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0040,  62 ),
	CG_FRAME(    1,    0,    2,           0,   427, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0040,  62 ),
	CG_FRAME(    1,    0,    1,           0,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0041,  62 ),
	CG_FRAME(    1,    0,    2,           0,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0041,  62 ),
	CG_FRAME(    1,   67,    1,           0,   427, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x0042,  62 ),
	CG_FRAME(    1,    0,    2,           0,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0042,  62 ),
	CG_FRAME(    1,    0,    1,           0,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0043,  62 ),
	CG_FRAME(    1,    0,    2,           0,   427, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0043,  62 ),
	CG_FRAME(    1,    0,    1,           0,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0044,  62 ),
	CG_FRAME(    1,    0,    2,           0,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0044,  62 ),
	CG_FRAME(    1,   67,    1,           0,   427, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x0045,  62 ),
	CG_FRAME(    1,    0,    2,           0,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0045,  62 ),
	CG_FRAME(    1,    0,    1,           0,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0046,  62 ),
	CG_FRAME(    1,    0,    2,           0,   427, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0046,  62 ),
	CG_FRAME(    1,    0,    1,           0,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0047,  62 ),
	CG_FRAME(    1,    0,    2,           0,   429, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0047,  62 ),
	CG_FRAME(    1,   67,    1,           0,   427, 0x90,   0x20,              ATK,  0x02,    0,    0,   0,     0, 0x0048,  62 ),
};

const Sint32 pl03_catch_15_loop[] = {
	CG_FRAME(    1,    0,    2, END_OF_DATA,   428, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0048,  62 ),
	CG_NEXT( pl03_catch_15_loop ),
};

/* Char_Catch 16
   Char_Super 24: jillusion_turn (Darkness Illusion (hyper)), shl03_03_jillusion_turn */
const Sint32 pl03_catch_16[] = {
	CG_FRAME(    0,    0,    4,           0,   423, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   7 ),
	CG_FRAME(    0,    0,    4,           0,   539, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   3 ),
	CG_FRAME(    0,    0,    4,           0,   540, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   3 ),
	CG_FRAME(    0,    0,    4,           0,   541, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   3 ),
	CG_FRAME(    0,    0,    4,           0,   542, 0x80,   0x20,          ATK | 1,  0x00, 0x04,    0,   0,     0, 0x0049,   3 ),
	CG_FRAME(    0,   68,    4,           0,   543, 0x80,   0x20,              ATK,  0x04,    0,    0,   0,     0, 0x0049,  60 ),
	CG_FRAME(    0,    0,    4,           0,   543, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   3 ),
	CG_FRAME(    0,    0,    4,           0,   544, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   545, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0, 0x0049,   3 ),
	CG_NEXT( pl03_catch_16 ),
};

/* Char_Super 25: shower_init (Finishing Shower (hyper)) */
const Sint32 pl03_super_25[] = {
	CG_FRAME(    2,    0,    4,           0,   473, 0x90,   0x15,              ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    2,    0,    4,           0,   474, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    2,    0,    4,           0,   475, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    2,    0,   20, END_OF_DATA,   476, 0x90,   0x59,       ATK | 0x0d,  0x00, 0x1a,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl03_super_25 ),
};

/* Char_Super 26: shower_wait (Finishing Shower (hyper)) */
const Sint32 pl03_super_26[] = {
	CG_FRAME(    2, 0x80,    8,           0,   476, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    1, 0x80,    8,           0,   476, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0, 0x80,    4,           0,   476, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0, 0x80,    4,           0, 32768, 0x90,   0x59,              ATK,  0x01,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1, 0x80,    4,           0, 32768, 0x90,   0x59,              ATK,  0x01,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    2, 0x80,    4,           0, 32774, 0x90,   0x59,              ATK,  0x01,    0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl03_super_26_loop[] = {
	CG_FRAME(    0, 0x80,    4, END_OF_DATA, 32774, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl03_super_26_loop ),
};

/* Char_Super 27: shower_pose (Finishing Shower (hyper)) */
const Sint32 pl03_super_27[] = {
	CG_FRAME(    0,    1,    4,           0, 32774, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32768, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32768, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32769, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl03_super_27_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32776, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32769, 0x90,   0x59,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl03_super_27_loop ),
};

/* Char_Super 28: shower_shot (Finishing Shower (hyper)) */
const Sint32 pl03_super_28[] = {
	CG_FRAME(    0,    0,    4,           0, 32769, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   16,           0, 32768, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   12,           0,   476, 0x90,   0x59,              ATE,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   323, 0x90,   0x17,              ATE,  0x00,    0,    0,   0,     0, 0x0800,  49 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   324, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0800,  49 ),
	CG_NEXT( pl03_super_28 ),
};

/* Char_Super 29 */
const Sint32 pl03_super_29[] = {
	CG_FRAME(    1,    1,    2,           0, 32818, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,    2,           0, 32818, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    3,    3,           0, 32824, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,    3,           0, 32818, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    3,    3,           0, 32824, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl03_super_29_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32815, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,    3,           0, 32818, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    2,    3,           0, 32821, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    3,    3,           0, 32824, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    3,           0, 32815, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,    3,           0, 32818, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    2,    3,           0, 32821, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    3,    3, END_OF_DATA, 32824, 0x90,   0x00,              ATK,  0x01,    0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl03_super_29_loop ),
};

/* Char_Super 30 */
const Sint32 pl03_super_30[] = {
	CG_FRAME(    0,    0,    3,           0, 32816, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,    3,           0, 32819, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    2,    3,           0, 32822, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    3,    3, END_OF_DATA, 32825, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl03_super_30 ),
};

/* Char_Super 31 */
const Sint32 pl03_super_31[] = {
	CG_FRAME(    0,    0,    3,           0, 32817, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    1,    3,           0, 32820, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    2,    3,           0, 32823, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    3,    3, END_OF_DATA, 32826, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl03_super_31 ),
};

/* Char_Super 32 */
const Sint32 pl03_super_32[] = {
	CG_FRAME(    0,    0,    6,           0, 32827, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0, 32830, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    6,           0, 32833, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    6, END_OF_DATA, 32836, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_32 ),
};

/* Char_Super 33 */
const Sint32 pl03_super_33[] = {
	CG_FRAME(    0,    0,    6,           0, 32828, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0, 32831, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    6,           0, 32834, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    6, END_OF_DATA, 32837, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_33 ),
};

/* Char_Super 34 */
const Sint32 pl03_super_34[] = {
	CG_FRAME(    0,    0,    6,           0, 32829, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0, 32832, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    6,           0, 32835, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    6, END_OF_DATA, 32838, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_34 ),
};

/* Char_Super 35 */
const Sint32 pl03_super_35[] = {
	CG_FRAME(    0,    0,    2,           0, 32827, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    0,    1,           0, 32827, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    0,    3,           0, 32827, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    1,    3,           0, 32830, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    0,    3,           0, 32827, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    2,    3,           0, 32833, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl03_super_35_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32827, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    1,    3,           0, 32830, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    2,    3,           0, 32833, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    0,    3,           0, 32827, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    1,    3,           0, 32830, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    1,    2,    3, END_OF_DATA, 32833, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl03_super_35_loop ),
};

/* Char_Super 36 */
const Sint32 pl03_super_36[] = {
	CG_FRAME(    0,    0,    3,           0, 32828, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    1,    3,           0, 32831, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    2,    3, END_OF_DATA, 32834, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl03_super_36 ),
};

/* Char_Super 37 */
const Sint32 pl03_super_37[] = {
	CG_FRAME(    0,    0,    3,           0, 32829, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    1,    3,           0, 32832, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    2,    3, END_OF_DATA, 32835, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl03_super_37 ),
};

/* Char_Super 38 */
const Sint32 pl03_super_38[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA, 32836, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_38 ),
};

/* Char_Super 39 */
const Sint32 pl03_super_39[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA, 32837, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_39 ),
};

/* Char_Super 40 */
const Sint32 pl03_super_40[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA, 32838, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_40 ),
};

/* Char_Super 41: silhouette_init (Silhouette Blade (hyper)) */
const Sint32 pl03_super_41[] = {
	CG_FRAME(    0,    0,    1,           0,   601, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   602, 0x90,   0x4e,       ATK | 0x0e,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   603, 0x90,   0x4e,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   604, 0x90,   0x4e,              ATK,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    1,    1,           0,   604, 0x90,   0x4e,              ATK,  0x00,    0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    1,    0,    4,           0,   605, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   607, 0x90,   0x00,          ATK | 1,  0x00, 0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   608, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   24,           0,   609, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   610, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   611, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_super_41 ),
};

/* Char_Super 42: silhouette_move (Silhouette Blade (hyper)) */
const Sint32 pl03_super_42[] = {
	CG_FRAME(    0,    0,    2,           0,   649, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0037,   1 ),
	CG_FRAME(    0,    0,   44, END_OF_DATA,   648, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0, 0x0037,   1 ),
	CG_NEXT( pl03_super_42 ),
};

/* Char_Super 43: silhouette_keep (Silhouette Blade (hyper)) */
const Sint32 pl03_super_43[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   336, 0x90,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl03_super_43 ),
};

/* Char_Super 44 */
const Sint32 pl03_super_44[] = {
	CG_FRAME(    0,    0,    6,           0, 32777, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32778, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32779, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    1,           0, 32780, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    1,           0, 32781, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32782, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl03_super_44 ),
};

/* Char_Super 45: shl03_02_start */
const Sint32 pl03_super_45[] = {
	CG_FRAME(    0,    0,    4,           0, 32783, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32785, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl03_super_45 ),
};

/* Char_Super 46: shl03_02_rise */
const Sint32 pl03_super_46[] = {
	CG_FRAME(    0,    0,    3,           0, 32786, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32787, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32788, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA, 32788, 0x90,   0x20,              ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_super_46 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl03_extra_00[] = {
	CG_FRAME(    0,    0,    3,           0,   419, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   420, 0x90,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl03_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl03_extra_01[] = {
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   20,           0,   302, 0x80,   0x00,                5,  0x00, 0x19,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   303, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_extra_01 ),
};

/* Char_Win 0 */
const Sint32 pl03_win_00[] = {
	CG_FRAME(    0,    1,   32,           0,   186, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   187, 0x80,   0x00,             0x18,  0x01, 0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   188, 0x80,   0x00,                0,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl03_win_01[] = {
	CG_FRAME(    0,    0,   31,           0,   171, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   171, 0x80,   0x00,                9,  0x00, 0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_01_loop[] = {
	CG_FRAME(    0,    0,    5,           0,   172, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   173, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   174, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   175, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   176, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   177, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   178, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   179, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   180, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   181, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   182, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   183, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   184, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   185, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl03_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,             0x0b,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   256, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_02_loop ),
};

/* Char_Win 3 */
const Sint32 pl03_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   259, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   263, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,             0x16,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   269, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   270, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   271, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_03_loop ),
};

/* Char_Win 4 */
const Sint32 pl03_win_04[] = {
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   278, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   280, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   281, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,             0x17,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   283, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   284, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   285, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   286, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_04_loop ),
};

/* Char_Win 5 */
const Sint32 pl03_win_05[] = {
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   287, 0x80,   0x00,                0,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_05_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   287, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   288, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   289, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   290, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_05_loop ),
};

/* Char_Win 6: pls03_03_win_init */
const Sint32 pl03_win_06[] = {
	CG_FRAME(    0,    1,   32,           0,   292, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl03_win_06_loop[] = {
	CG_FRAME(    0,    0,   16,           0,   292, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   24,           0,   291, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   292, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   24, END_OF_DATA,   293, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_win_06_loop ),
};

/* Char_Win 7: win_time */
const Sint32 pl03_win_07[] = {
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   191, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_07_loop ),
};

/* Char_Win 8: lose_time_ko */
const Sint32 pl03_win_08[] = {
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl03_win_08_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   193, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_win_08_loop ),
};

/* Char_Effect 0
   Char_Change 0: act_change_tag_init */
const Sint32 pl03_effect_00[] = {
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x0f,                4,  0x00, 0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x0e,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   297, 0x80,   0x0b,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   298, 0x80,   0x0b,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   299, 0x80,   0x0b,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   300, 0x80,   0x0b,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl03_effect_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   301, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_effect_00_loop ),
};

/* Char_Effect 1
   Char_Change 1: act_change_vcounter_init */
const Sint32 pl03_effect_01[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   302, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_effect_01 ),
};

/* Char_Effect 3
   Char_Change 3: act_stand, act_change_call */
const Sint32 pl03_effect_03[] = {
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   305, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   306, 0x80,   0x00,             0x0c,  0x00, 0x1a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   308, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   312, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   313, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   317, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   318, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_effect_03 ),
};

/* Char_Effect 4
   Char_Change 4: act_change_assist_end */
const Sint32 pl03_effect_04[] = {
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   305, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   306, 0x80,   0x00,                0,  0x00, 0x1a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   307, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   308, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   309, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   310, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   311, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   312, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   314, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   315, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   316, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   317, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   318, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl03_effect_04 ),
};

/* Char_Effect 2
   Char_Change 2: act_change_out_bound */
const Sint32 pl03_effect_02[] = {
	CG_FRAME(    0,    0,   32, END_OF_DATA,   138, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl03_effect_02 ),
};

/* Char_Caught 0 */
const Sint32 pl03_caught_00[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   691, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl03_caught_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   692, 0x80,   0x55,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl03_caught_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   693, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl03_caught_03[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   703, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl03_caught_04[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   704, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl03_caught_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   707, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl03_caught_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   709, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl03_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl03_caught_07[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   711, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl03_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl03_caught_08[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   713, 0x80,   0x40,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl03_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl03_caught_09[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   715, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl03_caught_10[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   716, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl03_caught_11[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   719, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl03_caught_12[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   734, 0x80,   0x60,                0,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl03_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl03_caught_13[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   726, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl03_caught_14[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   727, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl03_caught_15[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   728, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl03_caught_16[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   729, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl03_caught_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   730, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl03_caught_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   743, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl03_caught_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   749, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl03_caught_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   736, 0x80,   0xa0,                0,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl03_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl03_caught_21[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   723, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl03_caught_21 ),
};

const Sint32 pl03_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    1, END_OF_DATA,    71, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   1 ),
	0x00006988,
};

/* group 27 0 */
const Sint32 pl03_grp27_00[] = {
	EF_FRAME(    0,    0,    4,           0,   755 ),
	EF_FRAME(    0,    0,    4,           0,   756 ),
	EF_FRAME(    0,    0,    4,           0,   757 ),
	EF_FRAME(    0,    0,    4,           0,   758 ),
	EF_FRAME(    0,    0,    4,           0,   759 ),
	EF_FRAME(    0,    0,    4,           0,   760 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   761 ),
	CG_NEXT( pl03_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl03_grp27_01[] = {
	EF_FRAME(    0,    0,    4,           0,   803 ),
	EF_FRAME(    0,    0,    4,           0,   804 ),
	EF_FRAME(    0,    0,    4,           0,   805 ),
	EF_FRAME(    0,    0,    4,           0,   806 ),
	EF_FRAME(    0,    0,    4,           0,   807 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   808 ),
	CG_NEXT( pl03_grp27_01 ),
};

/* group 27 2: pls03_04_a_init, pls03_04_b_init */
const Sint32 pl03_grp27_02[] = {
	EF_FRAME(    0,    0,    4,           0,   762 ),
	EF_FRAME(    0,    0,    4,           0,   763 ),
	EF_FRAME(    0,    0,    4,           0,   764 ),
	EF_FRAME(    0,    0,    4,           0,   765 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   766 ),
	CG_NEXT( pl03_grp27_02 ),
};

/* group 27 3: pls03_04_c_init, pls03_04_d_init */
const Sint32 pl03_grp27_03[] = {
	EF_FRAME(    0,    0,    4,           0,   887 ),
	EF_FRAME(    0,    0,    4,           0,   888 ),
	EF_FRAME(    0,    0,    4,           0,   889 ),
	EF_FRAME(    0,    0,    4,           0,   890 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   891 ),
	CG_NEXT( pl03_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl03_grp27_04[] = {
	EF_FRAME(    0,    0,    8,           0,   771 ),
	EF_FRAME(    0,    0,    8,           0,   772 ),
	EF_FRAME(    0,    0,    8,           0,   773 ),
	EF_FRAME(    0,    0,    8,           0,   774 ),
	EF_FRAME(    0,    0,    8,           0,   775 ),
	EF_FRAME(    0,    0,    8,           0,   776 ),
	EF_FRAME(    0,    0,    8,           0,   777 ),
	EF_FRAME(    0,    0,    8, END_OF_DATA,   778 ),
	CG_NEXT( pl03_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl03_grp27_05[] = {
	EF_FRAME(    0,    0,    8,           0,   779 ),
	EF_FRAME(    0,    0,    8,           0,   780 ),
	EF_FRAME(    0,    0,    8,           0,   781 ),
	EF_FRAME(    0,    0,    8,           0,   782 ),
	EF_FRAME(    0,    0,    8,           0,   783 ),
	EF_FRAME(    0,    0,    8,           0,   784 ),
	EF_FRAME(    0,    0,    8,           0,   785 ),
	EF_FRAME(    0,    0,    8, END_OF_DATA,   786 ),
	CG_NEXT( pl03_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl03_grp27_06[] = {
	EF_FRAME(    0,    0,    8,           0,   787 ),
	EF_FRAME(    0,    0,    8,           0,   788 ),
	EF_FRAME(    0,    0,    8,           0,   789 ),
	EF_FRAME(    0,    0,    8,           0,   790 ),
	EF_FRAME(    0,    0,    8,           0,   791 ),
	EF_FRAME(    0,    0,    8,           0,   792 ),
	EF_FRAME(    0,    0,    8,           0,   793 ),
	EF_FRAME(    0,    0,    8, END_OF_DATA,   794 ),
	CG_NEXT( pl03_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl03_grp27_07[] = {
	EF_FRAME(    0,    0,    8,           0,   795 ),
	EF_FRAME(    0,    0,    8,           0,   796 ),
	EF_FRAME(    0,    0,    8,           0,   797 ),
	EF_FRAME(    0,    0,    8,           0,   798 ),
	EF_FRAME(    0,    0,    8,           0,   799 ),
	EF_FRAME(    0,    0,    8,           0,   800 ),
	EF_FRAME(    0,    0,    8,           0,   801 ),
	EF_FRAME(    0,    0,    8, END_OF_DATA,   802 ),
	CG_NEXT( pl03_grp27_07 ),
};

/* group 27 8: pls03_02_init, pls03_02_walk_start, pls03_02_stop_start, pls03_02_appear_start, ... */
const Sint32 pl03_grp27_08[] = {
	EF_FRAME(    0,    0,    4,           0, 32789 ),
	EF_FRAME(    0,    0,    4,           0, 32790 ),
	EF_FRAME(    0,    0,    4,           0, 32791 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32792 ),
	CG_NEXT( pl03_grp27_08 ),
};

/* group 27 9 */
const Sint32 pl03_grp27_09[] = {
	EF_FRAME(    0,    0,    4,           0, 32793 ),
	EF_FRAME(    0,    0,    4,           0, 32794 ),
	EF_FRAME(    0,    0,    4,           0, 32795 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32796 ),
	CG_NEXT( pl03_grp27_09 ),
};

/* group 27 10 */
const Sint32 pl03_grp27_10[] = {
	EF_FRAME(    2,    0,   20,           0,    -1 ),
	EF_FRAME(    1,    0,    3,           0, 32775 ),
	EF_FRAME(    0,    1,    8,           0, 32775 ),
};

const Sint32 pl03_grp27_10_loop[] = {
	EF_FRAME(    0,    0,   16, END_OF_DATA, 32775 ),
	CG_NEXT( pl03_grp27_10_loop ),
};

/* group 27 11: pls03_02_soul_start (11 + sub_type), pls03_02_soul_move (11 + sub_type) */
const Sint32 pl03_grp27_11[] = {
	EF_FRAME(    0,    0,    4,           0,   822 ),
	EF_FRAME(    0,    0,    4,           0,   824 ),
	EF_FRAME(    0,    0,    4,           0,   816 ),
	EF_FRAME(    0,    0,    4,           0,   856 ),
	EF_FRAME(    0,    0,    4,           0, 32807 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4,           0, 32809 ),
	EF_FRAME(    0,    0,    4,           0, 32810 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_11 ),
};

/* group 27 12 */
const Sint32 pl03_grp27_12[] = {
	EF_FRAME(    0,    0,    4,           0,   824 ),
	EF_FRAME(    0,    0,    4,           0,   856 ),
	EF_FRAME(    0,    0,    4,           0,   825 ),
	EF_FRAME(    0,    0,    4,           0, 32814 ),
	EF_FRAME(    0,    0,    4,           0, 32813 ),
	EF_FRAME(    0,    0,    4,           0, 32814 ),
	EF_FRAME(    0,    0,    4,           0, 32813 ),
	EF_FRAME(    0,    0,    4,           0, 32814 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_12 ),
};

/* group 27 13 */
const Sint32 pl03_grp27_13[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   824 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0, 32811 ),
	EF_FRAME(    0,    0,    4,           0, 32812 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_13 ),
};

/* group 27 14 */
const Sint32 pl03_grp27_14[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   822 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_14 ),
};

/* group 27 15: pls03_02_jsoul_start (15 + sub_type), pls03_02_jsoul_move (15 + sub_type) */
const Sint32 pl03_grp27_15[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   823 ),
	EF_FRAME(    0,    0,    4,           0,   824 ),
	EF_FRAME(    0,    0,    4,           0,   816 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   817 ),
	CG_NEXT( pl03_grp27_15 ),
};

/* group 27 16 */
const Sint32 pl03_grp27_16[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   822 ),
	EF_FRAME(    0,    0,    4,           0, 32810 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_16 ),
};

/* group 27 17 */
const Sint32 pl03_grp27_17[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   824 ),
	EF_FRAME(    0,    0,    4,           0, 32808 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_17 ),
};

/* group 27 18 */
const Sint32 pl03_grp27_18[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   856 ),
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,    -1 ),
	CG_NEXT( pl03_grp27_18 ),
};

/* group 27 19: pls03_02_silhouette_first_start */
const Sint32 pl03_grp27_19[] = {
	EF_FRAME(    0,    0,    4,           0,   622 ),
	EF_FRAME(    0,    0,    4,           0,   623 ),
	EF_FRAME(    0,    0,    4,           0,   624 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   625 ),
	CG_NEXT( pl03_grp27_19 ),
};

/* group 27 20 */
const Sint32 pl03_grp27_20[] = {
	EF_FRAME(    0,    0,    4,           0,   813 ),
	EF_FRAME(    0,    0,    3,           0,   814 ),
	EF_FRAME(    0,    0,    1,           0,   815 ),
	EF_FRAME(    0,    0,    3,           0,   816 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   817 ),
	CG_NEXT( pl03_grp27_20 ),
};

/* group 27 21 */
const Sint32 pl03_grp27_21[] = {
	EF_FRAME(    0,    0,    4,           0,    -1 ),
	EF_FRAME(    0,    0,    4,           0,   903 ),
	EF_FRAME(    0,    0,    4,           0,   904 ),
	EF_FRAME(    0,    0,    4,           0,   905 ),
	EF_FRAME(    0,    0,    4,           0,   906 ),
	EF_FRAME(    0,    0,    4,           0,   907 ),
	EF_FRAME(    0,    0,    4,           0,   908 ),
};

const Sint32 pl03_grp27_21_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   909 ),
	CG_NEXT( pl03_grp27_21_loop ),
};

/* group 27 22 */
const Sint32 pl03_grp27_22[] = {
	EF_FRAME(    0,    0,   32,           0,   909 ),
	EF_FRAME(    0,    0,    4,           0,   910 ),
	EF_FRAME(    0,    0,    4,           0,   911 ),
	EF_FRAME(    0,    0,    4,           0,   912 ),
	EF_FRAME(    0,    0,    4,           0,   913 ),
	EF_FRAME(    0,    0,    4,           0,   914 ),
	EF_FRAME(    0,    0,    4,           0,   915 ),
};

const Sint32 pl03_grp27_22_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA,   916 ),
	CG_NEXT( pl03_grp27_22_loop ),
};

/* group 27 23 */
const Sint32 pl03_grp27_23[] = {
	EF_FRAME(    0,    0,    4,           0,   931 ),
	EF_FRAME(    0,    0,    4,           0,   932 ),
	EF_FRAME(    0,    0,    4,           0,   933 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   934 ),
	CG_NEXT( pl03_grp27_23 ),
};

/* group 27 24 */
const Sint32 pl03_grp27_24[] = {
	EF_FRAME(    0,    0,    4,           0,   935 ),
	EF_FRAME(    0,    0,    4,           0,   936 ),
	EF_FRAME(    0,    0,    4,           0,   937 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   938 ),
	CG_NEXT( pl03_grp27_24 ),
};

/* group 27 25 */
const Sint32 pl03_grp27_25[] = {
	EF_FRAME(    0,    0,    3,           0,   918 ),
	EF_FRAME(    0,    0,    5,           0,   919 ),
	EF_FRAME(    0,    0,   16,           0,   920 ),
	EF_FRAME(    0,    0,    5,           0,   919 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   918 ),
	CG_NEXT( pl03_grp27_25 ),
};
