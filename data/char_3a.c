/* Servbot (character 3a): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_3a      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_3a     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl3a.c) says which group
                   and number each move uses.
   pl3a_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl3a_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl3a_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl3a_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_3a
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_3a, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_3a */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl3a_special_00[], pl3a_stand_00[], pl3a_stand_11[], pl3a_stand_12[],
	pl3a_stand_02[], pl3a_stand_03[], pl3a_stand_06[], pl3a_stand_07[], pl3a_jump_00[],
	pl3a_jump_09[], pl3a_jump_01[], pl3a_jump_01_loop[], pl3a_jump_02[], pl3a_jump_02_loop[],
	pl3a_jump_03[], pl3a_jump_05[], pl3a_jump_06[], pl3a_jump_06_loop[], pl3a_jump_07[],
	pl3a_jump_07_loop[], pl3a_jump_08[], pl3a_jump_08_loop[], pl3a_dash_00[], pl3a_dash_02[],
	pl3a_dash_01[], pl3a_dash_03[], pl3a_crouch_00[], pl3a_crouch_01[], pl3a_crouch_02[],
	pl3a_turn_00[], pl3a_turn_01[], pl3a_guard_00[], pl3a_guard_01[], pl3a_guard_02[],
	pl3a_guard_04[], pl3a_guard_05[], pl3a_guard_06[], pl3a_air_guard_00[], pl3a_air_guard_01[],
	pl3a_stand_p_00[], pl3a_stand_p_00_loop[], pl3a_stand_p_01[], pl3a_stand_p_01_loop[],
	pl3a_stand_k_00[], pl3a_stand_k_00_loop[], pl3a_stand_k_01[], pl3a_stand_k_01_loop[],
	pl3a_grp27_07[], pl3a_grp27_08[], pl3a_stand_p_02[], pl3a_stand_p_02_loop[], pl3a_extra_01[],
	pl3a_extra_01_loop[], pl3a_grp27_02[], pl3a_grp27_05[], pl3a_grp27_05_loop[], pl3a_grp27_06[],
	pl3a_grp27_06_loop[], pl3a_grp27_03[], pl3a_grp27_03_loop[], pl3a_grp27_04[],
	pl3a_grp27_04_loop[], pl3a_crouch_p_00[], pl3a_crouch_p_00_loop[], pl3a_crouch_p_01[],
	pl3a_crouch_p_01_loop[], pl3a_crouch_k_00[], pl3a_crouch_k_00_loop[], pl3a_crouch_k_01[],
	pl3a_crouch_k_01_loop[], pl3a_crouch_p_02[], pl3a_crouch_p_02_loop[], pl3a_jump_p_00[],
	pl3a_jump_p_00_loop[], pl3a_jump_p_01[], pl3a_jump_p_01_loop[], pl3a_jump_p_02[],
	pl3a_jump_p_02_loop[], pl3a_jump_k_00[], pl3a_jump_k_00_loop[], pl3a_jump_k_01[],
	pl3a_jump_k_01_loop[], pl3a_jump_k_02[], pl3a_jump_k_02_loop[], pl3a_get_up_00[],
	pl3a_get_up_01[], pl3a_get_up_02[], pl3a_get_up_03[], pl3a_damage_00[], pl3a_damage_01[],
	pl3a_damage_02[], pl3a_damage_03[], pl3a_damage_04[], pl3a_damage_05[], pl3a_damage_06[],
	pl3a_damage_07[], pl3a_damage_08[], pl3a_damage_10[], pl3a_damage_11[], pl3a_damage_12[],
	pl3a_damage_13[], pl3a_damage_34[], pl3a_damage_09[], pl3a_damage_16[], pl3a_damage_17[],
	pl3a_damage_18[], pl3a_damage_19[], pl3a_damage_20[], pl3a_damage_21[], pl3a_damage_22[],
	pl3a_damage_23[], pl3a_damage_24[], pl3a_damage_25[], pl3a_damage_26[], pl3a_damage_27[],
	pl3a_damage_27_loop[], pl3a_damage_31[], pl3a_damage_28[], pl3a_damage_29[], pl3a_caught_00[],
	pl3a_caught_01[], pl3a_caught_02[], pl3a_caught_03[], pl3a_caught_04[], pl3a_caught_05[],
	pl3a_caught_06[], pl3a_caught_07[], pl3a_caught_08[], pl3a_caught_09[], pl3a_caught_10[],
	pl3a_caught_11[], pl3a_caught_12[], pl3a_caught_13[], pl3a_caught_14[], pl3a_caught_15[],
	pl3a_caught_16[], pl3a_caught_17[], pl3a_caught_18[], pl3a_caught_19[], pl3a_caught_20[],
	pl3a_caught_21[], pl3a_dizzy_00[], pl3a_win_00[], pl3a_win_00_loop[], pl3a_win_01[],
	pl3a_win_03[], pl3a_win_04[], pl3a_effect_35[], pl3a_effect_34[], pl3a_win_02[],
	pl3a_appear_00[], pl3a_appear_00_loop[], pl3a_grp27_00[], pl3a_grp27_00_loop[], pl3a_appear_01[],
	pl3a_appear_02[], pl3a_special_01[], pl3a_special_02[], pl3a_special_02_loop[], pl3a_change_00[],
	pl3a_change_00_loop[], pl3a_change_02[], pl3a_change_01[], pl3a_change_03[],
	pl3a_change_03_loop[], pl3a_change_04[], pl3a_change_04_loop[], pl3a_super_16[],
	pl3a_super_16_loop[], pl3a_super_17[], pl3a_super_18[], pl3a_super_20[], pl3a_super_21[],
	pl3a_super_21_loop[], pl3a_super_22[], pl3a_super_23[], pl3a_super_23_loop[], pl3a_special_05[],
	pl3a_special_05_loop[], pl3a_special_06[], pl3a_extra_00[], pl3a_extra_00_loop[],
	pl3a_super_00[], pl3a_super_02[], pl3a_super_02_loop[], pl3a_super_03[], pl3a_super_03_loop[],
	pl3a_super_04[], pl3a_super_04_loop[], pl3a_super_05[], pl3a_super_11[], pl3a_super_11_loop[],
	pl3a_super_06[], pl3a_super_12[], pl3a_super_08[], pl3a_super_07[], pl3a_super_14[],
	pl3a_super_09[], pl3a_super_15[], pl3a_super_10[], pl3a_super_13[], pl3a_effect_02[],
	pl3a_effect_03[], pl3a_effect_04[], pl3a_effect_05[], pl3a_effect_06[], pl3a_effect_07[],
	pl3a_effect_08[], pl3a_effect_09[], pl3a_effect_10[], pl3a_effect_11[], pl3a_effect_12[],
	pl3a_effect_13[], pl3a_effect_14[], pl3a_effect_15[], pl3a_effect_16[], pl3a_effect_17[],
	pl3a_effect_18[], pl3a_effect_19[], pl3a_effect_20[], pl3a_effect_21[], pl3a_effect_22[],
	pl3a_effect_23[], pl3a_effect_24[], pl3a_effect_25[], pl3a_effect_26[], pl3a_effect_27[],
	pl3a_effect_28[], pl3a_effect_29[], pl3a_effect_30[], pl3a_effect_31[], pl3a_special_07[],
	pl3a_special_11[], pl3a_special_11_loop[], pl3a_special_12[], pl3a_special_12_loop[],
	pl3a_special_13[], pl3a_special_08[], pl3a_special_08_loop[], pl3a_special_09[],
	pl3a_special_09_loop[], pl3a_special_10[], pl3a_special_16[], pl3a_special_03[],
	pl3a_special_14[], pl3a_special_14_loop[], pl3a_special_04[], pl3a_special_15[],
	pl3a_special_15_loop[], pl3a_effect_00[], pl3a_effect_00_loop[], pl3a_effect_32[],
	pl3a_effect_01[], pl3a_effect_01_loop[], pl3a_effect_33[], pl3a_catch_00[], pl3a_catch_00_loop[],
	pl3a_catch_01[], pl3a_catch_01_loop[], pl3a_catch_02[], pl3a_catch_02_loop[], pl3a_grp27_01[],
	pl3a_grp27_01_loop[], pl3a_effect_43[], pl3a_effect_42[], pl3a_effect_50[], pl3a_effect_51[],
	pl3a_effect_37[], pl3a_effect_36[], pl3a_effect_45[], pl3a_effect_45_loop[], pl3a_effect_46[],
	pl3a_effect_46_loop[], pl3a_effect_39[], pl3a_effect_39_loop[], pl3a_effect_40[],
	pl3a_effect_40_loop[], pl3a_effect_38[], pl3a_effect_41[], pl3a_effect_44[], pl3a_effect_47[],
	pl3a_effect_48[], pl3a_effect_49[], pl3a_effect_49_loop[], pl3a_effect_52[], pl3a_effect_53[],
	pl3a_effect_54[], pl3a_effect_55[], pl3a_grp27_09[], pl3a_grp27_10[], pl3a_grp27_11[],
	pl3a_grp27_12[], pl3a_grp27_13[], pl3a_grp27_14[], pl3a_grp27_15[], pl3a_grp27_16[],
	pl3a_grp27_16_loop[], pl3a_grp27_17[], pl3a_grp27_18[];
