/* Sabretooth (character 2b): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_2b      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_2b     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl2b.c) says which group
                   and number each move uses.
   pl2b_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl2b_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl2b_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl2b_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_2b
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_2b, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_2b */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl2b_stand_00[], pl2b_stand_02[], pl2b_stand_03[], pl2b_jump_00[],
	pl2b_jump_00_loop[], pl2b_jump_01[], pl2b_jump_01_loop[], pl2b_jump_02[], pl2b_jump_02_loop[],
	pl2b_jump_12[], pl2b_jump_12_loop[], pl2b_jump_03[], pl2b_jump_06[], pl2b_jump_06_loop[],
	pl2b_jump_07[], pl2b_jump_07_loop[], pl2b_jump_08[], pl2b_jump_08_loop[], pl2b_jump_09[],
	pl2b_jump_10[], pl2b_jump_11[], pl2b_dash_00[], pl2b_dash_00_loop[], pl2b_dash_02[],
	pl2b_dash_01[], pl2b_dash_01_loop[], pl2b_dash_03[], pl2b_crouch_00[], pl2b_crouch_01[],
	pl2b_crouch_02[], pl2b_turn_00[], pl2b_turn_01[], pl2b_guard_00[], pl2b_guard_01[],
	pl2b_guard_02[], pl2b_guard_03[], pl2b_guard_04[], pl2b_guard_05[], pl2b_guard_06[],
	pl2b_guard_07[], pl2b_stand_p_00[], pl2b_stand_p_01[], pl2b_stand_p_02[], pl2b_stand_k_00[],
	pl2b_stand_k_01[], pl2b_stand_k_02[], pl2b_air_guard_00[], pl2b_air_guard_01[],
	pl2b_air_guard_02[], pl2b_crouch_p_00[], pl2b_crouch_p_01[], pl2b_crouch_p_02[],
	pl2b_crouch_k_00[], pl2b_crouch_k_01[], pl2b_crouch_k_02[], pl2b_jump_p_00[], pl2b_jump_p_01[],
	pl2b_jump_p_02[], pl2b_jump_k_00[], pl2b_jump_k_01[], pl2b_jump_k_02[], pl2b_get_up_00[],
	pl2b_get_up_01[], pl2b_get_up_01_loop[], pl2b_get_up_02[], pl2b_get_up_02_loop[],
	pl2b_get_up_03[], pl2b_get_up_03_loop[], pl2b_damage_00[], pl2b_damage_01[], pl2b_damage_02[],
	pl2b_damage_03[], pl2b_damage_04[], pl2b_damage_05[], pl2b_damage_06[], pl2b_damage_07[],
	pl2b_damage_08[], pl2b_damage_10[], pl2b_damage_11[], pl2b_damage_12[], pl2b_damage_13[],
	pl2b_damage_13_loop[], pl2b_damage_09[], pl2b_damage_09_loop[], pl2b_damage_16[],
	pl2b_damage_17[], pl2b_damage_18[], pl2b_damage_19[], pl2b_damage_20[], pl2b_damage_21[],
	pl2b_damage_22[], pl2b_damage_23[], pl2b_damage_24[], pl2b_damage_25[], pl2b_damage_26[],
	pl2b_damage_27[], pl2b_damage_30[], pl2b_damage_32[], pl2b_damage_33[], pl2b_damage_33_loop[],
	pl2b_damage_28[], pl2b_damage_29[], pl2b_damage_31[], pl2b_dizzy_00[], pl2b_caught_00[],
	pl2b_caught_01[], pl2b_caught_02[], pl2b_caught_03[], pl2b_caught_04[], pl2b_caught_05[],
	pl2b_caught_06[], pl2b_caught_07[], pl2b_caught_08[], pl2b_caught_09[], pl2b_caught_10[],
	pl2b_caught_11[], pl2b_caught_12[], pl2b_caught_13[], pl2b_caught_14[], pl2b_caught_15[],
	pl2b_caught_16[], pl2b_caught_17[], pl2b_caught_18[], pl2b_caught_19[], pl2b_caught_20[],
	pl2b_caught_21[], pl2b_catch_00[], pl2b_catch_00_loop[], pl2b_catch_01[], pl2b_catch_02[],
	pl2b_catch_02_loop[], pl2b_catch_03[], pl2b_catch_03_loop[], pl2b_catch_04[],
	pl2b_catch_04_loop[], pl2b_effect_24[], pl2b_effect_25[], pl2b_special_00[],
	pl2b_special_00_loop[], pl2b_special_01[], pl2b_special_01_loop[], pl2b_special_02[],
	pl2b_special_04[], pl2b_special_04_loop[], pl2b_special_03[], pl2b_special_03_loop[],
	pl2b_special_05[], pl2b_special_06[], pl2b_special_06_loop[], pl2b_catch_05[],
	pl2b_catch_06_loop[], pl2b_catch_06[], pl2b_special_09[], pl2b_special_09_loop[],
	pl2b_special_10[], pl2b_special_10_loop[], pl2b_super_00[], pl2b_super_01[],
	pl2b_super_01_loop[], pl2b_super_02[], pl2b_super_03[], pl2b_super_03_loop[], pl2b_super_04[],
	pl2b_super_05[], pl2b_super_05_loop[], pl2b_super_06[], pl2b_super_06_loop[], pl2b_super_07[],
	pl2b_super_08[], pl2b_super_08_loop[], pl2b_super_09[], pl2b_super_09_loop[], pl2b_super_10[],
	pl2b_super_10_loop[], pl2b_super_11[], pl2b_super_12[], pl2b_super_12_loop[], pl2b_super_13[],
	pl2b_super_13_loop[], pl2b_super_14[], pl2b_super_15[], pl2b_extra_00[], pl2b_extra_01[],
	pl2b_extra_01_loop[], pl2b_special_11[], pl2b_special_11_loop[], pl2b_special_12[],
	pl2b_extra_02[], pl2b_extra_02_loop[], pl2b_extra_03[], pl2b_change_04[], pl2b_extra_04[],
	pl2b_extra_04_loop[], pl2b_extra_05[], pl2b_appear_00[], pl2b_appear_01[], pl2b_change_03[],
	pl2b_change_03_loop[], pl2b_change_00[], pl2b_change_02[], pl2b_change_01[], pl2b_unused_00[],
	pl2b_unused_01[], pl2b_win_00[], pl2b_win_00_loop[], pl2b_win_01[], pl2b_win_01_loop[],
	pl2b_win_02[], pl2b_win_02_loop[], pl2b_win_03[], pl2b_effect_00[], pl2b_effect_00_loop[],
	pl2b_effect_01[], pl2b_effect_02[], pl2b_effect_03[], pl2b_effect_04[], pl2b_effect_04_loop[],
	pl2b_effect_05[], pl2b_effect_05_loop[], pl2b_effect_06[], pl2b_effect_07[], pl2b_effect_08[],
	pl2b_effect_09[], pl2b_effect_10[], pl2b_effect_11[], pl2b_effect_11_loop[], pl2b_effect_12[],
	pl2b_effect_13[], pl2b_effect_14[], pl2b_effect_15[], pl2b_effect_16[], pl2b_effect_17[],
	pl2b_effect_18[], pl2b_effect_19[], pl2b_effect_20[], pl2b_effect_20_loop[], pl2b_effect_21[],
	pl2b_effect_22[], pl2b_effect_23[], pl2b_effect_26[], pl2b_effect_27[], pl2b_effect_27_loop[],
	pl2b_effect_28[], pl2b_effect_29[], pl2b_effect_29_loop[], pl2b_effect_30[],
	pl2b_effect_30_loop[];
