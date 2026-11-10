/* Cyclops (character 06): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_06      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_06     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl06.c) says which group
                   and number each move uses.
   pl06_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl06_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl06_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl06_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_06
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_06, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_06 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl06_stand_00[], pl06_stand_06[], pl06_stand_02[], pl06_stand_03[],
	pl06_stand_05[], pl06_jump_00[], pl06_jump_00_loop[], pl06_jump_01[], pl06_jump_01_loop[],
	pl06_jump_02[], pl06_jump_02_loop[], pl06_jump_03[], pl06_jump_06[], pl06_jump_09[],
	pl06_jump_07[], pl06_jump_07_loop[], pl06_jump_08[], pl06_jump_08_loop[], pl06_dash_00[],
	pl06_dash_01[], pl06_crouch_00[], pl06_crouch_01[], pl06_crouch_02[], pl06_turn_00[],
	pl06_turn_01[], pl06_guard_00[], pl06_guard_01[], pl06_guard_02[], pl06_guard_03[],
	pl06_guard_04[], pl06_guard_05[], pl06_guard_06[], pl06_guard_07[], pl06_air_guard_00[],
	pl06_air_guard_02[], pl06_air_guard_01[], pl06_stand_p_00[], pl06_stand_p_03[],
	pl06_stand_p_01[], pl06_stand_p_02[], pl06_stand_k_00[], pl06_stand_k_03[], pl06_stand_k_01[],
	pl06_stand_k_02[], pl06_stand_k_04[], pl06_crouch_p_00[], pl06_crouch_p_01[], pl06_crouch_p_02[],
	pl06_crouch_k_00[], pl06_crouch_k_01[], pl06_crouch_k_02[], pl06_jump_p_00[], pl06_jump_p_01[],
	pl06_jump_p_02[], pl06_jump_p_03[], pl06_jump_p_04[], pl06_jump_k_00[], pl06_jump_k_01[],
	pl06_jump_k_02[], pl06_jump_k_03[], pl06_jump_k_04[], pl06_jump_k_05[], pl06_get_up_00[],
	pl06_get_up_01[], pl06_get_up_02[], pl06_get_up_03[], pl06_damage_00[], pl06_damage_01[],
	pl06_damage_02[], pl06_damage_03[], pl06_damage_04[], pl06_damage_05[], pl06_damage_06[],
	pl06_damage_07[], pl06_damage_08[], pl06_damage_10[], pl06_damage_11[], pl06_damage_12[],
	pl06_damage_13[], pl06_damage_13_loop[], pl06_damage_09[], pl06_damage_09_loop[],
	pl06_damage_16[], pl06_damage_17[], pl06_damage_18[], pl06_damage_19[], pl06_damage_20[],
	pl06_damage_21[], pl06_damage_22[], pl06_damage_23[], pl06_damage_24[], pl06_damage_25[],
	pl06_damage_26[], pl06_damage_30[], pl06_damage_32[], pl06_damage_27[], pl06_damage_34[],
	pl06_damage_28[], pl06_damage_29[], pl06_damage_31[], pl06_dizzy_00[], pl06_appear_00[],
	pl06_appear_00_loop[], pl06_appear_01[], pl06_caught_00[], pl06_caught_01[], pl06_caught_02[],
	pl06_caught_03[], pl06_caught_04[], pl06_caught_05[], pl06_caught_06[], pl06_caught_07[],
	pl06_caught_08[], pl06_caught_09[], pl06_caught_10[], pl06_caught_11[], pl06_caught_12[],
	pl06_caught_13[], pl06_caught_14[], pl06_caught_15[], pl06_caught_16[], pl06_caught_17[],
	pl06_caught_18[], pl06_caught_19[], pl06_caught_20[], pl06_extra_00[], pl06_extra_13[],
	pl06_extra_01[], pl06_extra_14[], pl06_extra_02[], pl06_extra_02_loop[], pl06_extra_03[],
	pl06_extra_03_loop[], pl06_extra_04[], pl06_extra_05[], pl06_extra_05_loop[], pl06_extra_15[],
	pl06_extra_06[], pl06_extra_06_loop[], pl06_unused_00[], pl06_special_00[],
	pl06_special_00_loop[], pl06_special_04[], pl06_special_01[], pl06_special_01_loop[],
	pl06_special_05[], pl06_special_02[], pl06_special_02_loop[], pl06_special_06[],
	pl06_special_03[], pl06_special_03_loop[], pl06_special_07[], pl06_special_08[],
	pl06_special_09[], pl06_special_10[], pl06_special_11[], pl06_special_12[], pl06_special_13[],
	pl06_special_14[], pl06_special_15[], pl06_special_16[], pl06_special_17[], pl06_special_18[],
	pl06_special_19[], pl06_special_20[], pl06_special_21[], pl06_special_22[], pl06_special_23[],
	pl06_special_24[], pl06_special_25[], pl06_special_26[], pl06_special_27[], pl06_special_28[],
	pl06_special_29[], pl06_special_30[], pl06_special_31[], pl06_special_32[], pl06_special_33[],
	pl06_special_34[], pl06_special_35[], pl06_special_36[], pl06_special_37[], pl06_special_38[],
	pl06_special_39[], pl06_special_40[], pl06_special_41[], pl06_special_42[], pl06_special_43[],
	pl06_special_44[], pl06_special_45[], pl06_special_45_loop[], pl06_special_46[], pl06_super_00[],
	pl06_super_00_loop[], pl06_super_08[], pl06_super_08_loop[], pl06_super_09[],
	pl06_super_09_loop[], pl06_super_10[], pl06_super_01[], pl06_super_02[], pl06_special_47[],
	pl06_win_00[], pl06_win_00_loop[], pl06_win_01[], pl06_win_01_loop[], pl06_win_02[],
	pl06_win_02_loop[], pl06_win_03[], pl06_win_03_loop[], pl06_win_04[], pl06_win_04_loop[],
	pl06_win_05[], pl06_win_05_loop[], pl06_catch_00[], pl06_catch_01[], pl06_catch_02[],
	pl06_catch_03[], pl06_catch_04[], pl06_catch_04_loop[], pl06_catch_05[], pl06_catch_05_loop[],
	pl06_catch_06[], pl06_catch_06_loop[], pl06_catch_07[], pl06_catch_07_loop[], pl06_catch_08[],
	pl06_catch_08_loop[], pl06_catch_09[], pl06_catch_10[], pl06_change_00[], pl06_change_02[],
	pl06_change_03[], pl06_change_03_loop[], pl06_change_04[], pl06_change_04_loop[],
	pl06_change_01[], pl06_extra_07[], pl06_extra_07_loop[], pl06_extra_08[], pl06_super_03[],
	pl06_super_04[], pl06_super_04_loop[], pl06_super_05[], pl06_super_06[], pl06_super_07[],
	pl06_extra_09[], pl06_extra_10[], pl06_extra_11[], pl06_extra_12[], pl06_extra_16[],
	pl06_extra_17[], pl06_extra_18[], pl06_effect_00[], pl06_grp25_00[], pl06_grp27_02[],
	pl06_grp27_03[], pl06_grp27_03_loop[], pl06_grp27_04[], pl06_grp27_05[], pl06_grp27_06[],
	pl06_grp27_07[], pl06_grp27_08[], pl06_grp27_00[], pl06_grp27_01[];
extern const Sint32 *const pl06_stand_tbl[], *const pl06_jump_tbl[], *const pl06_dash_tbl[],
	*const pl06_crouch_tbl[], *const pl06_turn_tbl[], *const pl06_guard_tbl[],
	*const pl06_air_guard_tbl[], *const pl06_stand_p_tbl[], *const pl06_stand_k_tbl[],
	*const pl06_crouch_p_tbl[], *const pl06_crouch_k_tbl[], *const pl06_jump_p_tbl[],
	*const pl06_jump_k_tbl[], *const pl06_damage_tbl[], *const pl06_dizzy_tbl[],
	*const pl06_catch_tbl[], *const pl06_caught_tbl[], *const pl06_get_up_tbl[],
	*const pl06_appear_tbl[], *const pl06_win_tbl[], *const pl06_extra_tbl[],
	*const pl06_special_tbl[], *const pl06_super_tbl[], *const pl06_effect_tbl[],
	*const pl06_change_tbl[], *const pl06_grp25_tbl[], *const pl06_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_06[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 1, 0, 0 },
	{ 1000, 1000, -16384, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_06[] = {
	pl06_stand_tbl, pl06_jump_tbl, pl06_dash_tbl, pl06_crouch_tbl, pl06_turn_tbl, pl06_guard_tbl,
	pl06_air_guard_tbl, pl06_stand_p_tbl, pl06_stand_k_tbl, pl06_crouch_p_tbl, pl06_crouch_k_tbl, pl06_jump_p_tbl,
	pl06_jump_k_tbl, pl06_damage_tbl, pl06_dizzy_tbl, pl06_catch_tbl, pl06_caught_tbl, pl06_get_up_tbl,
	pl06_appear_tbl, pl06_win_tbl, pl06_extra_tbl, pl06_special_tbl, pl06_super_tbl, pl06_effect_tbl,
	pl06_change_tbl, pl06_grp25_tbl, pl06_stand_tbl, pl06_grp27_tbl, 0,
};

const Sint32 *const pl06_stand_tbl[] = {
	pl06_stand_00, pl06_stand_00, pl06_stand_02, pl06_stand_03, pl06_stand_00, pl06_stand_05,
	pl06_stand_06, pl06_stand_06, 0,
};

const Sint32 *const pl06_jump_tbl[] = {
	pl06_jump_00, pl06_jump_01, pl06_jump_02, pl06_jump_03, pl06_jump_03, pl06_jump_03,
	pl06_jump_06, pl06_jump_07, pl06_jump_08, pl06_jump_09, pl06_jump_09, pl06_jump_09,
	0,
};

const Sint32 *const pl06_dash_tbl[] = {
	pl06_dash_00, pl06_dash_01, pl06_stand_00, pl06_dash_01, 0,
};

const Sint32 *const pl06_crouch_tbl[] = {
	pl06_crouch_00, pl06_crouch_01, pl06_crouch_02, 0,
};

const Sint32 *const pl06_turn_tbl[] = {
	pl06_turn_00, pl06_turn_01, 0,
};

const Sint32 *const pl06_guard_tbl[] = {
	pl06_guard_00, pl06_guard_01, pl06_guard_02, pl06_guard_03, pl06_guard_04, pl06_guard_05,
	pl06_guard_06, pl06_guard_07, 0,
};

const Sint32 *const pl06_air_guard_tbl[] = {
	pl06_air_guard_00, pl06_air_guard_01, pl06_air_guard_02, 0,
};

const Sint32 *const pl06_stand_p_tbl[] = {
	pl06_stand_p_00, pl06_stand_p_01, pl06_stand_p_02, pl06_stand_p_03, 0,
};

const Sint32 *const pl06_stand_k_tbl[] = {
	pl06_stand_k_00, pl06_stand_k_01, pl06_stand_k_02, pl06_stand_k_03, pl06_stand_k_04, 0,
};

const Sint32 *const pl06_crouch_p_tbl[] = {
	pl06_crouch_p_00, pl06_crouch_p_01, pl06_crouch_p_02, 0,
};

const Sint32 *const pl06_crouch_k_tbl[] = {
	pl06_crouch_k_00, pl06_crouch_k_01, pl06_crouch_k_02, 0,
};

const Sint32 *const pl06_jump_p_tbl[] = {
	pl06_jump_p_00, pl06_jump_p_01, pl06_jump_p_02, pl06_jump_p_03, pl06_jump_p_04, 0,
};

const Sint32 *const pl06_jump_k_tbl[] = {
	pl06_jump_k_00, pl06_jump_k_01, pl06_jump_k_02, pl06_jump_k_03, pl06_jump_k_04, pl06_jump_k_05,
	0,
};

const Sint32 *const pl06_damage_tbl[] = {
	pl06_damage_00, pl06_damage_01, pl06_damage_02, pl06_damage_03, pl06_damage_04, pl06_damage_05,
	pl06_damage_06, pl06_damage_07, pl06_damage_08, pl06_damage_09, pl06_damage_10, pl06_damage_11,
	pl06_damage_12, pl06_damage_13, pl06_damage_13, pl06_damage_13, pl06_damage_16, pl06_damage_17,
	pl06_damage_18, pl06_damage_19, pl06_damage_20, pl06_damage_21, pl06_damage_22, pl06_damage_23,
	pl06_damage_24, pl06_damage_25, pl06_damage_26, pl06_damage_27, pl06_damage_28, pl06_damage_29,
	pl06_damage_30, pl06_damage_31, pl06_damage_32, pl06_damage_27, pl06_damage_34, 0,
};

const Sint32 *const pl06_dizzy_tbl[] = {
	pl06_dizzy_00, 0,
};

const Sint32 *const pl06_catch_tbl[] = {
	pl06_catch_00, pl06_catch_01, pl06_catch_02, pl06_catch_03, pl06_catch_04, pl06_catch_05,
	pl06_catch_06, pl06_catch_07, pl06_catch_08, pl06_catch_09, pl06_catch_10, 0,
};

const Sint32 *const pl06_caught_tbl[] = {
	pl06_caught_00, pl06_caught_01, pl06_caught_02, pl06_caught_03, pl06_caught_04, pl06_caught_05,
	pl06_caught_06, pl06_caught_07, pl06_caught_08, pl06_caught_09, pl06_caught_10, pl06_caught_11,
	pl06_caught_12, pl06_caught_13, pl06_caught_14, pl06_caught_15, pl06_caught_16, pl06_caught_17,
	pl06_caught_18, pl06_caught_19, pl06_caught_20, pl06_caught_20, 0,
};

const Sint32 *const pl06_get_up_tbl[] = {
	pl06_get_up_00, pl06_get_up_01, pl06_get_up_02, pl06_get_up_03, 0,
};

const Sint32 *const pl06_appear_tbl[] = {
	pl06_appear_00, pl06_appear_01, 0,
};

const Sint32 *const pl06_win_tbl[] = {
	pl06_win_00, pl06_win_01, pl06_win_02, pl06_win_03, pl06_win_04, pl06_win_05,
	0,
};

const Sint32 *const pl06_extra_tbl[] = {
	pl06_extra_00, pl06_extra_01, pl06_extra_02, pl06_extra_03, pl06_extra_04, pl06_extra_05,
	pl06_extra_06, pl06_extra_07, pl06_extra_08, pl06_extra_09, pl06_extra_10, pl06_extra_11,
	pl06_extra_12, pl06_extra_13, pl06_extra_14, pl06_extra_15, pl06_extra_16, pl06_extra_17,
	pl06_extra_18, 0,
};

const Sint32 *const pl06_special_tbl[] = {
	pl06_special_00, pl06_special_01, pl06_special_02, pl06_special_03, pl06_special_04, pl06_special_05,
	pl06_special_06, pl06_special_07, pl06_special_08, pl06_special_09, pl06_special_10, pl06_special_11,
	pl06_special_12, pl06_special_13, pl06_special_14, pl06_special_15, pl06_special_16, pl06_special_17,
	pl06_special_18, pl06_special_19, pl06_special_20, pl06_special_21, pl06_special_22, pl06_special_23,
	pl06_special_24, pl06_special_25, pl06_special_26, pl06_special_27, pl06_special_28, pl06_special_29,
	pl06_special_30, pl06_special_31, pl06_special_32, pl06_special_33, pl06_special_34, pl06_special_35,
	pl06_special_36, pl06_special_37, pl06_special_38, pl06_special_39, pl06_special_40, pl06_special_41,
	pl06_special_42, pl06_special_43, pl06_special_44, pl06_special_45, pl06_special_46, pl06_special_47,
	0,
};

const Sint32 *const pl06_super_tbl[] = {
	pl06_super_00, pl06_super_01, pl06_super_02, pl06_super_03, pl06_super_04, pl06_super_05,
	pl06_super_06, pl06_super_07, pl06_super_08, pl06_super_09, pl06_super_10, 0,
};

const Sint32 *const pl06_effect_tbl[] = {
	pl06_effect_00, 0,
};

const Sint32 *const pl06_change_tbl[] = {
	pl06_change_00, pl06_change_01, pl06_change_02, pl06_change_03, pl06_change_04, 0,
};

const Sint32 *const pl06_grp25_tbl[] = {
	pl06_grp25_00, 0,
};

const Sint32 *const pl06_grp27_tbl[] = {
	pl06_grp27_00, pl06_grp27_01, pl06_grp27_02, pl06_grp27_03, pl06_grp27_04, pl06_grp27_05,
	pl06_grp27_06, pl06_grp27_07, pl06_grp27_08, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow              atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Dash 2
   Char_Fly 0
   Char_Fly 1: act_fly */
