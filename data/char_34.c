/* Sentinel (character 34): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_34      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_34     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl34.c) says which group
                   and number each move uses.
   pl34_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl34_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl34_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl34_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_34
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_34, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_34 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl34_stand_00[], pl34_stand_02[], pl34_stand_03[], pl34_stand_06[],
	pl34_stand_07[], pl34_jump_00[], pl34_jump_00_loop[], pl34_fly_00[], pl34_fly_00_loop[],
	pl34_jump_03[], pl34_jump_06[], pl34_jump_09[], pl34_fly_09[], pl34_fly_09_loop[], pl34_fly_08[],
	pl34_fly_08_loop[], pl34_fly_07[], pl34_fly_07_loop[], pl34_fly_06[], pl34_fly_06_loop[],
	pl34_fly_03[], pl34_fly_03_loop[], pl34_fly_04[], pl34_fly_04_loop[], pl34_fly_05[],
	pl34_fly_05_loop[], pl34_fly_02[], pl34_fly_02_loop[], pl34_dash_00[], pl34_dash_00_loop[],
	pl34_dash_02[], pl34_dash_01[], pl34_dash_01_loop[], pl34_dash_03[], pl34_crouch_00[],
	pl34_crouch_01[], pl34_crouch_02[], pl34_turn_00[], pl34_turn_01[], pl34_guard_00[],
	pl34_guard_01[], pl34_guard_03[], pl34_guard_02[], pl34_guard_04[], pl34_guard_05[],
	pl34_guard_07[], pl34_guard_06[], pl34_air_guard_00[], pl34_air_guard_01[], pl34_stand_p_00[],
	pl34_stand_p_01[], pl34_stand_p_02[], pl34_stand_p_02_loop[], pl34_stand_p_03[],
	pl34_stand_k_00[], pl34_stand_k_01[], pl34_stand_k_02[], pl34_stand_k_02_loop[],
	pl34_crouch_p_00[], pl34_crouch_p_01[], pl34_crouch_p_02[], pl34_crouch_p_02_loop[],
	pl34_crouch_p_03[], pl34_crouch_k_00[], pl34_crouch_k_01[], pl34_crouch_k_02[], pl34_jump_p_00[],
	pl34_jump_p_01[], pl34_jump_p_02[], pl34_jump_k_00[], pl34_jump_k_01[], pl34_jump_k_02[],
	pl34_get_up_00[], pl34_get_up_01[], pl34_get_up_02[], pl34_get_up_03[], pl34_damage_00[],
	pl34_damage_01[], pl34_damage_02[], pl34_damage_03[], pl34_damage_04[], pl34_damage_05[],
	pl34_damage_06[], pl34_damage_07[], pl34_damage_08[], pl34_damage_09[], pl34_damage_09_loop[],
	pl34_damage_10[], pl34_damage_11[], pl34_damage_12[], pl34_damage_13[], pl34_damage_16[],
	pl34_damage_17[], pl34_damage_18[], pl34_damage_19[], pl34_damage_20[], pl34_damage_21[],
	pl34_damage_22[], pl34_damage_23[], pl34_damage_24[], pl34_damage_25[], pl34_damage_26[],
	pl34_damage_27[], pl34_damage_31[], pl34_damage_32[], pl34_damage_28[], pl34_damage_29[],
	pl34_damage_30[], pl34_damage_34[], pl34_caught_00[], pl34_caught_01[], pl34_caught_02[],
	pl34_caught_03[], pl34_caught_04[], pl34_caught_05[], pl34_caught_06[], pl34_caught_07[],
	pl34_caught_08[], pl34_caught_09[], pl34_caught_10[], pl34_caught_11[], pl34_caught_12[],
	pl34_caught_13[], pl34_caught_14[], pl34_caught_15[], pl34_caught_16[], pl34_caught_17[],
	pl34_caught_18[], pl34_caught_19[], pl34_caught_20[], pl34_caught_21[], pl34_dizzy_00[],
	pl34_appear_00[], pl34_appear_00_loop[], pl34_appear_01[], pl34_extra_00[], pl34_extra_00_loop[],
	pl34_special_00[], pl34_special_00_loop[], pl34_special_01[], pl34_special_01_loop[],
	pl34_catch_00[], pl34_catch_00_loop[], pl34_special_03[], pl34_catch_01[], pl34_special_04[],
	pl34_special_06[], pl34_special_06_loop[], pl34_special_07[], pl34_special_09[],
	pl34_special_08[], pl34_special_10[], pl34_special_10_loop[], pl34_special_11[],
	pl34_special_13[], pl34_special_12[], pl34_special_14[], pl34_special_14_loop[],
	pl34_special_15[], pl34_special_15_loop[], pl34_catch_02[], pl34_catch_02_loop[],
	pl34_special_17[], pl34_special_18[], pl34_special_18_loop[], pl34_special_19[],
	pl34_special_20[], pl34_special_20_loop[], pl34_special_21[], pl34_special_22[],
	pl34_special_22_loop[], pl34_special_22_loop_loop[], pl34_super_04[], pl34_super_05[],
	pl34_super_06[], pl34_super_06_loop[], pl34_super_07[], pl34_super_07_loop[], pl34_super_08[],
	pl34_special_23[], pl34_special_24[], pl34_special_24_loop[], pl34_special_25[],
	pl34_special_25_loop[], pl34_special_26[], pl34_special_27[], pl34_special_27_loop[],
	pl34_special_28[], pl34_special_29[], pl34_extra_01[], pl34_special_30[], pl34_super_00[],
	pl34_super_01[], pl34_super_02[], pl34_effect_00[], pl34_unused_00[], pl34_effect_01[],
	pl34_effect_01_loop[], pl34_effect_02[], pl34_effect_03[], pl34_change_00[],
	pl34_change_00_loop[], pl34_change_01[], pl34_change_02[], pl34_change_02_loop[],
	pl34_change_03[], pl34_change_04[], pl34_grp25_00[];
extern const Sint32 *const pl34_stand_tbl[], *const pl34_jump_tbl[], *const pl34_dash_tbl[],
	*const pl34_crouch_tbl[], *const pl34_turn_tbl[], *const pl34_guard_tbl[],
	*const pl34_air_guard_tbl[], *const pl34_stand_p_tbl[], *const pl34_stand_k_tbl[],
	*const pl34_crouch_p_tbl[], *const pl34_crouch_k_tbl[], *const pl34_jump_p_tbl[],
	*const pl34_jump_k_tbl[], *const pl34_damage_tbl[], *const pl34_dizzy_tbl[],
	*const pl34_catch_tbl[], *const pl34_caught_tbl[], *const pl34_get_up_tbl[],
	*const pl34_appear_tbl[], *const pl34_extra_tbl[], *const pl34_special_tbl[],
	*const pl34_super_tbl[], *const pl34_effect_tbl[], *const pl34_change_tbl[],
	*const pl34_grp25_tbl[], *const pl34_fly_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_34[32] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 5120, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 5120, -48, -136, 0, 0, 0 },
	{ 1000, 1000, 5120, -64, -136, 0, 0, 0 },
	{ 1000, 1000, 5120, -56, -124, 0, 0, 0 },
	{ 1000, 1000, 5120, -24, -88, 0, 0, 0 },
	{ 1000, 1000, 5120, -48, -104, 0, 0, 0 },
	{ 1000, 1000, 5120, 0, 0, 0, 0, 0 },
	{ 1100, 1100, 0, 0, 0, 0, 0, 0 },
	{ 1200, 1200, 7936, 0, 0, 0, 0, 0 },
	{ 1300, 1300, 0, 0, 0, 0, 0, 0 },
	{ 1400, 1400, 7936, 0, 0, 0, 0, 0 },
	{ 1500, 1500, 0, 0, 0, 0, 0, 0 },
	{ 1600, 1600, 7936, 0, 0, 0, 0, 0 },
	{ 1600, 1600, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -5120, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -5120, -32, -128, 0, 0, 0 },
	{ 1000, 1000, -5120, -48, -96, 0, 0, 0 },
	{ 1000, 1000, -5120, -48, -112, 0, 0, 0 },
	{ 1000, 1000, -5120, -32, -112, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_34[] = {
	pl34_stand_tbl, pl34_jump_tbl, pl34_dash_tbl, pl34_crouch_tbl, pl34_turn_tbl, pl34_guard_tbl,
	pl34_air_guard_tbl, pl34_stand_p_tbl, pl34_stand_k_tbl, pl34_crouch_p_tbl, pl34_crouch_k_tbl, pl34_jump_p_tbl,
	pl34_jump_k_tbl, pl34_damage_tbl, pl34_dizzy_tbl, pl34_catch_tbl, pl34_caught_tbl, pl34_get_up_tbl,
	pl34_appear_tbl, pl34_appear_tbl, pl34_extra_tbl, pl34_special_tbl, pl34_super_tbl, pl34_effect_tbl,
	pl34_change_tbl, pl34_grp25_tbl, pl34_fly_tbl, 0,
};

const Sint32 *const pl34_stand_tbl[] = {
	pl34_stand_00, pl34_stand_00, pl34_stand_02, pl34_stand_03, pl34_stand_00, pl34_stand_00,
	pl34_stand_06, pl34_stand_07, pl34_stand_02, pl34_stand_03, 0,
};

const Sint32 *const pl34_jump_tbl[] = {
	pl34_jump_00, pl34_jump_00, pl34_jump_00, pl34_jump_03, pl34_jump_03, pl34_jump_03,
	pl34_jump_06, pl34_jump_06, pl34_jump_06, pl34_jump_09, pl34_jump_09, pl34_jump_09,
	0,
};

const Sint32 *const pl34_dash_tbl[] = {
	pl34_dash_00, pl34_dash_01, pl34_dash_02, pl34_dash_03, 0,
};

const Sint32 *const pl34_crouch_tbl[] = {
	pl34_crouch_00, pl34_crouch_01, pl34_crouch_02, 0,
};

const Sint32 *const pl34_turn_tbl[] = {
	pl34_turn_00, pl34_turn_01, 0,
};

const Sint32 *const pl34_guard_tbl[] = {
	pl34_guard_00, pl34_guard_01, pl34_guard_02, pl34_guard_03, pl34_guard_04, pl34_guard_05,
	pl34_guard_06, pl34_guard_07, 0,
};

const Sint32 *const pl34_air_guard_tbl[] = {
	pl34_air_guard_00, pl34_air_guard_01, pl34_air_guard_01, 0,
};

const Sint32 *const pl34_stand_p_tbl[] = {
	pl34_stand_p_00, pl34_stand_p_01, pl34_stand_p_02, pl34_stand_p_03, 0,
};

const Sint32 *const pl34_stand_k_tbl[] = {
	pl34_stand_k_00, pl34_stand_k_01, pl34_stand_k_02, 0,
};

const Sint32 *const pl34_crouch_p_tbl[] = {
	pl34_crouch_p_00, pl34_crouch_p_01, pl34_crouch_p_02, pl34_crouch_p_03, 0,
};

const Sint32 *const pl34_crouch_k_tbl[] = {
	pl34_crouch_k_00, pl34_crouch_k_01, pl34_crouch_k_02, 0,
};

const Sint32 *const pl34_jump_p_tbl[] = {
	pl34_jump_p_00, pl34_jump_p_01, pl34_jump_p_02, pl34_jump_p_00, pl34_jump_p_01, pl34_jump_p_02,
	0,
};

const Sint32 *const pl34_jump_k_tbl[] = {
	pl34_jump_k_00, pl34_jump_k_01, pl34_jump_k_02, pl34_jump_k_00, pl34_jump_k_01, pl34_jump_k_02,
	0,
};

const Sint32 *const pl34_damage_tbl[] = {
	pl34_damage_00, pl34_damage_01, pl34_damage_02, pl34_damage_03, pl34_damage_04, pl34_damage_05,
	pl34_damage_06, pl34_damage_07, pl34_damage_08, pl34_damage_09, pl34_damage_10, pl34_damage_11,
	pl34_damage_12, pl34_damage_13, pl34_damage_13, pl34_damage_13, pl34_damage_16, pl34_damage_17,
	pl34_damage_18, pl34_damage_19, pl34_damage_20, pl34_damage_21, pl34_damage_22, pl34_damage_23,
	pl34_damage_24, pl34_damage_25, pl34_damage_26, pl34_damage_27, pl34_damage_28, pl34_damage_29,
	pl34_damage_30, pl34_damage_31, pl34_damage_32, pl34_damage_32, pl34_damage_34, 0,
};

const Sint32 *const pl34_dizzy_tbl[] = {
	pl34_dizzy_00, 0,
};

const Sint32 *const pl34_catch_tbl[] = {
	pl34_catch_00, pl34_catch_01, pl34_catch_02, 0,
};

const Sint32 *const pl34_caught_tbl[] = {
	pl34_caught_00, pl34_caught_01, pl34_caught_02, pl34_caught_03, pl34_caught_04, pl34_caught_05,
	pl34_caught_06, pl34_caught_07, pl34_caught_08, pl34_caught_09, pl34_caught_10, pl34_caught_11,
	pl34_caught_12, pl34_caught_13, pl34_caught_14, pl34_caught_15, pl34_caught_16, pl34_caught_17,
	pl34_caught_18, pl34_caught_19, pl34_caught_20, pl34_caught_21, 0,
};

const Sint32 *const pl34_get_up_tbl[] = {
	pl34_get_up_00, pl34_get_up_01, pl34_get_up_02, pl34_get_up_03, 0,
};

const Sint32 *const pl34_appear_tbl[] = {
	pl34_appear_00, pl34_appear_01, 0,
};

const Sint32 *const pl34_extra_tbl[] = {
	pl34_extra_00, pl34_extra_01, 0,
};

const Sint32 *const pl34_special_tbl[] = {
	pl34_special_00, pl34_special_01, pl34_catch_00, pl34_special_03, pl34_special_04, pl34_catch_01,
	pl34_special_06, pl34_special_07, pl34_special_08, pl34_special_09, pl34_special_10, pl34_special_11,
	pl34_special_12, pl34_special_13, pl34_special_14, pl34_special_15, pl34_catch_02, pl34_special_17,
	pl34_special_18, pl34_special_19, pl34_special_20, pl34_special_21, pl34_special_22, pl34_special_23,
	pl34_special_24, pl34_special_25, pl34_special_26, pl34_special_27, pl34_special_28, pl34_special_29,
	pl34_special_30, 0,
};

const Sint32 *const pl34_super_tbl[] = {
	pl34_super_00, pl34_super_01, pl34_super_02, pl34_special_22, pl34_super_04, pl34_super_05,
	pl34_super_06, pl34_super_07, pl34_super_08, 0,
};

const Sint32 *const pl34_effect_tbl[] = {
	pl34_effect_00, pl34_effect_01, pl34_effect_02, pl34_effect_03, 0,
};

const Sint32 *const pl34_change_tbl[] = {
	pl34_change_00, pl34_change_01, pl34_change_02, pl34_change_03, pl34_change_04, 0,
};

const Sint32 *const pl34_grp25_tbl[] = {
	pl34_grp25_00, 0,
};

const Sint32 *const pl34_fly_tbl[] = {
	pl34_fly_00, pl34_jump_09, pl34_fly_02, pl34_fly_03, pl34_fly_04, pl34_fly_05,
	pl34_fly_06, pl34_fly_07, pl34_fly_08, pl34_fly_09, pl34_jump_09, pl34_jump_09,
	pl34_jump_09, pl34_jump_09, pl34_jump_09, pl34_jump_09, pl34_jump_09, pl34_jump_09,
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl34_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,    40, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    40, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  22, END_OF_DATA,    40, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 8 */
const Sint32 pl34_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0xab,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    43, 0x80,   0xcd,    0,  0x01, 0x1f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    44, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x8e,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x0d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x6d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    48, 0x80,   0x8d,    0,  0x01, 0x1f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    49, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    50, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x31,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    52, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 9 */
const Sint32 pl34_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x4d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x31,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    50, 0x80,   0x8d,    0,  0x01, 0x1f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    49, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x6d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x0d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    45, 0x80,   0x8e,    0,  0x01, 0x1f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    44, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   7,           0,    43, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    53, 0x80,   0xab,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl34_stand_06[] = {
	CG_FRAME(    0,    0,  16,           0,    42, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,  16,           0,    44, 0x80,   0xcd,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    45, 0x80,   0x8e,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl34_stand_07[] = {
	CG_FRAME(    0,    1,  16,           0,    49, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,    48, 0x80,   0x8d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,    47, 0x80,   0x6d,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_stand_07 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl34_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    58, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x12,    0,  0x00, 0x2f,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_jump_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_jump_00_loop ),
};

/* Char_Fly 0: flight_init (Flight) */
const Sint32 pl34_fly_00[] = {
	CG_FRAME(    0,    1,   4,           0,    58, 0x80,   0x32,    0,  0x00, 0x30,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   281, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   282, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_00_loop ),
};

/* Char_Jump 3: appear_fall
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl34_jump_03[] = {
	CG_FRAME(    0,    0,   1,           0,    63, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    66, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl34_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    58, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x12,    0,  0x00, 0x2f,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover
   Char_Fly 1: act_fly
   Char_Fly 10
   Char_Fly 11 */
const Sint32 pl34_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_jump_09 ),
};

