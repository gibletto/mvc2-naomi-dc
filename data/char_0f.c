/* Doctor Doom (character 0f): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_0f      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_0f     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl0f.c) says which group
                   and number each move uses.
   pl0f_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl0f_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl0f_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl0f_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_0f
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_0f, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_0f */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl0f_stand_00[], pl0f_stand_02[], pl0f_stand_03[], pl0f_jump_00[], pl0f_win_07[],
	pl0f_jump_01[], pl0f_jump_01_loop[], pl0f_jump_02[], pl0f_jump_02_loop[], pl0f_jump_03[],
	pl0f_jump_04[], pl0f_jump_05[], pl0f_jump_06[], pl0f_jump_09[], pl0f_dash_00[], pl0f_dash_02[],
	pl0f_dash_01[], pl0f_dash_03[], pl0f_crouch_00[], pl0f_crouch_01[], pl0f_crouch_02[],
	pl0f_turn_00[], pl0f_turn_01[], pl0f_guard_00[], pl0f_guard_01[], pl0f_guard_02[],
	pl0f_guard_03[], pl0f_guard_04[], pl0f_guard_05[], pl0f_guard_06[], pl0f_guard_07[],
	pl0f_air_guard_00[], pl0f_air_guard_01[], pl0f_stand_p_00[], pl0f_stand_p_01[],
	pl0f_special_06[], pl0f_stand_p_02[], pl0f_stand_k_00[], pl0f_stand_k_01[], pl0f_extra_07[],
	pl0f_stand_k_02[], pl0f_crouch_p_00[], pl0f_crouch_p_01[], pl0f_crouch_p_02[], pl0f_extra_06[],
	pl0f_crouch_k_00[], pl0f_crouch_k_01[], pl0f_crouch_k_02[], pl0f_jump_p_00[], pl0f_jump_p_01[],
	pl0f_jump_p_02[], pl0f_jump_k_00[], pl0f_jump_k_01[], pl0f_jump_k_02[], pl0f_jump_k_02_loop[],
	pl0f_extra_05[], pl0f_catch_00[], pl0f_catch_00_loop[], pl0f_catch_01[], pl0f_catch_02[],
	pl0f_catch_03[], pl0f_catch_04[], pl0f_catch_04_loop[], pl0f_catch_05[], pl0f_extra_03[],
	pl0f_extra_03_loop[], pl0f_extra_04[], pl0f_get_up_00[], pl0f_get_up_01[], pl0f_get_up_02[],
	pl0f_get_up_03[], pl0f_change_00[], pl0f_change_00_loop[], pl0f_change_02[],
	pl0f_change_02_loop[], pl0f_extra_00[], pl0f_extra_01[], pl0f_change_03[], pl0f_special_00[],
	pl0f_special_01[], pl0f_special_01_loop[], pl0f_special_02[], pl0f_special_03[],
	pl0f_special_04[], pl0f_special_05[], pl0f_special_05_loop[], pl0f_super_00[],
	pl0f_super_00_loop[], pl0f_super_07[], pl0f_super_01[], pl0f_super_01_loop[], pl0f_super_08[],
	pl0f_super_02[], pl0f_super_02_loop[], pl0f_super_03[], pl0f_super_03_loop[], pl0f_super_04[],
	pl0f_super_05[], pl0f_super_05_loop[], pl0f_super_06[], pl0f_damage_00[], pl0f_damage_01[],
	pl0f_damage_02[], pl0f_damage_03[], pl0f_damage_04[], pl0f_damage_05[], pl0f_damage_06[],
	pl0f_damage_07[], pl0f_damage_08[], pl0f_damage_10[], pl0f_damage_11[], pl0f_damage_12[],
	pl0f_damage_13[], pl0f_damage_13_loop[], pl0f_damage_09[], pl0f_damage_09_loop[],
	pl0f_damage_16[], pl0f_damage_17[], pl0f_damage_18[], pl0f_damage_19[], pl0f_damage_20[],
	pl0f_damage_21[], pl0f_damage_22[], pl0f_damage_23[], pl0f_damage_24[], pl0f_damage_25[],
	pl0f_damage_26[], pl0f_damage_27[], pl0f_damage_30[], pl0f_damage_31[], pl0f_damage_32[],
	pl0f_damage_28[], pl0f_damage_29[], pl0f_caught_00[], pl0f_caught_01[], pl0f_caught_02[],
	pl0f_caught_03[], pl0f_caught_04[], pl0f_caught_05[], pl0f_caught_06[], pl0f_caught_07[],
	pl0f_caught_08[], pl0f_caught_09[], pl0f_caught_10[], pl0f_caught_11[], pl0f_caught_12[],
	pl0f_caught_13[], pl0f_caught_14[], pl0f_caught_15[], pl0f_caught_16[], pl0f_caught_17[],
	pl0f_caught_18[], pl0f_caught_19[], pl0f_caught_20[], pl0f_caught_21[], pl0f_dizzy_00[],
	pl0f_win_00[], pl0f_win_03[], pl0f_win_04[], pl0f_win_05[], pl0f_win_06[], pl0f_win_01[],
	pl0f_win_01_loop[], pl0f_grp27_00[], pl0f_win_02[], pl0f_win_02_loop[], pl0f_appear_00[],
	pl0f_appear_00_loop[], pl0f_appear_01[], pl0f_appear_01_loop[], pl0f_extra_02[],
	pl0f_change_04[], pl0f_effect_04[], pl0f_effect_05[], pl0f_effect_06[], pl0f_effect_07[],
	pl0f_effect_08[], pl0f_effect_09[], pl0f_effect_10[], pl0f_effect_11[], pl0f_effect_12[],
	pl0f_effect_13[], pl0f_effect_14[], pl0f_effect_15[], pl0f_effect_24[], pl0f_effect_16[],
	pl0f_effect_23[], pl0f_effect_17[], pl0f_effect_00[], pl0f_effect_01[], pl0f_effect_02[],
	pl0f_effect_03[], pl0f_effect_03_loop[], pl0f_effect_20[], pl0f_effect_21[], pl0f_effect_18[],
	pl0f_effect_19[], pl0f_effect_22[], pl0f_fly_00[], pl0f_fly_01[], pl0f_fly_02[],
	pl0f_fly_02_loop[], pl0f_fly_10[], pl0f_fly_07[], pl0f_fly_07_loop[], pl0f_fly_15[],
	pl0f_fly_03[], pl0f_fly_03_loop[], pl0f_fly_11[];