extern const Sint32 *const pl2b_stand_tbl[], *const pl2b_jump_tbl[], *const pl2b_dash_tbl[],
	*const pl2b_crouch_tbl[], *const pl2b_turn_tbl[], *const pl2b_guard_tbl[],
	*const pl2b_air_guard_tbl[], *const pl2b_stand_p_tbl[], *const pl2b_stand_k_tbl[],
	*const pl2b_crouch_p_tbl[], *const pl2b_crouch_k_tbl[], *const pl2b_jump_p_tbl[],
	*const pl2b_jump_k_tbl[], *const pl2b_damage_tbl[], *const pl2b_dizzy_tbl[],
	*const pl2b_catch_tbl[], *const pl2b_caught_tbl[], *const pl2b_get_up_tbl[],
	*const pl2b_appear_tbl[], *const pl2b_win_tbl[], *const pl2b_extra_tbl[],
	*const pl2b_special_tbl[], *const pl2b_super_tbl[], *const pl2b_effect_tbl[],
	*const pl2b_change_tbl[], *const pl2b_grp25_tbl[], *const pl2b_fly_tbl[],
	*const pl2b_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_2b[16] = {
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
const Sint32 *const *const char_tbl_2b[] = {
	pl2b_stand_tbl, pl2b_jump_tbl, pl2b_dash_tbl, pl2b_crouch_tbl, pl2b_turn_tbl, pl2b_guard_tbl,
	pl2b_air_guard_tbl, pl2b_stand_p_tbl, pl2b_stand_k_tbl, pl2b_crouch_p_tbl, pl2b_crouch_k_tbl, pl2b_jump_p_tbl,
	pl2b_jump_k_tbl, pl2b_damage_tbl, pl2b_dizzy_tbl, pl2b_catch_tbl, pl2b_caught_tbl, pl2b_get_up_tbl,
	pl2b_appear_tbl, pl2b_win_tbl, pl2b_extra_tbl, pl2b_special_tbl, pl2b_super_tbl, pl2b_effect_tbl,
	pl2b_change_tbl, pl2b_grp25_tbl, pl2b_fly_tbl, pl2b_grp27_tbl, 0,
};

const Sint32 *const pl2b_stand_tbl[] = {
	pl2b_stand_00, pl2b_stand_00, pl2b_stand_02, pl2b_stand_03, pl2b_stand_00, pl2b_stand_00,
	pl2b_stand_02, pl2b_stand_03, 0,
};

const Sint32 *const pl2b_jump_tbl[] = {
	pl2b_jump_00, pl2b_jump_01, pl2b_jump_02, pl2b_jump_03, pl2b_jump_03, pl2b_jump_03,
	pl2b_jump_06, pl2b_jump_07, pl2b_jump_08, pl2b_jump_09, pl2b_jump_10, pl2b_jump_11,
	pl2b_jump_12, 0,
};

const Sint32 *const pl2b_dash_tbl[] = {
	pl2b_dash_00, pl2b_dash_01, pl2b_dash_02, pl2b_dash_03, 0,
};

const Sint32 *const pl2b_crouch_tbl[] = {
	pl2b_crouch_00, pl2b_crouch_01, pl2b_crouch_02, 0,
};

const Sint32 *const pl2b_turn_tbl[] = {
	pl2b_turn_00, pl2b_turn_01, 0,
};

const Sint32 *const pl2b_guard_tbl[] = {
	pl2b_guard_00, pl2b_guard_01, pl2b_guard_02, pl2b_guard_03, pl2b_guard_04, pl2b_guard_05,
	pl2b_guard_06, pl2b_guard_07, 0,
};

const Sint32 *const pl2b_air_guard_tbl[] = {
	pl2b_air_guard_00, pl2b_air_guard_01, pl2b_air_guard_02, 0,
};

const Sint32 *const pl2b_stand_p_tbl[] = {
	pl2b_stand_p_00, pl2b_stand_p_01, pl2b_stand_p_02, 0,
};

const Sint32 *const pl2b_stand_k_tbl[] = {
	pl2b_stand_k_00, pl2b_stand_k_01, pl2b_stand_k_02, 0,
};

const Sint32 *const pl2b_crouch_p_tbl[] = {
	pl2b_crouch_p_00, pl2b_crouch_p_01, pl2b_crouch_p_02, 0,
};

const Sint32 *const pl2b_crouch_k_tbl[] = {
	pl2b_crouch_k_00, pl2b_crouch_k_01, pl2b_crouch_k_02, 0,
};

const Sint32 *const pl2b_jump_p_tbl[] = {
	pl2b_jump_p_00, pl2b_jump_p_01, pl2b_jump_p_02, pl2b_jump_p_00, pl2b_jump_p_01, pl2b_jump_p_02,
	0,
};

const Sint32 *const pl2b_jump_k_tbl[] = {
	pl2b_jump_k_00, pl2b_jump_k_01, pl2b_jump_k_02, pl2b_jump_k_00, pl2b_jump_k_01, pl2b_jump_k_02,
	0,
};

const Sint32 *const pl2b_damage_tbl[] = {
	pl2b_damage_00, pl2b_damage_01, pl2b_damage_02, pl2b_damage_03, pl2b_damage_04, pl2b_damage_05,
	pl2b_damage_06, pl2b_damage_07, pl2b_damage_08, pl2b_damage_09, pl2b_damage_10, pl2b_damage_11,
	pl2b_damage_12, pl2b_damage_13, pl2b_damage_13, pl2b_damage_13, pl2b_damage_16, pl2b_damage_17,
	pl2b_damage_18, pl2b_damage_19, pl2b_damage_20, pl2b_damage_21, pl2b_damage_22, pl2b_damage_23,
	pl2b_damage_24, pl2b_damage_25, pl2b_damage_26, pl2b_damage_27, pl2b_damage_28, pl2b_damage_29,
	pl2b_damage_30, pl2b_damage_31, pl2b_damage_32, pl2b_damage_33, pl2b_damage_13, 0,
};

const Sint32 *const pl2b_dizzy_tbl[] = {
	pl2b_dizzy_00, 0,
};

const Sint32 *const pl2b_catch_tbl[] = {
	pl2b_catch_00, pl2b_catch_01, pl2b_catch_02, pl2b_catch_03, pl2b_catch_04, pl2b_catch_05,
	pl2b_catch_06, 0,
};

const Sint32 *const pl2b_caught_tbl[] = {
	pl2b_caught_00, pl2b_caught_01, pl2b_caught_02, pl2b_caught_03, pl2b_caught_04, pl2b_caught_05,
	pl2b_caught_06, pl2b_caught_07, pl2b_caught_08, pl2b_caught_09, pl2b_caught_10, pl2b_caught_11,
	pl2b_caught_12, pl2b_caught_13, pl2b_caught_14, pl2b_caught_15, pl2b_caught_16, pl2b_caught_17,
	pl2b_caught_18, pl2b_caught_19, pl2b_caught_20, pl2b_caught_21, 0,
};

const Sint32 *const pl2b_get_up_tbl[] = {
	pl2b_get_up_00, pl2b_get_up_01, pl2b_get_up_02, pl2b_get_up_03, 0,
};

const Sint32 *const pl2b_appear_tbl[] = {
	pl2b_appear_00, pl2b_appear_01, 0,
};

const Sint32 *const pl2b_win_tbl[] = {
	pl2b_win_00, pl2b_win_01, pl2b_win_02, pl2b_win_03, 0,
};

const Sint32 *const pl2b_extra_tbl[] = {
	pl2b_extra_00, pl2b_extra_01, pl2b_extra_02, pl2b_extra_03, pl2b_extra_04, pl2b_extra_05,
	0,
};

const Sint32 *const pl2b_special_tbl[] = {
	pl2b_special_00, pl2b_special_01, pl2b_special_02, pl2b_special_03, pl2b_special_04, pl2b_special_05,
	pl2b_special_06, pl2b_catch_05, pl2b_catch_06, pl2b_special_09, pl2b_special_10, pl2b_special_11,
	pl2b_special_12, 0,
};

const Sint32 *const pl2b_super_tbl[] = {
	pl2b_super_00, pl2b_super_01, pl2b_super_02, pl2b_super_03, pl2b_super_04, pl2b_super_05,
	pl2b_super_06, pl2b_super_07, pl2b_super_08, pl2b_super_09, pl2b_super_10, pl2b_super_11,
	pl2b_super_12, pl2b_super_13, pl2b_super_14, pl2b_super_15, 0,
};

const Sint32 *const pl2b_effect_tbl[] = {
	pl2b_effect_00, pl2b_effect_01, pl2b_effect_02, pl2b_effect_03, pl2b_effect_04, pl2b_effect_05,
	pl2b_effect_06, pl2b_effect_07, pl2b_effect_08, pl2b_effect_09, pl2b_effect_10, pl2b_effect_11,
	pl2b_effect_12, pl2b_effect_13, pl2b_effect_14, pl2b_effect_15, pl2b_effect_16, pl2b_effect_17,
	pl2b_effect_18, pl2b_effect_19, pl2b_effect_20, pl2b_effect_21, pl2b_effect_22, pl2b_effect_23,
	pl2b_effect_24, pl2b_effect_25, pl2b_effect_26, pl2b_effect_27, pl2b_effect_28, pl2b_effect_29,
	pl2b_effect_30, 0,
};

const Sint32 *const pl2b_change_tbl[] = {
	pl2b_change_00, pl2b_change_01, pl2b_change_02, pl2b_change_03, pl2b_change_04, 0,
};

const Sint32 *const pl2b_grp25_tbl[] = {
	pl2b_stand_00, pl2b_stand_00, pl2b_stand_00, pl2b_stand_00, 0,
};

const Sint32 *const pl2b_fly_tbl[] = {
	pl2b_stand_00, 0,
};

const Sint32 *const pl2b_grp27_tbl[] = {
	pl2b_stand_00, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1    se  zoom   unk  flag2   rival  box */

/* Char_Stand 0: appear_move, appear_ef_move, act_stand, appear_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0
   group 25 1 */
const Sint32 pl2b_stand_00[] = {
	CG_FRAME(    0,    0,   8,           0,    52, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    53, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    54, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    55, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    56, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    57, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    58, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    59, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    60, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    61, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    62, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    63, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    64, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl2b_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    67, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    68, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    69, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    70, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    73, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    74, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    75, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,    76, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl2b_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,    77, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    78, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    79, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    80, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    81, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    82, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    83, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    84, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    85, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    86, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    87, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    88, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl2b_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   110, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl2b_jump_01[] = {
	CG_FRAME(    0,    1,   2,           0,   109, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    96, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   100, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_01_loop ),
};

/* Char_Jump 2: weapon_jump (Weapon X Rush (hyper)) */
const Sint32 pl2b_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_02_loop ),
};

/* Char_Jump 12: act_wall_jump */
const Sint32 pl2b_jump_12[] = {
	CG_FRAME(    0,    1,   4,           0,   355, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_12_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   356, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_12_loop ),
};

/* Char_Jump 3: fang_fly (Wild Fang), weapon_end (Weapon X Rush (hyper)), jump_catch_land
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl2b_jump_03[] = {
	CG_FRAME(    0,    0,   6,           0,   114, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   115, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl2b_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   110, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_06_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl2b_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl2b_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   109, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_jump_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_08_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl2b_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_09 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl2b_jump_10[] = {
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_10 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl2b_jump_11[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   105, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl2b_dash_00[] = {
	CG_FRAME(    1,    1,   3,           0,    89, 0x80,   0x00,          0,  0x00, 0x06,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_dash_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    90, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl2b_dash_02[] = {
	CG_FRAME(    0,    0,  16,           0,    91, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl2b_dash_01[] = {
	CG_FRAME(    0,    1,   6,           0,   109, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,    92, 0x80,   0x00,          0,  0x00, 0x04,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_dash_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    93, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl2b_dash_03[] = {
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl2b_crouch_00[] = {
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   111, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   113, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl2b_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   113, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl2b_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl2b_turn_00[] = {
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   119, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   120, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl2b_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   122, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   124, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl2b_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl2b_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   127, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl2b_guard_02[] = {
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   125, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl2b_guard_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   126, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl2b_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl2b_guard_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   131, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl2b_guard_06[] = {
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   129, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl2b_guard_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl2b_stand_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   237, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   238, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   5,           0,   239, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   240, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   241, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   242, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  10 ),
	CG_NEXT( pl2b_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl2b_stand_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   245, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   247, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,   2,           0,   248, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   249, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,  10,           0,   250, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   252, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_NEXT( pl2b_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl2b_stand_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   253, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   256, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,   2,           0,   257, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   258, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   2,           0,   259, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   260, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   259, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   2,           0,   260, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   2,           0,   263, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME( 0x80,    0,   2,           0,   264, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,  10,           0,   265, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   252, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_NEXT( pl2b_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl2b_stand_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,   266, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  20 ),
	CG_FRAME( 0x80,    0,   2,           0,   267, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,  21 ),
	CG_FRAME( 0x80,    0,   2,           0,   268, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   2,           0,   267, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   268, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   119, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl2b_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl2b_stand_k_01[] = {
	CG_FRAME(    0,    0,   7,           0,   269, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,   2,           0,   270, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,   2,           0,   271, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,   2,           0,   272, 0x80,   0x00,        ATE,  0x00, 0x06,    0,    0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   2,           0,   271, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   6,           0,   269, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   119, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl2b_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl2b_stand_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   273, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   275, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,   2,           0,   276, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   8,           0,   276, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   6,           0,   277, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   5,           0,   278, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   115, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl2b_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl2b_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   133, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl2b_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   133, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl2b_air_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   134, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_air_guard_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl2b_crouch_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   280, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2,           0,   281, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   3,           0,   282, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   3,           0,   282, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   124, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl2b_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl2b_crouch_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   283, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   284, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   285, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   2,           0,   286, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  32 ),
	CG_FRAME( 0x80,    0,   2,           0,   287, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   9,           0,   288, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   5,           0,   289, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   290, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  33 ),
	CG_NEXT( pl2b_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl2b_crouch_p_02[] = {
	CG_FRAME(    0,    0,   8,           0,   291, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   292, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   293, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   294, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   1,           0,   295, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,   2,           0,   296, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,  10,           0,   297, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   291, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl2b_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl2b_crouch_k_00[] = {
	CG_FRAME( 0x80,    0,   4,           0,   298, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   2,           0,   299, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,   2,           0,   300, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   299, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   5,           0,   300, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   298, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  37 ),
	CG_NEXT( pl2b_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl2b_crouch_k_01[] = {
	CG_FRAME(    0,    0,   7,           0,   301, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   302, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   303, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   304, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   8,           0,   303, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   5,           0,   122, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   111, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl2b_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl2b_crouch_k_02[] = {
	CG_FRAME(    0,    0,   5,           0,   305, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   306, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   307, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   6,           0,   309, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   8,           0,   310, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   311, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   123, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   124, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl2b_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl2b_jump_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   313, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,  45 ),
	CG_FRAME( 0x80,    0,   2,           0,   314, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  45 ),
	CG_FRAME( 0x80,    0,   2,           0,   315, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  45 ),
	CG_FRAME( 0x80,    0,   2,           0,   315, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   338, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl2b_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl2b_jump_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   316, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   5,           0,   317, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,   2,           0,   318, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   319, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,   2,           0,   320, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6,           0,   321, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   330, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl2b_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl2b_jump_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   323, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   324, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   325, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   326, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   327, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   2,           0,   328, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   8,           0,   329, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   330, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl2b_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl2b_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   338, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   338, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   4,           0,   331, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   6,           0,   332, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   338, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   7 ),
	CG_NEXT( pl2b_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl2b_jump_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   333, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   5,           0,   334, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   335, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   2,           0,   336, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   8,           0,   336, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   5,           0,   337, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  55 ),
	CG_NEXT( pl2b_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl2b_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   339, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   7,           0,   340, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   2,           0,   341, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  58 ),
	CG_FRAME( 0x80,    0,   2,           0,   342, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  58 ),
	CG_FRAME(    0,    0,  12,           0,   343, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   338, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  57 ),
	CG_NEXT( pl2b_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl2b_get_up_00[] = {
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0xa0,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x1a,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x1d,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x1d,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x37,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   423, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl2b_get_up_01[] = {
	CG_FRAME(    0,    0,   1,           0,   193, 0x80,   0xa0,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   194, 0x80,   0x1a,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   114, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   115, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_get_up_01_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   116, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_get_up_01_loop ),
};

/* Char_Get_Up 2 */
const Sint32 pl2b_get_up_02[] = {
	CG_FRAME(    0,    0,   1,           0,   193, 0x80,   0xa0,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x1a,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   114, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   115, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_get_up_02_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   116, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_get_up_02_loop ),
};

/* Char_Get_Up 3 */
const Sint32 pl2b_get_up_03[] = {
	CG_FRAME(    0,    0,   1,           0,   193, 0x80,   0xa0,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x1a,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   201, 0x80,   0x18,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   114, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   115, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_get_up_03_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_get_up_03_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl2b_damage_00[] = {
	CG_FRAME(    0,    0,   1,           0,   413, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   414, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   420, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   423, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl2b_damage_01[] = {
	CG_FRAME(    0,    0,   2,           0,   415, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   416, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   423, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_damage_00 ),
};

/* Char_Damage 2 */
const Sint32 pl2b_damage_02[] = {
	CG_FRAME(    0,    0,   4,           0,   417, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl2b_damage_03[] = {
	CG_FRAME( 0x80,    0,   1,           0,   424, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   2,           0,   425, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl2b_damage_04[] = {
	CG_FRAME( 0x80,    0,   2,           0,   426, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   4,           0,   427, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl2b_damage_05[] = {
	CG_FRAME( 0x80,    0,   4,           0,   428, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,   6,           0,   429, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   422, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl2b_damage_06[] = {
	CG_FRAME( 0x80,    0,   2,           0,   430, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   2,           0,   431, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   112, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl2b_damage_07[] = {
	CG_FRAME( 0x80,    0,   3,           0,   432, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   5,           0,   433, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   112, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl2b_damage_08[] = {
	CG_FRAME( 0x80,    0,   4,           0,   434, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   3,           0,   435, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   3,           0,   436, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,   4,           0,   437, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   111, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   112, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl2b_damage_10[] = {
	CG_FRAME(    0,    0,   3,           0,   438, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5,           0,   439, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   440, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl2b_damage_11[] = {
	CG_FRAME(    0,    0,   2,           0,   441, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   442, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   444, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    93, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl2b_damage_12[] = {
	CG_FRAME(    0,    0,   3,           0,   445, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   448, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl2b_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x8d,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
};

const Sint32 pl2b_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   471, 0x80,   0x8d,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl2b_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl2b_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   457, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   467, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x54,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   469, 0x80,   0x54,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   470, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
};

const Sint32 pl2b_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   473, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl2b_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl2b_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   459, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl2b_damage_17[] = {
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   461, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl2b_damage_18[] = {
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   463, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl2b_damage_19[] = {
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   465, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl2b_damage_20[] = {
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   467, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl2b_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl2b_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl2b_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl2b_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   464, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   465, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl2b_damage_25[] = {
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   467, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl2b_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x54,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   469, 0x80,   0x54,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   470, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   472, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   473, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   472, 0x80,   0xab,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl2b_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl2b_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x54,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   101, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl2b_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   101, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_30 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl2b_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   474, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   475, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   101, 0x80,   0x0d,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl2b_damage_33[] = {
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_damage_33_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   114, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl2b_damage_33_loop ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl2b_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   476, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   477, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   478, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   481, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl2b_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   482, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   486, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   487, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl2b_damage_31[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   473, 0x80,   0xe8,          0,  0x00,    0,    0,    0,     0,      0,   4 ),
	CG_NEXT( pl2b_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl2b_dizzy_00[] = {
	CG_FRAME(    0,    0,   4,           0,   488, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   489, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   489, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_dizzy_00 ),
};

/* Char_Caught 0 */
const Sint32 pl2b_caught_00[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   413, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  60 ),
	CG_NEXT( pl2b_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl2b_caught_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   415, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  60 ),
	CG_NEXT( pl2b_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl2b_caught_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   417, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  60 ),
	CG_NEXT( pl2b_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl2b_caught_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   424, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  60 ),
	CG_NEXT( pl2b_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl2b_caught_04[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   426, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  60 ),
	CG_NEXT( pl2b_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl2b_caught_05[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   428, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  60 ),
	CG_NEXT( pl2b_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl2b_caught_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   430, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  61 ),
	CG_NEXT( pl2b_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl2b_caught_07[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   432, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  61 ),
	CG_NEXT( pl2b_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl2b_caught_08[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   434, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  61 ),
	CG_NEXT( pl2b_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl2b_caught_09[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   438, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  62 ),
	CG_NEXT( pl2b_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl2b_caught_10[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   441, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  62 ),
	CG_NEXT( pl2b_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl2b_caught_11[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   445, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  62 ),
	CG_NEXT( pl2b_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl2b_caught_12[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   455, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  63 ),
	CG_NEXT( pl2b_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl2b_caught_13[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   458, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  62 ),
	CG_NEXT( pl2b_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl2b_caught_14[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   460, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  64 ),
	CG_NEXT( pl2b_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl2b_caught_15[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   462, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  64 ),
	CG_NEXT( pl2b_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl2b_caught_16[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   464, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  64 ),
	CG_NEXT( pl2b_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl2b_caught_17[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   466, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  64 ),
	CG_NEXT( pl2b_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl2b_caught_18[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   476, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  62 ),
	CG_NEXT( pl2b_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl2b_caught_19[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   482, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  62 ),
	CG_NEXT( pl2b_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl2b_caught_20[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   473, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  65 ),
	CG_NEXT( pl2b_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl2b_caught_21[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   456, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  66 ),
	CG_NEXT( pl2b_caught_21 ),
};

/* Char_Catch 0: nml_catch_set */
const Sint32 pl2b_catch_00[] = {
	CG_FRAME(    0,    0,   8,           0,   344, 0x80,   0x00,          0,  0x00, 0x06,    0,    0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   3,           0,   345, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0002,   1 ),
};

const Sint32 pl2b_catch_00_loop[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   346, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0003,   1 ),
	CG_NEXT( pl2b_catch_00_loop ),
};

/* Char_Catch 1: nml_catch_grab */
const Sint32 pl2b_catch_01[] = {
	CG_FRAME(    0,    1,   2,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0004,   1 ),
	CG_FRAME(    2,    2,   2,           0,   350, 0x80,   0x00,          0,  0x00, 0x06,    0,    0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,  10,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,  10,           0,   346, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    1,   2,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0004,   1 ),
	CG_FRAME(    2,    2,   2,           0,   350, 0x80,   0x00,          0,  0x00, 0x06,    0,    0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,  10,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,  10,           0,   346, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    1,   2,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0004,   1 ),
	CG_FRAME(    2,    2,   2,           0,   350, 0x80,   0x00,          0,  0x00, 0x06,    0,    0,     0, 0x0005,   1 ),
	CG_FRAME(    0,    1,  10,           0,   349, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0004,   1 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0006,   1 ),
	CG_FRAME(    0,    0,   7,           0,   352, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0007,   1 ),
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0008,   1 ),
	CG_FRAME(    1,    0,  16,           0,   354, 0x80,   0x00,          0,  0x00, 0x03,    0,    0,     0, 0x0008,   1 ),
	CG_FRAME(    0,    0,   6,           0,   119, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   120, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x000a,   1 ),
	CG_NEXT( pl2b_catch_01 ),
};

/* Char_Catch 2: jump_catch_set */
const Sint32 pl2b_catch_02[] = {
	CG_FRAME(    0,    0,   6,           0,   387, 0x80,   0x00,          0,  0x00, 0x06,    0,    0,     0, 0x000b,   1 ),
	CG_FRAME(    0,    0,   3,           0,   388, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x000c,   1 ),
};

const Sint32 pl2b_catch_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   389, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x000d,   1 ),
	CG_NEXT( pl2b_catch_02_loop ),
};

/* Char_Catch 3: jump_catch_hold */
const Sint32 pl2b_catch_03[] = {
	CG_FRAME(    1,    0,   2,           0,   390, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x000e,   1 ),
};

const Sint32 pl2b_catch_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x000f,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   390, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x000e,   1 ),
	CG_NEXT( pl2b_catch_03_loop ),
};

/* Char_Catch 4: jump_catch_fall */
const Sint32 pl2b_catch_04[] = {
	CG_FRAME( 0x80,    1,   2,           0,   392, 0x80,   0x00,          0,  0x00, 0x03,    0,    0,     0,      0,   1 ),
	CG_FRAME( 0x80,    1,   2,           0,   393, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    1,    1,   6,           0,   392, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0013,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0015,   1 ),
};

const Sint32 pl2b_catch_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0, 0x0016,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   103, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0, 0x0016,   3 ),
	CG_NEXT( pl2b_catch_04_loop ),
};

/* Char_Effect 24: pls2b_05_move (24 + type) */
const Sint32 pl2b_effect_24[] = {
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   348, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl2b_effect_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   394, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_25 ),
};

/* Char_Special 0 */
const Sint32 pl2b_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   6,           0,    89, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    1,   2,           0,   378, 0x80,   0x00,        ATK,  0x00, 0x14,    0,    0,     0,      0,  75 ),
	CG_FRAME(  -27,    0,  12,           0,   377, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  76 ),
};

const Sint32 pl2b_special_00_loop[] = {
	CG_FRAME(  -32,    0,   8, END_OF_DATA,   377, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl2b_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl2b_special_01[] = {
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    0,  12,           0,    89, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    1,   2,           0,   378, 0x80,   0x00,        ATK,  0x00, 0x14,    0,    0,     0,      0,  75 ),
	CG_FRAME(  -53,    0,  16,           0,   377, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  76 ),
};

const Sint32 pl2b_special_01_loop[] = {
	CG_FRAME(  -58,    0,  16, END_OF_DATA,   377, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  76 ),
	CG_NEXT( pl2b_special_01_loop ),
};

/* Char_Special 2 */
const Sint32 pl2b_special_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   395, 0x20,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  77 ),
	CG_NEXT( pl2b_special_02 ),
};

/* Char_Special 4 */
const Sint32 pl2b_special_04[] = {
	CG_FRAME(    0,    0,  10,           0,    92, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    93, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   109, 0x80,   0x40,          0,  0x00,    0,    0, 0x40,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x40,          0,  0x00,    0,    0, 0x40,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x40,          0,  0x00,    0,    0, 0x40,     0,      0,   1 ),
};

const Sint32 pl2b_special_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0, 0x40,     0,      0,   2 ),
	CG_NEXT( pl2b_special_04_loop ),
};

/* Char_Special 3 */
const Sint32 pl2b_special_03[] = {
	CG_FRAME(    0,    0,   8,           0,    92, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,    93, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   109, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x40,          0,  0x00,    0,    0, 0x40,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x40,          0,  0x00,    0,    0, 0x40,     0,      0,   1 ),
};

const Sint32 pl2b_special_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,          0,  0x00,    0,    0, 0x40,     0,      0,   2 ),
	CG_NEXT( pl2b_special_03_loop ),
};

/* Char_Special 5: birdie_init (Birdie) */
const Sint32 pl2b_special_05[] = {
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   6,           0,   372, 0x80,   0x00,        ATK,  0x00, 0x17,    0,    0,     0,      0,  78 ),
	CG_FRAME(    0,    0,  12,           0,   373, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   4,           0,   374, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  78 ),
	CG_FRAME(    0,    1,  10,           0,   375, 0x80,   0x00,        ATK,  0x00, 0x1f,    0,    0,     0,      0,  78 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   372, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  78 ),
	CG_NEXT( pl2b_special_05 ),
};

/* Char_Special 6: fang_init (Wild Fang) */
const Sint32 pl2b_special_06[] = {
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x40,        ATK,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    1,  12,           0,   357, 0x80,   0x00,        ATK,  0x00, 0x15,    0,    0,     0,      0,  79 ),
};

const Sint32 pl2b_special_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   357, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  80 ),
	CG_NEXT( pl2b_special_06_loop ),
};

/* Char_Catch 5: fang_fly (Wild Fang)
   Char_Special 7 */
const Sint32 pl2b_catch_05[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   4,           0,   358, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   4,           0,   360, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    0,   3,           0,   361, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001b,   0 ),
};

const Sint32 pl2b_catch_06_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   362, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001c,   0 ),
	CG_NEXT( pl2b_catch_06_loop ),
};

/* Char_Catch 6: fang_land (Wild Fang)
   Char_Special 8 */
const Sint32 pl2b_catch_06[] = {
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,   3,           0,   364, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001e,   1 ),
	CG_FRAME(    0,    0,   3,           0,   365, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001f,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   365, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0, 0x001f,   1 ),
	CG_NEXT( pl2b_catch_06_loop ),
};

/* Char_Special 9: fang_throw (Wild Fang) */
const Sint32 pl2b_special_09[] = {
	CG_FRAME(    0,    0,   2,           0,   366, 0x80,   0x00,          0,  0x00, 0x03,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   367, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   368, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_special_09_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   370, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_special_09_loop ),
};

/* Char_Special 10: fang_fly (Wild Fang) */
const Sint32 pl2b_special_10[] = {
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
};

const Sint32 pl2b_special_10_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   105, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   3 ),
	CG_NEXT( pl2b_special_10_loop ),
};

/* Char_Super 0: clawx_start (Berserker Claw X (hyper)), clawx_pose (Berserker Claw X (hyper)) */
const Sint32 pl2b_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   399, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   1,           0,   401, 0x80,   0x00,          0,  0x00, 0x05,    0,    0,     0,      0,  83 ),
	CG_FRAME( 0x80,    1,   2,           0,   401, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  83 ),
	CG_FRAME( 0x82,    0,   2,           0,   402, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   423, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  83 ),
	CG_NEXT( pl2b_super_00 ),
};

/* Char_Super 1: clawx_ready (Berserker Claw X (hyper)) */
const Sint32 pl2b_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   5,           0,    89, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    1,   2,           0,   378, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  75 ),
};

const Sint32 pl2b_super_01_loop[] = {
	CG_FRAME(  -58,    0,  20, END_OF_DATA,   377, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  84 ),
	CG_NEXT( pl2b_super_01_loop ),
};

/* Char_Super 2 */
const Sint32 pl2b_super_02[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   395, 0x20,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  77 ),
	CG_NEXT( pl2b_super_02 ),
};

/* Char_Super 3 */
const Sint32 pl2b_super_03[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   5,           0,    89, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_FRAME(    0,    1,   2,           0,   396, 0x80,   0x00,        ATK,  0x30, 0x03,    0,    0,     0,      0,  77 ),
};

const Sint32 pl2b_super_03_loop[] = {
	CG_FRAME(  -58,    0,  20, END_OF_DATA,   395, 0x20,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  85 ),
	CG_NEXT( pl2b_super_03_loop ),
};

/* Char_Super 4 */
const Sint32 pl2b_super_04[] = {
	CG_FRAME(    0,    0,  24, END_OF_DATA,   377, 0x20,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  75 ),
	CG_NEXT( pl2b_super_04 ),
};

/* Char_Super 5: clawx_rush (Berserker Claw X (hyper)) */
const Sint32 pl2b_super_05[] = {
	CG_FRAME(    0,    0,   3,           0,    92, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   367, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  87 ),
};

const Sint32 pl2b_super_05_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   368, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  87 ),
	CG_NEXT( pl2b_super_05_loop ),
};

/* Char_Super 6 */
const Sint32 pl2b_super_06[] = {
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(  -53,    1,   2,           0,    90, 0x80,   0x00,        ATK,  0x00, 0x05,    0,    0,     0,      0,  86 ),
};

const Sint32 pl2b_super_06_loop[] = {
	CG_FRAME(  -58,    0,   2, END_OF_DATA,    90, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  86 ),
	CG_NEXT( pl2b_super_06_loop ),
};

/* Char_Super 7 */
const Sint32 pl2b_super_07[] = {
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  88 ),
	CG_FRAME(    0,   32,   2, END_OF_DATA,   404, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  88 ),
	CG_NEXT( pl2b_super_07 ),
};

/* Char_Super 8: clawx_rush (Berserker Claw X (hyper)), clawx_last (Berserker Claw X (hyper)) */
const Sint32 pl2b_super_08[] = {
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   109, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   111, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
};

const Sint32 pl2b_super_08_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   113, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_NEXT( pl2b_super_08_loop ),
};

/* Char_Super 9: hbirdie_init (Hyper Birdie) */
const Sint32 pl2b_super_09[] = {
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  16,           0,   372, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   1,           0,   372, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   373, 0x80,   0x00,        ATK,  0x00, 0x17,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   4,           0,   374, 0x80,   0x00,        ATK,  0x00, 0x1f,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   4,           0,   375, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  89 ),
};

const Sint32 pl2b_super_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   376, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  89 ),
	CG_NEXT( pl2b_super_09_loop ),
};

/* Char_Super 10: hbirdie_move (Hyper Birdie) */
const Sint32 pl2b_super_10[] = {
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x00,        ATE,  0x00, 0x11,    0,    0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  89 ),
};

const Sint32 pl2b_super_10_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   371, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_super_10_loop ),
};

/* Char_Super 11: weapon_start (Weapon X Rush (hyper)), weapon_pose (Weapon X Rush (hyper)) */
const Sint32 pl2b_super_11[] = {
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8,           0,   399, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,        ATK,  0x00, 0x05,    0,    0,     0,      0,  90 ),
	CG_FRAME( 0x80,    1,   2,           0,   401, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_FRAME( 0x82,    0,   2,           0,   402, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   423, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  90 ),
	CG_NEXT( pl2b_super_11 ),
};

/* Char_Super 12: weapon_ready (Weapon X Rush (hyper)) */
const Sint32 pl2b_super_12[] = {
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x40,          0,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x40,        ATK,  0x00,    0,    0,    0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   4,           0,   357, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0, 0x0017,  79 ),
};

const Sint32 pl2b_super_12_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   357, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0, 0x0017,  91 ),
	CG_NEXT( pl2b_super_12_loop ),
};

/* Char_Super 13: weapon_jump (Weapon X Rush (hyper)) */
const Sint32 pl2b_super_13[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   279, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   280, 0x80,   0x00,        ATK,  0x03, 0x06,    0,    0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   1,           0,   281, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   282, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   283, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   285, 0x80,   0x00,        ATK,  0x03, 0x04,    0,    0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   1,           0,   286, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3,           0,   287, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3,           0,   291, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   293, 0x80,   0x00,        ATK,  0x04, 0x03,    0,    0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   1,           0,   294, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   296, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   331, 0x80,   0x00,        ATK,  0x01, 0x06,    0,    0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   274, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   275, 0x80,   0x00,        ATK,  0x02, 0x03,    0,    0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,   3,           0,   276, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   277, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,        ATK,  0x03,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   238, 0x80,   0x00,        ATK,  0x00, 0x06,    0,    0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   246, 0x80,   0x00,        ATK,  0x03,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   247, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,   1,           0,   248, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   2,           0,   252, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   301, 0x80,   0x00,        ATK,  0x01,    0,    0,    0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   302, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   3,           0,   303, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   323, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   1,           0,   325, 0x80,   0x00,        ATK,  0x02,    0,    0,    0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   326, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   327, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   1,           0,   328, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   329, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x00,        ATK,  0x04,    0,    0,    0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  18 ),
	CG_FRAME( 0x80,    0,   1,           0,   263, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   254, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   255, 0x80,   0x00,        ATK,  0x05,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   256, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,   1,           0,   257, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   1,           0,   258, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   5,           0,    89, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   403, 0x80,   0x00,        ATK,  0x00, 0x16,    0,    0,     0,      0,   5 ),
};

const Sint32 pl2b_super_13_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   403, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   404, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  92 ),
	CG_NEXT( pl2b_super_13_loop ),
};

