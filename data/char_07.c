/* Wolverine (character 07): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_07      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_07     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl07.c) says which group
                   and number each move uses.
   pl07_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl07_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl07_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl07_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_07
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_07, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_07 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl07_stand_00[], pl07_stand_02[], pl07_stand_06[], pl07_stand_03[],
	pl07_stand_07[], pl07_jump_00[], pl07_jump_00_loop[], pl07_jump_01[], pl07_jump_01_loop[],
	pl07_jump_02[], pl07_jump_02_loop[], pl07_jump_03[], pl07_jump_06[], pl07_jump_06_loop[],
	pl07_jump_07[], pl07_jump_07_loop[], pl07_jump_08[], pl07_jump_08_loop[], pl07_jump_09[],
	pl07_jump_10[], pl07_jump_11[], pl07_dash_00[], pl07_dash_00_loop[], pl07_dash_01[],
	pl07_dash_01_loop[], pl07_crouch_00[], pl07_crouch_01[], pl07_crouch_02[], pl07_turn_00[],
	pl07_turn_01[], pl07_guard_00[], pl07_guard_01[], pl07_guard_02[], pl07_guard_03[],
	pl07_guard_04[], pl07_guard_05[], pl07_guard_06[], pl07_guard_07[], pl07_stand_p_00[],
	pl07_stand_p_00_loop[], pl07_stand_p_03[], pl07_stand_p_03_loop[], pl07_stand_p_01[],
	pl07_stand_p_02[], pl07_stand_k_00[], pl07_stand_k_01[], pl07_stand_k_02[], pl07_extra_01[],
	pl07_air_guard_00[], pl07_air_guard_01[], pl07_crouch_p_00[], pl07_crouch_p_03[],
	pl07_crouch_p_01[], pl07_crouch_p_02[], pl07_crouch_k_00[], pl07_crouch_k_01[],
	pl07_crouch_k_02[], pl07_jump_p_00[], pl07_jump_p_01[], pl07_jump_p_02[], pl07_jump_p_04[],
	pl07_jump_k_00[], pl07_jump_k_01[], pl07_jump_k_02[], pl07_jump_k_05[], pl07_jump_k_05_loop[],
	pl07_win_00[], pl07_win_00_loop[], pl07_win_01[], pl07_change_03[], pl07_change_03_loop[],
	pl07_win_02[], pl07_win_02_loop[], pl07_appear_00[], pl07_appear_01[], pl07_appear_01_loop[],
	pl07_appear_02[], pl07_get_up_00[], pl07_get_up_01[], pl07_unused_00[], pl07_get_up_02[],
	pl07_get_up_03[], pl07_catch_00[], pl07_catch_00_loop[], pl07_catch_01[], pl07_catch_02[],
	pl07_catch_02_loop[], pl07_catch_03[], pl07_catch_03_loop[], pl07_catch_04[],
	pl07_catch_04_loop[], pl07_catch_05[], pl07_jump_12[], pl07_change_04[], pl07_special_00[],
	pl07_special_01[], pl07_special_02[], pl07_special_03[], pl07_special_04[], pl07_special_05[],
	pl07_special_06[], pl07_special_08[], pl07_special_09[], pl07_special_10[],
	pl07_special_10_loop[], pl07_special_11[], pl07_special_11_loop[], pl07_special_12[],
	pl07_special_12_loop[], pl07_special_13[], pl07_special_13_loop[], pl07_special_14[],
	pl07_special_14_loop[], pl07_special_15[], pl07_special_15_loop[], pl07_special_16[],
	pl07_special_16_loop[], pl07_special_17[], pl07_special_17_loop[], pl07_special_18[],
	pl07_special_18_loop[], pl07_special_19[], pl07_special_19_loop[], pl07_special_20[],
	pl07_special_20_loop[], pl07_special_21[], pl07_special_22[], pl07_special_28[],
	pl07_special_28_loop[], pl07_special_25[], pl07_special_25_loop[], pl07_special_31[],
	pl07_special_23[], pl07_special_29[], pl07_special_29_loop[], pl07_special_26[],
	pl07_special_26_loop[], pl07_special_32[], pl07_super_05[], pl07_super_05_loop[],
	pl07_super_06[], pl07_super_06_loop[], pl07_super_08[], pl07_super_08_loop[], pl07_super_07[],
	pl07_super_07_loop[], pl07_super_09[], pl07_super_10[], pl07_super_00[], pl07_super_00_loop[],
	pl07_super_01[], pl07_super_01_loop[], pl07_super_02[], pl07_super_02_loop[], pl07_super_03[],
	pl07_super_04[], pl07_super_11[], pl07_super_11_loop[], pl07_super_12[], pl07_super_13[],
	pl07_special_36[], pl07_special_37[], pl07_special_39[], pl07_special_40[], pl07_special_41[],
	pl07_special_42[], pl07_unused_01[], pl07_unused_02[], pl07_unused_03[], pl07_damage_00[],
	pl07_damage_01[], pl07_damage_02[], pl07_damage_03[], pl07_damage_04[], pl07_damage_05[],
	pl07_damage_06[], pl07_damage_07[], pl07_damage_08[], pl07_damage_10[], pl07_damage_11[],
	pl07_damage_12[], pl07_damage_13[], pl07_damage_13_loop[], pl07_damage_09[],
	pl07_damage_09_loop[], pl07_damage_16[], pl07_damage_17[], pl07_damage_18[], pl07_damage_19[],
	pl07_damage_20[], pl07_damage_21[], pl07_damage_22[], pl07_damage_23[], pl07_damage_24[],
	pl07_damage_25[], pl07_unused_04[], pl07_damage_26[], pl07_damage_27[], pl07_damage_33[],
	pl07_damage_28[], pl07_damage_29[], pl07_damage_31[], pl07_dizzy_00[], pl07_stand_p_04[],
	pl07_stand_k_04[], pl07_stand_k_04_loop[], pl07_crouch_p_05[], pl07_unused_05[],
	pl07_change_00[], pl07_change_00_loop[], pl07_change_01[], pl07_unused_06[], pl07_unused_07[],
	pl07_change_02[], pl07_super_14[], pl07_special_34[], pl07_special_34_loop[], pl07_special_35[],
	pl07_special_44[], pl07_caught_00[], pl07_caught_01[], pl07_caught_02[], pl07_caught_03[],
	pl07_caught_04[], pl07_caught_05[], pl07_caught_06[], pl07_caught_07[], pl07_caught_08[],
	pl07_caught_09[], pl07_caught_10[], pl07_caught_11[], pl07_caught_12[], pl07_caught_13[],
	pl07_caught_14[], pl07_caught_15[], pl07_caught_16[], pl07_caught_17[], pl07_caught_18[],
	pl07_caught_19[], pl07_caught_20[], pl07_effect_02[], pl07_effect_03[], pl07_effect_04[],
	pl07_effect_05[], pl07_effect_06[], pl07_effect_07[], pl07_effect_08[], pl07_effect_09[],
	pl07_effect_00[], pl07_effect_01[], pl07_effect_14[], pl07_effect_15[], pl07_effect_16[],
	pl07_effect_17[], pl07_effect_18[], pl07_effect_19[], pl07_grp27_00[], pl07_grp27_01[],
	pl07_grp27_02[], pl07_grp27_03[], pl07_grp27_04[], pl07_grp27_05[], pl07_grp27_05_loop[],
	pl07_grp27_06[], pl07_grp27_07[];
extern const Sint32 *const pl07_stand_tbl[], *const pl07_jump_tbl[], *const pl07_dash_tbl[],
	*const pl07_crouch_tbl[], *const pl07_turn_tbl[], *const pl07_guard_tbl[],
	*const pl07_air_guard_tbl[], *const pl07_stand_p_tbl[], *const pl07_stand_k_tbl[],
	*const pl07_crouch_p_tbl[], *const pl07_crouch_k_tbl[], *const pl07_jump_p_tbl[],
	*const pl07_jump_k_tbl[], *const pl07_damage_tbl[], *const pl07_dizzy_tbl[],
	*const pl07_catch_tbl[], *const pl07_caught_tbl[], *const pl07_get_up_tbl[],
	*const pl07_appear_tbl[], *const pl07_win_tbl[], *const pl07_extra_tbl[],
	*const pl07_special_tbl[], *const pl07_super_tbl[], *const pl07_effect_tbl[],
	*const pl07_change_tbl[], *const pl07_grp25_tbl[], *const pl07_fly_tbl[],
	*const pl07_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_07[16] = {
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
const Sint32 *const *const char_tbl_07[] = {
	pl07_stand_tbl, pl07_jump_tbl, pl07_dash_tbl, pl07_crouch_tbl, pl07_turn_tbl, pl07_guard_tbl,
	pl07_air_guard_tbl, pl07_stand_p_tbl, pl07_stand_k_tbl, pl07_crouch_p_tbl, pl07_crouch_k_tbl, pl07_jump_p_tbl,
	pl07_jump_k_tbl, pl07_damage_tbl, pl07_dizzy_tbl, pl07_catch_tbl, pl07_caught_tbl, pl07_get_up_tbl,
	pl07_appear_tbl, pl07_win_tbl, pl07_extra_tbl, pl07_special_tbl, pl07_super_tbl, pl07_effect_tbl,
	pl07_change_tbl, pl07_grp25_tbl, pl07_fly_tbl, pl07_grp27_tbl, 0,
};

const Sint32 *const pl07_stand_tbl[] = {
	pl07_stand_00, pl07_stand_00, pl07_stand_02, pl07_stand_03, pl07_stand_00, pl07_stand_00,
	pl07_stand_06, pl07_stand_07, pl07_stand_00, pl07_stand_00, 0,
};

const Sint32 *const pl07_jump_tbl[] = {
	pl07_jump_00, pl07_jump_01, pl07_jump_02, pl07_jump_03, pl07_jump_03, pl07_jump_03,
	pl07_jump_06, pl07_jump_07, pl07_jump_08, pl07_jump_09, pl07_jump_10, pl07_jump_11,
	pl07_jump_12, 0,
};

const Sint32 *const pl07_dash_tbl[] = {
	pl07_dash_00, pl07_dash_01, pl07_stand_00, pl07_stand_00, 0,
};

const Sint32 *const pl07_crouch_tbl[] = {
	pl07_crouch_00, pl07_crouch_01, pl07_crouch_02, 0,
};

const Sint32 *const pl07_turn_tbl[] = {
	pl07_turn_00, pl07_turn_01, 0,
};

const Sint32 *const pl07_guard_tbl[] = {
	pl07_guard_00, pl07_guard_01, pl07_guard_02, pl07_guard_03, pl07_guard_04, pl07_guard_05,
	pl07_guard_06, pl07_guard_07, 0,
};

const Sint32 *const pl07_air_guard_tbl[] = {
	pl07_air_guard_00, pl07_air_guard_01, pl07_air_guard_01, 0,
};

const Sint32 *const pl07_stand_p_tbl[] = {
	pl07_stand_p_00, pl07_stand_p_01, pl07_stand_p_02, pl07_stand_p_03, pl07_stand_p_04, pl07_stand_p_02,
	0,
};

const Sint32 *const pl07_stand_k_tbl[] = {
	pl07_stand_k_00, pl07_stand_k_01, pl07_stand_k_02, pl07_stand_k_00, pl07_stand_k_04, pl07_stand_k_02,
	0,
};

const Sint32 *const pl07_crouch_p_tbl[] = {
	pl07_crouch_p_00, pl07_crouch_p_01, pl07_crouch_p_02, pl07_crouch_p_03, pl07_crouch_p_01, pl07_crouch_p_05,
	0,
};

const Sint32 *const pl07_crouch_k_tbl[] = {
	pl07_crouch_k_00, pl07_crouch_k_01, pl07_crouch_k_02, 0,
};

const Sint32 *const pl07_jump_p_tbl[] = {
	pl07_jump_p_00, pl07_jump_p_01, pl07_jump_p_02, pl07_jump_p_00, pl07_jump_p_04, pl07_jump_p_02,
	0,
};

const Sint32 *const pl07_jump_k_tbl[] = {
	pl07_jump_k_00, pl07_jump_k_01, pl07_jump_k_02, pl07_jump_k_00, pl07_jump_k_01, pl07_jump_k_05,
	0,
};

const Sint32 *const pl07_damage_tbl[] = {
	pl07_damage_00, pl07_damage_01, pl07_damage_02, pl07_damage_03, pl07_damage_04, pl07_damage_05,
	pl07_damage_06, pl07_damage_07, pl07_damage_08, pl07_damage_09, pl07_damage_10, pl07_damage_11,
	pl07_damage_12, pl07_damage_13, pl07_damage_13, pl07_damage_13, pl07_damage_16, pl07_damage_17,
	pl07_damage_18, pl07_damage_19, pl07_damage_20, pl07_damage_21, pl07_damage_22, pl07_damage_23,
	pl07_damage_24, pl07_damage_25, pl07_damage_26, pl07_damage_27, pl07_damage_28, pl07_damage_29,
	pl07_damage_27, pl07_damage_31, pl07_damage_27, pl07_damage_33, pl07_damage_13, 0,
};

const Sint32 *const pl07_dizzy_tbl[] = {
	pl07_dizzy_00, pl07_stand_00, 0,
};

const Sint32 *const pl07_catch_tbl[] = {
	pl07_catch_00, pl07_catch_01, pl07_catch_02, pl07_catch_03, pl07_catch_04, pl07_catch_05,
	0,
};

const Sint32 *const pl07_caught_tbl[] = {
	pl07_caught_00, pl07_caught_01, pl07_caught_02, pl07_caught_03, pl07_caught_04, pl07_caught_05,
	pl07_caught_06, pl07_caught_07, pl07_caught_08, pl07_caught_09, pl07_caught_10, pl07_caught_11,
	pl07_caught_12, pl07_caught_13, pl07_caught_14, pl07_caught_15, pl07_caught_16, pl07_caught_17,
	pl07_caught_18, pl07_caught_19, pl07_caught_20, pl07_caught_20, 0,
};

const Sint32 *const pl07_get_up_tbl[] = {
	pl07_get_up_00, pl07_get_up_01, pl07_get_up_02, pl07_get_up_03, 0,
};

const Sint32 *const pl07_appear_tbl[] = {
	pl07_appear_00, pl07_appear_01, pl07_appear_02, 0,
};

const Sint32 *const pl07_win_tbl[] = {
	pl07_win_00, pl07_win_01, pl07_win_02, 0,
};

const Sint32 *const pl07_extra_tbl[] = {
	pl07_win_01, pl07_extra_01, 0,
};

const Sint32 *const pl07_special_tbl[] = {
	pl07_special_00, pl07_special_01, pl07_special_02, pl07_special_03, pl07_special_04, pl07_special_05,
	pl07_special_06, pl07_special_06, pl07_special_08, pl07_special_09, pl07_special_10, pl07_special_11,
	pl07_special_12, pl07_special_13, pl07_special_14, pl07_special_15, pl07_special_16, pl07_special_17,
	pl07_special_18, pl07_special_19, pl07_special_20, pl07_special_21, pl07_special_22, pl07_special_23,
	pl07_special_23, pl07_special_25, pl07_special_26, pl07_special_23, pl07_special_28, pl07_special_29,
	pl07_special_23, pl07_special_31, pl07_special_32, pl07_special_23, pl07_special_34, pl07_special_35,
	pl07_special_36, pl07_special_37, pl07_special_37, pl07_special_39, pl07_special_40, pl07_special_41,
	pl07_special_42, pl07_special_42, pl07_special_44, 0,
};

const Sint32 *const pl07_super_tbl[] = {
	pl07_super_00, pl07_super_01, pl07_super_02, pl07_super_03, pl07_super_04, pl07_super_05,
	pl07_super_06, pl07_super_07, pl07_super_08, pl07_super_09, pl07_super_10, pl07_super_11,
	pl07_super_12, pl07_super_13, pl07_super_14, 0,
};

const Sint32 *const pl07_effect_tbl[] = {
	pl07_effect_00, pl07_effect_01, pl07_effect_02, pl07_effect_03, pl07_effect_04, pl07_effect_05,
	pl07_effect_06, pl07_effect_07, pl07_effect_08, pl07_effect_09, pl07_effect_06, pl07_effect_06,
	pl07_effect_06, pl07_effect_06, pl07_effect_14, pl07_effect_15, pl07_effect_16, pl07_effect_17,
	pl07_effect_18, pl07_effect_19, 0,
};

const Sint32 *const pl07_change_tbl[] = {
	pl07_change_00, pl07_change_01, pl07_change_02, pl07_change_03, pl07_change_04, 0,
};

const Sint32 *const pl07_grp25_tbl[] = {
	pl07_stand_00, 0,
};

const Sint32 *const pl07_fly_tbl[] = {
	pl07_stand_00, 0,
};

const Sint32 *const pl07_grp27_tbl[] = {
	pl07_grp27_00, pl07_grp27_01, pl07_grp27_02, pl07_grp27_03, pl07_grp27_04, pl07_grp27_05,
	pl07_grp27_06, pl07_grp27_07, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow        atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_land, win_move, act_stand, appear_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl07_stand_00[] = {
	CG_FRAME(    0,    0,    4,           0,    21, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    22, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    23, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    24, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    25, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    26, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    27, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    28, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    29, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    30, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    31, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    32, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    33, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    34, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    35, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    36, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    37, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl07_stand_02[] = {
	CG_FRAME(    0,    0,    3,           0,    39, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    40, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    41, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    42, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    43, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    44, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    45, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    46, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    47, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    48, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    49, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    50, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    51, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    52, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    54, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_stand_02 ),
};

/* Char_Stand 6 */
const Sint32 pl07_stand_06[] = {
	CG_FRAME(    0,    0,    7,           0,    39, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,    40, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_stand_06 ),
};