extern const Sint32 *const pl3a_stand_tbl[], *const pl3a_jump_tbl[], *const pl3a_dash_tbl[],
	*const pl3a_crouch_tbl[], *const pl3a_turn_tbl[], *const pl3a_guard_tbl[],
	*const pl3a_air_guard_tbl[], *const pl3a_stand_p_tbl[], *const pl3a_stand_k_tbl[],
	*const pl3a_crouch_p_tbl[], *const pl3a_crouch_k_tbl[], *const pl3a_jump_p_tbl[],
	*const pl3a_jump_k_tbl[], *const pl3a_damage_tbl[], *const pl3a_dizzy_tbl[],
	*const pl3a_catch_tbl[], *const pl3a_caught_tbl[], *const pl3a_get_up_tbl[],
	*const pl3a_appear_tbl[], *const pl3a_win_tbl[], *const pl3a_extra_tbl[],
	*const pl3a_special_tbl[], *const pl3a_super_tbl[], *const pl3a_effect_tbl[],
	*const pl3a_change_tbl[], *const pl3a_grp25_tbl[], *const pl3a_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_3a[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1250, 1250, 0, 0, 0, 0, 0, 0 },
	{ 1490, 1490, 0, 0, 0, 0, 0, 0 },
	{ 1750, 1750, 0, 0, 0, 0, 0, 0 },
	{ 1870, 1870, 0, 0, 0, 0, 0, 0 },
	{ 2000, 2000, 0, 0, 0, 0, 0, 0 },
	{ 2000, 2000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 2560, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -2560, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1600, 1600, 0, 0, 0, 0, 0, 0 },
	{ 2200, 2200, 0, 0, 0, 0, 0, 0 },
	{ 2800, 2800, 0, 0, 0, 0, 0, 0 },
	{ 3400, 3400, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
};

/* the groups */
const Sint32 *const *const char_tbl_3a[] = {
	pl3a_stand_tbl, pl3a_jump_tbl, pl3a_dash_tbl, pl3a_crouch_tbl, pl3a_turn_tbl, pl3a_guard_tbl,
	pl3a_air_guard_tbl, pl3a_stand_p_tbl, pl3a_stand_k_tbl, pl3a_crouch_p_tbl, pl3a_crouch_k_tbl, pl3a_jump_p_tbl,
	pl3a_jump_k_tbl, pl3a_damage_tbl, pl3a_dizzy_tbl, pl3a_catch_tbl, pl3a_caught_tbl, pl3a_get_up_tbl,
	pl3a_appear_tbl, pl3a_win_tbl, pl3a_extra_tbl, pl3a_special_tbl, pl3a_super_tbl, pl3a_effect_tbl,
	pl3a_change_tbl, pl3a_grp25_tbl, pl3a_grp25_tbl, pl3a_grp27_tbl, 0,
};

const Sint32 *const pl3a_stand_tbl[] = {
	pl3a_stand_00, pl3a_stand_00, pl3a_stand_02, pl3a_stand_03, pl3a_stand_00, pl3a_stand_00,
	pl3a_stand_06, pl3a_stand_07, pl3a_stand_02, pl3a_stand_03, pl3a_stand_00, pl3a_stand_11,
	pl3a_stand_12, 0,
};

const Sint32 *const pl3a_jump_tbl[] = {
	pl3a_jump_00, pl3a_jump_01, pl3a_jump_02, pl3a_jump_03, pl3a_jump_03, pl3a_jump_05,
	pl3a_jump_06, pl3a_jump_07, pl3a_jump_08, pl3a_jump_09, pl3a_jump_09, pl3a_jump_09,
	0,
};

const Sint32 *const pl3a_dash_tbl[] = {
	pl3a_dash_00, pl3a_dash_01, pl3a_dash_02, pl3a_dash_03, 0,
};

const Sint32 *const pl3a_crouch_tbl[] = {
	pl3a_crouch_00, pl3a_crouch_01, pl3a_crouch_02, 0,
};

const Sint32 *const pl3a_turn_tbl[] = {
	pl3a_turn_00, pl3a_turn_01, 0,
};

const Sint32 *const pl3a_guard_tbl[] = {
	pl3a_guard_00, pl3a_guard_01, pl3a_guard_02, pl3a_guard_01, pl3a_guard_04, pl3a_guard_05,
	pl3a_guard_06, pl3a_guard_05, 0,
};

const Sint32 *const pl3a_air_guard_tbl[] = {
	pl3a_air_guard_00, pl3a_air_guard_01, pl3a_air_guard_01, 0,
};

const Sint32 *const pl3a_stand_p_tbl[] = {
	pl3a_stand_p_00, pl3a_stand_p_01, pl3a_stand_p_02, 0,
};

const Sint32 *const pl3a_stand_k_tbl[] = {
	pl3a_stand_k_00, pl3a_stand_k_01, pl3a_stand_p_02, 0,
};

const Sint32 *const pl3a_crouch_p_tbl[] = {
	pl3a_crouch_p_00, pl3a_crouch_p_01, pl3a_crouch_p_02, 0,
};

const Sint32 *const pl3a_crouch_k_tbl[] = {
	pl3a_crouch_k_00, pl3a_crouch_k_01, pl3a_crouch_p_02, 0,
};

const Sint32 *const pl3a_jump_p_tbl[] = {
	pl3a_jump_p_00, pl3a_jump_p_01, pl3a_jump_p_02, pl3a_jump_p_00, pl3a_jump_p_01, pl3a_jump_p_02,
	0,
};

const Sint32 *const pl3a_jump_k_tbl[] = {
	pl3a_jump_k_00, pl3a_jump_k_01, pl3a_jump_k_02, pl3a_jump_k_00, pl3a_jump_k_01, pl3a_jump_k_02,
	0,
};

const Sint32 *const pl3a_damage_tbl[] = {
	pl3a_damage_00, pl3a_damage_01, pl3a_damage_02, pl3a_damage_03, pl3a_damage_04, pl3a_damage_05,
	pl3a_damage_06, pl3a_damage_07, pl3a_damage_08, pl3a_damage_09, pl3a_damage_10, pl3a_damage_11,
	pl3a_damage_12, pl3a_damage_13, pl3a_damage_13, pl3a_damage_13, pl3a_damage_16, pl3a_damage_17,
	pl3a_damage_18, pl3a_damage_19, pl3a_damage_20, pl3a_damage_21, pl3a_damage_22, pl3a_damage_23,
	pl3a_damage_24, pl3a_damage_25, pl3a_damage_26, pl3a_damage_27, pl3a_damage_28, pl3a_damage_29,
	pl3a_damage_27, pl3a_damage_31, pl3a_damage_27, pl3a_damage_27, pl3a_damage_34, 0,
};

const Sint32 *const pl3a_dizzy_tbl[] = {
	pl3a_dizzy_00, 0,
};

const Sint32 *const pl3a_catch_tbl[] = {
	pl3a_catch_00, pl3a_catch_01, pl3a_catch_02, 0,
};

const Sint32 *const pl3a_caught_tbl[] = {
	pl3a_caught_00, pl3a_caught_01, pl3a_caught_02, pl3a_caught_03, pl3a_caught_04, pl3a_caught_05,
	pl3a_caught_06, pl3a_caught_07, pl3a_caught_08, pl3a_caught_09, pl3a_caught_10, pl3a_caught_11,
	pl3a_caught_12, pl3a_caught_13, pl3a_caught_14, pl3a_caught_15, pl3a_caught_16, pl3a_caught_17,
	pl3a_caught_18, pl3a_caught_19, pl3a_caught_20, pl3a_caught_21, 0,
};

const Sint32 *const pl3a_get_up_tbl[] = {
	pl3a_get_up_00, pl3a_get_up_01, pl3a_get_up_02, pl3a_get_up_03, 0,
};

const Sint32 *const pl3a_appear_tbl[] = {
	pl3a_appear_00, pl3a_appear_01, pl3a_appear_02, 0,
};

const Sint32 *const pl3a_win_tbl[] = {
	pl3a_win_00, pl3a_win_01, pl3a_win_02, pl3a_win_03, pl3a_win_04, 0,
};

const Sint32 *const pl3a_extra_tbl[] = {
	pl3a_extra_00, pl3a_extra_01, 0,
};

const Sint32 *const pl3a_special_tbl[] = {
	pl3a_special_00, pl3a_special_01, pl3a_special_02, pl3a_special_03, pl3a_special_04, pl3a_special_05,
	pl3a_special_06, pl3a_special_07, pl3a_special_08, pl3a_special_09, pl3a_special_10, pl3a_special_11,
	pl3a_special_12, pl3a_special_13, pl3a_special_14, pl3a_special_15, pl3a_special_16, 0,
};

const Sint32 *const pl3a_super_tbl[] = {
	pl3a_super_00, pl3a_super_00, pl3a_super_02, pl3a_super_03, pl3a_super_04, pl3a_super_05,
	pl3a_super_06, pl3a_super_07, pl3a_super_08, pl3a_super_09, pl3a_super_10, pl3a_super_11,
	pl3a_super_12, pl3a_super_13, pl3a_super_14, pl3a_super_15, pl3a_super_16, pl3a_super_17,
	pl3a_super_18, pl3a_super_16, pl3a_super_20, pl3a_super_21, pl3a_super_22, pl3a_super_23,
	0,
};

