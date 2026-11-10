/* Psylocke (character 08): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_08      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_08     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl08.c) says which group
                   and number each move uses.
   pl08_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl08_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl08_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl08_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_08
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_08, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_08 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl08_stand_00[], pl08_stand_02[], pl08_stand_03[], pl08_stand_06[],
	pl08_unused_00[], pl08_jump_00[], pl08_jump_00_loop[], pl08_jump_01[], pl08_jump_01_loop[],
	pl08_jump_02[], pl08_jump_02_loop[], pl08_jump_03[], pl08_jump_06[], pl08_jump_09[],
	pl08_jump_07[], pl08_jump_10[], pl08_jump_08[], pl08_jump_11[], pl08_dash_00[], pl08_dash_02[],
	pl08_dash_01[], pl08_dash_03[], pl08_crouch_00[], pl08_crouch_01[], pl08_crouch_01_loop[],
	pl08_crouch_02[], pl08_turn_00[], pl08_turn_01[], pl08_guard_00[], pl08_guard_00_loop[],
	pl08_guard_02[], pl08_guard_01[], pl08_guard_04[], pl08_guard_05[], pl08_guard_07[],
	pl08_guard_07_loop[], pl08_guard_06[], pl08_air_guard_00[], pl08_air_guard_02[],
	pl08_air_guard_01[], pl08_stand_p_00[], pl08_unused_01[], pl08_stand_p_01[], pl08_unused_02[],
	pl08_stand_p_02[], pl08_stand_k_00[], pl08_stand_k_01[], pl08_stand_k_04[], pl08_stand_k_02[],
	pl08_stand_k_06[], pl08_stand_k_05[], pl08_stand_k_03[], pl08_crouch_p_00[], pl08_extra_03[],
	pl08_extra_04[], pl08_extra_04_loop[], pl08_crouch_p_01[], pl08_crouch_p_02[],
	pl08_crouch_k_00[], pl08_crouch_k_01[], pl08_crouch_k_02[], pl08_jump_p_00[], pl08_jump_p_01[],
	pl08_jump_p_02[], pl08_jump_k_00[], pl08_jump_k_01[], pl08_jump_k_02[], pl08_jump_k_02_loop[],
	pl08_unused_03[], pl08_jump_k_05[], pl08_unused_04[], pl08_unused_05[], pl08_jump_k_06[],
	pl08_jump_k_07[], pl08_special_47[], pl08_unused_06[], pl08_unused_07[], pl08_catch_00[],
	pl08_catch_00_loop[], pl08_catch_01[], pl08_catch_02[], pl08_catch_02_loop[], pl08_catch_03[],
	pl08_unused_08[], pl08_special_34[], pl08_special_35[], pl08_special_36[], pl08_special_37[],
	pl08_special_38[], pl08_special_39[], pl08_special_40[], pl08_special_41[], pl08_special_42[],
	pl08_special_43[], pl08_special_44[], pl08_special_45[], pl08_special_00[], pl08_special_01[],
	pl08_special_08[], pl08_special_04[], pl08_special_10[], pl08_special_03[], pl08_special_02[],
	pl08_special_11[], pl08_special_11_loop[], pl08_special_12[], pl08_special_12_loop[],
	pl08_special_14[], pl08_special_46[], pl08_special_46_loop[], pl08_damage_00[], pl08_caught_00[],
	pl08_damage_01[], pl08_caught_01[], pl08_damage_02[], pl08_caught_02[], pl08_damage_03[],
	pl08_caught_03[], pl08_damage_04[], pl08_caught_04[], pl08_damage_05[], pl08_caught_05[],
	pl08_damage_06[], pl08_caught_06[], pl08_damage_07[], pl08_caught_07[], pl08_damage_08[],
	pl08_caught_08[], pl08_damage_10[], pl08_caught_09[], pl08_damage_11[], pl08_damage_11_loop[],
	pl08_caught_10[], pl08_damage_12[], pl08_damage_12_loop[], pl08_caught_11[], pl08_damage_13[],
	pl08_caught_12[], pl08_damage_09[], pl08_damage_09_loop[], pl08_caught_21[], pl08_damage_16[],
	pl08_caught_13[], pl08_unused_09[], pl08_damage_17[], pl08_damage_17_loop[], pl08_caught_14[],
	pl08_damage_18[], pl08_caught_15[], pl08_damage_19[], pl08_caught_16[], pl08_damage_20[],
	pl08_caught_17[], pl08_damage_21[], pl08_damage_22[], pl08_damage_23[], pl08_damage_24[],
	pl08_damage_25[], pl08_damage_26[], pl08_damage_26_loop[], pl08_damage_33[], pl08_damage_27[],
	pl08_damage_28[], pl08_caught_18[], pl08_caught_19[], pl08_damage_29[], pl08_damage_31[],
	pl08_caught_20[], pl08_unused_10[], pl08_dizzy_00[], pl08_win_00[], pl08_win_00_loop[],
	pl08_win_01[], pl08_win_01_loop[], pl08_win_02[], pl08_win_02_loop[], pl08_win_03[],
	pl08_win_03_loop[], pl08_win_04[], pl08_win_04_loop[], pl08_appear_00[], pl08_appear_01[],
	pl08_appear_01_loop[], pl08_super_00[], pl08_super_02[], pl08_super_03[], pl08_super_03_loop[],
	pl08_super_04[], pl08_super_04_loop[], pl08_get_up_00[], pl08_get_up_01[], pl08_get_up_02[],
	pl08_get_up_03[], pl08_special_15[], pl08_special_15_loop[], pl08_special_16[],
	pl08_special_17[], pl08_special_19[], pl08_special_21[], pl08_special_18[], pl08_special_20[],
	pl08_special_22[], pl08_special_23[], pl08_special_24[], pl08_special_25[], pl08_special_26[],
	pl08_special_27[], pl08_special_28[], pl08_special_29[], pl08_special_30[], pl08_special_31[],
	pl08_special_32[], pl08_special_32_loop[], pl08_special_33[], pl08_unused_11[], pl08_unused_12[],
	pl08_unused_13[], pl08_unused_14[], pl08_unused_15[], pl08_unused_16[], pl08_unused_17[],
	pl08_extra_05[], pl08_extra_05_loop[], pl08_special_51[], pl08_extra_08[], pl08_extra_09[],
	pl08_special_05[], pl08_special_06[], pl08_grp27_00[], pl08_grp27_01[], pl08_extra_00[];
extern const Sint32 *const pl08_stand_tbl[], *const pl08_jump_tbl[], *const pl08_dash_tbl[],
	*const pl08_crouch_tbl[], *const pl08_turn_tbl[], *const pl08_guard_tbl[],
	*const pl08_air_guard_tbl[], *const pl08_stand_p_tbl[], *const pl08_stand_k_tbl[],
	*const pl08_crouch_p_tbl[], *const pl08_crouch_k_tbl[], *const pl08_jump_p_tbl[],
	*const pl08_jump_k_tbl[], *const pl08_damage_tbl[], *const pl08_dizzy_tbl[],
	*const pl08_catch_tbl[], *const pl08_caught_tbl[], *const pl08_get_up_tbl[],
	*const pl08_appear_tbl[], *const pl08_win_tbl[], *const pl08_extra_tbl[],
	*const pl08_special_tbl[], *const pl08_super_tbl[], *const pl08_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_08[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 750, 750, 0, 0, 0, 0, 0, 0 },
	{ 500, 500, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -16384, 0, 0, 0, 0, 0 },
	{ 750, 750, -16384, 0, 0, 0, 0, 0 },
	{ 500, 500, -16384, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_08[] = {
	pl08_stand_tbl, pl08_jump_tbl, pl08_dash_tbl, pl08_crouch_tbl, pl08_turn_tbl, pl08_guard_tbl,
	pl08_air_guard_tbl, pl08_stand_p_tbl, pl08_stand_k_tbl, pl08_crouch_p_tbl, pl08_crouch_k_tbl, pl08_jump_p_tbl,
	pl08_jump_k_tbl, pl08_damage_tbl, pl08_dizzy_tbl, pl08_catch_tbl, pl08_caught_tbl, pl08_get_up_tbl,
	pl08_appear_tbl, pl08_win_tbl, pl08_extra_tbl, pl08_special_tbl, pl08_super_tbl, pl08_super_tbl,
	pl08_super_tbl, pl08_super_tbl, pl08_stand_tbl, pl08_grp27_tbl, 0,
};

const Sint32 *const pl08_stand_tbl[] = {
	pl08_stand_00, pl08_stand_00, pl08_stand_02, pl08_stand_03, pl08_stand_00, pl08_stand_00,
	pl08_stand_06, pl08_stand_06, pl08_stand_06, pl08_stand_02, pl08_stand_03, 0,
};

const Sint32 *const pl08_jump_tbl[] = {
	pl08_jump_00, pl08_jump_01, pl08_jump_02, pl08_jump_03, pl08_jump_03, pl08_jump_03,
	pl08_jump_06, pl08_jump_07, pl08_jump_08, pl08_jump_09, pl08_jump_10, pl08_jump_11,
	0,
};

const Sint32 *const pl08_dash_tbl[] = {
	pl08_dash_00, pl08_dash_01, pl08_dash_02, pl08_dash_03, 0,
};

const Sint32 *const pl08_crouch_tbl[] = {
	pl08_crouch_00, pl08_crouch_01, pl08_crouch_02, 0,
};

const Sint32 *const pl08_turn_tbl[] = {
	pl08_turn_00, pl08_turn_01, 0,
};

const Sint32 *const pl08_guard_tbl[] = {
	pl08_guard_00, pl08_guard_01, pl08_guard_02, pl08_guard_00, pl08_guard_04, pl08_guard_05,
	pl08_guard_06, pl08_guard_07, 0,
};

const Sint32 *const pl08_air_guard_tbl[] = {
	pl08_air_guard_00, pl08_air_guard_01, pl08_air_guard_02, 0,
};

const Sint32 *const pl08_stand_p_tbl[] = {
	pl08_stand_p_00, pl08_stand_p_01, pl08_stand_p_02, 0,
};

const Sint32 *const pl08_stand_k_tbl[] = {
	pl08_stand_k_00, pl08_stand_k_01, pl08_stand_k_02, pl08_stand_k_03, pl08_stand_k_04, pl08_stand_k_05,
	pl08_stand_k_06, 0,
};

const Sint32 *const pl08_crouch_p_tbl[] = {
	pl08_crouch_p_00, pl08_crouch_p_01, pl08_crouch_p_02, 0,
};

const Sint32 *const pl08_crouch_k_tbl[] = {
	pl08_crouch_k_00, pl08_crouch_k_01, pl08_crouch_k_02, 0,
};

const Sint32 *const pl08_jump_p_tbl[] = {
	pl08_jump_p_00, pl08_jump_p_01, pl08_jump_p_02, pl08_jump_p_00, pl08_jump_p_01, pl08_jump_p_02,
	0,
};

const Sint32 *const pl08_jump_k_tbl[] = {
	pl08_jump_k_00, pl08_jump_k_01, pl08_jump_k_02, pl08_jump_k_00, pl08_jump_k_01, pl08_jump_k_05,
	pl08_jump_k_06, pl08_jump_k_07, 0,
};

const Sint32 *const pl08_damage_tbl[] = {
	pl08_damage_00, pl08_damage_01, pl08_damage_02, pl08_damage_03, pl08_damage_04, pl08_damage_05,
	pl08_damage_06, pl08_damage_07, pl08_damage_08, pl08_damage_09, pl08_damage_10, pl08_damage_11,
	pl08_damage_12, pl08_damage_13, pl08_damage_13, pl08_damage_13, pl08_damage_16, pl08_damage_17,
	pl08_damage_18, pl08_damage_19, pl08_damage_20, pl08_damage_21, pl08_damage_22, pl08_damage_23,
	pl08_damage_24, pl08_damage_25, pl08_damage_26, pl08_damage_27, pl08_damage_28, pl08_damage_29,
	pl08_damage_27, pl08_damage_31, pl08_damage_27, pl08_damage_33, pl08_damage_13, pl08_damage_33,
	0,
};

const Sint32 *const pl08_dizzy_tbl[] = {
	pl08_dizzy_00, 0,
};

const Sint32 *const pl08_catch_tbl[] = {
	pl08_catch_00, pl08_catch_01, pl08_catch_02, pl08_catch_03, 0,
};

const Sint32 *const pl08_caught_tbl[] = {
	pl08_caught_00, pl08_caught_01, pl08_caught_02, pl08_caught_03, pl08_caught_04, pl08_caught_05,
	pl08_caught_06, pl08_caught_07, pl08_caught_08, pl08_caught_09, pl08_caught_10, pl08_caught_11,
	pl08_caught_12, pl08_caught_13, pl08_caught_14, pl08_caught_15, pl08_caught_16, pl08_caught_17,
	pl08_caught_18, pl08_caught_19, pl08_caught_20, pl08_caught_21, 0,
};

const Sint32 *const pl08_get_up_tbl[] = {
	pl08_get_up_00, pl08_get_up_01, pl08_get_up_02, pl08_get_up_03, 0,
};

const Sint32 *const pl08_appear_tbl[] = {
	pl08_appear_00, pl08_appear_01, 0,
};

const Sint32 *const pl08_win_tbl[] = {
	pl08_win_00, pl08_win_01, pl08_win_02, pl08_win_03, pl08_win_04, pl08_win_04,
	0,
};

const Sint32 *const pl08_extra_tbl[] = {
	pl08_extra_00, pl08_extra_00, pl08_extra_00, pl08_extra_03, pl08_extra_04, pl08_extra_05,
	pl08_extra_00, pl08_extra_00, pl08_extra_08, pl08_extra_09, 0,
};

const Sint32 *const pl08_special_tbl[] = {
	pl08_special_00, pl08_special_01, pl08_special_02, pl08_special_03, pl08_special_04, pl08_special_05,
	pl08_special_06, pl08_special_06, pl08_special_08, pl08_special_04, pl08_special_10, pl08_special_11,
	pl08_special_12, pl08_special_12, pl08_special_14, pl08_special_15, pl08_special_16, pl08_special_17,
	pl08_special_18, pl08_special_19, pl08_special_20, pl08_special_21, pl08_special_22, pl08_special_23,
	pl08_special_24, pl08_special_25, pl08_special_26, pl08_special_27, pl08_special_28, pl08_special_29,
	pl08_special_30, pl08_special_31, pl08_special_32, pl08_special_33, pl08_special_34, pl08_special_35,
	pl08_special_36, pl08_special_37, pl08_special_38, pl08_special_39, pl08_special_40, pl08_special_41,
	pl08_special_42, pl08_special_43, pl08_special_44, pl08_special_45, pl08_special_46, pl08_special_47,
	pl08_extra_00, pl08_extra_00, pl08_extra_00, pl08_special_51, 0,
};

const Sint32 *const pl08_super_tbl[] = {
	pl08_super_00, pl08_super_00, pl08_super_02, pl08_super_03, pl08_super_04, 0,
};

const Sint32 *const pl08_grp27_tbl[] = {
	pl08_grp27_00, pl08_grp27_01, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1    se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Fly 0
   Char_Fly 1: act_fly */
