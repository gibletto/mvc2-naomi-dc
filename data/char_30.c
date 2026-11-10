/* Omega Red (character 30): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_30      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_30     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl30.c) says which group
                   and number each move uses.
   pl30_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl30_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl30_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl30_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_30
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_30, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_30 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl30_stand_00[], pl30_stand_02[], pl30_stand_03[], pl30_jump_00[],
	pl30_jump_00_loop[], pl30_jump_01[], pl30_jump_01_loop[], pl30_jump_02[], pl30_jump_02_loop[],
	pl30_jump_03[], pl30_jump_04[], pl30_jump_05[], pl30_jump_06[], pl30_jump_09[], pl30_jump_07[],
	pl30_jump_10[], pl30_jump_08[], pl30_jump_11[], pl30_dash_00[], pl30_dash_01[], pl30_dash_02[],
	pl30_dash_04[], pl30_crouch_00[], pl30_crouch_01[], pl30_crouch_02[], pl30_turn_00[],
	pl30_turn_01[], pl30_guard_00[], pl30_guard_01[], pl30_guard_02[], pl30_unused_00[],
	pl30_unused_01[], pl30_unused_02[], pl30_unused_03[], pl30_unused_04[], pl30_unused_05[],
	pl30_unused_06[], pl30_unused_07[], pl30_unused_08[], pl30_unused_09[], pl30_unused_10[],
	pl30_unused_11[], pl30_unused_12[], pl30_unused_13[], pl30_unused_14[], pl30_guard_03[],
	pl30_guard_04[], pl30_guard_05[], pl30_guard_06[], pl30_unused_15[], pl30_unused_16[],
	pl30_unused_17[], pl30_unused_18[], pl30_unused_19[], pl30_unused_20[], pl30_unused_21[],
	pl30_unused_22[], pl30_unused_23[], pl30_unused_24[], pl30_unused_25[], pl30_unused_26[],
	pl30_unused_27[], pl30_unused_28[], pl30_unused_29[], pl30_guard_07[], pl30_air_guard_00[],
	pl30_air_guard_01[], pl30_air_guard_02[], pl30_stand_p_00[], pl30_extra_16[], pl30_stand_p_01[],
	pl30_stand_p_02[], pl30_stand_p_03[], pl30_stand_k_00[], pl30_stand_k_01[], pl30_stand_k_02[],
	pl30_crouch_p_00[], pl30_crouch_p_01[], pl30_crouch_p_02[], pl30_crouch_k_00[],
	pl30_crouch_k_01[], pl30_crouch_k_02[], pl30_crouch_k_02_loop[], pl30_extra_00[],
	pl30_extra_01[], pl30_jump_p_00[], pl30_jump_p_01[], pl30_jump_p_02[], pl30_jump_k_00[],
	pl30_jump_k_01[], pl30_jump_k_02[], pl30_damage_00[], pl30_damage_01[], pl30_damage_02[],
	pl30_damage_03[], pl30_damage_04[], pl30_damage_05[], pl30_damage_06[], pl30_damage_07[],
	pl30_damage_08[], pl30_damage_09[], pl30_damage_09_loop[], pl30_damage_10[], pl30_damage_11[],
	pl30_damage_12[], pl30_damage_13[], pl30_damage_16[], pl30_damage_17[], pl30_damage_18[],
	pl30_damage_19[], pl30_damage_20[], pl30_damage_21[], pl30_damage_22[], pl30_damage_23[],
	pl30_damage_24[], pl30_damage_25[], pl30_damage_26[], pl30_damage_27[], pl30_damage_28[],
	pl30_damage_29[], pl30_damage_30[], pl30_damage_31[], pl30_damage_32[], pl30_dizzy_00[],
	pl30_catch_00[], pl30_catch_01[], pl30_catch_02[], pl30_catch_03[], pl30_get_up_00[],
	pl30_get_up_01[], pl30_get_up_02[], pl30_appear_00[], pl30_appear_01[], pl30_appear_01_loop[],
	pl30_appear_02[], pl30_appear_02_loop[], pl30_appear_03[], pl30_appear_04[], pl30_appear_05[],
	pl30_win_00[], pl30_win_00_loop[], pl30_win_01[], pl30_win_01_loop[], pl30_win_02[],
	pl30_win_03[], pl30_win_04[], pl30_win_05[], pl30_win_06[], pl30_win_06_loop[], pl30_win_07[],
	pl30_win_07_loop[], pl30_win_08[], pl30_win_08_loop[], pl30_super_00[], pl30_effect_00[],
	pl30_super_01[], pl30_super_02[], pl30_super_02_loop[], pl30_super_03[], pl30_super_04[],
	pl30_super_04_loop[], pl30_super_05[], pl30_effect_01[], pl30_effect_02[], pl30_effect_03[],
	pl30_effect_04[], pl30_effect_05[], pl30_effect_06[], pl30_effect_07[], pl30_effect_08[],
	pl30_effect_09[], pl30_effect_10[], pl30_effect_11[], pl30_effect_12[], pl30_effect_13[],
	pl30_effect_13_loop[], pl30_effect_14[], pl30_change_00[], pl30_change_00_loop[],
	pl30_change_01[], pl30_change_02[], pl30_change_03[], pl30_change_03_loop[], pl30_change_04[],
	pl30_extra_02[], pl30_extra_03[], pl30_extra_04[], pl30_extra_04_loop[], pl30_extra_05[],
	pl30_extra_05_loop[], pl30_extra_06[], pl30_extra_06_loop[], pl30_extra_07[],
	pl30_extra_07_loop[], pl30_extra_08[], pl30_extra_08_loop[], pl30_extra_09[],
	pl30_extra_09_loop[], pl30_extra_10[], pl30_extra_10_loop[], pl30_extra_11[],
	pl30_extra_11_loop[], pl30_extra_12[], pl30_extra_12_loop[], pl30_extra_13[],
	pl30_extra_13_loop[], pl30_extra_14[], pl30_extra_14_loop[], pl30_extra_15[],
	pl30_extra_15_loop[], pl30_special_04[], pl30_special_04_loop[], pl30_special_05[],
	pl30_special_02[], pl30_special_02_loop[], pl30_special_03[], pl30_special_00[],
	pl30_special_00_loop[], pl30_special_01[], pl30_special_06[], pl30_special_06_loop[],
	pl30_special_07[], pl30_special_08[], pl30_special_08_loop[], pl30_special_09[],
	pl30_special_10[], pl30_special_10_loop[], pl30_special_11[], pl30_special_12[],
	pl30_special_12_loop[], pl30_special_13[], pl30_special_14[], pl30_special_14_loop[],
	pl30_special_15[], pl30_special_16[], pl30_special_16_loop[], pl30_special_17[],
	pl30_special_18[], pl30_special_18_loop[], pl30_special_19[], pl30_special_20[],
	pl30_special_20_loop[], pl30_special_21[], pl30_special_22[], pl30_special_22_loop[],
	pl30_special_23[], pl30_special_24[], pl30_special_24_loop[], pl30_special_25[],
	pl30_special_26[], pl30_special_26_loop[], pl30_special_27[], pl30_special_28[],
	pl30_special_28_loop[], pl30_special_29[], pl30_special_30[], pl30_special_31[],
	pl30_special_32[], pl30_special_33[], pl30_special_34[], pl30_special_35[], pl30_special_36[],
	pl30_special_37[], pl30_special_54[], pl30_special_55[], pl30_special_38[], pl30_special_39[],
	pl30_special_40[], pl30_special_41[], pl30_special_42[], pl30_special_43[], pl30_special_44[],
	pl30_special_45[], pl30_special_46[], pl30_special_47[], pl30_special_48[], pl30_special_49[],
	pl30_special_50[], pl30_special_51[], pl30_special_52[], pl30_special_53[], pl30_special_56[],
	pl30_special_57[], pl30_special_57_loop[], pl30_special_58[], pl30_special_59[],
	pl30_special_59_loop[], pl30_unused_30[], pl30_special_60[], pl30_unused_31[], pl30_special_61[],
	pl30_special_61_loop[], pl30_special_62[], pl30_special_63[], pl30_special_64[],
	pl30_special_65[], pl30_special_66[], pl30_special_67[], pl30_extra_17[], pl30_caught_00[],
	pl30_caught_01[], pl30_caught_02[], pl30_caught_03[], pl30_caught_04[], pl30_caught_05[],
	pl30_caught_06[], pl30_caught_07[], pl30_caught_08[], pl30_caught_09[], pl30_caught_10[],
	pl30_caught_11[], pl30_caught_12[], pl30_caught_13[], pl30_caught_14[], pl30_caught_15[],
	pl30_caught_16[], pl30_caught_17[], pl30_caught_18[], pl30_caught_19[], pl30_caught_20[],
	pl30_grp27_00[], pl30_grp27_01[], pl30_grp27_02[], pl30_grp27_03[], pl30_grp27_04[],
	pl30_grp27_05[], pl30_grp27_06[], pl30_grp27_07[], pl30_grp27_08[], pl30_grp27_09[],
	pl30_grp27_10[], pl30_grp27_11[], pl30_unused_32[];
extern const Sint32 *const pl30_stand_tbl[], *const pl30_jump_tbl[], *const pl30_dash_tbl[],
	*const pl30_crouch_tbl[], *const pl30_turn_tbl[], *const pl30_guard_tbl[],
	*const pl30_air_guard_tbl[], *const pl30_stand_p_tbl[], *const pl30_stand_k_tbl[],
	*const pl30_crouch_p_tbl[], *const pl30_crouch_k_tbl[], *const pl30_jump_p_tbl[],
	*const pl30_jump_k_tbl[], *const pl30_damage_tbl[], *const pl30_dizzy_tbl[],
	*const pl30_catch_tbl[], *const pl30_caught_tbl[], *const pl30_get_up_tbl[],
	*const pl30_appear_tbl[], *const pl30_win_tbl[], *const pl30_extra_tbl[],
	*const pl30_special_tbl[], *const pl30_super_tbl[], *const pl30_effect_tbl[],
	*const pl30_change_tbl[], *const pl30_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_30[16] = {
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
const Sint32 *const *const char_tbl_30[] = {
	pl30_stand_tbl, pl30_jump_tbl, pl30_dash_tbl, pl30_crouch_tbl, pl30_turn_tbl, pl30_guard_tbl,
	pl30_air_guard_tbl, pl30_stand_p_tbl, pl30_stand_k_tbl, pl30_crouch_p_tbl, pl30_crouch_k_tbl, pl30_jump_p_tbl,
	pl30_jump_k_tbl, pl30_damage_tbl, pl30_dizzy_tbl, pl30_catch_tbl, pl30_caught_tbl, pl30_get_up_tbl,
	pl30_appear_tbl, pl30_win_tbl, pl30_extra_tbl, pl30_special_tbl, pl30_super_tbl, pl30_effect_tbl,
	pl30_change_tbl, pl30_change_tbl, pl30_change_tbl, pl30_grp27_tbl, 0,
};

const Sint32 *const pl30_stand_tbl[] = {
	pl30_stand_00, pl30_stand_00, pl30_stand_02, pl30_stand_03, pl30_stand_00, pl30_stand_00,
	pl30_stand_02, pl30_stand_03, 0,
};

const Sint32 *const pl30_jump_tbl[] = {
	pl30_jump_00, pl30_jump_01, pl30_jump_02, pl30_jump_03, pl30_jump_04, pl30_jump_05,
	pl30_jump_06, pl30_jump_07, pl30_jump_08, pl30_jump_09, pl30_jump_10, pl30_jump_11,
	0,
};

const Sint32 *const pl30_dash_tbl[] = {
	pl30_dash_00, pl30_dash_01, pl30_dash_02, pl30_dash_02, pl30_dash_04, 0,
};

const Sint32 *const pl30_crouch_tbl[] = {
	pl30_crouch_00, pl30_crouch_01, pl30_crouch_02, 0,
};

const Sint32 *const pl30_turn_tbl[] = {
	pl30_turn_00, pl30_turn_01, 0,
};

const Sint32 *const pl30_guard_tbl[] = {
	pl30_guard_00, pl30_guard_01, pl30_guard_02, pl30_guard_03, pl30_guard_04, pl30_guard_05,
	pl30_guard_06, pl30_guard_07, 0,
};

const Sint32 *const pl30_air_guard_tbl[] = {
	pl30_air_guard_00, pl30_air_guard_01, pl30_air_guard_02, 0,
};

const Sint32 *const pl30_stand_p_tbl[] = {
	pl30_stand_p_00, pl30_stand_p_01, pl30_stand_p_02, pl30_stand_p_03, 0,
};

const Sint32 *const pl30_stand_k_tbl[] = {
	pl30_stand_k_00, pl30_stand_k_01, pl30_stand_k_02, 0,
};

const Sint32 *const pl30_crouch_p_tbl[] = {
	pl30_crouch_p_00, pl30_crouch_p_01, pl30_crouch_p_02, 0,
};

const Sint32 *const pl30_crouch_k_tbl[] = {
	pl30_crouch_k_00, pl30_crouch_k_01, pl30_crouch_k_02, 0,
};

const Sint32 *const pl30_jump_p_tbl[] = {
	pl30_jump_p_00, pl30_jump_p_01, pl30_jump_p_02, pl30_jump_p_00, pl30_jump_p_01, pl30_jump_p_02,
	0,
};

const Sint32 *const pl30_jump_k_tbl[] = {
	pl30_jump_k_00, pl30_jump_k_01, pl30_jump_k_02, pl30_jump_k_00, pl30_jump_k_01, pl30_jump_k_02,
	0,
};

const Sint32 *const pl30_damage_tbl[] = {
	pl30_damage_00, pl30_damage_01, pl30_damage_02, pl30_damage_03, pl30_damage_04, pl30_damage_05,
	pl30_damage_06, pl30_damage_07, pl30_damage_08, pl30_damage_09, pl30_damage_10, pl30_damage_11,
	pl30_damage_12, pl30_damage_13, pl30_damage_13, pl30_damage_13, pl30_damage_16, pl30_damage_17,
	pl30_damage_18, pl30_damage_19, pl30_damage_20, pl30_damage_21, pl30_damage_22, pl30_damage_23,
	pl30_damage_24, pl30_damage_25, pl30_damage_26, pl30_damage_27, pl30_damage_28, pl30_damage_29,
	pl30_damage_30, pl30_damage_31, pl30_damage_32, pl30_damage_32, pl30_damage_13, 0,
};

const Sint32 *const pl30_dizzy_tbl[] = {
	pl30_dizzy_00, 0,
};

const Sint32 *const pl30_catch_tbl[] = {
	pl30_catch_00, pl30_catch_01, pl30_catch_02, pl30_catch_03, 0,
};

const Sint32 *const pl30_caught_tbl[] = {
	pl30_caught_00, pl30_caught_01, pl30_caught_02, pl30_caught_03, pl30_caught_04, pl30_caught_05,
	pl30_caught_06, pl30_caught_07, pl30_caught_08, pl30_caught_09, pl30_caught_10, pl30_caught_11,
	pl30_caught_12, pl30_caught_13, pl30_caught_14, pl30_caught_15, pl30_caught_16, pl30_caught_17,
	pl30_caught_18, pl30_caught_19, pl30_caught_20, pl30_caught_20, 0,
};

const Sint32 *const pl30_get_up_tbl[] = {
	pl30_get_up_00, pl30_get_up_01, pl30_get_up_02, pl30_get_up_00, 0,
};

const Sint32 *const pl30_appear_tbl[] = {
	pl30_appear_00, pl30_appear_01, pl30_appear_02, pl30_appear_03, pl30_appear_04, pl30_appear_05,
	0,
};

const Sint32 *const pl30_win_tbl[] = {
	pl30_win_00, pl30_win_01, pl30_win_02, pl30_win_03, pl30_win_04, pl30_win_05,
	pl30_win_06, pl30_win_07, pl30_win_08, 0,
};

const Sint32 *const pl30_extra_tbl[] = {
	pl30_extra_00, pl30_extra_01, pl30_extra_02, pl30_extra_03, pl30_extra_04, pl30_extra_05,
	pl30_extra_06, pl30_extra_07, pl30_extra_08, pl30_extra_09, pl30_extra_10, pl30_extra_11,
	pl30_extra_12, pl30_extra_13, pl30_extra_14, pl30_extra_15, pl30_extra_16, pl30_extra_17,
	0,
};

const Sint32 *const pl30_special_tbl[] = {
	pl30_special_00, pl30_special_01, pl30_special_02, pl30_special_03, pl30_special_04, pl30_special_05,
	pl30_special_06, pl30_special_07, pl30_special_08, pl30_special_09, pl30_special_10, pl30_special_11,
	pl30_special_12, pl30_special_13, pl30_special_14, pl30_special_15, pl30_special_16, pl30_special_17,
	pl30_special_18, pl30_special_19, pl30_special_20, pl30_special_21, pl30_special_22, pl30_special_23,
	pl30_special_24, pl30_special_25, pl30_special_26, pl30_special_27, pl30_special_28, pl30_special_29,
	pl30_special_30, pl30_special_31, pl30_special_32, pl30_special_33, pl30_special_34, pl30_special_35,
	pl30_special_36, pl30_special_37, pl30_special_38, pl30_special_39, pl30_special_40, pl30_special_41,
	pl30_special_42, pl30_special_43, pl30_special_44, pl30_special_45, pl30_special_46, pl30_special_47,
	pl30_special_48, pl30_special_49, pl30_special_50, pl30_special_51, pl30_special_52, pl30_special_53,
	pl30_special_54, pl30_special_55, pl30_special_56, pl30_special_57, pl30_special_58, pl30_special_59,
	pl30_special_60, pl30_special_61, pl30_special_62, pl30_special_63, pl30_special_64, pl30_special_65,
	pl30_special_66, pl30_special_67, 0,
};

const Sint32 *const pl30_super_tbl[] = {
	pl30_super_00, pl30_super_01, pl30_super_02, pl30_super_03, pl30_super_04, pl30_super_05,
	0,
};

const Sint32 *const pl30_effect_tbl[] = {
	pl30_effect_00, pl30_effect_01, pl30_effect_02, pl30_effect_03, pl30_effect_04, pl30_effect_05,
	pl30_effect_06, pl30_effect_07, pl30_effect_08, pl30_effect_09, pl30_effect_10, pl30_effect_11,
	pl30_effect_12, pl30_effect_13, pl30_effect_14, 0,
};

const Sint32 *const pl30_change_tbl[] = {
	pl30_change_00, pl30_change_01, pl30_change_02, pl30_change_03, pl30_change_04, 0,
};

const Sint32 *const pl30_grp27_tbl[] = {
	pl30_grp27_00, pl30_grp27_01, pl30_grp27_02, pl30_grp27_03, pl30_grp27_04, pl30_grp27_05,
	pl30_grp27_06, pl30_grp27_07, pl30_grp27_08, pl30_grp27_09, pl30_grp27_10, pl30_grp27_11,
	0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl30_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl30_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,   173, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   174, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   175, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   176, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   177, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   178, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   180, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   181, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   182, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   183, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   184, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   185, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   186, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl30_stand_03[] = {
	CG_FRAME(    0,    0,   6,           0,   187, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   188, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   190, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   191, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   192, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   193, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   194, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   195, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   196, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   197, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   198, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   199, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   200, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl30_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   222, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl30_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   233, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   234, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl30_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   222, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl30_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   241, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl30_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   222, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl30_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   248, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_02_loop ),
};

/* Char_Jump 3 */
const Sint32 pl30_jump_03[] = {
	CG_FRAME(    0,    0,   4,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl30_jump_04[] = {
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl30_jump_05[] = {
	CG_FRAME(    0,    0,   4,           0,   249, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_jump_05 ),
};

/* Char_Jump 6 */
const Sint32 pl30_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   222, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   224, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl30_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   355, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl30_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   222, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl30_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   355, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl30_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   222, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl30_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   355, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl30_dash_00[] = {
	CG_FRAME(    0,    1,   4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   205, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   4,           0,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   208, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   209, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl30_dash_00 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl30_dash_01[] = {
	CG_FRAME(    0,    1,   4,           0,   214, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   215, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   217, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl30_dash_01 ),
};

/* Char_Dash 2
   Char_Dash 3: noki_jump */
const Sint32 pl30_dash_02[] = {
	CG_FRAME(    0,    0,   4,           0,   218, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   221, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl30_dash_02 ),
};

/* Char_Dash 4: air_dash_init, fly_start_init, fly_start2_init */
const Sint32 pl30_dash_04[] = {
	CG_FRAME(    0,    1,   4,           0,   487, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,   4,           0,   488, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   489, 0x80,   0x00,             0x16,  0x00,       0x07,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,   12,   4,           0,   487, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    8,   4,           0,   488, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    4,   4, END_OF_DATA,   489, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl30_dash_04 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl30_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl30_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   172, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl30_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   221, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl30_turn_00[] = {
	CG_FRAME(    0,    0,   4,           0,   259, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   262, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl30_turn_01[] = {
	CG_FRAME(    0,    0,   4,           0,   263, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl30_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl30_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   269, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl30_guard_02[] = {
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_guard_02 ),
};

const Sint32 pl30_unused_00[] = {	/* nothing reaches it */
	0x00020080,
};

const Sint32 pl30_unused_01[] = {
	0x0000010e,
};

const Sint32 pl30_unused_02[] = {
	0x00000080,
};

const Sint32 pl30_unused_03[] = {
	0x00000000,
};

const Sint32 pl30_unused_04[] = {
	0x00010000,
};

const Sint32 pl30_unused_05[] = {
	0x00020080,
};

const Sint32 pl30_unused_06[] = {
	0x0000010b,
};

const Sint32 pl30_unused_07[] = {
	0x00000080,
};

const Sint32 pl30_unused_08[] = {
	0x00000000,
};

const Sint32 pl30_unused_09[] = {
	0x00010000,
};

const Sint32 pl30_unused_10[] = {
	0x00020080,
};

const Sint32 pl30_unused_11[] = {
	0x0000010c,
};

const Sint32 pl30_unused_12[] = {
	0x00000080,
};

const Sint32 pl30_unused_13[] = {
	0x00000000,
};

const Sint32 pl30_unused_14[] = {
	0x00010000,
};

/* Char_Guard 3 */
const Sint32 pl30_guard_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   269, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl30_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   271, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl30_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl30_guard_06[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_guard_06 ),
};

const Sint32 pl30_unused_15[] = {	/* nothing reaches it */
	0x00020080,
};

const Sint32 pl30_unused_16[] = {
	0x0000010f,
};

const Sint32 pl30_unused_17[] = {
	0x00000080,
};

const Sint32 pl30_unused_18[] = {
	0x00000000,
};

const Sint32 pl30_unused_19[] = {
	0x00020000,
};

const Sint32 pl30_unused_20[] = {
	0x00020080,
};

const Sint32 pl30_unused_21[] = {
	0x00000110,
};

const Sint32 pl30_unused_22[] = {
	0x00000080,
};

const Sint32 pl30_unused_23[] = {
	0x00000000,
};

const Sint32 pl30_unused_24[] = {
	0x00020000,
};

const Sint32 pl30_unused_25[] = {
	0x00020080,
};

const Sint32 pl30_unused_26[] = {
	0x00000111,
};

const Sint32 pl30_unused_27[] = {
	0x00000080,
};

const Sint32 pl30_unused_28[] = {
	0x00000000,
};

const Sint32 pl30_unused_29[] = {
	0x00020000,
};

/* Char_Guard 7 */
const Sint32 pl30_guard_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl30_air_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl30_air_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   277, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl30_air_guard_02[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   282, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_air_guard_02 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl30_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   367, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0x80,   0x00,       ATK | 0x16,  0x00,       0x07,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   5,           0,   367, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,   -1,   4, END_OF_DATA,   367, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl30_stand_p_00 ),
};

/* Char_Extra 16 */
const Sint32 pl30_extra_16[] = {
	CG_FRAME(    0,    0,   3,           0,   367, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0x80,   0x00,       ATK | 0x16,  0x00,       0x07,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2,           0,   368, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   5,           0,   367, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   367, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl30_extra_16 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl30_stand_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   369, 0x80,   0x04,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x04,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x80,    0,   2,           0,   370, 0x80,   0x04,              ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x81,    0,   4,           0,   371, 0x80,   0x04,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,  10,           0,   371, 0x80,   0x04,              ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   369, 0x80,   0x04,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl30_stand_p_02[] = {
	CG_FRAME(    0,    0,   5,           0,   372, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   373, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x65,       ATK | 0x0a,  0x00,       0x21,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   375, 0x80,   0x65,       ATK | 0x0d,  0x00,       0x23,    0,   0,     0,      0, 110 ),
	CG_FRAME(   -8,    0,   2,           0,   376, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(  -13,    0,   4,           0,   377, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(  -18,    0,   4,           0,   378, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_stand_p_02 ),
};

/* Char_Stand_P 3 */
const Sint32 pl30_stand_p_03[] = {
	CG_FRAME( 0x80,    0,   4,           0,   386, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x82,    0,   2,           0,   387, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_stand_p_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl30_stand_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   388, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   389, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   388, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl30_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl30_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   391, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   392, 0x80,   0x88,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x80,   0x88,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   8,           0,   393, 0x80,   0x88,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   391, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl30_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   394, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   395, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   397, 0x80,   0x00,       ATK | 0x1d,  0x00, 0x16 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   398, 0x80,   0xe1,          ATK | 5,  0x00,       0x04,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,  10,           0,   399, 0x80,   0xe1,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   8,           0,   400, 0x80,   0xe1,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   721, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl30_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   3,           0,   402, 0x80,   0x00,       ATK | 0x16,  0x00,       0x07,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   401, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl30_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl30_crouch_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0xaa,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   2,           0,   404, 0x80,   0xaa,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   1,           0,   405, 0x80,   0xaa,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME( 0x80,    0,   2,           0,   406, 0x80,   0x6d,       ATK | 0x0a,  0x00,       0x21,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x80,    0,   2,           0,   407, 0x80,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME( 0x82,    0,   3,           0,   408, 0x80,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   409, 0x80,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6,           0,   410, 0x80,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   411, 0x80,   0x6d,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl30_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl30_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x80,   0xa5,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0xa5,       ATK | 0x0b,  0x00,       0x22,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0xa5,              ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0xa5,       ATK | 0x0b,  0x00,       0x22,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0xa5,              ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0xa5,              ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl30_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl30_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   420, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x8c,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   420, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl30_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl30_crouch_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   422, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   423, 0x70,   0x86,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   424, 0x80,   0x86,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3,           0,   425, 0x80,   0x86,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   3,           0,   426, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   3,           0,   427, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl30_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl30_crouch_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   429, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   430, 0x80,   0x7c,          ATK | 5,  0x00,       0x04,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    1,   3,           0,   431, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   432, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl30_crouch_k_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   434, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   435, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl30_crouch_k_02_loop ),
};

/* Char_Extra 0: shl30_02_burrow_move */
const Sint32 pl30_extra_00[] = {
	CG_FRAME(    0,    0,   4,           0,   436, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,       ATK | 0x0a,  0x00,       0x21,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   4,           0,   440, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x82,    0,   4,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x82,    0,   4,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x82,    0,   4, END_OF_DATA,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_00 ),
};

/* Char_Extra 1: crouch_kick_move */
const Sint32 pl30_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x7c,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   275, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl30_extra_01 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl30_jump_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   445, 0x80,   0x00,       ATK | 0x0b,  0x00,       0x22,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   446, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   447, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   448, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   446, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   447, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   448, 0x80,   0x00,       ATK | 0x0b,  0x00,       0x22,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   3,           0,   444, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl30_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl30_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   450, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   4,           0,   451, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0, 107 ),
	CG_FRAME( 0x80,    0,   4,           0,   452, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x81,    0,   4,           0,   453, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   454, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl30_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl30_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   457, 0x80,   0x00,          ATK | 5,  0x00,       0x04,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,       ATK | 0x0b,  0x01,       0x22,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   457, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,       ATK | 0x0b,  0x01,       0x22,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   455, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   456, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   232, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl30_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl30_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   462, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl30_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   6,           0,   464, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl30_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl30_jump_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   4,           0,   467, 0x80,   0x00,          ATK | 5,  0x00,       0x04,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x80,    0,   4,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,   4,           0,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,   4,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,   4,           0,   469, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x80,    0,   4,           0,   468, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x82,    0,   4,           0,   469, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_jump_k_02 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl30_damage_00[] = {
	CG_FRAME(    0,    0,   5,           0,   741, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   742, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl30_damage_01[] = {
	CG_FRAME(    0,    0,   8,           0,   743, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   744, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   742, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl30_damage_02[] = {
	CG_FRAME(    0,    0,   6,           0,   745, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   746, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   747, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   748, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl30_damage_03[] = {
	CG_FRAME(    0,    0,   7,           0,   749, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl30_damage_04[] = {
	CG_FRAME(    0,    0,   6,           0,   749, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   750, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   751, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl30_damage_05[] = {
	CG_FRAME(    0,    0,   5,           0,   752, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   753, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   754, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   755, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   756, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   757, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   758, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl30_damage_06[] = {
	CG_FRAME(    0,    0,  11, END_OF_DATA,   759, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl30_damage_07[] = {
	CG_FRAME(    0,    0,   8,           0,   760, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   6,           0,   761, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   762, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl30_damage_08[] = {
	CG_FRAME(    0,    0,   3,           0,   763, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   764, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   765, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   766, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   767, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3,           0,   761, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   762, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl30_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl30_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   776, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   777, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   778, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl30_damage_09_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   790, 0x80,   0xef,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl30_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl30_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl30_damage_11[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl30_damage_12[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl30_damage_13[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl30_damage_13 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl30_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl30_damage_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl30_damage_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl30_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl30_damage_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl30_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl30_damage_22[] = {
	CG_FRAME(    0,    0,   4,           0,   782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl30_damage_23[] = {
	CG_FRAME(    0,    0,   4,           0,   783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl30_damage_24[] = {
	CG_FRAME(    0,    0,   4,           0,   784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl30_damage_25[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl30_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   786, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   787, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   788, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   789, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   790, 0x80,   0xef,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl30_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl30_damage_27[] = {
	CG_FRAME(    0,    0,   4,           0,   791, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl30_damage_28[] = {
	CG_FRAME(    0,    0,   4,           0,   794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   796, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   797, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   798, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl30_damage_29[] = {
	CG_FRAME(    0,    0,   4,           0,   800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   802, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   803, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   804, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   805, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl30_damage_30[] = {
	CG_FRAME(    0,    0,   4,           0,   791, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_30 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl30_damage_31[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   790, 0x80,   0xef,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl30_damage_31 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl30_damage_32[] = {
	CG_FRAME(    0,    0,   4,           0,   819, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   820, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl30_damage_32 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl30_dizzy_00[] = {
	CG_FRAME(    0,    0,   6,           0,   812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   813, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   815, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_dizzy_00 ),
};

/* Char_Catch 0 */
const Sint32 pl30_catch_00[] = {
	CG_FRAME(    0,    0,   1,           0, 32806, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32810, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32822, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32830, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32806, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32830, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32822, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32810, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_NEXT( pl30_catch_00 ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl30_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,   471, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   4,           0,   473, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    1,   4,           0,   474, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0x80,   0x00,       ATK | 0x15,  0x00,       0x06,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,   2,           0,   475, 0x80,   0x00,              ATE,  0x27,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,   4,           0,   476, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   477, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   478, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   212, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   480, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_catch_01 ),
};

/* Char_Catch 2: nml_p_set */
const Sint32 pl30_catch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,   471, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   481, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_FRAME(    0,    1,   4,           0,   304, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0019,   0 ),
	CG_FRAME(    0,    0,   4,           0,   482, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0, 0x001a,   0 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x86,          ATK | 6,  0x00,       0x05,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x86,              ATK,  0x28,          0,    0,   0,     0, 0x001b,   0 ),
	CG_FRAME(    0,    0,   9,           0,   484, 0x80,   0x86,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   9,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   212, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_catch_02 ),
};

/* Char_Catch 3: jump_set */
const Sint32 pl30_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x001c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x001d,   0 ),
	CG_FRAME(    0,    0,   4,           0,   494, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x001e,   0 ),
	CG_FRAME(    0,    0,   4,           0,   495, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x001f,   0 ),
	CG_FRAME(    0,    1,   4,           0,   496, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0020,   0 ),
	CG_FRAME(    0,   -1,   1,           0,   497, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0021,   0 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x86,              ATK,  0x00,          0,    0,   0,     0, 0x0022,   0 ),
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x86,          ATK | 6,  0x29,       0x05,    0,   0,     0, 0x0022,   0 ),
	CG_FRAME(    0,    0,  10,           0,   484, 0x80,   0x86,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   9,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   212, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_catch_03 ),
};

/* Char_Get_Up 0: dm_dizzy_wait
   Char_Get_Up 3 */
const Sint32 pl30_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   806, 0x80,   0xef,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   807, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   808, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   809, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   810, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl30_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   331, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   332, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   333, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl30_get_up_02[] = {
	CG_FRAME(    0,    0,   6,           0,   331, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_get_up_02 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl30_appear_00[] = {
	CG_FRAME(    0,    0,  16,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,  16,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,  16,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,  16,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   -1,  16,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   316, 0x80,   0x00,                8,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   317, 0x80,   0x00,                5,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   318, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   323, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_appear_00 ),
};

/* Char_Appear 1: shl30_02_appear_init (1 + sub_type) */
const Sint32 pl30_appear_01[] = {
	CG_FRAME(    0,    0,   4,           0,   867, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   893, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   894, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_appear_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   871, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   872, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   873, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   874, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   875, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   876, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   877, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   878, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   879, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   880, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   881, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   882, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   883, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   884, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   885, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   886, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   887, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   888, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   889, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   890, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   891, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   892, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_appear_01_loop ),
};

/* Char_Appear 2 */
const Sint32 pl30_appear_02[] = {
	CG_FRAME(    0,    0,   4,           0,   870, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   917, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   918, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_appear_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   895, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   896, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   897, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   898, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   899, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   900, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   901, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   902, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   903, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   904, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   905, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   906, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   907, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   908, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   909, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   910, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   911, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   912, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   913, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   914, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   915, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   916, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_appear_02_loop ),
};

/* Char_Appear 3: shl30_02_appear_move (3 + sub_type) */
const Sint32 pl30_appear_03[] = {
	CG_FRAME(    0,    0,   4,           0,   865, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   866, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32790, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_appear_03 ),
};

/* Char_Appear 4 */
const Sint32 pl30_appear_04[] = {
	CG_FRAME(    0,    0,   4,           0,   868, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   869, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32791, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32792, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32796, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32797, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32798, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32799, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32801, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32802, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32803, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32804, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32805, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_appear_04 ),
};

/* Char_Appear 5: shl30_02_piece_init */
const Sint32 pl30_appear_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32769, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_appear_05 ),
};

/* Char_Win 0 */
const Sint32 pl30_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,   212, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   283, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   284, 0x80,   0x14,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   285, 0x80,   0x14,             0x0b,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   8,           0,   286, 0x80,   0x14,             0x0b,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   286, 0x80,   0x14,             0x0b,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   287, 0x80,   0x92,             0x0b,  0x00,       0x22,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_win_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x92,             0x0b,  0x00,       0x22,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   289, 0x80,   0x92,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_00_loop ),
};

/* Char_Win 1: win_ko1_next */
const Sint32 pl30_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x92,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x92,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x92,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   293, 0x80,   0x92,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl30_win_02[] = {
	CG_FRAME(    0,    0,   1,           0,   821, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   822, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   823, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   824, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_02 ),
};

/* Char_Win 3 */
const Sint32 pl30_win_03[] = {
	CG_FRAME(    0,    0,   1,           0,   825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   827, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   828, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_03 ),
};

/* Char_Win 4 */
const Sint32 pl30_win_04[] = {
	CG_FRAME(    0,    0,   1,           0,   829, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   830, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   831, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   832, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_04 ),
};

/* Char_Win 5 */
const Sint32 pl30_win_05[] = {
	CG_FRAME(    0,    0,   1,           0,   833, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   834, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   835, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   836, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_05 ),
};

/* Char_Win 6 */
const Sint32 pl30_win_06[] = {
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x80,   0x00,                6,  0x00,       0x05,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_win_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   302, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_06_loop ),
};

/* Char_Win 7: win_time_init */
const Sint32 pl30_win_07[] = {
	CG_FRAME(    0,    0,   1,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x00,             0x19,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_win_07_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_07_loop ),
};

/* Char_Win 8: lose_init */
const Sint32 pl30_win_08[] = {
	CG_FRAME(    0,    0,   1,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   311, 0x80,   0x00,             0x1a,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   314, 0x80,   0x00,                0,  0x01,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_win_08_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_win_08_loop ),
};

/* Char_Super 0: destroyer_init (Omega Destroyer (hyper)) */
const Sint32 pl30_super_00[] = {
	CG_FRAME(    0,    0,   6,           0,   498, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xff00,   1 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x14,       ATK | 0x18,  0x00,       0x19,    0,   0,     0, 0xff00, 102 ),
	CG_FRAME(    0,    0,  16,           0,   500, 0x80,   0x92,              ATK,  0x00,          0,    0,   0,     0, 0xff00, 102 ),
	CG_FRAME(    0,    1,   8,           0,   501, 0x80,   0x92,              ATK,  0x00,          0,    0,   0,     0, 0xff00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x92,       ATK | 0x11,  0x03,       0x1a,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x92,              ATK,  0x03,          0,    0,   0,     0, 0x0100, 102 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x92,       ATK | 0x0b,  0x03,       0x22,    0,   0,     0, 0x0200, 102 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x92,              ATK,  0x03,          0,    0,   0,     0, 0x0300, 102 ),
	CG_FRAME(    0,    0,   6,           0,   501, 0x80,   0x92,       ATK | 0x0b,  0x03,       0x22,    0,   0,     0, 0x0200, 102 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x92,              ATK,  0x03,          0,    0,   0,     0, 0x0300, 102 ),
	CG_FRAME(    0,    0,   3,           0,   501, 0x80,   0x92,       ATK | 0x0b,  0x03,       0x22,    0,   0,     0, 0x0400, 102 ),
	CG_FRAME(    0,    0,   3,           0,   500, 0x80,   0x92,              ATK,  0x02,          0,    0,   0,     0, 0x0500, 102 ),
	CG_FRAME(    0,    0,   3,           0,   499, 0x80,   0x92,       ATK | 0x0b,  0x01,       0x22,    0,   0,     0, 0x0600, 102 ),
	CG_FRAME(    0,    0,   3,           0,   502, 0x80,   0x92,              ATK,  0x04,          0,    0,   0,     0, 0x0700, 102 ),
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x92,       ATK | 0x0b,  0x05,       0x22,    0,   0,     0, 0x0800, 102 ),
	CG_FRAME(    0,    0,   3,           0,   502, 0x80,   0x92,              ATK,  0x04,          0,    0,   0,     0, 0x0900, 102 ),
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x92,       ATK | 0x0b,  0x05,       0x22,    0,   0,     0, 0x0a00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   502, 0x80,   0x92,              ATK,  0x04,          0,    0,   0,     0, 0x0b00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   502, 0x80,   0x92,       ATK | 0x0b,  0x04,       0x22,    0,   0,     0, 0x0c00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x92,              ATK,  0x05,          0,    0,   0,     0, 0x0d00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   502, 0x80,   0x92,       ATK | 0x0b,  0x04,       0x22,    0,   0,     0, 0x0900, 102 ),
	CG_FRAME(    0,    0,   3,           0,   503, 0x80,   0x92,              ATK,  0x05,          0,    0,   0,     0, 0x0a00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   502, 0x80,   0x92,       ATK | 0x0b,  0x04,       0x22,    0,   0,     0, 0x0b00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   504, 0x80,   0x92,              ATK,  0x06,          0,    0,   0,     0, 0x0e00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   505, 0x80,   0x92,       ATK | 0x0b,  0x07,       0x22,    0,   0,     0, 0x0f00, 102 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0x92,              ATK,  0x08,          0,    0,   0,     0, 0x1000, 102 ),
	CG_FRAME(    0,    0,   3,           0,   507, 0x80,   0x92,       ATK | 0x0b,  0x09,       0x22,    0,   0,     0, 0x1100, 102 ),
	CG_FRAME(    0,    0,   3,           0,   506, 0x80,   0x92,              ATK,  0x08,          0,    0,   0,     0, 0x1200, 102 ),
	CG_FRAME(    0,    0,   3,           0,   507, 0x80,   0x92,       ATK | 0x0b,  0x09,       0x22,    0,   0,     0, 0x1300, 102 ),
	CG_FRAME(    0,    0,   3,           0,   508, 0x80,   0x92,              ATK,  0x0a,          0,    0,   0,     0, 0x1400, 102 ),
	CG_FRAME(    0,    0,   3,           0,   507, 0x80,   0x92,       ATK | 0x0b,  0x09,       0x22,    0,   0,     0, 0x1500, 102 ),
	CG_FRAME(    0,    0,   3,           0,   508, 0x80,   0x92,              ATK,  0x0a,          0,    0,   0,     0, 0x1600, 102 ),
	CG_FRAME(    0,    0,   3,           0,   507, 0x80,   0x92,       ATK | 0x0b,  0x09,       0x22,    0,   0,     0, 0x1700, 102 ),
	CG_FRAME(    0,    0,   3,           0,   508, 0x80,   0x92,              ATE,  0x0a,          0,    0,   0,     0, 0x1800, 102 ),
	CG_FRAME(    0,    0,   3,           0,   508, 0x80,   0x92,       ATE | 0x0b,  0x0a,       0x22,    0,   0,     0, 0x1900, 102 ),
	CG_FRAME(    0,    0,   3,           0,   508, 0x80,   0x92,              ATE,  0x0a,          0,    0,   0,     0, 0x1a00, 102 ),
	CG_FRAME(    0,    0,   6,           0,   507, 0x80,   0x92,              ATE,  0x00,          0,    0,   0,     0, 0xff00, 102 ),
	CG_FRAME(    0,    0,   8,           0,   506, 0x80,   0x92,              ATE,  0x00,          0,    0,   0,     0, 0xff00, 102 ),
	CG_FRAME(    0,    0,  10,           0,   505, 0x80,   0x92,              ATE,  0x00,          0,    0,   0,     0, 0xff00, 102 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   498, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0xff00, 102 ),
	CG_NEXT( pl30_super_00 ),
};

/* Char_Effect 0: shl30_00_move */
const Sint32 pl30_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0, 32838, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   1,           0, 32839, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   1,           0, 32840, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   1,           0, 32841, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0, 32842, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   1,           0, 32843, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   1,           0, 32844, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   1,           0, 32845, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   1,           0, 32846, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   1,           0, 32847, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   1,           0, 32848, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   1,           0, 32849, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   1,           0, 32850, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   1,           0, 32851, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0, 32852, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   1,           0, 32853, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0, 32854, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   1,           0, 32855, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   1,           0, 32856, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1,           0, 32857, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   1,           0, 32858, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   1,           0, 32859, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0, 32860, 0x80,   0x00,              ATK,  0x31,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   1,           0, 32861, 0x80,   0x00,              ATK,  0x32,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   1,           0, 32862, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32863, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32864, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_00 ),
};

/* Char_Super 1: jdestroyer_dash (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_super_01[] = {
	CG_FRAME(    0,    0,   2,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0x80,   0x00,          ATK | 4,  0x3e,       0x03,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   464, 0x80,   0x00,          ATK | 4,  0x3f,       0x03,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,       ATK | 0x16,  0x3e,       0x07,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   3,           0,   462, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   2,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME( 0x80,    0,   2,           0,   452, 0x80,   0x00,          ATK | 4,  0x3f,       0x03,    0,   0,     0,      0,  61 ),
	CG_FRAME( 0x81,    0,   2,           0,   453, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   464, 0x80,   0x00,              ATK,  0x3f,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   467, 0x80,   0x00,       ATK | 0x16,  0x3e,       0x07,    0,   0,     0,      0,  70 ),
	CG_FRAME( 0x80,    0,   2,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,       ATK | 0x0b,  0x41,       0x22,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   459, 0x80,   0x00,          ATK | 4,  0x40,       0x03,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   460, 0x80,   0x00,              ATK,  0x41,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   457, 0x80,   0x00,          ATK | 4,  0x40,       0x03,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   458, 0x80,   0x00,       ATK | 0x0b,  0x41,       0x22,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   459, 0x80,   0x00,          ATE | 4,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   460, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl30_super_01 ),
};

/* Char_Super 2: jdestroyer_dash_init (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_super_02[] = {
	CG_FRAME(    0,    0,   5,           0,   487, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   5,           0,   488, 0x80,   0x00,       ATK | 0x10,  0x00,       0x15,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   6,           0,   489, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   6,           0,   490, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,   1,           0,   490, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl30_super_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   490, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   491, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl30_super_02_loop ),
};

/* Char_Super 3: jdestroyer_dash (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   487, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   4,           0,   488, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   489, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_NEXT( pl30_super_03 ),
};

/* Char_Super 4: jdestroyer_hit (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_super_04[] = {
	CG_FRAME(    0,   64,   5,           0,   634, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   65,   5,           0,   635, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   66,   5,           0,   636, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   67,   5,           0,   637, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   68,   5,           0,   638, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   69,   7,           0,   639, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   70,   4,           0,   640, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_super_04_loop[] = {
	CG_FRAME(    0,   70,   3, END_OF_DATA,   641, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_super_04_loop ),
};

/* Char_Super 5: jdestroyer_end_set (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_super_05[] = {
	CG_FRAME(    0,    0,   3,           0,   641, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   641, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_super_05 ),
};

/* Char_Effect 1 */
const Sint32 pl30_effect_01[] = {
	CG_FRAME(    0,    0,   1,           0,  1064, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1068, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1072, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1076, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_01 ),
};

/* Char_Effect 2 */
const Sint32 pl30_effect_02[] = {
	CG_FRAME(    0,    0,   1,           0,  1065, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1069, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1073, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1077, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_02 ),
};

/* Char_Effect 3 */
const Sint32 pl30_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,  1066, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1070, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1074, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1078, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_03 ),
};

/* Char_Effect 4 */
const Sint32 pl30_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0,  1067, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1071, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,  1075, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,  1079, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl30_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32873, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32877, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32881, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32885, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl30_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0, 32874, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32878, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32882, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32886, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl30_effect_07[] = {
	CG_FRAME(    0,    0,   1,           0, 32875, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32879, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32883, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32887, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl30_effect_08[] = {
	CG_FRAME(    0,    0,   1,           0, 32876, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32880, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32884, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32888, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl30_effect_09[] = {
	CG_FRAME(    0,    0,   1,           0, 32889, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32893, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32897, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32901, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl30_effect_10[] = {
	CG_FRAME(    0,    0,   1,           0, 32890, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32894, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32898, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32902, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl30_effect_11[] = {
	CG_FRAME(    0,    0,   1,           0, 32891, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32895, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32899, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32903, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl30_effect_12[] = {
	CG_FRAME(    0,    0,   1,           0, 32892, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32896, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32900, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32904, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl30_effect_13[] = {
	CG_FRAME(    0,    0,   2,           0,   850, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   851, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_effect_13_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   837, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   838, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   839, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   840, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   841, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   842, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   843, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   844, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   845, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   846, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   848, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   849, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_13_loop ),
};

/* Char_Effect 14 */
const Sint32 pl30_effect_14[] = {
	CG_FRAME(    0,    0,   2,           0,   852, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   853, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   854, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   855, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   856, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   857, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   858, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   860, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   861, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   862, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   863, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   864, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_effect_14 ),
};

/* Char_Change 0: act_change_tag_init
   group 25 0
   Char_Fly 0 */
const Sint32 pl30_change_00[] = {
	CG_FRAME(    0,    0,   2,           0,   356, 0x80,   0x00,             0x1c,  0x00,       0x06,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   360, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   361, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   362, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   363, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_change_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   364, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   366, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_change_00_loop ),
};

/* Char_Change 1: act_change_vcounter_init
   group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl30_change_01[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_change_01 ),
};

/* Char_Change 2: act_change_out_bound
   group 25 2
   Char_Fly 2 */
const Sint32 pl30_change_02[] = {
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call
   group 25 3
   Char_Fly 3 */
const Sint32 pl30_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   311, 0x80,   0x00,             0x1f,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_change_03_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end
   group 25 4
   Char_Fly 4 */
const Sint32 pl30_change_04[] = {
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                6,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   221, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_change_04 ),
};

/* Char_Extra 2: t_kick_init, fly_start5 */
const Sint32 pl30_extra_02[] = {
	CG_FRAME(    0,    0,   4,           0,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl30_extra_02 ),
};

/* Char_Extra 3: t_kick_move, fly_start5 */
const Sint32 pl30_extra_03[] = {
	CG_FRAME(    0,    0,   3,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   220, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,             0x1b,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   323, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_extra_03 ),
};

/* Char_Extra 4 */
const Sint32 pl30_extra_04[] = {
	CG_FRAME(    0,    0,   4,           0, 32905, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_04_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32906, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_04_loop ),
};

/* Char_Extra 5 */
const Sint32 pl30_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0, 32907, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_05_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32908, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_05_loop ),
};

/* Char_Extra 6 */
const Sint32 pl30_extra_06[] = {
	CG_FRAME(    0,    0,   4,           0, 32909, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_06_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32910, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_06_loop ),
};

/* Char_Extra 7 */
const Sint32 pl30_extra_07[] = {
	CG_FRAME(    0,    0,   4,           0, 32911, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_07_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32912, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_07_loop ),
};

/* Char_Extra 8 */
const Sint32 pl30_extra_08[] = {
	CG_FRAME(    0,    0,   4,           0, 32913, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_08_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32914, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_08_loop ),
};

/* Char_Extra 9 */
const Sint32 pl30_extra_09[] = {
	CG_FRAME(    0,    0,   4,           0, 32915, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_09_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32916, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_09_loop ),
};

/* Char_Extra 10 */
const Sint32 pl30_extra_10[] = {
	CG_FRAME(    0,    0,   4,           0, 32917, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_10_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32918, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_10_loop ),
};

/* Char_Extra 11 */
const Sint32 pl30_extra_11[] = {
	CG_FRAME(    0,    0,   4,           0, 32919, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_11_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32920, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_11_loop ),
};

/* Char_Extra 12 */
const Sint32 pl30_extra_12[] = {
	CG_FRAME(    0,    0,   4,           0, 32921, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_12_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32922, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_12_loop ),
};

/* Char_Extra 13 */
const Sint32 pl30_extra_13[] = {
	CG_FRAME(    0,    0,   4,           0, 32923, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_13_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32924, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_13_loop ),
};

/* Char_Extra 14 */
const Sint32 pl30_extra_14[] = {
	CG_FRAME(    0,    0,   4,           0, 32925, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_14_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32926, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_14_loop ),
};

/* Char_Extra 15 */
const Sint32 pl30_extra_15[] = {
	CG_FRAME(    0,    0,   4,           0, 32927, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl30_extra_15_loop[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA, 32928, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_extra_15_loop ),
};

/* Char_Special 4 */
const Sint32 pl30_special_04[] = {
	CG_FRAME(    0,    0,   6,           0,   519, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   511, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   526, 0x80,   0x6c,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
};

const Sint32 pl30_special_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   527, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl30_special_04_loop ),
};

/* Char_Special 5 */
const Sint32 pl30_special_05[] = {
	CG_FRAME(    0,    0,   7,           0,   528, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   7,           0,   529, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   7,           0,   518, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   519, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_05 ),
};

/* Char_Special 2 */
const Sint32 pl30_special_02[] = {
	CG_FRAME(    0,    0,   6,           0,   519, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   511, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   520, 0x80,   0x6c,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    1,   4,           0,   521, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
};

const Sint32 pl30_special_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   520, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   521, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl30_special_02_loop ),
};

/* Char_Special 3 */
const Sint32 pl30_special_03[] = {
	CG_FRAME(    0,    0,   7,           0,   522, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   7,           0,   523, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   7,           0,   518, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   519, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_03 ),
};

/* Char_Special 0 */
const Sint32 pl30_special_00[] = {
	CG_FRAME(    0,    0,   6,           0,   519, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   511, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   514, 0x80,   0x6c,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   4,           0,   515, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl30_special_00_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   515, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl30_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl30_special_01[] = {
	CG_FRAME(    0,    0,   7,           0,   516, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   7,           0,   517, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   7,           0,   518, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   519, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_01 ),
};

/* Char_Special 6 */
const Sint32 pl30_special_06[] = {
	CG_FRAME(    0,    0,   6,           0,   519, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   511, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_06_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   530, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   531, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_06_loop ),
};

/* Char_Special 7 */
const Sint32 pl30_special_07[] = {
	CG_FRAME(    0,    0,   7,           0,   532, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   533, 0x80,   0x6c,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   518, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   519, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_07 ),
};

/* Char_Special 8 */
const Sint32 pl30_special_08[] = {
	CG_FRAME(    0,    0,   5,           0,   550, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   5,           0,   551, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,   5,           0,   552, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    3,   5,           0,   553, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    4,   5,           0,   554, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    5,   5,           0,   555, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    6,   4,           0,   556, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    6,   4,           0,   557, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    6,   4,           0,   559, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_08_loop[] = {
	CG_FRAME(    0,    6,  16, END_OF_DATA,   560, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0, 0x0600,   1 ),
	CG_NEXT( pl30_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl30_special_09[] = {
	CG_FRAME(    0,    0,   4,           0,   561, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   562, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl30_special_10[] = {
	CG_FRAME(    0,    8,   5,           0,   563, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    9,   5,           0,   564, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   10,   5,           0,   565, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   11,   5,           0,   566, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   12,   5,           0,   567, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   13,   5,           0,   568, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   14,   4,           0,   569, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   14,   4,           0,   570, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   14,   4,           0,   572, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_10_loop[] = {
	CG_FRAME(    0,   14,  16, END_OF_DATA,   573, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_10_loop ),
};

/* Char_Special 11 */
const Sint32 pl30_special_11[] = {
	CG_FRAME(    0,    0,   4,           0,   574, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   575, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_11 ),
};

/* Char_Special 12 */
const Sint32 pl30_special_12[] = {
	CG_FRAME(    0,   16,   5,           0,   576, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   17,   5,           0,   577, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   18,   5,           0,   578, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   19,   5,           0,   579, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   20,   5,           0,   580, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   21,   5,           0,   581, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   22,   4,           0,   582, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   22,   4,           0,   583, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   22,   4,           0,   585, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_12_loop[] = {
	CG_FRAME(    0,   22,  16, END_OF_DATA,   586, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_12_loop ),
};

/* Char_Special 13 */
const Sint32 pl30_special_13[] = {
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   588, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl30_special_14[] = {
	CG_FRAME(    0,   24,   5,           0,   589, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   25,   5,           0,   590, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   26,   5,           0,   591, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   27,   5,           0,   592, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   28,   5,           0,   593, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   29,   5,           0,   594, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,   4,           0,   595, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,   4,           0,   596, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,   4,           0,   598, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_14_loop[] = {
	CG_FRAME(    0,   30,  16, END_OF_DATA,   599, 0x80,   0x76,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_14_loop ),
};

/* Char_Special 15 */
const Sint32 pl30_special_15[] = {
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   601, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl30_special_16[] = {
	CG_FRAME(    0,   72,   7,           0,   642, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   73,   7,           0,   643, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   74,   7,           0,   644, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   75,   7,           0,   645, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   76,   4,           0,   646, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   76,   4,           0,   647, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   76,   4,           0,   648, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_16_loop[] = {
	CG_FRAME(    0,   76,  16, END_OF_DATA,   649, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_16_loop ),
};

/* Char_Special 17 */
const Sint32 pl30_special_17[] = {
	CG_FRAME(    0,    0,   4,           0,   650, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   651, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_17 ),
};

/* Char_Special 18 */
const Sint32 pl30_special_18[] = {
	CG_FRAME(    0,   88,   5,           0,   652, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   89,   5,           0,   653, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   90,   5,           0,   654, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   91,   5,           0,   655, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   92,   5,           0,   656, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   93,   4,           0,   657, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   93,   4,           0,   658, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   93,   4,           0,   659, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_18_loop[] = {
	CG_FRAME(    0,   93,   5, END_OF_DATA,   660, 0x80,   0x6c,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_18_loop ),
};

/* Char_Special 19 */
const Sint32 pl30_special_19[] = {
	CG_FRAME(    0,    0,   3,           0,   661, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   662, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_19 ),
};

/* Char_Special 20 */
const Sint32 pl30_special_20[] = {
	CG_FRAME(    0,    0,   5,           0,   534, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   537, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   538, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
};

const Sint32 pl30_special_20_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   537, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   538, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl30_special_20_loop ),
};

/* Char_Special 21 */
const Sint32 pl30_special_21[] = {
	CG_FRAME(    0,    0,   6,           0,   537, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   538, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_21 ),
};

/* Char_Special 22 */
const Sint32 pl30_special_22[] = {
	CG_FRAME(    0,    0,   5,           0,   534, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   535, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   536, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   539, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   540, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
};

const Sint32 pl30_special_22_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   539, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   540, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl30_special_22_loop ),
};

/* Char_Special 23 */
const Sint32 pl30_special_23[] = {
	CG_FRAME(    0,    0,   6,           0,   539, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   540, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_23 ),
};

/* Char_Special 24: jdestroyer_init (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_special_24[] = {
	CG_FRAME(    0,    0,   5,           0,   534, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   541, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   542, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   543, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4,           0,   544, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl30_special_24_loop[] = {
	CG_FRAME(    0,    1,   4,           0,   543, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   544, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl30_special_24_loop ),
};

/* Char_Special 25: jdestroyer_miss (Omega Destroyer in the air (hyper)) */
const Sint32 pl30_special_25[] = {
	CG_FRAME(    0,    0,   6,           0,   543, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   544, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_25 ),
};

/* Char_Special 26 */
const Sint32 pl30_special_26[] = {
	CG_FRAME(    0,    0,   5,           0,   534, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   541, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   542, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   545, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   4,           0,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
};

const Sint32 pl30_special_26_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   545, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   546, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl30_special_26_loop ),
};

/* Char_Special 27 */
const Sint32 pl30_special_27[] = {
	CG_FRAME(    0,    0,   6,           0,   545, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   546, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_27 ),
};

/* Char_Special 28 */
const Sint32 pl30_special_28[] = {
	CG_FRAME(    0,    0,   5,           0,   534, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   541, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   547, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   548, 0x80,   0x00,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   4,           0,   549, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
};

const Sint32 pl30_special_28_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   548, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   549, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_NEXT( pl30_special_28_loop ),
};

/* Char_Special 29 */
const Sint32 pl30_special_29[] = {
	CG_FRAME(    0,    0,   6,           0,   548, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   549, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl30_special_30[] = {
	CG_FRAME(    0,   32,   5,           0,   602, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   33,   5,           0,   603, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   34,   5,           0,   604, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   35,   5,           0,   605, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   36,   5,           0,   606, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   37,   5,           0,   607, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   38,   5,           0,   608, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 31 */
const Sint32 pl30_special_31[] = {
	CG_FRAME(    0,   38,   3, END_OF_DATA,   609, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_31 ),
};

/* Char_Special 32 */
const Sint32 pl30_special_32[] = {
	CG_FRAME(    0,   40,   5,           0,   610, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   41,   5,           0,   611, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   42,   5,           0,   612, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   43,   5,           0,   613, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   44,   5,           0,   614, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   45,   5,           0,   615, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   46,   5,           0,   616, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 33 */
const Sint32 pl30_special_33[] = {
	CG_FRAME(    0,   46,   3, END_OF_DATA,   617, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_33 ),
};

/* Char_Special 34 */
const Sint32 pl30_special_34[] = {
	CG_FRAME(    0,   48,   5,           0,   618, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   49,   5,           0,   619, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   50,   5,           0,   620, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   51,   5,           0,   621, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   52,   5,           0,   622, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   53,   5,           0,   623, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   54,   5,           0,   624, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 35 */
const Sint32 pl30_special_35[] = {
	CG_FRAME(    0,   54,   3, END_OF_DATA,   625, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_35 ),
};

/* Char_Special 36 */
const Sint32 pl30_special_36[] = {
	CG_FRAME(    0,   56,   5,           0,   626, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   57,   5,           0,   627, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   58,   5,           0,   628, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   59,   5,           0,   629, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   60,   5,           0,   630, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   61,   5,           0,   631, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   62,   5,           0,   632, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 37 */
const Sint32 pl30_special_37[] = {
	CG_FRAME(    0,   62,   3, END_OF_DATA,   633, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_37 ),
};

/* Char_Special 54 */
const Sint32 pl30_special_54[] = {
	CG_FRAME(    0,   64,   5,           0,   634, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   65,   5,           0,   635, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   66,   5,           0,   636, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   67,   5,           0,   637, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   68,   5,           0,   638, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   69,   5,           0,   639, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   70,   5,           0,   640, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

/* Char_Special 55 */
const Sint32 pl30_special_55[] = {
	CG_FRAME(    0,   70,   3, END_OF_DATA,   641, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl30_special_55 ),
};

/* Char_Special 38 */
const Sint32 pl30_special_38[] = {
	CG_FRAME(    0,   80,   7,           0,   663, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   81,   7,           0,   664, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   82,   7,           0,   665, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   83,   7,           0,   666, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   84,   5,           0,   667, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 39 */
const Sint32 pl30_special_39[] = {
	CG_FRAME(    0,   84,   3, END_OF_DATA,   668, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_39 ),
};

/* Char_Special 40 */
const Sint32 pl30_special_40[] = {
	CG_FRAME(    0,   96,   5,           0,   669, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   97,   5,           0,   670, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   98,   5,           0,   671, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   99,   5,           0,   672, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,  100,   5,           0,   673, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,  101,   5,           0,   674, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Special 41 */
const Sint32 pl30_special_41[] = {
	CG_FRAME(    0,  101,   3, END_OF_DATA,   675, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_41 ),
};

/* Char_Special 42 */
const Sint32 pl30_special_42[] = {
	CG_FRAME(    0,    0,   2,           0,   712, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   714, 0x80,   0x00,       ATK | 0x12,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   715, 0x80,   0x00,          ATK | 9,  0x00,       0x20,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0, 0x80,   3,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0, 0x80,   3,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0, 0x80,   3,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0, 0x80,   3,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0, 0x80,   3,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   718, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0, 0x80,   3,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   716, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl30_special_42 ),
};

/* Char_Special 43 */
const Sint32 pl30_special_43[] = {
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   698, 0x80,   0x00,       ATK | 0x12,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   699, 0x80,   0x00,          ATK | 9,  0x00,       0x20,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   699, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   3,           0,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0, 0x80,   3,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0, 0x80,   3,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0, 0x80,   3,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0, 0x80,   3,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0, 0x80,   3,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3,           0,   702, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0, 0x80,   3,           0,   701, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   700, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl30_special_43 ),
};

/* Char_Special 44 */
const Sint32 pl30_special_44[] = {
	CG_FRAME(    0,    0,   2,           0,   676, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   677, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   678, 0x80,   0x00,       ATK | 0x12,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   6,           0,   679, 0x80,   0x00,          ATK | 9,  0x00,       0x20,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   679, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    1,   3,           0,   680, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x80,   3,           0,   681, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   680, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x80,   3,           0,   681, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   680, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x80,   3,           0,   681, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   680, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x80,   3,           0,   681, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   680, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x80,   3,           0,   681, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3,           0,   682, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0, 0x80,   3,           0,   681, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   680, 0x80,   0xcc,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl30_special_44 ),
};

/* Char_Special 45 */
const Sint32 pl30_special_45[] = {
	CG_FRAME(    0,    0,   6,           0,   723, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   724, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_45 ),
};

/* Char_Special 46 */
const Sint32 pl30_special_46[] = {
	CG_FRAME(    0,    0,   6,           0,   707, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   708, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_46 ),
};

/* Char_Special 47 */
const Sint32 pl30_special_47[] = {
	CG_FRAME(    0,    0,   4,           0,   688, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   689, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   690, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   691, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   692, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   693, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_47 ),
};

/* Char_Special 48 */
const Sint32 pl30_special_48[] = {
	CG_FRAME(    0,    0,   4,           0,   719, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   720, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl30_special_48 ),
};

/* Char_Special 49 */
const Sint32 pl30_special_49[] = {
	CG_FRAME(    0,    0,   4,           0,   703, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   704, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl30_special_49 ),
};

/* Char_Special 50 */
const Sint32 pl30_special_50[] = {
	CG_FRAME(    0,    0,   4,           0,   683, 0x80,   0xcc,              ATE,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   684, 0x80,   0xcc,              ATE,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl30_special_50 ),
};

/* Char_Special 51 */
const Sint32 pl30_special_51[] = {
	CG_FRAME(    0,    0,   4,           0,   727, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   721, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   722, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_51 ),
};

/* Char_Special 52 */
const Sint32 pl30_special_52[] = {
	CG_FRAME(    0,    0,   4,           0,   711, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   705, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   706, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_52 ),
};

/* Char_Special 53 */
const Sint32 pl30_special_53[] = {
	CG_FRAME(    0,    0,   4,           0,   685, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  10,           0,   686, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   687, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_53 ),
};

/* Char_Special 56: sp_chouhatsu (the taunt) */
const Sint32 pl30_special_56[] = {
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                6,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   509, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   510, 0x80,   0x6b,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   221, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_56 ),
};

/* Char_Special 57 */
const Sint32 pl30_special_57[] = {
	CG_FRAME(    0,    0,   6,           0,   519, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   511, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   511, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   524, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   526, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
};

const Sint32 pl30_special_57_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   526, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   527, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl30_special_57_loop ),
};

/* Char_Special 58 */
const Sint32 pl30_special_58[] = {
	CG_FRAME(    0,    0,   7,           0,   528, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   7,           0,   529, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   7,           0,   518, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7, END_OF_DATA,   519, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_58 ),
};

/* Char_Special 59 */
const Sint32 pl30_special_59[] = {
	CG_FRAME(    0,   88,   5,           0,   652, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   89,   5,           0,   653, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   90,   5,           0,   654, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   91,   5,           0,   655, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   92,   5,           0,   656, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   93,   4,           0,   657, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   93,   4,           0,   658, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   93,   4,           0,   659, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl30_special_59_loop[] = {
	CG_FRAME(    0,   93,   5, END_OF_DATA,   660, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_59_loop ),
};

const Sint32 pl30_unused_30[] = {	/* nothing reaches it */
	CG_FRAME(    0,   24,   5,           0,   589, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   25,   5,           0,   590, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   26,   5,           0,   591, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   27,   5,           0,   592, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   28,   5,           0,   593, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   29,   5,           0,   594, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,   4,           0,   595, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,   4,           0,   596, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,   4,           0,   598, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   30,  16, END_OF_DATA,   599, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_59_loop ),
};

/* Char_Special 60 */
const Sint32 pl30_special_60[] = {
	CG_FRAME(    0,    0,   3,           0,   661, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   662, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_60 ),
};

const Sint32 pl30_unused_31[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   601, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_special_60 ),
};

/* Char_Special 61: sp_burrow (Burrowing Coil (gamma assist)) */
const Sint32 pl30_special_61[] = {
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   429, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   430, 0x80,   0x7c,          ATK | 5,  0x00,       0x04,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    1,   3,           0,   431, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   432, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl30_special_61_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   434, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   435, 0x80,   0x7c,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl30_special_61_loop ),
};

/* Char_Special 62: sp_burrow (Burrowing Coil (gamma assist)) */
const Sint32 pl30_special_62[] = {
	CG_FRAME(    0,    0,   4,           0,   429, 0x80,   0x7c,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   275, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl30_special_62 ),
};

/* Char_Special 63: snapback_init (the snapback) */
const Sint32 pl30_special_63[] = {
	CG_FRAME(    0,    0,   5,           0,   372, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   1,           0,   373, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x65,       ATK | 0x0a,  0x00,       0x21,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   375, 0x80,   0x65,       ATK | 0x0d,  0x00,       0x23,    0,   0,     0,      0, 110 ),
	CG_FRAME(   -8,    0,   2,           0,   376, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(  -13,    0,   4,           0,   377, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(  -18,    0,   4,           0,   378, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_special_63 ),
};

/* Char_Special 64 */
const Sint32 pl30_special_64[] = {
	CG_FRAME( 0x80,    0,   4,           0,   386, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x82,    0,   2,           0,   387, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_special_64 ),
};

/* Char_Special 65: sp_counter_a (the alpha type's Variable Counter) */
const Sint32 pl30_special_65[] = {
	CG_FRAME(    0,    0,   5,           0,   372, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,   373, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0x80,   0x65,       ATK | 0x0a,  0x00,       0x21,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   2,           0,   375, 0x80,   0x65,       ATK | 0x0d,  0x00,       0x23,    0,   0,     0,      0, 110 ),
	CG_FRAME(   -8,    0,   2,           0,   376, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(  -13,    0,   4,           0,   377, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(  -18,    0,   4,           0,   378, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   379, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x65,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_special_65 ),
};

/* Char_Special 66 */
const Sint32 pl30_special_66[] = {
	CG_FRAME( 0x80,    0,   4,           0,   386, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x82,    0,   2,           0,   387, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   380, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   381, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   382, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   2,           0,   383, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x65,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   6,           0,   385, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   220, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl30_special_66 ),
};

/* Char_Special 67: sp_counter_g (the gamma type's Variable Counter) */
const Sint32 pl30_special_67[] = {
	CG_FRAME(    0,    0,   2,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   415, 0x80,   0xa5,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0xa5,       ATK | 0x0b,  0x00,       0x22,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0xa5,              ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0xa5,       ATK | 0x0b,  0x00,       0x22,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0xa5,              ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   416, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   417, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0xa5,              ATK,  0x01,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0xa5,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl30_special_67 ),
};

/* Char_Extra 17: pl30_upper */
const Sint32 pl30_extra_17[] = {
	CG_FRAME(    0,    0,   6,           0,   369, 0x80,   0x04,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   370, 0x80,   0x04,          ATK | 4,  0x00,       0x03,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x80,    0,   2,           0,   370, 0x80,   0x04,              ATK,  0x00,          0,    0,   0,     0,      0, 108 ),
	CG_FRAME( 0x81,    0,   4,           0,   371, 0x80,   0x04,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,  10,           0,   371, 0x80,   0x04,              ATE,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   369, 0x80,   0x04,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl30_extra_17 ),
};

/* Char_Caught 0 */
const Sint32 pl30_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   741, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl30_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   744, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl30_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   746, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl30_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   749, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl30_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   750, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl30_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   752, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl30_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   759, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl30_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl30_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   760, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl30_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl30_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   763, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl30_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl30_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl30_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl30_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl30_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl30_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl30_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl30_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl30_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl30_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl30_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   794, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl30_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   800, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl30_caught_19 ),
};

/* Char_Caught 20
   Char_Caught 21 */
const Sint32 pl30_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   790, 0x80,   0xef,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl30_caught_20 ),
};

/* group 27 0 */
const Sint32 pl30_grp27_00[] = {
	EF_FRAME(    0,    0,   1,           0,   942 ),
	EF_FRAME(    0,    0,   1,           0,   943 ),
	EF_FRAME(    0,    0,   1,           0,   944 ),
	EF_FRAME(    0,    0,   1,           0,   945 ),
	EF_FRAME(    0,    0,   1,           0,   946 ),
	EF_FRAME(    0,    0,   1,           0,   947 ),
	EF_FRAME(    0,    0,   1,           0,   948 ),
	EF_FRAME(    0,    0,   1,           0,   949 ),
	EF_FRAME(    0,    0,   1,           0,   950 ),
	EF_FRAME(    0,    0,   1,           0,   951 ),
	EF_FRAME(    0,    0,   1,           0,   952 ),
	EF_FRAME(    0,    0,   1,           0,   953 ),
	EF_FRAME(    0,    0,   1,           0,   954 ),
	EF_FRAME(    0,    0,   1,           0,   955 ),
	EF_FRAME(    0,    0,   1,           0,   956 ),
	EF_FRAME(    0,    0,   1,           0,   957 ),
	EF_FRAME(    0,    0,   1,           0,   958 ),
	EF_FRAME(    0,    0,   1,           0,   959 ),
	EF_FRAME(    0,    0,   1,           0,   960 ),
	EF_FRAME(    0,    0,   1,           0,   961 ),
	EF_FRAME(    0,    0,   1,           0,   962 ),
	EF_FRAME(    0,    0,   1,           0,   963 ),
	EF_FRAME(    0,    0,   1,           0,   964 ),
	EF_FRAME(    0,    0,   1,           0,   965 ),
	EF_FRAME(    0,    0,   1,           0,   966 ),
	EF_FRAME(    0,    0,   1,           0,   967 ),
	EF_FRAME(    0,    0,   1,           0,   968 ),
	EF_FRAME(    0,    0,   1,           0,   969 ),
	EF_FRAME(    0,    0,   1,           0,   970 ),
	EF_FRAME(    0,    0,   1,           0,   971 ),
	EF_FRAME(    0,    0,   1,           0,   972 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   973 ),
	CG_NEXT( pl30_grp27_00 ),
};

/* group 27 1: shl30_03_head_init */
const Sint32 pl30_grp27_01[] = {
	EF_FRAME(    0,    0,   1,           0, 32806 ),
	EF_FRAME(    0,    0,   1,           0, 32807 ),
	EF_FRAME(    0,    0,   1,           0, 32808 ),
	EF_FRAME(    0,    0,   1,           0, 32809 ),
	EF_FRAME(    0,    0,   1,           0, 32810 ),
	EF_FRAME(    0,    0,   1,           0, 32811 ),
	EF_FRAME(    0,    0,   1,           0, 32812 ),
	EF_FRAME(    0,    0,   1,           0, 32813 ),
	EF_FRAME(    0,    0,   1,           0, 32814 ),
	EF_FRAME(    0,    0,   1,           0, 32815 ),
	EF_FRAME(    0,    0,   1,           0, 32816 ),
	EF_FRAME(    0,    0,   1,           0, 32817 ),
	EF_FRAME(    0,    0,   1,           0, 32818 ),
	EF_FRAME(    0,    0,   1,           0, 32819 ),
	EF_FRAME(    0,    0,   1,           0, 32820 ),
	EF_FRAME(    0,    0,   1,           0, 32821 ),
	EF_FRAME(    0,    0,   1,           0, 32822 ),
	EF_FRAME(    0,    0,   1,           0, 32823 ),
	EF_FRAME(    0,    0,   1,           0, 32824 ),
	EF_FRAME(    0,    0,   1,           0, 32825 ),
	EF_FRAME(    0,    0,   1,           0, 32826 ),
	EF_FRAME(    0,    0,   1,           0, 32827 ),
	EF_FRAME(    0,    0,   1,           0, 32828 ),
	EF_FRAME(    0,    0,   1,           0, 32829 ),
	EF_FRAME(    0,    0,   1,           0, 32830 ),
	EF_FRAME(    0,    0,   1,           0, 32831 ),
	EF_FRAME(    0,    0,   1,           0, 32832 ),
	EF_FRAME(    0,    0,   1,           0, 32833 ),
	EF_FRAME(    0,    0,   1,           0, 32834 ),
	EF_FRAME(    0,    0,   1,           0, 32835 ),
	EF_FRAME(    0,    0,   1,           0, 32836 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32837 ),
	CG_NEXT( pl30_grp27_01 ),
};

/* group 27 2 */
const Sint32 pl30_grp27_02[] = {
	EF_FRAME(    0,    0,   1,           0,   974 ),
	EF_FRAME(    0,    0,   1,           0,   975 ),
	EF_FRAME(    0,    0,   1,           0,   976 ),
	EF_FRAME(    0,    0,   1,           0,   977 ),
	EF_FRAME(    0,    0,   1,           0,   978 ),
	EF_FRAME(    0,    0,   1,           0,   979 ),
	EF_FRAME(    0,    0,   1,           0,   980 ),
	EF_FRAME(    0,    0,   1,           0,   981 ),
	EF_FRAME(    0,    0,   1,           0,   982 ),
	EF_FRAME(    0,    0,   1,           0,   983 ),
	EF_FRAME(    0,    0,   1,           0,   984 ),
	EF_FRAME(    0,    0,   1,           0,   985 ),
	EF_FRAME(    0,    0,   1,           0,   986 ),
	EF_FRAME(    0,    0,   1,           0,   987 ),
	EF_FRAME(    0,    0,   1,           0,   988 ),
	EF_FRAME(    0,    0,   1,           0,   989 ),
	EF_FRAME(    0,    0,   1,           0,   990 ),
	EF_FRAME(    0,    0,   1,           0,   991 ),
	EF_FRAME(    0,    0,   1,           0,   992 ),
	EF_FRAME(    0,    0,   1,           0,   993 ),
	EF_FRAME(    0,    0,   1,           0,   994 ),
	EF_FRAME(    0,    0,   1,           0,   995 ),
	EF_FRAME(    0,    0,   1,           0,   996 ),
	EF_FRAME(    0,    0,   1,           0,   997 ),
	EF_FRAME(    0,    0,   1,           0,   998 ),
	EF_FRAME(    0,    0,   1,           0,   999 ),
	EF_FRAME(    0,    0,   1,           0,  1000 ),
	EF_FRAME(    0,    0,   1,           0,  1001 ),
	EF_FRAME(    0,    0,   1,           0,  1002 ),
	EF_FRAME(    0,    0,   1,           0,  1003 ),
	EF_FRAME(    0,    0,   1,           0,  1004 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,  1005 ),
	CG_NEXT( pl30_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl30_grp27_03[] = {
	EF_FRAME(    0,    0,   1,           0,  1006 ),
	EF_FRAME(    0,    0,   1,           0,  1007 ),
	EF_FRAME(    0,    0,   1,           0,  1008 ),
	EF_FRAME(    0,    0,   1,           0,  1009 ),
	EF_FRAME(    0,    0,   1,           0,  1010 ),
	EF_FRAME(    0,    0,   1,           0,  1011 ),
	EF_FRAME(    0,    0,   1,           0,  1012 ),
	EF_FRAME(    0,    0,   1,           0,  1013 ),
	EF_FRAME(    0,    0,   1,           0,  1014 ),
	EF_FRAME(    0,    0,   1,           0,  1015 ),
	EF_FRAME(    0,    0,   1,           0,  1016 ),
	EF_FRAME(    0,    0,   1,           0,  1017 ),
	EF_FRAME(    0,    0,   1,           0,  1018 ),
	EF_FRAME(    0,    0,   1,           0,  1019 ),
	EF_FRAME(    0,    0,   1,           0,  1020 ),
	EF_FRAME(    0,    0,   1,           0,  1021 ),
	EF_FRAME(    0,    0,   1,           0,  1022 ),
	EF_FRAME(    0,    0,   1,           0,  1023 ),
	EF_FRAME(    0,    0,   1,           0,  1024 ),
	EF_FRAME(    0,    0,   1,           0,  1025 ),
	EF_FRAME(    0,    0,   1,           0,  1026 ),
	EF_FRAME(    0,    0,   1,           0,  1027 ),
	EF_FRAME(    0,    0,   1,           0,  1028 ),
	EF_FRAME(    0,    0,   1,           0,  1029 ),
	EF_FRAME(    0,    0,   1,           0,  1030 ),
	EF_FRAME(    0,    0,   1,           0,  1031 ),
	EF_FRAME(    0,    0,   1,           0,  1032 ),
	EF_FRAME(    0,    0,   1,           0,  1033 ),
	EF_FRAME(    0,    0,   1,           0,  1034 ),
	EF_FRAME(    0,    0,   1,           0,  1035 ),
	EF_FRAME(    0,    0,   1,           0,  1036 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,  1037 ),
	CG_NEXT( pl30_grp27_03 ),
};

/* group 27 4 */
const Sint32 pl30_grp27_04[] = {
	EF_FRAME(    0,    0,   1,           0,   725 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   726 ),
	CG_NEXT( pl30_grp27_04 ),
};

/* group 27 5 */
const Sint32 pl30_grp27_05[] = {
	EF_FRAME(    0,    0,   1,           0,   709 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   710 ),
	CG_NEXT( pl30_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl30_grp27_06[] = {
	EF_FRAME(    0,    0,   1,           0,   694 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   695 ),
	CG_NEXT( pl30_grp27_06 ),
};

/* group 27 7 */
const Sint32 pl30_grp27_07[] = {
	EF_FRAME(    0,    0,   1,           0, 32871 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32872 ),
	CG_NEXT( pl30_grp27_07 ),
};

/* group 27 8 */
const Sint32 pl30_grp27_08[] = {
	EF_FRAME(    0,    0,   1,           0, 32869 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32870 ),
	CG_NEXT( pl30_grp27_08 ),
};

/* group 27 9 */
const Sint32 pl30_grp27_09[] = {
	EF_FRAME(    0,    0,   1,           0, 32865 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA, 32866 ),
	CG_NEXT( pl30_grp27_09 ),
};

/* group 27 10: pls30_00_move */
const Sint32 pl30_grp27_10[] = {
	EF_FRAME(    0,    0,   1,           0,  1214 ),
	EF_FRAME(    0,    0,   1,           0,  1215 ),
	EF_FRAME(    0,    0,   1,           0,  1216 ),
	EF_FRAME(    0,    0,   1,           0,  1208 ),
	EF_FRAME(    0,    0,   1,           0,  1209 ),
	EF_FRAME(    0,    0,   1,           0,  1210 ),
	EF_FRAME(    0,    0,   1,           0,  1211 ),
	EF_FRAME(    0,    0,   1,           0,  1212 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,  1213 ),
	CG_NEXT( pl30_grp27_10 ),
};

/* group 27 11: pls30_00_ground */
const Sint32 pl30_grp27_11[] = {
	EF_FRAME(    0,    0,   1,           0,   939 ),
	EF_FRAME(    0,    0,   1,           0,   940 ),
	EF_FRAME(    0,    0,   1, END_OF_DATA,   941 ),
	CG_NEXT( pl30_grp27_11 ),
};

const Sint32 pl30_unused_32[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   6,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	0x00007d58,
};
