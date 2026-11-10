/* Ryu (character 00): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_00      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_00     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl00.c) says which group
                   and number each move uses.
   pl00_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl00_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl00_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl00_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_00
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_00, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_00 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl00_stand_00[], pl00_stand_02[], pl00_stand_03[], pl00_jump_00[],
	pl00_jump_00_loop[], pl00_jump_01[], pl00_jump_01_loop[], pl00_jump_02[], pl00_jump_02_loop[],
	pl00_jump_03[], pl00_jump_06[], pl00_jump_09[], pl00_jump_07[], pl00_jump_10[], pl00_jump_08[],
	pl00_jump_11[], pl00_dash_00[], pl00_dash_00_loop[], pl00_dash_02[], pl00_dash_02_loop[],
	pl00_dash_01[], pl00_dash_01_loop[], pl00_dash_03[], pl00_dash_03_loop[], pl00_crouch_00[],
	pl00_crouch_01[], pl00_crouch_02[], pl00_turn_00[], pl00_turn_01[], pl00_guard_00[],
	pl00_guard_01[], pl00_guard_02[], pl00_guard_03[], pl00_guard_04[], pl00_guard_05[],
	pl00_guard_06[], pl00_guard_07[], pl00_stand_p_00[], pl00_stand_p_01[], pl00_stand_p_02[],
	pl00_stand_k_00[], pl00_stand_k_01[], pl00_stand_k_02[], pl00_air_guard_00[],
	pl00_air_guard_01[], pl00_crouch_p_00[], pl00_crouch_p_01[], pl00_crouch_p_02[], pl00_extra_11[],
	pl00_crouch_k_00[], pl00_crouch_k_01[], pl00_crouch_k_02[], pl00_jump_p_00[], pl00_jump_p_01[],
	pl00_jump_p_02[], pl00_jump_k_00[], pl00_jump_k_01[], pl00_jump_k_02[], pl00_get_up_00[],
	pl00_get_up_01[], pl00_get_up_02[], pl00_get_up_03[], pl00_damage_00[], pl00_damage_01[],
	pl00_damage_02[], pl00_damage_03[], pl00_damage_04[], pl00_damage_05[], pl00_damage_06[],
	pl00_damage_07[], pl00_damage_08[], pl00_damage_10[], pl00_damage_11[], pl00_damage_12[],
	pl00_damage_13[], pl00_damage_13_loop[], pl00_damage_09[], pl00_damage_09_loop[],
	pl00_damage_16[], pl00_damage_17[], pl00_damage_18[], pl00_damage_19[], pl00_damage_20[],
	pl00_damage_21[], pl00_damage_22[], pl00_damage_23[], pl00_damage_24[], pl00_damage_25[],
	pl00_damage_26[], pl00_damage_27[], pl00_damage_31[], pl00_damage_33[], pl00_damage_28[],
	pl00_damage_29[], pl00_caught_00[], pl00_caught_01[], pl00_caught_02[], pl00_caught_03[],
	pl00_caught_04[], pl00_caught_05[], pl00_caught_06[], pl00_caught_07[], pl00_caught_08[],
	pl00_caught_09[], pl00_caught_10[], pl00_caught_11[], pl00_caught_12[], pl00_caught_13[],
	pl00_caught_14[], pl00_caught_15[], pl00_caught_16[], pl00_caught_17[], pl00_caught_18[],
	pl00_caught_19[], pl00_caught_20[], pl00_caught_21[], pl00_dizzy_00[], pl00_win_00[],
	pl00_win_00_loop[], pl00_win_01[], pl00_win_01_loop[], pl00_win_02[], pl00_win_02_loop[],
	pl00_win_03[], pl00_win_03_loop[], pl00_win_04[], pl00_win_06[], pl00_win_06_loop[],
	pl00_appear_00[], pl00_appear_00_loop[], pl00_extra_00[], pl00_special_23[], pl00_extra_01[],
	pl00_extra_01_loop[], pl00_extra_02[], pl00_extra_03[], pl00_extra_04[], pl00_extra_05[],
	pl00_extra_06[], pl00_extra_07[], pl00_extra_08[], pl00_extra_08_loop[], pl00_extra_09[],
	pl00_extra_09_loop[], pl00_extra_10[], pl00_special_20[], pl00_change_00[],
	pl00_change_00_loop[], pl00_change_02[], pl00_change_03[], pl00_change_03_loop[],
	pl00_change_04[], pl00_catch_00[], pl00_catch_01[], pl00_catch_01_loop[], pl00_catch_02[],
	pl00_special_11[], pl00_special_11_loop[], pl00_special_12[], pl00_special_12_loop[],
	pl00_special_13[], pl00_special_00[], pl00_special_01[], pl00_special_22[], pl00_special_14[],
	pl00_special_15[], pl00_special_16[], pl00_special_16_loop[], pl00_special_17[],
	pl00_special_17_loop[], pl00_special_02[], pl00_special_03[], pl00_special_04[],
	pl00_special_06[], pl00_special_05[], pl00_special_07[], pl00_special_08[],
	pl00_special_08_loop[], pl00_special_09[], pl00_special_09_loop[], pl00_special_10[],
	pl00_super_00[], pl00_super_00_loop[], pl00_super_01[], pl00_super_08[], pl00_super_02[],
	pl00_super_03[], pl00_super_09[], pl00_super_09_loop[], pl00_super_10[], pl00_super_10_loop[],
	pl00_super_16[], pl00_super_11[], pl00_super_11_loop[], pl00_super_12[], pl00_super_12_loop[],
	pl00_super_13[], pl00_super_13_loop[], pl00_super_14[], pl00_super_14_loop[], pl00_super_15[],
	pl00_grp25_00[], pl00_grp25_01[], pl00_grp25_02[], pl00_grp25_03[], pl00_grp25_04[],
	pl00_grp25_05[], pl00_grp25_06[], pl00_grp25_07[], pl00_grp25_08[], pl00_grp25_09[],
	pl00_grp25_10[], pl00_grp25_11[], pl00_grp25_12[], pl00_grp25_13[], pl00_grp25_14[],
	pl00_grp25_15[], pl00_grp25_16[], pl00_grp25_17[], pl00_grp25_18[], pl00_grp25_19[],
	pl00_grp25_20[], pl00_grp25_21[], pl00_grp25_22[], pl00_grp25_23[], pl00_super_04[],
	pl00_super_04_loop[], pl00_super_05[], pl00_super_06[], pl00_super_06_loop[], pl00_super_07[],
	pl00_effect_00[], pl00_effect_00_loop[], pl00_effect_02[], pl00_effect_02_loop[],
	pl00_effect_03[], pl00_effect_03_loop[], pl00_effect_04[], pl00_effect_05[],
	pl00_effect_05_loop[], pl00_effect_06[], pl00_effect_06_loop[], pl00_effect_07[],
	pl00_effect_08[], pl00_effect_08_loop[], pl00_effect_09[], pl00_effect_13[];
extern const Sint32 *const pl00_stand_tbl[], *const pl00_jump_tbl[], *const pl00_dash_tbl[],
	*const pl00_crouch_tbl[], *const pl00_turn_tbl[], *const pl00_guard_tbl[],
	*const pl00_air_guard_tbl[], *const pl00_stand_p_tbl[], *const pl00_stand_k_tbl[],
	*const pl00_crouch_p_tbl[], *const pl00_crouch_k_tbl[], *const pl00_jump_p_tbl[],
	*const pl00_jump_k_tbl[], *const pl00_damage_tbl[], *const pl00_dizzy_tbl[],
	*const pl00_catch_tbl[], *const pl00_caught_tbl[], *const pl00_get_up_tbl[],
	*const pl00_appear_tbl[], *const pl00_win_tbl[], *const pl00_extra_tbl[],
	*const pl00_special_tbl[], *const pl00_super_tbl[], *const pl00_effect_tbl[],
	*const pl00_change_tbl[], *const pl00_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_00[16] = {
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
const Sint32 *const *const char_tbl_00[] = {
	pl00_stand_tbl, pl00_jump_tbl, pl00_dash_tbl, pl00_crouch_tbl, pl00_turn_tbl, pl00_guard_tbl,
	pl00_air_guard_tbl, pl00_stand_p_tbl, pl00_stand_k_tbl, pl00_crouch_p_tbl, pl00_crouch_k_tbl, pl00_jump_p_tbl,
	pl00_jump_k_tbl, pl00_damage_tbl, pl00_dizzy_tbl, pl00_catch_tbl, pl00_caught_tbl, pl00_get_up_tbl,
	pl00_appear_tbl, pl00_win_tbl, pl00_extra_tbl, pl00_special_tbl, pl00_super_tbl, pl00_effect_tbl,
	pl00_change_tbl, pl00_grp25_tbl, 0,
};

const Sint32 *const pl00_stand_tbl[] = {
	pl00_stand_00, pl00_stand_00, pl00_stand_02, pl00_stand_03, pl00_stand_00, pl00_stand_00,
	pl00_stand_02, pl00_stand_03, 0,
};

const Sint32 *const pl00_jump_tbl[] = {
	pl00_jump_00, pl00_jump_01, pl00_jump_02, pl00_jump_03, pl00_jump_03, pl00_jump_03,
	pl00_jump_06, pl00_jump_07, pl00_jump_08, pl00_jump_09, pl00_jump_10, pl00_jump_11,
	0,
};

const Sint32 *const pl00_dash_tbl[] = {
	pl00_dash_00, pl00_dash_01, pl00_dash_02, pl00_dash_03, 0,
};

const Sint32 *const pl00_crouch_tbl[] = {
	pl00_crouch_00, pl00_crouch_01, pl00_crouch_02, 0,
};

const Sint32 *const pl00_turn_tbl[] = {
	pl00_turn_00, pl00_turn_01, 0,
};

const Sint32 *const pl00_guard_tbl[] = {
	pl00_guard_00, pl00_guard_01, pl00_guard_02, pl00_guard_03, pl00_guard_04, pl00_guard_05,
	pl00_guard_06, pl00_guard_07, 0,
};

const Sint32 *const pl00_air_guard_tbl[] = {
	pl00_air_guard_00, pl00_air_guard_01, pl00_air_guard_01, 0,
};

const Sint32 *const pl00_stand_p_tbl[] = {
	pl00_stand_p_00, pl00_stand_p_01, pl00_stand_p_02, 0,
};

const Sint32 *const pl00_stand_k_tbl[] = {
	pl00_stand_k_00, pl00_stand_k_01, pl00_stand_k_02, 0,
};

const Sint32 *const pl00_crouch_p_tbl[] = {
	pl00_crouch_p_00, pl00_crouch_p_01, pl00_crouch_p_02, 0,
};

const Sint32 *const pl00_crouch_k_tbl[] = {
	pl00_crouch_k_00, pl00_crouch_k_01, pl00_crouch_k_02, 0,
};

const Sint32 *const pl00_jump_p_tbl[] = {
	pl00_jump_p_00, pl00_jump_p_01, pl00_jump_p_02, pl00_jump_p_00, pl00_jump_p_01, pl00_jump_p_02,
	0,
};

const Sint32 *const pl00_jump_k_tbl[] = {
	pl00_jump_k_00, pl00_jump_k_01, pl00_jump_k_02, pl00_jump_k_00, pl00_jump_k_01, pl00_jump_k_02,
	0,
};

const Sint32 *const pl00_damage_tbl[] = {
	pl00_damage_00, pl00_damage_01, pl00_damage_02, pl00_damage_03, pl00_damage_04, pl00_damage_05,
	pl00_damage_06, pl00_damage_07, pl00_damage_08, pl00_damage_09, pl00_damage_10, pl00_damage_11,
	pl00_damage_12, pl00_damage_13, pl00_damage_13, pl00_damage_13, pl00_damage_16, pl00_damage_17,
	pl00_damage_18, pl00_damage_19, pl00_damage_20, pl00_damage_21, pl00_damage_22, pl00_damage_23,
	pl00_damage_24, pl00_damage_25, pl00_damage_26, pl00_damage_27, pl00_damage_28, pl00_damage_29,
	pl00_damage_27, pl00_damage_31, pl00_damage_27, pl00_damage_33, pl00_damage_13, 0,
};

const Sint32 *const pl00_dizzy_tbl[] = {
	pl00_dizzy_00, 0,
};

const Sint32 *const pl00_catch_tbl[] = {
	pl00_catch_00, pl00_catch_01, pl00_catch_02, 0,
};

const Sint32 *const pl00_caught_tbl[] = {
	pl00_caught_00, pl00_caught_01, pl00_caught_02, pl00_caught_03, pl00_caught_04, pl00_caught_05,
	pl00_caught_06, pl00_caught_07, pl00_caught_08, pl00_caught_09, pl00_caught_10, pl00_caught_11,
	pl00_caught_12, pl00_caught_13, pl00_caught_14, pl00_caught_15, pl00_caught_16, pl00_caught_17,
	pl00_caught_18, pl00_caught_19, pl00_caught_20, pl00_caught_21, 0,
};

const Sint32 *const pl00_get_up_tbl[] = {
	pl00_get_up_00, pl00_get_up_01, pl00_get_up_02, pl00_get_up_03, 0,
};

const Sint32 *const pl00_appear_tbl[] = {
	pl00_appear_00, 0,
};

const Sint32 *const pl00_win_tbl[] = {
	pl00_win_00, pl00_win_01, pl00_win_02, pl00_win_03, pl00_win_04, pl00_win_04,
	pl00_win_06, 0,
};

const Sint32 *const pl00_extra_tbl[] = {
	pl00_extra_00, pl00_extra_01, pl00_extra_02, pl00_extra_03, pl00_extra_04, pl00_extra_05,
	pl00_extra_06, pl00_extra_07, pl00_extra_08, pl00_extra_09, pl00_extra_10, pl00_extra_11,
	0,
};

const Sint32 *const pl00_special_tbl[] = {
	pl00_special_00, pl00_special_01, pl00_special_02, pl00_special_03, pl00_special_04, pl00_special_05,
	pl00_special_06, pl00_special_07, pl00_special_08, pl00_special_09, pl00_special_10, pl00_special_11,
	pl00_special_12, pl00_special_13, pl00_special_14, pl00_special_15, pl00_special_16, pl00_special_17,
	pl00_special_17, pl00_special_17, pl00_special_20, pl00_special_20, pl00_special_22, pl00_special_23,
	0,
};

const Sint32 *const pl00_super_tbl[] = {
	pl00_super_00, pl00_super_01, pl00_super_02, pl00_super_03, pl00_super_04, pl00_super_05,
	pl00_super_06, pl00_super_07, pl00_super_08, pl00_super_09, pl00_super_10, pl00_super_11,
	pl00_super_12, pl00_super_13, pl00_super_14, pl00_super_15, pl00_super_16, 0,
};

const Sint32 *const pl00_effect_tbl[] = {
	pl00_effect_00, pl00_effect_00, pl00_effect_02, pl00_effect_03, pl00_effect_04, pl00_effect_05,
	pl00_effect_06, pl00_effect_07, pl00_effect_08, pl00_effect_09, pl00_effect_09, pl00_effect_09,
	pl00_effect_09, pl00_effect_13, 0,
};

const Sint32 *const pl00_change_tbl[] = {
	pl00_change_00, pl00_special_17, pl00_change_02, pl00_change_03, pl00_change_04, 0,
};

const Sint32 *const pl00_grp25_tbl[] = {
	pl00_grp25_00, pl00_grp25_01, pl00_grp25_02, pl00_grp25_03, pl00_grp25_04, pl00_grp25_05,
	pl00_grp25_06, pl00_grp25_07, pl00_grp25_08, pl00_grp25_09, pl00_grp25_10, pl00_grp25_11,
	pl00_grp25_12, pl00_grp25_13, pl00_grp25_14, pl00_grp25_15, pl00_grp25_16, pl00_grp25_17,
	pl00_grp25_18, pl00_grp25_19, pl00_grp25_20, pl00_grp25_21, pl00_grp25_22, pl00_grp25_23,
	0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl00_stand_00[] = {
	CG_FRAME(    0,    0,    3,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    65, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    66, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    67, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    68, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    62, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl00_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    70, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    71, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    72, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    73, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    74, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl00_stand_03[] = {
	CG_FRAME(    0,    0,    3,           0,    75, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    76, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    77, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    80, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl00_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    6,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    6,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    6,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    6,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl00_jump_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl00_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl00_jump_01[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    8,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl00_jump_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   157, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl00_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl00_jump_02[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    8,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
};

const Sint32 pl00_jump_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_NEXT( pl00_jump_02_loop ),
};

/* Char_Jump 3: jtatsu_spin (Tatsumaki Senpuukyaku in the air), jtatsu_fall (Tatsumaki Senpuukyaku in the air)
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl00_jump_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    69, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl00_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    90, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   156, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl00_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   157, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl00_jump_07[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl00_jump_10[] = {
	CG_FRAME(    0,    0,    4,           0,    95, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   157, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl00_jump_08[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl00_jump_11[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_jump_11 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl00_dash_00[] = {
	CG_FRAME(    0,    1,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,    81, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl00_dash_00_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,    81, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl00_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl00_dash_02[] = {
	CG_FRAME(    0,    0,    5,           0,    82, 0x80,   0x78,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    83, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_dash_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl00_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_dash_01_loop[] = {
	CG_FRAME(    0,    1,    4, END_OF_DATA,    85, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_dash_01_loop ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl00_dash_03[] = {
	CG_FRAME(    0,    0,    5,           0,    86, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    87, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    88, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_dash_03_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_dash_03_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl00_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   103, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl00_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl00_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl00_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   103, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl00_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl00_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,   107, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   108, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   109, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl00_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl00_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl00_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl00_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl00_guard_02[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl00_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl00_guard_03[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl00_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl00_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   113, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl00_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   113, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl00_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl00_guard_06[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   112, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl00_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl00_guard_07[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   113, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl00_guard_07 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl00_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    3,           0,   201, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    5,           0,   201, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 136 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   200, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl00_stand_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   202, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   203, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4,           0,   204, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    9,           0,   204, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   203, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl00_stand_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   200, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   205, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   206, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    9,           0,   206, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 137 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl00_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   208, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   210, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    7,           0,   210, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    3,           0,   209, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl00_stand_k_01[] = {
	CG_FRAME(    0,    0,    8,           0,   209, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   211, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME( 0x80,    1,    4,           0,   212, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    9,           0,   212, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    9,           0,   209, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   102, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl00_stand_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   213, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    4,           0,   214, 0x80,   0x00,              ATK,  0x00,       0x05,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    5,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    5,           0,   216, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   216, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   200, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl00_air_guard_00[] = {
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl00_air_guard_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   115, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl00_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl00_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   218, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,    3,           0,   219, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    5,           0,   219, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   218, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl00_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   220, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   222, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    6,           0,   222, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   221, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl00_crouch_p_02[] = {
	CG_FRAME(    0,    0,    9,           0,   223, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,    2,           0,   224, 0x80,   0x40,          ATK | 7,  0x00,       0x05,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   10,           0,   225, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    6,           0,   224, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   223, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_crouch_p_02 ),
};

/* Char_Extra 11: pl00_upper */
const Sint32 pl00_extra_11[] = {
	CG_FRAME(    0,    0,    9,           0,   223, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,    2,           0,   224, 0x80,   0x40,          ATK | 7,  0x00,       0x05,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   10,           0,   225, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    6,           0,   224, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   224, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_extra_11 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl00_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   226, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,    2,           0,   227, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    4,           0,   227, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   226, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl00_crouch_k_01[] = {
	CG_FRAME(    0,    0,    5,           0,   226, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    1,           0,   228, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3,           0,   229, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    9,           0,   229, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   226, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl00_crouch_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   226, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    5,           0,   230, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   231, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    8,           0,   231, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    8,           0,   232, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    8,           0,   233, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   107, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl00_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl00_jump_p_00[] = {
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   10,           0,   235, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   234, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl00_jump_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   234, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   236, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    8,           0,   237, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   234, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl00_jump_p_02[] = {
	CG_FRAME(    0,    0,    2,           0,   234, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    6,           0,   236, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    5,           0,   237, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    9,           0,   237, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0, 138 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   234, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl00_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   238, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   10,           0,   239, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl00_jump_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   238, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    4,           0,   239, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    8,           0,   239, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl00_jump_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   245, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   246, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   241, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    4,           0,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 139 ),
	CG_FRAME(    0,    0,    6,           0,   243, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   244, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl00_get_up_00[] = {
	CG_FRAME(    0,    0,    6,           0,   134, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   135, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   136, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl00_get_up_01[] = {
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   103, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl00_get_up_02[] = {
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   103, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl00_get_up_03[] = {
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   103, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl00_damage_00[] = {
	CG_FRAME( 0x80,    0,    2,           0,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl00_damage_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl00_damage_02[] = {
	CG_FRAME( 0x80,    0,    4,           0,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl00_damage_03[] = {
	CG_FRAME( 0x80,    0,    2,           0,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl00_damage_04[] = {
	CG_FRAME( 0x80,    0,    2,           0,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl00_damage_05[] = {
	CG_FRAME( 0x80,    0,    4,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl00_damage_06[] = {
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x82,    0,    5,           0,   349, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   348, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl00_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl00_damage_07[] = {
	CG_FRAME( 0x80,    0,    2,           0,   348, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   10,           0,   349, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   348, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl00_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl00_damage_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   348, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME( 0x80,    0,    2,           0,   349, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   10,           0,   350, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   349, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   348, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl00_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl00_damage_10[] = {
	CG_FRAME( 0x80,    0,    1,           0,   351, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,   352, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   351, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl00_damage_11[] = {
	CG_FRAME( 0x80,    0,    1,           0,   351, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    1,           0,   352, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   352, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   351, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl00_damage_12[] = {
	CG_FRAME(    0,    0,    1,           0,   351, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   352, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   354, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   353, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   352, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   351, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl00_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x71,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl00_damage_13_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   359, 0x80,   0x80,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl00_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl00_damage_09[] = {
	CG_FRAME(    0,    0,    8,           0,   360, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   361, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   362, 0x80,   0x15,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   370, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    1,           0,   373, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
};

const Sint32 pl00_damage_09_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   373, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl00_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl00_damage_16[] = {
	CG_FRAME(    0,    0,    4,           0,   363, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   364, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl00_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   365, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl00_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   366, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl00_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   367, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl00_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   368, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl00_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   363, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   364, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl00_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   365, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl00_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   366, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl00_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   367, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl00_damage_25[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   368, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl00_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   369, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   370, 0x80,   0x2d,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   371, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   372, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   373, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl00_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape */
const Sint32 pl00_damage_27[] = {
	CG_FRAME(    0,    0,    2,           0,    99, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    98, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,    97, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl00_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   374, 0x80,   0x8c,                0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl00_damage_31 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl00_damage_33[] = {
	CG_FRAME(    0,    0,    4,           0,   136, 0x80,   0x13,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   137, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   101, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    69, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl00_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   377, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   379, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   380, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl00_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   381, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   382, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   383, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   384, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   386, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl00_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   341, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl00_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl00_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   342, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl00_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl00_caught_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl00_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl00_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl00_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl00_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl00_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl00_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl00_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl00_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl00_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   350, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl00_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl00_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   352, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_00 ),
};

/* Char_Caught 10 */
const Sint32 pl00_caught_10[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   353, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl00_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   354, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl00_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   358, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl00_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl00_caught_13[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   363, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl00_caught_14[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   365, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl00_caught_15[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   366, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl00_caught_16[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   367, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl00_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   368, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl00_caught_18[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   375, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl00_caught_19[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   381, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl00_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl00_caught_20[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   374, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl00_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl00_caught_21[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   360, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl00_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl00_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   387, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   388, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   389, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   388, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_dizzy_00 ),
};

/* Char_Win 0 */
const Sint32 pl00_win_00[] = {
	CG_FRAME(    0,    0,    6,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   116, 0x80,   0x00,                0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_win_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl00_win_01[] = {
	CG_FRAME(    0,    0,    4,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_win_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_win_01_loop ),
};

/* Char_Win 2 */
const Sint32 pl00_win_02[] = {
	CG_FRAME(    0,    0,    6,           0,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   117, 0x80,   0x00,                0,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_win_02_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   159, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_win_02_loop ),
};

/* Char_Win 3 */
const Sint32 pl00_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   172, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   173, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   176, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_win_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   182, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_win_03_loop ),
};

/* Char_Win 4
   Char_Win 5 */
const Sint32 pl00_win_04[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   340, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_win_04 ),
};

/* Char_Win 6 */
const Sint32 pl00_win_06[] = {
	CG_FRAME(    0,    0,    4,           0,   183, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x30,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   187, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_win_06_loop[] = {
	CG_FRAME(    0,    0,   48, END_OF_DATA,   188, 0x30,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_win_06_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl00_appear_00[] = {
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   133, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_appear_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_appear_00_loop ),
};

/* Char_Extra 0 */
const Sint32 pl00_extra_00[] = {
	CG_FRAME(    0,    0,    6,           0,   267, 0x80,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   269, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    1,    0,    4,           0,   270, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    1,    0,    4,           0,   271, 0x80,   0x08, ATK | CG_ATCK_NG | 0x15,  0x00, 0x16 | CSE,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,    4,           0,   272, 0x80,   0x08, ATK | CG_ATCK_NG | 5,  0x00,       0x03,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   274, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl00_extra_00 ),
};

/* Char_Special 23: snapback_init (the snapback) */
const Sint32 pl00_special_23[] = {
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x0e,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0, 0x80,    4,           0,   269, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    1,    0,    4,           0,   270, 0x80,   0x0e, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    1,    0,    4,           0,   271, 0x80,   0x08, ATK | CG_ATCK_NG | 0x15,  0x00, 0x16 | CSE,    0,   0,     0,      0,  63 ),
	CG_FRAME(    1,    1,    4,           0,   272, 0x80,   0x08, ATK | CG_ATCK_NG | 5,  0x00,       0x03,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    4,           0,   275, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   274, 0x80,   0x08, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_NEXT( pl00_special_23 ),
};

/* Char_Extra 1 */
const Sint32 pl00_extra_01[] = {
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   314, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,    2,           0,   286, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   288, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   289, 0x80,   0x20, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x80,   0x20, ATK | CG_ATCK_NG | 0x15,  0x00, 0x16 | CSE,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x20, ATK | CG_ATCK_NG | 5,  0x00,       0x03,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_extra_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   244, 0x80,   0x20, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_extra_01_loop ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl00_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   250, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   249, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    3,           0,   248, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    1,    8, END_OF_DATA,   247, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_NEXT( pl00_extra_02 ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl00_extra_03[] = {
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   253, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl00_extra_03 ),
};

/* Char_Extra 4 */
const Sint32 pl00_extra_04[] = {
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    5,           0,   255, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    7,           0,   256, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   255, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl00_extra_05[] = {
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    6,           0,   256, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   255, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_extra_05 ),
};

/* Char_Extra 6: sp_chouhatsu (the taunt) */
const Sint32 pl00_extra_06[] = {
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_extra_06 ),
};

/* Char_Extra 7 */
const Sint32 pl00_extra_07[] = {
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,                0,  0x00,       0x11,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_extra_07 ),
};

/* Char_Extra 8: t_kick_init */
const Sint32 pl00_extra_08[] = {
	CG_FRAME(    0,    0,    2,           0,   254, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
};

const Sint32 pl00_extra_08_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   256, 0xa0,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl00_extra_08_loop ),
};

/* Char_Extra 9: t_kick_move */
const Sint32 pl00_extra_09[] = {
	CG_FRAME(    0,    0,    6,           0,   189, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x20,       ATE | 0x13,  0x00,       0x0b,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,  90 ),
};

const Sint32 pl00_extra_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   192, 0x80,   0x17,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_extra_09_loop ),
};

/* Char_Extra 10: stand_kick_move */
const Sint32 pl00_extra_10[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    69, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_extra_10 ),
};

/* Char_Special 20
   Char_Special 21 */
const Sint32 pl00_special_20[] = {
	CG_FRAME(    0,    0,    4,           0,   390, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   391, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   392, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   393, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   394, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   395, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_special_20 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl00_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   183, 0x30,   0x00,              ATK,  0x00,       0x0e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   187, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_change_00_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   188, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl00_change_02[] = {
	CG_FRAME(    0,    0,    4,           0,    96, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   158, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl00_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x20,                0,  0x00,       0x0c,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   195, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   195, 0x80,   0x20,             0x12,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl00_change_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl00_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,                0,  0x00,       0x11,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl00_change_04 ),
};

/* Char_Catch 0: nml_k_set */
const Sint32 pl00_catch_00[] = {
	CG_FRAME(    0,    0,   10,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    4,           0,   259, 0x80,   0x5f,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    1,   12,           0,   260, 0x80,   0x5f,                7,  0x00,       0x05,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   24,           0,   261, 0x80,   0x5f,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   262, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_catch_00 ),
};

/* Char_Catch 1: nml_p_set */
const Sint32 pl00_catch_01[] = {
	CG_FRAME(    0,    0,    1,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    8,           0,   263, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,   27,           0,   266, 0x80,   0x00,                6,  0x00,       0x04,    0,   0,     0, 0x0009,   0 ),
};

const Sint32 pl00_catch_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_catch_01_loop ),
};

/* Char_Catch 2: jump_k_set */
const Sint32 pl00_catch_02[] = {
	CG_FRAME(    0,    0,   10,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,    4,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,    6,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    1,    8,           0,    99, 0x80,   0x00,                7,  0x00,       0x05,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,    8,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl00_catch_02 ),
};

/* Char_Special 11 */
const Sint32 pl00_special_11[] = {
	CG_FRAME(    0,    0,    4,           0,   396, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl00_special_11_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl00_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl00_special_12[] = {
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
};

const Sint32 pl00_special_12_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl00_special_12_loop ),
};

/* Char_Special 13: shl00_00_hit */
const Sint32 pl00_special_13[] = {
	CG_FRAME(    0,    0,    4,           0,   405, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   406, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   407, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   408, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   409, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   410, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_special_13 ),
};

/* Char_Special 0 */
const Sint32 pl00_special_00[] = {
	CG_FRAME(    0,    0,    1,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0, 0x80,    1,           0,   279, 0x80,   0x49,          ATK | 1,  0x00,       0x15,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x49,          ATK | 9,  0x00,       0x1e,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_special_00 ),
};

/* Char_Special 1 */
const Sint32 pl00_special_01[] = {
	CG_FRAME(    0,    0,    1,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0, 0x80,    1,           0,   279, 0x80,   0x49,          ATK | 1,  0x00,       0x15,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x49,          ATK | 9,  0x00,       0x1e,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_special_01 ),
};

/* Char_Special 22 */
const Sint32 pl00_special_22[] = {
	CG_FRAME(    0,    0,    1,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0, 0x80,    1,           0,   279, 0x80,   0x49,          ATK | 1,  0x00,       0x15,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x49,          ATK | 9,  0x00,       0x1e,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_special_22 ),
};

/* Char_Special 14: jhadou_init (Hadouken in the air, 14 + atck_level) */
const Sint32 pl00_special_14[] = {
	CG_FRAME(    0,    0,    5,           0,   298, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    1,           0,   299, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0, 0x80,    1,           0,   301, 0x80,   0x4e,          ATK | 1,  0x00,       0x15,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x4e,          ATK | 9,  0x00,       0x1e,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    1,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   303, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl00_special_15[] = {
	CG_FRAME(    0,    0,    5,           0,   298, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    1,           0,   299, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0, 0x80,    1,           0,   301, 0x80,   0x4e,          ATK | 1,  0x00,       0x15,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x4e,          ATK | 9,  0x00,       0x1e,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    1,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   303, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl00_special_16[] = {
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl00_special_16_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl00_special_16_loop ),
};

/* Char_Special 17
   Char_Special 18
   Char_Special 19
   Char_Change 1: act_change_vcounter_init */
const Sint32 pl00_special_17[] = {
	CG_FRAME(    0,    0,    2,           0,   321, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   322, 0x80,   0x56,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_special_17_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   323, 0x80,   0x56,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_special_17_loop ),
};

/* Char_Special 2: shouryu_init (Shoryuken, 2 + atck_level) */
const Sint32 pl00_special_02[] = {
	CG_FRAME(    0, 0x80,    1,           0,   280, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,    2,           0,   280, 0x80,   0x00,          ATK | 2,  0x00,       0x16,    0,   0,     0,      0,   0 ),
	CG_FRAME(   51, 0x80,    4,           0,   281, 0x80,   0x00,       ATK | 0x16,  0x00, 0x17 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(   52,    0,    6,           0,   282, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    6,           0,   283, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0, 0x80,    2,           0,   284, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0, 0x80,    2,           0,   285, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,    69, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   102, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl00_special_03[] = {
	CG_FRAME(    0, 0x80,    1,           0,   280, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0, 0x80,    3,           0,   280, 0x80,   0x00,          ATK | 2,  0x00,       0x16,    0,   0,     0,      0,   0 ),
	CG_FRAME(   55, 0x80,    4,           0,   281, 0x80,   0x00,       ATK | 0x16,  0x00, 0x17 | CSE,    0,   0,     0,      0,  69 ),
	CG_FRAME(   56,    0,    6,           0,   282, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   12,           0,   282, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    6,           0,   283, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0, 0x80,    2,           0,   284, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0, 0x80,    2,           0,   285, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    1,           0,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,    69, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   103, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   102, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_special_03 ),
};

/* Char_Special 4: tatsu_init (Tatsumaki Senpuukyaku) */
const Sint32 pl00_special_04[] = {
	CG_FRAME(    0,    0,    3,           0,   286, 0x80,   0x20,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   287, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   288, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   289, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_special_04 ),
};

/* Char_Special 6: tatsu_rise (Tatsumaki Senpuukyaku) */
const Sint32 pl00_special_06[] = {
	CG_FRAME(    0,    1,    3,           0,   290, 0x80,   0x20,       ATK | 0x15,  0x00, 0x16 | CSE,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3,           0,   291, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,    3,           0,   292, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,   -1,    1, END_OF_DATA,   293, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_special_06 ),
};

/* Char_Special 5: tatsu_spin (Tatsumaki Senpuukyaku) */
const Sint32 pl00_special_05[] = {
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   297, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_special_05 ),
};

/* Char_Special 7: tatsu_fall (Tatsumaki Senpuukyaku) */
const Sint32 pl00_special_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   102, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_special_07 ),
};

/* Char_Special 8: jtatsu_init (Tatsumaki Senpuukyaku in the air) */
const Sint32 pl00_special_08[] = {
	CG_FRAME(    0,    0,    6,           0,   289, 0x80,   0x20,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,  10 ),
};

const Sint32 pl00_special_08_loop[] = {
	CG_FRAME(    0,    1,    3,           0,   290, 0x80,   0x20,       ATK | 0x15,  0x00, 0x16 | CSE,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3,           0,   291, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,    3,           0,   292, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,   -1,    1, END_OF_DATA,   293, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl00_special_09[] = {
	CG_FRAME(    0,    0,    6,           0,   289, 0x80,   0x20,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,  10 ),
};

const Sint32 pl00_special_09_loop[] = {
	CG_FRAME(    0,    1,    3,           0,   290, 0x80,   0x20,       ATK | 0x15,  0x00, 0x16 | CSE,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3,           0,   291, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,    3,           0,   292, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,   -1,    1, END_OF_DATA,   293, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_special_09_loop ),
};

/* Char_Special 10: jtatsu_spin (Tatsumaki Senpuukyaku in the air) */
const Sint32 pl00_special_10[] = {
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   296, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    5,           0,   297, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   294, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_special_10 ),
};

/* Char_Super 0: shinkuu_tatsu_init (Shinkuu Tatsumaki Senpuukyaku (hyper)) */
const Sint32 pl00_super_00[] = {
	CG_FRAME(    0,    0,    3,           0,   308, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   311, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    1,    0,    1,           0,   312, 0x80,   0x00,          ATK | 4,  0x00,       0x14,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   314, 0x17,   0x00,          ATK | 3,  0x00,       0x17,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    1,    1,           0,   286, 0x80,   0x00,       ATK | 0x0f,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    1,           0,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    1,           0,   288, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
};

const Sint32 pl00_super_00_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   289, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_super_00_loop ),
};

/* Char_Super 1: shinkuu_tatsu_rise (Shinkuu Tatsumaki Senpuukyaku (hyper)) */
const Sint32 pl00_super_01[] = {
	CG_FRAME(    0,    0,    3,           0,   290, 0x80,   0x00,              ATK,  0x51, 0x14 | CSE,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   291, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   292, 0x80,   0x00,              ATK,  0x52, 0x14 | CSE,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   293, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl00_super_01 ),
};

/* Char_Super 8: shinkuu_tatsu_spin (Shinkuu Tatsumaki Senpuukyaku (hyper)) */
const Sint32 pl00_super_08[] = {
	CG_FRAME(    0,    0,    3,           0,   290, 0x80,   0x00,              ATK,  0x53, 0x14 | CSE,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   291, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3,           0,   292, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   293, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl00_super_08 ),
};

/* Char_Super 2: shinkuu_tatsu_spin_end (Shinkuu Tatsumaki Senpuukyaku (hyper)) */
const Sint32 pl00_super_02[] = {
	CG_FRAME(    0,    0,    3,           0,   294, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   295, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   296, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   297, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl00_super_02 ),
};

/* Char_Super 3: shinkuu_tatsu_fall (Shinkuu Tatsumaki Senpuukyaku (hyper)) */
const Sint32 pl00_super_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   102, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_super_03 ),
};

/* Char_Super 9: shin_shouryu_init (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_09[] = {
	CG_FRAME(    0,    0,   12,           0,   280, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   280, 0x80,   0x00,              ATK,  0x58,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    1,    1,           0,   281, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
};

const Sint32 pl00_super_09_loop[] = {
	CG_FRAME( 0x81,    0,    8, END_OF_DATA,   281, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl00_super_09_loop ),
};

/* Char_Super 10: shin_shouryu_dash (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_10[] = {
	CG_FRAME(    0,    0,   16,           0,   281, 0x80,   0x00,          ATK | 8,  0x00,       0x18,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   202, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    2,    0,    4,           0,   203, 0x80,   0x00,       ATK | 0x16,  0x59, 0x17 | CSE,    0,   0,     0,      0,  16 ),
	CG_FRAME(    3,    1,    1,           0,   315, 0x80,   0x00,       ATK | 0x14,  0x5a,       0x19,    0,   0,     0,      0,  70 ),
};

const Sint32 pl00_super_10_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   315, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl00_super_10_loop ),
};

/* Char_Super 16 */
const Sint32 pl00_super_16[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_super_16 ),
};

/* Char_Super 11: shin_shouryu_rise (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_11[] = {
	CG_FRAME(    0,    0,    4,           0,   316, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   317, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   318, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl00_super_11_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   319, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl00_super_11_loop ),
};

/* Char_Super 12: shin_shouryu_fall (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_12[] = {
	CG_FRAME(    0,    0,    4,           0,   320, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl00_super_12_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    69, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_super_12_loop ),
};

/* Char_Super 13: shin_shouryu_dash (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_13[] = {
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,       ATK | 0x16,  0x5b, 0x17 | CSE,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,          ATK | 2,  0x5b,       0x16,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,              ATK,  0x5b,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,              ATK,  0x5b,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,              ATK,  0x5b,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,              ATK,  0x5b,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   282, 0x80,   0x00,              ATK,  0x5b,          0,    0,   0,     0,      0,  93 ),
};

const Sint32 pl00_super_13_loop[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   282, 0x80,   0x00,              ATK,  0x5c,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl00_super_13_loop ),
};

/* Char_Super 14: shin_shouryu_rise (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_14[] = {
	CG_FRAME(    0,    0,    6,           0,   283, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
};

const Sint32 pl00_super_14_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   284, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl00_super_14_loop ),
};

/* Char_Super 15: shin_shouryu_fall (Shin Shoryuken (level 3 hyper)) */
const Sint32 pl00_super_15[] = {
	CG_FRAME(    0,    0,    6,           0,   285, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   105, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   106, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_super_15 ),
};

/* group 25 0 */
const Sint32 pl00_grp25_00[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_00 ),
};

/* group 25 1 */
const Sint32 pl00_grp25_01[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32785, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_01 ),
};

/* group 25 2 */
const Sint32 pl00_grp25_02[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_02 ),
};

/* group 25 3 */
const Sint32 pl00_grp25_03[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_03 ),
};

/* group 25 4 */
const Sint32 pl00_grp25_04[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_04 ),
};

/* group 25 5 */
const Sint32 pl00_grp25_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_05 ),
};

/* group 25 6 */
const Sint32 pl00_grp25_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_06 ),
};

/* group 25 7 */
const Sint32 pl00_grp25_07[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_07 ),
};

/* group 25 8 */
const Sint32 pl00_grp25_08[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32792, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_08 ),
};

/* group 25 9 */
const Sint32 pl00_grp25_09[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32793, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_09 ),
};

/* group 25 10 */
const Sint32 pl00_grp25_10[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32794, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_10 ),
};

/* group 25 11 */
const Sint32 pl00_grp25_11[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32795, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_11 ),
};

/* group 25 12 */
const Sint32 pl00_grp25_12[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_12 ),
};

/* group 25 13 */
const Sint32 pl00_grp25_13[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_13 ),
};

/* group 25 14 */
const Sint32 pl00_grp25_14[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_14 ),
};

/* group 25 15 */
const Sint32 pl00_grp25_15[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_15 ),
};

/* group 25 16 */
const Sint32 pl00_grp25_16[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_16 ),
};

/* group 25 17 */
const Sint32 pl00_grp25_17[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_17 ),
};

/* group 25 18 */
const Sint32 pl00_grp25_18[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32802, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_18 ),
};

/* group 25 19 */
const Sint32 pl00_grp25_19[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_19 ),
};

/* group 25 20 */
const Sint32 pl00_grp25_20[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA, 32804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_20 ),
};

/* group 25 21 */
const Sint32 pl00_grp25_21[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32805, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_NEXT( pl00_grp25_21 ),
};

/* group 25 22 */
const Sint32 pl00_grp25_22[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32806, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_22 ),
};

/* group 25 23 */
const Sint32 pl00_grp25_23[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl00_grp25_23 ),
};

/* Char_Super 4: shinkuu_init (Shinkuu Hadouken (hyper)) */
const Sint32 pl00_super_04[] = {
	CG_FRAME(    0,    0,    4,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   18,           0,   277, 0x80,   0x00,          ATK | 4,  0x00,       0x14,    0,   0,     0, 0x0300,   8 ),
	CG_FRAME(    0,    1,    4,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   8 ),
	CG_FRAME(    0,    0,    1,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0400,   8 ),
	CG_FRAME(    0, 0x80,    1,           0,   279, 0x80,   0x49,          ATK | 1,  0x00,       0x15,    0,   0,     0, 0x0500,   8 ),
	CG_FRAME(    0,    0,    1,           0,   279, 0x80,   0x49,       ATK | 0x0a,  0x00, 0x48 | CSE,    0,   0,     0, 0x0500,   8 ),
};

const Sint32 pl00_super_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   307, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  67 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   279, 0x80,   0x49,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  67 ),
	CG_NEXT( pl00_super_04_loop ),
};

/* Char_Super 5 */
const Sint32 pl00_super_05[] = {
	CG_FRAME(    0,    0,    5,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   307, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    5,           0,   279, 0x80,   0x49,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   207, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl00_super_05 ),
};

/* Char_Super 6: shinkuu_init (Shinkuu Hadouken (hyper)) */
const Sint32 pl00_super_06[] = {
	CG_FRAME(    0,    0,    2,           0,   298, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    8,           0,   299, 0x80,   0x20,          ATK | 4,  0x00,       0x14,    0,   0,     0, 0x0600,  10 ),
	CG_FRAME(    0,    1,    4,           0,   299, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  10 ),
	CG_FRAME(    0,    0,    1,           0,   300, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0700,  72 ),
	CG_FRAME(    0, 0x80,    1,           0,   301, 0x80,   0x4e,          ATK | 1,  0x00,       0x15,    0,   0,     0, 0x0800,  72 ),
	CG_FRAME(    0,    0,    1,           0,   301, 0x80,   0x4e,       ATK | 0x0a,  0x00, 0x48 | CSE,    0,   0,     0, 0x0800,  72 ),
};

const Sint32 pl00_super_06_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  72 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   301, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  72 ),
	CG_NEXT( pl00_super_06_loop ),
};

/* Char_Super 7 */
const Sint32 pl00_super_07[] = {
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   302, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    5,           0,   301, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   303, 0x80,   0x20,              ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl00_super_07 ),
};

/* Char_Effect 0
   Char_Effect 1 */
const Sint32 pl00_effect_00[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_effect_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32769, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_00_loop ),
};

/* Char_Effect 2 */
const Sint32 pl00_effect_02[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_effect_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32776, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32777, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32778, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_02_loop ),
};

/* Char_Effect 3 */
const Sint32 pl00_effect_03[] = {
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_effect_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   413, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   414, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_03_loop ),
};

/* Char_Effect 4 */
const Sint32 pl00_effect_04[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   415, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl00_effect_05[] = {
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   417, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   418, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_effect_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32808, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32809, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32810, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32811, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32812, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32813, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32814, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32815, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32816, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32817, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32818, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32819, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_05_loop ),
};

/* Char_Effect 6 */
const Sint32 pl00_effect_06[] = {
	CG_FRAME(    0,    0,    2,           0,   433, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   434, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_effect_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   435, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   436, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_06_loop ),
};

/* Char_Effect 7 */
const Sint32 pl00_effect_07[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   437, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl00_effect_08[] = {
	CG_FRAME(    0,    0,    2,           0,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   440, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl00_effect_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   421, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   422, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   423, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   424, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   425, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   426, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   427, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   428, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   429, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   430, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   431, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   432, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_08_loop ),
};

/* Char_Effect 9
   Char_Effect 10
   Char_Effect 11
   Char_Effect 12 */
const Sint32 pl00_effect_09[] = {
	CG_FRAME(    0,    0,    2,           0, 32820, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32821, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32822, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32823, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32824, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_09 ),
};

/* Char_Effect 13 */
const Sint32 pl00_effect_13[] = {
	CG_FRAME(    0,    0,    2,           0, 32825, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32826, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32827, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32828, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl00_effect_13 ),
};