/* Char_Stand 3 */
const Sint32 pl07_stand_03[] = {
	CG_FRAME(    0,    0,    3,           0,    55, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    56, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    57, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    58, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    60, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    61, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    62, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    64, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    66, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    67, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    69, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    70, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_stand_03 ),
};

/* Char_Stand 7 */
const Sint32 pl07_stand_07[] = {
	CG_FRAME(    0,    0,    7,           0,    55, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,    56, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl07_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    81, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
};

const Sint32 pl07_jump_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    85, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_NEXT( pl07_jump_00_loop ),
};

/* Char_Jump 1: weapon_x_float (Weapon X), fatal_slash (Fatal Claw), act_wall_jump, act_change_gattai_init, ... */
const Sint32 pl07_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    90, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    91, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
};

const Sint32 pl07_jump_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_NEXT( pl07_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl07_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    89, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    88, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
};

const Sint32 pl07_jump_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_NEXT( pl07_jump_02_loop ),
};

/* Char_Jump 3: dive_fall
   Char_Jump 4: fatal_fall (Fatal Claw)
   Char_Jump 5 */
const Sint32 pl07_jump_03[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0x90,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x90,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl07_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    79, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    81, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
};

const Sint32 pl07_jump_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    85, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_NEXT( pl07_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl07_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    90, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    91, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
};

