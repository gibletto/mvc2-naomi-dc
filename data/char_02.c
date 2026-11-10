/* Guile (character 02): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_02      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_02     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl02.c) says which group
                   and number each move uses.
   pl02_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl02_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl02_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl02_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_02
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_02, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_02 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl02_stand_00[], pl02_dash_00[], pl02_dash_00_loop[], pl02_dash_02[],
	pl02_dash_02_loop[], pl02_dash_01[], pl02_dash_01_loop[], pl02_dash_03[], pl02_dash_03_loop[],
	pl02_crouch_01[], pl02_stand_02[], pl02_stand_03[], pl02_crouch_02[], pl02_crouch_02_loop[],
	pl02_crouch_00[], pl02_crouch_00_loop[], pl02_turn_00[], pl02_turn_01[], pl02_turn_01_loop[],
	pl02_jump_00[], pl02_jump_00_loop[], pl02_jump_03[], pl02_jump_01[], pl02_jump_01_loop[],
	pl02_jump_02[], pl02_jump_02_loop[], pl02_jump_06[], pl02_jump_09[], pl02_guard_00[],
	pl02_guard_01[], pl02_guard_02[], pl02_guard_03[], pl02_guard_04[], pl02_guard_05[],
	pl02_guard_06[], pl02_guard_07[], pl02_air_guard_00[], pl02_air_guard_01[], pl02_unused_00[],
	pl02_unused_01[], pl02_unused_02[], pl02_unused_03[], pl02_extra_00[], pl02_extra_00_loop[],
	pl02_extra_01[], pl02_special_05[], pl02_appear_00[], pl02_win_00[], pl02_win_00_loop[],
	pl02_win_01[], pl02_win_01_loop[], pl02_win_02[], pl02_win_02_loop[], pl02_win_03[],
	pl02_win_03_loop[], pl02_win_05[], pl02_win_05_loop[], pl02_change_00[], pl02_change_01[],
	pl02_change_02[], pl02_change_03[], pl02_unused_04[], pl02_caught_00[], pl02_caught_01[],
	pl02_caught_02[], pl02_caught_03[], pl02_caught_04[], pl02_caught_05[], pl02_caught_06[],
	pl02_caught_07[], pl02_caught_08[], pl02_caught_09[], pl02_caught_10[], pl02_caught_11[],
	pl02_caught_12[], pl02_caught_13[], pl02_caught_14[], pl02_caught_15[], pl02_caught_16[],
	pl02_caught_17[], pl02_caught_18[], pl02_caught_19[], pl02_caught_20[], pl02_caught_21[],
	pl02_damage_00[], pl02_damage_01_loop[], pl02_damage_01[], pl02_damage_02[],
	pl02_damage_02_loop[], pl02_damage_03[], pl02_damage_03_loop[], pl02_damage_04[],
	pl02_damage_04_loop[], pl02_damage_05[], pl02_damage_05_loop[], pl02_damage_10[],
	pl02_damage_10_loop[], pl02_damage_11[], pl02_damage_11_loop[], pl02_damage_12[],
	pl02_damage_12_loop[], pl02_damage_06[], pl02_damage_06_loop[], pl02_damage_07[],
	pl02_damage_07_loop[], pl02_damage_08[], pl02_damage_08_loop[], pl02_damage_26[],
	pl02_damage_31[], pl02_unused_05[], pl02_unused_06[], pl02_damage_09[], pl02_damage_09_loop[],
	pl02_damage_21[], pl02_damage_22[], pl02_damage_23[], pl02_damage_24[], pl02_damage_25[],
	pl02_damage_16[], pl02_damage_17[], pl02_damage_18[], pl02_damage_19[], pl02_damage_20[],
	pl02_damage_13[], pl02_damage_13_loop[], pl02_damage_27[], pl02_damage_27_loop[],
	pl02_dizzy_00[], pl02_get_up_00[], pl02_get_up_00_loop[], pl02_get_up_01[],
	pl02_get_up_01_loop[], pl02_get_up_02[], pl02_get_up_02_loop[], pl02_get_up_03[],
	pl02_get_up_03_loop[], pl02_extra_06[], pl02_extra_06_loop[], pl02_extra_07[], pl02_damage_28[],
	pl02_damage_29[], pl02_stand_p_00[], pl02_stand_p_01[], pl02_stand_p_04[], pl02_stand_p_02[],
	pl02_stand_p_05[], pl02_stand_p_06[], pl02_stand_k_00[], pl02_stand_k_01[], pl02_stand_k_06[],
	pl02_unused_07[], pl02_unused_08[], pl02_unused_09[], pl02_unused_10[], pl02_unused_11[],
	pl02_unused_12[], pl02_unused_13[], pl02_unused_14[], pl02_unused_15[], pl02_unused_16[],
	pl02_unused_17[], pl02_unused_18[], pl02_unused_19[], pl02_unused_20[], pl02_unused_21[],
	pl02_unused_22[], pl02_unused_23[], pl02_unused_24[], pl02_unused_25[], pl02_unused_26[],
	pl02_unused_27[], pl02_unused_28[], pl02_unused_29[], pl02_unused_30[], pl02_unused_31[],
	pl02_unused_32[], pl02_unused_33[], pl02_unused_34[], pl02_unused_35[], pl02_unused_36[],
	pl02_unused_37[], pl02_unused_38[], pl02_unused_39[], pl02_unused_40[], pl02_unused_41[],
	pl02_unused_42[], pl02_unused_43[], pl02_unused_44[], pl02_unused_45[], pl02_unused_46[],
	pl02_unused_47[], pl02_unused_48[], pl02_unused_49[], pl02_unused_50[], pl02_unused_51[],
	pl02_stand_k_04[], pl02_stand_k_02[], pl02_stand_k_07[], pl02_crouch_p_00[], pl02_crouch_p_01[],
	pl02_crouch_p_02[], pl02_crouch_k_00[], pl02_crouch_k_01[], pl02_crouch_k_02[], pl02_unused_52[],
	pl02_unused_53[], pl02_jump_p_02[], pl02_jump_k_00[], pl02_jump_k_01[], pl02_jump_k_02[],
	pl02_jump_p_00[], pl02_jump_p_01[], pl02_jump_p_05[], pl02_jump_k_03[], pl02_jump_k_04[],
	pl02_jump_k_05[], pl02_extra_04[], pl02_catch_00[], pl02_catch_01[], pl02_catch_03[],
	pl02_catch_03_loop[], pl02_catch_04[], pl02_catch_04_loop[], pl02_catch_02[], pl02_unused_54[],
	pl02_unused_55[], pl02_unused_56[], pl02_special_00[], pl02_special_00_loop[], pl02_special_01[],
	pl02_special_01_loop[], pl02_super_00[], pl02_super_00_loop[], pl02_super_09[], pl02_super_10[],
	pl02_super_10_loop[], pl02_special_02[], pl02_special_02_loop[], pl02_special_03[],
	pl02_special_03_loop[], pl02_special_06[], pl02_special_06_loop[], pl02_special_08[],
	pl02_special_07[], pl02_special_07_loop[], pl02_special_04[], pl02_super_01[], pl02_super_02[],
	pl02_super_03[], pl02_super_04[], pl02_super_05[], pl02_change_04[], pl02_extra_05[],
	pl02_stand_k_08[], pl02_stand_k_08_loop[], pl02_stand_k_09[], pl02_stand_k_09_loop[],
	pl02_super_06[], pl02_super_07[], pl02_super_07_loop[], pl02_super_08[], pl02_super_08_loop[],
	pl02_unused_57[], pl02_effect_00[], pl02_effect_00_loop[], pl02_effect_01[],
	pl02_effect_01_loop[], pl02_effect_02[], pl02_effect_02_loop[], pl02_effect_04[],
	pl02_effect_05[], pl02_effect_03[], pl02_effect_06[], pl02_effect_07[], pl02_extra_02[],
	pl02_extra_03[];
extern const Sint32 *const pl02_stand_tbl[], *const pl02_jump_tbl[], *const pl02_dash_tbl[],
	*const pl02_crouch_tbl[], *const pl02_turn_tbl[], *const pl02_guard_tbl[],
	*const pl02_air_guard_tbl[], *const pl02_stand_p_tbl[], *const pl02_stand_k_tbl[],
	*const pl02_crouch_p_tbl[], *const pl02_crouch_k_tbl[], *const pl02_jump_p_tbl[],
	*const pl02_jump_k_tbl[], *const pl02_damage_tbl[], *const pl02_dizzy_tbl[],
	*const pl02_catch_tbl[], *const pl02_caught_tbl[], *const pl02_get_up_tbl[],
	*const pl02_appear_tbl[], *const pl02_win_tbl[], *const pl02_extra_tbl[],
	*const pl02_special_tbl[], *const pl02_super_tbl[], *const pl02_effect_tbl[],
	*const pl02_change_tbl[], *const pl02_grp25_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_02[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1080, 1000, -7968, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_02[] = {
	pl02_stand_tbl, pl02_jump_tbl, pl02_dash_tbl, pl02_crouch_tbl, pl02_turn_tbl, pl02_guard_tbl,
	pl02_air_guard_tbl, pl02_stand_p_tbl, pl02_stand_k_tbl, pl02_crouch_p_tbl, pl02_crouch_k_tbl, pl02_jump_p_tbl,
	pl02_jump_k_tbl, pl02_damage_tbl, pl02_dizzy_tbl, pl02_catch_tbl, pl02_caught_tbl, pl02_get_up_tbl,
	pl02_appear_tbl, pl02_win_tbl, pl02_extra_tbl, pl02_special_tbl, pl02_super_tbl, pl02_effect_tbl,
	pl02_change_tbl, pl02_grp25_tbl, 0,
};

const Sint32 *const pl02_stand_tbl[] = {
	pl02_stand_00, pl02_stand_00, pl02_stand_02, pl02_stand_03, pl02_stand_00, pl02_stand_00,
	pl02_stand_02, pl02_stand_03, 0,
};

const Sint32 *const pl02_jump_tbl[] = {
	pl02_jump_00, pl02_jump_01, pl02_jump_02, pl02_jump_03, pl02_jump_03, pl02_jump_03,
	pl02_jump_06, pl02_jump_06, pl02_jump_06, pl02_jump_09, pl02_jump_09, pl02_jump_09,
	0,
};

const Sint32 *const pl02_dash_tbl[] = {
	pl02_dash_00, pl02_dash_01, pl02_dash_02, pl02_dash_03, 0,
};

const Sint32 *const pl02_crouch_tbl[] = {
	pl02_crouch_00, pl02_crouch_01, pl02_crouch_02, 0,
};

const Sint32 *const pl02_turn_tbl[] = {
	pl02_turn_00, pl02_turn_01, 0,
};

const Sint32 *const pl02_guard_tbl[] = {
	pl02_guard_00, pl02_guard_01, pl02_guard_02, pl02_guard_03, pl02_guard_04, pl02_guard_05,
	pl02_guard_06, pl02_guard_07, 0,
};

const Sint32 *const pl02_air_guard_tbl[] = {
	pl02_air_guard_00, pl02_air_guard_01, pl02_air_guard_01, 0,
};

const Sint32 *const pl02_stand_p_tbl[] = {
	pl02_stand_p_00, pl02_stand_p_01, pl02_stand_p_02, pl02_stand_p_00, pl02_stand_p_04, pl02_stand_p_05,
	pl02_stand_p_06, 0,
};

const Sint32 *const pl02_stand_k_tbl[] = {
	pl02_stand_k_00, pl02_stand_k_01, pl02_stand_k_02, pl02_stand_k_00, pl02_stand_k_04, pl02_stand_k_02,
	pl02_stand_k_06, pl02_stand_k_07, pl02_stand_k_08, pl02_stand_k_09, 0,
};

const Sint32 *const pl02_crouch_p_tbl[] = {
	pl02_crouch_p_00, pl02_crouch_p_01, pl02_crouch_p_02, 0,
};

const Sint32 *const pl02_crouch_k_tbl[] = {
	pl02_crouch_k_00, pl02_crouch_k_01, pl02_crouch_k_02, 0,
};

const Sint32 *const pl02_jump_p_tbl[] = {
	pl02_jump_p_00, pl02_jump_p_01, pl02_jump_p_02, pl02_jump_p_00, pl02_jump_p_01, pl02_jump_p_05,
	0,
};

const Sint32 *const pl02_jump_k_tbl[] = {
	pl02_jump_k_00, pl02_jump_k_01, pl02_jump_k_02, pl02_jump_k_03, pl02_jump_k_04, pl02_jump_k_05,
	0,
};

const Sint32 *const pl02_damage_tbl[] = {
	pl02_damage_00, pl02_damage_01, pl02_damage_02, pl02_damage_03, pl02_damage_04, pl02_damage_05,
	pl02_damage_06, pl02_damage_07, pl02_damage_08, pl02_damage_09, pl02_damage_10, pl02_damage_11,
	pl02_damage_12, pl02_damage_13, pl02_damage_13, pl02_damage_13, pl02_damage_16, pl02_damage_17,
	pl02_damage_18, pl02_damage_19, pl02_damage_20, pl02_damage_21, pl02_damage_22, pl02_damage_23,
	pl02_damage_24, pl02_damage_25, pl02_damage_26, pl02_damage_27, pl02_damage_28, pl02_damage_29,
	pl02_damage_27, pl02_damage_31, pl02_damage_27, pl02_damage_27, pl02_damage_13, 0,
};

const Sint32 *const pl02_dizzy_tbl[] = {
	pl02_dizzy_00, 0,
};

const Sint32 *const pl02_catch_tbl[] = {
	pl02_catch_00, pl02_catch_01, pl02_catch_02, pl02_catch_03, pl02_catch_04, 0,
};

const Sint32 *const pl02_caught_tbl[] = {
	pl02_caught_00, pl02_caught_01, pl02_caught_02, pl02_caught_03, pl02_caught_04, pl02_caught_05,
	pl02_caught_06, pl02_caught_07, pl02_caught_08, pl02_caught_09, pl02_caught_10, pl02_caught_11,
	pl02_caught_12, pl02_caught_13, pl02_caught_14, pl02_caught_15, pl02_caught_16, pl02_caught_17,
	pl02_caught_18, pl02_caught_19, pl02_caught_20, pl02_caught_21, 0,
};

const Sint32 *const pl02_get_up_tbl[] = {
	pl02_get_up_00, pl02_get_up_01, pl02_get_up_02, pl02_get_up_03, 0,
};

const Sint32 *const pl02_appear_tbl[] = {
	pl02_appear_00, 0,
};

const Sint32 *const pl02_win_tbl[] = {
	pl02_win_00, pl02_win_01, pl02_win_02, pl02_win_03, pl02_win_03, pl02_win_05,
	pl02_win_05, 0,
};

const Sint32 *const pl02_extra_tbl[] = {
	pl02_extra_00, pl02_extra_01, pl02_extra_02, pl02_extra_03, pl02_extra_04, pl02_extra_05,
	pl02_extra_06, pl02_extra_07, 0,
};

const Sint32 *const pl02_special_tbl[] = {
	pl02_special_00, pl02_special_01, pl02_special_02, pl02_special_03, pl02_special_04, pl02_special_05,
	pl02_special_06, pl02_special_07, pl02_special_08, 0,
};

const Sint32 *const pl02_super_tbl[] = {
	pl02_super_00, pl02_super_01, pl02_super_02, pl02_super_03, pl02_super_04, pl02_super_05,
	pl02_super_06, pl02_super_07, pl02_super_08, pl02_super_09, pl02_super_10, 0,
};

const Sint32 *const pl02_effect_tbl[] = {
	pl02_effect_00, pl02_effect_01, pl02_effect_02, pl02_effect_03, pl02_effect_04, pl02_effect_05,
	pl02_effect_06, pl02_effect_07, 0,
};

const Sint32 *const pl02_change_tbl[] = {
	pl02_change_00, pl02_change_01, pl02_change_02, pl02_change_03, pl02_change_04, 0,
};

const Sint32 *const pl02_grp25_tbl[] = {
	pl02_stand_00, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   group 25 0 */
