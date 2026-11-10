/* Iceman (character 09): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_09      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_09     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl09.c) says which group
                   and number each move uses.
   pl09_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl09_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl09_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl09_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_09
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_09, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_09 */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl09_change_00[], pl09_change_01[], pl09_change_02[], pl09_change_03[],
	pl09_change_03_loop[], pl09_change_04[], pl09_stand_06[], pl09_stand_07[], pl09_stand_00[],
	pl09_stand_p_00[], pl09_stand_p_01[], pl09_stand_p_02[], pl09_extra_08[], pl09_effect_00[],
	pl09_crouch_p_00[], pl09_crouch_p_01[], pl09_crouch_p_02[], pl09_effect_06[], pl09_effect_07[],
	pl09_stand_k_00[], pl09_stand_k_01[], pl09_stand_k_02[], pl09_effect_01[], pl09_crouch_k_00[],
	pl09_crouch_k_01[], pl09_crouch_k_02[], pl09_extra_09[], pl09_appear_00[], pl09_crouch_01[],
	pl09_crouch_00[], pl09_unused_00[], pl09_crouch_02[], pl09_turn_00[], pl09_turn_01[],
	pl09_guard_00[], pl09_guard_01[], pl09_guard_02[], pl09_guard_04[], pl09_guard_05[],
	pl09_guard_06[], pl09_stand_02[], pl09_stand_03[], pl09_jump_00[], pl09_jump_01[],
	pl09_jump_02[], pl09_jump_03[], pl09_jump_04[], pl09_jump_06[], pl09_jump_09[], pl09_jump_07[],
	pl09_jump_10[], pl09_jump_08[], pl09_jump_11[], pl09_air_guard_00[], pl09_air_guard_01[],
	pl09_air_guard_03[], pl09_air_guard_04[], pl09_air_guard_05[], pl09_dash_01[],
	pl09_dash_01_loop[], pl09_dash_00[], pl09_dash_00_loop[], pl09_dash_02[], pl09_jump_p_00[],
	pl09_effect_02[], pl09_jump_p_01[], pl09_jump_p_05[], pl09_effect_03[], pl09_jump_p_02[],
	pl09_unused_01[], pl09_jump_k_00[], pl09_jump_k_01[], pl09_jump_k_05[], pl09_jump_k_02[],
	pl09_win_00[], pl09_win_00_loop[], pl09_win_05[], pl09_win_05_loop[], pl09_win_07[],
	pl09_win_08[], pl09_win_01[], pl09_win_01_loop[], pl09_win_06[], pl09_win_06_loop[],
	pl09_win_02[], pl09_win_02_loop[], pl09_win_04[], pl09_win_04_loop[], pl09_win_03[],
	pl09_win_03_loop[], pl09_extra_07[], pl09_damage_00[], pl09_damage_01[], pl09_damage_02[],
	pl09_damage_03[], pl09_damage_04[], pl09_damage_05[], pl09_damage_06[], pl09_damage_07[],
	pl09_damage_08[], pl09_damage_09[], pl09_damage_09_loop[], pl09_damage_10[], pl09_damage_11[],
	pl09_damage_12[], pl09_damage_31[], pl09_get_up_00[], pl09_get_up_01[], pl09_get_up_02[],
	pl09_get_up_03[], pl09_damage_13[], pl09_damage_14[], pl09_damage_15[], pl09_damage_16[],
	pl09_damage_17[], pl09_damage_18[], pl09_damage_19[], pl09_damage_20[], pl09_damage_21[],
	pl09_damage_22[], pl09_damage_23[], pl09_damage_24[], pl09_damage_25[], pl09_damage_26[],
	pl09_damage_27[], pl09_unused_02[], pl09_damage_32[], pl09_damage_33[], pl09_damage_28[],
	pl09_damage_29[], pl09_damage_30[], pl09_dizzy_00[], pl09_catch_00[], pl09_catch_00_loop[],
	pl09_catch_01[], pl09_effect_08[], pl09_unused_03[], pl09_catch_02[], pl09_special_05[],
	pl09_unused_04[], pl09_unused_05[], pl09_unused_06[], pl09_extra_05[], pl09_extra_05_loop[],
	pl09_extra_06[], pl09_extra_06_loop[], pl09_special_01[], pl09_special_01_loop[],
	pl09_special_02[], pl09_special_03[], pl09_special_04[], pl09_super_04[], pl09_super_05[],
	pl09_special_14[], pl09_special_15[], pl09_special_16[], pl09_special_17[], pl09_special_18[],
	pl09_special_19[], pl09_special_20[], pl09_special_21[], pl09_special_22[], pl09_special_23[],
	pl09_special_24[], pl09_special_25[], pl09_special_26[], pl09_special_27[], pl09_special_28[],
	pl09_special_29[], pl09_special_30[], pl09_special_31[], pl09_extra_02[], pl09_special_06[],
	pl09_special_06_loop[], pl09_special_10[], pl09_special_07[], pl09_special_07_loop[],
	pl09_special_11[], pl09_unused_07[], pl09_unused_08[], pl09_special_09[], pl09_special_09_loop[],
	pl09_special_13[], pl09_unused_09[], pl09_unused_10[], pl09_special_08[], pl09_special_08_loop[],
	pl09_special_12[], pl09_special_00[], pl09_unused_11[], pl09_super_00[], pl09_super_00_loop[],
	pl09_super_02[], pl09_super_02_loop[], pl09_super_06[], pl09_super_01[], pl09_super_01_loop[],
	pl09_super_03[], pl09_super_03_loop[], pl09_super_07[], pl09_extra_03[], pl09_extra_04[],
	pl09_effect_05[], pl09_extra_00[], pl09_extra_01[], pl09_effect_04[], pl09_caught_00[],
	pl09_caught_01[], pl09_caught_02[], pl09_caught_03[], pl09_caught_04[], pl09_caught_05[],
	pl09_caught_06[], pl09_caught_07[], pl09_caught_08[], pl09_caught_09[], pl09_caught_10[],
	pl09_caught_11[], pl09_caught_12[], pl09_caught_13[], pl09_caught_14[], pl09_caught_15[],
	pl09_caught_16[], pl09_caught_17[], pl09_caught_18[], pl09_caught_19[], pl09_caught_20[],
	pl09_caught_21[], pl09_unused_12[], pl09_unused_13[], pl09_unused_14[], pl09_unused_15[],
	pl09_unused_16[], pl09_unused_17[], pl09_unused_18[], pl09_unused_19[], pl09_unused_20[],
	pl09_unused_21[], pl09_unused_22[], pl09_unused_23[], pl09_unused_24[], pl09_unused_25[],
	pl09_unused_26[], pl09_unused_27[], pl09_unused_28[], pl09_unused_29[], pl09_unused_30[],
	pl09_grp27_00[], pl09_grp27_00_loop[], pl09_grp27_01[], pl09_grp27_01_loop[], pl09_grp27_02[],
	pl09_grp27_02_loop[], pl09_grp27_03[], pl09_grp27_03_loop[], pl09_grp27_04[],
	pl09_grp27_04_loop[];
