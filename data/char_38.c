/* Captain Commando (character 38): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_38      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_38     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl38.c) says which group
                   and number each move uses.
   pl38_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl38_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl38_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl38_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_38
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_38, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_38 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl38_stand_00[], pl38_stand_00_loop[], pl38_stand_11[], pl38_stand_11_loop[],
	pl38_stand_12[], pl38_stand_12_loop[], pl38_stand_13[], pl38_stand_13_loop[], pl38_stand_14[],
	pl38_stand_14_loop[], pl38_stand_15[], pl38_stand_15_loop[], pl38_stand_16[], pl38_stand_17[],
	pl38_stand_18[], pl38_stand_19[], pl38_stand_20[], pl38_stand_21[], pl38_stand_22[],
	pl38_stand_23[], pl38_stand_24[], pl38_stand_25[], pl38_stand_26[], pl38_stand_27[],
	pl38_stand_02[], pl38_stand_06[], pl38_stand_03[], pl38_stand_07[], pl38_jump_00[],
	pl38_jump_00_loop[], pl38_jump_01[], pl38_jump_01_loop[], pl38_jump_02[], pl38_jump_02_loop[],
	pl38_jump_03[], pl38_jump_06[], pl38_jump_09[], pl38_jump_07[], pl38_jump_10[], pl38_jump_08[],
	pl38_jump_11[], pl38_dash_00[], pl38_dash_00_loop[], pl38_dash_02[], pl38_dash_01[],
	pl38_dash_03[], pl38_crouch_00[], pl38_crouch_01[], pl38_crouch_02[], pl38_turn_00[],
	pl38_turn_01[], pl38_guard_00[], pl38_guard_01[], pl38_guard_02[], pl38_guard_03[],
	pl38_guard_04[], pl38_guard_05[], pl38_guard_06[], pl38_guard_07[], pl38_air_guard_00[],
	pl38_air_guard_01[], pl38_air_guard_02[], pl38_stand_p_00[], pl38_stand_p_00_loop[],
	pl38_stand_p_01[], pl38_stand_p_01_loop[], pl38_stand_p_02[], pl38_stand_p_02_loop[],
	pl38_stand_k_00[], pl38_stand_k_01[], pl38_stand_k_02[], pl38_crouch_p_00[], pl38_crouch_p_01[],
	pl38_crouch_p_02[], pl38_crouch_k_00[], pl38_crouch_k_01[], pl38_crouch_k_02[], pl38_jump_p_00[],
	pl38_jump_p_01[], pl38_jump_p_02[], pl38_jump_k_00[], pl38_jump_k_01[], pl38_jump_k_02[],
	pl38_damage_00[], pl38_damage_01[], pl38_damage_02[], pl38_damage_03[], pl38_damage_04[],
	pl38_damage_05[], pl38_damage_06[], pl38_damage_07[], pl38_damage_08[], pl38_damage_09[],
	pl38_damage_09_loop[], pl38_damage_10[], pl38_damage_11[], pl38_damage_12[], pl38_damage_13[],
	pl38_damage_16[], pl38_damage_17[], pl38_damage_18[], pl38_damage_19[], pl38_damage_20[],
	pl38_damage_21[], pl38_damage_22[], pl38_damage_23[], pl38_damage_24[], pl38_damage_25[],
	pl38_damage_26[], pl38_damage_27[], pl38_damage_28[], pl38_damage_29[], pl38_damage_30[],
	pl38_damage_31[], pl38_damage_34[], pl38_dizzy_00[], pl38_catch_00[], pl38_catch_08[],
	pl38_catch_09[], pl38_catch_01[], pl38_catch_02[], pl38_catch_03[], pl38_catch_03_loop[],
	pl38_catch_04[], pl38_catch_05[], pl38_catch_06[], pl38_catch_07[], pl38_get_up_00[],
	pl38_get_up_01[], pl38_get_up_02[], pl38_get_up_03[], pl38_appear_00[], pl38_appear_00_loop[],
	pl38_appear_01[], pl38_appear_13[], pl38_appear_13_loop[], pl38_appear_02[], pl38_appear_03[],
	pl38_appear_04[], pl38_appear_05[], pl38_appear_06[], pl38_appear_07[], pl38_appear_08[],
	pl38_appear_09[], pl38_appear_10[], pl38_appear_11[], pl38_appear_12[], pl38_extra_02[],
	pl38_extra_00[], pl38_extra_01[], pl38_extra_03[], pl38_extra_04[], pl38_extra_05[],
	pl38_extra_06[], pl38_extra_07[], pl38_extra_08[], pl38_extra_09[], pl38_extra_10[],
	pl38_extra_11[], pl38_extra_12[], pl38_extra_26[], pl38_extra_27[], pl38_special_38[],
	pl38_special_38_loop[], pl38_extra_13[], pl38_extra_14[], pl38_extra_15[], pl38_extra_16[],
	pl38_extra_17[], pl38_extra_18[], pl38_extra_19[], pl38_extra_20[], pl38_extra_21[],
	pl38_extra_25[], pl38_extra_22[], pl38_extra_23[], pl38_extra_24[], pl38_unused_00[],
	pl38_unused_01[], pl38_unused_02[], pl38_win_00[], pl38_win_00_loop[], pl38_win_01[],
	pl38_win_01_loop[], pl38_win_02[], pl38_win_02_loop[], pl38_win_03[], pl38_win_03_loop[],
	pl38_change_00[], pl38_change_01[], pl38_change_04[], pl38_change_04_loop[], pl38_change_03[],
	pl38_change_03_loop[], pl38_grp25_00[], pl38_grp25_01[], pl38_grp25_02[], pl38_grp25_03[],
	pl38_grp25_07[], pl38_grp25_04[], pl38_grp25_04_loop[], pl38_grp25_05[], pl38_grp25_06[],
	pl38_change_02[], pl38_change_02_loop[], pl38_special_00[], pl38_special_09[], pl38_special_03[],
	pl38_special_11[], pl38_special_06[], pl38_special_12[], pl38_special_13[], pl38_special_14[],
	pl38_special_15[], pl38_special_16[], pl38_special_17[], pl38_special_18[],
	pl38_special_18_loop[], pl38_special_19[], pl38_special_20[], pl38_special_20_loop[],
	pl38_special_21[], pl38_special_21_loop[], pl38_special_22[], pl38_special_23[],
	pl38_special_23_loop[], pl38_special_24[], pl38_special_25[], pl38_special_25_loop[],
	pl38_special_26[], pl38_special_27[], pl38_special_28[], pl38_special_28_loop[],
	pl38_special_29[], pl38_special_29_loop[], pl38_special_30[], pl38_special_31[],
	pl38_special_32[], pl38_special_33[], pl38_special_34[], pl38_special_35[], pl38_special_36[],
	pl38_special_37[], pl38_super_00[], pl38_super_02[], pl38_super_03[], pl38_super_04[],
	pl38_super_05[], pl38_super_06[], pl38_super_07[], pl38_super_09[], pl38_super_13[],
	pl38_super_10[], pl38_super_14[], pl38_super_11[], pl38_super_15[], pl38_super_12[],
	pl38_super_16[], pl38_super_21[], pl38_super_22[], pl38_super_25[], pl38_super_24[],
	pl38_super_24_loop[], pl38_super_30[], pl38_super_30_loop[], pl38_super_26[], pl38_super_27[],
	pl38_super_27_loop[], pl38_super_28[], pl38_super_29[], pl38_caught_00[], pl38_caught_01[],
	pl38_caught_02[], pl38_caught_03[], pl38_caught_04[], pl38_caught_05[], pl38_caught_06[],
	pl38_caught_07[], pl38_caught_08[], pl38_caught_09[], pl38_caught_10[], pl38_caught_11[],
	pl38_caught_12[], pl38_caught_13[], pl38_caught_14[], pl38_caught_15[], pl38_caught_16[],
	pl38_caught_17[], pl38_caught_18[], pl38_caught_19[], pl38_caught_20[], pl38_caught_21[],
	pl38_unused_03[], pl38_effect_00[], pl38_effect_02[], pl38_effect_03[], pl38_effect_04[],
	pl38_effect_05[], pl38_effect_06[], pl38_effect_07[], pl38_effect_08[], pl38_effect_09[],
	pl38_effect_10[], pl38_effect_11[], pl38_effect_12[], pl38_effect_13[], pl38_effect_14[],
	pl38_effect_15[], pl38_effect_15_loop[], pl38_effect_17[], pl38_effect_18[],
	pl38_effect_18_loop[], pl38_grp27_00[], pl38_grp27_01[];
extern const Sint32 *const pl38_stand_tbl[], *const pl38_jump_tbl[], *const pl38_dash_tbl[],
	*const pl38_crouch_tbl[], *const pl38_turn_tbl[], *const pl38_guard_tbl[],
	*const pl38_air_guard_tbl[], *const pl38_stand_p_tbl[], *const pl38_stand_k_tbl[],
	*const pl38_crouch_p_tbl[], *const pl38_crouch_k_tbl[], *const pl38_jump_p_tbl[],
	*const pl38_jump_k_tbl[], *const pl38_damage_tbl[], *const pl38_dizzy_tbl[],
	*const pl38_catch_tbl[], *const pl38_caught_tbl[], *const pl38_get_up_tbl[],
	*const pl38_appear_tbl[], *const pl38_win_tbl[], *const pl38_extra_tbl[],
	*const pl38_special_tbl[], *const pl38_super_tbl[], *const pl38_effect_tbl[],
	*const pl38_change_tbl[], *const pl38_grp25_tbl[], *const pl38_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_38[16] = {
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
const Sint32 *const *const char_tbl_38[] = {
	pl38_stand_tbl, pl38_jump_tbl, pl38_dash_tbl, pl38_crouch_tbl, pl38_turn_tbl, pl38_guard_tbl,
	pl38_air_guard_tbl, pl38_stand_p_tbl, pl38_stand_k_tbl, pl38_crouch_p_tbl, pl38_crouch_k_tbl, pl38_jump_p_tbl,
	pl38_jump_k_tbl, pl38_damage_tbl, pl38_dizzy_tbl, pl38_catch_tbl, pl38_caught_tbl, pl38_get_up_tbl,
	pl38_appear_tbl, pl38_win_tbl, pl38_extra_tbl, pl38_special_tbl, pl38_super_tbl, pl38_effect_tbl,
	pl38_change_tbl, pl38_grp25_tbl, pl38_grp25_tbl, pl38_grp27_tbl, 0,
};

const Sint32 *const pl38_stand_tbl[] = {
	pl38_stand_00, pl38_stand_00, pl38_stand_02, pl38_stand_03, pl38_stand_00, pl38_stand_00,
	pl38_stand_06, pl38_stand_07, pl38_stand_02, pl38_stand_03, pl38_stand_00, pl38_stand_11,
	pl38_stand_12, pl38_stand_13, pl38_stand_14, pl38_stand_15, pl38_stand_16, pl38_stand_17,
	pl38_stand_18, pl38_stand_19, pl38_stand_20, pl38_stand_21, pl38_stand_22, pl38_stand_23,
	pl38_stand_24, pl38_stand_25, pl38_stand_26, pl38_stand_27, 0,
};

const Sint32 *const pl38_jump_tbl[] = {
	pl38_jump_00, pl38_jump_01, pl38_jump_02, pl38_jump_03, pl38_jump_03, pl38_jump_03,
	pl38_jump_06, pl38_jump_07, pl38_jump_08, pl38_jump_09, pl38_jump_10, pl38_jump_11,
	0,
};

const Sint32 *const pl38_dash_tbl[] = {
	pl38_dash_00, pl38_dash_01, pl38_dash_02, pl38_dash_03, 0,
};

const Sint32 *const pl38_crouch_tbl[] = {
	pl38_crouch_00, pl38_crouch_01, pl38_crouch_02, 0,
};

const Sint32 *const pl38_turn_tbl[] = {
	pl38_turn_00, pl38_turn_01, 0,
};

const Sint32 *const pl38_guard_tbl[] = {
	pl38_guard_00, pl38_guard_01, pl38_guard_02, pl38_guard_03, pl38_guard_04, pl38_guard_05,
	pl38_guard_06, pl38_guard_07, 0,
};

const Sint32 *const pl38_air_guard_tbl[] = {
	pl38_air_guard_00, pl38_air_guard_01, pl38_air_guard_02, 0,
};

const Sint32 *const pl38_stand_p_tbl[] = {
	pl38_stand_p_00, pl38_stand_p_01, pl38_stand_p_02, 0,
};

const Sint32 *const pl38_stand_k_tbl[] = {
	pl38_stand_k_00, pl38_stand_k_01, pl38_stand_k_02, 0,
};

const Sint32 *const pl38_crouch_p_tbl[] = {
	pl38_crouch_p_00, pl38_crouch_p_01, pl38_crouch_p_02, 0,
};

const Sint32 *const pl38_crouch_k_tbl[] = {
	pl38_crouch_k_00, pl38_crouch_k_01, pl38_crouch_k_02, 0,
};

const Sint32 *const pl38_jump_p_tbl[] = {
	pl38_jump_p_00, pl38_jump_p_01, pl38_jump_p_02, pl38_jump_p_00, pl38_jump_p_01, pl38_jump_p_02,
	0,
};

const Sint32 *const pl38_jump_k_tbl[] = {
	pl38_jump_k_00, pl38_jump_k_01, pl38_jump_k_02, pl38_jump_k_00, pl38_jump_k_01, pl38_jump_k_02,
	0,
};

const Sint32 *const pl38_damage_tbl[] = {
	pl38_damage_00, pl38_damage_01, pl38_damage_02, pl38_damage_03, pl38_damage_04, pl38_damage_05,
	pl38_damage_06, pl38_damage_07, pl38_damage_08, pl38_damage_09, pl38_damage_10, pl38_damage_11,
	pl38_damage_12, pl38_damage_13, pl38_damage_13, pl38_damage_13, pl38_damage_16, pl38_damage_17,
	pl38_damage_18, pl38_damage_19, pl38_damage_20, pl38_damage_21, pl38_damage_22, pl38_damage_23,
	pl38_damage_24, pl38_damage_25, pl38_damage_26, pl38_damage_27, pl38_damage_28, pl38_damage_29,
	pl38_damage_30, pl38_damage_31, pl38_damage_30, pl38_damage_27, pl38_damage_34, 0,
};

const Sint32 *const pl38_dizzy_tbl[] = {
	pl38_dizzy_00, 0,
};

const Sint32 *const pl38_catch_tbl[] = {
	pl38_catch_00, pl38_catch_01, pl38_catch_02, pl38_catch_03, pl38_catch_04, pl38_catch_05,
	pl38_catch_06, pl38_catch_07, pl38_catch_08, pl38_catch_09, 0,
};

const Sint32 *const pl38_caught_tbl[] = {
	pl38_caught_00, pl38_caught_01, pl38_caught_02, pl38_caught_03, pl38_caught_04, pl38_caught_05,
	pl38_caught_06, pl38_caught_07, pl38_caught_08, pl38_caught_09, pl38_caught_10, pl38_caught_11,
	pl38_caught_12, pl38_caught_13, pl38_caught_14, pl38_caught_15, pl38_caught_16, pl38_caught_17,
	pl38_caught_18, pl38_caught_19, pl38_caught_20, pl38_caught_21, 0,
};

const Sint32 *const pl38_get_up_tbl[] = {
	pl38_get_up_00, pl38_get_up_01, pl38_get_up_02, pl38_get_up_03, 0,
};

const Sint32 *const pl38_appear_tbl[] = {
	pl38_appear_00, pl38_appear_01, pl38_appear_02, pl38_appear_03, pl38_appear_04, pl38_appear_05,
	pl38_appear_06, pl38_appear_07, pl38_appear_08, pl38_appear_09, pl38_appear_10, pl38_appear_11,
	pl38_appear_12, pl38_appear_13, 0,
};

const Sint32 *const pl38_win_tbl[] = {
	pl38_win_00, pl38_win_01, pl38_win_02, pl38_win_03, 0,
};

const Sint32 *const pl38_extra_tbl[] = {
	pl38_extra_00, pl38_extra_01, pl38_extra_02, pl38_extra_03, pl38_extra_04, pl38_extra_05,
	pl38_extra_06, pl38_extra_07, pl38_extra_08, pl38_extra_09, pl38_extra_10, pl38_extra_11,
	pl38_extra_12, pl38_extra_13, pl38_extra_14, pl38_extra_15, pl38_extra_16, pl38_extra_17,
	pl38_extra_18, pl38_extra_19, pl38_extra_20, pl38_extra_21, pl38_extra_22, pl38_extra_23,
	pl38_extra_24, pl38_extra_25, pl38_extra_26, pl38_extra_27, 0,
};

const Sint32 *const pl38_special_tbl[] = {
	pl38_special_00, pl38_special_00, pl38_special_00, pl38_special_03, pl38_special_03, pl38_special_03,
	pl38_special_06, pl38_special_06, pl38_special_06, pl38_special_09, pl38_special_03, pl38_special_11,
	pl38_special_12, pl38_special_13, pl38_special_14, pl38_special_15, pl38_special_16, pl38_special_17,
	pl38_special_18, pl38_special_19, pl38_special_20, pl38_special_21, pl38_special_22, pl38_special_23,
	pl38_special_24, pl38_special_25, pl38_special_26, pl38_special_27, pl38_special_28, pl38_special_29,
	pl38_special_30, pl38_special_31, pl38_special_32, pl38_special_33, pl38_special_34, pl38_special_35,
	pl38_special_36, pl38_special_37, pl38_special_38, 0,
};

const Sint32 *const pl38_super_tbl[] = {
	pl38_super_00, pl38_super_00, pl38_super_02, pl38_super_03, pl38_super_04, pl38_super_05,
	pl38_super_06, pl38_super_07, pl38_super_07, pl38_super_09, pl38_super_10, pl38_super_11,
	pl38_super_12, pl38_super_13, pl38_super_14, pl38_super_15, pl38_super_16, pl38_super_13,
	pl38_super_14, pl38_super_15, pl38_super_16, pl38_super_21, pl38_super_22, pl38_super_22,
	pl38_super_24, pl38_super_25, pl38_super_26, pl38_super_27, pl38_super_28, pl38_super_29,
	pl38_super_30, 0,
};

const Sint32 *const pl38_effect_tbl[] = {
	pl38_effect_00, pl38_effect_00, pl38_effect_02, pl38_effect_03, pl38_effect_04, pl38_effect_05,
	pl38_effect_06, pl38_effect_07, pl38_effect_08, pl38_effect_09, pl38_effect_10, pl38_effect_11,
	pl38_effect_12, pl38_effect_13, pl38_effect_14, pl38_effect_15, pl38_effect_15, pl38_effect_17,
	pl38_effect_18, 0,
};

const Sint32 *const pl38_change_tbl[] = {
	pl38_change_00, pl38_change_01, pl38_change_02, pl38_change_03, pl38_change_04, 0,
};

const Sint32 *const pl38_grp25_tbl[] = {
	pl38_grp25_00, pl38_grp25_01, pl38_grp25_02, pl38_grp25_03, pl38_grp25_04, pl38_grp25_05,
	pl38_grp25_06, pl38_grp25_07, 0,
};

const Sint32 *const pl38_grp27_tbl[] = {
	pl38_grp27_00, pl38_grp27_01, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: appear_end, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 10 */