const Sint32 pl06_stand_00[] = {
	CG_FRAME(    0,    0,    1,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    2,           0,    98, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    3,           0,    99, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    4,           0,   100, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    5,           0,   101, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    6,           0,   102, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    7,           0,   103, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   104, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   11,           0,   105, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,   10,           0,   106, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    8,           0,   107, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    7,           0,   108, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    7,           0,   109, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    7,           0,   110, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    7,           0,   111, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    8,           0,   112, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   31, END_OF_DATA,   113, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl06_stand_00 ),
};

/* Char_Stand 6
   Char_Stand 7
   Char_Fly 6
   Char_Fly 7 */
const Sint32 pl06_stand_06[] = {
	CG_FRAME(    0,    0,    4,           0,    97, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    4,           0,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   273, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   274, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_stand_06 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Fly 2 */
const Sint32 pl06_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,    4,           0,   118, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0700,   1 ),
	CG_FRAME(    0,    0,    5,           0,   119, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0900,   1 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0300,   1 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0500,   1 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0600,   1 ),
	CG_FRAME(    0,    0,    4,           0,   123, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0700,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   124, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0800,   1 ),
	CG_NEXT( pl06_stand_02 ),
};

/* Char_Stand 3
   Char_Fly 3 */
const Sint32 pl06_stand_03[] = {
	CG_FRAME(    0,    0,    8,           0,   125, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,    8,           0,   126, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0d00,   1 ),
	CG_FRAME(    0,    0,    8,           0,   127, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0b00,   1 ),
	CG_FRAME(    0,    0,    8,           0,   128, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_FRAME(    0,    0,    8,           0,   129, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   1 ),
	CG_FRAME(    0,    0,    8,           0,   130, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0c00,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   131, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0a00,   1 ),
	CG_NEXT( pl06_stand_03 ),
};

/* Char_Stand 5
   Char_Fly 5 */
const Sint32 pl06_stand_05[] = {
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   133, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_stand_00 ),
};