/* Char_Fly 9 */
const Sint32 pl34_fly_09[] = {
	CG_FRAME(    0,    0,   1,           0,   283, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_09_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   284, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   283, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_09_loop ),
};

/* Char_Fly 8 */
const Sint32 pl34_fly_08[] = {
	CG_FRAME(    0,    0,   1,           0,   285, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_08_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   286, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   285, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_08_loop ),
};

/* Char_Fly 7 */
const Sint32 pl34_fly_07[] = {
	CG_FRAME(    0,    0,   1,           0,   287, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_07_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   288, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   287, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_07_loop ),
};

/* Char_Fly 6: appear_init */
const Sint32 pl34_fly_06[] = {
	CG_FRAME(    0,    0,   1,           0,   289, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_06_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   290, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   289, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_06_loop ),
};

/* Char_Fly 3 */
const Sint32 pl34_fly_03[] = {
	CG_FRAME(    0,    0,   1,           0,   275, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_03_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   276, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   275, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_03_loop ),
};

/* Char_Fly 4 */
const Sint32 pl34_fly_04[] = {
	CG_FRAME(    0,    0,   1,           0,   277, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_04_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   278, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   277, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_04_loop ),
};

/* Char_Fly 5 */
const Sint32 pl34_fly_05[] = {
	CG_FRAME(    0,    0,   1,           0,   279, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_05_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   280, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   279, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_05_loop ),
};