const Sint32 pl38_stand_00[] = {
	CG_FRAME(    0,    0,   1,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_stand_00_loop[] = {
	CG_FRAME(    0,    1,  48, END_OF_DATA,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_00_loop ),
};

/* Char_Stand 11 */
const Sint32 pl38_stand_11[] = {
	CG_FRAME(    0,    0,   1,           0,    82, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_stand_11_loop[] = {
	CG_FRAME(    0,    1,  52, END_OF_DATA,    82, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_11_loop ),
};

/* Char_Stand 12 */
const Sint32 pl38_stand_12[] = {
	CG_FRAME(    0,    0,   1,           0,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_stand_12_loop[] = {
	CG_FRAME(    0,    1,  54, END_OF_DATA,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_12_loop ),
};

/* Char_Stand 13 */
const Sint32 pl38_stand_13[] = {
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    85, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,    88, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    90, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_stand_13_loop[] = {
	CG_FRAME(    0,    2,   1, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_13_loop ),
};

/* Char_Stand 14 */
const Sint32 pl38_stand_14[] = {
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    97, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    98, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    99, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  13,           0,   100, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   101, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_stand_14_loop[] = {
	CG_FRAME(    0,    3,   1, END_OF_DATA,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_14_loop ),
};

/* Char_Stand 15 */
const Sint32 pl38_stand_15[] = {
	CG_FRAME(    0,    0,   4,           0,    84, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    85, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    87, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,    88, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    90, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    97, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    98, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    99, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   100, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   101, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_stand_15_loop[] = {
	CG_FRAME(    0,    3,   1, END_OF_DATA,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_15_loop ),
};

/* Char_Stand 16 */
const Sint32 pl38_stand_16[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    93, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_00 ),
};

/* Char_Stand 17 */
const Sint32 pl38_stand_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    94, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_11 ),
};

/* Char_Stand 18 */
const Sint32 pl38_stand_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    95, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_12 ),
};

/* Char_Stand 19 */
const Sint32 pl38_stand_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_00 ),
};

/* Char_Stand 20 */
const Sint32 pl38_stand_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   106, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_11 ),
};

/* Char_Stand 21 */
const Sint32 pl38_stand_21[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   107, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_12 ),
};