const Sint32 pl02_stand_00[] = {
	CG_FRAME(    0,    0,   6,           0,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    39, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    40, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    41, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_stand_00 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl02_dash_00[] = {
	CG_FRAME(    0,    1,   6,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_dash_00_loop[] = {
	CG_FRAME(    0,    0,  10,           0,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl02_dash_02[] = {
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_dash_02_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_dash_02_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl02_dash_01[] = {
	CG_FRAME(    0,    1,   6,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_dash_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   137, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_dash_01_loop ),
};

/* Char_Dash 3: noki_jump */
const Sint32 pl02_dash_03[] = {
	CG_FRAME(    0,    1,   3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_dash_03_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_dash_03_loop ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl02_crouch_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_crouch_01 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl02_stand_02[] = {
	CG_FRAME(    0,    0,   6,           0,    45, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    46, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    47, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    48, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    49, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    50, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl02_stand_03[] = {
	CG_FRAME(    0,    0,   3,           0,    51, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    52, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    53, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    54, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    55, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,    56, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_stand_03 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl02_crouch_02[] = {
	CG_FRAME(    0,    0,   2,           0,    44, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_crouch_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_crouch_02_loop ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl02_crouch_00[] = {
	CG_FRAME(    0,    0,   2,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    44, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl02_crouch_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_crouch_00_loop ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl02_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,    57, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    59, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl02_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,    60, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    61, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,    62, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl02_turn_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_turn_01_loop ),
};

/* Char_Jump 0 */
const Sint32 pl02_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,    42, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,  16,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl02_jump_00_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl02_jump_00_loop ),
};

/* Char_Jump 3: crossfire_catch (Crossfire Assault (hyper, in the air))
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl02_jump_03[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_jump_03 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl02_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,    68, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    72, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl02_jump_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl02_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl02_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,    76, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    75, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    74, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    73, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    77, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    71, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    70, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    69, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

const Sint32 pl02_jump_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    68, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl02_jump_02_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl02_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,    42, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,  16,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    66, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
};

/* Char_Jump 9: crossfire_dash (Crossfire Assault (hyper, in the air)), act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl02_jump_09[] = {
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    67, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl02_jump_09 ),
};

/* Char_Guard 0 */
const Sint32 pl02_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl02_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl02_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    78, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl02_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    79, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl02_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,    81, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl02_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    81, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl02_guard_06[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    80, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl02_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    81, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl02_air_guard_00[] = {
	CG_FRAME(    0,    0,   4,           0,    83, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl02_air_guard_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    82, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_air_guard_01 ),
};

const Sint32 pl02_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00000d04,
};

const Sint32 pl02_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00000d58,
};

const Sint32 pl02_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00000dac,
};

const Sint32 pl02_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00000e00,
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl02_extra_00[] = {
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   217, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
};

const Sint32 pl02_extra_00_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   217, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl02_extra_00_loop ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl02_extra_01[] = {
	CG_FRAME(    0,    0,   9,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   112, 0x80,   0x00,                0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_extra_01 ),
};

/* Char_Special 5: snapback_init (the snapback) */
const Sint32 pl02_special_05[] = {
	CG_FRAME(    0,    0,  10,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   5,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,  10,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,   231, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_special_05 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl02_appear_00[] = {
	CG_FRAME(    0,    0,  32,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,                0,  0x00,       0x0c,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  12,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_appear_00 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl02_win_00[] = {
	CG_FRAME(    0,    0,   5,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,                0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_win_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_win_00_loop ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl02_win_01[] = {
	CG_FRAME(    0,    0,   8,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_win_01_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_win_01_loop ),
};

/* Char_Win 2: win_ko_init */
const Sint32 pl02_win_02[] = {
	CG_FRAME(    0,    0,   5,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   117, 0x80,   0x00,                0,  0x00,       0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_win_02_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_win_02_loop ),
};

/* Char_Win 3: win_ko_init
   Char_Win 4: win_ko_init */
const Sint32 pl02_win_03[] = {
	CG_FRAME(    0,    0,   5,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   125, 0x80,   0x00,                0,  0x00,       0x11,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_win_03_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_win_03_loop ),
};

/* Char_Win 5: lose_time_ko
   Char_Win 6: draw_time */
const Sint32 pl02_win_05[] = {
	CG_FRAME(    0,    0,   5,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   351, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_win_05_loop[] = {
	CG_FRAME(    0,    0,   5, END_OF_DATA,   350, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_win_05_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl02_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   101, 0x80,   0x00,                0,  0x00,       0x0a,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4, END_OF_DATA,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl02_change_01[] = {
	CG_FRAME( 0x80,    0,   5,           0,   124, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   125, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl02_change_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl02_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl02_change_03[] = {
	CG_FRAME(    0,    0,   8,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   3,           0,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_change_03 ),
};

const Sint32 pl02_unused_04[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   5,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x0000175c,
};

/* Char_Caught 0 */
const Sint32 pl02_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl02_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl02_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl02_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl02_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl02_caught_05[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl02_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   335, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl02_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl02_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   336, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl02_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl02_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   337, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_NEXT( pl02_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl02_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl02_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl02_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl02_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   357, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl02_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   377, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl02_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   379, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl02_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   380, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl02_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   381, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl02_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   383, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl02_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   418, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl02_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   424, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl02_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   360, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl02_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl02_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl02_caught_21 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl02_damage_00[] = {
	CG_FRAME(    0,    0,   3,           0,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_damage_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_damage_01_loop ),
};

/* Char_Damage 1 */
const Sint32 pl02_damage_01[] = {
	CG_FRAME(    0,    0,   6,           0,   320, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_damage_01_loop ),
};

/* Char_Damage 2 */
const Sint32 pl02_damage_02[] = {
	CG_FRAME(    0,    0,   8,           0,   321, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   322, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_damage_02_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   319, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_damage_02_loop ),
};

/* Char_Damage 3 */
const Sint32 pl02_damage_03[] = {
	CG_FRAME(    0,    0,   3,           0,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_damage_03_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_damage_03_loop ),
};

/* Char_Damage 4 */
const Sint32 pl02_damage_04[] = {
	CG_FRAME(    0,    0,   6,           0,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_damage_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_damage_04_loop ),
};

/* Char_Damage 5 */
const Sint32 pl02_damage_05[] = {
	CG_FRAME(    0,    0,   8,           0,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_damage_05_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_damage_05_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl02_damage_10[] = {
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl02_damage_10_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_10_loop ),
};

/* Char_Damage 11 */
const Sint32 pl02_damage_11[] = {
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl02_damage_11_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_11_loop ),
};

/* Char_Damage 12 */
const Sint32 pl02_damage_12[] = {
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   330, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl02_damage_12_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   327, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_12_loop ),
};

/* Char_Damage 6 */
const Sint32 pl02_damage_06[] = {
	CG_FRAME(    0,    0,   8,           0,   335, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl02_damage_06_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   335, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_damage_06_loop ),
};

/* Char_Damage 7 */
const Sint32 pl02_damage_07[] = {
	CG_FRAME(    0,    0,   6,           0,   335, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   8,           0,   336, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl02_damage_07_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_damage_07_loop ),
};

/* Char_Damage 8 */
const Sint32 pl02_damage_08[] = {
	CG_FRAME(    0,    0,   8,           0,   336, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,  12,           0,   337, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl02_damage_08_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   335, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl02_damage_08_loop ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl02_damage_26[] = {
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   358, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl02_damage_31[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   360, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl02_damage_31 ),
};

const Sint32 pl02_unused_05[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   8,           0,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   334, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00001cd8,
};

const Sint32 pl02_unused_06[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   355, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   388, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00001cf0,
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl02_damage_09[] = {
	CG_FRAME(    0,    0,   6,           0,   338, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   339, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   357, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   357, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   358, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   359, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl02_damage_09_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   360, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl02_damage_09_loop ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl02_damage_21[] = {
	CG_FRAME(    0,    0,   8,           0,   377, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl02_damage_22[] = {
	CG_FRAME(    0,    0,   8,           0,   379, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl02_damage_23[] = {
	CG_FRAME(    0,    0,   8,           0,   380, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl02_damage_24[] = {
	CG_FRAME(    0,    0,   8,           0,   381, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl02_damage_25[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   383, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_25 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl02_damage_16[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   377, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl02_damage_17[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   379, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl02_damage_18[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   380, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl02_damage_19[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   381, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl02_damage_20[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   383, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_20 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl02_damage_13[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

const Sint32 pl02_damage_13_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   359, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl02_damage_13_loop ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ...
   Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl02_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   398, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   399, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   400, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl02_damage_27_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_27_loop ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl02_dizzy_00[] = {
	CG_FRAME(    0,    0,  12,           0,   350, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   351, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,   352, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12, END_OF_DATA,   351, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_dizzy_00 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl02_get_up_00[] = {
	CG_FRAME(    0,    0,   4,           0,   342, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   345, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_get_up_00_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_get_up_00_loop ),
};

/* Char_Get_Up 1 */
const Sint32 pl02_get_up_01[] = {
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_get_up_01_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_get_up_01_loop ),
};

/* Char_Get_Up 2 */
const Sint32 pl02_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_get_up_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_get_up_02_loop ),
};

/* Char_Get_Up 3 */
const Sint32 pl02_get_up_03[] = {
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   349, 0x80,   0xce,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   348, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   347, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   346, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_get_up_03_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_get_up_03_loop ),
};

/* Char_Extra 6: crossfire_dash (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_extra_06[] = {
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   399, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,    64, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl02_extra_06_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_extra_06_loop ),
};

/* Char_Extra 7: crossfire_miss (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_extra_07[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_extra_07 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl02_damage_28[] = {
	CG_FRAME(    0,    0,   3,           0,   418, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   419, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   420, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   422, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   423, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl02_damage_29[] = {
	CG_FRAME(    0,    0,   3,           0,   424, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   425, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   426, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   427, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   428, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   429, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl02_damage_29 ),
};

/* Char_Stand_P 0 (light)
   Char_Stand_P 3 */
const Sint32 pl02_stand_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   144, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,   4,           0,   144, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   3,           0,   145, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   143, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl02_stand_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   146, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,   5,           0,   148, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   6,           0,   149, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_p_01 ),
};

/* Char_Stand_P 4 */
const Sint32 pl02_stand_p_04[] = {
	CG_FRAME(    0,    0,   5,           0,   291, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,   2,           0,   292, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,   4,           0,   293, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   4,           0,   294, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,   3,           0,   295, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_p_04 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl02_stand_p_02[] = {
	CG_FRAME(    0,    0,   6,           0,   150, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   151, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,   1,           0,   151, 0x80,   0x00,              ATK,  0x00,       0x04,    0,   0,     0,      0, 111 ),
	CG_FRAME( 0x80,    0,   3,           0,   152, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,   8,           0,   153, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   6,           0,   154, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,   5,           0,   155, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_p_02 ),
};

/* Char_Stand_P 5 */
const Sint32 pl02_stand_p_05[] = {
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   296, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   4,           0,   299, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,   5,           0,   300, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   4,           0,   301, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   3,           0,   302, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_p_05 ),
};

/* Char_Stand_P 6 */
const Sint32 pl02_stand_p_06[] = {
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   228, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   5,           0,   229, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,  10,           0,   230, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,   8,           0,   231, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_p_06 ),
};

/* Char_Stand_K 0 (light)
   Char_Stand_K 3 */
const Sint32 pl02_stand_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   156, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   2,           0,   158, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   3,           0,   156, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl02_stand_k_01[] = {
	CG_FRAME(    0,    0,   3,           0,   159, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   161, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_FRAME(    0,    0,   2,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   4,           0,   162, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_FRAME(    0,    0,   4,           0,   163, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_01 ),
};

/* Char_Stand_K 6 */
const Sint32 pl02_stand_k_06[] = {
	CG_FRAME(    0,    0,   3,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   304, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   4,           0,   305, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   306, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   4,           0,   306, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,   4,           0,   307, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   3,           0,   308, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,   2,           0,   309, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   3,           0,    44, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_06 ),
};

const Sint32 pl02_unused_07[] = {	/* nothing reaches it */
	0x00020000,
};

const Sint32 pl02_unused_08[] = {
	0x000000a5,
};

const Sint32 pl02_unused_09[] = {
	0x00000000,
};

const Sint32 pl02_unused_10[] = {
	0x00000000,
};

const Sint32 pl02_unused_11[] = {
	0x00040000,
};

const Sint32 pl02_unused_12[] = {
	0x000000a6,
};

const Sint32 pl02_unused_13[] = {
	0x00000000,
};

const Sint32 pl02_unused_14[] = {
	0x00000000,
};

const Sint32 pl02_unused_15[] = {
	0x00020000,
};

const Sint32 pl02_unused_16[] = {
	0x000000a7,
};

const Sint32 pl02_unused_17[] = {
	0x00000000,
};

const Sint32 pl02_unused_18[] = {
	0x00000000,
};

const Sint32 pl02_unused_19[] = {
	0x00020000,
};

const Sint32 pl02_unused_20[] = {
	0x000000a8,
};

const Sint32 pl02_unused_21[] = {
	0x00000000,
};

const Sint32 pl02_unused_22[] = {
	0x00000000,
};

const Sint32 pl02_unused_23[] = {
	0x00020000,
};

const Sint32 pl02_unused_24[] = {
	0x000000a8,
};

const Sint32 pl02_unused_25[] = {
	0x00000000,
};

const Sint32 pl02_unused_26[] = {
	0x00000000,
};

const Sint32 pl02_unused_27[] = {
	0x00040000,
};

const Sint32 pl02_unused_28[] = {
	0x000000a9,
};

const Sint32 pl02_unused_29[] = {
	0x00000000,
};

const Sint32 pl02_unused_30[] = {
	0x00000000,
};

const Sint32 pl02_unused_31[] = {
	0x00050000,
};

const Sint32 pl02_unused_32[] = {
	0x000000aa,
};

const Sint32 pl02_unused_33[] = {
	0x00000000,
};

const Sint32 pl02_unused_34[] = {
	0x00000000,
};

const Sint32 pl02_unused_35[] = {
	0x00040000,
};

const Sint32 pl02_unused_36[] = {
	0x000000ab,
};

const Sint32 pl02_unused_37[] = {
	0x00000000,
};

const Sint32 pl02_unused_38[] = {
	0x00000000,
};

const Sint32 pl02_unused_39[] = {
	0x00030000,
};

const Sint32 pl02_unused_40[] = {
	0x000000ac,
};

const Sint32 pl02_unused_41[] = {
	0x00000000,
};

const Sint32 pl02_unused_42[] = {
	0x00000000,
};

const Sint32 pl02_unused_43[] = {
	0x00020000,
};

const Sint32 pl02_unused_44[] = {
	0x0000002a,
};

const Sint32 pl02_unused_45[] = {
	0x00000000,
};

const Sint32 pl02_unused_46[] = {
	0x00000000,
};

const Sint32 pl02_unused_47[] = {
	0x80020000,
};

const Sint32 pl02_unused_48[] = {
	0x0000002a,
};

const Sint32 pl02_unused_49[] = {
	0x00000000,
};

const Sint32 pl02_unused_50[] = {
	0x00000000,
};

const Sint32 pl02_unused_51[] = {
	0x00002a08,
};

/* Char_Stand_K 4 */
const Sint32 pl02_stand_k_04[] = {
	CG_FRAME(    0,    0,   3,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   279, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   2,           0,   279, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   3,           0,   280, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   3,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   281, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_04 ),
};

/* Char_Stand_K 2 (heavy)
   Char_Stand_K 5 */
const Sint32 pl02_stand_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   284, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   2,           0,   284, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   2,           0,   284, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   6,           0,   285, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   7,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   288, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    58, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,    59, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_02 ),
};

/* Char_Stand_K 7 */
const Sint32 pl02_stand_k_07[] = {
	CG_FRAME(    0,    0,   2,           0,    68, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    69, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    70, 0x80,   0x00,              ATK,  0x00,       0x03,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   4,           0,   236, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   6,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   4,           0,   238, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   3,           0,   239, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,    76, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_07 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl02_crouch_p_00[] = {
	CG_FRAME(    0,    0,   3,           0,   173, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   1,           0,   174, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   1,           0,   174, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   4,           0,   175, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   173, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl02_crouch_p_01[] = {
	CG_FRAME(    0,    0,   5,           0,   176, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   177, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   2,           0,   177, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   5,           0,   178, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   4,           0,   176, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl02_crouch_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5,           0,   179, 0x80,   0x4e,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(   48,    0,   2,           0,   180, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME( 0x80,    0,   2,           0,   180, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   1,           0,   181, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,  11,           0,   181, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   5,           0,   182, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   185, 0x80,   0x4e,              ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl02_crouch_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   188, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   2,           0,   188, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   187, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   3,           0,   186, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl02_crouch_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   189, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   190, 0x80,   0x4e,              ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   191, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   191, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6,           0,   192, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   5,           0,   193, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   194, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl02_crouch_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   195, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   196, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   3,           0,   197, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   4,           0,   198, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   6,           0,   199, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   8,           0,   200, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6,           0,   189, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   190, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    1,   2,           0,   191, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   3,           0,   191, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   4,           0,   192, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,   4,           0,   193, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    44, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_crouch_k_02 ),
};

const Sint32 pl02_unused_52[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   203, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	0x00003184,
};

const Sint32 pl02_unused_53[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   203, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	0x00003200,
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl02_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   201, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x20,              ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_p_02 ),
};

/* Char_Jump_K 0 (light) */
const Sint32 pl02_jump_k_00[] = {
	CG_FRAME(    0,    0,   2,           0,   205, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   207, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   4,           0,   208, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_k_00 ),
};

/* Char_Jump_K 1 (medium) */
const Sint32 pl02_jump_k_01[] = {
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   5,           0,   210, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   212, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   2,           0,   214, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl02_jump_k_02[] = {
	CG_FRAME(    0,    0,   3,           0,   215, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   217, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   217, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl02_jump_p_00[] = {
	CG_FRAME(    0,    0,   1,           0,   201, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   203, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   4,           0,   204, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl02_jump_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   221, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   8,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   1,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_p_01 ),
};

/* Char_Jump_P 5 */
const Sint32 pl02_jump_p_05[] = {
	CG_FRAME(    0,    0,   4,           0,   220, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   221, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0, 0x0300,   7 ),
	CG_FRAME(    0,    0,   4,           0,   222, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  78 ),
	CG_FRAME(    0,    0,   4,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_p_05 ),
};

/* Char_Jump_K 3 */
const Sint32 pl02_jump_k_03[] = {
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,  10,           0,   224, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_k_03 ),
};

/* Char_Jump_K 4 */
const Sint32 pl02_jump_k_04[] = {
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   6,           0,   226, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,   4,           0,   223, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_k_04 ),
};

/* Char_Jump_K 5 */
const Sint32 pl02_jump_k_05[] = {
	CG_FRAME(    0,    0,   3,           0,   215, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0x20,              ATK,  0x00,       0x04,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   3,           0,   217, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   4,           0,   218, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   4,           0,   219, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    63, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl02_jump_k_05 ),
};

/* Char_Extra 4: pl02_upper */
const Sint32 pl02_extra_04[] = {
	CG_FRAME(    0,    0,   2,           0,   185, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x80,   0x4e,              ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   180, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   1,           0,   181, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,  11,           0,   181, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   5,           0,   182, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   4,           0,   183, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   1,           0,   184, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   185, 0x80,   0x4e,              ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl02_extra_04 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl02_catch_00[] = {
	CG_FRAME(    0,    0,  15,           0,   240, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,  10,           0,   241, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   6,           0,   242, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,   5,           0,   243, 0x80,   0x00,                0,  0x00,       0x04,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,  24,           0,   244, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   6,           0,   245, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   246, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_catch_00 ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl02_catch_01[] = {
	CG_FRAME(    0,    0,  15,           0,   247, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   6,           0,   248, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    0,    0,   6,           0,   249, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   6,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   6,           0,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   6,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,   6,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,    60, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,    61, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_catch_01 ),
};

/* Char_Catch 3: jump_k_set */
const Sint32 pl02_catch_03[] = {
	CG_FRAME(    0,    0,  15,           0,   257, 0x80,   0x00,                0,  0x00,       0x05,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    0,   8,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   8,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
};

const Sint32 pl02_catch_03_loop[] = {
	CG_FRAME(    0,    1,   2, END_OF_DATA,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_NEXT( pl02_catch_03_loop ),
};

/* Char_Catch 4: jump_k_catch */
const Sint32 pl02_catch_04[] = {
	CG_FRAME(    0,   -1,   4,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    0,   4,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    1,   1,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_catch_04_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_catch_04_loop ),
};

/* Char_Catch 2: jump_p_set */
const Sint32 pl02_catch_02[] = {
	CG_FRAME(    0,    0,  15,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,                0,  0x00,       0x04,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    1,   2,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_catch_02 ),
};

const Sint32 pl02_unused_54[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  16,           0,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   290, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003b54,
};

const Sint32 pl02_unused_55[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   227, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   228, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   229, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,  10,           0,   230, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   8,           0,   231, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003c0c,
};

const Sint32 pl02_unused_56[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   304, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   305, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   5,           0,   306, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   307, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   308, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   309, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    44, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    43, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    42, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003cec,
};

/* Char_Special 0 */
const Sint32 pl02_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   270, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    1,    0,  16,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   5,           0,   275, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl02_special_00_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_special_00_loop ),
};

/* Char_Special 1 */
const Sint32 pl02_special_01[] = {
	CG_FRAME(    0,    0,   2,           0,   270, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   271, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   2,           0,   272, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    1,    0,  16,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   5,           0,   275, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   276, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl02_special_01_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_special_01_loop ),
};

/* Char_Super 0: hurricane_init (Sonic Hurricane (hyper)) */
const Sint32 pl02_super_00[] = {
	CG_FRAME(    0,    0,   3,           0,   270, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,   270, 0x80,   0x00,              ATK,  0x00,       0x17,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   4,           0,   271, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   272, 0x80,   0x00,              ATK,  0x00,       0x18,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_super_00_loop[] = {
	CG_FRAME(    0,    1,   3, END_OF_DATA,   274, 0x80,   0x00,              ATK,  0x00,       0x1e,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_super_00_loop ),
};

/* Char_Super 9: hurricane_move (Sonic Hurricane (hyper)) */
const Sint32 pl02_super_09[] = {
	CG_FRAME(    0,    0,   2,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl02_super_09 ),
};

/* Char_Super 10: hurricane_keep (Sonic Hurricane (hyper)) */
const Sint32 pl02_super_10[] = {
	CG_FRAME(    0,    0,  16,           0,   275, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   6,           0,   276, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl02_super_10_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    38, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_super_10_loop ),
};

/* Char_Special 2: smr_init (Somersault Kick, 2 + atck_level) */
const Sint32 pl02_special_02[] = {
	CG_FRAME(    0,    1,   4,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    1,    0,   1,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl02_special_02_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   269, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl02_special_02_loop ),
};

/* Char_Special 3 */
const Sint32 pl02_special_03[] = {
	CG_FRAME(    0,    1,   4,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,   5,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    1,   5,           0,   268, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl02_special_03_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   269, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl02_special_03_loop ),
};

/* Char_Special 6: jsmr_init (Somersault Kick in the air, 6 + atck_level) */
const Sint32 pl02_special_06[] = {
	CG_FRAME(    0,    1,   2,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   260, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    1,    0,   1,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   2,           0,   266, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl02_special_06_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   269, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl02_special_06_loop ),
};

/* Char_Special 8: crossfire_kick_jump (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_special_08[] = {
	CG_FRAME(    0,    0,   4,           0,    63, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    67, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_NEXT( pl02_special_08 ),
};

/* Char_Special 7: crossfire_kick_init (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_special_07[] = {
	CG_FRAME(    0,    1,   4,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,   1,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    1,    0,   5,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   267, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   5,           0,   268, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
};

const Sint32 pl02_special_07_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   269, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_NEXT( pl02_special_07_loop ),
};

/* Char_Special 4: smr_jump (Somersault Kick), jsmr_jump, crossfire_kick_jump (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_special_04[] = {
	CG_FRAME(    0,    0,   6,           0,    42, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   5,           0,    43, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    44, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_special_04 ),
};

/* Char_Super 1: strike_init (Somersault Strike (hyper)) */
const Sint32 pl02_super_01[] = {
	CG_FRAME(    0,    0,   4,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,   1,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Super 2: strike_move (Somersault Strike (hyper)) */
const Sint32 pl02_super_02[] = {
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0, 0x0200,  82 ),
	CG_FRAME(    0,    0,   2,           0,   263, 0x80,   0x00,              ATK,  0x01,          0,    0,   0,     0, 0x0500,  82 ),
	CG_FRAME(    0,    2,   2,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0700,  82 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  83 ),
	CG_FRAME(    0,    2,   2,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0900,  84 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0900,  84 ),
	CG_FRAME(    0,    0,   1,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0100,  83 ),
	CG_FRAME(    0,    0,   1,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   1,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   1,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   269, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  83 ),
	CG_NEXT( pl02_super_01 ),
};

/* Char_Super 3: strike_move (Somersault Strike (hyper)) */
const Sint32 pl02_super_03[] = {
	CG_FRAME(    0,    0,   1,           0,   258, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   259, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,              ATK,  0x00,       0x15,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   262, 0x80,   0x00,              ATK,  0x00,       0x1f,    0,   0,     0, 0x0200,  82 ),
	CG_FRAME(    0,    0,   2,           0,   263, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0500,  82 ),
	CG_FRAME(    0,   10,   3,           0,   264, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0600,  82 ),
	CG_FRAME(    0,   12,   3,           0,   265, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   3,           0,   266, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0800,  84 ),
	CG_FRAME(    0,   12,   3,           0,   267, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   3,           0,   268, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   269, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0xfe00,  84 ),
	CG_NEXT( pl02_super_02 ),
};

/* Char_Super 4: strike_air (Somersault Strike (hyper)) */
const Sint32 pl02_super_04[] = {
	CG_FRAME(    0,    0,   6,           0,   268, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   269, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0xfe00,  64 ),
	CG_NEXT( pl02_super_04 ),
};

/* Char_Super 5: strike_fall (Somersault Strike (hyper)) */
const Sint32 pl02_super_05[] = {
	CG_FRAME(    0,    0,   8,           0,    42, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,    43, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,    44, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_super_05 ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl02_change_04[] = {
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   7,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_change_04 ),
};

/* Char_Extra 5: chouhatsu_init (the taunt) */
const Sint32 pl02_extra_05[] = {
	CG_FRAME(    0,    0,   5,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    96, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    95, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    94, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    93, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    92, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,    91, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    90, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,    89, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    88, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,    87, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,  12,           0,    86, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    85, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,    84, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_extra_05 ),
};

/* Char_Stand_K 8 */
const Sint32 pl02_stand_k_08[] = {
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,   7,           0,   233, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  25 ),
};

const Sint32 pl02_stand_k_08_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   233, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_NEXT( pl02_stand_k_08_loop ),
};

/* Char_Stand_K 9: lever_lk_jump */
const Sint32 pl02_stand_k_09[] = {
	CG_FRAME(    0,    0,   2,           0,   234, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,   3,           0,    43, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl02_stand_k_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,    42, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl02_stand_k_09_loop ),
};

/* Char_Super 6: crossfire_init (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_super_06[] = {
	CG_FRAME(    0,    0,   5,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   4,           0,    65, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   9,           0,    66, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,   3,           0,    66, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,   3,           0,    64, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   131, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 121 ),
	CG_NEXT( pl02_super_06 ),
};

/* Char_Super 7: crossfire_dash (Crossfire Assault (hyper, in the air)) */
const Sint32 pl02_super_07[] = {
	CG_FRAME(    0,    0,   5,           0,   225, 0x80,   0x20,              ATK,  0x4b,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   224, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_FRAME(    0,    0,   3,           0,   225, 0x80,   0x20,              ATK,  0x49,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,   2,           0,    69, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,    71, 0x80,   0x00,              ATK,  0x4a,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   237, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,   3,           0,    74, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,    75, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,   3,           0,   210, 0x80,   0x20,              ATK,  0x49,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   1,           0,   211, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   3,           0,   213, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,   3,           0,   216, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   2,           0,   217, 0x80,   0x20,              ATK,  0x4a,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   3,           0,   219, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,   3,           0,   232, 0x80,   0x00, ATK | CG_ATCK_NG,  0x49,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   233, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   206, 0x80,   0x20,              ATK,  0x4c,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   207, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
};

const Sint32 pl02_super_07_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   208, 0x80,   0x20,              ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl02_super_07_loop ),
};

/* Char_Super 8 */
const Sint32 pl02_super_08[] = {
	CG_FRAME(    0,    0,   3,           0,   132, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   133, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   6,           0,    44, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,    43, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl02_super_08_loop[] = {
	CG_FRAME(    0,    0,   6, END_OF_DATA,    42, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl02_super_08_loop ),
};

const Sint32 pl02_unused_57[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   4,           0,   310, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   311, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   312, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   313, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   314, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   316, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   317, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,    38, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00004c34,
};

/* Char_Effect 0 */
const Sint32 pl02_effect_00[] = {
	CG_FRAME(    0,    0,   1,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl02_effect_00_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   439, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   440, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl02_effect_00_loop ),
};

/* Char_Effect 1 */
const Sint32 pl02_effect_01[] = {
	CG_FRAME(    0,    0,   1,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME(    0,    0,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl02_effect_01_loop[] = {
	CG_FRAME(    0,    0,   1,           0,   439, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   440, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl02_effect_01_loop ),
};

/* Char_Effect 2 */
const Sint32 pl02_effect_02[] = {
	CG_FRAME(    0,    0,   4,           0,   445, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   446, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   447, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   448, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   449, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   450, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl02_effect_02_loop[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,    -1, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_effect_02_loop ),
};

/* Char_Effect 4: shl02_01_init */
const Sint32 pl02_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   1,           0, 32791, 0x80,   0x00,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   1,           0, 32792, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32795, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl02_effect_04 ),
};

/* Char_Effect 5: shl02_01_part_init, shl02_04_init */
const Sint32 pl02_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_effect_05 ),
};

/* Char_Effect 3: shl02_02_move */
const Sint32 pl02_effect_03[] = {
	CG_FRAME(    0,    0,   1,           0,   433, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   1,           0,   434, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,   1,           0,   435, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,    -1, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   0 ),
	CG_NEXT( pl02_effect_03 ),
};

/* Char_Effect 6: shl02_03_init */
const Sint32 pl02_effect_06[] = {
	CG_FRAME(    0,    0,   2,           0,   436, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   0 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0400,   0 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   438, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   0 ),
	CG_NEXT( pl02_effect_06 ),
};

/* Char_Effect 7: shl02_05_init */
const Sint32 pl02_effect_07[] = {
	CG_FRAME(    0,    0,   1,           0, 32768, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32769, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32770, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32771, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32772, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32773, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32774, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32775, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32776, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32777, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32778, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32779, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32780, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32781, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32782, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32783, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32784, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32785, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32786, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32787, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0, 32788, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32789, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl02_effect_07 ),
};

/* Char_Extra 2: shl02_06_init, fly_start5 */
const Sint32 pl02_extra_02[] = {
	CG_FRAME(    0,    0,   1,           0, 32802, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0, 32803, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1,           0, 32804, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32805, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  81 ),
	CG_NEXT( pl02_extra_02 ),
};

/* Char_Extra 3: shl02_06_init, fly_start5 */
const Sint32 pl02_extra_03[] = {
	CG_FRAME(    0,    0,   1,           0, 32802, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0, 32803, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1,           0, 32804, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  85 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32805, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  85 ),
	CG_NEXT( pl02_extra_03 ),
};
