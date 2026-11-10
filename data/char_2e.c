/* War Machine (character 2e): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_2e      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_2e     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl2e.c) says which group
                   and number each move uses.
   pl2e_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl2e_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl2e_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl2e_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_2e
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_2e, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_2e */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl2e_stand_00[], pl2e_stand_02[], pl2e_stand_03[], pl2e_dash_00[],
	pl2e_dash_00_loop[], pl2e_dash_02[], pl2e_dash_01[], pl2e_dash_01_loop[], pl2e_dash_03[],
	pl2e_jump_00[], pl2e_jump_00_loop[], pl2e_jump_06[], pl2e_jump_06_loop[], pl2e_jump_01[],
	pl2e_jump_01_loop[], pl2e_jump_07[], pl2e_jump_07_loop[], pl2e_jump_02[], pl2e_jump_02_loop[],
	pl2e_jump_08[], pl2e_jump_08_loop[], pl2e_jump_03[], pl2e_jump_09[], pl2e_crouch_00[],
	pl2e_crouch_01[], pl2e_crouch_02[], pl2e_turn_00[], pl2e_turn_01[], pl2e_guard_00[],
	pl2e_guard_01[], pl2e_guard_02[], pl2e_guard_04[], pl2e_guard_05[], pl2e_guard_06[],
	pl2e_air_guard_00[], pl2e_air_guard_01[], pl2e_win_00[], pl2e_win_00_loop[], pl2e_win_01[],
	pl2e_win_01_loop[], pl2e_win_02[], pl2e_win_02_loop[], pl2e_change_00[], pl2e_effect_52[],
	pl2e_change_01[], pl2e_change_01_loop[], pl2e_change_02[], pl2e_change_03[], pl2e_appear_00[],
	pl2e_appear_01[], pl2e_appear_02[], pl2e_appear_03[], pl2e_appear_04[], pl2e_appear_05[],
	pl2e_appear_05_loop[], pl2e_appear_06[], pl2e_appear_07[], pl2e_appear_08[], pl2e_appear_09[],
	pl2e_get_up_00[], pl2e_get_up_01[], pl2e_get_up_02[], pl2e_get_up_03[], pl2e_stand_06[],
	pl2e_stand_07[], pl2e_stand_p_00[], pl2e_extra_07[], pl2e_stand_p_01[], pl2e_stand_p_02[],
	pl2e_stand_p_02_loop[], pl2e_extra_00[], pl2e_stand_k_00[], pl2e_stand_k_01[], pl2e_stand_k_02[],
	pl2e_extra_06[], pl2e_crouch_p_00[], pl2e_crouch_p_01[], pl2e_crouch_p_02[], pl2e_unused_00[],
	pl2e_crouch_k_00[], pl2e_extra_08[], pl2e_crouch_k_01[], pl2e_crouch_k_02[], pl2e_jump_p_00[],
	pl2e_jump_p_01[], pl2e_jump_p_02[], pl2e_jump_p_06[], pl2e_jump_p_07[], pl2e_jump_k_00[],
	pl2e_jump_k_01[], pl2e_extra_04[], pl2e_extra_04_loop[], pl2e_extra_05[], pl2e_extra_05_loop[],
	pl2e_jump_k_06[], pl2e_jump_k_02[], pl2e_extra_01[], pl2e_extra_02[], pl2e_special_16[],
	pl2e_special_17[], pl2e_special_00[], pl2e_special_00_loop[], pl2e_special_01[],
	pl2e_special_01_loop[], pl2e_special_02[], pl2e_special_03[], pl2e_special_04[],
	pl2e_special_04_loop[], pl2e_special_05[], pl2e_special_05_loop[], pl2e_special_06[],
	pl2e_special_08[], pl2e_special_08_loop[], pl2e_special_09[], pl2e_special_09_loop[],
	pl2e_special_10[], pl2e_special_12[], pl2e_special_13[], pl2e_special_14[], pl2e_special_15[],
	pl2e_catch_00[], pl2e_catch_00_loop[], pl2e_catch_01[], pl2e_catch_01_loop[], pl2e_catch_02[],
	pl2e_catch_03[], pl2e_catch_04[], pl2e_catch_04_loop[], pl2e_effect_30[], pl2e_effect_30_loop[],
	pl2e_effect_31[], pl2e_super_00[], pl2e_super_00_loop[], pl2e_super_01[], pl2e_super_01_loop[],
	pl2e_super_02[], pl2e_super_03[], pl2e_super_03_loop[], pl2e_super_04[], pl2e_effect_32[],
	pl2e_effect_32_loop[], pl2e_effect_33[], pl2e_effect_34[], pl2e_effect_34_loop[],
	pl2e_effect_35[], pl2e_effect_36[], pl2e_effect_36_loop[], pl2e_effect_37[],
	pl2e_effect_37_loop[], pl2e_damage_00[], pl2e_damage_01[], pl2e_damage_02[], pl2e_damage_03[],
	pl2e_damage_04[], pl2e_damage_05[], pl2e_damage_06[], pl2e_damage_07[], pl2e_damage_08[],
	pl2e_damage_10[], pl2e_damage_11[], pl2e_damage_12[], pl2e_damage_13[], pl2e_damage_09[],
	pl2e_damage_09_loop[], pl2e_damage_26[], pl2e_damage_27[], pl2e_damage_30[], pl2e_damage_33[],
	pl2e_damage_16[], pl2e_damage_17[], pl2e_damage_18[], pl2e_damage_19[], pl2e_damage_20[],
	pl2e_damage_21[], pl2e_damage_22[], pl2e_damage_23[], pl2e_damage_24[], pl2e_damage_25[],
	pl2e_damage_28[], pl2e_damage_29[], pl2e_dizzy_00[], pl2e_damage_31[], pl2e_caught_00[],
	pl2e_caught_01[], pl2e_caught_02[], pl2e_caught_03[], pl2e_caught_04[], pl2e_caught_05[],
	pl2e_caught_06[], pl2e_caught_07[], pl2e_caught_08[], pl2e_caught_09[], pl2e_caught_10[],
	pl2e_caught_11[], pl2e_caught_12[], pl2e_caught_13[], pl2e_unused_01[], pl2e_caught_14[],
	pl2e_caught_15[], pl2e_caught_16[], pl2e_caught_17[], pl2e_caught_18[], pl2e_caught_19[],
	pl2e_caught_20[], pl2e_caught_21[], pl2e_effect_41[], pl2e_effect_41_loop[], pl2e_effect_42[],
	pl2e_effect_44[], pl2e_effect_40[], pl2e_effect_39[], pl2e_effect_43[], pl2e_effect_07[],
	pl2e_effect_07_loop[], pl2e_effect_38[], pl2e_effect_45[], pl2e_effect_46[], pl2e_effect_47[],
	pl2e_unused_02[], pl2e_unused_03[], pl2e_unused_04[], pl2e_unused_05[], pl2e_unused_06[],
	pl2e_effect_48[], pl2e_appear_10[], pl2e_effect_00[], pl2e_effect_01[], pl2e_effect_02[],
	pl2e_effect_03[], pl2e_effect_04[], pl2e_effect_05[], pl2e_effect_06[], pl2e_effect_12[],
	pl2e_effect_13[], pl2e_effect_14[], pl2e_effect_10[], pl2e_effect_11[], pl2e_effect_08[],
	pl2e_effect_09[], pl2e_effect_19[], pl2e_effect_19_loop[], pl2e_effect_20[], pl2e_effect_21[],
	pl2e_effect_22[], pl2e_effect_23[], pl2e_effect_24[], pl2e_effect_25[], pl2e_effect_26[],
	pl2e_effect_27[], pl2e_effect_28[], pl2e_effect_29[], pl2e_fly_00[], pl2e_fly_00_loop[],
	pl2e_fly_01[], pl2e_effect_15[], pl2e_fly_02[], pl2e_effect_16[], pl2e_fly_08[],
	pl2e_effect_17[], pl2e_fly_03[], pl2e_effect_18[], pl2e_effect_49[], pl2e_effect_50[],
	pl2e_effect_51[], pl2e_extra_03[], pl2e_change_04[], pl2e_special_18[], pl2e_special_18_loop[],
	pl2e_special_19[];
