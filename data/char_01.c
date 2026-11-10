/* Zangief (character 01): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_01      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_01     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl01.c) says which group
                   and number each move uses.
   pl01_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl01_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl01_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl01_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_01
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_01, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_01 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl01_stand_00[], pl01_stand_01[], pl01_stand_02[], pl01_stand_08[],
	pl01_stand_03[], pl01_stand_09[], pl01_stand_06[], pl01_stand_07[], pl01_jump_00[],
	pl01_jump_00_loop[], pl01_jump_03[], pl01_jump_06[], pl01_jump_09[], pl01_jump_12[],
	pl01_dash_00[], pl01_dash_00_loop[], pl01_dash_01[], pl01_crouch_01[], pl01_crouch_02[],
	pl01_turn_00[], pl01_turn_01[], pl01_guard_00[], pl01_guard_01[], pl01_guard_02[],
	pl01_guard_04[], pl01_guard_05[], pl01_guard_06[], pl01_air_guard_00[], pl01_air_guard_01[],
	pl01_stand_p_00[], pl01_stand_p_01[], pl01_stand_p_02[], pl01_stand_p_03[], pl01_stand_k_00[],
	pl01_stand_k_01[], pl01_stand_k_02[], pl01_crouch_p_00[], pl01_crouch_p_01[], pl01_crouch_p_02[],
	pl01_crouch_k_00[], pl01_crouch_k_01[], pl01_crouch_k_02[], pl01_jump_p_00[], pl01_jump_p_01[],
	pl01_jump_p_02[], pl01_jump_p_05[], pl01_jump_p_06[], pl01_jump_p_08[], pl01_jump_p_08_loop[],
	pl01_jump_p_09[], pl01_unused_00[], pl01_unused_01[], pl01_unused_02[], pl01_unused_03[],
	pl01_unused_04[], pl01_unused_05[], pl01_unused_06[], pl01_unused_07[], pl01_unused_08[],
	pl01_unused_09[], pl01_unused_10[], pl01_unused_11[], pl01_unused_12[], pl01_unused_13[],
	pl01_unused_14[], pl01_jump_p_07[], pl01_jump_p_07_loop[], pl01_jump_k_00[], pl01_jump_k_01[],
	pl01_jump_k_02[], pl01_jump_k_06[], pl01_jump_k_06_loop[], pl01_get_up_00[], pl01_get_up_01[],
	pl01_get_up_02[], pl01_get_up_03[], pl01_appear_00[], pl01_win_00[], pl01_win_00_loop[],
	pl01_win_01[], pl01_win_01_loop[], pl01_win_02[], pl01_win_02_loop[], pl01_extra_00[],
	pl01_extra_01[], pl01_extra_01_loop[], pl01_extra_02[], pl01_extra_03[], pl01_special_06[],
	pl01_special_07[], pl01_special_14[], pl01_special_14_loop[], pl01_special_15[],
	pl01_special_09[], pl01_special_09_loop[], pl01_special_10[], pl01_special_11[],
	pl01_special_12[], pl01_special_12_loop[], pl01_special_13[], pl01_special_08[],
	pl01_special_00[], pl01_special_01[], pl01_special_01_loop[], pl01_special_02[],
	pl01_special_03[], pl01_special_03_loop[], pl01_special_04[], pl01_special_16[],
	pl01_damage_00[], pl01_damage_01[], pl01_damage_02[], pl01_damage_03[], pl01_damage_04[],
	pl01_damage_05[], pl01_damage_06[], pl01_damage_07[], pl01_damage_08[], pl01_damage_10[],
	pl01_damage_11[], pl01_damage_12[], pl01_damage_13[], pl01_damage_09[], pl01_damage_09_loop[],
	pl01_damage_16[], pl01_damage_17[], pl01_damage_18[], pl01_damage_19[], pl01_damage_20[],
	pl01_damage_21[], pl01_damage_22[], pl01_damage_23[], pl01_damage_24[], pl01_damage_25[],
	pl01_damage_26[], pl01_damage_27[], pl01_damage_30[], pl01_damage_32[], pl01_damage_28[],
	pl01_damage_29[], pl01_damage_31[], pl01_dizzy_00[], pl01_super_00[], pl01_change_00[],
	pl01_change_00_loop[], pl01_change_03[], pl01_change_03_loop[], pl01_change_01[],
	pl01_change_04[], pl01_change_02[], pl01_change_02_loop[], pl01_catch_00[], pl01_catch_01[],
	pl01_catch_07[], pl01_catch_07_loop[], pl01_catch_50[], pl01_catch_02[], pl01_catch_51[],
	pl01_catch_52[], pl01_catch_03[], pl01_catch_04[], pl01_catch_05[], pl01_catch_08[],
	pl01_catch_09[], pl01_catch_10[], pl01_catch_11[], pl01_catch_12[], pl01_catch_41[],
	pl01_catch_13[], pl01_catch_14[], pl01_unused_15[], pl01_catch_15[], pl01_catch_16[],
	pl01_catch_17[], pl01_catch_18[], pl01_catch_19[], pl01_catch_20[], pl01_catch_21[],
	pl01_unused_16[], pl01_catch_22[], pl01_catch_22_loop[], pl01_catch_61[], pl01_catch_61_loop[],
	pl01_catch_23[], pl01_catch_24[], pl01_catch_25[], pl01_catch_26[], pl01_catch_26_loop[],
	pl01_catch_27[], pl01_catch_28[], pl01_unused_17[], pl01_unused_18[], pl01_unused_19[],
	pl01_unused_20[], pl01_unused_21[], pl01_unused_22[], pl01_unused_23[], pl01_unused_24[],
	pl01_unused_25[], pl01_unused_26[], pl01_catch_29[], pl01_catch_30[], pl01_catch_31[],
	pl01_catch_31_loop[], pl01_catch_32[], pl01_catch_33[], pl01_catch_34[], pl01_catch_06[],
	pl01_catch_06_loop[], pl01_catch_35[], pl01_catch_35_loop[], pl01_catch_36[], pl01_catch_37[],
	pl01_catch_38[], pl01_catch_39[], pl01_catch_40[], pl01_catch_42[], pl01_catch_43[],
	pl01_catch_43_loop[], pl01_catch_44[], pl01_catch_45[], pl01_catch_45_loop[], pl01_catch_46[],
	pl01_catch_47[], pl01_catch_48[], pl01_catch_48_loop[], pl01_catch_49[], pl01_catch_53[],
	pl01_catch_55[], pl01_catch_54[], pl01_catch_56[], pl01_catch_57[], pl01_catch_57_loop[],
	pl01_catch_58[], pl01_catch_59[], pl01_catch_59_loop[], pl01_catch_60[], pl01_caught_00[],
	pl01_caught_01[], pl01_caught_02[], pl01_caught_03[], pl01_caught_04[], pl01_caught_05[],
	pl01_caught_06[], pl01_caught_07[], pl01_caught_08[], pl01_caught_09[], pl01_caught_10[],
	pl01_caught_11[], pl01_caught_12[], pl01_caught_13[], pl01_caught_14[], pl01_caught_15[],
	pl01_caught_16[], pl01_caught_17[], pl01_caught_18[], pl01_caught_19[], pl01_caught_20[],
	pl01_caught_21[], pl01_unused_27[], pl01_unused_28[], pl01_unused_29[], pl01_unused_30[],
	pl01_unused_31[], pl01_unused_32[], pl01_unused_33[], pl01_unused_34[], pl01_unused_35[],
	pl01_unused_36[], pl01_unused_37[], pl01_unused_38[], pl01_unused_39[], pl01_unused_40[],
	pl01_unused_41[], pl01_unused_42[], pl01_unused_43[], pl01_unused_44[], pl01_unused_45[],
	pl01_unused_46[], pl01_unused_47[];
extern const Sint32 *const pl01_stand_tbl[], *const pl01_jump_tbl[], *const pl01_dash_tbl[],
	*const pl01_crouch_tbl[], *const pl01_turn_tbl[], *const pl01_guard_tbl[],
	*const pl01_air_guard_tbl[], *const pl01_stand_p_tbl[], *const pl01_stand_k_tbl[],
	*const pl01_crouch_p_tbl[], *const pl01_crouch_k_tbl[], *const pl01_jump_p_tbl[],
	*const pl01_jump_k_tbl[], *const pl01_damage_tbl[], *const pl01_dizzy_tbl[],
	*const pl01_catch_tbl[], *const pl01_caught_tbl[], *const pl01_get_up_tbl[],
	*const pl01_appear_tbl[], *const pl01_win_tbl[], *const pl01_extra_tbl[],
	*const pl01_special_tbl[], *const pl01_super_tbl[], *const pl01_change_tbl[],
	*const pl01_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_01[16] = {
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
const Sint32 *const *const char_tbl_01[] = {
	pl01_stand_tbl, pl01_jump_tbl, pl01_dash_tbl, pl01_crouch_tbl, pl01_turn_tbl, pl01_guard_tbl,
	pl01_air_guard_tbl, pl01_stand_p_tbl, pl01_stand_k_tbl, pl01_crouch_p_tbl, pl01_crouch_k_tbl, pl01_jump_p_tbl,
	pl01_jump_k_tbl, pl01_damage_tbl, pl01_dizzy_tbl, pl01_catch_tbl, pl01_caught_tbl, pl01_get_up_tbl,
	pl01_appear_tbl, pl01_win_tbl, pl01_extra_tbl, pl01_special_tbl, pl01_super_tbl, pl01_super_tbl,
	pl01_change_tbl, pl01_grp25_tbl, 0,
};

const Sint32 *const pl01_stand_tbl[] = {
	pl01_stand_00, pl01_stand_01, pl01_stand_02, pl01_stand_03, pl01_stand_00, pl01_stand_00,
	pl01_stand_06, pl01_stand_07, pl01_stand_08, pl01_stand_09, 0,
};

const Sint32 *const pl01_jump_tbl[] = {
	pl01_jump_00, pl01_jump_00, pl01_jump_00, pl01_jump_03, pl01_jump_03, pl01_jump_03,
	pl01_jump_06, pl01_jump_06, pl01_jump_06, pl01_jump_09, pl01_jump_09, pl01_jump_09,
	pl01_jump_12, 0,
};

const Sint32 *const pl01_dash_tbl[] = {
	pl01_dash_00, pl01_dash_01, pl01_stand_00, pl01_stand_00, 0,
};

const Sint32 *const pl01_crouch_tbl[] = {
	pl01_dash_01, pl01_crouch_01, pl01_crouch_02, 0,
};

const Sint32 *const pl01_turn_tbl[] = {
	pl01_turn_00, pl01_turn_01, 0,
};

const Sint32 *const pl01_guard_tbl[] = {
	pl01_guard_00, pl01_guard_01, pl01_guard_02, pl01_guard_01, pl01_guard_04, pl01_guard_05,
	pl01_guard_06, pl01_guard_05, 0,
};

const Sint32 *const pl01_air_guard_tbl[] = {
	pl01_air_guard_00, pl01_air_guard_01, pl01_air_guard_01, 0,
};

const Sint32 *const pl01_stand_p_tbl[] = {
	pl01_stand_p_00, pl01_stand_p_01, pl01_stand_p_02, pl01_stand_p_03, 0,
};

const Sint32 *const pl01_stand_k_tbl[] = {
	pl01_stand_k_00, pl01_stand_k_01, pl01_stand_k_02, 0,
};

const Sint32 *const pl01_crouch_p_tbl[] = {
	pl01_crouch_p_00, pl01_crouch_p_01, pl01_crouch_p_02, 0,
};

const Sint32 *const pl01_crouch_k_tbl[] = {
	pl01_crouch_k_00, pl01_crouch_k_01, pl01_crouch_k_02, 0,
};

const Sint32 *const pl01_jump_p_tbl[] = {
	pl01_jump_p_00, pl01_jump_p_01, pl01_jump_p_02, pl01_jump_p_00, pl01_jump_p_01, pl01_jump_p_05,
	pl01_jump_p_06, pl01_jump_p_07, pl01_jump_p_08, pl01_jump_p_09, 0,
};

const Sint32 *const pl01_jump_k_tbl[] = {
	pl01_jump_k_00, pl01_jump_k_01, pl01_jump_k_02, pl01_jump_k_00, pl01_jump_k_01, pl01_jump_k_02,
	pl01_jump_k_06, 0,
};

const Sint32 *const pl01_damage_tbl[] = {
	pl01_damage_00, pl01_damage_01, pl01_damage_02, pl01_damage_03, pl01_damage_04, pl01_damage_05,
	pl01_damage_06, pl01_damage_07, pl01_damage_08, pl01_damage_09, pl01_damage_10, pl01_damage_11,
	pl01_damage_12, pl01_damage_13, pl01_damage_13, pl01_damage_13, pl01_damage_16, pl01_damage_17,
	pl01_damage_18, pl01_damage_19, pl01_damage_20, pl01_damage_21, pl01_damage_22, pl01_damage_23,
	pl01_damage_24, pl01_damage_25, pl01_damage_26, pl01_damage_27, pl01_damage_28, pl01_damage_29,
	pl01_damage_30, pl01_damage_31, pl01_damage_32, pl01_damage_32, pl01_damage_13, 0,
};

const Sint32 *const pl01_dizzy_tbl[] = {
	pl01_dizzy_00, 0,
};

const Sint32 *const pl01_catch_tbl[] = {
	pl01_catch_00, pl01_catch_01, pl01_catch_02, pl01_catch_03, pl01_catch_04, pl01_catch_05,
	pl01_catch_06, pl01_catch_07, pl01_catch_08, pl01_catch_09, pl01_catch_10, pl01_catch_11,
	pl01_catch_12, pl01_catch_13, pl01_catch_14, pl01_catch_15, pl01_catch_16, pl01_catch_17,
	pl01_catch_18, pl01_catch_19, pl01_catch_20, pl01_catch_21, pl01_catch_22, pl01_catch_23,
	pl01_catch_24, pl01_catch_25, pl01_catch_26, pl01_catch_27, pl01_catch_28, pl01_catch_29,
	pl01_catch_30, pl01_catch_31, pl01_catch_32, pl01_catch_33, pl01_catch_34, pl01_catch_35,
	pl01_catch_36, pl01_catch_37, pl01_catch_38, pl01_catch_39, pl01_catch_40, pl01_catch_41,
	pl01_catch_42, pl01_catch_43, pl01_catch_44, pl01_catch_45, pl01_catch_46, pl01_catch_47,
	pl01_catch_48, pl01_catch_49, pl01_catch_50, pl01_catch_51, pl01_catch_52, pl01_catch_53,
	pl01_catch_54, pl01_catch_55, pl01_catch_56, pl01_catch_57, pl01_catch_58, pl01_catch_59,
	pl01_catch_60, pl01_catch_61, 0,
};

const Sint32 *const pl01_caught_tbl[] = {
	pl01_caught_00, pl01_caught_01, pl01_caught_02, pl01_caught_03, pl01_caught_04, pl01_caught_05,
	pl01_caught_06, pl01_caught_07, pl01_caught_08, pl01_caught_09, pl01_caught_10, pl01_caught_11,
	pl01_caught_12, pl01_caught_13, pl01_caught_14, pl01_caught_15, pl01_caught_16, pl01_caught_17,
	pl01_caught_18, pl01_caught_19, pl01_caught_20, pl01_caught_21, 0,
};

const Sint32 *const pl01_get_up_tbl[] = {
	pl01_get_up_00, pl01_get_up_01, pl01_get_up_02, pl01_get_up_03, 0,
};

const Sint32 *const pl01_appear_tbl[] = {
	pl01_appear_00, 0,
};

const Sint32 *const pl01_win_tbl[] = {
	pl01_win_00, pl01_win_01, pl01_win_02, 0,
};

const Sint32 *const pl01_extra_tbl[] = {
	pl01_extra_00, pl01_extra_01, pl01_extra_02, pl01_extra_03, 0,
};

const Sint32 *const pl01_special_tbl[] = {
	pl01_special_00, pl01_special_01, pl01_special_02, pl01_special_03, pl01_special_04, pl01_special_04,
	pl01_special_06, pl01_special_07, pl01_special_08, pl01_special_09, pl01_special_10, pl01_special_11,
	pl01_special_12, pl01_special_13, pl01_special_14, pl01_special_15, pl01_special_16, 0,
};

const Sint32 *const pl01_super_tbl[] = {
	pl01_super_00, 0,
};

const Sint32 *const pl01_change_tbl[] = {
	pl01_change_00, pl01_change_01, pl01_change_02, pl01_change_03, pl01_change_04, 0,
};

const Sint32 *const pl01_grp25_tbl[] = {
	pl01_stand_00, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 4
   Char_Stand 5
   Char_Dash 2
   Char_Dash 3
   group 25 0 */