const Sint32 pl08_stand_00[] = {
	CG_FRAME(    0,    0,   4,           0,    45, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    46, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    47, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    48, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    49, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    50, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    51, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    52, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    53, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    54, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,    55, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    56, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 9
   Char_Fly 2
   Char_Fly 9 */
const Sint32 pl08_stand_02[] = {
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    63, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    64, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    65, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    66, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    67, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    68, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    69, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    72, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    73, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    74, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    75, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    76, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    83, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    84, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    85, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    86, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    87, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 10
   Char_Fly 3
   Char_Fly 10 */
const Sint32 pl08_stand_03[] = {
	CG_FRAME(    0,    0,   2,           0,    88, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    96, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    97, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    98, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    99, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   100, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   101, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   102, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   103, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   104, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   111, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   112, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   113, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   114, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   115, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_stand_03 ),
};

/* Char_Stand 6
   Char_Stand 7
   Char_Stand 8
   Char_Fly 6
   Char_Fly 7
   Char_Fly 8 */
const Sint32 pl08_stand_06[] = {
	CG_FRAME(    0,    0,   2,           0,    77, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    78, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    82, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    80, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_stand_06 ),
};

const Sint32 pl08_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   110, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   109, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   108, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   107, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   106, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	0x00000b60,
};

/* Char_Jump 0 */
const Sint32 pl08_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   131, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   132, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2,           0,   133, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   134, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   1,           0,   135, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2,           0,   136, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2,           0,   137, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2,           0,   138, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   139, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   6,           0,   140, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   141, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl08_jump_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl08_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   131, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl08_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_jump_01_loop ),
};