/* Char_Fly 2 */
const Sint32 pl34_fly_02[] = {
	CG_FRAME(    0,    0,   1,           0,   281, 0x80,   0x11,    0,  0x00, 0x30,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_fly_02_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   282, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   281, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_fly_02_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl34_dash_00[] = {
	CG_FRAME(    0,    0,   2,           0,    54, 0x80,   0x6e,    0,  0x00, 0x2f,    0,   0,     0,      0,   4 ),
};

const Sint32 pl34_dash_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    55, 0x80,   0x6e,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    54, 0x80,   0x6e,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl34_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl34_dash_02[] = {
	CG_FRAME(    0,    0,   1,           0,    63, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    66, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl34_dash_01[] = {
	CG_FRAME(    0,    0,   2,           0,    56, 0x80,   0x55,    0,  0x00, 0x2f,    0,   0,     0,      0,  45 ),
};

const Sint32 pl34_dash_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    57, 0x80,   0x55,    0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    56, 0x80,   0x55,    0,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl34_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl34_dash_03[] = {
	CG_FRAME(    0,    0,   1,           0,    63, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    66, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl34_crouch_00[] = {
	CG_FRAME(    5,    0,   3,           0,    67, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    4,    0,   2,           0,    68, 0x80,   0x12,    0,  0x00, 0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   2,           0,    69, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    2,    0,   3,           0,    70, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    41, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl34_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    41, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl34_crouch_02[] = {
	CG_FRAME(    8,    0,   1,           0,    41, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    8,    0,   1,           0,    41, 0x80,   0x70,    0,  0x00, 0x22,    0,   0,     0,      0,   2 ),
	CG_FRAME(    6,    0,   2,           0,    70, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    5,    0,   2,           0,    69, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    4,    0,   2,           0,    68, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    3,    0,   2, END_OF_DATA,    67, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl34_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0xd0,    0,  0x00,    0,    1,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,    72, 0x80,   0xcf,    0,  0x00,    0,    1,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,    73, 0x80,   0x6e,    0,  0x00,    0,    1,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    1,   0,     0,      0,  63 ),
	CG_NEXT( pl34_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl34_turn_01[] = {
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0xf0,    0,  0x00,    0,    1,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0xd5,    0,  0x00,    0,    1,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x70,    0,  0x00,    0,    1,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    41, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl34_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl34_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,    77, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl34_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_guard_01 ),
};

/* Char_Guard 3 */
const Sint32 pl34_guard_03[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    79, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_guard_03 ),
};

/* Char_Guard 2 */
const Sint32 pl34_guard_02[] = {
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    77, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl34_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl34_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    82, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_guard_05 ),
};

/* Char_Guard 7 */
const Sint32 pl34_guard_07[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,    82, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_guard_07 ),
};

/* Char_Guard 6 */
const Sint32 pl34_guard_06[] = {
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    80, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl34_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,    83, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    84, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl34_air_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    85, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl34_stand_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   130, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl34_stand_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   124, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   132, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x32,  ATK,  0x00, 0x25,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    1,   3,           0,   126, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   8,           0,   132, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   5,           0,   131, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   130, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl34_stand_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   134, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   137, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   136, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   1,           0,   137, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   139, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   140, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   141, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   142, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    8,   2,           0,   150, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    8,   2,           0,   151, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    6,   2,           0,   152, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    1,    0,   3,           0,   146, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   154, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl34_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl34_stand_p_02_loop ),
};

/* Char_Stand_P 3: stand_punch_move */
const Sint32 pl34_stand_p_03[] = {
	CG_FRAME(  127,    2,   3,           0,   161, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    4,   3,           0,   162, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    6,   2,           0,   163, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    8,   2,           0,   160, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   138, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   145, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_stand_p_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl34_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   168, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   5,           0,   165, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   164, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl34_stand_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0x32,  ATK,  0x00, 0x25,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   1,           0,   166, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   3,           0,   167, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   168, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   164, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl34_stand_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x3f,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x3f,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   8,           0,   172, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    1,   2,           0,   172, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME( 0x80,    2,   2,           0,   173, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    3,   2,           0,   175, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   177, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,  12,           0,   176, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x31,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl34_stand_k_02_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   169, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_stand_k_02_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl34_crouch_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   179, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   188, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   8,           0,   188, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   187, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl34_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl34_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   180, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x70,  ATK,  0x00, 0x25,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    1,   3,           0,   182, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   184, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   4,           0,   185, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   8,           0,   188, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   187, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl34_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl34_crouch_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   190, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   193, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   192, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   193, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   195, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   196, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   197, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   198, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   205, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   206, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    8,   1,           0,   207, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    6,   1,           0,   208, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    2,   3,           0,   202, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    1,    0,   3,           0,   203, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   204, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
};

const Sint32 pl34_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   214, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   215, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl34_crouch_p_02_loop ),
};

/* Char_Crouch_P 3: crouch_punch_move */
const Sint32 pl34_crouch_p_03[] = {
	CG_FRAME(    0,    2,   3,           0,   216, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    4,   3,           0,   217, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    6,   3,           0,   218, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    8,   3,           0,   196, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   198, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   193, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   200, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   199, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   191, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   189, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   201, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl34_crouch_p_03 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl34_crouch_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   222, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0xa7,  ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   8,           0,   234, 0x80,   0xa7,  ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   220, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl34_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl34_crouch_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   5,           0,   224, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0xa7,  ATK,  0x00, 0x25,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,   18,   2,           0,   227, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,  10,           0,   234, 0x80,   0xa7,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl34_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl34_crouch_k_02[] = {
	CG_FRAME(    0,    0,   1,           0,   220, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x70,  ATK,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0xa7,  ATK,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   5,           0,   224, 0x80,   0xa7,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0xa7,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0xa0,  ATK,  0x00, 0x25,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,   21,   2,           0,   227, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,   22,   2,           0,   230, 0x80,   0xe0,  ATK,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0xe0,  ATE,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   2,           0,   232, 0x80,   0xc0,  ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x80,   0xa0,  ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,  16,           0,   234, 0x80,   0xa0,  ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x70,  ATE,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl34_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl34_jump_p_00[] = {
	CG_FRAME(    0,    0,   5,           0,   235, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x12,  ATK,  0x00, 0x25,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,  10,           0,   238, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   240, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl34_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl34_jump_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   235, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x12,  ATK,  0x00, 0x25,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,  10,           0,   238, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   8,           0,   240, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   240, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl34_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl34_jump_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   241, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   245, 0x80,   0x12,  ATK,  0x00, 0x25,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   246, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,   2,           0,   247, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,  16,           0,   249, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   244, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   241, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl34_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl34_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   253, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   5,           0,   255, 0x80,   0x12,  ATK,  0x00, 0x25,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   2,           0,   256, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   8,           0,   257, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   258, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   253, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl34_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl34_jump_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   259, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   4,           0,   261, 0x80,   0x12,  ATK,  0x00, 0x25,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   2,           0,   262, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,  10,           0,   263, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3,           0,   265, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   6,           0,   264, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   259, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl34_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl34_jump_k_02[] = {
	CG_FRAME(    0,    0,   8,           0,   266, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0x12,  ATK,  0x00, 0x25,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,   2,           0,   271, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   2,           0,   269, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   2,           0,   272, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   2,           0,   269, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   2,           0,   272, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   6,           0,   270, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   266, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl34_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl34_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,    86, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    90, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,    91, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,    92, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl34_get_up_01[] = {
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl34_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl34_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    93, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    89, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl34_damage_00[] = {
	CG_FRAME(    0,    2,   7,           0,   377, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl34_damage_01[] = {
	CG_FRAME( 0x80,    0,   8,           0,   334, 0x80,   0x78,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   335, 0x80,   0x78,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl34_damage_02[] = {
	CG_FRAME( 0x82,    0,   8,           0,   337, 0x80,   0x52,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   336, 0x80,   0x52,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   335, 0x80,   0x77,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl34_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   338, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl34_damage_04[] = {
	CG_FRAME(    0,    0,  12,           0,   339, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl34_damage_05[] = {
	CG_FRAME(    0,    0,  12,           0,   340, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   342, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    39, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl34_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   344, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   343, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl34_damage_07[] = {
	CG_FRAME(    0,    0,   8,           0,   345, 0x80,   0x75,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   344, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   346, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl34_damage_08[] = {
	CG_FRAME(    0,    0,  10,           0,   348, 0x80,   0x7e,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   347, 0x80,   0x7e,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   349, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   346, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl34_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl34_damage_09[] = {
	CG_FRAME(    0,    0,  14,           0,   358, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   359, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl34_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   362, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl34_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl34_damage_10[] = {
	CG_FRAME(    0,    0,   5,           0,   351, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   350, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl34_damage_11[] = {
	CG_FRAME( 0x82,    0,   5,           0,   353, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   352, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl34_damage_12[] = {
	CG_FRAME(    0,    1,   5,           0,   354, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   355, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl34_damage_13[] = {
	CG_FRAME( 0x80,    0,   4,           0,   356, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   357, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl34_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl34_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   364, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl34_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   365, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl34_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   366, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl34_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   367, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl34_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   368, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl34_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl34_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl34_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   366, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl34_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   367, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl34_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   368, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl34_damage_26[] = {
	CG_FRAME(    0,    0,   6,           0,   369, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   370, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl34_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl34_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   380, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x80,   0x2a,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   382, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   385, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl34_damage_31[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   357, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl34_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl34_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   380, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x80,   0x2a,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   382, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl34_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   371, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   372, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   373, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   374, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   375, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   376, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl34_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   386, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   387, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   388, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   389, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   390, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   391, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl34_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   378, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   380, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   382, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_30 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl34_damage_34[] = {
	CG_FRAME( 0x80,    0,   4,           0,   356, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   357, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_damage_34 ),
};

/* Char_Caught 0 */
const Sint32 pl34_caught_00[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   377, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl34_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl34_caught_01[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   334, 0x80,   0x78,    0,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl34_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl34_caught_02[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   337, 0x80,   0x52,    0,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl34_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl34_caught_03[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   338, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl34_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl34_caught_04[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   339, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl34_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl34_caught_05[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   340, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl34_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl34_caught_06[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   344, 0x80,   0x70,    0,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl34_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl34_caught_07[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   345, 0x80,   0x75,    0,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl34_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl34_caught_08[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   348, 0x80,   0x7e,    0,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl34_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl34_caught_09[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   351, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl34_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl34_caught_10[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   353, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl34_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl34_caught_11[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   354, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl34_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl34_caught_12[] = {
	CG_FRAME( 0x80,    0,   2, END_OF_DATA,   356, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl34_caught_13[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   363, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl34_caught_14[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   365, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl34_caught_15[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   366, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl34_caught_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   367, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl34_caught_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   368, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl34_caught_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   371, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl34_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   386, 0x80,   0x11,    0,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl34_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl34_caught_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   357, 0x80,   0xf2,    0,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl34_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl34_caught_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl34_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl34_dizzy_00[] = {
	CG_FRAME(    0,   15,   4, END_OF_DATA,   377, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_dizzy_00 ),
};

/* Char_Appear 0
   Char_Win 0: win_pose */
const Sint32 pl34_appear_00[] = {
	CG_FRAME(    0,   15,   8,           0,    95, 0x80,   0x32,    0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   14,   8,           0,    97, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   13,   3,           0,    94, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   4,           0,   104, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   4,           0,   105, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   4,           0,   106, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   4,           0,   106, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0,   107, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   4,           0,   107, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   108, 0x80,   0x10,    0,  0x00, 0x23,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   4,           0,   109, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   109, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   4,           0,   110, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   111, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   112, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,    99, 0x80,   0x10,    0,  0x00, 0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   100, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   101, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   103, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   116, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   117, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   118, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   119, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   120, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   121, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   122, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,   4,           0,   112, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl34_appear_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   112, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_appear_00_loop ),
};

/* Char_Appear 1
   Char_Win 1: lose_pose */
const Sint32 pl34_appear_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   377, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_appear_01 ),
};

/* Char_Extra 0: pl34_upper */
const Sint32 pl34_extra_00[] = {
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x3f,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x3f,  ATK,  0x01,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   8,           0,   172, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    1,   2,           0,   172, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME( 0x80,    2,   2,           0,   173, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    3,   2,           0,   175, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   177, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,  12,           0,   176, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x31,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl34_extra_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   169, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_extra_00_loop ),
};

/* Char_Special 0: rocket_init, nml_catch_set */
const Sint32 pl34_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   308, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   3,           0,   309, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    2,   1,           0,   310, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    3,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    4,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    5,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    6,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    7,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    8,   1,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    9,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    1,   10,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   11,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   12,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   11,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   12,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   11,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   12,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   11,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   12,   2,           0,   311, 0x80,   0x4e,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl34_special_00_loop[] = {
	CG_FRAME(    0,   11,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   12,   2, END_OF_DATA,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl34_special_00_loop ),
};

/* Char_Special 1: nml_catch_wait */
const Sint32 pl34_special_01[] = {
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    1,    1,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    2,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    3,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    4,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    5,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    6,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    7,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    8,   2,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    9,   3,           0,   311, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,   10,   3,           0,   308, 0x80,   0x4e,  ATE,  0x00,    0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl34_special_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    39, 0x80,   0x32,  ATE,  0x01,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_special_01_loop ),
};

/* Char_Catch 0
   Char_Special 2 */
const Sint32 pl34_catch_00[] = {
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    1,   3,           0,   294, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    2,   1,           0,   295, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    3,   1,           0,   296, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    4,   1,           0,   297, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    5,   1,           0,   298, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    6,   1,           0,   299, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    7,   1,           0,   300, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    8,   1,           0,   301, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    9,   2,           0,   302, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    1,   10,   2,           0,   303, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000b,   0 ),
};

const Sint32 pl34_catch_00_loop[] = {
	CG_FRAME(    0,   11,   2,           0,   306, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,   12,   2, END_OF_DATA,   307, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_NEXT( pl34_catch_00_loop ),
};

/* Char_Special 3 */
const Sint32 pl34_special_03[] = {
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   302, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   301, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   300, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   2,           0,   299, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   298, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   2,           0,   297, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   2,           0,   296, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   2,           0,   295, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   3,           0,   294, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,   3, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_03 ),
};

/* Char_Catch 1
   Char_Special 5 */
const Sint32 pl34_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000e,  61 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   305, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x000f,  61 ),
	CG_NEXT( pl34_catch_01 ),
};

/* Char_Special 4 */
const Sint32 pl34_special_04[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32770, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_04 ),
};

/* Char_Special 6 */
const Sint32 pl34_special_06[] = {
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,  ATK,  0x00,    0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,  ATK,  0x00,    0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   298, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   300, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   301, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   303, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
};

const Sint32 pl34_special_06_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   306, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   307, 0x80,   0x00,  ATK,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_06_loop ),
};

/* Char_Special 7 */
const Sint32 pl34_special_07[] = {
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   302, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   301, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,  ATE,  0x00,    0,    5,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_07 ),
};

/* Char_Special 9 */
const Sint32 pl34_special_09[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   305, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,  62 ),
	CG_NEXT( pl34_special_09 ),
};

/* Char_Special 8 */
const Sint32 pl34_special_08[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32770, 0x80,   0x00,  ATK,  0x00,    0,    2,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_08 ),
};

/* Char_Special 10 */
const Sint32 pl34_special_10[] = {
	CG_FRAME(    0,    0,   3,           0,   293, 0x80,   0x00,  ATK,  0x00,    0,   17,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   298, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   300, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   301, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   303, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
};

const Sint32 pl34_special_10_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   306, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   307, 0x80,   0x00,  ATK,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_10_loop ),
};

/* Char_Special 11 */
const Sint32 pl34_special_11[] = {
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   302, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   301, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,  ATE,  0x00,    0,   18,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   294, 0x80,   0x00,  ATE,  0x00,    0,   17,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   293, 0x80,   0x00,  ATE,  0x00,    0,   17,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_11 ),
};

/* Char_Special 13 */
const Sint32 pl34_special_13[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,  ATK,  0x00,    0,   16,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   305, 0x80,   0x00,  ATK,  0x00,    0,   16,   0,     0,      0,  75 ),
	CG_NEXT( pl34_special_13 ),
};

/* Char_Special 12 */
const Sint32 pl34_special_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,   16,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32770, 0x80,   0x00,  ATK,  0x00,    0,   16,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_12 ),
};

/* Char_Special 14: rocket_init, jump_catch_set */
const Sint32 pl34_special_14[] = {
	CG_FRAME(    0,    0,   3,           0,   326, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,   2,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    2,   1,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    3,   1,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    4,   1,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    5,   1,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    6,   1,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    7,   1,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    8,   1,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    9,   1,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,   10,   2,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   11,   2,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   12,   2,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   11,   2,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   12,   2,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   11,   2,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   12,   2,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   11,   2,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   12,   2,           0,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_special_14_loop[] = {
	CG_FRAME(    0,   11,   2,           0,   328, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   12,   2, END_OF_DATA,   327, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_special_14_loop ),
};

/* Char_Special 15: jump_catch_wait */
const Sint32 pl34_special_15[] = {
	CG_FRAME(    0,    0,   2,           0,   329, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    1,   2,           0,   327, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    2,   2,           0,   328, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    3,   2,           0,   327, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    4,   2,           0,   328, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    5,   2,           0,   327, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    6,   2,           0,   328, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    7,   2,           0,   327, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    8,   2,           0,   328, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    9,   3,           0,   327, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   10,   3,           0,   328, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_special_15_loop[] = {
	CG_FRAME(    0,   10,   1, END_OF_DATA,    61, 0x80,   0x12,  ATE,  0x01,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_special_15_loop ),
};

/* Char_Catch 2
   Char_Special 16 */
const Sint32 pl34_catch_02[] = {
	CG_FRAME(    0,    0,   3,           0,   313, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    1,   2,           0,   314, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    2,   1,           0,   315, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    3,   1,           0,   316, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    4,   1,           0,   317, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    5,   1,           0,   318, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    6,   1,           0,   319, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    7,   1,           0,   320, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    8,   1,           0,   321, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    9,   1,           0,   322, 0x80,   0x00,  ATK,  0x01,    0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    1,   10,   2,           0,   323, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl34_catch_02_loop[] = {
	CG_FRAME(    0,   11,   2,           0,   324, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,   2, END_OF_DATA,   325, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_catch_02_loop ),
};

/* Char_Special 17 */
const Sint32 pl34_special_17[] = {
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   322, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   321, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   320, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   2,           0,   319, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   318, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   2,           0,   317, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   2,           0,   316, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   3,           0,   314, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,   3, END_OF_DATA,   313, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_17 ),
};

/* Char_Special 18 */
const Sint32 pl34_special_18[] = {
	CG_FRAME(    0,    0,   3,           0,   313, 0x80,   0x00,  ATK,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   1,           0,   315, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   1,           0,   316, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   1,           0,   317, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   1,           0,   318, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   1,           0,   319, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   1,           0,   320, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   1,           0,   321, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   1,           0,   322, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    1,   10,   2,           0,   323, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
};

const Sint32 pl34_special_18_loop[] = {
	CG_FRAME(    0,   11,   2,           0,   324, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,   2, END_OF_DATA,   325, 0x80,   0x00,  ATK,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_18_loop ),
};

/* Char_Special 19 */
const Sint32 pl34_special_19[] = {
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   322, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   321, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   320, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   2,           0,   319, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   318, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   2,           0,   317, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   2,           0,   316, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   3,           0,   314, 0x80,   0x00,  ATE,  0x00,    0,    7,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,   3, END_OF_DATA,   313, 0x80,   0x00,  ATE,  0x00,    0,    6,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_19 ),
};

/* Char_Special 20 */
const Sint32 pl34_special_20[] = {
	CG_FRAME(    0,    0,   3,           0,   313, 0x80,   0x00,  ATK,  0x00,    0,   19,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   314, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   1,           0,   315, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   1,           0,   316, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   1,           0,   317, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   1,           0,   318, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   1,           0,   319, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   1,           0,   320, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   1,           0,   321, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   1,           0,   322, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    1,   10,   2,           0,   323, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
};

const Sint32 pl34_special_20_loop[] = {
	CG_FRAME(    0,   11,   2,           0,   324, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,   12,   2, END_OF_DATA,   325, 0x80,   0x00,  ATK,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_20_loop ),
};

/* Char_Special 21 */
const Sint32 pl34_special_21[] = {
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    1,    1,   2,           0,   322, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,   2,           0,   321, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   320, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,   2,           0,   319, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   318, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,   2,           0,   317, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,   2,           0,   316, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    8,   2,           0,   315, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,    9,   3,           0,   314, 0x80,   0x00,  ATE,  0x00,    0,   20,   0,     0,      0,   0 ),
	CG_FRAME(    0,   10,   3, END_OF_DATA,   313, 0x80,   0x00,  ATE,  0x00,    0,   19,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_21 ),
};

/* Char_Special 22: force_init (Sentinel Force)
   Char_Super 3: hforce_init (Hyper Sentinel Force (hyper)) */
const Sint32 pl34_special_22[] = {
	CG_FRAME(    0,    0,   2,           0,   291, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0, 32768, 0x80,   0x2d,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32768, 0x80,   0x2d,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl34_special_22_loop ),
};

const Sint32 pl34_special_22_loop[] = {
	CG_FRAME(    0,    0,  16,           0, 32768, 0x80,   0x2d,  ATK,  0x00,    0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl34_special_22_loop_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA, 32768, 0x80,   0x2d,  ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl34_special_22_loop_loop ),
};

/* Char_Super 4: hforce_fire (Hyper Sentinel Force (hyper)) */
const Sint32 pl34_super_04[] = {
	CG_FRAME(    0,    0,  16,           0, 32768, 0x80,   0x2d,    0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   291, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_super_04 ),
};

/* Char_Super 5: hdrive_init (Hard Drive (hyper)) */
const Sint32 pl34_super_05[] = {
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x12,  ATK,  0x00, 0x15,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x12,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   281, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_super_05 ),
};

/* Char_Super 6: hdrive_wait (Hard Drive (hyper)) */
const Sint32 pl34_super_06[] = {
	CG_FRAME(    0,    0,   1,           0,   283, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    1,   1,           0,   286, 0x80,   0x11,  ATK,  0x00, 0x2f,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,   1,           0,   285, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    1,   1,           0,   286, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,   1,           0,   285, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl34_super_06_loop[] = {
	CG_FRAME(    1,    1,   1,           0,   286, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,   1,           0,   285, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    1,    1,   1,           0,   286, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   285, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl34_super_06_loop ),
};

/* Char_Super 7: hdrive_rush (Hard Drive (hyper)) */
const Sint32 pl34_super_07[] = {
	CG_FRAME(    0,    0,   1,           0,   286, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   286, 0x80,   0x11,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl34_super_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_super_07_loop ),
};

/* Char_Super 8: hdrive_fall (Hard Drive (hyper)) */
const Sint32 pl34_super_08[] = {
	CG_FRAME(    0,    0,   1,           0,    63, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x12,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    65, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    66, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_super_08 ),
};

/* Char_Special 23: shl34_01_init */
const Sint32 pl34_special_23[] = {
	CG_FRAME(    0,    0,   2,           0, 32798, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0, 32799, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32800, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl34_special_23 ),
};

/* Char_Special 24: shl34_01_start */
const Sint32 pl34_special_24[] = {
	CG_FRAME(    0,    0,   4,           0, 32801, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl34_special_24_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32804, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32803, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl34_special_24_loop ),
};

/* Char_Special 25: shl34_01_fire */
const Sint32 pl34_special_25[] = {
	CG_FRAME(    0,    0,   4,           0, 32804, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl34_special_25_loop[] = {
	CG_FRAME(    0,    0,   4,           0, 32798, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32800, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_special_25_loop ),
};

/* Char_Special 26: shl34_02_init */
const Sint32 pl34_special_26[] = {
	CG_FRAME(    0,    0,   3,           0, 32771, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0, 32772, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0, 32773, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0, 32774, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0, 32775, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0, 32776, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3,           0, 32777, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32778, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl34_special_26 ),
};

/* Char_Special 27: snapback_init (the snapback) */
const Sint32 pl34_special_27[] = {
	CG_FRAME(    0,    0,   1,           0,   171, 0x80,   0x3f,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x3f,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x30,  ATK,  0x01,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   172, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    1,   2,           0,   172, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME( 0x80,    2,   2,           0,   173, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME( 0x80,    3,   2,           0,   175, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   177, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,  12,           0,   176, 0x80,   0x30,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x30,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x31,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl34_special_27_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   169, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_special_27_loop ),
};

/* Char_Special 28 */
const Sint32 pl34_special_28[] = {
	CG_FRAME(  127,    2,   3,           0,   161, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    4,   3,           0,   162, 0x80,   0x32,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    6,   2,           0,   163, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    8,   2,           0,   160, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   138, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   145, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl34_special_28 ),
};

/* Char_Special 29: t_kick_init */
const Sint32 pl34_special_29[] = {
	CG_FRAME(    0,    0,   4,           0,   273, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   274, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl34_special_29 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl34_extra_01[] = {
	CG_FRAME(    0,    0,   1,           0,   108, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   108, 0x80,   0x32,    0,  0x00, 0x23,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    99, 0x80,   0x32,    0,  0x00, 0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   101, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   116, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   121, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   122, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   112, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    97, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    95, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_extra_01 ),
};

/* Char_Special 30: chouhatsu_init (the taunt) */
const Sint32 pl34_special_30[] = {
	CG_FRAME(    0,    0,   3,           0,    95, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x32,    0,  0x00, 0x32,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_special_30 ),
};

/* Char_Super 0: plasma_init (Plasma Storm (hyper)) */
const Sint32 pl34_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   330, 0x80,   0x2c,  ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   331, 0x80,   0xc3,  ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   332, 0x80,   0xc3,  ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
};

/* Char_Super 1: plasma_wait (Plasma Storm (hyper)) */
const Sint32 pl34_super_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   333, 0x80,   0xc3,  ATK,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl34_super_01 ),
};

/* Char_Super 2: plasma_move (Plasma Storm (hyper)) */
const Sint32 pl34_super_02[] = {
	CG_FRAME(    0,    0,  32,           0,   333, 0x80,   0xc3,  ATE,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   6,           0,   331, 0x80,   0xc3,  ATE,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   330, 0x80,   0x32,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_super_02 ),
};

/* Char_Effect 0: shl34_03_char_set */
const Sint32 pl34_effect_00[] = {
	CG_FRAME(    0,    0,   8,           0, 32779, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32780, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32781, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32782, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32783, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32784, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32785, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32786, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   8,           0, 32787, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0, 32788, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA, 32789, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl34_effect_00 ),
};

const Sint32 pl34_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,   4,           0,   380, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   379, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   378, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   381, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   382, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   383, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   385, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	0x00004f84,
};

/* Char_Effect 1: shl34_04_init */
const Sint32 pl34_effect_01[] = {
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00, 0x2d,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   10,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATK,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x0a,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,  ATK,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,  ATK,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,   55,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   12,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,   11,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   12,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,   13,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   13,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATK,  0x00,    0,   13,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   13,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
};

const Sint32 pl34_effect_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,   55,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,   55,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,    0,   14,   0,     0,      0,  69 ),
	CG_NEXT( pl34_effect_01_loop ),
};

/* Char_Effect 2: shl34_04_move */
const Sint32 pl34_effect_02[] = {
	CG_FRAME(    0,   56,   2,           0, 32790, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,  ATK,  0x00,    0,   15,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATE,  0x00,    0,   14,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,  ATE,  0x00,    0,   15,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATE,  0x00,    0,   13,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATE,  0x00,    0,   13,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATE,  0x00,    0,   12,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATE,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,  ATE,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,  ATE,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,  ATE,  0x00,    0,   10,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,  ATE,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32790, 0x80,   0x00,  ATE,  0x00,    0,    9,   0,     0,      0,   0 ),
	CG_NEXT( pl34_effect_02 ),
};

/* Char_Effect 3: pls34_01_init */
const Sint32 pl34_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,    0,  0x00,    0,   15,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,    0,  0x00,    0,   15,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,    0,  0x00,    0,   13,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32794, 0x80,   0x00,    0,  0x00,    0,   11,   0,     0,      0,   0 ),
	CG_NEXT( pl34_effect_03 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl34_change_00[] = {
	CG_FRAME(    0,   13,   4,           0,    95, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   4,           0,    97, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   4,           0,   106, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl34_change_00_loop[] = {
	CG_FRAME(    0,   13,   5, END_OF_DATA,   107, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl34_change_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    78, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl34_change_02[] = {
	CG_FRAME(    0,    0,   1,           0,    61, 0x80,   0x12,    0,  0x00, 0x2f,    0,   0,     0,      0,   3 ),
};

const Sint32 pl34_change_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    62, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl34_change_02_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl34_change_03[] = {
	CG_FRAME(    0,   15,   3,           0,   135, 0x80,   0x12,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   14,   3,           0,   138, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   134, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   139, 0x80,   0x10,    0,  0x00, 0x23,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   140, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   141, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   142, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   134, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   143, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   138, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3,           0,   135, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   3, END_OF_DATA,   133, 0x80,   0x10,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_change_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl34_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x32,    0,  0x00, 0x32,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   104, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    98, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    97, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    95, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_change_04 ),
};

/* group 25 0 */
const Sint32 pl34_grp25_00[] = {
	CG_FRAME(    0,    0,   3,           0, 32806, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   252, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   292, 0x80,   0x32,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl34_grp25_00 ),
};