/* Char_Super 14: weapon_slide (Weapon X Rush (hyper)) */
const Sint32 pl2b_super_14[] = {
	CG_FRAME(    0,    0,  16,           0,    91, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   114, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl2b_super_14 ),
};

/* Char_Super 15: weapon_jump (Weapon X Rush (hyper)) */
const Sint32 pl2b_super_15[] = {
	CG_FRAME(    0,    0,  10,           0,   114, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   115, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   116, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_NEXT( pl2b_super_15 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl2b_extra_00[] = {
	CG_FRAME(    0,    0,  12, END_OF_DATA,   336, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  72 ),
	CG_NEXT( pl2b_extra_00 ),
};

/* Char_Extra 1: t_kick_jump */
const Sint32 pl2b_extra_01[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   400, 0x80,   0x00,        ATE,  0x00, 0x05,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_extra_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,   32,   2, END_OF_DATA,   402, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_extra_01_loop ),
};

/* Char_Special 11: crouch_hp_init (crouching heavy punch (gamma assist)) */
const Sint32 pl2b_special_11[] = {
	CG_FRAME(    0,    0,   6,           0,   260, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  73 ),
	CG_FRAME( 0x80,    0,   2,           0,   263, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,  11,           0,   265, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   6,           0,   265, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   5,           0,   252, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  14 ),
};

const Sint32 pl2b_special_11_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   125, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_special_11_loop ),
};

