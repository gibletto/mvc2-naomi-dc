/* Blackheart (character 35): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_35      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_35     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl35.c) says which group
                   and number each move uses.
   pl35_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl35_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl35_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl35_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_35
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_35, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_35 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl35_stand_00[], pl35_stand_02[], pl35_stand_03[], pl35_jump_00[],
	pl35_jump_00_loop[], pl35_jump_01[], pl35_jump_01_loop[], pl35_jump_02[], pl35_jump_02_loop[],
	pl35_jump_03[], pl35_jump_09[], pl35_jump_06[], pl35_jump_06_loop[], pl35_jump_07[],
	pl35_jump_07_loop[], pl35_jump_08[], pl35_jump_08_loop[], pl35_dash_00[], pl35_dash_04[],
	pl35_dash_05[], pl35_dash_01[], pl35_dash_06[], pl35_dash_07[], pl35_dash_02[], pl35_crouch_00[],
	pl35_crouch_01[], pl35_crouch_02[], pl35_turn_00[], pl35_turn_01[], pl35_guard_00[],
	pl35_guard_02[], pl35_guard_04[], pl35_guard_06[], pl35_air_guard_00[], pl35_air_guard_00_loop[],
	pl35_damage_00[], pl35_damage_01[], pl35_damage_02[], pl35_damage_03[], pl35_damage_04[],
	pl35_damage_05[], pl35_damage_06[], pl35_damage_07[], pl35_damage_08[], pl35_damage_09[],
	pl35_damage_09_loop[], pl35_damage_10[], pl35_damage_11[], pl35_damage_12[], pl35_damage_13[],
	pl35_damage_16[], pl35_damage_17[], pl35_damage_18[], pl35_damage_19[], pl35_damage_20[],
	pl35_damage_21[], pl35_damage_22[], pl35_damage_23[], pl35_damage_24[], pl35_damage_25[],
	pl35_damage_26[], pl35_damage_27[], pl35_damage_28[], pl35_damage_29[], pl35_damage_30[],
	pl35_damage_33[], pl35_damage_31[], pl35_dizzy_00[], pl35_catch_00[], pl35_catch_00_loop[],
	pl35_catch_01[], pl35_catch_02[], pl35_catch_03[], pl35_catch_03_loop[], pl35_catch_04[],
	pl35_catch_05[], pl35_catch_05_loop[], pl35_caught_00[], pl35_caught_01[], pl35_caught_02[],
	pl35_caught_03[], pl35_caught_04[], pl35_caught_05[], pl35_caught_06[], pl35_caught_07[],
	pl35_caught_08[], pl35_caught_09[], pl35_caught_10[], pl35_caught_11[], pl35_caught_12[],
	pl35_caught_13[], pl35_caught_14[], pl35_caught_15[], pl35_caught_16[], pl35_caught_17[],
	pl35_caught_18[], pl35_caught_19[], pl35_caught_20[], pl35_caught_21[], pl35_stand_p_00[],
	pl35_stand_p_00_loop[], pl35_stand_p_01[], pl35_stand_p_01_loop[], pl35_special_27[],
	pl35_special_27_loop[], pl35_stand_p_02[], pl35_stand_p_02_loop[], pl35_stand_k_00[],
	pl35_stand_k_01[], pl35_stand_k_01_loop[], pl35_stand_k_02[], pl35_stand_k_02_loop[],
	pl35_crouch_p_00[], pl35_crouch_p_00_loop[], pl35_crouch_p_01[], pl35_crouch_p_01_loop[],
	pl35_special_26[], pl35_special_26_loop[], pl35_crouch_p_02[], pl35_crouch_p_02_loop[],
	pl35_crouch_k_00[], pl35_crouch_k_01[], pl35_crouch_k_01_loop[], pl35_crouch_k_02[],
	pl35_crouch_k_02_loop[], pl35_jump_p_00[], pl35_jump_p_00_loop[], pl35_jump_p_01[],
	pl35_jump_p_01_loop[], pl35_jump_p_02[], pl35_jump_p_02_loop[], pl35_jump_k_00[],
	pl35_jump_k_01[], pl35_jump_k_01_loop[], pl35_jump_k_02[], pl35_super_16_loop[],
	pl35_get_up_00[], pl35_get_up_01[], pl35_get_up_02[], pl35_get_up_03[], pl35_get_up_05[],
	pl35_get_up_04[], pl35_get_up_08[], pl35_appear_00[], pl35_appear_00_loop[], pl35_appear_01[],
	pl35_appear_02[], pl35_appear_03[], pl35_appear_04[], pl35_appear_05[], pl35_appear_06[],
	pl35_appear_07[], pl35_appear_08[], pl35_appear_09[], pl35_appear_10[], pl35_appear_11[],
	pl35_appear_12[], pl35_appear_67[], pl35_appear_13[], pl35_appear_14[], pl35_appear_15[],
	pl35_appear_23[], pl35_appear_31[], pl35_appear_38[], pl35_appear_63[], pl35_appear_45[],
	pl35_win_00[], pl35_win_00_loop[], pl35_win_01[], pl35_win_01_loop[], pl35_win_03[],
	pl35_win_03_loop[], pl35_win_02[], pl35_win_02_loop[], pl35_change_02[], pl35_change_00[],
	pl35_change_00_loop[], pl35_change_01[], pl35_change_01_loop[], pl35_change_03[],
	pl35_change_03_loop[], pl35_extra_00[], pl35_extra_00_loop[], pl35_extra_01[],
	pl35_extra_01_loop[], pl35_extra_02[], pl35_extra_02_loop[], pl35_extra_03[],
	pl35_extra_03_loop[], pl35_extra_04[], pl35_extra_04_loop[], pl35_change_04[],
	pl35_change_04_loop[], pl35_extra_05[], pl35_extra_05_loop[], pl35_special_00[],
	pl35_special_00_loop[], pl35_special_01[], pl35_special_01_loop[], pl35_special_02[],
	pl35_effect_00[], pl35_effect_01[], pl35_effect_02[], pl35_effect_03[], pl35_effect_03_loop[],
	pl35_effect_04[], pl35_effect_04_loop[], pl35_effect_05[], pl35_effect_05_loop[],
	pl35_effect_06[], pl35_effect_06_loop[], pl35_effect_07[], pl35_effect_08[], pl35_effect_09[],
	pl35_effect_10[], pl35_effect_11[], pl35_effect_13[], pl35_effect_14[], pl35_effect_15[],
	pl35_effect_16[], pl35_effect_17[], pl35_effect_18[], pl35_effect_22[], pl35_special_03[],
	pl35_special_03_loop[], pl35_special_04[], pl35_special_04_loop[], pl35_special_06[],
	pl35_special_07[], pl35_special_09[], pl35_special_10[], pl35_special_10_loop[],
	pl35_special_15[], pl35_special_15_loop[], pl35_special_17[], pl35_special_21[],
	pl35_special_18[], pl35_special_18_loop[], pl35_special_19[], pl35_special_23[],
	pl35_special_24[], pl35_special_25[], pl35_super_00[], pl35_super_00_loop[], pl35_super_01[],
	pl35_super_02[], pl35_super_03[], pl35_super_04[], pl35_super_04_loop[], pl35_super_05[],
	pl35_super_05_loop[], pl35_super_06[], pl35_super_12[], pl35_super_13[], pl35_super_13_loop[],
	pl35_super_14[], pl35_super_15[], pl35_super_15_loop[], pl35_super_16[], pl35_grp25_00[],
	pl35_grp25_00_loop[], pl35_grp25_27[], pl35_grp25_27_loop[], pl35_grp25_01[], pl35_grp25_02[],
	pl35_grp25_03[], pl35_grp25_04[], pl35_grp25_04_loop[], pl35_grp25_05[], pl35_grp25_05_loop[],
	pl35_grp25_06[], pl35_grp25_06_loop[], pl35_grp25_07[], pl35_grp25_08[], pl35_grp25_09[],
	pl35_grp25_10[], pl35_grp25_11[], pl35_grp25_12[], pl35_grp25_13[], pl35_grp25_13_loop[],
	pl35_grp25_14[], pl35_grp25_15[], pl35_grp25_16[], pl35_grp25_17[], pl35_grp25_18[],
	pl35_grp25_19[], pl35_grp25_19_loop[], pl35_grp25_20[], pl35_grp25_20_loop[], pl35_grp25_21[],
	pl35_grp25_21_loop[], pl35_grp25_22[], pl35_grp25_22_loop[], pl35_grp25_23[], pl35_grp25_24[],
	pl35_grp25_25[], pl35_grp25_26[];
extern const Sint32 *const pl35_stand_tbl[], *const pl35_jump_tbl[], *const pl35_dash_tbl[],
	*const pl35_crouch_tbl[], *const pl35_turn_tbl[], *const pl35_guard_tbl[],
	*const pl35_air_guard_tbl[], *const pl35_stand_p_tbl[], *const pl35_stand_k_tbl[],
	*const pl35_crouch_p_tbl[], *const pl35_crouch_k_tbl[], *const pl35_jump_p_tbl[],
	*const pl35_jump_k_tbl[], *const pl35_damage_tbl[], *const pl35_dizzy_tbl[],
	*const pl35_catch_tbl[], *const pl35_caught_tbl[], *const pl35_get_up_tbl[],
	*const pl35_appear_tbl[], *const pl35_win_tbl[], *const pl35_extra_tbl[],
	*const pl35_special_tbl[], *const pl35_super_tbl[], *const pl35_effect_tbl[],
	*const pl35_change_tbl[], *const pl35_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_35[16] = {
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
const Sint32 *const *const char_tbl_35[] = {
	pl35_stand_tbl, pl35_jump_tbl, pl35_dash_tbl, pl35_crouch_tbl, pl35_turn_tbl, pl35_guard_tbl,
	pl35_air_guard_tbl, pl35_stand_p_tbl, pl35_stand_k_tbl, pl35_crouch_p_tbl, pl35_crouch_k_tbl, pl35_jump_p_tbl,
	pl35_jump_k_tbl, pl35_damage_tbl, pl35_dizzy_tbl, pl35_catch_tbl, pl35_caught_tbl, pl35_get_up_tbl,
	pl35_appear_tbl, pl35_win_tbl, pl35_extra_tbl, pl35_special_tbl, pl35_super_tbl, pl35_effect_tbl,
	pl35_change_tbl, pl35_grp25_tbl, 0,
};

const Sint32 *const pl35_stand_tbl[] = {
	pl35_stand_00, pl35_stand_00, pl35_stand_02, pl35_stand_03, pl35_stand_00, pl35_stand_00,
	pl35_stand_02, pl35_stand_03, 0,
};

const Sint32 *const pl35_jump_tbl[] = {
	pl35_jump_00, pl35_jump_01, pl35_jump_02, pl35_jump_03, pl35_jump_03, pl35_jump_03,
	pl35_jump_06, pl35_jump_07, pl35_jump_08, pl35_jump_09, pl35_jump_09, pl35_jump_09,
	0,
};

const Sint32 *const pl35_dash_tbl[] = {
	pl35_dash_00, pl35_dash_01, pl35_dash_02, pl35_dash_02, pl35_dash_04, pl35_dash_05,
	pl35_dash_06, pl35_dash_07, 0,
};

const Sint32 *const pl35_crouch_tbl[] = {
	pl35_crouch_00, pl35_crouch_01, pl35_crouch_02, 0,
};

const Sint32 *const pl35_turn_tbl[] = {
	pl35_turn_00, pl35_turn_01, 0,
};

const Sint32 *const pl35_guard_tbl[] = {
	pl35_guard_00, pl35_guard_00, pl35_guard_02, pl35_guard_00, pl35_guard_04, pl35_guard_04,
	pl35_guard_06, pl35_guard_04, 0,
};

const Sint32 *const pl35_air_guard_tbl[] = {
	pl35_air_guard_00, pl35_air_guard_00, pl35_air_guard_00, 0,
};

const Sint32 *const pl35_stand_p_tbl[] = {
	pl35_stand_p_00, pl35_stand_p_01, pl35_stand_p_02, 0,
};

const Sint32 *const pl35_stand_k_tbl[] = {
	pl35_stand_k_00, pl35_stand_k_01, pl35_stand_k_02, 0,
};

const Sint32 *const pl35_crouch_p_tbl[] = {
	pl35_crouch_p_00, pl35_crouch_p_01, pl35_crouch_p_02, 0,
};

const Sint32 *const pl35_crouch_k_tbl[] = {
	pl35_crouch_k_00, pl35_crouch_k_01, pl35_crouch_k_02, 0,
};

const Sint32 *const pl35_jump_p_tbl[] = {
	pl35_jump_p_00, pl35_jump_p_01, pl35_jump_p_02, pl35_jump_p_00, pl35_jump_p_01, pl35_jump_p_02,
	0,
};

const Sint32 *const pl35_jump_k_tbl[] = {
	pl35_jump_k_00, pl35_jump_k_01, pl35_jump_k_02, pl35_jump_k_00, pl35_jump_k_01, pl35_jump_k_02,
	0,
};

const Sint32 *const pl35_damage_tbl[] = {
	pl35_damage_00, pl35_damage_01, pl35_damage_02, pl35_damage_03, pl35_damage_04, pl35_damage_05,
	pl35_damage_06, pl35_damage_07, pl35_damage_08, pl35_damage_09, pl35_damage_10, pl35_damage_11,
	pl35_damage_12, pl35_damage_13, pl35_damage_13, pl35_damage_13, pl35_damage_16, pl35_damage_17,
	pl35_damage_18, pl35_damage_19, pl35_damage_20, pl35_damage_21, pl35_damage_22, pl35_damage_23,
	pl35_damage_24, pl35_damage_25, pl35_damage_26, pl35_damage_27, pl35_damage_28, pl35_damage_29,
	pl35_damage_30, pl35_damage_31, pl35_damage_30, pl35_damage_33, pl35_damage_13, 0,
};

const Sint32 *const pl35_dizzy_tbl[] = {
	pl35_dizzy_00, 0,
};

const Sint32 *const pl35_catch_tbl[] = {
	pl35_catch_00, pl35_catch_01, pl35_catch_02, pl35_catch_03, pl35_catch_04, pl35_catch_05,
	0,
};

const Sint32 *const pl35_caught_tbl[] = {
	pl35_caught_00, pl35_caught_01, pl35_caught_02, pl35_caught_03, pl35_caught_04, pl35_caught_05,
	pl35_caught_06, pl35_caught_07, pl35_caught_08, pl35_caught_09, pl35_caught_10, pl35_caught_11,
	pl35_caught_12, pl35_caught_13, pl35_caught_14, pl35_caught_15, pl35_caught_16, pl35_caught_17,
	pl35_caught_18, pl35_caught_19, pl35_caught_20, pl35_caught_21, 0,
};

const Sint32 *const pl35_get_up_tbl[] = {
	pl35_get_up_00, pl35_get_up_01, pl35_get_up_02, pl35_get_up_03, pl35_get_up_04, pl35_get_up_05,
	pl35_get_up_05, pl35_get_up_05, pl35_get_up_08, 0,
};

const Sint32 *const pl35_appear_tbl[] = {
	pl35_appear_00, pl35_appear_01, pl35_appear_02, pl35_appear_03, pl35_appear_04, pl35_appear_05,
	pl35_appear_06, pl35_appear_07, pl35_appear_08, pl35_appear_09, pl35_appear_10, pl35_appear_11,
	pl35_appear_12, pl35_appear_13, pl35_appear_14, pl35_appear_15, pl35_appear_15, pl35_appear_15,
	pl35_appear_15, pl35_appear_15, pl35_appear_15, pl35_appear_15, pl35_appear_15, pl35_appear_23,
	pl35_appear_23, pl35_appear_23, pl35_appear_23, pl35_appear_23, pl35_appear_23, pl35_appear_23,
	pl35_appear_23, pl35_appear_31, pl35_appear_31, pl35_appear_31, pl35_appear_31, pl35_appear_31,
	pl35_appear_31, pl35_appear_31, pl35_appear_38, pl35_appear_38, pl35_appear_38, pl35_appear_38,
	pl35_appear_38, pl35_appear_38, pl35_appear_38, pl35_appear_45, pl35_appear_45, pl35_appear_45,
	pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_45,
	pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_45,
	pl35_appear_45, pl35_appear_45, pl35_appear_45, pl35_appear_63, pl35_appear_63, pl35_appear_63,
	pl35_appear_63, pl35_appear_67, 0,
};

const Sint32 *const pl35_win_tbl[] = {
	pl35_win_00, pl35_win_01, pl35_win_02, pl35_win_03, pl35_win_03, pl35_win_03,
	0,
};

const Sint32 *const pl35_extra_tbl[] = {
	pl35_extra_00, pl35_extra_01, pl35_extra_02, pl35_extra_03, pl35_extra_04, pl35_extra_05,
	0,
};

const Sint32 *const pl35_special_tbl[] = {
	pl35_special_00, pl35_special_01, pl35_special_02, pl35_special_03, pl35_special_04, pl35_special_04,
	pl35_special_06, pl35_special_07, pl35_special_07, pl35_special_09, pl35_special_10, pl35_special_10,
	pl35_special_10, pl35_special_10, pl35_special_10, pl35_special_15, pl35_special_10, pl35_special_17,
	pl35_special_18, pl35_special_19, pl35_special_10, pl35_special_21, pl35_special_10, pl35_special_23,
	pl35_special_24, pl35_special_25, pl35_special_26, pl35_special_27, 0,
};

const Sint32 *const pl35_super_tbl[] = {
	pl35_super_00, pl35_super_01, pl35_super_02, pl35_super_03, pl35_super_04, pl35_super_05,
	pl35_super_06, pl35_super_00, pl35_super_00, pl35_super_00, pl35_super_00, pl35_super_00,
	pl35_super_12, pl35_super_13, pl35_super_14, pl35_super_15, pl35_super_16, 0,
};

const Sint32 *const pl35_effect_tbl[] = {
	pl35_effect_00, pl35_effect_01, pl35_effect_02, pl35_effect_03, pl35_effect_04, pl35_effect_05,
	pl35_effect_06, pl35_effect_07, pl35_effect_08, pl35_effect_09, pl35_effect_10, pl35_effect_11,
	pl35_effect_11, pl35_effect_13, pl35_effect_14, pl35_effect_15, pl35_effect_16, pl35_effect_17,
	pl35_effect_18, pl35_effect_16, pl35_effect_17, pl35_effect_18, pl35_effect_22, 0,
};

const Sint32 *const pl35_change_tbl[] = {
	pl35_change_00, pl35_change_01, pl35_change_02, pl35_change_03, pl35_change_04, 0,
};

const Sint32 *const pl35_grp25_tbl[] = {
	pl35_grp25_00, pl35_grp25_01, pl35_grp25_02, pl35_grp25_03, pl35_grp25_04, pl35_grp25_05,
	pl35_grp25_06, pl35_grp25_07, pl35_grp25_08, pl35_grp25_09, pl35_grp25_10, pl35_grp25_11,
	pl35_grp25_12, pl35_grp25_13, pl35_grp25_14, pl35_grp25_15, pl35_grp25_16, pl35_grp25_17,
	pl35_grp25_18, pl35_grp25_19, pl35_grp25_20, pl35_grp25_21, pl35_grp25_22, pl35_grp25_23,
	pl35_grp25_24, pl35_grp25_25, pl35_grp25_26, pl35_grp25_27, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl35_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    77, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    80, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    81, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl35_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    86, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    87, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    88, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    89, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    90, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    91, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    92, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    93, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    94, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    95, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    96, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    97, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl35_stand_03[] = {
	CG_FRAME(    0,    0,    5,           0,   109, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   108, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   107, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   106, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   105, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   104, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   103, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   102, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   101, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   100, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    99, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    98, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl35_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl35_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_jump_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl35_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   150, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   151, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   152, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_jump_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl35_jump_03[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   145, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   147, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_jump_03 ),
};

/* Char_Jump 9: armageddon_move (Armageddon (hyper)), act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl35_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_09 ),
};

/* Char_Jump 6 */
const Sint32 pl35_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_jump_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl35_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   134, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   135, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   136, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   137, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   138, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_jump_07_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl35_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   144, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   150, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   151, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   152, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   139, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   140, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   141, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_jump_08_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   142, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   143, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl35_dash_00[] = {
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x00,    0,  0x00, 0x1f,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x81,    4, END_OF_DATA,   114, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl35_dash_00 ),
};