const Sint32 *const pl3a_effect_tbl[] = {
	pl3a_effect_00, pl3a_effect_01, pl3a_effect_02, pl3a_effect_03, pl3a_effect_04, pl3a_effect_05,
	pl3a_effect_06, pl3a_effect_07, pl3a_effect_08, pl3a_effect_09, pl3a_effect_10, pl3a_effect_11,
	pl3a_effect_12, pl3a_effect_13, pl3a_effect_14, pl3a_effect_15, pl3a_effect_16, pl3a_effect_17,
	pl3a_effect_18, pl3a_effect_19, pl3a_effect_20, pl3a_effect_21, pl3a_effect_22, pl3a_effect_23,
	pl3a_effect_24, pl3a_effect_25, pl3a_effect_26, pl3a_effect_27, pl3a_effect_28, pl3a_effect_29,
	pl3a_effect_30, pl3a_effect_31, pl3a_effect_32, pl3a_effect_33, pl3a_effect_34, pl3a_effect_35,
	pl3a_effect_36, pl3a_effect_37, pl3a_effect_38, pl3a_effect_39, pl3a_effect_40, pl3a_effect_41,
	pl3a_effect_42, pl3a_effect_43, pl3a_effect_44, pl3a_effect_45, pl3a_effect_46, pl3a_effect_47,
	pl3a_effect_48, pl3a_effect_49, pl3a_effect_50, pl3a_effect_51, pl3a_effect_52, pl3a_effect_53,
	pl3a_effect_54, pl3a_effect_55, 0,
};

const Sint32 *const pl3a_change_tbl[] = {
	pl3a_change_00, pl3a_change_01, pl3a_change_02, pl3a_change_03, pl3a_change_04, 0,
};

const Sint32 *const pl3a_grp25_tbl[] = {
	pl3a_stand_00, 0,
};

const Sint32 *const pl3a_grp27_tbl[] = {
	pl3a_grp27_00, pl3a_grp27_01, pl3a_grp27_02, pl3a_grp27_03, pl3a_grp27_04, pl3a_grp27_05,
	pl3a_grp27_06, pl3a_grp27_07, pl3a_grp27_08, pl3a_grp27_09, pl3a_grp27_10, pl3a_grp27_11,
	pl3a_grp27_12, pl3a_grp27_13, pl3a_grp27_14, pl3a_grp27_15, pl3a_grp27_16, pl3a_grp27_17,
	pl3a_grp27_18, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Special 0 */
const Sint32 pl3a_special_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_00 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Stand 10
   group 25 0 */
const Sint32 pl3a_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_00 ),
};

/* Char_Stand 11 */
const Sint32 pl3a_stand_11[] = {
	CG_FRAME(    0,    0,   6,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_11 ),
};

/* Char_Stand 12 */
const Sint32 pl3a_stand_12[] = {
	CG_FRAME(    0,    0,   6,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   281, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   280, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   278, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6, END_OF_DATA,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_12 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 8 */
const Sint32 pl3a_stand_02[] = {
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 9 */
const Sint32 pl3a_stand_03[] = {
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_03 ),
};

/* Char_Stand 6 */
const Sint32 pl3a_stand_06[] = {
	CG_FRAME(    1,    0,   4,           0,   879, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   880, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   881, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   882, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   883, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   884, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   885, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA,   886, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl3a_stand_07[] = {
	CG_FRAME(    1,    0,   4,           0,   879, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   880, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   881, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   882, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   883, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   884, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   885, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA,   886, 0x80,   0x00,    0,  0x00,          0,    9,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_07 ),
};

/* Char_Jump 0 */
const Sint32 pl3a_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: shl3a_01_mode_set, act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl3a_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl3a_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl3a_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_02_loop ),
};

/* Char_Jump 3: shl3a_01_mode_land, shl3a_01_mode_drop
   Char_Jump 4 */
const Sint32 pl3a_jump_03[] = {
	CG_FRAME(    0,    0,   3,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_jump_03 ),
};

/* Char_Jump 5 */
const Sint32 pl3a_jump_05[] = {
	CG_FRAME(    0,    0,   3,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_jump_05 ),
};

/* Char_Jump 6 */
const Sint32 pl3a_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl3a_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_07_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl3a_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_08_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl3a_dash_00[] = {
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_dash_00 ),
};

/* Char_Dash 2 */
const Sint32 pl3a_dash_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl3a_dash_01[] = {
	CG_FRAME(    0,    0,   3,           0,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_dash_01 ),
};

/* Char_Dash 3 */
const Sint32 pl3a_dash_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl3a_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl3a_crouch_01[] = {
	CG_FRAME(    0,    0,   6,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   284, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   288, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   289, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl3a_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl3a_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl3a_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl3a_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl3a_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl3a_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl3a_guard_04[] = {
	CG_FRAME(    0,    0,   4,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl3a_guard_05[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl3a_guard_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl3a_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl3a_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl3a_stand_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    6,   2,           0,   469, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
};

const Sint32 pl3a_stand_p_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   471, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_p_00_loop ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl3a_stand_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    7,   2,           0,   482, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
};

const Sint32 pl3a_stand_p_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   484, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_p_01_loop ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl3a_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   467, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   468, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    6,   2,           0,   469, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
};

const Sint32 pl3a_stand_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   471, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_k_00_loop ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl3a_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   480, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   481, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    7,   2,           0,   482, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
};

const Sint32 pl3a_stand_k_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   484, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_k_01_loop ),
};

/* group 27 7 */
const Sint32 pl3a_grp27_07[] = {
	EF_FRAME(    0,    0,   2,           0,   472 ),
	EF_FRAME(    0,    0,   2,           0,   473 ),
	EF_FRAME(    0,    0,   2,           0,   474 ),
	EF_FRAME(    0,    0,   2,           0,   475 ),
	EF_FRAME(    0,    0,   2,           0,   476 ),
	EF_FRAME(    0,    0,   2,           0,   477 ),
	EF_FRAME(    0,    0,   2,           0,   478 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   479 ),
	CG_NEXT( pl3a_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl3a_grp27_08[] = {
	EF_FRAME(    0,    0,   2,           0,   485 ),
	EF_FRAME(    0,    0,   2,           0,   486 ),
	EF_FRAME(    0,    0,   2,           0,   487 ),
	EF_FRAME(    0,    0,   2,           0,   488 ),
	EF_FRAME(    0,    0,   2,           0,   489 ),
	EF_FRAME(    0,    0,   2,           0,   490 ),
	EF_FRAME(    0,    0,   2,           0,   491 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   492 ),
	CG_NEXT( pl3a_grp27_08 ),
};

/* Char_Stand_P 2 (heavy)
   Char_Stand_K 2 (heavy) */
const Sint32 pl3a_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   493, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   494, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   495, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   496, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   500, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   501, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   504, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   505, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_stand_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   506, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_stand_p_02_loop ),
};

/* Char_Extra 1: pl3a_upper */
const Sint32 pl3a_extra_01[] = {
	CG_FRAME(    0,    0,   2,           0,   493, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   494, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   495, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   496, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    1,   2,           0,   497, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   500, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   501, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   502, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   503, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   504, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   505, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_extra_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   506, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_extra_01_loop ),
};

/* group 27 2 */
const Sint32 pl3a_grp27_02[] = {
	EF_FRAME(    0,    0,   2,           0,   507 ),
	EF_FRAME(    0,    0,   2,           0,   508 ),
	EF_FRAME(    0,    0,   2,           0,   509 ),
	EF_FRAME(    0,    0,   2,           0,   510 ),
	EF_FRAME(    0,    0,   4,           0,   511 ),
	EF_FRAME(    0,    0,   4,           0,   512 ),
	EF_FRAME(    0,    0,   4,           0,   513 ),
	EF_FRAME(    0,    0,   4,           0,   514 ),
	EF_FRAME(    0,    0,   4,           0,   515 ),
	EF_FRAME(    0,    0,   4,           0,   516 ),
	EF_FRAME(    0,    0,   4,           0,   517 ),
	EF_FRAME(    0,    0,   4,           0,   518 ),
	EF_FRAME(    0,    0,   4,           0,   519 ),
	EF_FRAME(    0,    0,   4,           0,   520 ),
	EF_FRAME(    0,    0,   4,           0,   521 ),
	EF_FRAME(    0,    0,   4,           0,   522 ),
	EF_FRAME(    0,    0,   4,           0,   523 ),
	EF_FRAME(    0,    0,   4,           0,   524 ),
	EF_FRAME(    0,    1,   4,           0,   524 ),
	EF_FRAME(    0,    1,   4, END_OF_DATA,   524 ),
	CG_NEXT( pl3a_grp27_02 ),
};

/* group 27 5 */
const Sint32 pl3a_grp27_05[] = {
	EF_FRAME(    0,    0,   3,           0,   543 ),
	EF_FRAME(    0,    0,   3,           0,   544 ),
	EF_FRAME(    0,    0,   3,           0,   545 ),
	EF_FRAME(    0,    0,   3,           0,   543 ),
	EF_FRAME(    0,    0,   3,           0,   543 ),
	EF_FRAME(    0,    0,   3,           0,   543 ),
	EF_FRAME(    0,    0,   3,           0,   543 ),
	EF_FRAME(    0,    0,   3,           0,   543 ),
	EF_FRAME(    0,    0,   3,           0,   543 ),
};