/* Char_Special 12: snapback_init (Snapback) */
const Sint32 pl2b_special_12[] = {
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   245, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   245, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   2,           0,   246, 0x80,   0x00,        ATK,  0x00, 0x04,    0,    0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   247, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,   2,           0,   248, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   2,           0,   249, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,  10,           0,   250, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6,           0,   251, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   252, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  13 ),
	CG_NEXT( pl2b_special_12 ),
};

/* Char_Extra 2: chouhatsu_init (Taunt), fly_start5 */
const Sint32 pl2b_extra_02[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   400, 0x80,   0x00,          0,  0x00, 0x05,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_extra_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_extra_02_loop ),
};

/* Char_Extra 3: chouhatsu_move (Taunt), fly_start5 */
const Sint32 pl2b_extra_03[] = {
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_extra_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl2b_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   400, 0x80,   0x00,          0,  0x00, 0x05,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_change_04 ),
};

/* Char_Extra 4 */
const Sint32 pl2b_extra_04[] = {
	CG_FRAME( 0x80,    0,   1,           0,   260, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  73 ),
	CG_FRAME( 0x80,    0,   2,           0,   263, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   8,           0,   265, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  19 ),
};

const Sint32 pl2b_extra_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   252, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  14 ),
	CG_NEXT( pl2b_extra_04_loop ),
};