extern const Sint32 *const pl0f_stand_tbl[], *const pl0f_jump_tbl[], *const pl0f_dash_tbl[],
	*const pl0f_crouch_tbl[], *const pl0f_turn_tbl[], *const pl0f_guard_tbl[],
	*const pl0f_air_guard_tbl[], *const pl0f_stand_p_tbl[], *const pl0f_stand_k_tbl[],
	*const pl0f_crouch_p_tbl[], *const pl0f_crouch_k_tbl[], *const pl0f_jump_p_tbl[],
	*const pl0f_jump_k_tbl[], *const pl0f_damage_tbl[], *const pl0f_dizzy_tbl[],
	*const pl0f_catch_tbl[], *const pl0f_caught_tbl[], *const pl0f_get_up_tbl[],
	*const pl0f_appear_tbl[], *const pl0f_win_tbl[], *const pl0f_extra_tbl[],
	*const pl0f_special_tbl[], *const pl0f_super_tbl[], *const pl0f_effect_tbl[],
	*const pl0f_change_tbl[], *const pl0f_fly_tbl[], *const pl0f_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_0f[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 1, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 1, 0, 0 },
	{ 1000, 1000, 9728, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -7168, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_0f[] = {
	pl0f_stand_tbl, pl0f_jump_tbl, pl0f_dash_tbl, pl0f_crouch_tbl, pl0f_turn_tbl, pl0f_guard_tbl,
	pl0f_air_guard_tbl, pl0f_stand_p_tbl, pl0f_stand_k_tbl, pl0f_crouch_p_tbl, pl0f_crouch_k_tbl, pl0f_jump_p_tbl,
	pl0f_jump_k_tbl, pl0f_damage_tbl, pl0f_dizzy_tbl, pl0f_catch_tbl, pl0f_caught_tbl, pl0f_get_up_tbl,
	pl0f_appear_tbl, pl0f_win_tbl, pl0f_extra_tbl, pl0f_special_tbl, pl0f_super_tbl, pl0f_effect_tbl,
	pl0f_change_tbl, pl0f_change_tbl, pl0f_fly_tbl, pl0f_grp27_tbl, 0,
};

const Sint32 *const pl0f_stand_tbl[] = {
	pl0f_stand_00, pl0f_stand_00, pl0f_stand_02, pl0f_stand_03, pl0f_stand_00, pl0f_stand_00,
	pl0f_stand_02, pl0f_stand_03, 0,
};

const Sint32 *const pl0f_jump_tbl[] = {
	pl0f_jump_00, pl0f_jump_01, pl0f_jump_02, pl0f_jump_03, pl0f_jump_04, pl0f_jump_05,
	pl0f_jump_06, pl0f_jump_06, pl0f_jump_06, pl0f_jump_09, pl0f_jump_09, pl0f_jump_09,
	0,
};

const Sint32 *const pl0f_dash_tbl[] = {
	pl0f_dash_00, pl0f_dash_01, pl0f_dash_02, pl0f_dash_03, 0,
};

const Sint32 *const pl0f_crouch_tbl[] = {
	pl0f_crouch_00, pl0f_crouch_01, pl0f_crouch_02, 0,
};

const Sint32 *const pl0f_turn_tbl[] = {
	pl0f_turn_00, pl0f_turn_01, 0,
};

const Sint32 *const pl0f_guard_tbl[] = {
	pl0f_guard_00, pl0f_guard_01, pl0f_guard_02, pl0f_guard_03, pl0f_guard_04, pl0f_guard_05,
	pl0f_guard_06, pl0f_guard_07, 0,
};

const Sint32 *const pl0f_air_guard_tbl[] = {
	pl0f_air_guard_00, pl0f_air_guard_01, pl0f_air_guard_01, 0,
};

const Sint32 *const pl0f_stand_p_tbl[] = {
	pl0f_stand_p_00, pl0f_stand_p_01, pl0f_stand_p_02, 0,
};

const Sint32 *const pl0f_stand_k_tbl[] = {
	pl0f_stand_k_00, pl0f_stand_k_01, pl0f_stand_k_02, 0,
};

const Sint32 *const pl0f_crouch_p_tbl[] = {
	pl0f_crouch_p_00, pl0f_crouch_p_01, pl0f_crouch_p_02, 0,
};

const Sint32 *const pl0f_crouch_k_tbl[] = {
	pl0f_crouch_k_00, pl0f_crouch_k_01, pl0f_crouch_k_02, 0,
};

const Sint32 *const pl0f_jump_p_tbl[] = {
	pl0f_jump_p_00, pl0f_jump_p_01, pl0f_jump_p_02, pl0f_jump_p_00, pl0f_jump_p_01, pl0f_jump_p_02,
	0,
};

const Sint32 *const pl0f_jump_k_tbl[] = {
	pl0f_jump_k_00, pl0f_jump_k_01, pl0f_jump_k_02, pl0f_jump_k_00, pl0f_jump_k_01, pl0f_jump_k_02,
	0,
};

const Sint32 *const pl0f_damage_tbl[] = {
	pl0f_damage_00, pl0f_damage_01, pl0f_damage_02, pl0f_damage_03, pl0f_damage_04, pl0f_damage_05,
	pl0f_damage_06, pl0f_damage_07, pl0f_damage_08, pl0f_damage_09, pl0f_damage_10, pl0f_damage_11,
	pl0f_damage_12, pl0f_damage_13, pl0f_damage_13, pl0f_damage_13, pl0f_damage_16, pl0f_damage_17,
	pl0f_damage_18, pl0f_damage_19, pl0f_damage_20, pl0f_damage_21, pl0f_damage_22, pl0f_damage_23,
	pl0f_damage_24, pl0f_damage_25, pl0f_damage_26, pl0f_damage_27, pl0f_damage_28, pl0f_damage_29,
	pl0f_damage_30, pl0f_damage_31, pl0f_damage_32, pl0f_damage_27, pl0f_damage_13, 0,
};

const Sint32 *const pl0f_dizzy_tbl[] = {
	pl0f_dizzy_00, 0,
};

const Sint32 *const pl0f_catch_tbl[] = {
	pl0f_catch_00, pl0f_catch_01, pl0f_catch_02, pl0f_catch_03, pl0f_catch_04, pl0f_catch_05,
	0,
};

const Sint32 *const pl0f_caught_tbl[] = {
	pl0f_caught_00, pl0f_caught_01, pl0f_caught_02, pl0f_caught_03, pl0f_caught_04, pl0f_caught_05,
	pl0f_caught_06, pl0f_caught_07, pl0f_caught_08, pl0f_caught_09, pl0f_caught_10, pl0f_caught_11,
	pl0f_caught_12, pl0f_caught_13, pl0f_caught_14, pl0f_caught_15, pl0f_caught_16, pl0f_caught_17,
	pl0f_caught_18, pl0f_caught_19, pl0f_caught_20, pl0f_caught_21, 0,
};

const Sint32 *const pl0f_get_up_tbl[] = {
	pl0f_get_up_00, pl0f_get_up_01, pl0f_get_up_02, pl0f_get_up_03, 0,
};

const Sint32 *const pl0f_appear_tbl[] = {
	pl0f_appear_00, pl0f_appear_01, 0,
};

const Sint32 *const pl0f_win_tbl[] = {
	pl0f_win_00, pl0f_win_01, pl0f_win_02, pl0f_win_03, pl0f_win_04, pl0f_win_05,
	pl0f_win_06, pl0f_win_07, 0,
};

const Sint32 *const pl0f_extra_tbl[] = {
	pl0f_extra_00, pl0f_extra_01, pl0f_extra_02, pl0f_extra_03, pl0f_extra_04, pl0f_extra_05,
	pl0f_extra_06, pl0f_extra_07, 0,
};

const Sint32 *const pl0f_special_tbl[] = {
	pl0f_special_00, pl0f_special_01, pl0f_special_02, pl0f_special_03, pl0f_special_04, pl0f_special_05,
	pl0f_special_06, 0,
};

const Sint32 *const pl0f_super_tbl[] = {
	pl0f_super_00, pl0f_super_01, pl0f_super_02, pl0f_super_03, pl0f_super_04, pl0f_super_05,
	pl0f_super_06, pl0f_super_07, pl0f_super_08, 0,
};

const Sint32 *const pl0f_effect_tbl[] = {
	pl0f_effect_00, pl0f_effect_01, pl0f_effect_02, pl0f_effect_03, pl0f_effect_04, pl0f_effect_05,
	pl0f_effect_06, pl0f_effect_07, pl0f_effect_08, pl0f_effect_09, pl0f_effect_10, pl0f_effect_11,
	pl0f_effect_12, pl0f_effect_13, pl0f_effect_14, pl0f_effect_15, pl0f_effect_16, pl0f_effect_17,
	pl0f_effect_18, pl0f_effect_19, pl0f_effect_20, pl0f_effect_21, pl0f_effect_22, pl0f_effect_23,
	pl0f_effect_24, 0,
};

const Sint32 *const pl0f_change_tbl[] = {
	pl0f_change_00, pl0f_change_00, pl0f_change_02, pl0f_change_03, pl0f_change_04, 0,
};

const Sint32 *const pl0f_fly_tbl[] = {
	pl0f_fly_00, pl0f_fly_01, pl0f_fly_02, pl0f_fly_03, pl0f_fly_03, pl0f_fly_03,
	pl0f_fly_02, pl0f_fly_07, pl0f_fly_07, pl0f_fly_07, pl0f_fly_10, pl0f_fly_11,
	pl0f_fly_11, pl0f_fly_11, pl0f_fly_10, pl0f_fly_15, pl0f_fly_15, pl0f_fly_15,
	0,
};

const Sint32 *const pl0f_grp27_tbl[] = {
	pl0f_grp27_00, 0,
};

	/*         prm  type  ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: act_stand, appear_move, act_change_gattai_move, act_change_zero_move, ...
   Char_Stand 1
   Char_Stand 4
   Char_Stand 5 */
const Sint32 pl0f_stand_00[] = {
	CG_FRAME(    0,    0,   5,           0,   104, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_stand_00 ),
};

/* Char_Stand 2: win_ko_init, act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl0f_stand_02[] = {
	CG_FRAME(    0,    0,   8,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_stand_02 ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl0f_stand_03[] = {
	CG_FRAME(    0,    0,   8,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_stand_03 ),
};

/* Char_Jump 0 */
const Sint32 pl0f_jump_00[] = {
	CG_FRAME(    0,    1,   4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   8,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Win 7: pls0f_00_c_init */
const Sint32 pl0f_win_07[] = {
	CG_FRAME(    0,    0,   3,           0,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_win_07 ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl0f_jump_01[] = {
	CG_FRAME(    0,    1,   4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0f_jump_01_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl0f_jump_02[] = {
	CG_FRAME(    0,    1,   4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0f_jump_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_jump_02_loop ),
};

/* Char_Jump 3 */
const Sint32 pl0f_jump_03[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_jump_03 ),
};

/* Char_Jump 4 */
const Sint32 pl0f_jump_04[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_jump_04 ),
};

/* Char_Jump 5 */
const Sint32 pl0f_jump_05[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_jump_04 ),
};

/* Char_Jump 6
   Char_Jump 7
   Char_Jump 8 */
const Sint32 pl0f_jump_06[] = {
	CG_FRAME(    0,    1,   4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   209, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   210, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   213, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   214, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   215, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   4,           0,   216, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   205, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   6,           0,   206, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl0f_jump_09[] = {
	CG_FRAME(    0,    0,   2,           0,   207, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   208, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl0f_dash_00[] = {
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   136, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl0f_dash_00 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl0f_dash_02[] = {
	CG_FRAME(    0,    0,   3,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_dash_02 ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl0f_dash_01[] = {
	CG_FRAME(    0,    0,   3,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   148, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3,           0,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl0f_dash_01 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl0f_dash_03[] = {
	CG_FRAME(    0,    0,   3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl0f_crouch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl0f_crouch_01[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl0f_crouch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   752, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl0f_turn_00[] = {
	CG_FRAME(    0,    0,   2,           0,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl0f_turn_01[] = {
	CG_FRAME(    0,    0,   2,           0,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl0f_guard_00[] = {
	CG_FRAME(    0,    0,   2,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl0f_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl0f_guard_02[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl0f_guard_03[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_guard_03 ),
};

/* Char_Guard 4 */
const Sint32 pl0f_guard_04[] = {
	CG_FRAME(    0,    0,   2,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl0f_guard_05[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   199, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl0f_guard_06[] = {
	CG_FRAME(    0,    0,   2,           0,   192, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl0f_guard_07[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   199, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_guard_07 ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl0f_air_guard_00[] = {
	CG_FRAME(    0,    0,   1,           0,   200, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Air_Guard 1: act_air_guard_start
   Char_Air_Guard 2: guard_char_set */
const Sint32 pl0f_air_guard_01[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   201, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_air_guard_01 ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl0f_stand_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   383, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   384, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,   2,           0,   385, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   386, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl0f_stand_p_01[] = {
	CG_FRAME(    0,    0,   2,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   8,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   395, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_stand_p_01 ),
};

/* Char_Special 6: snapback_init (Snapback) */
const Sint32 pl0f_special_06[] = {
	CG_FRAME(    0,    0,   8,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    1,   1,           0,   387, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   388, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   389, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   390, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   2,           0,   391, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   8,           0,   392, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,   4,           0,   393, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,   4,           0,   394, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,   4,           0,   395, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_special_06 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl0f_stand_p_02[] = {
	CG_FRAME(    0,    0,   2,           0,   396, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   397, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   398, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   399, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   400, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   2,           0,   401, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  21 ),
	CG_FRAME(    0,    0,   4,           0,   402, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  22 ),
	CG_FRAME(    0,    0,   4,           0,   403, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   404, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   405, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   406, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,   4,           0,   407, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl0f_stand_k_00[] = {
	CG_FRAME(    0,    0,   3,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   410, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,   2,           0,   411, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,   2,           0,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl0f_stand_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   753, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   414, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,  16,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6,           0,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   753, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_stand_k_01 ),
};

/* Char_Extra 7: pl0f_upper */
const Sint32 pl0f_extra_07[] = {
	CG_FRAME(    0,    0,   6,           0,   753, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   414, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,   4,           0,   415, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,  16,           0,   415, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   6,           0,   414, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   753, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_extra_07 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl0f_stand_k_02[] = {
	CG_FRAME(    0,    0,   2,           0,   408, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   416, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   417, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   418, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   419, 0x80,   0x00,  ATK,  0x00, 0x16 | CSE,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,   2,           0,   409, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    1,   4,           0,   410, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,  10,           0,   411, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   6,           0,   412, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   413, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl0f_crouch_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   420, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   421, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   423, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl0f_crouch_p_01[] = {
	CG_FRAME(    0,    0,   6,           0,   420, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   421, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,   2,           0,   422, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  35 ),
	CG_FRAME(    0,    0,   6,           0,   423, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   186, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl0f_crouch_p_02[] = {
	CG_FRAME(    0,    0,   1,           0,   424, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   430, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   431, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   4,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,   435, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   424, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_crouch_p_02 ),
};

/* Char_Extra 6: pl0f_upper */
const Sint32 pl0f_extra_06[] = {
	CG_FRAME(    0,    0,   1,           0,   424, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   425, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   426, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   427, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   428, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   429, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   430, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   431, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   1,           0,   432, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   433, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,   4,           0,   434, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,   435, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   4,           0,   436, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   437, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   438, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   439, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   425, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   424, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_extra_06 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl0f_crouch_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   440, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   441, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,  ATE,  0x00,       0x03,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,   2,           0,   440, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,   4,           0,   442, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl0f_crouch_k_01[] = {
	CG_FRAME(    0,    0,   6,           0,   444, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   441, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   3,           0,   445, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,   6,           0,   440, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   442, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   443, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   187, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl0f_crouch_k_02[] = {
	CG_FRAME(    0,    0,   4,           0,   446, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   447, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   2,           0,   448, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   449, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,   8,           0,   450, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   3,           0,   451, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   3,           0,   452, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   3,           0,   453, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4,           0,   454, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   455, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl0f_crouch_k_02 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl0f_jump_p_00[] = {
	CG_FRAME(    0,    0,   4,           0,   456, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  16,           0,   457, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,   4,           0,   458, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   459, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl0f_jump_p_01[] = {
	CG_FRAME(    0,    0,   4,           0,   460, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   461, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   462, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   469, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   2,           0,   463, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,   4,           0,   470, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   464, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   465, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   466, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,   4,           0,   467, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   468, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl0f_jump_p_02[] = {
	CG_FRAME(    0,    0,   4,           0,   471, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   472, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  12,           0,   473, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   2,           0,   474, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    1,  18,           0,   475, 0x80,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    2,  12,           0,   477, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,   6,           0,   478, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   459, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_jump_p_02 ),
};

/* Char_Jump_K 0 (light): jump_kick
   Char_Jump_K 3 */
const Sint32 pl0f_jump_k_00[] = {
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,  12,           0,   480, 0x80,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,   4,           0,   479, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  50 ),
	CG_FRAME(    0,    0,   3,           0,   481, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   482, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_jump_k_00 ),
};

/* Char_Jump_K 1 (medium): jump_kick
   Char_Jump_K 4 */
const Sint32 pl0f_jump_k_01[] = {
	CG_FRAME(    0,    0,   2,           0,   483, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   485, 0x80,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,   8,           0,   485, 0x80,   0x00,  ATE,  0x00,       0x04,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4,           0,   484, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   483, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy): jump_kick
   Char_Jump_K 5 */
const Sint32 pl0f_jump_k_02[] = {
	CG_FRAME(    0,    0,  12,           0,   486, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   487, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    1,   4,           0,   488, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0,      0,  72 ),
};

const Sint32 pl0f_jump_k_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   489, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   490, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_NEXT( pl0f_jump_k_02_loop ),
};

/* Char_Extra 5: dive_move */
const Sint32 pl0f_extra_05[] = {
	CG_FRAME(    0,    0,   4,           0,   491, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   492, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   493, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   494, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   495, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_extra_05 ),
};

/* Char_Catch 0: nml_p_set */
const Sint32 pl0f_catch_00[] = {
	CG_FRAME(    0,    0,   4,           0,   496, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0001,   0 ),
	CG_FRAME(    0,    0,   4,           0,   497, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   498, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0002,   0 ),
	CG_FRAME(    0,    0,   4,           0,   499, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0004,   0 ),
	CG_FRAME(    0,    0,   4,           0,   500, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0005,   0 ),
	CG_FRAME(    0,    0,   4,           0,   501, 0x80,   0x00,    0,  0x00,       0x06,    0,   0,     0, 0x0006,   0 ),
};

const Sint32 pl0f_catch_00_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   502, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   503, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_catch_00_loop ),
};

/* Char_Catch 1: nml_p_wait */
const Sint32 pl0f_catch_01[] = {
	CG_FRAME(    0,    0,   4,           0,   504, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   505, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   506, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_catch_01 ),
};

/* Char_Catch 2: nml_k_set */
const Sint32 pl0f_catch_02[] = {
	CG_FRAME(    0,    0,   4,           0,   748, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0011,   0 ),
	CG_FRAME(    0,    0,   4,           0,   749, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0012,   0 ),
	CG_FRAME(    0,    0,   4,           0,   750, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013,   0 ),
	CG_FRAME(    0,    0,   4,           0,   751, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0014,   0 ),
	CG_FRAME(    0,    0,   4,           0,   507, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015,   0 ),
	CG_FRAME(    0,    0,   4,           0,   507, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0017,   0 ),
	CG_FRAME(    0,    0,   4,           0,   508, 0x80,   0x00,    0,  0x00,       0x06,    0,   0,     0, 0x0016,   0 ),
	CG_FRAME(    0,    1,  12,           0,   508, 0x80,   0x00,    0,  0x00,       0x06,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   186, 0x00,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_catch_02 ),
};

/* Char_Catch 3: jump_k_set */
const Sint32 pl0f_catch_03[] = {
	CG_FRAME(    0,    0,   4,           0,   509, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0007,   0 ),
	CG_FRAME(    0,    0,   4,           0,   510, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0008,   0 ),
	CG_FRAME(    0,    0,   4,           0,   511, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0009,   0 ),
	CG_FRAME(    0,    0,   4,           0,   512, 0x80,   0x00,    0,  0x00,       0x06,    0,   0,     0, 0x000a,   0 ),
	CG_FRAME(    0,    1,   4,           0,   458, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   459, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_catch_03 ),
};

/* Char_Catch 4: jump_p_set */
const Sint32 pl0f_catch_04[] = {
	CG_FRAME(    0,    0,   4,           0,   513, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000b,   0 ),
	CG_FRAME(    0,    0,   4,           0,   514, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   515, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   516, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000c,   0 ),
	CG_FRAME(    0,    0,   4,           0,   517, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f,   0 ),
	CG_FRAME(    0,    0,   4,           0,   518, 0x80,   0x00,    0,  0x00,       0x06,    0,   0,     0, 0x0010,   0 ),
};

const Sint32 pl0f_catch_04_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   519, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   520, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_catch_04_loop ),
};

/* Char_Catch 5: jump_p_wait */
const Sint32 pl0f_catch_05[] = {
	CG_FRAME(    0,    0,   4,           0,   521, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   522, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   523, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_catch_05 ),
};

/* Char_Extra 3: jump_kick, fly_start5 */
const Sint32 pl0f_extra_03[] = {
	CG_FRAME(    0,    0,  12,           0,   524, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   525, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    1,   4,           0,   526, 0x80,   0x00,  ATK,  0x00,       0x24,    0,   0,     0,      0,  69 ),
};

const Sint32 pl0f_extra_03_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   527, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   528, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_NEXT( pl0f_extra_03_loop ),
};

/* Char_Extra 4: dive_move */
const Sint32 pl0f_extra_04[] = {
	CG_FRAME(    0,    0,   2,           0,   529, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   530, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   531, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   532, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   534, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8,           0,   535, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   8, END_OF_DATA,   536, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_extra_04 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl0f_get_up_00[] = {
	CG_FRAME(    0,    0,   3,           0,   217, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   218, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   219, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   230, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl0f_get_up_01[] = {
	CG_FRAME(    0,    0,   2,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   245, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   237, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   239, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   241, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   243, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl0f_get_up_02[] = {
	CG_FRAME(    0,    0,   3,           0,   231, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   245, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   247, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   248, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   249, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   234, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   235, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   236, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   237, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   238, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   239, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   240, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   241, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   242, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   243, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   244, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl0f_get_up_03[] = {
	CG_FRAME(    0,    0,   2,           0,   250, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   260, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   261, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1,           0,   262, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_get_up_03 ),
};

/* Char_Change 0: act_change_tag_init
   Char_Change 1: act_change_vcounter_init
   group 25 0
   group 25 1 */
const Sint32 pl0f_change_00[] = {
	CG_FRAME(    0,    0,   4,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   270, 0x80,   0x00,    0,  0x00,       0x04,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0f_change_00_loop[] = {
	CG_FRAME(    0,    1,   3, END_OF_DATA,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_change_00_loop ),
};

/* Char_Change 2: act_change_out_bound
   group 25 2 */
const Sint32 pl0f_change_02[] = {
	CG_FRAME(    0,    0,   3,           0,   743, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   744, 0x80,   0x00,    0,  0x00,       0x24,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   745, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0,      0,   0 ),
};

const Sint32 pl0f_change_02_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   746, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   747, 0x80,   0x00,    0,  0x00,          0,    1,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_change_02_loop ),
};

/* Char_Extra 0: t_kick_init */
const Sint32 pl0f_extra_00[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   480, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl0f_extra_00 ),
};

/* Char_Extra 1: t_kick_jump */
const Sint32 pl0f_extra_01[] = {
	CG_FRAME(    0,    0,   4,           0,   276, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   277, 0x80,   0x00,  ATE,  0x00,       0x05,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   278, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   279, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   280, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   281, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_extra_01 ),
};

/* Char_Change 3: act_stand, act_change_call
   group 25 3 */
const Sint32 pl0f_change_03[] = {
	CG_FRAME(    0,    0,   4,           0,   282, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   283, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   284, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   285, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   286, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   287, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   288, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_change_03 ),
};

/* Char_Special 0: plasma_gnd_init (Plasma Beam) */
const Sint32 pl0f_special_00[] = {
	CG_FRAME(    0,    0,   2,           0,   555, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   2,           0,   556, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME( 0x80,    0,   2,           0,   557, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   558, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   559, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   560, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   561, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    1,   4,           0,   572, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   573, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   574, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   575, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   576, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   577, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   578, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   2,           0,   569, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   570, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   571, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME( 0x80,    0,   4,           0,   752, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_special_00 ),
};

/* Char_Special 1: plasma_air_init (Plasma Beam) */
const Sint32 pl0f_special_01[] = {
	CG_FRAME(    0,    0,  12,           0,   603, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   604, 0x80,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   605, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   4,           0,   606, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    1,   2,           0,   607, 0x80,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,  58 ),
};

const Sint32 pl0f_special_01_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   608, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   609, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl0f_special_01_loop ),
};

/* Char_Special 2: plasma_air_move (Plasma Beam) */
const Sint32 pl0f_special_02[] = {
	CG_FRAME(    0,    0,   4,           0,   610, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   611, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   612, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4,           0,   613, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   614, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_special_02 ),
};

/* Char_Special 3: photon_gnd_init (Photon Shot) */
const Sint32 pl0f_special_03[] = {
	CG_FRAME(    0,    0,   4,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   539, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   541, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   542, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   543, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    1,   4,           0,   544, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   4,           0,   545, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   546, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   548, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   549, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   550, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   551, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   552, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   553, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   554, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_special_03 ),
};

/* Char_Special 4: photon_air_init (Photon Shot) */
const Sint32 pl0f_special_04[] = {
	CG_FRAME(    0,    0,  14,           0,   579, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   5,           0,   580, 0x80,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   581, 0x80,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    1,   4,           0,   582, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    2,   4,           0,   586, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   588, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   6,           0,   589, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   590, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_special_04 ),
};

/* Char_Special 5: shield_init (Molecular Shield) */
const Sint32 pl0f_special_05[] = {
	CG_FRAME(    0,    0,   4,           0,   591, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   592, 0x80,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   593, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   594, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   595, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   596, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   597, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   598, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   599, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
};

const Sint32 pl0f_special_05_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   600, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4,           0,   601, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   602, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl0f_special_05_loop ),
};

/* Char_Super 0: array_gnd_init (Photon Array) */
const Sint32 pl0f_super_00[] = {
	CG_FRAME(    0,    0,   4,           0,   537, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   538, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   539, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   540, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   541, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   542, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   543, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
};

const Sint32 pl0f_super_00_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   544, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   2,           0,   545, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   546, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   547, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   548, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   549, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   550, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2,           0,   551, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   552, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl0f_super_00_loop ),
};

/* Char_Super 7: array_gnd_move (Photon Array) */
const Sint32 pl0f_super_07[] = {
	CG_FRAME(    0,    0,   4,           0,   553, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   554, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,   4,           0,   396, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4,           0,   106, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_super_07 ),
};

/* Char_Super 1: array_air_init (Photon Array) */
const Sint32 pl0f_super_01[] = {
	CG_FRAME(    0,    0,   6,           0,   579, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
};

const Sint32 pl0f_super_01_loop[] = {
	CG_FRAME(    0,    1,   2,           0,   580, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   581, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2,           0,   582, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   583, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_NEXT( pl0f_super_01_loop ),
};

/* Char_Super 8: array_air_move (Photon Array) */
const Sint32 pl0f_super_08[] = {
	CG_FRAME(    0,    0,   4,           0,   584, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   585, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   586, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   587, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   588, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,   4,           0,   589, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   590, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_super_08 ),
};

/* Char_Super 2: cage_init (Electric Cage) */
const Sint32 pl0f_super_02[] = {
	CG_FRAME(    0,    0,  12,           0,   615, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    1,   8,           0,   616, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   6,           0,   617, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    2,   4,           0,   618, 0x80,   0x00,  ATK,  0x00,       0x23,    0,   0,     0,      0,  62 ),
};

const Sint32 pl0f_super_02_loop[] = {
	CG_FRAME(    0,    0,   3,           0,   619, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   620, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl0f_super_02_loop ),
};

/* Char_Super 3: cage_move (Electric Cage) */
const Sint32 pl0f_super_03[] = {
	CG_FRAME(    0,    0,   4,           0,   621, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
};

const Sint32 pl0f_super_03_loop[] = {
	CG_FRAME( 0x80,    0,   3,           0,   622, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME( 0x82,    0,   3, END_OF_DATA,   623, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_NEXT( pl0f_super_03_loop ),
};

/* Char_Super 4: cage_hold (Electric Cage) */
const Sint32 pl0f_super_04[] = {
	CG_FRAME(    0,    0,   8,           0,   624, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   8,           0,   625, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   104, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_super_04 ),
};

/* Char_Super 5: flame_init (Sphere Flame) */
const Sint32 pl0f_super_05[] = {
	CG_FRAME(    0,    0,   4,           0,   626, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   4,           0,   627, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    1,   3,           0,   626, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x82,    0,   3,           0,   627, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,   3,           0,   628, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME( 0x80,    0,   3,           0,   629, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME( 0x80,    0,   2,           0,   630, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    2,   4,           0,   632, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
};

const Sint32 pl0f_super_05_loop[] = {
	CG_FRAME(    0,    0,   4,           0,   631, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   632, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_NEXT( pl0f_super_05_loop ),
};

/* Char_Super 6: flame_move (Sphere Flame) */
const Sint32 pl0f_super_06[] = {
	CG_FRAME(    0,    0,   6,           0,   633, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  65 ),
	CG_FRAME(    0,    0,   6,           0,   634, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6,           0,   635, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   105, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl0f_super_06 ),
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl0f_damage_00[] = {
	CG_FRAME(    0,    0,   9,           0,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl0f_damage_01[] = {
	CG_FRAME(    0,    0,  16,           0,   638, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl0f_damage_02[] = {
	CG_FRAME(    0,    0,  16,           0,   639, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4,           0,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl0f_damage_03[] = {
	CG_FRAME(    0,    0,   9,           0,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_damage_03 ),
};

/* Char_Damage 4 */
const Sint32 pl0f_damage_04[] = {
	CG_FRAME(    0,    0,  14,           0,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_damage_04 ),
};

/* Char_Damage 5 */
const Sint32 pl0f_damage_05[] = {
	CG_FRAME( 0x81,    0,  16,           0,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME( 0x81,    0,   4,           0,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   640, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_damage_05 ),
};

/* Char_Damage 6 */
const Sint32 pl0f_damage_06[] = {
	CG_FRAME(    0,    0,   7,           0,   644, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl0f_damage_07[] = {
	CG_FRAME(    0,    0,  14,           0,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl0f_damage_08[] = {
	CG_FRAME(    0,    0,  20,           0,   646, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl0f_damage_08 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl0f_damage_10[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_10 ),
};

/* Char_Damage 11 */
const Sint32 pl0f_damage_11[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_11 ),
};

/* Char_Damage 12 */
const Sint32 pl0f_damage_12[] = {
	CG_FRAME(    0,    0,  10, END_OF_DATA,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_12 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl0f_damage_13[] = {
	CG_FRAME(    0,    0,   2,           0,   647, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   648, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   2,           0,   649, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl0f_damage_13_loop[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl0f_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl0f_damage_09[] = {
	CG_FRAME(    0,    0,   4,           0,   651, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   652, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   653, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   654, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   655, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   656, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   657, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl0f_damage_09_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   658, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl0f_damage_09_loop ),
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl0f_damage_16[] = {
	CG_FRAME(    0,    0,   2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl0f_damage_17[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl0f_damage_18[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl0f_damage_19[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl0f_damage_20[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl0f_damage_21[] = {
	CG_FRAME(    0,    0,   2,           0,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   660, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 22 */
const Sint32 pl0f_damage_22[] = {
	CG_FRAME(    0,    0,   2,           0,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 23 */
const Sint32 pl0f_damage_23[] = {
	CG_FRAME(    0,    0,   2,           0,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 24 */
const Sint32 pl0f_damage_24[] = {
	CG_FRAME(    0,    0,   2,           0,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

/* Char_Damage 25 */
const Sint32 pl0f_damage_25[] = {
	CG_FRAME(    0,    0,   2, END_OF_DATA,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_25 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl0f_damage_26[] = {
	CG_FRAME(    0,    0,   4,           0,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4,           0,   666, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   665, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl0f_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 33: act_escape */
const Sint32 pl0f_damage_27[] = {
	CG_FRAME(    0,    0,   3,           0,   702, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_27 ),
};

/* Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl0f_damage_30[] = {
	CG_FRAME(    0,    0,   3,           0,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   673, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_damage_27 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl0f_damage_31[] = {
	CG_FRAME(    0,    0,   8, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl0f_damage_31 ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl0f_damage_32[] = {
	CG_FRAME(    0,    0,   3,           0,   667, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   668, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   669, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   670, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   671, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   672, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   673, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   674, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   675, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   676, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   677, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   678, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   679, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   680, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_damage_27 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl0f_damage_28[] = {
	CG_FRAME(    0,    0,   2,           0,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   682, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   683, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   684, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   685, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   686, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_damage_28 ),
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl0f_damage_29[] = {
	CG_FRAME(    0,    0,   2,           0,   687, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   688, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   689, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2,           0,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl0f_damage_29 ),
};

/* Char_Caught 0 */
const Sint32 pl0f_caught_00[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   636, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl0f_caught_01[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   637, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl0f_caught_02[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   639, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_NEXT( pl0f_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl0f_caught_03[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl0f_caught_04[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl0f_caught_05[] = {
	CG_FRAME( 0x81,    0,   1, END_OF_DATA,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl0f_caught_06[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   644, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl0f_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl0f_caught_07[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   645, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl0f_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl0f_caught_08[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   646, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl0f_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl0f_caught_09[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   641, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl0f_caught_10[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   642, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl0f_caught_11[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   643, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl0f_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl0f_caught_12[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   647, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_NEXT( pl0f_caught_20 ),
};

/* Char_Caught 13 */
const Sint32 pl0f_caught_13[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   659, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl0f_caught_14[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   661, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl0f_caught_15[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   662, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl0f_caught_16[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   663, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl0f_caught_17[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   664, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl0f_caught_18[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   681, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl0f_caught_19[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   687, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_NEXT( pl0f_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl0f_caught_20[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   650, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_NEXT( pl0f_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl0f_caught_21[] = {
	CG_FRAME(    0,    0,   1, END_OF_DATA,   655, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_NEXT( pl0f_caught_21 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl0f_dizzy_00[] = {
	CG_FRAME(    0,    0,   2,           0,   693, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   693, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_dizzy_00 ),
};

/* Char_Win 0: win_ko_init, win0_move, win_time */
const Sint32 pl0f_win_00[] = {
	CG_FRAME(    0,    0,   4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_00 ),
};

/* Char_Win 3: pls0f_00_c_fall (3 + sub_type) */
const Sint32 pl0f_win_03[] = {
	CG_FRAME(    0,    0,   4,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_03 ),
};

/* Char_Win 4 */
const Sint32 pl0f_win_04[] = {
	CG_FRAME(    0,    0,   4,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_04 ),
};

/* Char_Win 5 */
const Sint32 pl0f_win_05[] = {
	CG_FRAME(    0,    0,   4,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_05 ),
};

/* Char_Win 6 */
const Sint32 pl0f_win_06[] = {
	CG_FRAME(    0,    0,   4,           0,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4, END_OF_DATA,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_06 ),
};

/* Char_Win 1: win_ko_init */
const Sint32 pl0f_win_01[] = {
	CG_FRAME(    0,    0,   4,           0,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   366, 0x0f,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   373, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0f_win_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_01_loop ),
};

/* group 27 0: pls0f_00_a_init */
const Sint32 pl0f_grp27_00[] = {
	EF_FRAME(    0,    0,   4, END_OF_DATA,   380 ),
	CG_NEXT( pl0f_grp27_00 ),
};

/* Char_Win 2: lose_time_ko, draw_time */
const Sint32 pl0f_win_02[] = {
	CG_FRAME(    0,    0,   2,           0,   289, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   290, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   291, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0f_win_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   292, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   293, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   294, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   295, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   296, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   297, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   298, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   299, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   300, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   301, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   302, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   303, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   304, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   305, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_win_02_loop ),
};

/* Char_Appear 0: appear_init */
const Sint32 pl0f_appear_00[] = {
	CG_FRAME(    0,    0,   3,           0,   306, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   307, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   308, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   309, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   310, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0,   311, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0f_appear_00_loop[] = {
	CG_FRAME(    0,    0,   3, END_OF_DATA,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_appear_00_loop ),
};

/* Char_Appear 1: appear_init */
const Sint32 pl0f_appear_01[] = {
	CG_FRAME(    0,    0,   4,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   322, 0x80,   0x00,    0,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   334, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl0f_appear_01_loop[] = {
	CG_FRAME(    0,    0,   4, END_OF_DATA,   754, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_appear_01_loop ),
};

/* Char_Extra 2: chouhatsu_init (Taunt), fly_start5 */
const Sint32 pl0f_extra_02[] = {
	CG_FRAME(    0,    0,   6,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   6, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_extra_02 ),
};

/* Char_Change 4: act_change_assist_end
   group 25 4 */
const Sint32 pl0f_change_04[] = {
	CG_FRAME(    0,    0,   5,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,   5, END_OF_DATA,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl0f_change_04 ),
};

/* Char_Effect 4 */
const Sint32 pl0f_effect_04[] = {
	CG_FRAME(    0,    0,   1,           0, 32793, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32794, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32795, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32796, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32797, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32798, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_04 ),
};

/* Char_Effect 5 */
const Sint32 pl0f_effect_05[] = {
	CG_FRAME(    0,    0,   1,           0, 32799, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32800, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32801, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32802, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32803, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32804, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_05 ),
};

/* Char_Effect 6 */
const Sint32 pl0f_effect_06[] = {
	CG_FRAME(    0,    0,   1,           0, 32805, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32806, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32807, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32808, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32809, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32810, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_06 ),
};

/* Char_Effect 7 */
const Sint32 pl0f_effect_07[] = {
	CG_FRAME(    0,    0,   1,           0, 32811, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32812, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32813, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32814, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32815, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32816, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_07 ),
};

/* Char_Effect 8 */
const Sint32 pl0f_effect_08[] = {
	CG_FRAME(    0,    0,   1,           0, 32817, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32818, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32819, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32820, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32821, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32822, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_08 ),
};

/* Char_Effect 9 */
const Sint32 pl0f_effect_09[] = {
	CG_FRAME(    0,    0,   1,           0, 32823, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32824, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32825, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32826, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32827, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32828, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_09 ),
};

/* Char_Effect 10 */
const Sint32 pl0f_effect_10[] = {
	CG_FRAME(    0,    0,   1,           0, 32829, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32830, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32831, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32832, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32833, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32834, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_10 ),
};

/* Char_Effect 11 */
const Sint32 pl0f_effect_11[] = {
	CG_FRAME(    0,    0,   1,           0, 32835, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32836, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32837, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32838, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32839, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32840, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_11 ),
};

/* Char_Effect 12 */
const Sint32 pl0f_effect_12[] = {
	CG_FRAME(    0,    0,   1,           0, 32841, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32842, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32843, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32844, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32845, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32846, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_12 ),
};

/* Char_Effect 13 */
const Sint32 pl0f_effect_13[] = {
	CG_FRAME(    0,    0,   1,           0, 32847, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32848, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32849, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32850, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32851, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32852, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_13 ),
};

/* Char_Effect 14 */
const Sint32 pl0f_effect_14[] = {
	CG_FRAME(    0,    0,   1,           0, 32853, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32854, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32855, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32856, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32857, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32858, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_14 ),
};

/* Char_Effect 15 */
const Sint32 pl0f_effect_15[] = {
	CG_FRAME(    0,    0,   1,           0, 32859, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32860, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32861, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32862, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1,           0, 32863, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,   1, END_OF_DATA, 32864, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl0f_effect_15 ),
};

/* Char_Effect 24 */
const Sint32 pl0f_effect_24[] = {
	CG_FRAME(    0,    0,   4,           0, 32865, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32866, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32867, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32868, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32869, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32870, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32871, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_effect_24 ),
};

/* Char_Effect 16 */
const Sint32 pl0f_effect_16[] = {
	CG_FRAME(    0,    0,   4,           0, 32865, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32866, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32867, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32868, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32869, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32870, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32871, 0x80,   0x00,    0,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_effect_16 ),
};

/* Char_Effect 23: shl0f_03_1_init */
const Sint32 pl0f_effect_23[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   727, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_NEXT( pl0f_effect_23 ),
};

/* Char_Effect 17: pls0f_00_b_init */
const Sint32 pl0f_effect_17[] = {
	CG_FRAME(    0,    0,   8,           0, 32865, 0x80,   0x00,  ATK,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   4,           0, 32866, 0x80,   0x00,  ATE,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32867, 0x80,   0x00,  ATE,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32868, 0x80,   0x00,  ATE,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32869, 0x80,   0x00,  ATE,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3,           0, 32870, 0x80,   0x00,  ATE,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA, 32871, 0x80,   0x00,  ATE,  0x00,          0,    4,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_effect_17 ),
};

/* Char_Effect 0: shl0f_00_init */
const Sint32 pl0f_effect_00[] = {
	CG_FRAME(    0,    0,   2,           0,   727, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   728, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2,           0,   729, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   730, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_NEXT( pl0f_effect_00 ),
};

/* Char_Effect 1: shl0f_00_fly */
const Sint32 pl0f_effect_01[] = {
	CG_FRAME(    0,    0,   2,           0,   731, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,   2,           0,   732, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,   2,           0,   733, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    1,   2, END_OF_DATA,   734, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl0f_effect_01 ),
};

/* Char_Effect 2: shl0f_00_end_set */
const Sint32 pl0f_effect_02[] = {
	CG_FRAME(    0,    0,   2,           0,   735, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   736, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   737, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_effect_02 ),
};

/* Char_Effect 3: shl0f_01_init */
const Sint32 pl0f_effect_03[] = {
	CG_FRAME(    0,    0,   2,           0,   738, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl0f_effect_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   739, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   740, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2,           0,   741, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   742, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl0f_effect_03_loop ),
};

/* Char_Effect 20: shl0f_04_source_init */
const Sint32 pl0f_effect_20[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   705, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl0f_effect_20 ),
};

/* Char_Effect 21: shl0f_04_source_fire */
const Sint32 pl0f_effect_21[] = {
	CG_FRAME(    0,    0,   2,           0,   706, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   707, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   708, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   709, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   710, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_effect_21 ),
};

/* Char_Effect 18 */
const Sint32 pl0f_effect_18[] = {
	CG_FRAME(    0,    0,  -1, END_OF_DATA,   711, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl0f_effect_18 ),
};

/* Char_Effect 19 */
const Sint32 pl0f_effect_19[] = {
	CG_FRAME(    0,    0,   2,           0,   712, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   713, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   714, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2,           0,   715, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   716, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl0f_effect_19 ),
};

/* Char_Effect 22: shl0f_05_init */
const Sint32 pl0f_effect_22[] = {
	CG_FRAME(    0,    0,   2,           0, 32768, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32769, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32770, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32771, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32772, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32773, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32775, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32776, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32777, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32778, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32779, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32780, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32781, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32782, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32783, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32784, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32785, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32786, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32787, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32788, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32789, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32790, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2,           0, 32791, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA, 32792, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl0f_effect_22 ),
};

/* Char_Fly 0: fly_init (Flight) */
const Sint32 pl0f_fly_00[] = {
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3,           0,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   3, END_OF_DATA,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_00 ),
};

/* Char_Fly 1: act_fly */
const Sint32 pl0f_fly_01[] = {
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_01 ),
};

/* Char_Fly 2
   Char_Fly 6 */
const Sint32 pl0f_fly_02[] = {
	CG_FRAME(    0,    0,   2,           0,   212, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0f_fly_02_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_02_loop ),
};

/* Char_Fly 10
   Char_Fly 14 */
const Sint32 pl0f_fly_10[] = {
	CG_FRAME(    0,    0,   2,           0,   211, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   212, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_10 ),
};

/* Char_Fly 7
   Char_Fly 8
   Char_Fly 9 */
const Sint32 pl0f_fly_07[] = {
	CG_FRAME(    0,    0,   2,           0,   755, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   756, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0f_fly_07_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   757, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   758, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_07_loop ),
};

/* Char_Fly 15
   Char_Fly 16
   Char_Fly 17 */
const Sint32 pl0f_fly_15[] = {
	CG_FRAME(    0,    0,   2,           0,   759, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   760, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   761, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   762, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_15 ),
};

/* Char_Fly 3
   Char_Fly 4
   Char_Fly 5 */
const Sint32 pl0f_fly_03[] = {
	CG_FRAME(    0,    0,   2,           0,   763, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   764, 0x80,   0x00,    0,  0x00,       0x24,    0,   0,     0,      0,   3 ),
};

const Sint32 pl0f_fly_03_loop[] = {
	CG_FRAME(    0,    0,   2,           0,   765, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   766, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_03_loop ),
};

/* Char_Fly 11
   Char_Fly 12
   Char_Fly 13 */
const Sint32 pl0f_fly_11[] = {
	CG_FRAME(    0,    0,   2,           0,   767, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2,           0,   768, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,   2, END_OF_DATA,   769, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl0f_fly_11 ),
};