/* Char_Jump 0 */
const Sint32 pl06_jump_00[] = {
	CG_FRAME(    0,    1,    2,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    2,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,   10,           0,   240, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    8,           0,   245, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    2,           0,   246, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
};

const Sint32 pl06_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   247, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl06_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl06_jump_01[] = {
	CG_FRAME(    0,    1,    1,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    3,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   249, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   250, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   251, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   252, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   253, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   255, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   256, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   257, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   258, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    3,           0,   259, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
};

const Sint32 pl06_jump_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   260, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl06_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl06_jump_02[] = {
	CG_FRAME(    0,    1,    1,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    1,    3,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
};

const Sint32 pl06_jump_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   262, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_NEXT( pl06_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl06_jump_03[] = {
	CG_FRAME(    0,    0,    2,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_stand_03 ),
};

/* Char_Jump 6 */
const Sint32 pl06_jump_06[] = {
	CG_FRAME(    0,    1,    1,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    3,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   240, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   241, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   240, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   241, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   242, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   244, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   246, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl06_jump_09[] = {
	CG_FRAME(    0,    0,    1,           0,   247, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   248, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl06_jump_07[] = {
	CG_FRAME(    0,    1,    1,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    3,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   249, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   250, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   259, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl06_jump_07_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   260, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   261, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl06_jump_08[] = {
	CG_FRAME(    0,    1,    1,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    3,           0,   161, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl06_jump_08_loop[] = {
	CG_FRAME(    0,    0,    1,           0,   262, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   263, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_jump_08_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl06_dash_00[] = {
	CG_FRAME(    0,    1,    2,           0,   134, 0x80,   0x57,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   143, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   144, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   145, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl06_dash_00 ),
};

/* Char_Dash 1: act_back_dash
   Char_Dash 3 */
const Sint32 pl06_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   155, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   156, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   157, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   158, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   159, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_dash_01 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl06_crouch_00[] = {
	CG_FRAME(    0,    3,    2,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1500,   2 ),
	CG_FRAME(    0,    2,    3,           0,   161, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x1600,   2 ),
	CG_FRAME(    0,    1,    4,           0,   162, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x1700,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   163, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x1800,   2 ),
	CG_NEXT( pl06_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl06_crouch_01[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   114, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x0e00,   2 ),
	CG_NEXT( pl06_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl06_crouch_02[] = {
	CG_FRAME(    0,    3,    3,           0,   164, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x1900,   1 ),
	CG_FRAME(    0,    2,    4,           0,   165, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0, 0x1a00,   1 ),
	CG_FRAME(    0,    1,    3,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1b00,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x1c00,   1 ),
	CG_NEXT( pl06_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl06_turn_00[] = {
	CG_FRAME(    0,    0,    3,           0,   168, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl06_turn_01[] = {
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   173, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   175, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl06_guard_00[] = {
	CG_FRAME(    0,    0,    1,           0,   176, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   177, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   179, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl06_guard_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   178, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl06_guard_02[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   179, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl06_guard_03[] = {
	CG_FRAME(    0,    0,    4,           0,   180, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   178, 0x80,   0x51,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_guard_01 ),
};

/* Char_Guard 4 */
const Sint32 pl06_guard_04[] = {
	CG_FRAME(    0,    0,    1,           0,   181, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   182, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   183, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   185, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl06_guard_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   184, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl06_guard_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   185, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl06_guard_07[] = {
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   184, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl06_air_guard_00[] = {
	CG_FRAME(    0,    0,    1,           0,   187, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl06_air_guard_02[] = {
	CG_FRAME(    0,    0,    1,           0,   188, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl06_air_guard_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   189, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl06_stand_p_00[] = {
	CG_FRAME(    0,    0,    5,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   275, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2,           0,   274, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_p_00 ),
};

/* Char_Stand_P 3 */
const Sint32 pl06_stand_p_03[] = {
	CG_FRAME(    0,    0,    3,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3,           0,   277, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME( 0x80,    0,    2,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   278, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    6,           0,   279, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_p_03 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl06_stand_p_01[] = {
	CG_FRAME(    0,    0,    5,           0,   280, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x4c,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0, 100 ),
	CG_FRAME( 0x80,    0,    2,           0,   281, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME( 0x81,    0,    5,           0,   282, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    2,           0,   283, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,    2,           0,   285, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   286, 0x80,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl06_stand_p_02[] = {
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0x00, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    8,           0,   289, 0x80,   0x6a, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2,           0,   290, 0x80,   0x6a, ATK | CG_ATCK_NG | 6,  0x00,       0x1e,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   292, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   292, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    6,           0,   293, 0x80,   0x6a, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   286, 0x80,   0x00, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl06_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   294, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    1,           0,   295, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   296, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x82,    0,    4,           0,   296, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    1,    4,           0,   297, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_k_00 ),
};

/* Char_Stand_K 3 */
const Sint32 pl06_stand_k_03[] = {
	CG_FRAME(    0,    0,    1,           0,   298, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    2,           0,   299, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    1,           0,   300, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x82,    0,    4,           0,   301, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    4,           0,   302, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_k_03 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl06_stand_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   304, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,    4,           0,   305, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    5,           0,   305, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   306, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   307, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   308, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl06_stand_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   309, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   311, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    1,           0,   312, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    0,    1,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    1,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    1,    7,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    1,    1,           0,   314, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    1,    4,           0,   315, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   316, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   317, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_k_02 ),
};

/* Char_Stand_K 4: stand_kick_move */
const Sint32 pl06_stand_k_04[] = {
	CG_FRAME(    0,    0,    1,           0,   317, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    4,           0,   318, 0x80,   0x00,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   319, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    2,           0,   320, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,    5,           0,   321, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME( 0x80,    0,    2,           0,   322, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME( 0x82,    0,    1,           0,   323, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   324, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   160, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   161, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   164, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   165, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_stand_k_04 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl06_crouch_p_00[] = {
	CG_FRAME(    0,    0,    5,           0,   325, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    2,           0,   326, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    2,           0,   325, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   327, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl06_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl06_crouch_p_01[] = {
	CG_FRAME(    0,    0,    6,           0,   328, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    1,           0,   329, 0x80,   0x4d,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  26 ),
	CG_FRAME( 0x80,    0,    1,           0,   330, 0x80,   0x4d,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    4,           0,   330, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    4,           0,   331, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   327, 0x80,   0x4d,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl06_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl06_crouch_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   332, 0x80,   0x40, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   333, 0x80,   0x40, ATK | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    1,    4,           0,   334, 0x80,   0x40, ATK | CG_ATCK_NG | 6,  0x00,       0x1e,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   334, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   334, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   334, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    7,           0,   335, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   327, 0x80,   0x40, ATE | CG_ATCK_NG,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_NEXT( pl06_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl06_crouch_k_00[] = {
	CG_FRAME(    0,    0,    5,           0,   337, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    2,           0,   336, 0x80,   0xa4,              ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    4,           0,   336, 0x80,   0xa4,              ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    4,           0,   337, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   337, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl06_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl06_crouch_k_01[] = {
	CG_FRAME(    0,    0,    3,           0,   338, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    1,           0,   339, 0x80,   0x40,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x81,    0,    5,           0,   339, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    8,           0,   339, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   340, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   341, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   162, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl06_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl06_crouch_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   342, 0x70,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   343, 0x70,   0xa1,              ATK,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    2,           0,   344, 0x80,   0xa1,              ATK,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    9,           0,   344, 0x80,   0xa1,              ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    6,           0,   345, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,    6,           0,   346, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   162, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl06_crouch_k_02 ),
};

/* Char_Jump_P 0 (light) */
const Sint32 pl06_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   347, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   348, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   10,           0,   349, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,    8,           0,   349, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_jump_p_00 ),
};

/* Char_Jump_P 1 (medium) */
const Sint32 pl06_jump_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    2,           0,   351, 0x80,   0x00,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  60 ),
	CG_FRAME( 0x80,    0,    4,           0,   352, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    6,           0,   352, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   358, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl06_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl06_jump_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   353, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   354, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    1,           0,   355, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    8,           0,   356, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME( 0x81,    0,    8,           0,   357, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   358, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl06_jump_p_02 ),
};

/* Char_Jump_P 3 */
const Sint32 pl06_jump_p_03[] = {
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   355, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    2,           0,   485, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x80,    0,    2,           0,   486, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME( 0x82,    0,    2,           0,   486, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   358, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl06_jump_p_03 ),
};

/* Char_Jump_P 4 */
const Sint32 pl06_jump_p_04[] = {
	CG_FRAME(    0,    0,    7,           0,   476, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    2,           0,   477, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    2,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    5,           0,   478, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   479, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl06_jump_p_04 ),
};

/* Char_Jump_K 0 (light) */
const Sint32 pl06_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   359, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,   10,           0,   360, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME( 0x80,    0,    8,           0,   360, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   360, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_jump_k_00 ),
};

/* Char_Jump_K 1 (medium) */
const Sint32 pl06_jump_k_01[] = {
	CG_FRAME(    0,    0,    1,           0,   359, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   362, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    3,           0,   363, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME( 0x82,    0,    2,           0,   363, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    8,           0,   363, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    3,           0,   364, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   365, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl06_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl06_jump_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   350, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    3,           0,   318, 0x80,   0x00,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   319, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   320, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    3,           0,   321, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   324, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_jump_k_02 ),
};

/* Char_Jump_K 3 */
const Sint32 pl06_jump_k_03[] = {
	CG_FRAME(    0,    0,    4,           0,   359, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   10,           0,   501, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    8,           0,   501, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   361, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_jump_k_03 ),
};

/* Char_Jump_K 4 */
const Sint32 pl06_jump_k_04[] = {
	CG_FRAME(    0,    0,    4,           0,   480, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   481, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x81,    0,    8,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4,           0,   483, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   484, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_jump_k_04 ),
};

/* Char_Jump_K 5 */
const Sint32 pl06_jump_k_05[] = {
	CG_FRAME(    0,    0,    6,           0,   472, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    1,           0,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    1,           0,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    6,           0,   473, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0,   474, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   475, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_jump_k_05 ),
};

/* Char_Get_Up 0: neckbreaker_catch_throw, dm_dizzy_wait */
const Sint32 pl06_get_up_00[] = {
	CG_FRAME(    0,    0,    5,           0,   221, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   222, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   223, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   224, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl06_get_up_01[] = {
	CG_FRAME(    0,    0,    1,           0,   221, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   233, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl06_get_up_02[] = {
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl06_get_up_03[] = {
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   230, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   225, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_get_up_03 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl06_damage_00[] = {
	CG_FRAME(    0,    0,    7,           0,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   545, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl06_damage_01[] = {
	CG_FRAME(    0,    0,   10,           0,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   547, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   548, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl06_damage_02[] = {
	CG_FRAME(    0,    0,    4,           0,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   550, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   551, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   552, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   553, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   554, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   555, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   556, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl06_damage_03[] = {
	CG_FRAME(    0,    0,    7,           0,   557, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   558, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl06_damage_04[] = {
	CG_FRAME(    0,    0,    8,           0,   559, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   560, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   561, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl06_damage_05[] = {
	CG_FRAME( 0x81,    0,    8,           0,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   564, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   565, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   566, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   567, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl06_damage_06[] = {
	CG_FRAME(    0,    0,    8,           0,   568, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   569, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl06_damage_07[] = {
	CG_FRAME(    0,    0,    8,           0,   570, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   571, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   569, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl06_damage_08[] = {
	CG_FRAME(    0,    0,    6,           0,   572, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   573, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   574, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   575, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   576, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   577, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   569, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl06_damage_10[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   557, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl06_damage_11[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   560, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl06_damage_12[] = {
	CG_FRAME(    0,    0,   10, END_OF_DATA,   563, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15 */
const Sint32 pl06_damage_13[] = {
	CG_FRAME(    0,    0,    2,           0,   582, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   583, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   584, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
};

const Sint32 pl06_damage_13_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   221, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl06_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl06_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   585, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   586, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   587, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   588, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   589, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   590, 0x80,   0xc8,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    1,           0,   582, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   583, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   584, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
};

const Sint32 pl06_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   221, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl06_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl06_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   593, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl06_damage_17[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   594, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl06_damage_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   595, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl06_damage_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   596, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl06_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl06_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   593, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl06_damage_22[] = {
	CG_FRAME(    0,    0,    4,           0,   594, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl06_damage_23[] = {
	CG_FRAME(    0,    0,    4,           0,   595, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl06_damage_24[] = {
	CG_FRAME(    0,    0,    4,           0,   596, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl06_damage_25[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl06_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   598, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   599, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   600, 0x80,   0x40,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   601, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2,           0,   602, 0x80,   0xad,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   603, 0x80,   0xc8,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl06_damage_26 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl06_damage_30[] = {
	CG_FRAME(    0,    0,    3,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   250, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_30 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl06_damage_32[] = {
	CG_FRAME(    0,    0,    3,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_32 ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl06_damage_27[] = {
	CG_FRAME(    0,    0,    6,           0,   599, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   600, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   622, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   163, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   114, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_27 ),
};

/* Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl06_damage_34[] = {
	CG_FRAME(    0,    0,    3,           0,   595, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   596, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   586, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   598, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_FRAME(    0,    0,    0,           0,   588, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    0,           0,   587, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   594, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_34 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl06_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   605, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   607, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   608, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   609, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl06_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   610, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   611, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   612, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   613, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   614, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   615, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl06_damage_31[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   591, 0x80,   0xc8,                0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl06_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl06_dizzy_00[] = {
	CG_FRAME(    0,    1,    2,           0,   616, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   616, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   616, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   616, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   618, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   619, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   621, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   621, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   621, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   619, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   618, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_dizzy_00 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl06_appear_00[] = {
	CG_FRAME(    0,    0,   16,           0,   208, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   209, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   210, 0x80,   0x0d,             0x0f,  0x00,       0x21,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,   18,           0,   211, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   212, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   213, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    1,    0,    8,           0,   213, 0x80,   0x0d,             0x15,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
};

const Sint32 pl06_appear_00_loop[] = {
	CG_FRAME(    2,    0,   64, END_OF_DATA,   213, 0x80,   0x0d,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_appear_00_loop ),
};

/* Char_Appear 1: appear_fade_out */
const Sint32 pl06_appear_01[] = {
	CG_FRAME(    0,    0,    4,           0,   169, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   170, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   171, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_appear_01 ),
};

/* Char_Caught 0 */
const Sint32 pl06_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   544, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl06_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   546, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl06_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   549, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl06_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   557, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl06_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   559, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl06_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   562, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl06_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   568, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl06_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl06_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   570, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl06_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl06_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   572, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl06_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl06_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   578, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl06_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   579, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl06_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl06_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   580, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  84 ),
	CG_NEXT( pl06_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl06_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   582, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl06_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   592, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl06_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   594, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl06_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   595, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl06_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   596, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl06_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   597, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl06_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl06_caught_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   604, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl06_caught_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   610, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl06_caught_19 ),
};

/* Char_Caught 20
   Char_Caught 21 */
const Sint32 pl06_caught_20[] = {
	CG_FRAME(    0,    0,   16, END_OF_DATA,   591, 0x80,   0xc8,                0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl06_caught_20 ),
};

/* Char_Extra 0: shl06_00_shot_init */
const Sint32 pl06_extra_00[] = {
	CG_FRAME(    0,    0,    3,           0, 32840, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32841, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32842, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32843, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl06_extra_00 ),
};

/* Char_Extra 13 */
const Sint32 pl06_extra_13[] = {
	CG_FRAME(    0,    0,    3,           0, 32844, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32845, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32846, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32847, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl06_extra_13 ),
};

/* Char_Extra 1: shl06_00_up_init */
const Sint32 pl06_extra_01[] = {
	CG_FRAME(    0,    0,    3,           0, 32848, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0, 32849, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0, 32850, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32851, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl06_extra_01 ),
};

/* Char_Extra 14 */
const Sint32 pl06_extra_14[] = {
	CG_FRAME(    0,    0,    3,           0, 32852, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0, 32853, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0, 32854, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32855, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_NEXT( pl06_extra_14 ),
};

/* Char_Extra 2: kick_init (Gene Splice), fly_start5 */
const Sint32 pl06_extra_02[] = {
	CG_FRAME(    0,   15,    1,           0,   161, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,   15,    4,           0,   487, 0x80,   0x00,       ATK | 0x1a,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   488, 0x80,   0x00,       ATK | 0x12,  0x00,       0x15,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    1,   12,           0,   489, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    2,    6,           0,   489, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    3,    1,           0,   494, 0x80,   0x00,          ATK | 6,  0x00,       0x1e,    0,   0,     0,      0,  66 ),
};

const Sint32 pl06_extra_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   495, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl06_extra_02_loop ),
};

/* Char_Extra 3: kick_fall (Gene Splice), fly_start5 */
const Sint32 pl06_extra_03[] = {
	CG_FRAME(    0,    0,    6,           0,   353, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   354, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
};

const Sint32 pl06_extra_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   247, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   248, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl06_extra_03_loop ),
};

/* Char_Extra 4: kick_land (Gene Splice) */
const Sint32 pl06_extra_04[] = {
	CG_FRAME(    0,    0,    6,           0,   160, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   165, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_extra_04 ),
};

/* Char_Extra 5 */
const Sint32 pl06_extra_05[] = {
	CG_FRAME(    0,    0,    2,           0,   490, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME( 0x80,    0,    2,           0,   491, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    8,           0,   492, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    2,    4,           0,   493, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    3,    1,           0,   494, 0x80,   0x00,          ATK | 6,  0x00,       0x1e,    0,   0,     0,      0,  66 ),
};

const Sint32 pl06_extra_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   494, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl06_extra_05_loop ),
};

/* Char_Extra 15 */
const Sint32 pl06_extra_15[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   492, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_extra_15 ),
};

/* Char_Extra 6 */
const Sint32 pl06_extra_06[] = {
	CG_FRAME(    0,    0,    2,           0,   488, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   12,           0,   489, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    2,    6,           0,   489, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    3,    1,           0,   494, 0x80,   0x00,          ATK | 6,  0x00,       0x1e,    0,   0,     0,      0,  66 ),
};

const Sint32 pl06_extra_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   494, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   495, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl06_extra_06_loop ),
};

const Sint32 pl06_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   496, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   497, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_extra_04 ),
};

/* Char_Special 0 */
const Sint32 pl06_special_00[] = {
	CG_FRAME(    0,    0,    6,           0,   377, 0x80,   0x00,          ATK | 1,  0x00,       0x14,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0x80,   0x00,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,  70 ),
};

const Sint32 pl06_special_00_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   379, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   380, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl06_special_00_loop ),
};

/* Char_Special 4 */
const Sint32 pl06_special_04[] = {
	CG_FRAME(    0,    1,   24,           0,   379, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   380, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl06_special_04 ),
};

/* Char_Special 1 */
const Sint32 pl06_special_01[] = {
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x00,          ATK | 1,  0x00,       0x14,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    4,           0,   385, 0x80,   0x00,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,  71 ),
};

const Sint32 pl06_special_01_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   385, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   386, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl06_special_01_loop ),
};

/* Char_Special 5 */
const Sint32 pl06_special_05[] = {
	CG_FRAME(    0,    0,   24,           0,   385, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   286, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_NEXT( pl06_special_05 ),
};

/* Char_Special 2 */
const Sint32 pl06_special_02[] = {
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x00,          ATK | 1,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   387, 0x80,   0x00,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,   7 ),
};

const Sint32 pl06_special_02_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   387, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   388, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_02_loop ),
};

/* Char_Special 6 */
const Sint32 pl06_special_06[] = {
	CG_FRAME(    0,    0,   24,           0,   387, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   286, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_06 ),
};

/* Char_Special 3: blast_init */
const Sint32 pl06_special_03[] = {
	CG_FRAME(    0,    0,    4,           0,   353, 0x80,   0x00,          ATK | 1,  0x00,       0x14,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   354, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   355, 0x80,   0x00,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,  57 ),
};

const Sint32 pl06_special_03_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   389, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   390, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl06_special_03_loop ),
};

/* Char_Special 7 */
const Sint32 pl06_special_07[] = {
	CG_FRAME(    0,    0,   22,           0,   389, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   350, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_special_07 ),
};

/* Char_Special 8: shl06_01_char_set */
const Sint32 pl06_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,   623, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   624, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32808, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32809, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   627, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32810, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   629, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32811, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   631, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32812, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   634, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   635, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32814, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32815, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   638, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32816, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   640, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32817, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   642, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32818, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32819, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   645, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   646, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32820, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32821, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32822, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   651, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32823, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   653, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32824, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32825, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   656, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   657, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32826, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32827, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   660, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32828, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   662, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32829, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   664, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32830, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32831, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   667, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   668, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32808, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32809, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   671, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32810, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   673, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32811, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   675, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32812, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0x80,   0x00,              ATK,  0x00,          0,    2,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   656, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   657, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32826, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32827, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   660, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32828, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   662, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32829, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   664, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32830, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32831, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   645, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   646, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32820, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32821, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   649, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32822, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   651, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32823, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   653, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32824, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32825, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   634, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   635, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32814, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32815, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   638, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32816, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   640, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32817, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   642, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32818, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32819, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   711, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   712, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32808, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32809, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   715, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32810, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0,   717, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    4,           0, 32811, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   719, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32812, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0x80,   0x00,              ATK,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   722, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   723, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   724, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   725, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   726, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   727, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   728, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   729, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   730, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   731, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   732, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   733, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   734, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   735, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   736, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   737, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   738, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   739, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_special_08 ),
};

/* Char_Special 9: super_init (Super Optic Blast (hyper)) */
const Sint32 pl06_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   530, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   531, 0x80,   0x6a,       ATK | 0x14,  0x00,       0x18,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    7,           0,   532, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,          ATK | 8,  0x00,       0x20,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   397, 0x80,   0x00,       ATK | 0x10,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   398, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_09 ),
};

/* Char_Special 10 */
const Sint32 pl06_special_10[] = {
	CG_FRAME(    0,    0,    3,           0,   410, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   411, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_10 ),
};

/* Char_Special 11 */
const Sint32 pl06_special_11[] = {
	CG_FRAME(    0,    0,    3,           0,   412, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   413, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_11 ),
};

/* Char_Special 12 */
const Sint32 pl06_special_12[] = {
	CG_FRAME(    0,    0,    3,           0,   414, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   415, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl06_special_13[] = {
	CG_FRAME(    0,    0,    3,           0,   416, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   417, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl06_special_14[] = {
	CG_FRAME(    0,    0,    3,           0,   418, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   419, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl06_special_15[] = {
	CG_FRAME(    0,    0,    3,           0,   420, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   421, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl06_special_16[] = {
	CG_FRAME(    0,    0,    3,           0,   422, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   423, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_16 ),
};

/* Char_Special 17 */
const Sint32 pl06_special_17[] = {
	CG_FRAME(    0,    0,    3,           0,   424, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   425, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_17 ),
};

/* Char_Special 18 */
const Sint32 pl06_special_18[] = {
	CG_FRAME(    0,    0,    3,           0,   426, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   427, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_18 ),
};

/* Char_Special 19 */
const Sint32 pl06_special_19[] = {
	CG_FRAME(    0,    0,    3,           0,   428, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   429, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_19 ),
};

/* Char_Special 20 */
const Sint32 pl06_special_20[] = {
	CG_FRAME(    0,    0,    3,           0,   430, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   431, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_20 ),
};

/* Char_Special 21 */
const Sint32 pl06_special_21[] = {
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   433, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_21 ),
};

/* Char_Special 22 */
const Sint32 pl06_special_22[] = {
	CG_FRAME(    0,    0,    3,           0,   399, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   403, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl06_special_23[] = {
	CG_FRAME(    0,    0,    3,           0,   404, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   405, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_23 ),
};

/* Char_Special 24 */
const Sint32 pl06_special_24[] = {
	CG_FRAME(    0,    0,    3,           0,   406, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   407, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_24 ),
};

/* Char_Special 25 */
const Sint32 pl06_special_25[] = {
	CG_FRAME(    0,    0,    3,           0,   408, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   409, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_25 ),
};

/* Char_Special 26 */
const Sint32 pl06_special_26[] = {
	CG_FRAME(    0,    0,    6,           0,   398, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   397, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   396, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   395, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   393, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   392, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_26 ),
};

/* Char_Special 27: super_init (Super Optic Blast (hyper)) */
const Sint32 pl06_special_27[] = {
	CG_FRAME(    0,    0,    8,           0,   435, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,    3,           0,   434, 0x80,   0x00,       ATK | 0x14,  0x00,       0x18,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   435, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   436, 0x80,   0x00,       ATK | 0x10,  0x00,       0x14,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   437, 0x80,   0x00,          ATK | 8,  0x00,       0x20,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   438, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_special_27 ),
};

/* Char_Special 28 */
const Sint32 pl06_special_28[] = {
	CG_FRAME(    0,    0,    3,           0,   447, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   448, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_28 ),
};

/* Char_Special 29 */
const Sint32 pl06_special_29[] = {
	CG_FRAME(    0,    0,    3,           0,   449, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   450, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl06_special_30[] = {
	CG_FRAME(    0,    0,    3,           0,   451, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   452, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_30 ),
};

/* Char_Special 31 */
const Sint32 pl06_special_31[] = {
	CG_FRAME(    0,    0,    3,           0,   453, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   454, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_31 ),
};

/* Char_Special 32 */
const Sint32 pl06_special_32[] = {
	CG_FRAME(    0,    0,    3,           0,   455, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   456, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_32 ),
};

/* Char_Special 33 */
const Sint32 pl06_special_33[] = {
	CG_FRAME(    0,    0,    3,           0,   457, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   458, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_33 ),
};

/* Char_Special 34 */
const Sint32 pl06_special_34[] = {
	CG_FRAME(    0,    0,    3,           0,   459, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   460, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_34 ),
};

/* Char_Special 35 */
const Sint32 pl06_special_35[] = {
	CG_FRAME(    0,    0,    3,           0,   461, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   462, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_35 ),
};

/* Char_Special 36 */
const Sint32 pl06_special_36[] = {
	CG_FRAME(    0,    0,    3,           0,   463, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   464, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_36 ),
};

/* Char_Special 37 */
const Sint32 pl06_special_37[] = {
	CG_FRAME(    0,    0,    3,           0,   465, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   466, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_37 ),
};

/* Char_Special 38 */
const Sint32 pl06_special_38[] = {
	CG_FRAME(    0,    0,    3,           0,   467, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   468, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_38 ),
};

/* Char_Special 39 */
const Sint32 pl06_special_39[] = {
	CG_FRAME(    0,    0,    3,           0,   437, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   438, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_39 ),
};

/* Char_Special 40 */
const Sint32 pl06_special_40[] = {
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   440, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_40 ),
};

/* Char_Special 41 */
const Sint32 pl06_special_41[] = {
	CG_FRAME(    0,    0,    3,           0,   441, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   442, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_41 ),
};

/* Char_Special 42 */
const Sint32 pl06_special_42[] = {
	CG_FRAME(    0,    0,    3,           0,   443, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   444, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_42 ),
};

/* Char_Special 43 */
const Sint32 pl06_special_43[] = {
	CG_FRAME(    0,    0,    3,           0,   445, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   446, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_43 ),
};

/* Char_Special 44 */
const Sint32 pl06_special_44[] = {
	CG_FRAME(    0,    0,    6,           0,   438, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    6,           0,   437, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    6,           0,   436, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   435, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_special_44 ),
};

/* Char_Special 45: hop_init (Cyclone Kick) */
const Sint32 pl06_special_45[] = {
	CG_FRAME(    0,    0,    6,           0,   480, 0x80,   0x00,       ATK | 0x13,  0x00,       0x17,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   481, 0x80,   0x00,       ATK | 0x19,  0x00, 0x15 | CSE,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,   482, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME(    0,    0,    3,           0,   483, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
};

const Sint32 pl06_special_45_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   484, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl06_special_45_loop ),
};

/* Char_Special 46: hop_move (Cyclone Kick) */
const Sint32 pl06_special_46[] = {
	CG_FRAME(    0,    0,    2,           0,   160, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   533, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   534, 0x80,   0x46,       ATK | 0x19,  0x00, 0x15 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   535, 0x80,   0xd0,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   535, 0x80,   0xd0,              ATK,  0x00,          0,    0,   0,     0,      0,  88 ),
	CG_FRAME(    0,    0,    6,           0,   536, 0x80,   0x61,              ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    8,           0,   537, 0x80,   0x61,              ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    3,           0,   538, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3,           0,   166, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   167, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_46 ),
};

/* Char_Super 0 */
const Sint32 pl06_super_00[] = {
	CG_FRAME(    0,    0,    2,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   530, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   531, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   14,           0,   532, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2,           0,   532, 0x80,   0x00,       ATK | 0x14,  0x00,       0x18,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,       ATK | 0x10,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,          ATK | 9,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl06_super_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl06_super_00_loop ),
};

/* Char_Super 8 */
const Sint32 pl06_super_08[] = {
	CG_FRAME(    0,    0,    4,           0,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   530, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   531, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   14,           0,   532, 0x80,   0x6a,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2,           0,   532, 0x80,   0x00,       ATK | 0x14,  0x00,       0x0c,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,       ATK | 0x10,  0x00,       0x14,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   400, 0x80,   0x00,          ATK | 9,  0x00,          0,    0,   0,     0,      0,   7 ),
};

const Sint32 pl06_super_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32769, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl06_super_08_loop ),
};

/* Char_Super 9 */
const Sint32 pl06_super_09[] = {
	CG_FRAME(    0,    0,    8,           0,   435, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    1,    3,           0,   434, 0x80,   0x00,       ATK | 0x14,  0x00,       0x18,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   435, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   436, 0x80,   0x00,       ATK | 0x10,  0x00,       0x14,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   437, 0x80,   0x00,          ATK | 8,  0x00,       0x20,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    3,           0,   438, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
};

const Sint32 pl06_super_09_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   439, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   440, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_super_09_loop ),
};

/* Char_Super 10 */
const Sint32 pl06_super_10[] = {
	CG_FRAME(    0,    0,    8,           0,   438, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    8,           0,   437, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    8,           0,   436, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   435, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl06_super_10 ),
};

/* Char_Super 1 */
const Sint32 pl06_super_01[] = {
	CG_FRAME(    0,    0,   12,           0,   399, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   397, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   396, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   395, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   394, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   393, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   392, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_super_01 ),
};

/* Char_Super 2: shl06_02_head_move, shl06_02_flash_move, shl06_02_char_set */
const Sint32 pl06_super_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32770, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32773, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32775, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32776, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32777, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32778, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32779, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32781, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32782, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32783, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32785, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32786, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32787, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32788, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4,           0, 32789, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32790, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32792, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32794, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32805, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4,           0, 32806, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl06_super_02 ),
};

/* Char_Special 47: snapback_init (The snapback) */
const Sint32 pl06_special_47[] = {
	CG_FRAME(    0,    0,    4,           0,   309, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    1,           0,   310, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   311, 0x80,   0x00,              ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   7 ),
	CG_FRAME( 0x80,    0,    1,           0,   312, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME( 0x80,    0,    1,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    1,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    7,           0,   313, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    1,           0,   314, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   316, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   317, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_special_47 ),
};

/* Char_Win 0 */
const Sint32 pl06_win_00[] = {
	CG_FRAME(    0,    0,    8,           0,   190, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   192, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   193, 0x80,   0x60,                7,  0x00,       0x1f,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_win_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   193, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   194, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_win_00_loop ),
};

/* Char_Win 1 */
const Sint32 pl06_win_01[] = {
	CG_FRAME(    0,    0,   32,           0,   213, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x0c,             0x18,  0x00,       0x10,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   199, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   200, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_win_01_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   200, 0x80,   0x0c,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_win_01_loop ),
};

/* Char_Win 2: lose_time_ko */
const Sint32 pl06_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   616, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   617, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   620, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   205, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   206, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_win_02_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_win_02_loop ),
};

/* Char_Win 3: draw_time */
const Sint32 pl06_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   264, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   266, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   267, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_win_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   268, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   269, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_win_03_loop ),
};

/* Char_Win 4: sp_chouhatsu (The taunt) */
const Sint32 pl06_win_04[] = {
	CG_FRAME(    0,    0,    4,           0,   843, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   844, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   845, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   846, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   848, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   849, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   850, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   853, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   854, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   855, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   856, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   857, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0x80,   0x00,             0x16,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   859, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   860, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   861, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   862, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   863, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   864, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_win_04_loop[] = {
	CG_FRAME(    0,    0,   20, END_OF_DATA,   865, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_win_04_loop ),
};

/* Char_Win 5: win_time */
const Sint32 pl06_win_05[] = {
	CG_FRAME(    0,    0,    8,           0,   190, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   192, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0,   193, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_win_05_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   193, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   194, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_win_05_loop ),
};

/* Char_Catch 0: neckbreaker_set */
const Sint32 pl06_catch_00[] = {
	CG_FRAME(    0,    4,    6,           0,   350, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    8,    2,           0,   581, 0x80,   0x00,                4,  0x00,       0x04,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,   12,    4,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,   16,    6,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0004,   1 ),
	CG_FRAME(    0,   20,   18,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0005,   1 ),
	CG_FRAME(    0,   24,    4,           0,   498, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0, 0x0014,   1 ),
	CG_FRAME(    0,    0,    4,           0,   499, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   500, 0x80,   0x8a,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_catch_00 ),
};

/* Char_Catch 1: crouch_k_set */
const Sint32 pl06_catch_01[] = {
	CG_FRAME(    0,    1,    4,           0,   366, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0006,   2 ),
	CG_FRAME(    0,    1,    8,           0,   367, 0x80,   0x00,                4,  0x00,       0x04,    0,   0,     0, 0x0007,   2 ),
	CG_FRAME(    0,    1,    2,           0,   368, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   2 ),
	CG_FRAME(    0,    2,    8,           0,   368, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0, 0x0008,   2 ),
	CG_FRAME(    0,    0,    8,           0,   622, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   162, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl06_catch_01 ),
};

/* Char_Catch 2: nml_p_set */
const Sint32 pl06_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   369, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0, 0x80,   12,           0,   370, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    0,    4,           0,   371, 0x80,   0x60,                4,  0x00,       0x04,    0,   0,     0, 0x000b,   1 ),
	CG_FRAME(    0,    1,   32, END_OF_DATA,   372, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0, 0x000f,   1 ),
	CG_NEXT( pl06_catch_02 ),
};

/* Char_Catch 3: beam_throw_set */
const Sint32 pl06_catch_03[] = {
	CG_FRAME(    0,    1,    4,           0,   369, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0, 0x0009,   1 ),
	CG_FRAME(    0,    1,    4,           0,   370, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0, 0x000a,   1 ),
	CG_FRAME(    0,    2,    1, END_OF_DATA,   370, 0x80,   0x60,                0,  0x00,          0,    0,   0,     0, 0x000c,   1 ),
	CG_NEXT( pl06_catch_03 ),
};

/* Char_Catch 4 */
const Sint32 pl06_catch_04[] = {
	CG_FRAME(    0,    0,    1,           0,   406, 0x80,   0x4f,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,   7 ),
};

const Sint32 pl06_catch_04_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   406, 0x80,   0x4f,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   407, 0x80,   0x4f,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_catch_04_loop ),
};

/* Char_Catch 5 */
const Sint32 pl06_catch_05[] = {
	CG_FRAME(    0,    0,    1,           0,   410, 0x80,   0x4f,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,   7 ),
};

const Sint32 pl06_catch_05_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   410, 0x80,   0x4f,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   411, 0x80,   0x4f,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_catch_05_loop ),
};

/* Char_Catch 6: jump_p_set */
const Sint32 pl06_catch_06[] = {
	CG_FRAME(    0,    1,    4,           0,   340, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x000d,   3 ),
	CG_FRAME(    0,    1,    6,           0,   341, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x000e,   3 ),
	CG_FRAME(    0,    2,    1,           0,   391, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0, 0x0013,   3 ),
	CG_FRAME(    0,    0,    1,           0,   192, 0x80,   0x00,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,   3 ),
};

const Sint32 pl06_catch_06_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   192, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_catch_06_loop ),
};

/* Char_Catch 7: jump_p_catch_fire */
const Sint32 pl06_catch_07[] = {
	CG_FRAME(    0,    0,    4,           0,   260, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   258, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   257, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   256, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   255, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   254, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   253, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   252, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   251, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl06_catch_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   262, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_catch_07_loop ),
};

/* Char_Catch 8: nml_k_set */
const Sint32 pl06_catch_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   166, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME( 0x80,    0,    2,           0,   502, 0x80,   0x4e,                7,  0x00,       0x1f,    0,   0,     0, 0x0010,   1 ),
};

const Sint32 pl06_catch_08_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   503, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0, 0x0010,   1 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   504, 0x80,   0x4e,                0,  0x00,          0,    0,   0,     0, 0x0011,   1 ),
	CG_NEXT( pl06_catch_08_loop ),
};

/* Char_Catch 9: nml_k_catch_fire */
const Sint32 pl06_catch_09[] = {
	CG_FRAME(    0,    1,    4,           0,   505, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0, 0x0012,   1 ),
	CG_FRAME(    0,    1,    3,           0,   506, 0x80,   0x87,                3,  0x00,       0x03,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   507, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   506, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    3,           0,   507, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    4,           0,   508, 0x80,   0x87,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   286, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   287, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_catch_09 ),
};

/* Char_Catch 10: grab_init (Running Neckbreaker) */
const Sint32 pl06_catch_10[] = {
	CG_FRAME(    0,    0,    2,           0,   134, 0x80,   0x76,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    1,           0,   134, 0x80,   0x76,                3,  0x00,       0x03,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    3,           0,   139, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    3,           0,   140, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    3,           0,   141, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    3,           0,   142, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    3,           0,   143, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    1,    3,           0,   144, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    3,           0,   145, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    3,           0,   146, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    4,           0,   147, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   148, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_NEXT( pl06_catch_10 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl06_change_00[] = {
	CG_FRAME(    0,    0,    2,           0,   169, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   168, 0x30,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   195, 0x30,   0x0d,       ATK | 0x0d,  0x00,       0x0b,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   10, END_OF_DATA,   195, 0x30,   0x0d,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl06_change_02[] = {
	CG_FRAME(    0,    0,    1,           0,   262, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   263, 0x80,   0x20,                0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl06_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl06_change_03[] = {
	CG_FRAME(    0,    0,    2,           0,   843, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   858, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   859, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   860, 0x80,   0x00,             0x17,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   861, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   862, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   863, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   864, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_change_03_loop[] = {
	CG_FRAME(    0,    0,   32, END_OF_DATA,   865, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl06_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   843, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   844, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   845, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   846, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   848, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   849, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   850, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   853, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   854, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   855, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   856, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   857, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   858, 0x80,   0x00,             0x16,  0x00,       0x0d,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   859, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   860, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   861, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   862, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   863, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   864, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_change_04_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   865, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_change_04_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl06_change_01[] = {
	CG_FRAME( 0x80,    0,    4,           0,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_change_01 ),
};

/* Char_Extra 7: t_kick_init */
const Sint32 pl06_extra_07[] = {
	CG_FRAME(    0,    0,    4,           0,   472, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME( 0x80,    0,    3,           0,   473, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME( 0x80,    0,    3,           0,   473, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME( 0x82,    0,    3,           0,   473, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
};

const Sint32 pl06_extra_07_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   473, 0xa0,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl06_extra_07_loop ),
};

/* Char_Extra 8: t_kick_move */
const Sint32 pl06_extra_08[] = {
	CG_FRAME(    0,    0,    2,           0,   162, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0, 0x1700,   1 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,       ATE | 0x0e,  0x00,       0x0f,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   192, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    2,           0,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   194, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   193, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   194, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   270, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   271, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   272, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_extra_08 ),
};

/* Char_Super 3 */
const Sint32 pl06_super_03[] = {
	CG_FRAME(    0,    0,    4,           0,   469, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   470, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   471, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   166, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   167, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_super_03 ),
};

/* Char_Super 4 */
const Sint32 pl06_super_04[] = {
	CG_FRAME(    0,    0,    4,           0,   509, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   510, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   511, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl06_super_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   512, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   513, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   514, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   515, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   516, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   517, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   518, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   519, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_super_04_loop ),
};

/* Char_Super 5: sweep_init (Optic Sweep) */
const Sint32 pl06_super_05[] = {
	CG_FRAME(    0,    0,    4,           0,   520, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    4,           0,   521, 0x80,   0x86,       ATK | 0x11,  0x00,       0x16,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    3,           0,   522, 0x80,   0x86,              ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    0,    6,           0,   523, 0x80,   0x86,          ATK | 7,  0x00,       0x1f,    0,   0,     0,      0,  96 ),
	CG_FRAME(    0,    1,   -1, END_OF_DATA,   523, 0x80,   0x86,              ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_NEXT( pl06_super_05 ),
};

/* Char_Super 6: sweep_stop (Optic Sweep) */
const Sint32 pl06_super_06[] = {
	CG_FRAME(    0,    1,   22,           0,   523, 0x80,   0x86,                0,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME(    0,    0,    4,           0,   164, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   165, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   166, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   167, 0x80,   0x00,                0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_super_06 ),
};

/* Char_Super 7 */
const Sint32 pl06_super_07[] = {
	CG_FRAME(    0,    0,    4,           0,   529, 0x80,   0x6e,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   524, 0x80,   0x45,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   525, 0x80,   0xc1,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   526, 0x80,   0x41,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   527, 0x80,   0x87,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   528, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_super_07 ),
};

/* Char_Extra 9: rush_init (Rapid Punches) */
const Sint32 pl06_extra_09[] = {
	CG_FRAME(    0,    0,    4,           0,   160, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   273, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   274, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    2,           0,   373, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   276, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   277, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    1,    3,           0,   374, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    1,    2,           0,   374, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   279, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_extra_09 ),
};

/* Char_Extra 10: rush_hit1 (Rapid Punches) */
const Sint32 pl06_extra_10[] = {
	CG_FRAME(    0,    0,    1,           0,   295, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   295, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    3,           0,   375, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   297, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   298, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x00,              ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   1 ),
	CG_FRAME(    1,    1,    4,           0,   376, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    1,    2,           0,   376, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   303, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   287, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_extra_10 ),
};

/* Char_Extra 11: rush_hit2 (Rapid Punches) */
const Sint32 pl06_extra_11[] = {
	CG_FRAME(    0,    0,    1,           0,   328, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x00,              ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    1,    2,           0,   329, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    1,    2,           0,   330, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   330, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   331, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_NEXT( pl06_extra_11 ),
};

/* Char_Extra 12: rush_hit3 (Rapid Punches) */
const Sint32 pl06_extra_12[] = {
	CG_FRAME(    0,    0,    1,           0,   289, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    3,           0,   290, 0x80,   0x00,          ATK | 6,  0x00,       0x1e,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    5,           0,   292, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    4,           0,   293, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,   10,           0,   293, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   286, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl06_extra_12 ),
};

/* Char_Extra 16: rush_hit1 (Rapid Punches), rush_hit2 (Rapid Punches), rush_hit3 (Rapid Punches) */
const Sint32 pl06_extra_16[] = {
	CG_FRAME(    0,    0,    7,           0,   147, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   148, 0x80,   0x00,              ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl06_extra_16 ),
};

/* Char_Extra 17: pl06_upper */
const Sint32 pl06_extra_17[] = {
	CG_FRAME(    0,    0,    5,           0,   280, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x4c,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0, 100 ),
	CG_FRAME( 0x80,    0,    2,           0,   281, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0, 100 ),
	CG_FRAME( 0x81,    0,    5,           0,   282, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME( 0x80,    0,    2,           0,   283, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,    2,           0,   285, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x4c,              ATK,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   286, 0x80,   0x4c,              ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl06_extra_17 ),
};

/* Char_Extra 18: pl06_upper */
const Sint32 pl06_extra_18[] = {
	CG_FRAME(    0,    0,    3,           0,   338, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    1,           0,   339, 0x80,   0x40,          ATK | 3,  0x00,       0x03,    0,   0,     0,      0,  32 ),
	CG_FRAME( 0x81,    0,    5,           0,   339, 0x80,   0x40,              ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    8,           0,   339, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4,           0,   340, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3,           0,   341, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   162, 0x80,   0x40,              ATE,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl06_extra_18 ),
};

/* Char_Effect 0 */
const Sint32 pl06_effect_00[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   797, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   798, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   799, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   800, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   801, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   802, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   803, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   804, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   805, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   806, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   807, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   808, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   809, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   810, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   811, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   812, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   813, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   814, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   815, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   816, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   817, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   818, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   819, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   820, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   821, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   822, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   823, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   824, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   825, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   826, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   827, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   828, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   829, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   830, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   831, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   832, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   833, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   834, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   835, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   836, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   837, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   838, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   839, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   840, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   841, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   842, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_effect_00 ),
};

/* group 25 0 */
const Sint32 pl06_grp25_00[] = {
	CG_FRAME(    0,    0,    2,           0,   740, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   741, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   742, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   743, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   744, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   745, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   746, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   747, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   748, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   749, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   750, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   752, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   753, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   754, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   755, 0x80,   0x00,              ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl06_grp25_00 ),
};

/* group 27 2: pls06_01_flash_init */
const Sint32 pl06_grp27_02[] = {
	EF_FRAME(    0,    0,    1,           0,   756 ),
	EF_FRAME(    0,    0,    1,           0,   757 ),
	EF_FRAME(    0,    0,    1,           0,   758 ),
	EF_FRAME(    0,    0,    1,           0,   759 ),
	EF_FRAME(    0,    0,    1,           0,   760 ),
	EF_FRAME(    0,    0,    1,           0,   761 ),
	EF_FRAME(    0,    0,    1,           0,   762 ),
	EF_FRAME(    0,    0,    1,           0,   763 ),
	EF_FRAME(    0,    0,    1,           0,   764 ),
	EF_FRAME(    0,    0,    1,           0,   765 ),
	EF_FRAME(    0,    0,    1,           0,   766 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   767 ),
	CG_NEXT( pl06_grp27_02 ),
};

/* group 27 3: pls06_01_appear_init */
const Sint32 pl06_grp27_03[] = {
	EF_FRAME(    0,    0,    1,           0,   214 ),
};

const Sint32 pl06_grp27_03_loop[] = {
	EF_FRAME(    0,    0,    3,           0,   215 ),
	EF_FRAME(    0,    0,    3,           0,   216 ),
	EF_FRAME(    0,    0,    3,           0,   217 ),
	EF_FRAME(    0,    0,    3,           0,   218 ),
	EF_FRAME(    0,    0,    3,           0,   219 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   220 ),
	CG_NEXT( pl06_grp27_03_loop ),
};

/* group 27 4: pls06_01_eye_init, pls06_01_t_kick_init */
const Sint32 pl06_grp27_04[] = {
	EF_FRAME(    0,    0,    2,           0,   740 ),
	EF_FRAME(    0,    0,    2,           0,   741 ),
	EF_FRAME(    0,    0,    2,           0,   742 ),
	EF_FRAME(    0,    0,    2,           0,   743 ),
	EF_FRAME(    0,    0,    2,           0,   744 ),
	EF_FRAME(    0,    0,    2,           0,   745 ),
	EF_FRAME(    0,    0,    2,           0,   746 ),
	EF_FRAME(    0,    0,    2,           0,   747 ),
	EF_FRAME(    0,    0,    2,           0,   748 ),
	EF_FRAME(    0,    0,    2,           0,   749 ),
	EF_FRAME(    0,    0,    3,           0,   750 ),
	EF_FRAME(    0,    0,    3,           0,   751 ),
	EF_FRAME(    0,    0,    3,           0,   752 ),
	EF_FRAME(    0,    0,    3,           0,   753 ),
	EF_FRAME(    0,    0,    3,           0,   754 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   755 ),
	CG_NEXT( pl06_grp27_04 ),
};

/* group 27 5: pls06_01_win_init, pls06_01_win_bound */
const Sint32 pl06_grp27_05[] = {
	EF_FRAME(    0,    0,    4,           0,   201 ),
	EF_FRAME(    0,    0,    4,           0,   202 ),
	EF_FRAME(    0,    0,    4,           0,   203 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   204 ),
	CG_NEXT( pl06_grp27_05 ),
};

/* group 27 6: pls06_01_ground_init */
const Sint32 pl06_grp27_06[] = {
	EF_FRAME(    0,    0,    2,           0, 32856 ),
	EF_FRAME(    0,    0,    2,           0, 32857 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32858 ),
	CG_NEXT( pl06_grp27_06 ),
};

/* group 27 7: pls06_01_cut_init */
const Sint32 pl06_grp27_07[] = {
	EF_FRAME(    0,    0,    1,           0, 32859 ),
	EF_FRAME(    0,    0,    1,           0, 32860 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA, 32861 ),
	CG_NEXT( pl06_grp27_07 ),
};

/* group 27 8: pls06_01_hit_init */
const Sint32 pl06_grp27_08[] = {
	EF_FRAME(    0,    0,    2,           0, 32862 ),
	EF_FRAME(    0,    0,    2,           0, 32863 ),
	EF_FRAME(    0,    0,    2,           0, 32864 ),
	EF_FRAME(    0,    0,    2,           0, 32862 ),
	EF_FRAME(    0,    0,    2,           0, 32863 ),
	EF_FRAME(    0,    0,    2, END_OF_DATA, 32864 ),
	CG_NEXT( pl06_grp27_08 ),
};

/* group 27 0 */
const Sint32 pl06_grp27_00[] = {
	EF_FRAME(    0,    0,    5,           0, 32832 ),
	EF_FRAME(    0,    0,    1,           0, 32833 ),
	EF_FRAME(    0,    0,    2,           0, 32832 ),
	EF_FRAME(    0,    0,    1,           0, 32834 ),
	EF_FRAME(    0,    0,    2,           0, 32832 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA, 32835 ),
	CG_NEXT( pl06_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl06_grp27_01[] = {
	EF_FRAME(    0,    0,    3,           0, 32836 ),
	EF_FRAME(    0,    0,    3,           0, 32837 ),
	EF_FRAME(    0,    0,    3,           0, 32838 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA, 32839 ),
	CG_NEXT( pl06_grp27_01 ),
};