const Sint32 pl07_jump_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_NEXT( pl07_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl07_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    89, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2,           0,    88, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
};

const Sint32 pl07_jump_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 206 ),
	CG_NEXT( pl07_jump_08_loop ),
};

/* Char_Jump 9: drill_fly (Drill Claw), act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl07_jump_09[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    85, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl07_jump_09 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl07_jump_10[] = {
	CG_FRAME(    0,    0,    2,           0,    92, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    93, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl07_jump_10 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl07_jump_11[] = {
	CG_FRAME(    0,    0,    2,           0,    87, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    86, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl07_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl07_dash_00[] = {
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x20,          0,  0x00, 0x03,    0,   0,     0,      0,   8 ),
};

const Sint32 pl07_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,    74, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,    75, 0x80,   0x00,       0x1a,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,    76, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    72, 0x80,   0x00,       0x1a,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl07_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl07_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x20,          0,  0x00, 0x03,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    78, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    8,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x40,       0x1a,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  10 ),
};

const Sint32 pl07_dash_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl07_dash_01_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl07_crouch_00[] = {
	CG_FRAME(    1,    0,    6,           0,    94, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    95, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl07_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl07_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    38, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl07_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl07_crouch_02[] = {
	CG_FRAME(    1,    0,    6,           0,    96, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    97, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl07_turn_00[] = {
	CG_FRAME(    0,    0,    3,           0,    98, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    99, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   100, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   101, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   102, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl07_turn_01[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   103, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl07_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl07_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   104, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   105, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  13 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl07_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   106, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl07_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl07_guard_02[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   104, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl07_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl07_guard_03[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   106, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl07_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl07_guard_04[] = {
	CG_FRAME(    0,    0,    4,           0,   107, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   108, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl07_guard_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   109, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl07_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl07_guard_06[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   107, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl07_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl07_guard_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   109, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl07_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl07_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,    21, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,    1,           0,   182, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    3,           0,   181, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    1,    1,           0,   183, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
};

const Sint32 pl07_stand_p_00_loop[] = {
	CG_FRAME(    0,    1,    6, END_OF_DATA,   196, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_NEXT( pl07_stand_p_00_loop ),
};

/* Char_Stand_P 3 */
const Sint32 pl07_stand_p_03[] = {
	CG_FRAME(    0,    0,    4,           0,    21, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,    1,           0,   182, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    6,           0,   181, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   183, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
};

const Sint32 pl07_stand_p_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   196, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_NEXT( pl07_stand_p_03_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl07_stand_p_01[] = {
	CG_FRAME(    0,    0,    2,           0,   184, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   185, 0xa0,   0x00,        ATK,  0x00, 0x23,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   186, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    1,           0,   187, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    1,           0,   188, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   11,           0,   189, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   196, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl07_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy)
   Char_Stand_P 5 */
const Sint32 pl07_stand_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   197, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    1,    2,           0,   198, 0xa0,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,    2,           0,   199, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   14,           0,   200, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    8,           0,   201, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    97, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl07_stand_p_02 ),
};

/* Char_Stand_K 0 (light)
   Char_Stand_K 3 */
const Sint32 pl07_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   202, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    1,           0,   203, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   205, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl07_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl07_stand_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,    21, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,    2,           0,   206, 0xa0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    1,    3,           0,   207, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    1,    7,           0,   207, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    4,           0,   399, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl07_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy)
   Char_Stand_K 5 */
const Sint32 pl07_stand_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   243, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   244, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0xa0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    8,           0,   245, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    6,           0,   248, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    5,           0,    94, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl07_stand_k_02 ),
};

/* Char_Extra 1: pl07_upper */
const Sint32 pl07_extra_01[] = {
	CG_FRAME(    0,    0,    5,           0,   243, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   244, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0xa0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    8,           0,   245, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   246, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    6,           0,   248, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    5,           0,    94, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl07_extra_01 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl07_air_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   110, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,  48 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl07_air_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   112, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl07_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl07_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,    38, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,    2,           0,   218, 0xa0,   0x40,        ATK,  0x00, 0x04,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    6,           0,   219, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    38, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl07_crouch_p_00 ),
};

/* Char_Crouch_P 3 */
const Sint32 pl07_crouch_p_03[] = {
	CG_FRAME(    0,    0,    6,           0,    38, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,    2,           0,   218, 0xa0,   0x40,        ATK,  0x00, 0x04,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   219, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    38, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl07_crouch_p_03 ),
};

/* Char_Crouch_P 1 (medium)
   Char_Crouch_P 4 */
