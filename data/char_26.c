/* M. Bison (character 26): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_26      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_26     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl26.c) says which group
                   and number each move uses.
   pl26_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl26_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl26_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl26_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_26
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_26, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_26 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl26_stand_00[], pl26_stand_02[], pl26_stand_03[], pl26_stand_04[],
	pl26_jump_00[], pl26_jump_00_loop[], pl26_jump_06[], pl26_jump_09[], pl26_jump_03[],
	pl26_dash_00[], pl26_dash_00_loop[], pl26_dash_02[], pl26_dash_01[], pl26_crouch_00[],
	pl26_crouch_01[], pl26_crouch_02[], pl26_turn_00[], pl26_turn_01[], pl26_guard_00[],
	pl26_guard_01[], pl26_guard_02[], pl26_guard_04[], pl26_guard_05[], pl26_guard_06[],
	pl26_stand_p_00[], pl26_stand_p_01[], pl26_stand_p_02[], pl26_stand_k_00[], pl26_stand_k_01[],
	pl26_stand_k_02[], pl26_air_guard_00[], pl26_air_guard_01[], pl26_crouch_p_00[],
	pl26_crouch_p_01[], pl26_crouch_p_02[], pl26_crouch_k_00[], pl26_crouch_k_01[],
	pl26_crouch_k_02[], pl26_jump_p_00[], pl26_jump_p_01[], pl26_jump_p_02[], pl26_jump_k_00[],
	pl26_jump_k_01[], pl26_jump_k_02[], pl26_get_up_00[], pl26_get_up_01[], pl26_get_up_02[],
	pl26_get_up_03[], pl26_appear_00[], pl26_appear_01[], pl26_appear_02[], pl26_win_00[],
	pl26_win_00_loop[], pl26_win_04[], pl26_win_01[], pl26_win_01_loop[], pl26_win_02[],
	pl26_win_02_loop[], pl26_special_00[], pl26_special_01[], pl26_special_01_loop[],
	pl26_special_02[], pl26_special_03[], pl26_special_03_loop[], pl26_special_04[],
	pl26_special_05[], pl26_special_05_loop[], pl26_special_06[], pl26_special_42[],
	pl26_special_42_loop[], pl26_special_43[], pl26_special_07[], pl26_special_07_loop[],
	pl26_special_08[], pl26_special_09[], pl26_special_10[], pl26_special_10_loop[],
	pl26_special_11[], pl26_special_11_loop[], pl26_special_12[], pl26_special_13[],
	pl26_special_13_loop[], pl26_special_14[], pl26_special_14_loop[], pl26_special_15[],
	pl26_special_16[], pl26_special_16_loop[], pl26_special_17[], pl26_special_17_loop[],
	pl26_special_18[], pl26_special_18_loop[], pl26_special_19[], pl26_special_19_loop[],
	pl26_special_20[], pl26_special_21[], pl26_special_40[], pl26_special_22[],
	pl26_special_22_loop[], pl26_special_26[], pl26_special_27[], pl26_special_27_loop[],
	pl26_special_31[], pl26_extra_00[], pl26_extra_00_loop[], pl26_extra_03[], pl26_extra_01[],
	pl26_extra_01_loop[], pl26_special_23[], pl26_special_24[], pl26_special_41[], pl26_special_25[],
	pl26_special_25_loop[], pl26_special_48[], pl26_special_36[], pl26_effect_01[], pl26_effect_02[],
	pl26_effect_02_loop[], pl26_effect_03[], pl26_effect_03_loop[], pl26_effect_04[],
	pl26_effect_05[], pl26_effect_17[], pl26_effect_17_loop[], pl26_effect_18[],
	pl26_effect_18_loop[], pl26_effect_19[], pl26_effect_06[], pl26_effect_07[], pl26_effect_08[],
	pl26_effect_09[], pl26_effect_10[], pl26_effect_11[], pl26_effect_12[], pl26_effect_13[],
	pl26_effect_14[], pl26_effect_15[], pl26_effect_16[], pl26_extra_04[], pl26_extra_05[],
	pl26_special_29[], pl26_special_29_loop[], pl26_change_00[], pl26_change_00_loop[],
	pl26_extra_02[], pl26_extra_02_loop[], pl26_change_04[], pl26_change_01[], pl26_change_02[],
	pl26_change_02_loop[], pl26_fly_00[], pl26_fly_01[], pl26_fly_08[], pl26_fly_04[], pl26_fly_16[],
	pl26_fly_12[], pl26_fly_09[], pl26_fly_02[], pl26_fly_03[], pl26_fly_07[], pl26_fly_06[],
	pl26_fly_05[], pl26_fly_10[], pl26_damage_00[], pl26_damage_01[], pl26_damage_02[],
	pl26_damage_03[], pl26_damage_04[], pl26_damage_05[], pl26_damage_06[], pl26_damage_07[],
	pl26_damage_08[], pl26_damage_10[], pl26_damage_11[], pl26_damage_12[], pl26_damage_13[],
	pl26_damage_09[], pl26_damage_09_loop[], pl26_damage_16[], pl26_damage_17[], pl26_damage_18[],
	pl26_damage_19[], pl26_damage_20[], pl26_damage_21[], pl26_damage_22[], pl26_damage_23[],
	pl26_damage_24[], pl26_damage_25[], pl26_damage_26[], pl26_damage_27[], pl26_damage_30[],
	pl26_damage_32[], pl26_damage_28[], pl26_damage_29[], pl26_damage_31[], pl26_dizzy_00[],
	pl26_catch_00[], pl26_catch_01[], pl26_special_47[], pl26_unused_00[], pl26_super_00[],
	pl26_super_00_loop[], pl26_super_01[], pl26_super_01_loop[], pl26_caught_00[], pl26_caught_01[],
	pl26_caught_02[], pl26_caught_03[], pl26_caught_04[], pl26_caught_05[], pl26_caught_06[],
	pl26_caught_07[], pl26_caught_08[], pl26_caught_09[], pl26_caught_10[], pl26_caught_11[],
	pl26_caught_12[], pl26_caught_13[], pl26_caught_14[], pl26_caught_15[], pl26_caught_16[],
	pl26_caught_17[], pl26_caught_18[], pl26_caught_19[], pl26_caught_20[], pl26_caught_21[],
	pl26_grp27_00[], pl26_grp27_01[], pl26_grp27_02[], pl26_grp27_03[], pl26_grp27_04[],
	pl26_grp27_05[], pl26_grp27_06[], pl26_grp27_07[], pl26_grp27_08[], pl26_grp27_09[],
	pl26_grp27_10[], pl26_grp27_11[], pl26_grp27_12[], pl26_grp27_13[], pl26_grp27_14[],
	pl26_grp27_15[];
extern const Sint32 *const pl26_stand_tbl[], *const pl26_jump_tbl[], *const pl26_dash_tbl[],
	*const pl26_crouch_tbl[], *const pl26_turn_tbl[], *const pl26_guard_tbl[],
	*const pl26_air_guard_tbl[], *const pl26_stand_p_tbl[], *const pl26_stand_k_tbl[],
	*const pl26_crouch_p_tbl[], *const pl26_crouch_k_tbl[], *const pl26_jump_p_tbl[],
	*const pl26_jump_k_tbl[], *const pl26_damage_tbl[], *const pl26_dizzy_tbl[],
	*const pl26_catch_tbl[], *const pl26_caught_tbl[], *const pl26_get_up_tbl[],
	*const pl26_appear_tbl[], *const pl26_win_tbl[], *const pl26_extra_tbl[],
	*const pl26_special_tbl[], *const pl26_super_tbl[], *const pl26_effect_tbl[],
	*const pl26_change_tbl[], *const pl26_grp25_tbl[], *const pl26_fly_tbl[],
	*const pl26_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_26[16] = {
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
const Sint32 *const *const char_tbl_26[] = {
	pl26_stand_tbl, pl26_jump_tbl, pl26_dash_tbl, pl26_crouch_tbl, pl26_turn_tbl, pl26_guard_tbl,
	pl26_air_guard_tbl, pl26_stand_p_tbl, pl26_stand_k_tbl, pl26_crouch_p_tbl, pl26_crouch_k_tbl, pl26_jump_p_tbl,
	pl26_jump_k_tbl, pl26_damage_tbl, pl26_dizzy_tbl, pl26_catch_tbl, pl26_caught_tbl, pl26_get_up_tbl,
	pl26_appear_tbl, pl26_win_tbl, pl26_extra_tbl, pl26_special_tbl, pl26_super_tbl, pl26_effect_tbl,
	pl26_change_tbl, pl26_grp25_tbl, pl26_fly_tbl, pl26_grp27_tbl, pl26_stand_tbl, pl26_stand_tbl,
	pl26_grp27_tbl, 0,
};

const Sint32 *const pl26_stand_tbl[] = {
	pl26_stand_00, pl26_stand_00, pl26_stand_02, pl26_stand_03, pl26_stand_04, pl26_stand_04,
	pl26_stand_02, pl26_stand_03, 0,
};

const Sint32 *const pl26_jump_tbl[] = {
	pl26_jump_00, pl26_jump_00, pl26_jump_00, pl26_jump_03, pl26_jump_03, pl26_jump_03,
	pl26_jump_06, pl26_jump_06, pl26_jump_06, pl26_jump_09, pl26_jump_09, pl26_jump_09,
	0,
};

const Sint32 *const pl26_dash_tbl[] = {
	pl26_dash_00, pl26_dash_01, pl26_dash_02, pl26_dash_02, 0,
};

const Sint32 *const pl26_crouch_tbl[] = {
	pl26_crouch_00, pl26_crouch_01, pl26_crouch_02, 0,
};

const Sint32 *const pl26_turn_tbl[] = {
	pl26_turn_00, pl26_turn_01, 0,
};

const Sint32 *const pl26_guard_tbl[] = {
	pl26_guard_00, pl26_guard_01, pl26_guard_02, pl26_guard_01, pl26_guard_04, pl26_guard_05,
	pl26_guard_06, pl26_guard_05, 0,
};

const Sint32 *const pl26_air_guard_tbl[] = {
	pl26_air_guard_00, pl26_air_guard_01, pl26_air_guard_01, 0,
};

const Sint32 *const pl26_stand_p_tbl[] = {
	pl26_stand_p_00, pl26_stand_p_01, pl26_stand_p_02, 0,
};

const Sint32 *const pl26_stand_k_tbl[] = {
	pl26_stand_k_00, pl26_stand_k_01, pl26_stand_k_02, 0,
};

const Sint32 *const pl26_crouch_p_tbl[] = {
	pl26_crouch_p_00, pl26_crouch_p_01, pl26_crouch_p_02, 0,
};

const Sint32 *const pl26_crouch_k_tbl[] = {
	pl26_crouch_k_00, pl26_crouch_k_01, pl26_crouch_k_02, 0,
};

const Sint32 *const pl26_jump_p_tbl[] = {
	pl26_jump_p_00, pl26_jump_p_01, pl26_jump_p_02, pl26_jump_p_00, pl26_jump_p_01, pl26_jump_p_02,
	0,
};

const Sint32 *const pl26_jump_k_tbl[] = {
	pl26_jump_k_00, pl26_jump_k_01, pl26_jump_k_02, pl26_jump_k_00, pl26_jump_k_01, pl26_jump_k_02,
	0,
};

const Sint32 *const pl26_damage_tbl[] = {
	pl26_damage_00, pl26_damage_01, pl26_damage_02, pl26_damage_03, pl26_damage_04, pl26_damage_05,
	pl26_damage_06, pl26_damage_07, pl26_damage_08, pl26_damage_09, pl26_damage_10, pl26_damage_11,
	pl26_damage_12, pl26_damage_13, pl26_damage_13, pl26_damage_13, pl26_damage_16, pl26_damage_17,
	pl26_damage_18, pl26_damage_19, pl26_damage_20, pl26_damage_21, pl26_damage_22, pl26_damage_23,
	pl26_damage_24, pl26_damage_25, pl26_damage_26, pl26_damage_27, pl26_damage_28, pl26_damage_29,
	pl26_damage_30, pl26_damage_31, pl26_damage_32, pl26_damage_32, pl26_damage_13, 0,
};

const Sint32 *const pl26_dizzy_tbl[] = {
	pl26_dizzy_00, 0,
};

const Sint32 *const pl26_catch_tbl[] = {
	pl26_catch_00, pl26_catch_01, 0,
};

const Sint32 *const pl26_caught_tbl[] = {
	pl26_caught_00, pl26_caught_01, pl26_caught_02, pl26_caught_03, pl26_caught_04, pl26_caught_05,
	pl26_caught_06, pl26_caught_07, pl26_caught_08, pl26_caught_09, pl26_caught_10, pl26_caught_11,
	pl26_caught_12, pl26_caught_13, pl26_caught_14, pl26_caught_15, pl26_caught_16, pl26_caught_17,
	pl26_caught_18, pl26_caught_19, pl26_caught_20, pl26_caught_21, 0,
};

const Sint32 *const pl26_get_up_tbl[] = {
	pl26_get_up_00, pl26_get_up_01, pl26_get_up_02, pl26_get_up_03, 0,
};

const Sint32 *const pl26_appear_tbl[] = {
	pl26_appear_00, pl26_appear_01, pl26_appear_02, 0,
};

const Sint32 *const pl26_win_tbl[] = {
	pl26_win_00, pl26_win_01, pl26_win_02, pl26_win_02, pl26_win_04, 0,
};

const Sint32 *const pl26_extra_tbl[] = {
	pl26_extra_00, pl26_extra_01, pl26_extra_02, pl26_extra_03, pl26_extra_04, pl26_extra_05,
	0,
};

const Sint32 *const pl26_special_tbl[] = {
	pl26_special_00, pl26_special_01, pl26_special_02, pl26_special_03, pl26_special_04, pl26_special_05,
	pl26_special_06, pl26_special_07, pl26_special_08, pl26_special_09, pl26_special_10, pl26_special_11,
	pl26_special_12, pl26_special_13, pl26_special_14, pl26_special_15, pl26_special_16, pl26_special_17,
	pl26_special_18, pl26_special_19, pl26_special_20, pl26_special_21, pl26_special_22, pl26_special_23,
	pl26_special_24, pl26_special_25, pl26_special_26, pl26_special_27, pl26_extra_04, pl26_special_29,
	pl26_special_27, pl26_special_31, pl26_special_26, pl26_special_26, pl26_special_26, pl26_special_26,
	pl26_special_36, pl26_special_36, pl26_special_36, pl26_special_36, pl26_special_40, pl26_special_41,
	pl26_special_42, pl26_special_43, pl26_special_17, pl26_special_40, pl26_special_41, pl26_special_47,
	pl26_special_48, 0,
};

const Sint32 *const pl26_super_tbl[] = {
	pl26_super_00, pl26_super_01, 0,
};

const Sint32 *const pl26_effect_tbl[] = {
	pl26_special_36, pl26_effect_01, pl26_effect_02, pl26_effect_03, pl26_effect_04, pl26_effect_05,
	pl26_effect_06, pl26_effect_07, pl26_effect_08, pl26_effect_09, pl26_effect_10, pl26_effect_11,
	pl26_effect_12, pl26_effect_13, pl26_effect_14, pl26_effect_15, pl26_effect_16, pl26_effect_17,
	pl26_effect_18, pl26_effect_19, 0,
};

const Sint32 *const pl26_change_tbl[] = {
	pl26_change_00, pl26_change_01, pl26_change_02, pl26_extra_02, pl26_change_04, 0,
};

const Sint32 *const pl26_grp25_tbl[] = {
	pl26_stand_00, 0,
};

const Sint32 *const pl26_fly_tbl[] = {
	pl26_fly_00, pl26_fly_01, pl26_fly_02, pl26_fly_03, pl26_fly_04, pl26_fly_05,
	pl26_fly_06, pl26_fly_07, pl26_fly_08, pl26_fly_09, pl26_fly_10, pl26_fly_10,
	pl26_fly_12, pl26_fly_10, pl26_fly_10, pl26_fly_10, pl26_fly_16, pl26_fly_10,
	0,
};

const Sint32 *const pl26_grp27_tbl[] = {
	pl26_grp27_00, pl26_grp27_01, pl26_grp27_02, pl26_grp27_03, pl26_grp27_04, pl26_grp27_05,
	pl26_grp27_06, pl26_grp27_07, pl26_grp27_08, pl26_grp27_09, pl26_grp27_10, pl26_grp27_11,
	pl26_grp27_12, pl26_grp27_13, pl26_grp27_14, pl26_grp27_15, pl26_grp27_15, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   group 25 0
   group 28 0
   group 28 1
   group 29 0 */