const Sint32 pl3a_grp27_05_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   546 ),
	CG_NEXT( pl3a_grp27_05_loop ),
};

/* group 27 6 */
const Sint32 pl3a_grp27_06[] = {
	EF_FRAME(    0,    0,   3,           0,   563 ),
	EF_FRAME(    0,    0,   3,           0,   564 ),
	EF_FRAME(    0,    0,   3,           0,   565 ),
	EF_FRAME(    0,    0,   3,           0,   563 ),
	EF_FRAME(    0,    0,   3,           0,   563 ),
	EF_FRAME(    0,    0,   3,           0,   563 ),
	EF_FRAME(    0,    0,   3,           0,   563 ),
	EF_FRAME(    0,    0,   3,           0,   563 ),
	EF_FRAME(    0,    0,   3,           0,   563 ),
};

const Sint32 pl3a_grp27_06_loop[] = {
	EF_FRAME(    0,    0,   3, END_OF_DATA,   566 ),
	CG_NEXT( pl3a_grp27_06_loop ),
};

/* group 27 3 */
const Sint32 pl3a_grp27_03[] = {
	EF_FRAME(    0,    0,   3,           0,   531 ),
	EF_FRAME(    0,    0,   3,           0,   532 ),
	EF_FRAME(    0,    0,   3,           0,   533 ),
	EF_FRAME(    0,    0,   3,           0,   534 ),
	EF_FRAME(    0,    0,   3,           0,   535 ),
	EF_FRAME(    0,    0,   3,           0,   537 ),
	EF_FRAME(    0,    0,   3,           0,   538 ),
	EF_FRAME(    0,    0,   3,           0,   539 ),
	EF_FRAME(    0,    0,   3,           0,   540 ),
	EF_FRAME(    0,    1,   3,           0,   540 ),
};

const Sint32 pl3a_grp27_03_loop[] = {
	EF_FRAME(    0,    1,   5, END_OF_DATA,   540 ),
	CG_NEXT( pl3a_grp27_03_loop ),
};

/* group 27 4 */
const Sint32 pl3a_grp27_04[] = {
	EF_FRAME(    0,    0,   3,           0,   551 ),
	EF_FRAME(    0,    0,   3,           0,   552 ),
	EF_FRAME(    0,    0,   3,           0,   553 ),
	EF_FRAME(    0,    0,   3,           0,   554 ),
	EF_FRAME(    0,    0,   3,           0,   555 ),
	EF_FRAME(    0,    0,   3,           0,   557 ),
	EF_FRAME(    0,    0,   3,           0,   558 ),
	EF_FRAME(    0,    0,   3,           0,   559 ),
	EF_FRAME(    0,    0,   3,           0,   560 ),
	EF_FRAME(    0,    1,   3,           0,   560 ),
};