extern const Sint32 *const pl09_stand_tbl[], *const pl09_jump_tbl[], *const pl09_dash_tbl[],
	*const pl09_crouch_tbl[], *const pl09_turn_tbl[], *const pl09_guard_tbl[],
	*const pl09_air_guard_tbl[], *const pl09_stand_p_tbl[], *const pl09_stand_k_tbl[],
	*const pl09_crouch_p_tbl[], *const pl09_crouch_k_tbl[], *const pl09_jump_p_tbl[],
	*const pl09_jump_k_tbl[], *const pl09_damage_tbl[], *const pl09_caught_tbl[],
	*const pl09_dizzy_tbl[], *const pl09_catch_tbl[], *const pl09_get_up_tbl[],
	*const pl09_appear_tbl[], *const pl09_win_tbl[], *const pl09_extra_tbl[],
	*const pl09_special_tbl[], *const pl09_super_tbl[], *const pl09_effect_tbl[],
	*const pl09_change_tbl[], *const pl09_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_09[16] = {
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
const Sint32 *const *const char_tbl_09[] = {
	pl09_stand_tbl, pl09_jump_tbl, pl09_dash_tbl, pl09_crouch_tbl, pl09_turn_tbl, pl09_guard_tbl,
	pl09_air_guard_tbl, pl09_stand_p_tbl, pl09_stand_k_tbl, pl09_crouch_p_tbl, pl09_crouch_k_tbl, pl09_jump_p_tbl,
	pl09_jump_k_tbl, pl09_damage_tbl, pl09_dizzy_tbl, pl09_catch_tbl, pl09_caught_tbl, pl09_get_up_tbl,
	pl09_appear_tbl, pl09_win_tbl, pl09_extra_tbl, pl09_special_tbl, pl09_super_tbl, pl09_effect_tbl,
	pl09_change_tbl, pl09_change_tbl, pl09_change_tbl, pl09_grp27_tbl, 0,
};

const Sint32 *const pl09_stand_tbl[] = {
	pl09_stand_00, pl09_stand_00, pl09_stand_02, pl09_stand_03, pl09_stand_00, pl09_stand_00,
	pl09_stand_06, pl09_stand_07, 0,
};

const Sint32 *const pl09_jump_tbl[] = {
	pl09_jump_00, pl09_jump_01, pl09_jump_02, pl09_jump_03, pl09_jump_04, pl09_jump_04,
	pl09_jump_06, pl09_jump_07, pl09_jump_08, pl09_jump_09, pl09_jump_10, pl09_jump_11,
	0,
};

const Sint32 *const pl09_dash_tbl[] = {
	pl09_dash_00, pl09_dash_01, pl09_dash_02, pl09_stand_00, 0,
};

const Sint32 *const pl09_crouch_tbl[] = {
	pl09_crouch_00, pl09_crouch_01, pl09_crouch_02, 0,
};

const Sint32 *const pl09_turn_tbl[] = {
	pl09_turn_00, pl09_turn_01, 0,
};

const Sint32 *const pl09_guard_tbl[] = {
	pl09_guard_00, pl09_guard_01, pl09_guard_02, pl09_guard_01, pl09_guard_04, pl09_guard_05,
	pl09_guard_06, pl09_guard_05, 0,
};

const Sint32 *const pl09_air_guard_tbl[] = {
	pl09_air_guard_00, pl09_air_guard_01, pl09_air_guard_01, pl09_air_guard_03, pl09_air_guard_04, pl09_air_guard_05,
	0,
};

const Sint32 *const pl09_stand_p_tbl[] = {
	pl09_stand_p_00, pl09_stand_p_01, pl09_stand_p_02, 0,
};

const Sint32 *const pl09_stand_k_tbl[] = {
	pl09_stand_k_00, pl09_stand_k_01, pl09_stand_k_02, 0,
};

const Sint32 *const pl09_crouch_p_tbl[] = {
	pl09_crouch_p_00, pl09_crouch_p_01, pl09_crouch_p_02, 0,
};

const Sint32 *const pl09_crouch_k_tbl[] = {
	pl09_crouch_k_00, pl09_crouch_k_01, pl09_crouch_k_02, 0,
};

const Sint32 *const pl09_jump_p_tbl[] = {
	pl09_jump_p_00, pl09_jump_p_01, pl09_jump_p_02, pl09_jump_p_00, pl09_jump_p_01, pl09_jump_p_05,
	0,
};

const Sint32 *const pl09_jump_k_tbl[] = {
	pl09_jump_k_00, pl09_jump_k_01, pl09_jump_k_02, pl09_jump_k_00, pl09_jump_k_01, pl09_jump_k_05,
	0,
};

const Sint32 *const pl09_damage_tbl[] = {
	pl09_damage_00, pl09_damage_01, pl09_damage_02, pl09_damage_03, pl09_damage_04, pl09_damage_05,
	pl09_damage_06, pl09_damage_07, pl09_damage_08, pl09_damage_09, pl09_damage_10, pl09_damage_11,
	pl09_damage_12, pl09_damage_13, pl09_damage_14, pl09_damage_15, pl09_damage_16, pl09_damage_17,
	pl09_damage_18, pl09_damage_19, pl09_damage_20, pl09_damage_21, pl09_damage_22, pl09_damage_23,
	pl09_damage_24, pl09_damage_25, pl09_damage_26, pl09_damage_27, pl09_damage_28, pl09_damage_29,
	pl09_damage_30, pl09_damage_31, pl09_damage_32, pl09_damage_33, pl09_damage_15, 0,
	0,
};

const Sint32 *const pl09_caught_tbl[] = {
	pl09_caught_00, pl09_caught_01, pl09_caught_02, pl09_caught_03, pl09_caught_04, pl09_caught_05,
	pl09_caught_06, pl09_caught_07, pl09_caught_08, pl09_caught_09, pl09_caught_10, pl09_caught_11,
	pl09_caught_12, pl09_caught_13, pl09_caught_14, pl09_caught_15, pl09_caught_16, pl09_caught_17,
	pl09_caught_18, pl09_caught_19, pl09_caught_20, pl09_caught_21, 0,
};

const Sint32 *const pl09_dizzy_tbl[] = {
	pl09_dizzy_00, 0,
};

const Sint32 *const pl09_catch_tbl[] = {
	pl09_catch_00, pl09_catch_01, pl09_catch_02, 0,
};

const Sint32 *const pl09_get_up_tbl[] = {
	pl09_get_up_00, pl09_get_up_01, pl09_get_up_02, pl09_get_up_03, 0,
};

const Sint32 *const pl09_appear_tbl[] = {
	pl09_appear_00, 0,
};

const Sint32 *const pl09_win_tbl[] = {
	pl09_win_00, pl09_win_01, pl09_win_02, pl09_win_03, pl09_win_04, pl09_win_05,
	pl09_win_06, pl09_win_07, pl09_win_08, 0,
};

const Sint32 *const pl09_extra_tbl[] = {
	pl09_extra_00, pl09_extra_01, pl09_extra_02, pl09_extra_03, pl09_extra_04, pl09_extra_05,
	pl09_extra_06, pl09_extra_07, pl09_extra_08, pl09_extra_09, 0,
};

const Sint32 *const pl09_special_tbl[] = {
	pl09_special_00, pl09_special_01, pl09_special_02, pl09_special_03, pl09_special_04, pl09_special_05,
	pl09_special_06, pl09_special_07, pl09_special_08, pl09_special_09, pl09_special_10, pl09_special_11,
	pl09_special_12, pl09_special_13, pl09_special_14, pl09_special_15, pl09_special_16, pl09_special_17,
	pl09_special_18, pl09_special_19, pl09_special_20, pl09_special_21, pl09_special_22, pl09_special_23,
	pl09_special_24, pl09_special_25, pl09_special_26, pl09_special_27, pl09_special_28, pl09_special_29,
	pl09_special_30, pl09_special_31, 0,
};

const Sint32 *const pl09_super_tbl[] = {
	pl09_super_00, pl09_super_01, pl09_super_02, pl09_super_03, pl09_super_04, pl09_super_05,
	pl09_super_06, pl09_super_07, 0,
};

const Sint32 *const pl09_effect_tbl[] = {
	pl09_effect_00, pl09_effect_01, pl09_effect_02, pl09_effect_03, pl09_effect_04, pl09_effect_05,
	pl09_effect_06, pl09_effect_07, pl09_effect_08, 0,
};

const Sint32 *const pl09_change_tbl[] = {
	pl09_change_00, pl09_change_01, pl09_change_02, pl09_change_03, pl09_change_04, 0,
};

const Sint32 *const pl09_grp27_tbl[] = {
	pl09_grp27_00, pl09_grp27_01, pl09_grp27_02, pl09_grp27_03, pl09_grp27_04, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Change 0: act_change_tag_init
   group 25 0
   Char_Fly 0 */
const Sint32 pl09_change_00[] = {
	CG_FRAME(    0,    0,   10,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   340, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   10, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_change_00 ),
};

/* Char_Change 1: act_change_vcounter_init
   group 25 1
   Char_Fly 1: act_fly */
const Sint32 pl09_change_01[] = {
	CG_FRAME( 0x80,    0,    2,           0,   293, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   294, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0, -128, END_OF_DATA,   295, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_change_01 ),
};

/* Char_Change 2: act_change_out_bound
   group 25 2
   Char_Fly 2 */
const Sint32 pl09_change_02[] = {
	CG_FRAME(    0,   80,    2,           0,   276, 0x80,   0x00,    0,  0x8d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   277, 0x80,   0x00,    0,  0x8e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   278, 0x80,   0x00,    0,  0x8f,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2, END_OF_DATA,   279, 0x80,   0x00,    0,  0x90,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call
   group 25 3
   Char_Fly 3 */
const Sint32 pl09_change_03[] = {
	CG_FRAME(    0,    0,    3,           0,   342, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl09_change_03_loop[] = {
	CG_FRAME(    0,    0,   42, END_OF_DATA,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_change_03_loop ),
};

/* Char_Change 4: act_change_assist_end
   group 25 4
   Char_Fly 4 */
const Sint32 pl09_change_04[] = {
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_change_04 ),
};

/* Char_Stand 6 */
const Sint32 pl09_stand_06[] = {
	CG_FRAME(    0,    0,    8,           0,   380, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   369, 0x80,   0x00,    0,  0x40,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8,           0,   406, 0x80,   0x00,    0,  0x41,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   221, 0x80,   0x00,    0,  0x42,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_stand_06 ),
};

/* Char_Stand 7 */
const Sint32 pl09_stand_07[] = {
	CG_FRAME(    0,    0,   10,           0,   221, 0x80,   0x00,    0,  0x42,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   406, 0x80,   0x00,    0,  0x41,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10,           0,   369, 0x80,   0x00,    0,  0x40,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   380, 0x80,   0x00,    0,  0x01,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_stand_07 ),
};

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5
   Char_Dash 3 */
const Sint32 pl09_stand_00[] = {
	CG_FRAME(    0,    0,    3,           0,   127, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   128, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   129, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   130, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   131, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   132, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   133, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   134, 0x80,   0x00,    0,  0x14,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   135, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   136, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   137, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   138, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   139, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   140, 0x80,   0x00,    0,  0x16,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   141, 0x80,   0x00,    0,  0x16,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   142, 0x80,   0x00,    0,  0x16,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   143, 0x80,   0x00,    0,  0x16,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   144, 0x80,   0x00,    0,  0x16,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   145, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   146, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   147, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   148, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3,           0,   149, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   85,    3, END_OF_DATA,   150, 0x80,   0x00,    0,  0x15,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_stand_00 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl09_stand_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   380, 0x80,   0x00,  ATK,  0x01,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    1,           0,   381, 0x80,   0x00,  ATK,  0x02,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x00,  ATK,  0x03,          0,    0,   0,     1,      0,   9 ),
	CG_FRAME(    0,    0,    6,           0,   383, 0x80,   0x00,  ATK,  0x04,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   384, 0x80,   0x00,  ATK,  0x05,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl09_stand_p_01[] = {
	CG_FRAME(    0,    0,    6,           0,   385, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   386, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     1,      0,  11 ),
	CG_FRAME(    0,    0,    8,           0,   386, 0x80,   0x00,  ATK,  0x06,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   387, 0x80,   0x00,  ATK,  0x07,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    3,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl09_stand_p_02[] = {
	CG_FRAME(    0,    0,    6,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    2,           0,   391, 0x80,   0x00,  ATK,  0x08,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x00,  ATK,  0x09,       0x03,    0,   0,     1,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   393, 0x80,   0x00,  ATK,  0x13,          0,    0,   0,     1,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   394, 0x80,   0x00,  ATK,  0x13,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   393, 0x80,   0x00,  ATK,  0x13,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,  ATK,  0x13,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   395, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_p_02 ),
};

/* Char_Extra 8: snapback_init (Snapback) */
const Sint32 pl09_extra_08[] = {
	CG_FRAME(    0,    0,    8,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    1,    0,    2,           0,   390, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    2,           0,   391, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   393, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    2,           0,   394, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   393, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    3,           0,   394, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    4,           0,   395, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_p_02 ),
};

/* Char_Effect 0 */
const Sint32 pl09_effect_00[] = {
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    6,           0,   400, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   402, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   403, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   404, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   405, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_00 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl09_crouch_p_00[] = {
	CG_FRAME(    0,    0,    3,           0,   433, 0x80,   0x00,  ATK,  0x0a,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    1,           0,   434, 0x80,   0x00,  ATK,  0x0b,          0,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,    4,           0,   435, 0x80,   0x00,  ATK,  0x0c,          0,    0,   0,     1,      0,  21 ),
	CG_FRAME(    0,    0,    2,           0,   436, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   437, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl09_crouch_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   438, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   439, 0x80,   0x00,  ATK,  0x0d,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    6,           0,   440, 0x80,   0x00,  ATK,  0x0e,          0,    0,   0,     1,      0,  23 ),
	CG_FRAME(    0,    0,    4,           0,   441, 0x80,   0x00,  ATK,  0x0f,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   442, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl09_crouch_p_02[] = {
	CG_FRAME(    0,    0,    3,           0,   443, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    1,    2,           0,   444, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    2,    1,           0,   445, 0x80,   0x00,  ATK,  0x10,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    3,    8,           0,   446, 0x80,   0x00,  ATE,  0x11,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    4,    5,           0,   447, 0x80,   0x00,  ATE,  0x12,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    5,   14,           0,   447, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    6,    6, END_OF_DATA,   448, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_crouch_p_02 ),
};

/* Char_Effect 6 */
const Sint32 pl09_effect_06[] = {
	CG_FRAME(    0,    0,    6,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   449, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   450, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   451, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   452, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl09_effect_07[] = {
	CG_FRAME(    0,    0,    6,           0,    -1, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   453, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   454, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    5,           0,   455, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    9, END_OF_DATA,   456, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_07 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl09_stand_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   407, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   411, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl09_stand_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   412, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   413, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    6,           0,   414, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    2,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    3,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   418, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl09_stand_k_02[] = {
	CG_FRAME(    0,    1,    4,           0,   419, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   420, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    3,           0,   421, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    5,           0,   422, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   424, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   425, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_stand_k_02 ),
};

/* Char_Effect 1 */
const Sint32 pl09_effect_01[] = {
	CG_FRAME(    0,    0,    4,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   428, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    5,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   431, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_01 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl09_crouch_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   457, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   458, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    2,           0,   459, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    2,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   461, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl09_crouch_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   462, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   463, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    6,           0,   464, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    2,           0,   465, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    2,           0,   466, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    2,           0,   467, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   468, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl09_crouch_k_02[] = {
	CG_FRAME(    0,    0,    4,           0,   469, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    8,           0,   471, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   474, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   475, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_crouch_k_02 ),
};

/* Char_Extra 9: pl09_upper */
const Sint32 pl09_extra_09[] = {
	CG_FRAME(    0,    0,    4,           0,   469, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   470, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    8,           0,   471, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,    2,           0,   472, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    2,           0,   473, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    4,           0,   474, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   475, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl09_extra_09 ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl09_appear_00[] = {
	CG_FRAME(    0,    0,   36,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    4,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    4,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    4,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    4,           0,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    4,           0,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    3,    4,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,    4,           0,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,    4,           0,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    4,    4,           0,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,    4,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,    4,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    5,    4,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_appear_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl09_crouch_01[] = {
	CG_FRAME(    0,   85,    3,           0,   151, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   152, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   153, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   154, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   155, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   156, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   157, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   158, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   159, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   160, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   161, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   162, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   163, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   164, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   165, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   166, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   167, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   168, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   169, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   170, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   171, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   172, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3,           0,   173, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    3, END_OF_DATA,   174, 0x80,   0x00,    0,  0x43,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_crouch_01 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl09_crouch_00[] = {
	CG_FRAME(    2,   85,    1,           0,   284, 0x80,   0x00,    0,  0x56,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,   85,    1,           0,   285, 0x80,   0x00,    0,  0x57,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    1, END_OF_DATA,   286, 0x80,   0x00,    0,  0x58,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_crouch_00 ),
};

const Sint32 pl09_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,   85,    1, END_OF_DATA,   286, 0x80,   0x00,    0,  0x58,          0,    0,   0,     0,      0,   2 ),
	0x000016a8,
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl09_crouch_02[] = {
	CG_FRAME(    2,   85,    1,           0,   286, 0x80,   0x00,    0,  0x58,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    1,   85,    1,           0,   285, 0x80,   0x00,    0,  0x57,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,   85,    1, END_OF_DATA,   284, 0x80,   0x00,    0,  0x56,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl09_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   289, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl09_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   292, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl09_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold
   Char_Guard 3 */
const Sint32 pl09_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl09_guard_02[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_guard_02 ),
};

/* Char_Guard 4 */
const Sint32 pl09_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   297, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold
   Char_Guard 7 */
const Sint32 pl09_guard_05[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl09_guard_06[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   297, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_guard_06 ),
};

/* Char_Stand 2: act_walk (2 + no) */
const Sint32 pl09_stand_02[] = {
	CG_FRAME(    0,    0,    4,           0,   177, 0x80,   0x00,    0,  0x19,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   178, 0x80,   0x00,    0,  0x1a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   179, 0x80,   0x00,    0,  0x1b,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    4,           0,   180, 0x80,   0x00,    0,  0x1c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   181, 0x80,   0x00,    0,  0x1d,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   182, 0x80,   0x00,    0,  0x1e,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   183, 0x80,   0x00,    0,  0x1f,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x00,    0,  0x20,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x80,   0x00,    0,  0x21,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   186, 0x80,   0x00,    0,  0x22,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   175, 0x80,   0x00,    0,  0x17,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   176, 0x80,   0x00,    0,  0x18,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_stand_02 ),
};

/* Char_Stand 3 */
const Sint32 pl09_stand_03[] = {
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x24,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x25,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   195, 0x80,   0x00,    0,  0x28,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    3,    4,           0,   194, 0x80,   0x00,    0,  0x26,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   193, 0x80,   0x00,    0,  0x26,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   192, 0x80,   0x00,    0,  0x27,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   191, 0x80,   0x00,    0,  0x28,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   190, 0x80,   0x00,    0,  0x29,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   189, 0x80,   0x00,    0,  0x2a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    4,    4,           0,   188, 0x80,   0x00,    0,  0x2b,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   187, 0x80,   0x00,    0,  0x23,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   198, 0x80,   0x00,    0,  0x23,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl09_jump_00[] = {
	CG_FRAME(    0,   81,    4,           0,   225, 0x80,   0x00,    0,  0x5c,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    6,           0,   226, 0x80,   0x00,    0,  0x5d,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4,           0,   227, 0x80,   0x00,    0,  0x5e,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    2,           0,   228, 0x80,   0x00,    0,  0x5f,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    8,           0,   229, 0x80,   0x00,    0,  0x60,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4,           0,   230, 0x80,   0x00,    0,  0x61,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    2,           0,   231, 0x80,   0x00,    0,  0x62,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,   -1, END_OF_DATA,   232, 0x80,   0x00,    0,  0x63,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_jump_09 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl09_jump_01[] = {
	CG_FRAME(    0,   81,    4,           0,   242, 0x80,   0x00,    0,  0x64,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    6,           0,   243, 0x80,   0x00,    0,  0x65,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4,           0,   244, 0x80,   0x00,    0,  0x66,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4,           0,   245, 0x80,   0x00,    0,  0x67,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   246, 0x80,   0x00,    0,  0x68,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   247, 0x80,   0x00,    0,  0x69,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   248, 0x80,   0x00,    0,  0x6a,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   249, 0x80,   0x00,    0,  0x6b,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   250, 0x80,   0x00,    0,  0x6c,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   251, 0x80,   0x00,    0,  0x6d,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   252, 0x80,   0x00,    0,  0x6e,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,   -1, END_OF_DATA,   253, 0x80,   0x00,    0,  0x6f,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_jump_09 ),
};

/* Char_Jump 2 */
const Sint32 pl09_jump_02[] = {
	CG_FRAME(    0,   81,    4,           0,   263, 0x80,   0x00,    0,  0x70,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    6,           0,   274, 0x80,   0x00,    0,  0x71,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4,           0,   273, 0x80,   0x00,    0,  0x72,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4,           0,   272, 0x80,   0x00,    0,  0x73,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   271, 0x80,   0x00,    0,  0x74,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   270, 0x80,   0x00,    0,  0x75,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   269, 0x80,   0x00,    0,  0x76,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   268, 0x80,   0x00,    0,  0x77,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   267, 0x80,   0x00,    0,  0x78,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   266, 0x80,   0x00,    0,  0x79,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    3,           0,   265, 0x80,   0x00,    0,  0x7a,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,   80,    4, END_OF_DATA,   264, 0x80,   0x00,    0,  0x7b,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_jump_09 ),
};

/* Char_Jump 3 */
const Sint32 pl09_jump_03[] = {
	CG_FRAME(    0,   80,    4,           0,   233, 0x80,   0x00,    0,  0x5c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4, END_OF_DATA,   225, 0x80,   0x00,    0,  0x5c,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_jump_03 ),
};

/* Char_Jump 4
   Char_Jump 5 */
const Sint32 pl09_jump_04[] = {
	CG_FRAME(    0,   80,    4,           0,   254, 0x80,   0x00,    0,  0x7c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4, END_OF_DATA,   242, 0x80,   0x00,    0,  0x7c,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_jump_04 ),
};

/* Char_Jump 6 */
const Sint32 pl09_jump_06[] = {
	CG_FRAME(    0,   81,    4,           0,   225, 0x80,   0x00,    0,  0x5c,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   226, 0x80,   0x00,    0,  0x5d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   234, 0x80,   0x00,    0,  0x7d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   235, 0x80,   0x00,    0,  0x7e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   236, 0x80,   0x00,    0,  0x7f,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   237, 0x80,   0x00,    0,  0x80,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   234, 0x80,   0x00,    0,  0x7d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   235, 0x80,   0x00,    0,  0x7e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   226, 0x80,   0x00,    0,  0x5d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    4,           0,   227, 0x80,   0x00,    0,  0x5e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   228, 0x80,   0x00,    0,  0x5f,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,   12,           0,   229, 0x80,   0x00,    0,  0x60,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    6,           0,   230, 0x80,   0x00,    0,  0x61,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    4,           0,   231, 0x80,   0x00,    0,  0x62,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   232, 0x80,   0x00,    0,  0x63,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump */
const Sint32 pl09_jump_09[] = {
	CG_FRAME(    0,   80,    2,           0,   238, 0x80,   0x00,    0,  0x81,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   239, 0x80,   0x00,    0,  0x82,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   240, 0x80,   0x00,    0,  0x83,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2, END_OF_DATA,   241, 0x80,   0x00,    0,  0x84,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_jump_09 ),
};

/* Char_Jump 7 */
const Sint32 pl09_jump_07[] = {
	CG_FRAME(    0,   81,    4,           0,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   255, 0x80,   0x00,    0,  0x85,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   256, 0x80,   0x00,    0,  0x86,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   257, 0x80,   0x00,    0,  0x87,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   258, 0x80,   0x00,    0,  0x88,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   255, 0x80,   0x00,    0,  0x85,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   256, 0x80,   0x00,    0,  0x86,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   243, 0x80,   0x00,    0,  0x65,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   244, 0x80,   0x00,    0,  0x66,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   245, 0x80,   0x00,    0,  0x67,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   246, 0x80,   0x00,    0,  0x68,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   247, 0x80,   0x00,    0,  0x69,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   248, 0x80,   0x00,    0,  0x6a,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   249, 0x80,   0x00,    0,  0x6b,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   250, 0x80,   0x00,    0,  0x6c,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   251, 0x80,   0x00,    0,  0x6d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   252, 0x80,   0x00,    0,  0x6e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   253, 0x80,   0x00,    0,  0x6f,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 10: change_in */
const Sint32 pl09_jump_10[] = {
	CG_FRAME(    0,   80,    2,           0,   259, 0x80,   0x00,    0,  0x89,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   260, 0x80,   0x00,    0,  0x8a,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   261, 0x80,   0x00,    0,  0x8b,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2, END_OF_DATA,   262, 0x80,   0x00,    0,  0x8c,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_jump_10 ),
};

/* Char_Jump 8 */
const Sint32 pl09_jump_08[] = {
	CG_FRAME(    0,   81,    4,           0,   263, 0x80,   0x00,    0,  0x70,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   276, 0x80,   0x00,    0,  0x8d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   277, 0x80,   0x00,    0,  0x8e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   278, 0x80,   0x00,    0,  0x8f,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   279, 0x80,   0x00,    0,  0x90,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   276, 0x80,   0x00,    0,  0x8d,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   277, 0x80,   0x00,    0,  0x8e,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   274, 0x80,   0x00,    0,  0x71,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   273, 0x80,   0x00,    0,  0x72,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   272, 0x80,   0x00,    0,  0x73,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   271, 0x80,   0x00,    0,  0x74,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   270, 0x80,   0x00,    0,  0x75,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   269, 0x80,   0x00,    0,  0x76,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   268, 0x80,   0x00,    0,  0x77,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   267, 0x80,   0x00,    0,  0x78,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   266, 0x80,   0x00,    0,  0x79,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   265, 0x80,   0x00,    0,  0x7a,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   264, 0x80,   0x00,    0,  0x7b,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    3,           0,   264, 0x80,   0x00,    0,  0x7b,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Jump 11: act_change_out_recover */
const Sint32 pl09_jump_11[] = {
	CG_FRAME(    0,   80,    2,           0,   280, 0x80,   0x00,    0,  0x91,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   281, 0x80,   0x00,    0,  0x92,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2,           0,   282, 0x80,   0x00,    0,  0x93,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,   80,    2, END_OF_DATA,   283, 0x80,   0x00,    0,  0x94,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_jump_11 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl09_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   299, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl09_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_air_guard_01 ),
};

/* Char_Air_Guard 3 */
const Sint32 pl09_air_guard_03[] = {
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_air_guard_03 ),
};

/* Char_Air_Guard 4 */
const Sint32 pl09_air_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_air_guard_04 ),
};

/* Char_Air_Guard 5 */
const Sint32 pl09_air_guard_05[] = {
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   232, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_air_guard_05 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl09_dash_01[] = {
	CG_FRAME(    0,    0,    2,           0,   216, 0x00,   0x00,    0,  0x4f,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,   18,           0,   217, 0x00,   0x00,    0,  0x50,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    2,    2,           0,   218, 0x00,   0x00,    0,  0x51,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    3,    2,           0,   219, 0x00,   0x00,    0,  0x52,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    4,    2,           0,   216, 0x00,   0x00,    0,  0x4f,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl09_dash_01_loop[] = {
	CG_FRAME(    0,    5,   20,           0,   220, 0x00,   0x00,    0,  0x53,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    6,    2,           0,   221, 0x00,   0x00,    0,  0x42,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    7,    2, END_OF_DATA,   222, 0x00,   0x00,    0,  0x54,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_dash_01_loop ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl09_dash_00[] = {
	CG_FRAME(    0,    0,    2,           0,   199, 0x80,   0x00,    0,  0x44,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   200, 0x80,   0x00,    0,  0x45,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl09_dash_00_loop[] = {
	CG_FRAME(    0,   80,    4,           0,   211, 0x80,   0x00,    0,  0x46,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   212, 0x80,   0x00,    0,  0x46,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   201, 0x80,   0x00,    0,  0x46,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   202, 0x80,   0x00,    0,  0x47,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   203, 0x80,   0x00,    0,  0x48,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   204, 0x80,   0x00,    0,  0x49,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   205, 0x00,   0x00,    0,  0x4a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   206, 0x80,   0x00,    0,  0x4b,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   207, 0x80,   0x00,    0,  0x4a,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   208, 0x00,   0x00,    0,  0x49,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   209, 0x80,   0x00,    0,  0x48,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   210, 0x00,   0x00,    0,  0x47,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_dash_00_loop ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl09_dash_02[] = {
	CG_FRAME(    0,   81,   16,           0,   213, 0x80,   0x00,    0,  0x4c,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,   80,    4,           0,   214, 0x80,   0x00,    0,  0x4d,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   215, 0x00,   0x00,    0,  0x4e,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_dash_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl09_jump_p_00[] = {
	CG_FRAME(    1,    0,    4,           0,   482, 0x80,   0x00,  ATK,  0x9b,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   483, 0x80,   0x00,  ATK,  0x9c,          0,    0,   0,     1,      0,  34 ),
	CG_FRAME(    0,    0,    3,           0,   484, 0x80,   0x00,  ATK,  0x9d,          0,    0,   0,     1,      0,  34 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   485, 0x80,   0x00,  ATK,  0x9e,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_p_00 ),
};

/* Char_Effect 2 */
const Sint32 pl09_effect_02[] = {
	CG_FRAME(    0,    0,    4,           0,   486, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   487, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   488, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    2,           0,   489, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   490, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   491, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_02 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl09_jump_p_01[] = {
	CG_FRAME(    0,    0,    7,           0,   476, 0x80,   0x00,  ATK,  0x95,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   477, 0x80,   0x00,  ATK,  0x96,          0,    0,   0,     1,      0,  35 ),
	CG_FRAME( 0x80,    0,    2,           0,   478, 0x80,   0x00,  ATK,  0x97,          0,    0,   0,     1,      0,  35 ),
	CG_FRAME(    0,    0,    8,           0,   479, 0x80,   0x00,  ATK,  0x98,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    5,           0,   480, 0x80,   0x00,  ATK,  0x99,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   481, 0x80,   0x00,  ATK,  0x9a,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_p_01 ),
};

/* Char_Jump_P 5 */
const Sint32 pl09_jump_p_05[] = {
	CG_FRAME(    0,    0,   10,           0,   514, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    4,           0,   515, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   516, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   -1,           0,   517, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   518, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_p_05 ),
};

/* Char_Effect 3 */
const Sint32 pl09_effect_03[] = {
	CG_FRAME(    0,    0,    4,           0,   519, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   520, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    1,           0,   521, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    3,           0,   522, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   523, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   524, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   525, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_03 ),
};

/* Char_Jump_P 2 (heavy) */
const Sint32 pl09_jump_p_02[] = {
	CG_FRAME(    0,    0,    8,           0,   492, 0x80,   0x00,  ATK,  0x9f,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   493, 0x80,   0x00,  ATK,  0xa0,       0x03,    0,   0,     1,      0,  37 ),
	CG_FRAME( 0x80,    0,    2,           0,   494, 0x80,   0x00,  ATK,  0xa1,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME( 0x80,    0,    2,           0,   495, 0x80,   0x00,  ATK,  0xa2,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   12,           0,   496, 0x80,   0x00,  ATK,  0xa3,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   497, 0x80,   0x00,  ATK,  0xa4,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_p_02 ),
};

const Sint32 pl09_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   492, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   493, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   494, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   495, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000027f0,
};

/* Char_Jump_K 0 (light)
   Char_Jump_K 3 */
const Sint32 pl09_jump_k_00[] = {
	CG_FRAME(    0,    0,    4,           0,   498, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   10,           0,   499, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    2,           0,   500, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    2,           0,   501, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   502, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl09_jump_k_01[] = {
	CG_FRAME(    0,    0,    7,           0,   503, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   504, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    8,           0,   504, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   505, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   506, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   507, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_k_01 ),
};

/* Char_Jump_K 5 */
const Sint32 pl09_jump_k_05[] = {
	CG_FRAME(    0,    0,    6,           0,   526, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   529, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    4,           0,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_k_05 ),
};

/* Char_Jump_K 2 (heavy) */
const Sint32 pl09_jump_k_02[] = {
	CG_FRAME(    0,    0,    9,           0,   508, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   509, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    8,           0,   509, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   510, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   511, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    2,           0,   512, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   513, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_jump_k_02 ),
};

/* Char_Win 0 */
const Sint32 pl09_win_00[] = {
	CG_FRAME(    0,    0,   18,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    8,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   325, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_00_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_00_loop ),
};

/* Char_Win 5 */
const Sint32 pl09_win_05[] = {
	CG_FRAME(    0,    0,    6,           0,   330, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   331, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   332, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   333, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   334, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_05_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   335, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_05_loop ),
};

/* Char_Win 7: pls09_03_move */
const Sint32 pl09_win_07[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   337, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_07 ),
};

/* Char_Win 8 */
const Sint32 pl09_win_08[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   338, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_08 ),
};

/* Char_Win 1 */
const Sint32 pl09_win_01[] = {
	CG_FRAME(    0,    0,   15,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_01_loop[] = {
	CG_FRAME(    0,    0,   -1, END_OF_DATA,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_01_loop ),
};

/* Char_Win 6 */
const Sint32 pl09_win_06[] = {
	CG_FRAME(    0,    0,    5,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_06_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_06_loop ),
};

/* Char_Win 2: win_init */
const Sint32 pl09_win_02[] = {
	CG_FRAME(    0,    0,    8,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   372, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_02_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   373, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_02_loop ),
};

/* Char_Win 4 */
const Sint32 pl09_win_04[] = {
	CG_FRAME(    0,    0,    8,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_04_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_04_loop ),
};

/* Char_Win 3 */
const Sint32 pl09_win_03[] = {
	CG_FRAME(    0,    0,    8,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_win_03_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_win_03_loop ),
};

/* Char_Extra 7: sp_chouhatsu (Taunt) */
const Sint32 pl09_extra_07[] = {
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_extra_07 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl09_damage_00[] = {
	CG_FRAME(    0,    0,    4,           0,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   736, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   737, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl09_damage_01[] = {
	CG_FRAME(    0,    0,    5,           0,   739, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   740, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   741, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   742, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl09_damage_02[] = {
	CG_FRAME(    0,    0,    4,           0,   744, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   745, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   746, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   747, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   748, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   749, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   750, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl09_damage_03[] = {
	CG_FRAME(    0,    0,    4,           0,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   753, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl09_damage_04[] = {
	CG_FRAME(    0,    0,    4,           0,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   758, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   759, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl09_damage_05[] = {
	CG_FRAME(    0,    0,    4,           0,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   764, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   770, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl09_damage_06[] = {
	CG_FRAME(    0,    0,    4,           0,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl09_damage_07[] = {
	CG_FRAME(    0,    0,    5,           0,   776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    5,           0,   777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   778, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl09_damage_08[] = {
	CG_FRAME(    0,    0,    4,           0,   781, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   782, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   783, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   786, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   787, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   788, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl09_damage_08 ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl09_damage_09[] = {
	CG_FRAME(    0,    0,    3,           0,   824, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   825, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   826, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   827, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   828, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   829, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   830, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   831, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_damage_09_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   831, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl09_damage_09_loop ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl09_damage_10[] = {
	CG_FRAME(    0,    0,    4,           0,   790, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   791, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl09_damage_11[] = {
	CG_FRAME(    0,    0,    4,           0,   793, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   794, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl09_damage_12[] = {
	CG_FRAME(    0,    0,    4,           0,   796, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   797, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_12 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl09_damage_31[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl09_damage_31 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl09_get_up_00[] = {
	CG_FRAME(    0,    0,    6,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl09_get_up_01[] = {
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2, END_OF_DATA,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl09_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   12,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    5,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    5, END_OF_DATA,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl09_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_get_up_03 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air */
const Sint32 pl09_damage_13[] = {
	CG_FRAME(    0,    0,    4,           0,   815, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   816, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl09_damage_13 ),
};

/* Char_Damage 14 */
const Sint32 pl09_damage_14[] = {
	CG_FRAME(    0,    0,    4,           0,   818, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   819, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl09_damage_14 ),
};

/* Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl09_damage_15[] = {
	CG_FRAME(    0,    0,    4,           0,   821, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   822, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl09_damage_15 ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl09_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   843, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   844, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl09_damage_17[] = {
	CG_FRAME(    0,    0,    2,           0,   845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   846, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl09_damage_18[] = {
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   848, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl09_damage_19[] = {
	CG_FRAME(    0,    0,    2,           0,   849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   850, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl09_damage_20[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl09_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   843, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   844, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 22 */
const Sint32 pl09_damage_22[] = {
	CG_FRAME(    0,    0,    2,           0,   845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   846, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 23 */
const Sint32 pl09_damage_23[] = {
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   848, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 24 */
const Sint32 pl09_damage_24[] = {
	CG_FRAME(    0,    0,    2,           0,   849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   850, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
};

/* Char_Damage 25 */
const Sint32 pl09_damage_25[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl09_damage_26[] = {
	CG_FRAME(    0,    0,    3,           0,   837, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   838, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3,           0,   839, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   841, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl09_damage_26 ),
};

/* Char_Damage 27 */
const Sint32 pl09_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_damage_27 ),
};

const Sint32 pl09_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   837, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   838, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   839, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   841, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    6,           0,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7,           0,   307, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_damage_27 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl09_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl09_damage_33[] = {
	CG_FRAME(    0,    0,    4,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl09_damage_28[] = {
	CG_FRAME(    0,    0,    2,           0,   853, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   854, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   855, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   856, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   857, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   858, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl09_damage_29[] = {
	CG_FRAME(    0,    0,    2,           0,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   862, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2,           0,   863, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   864, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl09_damage_29 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl09_damage_30[] = {
	CG_FRAME(    0,    0,    4,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl09_damage_30 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl09_dizzy_00[] = {
	CG_FRAME(    0,    0,    6,           0,   798, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   799, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   800, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   801, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   802, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   803, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   804, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   805, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   806, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   807, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   808, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   809, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   810, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   811, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   812, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   813, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_dizzy_00 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl09_catch_00[] = {
	CG_FRAME(    0,    0,   10,           0,   532, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,    4,           0,   533, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,    3,           0,   534, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0003,   0 ),
	CG_FRAME(    0,    0,    1,           0,   535, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0, 0x0004,   0 ),
};

const Sint32 pl09_catch_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   535, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   536, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   537, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   538, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   539, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   540, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   541, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   542, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   543, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,    4,           0,   544, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,    4,           0,   545, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   546, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_NEXT( pl09_catch_00_loop ),
};

/* Char_Catch 1: nml_k_set */
const Sint32 pl09_catch_01[] = {
	CG_FRAME(    1,    0,   10,           0,   547, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0006,   0 ),
	CG_FRAME(    1,    0,    4,           0,   548, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    1,    0,    2,           0,   549, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,    6,           0,   550, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    1,    2,           0,   551, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    2,    2,           0,   552, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    3,    8,           0,   552, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    3,    2,           0,   553, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    3,    2,           0,   554, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d,   0 ),
	CG_FRAME(    0, 0x84,   12,           0,   555, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e,   0 ),
	CG_FRAME(    0,    5,    6,           0,   556, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    6,    6,           0,   557, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    7,    4,           0,   558, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   558, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_catch_01 ),
};

/* Char_Effect 8: pls09_04_char_set */
const Sint32 pl09_effect_08[] = {
	CG_FRAME(    0,    0,    4,           0,   560, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   561, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   562, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   563, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   564, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   565, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   566, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_08 ),
};

const Sint32 pl09_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   567, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   568, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   569, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    2,           0,   570, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,   10,           0,   571, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   572, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   573, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   574, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   575, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   576, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   577, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   578, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   579, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   580, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   581, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    3,           0,   582, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   584, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    2,    6, END_OF_DATA,   583, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00003fe8,
};

/* Char_Catch 2: jump_p_set */
const Sint32 pl09_catch_02[] = {
	CG_FRAME(    1,    0,    4,           0,   585, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    1,    0,    4,           0,   586, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010,   0 ),
	CG_FRAME(    1,    0,    4,           0,   587, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    1,    0,    2,           0,   588, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    1,    0,   10,           0,   589, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    2,           0,   590, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    1,    0,    2,           0,   591, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    2,           0,   592, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    1,    0,    2,           0,   593, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    2,           0,   594, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    1,    0,    2,           0,   595, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    2,           0,   596, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    1,    0,    2,           0,   597, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    2,           0,   598, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    1,    0,    2,           0,   599, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    1,    0,    2,           0,   600, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   10,           0,   601, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,    4,           0,   602, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    0,    2,           0,   603, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    1,   12, END_OF_DATA,   604, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0018,   0 ),
	CG_NEXT( pl09_catch_02 ),
};

/* Char_Special 5: fist_init (Ice Fist) */
const Sint32 pl09_special_05[] = {
	CG_FRAME(    0,    0,    3,           0,   689, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   693, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   701, 0x80,   0x00,    0,  0x00,       0x20,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   702, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_special_05 ),
};

const Sint32 pl09_unused_04[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    6,           0,   689, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   693, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000442c,
};

const Sint32 pl09_unused_05[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   871, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   872, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   873, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   874, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   875, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   876, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   877, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   878, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000044d0,
};

const Sint32 pl09_unused_06[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   891, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   892, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   893, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   894, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00004574,
};

/* Char_Extra 5: t_kick_init */
const Sint32 pl09_extra_05[] = {
	CG_FRAME(    0,    0,    6,           0,   503, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl09_extra_05_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   504, 0xa0,   0x20,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_NEXT( pl09_extra_05_loop ),
};

/* Char_Extra 6: t_kick_move */
const Sint32 pl09_extra_06[] = {
	CG_FRAME(    0,    0,    2,           0,   704, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   705, 0x80,   0x00,  ATE,  0x00,       0x03,    0,   0,     0,      0,   6 ),
};

const Sint32 pl09_extra_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   706, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   707, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   708, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   709, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   710, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   711, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   706, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   707, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   708, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   709, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   710, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   711, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   706, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    2,           0,   707, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   708, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   709, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   710, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_extra_06_loop ),
};

/* Char_Special 1: shl09_02_block_init */
const Sint32 pl09_special_01[] = {
	CG_FRAME(    0,    0,    4,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,   0 ),
};

const Sint32 pl09_special_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0, 32768, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2,           0, 32769, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32770, 0x80,   0x80,  ATK,  0x00,          0,    0,   0,     0,      0,  57 ),
	CG_NEXT( pl09_special_01_loop ),
};

/* Char_Special 2 */
const Sint32 pl09_special_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32771, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32773, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_02 ),
};

/* Char_Special 3 */
const Sint32 pl09_special_03[] = {
	CG_FRAME(    0,    0,    4,           0, 32774, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32775, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_03 ),
};

/* Char_Special 4 */
const Sint32 pl09_special_04[] = {
	CG_FRAME(    0,    0,    4,           0, 32777, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32778, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32779, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_04 ),
};

/* Char_Super 4: shl09_01_init */
const Sint32 pl09_super_04[] = {
	CG_FRAME(    0,    0,    4,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_NEXT( pl09_super_04 ),
};

/* Char_Super 5: shl09_01_move */
const Sint32 pl09_super_05[] = {
	CG_FRAME(    0,    0,    2,           0, 32783, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32784, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32785, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_super_05 ),
};

/* Char_Special 14 */
const Sint32 pl09_special_14[] = {
	CG_FRAME(    0,    0,    4,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl09_special_15[] = {
	CG_FRAME(    0,    0,    4,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0, 32801, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32803, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_15 ),
};

/* Char_Special 16 */
const Sint32 pl09_special_16[] = {
	CG_FRAME(    0,    0,    4,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_16 ),
};

/* Char_Special 17 */
const Sint32 pl09_special_17[] = {
	CG_FRAME(    0,    0,    4,           0, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32798, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32799, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32800, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_17 ),
};

/* Char_Special 18 */
const Sint32 pl09_special_18[] = {
	CG_FRAME(    0,    0,    4,           0, 32792, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32793, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32794, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_18 ),
};

/* Char_Special 19 */
const Sint32 pl09_special_19[] = {
	CG_FRAME(    0,    0,    4,           0, 32804, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32805, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32806, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_19 ),
};

/* Char_Special 20 */
const Sint32 pl09_special_20[] = {
	CG_FRAME(    0,    0,    4,           0, 32807, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0, 32819, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32820, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32821, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_20 ),
};

/* Char_Special 21 */
const Sint32 pl09_special_21[] = {
	CG_FRAME(    0,    0,    4,           0, 32808, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32814, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32815, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_21 ),
};

/* Char_Special 22 */
const Sint32 pl09_special_22[] = {
	CG_FRAME(    0,    0,    4,           0, 32809, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32816, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32817, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32818, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_22 ),
};

/* Char_Special 23 */
const Sint32 pl09_special_23[] = {
	CG_FRAME(    0,    0,    4,           0, 32810, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32811, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32812, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_23 ),
};

/* Char_Special 24 */
const Sint32 pl09_special_24[] = {
	CG_FRAME(    0,    0,    4,           0, 32822, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32823, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32824, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_24 ),
};

/* Char_Special 25 */
const Sint32 pl09_special_25[] = {
	CG_FRAME(    0,    0,    4,           0, 32825, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0, 32837, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32838, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32839, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_25 ),
};

/* Char_Special 26 */
const Sint32 pl09_special_26[] = {
	CG_FRAME(    0,    0,    4,           0, 32826, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    4,           0, 32831, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32832, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32833, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_26 ),
};

/* Char_Special 27 */
const Sint32 pl09_special_27[] = {
	CG_FRAME(    0,    0,    4,           0, 32827, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32834, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32835, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32836, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_27 ),
};

/* Char_Special 28 */
const Sint32 pl09_special_28[] = {
	CG_FRAME(    0,    0,    4,           0, 32828, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32829, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32830, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_28 ),
};

/* Char_Special 29 */
const Sint32 pl09_special_29[] = {
	CG_FRAME(    0,    0,    4,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32797, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_29 ),
};

/* Char_Special 30 */
const Sint32 pl09_special_30[] = {
	CG_FRAME(    0,    0,    4,           0, 32808, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32814, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32815, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_30 ),
};

/* Char_Special 31 */
const Sint32 pl09_special_31[] = {
	CG_FRAME(    0,    0,    4,           0, 32826, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4,           0, 32831, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32832, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32833, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_special_31 ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl09_extra_02[] = {
	CG_FRAME(    0,    0,    4,           0, 32847, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32848, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32850, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_extra_02 ),
};

/* Char_Special 6: beam_init (Ice Beam, 6 + atck_level) */
const Sint32 pl09_special_06[] = {
	CG_FRAME(    0,    0,    5,           0,   605, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   606, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   607, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   608, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   609, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   610, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   611, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   611, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl09_special_06_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   612, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   613, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   609, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   610, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   611, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_special_06_loop ),
};

/* Char_Special 10: beam_wait (Ice Beam, 10 + atck_level) */
const Sint32 pl09_special_10[] = {
	CG_FRAME(    0,    0,    5,           0,   613, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    5,           0,   614, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   615, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   616, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   606, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   607, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   608, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_special_10 ),
};

/* Char_Special 7 */
const Sint32 pl09_special_07[] = {
	CG_FRAME(    0,    0,    4,           0,   617, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   618, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   619, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   620, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   621, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   622, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   623, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2,           0,   623, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl09_special_07_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   624, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   625, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   621, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6,           0,   622, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   623, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_special_07_loop ),
};

/* Char_Special 11 */
const Sint32 pl09_special_11[] = {
	CG_FRAME(    0,    0,    5,           0,   625, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    5,           0,   626, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    5,           0,   627, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   618, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   620, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl09_special_11 ),
};

const Sint32 pl09_unused_07[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,    6,           0,   633, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   634, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   635, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    6,           0,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000051fc,
};

const Sint32 pl09_unused_08[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    8,           0,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   639, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   630, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   631, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   632, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00005264,
};

/* Char_Special 9 */
const Sint32 pl09_special_09[] = {
	CG_FRAME(    0,    0,    4,           0,   641, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   642, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   643, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   644, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   645, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    6,           0,   646, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   647, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   648, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   648, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl09_special_09_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   649, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   650, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   646, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   647, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   648, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_special_09_loop ),
};

/* Char_Special 13 */
const Sint32 pl09_special_13[] = {
	CG_FRAME(    0,    1,    8,           0,   651, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    8,           0,   652, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   653, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   643, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   644, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   645, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   641, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_special_13 ),
};

const Sint32 pl09_unused_09[] = {	/* nothing reaches it */
	CG_FRAME(    0,    1,    6,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6,           0,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    6, END_OF_DATA,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000548c,
};

const Sint32 pl09_unused_10[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   11,           0,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   10,           0,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   654, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000054f4,
};

/* Char_Special 8 */
const Sint32 pl09_special_08[] = {
	CG_FRAME(    0,    0,    4,           0,   667, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   668, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   669, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   670, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   671, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    6,           0,   672, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   674, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl09_special_08_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   675, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   676, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   672, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   673, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   674, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_special_08_loop ),
};

/* Char_Special 12 */
const Sint32 pl09_special_12[] = {
	CG_FRAME(    0,    1,   11,           0,   677, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   10,           0,   678, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    4,           0,   679, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   669, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   670, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   671, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   667, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_special_12 ),
};

/* Char_Special 0: avalanche_init (Ice Avalanche) */
const Sint32 pl09_special_00[] = {
	CG_FRAME(    0,    0,    6,           0,   680, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   681, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   682, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,   10,           0,   683, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   684, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    3,           0,   684, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   685, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   686, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   52,           0,   687, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   688, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_special_00 ),
};

const Sint32 pl09_unused_11[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    6,           0,   680, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   681, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   682, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,   10,           0,   683, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   684, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3,           0,   684, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   685, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   686, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   52,           0,   687, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   688, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	0x000057fc,
};

/* Char_Super 0: arctic_init (Arctic Attack (hyper)) */
const Sint32 pl09_super_00[] = {
	CG_FRAME(    0,    0,    2,           0,   704, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   705, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl09_super_00_loop[] = {
	CG_FRAME( 0x80,    1,    2,           0,   706, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   707, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   708, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   709, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2,           0,   710, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   711, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_super_00_loop ),
};

/* Char_Super 2: arctic_set (Arctic Attack (hyper)) */
const Sint32 pl09_super_02[] = {
	CG_FRAME(    0,    0,    6,           0,   712, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    1,    6,           0,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
};

const Sint32 pl09_super_02_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   714, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_FRAME(    1,    0,    6,           0,   715, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_FRAME(    2,    0,    6,           0,   716, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_FRAME(    3,    0,    6,           0,   717, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_FRAME(    4,    0,    6,           0,   718, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_FRAME(    5,    0,    6, END_OF_DATA,   713, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_super_02_loop ),
};

/* Char_Super 6: arctic_keep (Arctic Attack (hyper)) */
const Sint32 pl09_super_06[] = {
	CG_FRAME(    0,    0,    6,           0,   714, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   715, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   716, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   717, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6,           0,   718, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   713, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl09_super_06 ),
};

/* Char_Super 1: arctic_init (Arctic Attack (hyper)) */
const Sint32 pl09_super_01[] = {
	CG_FRAME(    0,    0,    4,           0,   720, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl09_super_01_loop[] = {
	CG_FRAME( 0x80,    1,    2,           0,   721, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   722, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   723, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   724, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2,           0,   725, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   726, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_super_01_loop ),
};

/* Char_Super 3: arctic_set (Arctic Attack (hyper)) */
const Sint32 pl09_super_03[] = {
	CG_FRAME(    0,    0,    6,           0,   727, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    1,    6,           0,   728, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl09_super_03_loop[] = {
	CG_FRAME(    0,    0,    6,           0,   729, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   8 ),
	CG_FRAME(    1,    0,    6,           0,   730, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   8 ),
	CG_FRAME(    2,    0,    6,           0,   731, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   8 ),
	CG_FRAME(    3,    0,    6,           0,   732, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   8 ),
	CG_FRAME(    4,    0,    6,           0,   733, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   8 ),
	CG_FRAME(    5,    0,    6, END_OF_DATA,   728, 0x80,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_super_03_loop ),
};

/* Char_Super 7: arctic_keep (Arctic Attack (hyper)) */
const Sint32 pl09_super_07[] = {
	CG_FRAME(    0,    0,    6,           0,   729, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   730, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   731, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   732, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6,           0,   733, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   728, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl09_super_07 ),
};

/* Char_Extra 3: pls09_02_init, shl09_04_init, fly_start5 */
const Sint32 pl09_extra_03[] = {
	CG_FRAME(    0,    0,    6,           0,   907, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   908, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   909, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   910, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   911, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   912, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   913, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   914, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   915, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   916, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   917, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   918, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   919, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   920, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   921, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   922, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   923, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6,           0,   924, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   925, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_NEXT( pl09_extra_03 ),
};

/* Char_Extra 4: shl09_04_move */
const Sint32 pl09_extra_04[] = {
	CG_FRAME(    0,    0,    6,           0,   926, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   927, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   928, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   929, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_extra_04 ),
};

/* Char_Effect 5: pls09_00_init */
const Sint32 pl09_effect_05[] = {
	CG_FRAME(    0,    0,    2,           0, 32840, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32841, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32842, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32843, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32844, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32846, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_05 ),
};

/* Char_Extra 0: pls09_01_init */
const Sint32 pl09_extra_00[] = {
	CG_FRAME(    0,    0,    6,           0,   895, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   896, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   897, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   898, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   899, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   900, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_extra_00 ),
};

/* Char_Extra 1: pls09_01_init */
const Sint32 pl09_extra_01[] = {
	CG_FRAME(    0,    0,    6,           0,   901, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   902, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   903, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   904, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6,           0,   905, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   906, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_extra_01 ),
};

/* Char_Effect 4: pls09_00_init */
const Sint32 pl09_effect_04[] = {
	CG_FRAME(    0,    0,    1,           0, 32851, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32853, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32854, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32855, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32856, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32857, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32858, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32860, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32861, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl09_effect_04 ),
};

/* Char_Caught 0 */
const Sint32 pl09_caught_00[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   735, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl09_caught_01[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   739, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl09_caught_02[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   744, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl09_caught_03[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl09_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl09_caught_04[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   756, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl09_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl09_caught_05[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_NEXT( pl09_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl09_caught_06[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   772, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl09_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl09_caught_07[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   776, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl09_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl09_caught_08[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   781, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_NEXT( pl09_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl09_caught_09[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   790, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl09_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl09_caught_10[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   793, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl09_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl09_caught_11[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   796, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl09_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl09_caught_12[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   815, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_NEXT( pl09_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl09_caught_13[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   843, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl09_caught_14[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl09_caught_15[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   847, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl09_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl09_caught_16[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl09_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl09_caught_17[] = {
	CG_FRAME(    0,    0,    1, END_OF_DATA,   851, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl09_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl09_caught_18[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   853, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl09_caught_19[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   859, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_NEXT( pl09_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl09_caught_20[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_NEXT( pl09_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl09_caught_21[] = {
	CG_FRAME(    0,    0,    3, END_OF_DATA,   826, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl09_caught_21 ),
};

const Sint32 pl09_unused_12[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   843, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   844, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006288,
};

const Sint32 pl09_unused_13[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   845, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   846, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000062b4,
};

const Sint32 pl09_unused_14[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   847, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   848, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000062e0,
};

const Sint32 pl09_unused_15[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   849, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   850, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x0000630c,
};

const Sint32 pl09_unused_16[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2,           0,   851, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   852, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006338,
};

const Sint32 pl09_unused_17[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   837, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   838, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   839, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   841, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006364,
};

const Sint32 pl09_unused_18[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000063b8,
};

const Sint32 pl09_unused_19[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   871, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006434,
};

const Sint32 pl09_unused_20[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   872, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x0000644c,
};

const Sint32 pl09_unused_21[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   873, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006464,
};

const Sint32 pl09_unused_22[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   874, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x0000647c,
};

const Sint32 pl09_unused_23[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   875, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x00006494,
};

const Sint32 pl09_unused_24[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   876, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000064ac,
};

const Sint32 pl09_unused_25[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   877, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000064c4,
};

const Sint32 pl09_unused_26[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   878, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000064dc,
};

const Sint32 pl09_unused_27[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   891, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   892, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   893, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   894, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	0x000064f4,
};

const Sint32 pl09_unused_28[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,   24, END_OF_DATA,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x00006548,
};

const Sint32 pl09_unused_29[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    8,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x0000659c,
};

const Sint32 pl09_unused_30[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    8,           0,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	0x000065f0,
};

/* group 27 0 */
const Sint32 pl09_grp27_00[] = {
	EF_FRAME(    0,    0,    2,           0, 32851 ),
	EF_FRAME(    0,    0,    2,           0, 32852 ),
	EF_FRAME(    0,    0,    2,           0, 32853 ),
};

const Sint32 pl09_grp27_00_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32854 ),
	CG_NEXT( pl09_grp27_00_loop ),
};

/* group 27 1 */
const Sint32 pl09_grp27_01[] = {
	EF_FRAME(    0,    0,    2,           0, 32862 ),
	EF_FRAME(    0,    0,    2,           0, 32863 ),
	EF_FRAME(    0,    0,    2,           0, 32864 ),
};

const Sint32 pl09_grp27_01_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32865 ),
	CG_NEXT( pl09_grp27_01_loop ),
};

/* group 27 2 */
const Sint32 pl09_grp27_02[] = {
	EF_FRAME(    0,    0,    2,           0, 32873 ),
	EF_FRAME(    0,    0,    2,           0, 32874 ),
	EF_FRAME(    0,    0,    2,           0, 32875 ),
};

const Sint32 pl09_grp27_02_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32876 ),
	CG_NEXT( pl09_grp27_02_loop ),
};

/* group 27 3 */
const Sint32 pl09_grp27_03[] = {
	EF_FRAME(    0,    0,    2,           0, 32884 ),
	EF_FRAME(    0,    0,    2,           0, 32885 ),
	EF_FRAME(    0,    0,    2,           0, 32886 ),
};

const Sint32 pl09_grp27_03_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32887 ),
	CG_NEXT( pl09_grp27_03_loop ),
};

/* group 27 4: pls09_05_spark */
const Sint32 pl09_grp27_04[] = {
	EF_FRAME(    0,    0,    2,           0, 32855 ),
	EF_FRAME(    0,    0,    2,           0, 32856 ),
	EF_FRAME(    0,    0,    2,           0, 32857 ),
	EF_FRAME(    0,    0,    2,           0, 32858 ),
	EF_FRAME(    0,    0,    2,           0, 32859 ),
	EF_FRAME(    0,    0,    2,           0, 32860 ),
};

const Sint32 pl09_grp27_04_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32861 ),
	CG_NEXT( pl09_grp27_04_loop ),
};