const Sint32 pl01_stand_00[] = {
	CG_FRAME(    0,    0,    8,           0,    17, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    17, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    17, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    18, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    19, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    20, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    21, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    22, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    23, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    24, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    25, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    26, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    28, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    29, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    30, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    31, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    32, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    33, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_00 ),
};

/* Char_Stand 1 */
const Sint32 pl01_stand_01[] = {
	CG_FRAME(    0,    0,    8,           0,    27, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    28, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    29, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    30, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    31, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    32, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    33, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    34, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    35, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    36, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_01 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl01_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_02 ),
};

/* Char_Stand 8 */
const Sint32 pl01_stand_08[] = {
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,    38, 0x80,   0x00,                0,  0x01,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,    43, 0x80,   0x00,                0,  0x01,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_08 ),
};

/* Char_Stand 3 */
const Sint32 pl01_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_03 ),
};

/* Char_Stand 9 */
const Sint32 pl01_stand_09[] = {
	CG_FRAME(    0,    0,    6,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,    53, 0x80,   0x00,                0,  0x01,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,    48, 0x80,   0x00,                0,  0x01,       0x21,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_09 ),
};

/* Char_Stand 6 */
const Sint32 pl01_stand_06[] = {
	CG_FRAME(    0,    0,    8,           0,    37, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl01_stand_07[] = {
	CG_FRAME(    0,    0,    8,           0,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_stand_07 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl01_jump_00[] = {
	CG_FRAME(    0,    1,    5,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    4,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   10,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl01_jump_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl01_jump_00_loop ),
};

/* Char_Jump 3: blizzard_fall (Siberian Blizzard (Mecha Zangief's hyper))
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl01_jump_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_jump_03 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl01_jump_06[] = {
	CG_FRAME(    0,    1,    6,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   127, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   12,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl01_jump_09[] = {
	CG_FRAME(    0,    0,    6,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_jump_09 ),
};

/* Char_Jump 12: jump_punch_move, act_wall_jump */
const Sint32 pl01_jump_12[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_jump_12 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl01_dash_00[] = {
	CG_FRAME(    0,    1,    1,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    5,           0,    67, 0x80,   0x00,                6,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_dash_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_NEXT( pl01_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash
   Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl01_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl01_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl01_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    68, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl01_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl01_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl01_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl01_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl01_guard_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl01_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl01_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    81, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl01_guard_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl01_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    82, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl01_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    83, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl01_stand_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   132, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   135, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    6,           0,   133, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   134, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl01_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl01_stand_p_01[] = {
	CG_FRAME(    0,    0,    2,           0,   136, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   137, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   139, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    9,           0,   139, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   140, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl01_stand_p_02[] = {
	CG_FRAME(    0,    0,   12,           0,   141, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   142, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   142, 0x80,   0x00,          ATK | 9,  0x00,       0x04,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x00,       ATK | 0x17,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME( 0x80,    1,    4,           0,   143, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   12,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    6,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_stand_p_02 ),
};

/* Char_Stand_P 3 */
const Sint32 pl01_stand_p_03[] = {
	CG_FRAME(    0,    0,    2,           0,    72, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    71, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    70, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   328, 0x80,   0x00, ATK | CG_ATCK_NG | 7,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   12,           0,   329, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0x0100,  67 ),
	CG_FRAME(    0,    0,    6,           0,   330, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   16,           0,   330, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    6,           0,   331, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,    6,           0,   115, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    77, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    76, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    75, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_stand_p_03 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl01_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   146, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   147, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   148, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    5,           0,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3,           0,   146, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl01_stand_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   146, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    6,           0,   151, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    6,           0,   150, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    4,           0,   146, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl01_stand_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   153, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   154, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x0100,  41 ),
	CG_FRAME(    0,    0,    3,           0,   157, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    9,           0,   158, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    6,           0,   115, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    8,           0,   116, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    67, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl01_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   160, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   159, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl01_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl01_crouch_p_01[] = {
	CG_FRAME(    0,    0,    7,           0,   161, 0x80,   0x67,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   162, 0x80,   0x67,          ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   163, 0x80,   0x67,       ATK | 0x16,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x67,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   15,           0,   164, 0x80,   0x67,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   168, 0x80,   0x67,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl01_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl01_crouch_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   165, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    6,           0,   165, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x6b,       ATK | 0x17,  0x01,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x6b,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   13,           0,   167, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   168, 0x80,   0x6b,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl01_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl01_crouch_k_00[] = {
	CG_FRAME(    0,    0,    5,           0,   171, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    1,           0,   169, 0x80,   0x87,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0xa4,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    5,           0,   169, 0x80,   0x87,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   171, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl01_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl01_crouch_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   175, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   176, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   178, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    1,           0,   179, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   10,           0,   179, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6,           0,   176, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   175, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   171, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl01_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl01_crouch_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   175, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    5,           0,   172, 0x80,   0x40,       ATK | 0x17,  0x01,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,    2,           0,   173, 0x80,   0xe1,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    2,           0,   174, 0x80,   0xe1,              ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   17,           0,   174, 0x80,   0xe1,              ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    7,           0,   175, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   171, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl01_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl01_jump_p_00[] = {
	CG_FRAME(    0,    0,    1,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    1,           0,   181, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    4,           0,   182, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    6,           0,   182, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    63, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl01_jump_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   10,           0,   183, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    1,           0,   184, 0x80,   0x00,       ATK | 0x16,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    6,           0,   186, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   187, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,    63, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl01_jump_p_02[] = {
	CG_FRAME(    0,    0,    1,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    8,           0,   188, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   188, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    1,           0,   189, 0x80,   0x00,       ATK | 0x16,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    2,           0,   190, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    9,           0,   190, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    7,           0,   180, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    63, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_p_02 ),
};

/* Char_Jump_P 5 */
const Sint32 pl01_jump_p_05[] = {
	CG_FRAME(    0,    0,    1,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    7,           0,   204, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    1,           0,   205, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    7,           0,   206, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    6,           0,   203, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    63, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_p_05 ),
};

/* Char_Jump_P 6 */
const Sint32 pl01_jump_p_06[] = {
	CG_FRAME(    0,    0,    5,           0,   222, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,          ATK | 6,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   224, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   15,           0,   225, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    63, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_p_06 ),
};

/* Char_Jump_P 8 */
const Sint32 pl01_jump_p_08[] = {
	CG_FRAME(    0,    0,    1,           0,   337, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    6,           0,   337, 0x80,   0x00, ATK | CG_ATCK_NG | 8,  0x00,       0x04,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    7,           0,   338, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   339, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0, 0x0100,  38 ),
};

const Sint32 pl01_jump_p_08_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   340, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl01_jump_p_08_loop ),
};

/* Char_Jump_P 9: lever_atck0_dive */
const Sint32 pl01_jump_p_09[] = {
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    6,           0,   330, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    6,           0,   330, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    6,           0,   331, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,    6,           0,   115, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,    77, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    75, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_jump_p_09 ),
};

const Sint32 pl01_unused_00[] = {	/* nothing reaches it */
	0x00060000,
};

const Sint32 pl01_unused_01[] = {
	0x00000073,
};

const Sint32 pl01_unused_02[] = {
	0x00000080,
};

const Sint32 pl01_unused_03[] = {
	0x00000000,
};

const Sint32 pl01_unused_04[] = {
	0x002b0000,
};

const Sint32 pl01_unused_05[] = {
	0x00060000,
};

const Sint32 pl01_unused_06[] = {
	0x00000074,
};

const Sint32 pl01_unused_07[] = {
	0x00000080,
};

const Sint32 pl01_unused_08[] = {
	0x00000000,
};

const Sint32 pl01_unused_09[] = {
	0x002b0000,
};

const Sint32 pl01_unused_10[] = {
	0x80060000,
};

const Sint32 pl01_unused_11[] = {
	0x00000043,
};

const Sint32 pl01_unused_12[] = {
	0x00000080,
};

const Sint32 pl01_unused_13[] = {
	0x00000000,
};

const Sint32 pl01_unused_14[] = {
	0x002b0000,
};

/* Char_Jump_P 7 */
const Sint32 pl01_jump_p_07[] = {
	CG_FRAME(    0,    0,    1,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   226, 0x80,   0x00,          ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  42 ),
};

const Sint32 pl01_jump_p_07_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   226, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_NEXT( pl01_jump_p_07_loop ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl01_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,    62, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   191, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    1,           0,   192, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    7,           0,   193, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   191, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl01_jump_k_01[] = {
	CG_FRAME(    0,    0,    1,           0,    62, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    5,           0,   194, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    6,           0,   196, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    6,           0,   196, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   195, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl01_jump_k_02[] = {
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    8,           0,   197, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   197, 0x80,   0x00,       ATK | 0x17,  0x01,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   198, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    2,           0,   199, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   201, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   202, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl01_jump_k_02 ),
};

/* Char_Jump_K 6 */
const Sint32 pl01_jump_k_06[] = {
	CG_FRAME(    0,    0,    2,           0,    63, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x00,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
};

const Sint32 pl01_jump_k_06_loop[] = {
	CG_FRAME(    0,    0,    7, END_OF_DATA,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl01_jump_k_06_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl01_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   113, 0x80,   0xcb,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   114, 0x80,   0xac,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    9,           0,   115, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    9,           0,   116, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl01_get_up_01[] = {
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   119, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   120, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   115, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl01_get_up_02[] = {
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   119, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   120, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   119, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   120, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   119, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   120, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   115, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl01_get_up_03[] = {
	CG_FRAME(    0,    0,    2,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   120, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   119, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   120, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   119, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   115, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   116, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl01_appear_00[] = {
	CG_FRAME(    0,    0,   32,           0,    99, 0x80,   0x3b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   100, 0x80,   0x3b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    7,           0,   101, 0x80,   0x3b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   102, 0x80,   0x3b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   103, 0x80,   0x3b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   108, 0x80,   0x00,             0x0f,  0x00,       0x1f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_appear_00 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl01_win_00[] = {
	CG_FRAME(    0,    0,    3,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    89, 0x80,   0x00,                9,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_win_00_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl01_win_01[] = {
	CG_FRAME(    0,    0,    1,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    84, 0x80,   0x00,             0x0b,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_win_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_win_01_loop ),
};

/* Char_Win 2: lose_ko_init, lose_time_init */
const Sint32 pl01_win_02[] = {
	CG_FRAME(    0,    0,    3,           0,    79, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   393, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   392, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_win_02_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   390, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_win_02_loop ),
};

/* Char_Extra 0: t_kick_move */
const Sint32 pl01_extra_00[] = {
	CG_FRAME(    0,    0,    6,           0,    69, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    1,           0,    68, 0x80,   0x40,       ATE | 0x13,  0x00,       0x12,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    1,           0,    67, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,    84, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,    85, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,    93, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,    86, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,    87, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,    94, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    2,           0,    95, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    3,           0,    96, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    3,           0,    95, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    3,           0,    96, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    95, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl01_extra_00 ),
};

/* Char_Extra 1: shl01_01_init */
const Sint32 pl01_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    3,           0,   410, 0x80,   0x00,       ATK | 0x1e,  0x00, 0x4b | CSE,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    3,           0,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    3,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
};

const Sint32 pl01_extra_01_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,    3,           0,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   417, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,    3,           0,   418, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   419, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,    3,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    1,    3,           0,   417, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3,           0,   418, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   419, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl01_extra_01_loop ),
};

/* Char_Extra 2: shl01_01_move, fly_start5 */
const Sint32 pl01_extra_02[] = {
	CG_FRAME(    0,    0,    3,           0,   414, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   420, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   421, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   423, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   424, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   425, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_extra_02 ),
};

/* Char_Extra 3: pl01_upper, fly_start5 */
const Sint32 pl01_extra_03[] = {
	CG_FRAME(    0,    0,    7,           0,   161, 0x80,   0x67,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2,           0,   162, 0x80,   0x67,          ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    3,           0,   163, 0x80,   0x67,       ATK | 0x16,  0x01,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x67,              ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,   15,           0,   164, 0x80,   0x67,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   168, 0x80,   0x67,              ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl01_extra_03 ),
};

/* Char_Special 6: t_kick_init */
const Sint32 pl01_special_06[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   226, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl01_special_06 ),
};

/* Char_Special 7: chouhatsu_init (the taunt) */
const Sint32 pl01_special_07[] = {
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x69,       ATE | 0x14,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   145, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_07 ),
};

/* Char_Special 14: iron_init (Iron Body: Mecha Zangief and back (hyper)) */
const Sint32 pl01_special_14[] = {
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x69,       ATE | 0x1b,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_special_14_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_14_loop ),
};

/* Char_Special 15: iron_move (Iron Body: Mecha Zangief and back (hyper)) */
const Sint32 pl01_special_15[] = {
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   145, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_15 ),
};

/* Char_Special 9: vodka_init (Vodka Fire) */
const Sint32 pl01_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x69,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_special_09_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   270, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_09_loop ),
};

/* Char_Special 10: vodka_wait (Vodka Fire) */
const Sint32 pl01_special_10[] = {
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   145, 0x80,   0x69,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_10 ),
};

/* Char_Special 11: blizzard_init (Siberian Blizzard (Mecha Zangief's hyper)) */
const Sint32 pl01_special_11[] = {
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,       ATK | 0x1c,  0x00,       0x1a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_special_11 ),
};

/* Char_Special 12: blizzard_start (Siberian Blizzard (Mecha Zangief's hyper)) */
const Sint32 pl01_special_12[] = {
	CG_FRAME(    0,   40,    1,           0, 32768, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0, 32779, 0x80,   0x00,       ATK | 0x10,  0x01,       0x1b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32779, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   41,    2,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,   42,    2,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
};

const Sint32 pl01_special_12_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32779, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    2,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    2,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    2,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    2,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl01_special_12_loop ),
};

/* Char_Special 13: blizzard_rise (Siberian Blizzard (Mecha Zangief's hyper)) */
const Sint32 pl01_special_13[] = {
	CG_FRAME(    0,    0,    6,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   128, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_special_13 ),
};

/* Char_Special 8: team_init (His part in a team hyper combo) */
const Sint32 pl01_special_08[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,          ATK | 8,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    1,    2,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    2,    2,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,    2,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_08 ),
};

/* Char_Special 0: lariat_init (Double Lariat) */
const Sint32 pl01_special_00[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,       ATK | 0x18,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32770, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    1,    3,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,    3,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(   50,    1,    3,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,    3,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(   50,    1,    3,           0, 32770, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    4,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,    5,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(   45,    1,    6,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    7,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,    8,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    1,    3,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_00 ),
};

/* Char_Special 1: lariat_j_init (Double Lariat) */
const Sint32 pl01_special_01[] = {
	CG_FRAME(    0,    1,    3,           0, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,    3,           0, 32777, 0x80,   0x00,       ATK | 0x18,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,    3,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
};

const Sint32 pl01_special_01_loop[] = {
	CG_FRAME(    0,    1,    3,           0, 32779, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    1,    3,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,    3,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl01_special_01_loop ),
};

/* Char_Special 2: qlariat_init (Quick Double Lariat) */
const Sint32 pl01_special_02[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,       ATK | 0x18,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,    3,           0, 32770, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    1,    3,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    1,    3,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    1,    3,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,    3,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    1,    3,           0, 32775, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    1,    3,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,    3,           0, 32771, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0, 32776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_special_02 ),
};

/* Char_Special 3: qlariat_j_init (Quick Double Lariat) */
const Sint32 pl01_special_03[] = {
	CG_FRAME(    0,    1,    3,           0, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,    3,           0, 32777, 0x80,   0x00,       ATK | 0x18,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    1,    3,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
};

const Sint32 pl01_special_03_loop[] = {
	CG_FRAME(    0,    1,    3,           0, 32779, 0x80,   0x00,       ATK | 0x0d,  0x00,       0x1e,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    1,    3,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    1,    3,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    1,    3,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl01_special_03_loop ),
};

/* Char_Special 4: banish_init (Banishing Flat, 4 + atck_level)
   Char_Special 5 */
const Sint32 pl01_special_04[] = {
	CG_FRAME(    0,    0,    4,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  31 ),
	CG_FRAME(    0,    1,    4,           0,   228, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0200,  31 ),
	CG_FRAME(    0,    1,    6,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,  31 ),
	CG_FRAME(    0,    0,    8,           0,   230, 0x80,   0x4a,          ATK | 5,  0x00,       0x05,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x4a,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    8,           0,   230, 0x80,   0x4a,              ATE,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    8,           0,   231, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   232, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   149, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_special_04 ),
};

/* Char_Special 16: snapback_init (the snapback) */
const Sint32 pl01_special_16[] = {
	CG_FRAME(    0,    0,    8,           0,   141, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    1,    0,    2,           0,   141, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   142, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   142, 0x80,   0x00,          ATK | 9,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   142, 0x80,   0x00,       ATK | 0x17,  0x01,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    1,    4,           0,   143, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   12,           0,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    6,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl01_special_16 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl01_damage_00[] = {
	CG_FRAME(    0,    0,    9,           0,   342, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   341, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl01_damage_01[] = {
	CG_FRAME(    0,    0,    2,           0,   342, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   343, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   342, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   341, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl01_damage_02[] = {
	CG_FRAME(    0,    0,    2,           0,   343, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   14,           0,   344, 0x80,   0x77,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   343, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   342, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   341, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl01_damage_03[] = {
	CG_FRAME(    0,    0,    9,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl01_damage_04[] = {
	CG_FRAME(    0,    0,    2,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   347, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl01_damage_05[] = {
	CG_FRAME(    0,    0,    2,           0,   347, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   348, 0x80,   0x75,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   347, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl01_damage_06[] = {
	CG_FRAME(    0,    0,    9,           0,   350, 0x80,   0x75,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   349, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl01_damage_07[] = {
	CG_FRAME(    0,    0,    2,           0,   350, 0x80,   0x75,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   351, 0x80,   0x77,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x75,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   349, 0x80,   0x12,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl01_damage_08[] = {
	CG_FRAME(    0,    0,    2,           0,   351, 0x80,   0x77,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   10,           0,   352, 0x80,   0x9a,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x77,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   350, 0x80,   0x75,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   349, 0x80,   0x73,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl01_damage_10[] = {
	CG_FRAME(    0,    0,    2,           0,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl01_damage_11[] = {
	CG_FRAME(    0,    0,    2,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   355, 0x80,   0x33,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl01_damage_12[] = {
	CG_FRAME(    0,    0,    2,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   356, 0x80,   0x36,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl01_damage_13[] = {
	CG_FRAME( 0x80,    0,    2,           0,   361, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x80,    0,    2,           0,   362, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x82,    0,    2,           0,   363, 0x80,   0xeb,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   377, 0x80,   0xed,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl01_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl01_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   364, 0x80,   0x2b,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   365, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   366, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   367, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   376, 0x80,   0xcc,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   375, 0x80,   0xec,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
};

const Sint32 pl01_damage_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   377, 0x80,   0xec,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl01_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl01_damage_16[] = {
	CG_FRAME(    0,    0,    3,           0,   368, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   369, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl01_damage_17[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   370, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl01_damage_18[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   371, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl01_damage_19[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   372, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl01_damage_20[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   373, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl01_damage_21[] = {
	CG_FRAME(    0,    0,    3,           0,   368, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   369, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 22 */
const Sint32 pl01_damage_22[] = {
	CG_FRAME(    0,    0,    6,           0,   370, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 23 */
const Sint32 pl01_damage_23[] = {
	CG_FRAME(    0,    0,    6,           0,   371, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 24 */
const Sint32 pl01_damage_24[] = {
	CG_FRAME(    0,    0,    6,           0,   372, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 25 */
const Sint32 pl01_damage_25[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   373, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl01_damage_26[] = {
	CG_FRAME(    0,    0,    6,           0,   374, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0xac,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   375, 0x80,   0xec,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl01_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl01_damage_27[] = {
	CG_FRAME(    0,    0,    3,           0,   370, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   371, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   201, 0x80,   0x27,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   202, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl01_damage_30[] = {
	CG_FRAME(    0,    0,    3,           0,   199, 0x80,   0x28,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x25,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   201, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   202, 0x80,   0x2a,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    63, 0x80,   0x2a,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_30 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl01_damage_32[] = {
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x25,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   201, 0x80,   0x2c,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   202, 0x80,   0x2a,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl01_damage_28[] = {
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   380, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   381, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   382, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   383, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl01_damage_29[] = {
	CG_FRAME(    0,    0,    3,           0,   384, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   385, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   386, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   387, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   388, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   389, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl01_damage_31[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   377, 0x80,   0xed,                0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl01_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl01_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   357, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   358, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   359, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   360, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   359, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   358, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_dizzy_00 ),
};

/* Char_Super 0
   Char_Effect 0: shl01_00_init */
const Sint32 pl01_super_00[] = {
	CG_FRAME(    0,    0,    4,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    8,           0,   405, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   406, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   407, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   408, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_super_00 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl01_change_00[] = {
	CG_FRAME(    0,    0,    2,           0,    84, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    86, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    87, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    88, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    89, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x30,   0x00,       ATK | 0x11,  0x00,       0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,    91, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl01_change_00_loop[] = {
	CG_FRAME(    0,    1,    9, END_OF_DATA,    91, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_change_00_loop ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl01_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   130, 0x80,   0x00,                0,  0x00,       0x13,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_change_03_loop[] = {
	CG_FRAME(    0,    1,   22, END_OF_DATA,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_change_03_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl01_change_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,   256, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,   257, 0x80,   0x0a,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl01_change_01 ),
};

/* Char_Change 4: team_move (His part in a team hyper combo), act_change_assist_end */
const Sint32 pl01_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x69,                0,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   145, 0x80,   0x69,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_change_04 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl01_change_02[] = {
	CG_FRAME(    0,    0,    6,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
};

const Sint32 pl01_change_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl01_change_02_loop ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl01_catch_00[] = {
	CG_FRAME(    0,    0,    1,           0,   213, 0x80,   0x68,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
	CG_FRAME(    0,    0,    4,           0,   271, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0, 0x0101,   1 ),
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x00,                5,  0x00,       0x05,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x86,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    1,   12,           0,   277, 0x80,   0x86,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    0,    8,           0,   278, 0x80,   0xcc,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0xac,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   280, 0x80,   0x6c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   281, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   282, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_00 ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl01_catch_01[] = {
	CG_FRAME(    0,    0,    2,           0,   212, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
};

/* Char_Catch 7: dash_hold */
const Sint32 pl01_catch_07[] = {
	CG_FRAME(    0,    0,    5,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
	CG_FRAME(    0,    0,    6,           0,   219, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
};

const Sint32 pl01_catch_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   217, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0108,   1 ),
	CG_NEXT( pl01_catch_07_loop ),
};

/* Char_Catch 50: nml_k_hold */
const Sint32 pl01_catch_50[] = {
	CG_FRAME(    0,    0,    6,           0,   219, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
	CG_FRAME(    0,    0,    4,           0,   218, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
	CG_FRAME(    0,    2,   16,           0,   217, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0108,   1 ),
	CG_FRAME(    0,    1,   22,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0109,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_50 ),
};

/* Char_Catch 2: crouch_k_set */
const Sint32 pl01_catch_02[] = {
	CG_FRAME(    0,    0,    6,           0,   220, 0x80,   0x65,                0,  0x00,          0,    0,   0,     0, 0x010a,   3 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   221, 0x80,   0x65,                6,  0x00,          0,    0,   0,     0, 0x010b,   3 ),
	CG_NEXT( pl01_catch_02 ),
};

/* Char_Catch 51: crouch_k_hold */
const Sint32 pl01_catch_51[] = {
	CG_FRAME(    0,    0,   12,           0,   220, 0x80,   0x65,                0,  0x00,          0,    0,   0,     0, 0x010a,   3 ),
	CG_FRAME(    0,    2,   18,           0,   221, 0x80,   0x65,                0,  0x00,          0,    0,   0,     0, 0x010b,   3 ),
	CG_FRAME(    0,    1,   20,           0,   221, 0x80,   0x65,                0,  0x00,          0,    0,   0,     0, 0x010b,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   168, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_catch_51 ),
};

/* Char_Catch 52: dash_hold */
const Sint32 pl01_catch_52[] = {
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x00,                5,  0x00,       0x05,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    1,   12,           0,   277, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    0,    8,           0,   278, 0x80,   0xcc,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0xac,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   280, 0x80,   0x6c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   281, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   282, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_52 ),
};

/* Char_Catch 3: jump_p_set */
const Sint32 pl01_catch_03[] = {
	CG_FRAME(    0,    0,    8,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0029,   2 ),
	CG_FRAME(    0,    0,    2,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x002a,   2 ),
	CG_FRAME(    0,    0,    6,           0,   252, 0x80,   0x00,                5,  0x00,       0x05,    0,   0,     0, 0x002b,   2 ),
	CG_FRAME(    0,    0,    6,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x002c,   2 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x002d,   2 ),
	CG_NEXT( pl01_catch_03 ),
};

/* Char_Catch 4: fab_g_slam (Final Atomic Buster (hyper)), com_p_set */
const Sint32 pl01_catch_04[] = {
	CG_FRAME(    0,    0,    6,           0,   332, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_FRAME(    0,    0,    8,           0,   333, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010d,   1 ),
	CG_FRAME(    0,    0,    6,           0,   334, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010e,   1 ),
	CG_FRAME(    0,    1,    2,           0,   336, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   335, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_04 ),
};

/* Char_Catch 5: dash_move */
const Sint32 pl01_catch_05[] = {
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_05 ),
};

/* Char_Catch 8: screw_init (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_08[] = {
	CG_FRAME(    0,    0,    2,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl01_catch_08 ),
};

/* Char_Catch 9: screw_catch (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_09[] = {
	CG_FRAME(    0,    0,    2,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   22,           0,   214, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6,           0,   215, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl01_catch_09 ),
};

/* Char_Catch 10: screw_catch (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_10[] = {
	CG_FRAME(    0,    0,    2,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0131,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0115,   1 ),
	CG_NEXT( pl01_catch_10 ),
};

/* Char_Catch 11: screw_hold (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_11[] = {
	CG_FRAME(    0,    0,    8,           0,   283, 0x80,   0x04,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_FRAME(    0,    0,    8,           0,   284, 0x80,   0x04,                0,  0x00,          0,    0,   0,     0, 0x010d,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   285, 0x80,   0x04,                0,  0x00,          0,    0,   0,     0, 0x010e,   1 ),
	CG_NEXT( pl01_catch_11 ),
};

/* Char_Catch 12: screw_jump (Spinning Pile Driver (ground and air)), screw_j_jump (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_12[] = {
	CG_FRAME(    0,    1,    4,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x010f,   1 ),
	CG_FRAME(    0,    1,    4,           0,   287, 0x80,   0x00,             0x0d,  0x00,       0x1e,    0,   0,     0, 0x0110,   1 ),
};

/* Char_Catch 41: fab_g_jump (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_41[] = {
	CG_FRAME(    0,    1,    4,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0111,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   289, 0x80,   0x00,             0x0e,  0x00,       0x1e,    0,   0,     0, 0x0112,   1 ),
	CG_NEXT( pl01_catch_12 ),
};

/* Char_Catch 13: screw_rise (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_13[] = {
	CG_FRAME(    0,    0,    6,           0,   290, 0x80,   0x41,                0,  0x00,          0,    0,   0,     0, 0x0113,   2 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   291, 0x80,   0x41,                0,  0x00,          0,    0,   0,     0, 0x0114,   2 ),
	CG_NEXT( pl01_catch_13 ),
};

/* Char_Catch 14: screw_bound (Spinning Pile Driver (ground and air)), screw_j_bound (Spinning Pile Driver (ground and air)), fab_g_shake (Final Atomic Buster (hyper)), uab_g_shake (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_14[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   292, 0x80,   0x41,                0,  0x00,          0,    0,   0,     0, 0x0114,   2 ),
	CG_NEXT( pl01_catch_14 ),
};

const Sint32 pl01_unused_15[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3,           0,   293, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   297, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   298, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   299, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   300, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	0x00004eac,
};

/* Char_Catch 15: screw_j_init (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_15[] = {
	CG_FRAME(    0,    0,    8,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl01_catch_15 ),
};

/* Char_Catch 16: screw_j_catch (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_16[] = {
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    8,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl01_catch_16 ),
};

/* Char_Catch 17: screw_j_catch (Spinning Pile Driver (ground and air)), fab_g_catch (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_17[] = {
	CG_FRAME(    0,    0,    2,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0115,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0116,   2 ),
	CG_NEXT( pl01_catch_17 ),
};

/* Char_Catch 18: screw_j_hold (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_18[] = {
	CG_FRAME(    0,    0,    8,           0,   283, 0x80,   0x04,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_FRAME(    0,    0,    8,           0,   284, 0x80,   0x04,                0,  0x00,          0,    0,   0,     0, 0x010d,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   285, 0x80,   0x04,                0,  0x00,          0,    0,   0,     0, 0x010e,   1 ),
	CG_NEXT( pl01_catch_18 ),
};

/* Char_Catch 19 */
const Sint32 pl01_catch_19[] = {
	CG_FRAME(    0,    1,    4,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x010f,   1 ),
	CG_FRAME(    0,    1,    4,           0,   287, 0x80,   0x00,             0x0d,  0x00,       0x1e,    0,   0,     0, 0x0110,   1 ),
	CG_FRAME(    0,    1,    4,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0111,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   289, 0x80,   0x00,             0x0e,  0x00,       0x1e,    0,   0,     0, 0x0112,   1 ),
	CG_NEXT( pl01_catch_19 ),
};

/* Char_Catch 20: screw_j_rise (Spinning Pile Driver (ground and air)) */
const Sint32 pl01_catch_20[] = {
	CG_FRAME(    0,    0,    3,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0113,   2 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   291, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0114,   2 ),
	CG_NEXT( pl01_catch_20 ),
};

/* Char_Catch 21 */
const Sint32 pl01_catch_21[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   292, 0x80,   0x41,                0,  0x00,          0,    0,   0,     0, 0x0114,   2 ),
	CG_NEXT( pl01_catch_21 ),
};

const Sint32 pl01_unused_16[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	0x0000505c,
};

/* Char_Catch 22: powerbomb_init (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_22[] = {
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  83 ),
};

const Sint32 pl01_catch_22_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   234, 0x80,   0x00,             0x1d,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,             0x1d,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl01_catch_22_loop ),
};

/* Char_Catch 61: powerbomb_init (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_61[] = {
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  83 ),
	CG_FRAME(    0,    0,    3,           0,   234, 0x80,   0x00,             0x1d,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,             0x1d,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
};

const Sint32 pl01_catch_61_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   234, 0x80,   0x00,             0x1d,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,             0x1d,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl01_catch_61_loop ),
};

/* Char_Catch 23: powerbomb_run (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_23[] = {
	CG_FRAME(    0,    0,    2,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   16,           0,   215, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl01_catch_23 ),
};

/* Char_Catch 24: powerbomb_dash (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_24[] = {
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0131,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0115,   1 ),
	CG_NEXT( pl01_catch_24 ),
};

/* Char_Catch 25: powerbomb_g_hold (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_25[] = {
	CG_FRAME(    0,    0,    3,           0,   271, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0101,   1 ),
	CG_FRAME(    0,    0,    6,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    5,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    5,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,    3,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    1,    3,           0,   277, 0x80,   0x86,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012e,   1 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012f,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   317, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0130,   1 ),
	CG_NEXT( pl01_catch_25 ),
};

/* Char_Catch 26: powerbomb_g_lift (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_26[] = {
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0118,   1 ),
	CG_FRAME(    0,    0,    7,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0119,   1 ),
	CG_FRAME(    0,    0,    3,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011a,   1 ),
	CG_FRAME(    0,    1,   16,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011b,   1 ),
	CG_FRAME(    0,    0,    8,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011c,   1 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x00,                0,  0x00,       0x03,    0,   0,     0, 0x011d,   1 ),
};

const Sint32 pl01_catch_26_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011e,   1 ),
	CG_NEXT( pl01_catch_26_loop ),
};

/* Char_Catch 27: powerbomb_g_land (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_27[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011f,   1 ),
	CG_NEXT( pl01_catch_27 ),
};

/* Char_Catch 28 */
const Sint32 pl01_catch_28[] = {
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0020,   1 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0021,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   325, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_28 ),
};

const Sint32 pl01_unused_17[] = {	/* nothing reaches it */
	0x00030000,
};

const Sint32 pl01_unused_18[] = {
	0x00000147,
};

const Sint32 pl01_unused_19[] = {
	0x00000080,
};

const Sint32 pl01_unused_20[] = {
	0x00000000,
};

const Sint32 pl01_unused_21[] = {
	0x00010000,
};

const Sint32 pl01_unused_22[] = {
	0x80050000,
};

const Sint32 pl01_unused_23[] = {
	0x00000146,
};

const Sint32 pl01_unused_24[] = {
	0x00000080,
};

const Sint32 pl01_unused_25[] = {
	0x00000000,
};

const Sint32 pl01_unused_26[] = {
	0x00010000,
};

/* Char_Catch 29: powerbomb_run (Flying Powerbomb (Atomic Suplex up close)), uab_run (Ultra Final Atomic Buster (hyper)), uab_g_catch (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_29[] = {
	CG_FRAME(    0,    0,    3,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   211, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0131,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0115,   1 ),
	CG_NEXT( pl01_catch_29 ),
};

/* Char_Catch 30: powerbomb_r_hold (Flying Powerbomb (Atomic Suplex up close)), uab_g_hold (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_30[] = {
	CG_FRAME(    0,    0,    3,           0,   283, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_FRAME(    0,    0,    3,           0,   284, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x010d,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   285, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x010e,   1 ),
	CG_NEXT( pl01_catch_30 ),
};

/* Char_Catch 31: powerbomb_r_jump (Flying Powerbomb (Atomic Suplex up close)) */
const Sint32 pl01_catch_31[] = {
	CG_FRAME(    0,    0,   10,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0122,   1 ),
	CG_FRAME(    0,    0,    3,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0123,   1 ),
	CG_FRAME(    0,    0,    3,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0124,   1 ),
	CG_FRAME(    0,    0,    3,           0,   246, 0x80,   0x00,             0x1a,  0x00,       0x03,    0,   0,     0, 0x0125,   1 ),
};

const Sint32 pl01_catch_31_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0126,   1 ),
	CG_NEXT( pl01_catch_31_loop ),
};

/* Char_Catch 32: powerbomb_r_land (Flying Powerbomb (Atomic Suplex up close)), uab_g_land (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_32[] = {
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0127,   1 ),
	CG_FRAME(    0,    0,    2,           0,   249, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0128,   1 ),
	CG_FRAME(    0,    0,    2,           0,   248, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0127,   1 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   249, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0128,   1 ),
	CG_NEXT( pl01_catch_32 ),
};

/* Char_Catch 33: screw_slam (Spinning Pile Driver (ground and air)), screw_j_slam (Spinning Pile Driver (ground and air)), powerbomb_g_bound (Flying Powerbomb (Atomic Suplex up close)), powerbomb_r_bound (Flying Powerbomb (Atomic Suplex up close)), ... */
const Sint32 pl01_catch_33[] = {
	CG_FRAME(    0,    0,    6,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_catch_33 ),
};

/* Char_Catch 34: screw_fall (Spinning Pile Driver (ground and air)), screw_j_fall (Spinning Pile Driver (ground and air)), powerbomb_g_fall (Flying Powerbomb (Atomic Suplex up close)), powerbomb_r_fall (Flying Powerbomb (Atomic Suplex up close)), ... */
const Sint32 pl01_catch_34[] = {
	CG_FRAME(    0,    0,   12,           0,    66, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    67, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl01_catch_34 ),
};

/* Char_Catch 6: dash_set */
const Sint32 pl01_catch_06[] = {
	CG_FRAME(    0,    0,    3,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_catch_06_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0115,   1 ),
	CG_NEXT( pl01_catch_06_loop ),
};

/* Char_Catch 35: fab_init (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_35[] = {
	CG_FRAME(    0, 0x80,    1,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  41 ),
	CG_FRAME(    0, 0x80,    3,           0,   233, 0x80,   0x00,             0x15,  0x00,       0x19,    0,   0,     0, 0x0100,  41 ),
	CG_FRAME(    0,    1,    1,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  41 ),
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  41 ),
};

const Sint32 pl01_catch_35_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   234, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   237, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   239, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl01_catch_35_loop ),
};

/* Char_Catch 36: fab_run (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_36[] = {
	CG_FRAME(    0,    0,    3,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    8,           0,   214, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   16,           0,   215, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl01_catch_36 ),
};

/* Char_Catch 37: fab_run (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_37[] = {
	CG_FRAME(    0,    0,    3,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   212, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0131,   1 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0, 0x0115,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0116,   1 ),
	CG_NEXT( pl01_catch_37 ),
};

/* Char_Catch 38: fab_g_hold (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_38[] = {
	CG_FRAME(    0,    0,    3,           0,   271, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0101,   1 ),
	CG_FRAME(    0,    0,    6,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    5,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    5,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,    3,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    1,    3,           0,   277, 0x80,   0x87,                1,  0x00,       0x14,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012e,   1 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012f,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   317, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0130,   1 ),
	CG_NEXT( pl01_catch_38 ),
};

/* Char_Catch 39: fab_g_lift (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_39[] = {
	CG_FRAME(    0,    0,    3,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    3,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    5,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    5,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,    3,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    1,    3,           0,   277, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012e,   1 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012f,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   317, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0130,   1 ),
	CG_NEXT( pl01_catch_39 ),
};

/* Char_Catch 40: fab_g_lift2 (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_40[] = {
	CG_FRAME(    0,    0,    6,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    5,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_NEXT( pl01_catch_40 ),
};

/* Char_Catch 42: fab_g_rise (Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_42[] = {
	CG_FRAME(    0,    0,    1,           0,   290, 0x80,   0x61,                0,  0x00,          0,    0,   0,     0, 0x0113,   2 ),
	CG_FRAME(    0,    1,    5,           0,   290, 0x80,   0x61,                2,  0x00,       0x15,    0,   0,     0, 0x0113,   2 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   291, 0x80,   0x61,                0,  0x00,          0,    0,   0,     0, 0x0114,   2 ),
	CG_NEXT( pl01_catch_42 ),
};

/* Char_Catch 43: fab_g_up (Final Atomic Buster (hyper)), uab_g_up (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_43[] = {
	CG_FRAME(    0,    0,    4,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    6,           0,    62, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl01_catch_43_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl01_catch_43_loop ),
};

/* Char_Catch 44: fab_g_air_hold (Final Atomic Buster (hyper)), uab_g_air_hold (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_44[] = {
	CG_FRAME(    0,    0,    8,           0,   283, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_FRAME(    0,    0,    8,           0,   284, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x010d,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   285, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x010e,   1 ),
	CG_NEXT( pl01_catch_44 ),
};

/* Char_Catch 45: fab_g_dive (Final Atomic Buster (hyper)), uab_g_dive (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_45[] = {
	CG_FRAME(    0,    1,    4,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x010f,   1 ),
	CG_FRAME(    0,    1,    4,           0,   287, 0x80,   0x00,             0x0d,  0x00,       0x1e,    0,   0,     0, 0x0110,   1 ),
	CG_FRAME(    0,    1,    4,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0111,   1 ),
	CG_FRAME(    0,    1,    4,           0,   289, 0x80,   0x00,             0x0e,  0x00,       0x1e,    0,   0,     0, 0x0112,   1 ),
};

const Sint32 pl01_catch_45_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x010f,   1 ),
	CG_FRAME(    0,    1,    2,           0,   287, 0x80,   0x00,             0x0d,  0x00,       0x1e,    0,   0,     0, 0x0110,   1 ),
	CG_FRAME(    0,    1,    2,           0,   288, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0111,   1 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   289, 0x80,   0x00,             0x0e,  0x00,       0x1e,    0,   0,     0, 0x0112,   1 ),
	CG_NEXT( pl01_catch_45_loop ),
};

/* Char_Catch 46: fab_g_drop (Final Atomic Buster (hyper)), uab_g_drop (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_46[] = {
	CG_FRAME(    0,    0,    1,           0,   290, 0x80,   0x61,                0,  0x00,          0,    0,   0,     0, 0x0113,   2 ),
	CG_FRAME(    0,    0,    5,           0,   290, 0x80,   0x61,                3,  0x00,       0x16,    0,   0,     0, 0x0113,   2 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   291, 0x80,   0x61,                0,  0x00,          0,    0,   0,     0, 0x0114,   2 ),
	CG_NEXT( pl01_catch_46 ),
};

/* Char_Catch 47: russian_init (Aerial Russian Slam) */
const Sint32 pl01_catch_47[] = {
	CG_FRAME(    0,    0,    4,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_47 ),
};

/* Char_Catch 48: russian_jump (Aerial Russian Slam) */
const Sint32 pl01_catch_48[] = {
	CG_FRAME(    0,    1,    1,           0,   255, 0x80,   0x00,                5,  0x00,       0x05,    0,   0,     0,      0,   1 ),
};

const Sint32 pl01_catch_48_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_NEXT( pl01_catch_48_loop ),
};

/* Char_Catch 49: russian_air (Aerial Russian Slam) */
const Sint32 pl01_catch_49[] = {
	CG_FRAME(    0,    0,    8,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0029,   2 ),
	CG_FRAME(    0,    0,    8,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x002a,   2 ),
	CG_FRAME(    0,    0,    8,           0,   252, 0x80,   0x00,                7,  0x00,          0,    0,   0,     0, 0x002b,   2 ),
	CG_FRAME(    0,    0,    8,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x002c,   2 ),
	CG_FRAME(    0,    1,   16, END_OF_DATA,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x002d,   2 ),
	CG_NEXT( pl01_catch_49 ),
};

/* Char_Catch 53: uab_init (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_53[] = {
	CG_FRAME(    0,    0,    1,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   233, 0x80,   0x00,             0x15,  0x00,       0x19,    0,   0,     0, 0x0100,  41 ),
	CG_NEXT( pl01_catch_53 ),
};

/* Char_Catch 55: uab_start (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_55[] = {
	CG_FRAME(    0,    0,    1,           0,   233, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,  41 ),
	CG_FRAME(    0,    0,    2,           0,   210, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_NEXT( pl01_catch_55 ),
};

/* Char_Catch 54: uab_run (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_54[] = {
	CG_FRAME(    0,    0,    2,           0,   211, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   22,           0,   214, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6,           0,   215, 0x80,   0x67,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   216, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl01_catch_54 ),
};

/* Char_Catch 56: uab_g_land2 (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_56[] = {
	CG_FRAME(    0,    0,    8,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011f,   1 ),
	CG_FRAME(    0,    0,    3,           0,   317, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0130,   1 ),
	CG_FRAME(    1,    0,    1,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0118,   1 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   332, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_NEXT( pl01_catch_56 ),
};

/* Char_Catch 57: uab_g_jump (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_57[] = {
	CG_FRAME(    0,    0,   10,           0,   243, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0122,   1 ),
	CG_FRAME(    0,    0,    3,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0123,   1 ),
	CG_FRAME(    0,    0,    3,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0124,   1 ),
	CG_FRAME(    0,    0,    3,           0,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0125,   1 ),
};

const Sint32 pl01_catch_57_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0126,   1 ),
	CG_NEXT( pl01_catch_57_loop ),
};

/* Char_Catch 58: uab_g_turn (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_58[] = {
	CG_FRAME(    0,    0,    6,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0102,   1 ),
	CG_FRAME(    0,    0,    5,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0103,   1 ),
	CG_FRAME(    0,    0,    5,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0104,   1 ),
	CG_FRAME(    0,    0,    3,           0,   275, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0105,   1 ),
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0106,   1 ),
	CG_FRAME(    0,    1,    3,           0,   277, 0x80,   0x86,                0,  0x00,          0,    0,   0,     0, 0x0107,   1 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012e,   1 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x012f,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   317, 0x80,   0x01,                0,  0x00,          0,    0,   0,     0, 0x0130,   1 ),
	CG_NEXT( pl01_catch_58 ),
};

/* Char_Catch 59: uab_g_lift (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_59[] = {
	CG_FRAME(    0,    0,    4,           0,   272, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0118,   1 ),
	CG_FRAME(    0,    0,    7,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0119,   1 ),
	CG_FRAME(    0,    0,    3,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011a,   1 ),
	CG_FRAME(    0,    1,   16,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011b,   1 ),
	CG_FRAME(    0,    0,    8,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011c,   1 ),
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011d,   1 ),
};

const Sint32 pl01_catch_59_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x011e,   1 ),
	CG_NEXT( pl01_catch_59_loop ),
};

/* Char_Catch 60: uab_g_turn2 (Ultra Final Atomic Buster (hyper)) */
const Sint32 pl01_catch_60[] = {
	CG_FRAME(    0,    0,    6,           0,   332, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010c,   1 ),
	CG_FRAME(    0,    0,    8,           0,   333, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010d,   1 ),
	CG_FRAME(    0,    0,    6,           0,   334, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0, 0x010e,   1 ),
	CG_FRAME(    0,    1,    2,           0,   336, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   335, 0x80,   0x07,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl01_catch_60 ),
};

/* Char_Caught 0 */
const Sint32 pl01_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl01_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl01_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl01_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl01_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl01_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl01_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl01_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl01_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl01_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   350, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl01_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl01_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   351, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl01_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl01_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   352, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl01_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl01_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   354, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl01_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   355, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl01_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   356, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl01_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   362, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl01_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl01_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   369, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl01_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   370, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl01_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   371, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl01_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   372, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl01_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   373, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl01_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   380, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl01_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   386, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl01_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   377, 0x80,   0xed,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl01_caught_21[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   364, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl01_caught_21 ),
};

const Sint32 pl01_unused_27[] = {	/* nothing reaches it */
	0x00000002,
};

const Sint32 pl01_unused_28[] = {
	0x0000018f,
};

const Sint32 pl01_unused_29[] = {
	0x00000003,
};

const Sint32 pl01_unused_30[] = {
	0x00000190,
};

const Sint32 pl01_unused_31[] = {
	0x00000002,
};

const Sint32 pl01_unused_32[] = {
	0x00000191,
};

const Sint32 pl01_unused_33[] = {
	0x00000002,
};

const Sint32 pl01_unused_34[] = {
	0x00000192,
};

const Sint32 pl01_unused_35[] = {
	0x00000002,
};

const Sint32 pl01_unused_36[] = {
	0x00000193,
};

const Sint32 pl01_unused_37[] = {
	0x00000002,
};

const Sint32 pl01_unused_38[] = {
	0x00000194,
};

const Sint32 pl01_unused_39[] = {
	0x00000002,
};

const Sint32 pl01_unused_40[] = {
	0x00000195,
};

const Sint32 pl01_unused_41[] = {
	0x00000001,
};

const Sint32 pl01_unused_42[] = {
	0x00000196,
};

const Sint32 pl01_unused_43[] = {
	0x00000001,
};

const Sint32 pl01_unused_44[] = {
	0x00000197,
};

const Sint32 pl01_unused_45[] = {
	0x00008001,
};

const Sint32 pl01_unused_46[] = {
	0x00000198,
};

const Sint32 pl01_unused_47[] = {
	0x0000631c,
};