/* Char_Stand 22 */
const Sint32 pl38_stand_22[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_00 ),
};

/* Char_Stand 23 */
const Sint32 pl38_stand_23[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   106, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_11 ),
};

/* Char_Stand 24 */
const Sint32 pl38_stand_24[] = {
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   120, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   121, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   107, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_12 ),
};

/* Char_Stand 25 */
const Sint32 pl38_stand_25[] = {
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_00 ),
};

/* Char_Stand 26 */
const Sint32 pl38_stand_26[] = {
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   106, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_11 ),
};

/* Char_Stand 27 */
const Sint32 pl38_stand_27[] = {
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   107, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_12 ),
};

/* Char_Stand 2: storm_land (Captain Storm (hyper)), act_walk (2 + no)
   Char_Stand 8 */
const Sint32 pl38_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   110, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   111, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   112, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   113, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   114, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   115, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   116, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   117, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   118, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_02 ),
};

/* Char_Stand 6 */
const Sint32 pl38_stand_06[] = {
	CG_FRAME(    0,    0,   6,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_06 ),
};

/* Char_Stand 3
   Char_Stand 9 */
const Sint32 pl38_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,   122, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   123, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   124, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   125, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   126, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   127, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   128, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   129, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   133, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_03 ),
};

/* Char_Stand 7 */
const Sint32 pl38_stand_07[] = {
	CG_FRAME(    0,    0,   6,           0,    83, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    95, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl38_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   154, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl38_jump_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl38_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   154, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl38_jump_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl38_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   154, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl38_jump_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_jump_02_loop ),
};