const Sint32 pl26_stand_00[] = {
	CG_FRAME(    0,    0,    6,           0,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    26, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    27, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    28, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    29, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    30, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    31, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    32, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    33, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    34, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    35, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    36, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6
   group 28 2
   group 28 6
   group 29 2
   group 29 6 */
const Sint32 pl26_stand_02[] = {
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    38, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7
   group 28 3
   group 28 7
   group 29 3
   group 29 7 */
const Sint32 pl26_stand_03[] = {
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    40, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_stand_03 ),
};

/* Char_Stand 4
   Char_Stand 5
   group 28 4
   group 28 5
   group 29 4
   group 29 5 */
const Sint32 pl26_stand_04[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    26, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_stand_00 ),
};

/* Char_Jump 0
   Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ...
   Char_Jump 2 */
const Sint32 pl26_jump_00[] = {
	CG_FRAME(    0,    1,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    5,           0,    48, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    8,           0,    49, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    5,           0,    50, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
};

const Sint32 pl26_jump_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    52, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl26_jump_00_loop ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl26_jump_06[] = {
	CG_FRAME(    0,    1,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,    48, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   10,           0,    49, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    7,           0,    50, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl26_jump_09[] = {
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    52, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_jump_09 ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl26_jump_03[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_jump_03 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl26_dash_00[] = {
	CG_FRAME(    0,    1,    3,           0,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_dash_00_loop[] = {
	CG_FRAME(    0,    0,    6,           0,    42, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    43, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    41, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_dash_00_loop ),
};

/* Char_Dash 2: dash_move
   Char_Dash 3: noki_move */
const Sint32 pl26_dash_02[] = {
	CG_FRAME(    0,    0,    6,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl26_dash_01[] = {
	CG_FRAME(    0,    0,    6,           0,    44, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,    45, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    46, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_dash_01 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl26_crouch_00[] = {
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl26_crouch_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl26_crouch_02[] = {
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl26_turn_00[] = {
	CG_FRAME(    0,    0,    4,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    58, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl26_turn_01[] = {
	CG_FRAME(    0,    0,    4,           0,    60, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    61, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    62, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl26_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl26_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    64, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl26_guard_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    63, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl26_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl26_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    66, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl26_guard_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    65, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_guard_06 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl26_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   110, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   111, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl26_stand_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   110, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   111, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  10 ),
	CG_FRAME(    0,    0,    5,           0,   112, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,  11 ),
	CG_FRAME(    0,    0,    5,           0,   117, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl26_stand_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    1,           0,   114, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,    2,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  12 ),
	CG_FRAME(    0,    0,    8,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  13 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl26_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   119, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   119, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl26_stand_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   118, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x06,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x61,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    3,           0,   121, 0x80,   0x61,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   120, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl26_stand_k_02[] = {
	CG_FRAME(    0,    0,    7,           0,   118, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   120, 0x80,   0x06,  ATK,  0x00, 0x15 | CSE,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    6,           0,   122, 0x80,   0x61,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  18 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x61,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   123, 0x80,   0x63,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_stand_k_02 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl26_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,    67, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl26_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    68, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_air_guard_01 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl26_crouch_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   124, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   125, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    2,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl26_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl26_crouch_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   124, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2,           0,   124, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   7 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  22 ),
	CG_FRAME(    0,    0,    4,           0,   125, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,  23 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,   7 ),
	CG_NEXT( pl26_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl26_crouch_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   126, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    1,           0,   113, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x0100,   7 ),
	CG_FRAME(    0,    0,    2,           0,   114, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   7 ),
	CG_FRAME( 0x80,    0,    2,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  24 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  24 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,  25 ),
	CG_FRAME(    0,    0,    6,           0,   117, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,  25 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   124, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl26_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl26_crouch_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   127, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    2,           0,   128, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   127, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl26_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl26_crouch_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   127, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   128, 0x80,   0x69,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    3,           0,   128, 0x80,   0x69,  ATE,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   127, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl26_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl26_crouch_k_02[] = {
	CG_FRAME(    0,    0,    8,           0,   129, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   20,           0,   130, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  30 ),
	CG_FRAME(    0,    0,   14,           0,   130, 0x80,   0x80,  ATE,  0x00,          0,    0,   0,     0, 0x0100,  31 ),
	CG_FRAME(    0,    0,    8,           0,   127, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    56, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0200,   7 ),
	CG_NEXT( pl26_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl26_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   131, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   132, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   10,           0,   133, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl26_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   131, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    2,           0,   132, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_FRAME(    0,    0,    2,           0,   133, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  34 ),
	CG_FRAME(    0,    0,    4,           0,   133, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  35 ),
	CG_FRAME(    0,    0,    4,           0,   132, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   131, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl26_jump_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   134, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    4,           0,   134, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_FRAME(    0,    0,    6,           0,   135, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  36 ),
	CG_FRAME(    0,    0,    3,           0,   135, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,  37 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,   131, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl26_jump_p_02 ),
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl26_jump_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   137, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    3,           0,   138, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   136, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl26_jump_k_01[] = {
	CG_FRAME(    0,    0,    6,           0,   136, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   139, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    6,           0,   140, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   136, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl26_jump_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   136, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   141, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8,           0,   142, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    8,           0,   141, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   136, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_jump_k_02 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl26_get_up_00[] = {
	CG_FRAME(    0,    0,    8,           0,    87, 0x80,   0x6d,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    88, 0x80,   0x60,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,    89, 0x80,   0x53,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl26_get_up_01[] = {
	CG_FRAME(    0,    0,    3,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl26_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl26_get_up_03[] = {
	CG_FRAME(    0,    0,    6,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    89, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    90, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_get_up_03 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl26_appear_00[] = {
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0x20,    0,  0x00,       0x0a,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,    1, END_OF_DATA,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl26_appear_00 ),
};

/* Char_Appear 1: appear_start */
const Sint32 pl26_appear_01[] = {
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    81, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_appear_01 ),
};

/* Char_Appear 2: appear_fall_low */
const Sint32 pl26_appear_02[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    25, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_appear_02 ),
};

/* Char_Win 0: win_ko_init */
const Sint32 pl26_win_00[] = {
	CG_FRAME(    0,    0,    4,           0,    69, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_win_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_win_00_loop ),
};

/* Char_Win 4: win_ko_move, win_ko_appear */
const Sint32 pl26_win_04[] = {
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    73, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_win_04 ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl26_win_01[] = {
	CG_FRAME(    0,    0,    3,           0,    57, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    9,           0,    74, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    75, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,    76, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    77, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    78, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_win_01_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,    79, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_win_01_loop ),
};

/* Char_Win 2
   Char_Win 3: lose_time_ko, draw_time */
const Sint32 pl26_win_02[] = {
	CG_FRAME(    0,    0,    1,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_win_02_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_win_02_loop ),
};

/* Char_Special 0: shot_init (Psycho Shot) */
const Sint32 pl26_special_00[] = {
	CG_FRAME(    0,    0,    4,           0,    63, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   17,           0,   155, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    1,    1,           0,   113, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_00 ),
};

/* Char_Special 1: knee_g_init (Double Knee Press) */
const Sint32 pl26_special_01[] = {
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    1,    1,           0,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
};

const Sint32 pl26_special_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl26_special_01_loop ),
};

/* Char_Special 2: knee_g_move (Double Knee Press) */
const Sint32 pl26_special_02[] = {
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,    56, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_02 ),
};

/* Char_Special 3: knee_j_init (Double Knee Press) */
const Sint32 pl26_special_03[] = {
	CG_FRAME(    0,    0,    3,           0,   185, 0x80,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   186, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  59 ),
	CG_FRAME(    0,    0,    3,           0,   187, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    3,           0,   188, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    8,           0,   189, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,    3,           0,   189, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl26_special_03_loop[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   189, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl26_special_03_loop ),
};

/* Char_Special 4: knee_j_move (Double Knee Press) */
const Sint32 pl26_special_04[] = {
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   14,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_04 ),
};

/* Char_Special 5: win_ko_init, warp_init (Bison Warp) */
const Sint32 pl26_special_05[] = {
	CG_FRAME(    0,    0,    5,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0, 32769, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32773, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
};

const Sint32 pl26_special_05_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32774, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl26_special_05_loop ),
};

/* Char_Special 6: win_ko_wait, warp_wait (Bison Warp) */
const Sint32 pl26_special_06[] = {
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32772, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,    1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    8,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_special_06 ),
};

/* Char_Special 42: pls26_00_warp_appear */
const Sint32 pl26_special_42[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32773, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
};

const Sint32 pl26_special_42_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32774, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl26_special_42_loop ),
};

/* Char_Special 43: pls26_00_warp_wait */
const Sint32 pl26_special_43[] = {
	CG_FRAME(    0,    0,    2,           0, 32774, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32773, 0x80,   0x20,    0,  0x00,       0x1e,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32772, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,    1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    8,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_special_43 ),
};

/* Char_Special 7: head_init (Head Press) */
const Sint32 pl26_special_07[] = {
	CG_FRAME(    0,    0,    3,           0,    47, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,    47, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl26_special_07_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    49, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl26_special_07_loop ),
};

/* Char_Special 8: head_rise (Head Press) */
const Sint32 pl26_special_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    49, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  50 ),
	CG_NEXT( pl26_special_08 ),
};

/* Char_Special 9: pl26_main */
const Sint32 pl26_special_09[] = {
	CG_FRAME(    0,    0,    1,           0,   157, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   157, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   16, END_OF_DATA,   157, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_09 ),
};

/* Char_Special 10: head_bound (Head Press) */
const Sint32 pl26_special_10[] = {
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl26_special_10_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   154, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_special_10_loop ),
};

/* Char_Special 11: head_dive_wait (Head Press) */
const Sint32 pl26_special_11[] = {
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x0100,  52 ),
};

const Sint32 pl26_special_11_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   149, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl26_special_11_loop ),
};

/* Char_Special 12: head_fall (Head Press), head_air (Head Press), head_dive (Head Press) */
const Sint32 pl26_special_12[] = {
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    56, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_12 ),
};

/* Char_Special 13: skull_init (Somersault Skull Diver) */
const Sint32 pl26_special_13[] = {
	CG_FRAME(    0,    0,    3,           0,    47, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,    47, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,    53, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl26_special_13_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    49, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_NEXT( pl26_special_13_loop ),
};

/* Char_Special 14: skull_jump (Somersault Skull Diver) */
const Sint32 pl26_special_14[] = {
	CG_FRAME(    0,    0,    4,           0,   147, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   151, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   152, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   153, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   154, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   149, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0, 0x0100,  52 ),
};

const Sint32 pl26_special_14_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   150, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   149, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl26_special_14_loop ),
};

/* Char_Special 15: skull_jump (Somersault Skull Diver), skull_dive (Somersault Skull Diver) */
const Sint32 pl26_special_15[] = {
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,    55, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,    56, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_15 ),
};

/* Char_Special 16: nightmare_init (Knee Press Nightmare (hyper)) */
const Sint32 pl26_special_16[] = {
	CG_FRAME(    0,    0,    1,           0, 32768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    1,           0, 32769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    1,           0, 32769, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
};

const Sint32 pl26_special_16_loop[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_NEXT( pl26_special_16_loop ),
};

/* Char_Special 17: nightmare_flash_wait (Knee Press Nightmare (hyper)), shl26_01_init
   Char_Special 44 */
const Sint32 pl26_special_17[] = {
	CG_FRAME(    0,    0,    1,           0, 32774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    1,           0, 32773, 0x80,   0x00,    0,  0x00,       0x1e,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    1,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    1,           0, 32770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    1,    1,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    1,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_special_17_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_special_17_loop ),
};

/* Char_Special 18: nightmare_kick_init (Knee Press Nightmare (hyper)), shl26_01_wait */
const Sint32 pl26_special_18[] = {
	CG_FRAME(    0,    0,    1,           0,   174, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    1,           0,   174, 0x80,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,    1,           0,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl26_special_18_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl26_special_18_loop ),
};

/* Char_Special 19: nightmare_kick1 (Knee Press Nightmare (hyper)), shl26_01_fly */
const Sint32 pl26_special_19[] = {
	CG_FRAME(    0,    0,    1,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    2,           0,   159, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,    1,           0,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl26_special_19_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl26_special_19_loop ),
};

/* Char_Special 20: nightmare_kick2 (Knee Press Nightmare (hyper)), shl26_01_fall */
const Sint32 pl26_special_20[] = {
	CG_FRAME(    0,    0,    1,           0,   129, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   32,           0,   130, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  69 ),
	CG_FRAME(    0,    0,   10,           0,   174, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   7 ),
	CG_FRAME(    0,    0,   12, END_OF_DATA,    54, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl26_special_20 ),
};

/* Char_Special 21: crusher_g_init (Psycho Crusher (hyper)) */
const Sint32 pl26_special_21[] = {
	CG_FRAME(    0, 0x80,    7,           0,    63, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0, 0x80,    9,           0,   341, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   12, END_OF_DATA,   341, 0x80,   0x00,  ATK,  0x00,       0x2e,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_special_21 ),
};

/* Char_Special 40: crusher_g_flash (Psycho Crusher (hyper))
   Char_Special 45 */
const Sint32 pl26_special_40[] = {
	CG_FRAME(    0,    0,    1,           0,   341, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    2,           0,   110, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   342, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0, 0x0300,   1 ),
	CG_NEXT( pl26_special_40 ),
};

/* Char_Special 22: crusher_g_start (Psycho Crusher (hyper)) */
const Sint32 pl26_special_22[] = {
	CG_FRAME(    0,    1,    3,           0,   170, 0x80,   0xae,  ATK,  0x00,       0x1d,    0,   0,     0,      0,  53 ),
};

const Sint32 pl26_special_22_loop[] = {
	CG_FRAME(    2,    1,    1,           0,   171, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    4,    1,    1,           0,   172, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    6,    1,    1,           0,   173, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   170, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  53 ),
	CG_NEXT( pl26_special_22_loop ),
};

/* Char_Special 26: field_init (Psycho Field)
   Char_Special 32: shl26_02_1_init
   Char_Special 33
   Char_Special 34
   Char_Special 35 */
const Sint32 pl26_special_26[] = {
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  61 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   194, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   197, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl26_special_26 ),
};

/* Char_Special 27
   Char_Special 30 */
const Sint32 pl26_special_27[] = {
	CG_FRAME(    0,    0,    1,           0,   174, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   174, 0x80,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    1,    3,           0,   159, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    1,    1,           0,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
};

const Sint32 pl26_special_27_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   161, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl26_special_27_loop ),
};

/* Char_Special 31 */
const Sint32 pl26_special_31[] = {
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_31 ),
};

/* Char_Extra 0 */
const Sint32 pl26_extra_00[] = {
	CG_FRAME(    0,    0,    4,           0,    86, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0xff,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0xff,    0,  0x00,       0x16,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl26_extra_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    81, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_extra_00_loop ),
};

/* Char_Extra 3: fly_start5 */
const Sint32 pl26_extra_03[] = {
	CG_FRAME(    0,    1,    1,           0,    80, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    82, 0x80,   0xff,    0,  0x00,       0x1e,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    83, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    84, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    85, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    86, 0x80,   0xff,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_extra_03 ),
};

/* Char_Extra 1: t_kick_move */
const Sint32 pl26_extra_01[] = {
	CG_FRAME(    0,    0,    8,           0,    56, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,    55, 0x80,   0x00,  ATE,  0x00,       0x15,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,    69, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    70, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    71, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    72, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    73, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl26_extra_01_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,    58, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_extra_01_loop ),
};

/* Char_Special 23: crusher_g_fly (Psycho Crusher (hyper)) */
const Sint32 pl26_special_23[] = {
	CG_FRAME(    0,    0,    4,           0,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   16,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   174, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    54, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_23 ),
};

/* Char_Special 24: crusher_j_init (Psycho Crusher (hyper)) */
const Sint32 pl26_special_24[] = {
	CG_FRAME(    0, 0x80,   11,           0,   347, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   347, 0x80,   0x00,  ATK,  0x00,       0x2e,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl26_special_24 ),
};

/* Char_Special 41: crusher_j_flash (Psycho Crusher (hyper))
   Char_Special 46 */
const Sint32 pl26_special_41[] = {
	CG_FRAME(    0,    0,    1,           0,   347, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_FRAME(    0,    0,    3,           0,   347, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   348, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0, 0x0200,   6 ),
	CG_NEXT( pl26_special_41 ),
};

/* Char_Special 25: crusher_j_start (Psycho Crusher (hyper)) */
const Sint32 pl26_special_25[] = {
	CG_FRAME(    0,    1,    3,           0,   164, 0x80,   0xae,  ATK,  0x00,       0x1d,    0,   0,     0,      0,  63 ),
};

const Sint32 pl26_special_25_loop[] = {
	CG_FRAME(    2,    1,    3,           0,   165, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  63 ),
	CG_FRAME(    4,    1,    3,           0,   166, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  63 ),
	CG_FRAME(    6,    1,    3,           0,   167, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  63 ),
	CG_FRAME(    0,    1,    3, END_OF_DATA,   164, 0x80,   0xae,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  63 ),
	CG_NEXT( pl26_special_25_loop ),
};

/* Char_Special 48: crusher_j_fly (Psycho Crusher (hyper)) */
const Sint32 pl26_special_48[] = {
	CG_FRAME(    0,    0,    3,           0,    49, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,    50, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl26_special_48 ),
};

/* Char_Special 36: shl26_02_2_init
   Char_Special 37
   Char_Special 38
   Char_Special 39
   Char_Effect 0: shl26_00_shot_init */
const Sint32 pl26_special_36[] = {
	CG_FRAME(    0,    0,    4,           0,   297, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  44 ),
	CG_FRAME(    0,    1,    4,           0,   298, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4,           0,   299, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   300, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl26_special_36 ),
};

/* Char_Effect 1: shl26_00_shot_move */
const Sint32 pl26_effect_01[] = {
	CG_FRAME(    0,    0,    6,           0,   301, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   302, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   303, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_01 ),
};

/* Char_Effect 2: shl26_00_field_head_init */
const Sint32 pl26_effect_02[] = {
	CG_FRAME(    0,    0,    6,           0,   320, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
};

const Sint32 pl26_effect_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   0 ),
	CG_FRAME(    0,    1,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_02_loop ),
};

/* Char_Effect 3: shl26_00_field_head_fly, shl26_00_field_spark_init, shl26_00_field_spark2_init */
const Sint32 pl26_effect_03[] = {
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl26_effect_03_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl26_effect_03_loop ),
};

/* Char_Effect 4: shl26_00_field_head_stop, shl26_00_field_spark_stop, shl26_01_move */
const Sint32 pl26_effect_04[] = {
	CG_FRAME(    0,    0,    6,           0, 32781, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32782, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32783, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA, 32784, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_04 ),
};

/* Char_Effect 5: shl26_02_crusher_init */
const Sint32 pl26_effect_05[] = {
	CG_FRAME(    0,    0,    6,           0,   333, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   334, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   335, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   336, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   337, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   338, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_05 ),
};

/* Char_Effect 17: shl26_00_explosion_init */
const Sint32 pl26_effect_17[] = {
	CG_FRAME(    0,    0,    6,           0,   320, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  73 ),
	CG_FRAME(    0,    1,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  73 ),
};

const Sint32 pl26_effect_17_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    4,           0,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  73 ),
	CG_FRAME(    0,    1,    3,           0,   322, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   321, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl26_effect_17_loop ),
};

/* Char_Effect 18: shl26_00_explosion_fly */
const Sint32 pl26_effect_18[] = {
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    1,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
};

const Sint32 pl26_effect_18_loop[] = {
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl26_effect_18_loop ),
};

/* Char_Effect 19: shl26_00_explosion_hit */
const Sint32 pl26_effect_19[] = {
	CG_FRAME(    0,    0,    6,           0, 32781, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32782, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0, 32783, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA, 32784, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_19 ),
};

/* Char_Effect 6 */
const Sint32 pl26_effect_06[] = {
	CG_FRAME(    0,    0,    6,           0,   266, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   267, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   268, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   269, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   270, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   271, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl26_effect_07[] = {
	CG_FRAME(    0,    0,    6,           0,   272, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   273, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   274, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl26_effect_08[] = {
	CG_FRAME(    0,    0,    6,           0,   275, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   276, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   277, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   278, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl26_effect_09[] = {
	CG_FRAME(    0,    0,    6,           0,   279, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   280, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   281, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl26_effect_10[] = {
	CG_FRAME(    0,    0,    6,           0,   282, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   283, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl26_effect_11[] = {
	CG_FRAME(    0,    0,    6,           0,   284, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   285, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   286, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl26_effect_12[] = {
	CG_FRAME(    0,    0,    6,           0,   287, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   288, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   289, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl26_effect_13[] = {
	CG_FRAME(    0,    0,    6,           0,   290, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   291, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   292, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   295, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   296, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl26_effect_14[] = {
	CG_FRAME(    0,    0,    6,           0,   306, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   307, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl26_effect_15[] = {
	CG_FRAME(    0,    0,    3,           0,   310, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   311, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   312, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   313, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_15 ),
};

/* Char_Effect 16 */
const Sint32 pl26_effect_16[] = {
	CG_FRAME(    0,    0,    6,           0,   314, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   315, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   316, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   317, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   318, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   319, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_effect_16 ),
};

/* Char_Extra 4: t_kick_init
   Char_Special 28 */
const Sint32 pl26_extra_04[] = {
	CG_FRAME(    0,    0,    2,           0,   136, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   141, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   48, END_OF_DATA,   142, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_NEXT( pl26_extra_04 ),
};

/* Char_Extra 5: pl26_upper */
const Sint32 pl26_extra_05[] = {
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    1,           0,   114, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME( 0x80,    0,    2,           0,   115, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,    2,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  71 ),
	CG_FRAME(    0,    0,    8,           0,   116, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  72 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_extra_05 ),
};

/* Char_Special 29: sp_chouhatsu (Taunt) */
const Sint32 pl26_special_29[] = {
	CG_FRAME(    0,    0,    1,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    80, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_special_29_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_special_29_loop ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl26_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,    69, 0x30,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   104, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   104, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl26_change_00_loop[] = {
	CG_FRAME(    0,    0,    9, END_OF_DATA,   104, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_change_00_loop ),
};

/* Char_Extra 2: fly_start5
   Char_Change 3: act_stand, act_change_call */
const Sint32 pl26_extra_02[] = {
	CG_FRAME(    0,    0,    2,           0,    57, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    76, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,    76, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    77, 0x30,   0x00,    0,  0x00,       0x14,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    78, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    79, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_extra_02_loop[] = {
	CG_FRAME(    0,    1,   48, END_OF_DATA,    79, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_extra_02_loop ),
};

/* Char_Change 4: act_change_assist_end */
const Sint32 pl26_change_04[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,    80, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,    81, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    80, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_change_04 ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl26_change_01[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   175, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl26_change_01 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl26_change_02[] = {
	CG_FRAME(    0,    0,    4,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    48, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    49, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4,           0,    50, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
};

const Sint32 pl26_change_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    51, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    52, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl26_change_02_loop ),
};

/* Char_Fly 0: fly_init (Flight) */
const Sint32 pl26_fly_00[] = {
	CG_FRAME(    0,    0,   10,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_00 ),
};

/* Char_Fly 1: act_fly */
const Sint32 pl26_fly_01[] = {
	CG_FRAME(    0,    0,   10,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_01 ),
};

/* Char_Fly 8 */
const Sint32 pl26_fly_08[] = {
	CG_FRAME(    0,    0,    6,           0,    41, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,    42, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    43, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_08 ),
};

/* Char_Fly 4 */
const Sint32 pl26_fly_04[] = {
	CG_FRAME(    0,    0,    6,           0,    44, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6,           0,    45, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    46, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_04 ),
};

/* Char_Fly 16 */
const Sint32 pl26_fly_16[] = {
	CG_FRAME(    0,    0,    2,           0,    37, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    38, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_01 ),
};

/* Char_Fly 12 */
const Sint32 pl26_fly_12[] = {
	CG_FRAME(    0,    0,    2,           0,    39, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    40, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_01 ),
};

/* Char_Fly 9 */
const Sint32 pl26_fly_09[] = {
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    38, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_09 ),
};

/* Char_Fly 2 */
const Sint32 pl26_fly_02[] = {
	CG_FRAME(    0,    0,    8,           0,    47, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    53, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_02 ),
};

/* Char_Fly 3 */
const Sint32 pl26_fly_03[] = {
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    40, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_03 ),
};

/* Char_Fly 7 */
const Sint32 pl26_fly_07[] = {
	CG_FRAME(    0,    0,    6,           0,    37, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    38, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_07 ),
};

/* Char_Fly 6 */
const Sint32 pl26_fly_06[] = {
	CG_FRAME(    0,    0,   10,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_06 ),
};

/* Char_Fly 5 */
const Sint32 pl26_fly_05[] = {
	CG_FRAME(    0,    0,    6,           0,    39, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    40, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_05 ),
};

/* Char_Fly 10
   Char_Fly 11
   Char_Fly 13
   Char_Fly 14
   Char_Fly 15
   Char_Fly 17 */
const Sint32 pl26_fly_10[] = {
	CG_FRAME(    0,    0,    8,           0,    70, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,    71, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl26_fly_01 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl26_damage_00[] = {
	CG_FRAME(    0,    0,    2,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl26_damage_01[] = {
	CG_FRAME(    0,    0,    4,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl26_damage_02[] = {
	CG_FRAME( 0x80,    0,    4,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   210, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl26_damage_03[] = {
	CG_FRAME(    0,    0,   11, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl26_damage_04[] = {
	CG_FRAME( 0x80,    0,    2,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x80,    0,    8,           0,   214, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl26_damage_05[] = {
	CG_FRAME(    0,    0,    2,           0,   214, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   215, 0x80,   0x1a,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   214, 0x80,   0x17,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x82,    0,    4,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl26_damage_06[] = {
	CG_FRAME(    0,    0,    2,           0,   217, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    5,           0,   218, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   217, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl26_damage_07[] = {
	CG_FRAME( 0x80,    0,    4,           0,   217, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    8,           0,   218, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   217, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl26_damage_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   217, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    4,           0,   218, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x82,    0,    8,           0,   219, 0x80,   0x75,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   218, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   217, 0x80,   0x74,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl26_damage_10[] = {
	CG_FRAME(    0,    0,    3,           0,   220, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   221, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   220, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl26_damage_11[] = {
	CG_FRAME(    0,    0,    2,           0,   220, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   220, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl26_damage_12[] = {
	CG_FRAME(    0,    0,    1,           0,   220, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1,           0,   221, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   223, 0x80,   0x2c,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   221, 0x80,   0x28,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   220, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl26_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   234, 0x80,   0x80,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   235, 0x80,   0x80,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl26_damage_13 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl26_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   237, 0x80,   0x3a,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   245, 0x80,   0x80,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl26_damage_09_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   246, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl26_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl26_damage_16[] = {
	CG_FRAME(    0,    0,    4,           0,   238, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   239, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl26_damage_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl26_damage_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   241, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl26_damage_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   242, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl26_damage_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   243, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl26_damage_21[] = {
	CG_FRAME(    0,    0,    4,           0,   238, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 22 */
const Sint32 pl26_damage_22[] = {
	CG_FRAME(    0,    0,    8,           0,   240, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 23 */
const Sint32 pl26_damage_23[] = {
	CG_FRAME(    0,    0,    8,           0,   241, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 24 */
const Sint32 pl26_damage_24[] = {
	CG_FRAME(    0,    0,    8,           0,   242, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Damage 25 */
const Sint32 pl26_damage_25[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   243, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl26_damage_26[] = {
	CG_FRAME(    0,    0,    4,           0,   245, 0x80,   0x80,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4,           0,   247, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   246, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl26_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl26_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,    91, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    92, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    93, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    94, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    56, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,    55, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl26_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl26_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,   148, 0x80,   0x29,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   248, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    49, 0x80,   0x2d,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_30 ),
};

/* Char_Damage 32: act_escape
   Char_Damage 33: act_escape */
const Sint32 pl26_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,   265, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   248, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_32 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl26_damage_28[] = {
	CG_FRAME(    0,    0,    6,           0,   249, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   250, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   251, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   252, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   253, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   254, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl26_damage_29[] = {
	CG_FRAME(    0,    0,    6,           0,   255, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   256, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   257, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   258, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   259, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   260, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl26_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl26_damage_31[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   247, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl26_damage_31 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl26_dizzy_00[] = {
	CG_FRAME(    0,    0,    8,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   227, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_dizzy_00 ),
};

/* Char_Catch 0: nml_set */
const Sint32 pl26_catch_00[] = {
	CG_FRAME(    0,    0,    2,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   1 ),
	CG_FRAME(    0,    0,    8,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   1 ),
	CG_FRAME(    0,    0,    6,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   1 ),
	CG_FRAME(    0,    1,    4,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   32,           0,   146, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,    54, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl26_catch_00 ),
};

/* Char_Catch 1: jump_set */
const Sint32 pl26_catch_01[] = {
	CG_FRAME(    0,    0,    3,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   2 ),
	CG_FRAME(    0,    2,    3,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0105,   2 ),
	CG_FRAME(    0,    0,    6,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0106,   2 ),
	CG_FRAME(    0,    0,    2,           0,   135, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0, 0x0107,   2 ),
	CG_FRAME(    0,    1,   12, END_OF_DATA,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   2 ),
	CG_NEXT( pl26_catch_01 ),
};

/* Char_Special 47: snapback_init (Snapback) */
const Sint32 pl26_special_47[] = {
	CG_FRAME(    0,    0,    7,           0,   118, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    2,           0,   118, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,    3,           0,   118, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   120, 0x80,   0x06,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    6,           0,   122, 0x80,   0x61,  ATK,  0x00,          0,    0,   0,     0, 0x0100,  18 ),
	CG_FRAME(    0,    0,    4,           0,   122, 0x80,   0x61,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    5,           0,   123, 0x80,   0x63,  ATE,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    4,           0,   120, 0x80,   0x06,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   118, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_special_47 ),
};

const Sint32 pl26_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   118, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    4,           0,   118, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    2,    4,           0,   120, 0x80,   0x00,  ATK,  0x00, 0x14 | CSE,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   121, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    3,           0,   121, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    5,           0,   120, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	0x00004164,
};

/* Char_Super 0: explosion_init (Psycho Explosion (hyper)) */
const Sint32 pl26_super_00[] = {
	CG_FRAME(    0,    0,    7,           0,    63, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    2,           0,   155, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   16,           0,   155, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   5 ),
	CG_FRAME(    0,    1,    2,           0,   155, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    1,    1,           0,   113, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   113, 0x80,   0x00,  ATK,  0x00,       0x1a,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   114, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   115, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    1,    4,           0,   116, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    4,           0,   117, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl26_super_00_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl26_super_00_loop ),
};

/* Char_Super 1: explosion_wait (Psycho Explosion (hyper)) */
const Sint32 pl26_super_01[] = {
	CG_FRAME(    0,    0,    3,           0,   117, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,    3,           0,    59, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,    69, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    1,           0,    80, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl26_super_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,    80, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,    81, 0x80,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl26_super_01_loop ),
};

/* Char_Caught 0 */
const Sint32 pl26_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl26_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl26_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl26_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl26_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl26_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl26_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl26_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl26_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl26_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl26_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl26_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl26_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl26_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   219, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl26_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl26_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl26_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl26_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl26_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl26_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl26_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl26_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl26_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   241, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl26_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl26_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   243, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl26_caught_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl26_caught_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl26_caught_20[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   247, 0x80,   0xa0,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl26_caught_21[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl26_caught_21 ),
};

/* group 27 0: pls26_00_init_00
   group 30 0 */
const Sint32 pl26_grp27_00[] = {
	EF_FRAME(    0,    0,    1,           0,   272 ),
	EF_FRAME(    0,    0,    1,           0,   272 ),
	EF_FRAME(    0,    0,    1,           0,   273 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   274 ),
	CG_NEXT( pl26_grp27_00 ),
};

/* group 27 1: pls26_00_init_01
   group 30 1 */
const Sint32 pl26_grp27_01[] = {
	EF_FRAME(    0,    0,    1,           0,   275 ),
	EF_FRAME(    0,    0,    1,           0,   275 ),
	EF_FRAME(    0,    0,    1,           0,   276 ),
	EF_FRAME(    0,    0,    1,           0,   277 ),
	EF_FRAME(    0,    0,    1,           0,   278 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   278 ),
	CG_NEXT( pl26_grp27_01 ),
};

/* group 27 2: pls26_00_init_02
   group 30 2 */
const Sint32 pl26_grp27_02[] = {
	EF_FRAME(    0,    0,    1,           0,   279 ),
	EF_FRAME(    0,    0,    1,           0,   279 ),
	EF_FRAME(    0,    0,    1,           0,   280 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   281 ),
	CG_NEXT( pl26_grp27_02 ),
};

/* group 27 3: pls26_00_init_03
   group 30 3 */
const Sint32 pl26_grp27_03[] = {
	EF_FRAME(    0,    0,    1,           0,   275 ),
	EF_FRAME(    0,    0,    1,           0,   276 ),
	EF_FRAME(    0,    0,    1,           0,   277 ),
	EF_FRAME(    0,    0,    1,           0,   278 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   278 ),
	CG_NEXT( pl26_grp27_03 ),
};

/* group 27 4: pls26_00_init_04
   group 30 4 */
const Sint32 pl26_grp27_04[] = {
	EF_FRAME(    0,    0,    1,           0,   284 ),
	EF_FRAME(    0,    0,    1,           0,   285 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   286 ),
	CG_NEXT( pl26_grp27_04 ),
};

/* group 27 5: pls26_00_init_05
   group 30 5 */
const Sint32 pl26_grp27_05[] = {
	EF_FRAME(    0,    0,    1,           0,   287 ),
	EF_FRAME(    0,    0,    1,           0,   287 ),
	EF_FRAME(    0,    0,    1,           0,   288 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   289 ),
	CG_NEXT( pl26_grp27_05 ),
};

/* group 27 6: pls26_00_shot_init
   group 30 6 */
const Sint32 pl26_grp27_06[] = {
	EF_FRAME(    0,    0,    1,           0,   290 ),
	EF_FRAME(    0,    0,    1,           0,   291 ),
	EF_FRAME(    0,    0,    1,           0,   292 ),
	EF_FRAME(    0,    0,    1,           0,   293 ),
	EF_FRAME(    0,    0,    1,           0,   290 ),
	EF_FRAME(    0,    0,    1,           0,   291 ),
	EF_FRAME(    0,    0,    1,           0,   292 ),
	EF_FRAME(    0,    0,    1,           0,   293 ),
	EF_FRAME(    0,    0,    3,           0,   294 ),
	EF_FRAME(    0,    0,    3,           0,   295 ),
	EF_FRAME(    0,    0,    3, END_OF_DATA,   296 ),
	CG_NEXT( pl26_grp27_06 ),
};

/* group 27 7: pls26_00_dive_init
   group 30 7 */
const Sint32 pl26_grp27_07[] = {
	EF_FRAME(    0,    0,    4,           0,   306 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,   307 ),
	CG_NEXT( pl26_grp27_07 ),
};

/* group 27 8: pls26_00_crusher_g_flash_init
   group 30 8 */
const Sint32 pl26_grp27_08[] = {
	EF_FRAME(    0,    0,    1, END_OF_DATA,   308 ),
	CG_NEXT( pl26_grp27_08 ),
};

/* group 27 9
   group 30 9 */
const Sint32 pl26_grp27_09[] = {
	EF_FRAME(    0,    0,    1, END_OF_DATA,   272 ),
	CG_NEXT( pl26_grp27_09 ),
};

/* group 27 10
   group 30 10 */
const Sint32 pl26_grp27_10[] = {
	EF_FRAME(    0,    0,    1, END_OF_DATA,   309 ),
	CG_NEXT( pl26_grp27_10 ),
};

/* group 27 11: pls26_00_crusher_g_init
   group 30 11 */
const Sint32 pl26_grp27_11[] = {
	EF_FRAME(    0,    0,    1,           0,   310 ),
	EF_FRAME(    0,    0,    1,           0,   311 ),
	EF_FRAME(    0,    0,    1,           0,   312 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   313 ),
	CG_NEXT( pl26_grp27_11 ),
};

/* group 27 12: pls26_00_crusher_j_flash_init
   group 30 12 */
const Sint32 pl26_grp27_12[] = {
	EF_FRAME(    0,    0,    1, END_OF_DATA,   314 ),
	CG_NEXT( pl26_grp27_12 ),
};

/* group 27 13
   group 30 13 */
const Sint32 pl26_grp27_13[] = {
	EF_FRAME(    0,    0,    1, END_OF_DATA,   315 ),
	CG_NEXT( pl26_grp27_13 ),
};

/* group 27 14: pls26_00_crusher_j_init
   group 30 14 */
const Sint32 pl26_grp27_14[] = {
	EF_FRAME(    0,    0,    1,           0,   316 ),
	EF_FRAME(    0,    0,    1,           0,   317 ),
	EF_FRAME(    0,    0,    1,           0,   318 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,   319 ),
	CG_NEXT( pl26_grp27_14 ),
};

/* group 27 15
   group 27 16: pls26_00_appear_init
   group 30 15
   group 30 16 */
const Sint32 pl26_grp27_15[] = {
	EF_FRAME(    0,    0,    6,           0,   266 ),
	EF_FRAME(    0,    0,    6,           0,   267 ),
	EF_FRAME(    0,    0,    6,           0,   268 ),
	EF_FRAME(    0,    0,    6,           0,   269 ),
	EF_FRAME(    0,    0,    6,           0,   270 ),
	EF_FRAME(    0,    0,    6, END_OF_DATA,   271 ),
	CG_NEXT( pl26_grp27_15 ),
};