/* Char_Extra 5: pl2b_upper */
const Sint32 pl2b_extra_05[] = {
	CG_FRAME(    0,    0,   5,           0,   291, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   293, 0x80,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,   2,           0,   294, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,   1,           0,   295, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   6,           0,   297, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   291, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   6 ),
	CG_NEXT( pl2b_extra_05 ),
};

/* Char_Appear 0 */
const Sint32 pl2b_appear_00[] = {
	CG_FRAME(    0,    0,  16,           0,   183, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x00,          0,  0x00, 0x05,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   191, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   423, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_appear_00 ),
};

/* Char_Appear 1 */
const Sint32 pl2b_appear_01[] = {
	CG_FRAME(    0,    0,   8,           0,   217, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   218, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   219, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   220, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   221, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   222, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   223, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   224, 0x80,   0x00,          0,  0x00, 0x0a,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   225, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_appear_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl2b_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   397, 0x80,   0x00,          0,  0x00, 0x05,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   398, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   399, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   400, 0x80,   0x00,          1,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   400, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_change_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   421, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_change_03_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl2b_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   371, 0x30,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x30,   0x00,        ATK,  0x00, 0x03,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x30,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   154, 0x30,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   182, 0x30,   0x00, ATK | 0x12,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  48, END_OF_DATA,   182, 0x30,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl2b_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   108, 0x80,   0x20,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_change_02 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl2b_change_01[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   405, 0x80,   0x6e,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_change_01 ),
};

const Sint32 pl2b_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   8,           0,   177, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   178, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   179, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   180, 0x80,   0x00,       0x10,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   181, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	0x00004e74,
};