const Sint32 pl3a_grp27_04_loop[] = {
	EF_FRAME(    0,    1,   5, END_OF_DATA,   560 ),
	CG_NEXT( pl3a_grp27_04_loop ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl3a_crouch_p_00[] = {
	CG_FRAME(    4,    2,   3,           0,   527, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl3a_crouch_p_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_p_00_loop ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl3a_crouch_p_01[] = {
	CG_FRAME(    5,    3,   3,           0,   547, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl3a_crouch_p_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   550, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_p_01_loop ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl3a_crouch_k_00[] = {
	CG_FRAME(    4,    2,   3,           0,   527, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl3a_crouch_k_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_k_00_loop ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl3a_crouch_k_01[] = {
	CG_FRAME(    5,    3,   3,           0,   547, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl3a_crouch_k_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   550, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_k_01_loop ),
};

/* Char_Crouch_P 2 (heavy)
   Char_Crouch_K 2 (heavy) */
const Sint32 pl3a_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   567, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   568, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   571, 0x80,   0x0d,  ATK,  0x01,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   2,           0,   572, 0x80,   0x0d,  ATK,  0x01,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   4,           0,   573, 0x80,   0x0d,  ATK,  0x01,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,   4,           0,   574, 0x80,   0x0d,  ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   4,           0,   575, 0x80,   0x0d,  ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   576, 0x80,   0x0d,  ATE,  0x01,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   577, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   578, 0x80,   0x0f,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   579, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl3a_crouch_p_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   581, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_crouch_p_02_loop ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl3a_jump_p_00[] = {
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x00,  ATK,  0x00, 0x18 | CSE,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   2,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,   2,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   588, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_p_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   589, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_p_00_loop ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl3a_jump_p_01[] = {
	CG_FRAME(    0,    0,   1,           0,   590, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   591, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   592, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   593, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   594, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,   2,           0,   595, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   596, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   597, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,   2,           0,   598, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_p_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   601, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_p_01_loop ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl3a_jump_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   602, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   603, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   604, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   605, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   4,           0,   606, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    0,   4,           0,   607, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   4,           0,   608, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   4,           0,   609, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,   4,           0,   610, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   611, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_p_02_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   613, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_p_02_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl3a_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   614, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   615, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   2,           0,   616, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x80,    0,   2,           0,   617, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   2,           0,   618, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   4,           0,   619, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   4,           0,   620, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,   4,           0,   621, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   622, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
};

const Sint32 pl3a_jump_k_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   623, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_k_00_loop ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl3a_jump_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   624, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   2,           0,   625, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2,           0,   626, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,   2,           0,   627, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   2,           0,   628, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x80,    0,   4,           0,   629, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   630, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   631, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   632, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   633, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   634, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl3a_jump_k_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   635, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_k_01_loop ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl3a_jump_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   636, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   637, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   638, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   639, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,   4,           0,   640, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME( 0x80,    0,   4,           0,   641, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   4,           0,   642, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   4,           0,   643, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,   4,           0,   644, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   645, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,   646, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_jump_k_02_loop ),
};

/* Char_Get_Up 0: shl3a_01_mode_set, dm_dizzy_wait */
const Sint32 pl3a_get_up_00[] = {
	CG_FRAME(    0,    0,   6,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl3a_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl3a_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl3a_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl3a_damage_00[] = {
	CG_FRAME(    0,    0,   7,           0,   815, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   814, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl3a_damage_01[] = {
	CG_FRAME(    0,    0,  10,           0,   818, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   817, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   816, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl3a_damage_02[] = {
	CG_FRAME(    0,    0,  10,           0,   822, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   821, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   820, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   819, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl3a_damage_03[] = {
	CG_FRAME(    0,    0,   4,           0,   824, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   823, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl3a_damage_04[] = {
	CG_FRAME(    0,    0,   4,           0,   827, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   826, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   825, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl3a_damage_05[] = {
	CG_FRAME(    0,    0,   4,           0,   831, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   830, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   829, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   828, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl3a_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   833, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   832, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl3a_damage_07[] = {
	CG_FRAME(    0,    0,  10,           0,   836, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   835, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   834, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl3a_damage_08[] = {
	CG_FRAME(    0,    0,  10,           0,   840, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   839, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   838, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   837, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl3a_damage_10[] = {
	CG_FRAME(    0,    0,   4,           0,   842, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   841, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_10 ),
};

/* Char_Damage 11: shl3a_01_mode_set */
const Sint32 pl3a_damage_11[] = {
	CG_FRAME(    0,    0,   2,           0,   845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   844, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   843, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl3a_damage_12[] = {
	CG_FRAME(    0,    0,   2,           0,   849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   848, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   846, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl3a_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   862, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl3a_damage_13 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl3a_damage_34[] = {
	CG_FRAME(    0,    0,   4,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl3a_damage_34 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl3a_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   850, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   850, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   858, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   862, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl3a_damage_09 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl3a_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   853, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl3a_damage_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   854, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl3a_damage_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   855, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl3a_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   856, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl3a_damage_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   857, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl3a_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   853, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl3a_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   854, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl3a_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   855, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl3a_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   856, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl3a_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   857, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl3a_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   858, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   862, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl3a_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl3a_damage_27[] = {
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_damage_27_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_27_loop ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl3a_damage_31[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl3a_damage_31 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl3a_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   864, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl3a_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   871, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   872, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   873, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   874, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl3a_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   815, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl3a_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   818, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl3a_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl3a_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   822, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl3a_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl3a_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   824, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl3a_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl3a_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   827, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl3a_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl3a_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   831, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl3a_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl3a_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   833, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl3a_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl3a_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   836, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl3a_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl3a_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   840, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl3a_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl3a_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   842, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl3a_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl3a_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl3a_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl3a_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl3a_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl3a_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl3a_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl3a_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl3a_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   854, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl3a_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   855, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl3a_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   856, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl3a_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   857, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl3a_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl3a_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   871, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl3a_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   863, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl3a_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl3a_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   851, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl3a_caught_21 ),
};

/* Char_Dizzy 0: shl3a_01_mode_catch_start, act_dizzy */
const Sint32 pl3a_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   875, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   876, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   877, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   878, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_dizzy_00 ),
};

/* Char_Win 0 */
const Sint32 pl3a_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,   429, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   430, 0x80,   0x00,    0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   434, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   435, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   435, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   436, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   438, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   439, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   440, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   441, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   442, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   443, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   444, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   445, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_win_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   447, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl3a_win_01[] = {
	CG_FRAME(    0,    0,   8,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   747, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   748, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   749, 0x80,   0x0f,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   750, 0x80,   0x0e,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   449, 0x80,   0x0d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   450, 0x80,   0x0c,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   451, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   452, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   453, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Win 3: win_time_ko (3 + no) */
const Sint32 pl3a_win_03[] = {
	CG_FRAME(    1,    0,   5,           0,   454, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   455, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   456, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   457, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   458, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   459, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   460, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5, END_OF_DATA,   461, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_win_03 ),
};

/* Char_Win 4 */
const Sint32 pl3a_win_04[] = {
	CG_FRAME(    1,    0,   4,           0,   879, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   880, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   881, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   882, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   883, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   884, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   885, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA,   886, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_win_04 ),
};

/* Char_Effect 35 */
const Sint32 pl3a_effect_35[] = {
	CG_FRAME(    0,    0, 127, END_OF_DATA,    -1, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl3a_effect_35 ),
};

/* Char_Effect 34: shl3a_02_init */
const Sint32 pl3a_effect_34[] = {
	CG_FRAME(    0,    0,  10,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,  10,           0,   466, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_NEXT( pl3a_effect_34 ),
};

/* Char_Win 2: lose_time_ko */
const Sint32 pl3a_win_02[] = {
	CG_FRAME(    0,    0,   6,           0,   875, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   876, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   877, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   878, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_win_02 ),
};

/* Char_Appear 0 */
const Sint32 pl3a_appear_00[] = {
	CG_FRAME(    0,    0,  48,           0,   384, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  64,           0,   384, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_appear_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_appear_00_loop ),
};

/* group 27 0 */
const Sint32 pl3a_grp27_00[] = {
	EF_FRAME(    0,    1,   7,           0,   392 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   394 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   392 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   394 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   392 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   394 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   392 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,   7,           0,   394 ),
	EF_FRAME(    0,    1,   7,           0,   393 ),
	EF_FRAME(    0,    1,  12,           0,   395 ),
	EF_FRAME(    0,    1,   5,           0,   396 ),
	EF_FRAME(    0,    1,   5,           0,   397 ),
};

const Sint32 pl3a_grp27_00_loop[] = {
	EF_FRAME(    0,    0,   4,           0,   398 ),
	EF_FRAME(    0,    0,   4,           0,   399 ),
	EF_FRAME(    0,    0,   4,           0,   400 ),
	EF_FRAME(    0,    0,   4,           0,   401 ),
	EF_FRAME(    0,    0,   4,           0,   402 ),
	EF_FRAME(    0,    0,   4,           0,   403 ),
	EF_FRAME(    0,    0,   4,           0,   404 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   405 ),
	CG_NEXT( pl3a_grp27_00_loop ),
};

/* Char_Appear 1 */
const Sint32 pl3a_appear_01[] = {
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_appear_01 ),
};

/* Char_Appear 2: appear_fall */
const Sint32 pl3a_appear_02[] = {
	CG_FRAME(    0,    0,   6,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   424, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   426, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   427, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   428, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  31,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_appear_02 ),
};

/* Char_Special 1: t_kick_init */
const Sint32 pl3a_special_01[] = {
	CG_FRAME(    0,    0,   1,           0,   645, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
};

/* Char_Special 2 */
const Sint32 pl3a_special_02[] = {
	CG_FRAME(    0,    0,   6,           0,   379, 0x80,   0x00,  ATE,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   380, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  16,           0,   381, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   380, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_special_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   379, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_02_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl3a_change_00[] = {
	CG_FRAME(    0,    0,   3,           0,   374, 0x80,   0x00,    0,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_change_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl3a_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_change_02 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl3a_change_01[] = {
	CG_FRAME(    0,    1,   4, END_OF_DATA,   373, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_change_01 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl3a_change_03[] = {
	CG_FRAME(    0,    1,   4,           0,   354, 0x80,   0x00,    0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_change_03_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl3a_change_04[] = {
	CG_FRAME(    0,    0,   5,           0,   376, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_change_04_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_change_04_loop ),
};

/* Char_Super 16: lunch_burst_init
   Char_Super 19: lunch_drop_init */
const Sint32 pl3a_super_16[] = {
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_super_16_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   382, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   383, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_16_loop ),
};

/* Char_Super 17: lunch_burst_set */
const Sint32 pl3a_super_17[] = {
	CG_FRAME(    0,    0,   4,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_super_17 ),
};

/* Char_Super 18: lunch_burst_loop */
const Sint32 pl3a_super_18[] = {
	CG_FRAME(    0,    0,  20,           0,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_18 ),
};

/* Char_Super 20: lunch_drop_shot */
const Sint32 pl3a_super_20[] = {
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_20 ),
};

/* Char_Super 21: lunch_drop_wait */
const Sint32 pl3a_super_21[] = {
	CG_FRAME(    0,    0,   2,           0,   648, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
};

const Sint32 pl3a_super_21_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_NEXT( pl3a_super_21_loop ),
};

/* Char_Super 22: lunch_drop_stop */
const Sint32 pl3a_super_22[] = {
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Super 23: lunch_drop_end */
const Sint32 pl3a_super_23[] = {
	CG_FRAME(    0,    0,   4,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_super_23_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_23_loop ),
};

/* Char_Special 5: sp_engo */
const Sint32 pl3a_special_05[] = {
	CG_FRAME(    0,    1,   4,           0,   354, 0x80,   0x00,    0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_special_05_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   383, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_05_loop ),
};

/* Char_Special 6: sp_engo */
const Sint32 pl3a_special_06[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_06 ),
};

/* Char_Extra 0: sp_chouhatsu */
const Sint32 pl3a_extra_00[] = {
	CG_FRAME(    0,    0,   6,           0,   406, 0x80,   0x00,    0,  0x00,       0x0e,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   407, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   408, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   409, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   413, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   414, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   415, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   416, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_extra_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_extra_00_loop ),
};

/* Char_Super 0: snapback_init
   Char_Super 1 */
const Sint32 pl3a_super_00[] = {
	CG_FRAME(    0,    0,   8,           0,   567, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   2,           0,   568, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   569, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   570, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   571, 0x80,   0x0d,  ATK,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   572, 0x80,   0x0d,  ATK,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   4,           0,   573, 0x80,   0x0d,  ATK,  0x01,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME( 0x80,    0,   4,           0,   574, 0x80,   0x0d,  ATE,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,   4,           0,   575, 0x80,   0x0d,  ATE,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   576, 0x80,   0x0d,  ATE,  0x01,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   577, 0x80,   0x0d,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   578, 0x80,   0x0f,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   579, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   580, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   581, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl3a_super_00 ),
};

/* Char_Super 2: lunch_init */
const Sint32 pl3a_super_02[] = {
	CG_FRAME(    0,    0,   3,           0,   354, 0x80,   0x00,  ATK,  0x00,       0x1c,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    1,   3,           0,   747, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   748, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   749, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   750, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   751, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   752, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   753, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   754, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   755, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   756, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   3,           0,   757, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
};

const Sint32 pl3a_super_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   758, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   759, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   760, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   761, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_NEXT( pl3a_super_02_loop ),
};

/* Char_Super 3: lunch_move */
const Sint32 pl3a_super_03[] = {
	CG_FRAME(    0,    0,   3,           0,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_super_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_03_loop ),
};

/* Char_Super 4: lunch_wait */
const Sint32 pl3a_super_04[] = {
	CG_FRAME(    0,    0,   3,           0,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_super_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_04_loop ),
};

/* Char_Super 5: king_init, rking_init */
const Sint32 pl3a_super_05[] = {
	CG_FRAME(    0,    0,   6,           0,   776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   777, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   779, 0x80,   0x00,  ATK,  0x01,       0x24,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   779, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   779, 0x80,   0x00,  ATK,  0x03,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   779, 0x80,   0x00,  ATK,  0x04,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   779, 0x80,   0x20,  ATK,  0x04,          0,   10,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   779, 0x80,   0x40,  ATK,  0x04,          0,   11,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   5,           0,   779, 0x80,   0x60,  ATK,  0x04,          0,   12,   0,     0,      0,  12 ),
	CG_FRAME(    1,    0,   5,           0,   779, 0x80,   0x80,  ATK,  0x04,          0,   13,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    1,   5,           0,   785, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,   5,           0,   786, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   787, 0x80,   0x80,  ATE,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_NEXT( pl3a_super_12 ),
};

/* Char_Super 11: king_loop, rking_stop */
const Sint32 pl3a_super_11[] = {
	CG_FRAME(    0,    0,   6,           0,   787, 0x80,   0x80,    0,  0x00,       0x25,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6,           0,   786, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6,           0,   785, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   6,           0,   784, 0x80,   0x80,    0,  0x00,          0,    4,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6,           0,   783, 0x80,   0x60,    0,  0x00,          0,    3,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   6,           0,   782, 0x80,   0x40,    0,  0x00,          0,    2,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   781, 0x80,   0x20,    0,  0x00,          0,    1,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   780, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  24,           0,   779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   778, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_super_11_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_super_11_loop ),
};

/* Char_Super 6: turn_ck */
const Sint32 pl3a_super_06[] = {
	CG_FRAME(    0,    1,   2,           0,   790, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   2,           0,   789, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   2,           0,   788, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
};

/* Char_Super 12: king_stand_set */
const Sint32 pl3a_super_12[] = {
	CG_FRAME(    0,    0,   8,           0,   811, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   8,           0,   812, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   813, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_NEXT( pl3a_super_12 ),
};

/* Char_Super 8 */
const Sint32 pl3a_super_08[] = {
	CG_FRAME(    0,    1,   2,           0,   790, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   2,           0,   789, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    1,   2,           0,   788, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
};

/* Char_Super 7 */
const Sint32 pl3a_super_07[] = {
	CG_FRAME(    0,    0,   8,           0,   795, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    1,    1,   8,           0,   796, 0x80,   0x80,    0,  0x01, 0x30 | CSE,    5,   0,     0,      0,  12 ),
	CG_FRAME(    2,    0,   8,           0,   791, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    3,    0,   8,           0,   792, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    4,    0,   8,           0,   799, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    5,    1,   8,           0,   800, 0x80,   0x80,    0,  0x01, 0x30 | CSE,    5,   0,     0,      0,  12 ),
	CG_FRAME(    6,    0,   8,           0,   793, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    7,    0,   8, END_OF_DATA,   794, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_NEXT( pl3a_super_07 ),
};

/* Char_Super 14 */
const Sint32 pl3a_super_14[] = {
	CG_FRAME(    0,    0,   8,           0,   795, 0x80,   0x80,    0,  0x00,          0,    6,   0,     0,      0,  12 ),
	CG_FRAME(    1,    1,   8,           0,   796, 0x80,   0x80,    0,  0x01, 0x30 | CSE,    6,   0,     0,      0,  12 ),
	CG_FRAME(    2,    0,   8,           0,   791, 0x80,   0x80,    0,  0x00,          0,    6,   0,     0,      0,  12 ),
	CG_FRAME(    3,    0,   8,           0,   792, 0x80,   0x80,    0,  0x00,          0,    6,   0,     0,      0,  12 ),
	CG_FRAME(    4,    0,   8,           0,   799, 0x80,   0x80,    0,  0x00,          0,    6,   0,     0,      0,  12 ),
	CG_FRAME(    5,    1,   8,           0,   800, 0x80,   0x80,    0,  0x01, 0x30 | CSE,    6,   0,     0,      0,  12 ),
	CG_FRAME(    6,    0,   8,           0,   793, 0x80,   0x80,    0,  0x00,          0,    6,   0,     0,      0,  12 ),
	CG_FRAME(    7,    0,   8, END_OF_DATA,   794, 0x80,   0x80,    0,  0x00,          0,    6,   0,     0,      0,  12 ),
	CG_NEXT( pl3a_super_14 ),
};

/* Char_Super 9 */
const Sint32 pl3a_super_09[] = {
	CG_FRAME(    0,    0,   2,           0,   795, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   796, 0x80,   0x80,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   797, 0x80,   0x80,  ATK,  0x01,          0,    5,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   798, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   799, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   800, 0x80,   0x80,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   801, 0x80,   0x80,  ATK,  0x01,          0,    5,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   802, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   795, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   796, 0x80,   0x80,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   797, 0x80,   0x80,  ATK,  0x01,          0,    5,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   798, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   799, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   800, 0x80,   0x80,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   801, 0x80,   0x80,  ATK,  0x01,          0,    5,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   802, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_NEXT( pl3a_super_09 ),
};

/* Char_Super 15: rking_set */
const Sint32 pl3a_super_15[] = {
	CG_FRAME(    0,    0,   2,           0,   795, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   796, 0x80,   0x80,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   797, 0x80,   0x80,  ATK,  0x01,          0,    5,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   798, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   799, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   800, 0x80,   0x80,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   801, 0x80,   0x80,  ATK,  0x01,          0,    5,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   802, 0x80,   0x80,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_NEXT( pl3a_super_15 ),
};

/* Char_Super 10: dash_ck, rking_run */
const Sint32 pl3a_super_10[] = {
	CG_FRAME(    0,    0,   1,           0,   803, 0x80,   0x80,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   1,           0,   804, 0x80,   0x8a,    0,  0x00,          0,    5,   0,     0,      0,  12 ),
};

/* Char_Super 13: king_dash */
const Sint32 pl3a_super_13[] = {
	CG_FRAME(    0,    0,   1,           0,   805, 0x80,   0x87,  ATK,  0x00,          0,    5,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   3,           0,   805, 0x80,   0x87,  ATK,  0x00,       0x26,    5,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,   1,           0,   806, 0x80,   0x87,  ATK,  0x00,          0,    5,   0,     0,      0,  67 ),
	CG_FRAME( 0x80,    0,   2,           0,   807, 0x80,   0x87,  ATK,  0x01, 0x31 | CSE,    5,   0,     0,      0,  13 ),
	CG_FRAME( 0x80,    0,   2,           0,   808, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   807, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   808, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   807, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   808, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   807, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   808, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x80,    0,   2,           0,   807, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME( 0x82,    1,   2,           0,   808, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   2,           0,   808, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   2,           0,   809, 0x80,   0x87,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   810, 0x80,   0x8d,  ATE,  0x00,          0,    5,   0,     0,      0,  61 ),
	CG_NEXT( pl3a_super_13 ),
};

/* Char_Effect 2: shl3a_01_mode_set (2 + sub_type) */
const Sint32 pl3a_effect_02[] = {
	CG_FRAME(    0,    1,   3,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl3a_effect_03[] = {
	CG_FRAME(    0,    0,   3,           0, 32776, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32777, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32778, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32779, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32780, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32781, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32782, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32783, 0x80,   0x00,  ATK,  0x35,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl3a_effect_04[] = {
	CG_FRAME(    0,    1,   3,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl3a_effect_05[] = {
	CG_FRAME(    0,    0,   3,           0, 32792, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32793, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32794, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32795, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32796, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32797, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32798, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32799, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl3a_effect_06[] = {
	CG_FRAME(    0,    1,   3,           0, 32800, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32801, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32802, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32803, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32804, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32805, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32806, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32807, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl3a_effect_07[] = {
	CG_FRAME(    0,    0,   3,           0, 32808, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32809, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32810, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32811, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32812, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32813, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32814, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32815, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl3a_effect_08[] = {
	CG_FRAME(    0,    1,   3,           0, 32816, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32817, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32818, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32819, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32820, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32821, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32822, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32823, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl3a_effect_09[] = {
	CG_FRAME(    0,    0,   3,           0, 32824, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32825, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32826, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32827, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32828, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32829, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32830, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32831, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl3a_effect_10[] = {
	CG_FRAME(    0,    1,   3,           0, 32832, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32833, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32834, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32835, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32836, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32837, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32838, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32839, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl3a_effect_11[] = {
	CG_FRAME(    0,    0,   3,           0, 32840, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32841, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32842, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32843, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32844, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32845, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32846, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32847, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl3a_effect_12[] = {
	CG_FRAME(    0,    1,   3,           0, 32848, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32849, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32850, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32851, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32852, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32853, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32854, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32855, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl3a_effect_13[] = {
	CG_FRAME(    0,    0,   3,           0, 32856, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32857, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32858, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32859, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32860, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32861, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32862, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32863, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl3a_effect_14[] = {
	CG_FRAME(    0,    1,   3,           0, 32864, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32865, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32866, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32867, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32868, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32869, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32870, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32871, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl3a_effect_15[] = {
	CG_FRAME(    0,    0,   3,           0, 32872, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32873, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32874, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32875, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32876, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32877, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32878, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32879, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl3a_effect_16[] = {
	CG_FRAME(    0,    1,   3,           0, 32880, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32881, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32882, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32883, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32884, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32885, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32886, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32887, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_16 ),
};

/* Char_Effect 17 */
const Sint32 pl3a_effect_17[] = {
	CG_FRAME(    0,    0,   3,           0, 32888, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32889, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32890, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32891, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32892, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32893, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32894, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32895, 0x80,   0x00,  ATK,  0x31,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_17 ),
};

/* Char_Effect 18 */
const Sint32 pl3a_effect_18[] = {
	CG_FRAME(    0,    1,   3,           0, 32896, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32897, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32898, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32899, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32900, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32901, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32902, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32903, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl3a_effect_19[] = {
	CG_FRAME(    0,    0,   3,           0, 32904, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32905, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32906, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32907, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32908, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32909, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32910, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32911, 0x80,   0x00,  ATK,  0x37,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_19 ),
};

/* Char_Effect 20 */
const Sint32 pl3a_effect_20[] = {
	CG_FRAME(    0,    1,   3,           0, 32912, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32913, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32914, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32915, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32916, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32917, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32918, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32919, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_20 ),
};

/* Char_Effect 21 */
const Sint32 pl3a_effect_21[] = {
	CG_FRAME(    0,    0,   3,           0, 32920, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32921, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32922, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32923, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32924, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32925, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32926, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32927, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_21 ),
};

/* Char_Effect 22 */
const Sint32 pl3a_effect_22[] = {
	CG_FRAME(    0,    1,   3,           0, 32928, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32929, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32930, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32931, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32932, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32933, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32934, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32935, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_22 ),
};

/* Char_Effect 23 */
const Sint32 pl3a_effect_23[] = {
	CG_FRAME(    0,    0,   3,           0, 32936, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32937, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32938, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32939, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32940, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32941, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32942, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32943, 0x80,   0x00,  ATK,  0x2f,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_23 ),
};

/* Char_Effect 24 */
const Sint32 pl3a_effect_24[] = {
	CG_FRAME(    0,    1,   3,           0, 32944, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32945, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32946, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32947, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32948, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32949, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32950, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32951, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_24 ),
};

/* Char_Effect 25 */
const Sint32 pl3a_effect_25[] = {
	CG_FRAME(    0,    0,   3,           0, 32952, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32953, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32954, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32955, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32956, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32957, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32958, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32959, 0x80,   0x00,  ATK,  0x2d,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_25 ),
};

/* Char_Effect 26 */
const Sint32 pl3a_effect_26[] = {
	CG_FRAME(    0,    1,   3,           0, 32960, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32961, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32962, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32963, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32964, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32965, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_26 ),
};

/* Char_Effect 27 */
const Sint32 pl3a_effect_27[] = {
	CG_FRAME(    0,    0,   3,           0, 32967, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32968, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32969, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32970, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32971, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_27 ),
};

/* Char_Effect 28 */
const Sint32 pl3a_effect_28[] = {
	CG_FRAME(    0,    1,   3,           0, 32972, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32973, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32974, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32975, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32976, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32977, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_28 ),
};

/* Char_Effect 29 */
const Sint32 pl3a_effect_29[] = {
	CG_FRAME(    0,    0,   3,           0, 32979, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32980, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32981, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32982, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32983, 0x80,   0x00,  ATK,  0x20,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_29 ),
};

/* Char_Effect 30 */
const Sint32 pl3a_effect_30[] = {
	CG_FRAME(    0,    1,   3,           0, 32984, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32985, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32986, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32987, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3,           0, 32988, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA, 32989, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_30 ),
};

/* Char_Effect 31 */
const Sint32 pl3a_effect_31[] = {
	CG_FRAME(    0,    0,   3,           0, 32991, 0x80,   0x00,  ATK,  0x1f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32992, 0x80,   0x00,  ATK,  0x1f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32993, 0x80,   0x00,  ATK,  0x1f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0, 32994, 0x80,   0x00,  ATK,  0x1f,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32995, 0x80,   0x00,  ATK,  0x1f,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl3a_effect_31 ),
};

/* Char_Special 7: sp_plane */
const Sint32 pl3a_special_07[] = {
	CG_FRAME(    0,    0,   8,           0,   354, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   747, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   748, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   749, 0x80,   0x0f,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   750, 0x80,   0x0e,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   449, 0x80,   0x0d,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   450, 0x80,   0x0c,  ATK,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   2,           0,   451, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  32,           0,   452, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   453, 0x80,   0x0b,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_07 ),
};

/* Char_Special 11 */
const Sint32 pl3a_special_11[] = {
	CG_FRAME(    0,    1,   6,           0,   728, 0x80,   0x00,  ATK,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    1,  12,           0,   729, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  45 ),
};

const Sint32 pl3a_special_11_loop[] = {
	CG_FRAME(    1,    0,   4,           0,   730, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    1,    0,   4,           0,   732, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   733, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    1,    0,   4,           0,   730, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    1,    0,   4,           0,   732, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   733, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl3a_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl3a_special_12[] = {
	CG_FRAME(    0,    0,   6,           0,   734, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   735, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   736, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   737, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   738, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   739, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
};

const Sint32 pl3a_special_12_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   740, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   741, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   742, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   743, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl3a_special_12_loop ),
};

/* Char_Special 13 */
const Sint32 pl3a_special_13[] = {
	CG_FRAME(    0,    0,  10,           0,   744, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,   745, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   746, 0x80,   0x00,    0,  0x2d,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl3a_special_13 ),
};

/* Char_Special 8 */
const Sint32 pl3a_special_08[] = {
	CG_FRAME(    0,    1,   6,           0,   728, 0x80,   0x00,  ATK,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,  12,           0,   729, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   1,           0,   730, 0x80,   0x60,  ATK,  0x50, 0x29 | CSE,    0,   0,     0,      0,  65 ),
};

const Sint32 pl3a_special_08_loop[] = {
	CG_FRAME(    1,    0,   4,           0,   730, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1,    0,   4,           0,   732, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   733, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1,    0,   4,           0,   730, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4,           0,   731, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    1,    0,   4,           0,   732, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   733, 0x80,   0x60,  ATK,  0x50,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl3a_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl3a_special_09[] = {
	CG_FRAME(    0,    0,   6,           0,   734, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   735, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   736, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   737, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   738, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   739, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
};

const Sint32 pl3a_special_09_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   740, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   741, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6,           0,   742, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   743, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl3a_special_09_loop ),
};

/* Char_Special 10 */
const Sint32 pl3a_special_10[] = {
	CG_FRAME(    0,    0,  10,           0,   744, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   4,           0,   745, 0x80,   0x60,    0,  0x50,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   746, 0x80,   0x00,    0,  0x2d,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl3a_special_10 ),
};

/* Char_Special 16: tackle_init */
const Sint32 pl3a_special_16[] = {
	CG_FRAME(   34,   -2,   2,           0,   887, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   33,   -2,   2,           0,   888, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2,           0,   889, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2,           0,   890, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   34,   -2,   2,           0,   891, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   33,   -2,   2,           0,   892, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2,           0,   893, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2, END_OF_DATA,   894, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_16 ),
};

/* Char_Special 3: fire_init (3 + atck_level) */
const Sint32 pl3a_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   661, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   662, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   663, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   664, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   665, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   665, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   666, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   667, 0x80,   0x00,  ATK,  0x03,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   668, 0x80,   0x00,  ATK,  0x04,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   669, 0x80,   0x00,  ATK,  0x05,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 14: fire_move (14 + atck_level) */
const Sint32 pl3a_special_14[] = {
	CG_FRAME(    1,    1,   4,           0,   670, 0x80,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   671, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   672, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   673, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   674, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  20,           0,   675, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   676, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_special_14_loop[] = {
	CG_FRAME(    0,    0,  20, END_OF_DATA,   677, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_14_loop ),
};

/* Char_Special 4 */
const Sint32 pl3a_special_04[] = {
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   679, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   680, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   681, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   682, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   682, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   683, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   684, 0x80,   0x00,  ATK,  0x03,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,  16,           0,   685, 0x80,   0x00,  ATK,  0x04,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    0,   4,           0,   686, 0x80,   0x00,  ATK,  0x05,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 15 */
const Sint32 pl3a_special_15[] = {
	CG_FRAME(    1,    1,   4,           0,   687, 0x80,   0x00,  ATK,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   688, 0x80,   0x00,  ATK,  0x00, 0x4b | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   689, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   690, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   691, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  20,           0,   692, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   693, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_special_15_loop[] = {
	CG_FRAME(    0,    0,  20, END_OF_DATA,   694, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_special_15_loop ),
};

/* Char_Effect 0 */
const Sint32 pl3a_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0,   695, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   698, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl3a_effect_00_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   699, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   700, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   701, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   702, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   703, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   704, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   705, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl3a_effect_00_loop ),
};

/* Char_Effect 32: shl3a_00_move (32 + type) */
const Sint32 pl3a_effect_32[] = {
	CG_FRAME(    0,    0,   2,           0,   700, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   706, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   707, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   708, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   709, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   710, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   711, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_32 ),
};

/* Char_Effect 1: shl3a_01_mode_lunch (1 + char_no.b.no), shl3a_02_move (1 + char_no.b.no) */
const Sint32 pl3a_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   712, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   715, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   716, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
};

const Sint32 pl3a_effect_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   717, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   718, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   719, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   720, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   721, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_NEXT( pl3a_effect_01_loop ),
};

/* Char_Effect 33 */
const Sint32 pl3a_effect_33[] = {
	CG_FRAME(    0,    0,   2,           0,   722, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   723, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   724, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   725, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   726, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   727, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_33 ),
};

/* Char_Catch 0 */
const Sint32 pl3a_catch_00[] = {
	CG_FRAME(    0,    1,  16,           0,   648, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
};

const Sint32 pl3a_catch_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_NEXT( pl3a_catch_00_loop ),
};

/* Char_Catch 1: p_catch_rise */
const Sint32 pl3a_catch_01[] = {
	CG_FRAME(    0,    0,   1,           0,   651, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,  12,           0,   652, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    1,    0,   2,           0,   653, 0x80,   0x00,    0,  0x01,       0x05,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,  14,           0,   653, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl3a_catch_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl3a_catch_01_loop ),
};

/* Char_Catch 2: catch_fall */
const Sint32 pl3a_catch_02[] = {
	CG_FRAME(    0,    0,   8,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_catch_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_catch_02_loop ),
};

/* group 27 1 */
const Sint32 pl3a_grp27_01[] = {
	EF_FRAME(    0,    1,   6,           0,   654 ),
	EF_FRAME(    0,    1,   6,           0,   655 ),
	EF_FRAME(    0,    1,   6,           0,   656 ),
	EF_FRAME(    0,    1,   6,           0,   657 ),
};

const Sint32 pl3a_grp27_01_loop[] = {
	EF_FRAME(    0,    0,   3,           0,   658 ),
	EF_FRAME(    0,    0,   3,           0,   659 ),
	EF_FRAME(    0,    0,   3, END_OF_DATA,   660 ),
	CG_NEXT( pl3a_grp27_01_loop ),
};

/* Char_Effect 43: pls3a_03_gather_turn, shl3a_01_mode_set */
const Sint32 pl3a_effect_43[] = {
	CG_FRAME(    0,    0,   4,           0,   879, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   880, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   881, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   882, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   883, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   884, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   885, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   886, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_43 ),
};

/* Char_Effect 42: shl3a_01_mode_set */
const Sint32 pl3a_effect_42[] = {
	CG_FRAME(   34,   -2,   2,           0,   887, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   33,   -2,   2,           0,   888, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2,           0,   889, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2,           0,   890, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   34,   -2,   2,           0,   891, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   33,   -2,   2,           0,   892, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2,           0,   893, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   32,   -2,   2, END_OF_DATA,   894, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_42 ),
};

/* Char_Effect 50: shl3a_01_mode_set */
const Sint32 pl3a_effect_50[] = {
	CG_FRAME(   28,   -1,   4,           0,   902, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(   28,   -1,   4, END_OF_DATA,   903, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_50 ),
};

/* Char_Effect 51: shl3a_01_mode_land_item */
const Sint32 pl3a_effect_51[] = {
	CG_FRAME(   29,   -1,   4, END_OF_DATA,   904, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_51 ),
};

/* Char_Effect 37: shl3a_01_mode_set */
const Sint32 pl3a_effect_37[] = {
	CG_FRAME(    0,    0,  16, END_OF_DATA,   899, 0x80,   0x00,  ATK,  0x00,          0,    7,   0,     0,      0,  59 ),
	CG_NEXT( pl3a_effect_37 ),
};

/* Char_Effect 36: shl3a_01_mode_set */
const Sint32 pl3a_effect_36[] = {
	CG_FRAME(    0,    0,   3,           0, 33000, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33004, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33000, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33005, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33001, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33006, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33001, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33007, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33002, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33004, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33002, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33005, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33003, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33006, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0, 33003, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 33007, 0x80,   0x00,  ATK,  0x33,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl3a_effect_36 ),
};

/* Char_Effect 45: shl3a_01_mode_set */
const Sint32 pl3a_effect_45[] = {
	CG_FRAME(    0,    0,   4,           0,   932, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   933, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   934, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_effect_45_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   935, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   936, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_45_loop ),
};

/* Char_Effect 46: shl3a_01_mode_fall */
const Sint32 pl3a_effect_46[] = {
	CG_FRAME(    0,    0,   4,           0,   937, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   938, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   939, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_effect_46_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   940, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_46_loop ),
};

/* Char_Effect 39: shl3a_01_mode_set */
const Sint32 pl3a_effect_39[] = {
	CG_FRAME(    0,    0,   6,           0,   941, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   942, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_effect_39_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   943, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   944, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_39_loop ),
};

/* Char_Effect 40: shl3a_01_mode_set */
const Sint32 pl3a_effect_40[] = {
	CG_FRAME(    0,    0,   6,           0,   945, 0x80,   0x00,    0,  0x00,       0x33,    0,   0,     0,      0,   1 ),
};

const Sint32 pl3a_effect_40_loop[] = {
	CG_FRAME(    0,    0,   6,           0,   946, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    1,   1,           0,   947, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   948, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl3a_effect_40_loop ),
};

/* Char_Effect 38: shl3a_01_mode_catch_run */
const Sint32 pl3a_effect_38[] = {
	CG_FRAME(    0,    0,   6,           0,   900, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   901, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_38 ),
};

/* Char_Effect 41: shl3a_01_mode_set */
const Sint32 pl3a_effect_41[] = {
	CG_FRAME(    0,    0,   2,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_41 ),
};

/* Char_Effect 44: shl3a_01_mode_set */
const Sint32 pl3a_effect_44[] = {
	CG_FRAME(    1,    0,   5,           0,   454, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5,           0,   455, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5,           0,   456, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5,           0,   457, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5,           0,   458, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5,           0,   459, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5,           0,   460, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,   5, END_OF_DATA,   461, 0x80,   0x0b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_44 ),
};

/* Char_Effect 47: shl3a_01_mode_set */
const Sint32 pl3a_effect_47[] = {
	CG_FRAME(    0,    0,   2,           0,   312, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   313, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   314, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   315, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   316, 0x80,   0x00,    0,  0x00,          0,    8,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_47 ),
};

/* Char_Effect 48: shl3a_01_mode_set */
const Sint32 pl3a_effect_48[] = {
	CG_FRAME(    0,    0,   3,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl3a_effect_48 ),
};

/* Char_Effect 49: shl3a_01_mode_set */
const Sint32 pl3a_effect_49[] = {
	CG_FRAME(    0,    0,   2,           0,   936, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
};

const Sint32 pl3a_effect_49_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   935, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   936, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl3a_effect_49_loop ),
};

/* Char_Effect 52: shl3a_04_carry_init */
const Sint32 pl3a_effect_52[] = {
	CG_FRAME(    0,    1,   6,           0, 32996, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

/* Char_Effect 53 */
const Sint32 pl3a_effect_53[] = {
	CG_FRAME(    0,    1,   6,           0, 32997, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

/* Char_Effect 54 */
const Sint32 pl3a_effect_54[] = {
	CG_FRAME(    0,    1,   6,           0, 32998, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  68 ),
};

/* Char_Effect 55 */
const Sint32 pl3a_effect_55[] = {
	CG_FRAME(    0,    1,   6, END_OF_DATA, 32999, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl3a_effect_52 ),
};

/* group 27 9: pls3a_02_tackle_init */
const Sint32 pl3a_grp27_09[] = {
	EF_FRAME(    0,    1,   6,           0, 32996 ),
};

/* group 27 10 */
const Sint32 pl3a_grp27_10[] = {
	EF_FRAME(    0,    1,   6,           0, 32997 ),
};

/* group 27 11 */
const Sint32 pl3a_grp27_11[] = {
	EF_FRAME(    0,    1,   6,           0, 32998 ),
};

/* group 27 12 */
const Sint32 pl3a_grp27_12[] = {
	EF_FRAME(    0,    1,   6, END_OF_DATA, 32999 ),
	CG_NEXT( pl3a_grp27_09 ),
};

/* group 27 13: pls3a_02_appear_init */
const Sint32 pl3a_grp27_13[] = {
	EF_FRAME(    0,    0, 127, END_OF_DATA,   863 ),
	CG_NEXT( pl3a_grp27_13 ),
};

/* group 27 14: pls3a_02_smoke_init */
const Sint32 pl3a_grp27_14[] = {
	EF_FRAME(    0,    0,   2,           0, 33010 ),
	EF_FRAME(    0,    0,   2,           0, 33011 ),
	EF_FRAME(    0,    0,   2,           0, 33012 ),
	EF_FRAME(    0,    0,   2,           0, 33013 ),
	EF_FRAME(    0,    0,   2,           0, 33014 ),
	EF_FRAME(    0,    0,   2,           0, 33015 ),
	EF_FRAME(    0,    0,   1,           0, 33016 ),
	EF_FRAME(    0,    0,   1,           0, 33017 ),
	EF_FRAME(    0,    0,   1,           0, 33018 ),
	EF_FRAME(    0,    0,   1,           0, 33019 ),
	EF_FRAME(    0,    0,   1,           0, 33020 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 33021 ),
	CG_NEXT( pl3a_grp27_14 ),
};

/* group 27 15: pls3a_03_gather_init */
const Sint32 pl3a_grp27_15[] = {
	EF_FRAME(    0,    0,   2,           0,   312 ),
	EF_FRAME(    0,    0,   2,           0,   313 ),
	EF_FRAME(    0,    0,   2,           0,   314 ),
	EF_FRAME(    0,    0,   2,           0,   315 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   316 ),
	CG_NEXT( pl3a_grp27_15 ),
};

/* group 27 16: pls3a_03_gather_in */
const Sint32 pl3a_grp27_16[] = {
	EF_FRAME(    0,    0,   2,           0,   777 ),
	EF_FRAME(    0,    0,   2,           0,   778 ),
};

const Sint32 pl3a_grp27_16_loop[] = {
	EF_FRAME(    1,    0,   4, END_OF_DATA,   779 ),
	CG_NEXT( pl3a_grp27_16_loop ),
};

/* group 27 17 */
const Sint32 pl3a_grp27_17[] = {
	EF_FRAME(    0,    0,   4,           0,   914 ),
	EF_FRAME(    0,    0,   4, END_OF_DATA,   915 ),
	CG_NEXT( pl3a_grp27_17 ),
};

/* group 27 18 */
const Sint32 pl3a_grp27_18[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA,   899 ),
	CG_NEXT( pl3a_grp27_18 ),
};