const Sint32 pl07_crouch_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   220, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4,           0,   221, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    1,           0,   222, 0xa0,   0x40,        ATK,  0x00, 0x03,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,    2,           0,   223, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   224, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    6,           0,   225, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    95, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl07_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl07_crouch_p_02[] = {
	CG_FRAME(    0,    2,    2,           0,    95, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,   228, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,   229, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   230, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   231, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    1,    2,           0,   232, 0xa0,   0x40,        ATK,  0x00, 0x06,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0,   233, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   233, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl07_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl07_crouch_k_00[] = {
	CG_FRAME(    0,    0,    1,           0,    38, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    3,           0,   234, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,    2,           0,   235, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   234, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl07_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl07_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   237, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    1,           0,   238, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    8,           0,   239, 0xa0,   0x40,        ATK,  0x00, 0x23,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    6,           0,   240, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    6,           0,   241, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   242, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl07_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl07_crouch_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   213, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    5,           0,   213, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   214, 0xa0,   0x40, ATK | 0x17,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_FRAME(   12,    0,    8,           0,   215, 0xa0,   0x40,        ATK,  0x00, 0x06,    0,   0,     0,      0,  78 ),
	CG_FRAME(    8,    0,    4,           0,   216, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    4,    0,    4,           0,   217, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    2,    0,    4,           0,   217, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    1,    0,    4,           0,   217, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    38, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl07_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl07_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   249, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x80,    0,    1,           0,   250, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    9,           0,   251, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   249, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl07_jump_p_00 ),
};

/* Char_Jump_P 1 (medium) */
const Sint32 pl07_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   252, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0xa0,   0x20,        ATK,  0x00, 0x04,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    1,    6,           0,   255, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    1,   12, END_OF_DATA,   255, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl07_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl07_jump_p_02[] = {
	CG_FRAME(    0,    0,    4,           0,   261, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,   262, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    1,    2,           0,   263, 0xa0,   0x20,        ATK,  0x00, 0x05,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    2,           0,   264, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    81, 0x90,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl07_jump_p_02 ),
};

/* Char_Jump_P 4: jump_punch_move */
const Sint32 pl07_jump_p_04[] = {
	CG_FRAME(    0,    0,    3,           0,   256, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 200 ),
	CG_FRAME( 0x80,    0,    1,           0,   257, 0xa0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 200 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 201 ),
	CG_FRAME(    0,    0,    2,           0,   259, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 200 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   260, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 200 ),
	CG_NEXT( pl07_jump_p_04 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl07_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   210, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,    1,           0,   268, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   12,           0,   269, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   210, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl07_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl07_jump_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   270, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,    1,           0,   271, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME( 0x80,    0,    2,           0,   272, 0xa0,   0x20,        ATK,  0x00, 0x03,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   12,           0,   273, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   12,           0,   273, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   274, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl07_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl07_jump_k_02[] = {
	CG_FRAME(    0,    0,    6,           0,   270, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    4,           0,   275, 0xa0,   0x20,        ATK,  0x00, 0x03,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    1,    4,           0,   277, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    4,           0,   278, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,    4,           0,   275, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,   276, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    1,    4,           0,   277, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    6,           0,   278, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   210, 0xa0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl07_jump_k_02 ),
};

/* Char_Jump_K 5 */
const Sint32 pl07_jump_k_05[] = {
	CG_FRAME(    0,    0,    7,           0,   249, 0xa0,   0x0e,        ATK,  0x00, 0x04,    0,   0,     0,      0,  82 ),
};

const Sint32 pl07_jump_k_05_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   382, 0xa0,   0x0e,        ATK,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   383, 0xa0,   0x0e,        ATK,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_NEXT( pl07_jump_k_05_loop ),
};

/* Char_Win 0 */
const Sint32 pl07_win_00[] = {
	CG_FRAME(    0,    0,    5,           0,   113, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,    1,    1,           0,   113, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   114, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   115, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   20,           0,   116, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   117, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   119, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   120, 0x80,   0x00,          0,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   120, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl07_win_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   120, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_win_00_loop ),
};

/* Char_Win 1
   Char_Extra 0: chouhatsu_init (The taunt) */
const Sint32 pl07_win_01[] = {
	CG_FRAME(    0,    0,    7,           0,   310, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   302, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   303, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    7,           0,   304, 0xb0,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   305, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   306, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   307, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    7,           0,   308, 0xb0,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   309, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7,           0,   310, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   311, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_win_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl07_change_03[] = {
	CG_FRAME(    0,    0,    2,           0,   310, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   303, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   304, 0xb0,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   305, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   308, 0xb0,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl07_change_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   311, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_change_03_loop ),
};

/* Char_Win 2: win_init */
const Sint32 pl07_win_02[] = {
	CG_FRAME(    0,    0,    6,           0,   124, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   20,           0,   121, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   122, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl07_win_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   123, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_win_02_loop ),
};

/* Char_Appear 0: appear_jump_init */
const Sint32 pl07_appear_00[] = {
	CG_FRAME(    0,    0,    1,           0,   125, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   126, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   127, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   129, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   131, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   132, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_appear_00 ),
};

/* Char_Appear 1: appear_jump_start */
const Sint32 pl07_appear_01[] = {
	CG_FRAME(    0,    0,    3,           0,    85, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    83, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    82, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    81, 0x80,   0x20,          0,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
};

const Sint32 pl07_appear_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    85, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_appear_01_loop ),
};

/* Char_Appear 2: appear_jump_start, appear_jump_air */
const Sint32 pl07_appear_02[] = {
	CG_FRAME(    0,    0,    1,           0,   133, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   133, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   135, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   137, 0x80,   0x40,          0,  0x00, 0x21,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   138, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   139, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   143, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   399, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_appear_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl07_get_up_00[] = {
	CG_FRAME(    0,    0,    3,           0,   144, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   145, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   147, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   148, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   150, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    96, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl07_get_up_01[] = {
	CG_FRAME(    0,    0,    1,           0,   144, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   153, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   154, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   155, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   156, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   158, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   159, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   160, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   150, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   151, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_get_up_01 ),
};

const Sint32 pl07_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   144, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   154, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   158, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   160, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   150, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   151, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	0x00002814,
};

/* Char_Get_Up 2 */
const Sint32 pl07_get_up_02[] = {
	CG_FRAME(    0,    0,    2,           0,   144, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   154, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   158, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   159, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   160, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   150, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   151, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl07_get_up_03[] = {
	CG_FRAME(    0,    0,    2,           0,   144, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   160, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   158, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   157, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   156, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   154, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   150, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   151, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_get_up_03 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl07_catch_00[] = {
	CG_FRAME(    0,    8,   12,           0,   281, 0x80,   0x20,          0,  0x00, 0x03,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,   10,   24,           0,   282, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    2,    9,           0,   291, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
};

const Sint32 pl07_catch_00_loop[] = {
	CG_FRAME(    0,   16,   12,           0,   283, 0x80,   0x20,          0,  0x00, 0x03,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    8,           0,   283, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,   16,   12,           0,   290, 0x80,   0x20,          0,  0x00, 0x03,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   291, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_NEXT( pl07_catch_00_loop ),
};

/* Char_Catch 1: nml_p_catch */
const Sint32 pl07_catch_01[] = {
	CG_FRAME(    0,    2,   10,           0,   284, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    2,    3,           0,   285, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,   14,    3,           0,   286, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,    3,           0,   287, 0x80,   0x40,          0,  0x00, 0x03,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,    1,           0,   288, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    4,    9,           0,   288, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    6,   10, END_OF_DATA,   289, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_catch_01 ),
};

/* Char_Catch 2: nml_k_set */
const Sint32 pl07_catch_02[] = {
	CG_FRAME(    0,    8,    8,           0,   292, 0x80,   0x40,          0,  0x00, 0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,    8,           0,   293, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,    8,           0,   294, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,   48,           0,   295, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   12,           0,   296, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0011,   0 ),
};

const Sint32 pl07_catch_02_loop[] = {
	CG_FRAME(    0,   18,    8,           0,   297, 0x80,   0x40,          0,  0x00, 0x04,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,    4,           0,   296, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,   18,    8,           0,   298, 0x80,   0x40,          0,  0x00, 0x04,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   296, 0x80,   0x40,          0,  0x00,    0,    0,   0,     0, 0x0014,   0 ),
	CG_NEXT( pl07_catch_02_loop ),
};

/* Char_Catch 3: nml_k_catch */
const Sint32 pl07_catch_03[] = {
	CG_FRAME(    0,   14,    2,           0,    77, 0x80,   0x20,          0,  0x00, 0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,    2,           0,    78, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,    2,           0,    85, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,    2,           0,    84, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,    2,           0,    83, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,    2,           0,    82, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   16,    2,           0,    81, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl07_catch_03_loop[] = {
	CG_FRAME(    0,   16,   48, END_OF_DATA,    80, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_catch_03_loop ),
};

/* Char_Catch 4: jump_p_set */
const Sint32 pl07_catch_04[] = {
	CG_FRAME(    0,    8,   12,           0,   364, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x001a,   0 ),
	CG_FRAME(    0,   16,    6,           0,   364, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x001a,   0 ),
};

const Sint32 pl07_catch_04_loop[] = {
	CG_FRAME(    0,   12,    8,           0,   298, 0x80,   0x20,          0,  0x00, 0x04,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,   12,    8,           0,   363, 0x80,   0x20,          0,  0x00, 0x04,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   364, 0x80,   0x20,          0,  0x00,    0,    0,   0,     0, 0x001a,   0 ),
	CG_NEXT( pl07_catch_04_loop ),
};

/* Char_Catch 5: jump_p_catch */
const Sint32 pl07_catch_05[] = {
	CG_FRAME(    0,    2,    8,           0,   295, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    4,   22, END_OF_DATA,   362, 0x80,   0x00,          0,  0x00, 0x03,    0,   0,     0, 0x001c,   0 ),
	CG_NEXT( pl07_catch_05 ),
};

/* Char_Jump 12: act_wall_jump */
const Sint32 pl07_jump_12[] = {
	CG_FRAME(    0,    0,    4,           0,   299, 0x90,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x90,   0x20,       0x0d,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,   28,    4, END_OF_DATA,   301, 0x90,   0x20,          0,  0x00, 0x03,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl07_jump_12 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl07_change_04[] = {
	CG_FRAME(    0,    0,    2,           0,   310, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   303, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   304, 0xb0,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   305, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   308, 0xb0,   0x00,          0,  0x00, 0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   311, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_change_04 ),
};

/* Char_Special 0: tornado_init (Tornado Claw) */
const Sint32 pl07_special_00[] = {
	CG_FRAME(    0,    0,    6,           0,   103, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    1,    0,    4,           0,   312, 0xb0,   0x00,        ATK,  0x01, 0x14,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,    3,           0,   314, 0xb0,   0x20,        ATK,  0x02,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   315, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl07_special_00 ),
};

/* Char_Special 1: tornado_up (Tornado Claw), drill_fly (Drill Claw, 1 + char_no.b.no) */
const Sint32 pl07_special_01[] = {
	CG_FRAME(    1,    0,    2,           0,   316, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    1,    0,    2,           0,   317, 0xb0,   0x00,        ATK,  0x02,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    1,    0,    2,           0,   318, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    1,    0,    2, END_OF_DATA,   319, 0xb0,   0x00,        ATK,  0x02,    0,    0,   0,     0,      0, 121 ),
	CG_NEXT( pl07_special_01 ),
};

/* Char_Special 2 */
const Sint32 pl07_special_02[] = {
	CG_FRAME(    1,    0,    2,           0,   320, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 122 ),
	CG_FRAME(    1,    0,    2,           0,   313, 0xb0,   0x00,        ATK,  0x02,    0,    0,   0,     0,      0, 123 ),
	CG_FRAME(    1,    0,    2,           0,   314, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 124 ),
	CG_FRAME(    1,    0,    2, END_OF_DATA,   315, 0xb0,   0x00,        ATK,  0x02,    0,    0,   0,     0,      0, 125 ),
	CG_NEXT( pl07_special_02 ),
};

/* Char_Special 3: tornado_up (Tornado Claw), tornado_keep (Tornado Claw) */
const Sint32 pl07_special_03[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   361, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 126 ),
	CG_NEXT( pl07_special_03 ),
};

/* Char_Special 4: tornado_last (Tornado Claw) */
const Sint32 pl07_special_04[] = {
	CG_FRAME(    0,    0,    3,           0,   361, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,    3,           0,   356, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,    3,           0,   357, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,    3,           0,   358, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,    3,           0,   359, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   360, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_NEXT( pl07_special_04 ),
};

/* Char_Special 5: tornado_fall (Tornado Claw, 5 + atck_level) */
const Sint32 pl07_special_05[] = {
	CG_FRAME(    0,    0,    1,           0,    94, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0xb0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    2,           0,    38, 0xb0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    1,           0,    96, 0xb0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_NEXT( pl07_special_05 ),
};

/* Char_Special 6
   Char_Special 7 */
const Sint32 pl07_special_06[] = {
	CG_FRAME(    0,    0,    3,           0,    94, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    4,           0,    95, 0xb0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    3,           0,    38, 0xb0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    3,           0,    96, 0xb0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_NEXT( pl07_special_06 ),
};

/* Char_Special 8 */
const Sint32 pl07_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,    94, 0xb0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    95, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 129 ),
	CG_NEXT( pl07_special_08 ),
};

/* Char_Special 9 */
const Sint32 pl07_special_09[] = {
	CG_FRAME(    0,    0,    1,           0,   435, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1,           0,   442, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1,           0,   441, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1,           0,   439, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1,           0,   438, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1,           0,   437, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   436, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl07_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl07_special_10[] = {
	CG_FRAME(    0,    1,    1,           0,   322, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   323, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   324, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   325, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   326, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   321, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   322, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   323, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   324, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   325, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   326, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    1,           0,   321, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
};

const Sint32 pl07_special_10_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   322, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    2,           0,   323, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    2,           0,   324, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    2,           0,   325, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    2,           0,   326, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   321, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_NEXT( pl07_special_10_loop ),
};

/* Char_Special 11 */
const Sint32 pl07_special_11[] = {
	CG_FRAME(    0,    0,    3,           0,   355, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
};

const Sint32 pl07_special_11_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   442, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    1,           0,   438, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    1,           0,   436, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   435, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_NEXT( pl07_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl07_special_12[] = {
	CG_FRAME(    0,    1,    1,           0,   328, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   329, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   330, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   331, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   332, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   327, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   328, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   329, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   330, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   331, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   332, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    1,           0,   327, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
};

const Sint32 pl07_special_12_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   328, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    2,           0,   329, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    2,           0,   331, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   327, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_NEXT( pl07_special_12_loop ),
};

/* Char_Special 13 */
const Sint32 pl07_special_13[] = {
	CG_FRAME(    0,    0,    3,           0,   351, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
};

const Sint32 pl07_special_13_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   442, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    1,           0,   438, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    1,           0,   436, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   435, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_NEXT( pl07_special_13_loop ),
};

/* Char_Special 14 */
const Sint32 pl07_special_14[] = {
	CG_FRAME(    0,    1,    1,           0,   334, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   335, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   336, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   337, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   338, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   333, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   334, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   335, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   336, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   337, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   338, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    1,           0,   333, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
};

const Sint32 pl07_special_14_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   334, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    1,    2,           0,   336, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   333, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_NEXT( pl07_special_14_loop ),
};

/* Char_Special 15 */
const Sint32 pl07_special_15[] = {
	CG_FRAME(    0,    0,    3,           0,   352, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
};

const Sint32 pl07_special_15_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   442, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    1,           0,   438, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    1,           0,   436, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   435, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_NEXT( pl07_special_15_loop ),
};

/* Char_Special 16 */
const Sint32 pl07_special_16[] = {
	CG_FRAME(    0,    1,    1,           0,   340, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   341, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   342, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   343, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   344, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   339, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   340, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   341, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   342, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   343, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   344, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    1,           0,   339, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
};

const Sint32 pl07_special_16_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   340, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    2,           0,   342, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    2,           0,   343, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    2,           0,   344, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   339, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_NEXT( pl07_special_16_loop ),
};

/* Char_Special 17 */
const Sint32 pl07_special_17[] = {
	CG_FRAME(    0,    0,    3,           0,   353, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
};

const Sint32 pl07_special_17_loop[] = {
	CG_FRAME(    3,    0,    1,           0,   442, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    1,           0,   438, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    1,           0,   436, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   435, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_NEXT( pl07_special_17_loop ),
};

/* Char_Special 18 */
const Sint32 pl07_special_18[] = {
	CG_FRAME(    0,    1,    1,           0,   346, 0xb0,   0x00,        ATK,  0x00, 0x16,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   347, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   348, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   350, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   345, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   346, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   347, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   348, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   349, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   350, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    1,           0,   345, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
};

const Sint32 pl07_special_18_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   346, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    2,           0,   347, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    2,           0,   349, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    2,           0,   350, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   345, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_NEXT( pl07_special_18_loop ),
};

/* Char_Special 19: drill_fly (Drill Claw) */
const Sint32 pl07_special_19[] = {
	CG_FRAME(    0,    0,    3,           0,   354, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
};

const Sint32 pl07_special_19_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   442, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2,           0,   438, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2,           0,   436, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   435, 0xb0,   0x20,        ATE,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl07_special_19_loop ),
};

/* Char_Special 20: drill_fly (Drill Claw) */
const Sint32 pl07_special_20[] = {
	CG_FRAME(    0,    0,    4,           0,   299, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 140 ),
};

const Sint32 pl07_special_20_loop[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   300, 0xb0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_NEXT( pl07_special_20_loop ),
};

/* Char_Special 21: drill_fly (Drill Claw) */
const Sint32 pl07_special_21[] = {
	CG_FRAME(    0,    0,    4,           0,    94, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 141 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    95, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 141 ),
	CG_NEXT( pl07_special_21 ),
};

/* Char_Special 22: barrage_init (Berserker Barrage, 22 + atck_level) */
const Sint32 pl07_special_22[] = {
	CG_FRAME(    0,    0,    4,           0,   375, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 144 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,    73, 0x80,   0x00,        ATK,  0x00, 0x15,    0,   0,     0,      0, 145 ),
	CG_NEXT( pl07_special_22 ),
};

/* Char_Special 28 */
const Sint32 pl07_special_28[] = {
	CG_FRAME(    0,    0,    2,           0,   365, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 146 ),
	CG_FRAME(    0,    0,    2,           0,   366, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 146 ),
};

const Sint32 pl07_special_28_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   367, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 146 ),
	CG_NEXT( pl07_special_28_loop ),
};

/* Char_Special 25: barrage_start (Berserker Barrage, 25 + atck_level) */
const Sint32 pl07_special_25[] = {
	CG_FRAME(    0,    0,    2,           0,   368, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
	CG_FRAME(    0,    0,    2,           0,   369, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
};

const Sint32 pl07_special_25_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   370, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
	CG_NEXT( pl07_special_25_loop ),
};

/* Char_Special 31: barrage_move (Berserker Barrage, 31 + atck_level) */
const Sint32 pl07_special_31[] = {
	CG_FRAME(    0,    0,    3,           0,   397, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 148 ),
	CG_FRAME(    0,    0,    8,           0,   398, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 148 ),
	CG_FRAME(    0,    0,    4,           0,   399, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 148 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 148 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 148 ),
	CG_NEXT( pl07_special_31 ),
};

/* Char_Special 23
   Char_Special 24
   Char_Special 27
   Char_Special 30
   Char_Special 33 */
const Sint32 pl07_special_23[] = {
	CG_FRAME(    0,    0,   10,           0,   375, 0x90,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 155 ),
	CG_FRAME(    0,    1,    5, END_OF_DATA,    73, 0x80,   0x00,        ATK,  0x00, 0x15,    0,   0,     0,      0, 156 ),
	CG_NEXT( pl07_special_23 ),
};

/* Char_Special 29 */
const Sint32 pl07_special_29[] = {
	CG_FRAME(    0,    0,    3,           0,   365, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 157 ),
	CG_FRAME(    0,    0,    3,           0,   366, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 157 ),
};

const Sint32 pl07_special_29_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   367, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 157 ),
	CG_NEXT( pl07_special_29_loop ),
};

/* Char_Special 26 */
const Sint32 pl07_special_26[] = {
	CG_FRAME(    0,    0,    3,           0,   368, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 158 ),
	CG_FRAME(    0,    0,    3,           0,   369, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 158 ),
};

const Sint32 pl07_special_26_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   370, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 158 ),
	CG_NEXT( pl07_special_26_loop ),
};

/* Char_Special 32 */
const Sint32 pl07_special_32[] = {
	CG_FRAME(    0,    0,    5,           0,   397, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,    9,           0,   398, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,    7,           0,   399, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,    5,           0,    96, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 159 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 159 ),
	CG_NEXT( pl07_special_32 ),
};

/* Char_Super 5 */
const Sint32 pl07_super_05[] = {
	CG_FRAME(    0,    0,   16,           0,   375, 0xa0,   0x00,        ATK,  0x00, 0x0a,    0,   0,     0,      0, 160 ),
};

const Sint32 pl07_super_05_loop[] = {
	CG_FRAME(    0,    1,    1, END_OF_DATA,   375, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 160 ),
	CG_NEXT( pl07_super_05_loop ),
};

/* Char_Super 6 */
const Sint32 pl07_super_06[] = {
	CG_FRAME(    0,    0,    6,           0,   375, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 160 ),
	CG_FRAME( 0x80,    1,    4,           0,   375, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 160 ),
	CG_FRAME( 0x80,    0,    1,           0,   375, 0xa0,   0x00,        ATK,  0x00, 0x0a,    0,   0,     0,      0, 160 ),
};

const Sint32 pl07_super_06_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   375, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 160 ),
	CG_NEXT( pl07_super_06_loop ),
};

/* Char_Super 8 */
const Sint32 pl07_super_08[] = {
	CG_FRAME(    0,    0,    1,           0,   365, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 161 ),
	CG_FRAME(    0,    0,    1,           0,   366, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 161 ),
};

const Sint32 pl07_super_08_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   367, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 161 ),
	CG_NEXT( pl07_super_08_loop ),
};

/* Char_Super 7: barrage_x_move_init (Berserker Barrage X), barrage_x_move (Berserker Barrage X, 7 + timer1) */
const Sint32 pl07_super_07[] = {
	CG_FRAME(    0,    0,    1,           0,   368, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 162 ),
	CG_FRAME(    0,    0,    1,           0,   369, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 162 ),
};

const Sint32 pl07_super_07_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   370, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 162 ),
	CG_NEXT( pl07_super_07_loop ),
};

/* Char_Super 9 */
const Sint32 pl07_super_09[] = {
	CG_FRAME(    0,    0,    6,           0,   397, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 163 ),
	CG_FRAME(    0,    0,   10,           0,   398, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 163 ),
	CG_FRAME(    0,    0,    8,           0,   399, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 163 ),
	CG_FRAME(    0,    0,    6,           0,    96, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 163 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 163 ),
	CG_NEXT( pl07_super_09 ),
};

/* Char_Super 10: barrage_x_move (Berserker Barrage X) */
const Sint32 pl07_super_10[] = {
	CG_FRAME(    0,    0,    6,           0,   197, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 164 ),
	CG_FRAME(    0,    1,    4,           0,   198, 0xa0,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0, 165 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 166 ),
	CG_FRAME(    0,    0,   24,           0,   200, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 167 ),
	CG_FRAME(    0,    0,    8,           0,   201, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 167 ),
	CG_FRAME(    0,    0,    4,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 168 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 168 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 168 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 169 ),
	CG_NEXT( pl07_super_10 ),
};

/* Char_Super 0: weapon_x_init (Weapon X) */
const Sint32 pl07_super_00[] = {
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x00,        ATK,  0x00, 0x0a,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    2,           0,   136, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,        ATK,  0x00, 0x25,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    4,           0,   138, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    4,           0,   139, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x00,        ATK,  0x00, 0x21,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    2,    2,           0,   143, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    6,           0,   143, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 170 ),
	CG_FRAME(    0,    1,    2,           0,    71, 0x80,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 171 ),
};

const Sint32 pl07_super_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    71, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 171 ),
	CG_FRAME(    0,    0,    1,           0,    72, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 171 ),
	CG_FRAME(    0,    0,    3,           0,    73, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 171 ),
	CG_FRAME(    0,    0,    1,           0,    74, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 171 ),
	CG_FRAME(    0,    0,    1,           0,    75, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 171 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    76, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 171 ),
	CG_NEXT( pl07_super_00_loop ),
};

/* Char_Super 1: weapon_x_dash (Weapon X) */
const Sint32 pl07_super_01[] = {
	CG_FRAME(    1,   49,    2,           0,   182, 0xb0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 172 ),
	CG_FRAME(    0,    0,    2,           0,   181, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   182, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   183, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   184, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   185, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,   50,    2,           0,   186, 0xb0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0, 173 ),
	CG_FRAME(    0,    0,    2,           0,   187, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   188, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   189, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   190, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,   50,    2,           0,   192, 0xb0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 174 ),
	CG_FRAME(    0,    0,    2,           0,   193, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   202, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   51,    2,           0,   204, 0xb0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0, 175 ),
	CG_FRAME(    0,    0,    2,           0,   204, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   205, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   249, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   52,    2,           0,   250, 0xb0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 176 ),
	CG_FRAME(    0,    0,    2,           0,   251, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   251, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   53,    2,           0,   269, 0xb0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0, 177 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   252, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   54,    2,           0,   253, 0xb0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 178 ),
	CG_FRAME(    0,    0,    2,           0,   254, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   256, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   257, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    2,   54,    2,           0,   258, 0xb0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0, 179 ),
	CG_FRAME(    0,    0,    2,           0,   259, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   260, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   271, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,   56,    2,           0,   273, 0xb0,   0x00,        ATK,  0x00, 0x04,    0,   0,     0,      0, 180 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   274, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   261, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   262, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    3,   57,    2,           0,   264, 0xb0,   0x00,        ATK,  0x00, 0x03,    0,   0,     0,      0, 181 ),
	CG_FRAME(    0,    0,    2,           0,   265, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   266, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   267, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    94, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl07_super_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    95, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_super_01_loop ),
};

/* Char_Super 2: weapon_x_catch (Weapon X) */
const Sint32 pl07_super_02[] = {
	CG_FRAME(    0,    0,    2,           0,   371, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0xb0,   0x00,          0,  0x00, 0x06,    0,   0,     0,      0,   0 ),
};

const Sint32 pl07_super_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   371, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   372, 0xb0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_super_02_loop ),
};

/* Char_Super 3: weapon_x_dash (Weapon X), weapon_x_fall (Weapon X) */
const Sint32 pl07_super_03[] = {
	CG_FRAME(    0,    0,    5,           0,    94, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,    95, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,    38, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5,           0,    96, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_super_03 ),
};

/* Char_Super 4: weapon_x_dash (Weapon X) */
const Sint32 pl07_super_04[] = {
	CG_FRAME(    0,    0,    4,           0,   397, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,    9,           0,   398, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,    5,           0,   399, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 153 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 154 ),
	CG_NEXT( pl07_super_04 ),
};

/* Char_Super 11 */
const Sint32 pl07_super_11[] = {
	CG_FRAME(    0,    0,   10,           0,   197, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   10,           0,   197, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    2,    2,           0,   372, 0xb0,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0, 130 ),
};

const Sint32 pl07_super_11_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   371, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   372, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl07_super_11_loop ),
};

/* Char_Super 12 */
const Sint32 pl07_super_12[] = {
	CG_FRAME(    0,    0,    2,           0,   435, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   442, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   438, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   435, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   442, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   438, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    1,    2,           0,   442, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    2,    1, END_OF_DATA,   442, 0xb0,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl07_super_12 ),
};

/* Char_Super 13: fatal_jump (Fatal Claw) */
const Sint32 pl07_super_13[] = {
	CG_FRAME(    0,    0,    3,           0,   384, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   385, 0xb0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_NEXT( pl07_super_13 ),
};

/* Char_Special 36: slash_init (Berserker Slash, 36 + atck_level) */
const Sint32 pl07_special_36[] = {
	CG_FRAME(    0,    0,    3,           0,    38, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   197, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl07_special_36 ),
};

/* Char_Special 37
   Char_Special 38 */
const Sint32 pl07_special_37[] = {
	CG_FRAME(    0,    0,    3,           0,    38, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   11, END_OF_DATA,   197, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl07_special_37 ),
};

/* Char_Special 39: slash_start (Berserker Slash) */
const Sint32 pl07_special_39[] = {
	CG_FRAME(    0,    0,    2,           0,    72, 0x80,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,    73, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,    74, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    75, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl07_special_39 ),
};

/* Char_Special 40: slash_dash (Berserker Slash) */
const Sint32 pl07_special_40[] = {
	CG_FRAME(    0,    0,    5,           0,   197, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    1,           0,   197, 0xa0,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,    1,           0,   198, 0xa0,   0x00, ATK | 0x19,  0x00,    0,    0,   0,     0,      0, 214 ),
	CG_FRAME(    0,    0,    1,           0,   199, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 215 ),
	CG_FRAME(    0,    0,   12,           0,   200, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   201, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_NEXT( pl07_special_40 ),
};

/* Char_Special 41: slash_stop (Berserker Slash, 41 + atck_level) */
const Sint32 pl07_special_41[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl07_special_41 ),
};

/* Char_Special 42
   Char_Special 43 */
const Sint32 pl07_special_42[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl07_special_42 ),
};

const Sint32 pl07_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,    8,           0,   249, 0xa0,   0x0e,        ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   380, 0xa0,   0x0e,        ATK,  0x1e,    0,    0,   0,     0,      0, 203 ),
	CG_FRAME(    0,    0,    1,           0,   380, 0xa0,   0x0e,        ATK,  0x00,    0,    0,   0,     0,      0, 204 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   381, 0xa0,   0x0e,        ATK,  0x1e,    0,    0,   0,     0,      0, 204 ),
	0x00004c54,
};

const Sint32 pl07_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,    4,           0,    95, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x90,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x90,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  48 ),
	0x00004c80,
};

const Sint32 pl07_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,    97, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	0x00004cd4,
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl07_damage_00[] = {
	CG_FRAME(    0,    0,    5,           0,   391, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   392, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   393, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl07_damage_01[] = {
	CG_FRAME(    0,    0,    8,           0,   394, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    94, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    38, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    96, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    97, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl07_damage_02[] = {
	CG_FRAME(    0,    0,    8,           0,   395, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   396, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   397, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    96, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    97, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl07_damage_03[] = {
	CG_FRAME(    0,    0,    5,           0,   400, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   401, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    97, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl07_damage_04[] = {
	CG_FRAME(    0,    0,    8,           0,   402, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    38, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    96, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl07_damage_05[] = {
	CG_FRAME(    0,    0,    8,           0,   405, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   406, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   407, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    38, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    97, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl07_damage_06[] = {
	CG_FRAME(    0,    0,    6,           0,   409, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl07_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl07_damage_07[] = {
	CG_FRAME(    0,    0,   10,           0,   410, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x82,    0,    4,           0,   411, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl07_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl07_damage_08[] = {
	CG_FRAME(    0,    0,   10,           0,   413, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   414, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   415, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   412, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    95, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl07_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl07_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   416, 0x40,   0x0e,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl07_damage_11[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   417, 0x40,   0x0e,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl07_damage_12[] = {
	CG_FRAME( 0x82,    0,   10, END_OF_DATA,   418, 0x40,   0x0e,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl07_damage_13[] = {
	CG_FRAME(    0,    0,    2,           0,   433, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   434, 0x40,   0x60,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   433, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
};

const Sint32 pl07_damage_13_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   462, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_NEXT( pl07_damage_13_loop ),
};

/* Char_Damage 9: nml_k_slam, dm_sweep */
const Sint32 pl07_damage_09[] = {
	CG_FRAME(    0,    0,    5,           0,   400, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   420, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   421, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   429, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   433, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
};

const Sint32 pl07_damage_09_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   462, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_NEXT( pl07_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl07_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   423, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   424, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl07_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   426, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl07_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   428, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl07_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   430, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl07_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   432, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl07_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   423, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
};

/* Char_Damage 22 */
const Sint32 pl07_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   426, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
};

/* Char_Damage 23 */
const Sint32 pl07_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   428, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
};

/* Char_Damage 24 */
const Sint32 pl07_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   430, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    2,           0,   431, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
};

/* Char_Damage 25 */
const Sint32 pl07_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   432, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_25 ),
};

const Sint32 pl07_unused_04[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   425, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   427, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   464, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   465, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   468, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   469, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   471, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   474, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2,           0,   475, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   476, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	0x000052dc,
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl07_damage_26[] = {
	CG_FRAME(    0,    0,    3,           0,   433, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    3,           0,   434, 0x40,   0x60,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    3,           0,   433, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   462, 0x40,   0x80,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_NEXT( pl07_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl07_damage_27[] = {
	CG_FRAME(    0,    0,    2,           0,   435, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   436, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   437, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   438, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   441, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    2,           0,   442, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 191 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    89, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 192 ),
	CG_NEXT( pl07_damage_27 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl07_damage_33[] = {
	CG_FRAME(    0,    0,    3,           0,   159, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   148, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   211, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl07_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   443, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   444, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   445, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   446, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   447, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   448, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl07_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   449, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   450, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4,           0,   453, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   454, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 187 ),
	CG_NEXT( pl07_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl07_damage_31[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   462, 0x60,   0x8f,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_NEXT( pl07_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl07_dizzy_00[] = {
	CG_FRAME(    0,    0,    4,           0,   455, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   456, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   457, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   458, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   459, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   460, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   461, 0x50,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_dizzy_00 ),
};

/* Char_Stand_P 4: stand_punch_move */
const Sint32 pl07_stand_p_04[] = {
	CG_FRAME(    0,    0,    1,           0,   190, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0xa0,   0x00,        ATK,  0x00, 0x23,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    1,           0,   192, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 195 ),
	CG_FRAME(    0,    0,    1,           0,   193, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 195 ),
	CG_FRAME(    0,    0,    1,           0,   194, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   12,           0,   195, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   196, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl07_stand_p_04 ),
};

/* Char_Stand_K 4: stand_kick_move */
const Sint32 pl07_stand_k_04[] = {
	CG_FRAME( 0x80,    0,    1,           0,   208, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 197 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 197 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 196 ),
	CG_FRAME(    0,    0,    6,           0,   210, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0, 196 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
};

const Sint32 pl07_stand_k_04_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    38, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl07_stand_k_04_loop ),
};

/* Char_Crouch_P 5 */
const Sint32 pl07_crouch_p_05[] = {
	CG_FRAME(    0,    0,    6,           0,   375, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 198 ),
	CG_FRAME(    0,    1,    3,           0,   376, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0, 199 ),
	CG_FRAME(    0,    0,    3,           0,   377, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0, 199 ),
	CG_FRAME(    0,    0,    3,           0,   376, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0, 199 ),
	CG_FRAME(    0,    0,    3,           0,   377, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0, 199 ),
	CG_FRAME(    0,    0,    3,           0,   376, 0xa0,   0x40,        ATK,  0x00,    0,    0,   0,     0,      0, 199 ),
	CG_FRAME(    0,    0,    3,           0,   377, 0xa0,   0x40,        ATE,  0x00,    0,    0,   0,     0,      0, 217 ),
	CG_FRAME(    0,    2,    3,           0,   378, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   149, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   150, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   151, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_crouch_p_05 ),
};

const Sint32 pl07_unused_05[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3,           0,    94, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    95, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,    38, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    96, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    97, 0xb0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	0x00005984,
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl07_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   113, 0x30,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x30,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x30,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x30,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0,   0 ),
};

const Sint32 pl07_change_00_loop[] = {
	CG_FRAME(    0,   32,   26, END_OF_DATA,   116, 0x30,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl07_change_01[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   161, 0x90,   0x13,          0,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl07_change_01 ),
};

const Sint32 pl07_unused_06[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,    95, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   229, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   230, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   231, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   232, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   233, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   233, 0xa0,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	0x00005b0c,
};

const Sint32 pl07_unused_07[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    1,           0,   133, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   133, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   135, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   137, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   138, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   139, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	0x00005c00,
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl07_change_02[] = {
	CG_FRAME(    0,    0,    2,           0,    92, 0xa0,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    92, 0xa0,   0x20,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_change_02 ),
};

/* Char_Super 14: charge_init (Berserker Charge) */
const Sint32 pl07_super_14[] = {
	CG_FRAME(    0,    0,    4,           0,   167, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    8,           0,   168, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   169, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    2,    1,           0,   170, 0xa0,   0x20,        ATK,  0x00, 0x04,    0,   0,     0,      0, 216 ),
	CG_FRAME(    0,    0,    1,           0,   171, 0xa0,   0x20,        ATK,  0x00, 0x25,    0,   0,     0,      0, 216 ),
	CG_FRAME(    0,    0,    1,           0,   172, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   173, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   16,           0,   174, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    4,    4, END_OF_DATA,   196, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl07_super_14 ),
};

/* Char_Special 34: t_kick_init */
const Sint32 pl07_special_34[] = {
	CG_FRAME(    0,    0,    4,           0,   210, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    2,           0,   268, 0xa0,   0x20,        ATK,  0x00, 0x03,    0,   0,     0,      0, 205 ),
};

const Sint32 pl07_special_34_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   269, 0xa0,   0x20,        ATK,  0x00,    0,    0,   0,     0,      0, 205 ),
	CG_NEXT( pl07_special_34_loop ),
};

/* Char_Special 35: t_kick_move */
const Sint32 pl07_special_35[] = {
	CG_FRAME(    0,   36,    2,           0,   175, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   176, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   175, 0x40,   0x00,          0,  0x00, 0x06,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   176, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   177, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   178, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   177, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   178, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   179, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   180, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   100, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   101, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   102, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl07_special_35 ),
};

/* Char_Special 44: snapback_init (The snapback) */
const Sint32 pl07_special_44[] = {
	CG_FRAME(    0,    0,    8,           0,   197, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    2,    2,           0,   197, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    1,    2,           0,   198, 0xa0,   0x00,        ATK,  0x00, 0x06,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,    2,           0,   199, 0xa0,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   14,           0,   200, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    8,           0,   201, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,    38, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    97, 0xa0,   0x00,        ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_NEXT( pl07_special_44 ),
};

/* Char_Caught 0 */
const Sint32 pl07_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   391, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 185 ),
	CG_NEXT( pl07_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl07_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   394, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 185 ),
	CG_NEXT( pl07_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl07_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   395, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 185 ),
	CG_NEXT( pl07_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl07_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   400, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 185 ),
	CG_NEXT( pl07_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl07_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   402, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 185 ),
	CG_NEXT( pl07_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl07_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   405, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 185 ),
	CG_NEXT( pl07_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl07_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   409, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 186 ),
	CG_NEXT( pl07_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl07_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   410, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 186 ),
	CG_NEXT( pl07_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl07_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   413, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 186 ),
	CG_NEXT( pl07_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl07_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   391, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl07_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   394, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl07_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   395, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_10 ),
};

/* Char_Caught 12 */
const Sint32 pl07_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   419, 0x40,   0x00,          0,  0x00,    0,    0,   0,     0,      0, 190 ),
	CG_NEXT( pl07_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl07_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   423, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl07_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   426, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl07_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   428, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl07_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   430, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl07_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   432, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl07_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   443, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl07_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   449, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 188 ),
	CG_NEXT( pl07_caught_19 ),
};

/* Char_Caught 20
   Char_Caught 21 */
const Sint32 pl07_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   462, 0x40,   0x20,          0,  0x00,    0,    0,   0,     0,      0, 189 ),
	CG_NEXT( pl07_caught_20 ),
};

/* Char_Effect 2: shl07_00_init (2 + no) */
const Sint32 pl07_effect_02[] = {
	CG_FRAME(    0,    0,    1,           0,   488, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    1,           0,   489, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    1,           0,   490, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   491, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32773, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   498, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   499, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl07_effect_03[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32770, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32771, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl07_effect_04[] = {
	CG_FRAME(    0,    0,    1,           0, 32774, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    1,           0, 32776, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32777, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl07_effect_05[] = {
	CG_FRAME(    0,    0,    1,           0,   500, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   501, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   502, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   503, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32778, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32779, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   510, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   511, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_05 ),
};

/* Char_Effect 6
   Char_Effect 10
   Char_Effect 11
   Char_Effect 12
   Char_Effect 13 */
const Sint32 pl07_effect_06[] = {
	CG_FRAME(    0,    0,    2,           0,   488, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    2,           0,   489, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    2,           0,   490, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   491, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32772, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   498, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   499, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl07_effect_07[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32769, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32771, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl07_effect_08[] = {
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    2,           0, 32775, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 183 ),
	CG_FRAME(    0,    0,    2,           0, 32776, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32777, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl07_effect_09[] = {
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   502, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   503, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32778, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32779, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   511, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_09 ),
};

/* Char_Effect 0 */
const Sint32 pl07_effect_00[] = {
	CG_FRAME(    0,    0,   10,           0,   548, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 184 ),
	CG_FRAME(    0,    0,    5,           0,   549, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   550, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   551, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   552, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   553, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   554, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl07_effect_01[] = {
	CG_FRAME(    0,    0,   10,           0,   555, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   556, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   557, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   558, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   559, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   560, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   561, 0x80,   0x00,          0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_01 ),
};

/* Char_Effect 14: shl07_02_init (14 + type) */
const Sint32 pl07_effect_14[] = {
	CG_FRAME(    0,    0,    1,           0, 32780, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32780, 0x80,   0x00,        ATK,  0x45, 0x23,    0,   0,     0,      0, 207 ),
	CG_FRAME(    0,    0,    2,           0, 32781, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 208 ),
	CG_FRAME(    0,    0,    2,           0, 32782, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 208 ),
	CG_FRAME(    0,    1,    2,           0, 32783, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 208 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,        ATK,  0x44,    0,    0,   0,     0,      0, 208 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0, 208 ),
	CG_FRAME(    0,    0,    1,           0, 32786, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32787, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32788, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl07_effect_15[] = {
	CG_FRAME(    0,    0,    1,           0, 32780, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32780, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32781, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32782, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0, 32783, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32785, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32786, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32787, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32788, 0x80,   0x00,        ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl07_effect_15 ),
};

/* Char_Effect 16: shl07_02_part_init (16 + timer0) */
const Sint32 pl07_effect_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32782, 0x80,   0x00,        ATK,  0x46,    0,    0,   0,     0,      0, 209 ),
	CG_NEXT( pl07_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl07_effect_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32782, 0x80,   0x00,        ATK,  0x47,    0,    0,   0,     0,      0, 210 ),
	CG_NEXT( pl07_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl07_effect_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32782, 0x80,   0x00,        ATK,  0x48,    0,    0,   0,     0,      0, 211 ),
	CG_NEXT( pl07_effect_17 ),
};

/* Char_Effect 19 */
const Sint32 pl07_effect_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32782, 0x80,   0x00,        ATK,  0x49,    0,    0,   0,     0,      0, 212 ),
	CG_NEXT( pl07_effect_17 ),
};

/* group 27 0 */
const Sint32 pl07_grp27_00[] = {
	EF_FRAME(    0,    0,    8,           0,   512 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   513 ),
	CG_NEXT( pl07_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl07_grp27_01[] = {
	EF_FRAME(    0,    0,    6,           0,   514 ),
	EF_FRAME(    0,    0,    3,           0,   515 ),
	EF_FRAME(    0,    0,    3,           0,   516 ),
	EF_FRAME(    0,    0,    4,           0,   517 ),
	EF_FRAME(    0,    1,    4,           0,   518 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   519 ),
	CG_NEXT( pl07_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl07_grp27_02[] = {
	EF_FRAME(    0,    0,    4,           0,   520 ),
	EF_FRAME(    0,    0,    4,           0,   521 ),
	EF_FRAME(    0,    0,    4,           0,   522 ),
	EF_FRAME(    0,    0,    4,           0,   523 ),
	EF_FRAME(    0,    1,    4,           0,   524 ),
	EF_FRAME(    0,    0,    4,           0,   525 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   526 ),
	CG_NEXT( pl07_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl07_grp27_03[] = {
	EF_FRAME(    0,    0,    4,           0,   527 ),
	EF_FRAME(    0,    0,    4,           0,   528 ),
	EF_FRAME(    0,    0,    4,           0,   529 ),
	EF_FRAME(    0,    1,    4, END_OF_DATA,   530 ),
	CG_NEXT( pl07_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl07_grp27_04[] = {
	EF_FRAME(    0,    0,    3,           0,   531 ),
	EF_FRAME(    0,    0,    3,           0,   532 ),
	EF_FRAME(    0,    0,    3,           0,   533 ),
	EF_FRAME(    0,    0,    3,           0,   534 ),
	EF_FRAME(    0,    0,    3,           0,   535 ),
	EF_FRAME(    0,    1,    3,           0,   536 ),
	EF_FRAME(    0,    0,    3,           0,   537 ),
	EF_FRAME(    0,    0,    3,           0,   538 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   539 ),
	CG_NEXT( pl07_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl07_grp27_05[] = {
	EF_FRAME(    0,    0,    3,           0,   481 ),
};

const Sint32 pl07_grp27_05_loop[] = {
	EF_FRAME(    0,    0,    4,           0,   482 ),
	EF_FRAME(    0,    0,    4,           0,   483 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   484 ),
	CG_NEXT( pl07_grp27_05_loop ),
};

/* group 27 6: pls07_00_move5, pls07_00_move7 */
const Sint32 pl07_grp27_06[] = {
	EF_FRAME(    0,    0,    3,           0,   485 ),
	EF_FRAME(    0,    0,    3,           0,   486 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   487 ),
	CG_NEXT( pl07_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl07_grp27_07[] = {
	EF_FRAME(    0,    0,    4,           0,   540 ),
	EF_FRAME(    0,    0,    4,           0,   541 ),
	EF_FRAME(    0,    0,    4,           0,   542 ),
	EF_FRAME(    0,    0,    4,           0,   543 ),
	EF_FRAME(    0,    0,    4,           0,   544 ),
	EF_FRAME(    0,    0,    4,           0,   545 ),
	EF_FRAME(    0,    0,    4,           0,   546 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   547 ),
	CG_NEXT( pl07_grp27_07 ),
};