const Sint32 pl2b_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   374, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   372, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	0x00004f04,
};

/* Char_Win 0 */
const Sint32 pl2b_win_00[] = {
	CG_FRAME(    0,    0,   8,           0,   177, 0x80,   0x00,       0x0c,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   178, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   179, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   180, 0x80,   0x00,          0,  0x01,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_win_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   181, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl2b_win_01[] = {
	CG_FRAME(    0,    1,   4,           0,   371, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   152, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   153, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   154, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   152, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_win_01_loop ),
};

/* Char_Win 2: win_move */
const Sint32 pl2b_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   155, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   158, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   159, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
};

const Sint32 pl2b_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   162, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_win_02_loop ),
};

/* Char_Win 3 */
const Sint32 pl2b_win_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   489, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   1 ),
	CG_NEXT( pl2b_win_03 ),
};

/* Char_Effect 0: shl2b_00_init */
const Sint32 pl2b_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0,   492, 0x80,   0x00,        ATK,  0x00, 0x18,    0,    0,     0,      0,  96 ),
};

const Sint32 pl2b_effect_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,   494, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,   495, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   497, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  96 ),
	CG_NEXT( pl2b_effect_00_loop ),
};

/* Char_Effect 1 */
const Sint32 pl2b_effect_01[] = {
	CG_FRAME(    0,    1,   4,           0,   498, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    2,   4,           0,   499, 0x80,   0x00,        ATK,  0x00, 0x1e,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   500, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl2b_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl2b_effect_02[] = {
	CG_FRAME(    0,    1,   4,           0,   508, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    2,   4,           0,   509, 0x80,   0x00,          0,  0x00, 0x1e,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   510, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl2b_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl2b_effect_03[] = {
	CG_FRAME(    0,    1,   4,           0,   511, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    2,   4,           0,   512, 0x80,   0x00,          0,  0x00, 0x1e,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   513, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl2b_effect_03 ),
};

/* Char_Effect 4: shl2b_00_move */
const Sint32 pl2b_effect_04[] = {
	CG_FRAME(    0,    0,   4,           0,   501, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   504, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_04_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   505, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   506, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   507, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_04_loop ),
};

/* Char_Effect 5: shl2b_00_move */
const Sint32 pl2b_effect_05[] = {
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   515, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   516, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_05_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   505, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   506, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   507, 0x80,   0x00,        ATE,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_05_loop ),
};

/* Char_Effect 6 */
const Sint32 pl2b_effect_06[] = {
	CG_FRAME(    0,    1,   1,           0,   498, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    2,   2,           0,   499, 0x80,   0x00,        ATK,  0x00, 0x1e,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   500, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl2b_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl2b_effect_07[] = {
	CG_FRAME(    0,    1,   1,           0,   508, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    2,   2,           0,   509, 0x80,   0x00,        ATK,  0x00, 0x1e,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   510, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl2b_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl2b_effect_08[] = {
	CG_FRAME(    0,    1,   1,           0,   511, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    2,   2,           0,   512, 0x80,   0x00,        ATK,  0x00, 0x1e,    0,    0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   513, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  97 ),
	CG_NEXT( pl2b_effect_08 ),
};

/* Char_Effect 9: shl2b_01_init */
const Sint32 pl2b_effect_09[] = {
	CG_FRAME(    0,    0,   2,           0, 32805, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0, 32806, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0, 32807, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  98 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32808, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  98 ),
	CG_NEXT( pl2b_effect_09 ),
};

/* Char_Effect 10: pls2b_00_init */
const Sint32 pl2b_effect_10[] = {
	CG_FRAME(    0,    0,   2,           0, 32768, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32770, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32784, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_10 ),
};

/* Char_Effect 11: pls2b_01_main */
const Sint32 pl2b_effect_11[] = {
	CG_FRAME(    0,    1,   2,           0,   529, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   530, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   531, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   532, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   533, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   534, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_11_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   535, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_11_loop ),
};

/* Char_Effect 12: pls2b_02_init (12 + anm) */
const Sint32 pl2b_effect_12[] = {
	CG_FRAME(    0,    1,   2,           0, 32785, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32786, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32787, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32788, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl2b_effect_13[] = {
	CG_FRAME(    0,    1,   2,           0, 32789, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32790, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32791, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32792, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl2b_effect_14[] = {
	CG_FRAME(    0,    1,   2,           0, 32793, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   545, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   546, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32794, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl2b_effect_15[] = {
	CG_FRAME(    0,    1,   2,           0, 32815, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32816, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32817, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32818, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl2b_effect_16[] = {
	CG_FRAME(    0,    1,   2,           0, 32811, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32812, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0, 32813, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32814, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl2b_effect_17[] = {
	CG_FRAME(    0,    1,   2,           0, 32819, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   573, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   574, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   575, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_17 ),
};

/* Char_Effect 18: shl2b_02_init (18 + type) */
const Sint32 pl2b_effect_18[] = {
	CG_FRAME(    0,    0,   4,           0,   550, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,   553, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,   554, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,   555, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   556, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,  99 ),
	CG_NEXT( pl2b_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl2b_effect_19[] = {
	CG_FRAME(    0,    0,   4,           0,   557, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   558, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   559, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   560, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   561, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   562, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   563, 0x80,   0x00,        ATK,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_19 ),
};

/* Char_Effect 20: pls2b_03_init */
const Sint32 pl2b_effect_20[] = {
	CG_FRAME(    0,    1,   8,           0, 32809, 0x00,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_20_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA, 32810, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_20_loop ),
};

/* Char_Effect 21: pls2b_04_init (21 + sub_type) */
const Sint32 pl2b_effect_21[] = {
	CG_FRAME(    0,    1,   1,           0, 32785, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32786, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32787, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA, 32788, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl2b_effect_22[] = {
	CG_FRAME(    0,    1,   1,           0, 32789, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32790, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0, 32791, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA, 32792, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_22 ),
};

/* Char_Effect 23: pls2b_04_move */
const Sint32 pl2b_effect_23[] = {
	CG_FRAME(    0,    1,   1,           0, 32793, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   545, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   546, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA, 32794, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_23 ),
};

/* Char_Effect 26: pls2b_06_win_run_init */
const Sint32 pl2b_effect_26[] = {
	CG_FRAME(    0,    0,   4,           0,   521, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   523, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_26 ),
};

/* Char_Effect 27: pls2b_06_win_run_move */
const Sint32 pl2b_effect_27[] = {
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,       0x0d,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_27_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   528, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_27_loop ),
};

/* Char_Effect 28: pls2b_06_win_side_init */
const Sint32 pl2b_effect_28[] = {
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   164, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   168, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_28 ),
};

/* Char_Effect 29: pls2b_06_win_side_move */
const Sint32 pl2b_effect_29[] = {
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,       0x0e,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   170, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   171, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   173, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   174, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   176, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_29_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   226, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_29_loop ),
};

/* Char_Effect 30: pls2b_06_appear_init */
const Sint32 pl2b_effect_30[] = {
	CG_FRAME(    0,    0,  16,           0,   226, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   227, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   228, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  24,           0,   229, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   230, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
};

const Sint32 pl2b_effect_30_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   231, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   232, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   233, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   234, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   235, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   236, 0x80,   0x00,          0,  0x00,    0,    0,    0,     0,      0,   0 ),
	CG_NEXT( pl2b_effect_30_loop ),
};