/* Char_Jump 3: storm_fall (Captain Storm (hyper))
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl38_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl38_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl38_jump_09[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl38_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: kick_fall (Captain Kick), change_in */
const Sint32 pl38_jump_10[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl38_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   153, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   154, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl38_jump_11[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl38_dash_00[] = {
	CG_FRAME(    0,    0,   1,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   135, 0x80,   0x00,          0,  0x00,       0x04,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_dash_00_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_dash_00_loop ),
};

/* Char_Dash 2: dash_slow */
const Sint32 pl38_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl38_dash_01[] = {
	CG_FRAME(    0,    1,   1,           0,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,          0,  0x00, 0x34 | CSE,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   148, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_dash_01 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl38_dash_03[] = {
	CG_FRAME(    0,    0,   8,           0,   148, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl38_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl38_crouch_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl38_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl38_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   167, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl38_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   170, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   171, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl38_guard_00[] = {
	CG_FRAME(    0,    0,   6,           0,   172, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl38_guard_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   173, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl38_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl38_guard_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   173, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl38_guard_04[] = {
	CG_FRAME(    0,    0,   6,           0,   174, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl38_guard_05[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   175, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl38_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   174, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl38_guard_07[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   175, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl38_air_guard_00[] = {
	CG_FRAME(    0,    0,   6,           0,   176, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl38_air_guard_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   177, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl38_air_guard_02[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   177, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl38_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   207, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl38_stand_p_00_loop[] = {
	CG_FRAME(    0,    1,   6, END_OF_DATA,   207, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl38_stand_p_00_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl38_stand_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   209, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   210, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   1,           0,   212, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,  15,           0,   213, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
};

const Sint32 pl38_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   102, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl38_stand_p_01_loop ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl38_stand_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   214, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   3,           0,   215, 0x90,   0x00,        ATK,  0x00,       0x05,    0,   0,     0, 0x0100,  44 ),
	CG_FRAME(    1,    0,   2,           0,   216, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x0200,  44 ),
	CG_FRAME( 0x80,    0,   4,           0,   217, 0x90,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x0300,  12 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x0400,  12 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0500,  13 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0700,  13 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0800,  13 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0900,  13 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0a00,  13 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0b00,  44 ),
	CG_FRAME(    0,    0,   5,           0,   103, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
};

const Sint32 pl38_stand_p_02_loop[] = {
	CG_FRAME(    0,    1,   5, END_OF_DATA,   104, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl38_stand_p_02_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl38_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   220, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   221, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   222, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   220, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl38_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   224, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   1,           0,   225, 0x90,   0x0b,        ATK,  0x00,       0x03,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3,           0,   226, 0x90,   0x0b,        ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,   1,           0,   227, 0x90,   0x86,        ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   5,           0,   228, 0x90,   0x86,        ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x90,   0x86,        ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x90,   0x05,        ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x90,   0x0a,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,    91, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    92, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy): kick_move */
const Sint32 pl38_stand_k_02[] = {
	CG_FRAME(    0,    1,   5,           0,   231, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   233, 0x90,   0x0c,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x90,   0x0c,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   235, 0x90,   0x0c,        ATK,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x90,   0x0c,        ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,  64,           0,   236, 0x90,   0x0c,        ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   237, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   8,           0,   165, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,    92, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl38_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl38_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   239, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   240, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   164, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl38_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   242, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   244, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    0,   1,           0,   245, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   6,           0,   246, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   246, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   5,           0,   247, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   164, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl38_crouch_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   248, 0x90,   0x4a,        ATK,  0x00,          0,    0,   0,     0, 0x0c00,  25 ),
	CG_FRAME(    0,    0,   2,           0,   249, 0x90,   0x4a,        ATK,  0x00,          0,    0,   0,     0, 0x0d00,  25 ),
	CG_FRAME(    0,    0,   1,           0,   250, 0x90,   0x4a,        ATK,  0x00,       0x05,    0,   0,     0, 0x0e00,  24 ),
	CG_FRAME(    0,    0,   1,           0,   250, 0x90,   0x4a,        ATK,  0x00, 0x4f | CSE,    0,   0,     0, 0x0e00,  24 ),
	CG_FRAME(    0,    0,   6,           0,   251, 0x90,   0x4a,        ATK,  0x00,          0,    0,   0,     0, 0x0e00,  24 ),
	CG_FRAME(    0,    0,  16,           0,   251, 0x90,   0x4a,        ATE,  0x00,          0,    0,   0,     0, 0x0e00,  52 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   164, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl38_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl38_crouch_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,   252, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6,           0,   253, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   252, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl38_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl38_crouch_k_01[] = {
	CG_FRAME(    0,    0,   7,           0,   254, 0x90,   0x05,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   2,           0,   255, 0x90,   0xa1,        ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x90,   0xa1,        ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x90,   0xa1,        ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   257, 0x90,   0x05,        ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl38_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl38_crouch_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   258, 0x90,   0x06,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   5,           0,   258, 0x90,   0x06,        ATK,  0x00,       0x04,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x90,   0x02,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   260, 0x90,   0xa1,        ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   3,           0,   261, 0x90,   0xa1,        ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   5,           0,   262, 0x90,   0x46,        ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   7,           0,   263, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   264, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl38_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   265, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,  10,           0,   266, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   9,           0,   265, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   157, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl38_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl38_jump_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   267, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   267, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,  10,           0,   269, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   155, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_NEXT( pl38_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl38_jump_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   270, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   271, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   272, 0x90,   0x00,        ATK,  0x00,       0x05,    0,   0,     0, 0x1000, 117 ),
	CG_FRAME(    0,    0,   3,           0,   272, 0x90,   0x00,        ATK,  0x00, 0x4f | CSE,    0,   0,     0, 0x1000, 117 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x1000,  36 ),
	CG_FRAME(    0,    0,   8,           0,   272, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x1000,  37 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   234, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl38_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl38_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   152, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,  10,           0,   274, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   275, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_NEXT( pl38_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl38_jump_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   276, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   276, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   277, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   7,           0,   278, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   2,           0,   278, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   279, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl38_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl38_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   280, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   281, 0x90,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   1,           0,   282, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   8,           0,   283, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   2,           0,   283, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   280, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl38_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl38_damage_00[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   336, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl38_damage_01[] = {
	CG_FRAME(    0,    0,   2,           0,   336, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   337, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   336, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl38_damage_02[] = {
	CG_FRAME(    0,    0,   2,           0,   337, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   338, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   337, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   336, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl38_damage_03[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl38_damage_04[] = {
	CG_FRAME(    0,    0,   2,           0,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   340, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl38_damage_05[] = {
	CG_FRAME(    0,    0,   2,           0,   340, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   341, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   340, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl38_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   342, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl38_damage_07[] = {
	CG_FRAME(    0,    0,   2,           0,   342, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  10,           0,   343, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   342, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl38_damage_08[] = {
	CG_FRAME(    0,    0,   2,           0,   343, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  12,           0,   344, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   343, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   342, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl38_damage_09[] = {
	CG_FRAME(    0,    0,   2,           0,   350, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   351, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   352, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl38_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   190, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl38_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl38_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   345, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl38_damage_11[] = {
	CG_FRAME(    0,    0,   1,           0,   345, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   346, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   345, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl38_damage_12[] = {
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   7,           0,   347, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   345, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl38_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   348, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   349, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   364, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl38_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl38_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   354, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl38_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   355, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl38_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   356, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl38_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   357, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl38_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl38_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   353, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   2,           0,   354, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

/* Char_Damage 22 */
const Sint32 pl38_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   355, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

/* Char_Damage 23 */
const Sint32 pl38_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   356, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

/* Char_Damage 24 */
const Sint32 pl38_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
};

/* Char_Damage 25 */
const Sint32 pl38_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   358, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl38_damage_26[] = {
	CG_FRAME(    0,    0,   1,           0,   359, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   365, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl38_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl38_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   361, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   3,           0,   144, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   3,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl38_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl38_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   366, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   367, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   371, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl38_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   374, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   377, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl38_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl38_damage_31[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   190, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl38_damage_31 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl38_damage_34[] = {
	CG_FRAME(    0,    0,   4,           0,   348, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   364, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl38_damage_34 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl38_dizzy_00[] = {
	CG_FRAME(    0,    0,   8,           0,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_NEXT( pl38_dizzy_00 ),
};

/* Char_Catch 0 */
const Sint32 pl38_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,  49 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   245, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   8,           0,   246, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA,   247, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_catch_00 ),
};

/* Char_Catch 8 */
const Sint32 pl38_catch_08[] = {
	CG_FRAME(    0,    0,  14,           0,   284, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,  49 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   245, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,  64,           0,   246, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   247, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_catch_08 ),
};

/* Char_Catch 9 */
const Sint32 pl38_catch_09[] = {
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,  49 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME( 0x80,    0,   4,           0,   245, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,  36,           0,   246, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   247, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_catch_09 ),
};

/* Char_Catch 1: shl38_00_t15_init */
const Sint32 pl38_catch_01[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   285, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_catch_01 ),
};

/* Char_Catch 2: pl38_catch_set */
const Sint32 pl38_catch_02[] = {
	CG_FRAME(    0,    0,   6,           0,   310, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   6,           0,   311, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   6,           0,   312, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   6,           0,   313, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   6,           0,   314, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_catch_02 ),
};

/* Char_Catch 3: pl38_catch_set */
const Sint32 pl38_catch_03[] = {
	CG_FRAME(    0,    0,   2,           0,   288, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    1,    0,  12,           0,   290, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1,    1,   1,           0,   290, 0x80,   0x00,          0,  0x00, 0x4f | CSE,    0,   0,     0, 0x000c,   0 ),
};

const Sint32 pl38_catch_03_loop[] = {
	CG_FRAME(    2,    4,   2,           0,   291, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    2,    8,   2, END_OF_DATA,   291, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0, 0x000e,   0 ),
	CG_NEXT( pl38_catch_03_loop ),
};

/* Char_Catch 4: nml_k_catch_hold */
const Sint32 pl38_catch_04[] = {
	CG_FRAME(    0,    1,   2,           0,   235, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    1,    0,   2,           0,   235, 0x90,   0x00,        ATK,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   166, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_catch_04 ),
};

/* Char_Catch 5 */
const Sint32 pl38_catch_05[] = {
	CG_FRAME(    0,    0,  32,           0,   287, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   102, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   104, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_catch_05 ),
};

/* Char_Catch 6 */
const Sint32 pl38_catch_06[] = {
	CG_FRAME(    0,    0,  66,           0,   286, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,    91, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_catch_06 ),
};

/* Char_Catch 7 */
const Sint32 pl38_catch_07[] = {
	CG_FRAME(    0,    0,  84,           0,   294, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   292, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    92, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_catch_07 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl38_get_up_00[] = {
	CG_FRAME(    0,    0,   6,           0,   191, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    89, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    90, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl38_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   195, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl38_get_up_02[] = {
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   197, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   195, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl38_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   196, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   194, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   165, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_get_up_03 ),
};

/* Char_Appear 0 */
const Sint32 pl38_appear_00[] = {
	CG_FRAME(    0,    0,  48,           0,   182, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   183, 0x80,   0x00,          0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   184, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   184, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_appear_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   185, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_appear_00_loop ),
};

/* Char_Appear 1: appear_next */
const Sint32 pl38_appear_01[] = {
	CG_FRAME(    0,    0,  48,           0,   186, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    93, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_appear_01 ),
};

/* Char_Appear 13 */
const Sint32 pl38_appear_13[] = {
	CG_FRAME(    0,    0,  48,           0,   187, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   188, 0x80,   0x00,          0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   189, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   189, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   189, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_appear_13_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   185, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_appear_13_loop ),
};

/* Char_Appear 2 */
const Sint32 pl38_appear_02[] = {
	CG_FRAME(    0,    0,   1,           0,   571, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   572, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   577, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_02 ),
};

/* Char_Appear 3 */
const Sint32 pl38_appear_03[] = {
	CG_FRAME(    0,    0,   1,           0,   574, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   575, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   578, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_03 ),
};

/* Char_Appear 4: pls38_00_ride */
const Sint32 pl38_appear_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   573, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_04 ),
};

/* Char_Appear 5 */
const Sint32 pl38_appear_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   576, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_05 ),
};

/* Char_Appear 6: pls38_00_trail */
const Sint32 pl38_appear_06[] = {
	CG_FRAME(    0,    0,   2,           0,   579, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   580, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_06 ),
};

/* Char_Appear 7: pls38_00_keep (7 + sub_type) */
const Sint32 pl38_appear_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   560, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_07 ),
};

/* Char_Appear 8 */
const Sint32 pl38_appear_08[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   561, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_08 ),
};

/* Char_Appear 9 */
const Sint32 pl38_appear_09[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   564, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_09 ),
};

/* Char_Appear 10 */
const Sint32 pl38_appear_10[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   562, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_10 ),
};

/* Char_Appear 11 */
const Sint32 pl38_appear_11[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   563, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_11 ),
};

/* Char_Appear 12: pls38_00_last */
const Sint32 pl38_appear_12[] = {
	CG_FRAME(    0,    0,   4,           0,   565, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   566, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   566, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   567, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   568, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   569, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   570, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_appear_12 ),
};

/* Char_Extra 2: sp_chouhatsu (Taunt), fly_start5 */
const Sint32 pl38_extra_02[] = {
	CG_FRAME(    0,    0,   2,           0,    99, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   324, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   325, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   327, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   328, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x00,          0,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   330, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   332, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   325, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   324, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   102, 0x80,   0x00,          0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_extra_02 ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl38_extra_00[] = {
	CG_FRAME(    0,    0,  96, END_OF_DATA,   283, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl38_extra_00 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl38_extra_01[] = {
	CG_FRAME(    0,    0,   1,           0,   305, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   305, 0x80,   0x00,        ATE,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   202, 0x80,   0x40,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_extra_01 ),
};

/* Char_Extra 3: shl38_00_strike_l_hit, shl38_00_catch_l_move, fly_start5 */
const Sint32 pl38_extra_03[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   604, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_03 ),
};

/* Char_Extra 4 */
const Sint32 pl38_extra_04[] = {
	CG_FRAME(    0,    0,   1,           0,   586, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   589, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl38_extra_05[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   592, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_05 ),
};

/* Char_Extra 6 */
const Sint32 pl38_extra_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   595, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl38_extra_06 ),
};

/* Char_Extra 7 */
const Sint32 pl38_extra_07[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   601, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_07 ),
};

/* Char_Extra 8: shl38_00_catch_l_init */
const Sint32 pl38_extra_08[] = {
	CG_FRAME(    0,    0,  16,           0,   621, 0x80,   0x05,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   621, 0x80,   0x05,          0,  0x00,       0x1b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  96, END_OF_DATA,   621, 0x80,   0x05,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_08 ),
};

/* Char_Extra 9: shl38_00_strike_h_move, shl38_00_catch_h_init, shl38_00_catch_h_wait */
const Sint32 pl38_extra_09[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   605, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_09 ),
};

/* Char_Extra 10 */
const Sint32 pl38_extra_10[] = {
	CG_FRAME(    0,    0,   1,           0,   587, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   590, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_10 ),
};

/* Char_Extra 11 */
const Sint32 pl38_extra_11[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   593, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_11 ),
};

/* Char_Extra 12 */
const Sint32 pl38_extra_12[] = {
	CG_FRAME(    0,    0,   2,           0,   596, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2,           0,   597, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2,           0,   598, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   599, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl38_extra_12 ),
};

/* Char_Extra 26: shl38_00_catch_h_wait */
const Sint32 pl38_extra_26[] = {
	CG_FRAME(    0,    0,   2,           0,   596, 0x80,   0x00,        ATK,  0x00, 0x18 | CSE,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2,           0,   597, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2,           0,   598, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   599, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl38_extra_26 ),
};

/* Char_Extra 27: pl38_upper */
const Sint32 pl38_extra_27[] = {
	CG_FRAME(    0,    0,   4,           0,   242, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,   1,           0,   244, 0x90,   0x00,        ATK,  0x00,       0x03,    0,   0,     0,      0, 118 ),
	CG_FRAME( 0x80,    0,   1,           0,   245, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   6,           0,   246, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   246, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   5,           0,   247, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   164, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl38_extra_27 ),
};

/* Char_Special 38: snapback_init (Snapback) */
const Sint32 pl38_special_38[] = {
	CG_FRAME(    0,    0,   6,           0,   214, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   3,           0,   215, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x0100,  44 ),
	CG_FRAME(    1,    0,   2,           0,   216, 0x90,   0x00,        ATK,  0x00,       0x04,    0,   0,     0, 0x0200,  44 ),
	CG_FRAME( 0x80,    0,   4,           0,   217, 0x90,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x0300,  12 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x90,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x0400,  12 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0500,  13 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0600,  13 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0700,  13 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0800,  13 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0900,  13 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0a00,  13 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x0b00,  44 ),
	CG_FRAME(    0,    0,   5,           0,   103, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
};

const Sint32 pl38_special_38_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   104, 0x90,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl38_special_38_loop ),
};

/* Char_Extra 13 */
const Sint32 pl38_extra_13[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   602, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_13 ),
};

/* Char_Extra 14: shl38_00_catch_h_in */
const Sint32 pl38_extra_14[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   623, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_14 ),
};

/* Char_Extra 15: shl38_00_catch_x_init, shl38_00_catch_x_wait, shl38_00_catch_x_end */
const Sint32 pl38_extra_15[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   606, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_15 ),
};

/* Char_Extra 16 */
const Sint32 pl38_extra_16[] = {
	CG_FRAME(    0,    0,   1,           0,   588, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   591, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_16 ),
};

/* Char_Extra 17: shl38_00_catch_x_in */
const Sint32 pl38_extra_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   594, 0x80,   0x94,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_17 ),
};

/* Char_Extra 18 */
const Sint32 pl38_extra_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   600, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl38_extra_18 ),
};

/* Char_Extra 19 */
const Sint32 pl38_extra_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   603, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_19 ),
};

/* Char_Extra 20: shl38_00_catch_x_wait */
const Sint32 pl38_extra_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   624, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_20 ),
};

/* Char_Extra 21: shl38_00_catch_x_keep, shl38_01_wait3 */
const Sint32 pl38_extra_21[] = {
	CG_FRAME(    0,    0,  32, END_OF_DATA,   625, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_21 ),
};

/* Char_Extra 25: shl38_00_catch_x_shot_init */
const Sint32 pl38_extra_25[] = {
	CG_FRAME(    0,    0,   4,           0,   626, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   627, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   628, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   629, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   630, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   631, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   632, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_25 ),
};

/* Char_Extra 22 */
const Sint32 pl38_extra_22[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   653, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_22 ),
};

/* Char_Extra 23 */
const Sint32 pl38_extra_23[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   654, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_23 ),
};

/* Char_Extra 24 */
const Sint32 pl38_extra_24[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   655, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_extra_24 ),
};

const Sint32 pl38_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   6, END_OF_DATA,   586, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003d2c,
};

const Sint32 pl38_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   6, END_OF_DATA,   593, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003d44,
};

const Sint32 pl38_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   6, END_OF_DATA,   592, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003d5c,
};

/* Char_Win 0 */
const Sint32 pl38_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,    92, 0x80,   0x00,          0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_win_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   178, 0x80,   0x16,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl38_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   179, 0x80,   0x00,          0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_win_01_loop[] = {
	CG_FRAME(    3,    0,   4, END_OF_DATA,   180, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl38_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   202, 0x80,   0x40,          0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_win_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   202, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_win_02_loop ),
};

/* Char_Win 3: pl38_win */
const Sint32 pl38_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   181, 0x80,   0x00,       0x0a,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_win_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   181, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_win_03_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl38_change_00[] = {
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   201, 0x80,   0x00,          0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,  18, END_OF_DATA,   201, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl38_change_01[] = {
	CG_FRAME( 0x80,    0,   6, END_OF_DATA,   203, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_change_01 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl38_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   202, 0x80,   0x40,          0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_change_04_loop[] = {
	CG_FRAME(    0,    0,  35, END_OF_DATA,   202, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_change_04_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl38_change_03[] = {
	CG_FRAME(    0,    0,   6,           0,   203, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   1,           0,   204, 0x80,   0x00,          0,  0x00,       0x05,    0,   0,     0,      0,   1 ),
};

const Sint32 pl38_change_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   206, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_change_03_loop ),
};

/* group 25 0
   Char_Fly 0 */
const Sint32 pl38_grp25_00[] = {
	CG_FRAME(    0,    0,   1,           0,   535, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   536, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   537, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   538, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   540, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_grp25_00 ),
};

/* group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl38_grp25_01[] = {
	CG_FRAME(    0,    0,   1,           0,   541, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   542, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   543, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   544, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   545, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   546, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_grp25_01 ),
};

/* group 25 2
   Char_Fly 2 */
const Sint32 pl38_grp25_02[] = {
	CG_FRAME(    0,    0,   1,           0,   547, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   548, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   549, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   550, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   551, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   552, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_grp25_02 ),
};

/* group 25 3
   Char_Fly 3 */
const Sint32 pl38_grp25_03[] = {
	CG_FRAME(    0,    0,   1,           0,   553, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   554, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   555, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   556, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   557, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   558, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32, END_OF_DATA,    -1, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_grp25_03 ),
};

/* group 25 7
   Char_Fly 7 */
const Sint32 pl38_grp25_07[] = {
	CG_FRAME(    0,    0,   1,           0,   553, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   554, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   555, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   556, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   557, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   558, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_grp25_07 ),
};

/* group 25 4: pls38_02_main
   Char_Fly 4 */
const Sint32 pl38_grp25_04[] = {
	CG_FRAME(    0,    0,   2,           0,   581, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   584, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   585, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_grp25_04_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   585, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_grp25_04_loop ),
};

/* group 25 5
   Char_Fly 5 */
const Sint32 pl38_grp25_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    -1, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl38_grp25_05 ),
};

/* group 25 6
   Char_Fly 6 */
const Sint32 pl38_grp25_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   622, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl38_grp25_06 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl38_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl38_change_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl38_change_02_loop ),
};

/* Char_Special 0
   Char_Special 1
   Char_Special 2 */
const Sint32 pl38_special_00[] = {
	CG_FRAME(    0,    0,   1,           0,   298, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,  10,           0,   298, 0x80,   0x00,        ATK,  0x00,       0x18,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,  40,           0,   301, 0x80,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   166, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_special_00 ),
};

/* Char_Special 9 */
const Sint32 pl38_special_09[] = {
	CG_FRAME(    0,    0,   2,           0,   452, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   464, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_09 ),
};

/* Char_Special 3
   Char_Special 4
   Char_Special 5
   Char_Special 10 */
const Sint32 pl38_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   452, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   4,           0,   453, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   464, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_03 ),
};

/* Char_Special 11 */
const Sint32 pl38_special_11[] = {
	CG_FRAME(    0,    0,   5,           0,   452, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   4,           0,   453, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   3,           0,   454, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   464, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_11 ),
};

/* Char_Special 6
   Char_Special 7
   Char_Special 8 */
const Sint32 pl38_special_06[] = {
	CG_FRAME(    0,    0,   2,           0,   306, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   7,           0,   306, 0x80,   0x00,        ATK,  0x00,       0x18,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   2,           0,   307, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    1,  34, END_OF_DATA,   309, 0x80,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0, 100 ),
	CG_NEXT( pl38_special_06 ),
};

/* Char_Special 12 */
const Sint32 pl38_special_12[] = {
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  36,           0,   302, 0x80,   0x00,        ATK,  0x00,       0x08,    0,   0,     0,      0,   5 ),
};

/* Char_Special 13 */
const Sint32 pl38_special_13[] = {
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_special_12 ),
};

/* Char_Special 14 */
const Sint32 pl38_special_14[] = {
	CG_FRAME(    0,    0,   1,           0,   303, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  10,           0,   303, 0x80,   0x00,        ATK,  0x00,       0x07,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  21,           0,   303, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

/* Char_Special 15 */
const Sint32 pl38_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,   142, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl38_special_16[] = {
	CG_FRAME(    0,    0,   1,           0,   304, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8,           0,   304, 0x80,   0x00,        ATK,  0x00,       0x09,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,  40,           0,   304, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
};

/* Char_Special 17 */
const Sint32 pl38_special_17[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_special_17 ),
};

/* Char_Special 18: shl38_00_strike_l_init */
const Sint32 pl38_special_18[] = {
	CG_FRAME(    0,    0,   1,           0,   607, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   608, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_special_18_loop[] = {
	CG_FRAME(    0,    1,   1, END_OF_DATA,   614, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl38_special_18_loop ),
};

/* Char_Special 19: shl38_00_t08_init */
const Sint32 pl38_special_19[] = {
	CG_FRAME(    0,    0,   2,           0,   612, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   613, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_19 ),
};

/* Char_Special 20: shl38_00_strike_l_move */
const Sint32 pl38_special_20[] = {
	CG_FRAME(    0,    0,   2,           0,   615, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   616, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   2,           0,   617, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
};

const Sint32 pl38_special_20_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   617, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl38_special_20_loop ),
};

/* Char_Special 21: shl38_00_strike_l_move */
const Sint32 pl38_special_21[] = {
	CG_FRAME(    0,    0,   4,           0,   618, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,  16,           0,   619, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  86 ),
};

const Sint32 pl38_special_21_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   604, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_21_loop ),
};

/* Char_Special 22: shl38_00_strike_h_init */
const Sint32 pl38_special_22[] = {
	CG_FRAME(    0,    0,   2,           0,   596, 0x80,   0x00,        ATK,  0x00, 0x18 | CSE,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   597, 0x80,   0x00,        ATK,  0x43,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   598, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   599, 0x80,   0x00,        ATK,  0x43,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl38_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl38_special_23[] = {
	CG_FRAME(    0,    0,  16,           0,   620, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_special_23_loop[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   605, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_23_loop ),
};

/* Char_Special 24: shl38_00_strike_x_init */
const Sint32 pl38_special_24[] = {
	CG_FRAME(    0,    0,   1,           0,   606, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   633, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl38_special_24 ),
};

/* Char_Special 25: shl38_00_strike_x_land */
const Sint32 pl38_special_25[] = {
	CG_FRAME(    0,    0,   4,           0,   634, 0x80,   0x54,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   635, 0x80,   0x54,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   636, 0x80,   0x54,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_special_25_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   637, 0x80,   0x54,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_25_loop ),
};

/* Char_Special 26: shl38_00_strike_x_shot_init */
const Sint32 pl38_special_26[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   638, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl38_special_26 ),
};

/* Char_Special 27: shl38_00_strike_x_shot_move */
const Sint32 pl38_special_27[] = {
	CG_FRAME(    0,    0,   3,           0,   639, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   640, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   641, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   642, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_27 ),
};

/* Char_Special 28: shl38_00_strike_x_shot_hit */
const Sint32 pl38_special_28[] = {
	CG_FRAME(    0,    0,   4,           0,   633, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  24,           0,   643, 0x80,   0x05,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_special_28_loop[] = {
	CG_FRAME(    0,    1,  16, END_OF_DATA,   606, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_28_loop ),
};

/* Char_Special 29: shl38_00_strike_x_shot_miss */
const Sint32 pl38_special_29[] = {
	CG_FRAME(    0,    0,   8,           0,   633, 0x80,   0x54,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_special_29_loop[] = {
	CG_FRAME(    0,    1,  16, END_OF_DATA,   606, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_29_loop ),
};

/* Char_Special 30: shl38_00_strike_x_wait, shl38_00_strike_x_hit */
const Sint32 pl38_special_30[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   644, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_30 ),
};

/* Char_Special 31: shl38_00_t14_init */
const Sint32 pl38_special_31[] = {
	CG_FRAME(    0,    0,   2,           0,   645, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   646, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   647, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   648, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   649, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   650, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   651, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   652, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_special_31 ),
};

/* Char_Special 32 */
const Sint32 pl38_special_32[] = {
	CG_FRAME(    0,    0,   5,           0,   163, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   5,           0,   232, 0x80,   0x00,        ATK,  0x00,       0x06,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   1,           0,   233, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   7,           0,   276, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   277, 0x80,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x1200, 105 ),
	CG_FRAME(    0,    0,   6,           0,   278, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x1200, 105 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x1200, 106 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   279, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl38_special_32 ),
};

/* Char_Special 33 */
const Sint32 pl38_special_33[] = {
	CG_FRAME(    0,    0,   5,           0,   163, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   5,           0,   232, 0x80,   0x00,        ATK,  0x00,       0x06,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   1,           0,   233, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   7,           0,   276, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   277, 0x80,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x1200, 105 ),
	CG_FRAME(    0,    0,   7,           0,   278, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x1200, 105 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x1200, 106 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   279, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl38_special_33 ),
};

/* Char_Special 34 */
const Sint32 pl38_special_34[] = {
	CG_FRAME(    0,    0,   5,           0,   163, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   231, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   5,           0,   232, 0x80,   0x00,        ATK,  0x00,       0x06,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   1,           0,   233, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   7,           0,   276, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    1,   2,           0,   277, 0x80,   0x00,        ATK,  0x00, 0x4b | CSE,    0,   0,     0, 0x1200, 105 ),
	CG_FRAME(    0,    0,   8,           0,   278, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0, 0x1200, 105 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0, 0x1200, 106 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   279, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl38_special_34 ),
};

/* Char_Special 35 */
const Sint32 pl38_special_35[] = {
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_special_35 ),
};

/* Char_Special 36 */
const Sint32 pl38_special_36[] = {
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_special_36 ),
};

/* Char_Special 37 */
const Sint32 pl38_special_37[] = {
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   164, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   166, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_special_37 ),
};

/* Char_Super 0: sword_init (Captain Sword (hyper))
   Char_Super 1 */
const Sint32 pl38_super_00[] = {
	CG_FRAME(    0,    0,   1,           0,    92, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   1,           0,    92, 0x80,   0x00,        ATK,  0x00,       0x15,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   5,           0,   316, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   1,           0,   316, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,  16,           0,   316, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   2,           0,   317, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,   2,           0,   317, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    2,   2,           0,   317, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    3,   2,           0,   317, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    4,   2,           0,   317, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    5,   1,           0,   318, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    6,   1,           0,   318, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    7,   1,           0,   318, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    8,   1,           0,   318, 0x80,   0x00,        ATK,  0x01,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    9,   1,           0,   318, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,   10,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   11,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   12,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   13,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   14,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   15,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   16,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   17,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,   -1,   1,           0,   319, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,  36,           0,   320, 0x80,   0x00,        ATK,  0x02,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   104, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_super_00 ),
};

/* Char_Super 2: shl38_02_init */
const Sint32 pl38_super_02[] = {
	CG_FRAME(    0,    0,   1,           0,   676, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   677, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   678, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   679, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   680, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   681, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   682, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   683, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   684, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   685, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   686, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32808, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_02 ),
};

/* Char_Super 3: shl38_02_head_start, shl38_02_head_swing */
const Sint32 pl38_super_03[] = {
	CG_FRAME(    0,    0,   1,           0,   688, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   1,           0,   690, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   1,           0,   692, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   1,           0,   694, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   1,           0,   696, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,   698, 0x80,   0x00,        ATK,  0x35,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   700, 0x80,   0x00,        ATK,  0x36,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   702, 0x80,   0x00,        ATK,  0x37,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   704, 0x80,   0x00,        ATK,  0x38,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   1,           0,   706, 0x80,   0x00,        ATK,  0x39,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   1,           0,   708, 0x80,   0x00,        ATK,  0x3a,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   1,           0,   710, 0x80,   0x00,        ATK,  0x3b,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   712, 0x80,   0x00,        ATK,  0x3c,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   714, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   716, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   718, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   720, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   722, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_03 ),
};

/* Char_Super 4: shl38_02_chain_main */
const Sint32 pl38_super_04[] = {
	CG_FRAME(    0,    0,   1,           0, 32809, 0x80,   0x00,        ATK,  0x35,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0, 32810, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0, 32811, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0, 32812, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0, 32813, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0, 32814, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0, 32815, 0x80,   0x00,        ATK,  0x36,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0, 32816, 0x80,   0x00,        ATK,  0x37,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   1,           0, 32817, 0x80,   0x00,        ATK,  0x38,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0, 32818, 0x80,   0x00,        ATK,  0x39,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0, 32819, 0x80,   0x00,        ATK,  0x3a,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   1,           0, 32820, 0x80,   0x00,        ATK,  0x3b,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   1,           0, 32821, 0x80,   0x00,        ATK,  0x3c,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,   1,           0, 32822, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32823, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32824, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32825, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32826, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_04 ),
};

/* Char_Super 5: shl38_02_head_swing */
const Sint32 pl38_super_05[] = {
	CG_FRAME(    0,    0,   1,           0,   724, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   725, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   726, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   727, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   728, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   729, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_05 ),
};

/* Char_Super 6 */
const Sint32 pl38_super_06[] = {
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   293, 0x80,   0x00,        ATK,  0x00,       0x17,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   294, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    4,    1,  16,           0,   297, 0x80,   0x00,        ATK,  0x00,       0x20,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,  24,           0,   297, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    1,   4,           0,   297, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl38_super_06 ),
};

/* Char_Super 7
   Char_Super 8 */
const Sint32 pl38_super_07[] = {
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   293, 0x80,   0x00,        ATK,  0x00,       0x17,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   294, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   294, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    4,    1,  16,           0,   297, 0x80,   0x00,        ATK,  0x00,       0x20,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,  32,           0,   297, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    4,    1,   4,           0,   297, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    1,   4,           0,   165, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl38_super_07 ),
};

/* Char_Super 9 */
const Sint32 pl38_super_09[] = {
	CG_FRAME(    0,    0,   6,           0, 32771, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0, 32775, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,   4,           0, 32779, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   4,           0, 32787, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32791, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32795, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32807, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_09 ),
};

/* Char_Super 13
   Char_Super 17 */
const Sint32 pl38_super_13[] = {
	CG_FRAME(    0,    0,   6,           0, 32771, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0, 32775, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,   4,           0, 32779, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0, 32783, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   4,           0, 32787, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32791, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32795, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32807, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_13 ),
};

/* Char_Super 10 */
const Sint32 pl38_super_10[] = {
	CG_FRAME(    0,    0,   6,           0, 32770, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0, 32774, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32778, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32782, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32786, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32790, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32794, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32806, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_10 ),
};

/* Char_Super 14
   Char_Super 18 */
const Sint32 pl38_super_14[] = {
	CG_FRAME(    0,    0,   6,           0, 32770, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0, 32774, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32778, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32782, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32786, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32790, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32794, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32806, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_14 ),
};

/* Char_Super 11 */
const Sint32 pl38_super_11[] = {
	CG_FRAME(    0,    0,   6,           0, 32769, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0, 32773, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32777, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32781, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32785, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32789, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32793, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32805, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_11 ),
};

/* Char_Super 15
   Char_Super 19 */
const Sint32 pl38_super_15[] = {
	CG_FRAME(    0,    0,   6,           0, 32769, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0, 32773, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32777, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32781, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32785, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32789, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32793, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32805, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_15 ),
};

/* Char_Super 12 */
const Sint32 pl38_super_12[] = {
	CG_FRAME(    0,    0,   6,           0, 32768, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32776, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32780, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32792, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32796, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32804, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_12 ),
};

/* Char_Super 16
   Char_Super 20 */
const Sint32 pl38_super_16[] = {
	CG_FRAME(    0,    0,   6,           0, 32768, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0, 32776, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32780, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0, 32784, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32788, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32792, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32796, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32804, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_16 ),
};

/* Char_Super 21 */
const Sint32 pl38_super_21[] = {
	CG_FRAME(    0,    0,   1,           0,   519, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   520, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   521, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   522, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   523, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   524, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   525, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   526, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   527, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   530, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   532, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   534, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_21 ),
};

/* Char_Super 22
   Char_Super 23 */
const Sint32 pl38_super_22[] = {
	CG_FRAME( 0x80,    0,   1,           0,   519, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   520, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   521, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   1,           0,   522, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   523, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   524, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   525, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   526, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   527, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   530, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   532, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   533, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   534, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_super_22 ),
};

/* Char_Super 25: storm_meet (Captain Storm (hyper)) */
const Sint32 pl38_super_25[] = {
	CG_FRAME(    0,    0,   1,           0,   292, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   293, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   8,           0,   294, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   1,           0,   294, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   6,           0,   295, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   297, 0x80,   0x00,        ATK,  0x00,       0x05,    0,   0,     0,      0,   0 ),
	CG_FRAME(    4,    1,  48,           0,   297, 0x80,   0x00,        ATK,  0x00,       0x20,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0,   165, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl38_super_25 ),
};

/* Char_Super 24: storm_init (Captain Storm (hyper)) */
const Sint32 pl38_super_24[] = {
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   1,           0,   204, 0x80,   0x00,        ATK,  0x00,       0x14,    0,   0,     0,      0,   5 ),
};

const Sint32 pl38_super_24_loop[] = {
	CG_FRAME( 0x80,    1,   4,           0,   204, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    1,   4,           0,   205, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x83,    1,   4, END_OF_DATA,   206, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_super_24_loop ),
};

/* Char_Super 30 */
const Sint32 pl38_super_30[] = {
	CG_FRAME(    0,    0,  12,           0,   203, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,   1,           0,   204, 0x80,   0x00, ATK | 0x0d,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl38_super_30_loop[] = {
	CG_FRAME( 0x80,    1,   4,           0,   204, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    1,   4,           0,   205, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x83,    1,   4, END_OF_DATA,   206, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_super_30_loop ),
};

/* Char_Super 26: storm_rush (Captain Storm (hyper)) */
const Sint32 pl38_super_26[] = {
	CG_FRAME(    0,    0,   4,           0,   134, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   138, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl38_super_26 ),
};

/* Char_Super 27: storm_move (Captain Storm (hyper)) */
const Sint32 pl38_super_27[] = {
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    1,   8,           0,   323, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
};

const Sint32 pl38_super_27_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   151, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl38_super_27_loop ),
};

/* Char_Super 28 */
const Sint32 pl38_super_28[] = {
	CG_FRAME(    0,    0,   4,           0,   139, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   140, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   141, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   142, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_super_28 ),
};

/* Char_Super 29: storm_last (Captain Storm (hyper)) */
const Sint32 pl38_super_29[] = {
	CG_FRAME(    0,    0,  40,           0,   178, 0x80,   0x16,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,  32, END_OF_DATA,   178, 0x80,   0x16,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl38_super_29 ),
};

/* Char_Caught 0 */
const Sint32 pl38_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   336, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl38_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl38_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   337, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl38_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl38_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   338, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl38_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl38_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl38_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl38_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   340, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl38_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl38_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   341, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl38_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl38_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   342, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl38_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl38_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   343, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl38_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl38_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   344, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl38_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl38_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   345, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl38_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   346, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl38_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   347, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl38_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   349, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl38_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl38_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   353, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl38_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl38_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   355, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl38_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl38_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   356, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl38_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl38_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   357, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl38_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   358, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl38_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   368, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl38_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   374, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl38_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl38_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   190, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl38_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl38_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   351, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl38_caught_21 ),
};

const Sint32 pl38_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   1, END_OF_DATA,    81, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006780,
};

/* Char_Effect 0
   Char_Effect 1 */
const Sint32 pl38_effect_00[] = {
	CG_FRAME(    0,    0,  -1,           0,   382, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 2 */
const Sint32 pl38_effect_02[] = {
	CG_FRAME(    0,    0,  -1,           0,   383, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 3 */
const Sint32 pl38_effect_03[] = {
	CG_FRAME(    0,    0,  -1,           0,   384, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 4 */
const Sint32 pl38_effect_04[] = {
	CG_FRAME(    0,    0,  -1,           0,   385, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 5 */
const Sint32 pl38_effect_05[] = {
	CG_FRAME(    0,    0,  -1,           0,   386, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 6 */
const Sint32 pl38_effect_06[] = {
	CG_FRAME(    0,    0,  -1,           0,   387, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 7 */
const Sint32 pl38_effect_07[] = {
	CG_FRAME(    0,    0,  -1,           0,   388, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 8 */
const Sint32 pl38_effect_08[] = {
	CG_FRAME(    0,    0,  -1,           0,   389, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 9 */
const Sint32 pl38_effect_09[] = {
	CG_FRAME(    0,    0,  -1,           0,   390, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 10 */
const Sint32 pl38_effect_10[] = {
	CG_FRAME(    0,    0,  -1,           0,   391, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 11 */
const Sint32 pl38_effect_11[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   392, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_effect_00 ),
};

/* Char_Effect 12 */
const Sint32 pl38_effect_12[] = {
	CG_FRAME(    0,    0,  -1,           0,   401, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 13 */
const Sint32 pl38_effect_13[] = {
	CG_FRAME(    0,    0,  -1,           0,   402, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Effect 14 */
const Sint32 pl38_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   404, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   405, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   406, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   407, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   408, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   409, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   410, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   411, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,    -1, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_effect_14 ),
};

/* Char_Effect 15
   Char_Effect 16 */
const Sint32 pl38_effect_15[] = {
	CG_FRAME(    0,    0,   2,           0,   393, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   394, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   395, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   396, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   397, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   399, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_effect_15_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    -1, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_effect_15_loop ),
};

/* Char_Effect 17 */
const Sint32 pl38_effect_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   559, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl38_effect_18[] = {
	CG_FRAME(    0,    0,   2,           0,   656, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   657, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   658, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   659, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   657, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   658, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   659, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   660, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   661, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   662, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   663, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   664, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl38_effect_18_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    -1, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl38_effect_18_loop ),
};

/* group 27 0: pls38_01_sword_main */
const Sint32 pl38_grp27_00[] = {
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0, 32827 ),
	EF_FRAME(    0,    0,   1,           0, 32828 ),
	EF_FRAME(    0,    0,   1,           0, 32829 ),
	EF_FRAME(    0,    0,   1,           0, 32830 ),
	EF_FRAME(    0,    0,   1,           0, 32831 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0, 32832 ),
	EF_FRAME(    0,    0,   1,           0, 32833 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl38_grp27_00 ),
};

/* group 27 1: pls38_01_sword_main */
const Sint32 pl38_grp27_01[] = {
	EF_FRAME(    0,    0,   1,           0, 32834 ),
	EF_FRAME(    0,    0,   1,           0, 32835 ),
	EF_FRAME(    0,    0,   1,           0, 32836 ),
	EF_FRAME(    0,    0,   1,           0, 32837 ),
	EF_FRAME(    0,    0,   1,           0, 32838 ),
	EF_FRAME(    0,    0,   1,           0, 32839 ),
	EF_FRAME(    0,    0,   1,           0, 32840 ),
	EF_FRAME(    0,    0,   1,           0, 32841 ),
	EF_FRAME(    0,    0,   1,           0, 32842 ),
	EF_FRAME(    0,    0,   1,           0, 32843 ),
	EF_FRAME(    0,    0,   1,           0, 32844 ),
	EF_FRAME(    0,    0,   1,           0, 32845 ),
	EF_FRAME(    0,    0,   1,           0, 32846 ),
	EF_FRAME(    0,    0,   1,           0, 32847 ),
	EF_FRAME(    0,    0,   1,           0, 32848 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1,           0,    -1 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,    -1 ),
	CG_NEXT( pl38_grp27_01 ),
};