extern const Sint32 *const pl2e_stand_tbl[], *const pl2e_jump_tbl[], *const pl2e_dash_tbl[],
	*const pl2e_crouch_tbl[], *const pl2e_turn_tbl[], *const pl2e_guard_tbl[],
	*const pl2e_air_guard_tbl[], *const pl2e_stand_p_tbl[], *const pl2e_stand_k_tbl[],
	*const pl2e_crouch_p_tbl[], *const pl2e_crouch_k_tbl[], *const pl2e_jump_p_tbl[],
	*const pl2e_jump_k_tbl[], *const pl2e_damage_tbl[], *const pl2e_dizzy_tbl[],
	*const pl2e_catch_tbl[], *const pl2e_caught_tbl[], *const pl2e_get_up_tbl[],
	*const pl2e_appear_tbl[], *const pl2e_win_tbl[], *const pl2e_extra_tbl[],
	*const pl2e_special_tbl[], *const pl2e_super_tbl[], *const pl2e_effect_tbl[],
	*const pl2e_change_tbl[], *const pl2e_grp25_tbl[], *const pl2e_fly_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_2e[16] = {
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
const Sint32 *const *const char_tbl_2e[] = {
	pl2e_stand_tbl, pl2e_jump_tbl, pl2e_dash_tbl, pl2e_crouch_tbl, pl2e_turn_tbl, pl2e_guard_tbl,
	pl2e_air_guard_tbl, pl2e_stand_p_tbl, pl2e_stand_k_tbl, pl2e_crouch_p_tbl, pl2e_crouch_k_tbl, pl2e_jump_p_tbl,
	pl2e_jump_k_tbl, pl2e_damage_tbl, pl2e_dizzy_tbl, pl2e_catch_tbl, pl2e_caught_tbl, pl2e_get_up_tbl,
	pl2e_appear_tbl, pl2e_win_tbl, pl2e_extra_tbl, pl2e_special_tbl, pl2e_super_tbl, pl2e_effect_tbl,
	pl2e_change_tbl, pl2e_grp25_tbl, pl2e_fly_tbl, 0,
};

const Sint32 *const pl2e_stand_tbl[] = {
	pl2e_stand_00, pl2e_stand_00, pl2e_stand_02, pl2e_stand_03, pl2e_stand_00, pl2e_stand_00,
	pl2e_stand_06, pl2e_stand_07, pl2e_stand_00, pl2e_stand_00, 0,
};

const Sint32 *const pl2e_jump_tbl[] = {
	pl2e_jump_00, pl2e_jump_01, pl2e_jump_02, pl2e_jump_03, pl2e_jump_03, pl2e_jump_03,
	pl2e_jump_06, pl2e_jump_07, pl2e_jump_08, pl2e_jump_09, pl2e_jump_09, pl2e_jump_09,
	0,
};

const Sint32 *const pl2e_dash_tbl[] = {
	pl2e_dash_00, pl2e_dash_01, pl2e_dash_02, pl2e_dash_03, 0,
};

const Sint32 *const pl2e_crouch_tbl[] = {
	pl2e_crouch_00, pl2e_crouch_01, pl2e_crouch_02, 0,
};

const Sint32 *const pl2e_turn_tbl[] = {
	pl2e_turn_00, pl2e_turn_01, 0,
};

const Sint32 *const pl2e_guard_tbl[] = {
	pl2e_guard_00, pl2e_guard_01, pl2e_guard_02, pl2e_guard_01, pl2e_guard_04, pl2e_guard_05,
	pl2e_guard_06, pl2e_guard_05, 0,
};

const Sint32 *const pl2e_air_guard_tbl[] = {
	pl2e_air_guard_00, pl2e_air_guard_01, pl2e_air_guard_01, 0,
};

const Sint32 *const pl2e_stand_p_tbl[] = {
	pl2e_stand_p_00, pl2e_stand_p_01, pl2e_stand_p_02, 0,
};

const Sint32 *const pl2e_stand_k_tbl[] = {
	pl2e_stand_k_00, pl2e_stand_k_01, pl2e_stand_k_02, 0,
};

const Sint32 *const pl2e_crouch_p_tbl[] = {
	pl2e_crouch_p_00, pl2e_crouch_p_01, pl2e_crouch_p_02, 0,
};

const Sint32 *const pl2e_crouch_k_tbl[] = {
	pl2e_crouch_k_00, pl2e_crouch_k_01, pl2e_crouch_k_02, 0,
};

const Sint32 *const pl2e_jump_p_tbl[] = {
	pl2e_jump_p_00, pl2e_jump_p_01, pl2e_jump_p_02, pl2e_jump_p_00, pl2e_jump_p_01, pl2e_jump_p_02,
	pl2e_jump_p_06, pl2e_jump_p_07, 0,
};

const Sint32 *const pl2e_jump_k_tbl[] = {
	pl2e_jump_k_00, pl2e_jump_k_01, pl2e_jump_k_02, pl2e_jump_k_00, pl2e_jump_k_01, pl2e_jump_k_02,
	pl2e_jump_k_06, 0,
};

const Sint32 *const pl2e_damage_tbl[] = {
	pl2e_damage_00, pl2e_damage_01, pl2e_damage_02, pl2e_damage_03, pl2e_damage_04, pl2e_damage_05,
	pl2e_damage_06, pl2e_damage_07, pl2e_damage_08, pl2e_damage_09, pl2e_damage_10, pl2e_damage_11,
	pl2e_damage_12, pl2e_damage_13, pl2e_damage_13, pl2e_damage_13, pl2e_damage_16, pl2e_damage_17,
	pl2e_damage_18, pl2e_damage_19, pl2e_damage_20, pl2e_damage_21, pl2e_damage_22, pl2e_damage_23,
	pl2e_damage_24, pl2e_damage_25, pl2e_damage_26, pl2e_damage_27, pl2e_damage_28, pl2e_damage_29,
	pl2e_damage_30, pl2e_damage_31, pl2e_damage_30, pl2e_damage_33, pl2e_damage_13, 0,
};

const Sint32 *const pl2e_dizzy_tbl[] = {
	pl2e_dizzy_00, 0,
};

const Sint32 *const pl2e_catch_tbl[] = {
	pl2e_catch_00, pl2e_catch_01, pl2e_catch_02, pl2e_catch_03, pl2e_catch_04, 0,
};

const Sint32 *const pl2e_caught_tbl[] = {
	pl2e_caught_00, pl2e_caught_01, pl2e_caught_02, pl2e_caught_03, pl2e_caught_04, pl2e_caught_05,
	pl2e_caught_06, pl2e_caught_07, pl2e_caught_08, pl2e_caught_09, pl2e_caught_10, pl2e_caught_11,
	pl2e_caught_12, pl2e_caught_13, pl2e_caught_14, pl2e_caught_15, pl2e_caught_16, pl2e_caught_17,
	pl2e_caught_18, pl2e_caught_19, pl2e_caught_20, pl2e_caught_21, 0,
};

const Sint32 *const pl2e_get_up_tbl[] = {
	pl2e_get_up_00, pl2e_get_up_01, pl2e_get_up_02, pl2e_get_up_03, 0,
};

const Sint32 *const pl2e_appear_tbl[] = {
	pl2e_appear_00, pl2e_appear_01, pl2e_appear_02, pl2e_appear_03, pl2e_appear_04, pl2e_appear_05,
	pl2e_appear_06, pl2e_appear_07, pl2e_appear_08, pl2e_appear_09, pl2e_appear_10, 0,
};

const Sint32 *const pl2e_win_tbl[] = {
	pl2e_win_00, pl2e_win_01, pl2e_win_02, 0,
};

const Sint32 *const pl2e_extra_tbl[] = {
	pl2e_extra_00, pl2e_extra_01, pl2e_extra_02, pl2e_extra_03, pl2e_extra_04, pl2e_extra_05,
	pl2e_extra_06, pl2e_extra_07, pl2e_extra_08, 0,
};

const Sint32 *const pl2e_special_tbl[] = {
	pl2e_special_00, pl2e_special_01, pl2e_special_02, pl2e_special_03, pl2e_special_04, pl2e_special_05,
	pl2e_special_06, pl2e_special_06, pl2e_special_08, pl2e_special_09, pl2e_special_10, pl2e_special_10,
	pl2e_special_12, pl2e_special_13, pl2e_special_14, pl2e_special_15, pl2e_special_16, pl2e_special_17,
	pl2e_special_18, pl2e_special_19, 0,
};

const Sint32 *const pl2e_super_tbl[] = {
	pl2e_super_00, pl2e_super_01, pl2e_super_02, pl2e_super_03, pl2e_super_04, 0,
};

const Sint32 *const pl2e_effect_tbl[] = {
	pl2e_effect_00, pl2e_effect_01, pl2e_effect_02, pl2e_effect_03, pl2e_effect_04, pl2e_effect_05,
	pl2e_effect_06, pl2e_effect_07, pl2e_effect_08, pl2e_effect_09, pl2e_effect_10, pl2e_effect_11,
	pl2e_effect_12, pl2e_effect_13, pl2e_effect_14, pl2e_effect_15, pl2e_effect_16, pl2e_effect_17,
	pl2e_effect_18, pl2e_effect_19, pl2e_effect_20, pl2e_effect_21, pl2e_effect_22, pl2e_effect_23,
	pl2e_effect_24, pl2e_effect_25, pl2e_effect_26, pl2e_effect_27, pl2e_effect_28, pl2e_effect_29,
	pl2e_effect_30, pl2e_effect_31, pl2e_effect_32, pl2e_effect_33, pl2e_effect_34, pl2e_effect_35,
	pl2e_effect_36, pl2e_effect_37, pl2e_effect_38, pl2e_effect_39, pl2e_effect_40, pl2e_effect_41,
	pl2e_effect_42, pl2e_effect_43, pl2e_effect_44, pl2e_effect_45, pl2e_effect_46, pl2e_effect_47,
	pl2e_effect_48, pl2e_effect_49, pl2e_effect_50, pl2e_effect_51, pl2e_effect_52, 0,
};

const Sint32 *const pl2e_change_tbl[] = {
	pl2e_change_00, pl2e_change_01, pl2e_change_02, pl2e_change_03, pl2e_change_04, 0,
};

const Sint32 *const pl2e_grp25_tbl[] = {
	pl2e_stand_00, 0,
};

const Sint32 *const pl2e_fly_tbl[] = {
	pl2e_fly_00, pl2e_fly_01, pl2e_fly_02, pl2e_fly_03, pl2e_fly_03, pl2e_fly_01,
	pl2e_fly_01, pl2e_fly_01, pl2e_fly_08, pl2e_fly_08, pl2e_fly_01, pl2e_fly_01,
	pl2e_fly_01, pl2e_fly_01, pl2e_fly_01, pl2e_fly_01, pl2e_fly_01, pl2e_fly_01,
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow        atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 8
   Char_Stand 9 */
const Sint32 pl2e_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    95, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    94, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    93, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    92, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl2e_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   101, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   102, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   103, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   104, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   105, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   106, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   107, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   108, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   109, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl2e_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,   121, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   120, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   119, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   118, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   117, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   116, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   115, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   114, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   113, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   112, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   111, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   110, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_stand_03 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl2e_dash_00[] = {
	CG_FRAME(    0,    0,   1,           0,   122, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   123, 0x80,   0x00,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   125, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl2e_dash_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   126, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   127, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   128, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   129, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   130, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   131, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   132, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   133, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2e_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl2e_dash_02[] = {
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2e_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl2e_dash_01[] = {
	CG_FRAME(    0,    0,   2,           0,   147, 0x80,   0x00,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   148, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   2,           0,   149, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl2e_dash_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   150, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   151, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   152, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   153, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   154, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   155, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1,           0,   156, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   157, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2e_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl2e_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   159, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2e_dash_03 ),
};

/* Char_Jump 0 */
const Sint32 pl2e_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   160, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   177, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   178, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   179, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   161, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   0,           0,   162, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   163, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  12,           0,   164, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   165, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2e_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   166, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   167, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_00_loop ),
};

/* Char_Jump 6 */
const Sint32 pl2e_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   160, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   169, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   173, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   174, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   175, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   178, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   1,           0,   179, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  12,           0,   164, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   165, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2e_jump_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   166, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   167, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_06_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl2e_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   180, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   195, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2e_jump_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   181, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   184, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   186, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   188, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_01_loop ),
};

/* Char_Jump 7 */
const Sint32 pl2e_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   180, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   193, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   195, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   181, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   182, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   184, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   185, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   186, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   187, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   188, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   180, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  10,           0,   190, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2e_jump_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   191, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   192, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_07_loop ),
};