/* Char_Jump 2: thrust_end (Psy-Thrust (hyper)), ninjitsu_end (Ninjutsu) */
const Sint32 pl08_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   131, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   158, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   159, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl08_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl08_jump_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   145, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl08_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   131, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   132, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   133, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   134, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   135, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   136, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   137, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   138, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   5,           0,   139, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,  10,           0,   140, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   5,           0,   141, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   142, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Jump 9: spin_move (Psy-Blade), maelstrom_air (Psy-Maelstrom (hyper)), leap_move (Leaping Slash), act_fall, ... */
const Sint32 pl08_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl08_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl08_jump_07[] = {
	CG_FRAME(    0,    1,   4,           0,   131, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   146, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   148, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   147, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   149, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   150, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   151, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   152, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   153, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl08_jump_10[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl08_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl08_jump_08[] = {
	CG_FRAME(    0,    1,   4,           0,   131, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   157, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   156, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   158, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   159, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl08_jump_11[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl08_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl08_dash_00[] = {
	CG_FRAME(    0,    1,   3,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   3,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   3,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   6,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl08_dash_00 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl08_dash_02[] = {
	CG_FRAME(    0,    1,   3,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    1,   3,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    1,   3, END_OF_DATA,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl08_dash_01[] = {
	CG_FRAME(    0,    1,   2,           0,   122, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   123, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   124, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   125, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   126, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   128, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl08_dash_01 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl08_dash_03[] = {
	CG_FRAME(    0,    1,   4,           0,   129, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   130, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl08_crouch_00[] = {
	CG_FRAME(    2,    0,   2,           0,   165, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    1,    0,   2,           0,   166, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   167, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    2,    0,   2,           0,    57, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    1,    0,   2,           0,    58, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl08_crouch_01[] = {
	CG_FRAME(    0,    0,   4,           0,    57, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4,           0,    58, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
};

const Sint32 pl08_crouch_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    59, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_crouch_01_loop ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl08_crouch_02[] = {
	CG_FRAME(    2,    0,   2,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,   2,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,   2,           0,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,   4, END_OF_DATA,    46, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl08_turn_00[] = {
	CG_FRAME(    0,    0,   5,           0,   171, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2,           0,   172, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   173, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl08_turn_01[] = {
	CG_FRAME(    0,    0,   5,           0,   175, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   176, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   178, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_turn_01 ),
};

/* Char_Guard 0
   Char_Guard 3 */
const Sint32 pl08_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   179, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  72 ),
};

const Sint32 pl08_guard_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   180, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl08_guard_00_loop ),
};

/* Char_Guard 2 */
const Sint32 pl08_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   179, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl08_guard_02 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl08_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   180, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl08_guard_01 ),
};

/* Char_Guard 4 */
const Sint32 pl08_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   182, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl08_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl08_guard_05 ),
};

/* Char_Guard 7 */
const Sint32 pl08_guard_07[] = {
	CG_FRAME(    0,    0,   2,           0,   183, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
};

const Sint32 pl08_guard_07_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   181, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl08_guard_07_loop ),
};

/* Char_Guard 6 */
const Sint32 pl08_guard_06[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   183, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl08_guard_06 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl08_air_guard_00[] = {
	CG_FRAME(    0,    0,   1,           0,   184, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl08_air_guard_02[] = {
	CG_FRAME(    0,    0,   1,           0,   185, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl08_air_guard_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   186, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl08_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   307, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   308, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    0,   1,           0,   309, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  78 ),
	CG_FRAME( 0x80,    0,   3,           0,   309, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    1,   4,           0,   309, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    1,   3,           0,   310, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0, 0x81,   2, END_OF_DATA,   311, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_stand_p_00 ),
};

const Sint32 pl08_unused_01[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   2,           0,   309, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x82,    0,   2, END_OF_DATA,   308, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  38 ),
	0x000017dc,
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl08_stand_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   312, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   1,           0,   313, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  62 ),
	CG_FRAME( 0x80,    0,   2,           0,   314, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x82,    0,   2,           0,   315, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  76 ),
	CG_FRAME(  -48,    0,  10,           0,   316, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    1,   5,           0,   317, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0, 0x81,   4, END_OF_DATA,   318, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl08_stand_p_01 ),
};

const Sint32 pl08_unused_02[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   2,           0,   315, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x82,    0,   2, END_OF_DATA,   314, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  37 ),
	0x000018ac,
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl08_stand_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   319, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   1,           0,   320, 0xa0,   0x4d,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   321, 0xa0,   0x6a,              ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   8,           0,   322, 0xa0,   0xa4,              ATK,  0x00,    0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,   3,           0,   323, 0xa0,   0xa4,              ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   324, 0xa0,   0xa4,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   325, 0xa0,   0xa4,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0xa0,   0x87,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0xa0,   0x4d,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl08_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   333, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   1,           0,   334, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME( 0x80,    0,   2,           0,   335, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  80 ),
	CG_FRAME( 0x80,    0,   2,           0,   335, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x80,    0,   2,           0,   334, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x82,    0,   2,           0,   335, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   332, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl08_stand_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   338, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   3,           0,   339, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,   1,           0,   340, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   1,           0,   341, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x80,    0,   1,           0,   340, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME( 0x82,    0,   1,           0,   341, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,   6,           0,   341, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4,           0,   342, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   343, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   344, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl08_stand_k_01 ),
};

/* Char_Stand_K 4 */
const Sint32 pl08_stand_k_04[] = {
	CG_FRAME(    0,    0,   2,           0,   146, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   429, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   1,           0,   430, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,   1,           0,   431, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME( 0x81,    0,   6,           0,   432, 0x90,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   3,           0,   433, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0, 0x80,   4,           0,   434, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,  10, END_OF_DATA,   145, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_stand_k_04 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl08_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   345, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   347, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,   1,           0,   348, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   349, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   348, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME( 0x82,    0,   6,           0,   349, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    2,   4,           0,   349, 0xa0,   0x61,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x82,    0,   4,           0,   351, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   352, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl08_stand_k_02 ),
};

/* Char_Stand_K 6: stand_kick_move */
const Sint32 pl08_stand_k_06[] = {
	CG_FRAME(    0,    0,   4,           0,   118, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   119, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   6,           0,   120, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   121, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_stand_k_06 ),
};

/* Char_Stand_K 5 */
const Sint32 pl08_stand_k_05[] = {
	CG_FRAME(    0,    0,  24,           0,   191, 0x40,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   351, 0x40,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl08_stand_k_02 ),
};

/* Char_Stand_K 3: stand_kick_move */
const Sint32 pl08_stand_k_03[] = {
	CG_FRAME(    0,    0,   1,           0,   345, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   1,           0,   347, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME( 0x80,    0,   1,           0,   348, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   349, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  85 ),
	CG_FRAME( 0x80,    0,   1,           0,   348, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME( 0x82,    0,   6,           0,   349, 0xa0,   0x61,              ATK,  0x00,    0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    2,   4,           0,   349, 0xa0,   0x61,              ATE,  0x00,    0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x82,    0,   4,           0,   351, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   352, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  43 ),
	CG_NEXT( pl08_stand_k_03 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl08_crouch_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   353, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   354, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   355, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   9, END_OF_DATA,   355, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl08_crouch_p_00 ),
};

/* Char_Extra 3: t_kick_init, fly_start5 */
const Sint32 pl08_extra_03[] = {
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 145 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   233, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 145 ),
	CG_NEXT( pl08_extra_03 ),
};

/* Char_Extra 4: t_kick_jump */
const Sint32 pl08_extra_04[] = {
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,              ATE,  0x00, 0x0a,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl08_extra_04_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   245, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   246, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   247, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   248, 0x80,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_extra_04_loop ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl08_crouch_p_01[] = {
	CG_FRAME(    0,    0,   3,           0,   356, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   6,           0,   358, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   4,           0,   360, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   5,           0,   361, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    1,   6,           0,   362, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   363, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl08_crouch_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   364, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   369, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   370, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   7,           0,   372, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   373, 0xa0,   0x0c,              ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl08_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl08_crouch_k_00[] = {
	CG_FRAME(    0,    0,   1,           0,   374, 0xa0,   0x08,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   2,           0,   374, 0xa0,   0x08,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   375, 0xa0,   0x24,              ATK,  0x00,    0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0xa0,   0x24,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2,           0,   376, 0xa0,   0x08,              ATK,  0x00,    0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    1,   2,           0,   377, 0xa0,   0x08,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   176, 0xa0,   0x08,              ATK,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl08_crouch_k_01[] = {
	CG_FRAME(    0,    0,   1,           0,   378, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   1,           0,   380, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   1,           0,   381, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x80,    0,   1,           0,   380, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  91 ),
	CG_FRAME( 0x81,    0,   4,           0,   381, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  92 ),
	CG_FRAME( 0x80,    0,   4,           0,   382, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x81,    0,   2,           0,   383, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   378, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl08_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl08_crouch_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   385, 0x30,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   386, 0x30,   0x66,              ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   387, 0x30,   0x66,              ATK,  0x00,    0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   1,           0,   388, 0x30,   0x48,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0xa0,   0x48,              ATE,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0xa0,   0x0b,              ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   5,           0,   391, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   392, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   393, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl08_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl08_jump_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   397, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   1,           0,   398, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  94 ),
	CG_FRAME( 0x81,    0,  12,           0,   399, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   397, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl08_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl08_jump_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   401, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   402, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   405, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl08_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl08_jump_p_02[] = {
	CG_FRAME(    0,    0,   3,           0,   406, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   407, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   408, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   1,           0,   409, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,   1,           0,   410, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,   1,           0,   411, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,   1,           0,   412, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,   1,           0,   413, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   1,           0,   414, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x80,    0,   1,           0,   413, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME( 0x82,    0,   1,           0,   414, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   8,           0,   415, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   406, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl08_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl08_jump_k_00[] = {
	CG_FRAME(    0,    0,   5,           0,   416, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   1,           0,   417, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 141 ),
	CG_FRAME( 0x81,    0,  12,           0,   418, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   6,           0,   418, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   420, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl08_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl08_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   422, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   1,           0,   423, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,  10,           0,   424, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   3,           0,   426, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   427, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_NEXT( pl08_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl08_jump_k_02[] = {
	CG_FRAME(    0,    0,   6,           0,   428, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   5,           0,   429, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   1,           0,   430, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(  -42,    0,   1,           0,   431, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 101 ),
	CG_FRAME( 0x82,    0,   7, END_OF_DATA,   432, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl08_jump_k_02_loop ),
};

const Sint32 pl08_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   433, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA,   434, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl08_jump_k_02 ),
};

const Sint32 pl08_unused_03[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   1,           0,   431, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME( 0x82,    0,   2, END_OF_DATA,   432, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl08_jump_k_02_loop ),
};

/* Char_Jump_K 5 */
const Sint32 pl08_jump_k_05[] = {
	CG_FRAME(    0,    0,   3,           0,   435, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   2,           0,   436, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 134 ),
	CG_FRAME(  -28,    0,   3,           0,   437, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 104 ),
	CG_FRAME( 0x80,    0,   0,           0,   438, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   4,           0,   439, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(  -32,    0,   3,           0,   440, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x82,    0,   4,           0,   441, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   6, END_OF_DATA,   443, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl08_jump_k_05 ),
};

const Sint32 pl08_unused_04[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   1,           0,   438, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x82,    0,   1, END_OF_DATA,   437, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	0x000026d4,
};

const Sint32 pl08_unused_05[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   1,           0,   441, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x82,    0,   1, END_OF_DATA,   440, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	0x00002710,
};

/* Char_Jump_K 6 */
const Sint32 pl08_jump_k_06[] = {
	CG_FRAME(    0,    0,   4,           0,   428, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   429, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   430, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   431, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   432, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   433, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   434, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_jump_k_06 ),
};

/* Char_Jump_K 7 */
const Sint32 pl08_jump_k_07[] = {
	CG_FRAME(    0,    0,   4,           0,   435, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   4,           0,   440, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   443, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_NEXT( pl08_jump_k_07 ),
};

/* Char_Special 47: leap_init (Leaping Slash) */
const Sint32 pl08_special_47[] = {
	CG_FRAME(    0,    0,   2,           0,   435, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,   2,           0,   436, 0xa0,   0x13,              ATK,  0x00, 0x03,    0,   0,     0,      0, 134 ),
	CG_FRAME(  -28,    0,   3,           0,   437, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 104 ),
	CG_FRAME( 0x80,    0,   0,           0,   438, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    1,   4,           0,   439, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(  -32,    0,   3,           0,   440, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 105 ),
	CG_FRAME( 0x80,    0,   4,           0,   441, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0, 0x80,   4, END_OF_DATA,   443, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl08_special_47 ),
};

const Sint32 pl08_unused_06[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   1,           0,   438, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME( 0x82,    0,   1, END_OF_DATA,   437, 0xa0,   0x13,              ATK,  0x00,    0,    0,   0,     0,      0,  34 ),
	0x0000292c,
};

const Sint32 pl08_unused_07[] = {	/* nothing reaches it */
	CG_FRAME( 0x80,    0,   1,           0,   441, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x82,    0,   1, END_OF_DATA,   440, 0xa0,   0x13,              ATE,  0x00,    0,    0,   0,     0,      0,  35 ),
	0x00002968,
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl08_catch_00[] = {
	CG_FRAME(    0,    1,   5,           0,   475, 0x80,   0x00,                0,  0x00, 0x03,    0,   0,     0, 0x0001,   3 ),
	CG_FRAME(    0,    1,   9,           0,   475, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0002,   3 ),
	CG_FRAME(    0,    0,   3,           0,   476, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0003,   4 ),
	CG_FRAME(    0,    0,   3,           0,   477, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0004,   5 ),
	CG_FRAME(    0,    0,   3,           0,   478, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0005,   6 ),
	CG_FRAME(    0,    0,   3,           0,   479, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0006,   7 ),
	CG_FRAME(    0,    0,   3,           0,   480, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0007,   8 ),
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0008,   9 ),
	CG_FRAME(    0,    0,   3,           0,   482, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0009,  10 ),
	CG_FRAME(    0,    0,   3,           0,   483, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x000a,   3 ),
	CG_FRAME(    0,    0,   3,           0,   484, 0x80,   0x00,                0,  0x00, 0x05,    0,   0,     0, 0x000b,   4 ),
	CG_FRAME(    0,    0,   3,           0,   485, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x000c,   5 ),
	CG_FRAME(    0, 0x81,   2,           0,   478, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0005,   6 ),
	CG_FRAME(    0,    2,   2,           0,   479, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0006,   7 ),
};

const Sint32 pl08_catch_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   486, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl08_catch_00_loop ),
};

/* Char_Catch 1 */
const Sint32 pl08_catch_01[] = {
	CG_FRAME(    0,    0,   3,           0,   486, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   487, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl08_catch_01 ),
};

/* Char_Catch 2: nml_k_set */
const Sint32 pl08_catch_02[] = {
	CG_FRAME(    0,    0,  12,           0,   488, 0x80,   0x00,                0,  0x00, 0x03,    0,   0,     0, 0x0010,  19 ),
	CG_FRAME(    0,    0,   3,           0,   489, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0011,  20 ),
	CG_FRAME(    0,    0,   8,           0,   490, 0x80,   0x00,                0,  0x00, 0x05,    0,   0,     0, 0x0012,  21 ),
	CG_FRAME(    0,    0,   1,           0,   491, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0013,  22 ),
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0014,  23 ),
	CG_FRAME(    0,    1,   4,           0,   493, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0, 0x0015,  24 ),
	CG_FRAME(    0,    2,   2,           0,   494, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  26 ),
};

const Sint32 pl08_catch_02_loop[] = {
	CG_FRAME(    0,    2,   4, END_OF_DATA,   495, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  29 ),
	CG_NEXT( pl08_catch_02_loop ),
};

/* Char_Catch 3 */
const Sint32 pl08_catch_03[] = {
	CG_FRAME(    0,    0,  14,           0,   444, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4,           0,   160, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4,           0,   161, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   164, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  36 ),
	CG_NEXT( pl08_catch_03 ),
};

const Sint32 pl08_unused_08[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   446, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   447, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,   446, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   448, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  51 ),
	0x00002ccc,
};

/* Char_Special 34 */
const Sint32 pl08_special_34[] = {
	CG_FRAME(    0,    0,   1,           0,   556, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   557, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0,   558, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   559, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl08_special_34 ),
};

/* Char_Special 35 */
const Sint32 pl08_special_35[] = {
	CG_FRAME(    0,    0,   1,           0,   560, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   561, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    1,   1,           0,   562, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   563, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl08_special_35 ),
};

/* Char_Special 36 */
const Sint32 pl08_special_36[] = {
	CG_FRAME(    0,    0,   1,           0,   564, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1,           0,   565, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,   1,           0,   566, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   567, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl08_special_36 ),
};

/* Char_Special 37 */
const Sint32 pl08_special_37[] = {
	CG_FRAME(    0,    0,   1,           0,   568, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   569, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    1,   1,           0,   570, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   571, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl08_special_37 ),
};

/* Char_Special 38 */
const Sint32 pl08_special_38[] = {
	CG_FRAME(    0,    0,   1,           0,   572, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1,           0,   573, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    1,   1,           0,   574, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   575, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl08_special_38 ),
};

/* Char_Special 39 */
const Sint32 pl08_special_39[] = {
	CG_FRAME(    0,    0,   1,           0,   576, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1,           0,   577, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    1,   1,           0,   578, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   579, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl08_special_39 ),
};

/* Char_Special 40 */
const Sint32 pl08_special_40[] = {
	CG_FRAME(    0,    0,   1,           0,   580, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1,           0,   581, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    1,   1,           0,   582, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   583, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_NEXT( pl08_special_40 ),
};

/* Char_Special 41 */
const Sint32 pl08_special_41[] = {
	CG_FRAME(    0,    0,   1,           0,   584, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1,           0,   585, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    1,   1,           0,   586, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   587, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_NEXT( pl08_special_41 ),
};

/* Char_Special 42 */
const Sint32 pl08_special_42[] = {
	CG_FRAME(    0,    0,   2,           0,   591, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   592, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   591, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   593, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_special_42 ),
};

/* Char_Special 43: ninjitsu_init (Ninjutsu) */
const Sint32 pl08_special_43[] = {
	CG_FRAME(    0,    0,   3,           0,   591, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   592, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   593, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   594, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   595, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   596, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   597, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   598, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   599, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   600, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   601, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_special_43 ),
};

/* Char_Special 44: ninjitsu_wait (Ninjutsu) */
const Sint32 pl08_special_44[] = {
	CG_FRAME(    0,    0,   4,           0,   863, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   862, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   861, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   860, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   859, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl08_special_44 ),
};

/* Char_Special 45: pls08_00_init */
const Sint32 pl08_special_45[] = {
	CG_FRAME(    0,    0,   3,           0, 32774, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32773, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32772, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32771, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32770, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4,           0, 32768, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32769, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32770, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32771, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32772, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32773, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32774, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_special_45 ),
};

/* Char_Special 0 */
const Sint32 pl08_special_00[] = {
	CG_FRAME(    0,    0,   3,           0,   523, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   524, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   525, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   1,           0,   526, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,   2,           0,   527, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   530, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   531, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3,           0,   532, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   533, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl08_special_01[] = {
	CG_FRAME(    0,    0,   3,           0,   523, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   3,           0,   524, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   1,           0,   525, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   1,           0,   526, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    1,   2,           0,   527, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   528, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   529, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   530, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   531, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl08_special_01 ),
};

/* Char_Special 8 */
const Sint32 pl08_special_08[] = {
	CG_FRAME(    0,    0,   2,           0, 32790, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32791, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32792, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32793, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32794, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32795, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32796, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_special_08 ),
};

/* Char_Special 4
   Char_Special 9 */
const Sint32 pl08_special_04[] = {
	CG_FRAME(    0,    0,   2,           0, 32779, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32785, 0xb0,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_special_04 ),
};

/* Char_Special 10 */
const Sint32 pl08_special_10[] = {
	CG_FRAME(    0,    0,   2,           0,   894, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   895, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   896, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   897, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   898, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   899, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   900, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_special_10 ),
};

/* Char_Special 3 */
const Sint32 pl08_special_03[] = {
	CG_FRAME(    0,    0,   2,           0, 32775, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2,           0, 32777, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32778, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl08_special_03 ),
};

/* Char_Special 2 */
const Sint32 pl08_special_02[] = {
	CG_FRAME(    0,    0,   2,           0, 32786, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32789, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 136 ),
	CG_NEXT( pl08_special_02 ),
};

/* Char_Special 11: spin_set_11 (Psy-Blade) */
const Sint32 pl08_special_11[] = {
	CG_FRAME(    0,    0,   3,           0,   537, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_special_11_loop[] = {
	CG_FRAME( 0x80,    0,   1,           0,   538, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x81,    0,   2,           0,   539, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x80,    0,   1,           0,   540, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x81,    1,   2,           0,   539, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x80,    1,   1,           0,   541, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x81,    1,   2,           0,   542, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x80,    1,   1,           0,   543, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x81,    1,   2,           0,   542, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 138 ),
	CG_FRAME( 0x80,    1,   1,           0,   544, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME( 0x81,    1,   2,           0,   545, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME( 0x80,    1,   1,           0,   546, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME( 0x81,    1,   1,           0,   545, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 127 ),
	CG_FRAME( 0x80,    1,   1,           0,   547, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x81,    1,   1,           0,   548, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x80,    1,   1,           0,   549, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x81,    1,   1,           0,   548, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x80,    1,   1,           0,   550, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x81,    1,   1,           0,   551, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x80,    1,   1,           0,   552, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x81,    1,   1,           0,   551, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 137 ),
	CG_FRAME( 0x80,    1,   1,           0,   553, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 123 ),
	CG_FRAME( 0x81,    1,   1,           0,   554, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 123 ),
	CG_FRAME( 0x80,    1,   1,           0,   555, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 123 ),
	CG_FRAME( 0x81,    1,   1,           0,   554, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 123 ),
	CG_FRAME( 0x80,    1,   2, END_OF_DATA,   554, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 123 ),
	CG_NEXT( pl08_special_11_loop ),
};

/* Char_Special 12: spin_set_12 (Psy-Blade)
   Char_Special 13: spin_set_13 (Psy-Blade) */
const Sint32 pl08_special_12[] = {
	CG_FRAME(    0,    0,   3,           0,   537, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_special_12_loop[] = {
	CG_FRAME( 0x80,    0,   1,           0,   538, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x81,    0,   2,           0,   539, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x80,    0,   1,           0,   540, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x81,    1,   2,           0,   539, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 109 ),
	CG_FRAME( 0x80,    1,   1,           0,   541, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME( 0x81,    1,   2,           0,   542, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME( 0x80,    1,   1,           0,   543, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME( 0x81,    1,   2,           0,   542, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 110 ),
	CG_FRAME( 0x80,    1,   1,           0,   544, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 122 ),
	CG_FRAME( 0x81,    1,   2,           0,   545, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 122 ),
	CG_FRAME( 0x80,    1,   1,           0,   546, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 122 ),
	CG_FRAME( 0x80,    1,   1,           0,   545, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 122 ),
	CG_FRAME( 0x80,    1,   1,           0,   547, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 124 ),
	CG_FRAME( 0x80,    1,   1,           0,   548, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 124 ),
	CG_FRAME( 0x80,    1,   1,           0,   549, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 124 ),
	CG_FRAME( 0x80,    1,   1,           0,   548, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 124 ),
	CG_FRAME( 0x80,    1,   1,           0,   550, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    1,   1,           0,   551, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    1,   1,           0,   552, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    1,   1,           0,   551, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    1,   1,           0,   553, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME( 0x80,    1,   1,           0,   554, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME( 0x80,    1,   1,           0,   555, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME( 0x80,    1,   1,           0,   554, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_FRAME( 0x81,    1,   2, END_OF_DATA,   554, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 139 ),
	CG_NEXT( pl08_special_12_loop ),
};

/* Char_Special 14: spin_fall (Psy-Blade), thrust_fall (Psy-Thrust (hyper)), ninjitsu_dash (Ninjutsu), ninjitsu_jump (Ninjutsu), ... */
const Sint32 pl08_special_14[] = {
	CG_FRAME(    0,    0,   5,           0,   235, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   3,           0,   236, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_special_14 ),
};

/* Char_Special 46: maelstrom_init (Psy-Maelstrom (hyper)) */
const Sint32 pl08_special_46[] = {
	CG_FRAME(    0,    0,   4,           0,   195, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   6,           0,   450, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   1,           0,   707, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
};

const Sint32 pl08_special_46_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   701, 0xb0,   0x00,              ATK,  0x00, 0x21,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   1,           0,   702, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   2,           0,   703, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   1,           0,   704, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    1,   2,           0,   705, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 140 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   706, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl08_special_46_loop ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl08_damage_00[] = {
	CG_FRAME( 0x80,    0,   8,           0,   708, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   709, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_damage_00 ),
};

/* Char_Caught 0 */
const Sint32 pl08_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   708, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl08_caught_00 ),
};

/* Char_Damage 1 */
const Sint32 pl08_damage_01[] = {
	CG_FRAME(    0,    0,  18, END_OF_DATA,   711, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_damage_01 ),
};

/* Char_Caught 1 */
const Sint32 pl08_caught_01[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   711, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl08_caught_01 ),
};

/* Char_Damage 2 */
const Sint32 pl08_damage_02[] = {
	CG_FRAME( 0x80,    0,   6,           0,   712, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x82,    0,   4,           0,   713, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   714, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   6,           0,   715, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   716, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_damage_02 ),
};

/* Char_Caught 2 */
const Sint32 pl08_caught_02[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   712, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl08_caught_02 ),
};

/* Char_Damage 3 */
const Sint32 pl08_damage_03[] = {
	CG_FRAME(    0,    0,   8,           0,   717, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   718, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_damage_03 ),
};

/* Char_Caught 3 */
const Sint32 pl08_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   717, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl08_caught_03 ),
};

/* Char_Damage 4 */
const Sint32 pl08_damage_04[] = {
	CG_FRAME( 0x80,    0,  10,           0,   719, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x80,    0,   4,           0,   720, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   721, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_damage_04 ),
};

/* Char_Caught 4 */
const Sint32 pl08_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   719, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl08_caught_04 ),
};

/* Char_Damage 5 */
const Sint32 pl08_damage_05[] = {
	CG_FRAME( 0x80,    0,  12,           0,   722, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x80,    0,   4,           0,   723, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x82,    0,   4,           0,   724, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   725, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_damage_05 ),
};

/* Char_Caught 5 */
const Sint32 pl08_caught_05[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   722, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl08_caught_05 ),
};

/* Char_Damage 6 */
const Sint32 pl08_damage_06[] = {
	CG_FRAME( 0x80,    0,  11, END_OF_DATA,   726, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_damage_06 ),
};

/* Char_Caught 6 */
const Sint32 pl08_caught_06[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   726, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl08_caught_06 ),
};

/* Char_Damage 7 */
const Sint32 pl08_damage_07[] = {
	CG_FRAME(    0,    0,  14,           0,   727, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   726, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_damage_07 ),
};

/* Char_Caught 7 */
const Sint32 pl08_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   727, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl08_caught_07 ),
};

/* Char_Damage 8 */
const Sint32 pl08_damage_08[] = {
	CG_FRAME( 0x80,    0,  10,           0,   728, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,  12,           0,   729, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x82,    0,   2, END_OF_DATA,   730, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl08_damage_08 ),
};

/* Char_Caught 8 */
const Sint32 pl08_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   728, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl08_caught_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl08_damage_10[] = {
	CG_FRAME(    0,    0,   6,           0,   731, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   732, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_damage_10 ),
};

/* Char_Caught 9 */
const Sint32 pl08_caught_09[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   731, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_09 ),
};

/* Char_Damage 11 */
const Sint32 pl08_damage_11[] = {
	CG_FRAME( 0x80,    0,   4,           0,   733, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   3,           0,   734, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl08_damage_11_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   735, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_damage_11_loop ),
};

/* Char_Caught 10 */
const Sint32 pl08_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   733, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_10 ),
};

/* Char_Damage 12 */
const Sint32 pl08_damage_12[] = {
	CG_FRAME( 0x80,    0,   4,           0,   738, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,   2,           0,   737, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl08_damage_12_loop[] = {
	CG_FRAME( 0x80,    0,   4, END_OF_DATA,   736, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_damage_12_loop ),
};

/* Char_Caught 11 */
const Sint32 pl08_caught_11[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   736, 0x40,   0x13,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_11 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl08_damage_13[] = {
	CG_FRAME( 0x80,    0,   8, END_OF_DATA,   740, 0x40,   0x6b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl08_damage_13 ),
};

/* Char_Caught 12 */
const Sint32 pl08_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   740, 0x40,   0x6b,                0,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_NEXT( pl08_caught_12 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl08_damage_09[] = {
	CG_FRAME(    0,    0,   8,           0,   717, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   742, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   743, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   744, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   745, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   746, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   747, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   3,           0,   748, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl08_damage_09_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   747, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl08_damage_09_loop ),
};

/* Char_Caught 21 */
const Sint32 pl08_caught_21[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   746, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl08_caught_21 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl08_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   750, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   751, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_16 ),
};

/* Char_Caught 13 */
const Sint32 pl08_caught_13[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   751, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_13 ),
};

const Sint32 pl08_unused_09[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   750, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   751, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	0x000041cc,
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl08_damage_17[] = {
	CG_FRAME(    0,    0,   2,           0,   752, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

const Sint32 pl08_damage_17_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   753, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_17_loop ),
};

/* Char_Caught 14 */
const Sint32 pl08_caught_14[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   753, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_14 ),
};

/* Char_Damage 18 */
const Sint32 pl08_damage_18[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   754, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_18 ),
};

/* Char_Caught 15 */
const Sint32 pl08_caught_15[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   801, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_15 ),
};

/* Char_Damage 19 */
const Sint32 pl08_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   755, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_19 ),
};

/* Char_Caught 16 */
const Sint32 pl08_caught_16[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   755, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_16 ),
};

/* Char_Damage 20 */
const Sint32 pl08_damage_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   759, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_20 ),
};

/* Char_Caught 17 */
const Sint32 pl08_caught_17[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   759, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl08_caught_17 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl08_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   750, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2,           0,   751, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Damage 22 */
const Sint32 pl08_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   752, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   2,           0,   753, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Damage 23 */
const Sint32 pl08_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   754, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Damage 24 */
const Sint32 pl08_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   755, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
};

/* Char_Damage 25 */
const Sint32 pl08_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   759, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl08_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   760, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   4,           0,   762, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   2,           0,   763, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
};

const Sint32 pl08_damage_26_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   764, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl08_damage_26_loop ),
};

/* Char_Damage 33: act_escape
   Char_Damage 35 */
const Sint32 pl08_damage_33[] = {
	CG_FRAME(    0,    0,   4,           0,   259, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   260, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   351, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_33 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl08_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   158, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   159, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3,           0,   162, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   163, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl08_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl08_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   770, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   771, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   772, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   773, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   774, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   775, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_damage_28 ),
};

/* Char_Caught 18 */
const Sint32 pl08_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   770, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl08_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl08_caught_19[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   826, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl08_caught_19 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl08_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   776, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   777, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   778, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   779, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   780, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   781, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl08_damage_31[] = {
	CG_FRAME(    0,    0,  -1,           0,   795, 0x60,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   795, 0x40,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  17 ),
	CG_NEXT( pl08_damage_31 ),
};

/* Char_Caught 20 */
const Sint32 pl08_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   795, 0x60,   0x8b,                0,  0x00,    0,    0,   0,     0,      0,  18 ),
	CG_NEXT( pl08_caught_20 ),
};

const Sint32 pl08_unused_10[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   756, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   757, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   759, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   760, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   761, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl08_damage_13 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl08_dizzy_00[] = {
	CG_FRAME(    0,    0,   2,           0,   837, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   838, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   837, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   838, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   839, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   840, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   839, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   841, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   842, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   843, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   842, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   844, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   842, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   843, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   845, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   846, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   848, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   849, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   847, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   850, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   851, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   850, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   851, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   853, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   854, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   853, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   855, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   853, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   856, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   857, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   856, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   857, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   856, 0x50,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_dizzy_00 ),
};

/* Char_Win 0 */
const Sint32 pl08_win_00[] = {
	CG_FRAME(    0,    0,   6,           0,   262, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   9,           0,   263, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   264, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   265, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  32,           0,   266, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   9,           0,   267, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   268, 0x80,   0x15,                0,  0x00, 0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   269, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   270, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   273, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   274, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   275, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   276, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   277, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_win_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   279, 0x80,   0x15,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl08_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   281, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   288, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   289, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_win_01_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   290, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   291, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   292, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   293, 0x80,   0x11,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl08_win_02[] = {
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   295, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   297, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   298, 0x80,   0x7f,                0,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   300, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   302, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   303, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_win_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   306, 0x80,   0x7f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_win_02_loop ),
};

/* Char_Win 3: pl08_win */
const Sint32 pl08_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,   262, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   263, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x80,   0x20,              ATK,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_win_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   258, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_win_03_loop ),
};

/* Char_Win 4: pl08_win
   Char_Win 5: pl08_win */
const Sint32 pl08_win_04[] = {
	CG_FRAME(    0,    0,   4,           0,   225, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   226, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   227, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   230, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_win_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   231, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_win_04_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl08_appear_00[] = {
	CG_FRAME(    0,    0,   4,           0,   232, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   233, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_appear_00 ),
};

/* Char_Appear 1: appear_fall */
const Sint32 pl08_appear_01[] = {
	CG_FRAME(    0,    0,   4,           0,   234, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   235, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   236, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   237, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   238, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   250, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   239, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   242, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   243, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   244, 0x80,   0x00,                0,  0x00, 0x0a,    0,   0,     0,      0,   0 ),
};

const Sint32 pl08_appear_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_appear_01_loop ),
};

/* Char_Super 0
   Char_Super 1
   Char_Effect 0
   Char_Effect 1
   Char_Change 0: act_change_tag_init
   Char_Change 1: act_change_vcounter_init */
const Sint32 pl08_super_00[] = {
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   242, 0x80,   0x6f,                0,  0x00, 0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   244, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   245, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   246, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   247, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   248, 0x80,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_super_00 ),
};

/* Char_Super 2
   Char_Effect 2
   Char_Change 2: act_change_out_bound
   group 25 2 */
const Sint32 pl08_super_02[] = {
	CG_FRAME(    0,    0,   4,           0,   143, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   144, 0x90,   0x13,                0,  0x00,    0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl08_super_02 ),
};

/* Char_Super 3
   Char_Effect 3
   Char_Change 3: act_stand, act_change_call
   group 25 3 */
const Sint32 pl08_super_03[] = {
	CG_FRAME(    0,    0,   2,           0,   213, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   214, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   215, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x2f,                0,  0x00, 0x0a,    0,   0,     0,      0,  71 ),
};

const Sint32 pl08_super_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   217, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,  11, END_OF_DATA,   213, 0x80,   0x2f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_super_03_loop ),
};

/* Char_Super 4
   Char_Effect 4
   Char_Change 4: act_change_assist_end
   group 25 4 */
const Sint32 pl08_super_04[] = {
	CG_FRAME(    0,    0,   4,           0,   263, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x80,   0x20,                0,  0x00, 0x0a,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl08_super_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   256, 0x80,   0x20,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_super_04_loop ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl08_get_up_00[] = {
	CG_FRAME(    0,    0,   5,           0,   187, 0x40,   0x6f,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   5,           0,   188, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   5,           0,   189, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   5,           0,   190, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   5,           0,   169, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   170, 0x40,   0x00,                0,  0x00,    0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl08_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl08_get_up_01[] = {
	CG_FRAME(    0,    1,   2,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl08_get_up_02[] = {
	CG_FRAME(    0,    1,   2,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl08_get_up_03[] = {
	CG_FRAME(    0,    1,   2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   121, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   120, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   119, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   118, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   117, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   116, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   168, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2,           0,   169, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   170, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_get_up_03 ),
};

/* Char_Special 15: butterfly_init (Kocho-Gakure (hyper)) */
const Sint32 pl08_special_15[] = {
	CG_FRAME(    0,    0,   6,           0,   168, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   602, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   603, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   604, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   605, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   606, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   607, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,   1,           0,   608, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   605, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   606, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   607, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   608, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   609, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   610, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   611, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   612, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   613, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   614, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   615, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   616, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   613, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   614, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   615, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   616, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   617, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   618, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   617, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   618, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   619, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   620, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   621, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   622, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   623, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   624, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   625, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   626, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   625, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   626, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   627, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   628, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   627, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   628, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   629, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   630, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   629, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   630, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   629, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   630, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   629, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   632, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   631, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   632, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   631, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   632, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   631, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   632, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   1,           0,   631, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   632, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
};

const Sint32 pl08_special_15_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   633, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   634, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   635, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   636, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   633, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   634, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   635, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   636, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   633, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   634, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   635, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   636, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl08_special_15_loop ),
};

/* Char_Special 16: butterfly_shot (Kocho-Gakure (hyper)) */
const Sint32 pl08_special_16[] = {
	CG_FRAME(    0,    0,   2,           0,   633, 0xc0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   634, 0xc0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   635, 0xc0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   636, 0xc0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl08_special_16 ),
};

/* Char_Special 17 */
const Sint32 pl08_special_17[] = {
	CG_FRAME(    0,    0,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32808, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32809, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32810, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32811, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32812, 0xc0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 125 ),
	CG_NEXT( pl08_special_17 ),
};

/* Char_Special 19 */
const Sint32 pl08_special_19[] = {
	CG_FRAME(    0,    0,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32808, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32809, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32810, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32811, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32812, 0xc0,   0x00,              ATK,  0x00,    0,    1,   0,     0,      0, 125 ),
	CG_NEXT( pl08_special_19 ),
};

/* Char_Special 21 */
const Sint32 pl08_special_21[] = {
	CG_FRAME(    0,    0,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32808, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32809, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32810, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32811, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32812, 0xc0,   0x00,              ATK,  0x00,    0,    2,   0,     0,      0, 125 ),
	CG_NEXT( pl08_special_21 ),
};

/* Char_Special 18 */
const Sint32 pl08_special_18[] = {
	CG_FRAME(    0,    0,   4,           0, 32809, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32810, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32811, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32812, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32808, 0xc0,   0x00,              ATK,  0x00,    0,    3,   0,     0,      0, 125 ),
	CG_NEXT( pl08_special_18 ),
};

/* Char_Special 20 */
const Sint32 pl08_special_20[] = {
	CG_FRAME(    0,    0,   4,           0, 32809, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32810, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32811, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32812, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32808, 0xc0,   0x00,              ATK,  0x00,    0,    4,   0,     0,      0, 125 ),
	CG_NEXT( pl08_special_20 ),
};

/* Char_Special 22 */
const Sint32 pl08_special_22[] = {
	CG_FRAME(    0,    0,   4,           0, 32809, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32810, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32811, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32812, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32797, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32798, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32799, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32800, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32801, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32802, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32803, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32804, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32805, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32806, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4,           0, 32807, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA, 32808, 0xc0,   0x00,              ATK,  0x00,    0,    5,   0,     0,      0, 125 ),
	CG_NEXT( pl08_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl08_special_23[] = {
	CG_FRAME(    0,    0,   1,           0,   653, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   657, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0,   653, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   658, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   655, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1,           0,   659, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    1,   1,           0,   656, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   660, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl08_special_23 ),
};

/* Char_Special 24 */
const Sint32 pl08_special_24[] = {
	CG_FRAME(    0,    0,   1,           0,   645, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   649, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    1,   1,           0,   645, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   650, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   647, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1,           0,   651, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    1,   1,           0,   648, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   652, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl08_special_24 ),
};

/* Char_Special 25 */
const Sint32 pl08_special_25[] = {
	CG_FRAME(    0,    0,   1,           0,   637, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1,           0,   641, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,   1,           0,   637, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1,           0,   642, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1,           0,   639, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1,           0,   643, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,   1,           0,   640, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   644, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl08_special_25 ),
};

/* Char_Special 26 */
const Sint32 pl08_special_26[] = {
	CG_FRAME(    0,    0,   1,           0,   693, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   697, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    1,   1,           0,   693, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   698, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   695, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1,           0,   699, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    1,   1,           0,   696, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   700, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl08_special_26 ),
};

/* Char_Special 27 */
const Sint32 pl08_special_27[] = {
	CG_FRAME(    0,    0,   1,           0,   685, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1,           0,   689, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    1,   1,           0,   685, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1,           0,   690, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1,           0,   687, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1,           0,   691, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   1,           0,   688, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   692, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl08_special_27 ),
};

/* Char_Special 28 */
const Sint32 pl08_special_28[] = {
	CG_FRAME(    0,    0,   1,           0,   677, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1,           0,   681, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    1,   1,           0,   677, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1,           0,   682, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1,           0,   679, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1,           0,   683, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,   1,           0,   680, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   684, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 119 ),
	CG_NEXT( pl08_special_28 ),
};

/* Char_Special 29 */
const Sint32 pl08_special_29[] = {
	CG_FRAME(    0,    0,   1,           0,   669, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1,           0,   673, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    1,   1,           0,   669, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1,           0,   674, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1,           0,   671, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1,           0,   675, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    0,   1,           0,   672, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_FRAME(    0,    1,   1, END_OF_DATA,   676, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 120 ),
	CG_NEXT( pl08_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl08_special_30[] = {
	CG_FRAME(    0,    0,   1,           0,   661, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1,           0,   665, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    1,   1,           0,   661, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1,           0,   666, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1,           0,   663, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1,           0,   667, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    1,   1,           0,   664, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   668, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 121 ),
	CG_NEXT( pl08_special_30 ),
};

/* Char_Special 31: thrust_init (Psy-Thrust (hyper)) */
const Sint32 pl08_special_31[] = {
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   2,           0,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   446, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl08_special_31 ),
};

/* Char_Special 32: thrust_init (Psy-Thrust (hyper)) */
const Sint32 pl08_special_32[] = {
	CG_FRAME(    0,    0,   7,           0,   239, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   240, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   4,           0,   241, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   242, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   243, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   3,           0,   244, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
};

const Sint32 pl08_special_32_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   248, 0x80,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_special_32_loop ),
};

/* Char_Special 33: thrust_dash (Psy-Thrust (hyper)) */
const Sint32 pl08_special_33[] = {
	CG_FRAME(    0,    0,   1,           0,   865, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   864, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   863, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   864, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   865, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   864, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1,           0,   863, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   864, 0xb0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  19 ),
	CG_NEXT( pl08_special_33 ),
};

const Sint32 pl08_unused_11[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   475, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   2,           0,   476, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   477, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 113 ),
	0x00006e2c,
};

const Sint32 pl08_unused_12[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   488, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,   2,           0,   489, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   490, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 118 ),
	0x00006e6c,
};

const Sint32 pl08_unused_13[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   496, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    0,   2,           0,   497, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 128 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   498, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 128 ),
	0x00006eac,
};

const Sint32 pl08_unused_14[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   503, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    0,   2,           0,   504, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   505, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 130 ),
	0x00006eec,
};

const Sint32 pl08_unused_15[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   508, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    0,   2,           0,   509, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   510, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 131 ),
	0x00006f2c,
};

const Sint32 pl08_unused_16[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   513, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    0,   2,           0,   514, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 132 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   515, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 132 ),
	0x00006f6c,
};

const Sint32 pl08_unused_17[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   518, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    0,   2,           0,   519, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   520, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 133 ),
	0x00006fac,
};

/* Char_Extra 5: chouhatsu_init (Taunt) */
const Sint32 pl08_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0,   262, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   263, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   264, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   4,           0,   266, 0x80,   0x20,              ATK,  0x00, 0x0a,    0,   0,     0,      0,  67 ),
};

const Sint32 pl08_extra_05_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   4,           0,   252, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   253, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   254, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   255, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   256, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   257, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   258, 0x80,   0x20,              ATK,  0x00,    0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl08_extra_05_loop ),
};

/* Char_Special 51: snapback_init (Snapback) */
const Sint32 pl08_special_51[] = {
	CG_FRAME(    0,    0,   2,           0,   319, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    1,   2,           0,   320, 0xa0,   0x4d,              ATK,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    1,    0,   1,           0,   321, 0xa0,   0x6a,              ATK,  0x00,    0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   6,           0,   322, 0xa0,   0xa4,              ATK,  0x00,    0,    0,   0,     0,      0, 135 ),
	CG_FRAME(    0,    0,   3,           0,   323, 0xa0,   0xa4,              ATK,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   324, 0xa0,   0xa4,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   325, 0xa0,   0xa4,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0xa0,   0x87,              ATE,  0x00,    0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0xa0,   0x4d,              ATE,  0x00,    0,    0,   0,     0,      0,  25 ),
	CG_NEXT( pl08_special_51 ),
};

/* Char_Extra 8: pl08_upper */
const Sint32 pl08_extra_08[] = {
	CG_FRAME(    0,    0,   4,           0,   364, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   2,           0,   365, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   3,           0,   366, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0, 146 ),
	CG_FRAME(    0,    0,   3,           0,   367, 0xa0,   0x0a,              ATK,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   368, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   3,           0,   369, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   6,           0,   370, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   7,           0,   372, 0xa0,   0x0a,              ATE,  0x00,    0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   373, 0xa0,   0x0c,              ATE,  0x00,    0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl08_extra_08 ),
};

/* Char_Extra 9 */
const Sint32 pl08_extra_09[] = {
	CG_FRAME(    0,    0,   1,           0,   378, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   379, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,  16 ),
	CG_FRAME( 0x80,    0,   1,           0,   380, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
	CG_FRAME( 0x80,    0,   1,           0,   381, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
	CG_FRAME( 0x80,    0,   1,           0,   380, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
	CG_FRAME( 0x81,    0,   4,           0,   381, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 147 ),
	CG_FRAME( 0x80,    0,   4,           0,   382, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,  46 ),
	CG_FRAME( 0x81,    0,   2,           0,   383, 0xa0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   378, 0xa0,   0x00,              ATE,  0x00,    0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl08_extra_09 ),
};

/* Char_Special 5 */
const Sint32 pl08_special_05[] = {
	CG_FRAME(    0,    0,   2,           0, 32786, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 108 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32789, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 108 ),
	CG_NEXT( pl08_special_05 ),
};

/* Char_Special 6
   Char_Special 7 */
const Sint32 pl08_special_06[] = {
	CG_FRAME(    0,    0,   2,           0, 32775, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 143 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 143 ),
	CG_FRAME(    0,    0,   2,           0, 32777, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 143 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32778, 0xb0,   0x00,              ATK,  0x00,    0,    0,   0,     0,      0, 143 ),
	CG_NEXT( pl08_special_06 ),
};

/* group 27 0: pls08_01_init */
const Sint32 pl08_grp27_00[] = {
	EF_FRAME(    0,    0,   3,           0,   912 ),
	EF_FRAME(    0,    0,   3,           0,   913 ),
	EF_FRAME(    0,    0,   3,           0,   914 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   915 ),
	CG_NEXT( pl08_grp27_00 ),
};

/* group 27 1: pls08_01_init */
const Sint32 pl08_grp27_01[] = {
	EF_FRAME(    0,    0,   3,           0,   916 ),
	EF_FRAME(    0,    0,   3,           0,   917 ),
	EF_FRAME(    0,    0,   3,           0,   918 ),
	EF_FRAME(    0,    0,   2, END_OF_DATA,   919 ),
	CG_NEXT( pl08_grp27_01 ),
};

/* Char_Extra 0
   Char_Extra 1
   Char_Extra 2: fly_start5
   Char_Extra 6
   Char_Extra 7
   Char_Special 48 */
const Sint32 pl08_extra_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    45, 0x80,   0x00,                0,  0x00,    0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl08_extra_00 ),
};