/* Char_Dash 4: dash_move, fly_start_init, fly_start2_init */
const Sint32 pl35_dash_04[] = {
	CG_FRAME(    0, 0x80,    4,           0,   690, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   691, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   692, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   693, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   694, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   695, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   696, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    1, END_OF_DATA,   156, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl35_dash_04 ),
};

/* Char_Dash 5: dash_move */
const Sint32 pl35_dash_05[] = {
	CG_FRAME(    0, 0x80,    4,           0,   115, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   116, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x82,    4,           0,   117, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x83,    4,           0,   118, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x84,    4,           0,   119, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   120, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4, END_OF_DATA,   121, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl35_dash_05 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl35_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,    0,  0x00, 0x1f,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   124, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x85,    2,           0,   126, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    2, END_OF_DATA,   127, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl35_dash_01 ),
};

/* Char_Dash 6: noki_move */
const Sint32 pl35_dash_06[] = {
	CG_FRAME(    0, 0x80,    4,           0,   697, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   698, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   699, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   700, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   701, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   702, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   703, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    1, END_OF_DATA,   156, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl35_dash_06 ),
};

/* Char_Dash 7: noki_move */
const Sint32 pl35_dash_07[] = {
	CG_FRAME(    0, 0x80,    4,           0,   128, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x86,    4,           0,   129, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x87,    4,           0,   130, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x88,    4,           0,   131, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4,           0,   132, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0, 0x80,    4, END_OF_DATA,   133, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl35_dash_07 ),
};