/* Char_Jump 2 */
const Sint32 pl2e_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   196, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2e_jump_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   197, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   204, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_02_loop ),
};

/* Char_Jump 8 */
const Sint32 pl2e_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   196, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   199, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   200, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   201, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   202, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   203, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   204, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   196, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   189, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,  10,           0,   190, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2e_jump_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   191, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   192, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_08_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl2e_jump_03[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_jump_03 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl2e_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,   191, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   192, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_jump_09 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl2e_crouch_00[] = {
	CG_FRAME(    2,    0,   4,           0,   168, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,    0,   4,           0,   160, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   205, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_crouch_01 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl2e_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    97, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl2e_crouch_02[] = {
	CG_FRAME(    1,    0,   4,           0,   160, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    2,    0,   4,           0,   168, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl2e_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   207, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl2e_turn_01[] = {
	CG_FRAME(    0,    0,   8,           0,   208, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   209, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl2e_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl2e_guard_01[] = {
	CG_FRAME(    0,    0,   2,           0,   214, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   215, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   216, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   218, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl2e_guard_02[] = {
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   220, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl2e_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl2e_guard_05[] = {
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   228, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl2e_guard_06[] = {
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   230, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl2e_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   231, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   233, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl2e_air_guard_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   234, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   235, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   236, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   237, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   240, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_air_guard_01 ),
};

/* Char_Win 0: win_time */
const Sint32 pl2e_win_00[] = {
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0x80,   0x60,          2,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   247, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   248, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   249, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   250, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   251, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   252, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   253, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   254, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   255, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   256, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   257, 0x80,   0x60,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   2,           0,   258, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,   2,           0,   259, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   260, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_win_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   262, 0x80,   0x8e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl2e_win_01[] = {
	CG_FRAME(    0,    0,  12,           0,   414, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x80,   0x6e,          2,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   263, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   264, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  24,           0,   266, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_win_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   269, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   271, 0x80,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_win_01_loop ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl2e_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   168, 0x40,   0x6f,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x40,   0x6f,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   274, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   275, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   276, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   277, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_win_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   283, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   284, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_win_02_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl2e_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   397, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   398, 0x40,   0x00,          3,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   399, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0, 0x3200,   0 ),
	CG_FRAME(    0,    1,  10,           0,   399, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0, 0x3200,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   397, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_change_00 ),
};

/* Char_Effect 52 */
const Sint32 pl2e_effect_52[] = {
	CG_FRAME(    0,    0,   1,           0,  1094, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1095, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1096, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1097, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1098, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1099, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1100, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1101, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1102, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1103, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1104, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1105, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1106, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1107, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1108, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1109, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1110, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1111, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1112, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_52 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl2e_change_01[] = {
	CG_FRAME( 0x80,    0,   4,           0,   414, 0xa0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,   2,           0,   415, 0xa0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_change_01_loop[] = {
	CG_FRAME( 0x80,    0,   6, END_OF_DATA,   400, 0xa0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_change_01_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl2e_change_02[] = {
	CG_FRAME(    0,    0,   2,           0,   191, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   192, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl2e_change_03[] = {
	CG_FRAME(    0,    0,   2,           0,   405, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   406, 0x90,   0x00,          0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   407, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,  32,           0,   407, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   405, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_change_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl2e_appear_00[] = {
	CG_FRAME(    0,    0,   1,           0,   285, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   286, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   287, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   288, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   289, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   290, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   291, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   292, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_00 ),
};

/* Char_Appear 1: appear_land */
const Sint32 pl2e_appear_01[] = {
	CG_FRAME(    0,    0,   1,           0,   293, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   294, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   295, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   296, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   297, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   298, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   299, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   300, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_01 ),
};

/* Char_Appear 2: appear_rise */
const Sint32 pl2e_appear_02[] = {
	CG_FRAME(    0,    0,   1,           0,   301, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   302, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   303, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   304, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   305, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   306, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   307, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   308, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_02 ),
};

/* Char_Appear 3: appear_stand */
const Sint32 pl2e_appear_03[] = {
	CG_FRAME(    0,    0,   1,           0,   309, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   310, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   311, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   312, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   313, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   314, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   315, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   316, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_03 ),
};

/* Char_Appear 4: pls2e_00_appear_init */
const Sint32 pl2e_appear_04[] = {
	CG_FRAME(    0,    0,   1,           0,   318, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   319, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   320, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   321, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   322, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   323, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   324, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_04 ),
};

/* Char_Appear 5: pls2e_00_appear_wait */
const Sint32 pl2e_appear_05[] = {
	CG_FRAME(    0,    0,   1,           0,   325, 0x80,   0x00,          0,  0x00,       0x22,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_appear_05_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   326, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   327, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   328, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   329, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   330, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   332, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_05_loop ),
};

/* Char_Appear 6: pls2e_00_appear_start */
const Sint32 pl2e_appear_06[] = {
	CG_FRAME(    0,    0,   1,           0,   333, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   334, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   335, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   336, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   337, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   338, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   339, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   340, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_06 ),
};

/* Char_Appear 7: pls2e_00_appear_up */
const Sint32 pl2e_appear_07[] = {
	CG_FRAME(    0,    0,   1,           0,   341, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   342, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   343, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   344, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   345, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   346, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   348, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_07 ),
};

/* Char_Appear 8: pls2e_00_appear_up_next */
const Sint32 pl2e_appear_08[] = {
	CG_FRAME(    0,    0,   1,           0,   349, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   350, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   351, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   352, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   353, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   354, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   355, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   356, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_08 ),
};

/* Char_Appear 9: appear_hop */
const Sint32 pl2e_appear_09[] = {
	CG_FRAME(    0,    0,  16,           0,   164, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16, END_OF_DATA,   165, 0x80,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_09 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl2e_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   357, 0x40,   0xc9,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   358, 0x40,   0xaa,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   359, 0x40,   0x8b,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   360, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   361, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   362, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   363, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   364, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   160, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl2e_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   365, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x40,   0x09,       0x1c,  0x00,       0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x40,   0xaa,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   361, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   362, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   363, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   364, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   160, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   168, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl2e_get_up_02[] = {
	CG_FRAME(    0,    0,   2,           0,   365, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x40,   0x09,       0x1c,  0x00,       0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   366, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   358, 0x40,   0xaa,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   359, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   360, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   361, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   362, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   363, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   364, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1,           0,   160, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   168, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl2e_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   369, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x40,   0x09,       0x0e,  0x00,       0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   371, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x40,   0x09,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x40,   0xaa,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   361, 0x40,   0x2e,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   364, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   160, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   168, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_get_up_03 ),
};

/* Char_Stand 6 */
const Sint32 pl2e_stand_06[] = {
	CG_FRAME(    0,    0,   4,           0,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   408, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl2e_stand_07[] = {
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   408, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    91, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_stand_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl2e_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   408, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   409, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    1,   5,           0,   409, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   408, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_stand_p_00 ),
};

/* Char_Extra 7 */
const Sint32 pl2e_extra_07[] = {
	CG_FRAME(    0,    0,   3,           0,   410, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   3,           0,   412, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   413, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_extra_07 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl2e_stand_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   414, 0xa0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   415, 0xa0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   416, 0xa0,   0x0e, ATK | 0x11,  0x00,       0x03,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0xa0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   4,           0,   417, 0xa0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6,           0,   418, 0xa0,   0x0e,        ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   413, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl2e_stand_p_02[] = {
	CG_FRAME(    0,    0,   5,           0,   419, 0xa0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   420, 0xa0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   421, 0xa0,   0x8c,        ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   5,           0,   422, 0xa0,   0x8c, ATK | 0x12,  0x00,       0x04,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   2,           0,   423, 0xa0,   0x6f, ATK | 0x17,  0x00,       0x1e,    0,   0,     0,      0,  14 ),
};

const Sint32 pl2e_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   424, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   423, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl2e_stand_p_02_loop ),
};

/* Char_Extra 0: stand_hp_shot */
const Sint32 pl2e_extra_00[] = {
	CG_FRAME(    0,    0,   4,           0,   425, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   426, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl2e_extra_00 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl2e_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   558, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   559, 0xa0,   0x6c,        ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   6,           0,   560, 0xa0,   0x6c,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   561, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   562, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl2e_stand_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   427, 0xa0,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0xa0,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    1,   6,           0,   429, 0xa0,   0x40, ATK | 0x18,  0x00,       0x20,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   6,           0,   429, 0xa0,   0x40,        ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   5,           0,   558, 0xa0,   0x40,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   562, 0xa0,   0x40,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl2e_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   430, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0xa0,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   575, 0xa0,   0x20, ATK | 0x13,  0x01,       0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   6,           0,   444, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   8,           0,   451, 0xa0,   0x20, ATK | 0x18,  0x00,       0x20,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   434, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   437, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   438, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   443, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_stand_k_02 ),
};

/* Char_Extra 6: pl2e_upper */
const Sint32 pl2e_extra_06[] = {
	CG_FRAME(    0,    0,   2,           0,   430, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   427, 0xa0,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   575, 0xa0,   0x20, ATK | 0x13,  0x01,       0x05,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   6,           0,   444, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   8,           0,   451, 0xa0,   0x20, ATK | 0x18,  0x00,       0x20,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   434, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   436, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   437, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   438, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   443, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_extra_06 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl2e_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   464, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   465, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   5,           0,   466, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   467, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2e_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl2e_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   468, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   471, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   469, 0xa0,   0x6e, ATK | 0x11,  0x00,       0x03,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   9,           0,   470, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   471, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2e_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl2e_crouch_p_02[] = {
	CG_FRAME(    0,   10,   5,           0,   160, 0x80,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   5,           0,   205, 0xa0,   0x6e, ATK | 0x0b,  0x00,       0x03,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   3,           0,   473, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   474, 0xa0,   0x6e, ATK | 0x14,  0x00,       0x23,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,  14,           0,   482, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2e_crouch_p_02 ),
};

const Sint32 pl2e_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   205, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   473, 0xa0,   0x6e, ATK | 0x15,  0x00,       0x24,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   482, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0,   474, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   477, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,   2,           0,   478, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   479, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   480, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   481, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,  24,           0,   482, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl2e_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl2e_crouch_k_00[] = {
	CG_FRAME(    0,    0,   6,           0,   487, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   488, 0xa0,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0xa0,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   7,           0,   489, 0xa0,   0xa8,        ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    1,   3,           0,   490, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   487, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2e_crouch_k_00 ),
};

/* Char_Extra 8 */
const Sint32 pl2e_extra_08[] = {
	CG_FRAME(    0,    0,   2,           0,   491, 0xa0,   0xa8,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   3,           0,   490, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   487, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_extra_08 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl2e_crouch_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   208, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   205, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   492, 0xa0,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   493, 0xa0,   0x40,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,   2,           0,   494, 0xa0,   0xaa, ATK | 0x11,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   495, 0xa0,   0xaa,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,  22,           0,   494, 0xa0,   0xaa,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   208, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2e_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl2e_crouch_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   208, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   205, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   496, 0xa0,   0x4e,        ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   3,           0,   497, 0xa0,   0x68, ATK | 0x12,  0x00,       0x04,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,   8,           0,   498, 0xa0,   0x68, ATK | 0x18,  0x00,       0x20,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,  20,           0,   500, 0xa0,   0x68,        ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   501, 0xa0,   0x68,        ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl2e_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl2e_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   642, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   1,           0,   643, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   644, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   648, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4,           0,   645, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   649, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4,           0,   646, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   650, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   4,           0,   647, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   641, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0,   640, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   180, 0x90,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl2e_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   510, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    1,   3,           0,   511, 0xa0,   0x20, ATK | 0x19,  0x01,       0x20,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   511, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   511, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   511, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   511, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3,           0,   512, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   511, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   512, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl2e_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl2e_jump_p_02[] = {
	CG_FRAME(    0,    0,   5,           0,   513, 0xa0,   0x20,        ATK,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   514, 0xa0,   0x20,        ATK,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   521, 0xa0,   0x20, ATK | 0x12,  0x01,       0x04,    0,   0,     0,      0, 113 ),
	CG_FRAME( 0x80,    0,   1,           0,   522, 0xa0,   0x20, ATK | 0x1b,  0x01,       0x1f,    0,   0,     0,      0, 113 ),
	CG_FRAME( 0x80,    0,   1,           0,   523, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME( 0x80,    0,   1,           0,   524, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   525, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME( 0x80,    0,   1,           0,   526, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   527, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0,   528, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2,           0,   529, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   530, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   531, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   6,           0,   515, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   520, 0xa0,   0x20,        ATE,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0xa0,   0x20,        ATE,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_p_02 ),
};

/* Char_Jump_P 6 */
const Sint32 pl2e_jump_p_06[] = {
	CG_FRAME(    0,    0,   4,           0,   513, 0xa0,   0x20,        ATK,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0xa0,   0x20,        ATK,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   532, 0xa0,   0x20, ATK | 0x12,  0x01,       0x04,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   533, 0xa0,   0x20, ATK | 0x1b,  0x01,       0x1f,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   534, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   535, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,   1,           0,   536, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,   1,           0,   537, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   1,           0,   538, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   1,           0,   539, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   540, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   541, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   542, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   8,           0,   517, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   520, 0xa0,   0x20,        ATE,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0xa0,   0x20,        ATE,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_p_06 ),
};

/* Char_Jump_P 7 */
const Sint32 pl2e_jump_p_07[] = {
	CG_FRAME(    0,    0,   6,           0,   513, 0xa0,   0x20,        ATK,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   514, 0xa0,   0x20,        ATK,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   3,           0,   543, 0xa0,   0x20, ATK | 0x12,  0x01,       0x04,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   544, 0xa0,   0x20, ATK | 0x1b,  0x01,       0x1f,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   545, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   546, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME( 0x80,    0,   1,           0,   547, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   1,           0,   548, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   1,           0,   549, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   550, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   551, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   552, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   553, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   518, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   519, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   520, 0xa0,   0x20,        ATE,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   516, 0xa0,   0x20,        ATE,  0x03,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_p_07 ),
};

/* Char_Jump_K 0 (light): jump_kick
   Char_Jump_K 3 */
const Sint32 pl2e_jump_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   554, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   555, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6,           0,   556, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   5,           0,   557, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   554, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_k_00 ),
};

/* Char_Jump_K 1 (medium): jump_kick
   Char_Jump_K 4 */
const Sint32 pl2e_jump_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   713, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0xa0,   0x20, ATK | 0x11,  0x01,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   715, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   5,           0,   716, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   716, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   714, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   713, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_k_01 ),
};

/* Char_Extra 4: jump_kick */
const Sint32 pl2e_extra_04[] = {
	CG_FRAME(    0,    1,   4,           0,   563, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   4,           0,   563, 0xa0,   0x20, ATK | 0x11,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,   2,           0,   564, 0xa0,   0x20, ATK | 0x0e,  0x00,       0x22,    0,   0,     0,      0,  53 ),
};

const Sint32 pl2e_extra_04_loop[] = {
	CG_FRAME( 0x80,    0,   1,           0,   564, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1,           0,   565, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1,           0,   566, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1,           0,   567, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1,           0,   568, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   1, END_OF_DATA,   569, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl2e_extra_04_loop ),
};

/* Char_Extra 5: dive_move */
const Sint32 pl2e_extra_05[] = {
	CG_FRAME(    0,    0,  13,           0,   573, 0xa0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   574, 0xa0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl2e_extra_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   166, 0xa0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_extra_05_loop ),
};

/* Char_Jump_K 6: jump_kick */
const Sint32 pl2e_jump_k_06[] = {
	CG_FRAME(    0,    0,   2,           0,   578, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   576, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   575, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   580, 0xa0,   0x20, ATK | 0x13,  0x01,       0x05,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   2,           0,   581, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   6,           0,   580, 0xa0,   0x74,        ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   8,           0,   580, 0xa0,   0x74,        ATE,  0x01,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   575, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   576, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   577, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_k_06 ),
};

/* Char_Jump_K 2 (heavy): jump_kick
   Char_Jump_K 5 */
const Sint32 pl2e_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   575, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   576, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   577, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   578, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   579, 0xa0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   582, 0xa0,   0x20, ATK | 0x13,  0x01,       0x05,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   7,           0,   582, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   577, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   164, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   163, 0xa0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_jump_k_02 ),
};

/* Char_Extra 1: t_kick_init */
const Sint32 pl2e_extra_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   556, 0xa0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl2e_extra_01 ),
};

/* Char_Extra 2: t_kick_move, fly_start5 */
const Sint32 pl2e_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   400, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   401, 0xa0,   0x00,    ATE | 4,  0x00,       0x0a,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   402, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   404, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   404, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   404, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   400, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_extra_02 ),
};

/* Char_Special 16: bomb_init (Smart Bomb) */
const Sint32 pl2e_special_16[] = {
	CG_FRAME(    0,    0,   3,           0,   610, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   6,           0,   592, 0xb0,   0x6f, ATK | 0x0d,  0x00,       0x16,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   2,           0,   593, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   597, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   599, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   601, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   2,           0,   603, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    1,   2,           0,   604, 0xb0,   0x6f,    ATK | 9,  0x00, 0x7f | CSE,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   609, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,  10,           0,   600, 0xb0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   598, 0xb0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8,           0,   596, 0xb0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   594, 0xb0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2e_special_16 ),
};

/* Char_Special 17: bomb_air_init (Smart Bomb) */
const Sint32 pl2e_special_17[] = {
	CG_FRAME(    0,    0,   3,           0,   605, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   606, 0xb0,   0x20, ATK | 0x0d,  0x00,       0x16,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    2,   2,           0,   593, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   595, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   597, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   599, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   601, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   2,           0,   603, 0xb0,   0x20,        ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    1,   2,           0,   604, 0xb0,   0x20,    ATK | 9,  0x00, 0x7f | CSE,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   607, 0xb0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   609, 0xb0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8,           0,   597, 0xb0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8,           0,   595, 0xb0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   8,           0,   593, 0xb0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   594, 0xb0,   0x20,        ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl2e_special_17 ),
};

/* Char_Special 0 */
const Sint32 pl2e_special_00[] = {
	CG_FRAME(    0,   19,   3,           0,   610, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0xb0,   0x6e, ATK | 0x0b,  0x00,       0x14,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2,           0,   612, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2,           0,   613, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   5,           0,   614, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   2,           0,   615, 0xb0,   0x6e, ATK | 0x14,  0x00,       0x24,    0,   0,     0,      0,  84 ),
};

const Sint32 pl2e_special_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   616, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   617, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl2e_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl2e_special_01[] = {
	CG_FRAME(    0,   32,  10,           0,   610, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0xb0,   0x6e, ATK | 0x0b,  0x00,       0x14,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2,           0,   612, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2,           0,   613, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   5,           0,   614, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    1,   2,           0,   615, 0xb0,   0x6e, ATK | 0x14,  0x00,       0x24,    0,   0,     0,      0,  84 ),
};

const Sint32 pl2e_special_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   616, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   617, 0xb0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl2e_special_01_loop ),
};

/* Char_Special 2: cannon_shot (Shoulder Cannon, 2 + atck_level) */
const Sint32 pl2e_special_02[] = {
	CG_FRAME(    0,    0,  14,           0,   617, 0xb0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   618, 0xb0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl2e_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl2e_special_03[] = {
	CG_FRAME(    0,    0,  18,           0,   617, 0xb0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   618, 0xb0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl2e_special_03 ),
};

/* Char_Special 4: cannon_air_init (Shoulder Cannon, 4 + atck_level) */
const Sint32 pl2e_special_04[] = {
	CG_FRAME(    0,   19,   2,           0,   625, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0,   626, 0xb0,   0x20, ATK | 0x0b,  0x01,       0x14,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0,   627, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0,   628, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   3,           0,   629, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0,   630, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   1,           0,   631, 0xb0,   0x20, ATK | 0x14,  0x01,       0x24,    0,   0,     0,      0,  85 ),
};

const Sint32 pl2e_special_04_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   632, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   633, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2e_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl2e_special_05[] = {
	CG_FRAME(    0,   32,  10,           0,   625, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   626, 0xb0,   0x20, ATK | 0x0b,  0x01,       0x14,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   627, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   628, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   629, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2,           0,   630, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,   2,           0,   631, 0xb0,   0x20, ATK | 0x14,  0x01,       0x24,    0,   0,     0,      0,  85 ),
};

const Sint32 pl2e_special_05_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   632, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   633, 0xb0,   0x20,        ATK,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2e_special_05_loop ),
};

/* Char_Special 6: cannon_air_shot (Shoulder Cannon, 6 + atck_level)
   Char_Special 7 */
const Sint32 pl2e_special_06[] = {
	CG_FRAME(    0,    0,  22,           0,   633, 0xb0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   634, 0xb0,   0x20,        ATE,  0x01,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2e_special_06 ),
};

/* Char_Special 8: ccannon_init (Shoulder Cannon, crouching, 8 + atck_level) */
const Sint32 pl2e_special_08[] = {
	CG_FRAME(    0,   19,   5,           0,   160, 0x80,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   5,           0,   205, 0xa0,   0x6e, ATK | 0x0b,  0x00,       0x14,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   3,           0,   473, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   474, 0xa0,   0x6e, ATK | 0x14,  0x00,       0x24,    0,   0,     0,      0, 103 ),
};

const Sint32 pl2e_special_08_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   475, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   474, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2e_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl2e_special_09[] = {
	CG_FRAME(    0,   32,   5,           0,   160, 0x80,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   8,           0,   205, 0xa0,   0x6e, ATK | 0x0b,  0x00,       0x14,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   3,           0,   473, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   2,           0,   474, 0xa0,   0x6e, ATK | 0x14,  0x00,       0x24,    0,   0,     0,      0, 103 ),
};

const Sint32 pl2e_special_09_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   475, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   474, 0xa0,   0x6e,        ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2e_special_09_loop ),
};

/* Char_Special 10: ccannon_shot (Shoulder Cannon, crouching, 10 + atck_level)
   Char_Special 11 */
const Sint32 pl2e_special_10[] = {
	CG_FRAME(    0,    0,  14,           0,   482, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   484, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   2,           0,   485, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   486, 0xa0,   0x6e,        ATE,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2e_special_10 ),
};

/* Char_Special 12: repulsor_init (Repulsor Blast, 12 + atck_level) */
const Sint32 pl2e_special_12[] = {
	CG_FRAME(    0,    0,   3,           0,   610, 0xb0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   610, 0xb0,   0x0f, ATK | 0x0c,  0x00,       0x15,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl2e_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl2e_special_13[] = {
	CG_FRAME(    0,    0,   3,           0,   610, 0xb0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   610, 0xb0,   0x0f, ATK | 0x0c,  0x00,       0x15,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl2e_special_13 ),
};

/* Char_Special 14: repulsor_move (Repulsor Blast) */
const Sint32 pl2e_special_14[] = {
	CG_FRAME(    0,    0,   3,           0,   619, 0xb0,   0x12, ATK | 0x16,  0x00,       0x21,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   620, 0xb0,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   621, 0xb0,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   622, 0xb0,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   623, 0xb0,   0x12,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_special_14 ),
};

/* Char_Special 15: repulsor_wait (Repulsor Blast) */
const Sint32 pl2e_special_15[] = {
	CG_FRAME(    0,    0,   8,           0,   619, 0xb0,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   624, 0xb0,   0x12,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_special_15 ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl2e_catch_00[] = {
	CG_FRAME(    0,    1,   4,           0,   584, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    1,   4,           0,   585, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    1,   4,           0,   588, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,   4,           0,   587, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,   4,           0,   588, 0x80,   0x00,       0x0e,  0x00,       0x22,    0,   0,     0, 0x0003,   0 ),
};

const Sint32 pl2e_catch_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   136, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   137, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   138, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   1,           0,   139, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   140, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   141, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   142, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   1,           0,   143, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   144, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0,   145, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   146, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_NEXT( pl2e_catch_00_loop ),
};

/* Char_Catch 1: nml_k_drag */
const Sint32 pl2e_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   2,           0,   589, 0x80,   0x00,       0x13,  0x00,       0x05,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    2,   2,           0,   590, 0x80,   0x00,       0x18,  0x00,       0x20,    0,   0,     0, 0x0009,   0 ),
};

const Sint32 pl2e_catch_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   589, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   589, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_catch_01_loop ),
};

/* Char_Catch 2 */
const Sint32 pl2e_catch_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   135, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_catch_02 ),
};

/* Char_Catch 3: jump_p_set */
const Sint32 pl2e_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   4,           0,   586, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   4,           0,   591, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   6,           0,   635, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   636, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   637, 0x80,   0x20,       0x12,  0x01,       0x04,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    1,   2,           0,   638, 0x80,   0x20,       0x18,  0x01,       0x20,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    2,   4,           0,   639, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   8,           0,   640, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   640, 0x80,   0x20,          0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_catch_03 ),
};

/* Char_Catch 4: nml_p_set */
const Sint32 pl2e_catch_04[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   585, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   8,           0,   588, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   3,           0,   587, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   3,           0,   588, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,  10,           0,   584, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    1,   2,           0,   589, 0x80,   0x00,       0x12,  0x00,       0x04,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    2,   4,           0,   590, 0x80,   0x00,       0x18,  0x00,       0x20,    0,   0,     0, 0x0009,   0 ),
};

const Sint32 pl2e_catch_04_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   589, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   589, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_catch_04_loop ),
};

/* Char_Effect 30: shl2e_03_down_init */
const Sint32 pl2e_effect_30[] = {
	CG_FRAME(    0,    0,   1,           0,   665, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   671, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   666, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   672, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   667, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   673, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   668, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   674, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   669, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   675, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   670, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   675, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   679, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   675, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   676, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   677, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   676, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   677, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   676, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   678, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   676, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   678, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   680, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   678, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   676, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   680, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   681, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   682, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   683, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   1,           0,   682, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
};

const Sint32 pl2e_effect_30_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   683, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl2e_effect_30_loop ),
};

/* Char_Effect 31: shl2e_03_up_init */
const Sint32 pl2e_effect_31[] = {
	CG_FRAME(    0,    0,   2,           0,   683, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   681, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   680, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   678, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   680, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   678, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   678, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   678, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   677, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   677, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   676, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   675, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   679, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   675, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   670, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   675, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   669, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   674, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   668, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   673, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   667, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   672, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   666, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   671, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   684, 0xc0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_31 ),
};

/* Char_Super 0: proton_init (Proton Cannon) */
const Sint32 pl2e_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   206, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   0,           0,   414, 0xc0,   0x00,    ATK | 6,  0x00,       0x18,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   2,           0,   692, 0xc0,   0x0f,        ATK,  0x00,       0x18,    0,   0,     0,      0,   4 ),
};

const Sint32 pl2e_super_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   691, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   692, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_super_00_loop ),
};

/* Char_Super 1: proton_move (Proton Cannon) */
const Sint32 pl2e_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   691, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   692, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   691, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   692, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   691, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   692, 0xc0,   0x0f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   8,           0,   697, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    2,   2,           0,   693, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0xc0,   0x0e, ATK | 0x0a,  0x00,       0x1a,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   695, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   693, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
};

const Sint32 pl2e_super_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   694, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   695, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   693, 0xc0,   0x0e,        ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2e_super_01_loop ),
};

/* Char_Super 2: proton_wait (Proton Cannon) */
const Sint32 pl2e_super_02[] = {
	CG_FRAME(    0,    0,  48, END_OF_DATA,   697, 0xc0,   0x0f,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_super_02 ),
};

/* Char_Super 3: destroyer_init (War Destroyer) */
const Sint32 pl2e_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   207, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   685, 0x80,   0x00,    ATK | 5,  0x00,       0x17,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,  12,           0,   686, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   4,           0,   687, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    1,   4,           0,   688, 0x80,   0x00,    ATK | 7,  0x00,       0x19,    0,   0,     0,      0,   4 ),
};

const Sint32 pl2e_super_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   689, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   690, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_super_03_loop ),
};

/* Char_Super 4: destroyer_hold (War Destroyer) */
const Sint32 pl2e_super_04[] = {
	CG_FRAME(    0,    0,  28, END_OF_DATA,   685, 0x80,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_super_04 ),
};

/* Char_Effect 32: shl2e_05_destroyer_init (32 + sub_type) */
const Sint32 pl2e_effect_32[] = {
	CG_FRAME(    0,    0,   4,           0, 32837, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0, 32838, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
};

const Sint32 pl2e_effect_32_loop[] = {
	CG_FRAME(    0,    0,   1,           0, 32839, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32840, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32841, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32842, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32843, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32844, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32845, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32846, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32847, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl2e_effect_32_loop ),
};

/* Char_Effect 33 */
const Sint32 pl2e_effect_33[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32848, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl2e_effect_33 ),
};

/* Char_Effect 34: shl2e_05_destroyer_up (34 + sub_type) */
const Sint32 pl2e_effect_34[] = {
	CG_FRAME(    0,    0,   4,           0, 32825, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   2,           0, 32826, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
};

const Sint32 pl2e_effect_34_loop[] = {
	CG_FRAME(    0,    0,   1,           0, 32827, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32828, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32829, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32830, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32831, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32832, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32833, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1,           0, 32834, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32835, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl2e_effect_34_loop ),
};

/* Char_Effect 35 */
const Sint32 pl2e_effect_35[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32836, 0x80,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl2e_effect_35 ),
};