/* Char_Dash 2: dash_stop
   Char_Dash 3: noki_stop */
const Sint32 pl35_dash_02[] = {
	CG_FRAME(    1,    0,    4,           0,   156, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    1,    0,    4,           0,   157, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    1,    0,    4, END_OF_DATA,   158, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_dash_02 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl35_crouch_00[] = {
	CG_FRAME(    0,    3,    4,           0,   153, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    2,    4,           0,   154, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   155, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl35_crouch_01[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    85, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl35_crouch_02[] = {
	CG_FRAME(    0,    3,    4,           0,   155, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,    4,           0,   156, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,    4,           0,   157, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl35_turn_00[] = {
	CG_FRAME(    0,    0,    5,           0,   159, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    5,           0,   160, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    5,           0,   161, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   161, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl35_turn_01[] = {
	CG_FRAME(    0,    0,    5,           0,   162, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   163, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   164, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   164, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_turn_01 ),
};

/* Char_Guard 0
   Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl35_guard_00[] = {
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_guard_00 ),
};

/* Char_Guard 2 */
const Sint32 pl35_guard_02[] = {
	CG_FRAME(    0,    0,    3,           0,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_guard_02 ),
};

/* Char_Guard 4
   Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl35_guard_04[] = {
	CG_FRAME(    0,    0,    3,           0,   167, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   168, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_guard_04 ),
};

/* Char_Guard 6 */
const Sint32 pl35_guard_06[] = {
	CG_FRAME(    0,    0,    3,           0,   169, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   169, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard
   Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl35_air_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl35_air_guard_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   172, 0x80,   0x40,    0,  0x00,    0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl35_air_guard_00_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl35_damage_00[] = {
	CG_FRAME(    0,    0,    7,           0,   580, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   581, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl35_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl35_damage_01[] = {
	CG_FRAME(    0,    0,   10,           0,   579, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   580, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   581, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl35_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl35_damage_02[] = {
	CG_FRAME(    0,    0,   12,           0,   578, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    5,           0,   579, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    5,           0,   580, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   581, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl35_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl35_damage_03[] = {
	CG_FRAME(    0,    0,    7,           0,   584, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl35_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl35_damage_04[] = {
	CG_FRAME(    0,    0,    8,           0,   583, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    5,           0,   584, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl35_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl35_damage_05[] = {
	CG_FRAME(    0,    0,    8,           0,   582, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4,           0,   583, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    8,           0,   584, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl35_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl35_damage_06[] = {
	CG_FRAME(    0,    0,    7,           0,   590, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   590, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl35_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl35_damage_07[] = {
	CG_FRAME(    0,    0,    8,           0,   587, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    5,           0,   588, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   589, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl35_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl35_damage_08[] = {
	CG_FRAME(    0,    0,    8,           0,   586, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    6,           0,   587, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0,   588, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   589, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   589, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl35_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl35_damage_09[] = {
	CG_FRAME(    0,    0,    7,           0,   593, 0x80,   0x60,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   594, 0x80,   0x60,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   595, 0x80,   0x60,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   596, 0x80,   0x80,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl35_damage_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   597, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl35_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl35_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   591, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl35_damage_11[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   583, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl35_damage_12[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl35_damage_13[] = {
	CG_FRAME(    0,    0,    5,           0,   592, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   607, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl35_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl35_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl35_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   600, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl35_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   601, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl35_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   602, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl35_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   603, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl35_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    2,           0,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
};

/* Char_Damage 22 */
const Sint32 pl35_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   600, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
};

/* Char_Damage 23 */
const Sint32 pl35_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   601, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
};

/* Char_Damage 24 */
const Sint32 pl35_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   602, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
};

/* Char_Damage 25 */
const Sint32 pl35_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   603, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl35_damage_26[] = {
	CG_FRAME(    0,    0,    4,           0,   604, 0x80,   0xa0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4,           0,   605, 0x80,   0xa0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   606, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl35_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl35_damage_27[] = {
	CG_FRAME(    0,    0,    2,           0,   316, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    4,           0,   589, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    4,           0,   590, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    2,           0,   157, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   158, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl35_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl35_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   612, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   613, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   614, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   615, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   616, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl35_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   617, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   618, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   619, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   620, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4,           0,   621, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   622, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl35_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl35_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,   609, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   610, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   611, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_damage_30 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl35_damage_33[] = {
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   317, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   318, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   589, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   589, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_damage_33 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl35_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl35_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl35_dizzy_00[] = {
	CG_FRAME(    0,    0,    8,           0,   578, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   579, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   580, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   579, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl35_catch_00[] = {
	CG_FRAME(    0,    0,    4,           0,   484, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    1,    4,           0,   485, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   487, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    1,    0,    4,           0,   488, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   489, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
};

const Sint32 pl35_catch_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   490, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   491, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   492, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   493, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   495, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   496, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   497, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    4,           0,   499, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   500, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0003,   0 ),
	CG_NEXT( pl35_catch_00_loop ),
};

/* Char_Catch 1: p_catch_wait */
const Sint32 pl35_catch_01[] = {
	CG_FRAME(    0,    0,    4,           0,   501, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   502, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    3,           0,   365, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,    2,           0,   365, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    6,           0,   365, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    4,           0,   503, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,   26,           0,   504, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   505, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_catch_01 ),
};

/* Char_Catch 2: nml_k_set */
const Sint32 pl35_catch_02[] = {
	CG_FRAME(    1, 0x80,    4,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1, 0x81,    4,           0,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    1, 0x80,    4,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1, 0x81,    4,           0,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    1, 0x80,    4,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1, 0x81,    4,           0,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    1, 0x80,    4,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1, 0x81,    4,           0,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    1, 0x80,    4,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1, 0x81,    4,           0,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    1, 0x80,    4,           0,   463, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    1, 0x81,    4,           0,   464, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    1,    0,    4,           0,   465, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    1,    0,    3,           0,   466, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    1,    0,    3,           0,   467, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    1,    0,    2,           0,   468, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    1,    0,    8,           0,   468, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    1,    0,    3,           0,   469, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    3,           0,   470, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    1,    1,   15,           0,   471, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    1,    0,    3,           0,   472, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   471, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   472, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   471, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   472, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3,           0,   473, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    3, END_OF_DATA,   474, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_catch_02 ),
};

/* Char_Catch 3: k_catch_end */
const Sint32 pl35_catch_03[] = {
	CG_FRAME(    1,    0,    1,           0,   475, 0x80,   0x00,    0,  0x00, 0x30,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    1,           0,   476, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    1,           0,   477, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    1,           0,   478, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    1,           0,   479, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    1,           0,   480, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    1,           0,   481, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_catch_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_catch_03_loop ),
};

/* Char_Catch 4: jump_set */
const Sint32 pl35_catch_04[] = {
	CG_FRAME(    0,    0,    6,           0,   543, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   16,           0,   544, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    2,           0,   545, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    1,    2,           0,   546, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,    2,           0,   547, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   548, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   549, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   550, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   551, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   552, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_catch_04 ),
};

/* Char_Catch 5: j_catch_end */
const Sint32 pl35_catch_05[] = {
	CG_FRAME(    0,    0,    2,           0,   553, 0x80,   0x00,    0,  0x00, 0x30,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   554, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   555, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_catch_05_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   556, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_catch_05_loop ),
};

/* Char_Caught 0 */
const Sint32 pl35_caught_00[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   580, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl35_caught_01[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   579, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl35_caught_02[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   578, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl35_caught_03[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   582, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl35_caught_04[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   583, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl35_caught_05[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   584, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl35_caught_06[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   590, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl35_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl35_caught_07[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   588, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl35_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl35_caught_08[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   587, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl35_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl35_caught_09[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   584, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl35_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl35_caught_10[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   583, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl35_caught_11[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   585, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl35_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl35_caught_12[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   592, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl35_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl35_caught_13[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl35_caught_14[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   600, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl35_caught_15[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   601, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl35_caught_16[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   602, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl35_caught_17[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   603, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl35_caught_18[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   598, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl35_caught_19[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   617, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl35_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl35_caught_20[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   608, 0x80,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl35_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl35_caught_21[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   596, 0x80,   0xa0,    0,  0x00,    0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl35_caught_21 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl35_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   339, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   340, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  11 ),
};

const Sint32 pl35_stand_p_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   341, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_stand_p_00_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl35_stand_p_01[] = {
	CG_FRAME(    0,    0,   12,           0,   342, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   343, 0x80,   0x00,  ATK,  0x00, 0x22,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   344, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   22,           0,   344, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
};

const Sint32 pl35_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   374, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_stand_p_01_loop ),
};

/* Char_Special 27: sp_gamma (the gamma assist's and counter's attack) */
const Sint32 pl35_special_27[] = {
	CG_FRAME(    0,    0,   12,           0,   342, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   343, 0x80,   0x00,  ATK,  0x00, 0x22,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   344, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   22,           0,   344, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
};

const Sint32 pl35_special_27_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   374, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_special_27_loop ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl35_stand_p_02[] = {
	CG_FRAME(    0,    0,   12,           0,   353, 0x80,   0x00,  ATK,  0x00, 0x05,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   354, 0x80,   0x00,  ATK,  0x00, 0x21,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,    1,           0,   355, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    1,           0,   356, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4,           0,   357, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   358, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   359, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   360, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   361, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   363, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   357, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   358, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   359, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   360, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   361, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   362, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   363, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   357, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   358, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   359, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   360, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   361, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   362, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   363, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl35_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   364, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_stand_p_02_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl35_stand_k_00[] = {
	CG_FRAME(    0,    0,    5,           0,   365, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   366, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    4,           0,   367, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   368, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl35_stand_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   369, 0xa0,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   370, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    3,           0,   370, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    8,           0,   371, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    4,           0,   372, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   372, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   373, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl35_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   374, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_stand_k_01_loop ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl35_stand_k_02[] = {
	CG_FRAME(    0,    0,   10,           0,   375, 0xa0,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    8,           0,   376, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0, 0x80,    6,           0,   377, 0xa0,   0xa9,  ATK,  0x00, 0x20,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0xa0,   0xa9,  ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0xa0,   0xa9,  ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   27,           0,   380, 0xa0,   0xa9,  ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    6,           0,   381, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl35_stand_k_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   382, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_stand_k_02_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl35_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   383, 0xa0,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   383, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3,           0,   385, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  27 ),
};

const Sint32 pl35_crouch_p_00_loop[] = {
	CG_FRAME(    0,    1,    1, END_OF_DATA,   385, 0xa0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_p_00_loop ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl35_crouch_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   386, 0xa0,   0x80,  ATK,  0x00, 0x04,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,    4,           0,   386, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   24,           0,   389, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   390, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
};

const Sint32 pl35_crouch_p_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   393, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_p_01_loop ),
};

/* Char_Special 26: snapback_init (the snapback) */
const Sint32 pl35_special_26[] = {
	CG_FRAME(    0,    0,    8,           0,   386, 0xa0,   0x80,  ATK,  0x00, 0x04,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,    4,           0,   386, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    1,    0,    4,           0,   386, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   24,           0,   389, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   390, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
};

const Sint32 pl35_special_26_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   393, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_special_26_loop ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl35_crouch_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   405, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   406, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   407, 0xa0,   0x80,  ATK,  0x00, 0x05,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    8,   10,           0,   408, 0xa0,   0x80,  ATK,  0x00, 0x21,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   410, 0xa0,   0x80,  ATK,  0x00, 0x04,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0, 0x81,    4,           0,   411, 0xa0,   0x80,  ATK,  0x00, 0x21,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   412, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   413, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   414, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   414, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl35_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   414, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_p_02_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl35_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   415, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   416, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    2,           0,   417, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   417, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   418, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   419, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl35_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   420, 0xa0,   0x80,  ATK,  0x00, 0x03,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   420, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   421, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    4,           0,   421, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    0,           0,   424, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl35_crouch_k_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   424, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_k_01_loop ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl35_crouch_k_02[] = {
	CG_FRAME(    0,    0,    7,           0,   425, 0xa0,   0x80,  ATK,  0x00, 0x04,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    5,           0,   426, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    2,           0,   427, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    3,           0,   428, 0xa0,   0x80,  ATK,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0, 0x80,    3,           0,   429, 0xa0,   0x80,  ATK,  0x00, 0x20,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl35_crouch_k_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   429, 0xa0,   0x80,  ATE,  0x00,    0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl35_crouch_k_02_loop ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl35_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   430, 0x80,   0x00,  ATK,  0x00, 0x03,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,    1,           0,   431, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x81,    0,    9,           0,   431, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    1,           0,   431, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  37 ),
};

const Sint32 pl35_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   432, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl35_jump_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   433, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    1,    0,    8,           0,   434, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x80,    1,           0,   435, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x81,    1,           0,   436, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x82,    1,           0,   437, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x83,    1,           0,   436, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x84,    1,           0,   437, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x85,    1,           0,   436, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x86,    1,           0,   437, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x87,    1,           0,   436, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x88,    1,           0,   436, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
};

const Sint32 pl35_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   432, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl35_jump_p_02[] = {
	CG_FRAME(    0,    0,   12,           0,   454, 0x80,   0x00,  ATK,  0x00, 0x05,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4,           0,   447, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x80,    4,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   12,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   12,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   12,           0,   448, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
};

const Sint32 pl35_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   432, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl35_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   449, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   10,           0,   450, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    4,           0,   451, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    4,           0,   452, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   453, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl35_jump_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   454, 0x80,   0x00,  ATK,  0x00, 0x05,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   455, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   456, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   457, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   458, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
};

const Sint32 pl35_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   432, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl35_jump_k_02[] = {
	CG_FRAME(    0,    0,   16,           0,   459, 0x80,   0x00,  ATK,  0x00, 0x05,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   460, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    1,   38,           0,   461, 0x80,   0x00,  ATK,  0x00, 0x20,    0,   0,     0,      0,  38 ),
};

const Sint32 pl35_super_16_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   462, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_super_16_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl35_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   315, 0x40,   0xe0,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   316, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   589, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   590, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   158, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl35_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   315, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   319, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   320, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   589, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   590, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    85, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   157, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   158, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl35_get_up_02[] = {
	CG_FRAME(    0,    0,    6,           0,   315, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   319, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   589, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   590, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   157, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   158, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl35_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   315, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   317, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   318, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   589, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   590, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,    85, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   157, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   158, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_03 ),
};

/* Char_Get_Up 5
   Char_Get_Up 6
   Char_Get_Up 7 */
const Sint32 pl35_get_up_05[] = {
	CG_FRAME(    0,    0,    4,           0,   590, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,    85, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_05 ),
};

/* Char_Get_Up 4 */
const Sint32 pl35_get_up_04[] = {
	CG_FRAME(    0,    0,    4,           0,   315, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   587, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   588, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   589, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   589, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_04 ),
};

/* Char_Get_Up 8 */
const Sint32 pl35_get_up_08[] = {
	CG_FRAME(    0,    0,    4,           0,   162, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   157, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_get_up_08 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl35_appear_00[] = {
	CG_FRAME(    0,    1,   24,           0,    -1, 0x80,   0xff,    0,  0x00, 0x17,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   16,           0,   216, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_appear_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   217, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_00_loop ),
};

/* Char_Appear 1 */
const Sint32 pl35_appear_01[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_01 ),
};

/* Char_Appear 2 */
const Sint32 pl35_appear_02[] = {
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   221, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_02 ),
};

/* Char_Appear 3 */
const Sint32 pl35_appear_03[] = {
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   223, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_03 ),
};

/* Char_Appear 4 */
const Sint32 pl35_appear_04[] = {
	CG_FRAME(    0,    0,    2,           0,   224, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   225, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_04 ),
};

/* Char_Appear 5 */
const Sint32 pl35_appear_05[] = {
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   235, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_05 ),
};

/* Char_Appear 6 */
const Sint32 pl35_appear_06[] = {
	CG_FRAME(    0,    0,    2,           0,   236, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   237, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   238, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   239, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   244, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   246, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   247, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_06 ),
};

/* Char_Appear 7 */
const Sint32 pl35_appear_07[] = {
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   249, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   250, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   251, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   252, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   253, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   254, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   256, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   257, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   258, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   259, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_07 ),
};

/* Char_Appear 8 */
const Sint32 pl35_appear_08[] = {
	CG_FRAME(    0,    0,    2,           0,   260, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   261, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   262, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   264, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   265, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   266, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   267, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   268, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   271, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_08 ),
};

/* Char_Appear 9 */
const Sint32 pl35_appear_09[] = {
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   274, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   275, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   276, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   277, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   278, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   282, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   283, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_09 ),
};

/* Char_Appear 10 */
const Sint32 pl35_appear_10[] = {
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   289, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   292, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   294, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   295, 0x80,   0xff,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_10 ),
};

/* Char_Appear 11: appear_move */
const Sint32 pl35_appear_11[] = {
	CG_FRAME(    1,    0,    2,           0,   296, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   297, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   298, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   300, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   303, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   305, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   306, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   308, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   312, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   313, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   314, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_11 ),
};

/* Char_Appear 12: pls35_01_init (12 + type) */
const Sint32 pl35_appear_12[] = {
	CG_FRAME(    0,    0,    8,           0,   624, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   625, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   626, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   627, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   627, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   627, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   628, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

/* Char_Appear 67 */
const Sint32 pl35_appear_67[] = {
	CG_FRAME(    0,    0,    4,           0,   628, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   629, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_67 ),
};

/* Char_Appear 13 */
const Sint32 pl35_appear_13[] = {
	CG_FRAME(    0,    0,    4,           0,   630, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   631, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_13 ),
};

/* Char_Appear 14 */
const Sint32 pl35_appear_14[] = {
	CG_FRAME(    0,    0,    4,           0,   632, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   633, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_appear_14 ),
};

/* Char_Appear 15
   Char_Appear 16
   Char_Appear 17
   Char_Appear 18
   Char_Appear 19
   Char_Appear 20 */
const Sint32 pl35_appear_15[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,    4,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4,           0, 32770, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA, 32771, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl35_appear_15 ),
};

/* Char_Appear 23
   Char_Appear 24
   Char_Appear 25
   Char_Appear 26
   Char_Appear 27
   Char_Appear 28 */
const Sint32 pl35_appear_23[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,    4,           0, 32769, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4,           0, 32770, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA, 32771, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl35_appear_23 ),
};

/* Char_Appear 31
   Char_Appear 32
   Char_Appear 33
   Char_Appear 34
   Char_Appear 35
   Char_Appear 36 */
const Sint32 pl35_appear_31[] = {
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,    4,           0, 32773, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4,           0, 32774, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl35_appear_31 ),
};

/* Char_Appear 38
   Char_Appear 39
   Char_Appear 40
   Char_Appear 41
   Char_Appear 42
   Char_Appear 43 */
const Sint32 pl35_appear_38[] = {
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,    4,           0, 32773, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4,           0, 32774, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    3,    4, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl35_appear_38 ),
};

/* Char_Appear 63
   Char_Appear 64
   Char_Appear 65
   Char_Appear 66 */
const Sint32 pl35_appear_63[] = {
	CG_FRAME(    0,    0,    4,           0, 32776, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,    4,           0, 32777, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    2,    4, END_OF_DATA, 32778, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl35_appear_63 ),
};

/* Char_Appear 45
   Char_Appear 46
   Char_Appear 47
   Char_Appear 48
   Char_Appear 49
   Char_Appear 50 */
const Sint32 pl35_appear_45[] = {
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA, 32780, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl35_appear_45 ),
};

/* Char_Win 0 */
const Sint32 pl35_win_00[] = {
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,    0,  0x00, 0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   187, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   188, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   200, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   201, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   203, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_win_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   205, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl35_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,    74, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    75, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x51,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   208, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   28,           0,   209, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x80,   0x58,    0,  0x00, 0x0f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_win_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   214, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_win_01_loop ),
};

/* Char_Win 3: lose
   Char_Win 4
   Char_Win 5 */
const Sint32 pl35_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,    74, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   153, 0x40,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   154, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_win_03_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   215, 0x40,   0x80,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_win_03_loop ),
};

/* Char_Win 2: win_time */
const Sint32 pl35_win_02[] = {
	CG_FRAME(    0,    0,    2,           0,   566, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   567, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   568, 0x80,   0x00,    0,  0x00, 0x17,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_win_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   569, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   570, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   571, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   572, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   573, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   574, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   575, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   576, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   577, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_win_02_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl35_change_02[] = {
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   149, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_change_02 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl35_change_00[] = {
	CG_FRAME(    0,    0,    6,           0,   566, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   567, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   568, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_change_00_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   569, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   570, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   571, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl35_change_01[] = {
	CG_FRAME(    0,    0,    8,           0,   370, 0x80,   0x51,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl35_change_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   370, 0x80,   0x51,  ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl35_change_01_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl35_change_03[] = {
	CG_FRAME(    0,    0,    3,           0,   207, 0x80,   0x51,  ATK,  0x00, 0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   208, 0x80,   0x74,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    5,           0,   208, 0x80,   0x74,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_change_03_loop[] = {
	CG_FRAME(    0,    0,   32, END_OF_DATA,   209, 0x80,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_change_03_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl35_extra_00[] = {
	CG_FRAME(    0,    0,    2,           0,   454, 0x80,   0x00,  ATK,  0x00, 0x17,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    2,           0,   455, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
};

const Sint32 pl35_extra_00_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   456, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl35_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl35_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x51,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   208, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   209, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   211, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x58,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_extra_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   213, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   214, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   214, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   214, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   213, 0x80,   0x97,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_extra_01_loop ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl35_extra_02[] = {
	CG_FRAME(    0,    0,    2,           0,   134, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   135, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   136, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   137, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   138, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_extra_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   141, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_extra_02_loop ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl35_extra_03[] = {
	CG_FRAME(    0,    0,    2,           0,   148, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   149, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   150, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   151, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   152, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   140, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl35_extra_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   141, 0x90,   0x20,    0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl35_extra_03_loop ),
};

/* Char_Extra 4: sp_chouhatsu (the taunt) */
const Sint32 pl35_extra_04[] = {
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x00,    0,  0x00, 0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   322, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   323, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   324, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   325, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   326, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   327, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   328, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   329, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   330, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   331, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   332, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   333, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   334, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   335, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   336, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_extra_04_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   338, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_extra_04_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl35_change_04[] = {
	CG_FRAME(    0,    0,    2,           0,   321, 0x80,   0x00,    0,  0x00, 0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   322, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   323, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   324, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   325, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   326, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   327, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   328, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   329, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   331, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   332, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   333, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   334, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   336, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_change_04_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   338, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_change_04_loop ),
};

/* Char_Extra 5: pl35_upper */
const Sint32 pl35_extra_05[] = {
	CG_FRAME(    0,    0,   12,           0,   342, 0x80,   0x00,  ATK,  0x00, 0x04,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   343, 0x80,   0x00,  ATK,  0x00, 0x22,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    3,           0,   344, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   22,           0,   344, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   345, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  15 ),
};

const Sint32 pl35_extra_05_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   374, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_extra_05_loop ),
};

/* Char_Special 0: inferno_init (Inferno) */
const Sint32 pl35_special_00[] = {
	CG_FRAME(    0,    0,    3,           0,   525, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   526, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   527, 0x80,   0x00,  ATK,  0x00, 0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   528, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   529, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    8,           0,   530, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   530, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_special_00_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   531, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   532, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   533, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   534, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   535, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   536, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   537, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   538, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   539, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   540, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   541, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   542, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_special_00_loop ),
};

/* Char_Special 1: thunder_init (Dark Thunder) */
const Sint32 pl35_special_01[] = {
	CG_FRAME(    0,    0,    4,           0,   507, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   507, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   508, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   509, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   510, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    1,    1,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   511, 0x80,   0x00,  ATK,  0x00, 0x30,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   512, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   513, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   514, 0x80,   0x00,  ATK,  0x00, 0x1e,    0,   0,     0,      0,   1 ),
};

const Sint32 pl35_special_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   515, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   516, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   517, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   518, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   519, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   520, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   521, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   522, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_special_01_loop ),
};

/* Char_Special 2: thunder_move (Dark Thunder) */
const Sint32 pl35_special_02[] = {
	CG_FRAME(    0,    0,    8,           0,   508, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   507, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   507, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl35_special_02 ),
};

/* Char_Effect 0 */
const Sint32 pl35_effect_00[] = {
	CG_FRAME(    0,    0,    3,           0,   865, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   866, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   867, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   868, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   869, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   870, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_00 ),
};

/* Char_Effect 1 */
const Sint32 pl35_effect_01[] = {
	CG_FRAME(    0,    0,    3,           0,   861, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   862, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl35_effect_02[] = {
	CG_FRAME(    0,    0,    3,           0,   863, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   864, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_02 ),
};

/* Char_Effect 3: pls35_00_p_catch_init */
const Sint32 pl35_effect_03[] = {
	CG_FRAME(    0,    0,    6,           0,   666, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   667, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   668, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   669, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   670, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   671, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   672, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   673, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   674, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   675, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   676, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   677, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   678, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   679, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   680, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   681, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   682, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   683, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   684, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   685, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   686, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   687, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   688, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   689, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_effect_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   666, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   667, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   668, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   669, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   670, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   671, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   672, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   673, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   674, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   675, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   676, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   677, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   678, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   679, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   680, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   681, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   682, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   683, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   684, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   685, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   686, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   687, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   688, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   689, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_03_loop ),
};

/* Char_Effect 4: shl35_04_mp_init */
const Sint32 pl35_effect_04[] = {
	CG_FRAME(    0,    0,    2,           0,   346, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   347, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   349, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   350, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   351, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_effect_04_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_04_loop ),
};

/* Char_Effect 5: shl35_04_cmp_init */
const Sint32 pl35_effect_05[] = {
	CG_FRAME(    0,    0,    2,           0,   394, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   395, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_effect_05_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   404, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_05_loop ),
};

/* Char_Effect 6: shl35_04_jump_mp_move */
const Sint32 pl35_effect_06[] = {
	CG_FRAME(    0,    0,    1,           0,   438, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    1,           0,   439, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    1,           0,   440, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   441, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   442, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    1,           0,   443, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   444, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   445, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_effect_06_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   446, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_06_loop ),
};

/* Char_Effect 7: pls35_00_k_catch_init */
const Sint32 pl35_effect_07[] = {
	CG_FRAME(    0,    0,   16,           0,   482, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   483, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_07 ),
};

/* Char_Effect 8: pls35_00_k_throw_init, pls35_00_j_throw_init */
const Sint32 pl35_effect_08[] = {
	CG_FRAME(    0,    0,    2,           0,   813, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   814, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   815, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   816, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   817, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   818, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   819, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_08 ),
};

/* Char_Effect 9: pls35_00_throw_move */
const Sint32 pl35_effect_09[] = {
	CG_FRAME(    0,    0,    1,           0,   820, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   821, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   822, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   823, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   824, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl35_effect_10[] = {
	CG_FRAME(    0,    0,    1,           0,   893, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   894, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    -1, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_10 ),
};

/* Char_Effect 11
   Char_Effect 12: shl35_03_base_init (12 + sub_type) */
const Sint32 pl35_effect_11[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   892, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_11 ),
};

/* Char_Effect 13 */
const Sint32 pl35_effect_13[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   891, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl35_effect_14[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   890, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl35_effect_15[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   889, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_15 ),
};

/* Char_Effect 16
   Char_Effect 19 */
const Sint32 pl35_effect_16[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32831, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_16 ),
};

/* Char_Effect 17
   Char_Effect 20 */
const Sint32 pl35_effect_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32830, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_17 ),
};

/* Char_Effect 18
   Char_Effect 21 */
const Sint32 pl35_effect_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32829, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_18 ),
};

/* Char_Effect 22: shl35_02_drop_init */
const Sint32 pl35_effect_22[] = {
	CG_FRAME(    0,    0,    2,           0,   892, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   891, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   890, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_effect_22 ),
};

/* Char_Special 3 */
const Sint32 pl35_special_03[] = {
	CG_FRAME(    0,    0,    3,           0,   843, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   844, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   845, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3,           0,   846, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3,           0,   847, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3,           0,   848, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    1,    3,           0,   849, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
};

const Sint32 pl35_special_03_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   850, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0081,  66 ),
	CG_FRAME(    0,    0,    3,           0,   851, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0082,  66 ),
	CG_FRAME(    0,    0,    3,           0,   852, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0083,  66 ),
	CG_FRAME(    0,    0,    3,           0,   853, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0084,  66 ),
	CG_FRAME(    0,    0,    3,           0,   854, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0085,  66 ),
	CG_FRAME(    0,    0,    3,           0,   855, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0086,  66 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   856, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0087,  66 ),
	CG_NEXT( pl35_special_03_loop ),
};

/* Char_Special 4
   Char_Special 5 */
const Sint32 pl35_special_04[] = {
	CG_FRAME(    0,    0,    3,           0,   825, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   826, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   827, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    3,           0,   828, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    3,           0,   829, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    1,    3,           0,   830, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl35_special_04_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   831, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0080,  69 ),
	CG_FRAME(    0,    0,    3,           0,   832, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0081,  69 ),
	CG_FRAME(    0,    0,    3,           0,   833, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0082,  69 ),
	CG_FRAME(    0,    0,    3,           0,   834, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0083,  69 ),
	CG_FRAME(    0,    0,    3,           0,   835, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0084,  69 ),
	CG_FRAME(    0,    0,    3,           0,   836, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0085,  69 ),
	CG_FRAME(    0,    0,    3,           0,   837, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0086,  69 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   838, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0087,  69 ),
	CG_NEXT( pl35_special_04_loop ),
};

/* Char_Special 6 */
const Sint32 pl35_special_06[] = {
	CG_FRAME(    0,    0,    3,           0,   857, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0088,   0 ),
	CG_FRAME(    0,    0,    3,           0,   858, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0089,   0 ),
	CG_FRAME(    0,    0,    3,           0,   859, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x008a,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   860, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x008b,   0 ),
	CG_NEXT( pl35_special_06 ),
};

/* Char_Special 7
   Char_Special 8 */
const Sint32 pl35_special_07[] = {
	CG_FRAME(    0,    0,    3,           0,   839, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0088,   0 ),
	CG_FRAME(    0,    0,    3,           0,   840, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x0089,   0 ),
	CG_FRAME(    0,    0,    3,           0,   841, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x008a,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   842, 0xb0,   0x00,  ATE,  0x00,    0,    0,   0,     0, 0x008b,   0 ),
	CG_NEXT( pl35_special_07 ),
};

/* Char_Special 9: shl35_01_init */
const Sint32 pl35_special_09[] = {
	CG_FRAME(    0,    0,    3,           0,   813, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0080,  72 ),
	CG_FRAME(    0,    0,    3,           0,   814, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0081,  72 ),
	CG_FRAME(    0,    0,    3,           0,   815, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0082,  72 ),
	CG_FRAME(    0,    0,    3,           0,   816, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0083,  72 ),
	CG_FRAME(    0,    0,    3,           0,   817, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0084,  72 ),
	CG_FRAME(    0,    0,    3,           0,   818, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0085,  72 ),
	CG_FRAME(    0,    0,    3,           0,   819, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0086,  72 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   820, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0, 0x0087,  72 ),
	CG_NEXT( pl35_special_09 ),
};

/* Char_Special 10
   Char_Special 11
   Char_Special 12
   Char_Special 13: shl35_03_init_1
   Char_Special 14
   Char_Special 16 */
const Sint32 pl35_special_10[] = {
	CG_FRAME(    0,    0,    1,           0,   704, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   705, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   706, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   707, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   708, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   709, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   710, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   711, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   712, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   713, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_special_10_loop[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   713, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_special_10_loop ),
};

/* Char_Special 15: shl35_03_base_fly */
const Sint32 pl35_special_15[] = {
	CG_FRAME(    0,    0,    7,           0, 32817, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0, 32817, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0, 32817, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_special_15_loop[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA, 32817, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_special_15_loop ),
};

/* Char_Special 17: shl35_03_pillar_init (17 + sub_type), shl35_03_pillar_up_init (17 + sub_type) */
const Sint32 pl35_special_17[] = {
	CG_FRAME(    0,    0,    2,           0,    -1, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    1,    2,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
};

/* Char_Special 21: shl35_03_pillar_up_init */
const Sint32 pl35_special_21[] = {
	CG_FRAME(    0,    0,    1,           0, 32799, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32800, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32801, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32802, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32803, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32804, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32805, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32806, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32807, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32808, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32809, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32810, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32811, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32812, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32813, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0, 32814, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32815, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl35_special_21 ),
};

/* Char_Special 18 */
const Sint32 pl35_special_18[] = {
	CG_FRAME(    0,    0,    2,           0,   735, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_special_18_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   714, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   715, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   716, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   717, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   718, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   719, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   720, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   721, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   722, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   723, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   724, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   725, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   726, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   727, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   728, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   729, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   730, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_special_18_loop ),
};

/* Char_Special 19: shl35_03_init_2 */
const Sint32 pl35_special_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32816, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_special_19 ),
};

/* Char_Special 23: shl35_02_drop_fall */
const Sint32 pl35_special_23[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   897, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_special_23 ),
};

/* Char_Special 24: shl35_02_drop_stay */
const Sint32 pl35_special_24[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   897, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_special_24 ),
};

/* Char_Special 25: shl35_02_blast_init */
const Sint32 pl35_special_25[] = {
	CG_FRAME(    0,    0,    1,           0, 32832, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32837, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32833, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32838, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32834, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32839, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32835, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32840, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1,           0, 32836, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32841, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl35_special_25 ),
};

/* Char_Super 0: heart_init (Heart of Darkness (hyper))
   Char_Super 7
   Char_Super 8
   Char_Super 9
   Char_Super 10
   Char_Super 11 */
const Sint32 pl35_super_00[] = {
	CG_FRAME(    0,    0,   12,           0,   207, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,    8,           0,   208, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    3,           0,   209, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6,           0,   210, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   211, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   557, 0xc0,   0x97,  ATK,  0x00, 0x16,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4,           0,   558, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
};

const Sint32 pl35_super_00_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   559, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   560, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4,           0,   561, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   13,    0,    4,           0,   562, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   563, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4,           0,   564, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   13,    0,    4,           0,   559, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   560, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4,           0,   561, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   13,    0,    4,           0,   562, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   563, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4, END_OF_DATA,   564, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl35_super_00_loop ),
};

/* Char_Super 1: heart_move (Heart of Darkness (hyper)) */
const Sint32 pl35_super_01[] = {
	CG_FRAME(    0,    0,   16,           0,   565, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    6,           0,    82, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl35_super_01 ),
};

/* Char_Super 2 */
const Sint32 pl35_super_02[] = {
	CG_FRAME(    0,    0,    6,           0, 32818, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA, 32819, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl35_super_02 ),
};

/* Char_Super 3: shl35_00_init */
const Sint32 pl35_super_03[] = {
	CG_FRAME(    0,    0,    6,           0, 32818, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA, 32819, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl35_super_03 ),
};

/* Char_Super 4: armageddon_init (Armageddon (hyper)) */
const Sint32 pl35_super_04[] = {
	CG_FRAME(    0,    0,   16,           0,   576, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,    6,           0,   576, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   566, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   567, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   568, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   569, 0x80,   0x00,  ATK,  0x00, 0x17,    0,   0,     0,      0,  44 ),
};

const Sint32 pl35_super_04_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   570, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   571, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   572, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   573, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   574, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   575, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   576, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   577, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   566, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   567, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6,           0,   568, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   569, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl35_super_04_loop ),
};

/* Char_Super 5 */
const Sint32 pl35_super_05[] = {
	CG_FRAME(    0,    0,    3,           0,   207, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,    8,           0,   208, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    3,           0,   209, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6,           0,   210, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    8,           0,   211, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0xc0,   0x58,  ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   557, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4,           0,   558, 0xc0,   0x97,  ATK,  0x00, 0x30,    0,   0,     0,      0,  62 ),
};

const Sint32 pl35_super_05_loop[] = {
	CG_FRAME(    0,    1,    4,           0,   559, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   560, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4,           0,   561, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   13,    0,    4,           0,   562, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   563, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4,           0,   564, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   13,    0,    4,           0,   559, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   560, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4,           0,   561, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   13,    0,    4,           0,   562, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   14,    0,    4,           0,   563, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(   15,    0,    4, END_OF_DATA,   564, 0xc0,   0x97,  ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl35_super_05_loop ),
};

/* Char_Super 6 */
const Sint32 pl35_super_06[] = {
	CG_FRAME(    0,    0,    4,           0,   565, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    84, 0x80,   0x00,    0,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl35_super_06 ),
};

/* Char_Super 12 */
const Sint32 pl35_super_12[] = {
	CG_FRAME(    0,    0,    2,           0,    74, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   339, 0x80,   0x00,  ATE,  0x00, 0x03,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,    4,           0,   339, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    1,    4,           0,   339, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   341, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_super_12 ),
};

/* Char_Super 13: judgement_init (Judgement Day) */
const Sint32 pl35_super_13[] = {
	CG_FRAME(    0,    0,   16,           0,   353, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   354, 0x80,   0x00,  ATK,  0x00, 0x05,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,    4,           0,   354, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    1,           0,   355, 0x80,   0x00,  ATK,  0x00, 0x21,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   356, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl35_super_13_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   357, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   358, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   359, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   360, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   361, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   363, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl35_super_13_loop ),
};

/* Char_Super 14 */
const Sint32 pl35_super_14[] = {
	CG_FRAME(    0,    0,   26, END_OF_DATA,   364, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl35_super_14 ),
};

/* Char_Super 15: judgement_init (Judgement Day) */
const Sint32 pl35_super_15[] = {
	CG_FRAME(    0,    0,   16,           0,   459, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    1,    3,           0,   460, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    8,           0,   461, 0x80,   0x00,  ATK,  0x00, 0x21,    0,   0,     0,      0,  38 ),
};

const Sint32 pl35_super_15_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   461, 0x80,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_super_15_loop ),
};

/* Char_Super 16 */
const Sint32 pl35_super_16[] = {
	CG_FRAME(    0,    0,   32,           0,   461, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   462, 0x80,   0x00,  ATE,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl35_super_16_loop ),
};

/* group 25 0: shl35_04_hp_init, shl35_04_chp_init */
const Sint32 pl35_grp25_00[] = {
	CG_FRAME( 0x80,    0,   14,           0, 32789, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    4,           0, 32789, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    4,           0, 32790, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    4,           0, 32792, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl35_grp25_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32781, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32782, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32783, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32785, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32786, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32787, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32788, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl35_grp25_00_loop ),
};

/* group 25 27: shl35_04_judgement_init */
const Sint32 pl35_grp25_27[] = {
	CG_FRAME(    0,    0,    2,           0, 32789, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    2,           0, 32790, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    2,           0, 32791, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    2,           0, 32792, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  47 ),
};

const Sint32 pl35_grp25_27_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32781, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32782, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32783, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32785, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32786, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0, 32787, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32788, 0xa0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl35_grp25_27_loop ),
};

/* group 25 1: shl35_04_hp_hold, shl35_04_chp_fly_out */
const Sint32 pl35_grp25_01[] = {
	CG_FRAME(    0,    0,    4,           0,   646, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   647, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   648, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   650, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   651, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_01 ),
};

/* group 25 2: shl35_04_hp_end_1, shl35_04_judgement_move, shl35_04_jjudgement_move */
const Sint32 pl35_grp25_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32793, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32798, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_02 ),
};

/* group 25 3: shl35_04_hp_end_0 */
const Sint32 pl35_grp25_03[] = {
	CG_FRAME(    0,    0,    4,           0, 32792, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32790, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32789, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_03 ),
};

/* group 25 4 */
const Sint32 pl35_grp25_04[] = {
	CG_FRAME(    0,    0,    4,           0,   767, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   768, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   769, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   740, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   741, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   742, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   743, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   744, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   745, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl35_grp25_04_loop ),
};

/* group 25 5 */
const Sint32 pl35_grp25_05[] = {
	CG_FRAME(    0,    0,    4,           0,   770, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   771, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   772, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_05_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   752, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   753, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   754, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   755, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4,           0,   756, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   757, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl35_grp25_05_loop ),
};

/* group 25 6 */
const Sint32 pl35_grp25_06[] = {
	CG_FRAME(    0,    0,    4,           0,   773, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   774, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   775, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   758, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   759, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   760, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   761, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   762, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   763, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl35_grp25_06_loop ),
};

/* group 25 7 */
const Sint32 pl35_grp25_07[] = {
	CG_FRAME(    0,    0,    4,           0,   743, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   744, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   745, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   744, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_07 ),
};

/* group 25 8 */
const Sint32 pl35_grp25_08[] = {
	CG_FRAME(    0,    0,    4,           0,   764, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   765, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   766, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   765, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_08 ),
};

/* group 25 9 */
const Sint32 pl35_grp25_09[] = {
	CG_FRAME(    0,    0,    4,           0,   746, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   747, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   746, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   747, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   748, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   749, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   750, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   751, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_09 ),
};

/* group 25 10: shl35_04_hk_end */
const Sint32 pl35_grp25_10[] = {
	CG_FRAME(    0,    0,    4,           0,   776, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   777, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   778, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   779, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   780, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   781, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_10 ),
};

/* group 25 11 */
const Sint32 pl35_grp25_11[] = {
	CG_FRAME(    0,    0,    4,           0,   658, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   659, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   660, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   661, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   662, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   663, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   664, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   665, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl35_grp25_11 ),
};

/* group 25 12 */
const Sint32 pl35_grp25_12[] = {
	CG_FRAME(    0,    0,    4,           0, 32781, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0, 32782, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0, 32783, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0, 32785, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0, 32786, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4,           0, 32787, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32788, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl35_grp25_12 ),
};

/* group 25 13: shl35_04_chk_init */
const Sint32 pl35_grp25_13[] = {
	CG_FRAME(    0,    0,    4,           0,   790, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   791, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   792, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_13_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   786, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0,   787, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0,   788, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   789, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl35_grp25_13_loop ),
};

/* group 25 14 */
const Sint32 pl35_grp25_14[] = {
	CG_FRAME(    0,    0,    4,           0,   793, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   794, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   793, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   795, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_14 ),
};

/* group 25 15 */
const Sint32 pl35_grp25_15[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   797, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   798, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   797, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_15 ),
};

/* group 25 16 */
const Sint32 pl35_grp25_16[] = {
	CG_FRAME(    0,    0,    4,           0,   799, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   800, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   801, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   800, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_16 ),
};

/* group 25 17: shl35_04_chk_end */
const Sint32 pl35_grp25_17[] = {
	CG_FRAME(    0,    0,    4,           0,   792, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   791, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   790, 0xb0,   0x00,    0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_17 ),
};

/* group 25 18 */
const Sint32 pl35_grp25_18[] = {
	CG_FRAME(    0,    0,    4,           0, 32781, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   802, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   803, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   806, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   804, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   805, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl35_grp25_18 ),
};

/* group 25 19: shl35_04_jhp_init */
const Sint32 pl35_grp25_19[] = {
	CG_FRAME(    0,    0,    2,           0, 32789, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32790, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32791, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32792, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_19_loop[] = {
	CG_FRAME(    1,    0,    2,           0, 32781, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    2,    0,    2,           0, 32782, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    3,    0,    2,           0, 32783, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    4,    0,    2,           0, 32784, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    5,    0,    2,           0, 32785, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    6,    0,    2,           0, 32786, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    7,    0,    2,           0, 32787, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    8,    0,    2, END_OF_DATA, 32788, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl35_grp25_19_loop ),
};

/* group 25 20: shl35_04_jhp_fly_back */
const Sint32 pl35_grp25_20[] = {
	CG_FRAME(    9,    0,    4,           0,   804, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(   10,    0,    4,           0,   805, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl35_grp25_20_loop[] = {
	CG_FRAME(   11,    0,    2,           0, 32781, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   12,    0,    2,           0, 32782, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   13,    0,    2,           0, 32783, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   14,    0,    2,           0, 32784, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   15,    0,    2,           0, 32785, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   16,    0,    2,           0, 32786, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   17,    0,    2,           0, 32787, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(   20,    0,    2, END_OF_DATA, 32788, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl35_grp25_20_loop ),
};

/* group 25 21: shl35_04_jjudgement_init */
const Sint32 pl35_grp25_21[] = {
	CG_FRAME(    0,    0,    4,           0, 32826, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32827, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32828, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_21_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32820, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0, 32821, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0, 32822, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0, 32823, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4,           0, 32824, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32825, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl35_grp25_21_loop ),
};

/* group 25 22 */
const Sint32 pl35_grp25_22[] = {
	CG_FRAME(    0,    0,    4,           0,   880, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   881, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   882, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl35_grp25_22_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   883, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   884, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   885, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   886, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4,           0,   887, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   888, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl35_grp25_22_loop ),
};

/* group 25 23: shl35_04_hk_end */
const Sint32 pl35_grp25_23[] = {
	CG_FRAME(    0,    0,    4,           0, 32827, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32826, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_23 ),
};

/* group 25 24 */
const Sint32 pl35_grp25_24[] = {
	CG_FRAME(    0,    0,    4,           0,   793, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   794, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   793, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   795, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_24 ),
};

/* group 25 25 */
const Sint32 pl35_grp25_25[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   797, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   798, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   797, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_25 ),
};

/* group 25 26 */
const Sint32 pl35_grp25_26[] = {
	CG_FRAME(    0,    0,    4,           0,   799, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   800, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   801, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   800, 0xb0,   0x00,  ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl35_grp25_26 ),
};