/* Char_Effect 36: pls2e_00_destroyer_init */
const Sint32 pl2e_effect_36[] = {
	CG_FRAME(    0,    0,   2,           0,  1078, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1080, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1082, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1084, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1086, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1088, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_effect_36_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32855, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32857, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_36_loop ),
};

/* Char_Effect 37: pls2e_00_destroyer_init */
const Sint32 pl2e_effect_37[] = {
	CG_FRAME(    0,    0,   2,           0, 32849, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32850, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32851, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32852, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32853, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32854, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_effect_37_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32856, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32858, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_37_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl2e_damage_00[] = {
	CG_FRAME( 0x80,    0,   2,           0,   788, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   6,           0,   789, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   790, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl2e_damage_01[] = {
	CG_FRAME( 0x80,    0,   2,           0,   791, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   8,           0,   792, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   793, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   790, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl2e_damage_02[] = {
	CG_FRAME( 0x80,    0,   3,           0,   794, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   9,           0,   795, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   792, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   793, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   790, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl2e_damage_03[] = {
	CG_FRAME( 0x80,    0,   2,           0,   796, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   6,           0,   797, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   798, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl2e_damage_04[] = {
	CG_FRAME( 0x80,    0,   2,           0,   799, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,  10,           0,   800, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   801, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl2e_damage_05[] = {
	CG_FRAME( 0x80,    0,   3,           0,   802, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   3,           0,   803, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   804, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   805, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   806, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl2e_damage_06[] = {
	CG_FRAME( 0x80,    0,   2,           0,   807, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,   9, END_OF_DATA,   808, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl2e_damage_07[] = {
	CG_FRAME( 0x80,    0,   2,           0,   809, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,  10,           0,   810, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   209, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl2e_damage_08[] = {
	CG_FRAME( 0x80,    0,   2,           0,   811, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x81,    0,  10,           0,   812, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   813, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   814, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   815, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2e_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl2e_damage_10[] = {
	CG_FRAME(    0,    0,   8,           0,   797, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   798, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl2e_damage_11[] = {
	CG_FRAME(    0,    0,   6,           0,   800, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   801, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   798, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl2e_damage_12[] = {
	CG_FRAME( 0x80,    0,   2,           0,   816, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x81,    0,   6,           0,   817, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   818, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl2e_damage_13[] = {
	CG_FRAME( 0x80,    0,   2,           0,   819, 0x40,   0xa9,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x81,    0,   2,           0,   820, 0x40,   0xa9,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   821, 0x40,   0xa9,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   822, 0x40,   0xa9,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2e_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl2e_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   833, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   832, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   834, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   831, 0x40,   0x8b,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   835, 0x40,   0xc9,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl2e_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   836, 0x40,   0xc9,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2e_damage_09_loop ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl2e_damage_26[] = {
	CG_FRAME(    0,    0,   3,           0,   829, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   830, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3,           0,   831, 0x40,   0x8c,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   863, 0x60,   0xc8,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2e_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl2e_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   837, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   845, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   846, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   841, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   842, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   843, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   844, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   165, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl2e_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   837, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   838, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   839, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   840, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   841, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   842, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   843, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   844, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   164, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   165, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_30 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl2e_damage_33[] = {
	CG_FRAME(    0,    0,   3,           0,   361, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   362, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   363, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   364, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   190, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_damage_33 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl2e_damage_16[] = {
	CG_FRAME(    0,    0,   4,           0,   823, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   824, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl2e_damage_17[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   825, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl2e_damage_18[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   826, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl2e_damage_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   827, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl2e_damage_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   828, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl2e_damage_21[] = {
	CG_FRAME(    0,    0,   4,           0,   823, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   824, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl2e_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   825, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl2e_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   826, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl2e_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   827, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl2e_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   828, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2e_damage_25 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl2e_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   848, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   849, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   850, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   851, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   852, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   853, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl2e_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   854, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   855, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   856, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   857, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   858, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   859, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2e_damage_29 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl2e_dizzy_00[] = {
	CG_FRAME(    0,    0,   8,           0,   860, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   861, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   862, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   861, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_dizzy_00 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl2e_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   863, 0x60,   0xc8,          0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2e_damage_31 ),
};

/* Char_Caught 0 */
const Sint32 pl2e_caught_00[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   789, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl2e_caught_01[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   791, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl2e_caught_02[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   794, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl2e_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl2e_caught_03[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   797, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl2e_caught_04[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   800, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl2e_caught_05[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   803, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl2e_caught_06[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   807, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl2e_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl2e_caught_07[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   810, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl2e_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl2e_caught_08[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   811, 0x40,   0x6e,          0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl2e_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl2e_caught_09[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   797, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl2e_caught_10[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   803, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl2e_caught_11[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   816, 0x40,   0x20,          0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl2e_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl2e_caught_12[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   820, 0x40,   0xc8,          0,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl2e_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl2e_caught_13[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   823, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_13 ),
};

const Sint32 pl2e_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,  15, END_OF_DATA,   824, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	0x00005d20,
};

/* Char_Caught 14 */
const Sint32 pl2e_caught_14[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   825, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl2e_caught_15[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   826, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl2e_caught_16[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   827, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl2e_caught_17[] = {
	CG_FRAME(    0,    0,  15, END_OF_DATA,   828, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2e_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl2e_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   848, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl2e_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl2e_caught_19[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   854, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl2e_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl2e_caught_20[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   863, 0x60,   0xc8,          0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl2e_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl2e_caught_21[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   833, 0x40,   0x00,          0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl2e_caught_21 ),
};

/* Char_Effect 41: shl2e_05_shot_init, shl2e_06_part_init */
const Sint32 pl2e_effect_41[] = {
	CG_FRAME(    0,    0,   1,           0,   874, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,   1,           0,   875, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 104 ),
};

const Sint32 pl2e_effect_41_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   876, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   868, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   869, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   870, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   871, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   872, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   873, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   874, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   875, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl2e_effect_41_loop ),
};

/* Char_Effect 42 */
const Sint32 pl2e_effect_42[] = {
	CG_FRAME(    0,    0,   4,           0,   877, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   878, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   879, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   880, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_42 ),
};

/* Char_Effect 44 */
const Sint32 pl2e_effect_44[] = {
	CG_FRAME(    0,    0,   2,           0,   881, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   882, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   883, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   884, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   885, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   886, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   887, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   888, 0xa0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_44 ),
};

/* Char_Effect 40 */
const Sint32 pl2e_effect_40[] = {
	CG_FRAME(    0,    0,   3,           0,   881, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   882, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   2,           0,   883, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   884, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   885, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   886, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   887, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   888, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_40 ),
};

/* Char_Effect 39 */
const Sint32 pl2e_effect_39[] = {
	CG_FRAME(    0,    0,   2,           0,   889, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   890, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   891, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   892, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   893, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   894, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   895, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   896, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_39 ),
};

/* Char_Effect 43 */
const Sint32 pl2e_effect_43[] = {
	CG_FRAME(    0,    0,   2,           0,   897, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   898, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   899, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   900, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3,           0,   901, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   902, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   903, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   904, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   905, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   906, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_43 ),
};

/* Char_Effect 7: pls2e_00_flash_init */
const Sint32 pl2e_effect_07[] = {
	CG_FRAME(    0,    0,   4,           0, 32768, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32770, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32771, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32773, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32774, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32775, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32776, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32777, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32778, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_effect_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32779, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32785, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32790, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_07_loop ),
};

/* Char_Effect 38 */
const Sint32 pl2e_effect_38[] = {
	CG_FRAME(    0,    0,   6,           0,   930, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0,   931, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   932, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   933, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   934, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   935, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   936, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   937, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   2,           0,   938, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   939, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   940, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   941, 0xa0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_38 ),
};

/* Char_Effect 45 */
const Sint32 pl2e_effect_45[] = {
	CG_FRAME(    0,    0,   2,           0,   460, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   462, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   452, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   453, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   454, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   455, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   456, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   459, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_45 ),
};

/* Char_Effect 46 */
const Sint32 pl2e_effect_46[] = {
	CG_FRAME(    0,    0,   2,           0,   502, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   503, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   505, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   506, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   507, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   2,           0,   508, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   509, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_46 ),
};

/* Char_Effect 47 */
const Sint32 pl2e_effect_47[] = {
	CG_FRAME(    0,    0,   4,           0,   659, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   660, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   663, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   661, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   664, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   662, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_47 ),
};

const Sint32 pl2e_unused_02[] = {	/* nothing reaches it */
	0x80010000,
};

const Sint32 pl2e_unused_03[] = {
	0x008000a0,
};

const Sint32 pl2e_unused_04[] = {
	0x00000000,
};

const Sint32 pl2e_unused_05[] = {
	0x00000000,
};

const Sint32 pl2e_unused_06[] = {
	CG_NEXT( pl2e_effect_47 ),
};

/* Char_Effect 48 */
const Sint32 pl2e_effect_48[] = {
	CG_FRAME(    0,    0,   4,           0,   651, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   652, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   653, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   654, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   655, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   656, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   657, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   658, 0xa0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_48 ),
};

/* Char_Appear 10: pls2e_00_appear_ground_init, pls2e_00_fly_ground_init */
const Sint32 pl2e_appear_10[] = {
	CG_FRAME(    0,    0,   2,           0,   942, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   943, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   944, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   945, 0x80,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_appear_10 ),
};

/* Char_Effect 0: shl2e_00_part_init */
const Sint32 pl2e_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0,   947, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   946, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl2e_effect_00 ),
};

/* Char_Effect 1: shl2e_00_beam_move */
const Sint32 pl2e_effect_01[] = {
	CG_FRAME(    0,    0,   1,           0,   952, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   953, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   958, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   959, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   964, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   965, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   970, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   971, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_01 ),
};

/* Char_Effect 2: shl2e_00_part_init */
const Sint32 pl2e_effect_02[] = {
	CG_FRAME(    0,    0,   1,           0, 32791, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
};

/* Char_Effect 3: shl2e_00_part_init */
const Sint32 pl2e_effect_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32792, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl2e_effect_02 ),
};

/* Char_Effect 4: shl2e_00_beam_move */
const Sint32 pl2e_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0, 32793, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32798, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32799, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32800, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_04 ),
};

/* Char_Effect 5: shl2e_00_head_init */
const Sint32 pl2e_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0,   950, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   951, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl2e_effect_05 ),
};

/* Char_Effect 6: shl2e_00_fire */
const Sint32 pl2e_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0,   956, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   957, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   962, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   963, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   968, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   969, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   974, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   975, 0xb0,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_06 ),
};

/* Char_Effect 12: shl2e_02_bomb_init, shl2e_02_trail_init */
const Sint32 pl2e_effect_12[] = {
	CG_FRAME(    0,    0,   2,           0, 32801, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0, 32802, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0, 32803, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0, 32804, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2,           0, 32805, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32806, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl2e_effect_12 ),
};

/* Char_Effect 13: shl2e_02_burst */
const Sint32 pl2e_effect_13[] = {
	CG_FRAME(    0,    0,   2,           0,  1069, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1070, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1071, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1072, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1073, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1074, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,  1075, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,  1076, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  1077, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl2e_effect_14[] = {
	CG_FRAME(    0,    0,   4,           0,  1076, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,  1077, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_14 ),
};

/* Char_Effect 10 */
const Sint32 pl2e_effect_10[] = {
	CG_FRAME(    0,    0,   1,           0,   986, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   987, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   988, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   989, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   976, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   977, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   0,           0,   979, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   980, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   0,           0,   981, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   1,           0,   982, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   1,           0,   983, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   0,           0,   982, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   0,           0,   983, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   983, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   984, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   985, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   984, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   983, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   983, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   982, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   983, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   982, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   981, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   980, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   979, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   978, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   977, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   989, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   988, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   987, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   986, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl2e_effect_11[] = {
	CG_FRAME(    0,    0,   1,           0,  1000, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1001, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1002, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1003, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   990, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   991, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   1,           0,   992, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   0,           0,   993, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   0,           0,   995, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   1,           0,   996, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   1,           0,   997, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   0,           0,   996, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   0,           0,   997, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   997, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   998, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   999, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   998, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   997, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   997, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   996, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   997, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,   996, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   995, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   994, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   993, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   992, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   991, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1003, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1002, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1001, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1000, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_11 ),
};

/* Char_Effect 8 */
const Sint32 pl2e_effect_08[] = {
	CG_FRAME(    0,    0,   1,           0,  1042, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1043, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1044, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1045, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1032, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1033, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   0,           0,  1034, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   0,           0,  1035, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1036, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   0,           0,  1037, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1038, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   1,           0,  1039, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1038, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1039, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1040, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1041, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1040, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1039, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1038, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1039, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1038, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1037, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1036, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1035, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1034, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1033, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1045, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1044, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1043, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1042, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl2e_effect_09[] = {
	CG_FRAME(    0,    0,   1,           0,  1056, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1057, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1058, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1059, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1046, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1047, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   0,           0,  1048, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   0,           0,  1049, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,  1050, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   0,           0,  1051, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,  1052, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   0,           0,  1053, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   0,           0,  1052, 0xb0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,  1053, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1054, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1055, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1054, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1053, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1052, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1053, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1052, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1051, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1050, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1049, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1048, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1047, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1059, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1058, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   0,           0,  1057, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1056, 0xb0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_09 ),
};

/* Char_Effect 19: shl2e_04_head_init */
const Sint32 pl2e_effect_19[] = {
	CG_FRAME(    0,    0,   1,           0, 32807, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32808, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32809, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32810, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2e_effect_19_loop[] = {
	CG_FRAME(    0,    0,   2,           0, 32811, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0, 32812, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   2,           0, 32813, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA, 32814, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl2e_effect_19_loop ),
};

/* Char_Effect 20: shl2e_04_fire */
const Sint32 pl2e_effect_20[] = {
	CG_FRAME(    0,    0,   2,           0, 32815, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32809, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32808, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32807, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_20 ),
};

/* Char_Effect 21: shl2e_04_part_init (21 + sub_type) */
const Sint32 pl2e_effect_21[] = {
	CG_FRAME(    0,    0,   1,           0, 32816, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 22 */
const Sint32 pl2e_effect_22[] = {
	CG_FRAME(    0,    0,   1,           0, 32816, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 23 */
const Sint32 pl2e_effect_23[] = {
	CG_FRAME(    0,    0,   1,           0, 32817, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 24 */
const Sint32 pl2e_effect_24[] = {
	CG_FRAME(    0,    0,   1,           0, 32817, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 25 */
const Sint32 pl2e_effect_25[] = {
	CG_FRAME(    0,    0,   1,           0, 32818, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 26 */
const Sint32 pl2e_effect_26[] = {
	CG_FRAME(    0,    0,   1,           0, 32818, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 27 */
const Sint32 pl2e_effect_27[] = {
	CG_FRAME(    0,    0,   1,           0, 32819, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

/* Char_Effect 28 */
const Sint32 pl2e_effect_28[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32819, 0xc0,   0x00,        ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl2e_effect_21 ),
};

/* Char_Effect 29: shl2e_04_beam */
const Sint32 pl2e_effect_29[] = {
	CG_FRAME(    0,    0,   2,           0, 32820, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32821, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32822, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32823, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32824, 0xc0,   0x00,        ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_29 ),
};

/* Char_Fly 0: fly_init (Flight) */
const Sint32 pl2e_fly_00[] = {
	CG_FRAME(    0,    0,   2,           0,   169, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   2,           0,   170, 0x90,   0x20,       0x0e,  0x00,       0x22,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   2,           0,   171, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
};

const Sint32 pl2e_fly_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   172, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   173, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   174, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   175, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   176, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   177, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   178, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   179, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl2e_fly_00_loop ),
};

/* Char_Fly 1: act_fly
   Char_Fly 5
   Char_Fly 6
   Char_Fly 7
   Char_Fly 10
   Char_Fly 11 */
const Sint32 pl2e_fly_01[] = {
	CG_FRAME(    0,    1,   1,           0,   717, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   718, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl2e_fly_01 ),
};

/* Char_Effect 15: pls2e_00_fly_init */
const Sint32 pl2e_effect_15[] = {
	CG_FRAME(    0,    0,   1,           0,   719, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   720, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   721, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   722, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   723, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   724, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   725, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   726, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_15 ),
};

/* Char_Fly 2 */
const Sint32 pl2e_fly_02[] = {
	CG_FRAME(    0,    2,   1,           0,   727, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    2,   1, END_OF_DATA,   728, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl2e_fly_02 ),
};

/* Char_Effect 16 */
const Sint32 pl2e_effect_16[] = {
	CG_FRAME(    0,    0,   1,           0,   729, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   730, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   731, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   732, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   733, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   734, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   735, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   736, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_16 ),
};

/* Char_Fly 8
   Char_Fly 9 */
const Sint32 pl2e_fly_08[] = {
	CG_FRAME(    0,    3,   1,           0,   737, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    3,   1, END_OF_DATA,   738, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl2e_fly_08 ),
};

/* Char_Effect 17 */
const Sint32 pl2e_effect_17[] = {
	CG_FRAME(    0,    0,   1,           0,   739, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   740, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   741, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   742, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   743, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   744, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   745, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   746, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_17 ),
};

/* Char_Fly 3
   Char_Fly 4 */
const Sint32 pl2e_fly_03[] = {
	CG_FRAME(    0,    4,   1,           0,   747, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    4,   1, END_OF_DATA,   748, 0x90,   0x20,          0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl2e_fly_03 ),
};

/* Char_Effect 18 */
const Sint32 pl2e_effect_18[] = {
	CG_FRAME(    0,    0,   1,           0,   749, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   750, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   751, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   752, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   753, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   754, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   755, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   756, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_18 ),
};

/* Char_Effect 49: pls2e_00_bomb_init */
const Sint32 pl2e_effect_49[] = {
	CG_FRAME(    0,    0,   1,           0,   757, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   758, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   759, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   760, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   761, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   762, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   763, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   764, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_49 ),
};

/* Char_Effect 50 */
const Sint32 pl2e_effect_50[] = {
	CG_FRAME(    0,    0,   1,           0,   765, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   766, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   767, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   768, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   769, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   770, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   771, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   772, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_50 ),
};

/* Char_Effect 51 */
const Sint32 pl2e_effect_51[] = {
	CG_FRAME(    0,    0,   1,           0,   773, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   774, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   775, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   776, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   777, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   778, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   779, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   780, 0x90,   0x00,          0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2e_effect_51 ),
};

/* Char_Extra 3: chouhatsu_init (Taunt), fly_start5 */
const Sint32 pl2e_extra_03[] = {
	CG_FRAME(    0,    0,   2,           0,   698, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   699, 0x80,   0x40,          1,  0x00,       0x10,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   700, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   701, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   702, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   703, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   704, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   705, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   706, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   707, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   708, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   709, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   710, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   711, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   712, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2e_extra_03 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl2e_change_04[] = {
	CG_FRAME(    0,    0,   2,           0,   698, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   699, 0x80,   0x40,          1,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   700, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   701, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   702, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   703, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   704, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   705, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   706, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   707, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   708, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   709, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   710, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   711, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  48, END_OF_DATA,   712, 0x80,   0x40,          0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2e_change_04 ),
};

/* Char_Special 18: snapback_init (Snapback) */
const Sint32 pl2e_special_18[] = {
	CG_FRAME(    0,    0,   5,           0,   419, 0xa0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   1,           0,   420, 0xa0,   0x6f,        ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   421, 0xa0,   0x8c,        ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    2,   2,           0,   422, 0xa0,   0x8c, ATK | 0x12,  0x00,       0x04,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    1,   2,           0,   423, 0xa0,   0x6f, ATK | 0x17,  0x00,       0x1e,    0,   0,     0,      0,  14 ),
};

const Sint32 pl2e_special_18_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   424, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   423, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl2e_special_18_loop ),
};

/* Char_Special 19: snapback_move (Snapback) */
const Sint32 pl2e_special_19[] = {
	CG_FRAME(    0,    0,   4,           0,   425, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   426, 0xa0,   0x6f,        ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl2e_special_19 ),
};
