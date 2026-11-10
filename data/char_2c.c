/* Magneto (character 2c): the animations. PLAYER_WORK's char_adrs points here.

   spr_prm_2c      a sprite's sizes and turn (SPR_PRM): a frame's zoom number picks a row, 0 for none.
   char_tbl_2c     the groups: pl_char_set( pl_w, group, no ) takes the group's table from here and
                   starts its animation no. The character's module (s_pl2c.c) says which group
                   and number each move uses.
   pl2c_GROUP_tbl  a group's animations. GROUP is the group's name in player.h (stand for
                   Char_Stand), or grpNN for a group with no name there.
   pl2c_GROUP_NN   an animation, named by the first group and number that has it: its frames in
                   order. The comment before it lists its places in the groups and, for each,
                   the routines that start it by that number, with their special move;
                   (2 + atck_level) is a routine that counts on from 2 by the attack's level.
                   A frame with END_OF_DATA is the last, and the word after it is the frame the
                   animation goes on from (CG_NEXT; pl2c_GROUP_NN_loop where that is no
                   animation's first frame). An effect's animation has short frames (EF_FRAME)
                   and may end with no word.
   pl2c_unused_NN  frames that no table or animation reaches.

   A player's frame (CG_FRAME, the 0x14 bytes of CHAR_DATA), with the names Capcom's tables used:
     prm, type  a number and a code for the character's module (a speed, an attack, a voice)
     ctr        the game frames it is shown for
     end        END_OF_DATA on the last frame
     spr        the sprite, -1 for none
     prio       added to the draw priority (a signed byte)
     shadow     the shadow's offset (the low five bits) and size, 0xff none
     atck       ATK while an attack is on its way or can hit, ATE in its frames after that;
                CG_ATCK_NG lets nothing cancel it
     flag1      a number for the module: most often the attack, a row of hit_dt_2c
     se         a voice (1 to 63), or with CSE a sound effect; 0 none
     zoom       the row of spr_prm_2c, 0 none
     unk        a byte the program is not seen to read
     flag2      a second code for the character's effects
     rival      where a held opponent is put (the low byte), and a code for the module (the high)
     box        the frame's row of frame_box_2c */
#include "player.h"
#include "char_tbl.h"

extern const Sint32 pl2c_stand_00[], pl2c_stand_02[], pl2c_stand_02_loop[], pl2c_stand_03[],
	pl2c_stand_03_loop[], pl2c_stand_04[], pl2c_stand_05[], pl2c_jump_00[], pl2c_jump_00_loop[],
	pl2c_jump_01[], pl2c_jump_01_loop[], pl2c_jump_02[], pl2c_jump_02_loop[], pl2c_jump_03[],
	pl2c_jump_06[], pl2c_jump_06_loop[], pl2c_jump_07[], pl2c_jump_07_loop[], pl2c_jump_08[],
	pl2c_jump_08_loop[], pl2c_jump_09[], pl2c_dash_00[], pl2c_dash_00_loop[], pl2c_dash_01[],
	pl2c_dash_02[], pl2c_dash_03[], pl2c_crouch_00[], pl2c_crouch_01[], pl2c_crouch_02[],
	pl2c_turn_00[], pl2c_turn_01[], pl2c_guard_00[], pl2c_guard_01[], pl2c_guard_02[],
	pl2c_guard_03[], pl2c_guard_03_loop[], pl2c_guard_04[], pl2c_guard_05[], pl2c_guard_06[],
	pl2c_guard_07[], pl2c_guard_07_loop[], pl2c_air_guard_00[], pl2c_air_guard_01[],
	pl2c_air_guard_02[], pl2c_air_guard_02_loop[], pl2c_stand_p_00[], pl2c_stand_p_01[],
	pl2c_stand_p_02[], pl2c_stand_k_00[], pl2c_stand_k_01[], pl2c_stand_k_02[], pl2c_crouch_p_00[],
	pl2c_crouch_p_01[], pl2c_crouch_p_02[], pl2c_crouch_k_00[], pl2c_crouch_k_01[],
	pl2c_crouch_k_02[], pl2c_crouch_k_02_loop[], pl2c_crouch_k_03[], pl2c_jump_p_00[],
	pl2c_jump_p_01[], pl2c_jump_p_02[], pl2c_jump_k_00[], pl2c_jump_k_01[], pl2c_jump_k_02[],
	pl2c_jump_k_03[], pl2c_unused_00[], pl2c_damage_00[], pl2c_damage_01[], pl2c_damage_02[],
	pl2c_damage_03[], pl2c_damage_10[], pl2c_damage_04[], pl2c_damage_11[], pl2c_damage_05[],
	pl2c_damage_12[], pl2c_damage_06[], pl2c_damage_07[], pl2c_damage_08[], pl2c_damage_13[],
	pl2c_damage_13_loop[], pl2c_damage_09[], pl2c_damage_09_loop[], pl2c_unused_01[],
	pl2c_unused_02[], pl2c_damage_16[], pl2c_damage_17[], pl2c_damage_18[], pl2c_damage_19[],
	pl2c_damage_20[], pl2c_damage_21[], pl2c_damage_22[], pl2c_damage_23[], pl2c_damage_24[],
	pl2c_damage_25[], pl2c_damage_26[], pl2c_damage_27[], pl2c_damage_27_loop[], pl2c_damage_32[],
	pl2c_damage_33[], pl2c_damage_28[], pl2c_unused_03[], pl2c_damage_29[], pl2c_damage_31[],
	pl2c_caught_00[], pl2c_caught_01[], pl2c_caught_02[], pl2c_caught_03[], pl2c_caught_04[],
	pl2c_caught_05[], pl2c_caught_06[], pl2c_caught_07[], pl2c_caught_08[], pl2c_caught_09[],
	pl2c_caught_10[], pl2c_caught_11[], pl2c_caught_12[], pl2c_caught_13[], pl2c_caught_14[],
	pl2c_caught_15[], pl2c_caught_16[], pl2c_caught_17[], pl2c_caught_18[], pl2c_caught_19[],
	pl2c_caught_20[], pl2c_caught_21[], pl2c_special_05[], pl2c_special_05_loop[], pl2c_special_08[],
	pl2c_special_08_loop[], pl2c_special_09[], pl2c_special_09_loop[], pl2c_special_10[],
	pl2c_special_10_loop[], pl2c_special_11[], pl2c_special_11_loop[], pl2c_special_12[],
	pl2c_special_13[], pl2c_special_14[], pl2c_special_15[], pl2c_appear_00[], pl2c_win_00[],
	pl2c_win_00_loop[], pl2c_win_01[], pl2c_win_01_loop[], pl2c_win_02[], pl2c_win_02_loop[],
	pl2c_win_03[], pl2c_fly_00[], pl2c_fly_01[], pl2c_fly_08[], pl2c_fly_08_loop[], pl2c_fly_04[],
	pl2c_fly_04_loop[], pl2c_fly_16[], pl2c_fly_12[], pl2c_fly_09[], pl2c_fly_02[], pl2c_fly_03[],
	pl2c_fly_07[], pl2c_fly_06[], pl2c_fly_05[], pl2c_fly_13[], pl2c_fly_10[], pl2c_dizzy_00[],
	pl2c_get_up_00[], pl2c_get_up_01[], pl2c_get_up_02[], pl2c_get_up_03[], pl2c_extra_05[],
	pl2c_extra_05_loop[], pl2c_extra_06[], pl2c_extra_07[], pl2c_special_00[], pl2c_extra_04[],
	pl2c_special_16[], pl2c_extra_02[], pl2c_extra_03[], pl2c_extra_03_loop[], pl2c_special_02[],
	pl2c_catch_08[], pl2c_special_04[], pl2c_extra_01[], pl2c_extra_00[], pl2c_catch_01[],
	pl2c_catch_02[], pl2c_catch_03[], pl2c_catch_04[], pl2c_catch_05[], pl2c_catch_06[],
	pl2c_catch_07[], pl2c_catch_00[], pl2c_catch_00_loop[], pl2c_effect_00[], pl2c_super_00[],
	pl2c_super_01[], pl2c_super_01_loop[], pl2c_super_02[], pl2c_super_13[], pl2c_super_14[],
	pl2c_super_14_loop[], pl2c_super_15[], pl2c_super_21[], pl2c_super_21_loop[], pl2c_super_16[],
	pl2c_super_16_loop[], pl2c_super_17[], pl2c_super_18[], pl2c_super_03[], pl2c_super_03_loop[],
	pl2c_super_04[], pl2c_super_05[], pl2c_super_05_loop[], pl2c_super_19[], pl2c_super_19_loop[],
	pl2c_super_06[], pl2c_super_20[], pl2c_super_07[], pl2c_super_07_loop[], pl2c_super_08[],
	pl2c_special_19[], pl2c_special_19_loop[], pl2c_special_20[], pl2c_special_20_loop[],
	pl2c_special_21[], pl2c_special_22[], pl2c_special_23[], pl2c_special_18[], pl2c_change_00[],
	pl2c_change_02[], pl2c_change_03[], pl2c_change_03_loop[], pl2c_change_01[],
	pl2c_change_01_loop[], pl2c_change_04[], pl2c_change_04_loop[], pl2c_grp27_00[], pl2c_grp27_01[],
	pl2c_grp27_02[], pl2c_grp27_03[], pl2c_grp27_04[], pl2c_grp27_05[], pl2c_grp27_06[],
	pl2c_grp27_06_loop[], pl2c_grp27_07[], pl2c_grp27_07_loop[], pl2c_grp27_08[],
	pl2c_grp27_08_loop[], pl2c_grp27_09[], pl2c_grp27_09_loop[], pl2c_grp27_10[], pl2c_grp27_11[],
	pl2c_grp27_12[], pl2c_grp27_13[], pl2c_grp27_18[];
extern const Sint32 *const pl2c_stand_tbl[], *const pl2c_jump_tbl[], *const pl2c_dash_tbl[],
	*const pl2c_crouch_tbl[], *const pl2c_turn_tbl[], *const pl2c_guard_tbl[],
	*const pl2c_air_guard_tbl[], *const pl2c_stand_p_tbl[], *const pl2c_stand_k_tbl[],
	*const pl2c_damage_tbl[], *const pl2c_dizzy_tbl[], *const pl2c_catch_tbl[],
	*const pl2c_caught_tbl[], *const pl2c_crouch_p_tbl[], *const pl2c_crouch_k_tbl[],
	*const pl2c_jump_p_tbl[], *const pl2c_jump_k_tbl[], *const pl2c_get_up_tbl[],
	*const pl2c_appear_tbl[], *const pl2c_win_tbl[], *const pl2c_extra_tbl[],
	*const pl2c_special_tbl[], *const pl2c_super_tbl[], *const pl2c_effect_tbl[],
	*const pl2c_change_tbl[], *const pl2c_grp25_tbl[], *const pl2c_fly_tbl[],
	*const pl2c_grp27_tbl[];

/* the sprites' sizes and turns, by a frame's zoom number */
const SPR_PRM spr_prm_2c[16] = {
	{ 0, 0, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, 0, 0, 0, 0, 0, 0 },
	{ 1000, 1000, -32768, 0, 0, 0, 0, 0 },
	{ 1000, 6500, 0, 0, 0, 0, 0, 0 },
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
const Sint32 *const *const char_tbl_2c[] = {
	pl2c_stand_tbl, pl2c_jump_tbl, pl2c_dash_tbl, pl2c_crouch_tbl, pl2c_turn_tbl, pl2c_guard_tbl,
	pl2c_air_guard_tbl, pl2c_stand_p_tbl, pl2c_stand_k_tbl, pl2c_crouch_p_tbl, pl2c_crouch_k_tbl, pl2c_jump_p_tbl,
	pl2c_jump_k_tbl, pl2c_damage_tbl, pl2c_dizzy_tbl, pl2c_catch_tbl, pl2c_caught_tbl, pl2c_get_up_tbl,
	pl2c_appear_tbl, pl2c_win_tbl, pl2c_extra_tbl, pl2c_special_tbl, pl2c_super_tbl, pl2c_effect_tbl,
	pl2c_change_tbl, pl2c_grp25_tbl, pl2c_fly_tbl, pl2c_grp27_tbl, 0,
};

const Sint32 *const pl2c_stand_tbl[] = {
	pl2c_stand_00, pl2c_stand_00, pl2c_stand_02, pl2c_stand_03, pl2c_stand_04, pl2c_stand_05,
	pl2c_stand_02, pl2c_stand_03, 0,
};

const Sint32 *const pl2c_jump_tbl[] = {
	pl2c_jump_00, pl2c_jump_01, pl2c_jump_02, pl2c_jump_03, pl2c_jump_03, pl2c_jump_03,
	pl2c_jump_06, pl2c_jump_07, pl2c_jump_08, pl2c_jump_09, pl2c_jump_09, pl2c_jump_09,
	0,
};

const Sint32 *const pl2c_dash_tbl[] = {
	pl2c_dash_00, pl2c_dash_01, pl2c_dash_02, pl2c_dash_03, 0,
};

const Sint32 *const pl2c_crouch_tbl[] = {
	pl2c_crouch_00, pl2c_crouch_01, pl2c_crouch_02, 0,
};

const Sint32 *const pl2c_turn_tbl[] = {
	pl2c_turn_00, pl2c_turn_01, 0,
};

const Sint32 *const pl2c_guard_tbl[] = {
	pl2c_guard_00, pl2c_guard_01, pl2c_guard_02, pl2c_guard_03, pl2c_guard_04, pl2c_guard_05,
	pl2c_guard_06, pl2c_guard_07, 0,
};

const Sint32 *const pl2c_air_guard_tbl[] = {
	pl2c_air_guard_00, pl2c_air_guard_01, pl2c_air_guard_02, 0,
};

const Sint32 *const pl2c_stand_p_tbl[] = {
	pl2c_stand_p_00, pl2c_stand_p_01, pl2c_stand_p_02, 0,
};

const Sint32 *const pl2c_stand_k_tbl[] = {
	pl2c_stand_k_00, pl2c_stand_k_01, pl2c_stand_k_02, 0,
};

const Sint32 *const pl2c_damage_tbl[] = {
	pl2c_damage_00, pl2c_damage_01, pl2c_damage_02, pl2c_damage_03, pl2c_damage_04, pl2c_damage_05,
	pl2c_damage_06, pl2c_damage_07, pl2c_damage_08, pl2c_damage_09, pl2c_damage_10, pl2c_damage_11,
	pl2c_damage_12, pl2c_damage_13, pl2c_damage_13, pl2c_damage_13, pl2c_damage_16, pl2c_damage_17,
	pl2c_damage_18, pl2c_damage_19, pl2c_damage_20, pl2c_damage_21, pl2c_damage_22, pl2c_damage_23,
	pl2c_damage_24, pl2c_damage_25, pl2c_damage_26, pl2c_damage_27, pl2c_damage_28, pl2c_damage_29,
	pl2c_damage_27, pl2c_damage_31, pl2c_damage_32, pl2c_damage_33, pl2c_damage_13, 0,
};

const Sint32 *const pl2c_dizzy_tbl[] = {
	pl2c_dizzy_00, pl2c_stand_00, 0,
};

const Sint32 *const pl2c_catch_tbl[] = {
	pl2c_catch_00, pl2c_catch_01, pl2c_catch_02, pl2c_catch_03, pl2c_catch_04, pl2c_catch_05,
	pl2c_catch_06, pl2c_catch_07, pl2c_catch_08, 0,
};

const Sint32 *const pl2c_caught_tbl[] = {
	pl2c_caught_00, pl2c_caught_01, pl2c_caught_02, pl2c_caught_03, pl2c_caught_04, pl2c_caught_05,
	pl2c_caught_06, pl2c_caught_07, pl2c_caught_08, pl2c_caught_09, pl2c_caught_10, pl2c_caught_11,
	pl2c_caught_12, pl2c_caught_13, pl2c_caught_14, pl2c_caught_15, pl2c_caught_16, pl2c_caught_17,
	pl2c_caught_18, pl2c_caught_19, pl2c_caught_20, pl2c_caught_21, 0,
};

const Sint32 *const pl2c_crouch_p_tbl[] = {
	pl2c_crouch_p_00, pl2c_crouch_p_01, pl2c_crouch_p_02, 0,
};

const Sint32 *const pl2c_crouch_k_tbl[] = {
	pl2c_crouch_k_00, pl2c_crouch_k_01, pl2c_crouch_k_02, pl2c_crouch_k_03, 0,
};

const Sint32 *const pl2c_jump_p_tbl[] = {
	pl2c_jump_p_00, pl2c_jump_p_01, pl2c_jump_p_02, pl2c_jump_p_00, pl2c_jump_p_01, pl2c_jump_p_02,
	0,
};

const Sint32 *const pl2c_jump_k_tbl[] = {
	pl2c_jump_k_00, pl2c_jump_k_01, pl2c_jump_k_02, pl2c_jump_k_03, pl2c_jump_k_01, pl2c_jump_k_02,
	0,
};

const Sint32 *const pl2c_get_up_tbl[] = {
	pl2c_get_up_00, pl2c_get_up_01, pl2c_get_up_02, pl2c_get_up_03, 0,
};

const Sint32 *const pl2c_appear_tbl[] = {
	pl2c_appear_00, 0,
};

const Sint32 *const pl2c_win_tbl[] = {
	pl2c_win_00, pl2c_win_01, pl2c_win_02, pl2c_win_03, 0,
};

const Sint32 *const pl2c_extra_tbl[] = {
	pl2c_extra_00, pl2c_extra_01, pl2c_extra_02, pl2c_extra_03, pl2c_extra_04, pl2c_extra_05,
	pl2c_extra_06, pl2c_extra_07, 0,
};

const Sint32 *const pl2c_special_tbl[] = {
	pl2c_special_00, pl2c_extra_03, pl2c_special_02, pl2c_catch_08, pl2c_special_04, pl2c_special_05,
	pl2c_special_05, pl2c_special_05, pl2c_special_08, pl2c_special_09, pl2c_special_10, pl2c_special_11,
	pl2c_special_12, pl2c_special_13, pl2c_special_14, pl2c_special_15, pl2c_special_16, pl2c_extra_03,
	pl2c_special_18, pl2c_special_19, pl2c_special_20, pl2c_special_21, pl2c_special_22, pl2c_special_23,
	0,
};

const Sint32 *const pl2c_super_tbl[] = {
	pl2c_super_00, pl2c_super_01, pl2c_super_02, pl2c_super_03, pl2c_super_04, pl2c_super_05,
	pl2c_super_06, pl2c_super_07, pl2c_super_08, pl2c_special_19, pl2c_special_19, pl2c_special_19,
	pl2c_special_19, pl2c_super_13, pl2c_super_14, pl2c_super_15, pl2c_super_16, pl2c_super_17,
	pl2c_super_18, pl2c_super_19, pl2c_super_20, pl2c_super_21, pl2c_super_06, 0,
};

const Sint32 *const pl2c_effect_tbl[] = {
	pl2c_effect_00, 0,
};

const Sint32 *const pl2c_change_tbl[] = {
	pl2c_change_00, pl2c_change_01, pl2c_change_02, pl2c_change_03, pl2c_change_04, 0,
};

const Sint32 *const pl2c_grp25_tbl[] = {
	pl2c_extra_05, pl2c_extra_05, pl2c_change_04, pl2c_change_04, pl2c_change_04, 0,
};

const Sint32 *const pl2c_fly_tbl[] = {
	pl2c_fly_00, pl2c_fly_01, pl2c_fly_02, pl2c_fly_03, pl2c_fly_04, pl2c_fly_05,
	pl2c_fly_06, pl2c_fly_07, pl2c_fly_08, pl2c_fly_09, pl2c_fly_10, pl2c_fly_10,
	pl2c_fly_12, pl2c_fly_13, pl2c_fly_13, pl2c_fly_13, pl2c_fly_16, pl2c_fly_10,
	0,
};

const Sint32 *const pl2c_grp27_tbl[] = {
	pl2c_grp27_00, pl2c_grp27_01, pl2c_grp27_02, pl2c_grp27_03, pl2c_grp27_04, pl2c_grp27_05,
	pl2c_grp27_06, pl2c_grp27_07, pl2c_grp27_08, pl2c_grp27_09, pl2c_grp27_10, pl2c_grp27_11,
	pl2c_grp27_12, pl2c_grp27_13, pl2c_grp27_10, pl2c_grp27_11, pl2c_grp27_12, pl2c_grp27_13,
	pl2c_grp27_18, 0,
};

	/*         prm  type   ctr          end    spr  prio  shadow  atck  flag1          se  zoom  unk  flag2   rival  box */

/* Char_Stand 0: pl2c_appear, act_stand, appear_move, act_change_gattai_move, ...
   Char_Stand 1
   Char_Dizzy 1 */
const Sint32 pl2c_stand_00[] = {
	CG_FRAME(    0,    0,   10,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   130, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,   1 ),
	CG_NEXT( pl2c_stand_00 ),
};

/* Char_Stand 2: act_walk (2 + no)
   Char_Stand 6 */
const Sint32 pl2c_stand_02[] = {
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2c_stand_02_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2c_stand_02_loop ),
};

/* Char_Stand 3
   Char_Stand 7 */
const Sint32 pl2c_stand_03[] = {
	CG_FRAME(    0,    0,    3,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2c_stand_03_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2c_stand_03_loop ),
};

/* Char_Stand 4 */
const Sint32 pl2c_stand_04[] = {
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2c_stand_00 ),
};

/* Char_Stand 5 */
const Sint32 pl2c_stand_05[] = {
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2c_stand_00 ),
};

/* Char_Jump 0 */
const Sint32 pl2c_jump_00[] = {
	CG_FRAME(    0,    1,    2,           0,   206, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,    2,           0,   207, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   208, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   209, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   210, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   211, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   212, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   214, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   216, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
};

const Sint32 pl2c_jump_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl2c_jump_00_loop ),
};

/* Char_Jump 1: act_wall_jump, act_change_gattai_init, act_change_zero_init, act_change_assist_init, ... */
const Sint32 pl2c_jump_01[] = {
	CG_FRAME(    0,    1,    2,           0,   206, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,    2,           0,   207, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   231, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   232, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   235, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   236, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   237, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   238, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
};

const Sint32 pl2c_jump_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl2c_jump_01_loop ),
};

/* Char_Jump 2 */
const Sint32 pl2c_jump_02[] = {
	CG_FRAME(    0,    1,    2,           0,   206, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    1,    2,           0,   207, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    3,           0,   240, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   243, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   237, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   238, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   239, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
};

const Sint32 pl2c_jump_02_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl2c_jump_02_loop ),
};

/* Char_Jump 3
   Char_Jump 4
   Char_Jump 5 */
const Sint32 pl2c_jump_03[] = {
	CG_FRAME(    0,    1,    2,           0,   220, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   221, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   222, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   223, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   224, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   225, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   226, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   227, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   228, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   229, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   230, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_jump_03 ),
};

/* Char_Jump 6 */
const Sint32 pl2c_jump_06[] = {
	CG_FRAME(    0,    1,    2,           0,   206, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    2,           0,   207, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   208, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   209, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   210, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   211, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    8,           0,   212, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   213, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   214, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   215, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   216, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    3,           0,   217, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl2c_jump_06_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2c_jump_06_loop ),
};

/* Char_Jump 7 */
const Sint32 pl2c_jump_07[] = {
	CG_FRAME(    0,    1,    2,           0,   206, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    2,           0,   207, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   231, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   232, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   233, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   234, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   235, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   236, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   237, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl2c_jump_07_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2c_jump_07_loop ),
};

/* Char_Jump 8 */
const Sint32 pl2c_jump_08[] = {
	CG_FRAME(    0,    1,    2,           0,   206, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    1,    2,           0,   207, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   240, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   241, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2,           0,   242, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   243, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   244, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   245, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    6,           0,   237, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   238, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   239, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    4,           0,   217, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
};

const Sint32 pl2c_jump_08_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   2 ),
	CG_NEXT( pl2c_jump_08_loop ),
};

/* Char_Jump 9: act_fall, act_air_guard_hold, act_wall_jump
   Char_Jump 10: change_in
   Char_Jump 11: act_change_out_recover */
const Sint32 pl2c_jump_09[] = {
	CG_FRAME(    0,    0,    2,           0,   218, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   219, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,   4 ),
	CG_NEXT( pl2c_jump_09 ),
};

/* Char_Dash 0: act_dash */
const Sint32 pl2c_dash_00[] = {
	CG_FRAME(    0,    1,    3,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
};

const Sint32 pl2c_dash_00_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   184, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   185, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4,           0,   186, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   187, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   5 ),
	CG_NEXT( pl2c_dash_00_loop ),
};

/* Char_Dash 1: act_back_dash */
const Sint32 pl2c_dash_01[] = {
	CG_FRAME(    0,    0,    4,           0,   196, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   197, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4,           0,   198, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   199, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   6 ),
	CG_NEXT( pl2c_dash_01 ),
};

/* Char_Dash 2: dash_move */
const Sint32 pl2c_dash_02[] = {
	CG_FRAME(    0,    0,    2,           0,   188, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   189, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   190, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   191, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   192, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   193, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   194, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   195, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_dash_02 ),
};

/* Char_Dash 3: noki_move */
const Sint32 pl2c_dash_03[] = {
	CG_FRAME(    0,    0,    2,           0,   200, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   201, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   202, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   203, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   204, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   205, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_dash_03 ),
};

/* Char_Crouch 0: act_crouch, act_stand_up */
const Sint32 pl2c_crouch_00[] = {
	CG_FRAME(    0,    0,    2,           0,   246, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    1,    0,    2,           0,   247, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    1,    0,    2,           0,   248, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    2,    0,    3,           0,   249, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    2,    0,    3, END_OF_DATA,   250, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2c_crouch_00 ),
};

/* Char_Crouch 1: act_crouching, act_escape */
const Sint32 pl2c_crouch_01[] = {
	CG_FRAME(    2,    0,    2, END_OF_DATA,   250, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2c_crouch_01 ),
};

/* Char_Crouch 2: act_stand_up, act_crouch */
const Sint32 pl2c_crouch_02[] = {
	CG_FRAME(    2,    0,    4,           0,   251, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    1,    0,    4,           0,   252, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   253, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_crouch_02 ),
};

/* Char_Turn 0: act_turn */
const Sint32 pl2c_turn_00[] = {
	CG_FRAME(    0,    0,    2,           0,   254, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   255, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   256, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   257, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    1,           0,   258, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   259, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_turn_00 ),
};

/* Char_Turn 1: act_crouch_turn */
const Sint32 pl2c_turn_01[] = {
	CG_FRAME(    0,    0,    2,           0,   260, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    4,           0,   261, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2,           0,   262, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   261, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   7 ),
	CG_NEXT( pl2c_turn_01 ),
};

/* Char_Guard 0 */
const Sint32 pl2c_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   263, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

/* Char_Guard 1: act_guard_hold */
const Sint32 pl2c_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   264, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2c_guard_01 ),
};

/* Char_Guard 2 */
const Sint32 pl2c_guard_02[] = {
	CG_FRAME(    0,    0,    3,           0,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2c_guard_02 ),
};

/* Char_Guard 3 */
const Sint32 pl2c_guard_03[] = {
	CG_FRAME(    0,    0,    2,           0,   265, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   266, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   267, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   268, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   269, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   270, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   271, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   272, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   273, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   274, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   275, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   276, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_FRAME(    0,    0,    2,           0,   277, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
};

const Sint32 pl2c_guard_03_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   279, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   8 ),
	CG_NEXT( pl2c_guard_03_loop ),
};

/* Char_Guard 4 */
const Sint32 pl2c_guard_04[] = {
	CG_FRAME(    0,    0,    2,           0,   280, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   281, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

/* Char_Guard 5: act_guard_hold */
const Sint32 pl2c_guard_05[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   282, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2c_guard_05 ),
};

/* Char_Guard 6 */
const Sint32 pl2c_guard_06[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   281, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2c_guard_06 ),
};

/* Char_Guard 7 */
const Sint32 pl2c_guard_07[] = {
	CG_FRAME(    0,    0,    2,           0,   283, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   284, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   285, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   286, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   287, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   288, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   289, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   290, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   291, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   292, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   293, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   294, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_FRAME(    0,    0,    2,           0,   295, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
};

const Sint32 pl2c_guard_07_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   281, 0x80,   0x40,    0,  0x00,          0,    0,   0,     0,      0,   9 ),
	CG_NEXT( pl2c_guard_07_loop ),
};

/* Char_Air_Guard 0: act_air_guard */
const Sint32 pl2c_air_guard_00[] = {
	CG_FRAME(    0,    0,    2,           0,   296, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   297, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
};

/* Char_Air_Guard 1: act_air_guard_start */
const Sint32 pl2c_air_guard_01[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   298, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl2c_air_guard_01 ),
};

/* Char_Air_Guard 2: guard_char_set */
const Sint32 pl2c_air_guard_02[] = {
	CG_FRAME(    0,    0,    2,           0,   299, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   300, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   301, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   302, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   303, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   304, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   305, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   306, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   307, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   308, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   309, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   310, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_FRAME(    0,    0,    2,           0,   311, 0x80,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
};

const Sint32 pl2c_air_guard_02_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   298, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0,  34 ),
	CG_NEXT( pl2c_air_guard_02_loop ),
};

/* Char_Stand_P 0 (light) */
const Sint32 pl2c_stand_p_00[] = {
	CG_FRAME(    0,    0,    1,           0,   476, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_FRAME(    0,    0,    1,           0,   477, 0xa0,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,  11 ),
	CG_FRAME( 0x80,    0,    1,           0,   478, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x80,    0,    1,           0,   479, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME( 0x82,    0,    1,           0,   480, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  12 ),
	CG_FRAME(    0,    0,    1,           0,   481, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4,           0,   477, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  11 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   482, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  10 ),
	CG_NEXT( pl2c_stand_p_00 ),
};

/* Char_Stand_P 1 (medium) */
const Sint32 pl2c_stand_p_01[] = {
	CG_FRAME(    0,    0,    1,           0,   483, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0xa0,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   483, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    2,           0,   484, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   485, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    0,           0,   486, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   487, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  13 ),
	CG_FRAME(    0,    0,    1,           0,   488, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  14 ),
	CG_FRAME( 0x80,    0,    3,           0,   489, 0xa0,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    3,           0,   490, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    1,           0,   491, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    1,           0,   492, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x80,    0,    1,           0,   493, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME( 0x82,    0,    1,           0,   494, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  15 ),
	CG_FRAME(    0,    0,    1,           0,   495, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   496, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    2,           0,   497, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    1,           0,   498, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    1,           0,   499, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  16 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   500, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_stand_p_01 ),
};

/* Char_Stand_P 2 (heavy) */
const Sint32 pl2c_stand_p_02[] = {
	CG_FRAME(    0,    0,    5,           0,   501, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME( 0x80,    0,    3,           0,   502, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,  18 ),
	CG_FRAME(    0,    0,    1,           0,   503, 0xa0,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  19 ),
	CG_FRAME(    0,    0,    3,           0,   504, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    6,           0,   505, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    8,           0,   506, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    9,           0,   507, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   508, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_stand_p_02 ),
};

/* Char_Stand_K 0 (light) */
const Sint32 pl2c_stand_k_00[] = {
	CG_FRAME(    0,    0,    3,           0,   509, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   510, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    0,    3,           0,   511, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    0,    3,           0,   512, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME(    0,    1,    3,           0,   513, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    1,    3,           0,   197, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_FRAME(    0,    1,    4, END_OF_DATA,   198, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  23 ),
	CG_NEXT( pl2c_stand_k_00 ),
};

/* Char_Stand_K 1 (medium) */
const Sint32 pl2c_stand_k_01[] = {
	CG_FRAME(    0,    0,    4,           0,   514, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    2,           0,   515, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME( 0x80,    0,    1,           0,   516, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  25 ),
	CG_FRAME( 0x80,    0,    6,           0,   517, 0x78,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  26 ),
	CG_FRAME(    0,    0,    5,           0,   518, 0x78,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  27 ),
	CG_FRAME(    0,    0,    3,           0,   519, 0x78,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    3,           0,   520, 0x78,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    3,           0,   521, 0x78,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    3,           0,   522, 0x78,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   523, 0x78,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  24 ),
	CG_NEXT( pl2c_stand_k_01 ),
};

/* Char_Stand_K 2 (heavy) */
const Sint32 pl2c_stand_k_02[] = {
	CG_FRAME(    0,    0,    5,           0,   524, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  28 ),
	CG_FRAME(    0,    0,    1,           0,   525, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  28 ),
	CG_FRAME( 0x80,    0,    2,           0,   526, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME( 0x80,    0,    4,           0,   527, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  29 ),
	CG_FRAME(    0,    0,    1,           0,   528, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  30 ),
	CG_FRAME(    0,    0,    1,           0,   529, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    1,    6,           0,   530, 0xa0,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   531, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   532, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  32 ),
	CG_FRAME(    0,    0,    3,           0,   533, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    3,           0,   534, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  33 ),
	CG_FRAME(    0,    0,    5,           0,   535, 0x80,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  31 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   536, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_stand_k_02 ),
};

/* Char_Crouch_P 0 (light) */
const Sint32 pl2c_crouch_p_00[] = {
	CG_FRAME(    0,    0,    1,           0,   537, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME(    0,    0,    2,           0,   538, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  36 ),
	CG_FRAME( 0x80,    0,    1,           0,   539, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x80,    0,    1,           0,   540, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME( 0x82,    0,    1,           0,   541, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  37 ),
	CG_FRAME(    0,    0,    4,           0,   542, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   543, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  38 ),
	CG_NEXT( pl2c_crouch_p_00 ),
};

/* Char_Crouch_P 1 (medium) */
const Sint32 pl2c_crouch_p_01[] = {
	CG_FRAME(    0,    0,    3,           0,   544, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    0,    3,           0,   545, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  39 ),
	CG_FRAME(    0,    1,    1,           0,   546, 0xa0,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    3,           0,   547, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    2,           0,   548, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    2,           0,   549, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,   32,           0,   550, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   551, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  40 ),
	CG_NEXT( pl2c_crouch_p_01 ),
};

/* Char_Crouch_P 2 (heavy) */
const Sint32 pl2c_crouch_p_02[] = {
	CG_FRAME(    0,    0,    1,           0,   576, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   577, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3,           0,   578, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   579, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   580, 0xa0,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   581, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   582, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   583, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   584, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   585, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   10,           0,   586, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   587, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl2c_crouch_p_02 ),
};

/* Char_Crouch_K 0 (light) */
const Sint32 pl2c_crouch_k_00[] = {
	CG_FRAME(    0,    0,    1,           0,   588, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    1,           0,   589, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    1,           0,   590, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    2,           0,   589, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  45 ),
	CG_FRAME(    0,    0,    5,           0,   590, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    3,           0,   591, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   592, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl2c_crouch_k_00 ),
};

/* Char_Crouch_K 1 (medium) */
const Sint32 pl2c_crouch_k_01[] = {
	CG_FRAME(    0,    0,    2,           0,   588, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_FRAME( 0x80,    0,    3,           0,   589, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,    3,           0,   590, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME( 0x80,    0,    2,           0,   589, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  47 ),
	CG_FRAME(    0,    0,    9,           0,   590, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    7,           0,   591, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    5, END_OF_DATA,   592, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl2c_crouch_k_01 ),
};

/* Char_Crouch_K 2 (heavy) */
const Sint32 pl2c_crouch_k_02[] = {
	CG_FRAME(    0,    0,    3,           0,   593, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    1,           0,   594, 0xa0,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  48 ),
	CG_FRAME(    0,    0,    2,           0,   594, 0xa0,   0x00,  ATK,  0x00, 0x29 | CSE,    0,   0,     0,      0,  48 ),
};

const Sint32 pl2c_crouch_k_02_loop[] = {
	CG_FRAME(    0,    1,    2,           0,   596, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,    2,           0,   597, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,    2,           0,   598, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    1,    2,           0,   599, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2,           0,   600, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   601, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_NEXT( pl2c_crouch_k_02_loop ),
};

/* Char_Crouch_K 3: kick_move */
const Sint32 pl2c_crouch_k_03[] = {
	CG_FRAME(    0,    0,    3,           0,   602, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,   603, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,   604, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,   605, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    3,           0,   606, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  49 ),
	CG_FRAME(    0,    0,    4,           0,   591, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  46 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   592, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  44 ),
	CG_NEXT( pl2c_crouch_k_03 ),
};

/* Char_Jump_P 0 (light)
   Char_Jump_P 3 */
const Sint32 pl2c_jump_p_00[] = {
	CG_FRAME(    0,    0,    2,           0,   628, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME( 0x80,    0,    2,           0,   629, 0xa0,   0x00,  ATK,  0x00,       0x1e,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    1,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  53 ),
	CG_FRAME( 0x80,    0,    2,           0,   629, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x80,    0,    1,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME( 0x81,    0,    2,           0,   629, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  52 ),
	CG_FRAME(    0,    0,    6,           0,   642, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   643, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  51 ),
	CG_NEXT( pl2c_jump_p_00 ),
};

/* Char_Jump_P 1 (medium)
   Char_Jump_P 4 */
const Sint32 pl2c_jump_p_01[] = {
	CG_FRAME(    0,    0,    4,           0,   621, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME( 0x80,    0,    1,           0,   667, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    1,           0,   668, 0xa0,   0x00,  ATK,  0x00,       0x1f,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x80,    0,    2,           0,   669, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME( 0x82,    0,    2,           0,   670, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  56 ),
	CG_FRAME(    0,    0,    2,           0,   671, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   672, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   673, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   674, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    1,           0,   675, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  55 ),
	CG_FRAME(    0,    0,    4,           0,   676, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   637, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  54 ),
	CG_NEXT( pl2c_jump_p_01 ),
};

/* Char_Jump_P 2 (heavy)
   Char_Jump_P 5 */
const Sint32 pl2c_jump_p_02[] = {
	CG_FRAME(    0,    0,    1,           0,   607, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    1,           0,   608, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    1,           0,   609, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    1,           0,   610, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   611, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    2,           0,   612, 0xa0,   0x00,  ATK,  0x00,       0x04,    0,   0,     0,      0,  58 ),
	CG_FRAME(    0,    0,    1,           0,   613, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    2,           0,   614, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    1,           0,   615, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,    2,           0,   616, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    1,           0,   617, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,    2,           0,   618, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x80,    0,    2,           0,   616, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME( 0x81,    0,    2,           0,   618, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  59 ),
	CG_FRAME(    0,    0,    4,           0,   619, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  60 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   620, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  58 ),
	CG_NEXT( pl2c_jump_p_02 ),
};

/* Char_Jump_K 0 (light) */
const Sint32 pl2c_jump_k_00[] = {
	CG_FRAME(    0,    0,    2,           0,   644, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,   10,           0,   645, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  62 ),
	CG_FRAME(    0,    0,    1,           0,   646, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   645, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   646, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   645, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    1,           0,   646, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  63 ),
	CG_FRAME(    0,    0,    2,           0,   644, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   647, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  61 ),
	CG_NEXT( pl2c_jump_k_00 ),
};

/* Char_Jump_K 1 (medium)
   Char_Jump_K 4 */
const Sint32 pl2c_jump_k_01[] = {
	CG_FRAME(    0,    0,    1,           0,   648, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME(    0,    0,    1,           0,   649, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  64 ),
	CG_FRAME( 0x80,    0,    1,           0,   650, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  65 ),
	CG_FRAME( 0x81,    0,    7,           0,   651, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  66 ),
	CG_FRAME(    0,    0,    1,           0,   652, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    1,           0,   653, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   654, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    4,           0,   655, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    6,           0,   656, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_FRAME(    0,    0,    7, END_OF_DATA,   657, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  67 ),
	CG_NEXT( pl2c_jump_k_01 ),
};

/* Char_Jump_K 2 (heavy)
   Char_Jump_K 5 */
const Sint32 pl2c_jump_k_02[] = {
	CG_FRAME(    0,    0,    1,           0,   659, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    5,           0,   660, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME( 0x80,    0,    1,           0,   661, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,    1,           0,   662, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,    1,           0,   663, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,    1,           0,   661, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x80,    0,    1,           0,   662, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME( 0x83,    0,    1,           0,   663, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  69 ),
	CG_FRAME(    0,    0,    3,           0,   664, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    3,           0,   665, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  70 ),
	CG_FRAME(    0,    0,    5,           0,   666, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   658, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  68 ),
	CG_NEXT( pl2c_jump_k_02 ),
};

/* Char_Jump_K 3: jump_atck */
const Sint32 pl2c_jump_k_03[] = {
	CG_FRAME(    0,    0,    1,           0,   677, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME(    0,    0,    1,           0,   679, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  71 ),
	CG_FRAME( 0x80,    0,    2,           0,   680, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    1,           0,   681, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    2,           0,   680, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    1,           0,   681, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    2,           0,   680, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x80,    0,    1,           0,   681, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME( 0x82,    0,    2,           0,   680, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  72 ),
	CG_FRAME(    0,    0,    3,           0,   679, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3,           0,   678, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   677, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  73 ),
	CG_NEXT( pl2c_jump_k_03 ),
};

const Sint32 pl2c_unused_00[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4,           0,   863, 0xa0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   864, 0xa0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	0x00002f84,
};

/* Char_Damage 0: dm_freeze, dm_freeze_air, caught_dm */
const Sint32 pl2c_damage_00[] = {
	CG_FRAME( 0x80,    0,    3,           0,   862, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,    3,           0,   863, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x82,    0,    2,           0,   864, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   865, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2c_damage_00 ),
};

/* Char_Damage 1 */
const Sint32 pl2c_damage_01[] = {
	CG_FRAME( 0x80,    0,    3,           0,   870, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,    3,           0,   870, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x82,    0,    6,           0,   869, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   868, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   868, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2c_damage_01 ),
};

/* Char_Damage 2 */
const Sint32 pl2c_damage_02[] = {
	CG_FRAME( 0x80,    0,    2,           0,   876, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,    4,           0,   875, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x82,    0,    4,           0,   876, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   874, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   873, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   875, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   872, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   874, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   872, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   874, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2c_damage_02 ),
};

/* Char_Damage 3 */
const Sint32 pl2c_damage_03[] = {
	CG_FRAME( 0x80,    0,    2,           0,   879, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x82,    0,    3,           0,   880, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3,           0,   881, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   882, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2c_damage_03 ),
};

/* Char_Damage 10: dm_ko_blow */
const Sint32 pl2c_damage_10[] = {
	CG_FRAME( 0x80,    0,    2,           0,   879, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x82,    0,    3,           0,   880, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   881, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   882, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_10 ),
};

/* Char_Damage 4 */
const Sint32 pl2c_damage_04[] = {
	CG_FRAME(    0,    0,    2,           0,   886, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    4,           0,   884, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    8,           0,   883, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    0,           0,   884, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   886, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2c_damage_04 ),
};

/* Char_Damage 11 */
const Sint32 pl2c_damage_11[] = {
	CG_FRAME( 0x80,    0,    2,           0,   883, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    0,    1,           0,   885, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x82,    0,    1,           0,   886, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   884, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   884, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   886, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_11 ),
};

/* Char_Damage 5 */
const Sint32 pl2c_damage_05[] = {
	CG_FRAME( 0x80,    0,    2,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x80,    0,    2,           0,   891, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME( 0x82,    0,    2,           0,   888, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   891, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   890, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   889, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1,           0,   889, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1,           0,   889, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    1,           0,   891, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   892, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  74 ),
	CG_NEXT( pl2c_damage_05 ),
};

/* Char_Damage 12 */
const Sint32 pl2c_damage_12[] = {
	CG_FRAME( 0x80,    0,    1,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x80,    0,    1,           0,   891, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME( 0x82,    0,    1,           0,   888, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   891, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   890, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   889, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1,           0,   887, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   889, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_12 ),
};

/* Char_Damage 6 */
const Sint32 pl2c_damage_06[] = {
	CG_FRAME( 0x80,    0,    2,           0,   893, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x82,    0,    3,           0,   895, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3,           0,   894, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   896, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl2c_damage_06 ),
};

/* Char_Damage 7 */
const Sint32 pl2c_damage_07[] = {
	CG_FRAME( 0x80,    0,    3,           0,   898, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x80,    0,    3,           0,   900, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x82,    0,    2,           0,   898, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   899, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   897, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   900, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   897, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   899, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl2c_damage_07 ),
};

/* Char_Damage 8 */
const Sint32 pl2c_damage_08[] = {
	CG_FRAME( 0x80,    0,    2,           0,   902, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x80,    0,    2,           0,   905, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME( 0x82,    0,    2,           0,   902, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   905, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   904, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   905, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   903, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   905, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   903, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   905, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    2,           0,   901, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    1,           0,   903, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   905, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  75 ),
	CG_NEXT( pl2c_damage_08 ),
};

/* Char_Damage 13: dm_snap_fly, dm_air
   Char_Damage 14
   Char_Damage 15
   Char_Damage 34: dm_snap_fly, dm_snap_down */
const Sint32 pl2c_damage_13[] = {
	CG_FRAME( 0x80,    0,    4,           0,   912, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl2c_damage_13_loop[] = {
	CG_FRAME( 0x82,    0,    4, END_OF_DATA,   913, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2c_damage_13_loop ),
};

/* Char_Damage 9: dm_sweep */
const Sint32 pl2c_damage_09[] = {
	CG_FRAME(    0,    0,    2,           0,   880, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   915, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   916, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   917, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   918, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   919, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   922, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   920, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   922, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    1,           0,   923, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    1,           0,   932, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
};

const Sint32 pl2c_damage_09_loop[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   921, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2c_damage_09_loop ),
};

const Sint32 pl2c_unused_01[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2, END_OF_DATA,   924, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	0x000037cc,
};

const Sint32 pl2c_unused_02[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    2, END_OF_DATA,   925, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	0x000037e4,
};

/* Char_Damage 16: dm_blow_char_set (16 + no), dm_spark, caught_shock_init */
const Sint32 pl2c_damage_16[] = {
	CG_FRAME(    0,    0,    2,           0,   924, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   925, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_16 ),
};

/* Char_Damage 17: dm_07_1 */
const Sint32 pl2c_damage_17[] = {
	CG_FRAME(    0,    0,    2,           0,   926, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   926, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_17 ),
};

/* Char_Damage 18 */
const Sint32 pl2c_damage_18[] = {
	CG_FRAME(    0,    0,    2,           0,   927, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   927, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_18 ),
};

/* Char_Damage 19 */
const Sint32 pl2c_damage_19[] = {
	CG_FRAME(    0,    0,    2,           0,   928, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   928, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_19 ),
};

/* Char_Damage 20 */
const Sint32 pl2c_damage_20[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   933, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_20 ),
};

/* Char_Damage 21: dm_fall_char_set (21 + no) */
const Sint32 pl2c_damage_21[] = {
	CG_FRAME(    0,    0,    2,           0,   924, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   925, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
};

/* Char_Damage 22 */
const Sint32 pl2c_damage_22[] = {
	CG_FRAME(    0,    0,    2,           0,   926, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   926, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
};

/* Char_Damage 23 */
const Sint32 pl2c_damage_23[] = {
	CG_FRAME(    0,    0,    2,           0,   927, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   927, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
};

/* Char_Damage 24 */
const Sint32 pl2c_damage_24[] = {
	CG_FRAME(    0,    0,    2,           0,   928, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    2,           0,   928, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
};

/* Char_Damage 25 */
const Sint32 pl2c_damage_25[] = {
	CG_FRAME(    0,    0,    2, END_OF_DATA,   933, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_20 ),
};

/* Char_Damage 26: dm_buttobi_fly, dm_07_1, dm_away_fly, dm_bound_hop, ... */
const Sint32 pl2c_damage_26[] = {
	CG_FRAME(    0,    0,    2,           0,   929, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   930, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   931, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   932, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2,           0,   931, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   953, 0x40,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2c_damage_26 ),
};

/* Char_Damage 27
   Char_Damage 30: act_air_recover, act_change_out_init, caught_blow_fly, caught_blow_fix_fly, ... */
const Sint32 pl2c_damage_27[] = {
	CG_FRAME(    0,    0,    4,           0,   956, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   935, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2,           0,   936, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   212, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   213, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
};

const Sint32 pl2c_damage_27_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   214, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   215, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl2c_damage_27_loop ),
};

/* Char_Damage 32: act_escape */
const Sint32 pl2c_damage_32[] = {
	CG_FRAME(    0,    0,    4,           0,   934, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    4,           0,   935, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   936, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  87 ),
	CG_NEXT( pl2c_damage_32 ),
};

/* Char_Damage 33: act_escape */
const Sint32 pl2c_damage_33[] = {
	CG_FRAME(    0,    0,    6,           0,   934, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   954, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   955, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl2c_damage_33 ),
};

/* Char_Damage 28: dm_away_init, dm_bound_fly, dm_piyo, dm_away */
const Sint32 pl2c_damage_28[] = {
	CG_FRAME(    0,    0,    4,           0,   943, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   944, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   945, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   946, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   947, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   948, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_28 ),
};

const Sint32 pl2c_unused_03[] = {	/* nothing reaches it */
	CG_FRAME(    0,    0,    4, END_OF_DATA,   937, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	0x00003bc0,
};

/* Char_Damage 29: dm_away_fly, dm_bound_hop, dm_bound, caught_spin_fly, ... */
const Sint32 pl2c_damage_29[] = {
	CG_FRAME(    0,    0,    4,           0,   937, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   938, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   939, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   940, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4,           0,   941, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   942, 0x40,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  76 ),
	CG_NEXT( pl2c_damage_29 ),
};

/* Char_Damage 31: act_down, dm_dizzy_down */
const Sint32 pl2c_damage_31[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   953, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  77 ),
	CG_NEXT( pl2c_damage_31 ),
};

/* Char_Caught 0 */
const Sint32 pl2c_caught_00[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   863, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_00 ),
};

/* Char_Caught 1 */
const Sint32 pl2c_caught_01[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   868, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_01 ),
};

/* Char_Caught 2 */
const Sint32 pl2c_caught_02[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   876, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_02 ),
};

/* Char_Caught 3 */
const Sint32 pl2c_caught_03[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   879, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_03 ),
};

/* Char_Caught 4 */
const Sint32 pl2c_caught_04[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   885, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_04 ),
};

/* Char_Caught 5 */
const Sint32 pl2c_caught_05[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   889, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl2c_caught_05 ),
};

/* Char_Caught 6 */
const Sint32 pl2c_caught_06[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   895, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl2c_caught_06 ),
};

/* Char_Caught 7 */
const Sint32 pl2c_caught_07[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   899, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl2c_caught_07 ),
};

/* Char_Caught 8 */
const Sint32 pl2c_caught_08[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   903, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  81 ),
	CG_NEXT( pl2c_caught_08 ),
};

/* Char_Caught 9 */
const Sint32 pl2c_caught_09[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   879, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_09 ),
};

/* Char_Caught 10 */
const Sint32 pl2c_caught_10[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   885, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  80 ),
	CG_NEXT( pl2c_caught_10 ),
};

/* Char_Caught 11 */
const Sint32 pl2c_caught_11[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   889, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  79 ),
	CG_NEXT( pl2c_caught_11 ),
};

/* Char_Caught 12 */
const Sint32 pl2c_caught_12[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   912, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0, 116 ),
	CG_NEXT( pl2c_caught_12 ),
};

/* Char_Caught 13 */
const Sint32 pl2c_caught_13[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   924, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2c_caught_13 ),
};

/* Char_Caught 14 */
const Sint32 pl2c_caught_14[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   926, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl2c_caught_14 ),
};

/* Char_Caught 15 */
const Sint32 pl2c_caught_15[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   927, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  83 ),
	CG_NEXT( pl2c_caught_15 ),
};

/* Char_Caught 16 */
const Sint32 pl2c_caught_16[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   928, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  86 ),
	CG_NEXT( pl2c_caught_16 ),
};

/* Char_Caught 17 */
const Sint32 pl2c_caught_17[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   933, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2c_caught_17 ),
};

/* Char_Caught 18 */
const Sint32 pl2c_caught_18[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   943, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2c_caught_18 ),
};

/* Char_Caught 19 */
const Sint32 pl2c_caught_19[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   937, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  85 ),
	CG_NEXT( pl2c_caught_19 ),
};

/* Char_Caught 20 */
const Sint32 pl2c_caught_20[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   953, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0, 117 ),
	CG_NEXT( pl2c_caught_20 ),
};

/* Char_Caught 21 */
const Sint32 pl2c_caught_21[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA,   919, 0x60,   0xab,    0,  0x00,          0,    0,   0,     0,      0,  82 ),
	CG_NEXT( pl2c_caught_21 ),
};

/* Char_Special 5
   Char_Special 6
   Char_Special 7: shl2c_01_main */
const Sint32 pl2c_special_05[] = {
	CG_FRAME(    0,    0,    4,           0, 32768, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0, 32769, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32770, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32771, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32772, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32773, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32774, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32775, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_special_05_loop[] = {
	CG_FRAME(    0,    0,    1,           0, 32776, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32777, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_05_loop ),
};

/* Char_Special 8: shl2c_01_start (8 + sub_type) */
const Sint32 pl2c_special_08[] = {
	CG_FRAME(    0,    0,    8,           0, 32778, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32779, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32780, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_special_08_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32781, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2c_special_08_loop ),
};

/* Char_Special 9 */
const Sint32 pl2c_special_09[] = {
	CG_FRAME(    0,    0,    4,           0, 32782, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32783, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32784, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_special_09_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32785, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2c_special_09_loop ),
};

/* Char_Special 10 */
const Sint32 pl2c_special_10[] = {
	CG_FRAME(    0,    0,    4,           0, 32786, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32787, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32788, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_special_10_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32789, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2c_special_10_loop ),
};

/* Char_Special 11 */
const Sint32 pl2c_special_11[] = {
	CG_FRAME(    0,    0,    4,           0, 32790, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32791, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32792, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_special_11_loop[] = {
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32793, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  89 ),
	CG_NEXT( pl2c_special_11_loop ),
};

/* Char_Special 12 */
const Sint32 pl2c_special_12[] = {
	CG_FRAME(    0,    0,    4,           0, 32794, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32795, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32796, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32797, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32798, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_12 ),
};

/* Char_Special 13 */
const Sint32 pl2c_special_13[] = {
	CG_FRAME(    0,    0,    4,           0, 32799, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32800, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32801, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32802, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32803, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32804, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32805, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32806, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_13 ),
};

/* Char_Special 14 */
const Sint32 pl2c_special_14[] = {
	CG_FRAME(    0,    0,    4,           0, 32807, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32808, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32809, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32810, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32811, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_14 ),
};

/* Char_Special 15 */
const Sint32 pl2c_special_15[] = {
	CG_FRAME(    0,    0,    4,           0, 32812, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32813, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32814, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32815, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32816, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32817, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32818, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32819, 0xb0,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_15 ),
};

/* Char_Appear 0: pl2c_appear */
const Sint32 pl2c_appear_00[] = {
	CG_FRAME(    0,    0,    2,           0,   378, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   379, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   380, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   381, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   382, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   383, 0x80,   0x00,    0,  0x00,       0x0a,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   384, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   385, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   386, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   387, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   388, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   389, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   390, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   391, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   392, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   393, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   394, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   395, 0x80,   0x00,    0,  0x00,       0x23,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   396, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   397, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   398, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   399, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   400, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   401, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   402, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   403, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   404, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   405, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   406, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   407, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   408, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   409, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   410, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   411, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   412, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   413, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   414, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   415, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   416, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   417, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   418, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   419, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   420, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   421, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   422, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   423, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   424, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   425, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_appear_00 ),
};

/* Char_Win 0: pl2c_win */
const Sint32 pl2c_win_00[] = {
	CG_FRAME(    0,    0,    3,           0,   312, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   313, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   314, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   315, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   316, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   317, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   318, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   319, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   320, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   321, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   322, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   323, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   324, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   325, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   326, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   327, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   328, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   329, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   330, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   331, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   332, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   333, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,   334, 0x80,   0x00,    0,  0x00,       0x0f,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2c_win_00_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   335, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   336, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   337, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   338, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   339, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   340, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   341, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   342, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_win_00_loop ),
};

/* Char_Win 1: pl2c_win, sp_chouhatsu (Taunt) */
const Sint32 pl2c_win_01[] = {
	CG_FRAME(    0,    0,    1,           0,   343, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0, 0x0101,   3 ),
};

const Sint32 pl2c_win_01_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_NEXT( pl2c_win_01_loop ),
};

/* Char_Win 2: pl2c_win */
const Sint32 pl2c_win_02[] = {
	CG_FRAME(    0,    0,    4,           0,   346, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   347, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   348, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   349, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   350, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   351, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   352, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   353, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   354, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   355, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   356, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   357, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2c_win_02_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   358, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   359, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   360, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   361, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   362, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   363, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   364, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   365, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   366, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   367, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   368, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   369, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   370, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   371, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   372, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   373, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_win_02_loop ),
};

/* Char_Win 3: pl2c_win */
const Sint32 pl2c_win_03[] = {
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_NEXT( pl2c_win_03 ),
};

/* Char_Fly 0: fly_init (Flight) */
const Sint32 pl2c_fly_00[] = {
	CG_FRAME(    0,    0,    8,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,    7,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  90 ),
	CG_NEXT( pl2c_fly_00 ),
};

/* Char_Fly 1: act_fly */
const Sint32 pl2c_fly_01[] = {
	CG_FRAME(    0,    0,   10,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   107, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   108, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   109, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   110, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   111, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   118, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   119, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   120, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   121, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   122, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   123, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   124, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   125, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   126, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   127, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   128, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   129, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   112, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   113, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   114, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   115, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   116, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   117, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   131, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   132, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   133, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   134, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   135, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   136, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   137, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   106, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   138, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   139, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   140, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   141, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   142, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   143, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   144, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   145, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   146, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   147, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   148, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   149, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   150, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   151, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10,           0,   152, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_FRAME(    0,    0,   10, END_OF_DATA,   153, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0100,  91 ),
	CG_NEXT( pl2c_fly_01 ),
};

/* Char_Fly 8 */
const Sint32 pl2c_fly_08[] = {
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
};

const Sint32 pl2c_fly_08_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   157, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   158, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   159, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   160, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   161, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   162, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   163, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   164, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_08_loop ),
};

/* Char_Fly 4 */
const Sint32 pl2c_fly_04[] = {
	CG_FRAME(    0,    0,    3,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
};

const Sint32 pl2c_fly_04_loop[] = {
	CG_FRAME(    0,    0,    3,           0,   173, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   174, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   175, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   176, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   177, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   178, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   179, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   180, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_04_loop ),
};

/* Char_Fly 16 */
const Sint32 pl2c_fly_16[] = {
	CG_FRAME(    0,    0,    3,           0,   156, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   155, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   154, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   165, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   166, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   167, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   168, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   169, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_01 ),
};

/* Char_Fly 12 */
const Sint32 pl2c_fly_12[] = {
	CG_FRAME(    0,    0,    3,           0,   172, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   171, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   170, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   181, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3,           0,   182, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   183, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_01 ),
};

/* Char_Fly 9 */
const Sint32 pl2c_fly_09[] = {
	CG_FRAME(    0,    0,    4,           0,   733, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   734, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_09 ),
};

/* Char_Fly 2 */
const Sint32 pl2c_fly_02[] = {
	CG_FRAME(    0,    0,    4,           0,   731, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   732, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_02 ),
};

/* Char_Fly 3 */
const Sint32 pl2c_fly_03[] = {
	CG_FRAME(    0,    0,    4,           0,   735, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   736, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_02 ),
};

/* Char_Fly 7 */
const Sint32 pl2c_fly_07[] = {
	CG_FRAME(    0,    0,    4,           0,   739, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   740, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_07 ),
};

/* Char_Fly 6 */
const Sint32 pl2c_fly_06[] = {
	CG_FRAME(    0,    0,    4,           0,   729, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   730, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_06 ),
};

/* Char_Fly 5 */
const Sint32 pl2c_fly_05[] = {
	CG_FRAME(    0,    0,    4,           0,   737, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   738, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_05 ),
};

/* Char_Fly 13
   Char_Fly 14
   Char_Fly 15 */
const Sint32 pl2c_fly_13[] = {
	CG_FRAME(    0,    0,    4,           0,   746, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   747, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   748, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   749, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
};

/* Char_Fly 10
   Char_Fly 11
   Char_Fly 17 */
const Sint32 pl2c_fly_10[] = {
	CG_FRAME(    0,    0,    4,           0,   741, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   742, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4,           0,   743, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   744, 0x90,   0x00,    0,  0x00,          0,    0,   0,     0,      0,  91 ),
	CG_NEXT( pl2c_fly_01 ),
};

/* Char_Dizzy 0: act_dizzy */
const Sint32 pl2c_dizzy_00[] = {
	CG_FRAME(    0,    0,    4,           0,   949, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   950, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   951, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   952, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   949, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   950, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   949, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   950, 0x50,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_dizzy_00 ),
};

/* Char_Get_Up 0: dm_dizzy_wait */
const Sint32 pl2c_get_up_00[] = {
	CG_FRAME(    0,    0,    4,           0,   426, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   428, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x6b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_get_up_00 ),
};

/* Char_Get_Up 1 */
const Sint32 pl2c_get_up_01[] = {
	CG_FRAME(    0,    0,    2,           0,   426, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   428, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   429, 0x80,   0x6b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   430, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_get_up_01 ),
};

/* Char_Get_Up 2 */
const Sint32 pl2c_get_up_02[] = {
	CG_FRAME(    0,    0,    4,           0,   426, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,   16,           0,   427, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   428, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x6b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_get_up_02 ),
};

/* Char_Get_Up 3 */
const Sint32 pl2c_get_up_03[] = {
	CG_FRAME(    0,    0,    4,           0,   426, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   427, 0x80,   0xab,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   428, 0x80,   0x8b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   429, 0x80,   0x6b,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   430, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   431, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3,           0,   432, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   433, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_get_up_03 ),
};

/* Char_Extra 5: t_kick_init
   group 25 0
   group 25 1 */
const Sint32 pl2c_extra_05[] = {
	CG_FRAME(    0,    0,    4,           0,   660, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  68 ),
};

const Sint32 pl2c_extra_05_loop[] = {
	CG_FRAME( 0x80,    0,    1,           0,   661, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,    1,           0,   662, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_FRAME( 0x80,    0,    1, END_OF_DATA,   663, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 114 ),
	CG_NEXT( pl2c_extra_05_loop ),
};

/* Char_Extra 6: t_kick_jump */
const Sint32 pl2c_extra_06[] = {
	CG_FRAME(    0,    0,    2,           0,   750, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   759, 0xb0,   0x00,  ATE,  0x00,       0x0b,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    6,           0,   760, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   769, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   773, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   774, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   775, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   776, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   777, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   778, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   779, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   780, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   781, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   782, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_extra_06 ),
};

/* Char_Extra 7: pl2c_upper */
const Sint32 pl2c_extra_07[] = {
	CG_FRAME(    0,    0,    1,           0,   576, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   577, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3,           0,   578, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   579, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   580, 0xa0,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   581, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   582, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  42 ),
	CG_FRAME(    0,    0,    3,           0,   583, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   584, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   585, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   10,           0,   586, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   587, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl2c_extra_07 ),
};

/* Char_Special 0: sp_disrupt (EM Disruptor) */
const Sint32 pl2c_special_00[] = {
	CG_FRAME(    0,    0,    4,           0,   785, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    2,           0,   786, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   790, 0xb0,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   792, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   793, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   794, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   795, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   796, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   797, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   798, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    1,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    2,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    3,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    0,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    0,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    4,    1,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x82,    4,    1,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   807, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    8,    1,           0,   808, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   809, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x82,    9,    1,           0,   810, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    1,           0,   811, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    5,    1,           0,   812, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   811, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    6,    2,           0,   812, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    7,    2,           0,   814, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   793, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   795, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   797, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   815, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   816, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2c_special_00 ),
};

/* Char_Extra 4 */
const Sint32 pl2c_extra_04[] = {
	CG_FRAME(    0,    0,    4,           0,   785, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   786, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   790, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,    1,           0,   792, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,    1,           0,   793, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,    1,           0,   794, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME( 0x80,    0,    1,           0,   795, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME( 0x80,    0,    1,           0,   796, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME( 0x80,    0,    1,           0,   797, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME( 0x80,    0,    1,           0,   798, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    0,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    0,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   805, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   806, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   805, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x82,    0,    1,           0,   806, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   807, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   808, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   809, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x82,    0,    1,           0,   810, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   811, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   812, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   811, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    2,           0,   812, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   814, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   793, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   795, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   797, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   815, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   816, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_extra_04 ),
};

/* Char_Special 16: sp_disrupt (EM Disruptor) */
const Sint32 pl2c_special_16[] = {
	CG_FRAME(    0,    0,    1,           0,   785, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  92 ),
	CG_FRAME(    0,    0,    1,           0,   786, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    1,           0,   789, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    1,           0,   790, 0xb0,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    1,           0,   791, 0xb0,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   792, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   793, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   794, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   795, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   796, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   797, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   798, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    1,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    2,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    3,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    0,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    0,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    4,    1,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x80,    0,    1,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME( 0x82,    0,    1,           0,   806, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   811, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    5,    2,           0,   812, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    7,    2,           0,   814, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   793, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   795, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   797, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4,           0,   815, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   816, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  93 ),
	CG_NEXT( pl2c_special_16 ),
};

/* Char_Extra 2: fly_start5 */
const Sint32 pl2c_extra_02[] = {
	CG_FRAME(    0,    0,    1,           0,   785, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   786, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   789, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   790, 0xb0,   0x00,  ATK,  0x00,       0x14,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   791, 0xb0,   0x00,  ATK,  0x00,       0x22,    0,   0,     0,      0,   0 ),
	CG_FRAME( 0x80,    0,    1,           0,   792, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,    1,           0,   793, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  94 ),
	CG_FRAME( 0x80,    0,    1,           0,   794, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME( 0x80,    0,    1,           0,   795, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  95 ),
	CG_FRAME( 0x80,    0,    1,           0,   796, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME( 0x80,    0,    1,           0,   797, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  96 ),
	CG_FRAME( 0x80,    0,    1,           0,   798, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  97 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   799, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   800, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  98 ),
	CG_FRAME( 0x80,    0,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   801, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   802, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    0,           0,   803, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    0,           0,   804, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   805, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   806, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x80,    0,    1,           0,   805, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME( 0x82,    0,    1,           0,   806, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  99 ),
	CG_FRAME(    0,    0,    1,           0,   811, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   812, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   814, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   813, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   793, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   795, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   797, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   789, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,   791, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   815, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   816, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_extra_02 ),
};

/* Char_Extra 3: fly_start5
   Char_Special 1: grav_init (Hyper Grav)
   Char_Special 17 */
const Sint32 pl2c_extra_03[] = {
	CG_FRAME(    0,    0,    2,           0,   750, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   752, 0xb0,   0x00,  ATK,  0x00,       0x16,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   753, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   754, 0xb0,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   755, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   756, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   757, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   758, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   759, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   760, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   761, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   762, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   763, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   764, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   765, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   766, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2c_extra_03_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   767, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   768, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   769, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   770, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   771, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   772, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   773, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   774, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   775, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   776, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   777, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   778, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   779, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   780, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   781, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   782, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_extra_03_loop ),
};

/* Char_Special 2: shl2c_03_main */
const Sint32 pl2c_special_02[] = {
	CG_FRAME(    0,    0,    1,           0, 32841, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32842, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32843, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32844, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32845, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32846, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32836, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32837, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32838, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32839, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0, 32840, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32835, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_02 ),
};

/* Char_Catch 8
   Char_Special 3: shl2c_03_start */
const Sint32 pl2c_catch_08[] = {
	CG_FRAME(    0,    0,    1,           0, 32827, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0037, 100 ),
	CG_FRAME(    0,    0,    1,           0, 32831, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0038, 100 ),
	CG_FRAME(    0,    0,    1,           0, 32828, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0037, 100 ),
	CG_FRAME(    0,    0,    1,           0, 32832, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0038, 100 ),
	CG_FRAME(    0,    0,    1,           0, 32829, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0037, 100 ),
	CG_FRAME(    0,    0,    1,           0, 32833, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0038, 100 ),
	CG_FRAME(    0,    0,    1,           0, 32830, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0037, 100 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA, 32834, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0038, 100 ),
	CG_NEXT( pl2c_catch_08 ),
};

/* Char_Special 4: shl2c_03_end */
const Sint32 pl2c_special_04[] = {
	CG_FRAME(    0,    0,    1,           0, 32846, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32845, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32844, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32843, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0, 32842, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA, 32841, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_special_04 ),
};

/* Char_Extra 1: shl2c_00_punch */
const Sint32 pl2c_extra_01[] = {
	CG_FRAME(    0,    0,    1,           0,   552, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   553, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   554, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   555, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   556, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   557, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   560, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   558, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   559, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   561, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   562, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   563, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   564, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    0,           0,   570, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   569, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   557, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   560, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   570, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   565, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   570, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   565, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   566, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   567, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   560, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    0,           0,   570, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    0,           0,   568, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   569, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   571, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    1,           0,   572, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 101 ),
	CG_FRAME(    0,    0,    2,           0,   573, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    1,           0,   574, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    0,           0,   566, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   575, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_extra_01 ),
};

/* Char_Extra 0: shl2c_00_jblast */
const Sint32 pl2c_extra_00[] = {
	CG_FRAME(    0,    0,    3,           0,   638, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    3,           0,   639, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    3,           0,   640, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   641, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 102 ),
	CG_NEXT( pl2c_extra_00 ),
};

/* Char_Catch 1 */
const Sint32 pl2c_catch_01[] = {
	CG_FRAME(    0,    0,    4,           0,   688, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d, 103 ),
	CG_FRAME(    0,    0,    4,           0,   689, 0x80,   0x00,    0,  0x00,       0x13,    0,   0,     0, 0x000e, 103 ),
	CG_FRAME(    0,    0,    4,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f, 103 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010, 103 ),
	CG_NEXT( pl2c_catch_01 ),
};

/* Char_Catch 2 */
const Sint32 pl2c_catch_02[] = {
	CG_FRAME(    0,    0,    4,           0,   684, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0013, 103 ),
	CG_FRAME(    0,    0,    4,           0,   685, 0x80,   0x00,    0,  0x00,       0x13,    0,   0,     0, 0x0014, 103 ),
	CG_FRAME(    0,    0,    4,           0,   686, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0015, 103 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   687, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0016, 103 ),
	CG_NEXT( pl2c_catch_02 ),
};

/* Char_Catch 3 */
const Sint32 pl2c_catch_03[] = {
	CG_FRAME(    0,    0,    4,           0,   692, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0019, 103 ),
	CG_FRAME(    0,    0,    4,           0,   693, 0x80,   0x00,    0,  0x00,       0x13,    0,   0,     0, 0x001a, 103 ),
	CG_FRAME(    0,    0,    4,           0,   694, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001b, 103 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   695, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001c, 103 ),
	CG_NEXT( pl2c_catch_03 ),
};

/* Char_Catch 4 */
const Sint32 pl2c_catch_04[] = {
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001d, 103 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001e, 103 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x001f, 103 ),
	CG_FRAME(    0,    0,    2,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0020, 103 ),
	CG_FRAME(    0,    0,    2,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0021, 103 ),
	CG_FRAME(    0,    0,    2,           0,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0022, 103 ),
	CG_FRAME(    0,    1,    4,           0,   702, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0023, 103 ),
	CG_FRAME(    0,    0,    4,           0,   703, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4,           0,   704, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   705, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2c_catch_04 ),
};

/* Char_Catch 5 */
const Sint32 pl2c_catch_05[] = {
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0024, 103 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0025, 103 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0026, 103 ),
	CG_FRAME(    0,    0,    2,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0027, 103 ),
	CG_FRAME(    0,    0,    2,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0028, 103 ),
	CG_FRAME(    0,    0,    2,           0,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0029, 103 ),
	CG_FRAME(    0,    1,    4,           0,   706, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x002a, 103 ),
	CG_FRAME(    0,    0,    4,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2c_catch_05 ),
};

/* Char_Catch 6 */
const Sint32 pl2c_catch_06[] = {
	CG_FRAME(    0,    0,    4,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002b, 103 ),
	CG_FRAME(    0,    0,    4,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002c, 103 ),
	CG_FRAME(    0,    0,    2,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002d, 103 ),
	CG_FRAME(    0,    0,    2,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002e, 103 ),
	CG_FRAME(    0,    0,    2,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x002f, 103 ),
	CG_FRAME(    0,    0,    2,           0,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0030, 103 ),
	CG_FRAME(    0,    1,    4,           0,   710, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0031, 103 ),
	CG_FRAME(    0,    0,    4,           0,   711, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4,           0,   712, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   713, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2c_catch_06 ),
};

/* Char_Catch 7 */
const Sint32 pl2c_catch_07[] = {
	CG_FRAME(    0,    0,    6,           0,   714, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0032, 103 ),
	CG_FRAME(    0,    0,    3,           0,   715, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0033, 103 ),
	CG_FRAME(    0,    0,    3,           0,   716, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0033, 103 ),
	CG_FRAME(    0,    0,    3,           0,   718, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0034, 103 ),
	CG_FRAME(    0,    0,    4,           0,   719, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0035, 104 ),
	CG_FRAME(    0,    1,    4,           0,   720, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x0036, 104 ),
	CG_FRAME(    0,    0,    4,           0,   721, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,   722, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,   723, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,   724, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4,           0,   725, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   726, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 104 ),
	CG_NEXT( pl2c_catch_07 ),
};

/* Char_Catch 0 */
const Sint32 pl2c_catch_00[] = {
	CG_FRAME(    1,    1,    4,           0,   688, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000d, 103 ),
	CG_FRAME(    1,    0,    4,           0,   689, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000e, 103 ),
	CG_FRAME(    1,    0,    4,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f, 103 ),
	CG_FRAME(    1,    0,    4,           0,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010, 103 ),
	CG_FRAME(    1,    0,    4,           0,   690, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x000f, 103 ),
	CG_FRAME(    0,    0,    4,           0,   691, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0010, 103 ),
	CG_FRAME(    0,    0,    1,           0,   696, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0024, 103 ),
	CG_FRAME(    0,    0,    1,           0,   697, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0025, 103 ),
	CG_FRAME(    0,    0,    4,           0,   698, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0026, 103 ),
	CG_FRAME(    0,    0,    4,           0,   699, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0027, 103 ),
	CG_FRAME(    0,    0,    4,           0,   700, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0028, 103 ),
	CG_FRAME(    0,    0,    4,           0,   701, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0029, 103 ),
	CG_FRAME(    0,    2,    6,           0,   706, 0x80,   0x00,    0,  0x00,       0x03,    0,   0,     0, 0x002a, 103 ),
	CG_FRAME(    0,    0,    4,           0,   707, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_FRAME(    0,    0,    4,           0,   708, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
};

const Sint32 pl2c_catch_00_loop[] = {
	CG_FRAME(    0,    0,    8, END_OF_DATA,   709, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 103 ),
	CG_NEXT( pl2c_catch_00_loop ),
};

/* Char_Effect 0 */
const Sint32 pl2c_effect_00[] = {
	CG_FRAME(    0,    0,    2,           0,  1012, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2,           0,  1013, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,  1014, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_effect_00 ),
};

/* Char_Super 0 */
const Sint32 pl2c_super_00[] = {
	CG_FRAME(    0,    0,    6,           0,   841, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   842, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   843, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   844, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   845, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   842, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   843, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,   844, 0xc0,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   845, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   846, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl2c_super_00 ),
};

/* Char_Super 1: shock_start (Magnetic Shockwave) */
const Sint32 pl2c_super_01[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
};

const Sint32 pl2c_super_01_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   852, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl2c_super_01_loop ),
};

/* Char_Super 2: shock_move (Magnetic Shockwave) */
const Sint32 pl2c_super_02[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    6,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    6,           0,   841, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   840, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_super_02 ),
};

/* Char_Super 13: field_wait (Magnetic Forcefield) */
const Sint32 pl2c_super_13[] = {
	CG_FRAME( 0x80,    0,    6,           0,   841, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   842, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   843, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   844, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2,           0,   845, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME( 0x80,    0,    2, END_OF_DATA,   846, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl2c_super_13 ),
};

/* Char_Super 14: field_catch (Magnetic Forcefield) */
const Sint32 pl2c_super_14[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATK,  0x00,       0x19,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2,           0,   852, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
};

const Sint32 pl2c_super_14_loop[] = {
	CG_FRAME(    0,    0,    2,           0,   851, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   852, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_NEXT( pl2c_super_14_loop ),
};

/* Char_Super 15: field_throw (Magnetic Forcefield) */
const Sint32 pl2c_super_15[] = {
	CG_FRAME(    0,    0,    6,           0,   851, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 105 ),
	CG_FRAME(    0,    0,    6,           0,   841, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   840, 0xc0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_super_15 ),
};

/* Char_Super 21 */
const Sint32 pl2c_super_21[] = {
	CG_FRAME(    0,    0,    4,           0, 32847, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0, 119 ),
};

const Sint32 pl2c_super_21_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32858, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0, 119 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32858, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0, 119 ),
	CG_NEXT( pl2c_super_21_loop ),
};

/* Char_Super 16: tempest_init (Magnetic Tempest) */
const Sint32 pl2c_super_16[] = {
	CG_FRAME(    0,    0,    2,           0,   750, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   751, 0xb0,   0x00,  ATK,  0x00,       0x17,    0,   0,     0,      0,   3 ),
};

const Sint32 pl2c_super_16_loop[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,   752, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_super_16_loop ),
};

/* Char_Super 17: tempest_start (Magnetic Tempest) */
const Sint32 pl2c_super_17[] = {
	CG_FRAME(    0,    0,    2,           0,   753, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   754, 0xb0,   0x00,  ATK,  0x00,       0x21,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   755, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   756, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   757, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   758, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   752, 0xb0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_super_17 ),
};

/* Char_Super 18: tempest_move (Magnetic Tempest) */
const Sint32 pl2c_super_18[] = {
	CG_FRAME(    0,    0,    8,           0,   752, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   753, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   754, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   755, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   756, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   757, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   758, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   752, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   760, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   761, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   762, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   763, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   764, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   765, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   760, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   761, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    4,           0,   762, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    2,           0,   763, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    8, END_OF_DATA,   764, 0xb0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_super_18 ),
};

/* Char_Super 3: shl2c_02_shock, shl2c_02_field */
const Sint32 pl2c_super_03[] = {
	CG_FRAME(    0,    0,    4,           0, 32848, 0xc0,   0x00,  ATK,  0x00, 0x4c | CSE,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_super_03_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32849, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32850, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_super_03_loop ),
};

/* Char_Super 4: shl2c_02_shock_keep, shl2c_02_field_keep */
const Sint32 pl2c_super_04[] = {
	CG_FRAME(    0,    0,    4,           0, 32851, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32852, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32853, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32854, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32855, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32856, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_super_04 ),
};

/* Char_Super 5 */
const Sint32 pl2c_super_05[] = {
	CG_FRAME(    0,    0,    4,           0, 32847, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
};

const Sint32 pl2c_super_05_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32858, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32858, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_super_05_loop ),
};

/* Char_Super 19 */
const Sint32 pl2c_super_19[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
};

const Sint32 pl2c_super_19_loop[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 106 ),
	CG_NEXT( pl2c_super_19_loop ),
};

/* Char_Super 6
   Char_Super 22 */
const Sint32 pl2c_super_06[] = {
	CG_FRAME(    0,    0,    4,           0, 32859, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32860, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32861, 0xc0,   0x00,  ATK,  0x00,          0,    3,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_super_06 ),
};

/* Char_Super 20 */
const Sint32 pl2c_super_20[] = {
	CG_FRAME(    0,    0,    4,           0,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,    -1, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_super_20 ),
};

/* Char_Super 7 */
const Sint32 pl2c_super_07[] = {
	CG_FRAME(    0,    0,    4,           0, 32862, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
};

const Sint32 pl2c_super_07_loop[] = {
	CG_FRAME(    0,    0,    4,           0, 32864, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32864, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 107 ),
	CG_NEXT( pl2c_super_07_loop ),
};

/* Char_Super 8 */
const Sint32 pl2c_super_08[] = {
	CG_FRAME(    0,    0,    4,           0, 32865, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0, 32866, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA, 32867, 0xc0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_super_08 ),
};

/* Char_Special 19: field_init (Magnetic Forcefield)
   Char_Super 9
   Char_Super 10
   Char_Super 11
   Char_Super 12 */
const Sint32 pl2c_special_19[] = {
	CG_FRAME(    0,    0,    3,           0,  1052, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,  1053, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    0,    3,           0,  1058, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_FRAME(    0,    1,    2,           0,  1054, 0x80,   0x00,    0,  0x00,       0x15,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1055, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1056, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1057, 0x80,   0x00,    0,  0x00,       0x23,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1058, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1059, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1060, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
};

const Sint32 pl2c_special_19_loop[] = {
	CG_FRAME(    0,    0,    1,           0,  1061, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1,           0,  1062, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,  1063, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0, 112 ),
	CG_NEXT( pl2c_special_19_loop ),
};

/* Char_Special 20: field_wait (Magnetic Forcefield) */
const Sint32 pl2c_special_20[] = {
	CG_FRAME(    0,    0,    1,           0,  1058, 0x80,   0x00,  ATK,  0x3c,          0,    0,   0,     0, 0x0200, 113 ),
	CG_FRAME(    0,    0,    1,           0,  1058, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200, 113 ),
	CG_FRAME(    0,    0,   16,           0,  1058, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0, 0x0200, 113 ),
	CG_FRAME(    0,    0,    2,           0,  1073, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
	CG_FRAME(    0,    0,    2,           0,  1073, 0x80,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 113 ),
};

const Sint32 pl2c_special_20_loop[] = {
	CG_FRAME(    0,    1,    2, END_OF_DATA,  1075, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_special_20_loop ),
};

/* Char_Special 21: field_wait (Magnetic Forcefield) */
const Sint32 pl2c_special_21[] = {
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   374, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   375, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   376, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   377, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_NEXT( pl2c_special_21 ),
};

/* Char_Special 22: snapback_init (Snapback) */
const Sint32 pl2c_special_22[] = {
	CG_FRAME(    0,    0,    8,           0,   501, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    1,    3,           0,   502, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    1,           0,   503, 0xa0,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0,  17 ),
	CG_FRAME(    0,    0,    3,           0,   504, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0,  20 ),
	CG_FRAME(    0,    0,    6,           0,   505, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    8,           0,   506, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    9,           0,   507, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0, 111 ),
	CG_FRAME(    0,    0,    1, END_OF_DATA,   508, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,   3 ),
	CG_NEXT( pl2c_special_22 ),
};

/* Char_Special 23: sp_crouch_hp (crouching heavy punch (gamma assist)) */
const Sint32 pl2c_special_23[] = {
	CG_FRAME(    0,    0,    1,           0,   576, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   577, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    3,           0,   578, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    1,           0,   579, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    2,           0,   580, 0xa0,   0x00,  ATK,  0x00,       0x05,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,    3,           0,   581, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,    3,           0,   582, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 118 ),
	CG_FRAME(    0,    0,    3,           0,   583, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   584, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,    4,           0,   585, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  43 ),
	CG_FRAME(    0,    0,   10,           0,   586, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_FRAME(    0,    0,    4, END_OF_DATA,   587, 0xa0,   0x00,  ATE,  0x00,          0,    0,   0,     0,      0,  41 ),
	CG_NEXT( pl2c_special_23 ),
};

/* Char_Special 18: sp_jblast (Magnetic Blast) */
const Sint32 pl2c_special_18[] = {
	CG_FRAME(    0,    0,    1,           0,   621, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   622, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   623, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   624, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   625, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   626, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    2,           0,   627, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    3,           0,   628, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   629, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   630, 0xa0,   0x00,  ATK,  0x00,       0x03,    0,   0,     0,      0, 109 ),
	CG_FRAME(    0,    0,    1,           0,   631, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    1,    1,           0,   632, 0xa0,   0x00,  ATK,  0x00,       0x20,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    1,           0,   633, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    2,           0,   634, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   635, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    4,           0,   636, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   643, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   643, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   643, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   643, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3,           0,   642, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_FRAME(    0,    0,    3, END_OF_DATA,   637, 0xa0,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0, 110 ),
	CG_NEXT( pl2c_special_18 ),
};

/* Char_Change 0: act_change_tag_init */
const Sint32 pl2c_change_00[] = {
	CG_FRAME(    0,    0,    4,           0,   853, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   854, 0x30,   0x00,  ATK,  0x00,       0x10,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   855, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    4,           0,   856, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    1,    4,           0,   857, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_FRAME(    0,    0,    6, END_OF_DATA,   859, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_change_00 ),
};

/* Char_Change 2: act_change_out_bound */
const Sint32 pl2c_change_02[] = {
	CG_FRAME(    0,    0,    3,           0,   240, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_FRAME(    0,    0,    2, END_OF_DATA,   241, 0x90,   0x20,    0,  0x00,          0,    0,   0,     0,      0, 115 ),
	CG_NEXT( pl2c_change_02 ),
};

/* Char_Change 3: act_stand, act_change_call */
const Sint32 pl2c_change_03[] = {
	CG_FRAME(    0,    0,    4,           0,   853, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   854, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   855, 0x30,   0x00,    0,  0x00,       0x0b,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    0,    4,           0,   856, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_FRAME(    0,    1,    4,           0,   857, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
};

const Sint32 pl2c_change_03_loop[] = {
	CG_FRAME(    0,    0,    6, END_OF_DATA,   859, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   1 ),
	CG_NEXT( pl2c_change_03_loop ),
};

/* Char_Change 1: act_change_vcounter_init */
const Sint32 pl2c_change_01[] = {
	CG_FRAME( 0x80,    0,    6,           0,   698, 0x30,   0x00,  ATK,  0x00,          0,    0,   0,     0,      0,   0 ),
};

const Sint32 pl2c_change_01_loop[] = {
	CG_FRAME(    0,    0, -128, END_OF_DATA,   706, 0x30,   0x00,    0,  0x00,          0,    0,   0,     0,      0,   0 ),
	CG_NEXT( pl2c_change_01_loop ),
};

/* Char_Change 4: act_change_assist_end
   group 25 2
   group 25 3
   group 25 4 */
const Sint32 pl2c_change_04[] = {
	CG_FRAME(    0,    0,    1,           0,   343, 0x80,   0x00,    0,  0x00,       0x0b,    0,   0,     0, 0x0101,   3 ),
};

const Sint32 pl2c_change_04_loop[] = {
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   345, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,    4,           0,   344, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_FRAME(    0,    0,   15, END_OF_DATA,   343, 0x80,   0x00,    0,  0x00,          0,    0,   0,     0, 0x0101,   3 ),
	CG_NEXT( pl2c_change_04_loop ),
};

/* group 27 0 */
const Sint32 pl2c_grp27_00[] = {
	EF_FRAME(    0,    0,    5,           0,   979 ),
	EF_FRAME(    0,    0,    5,           0,   980 ),
	EF_FRAME(    0,    0,    5,           0,   981 ),
	EF_FRAME(    0,    0,    5,           0,   982 ),
	EF_FRAME(    0,    0,    5,           0,   983 ),
	EF_FRAME(    0,    0,    5,           0,   984 ),
	EF_FRAME(    0,    0,    5,           0,   985 ),
	EF_FRAME(    0,    0,    5,           0,   986 ),
	EF_FRAME(    0,    0,    5,           0,   987 ),
	EF_FRAME(    0,    0,    5,           0,   988 ),
	EF_FRAME(    0,    0,    5,           0,   989 ),
	EF_FRAME(    0,    0,    5,           0,   990 ),
	EF_FRAME(    0,    0,    5,           0,   991 ),
	EF_FRAME(    0,    0,    5,           0,   992 ),
	EF_FRAME(    0,    0,    5,           0,   993 ),
	EF_FRAME(    0,    0,    5,           0,   994 ),
	EF_FRAME(    0,    0,    5,           0,   979 ),
	EF_FRAME(    0,    0,    5,           0,   980 ),
	EF_FRAME(    0,    0,    5,           0,   981 ),
	EF_FRAME(    0,    0,    5,           0,   982 ),
	EF_FRAME(    0,    0,    5,           0,   983 ),
	EF_FRAME(    0,    0,    5,           0,   984 ),
	EF_FRAME(    0,    0,    5,           0,   985 ),
	EF_FRAME(    0,    0,    5,           0,   986 ),
	EF_FRAME(    0,    0,    5,           0,   987 ),
	EF_FRAME(    0,    0,    5,           0,   988 ),
	EF_FRAME(    0,    0,    5,           0,   989 ),
	EF_FRAME(    0,    0,    5,           0,   990 ),
	EF_FRAME(    0,    0,    5,           0,   995 ),
	EF_FRAME(    0,    0,    5,           0,   996 ),
	EF_FRAME(    0,    0,    5,           0,   997 ),
	EF_FRAME(    0,    0,    5,           0,   998 ),
	EF_FRAME(    0,    0,    5,           0,   999 ),
	EF_FRAME(    0,    0,    5,           0,  1000 ),
	EF_FRAME(    0,    0,    5,           0,   996 ),
	EF_FRAME(    0,    0,    5,           0,   997 ),
	EF_FRAME(    0,    0,    5,           0,   998 ),
	EF_FRAME(    0,    0,    5,           0,   999 ),
	EF_FRAME(    0,    0,    5,           0,  1000 ),
	EF_FRAME(    0,    0,    5, END_OF_DATA,  1001 ),
	CG_NEXT( pl2c_grp27_00 ),
};

/* group 27 1 */
const Sint32 pl2c_grp27_01[] = {
	EF_FRAME(    0,    0,    1,           0,  1015 ),
	EF_FRAME(    0,    0,    1,           0,  1019 ),
	EF_FRAME(    0,    0,    1,           0,  1016 ),
	EF_FRAME(    0,    0,    1,           0,  1020 ),
	EF_FRAME(    0,    0,    1,           0,  1017 ),
	EF_FRAME(    0,    0,    1,           0,  1021 ),
	EF_FRAME(    0,    0,    1,           0,  1018 ),
	EF_FRAME(    0,    0,    1,           0,  1022 ),
	EF_FRAME(    0,    0,    1,           0,  1015 ),
	EF_FRAME(    0,    0,    1,           0,  1023 ),
	EF_FRAME(    0,    0,    1,           0,  1016 ),
	EF_FRAME(    0,    0,    1,           0,  1024 ),
	EF_FRAME(    0,    0,    1,           0,  1017 ),
	EF_FRAME(    0,    0,    1,           0,  1025 ),
	EF_FRAME(    0,    0,    1,           0,  1018 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,  1026 ),
	CG_NEXT( pl2c_grp27_01 ),
};

/* group 27 2: pls2c_01_trail (2 + type) */
const Sint32 pl2c_grp27_02[] = {
	EF_FRAME(    0,    0,    4,           0,  1002 ),
	EF_FRAME(    0,    0,    4,           0,  1003 ),
	EF_FRAME(    0,    0,    4,           0,  1004 ),
	EF_FRAME(    0,    0,    4,           0,  1005 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1006 ),
	CG_NEXT( pl2c_grp27_02 ),
};

/* group 27 3 */
const Sint32 pl2c_grp27_03[] = {
	EF_FRAME(    0,    0,    4,           0,  1007 ),
	EF_FRAME(    0,    0,    4,           0,  1008 ),
	EF_FRAME(    0,    0,    4,           0,  1009 ),
	EF_FRAME(    0,    0,    4,           0,  1010 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA,  1011 ),
	CG_NEXT( pl2c_grp27_03 ),
};

/* group 27 4: pls2c_01_ground */
const Sint32 pl2c_grp27_04[] = {
	EF_FRAME(    0,    0,    4,           0, 32873 ),
	EF_FRAME(    0,    0,    4,           0, 32872 ),
	EF_FRAME(    0,    0,    4,           0, 32871 ),
	EF_FRAME(    0,    0,    4,           0, 32870 ),
	EF_FRAME(    0,    0,    4,           0, 32869 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32868 ),
	CG_NEXT( pl2c_grp27_04 ),
};

/* group 27 5: pls2c_01_blink, pls2c_02_piece (5 + type) */
const Sint32 pl2c_grp27_05[] = {
	EF_FRAME(    0,    0,    1,           0,  1012 ),
	EF_FRAME(    0,    0,    1,           0,  1013 ),
	EF_FRAME(    0,    0,    1, END_OF_DATA,  1014 ),
	CG_NEXT( pl2c_grp27_05 ),
};

/* group 27 6 */
const Sint32 pl2c_grp27_06[] = {
	EF_FRAME(    0,    0,    2,           0, 32768 ),
	EF_FRAME(    0,    0,    2,           0, 32769 ),
	EF_FRAME(    0,    0,    2,           0, 32770 ),
	EF_FRAME(    0,    0,    2,           0, 32771 ),
	EF_FRAME(    0,    0,    2,           0, 32772 ),
	EF_FRAME(    0,    0,    2,           0, 32773 ),
	EF_FRAME(    0,    0,    2,           0, 32774 ),
	EF_FRAME(    0,    0,    2,           0, 32775 ),
	EF_FRAME(    0,    0,    2,           0, 32776 ),
	EF_FRAME(    0,    0,    2,           0, 32777 ),
	EF_FRAME(    0,    0,    2,           0, 32778 ),
	EF_FRAME(    0,    0,    2,           0, 32779 ),
	EF_FRAME(    0,    0,    2,           0, 32780 ),
};

const Sint32 pl2c_grp27_06_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32781 ),
	CG_NEXT( pl2c_grp27_06_loop ),
};

/* group 27 7 */
const Sint32 pl2c_grp27_07[] = {
	EF_FRAME(    0,    0,    2,           0, 32768 ),
	EF_FRAME(    0,    0,    2,           0, 32769 ),
	EF_FRAME(    0,    0,    2,           0, 32770 ),
	EF_FRAME(    0,    0,    2,           0, 32771 ),
	EF_FRAME(    0,    0,    2,           0, 32772 ),
	EF_FRAME(    0,    0,    2,           0, 32773 ),
	EF_FRAME(    0,    0,    2,           0, 32774 ),
	EF_FRAME(    0,    0,    2,           0, 32775 ),
	EF_FRAME(    0,    0,    2,           0, 32776 ),
	EF_FRAME(    0,    0,    2,           0, 32777 ),
	EF_FRAME(    0,    0,    2,           0, 32782 ),
	EF_FRAME(    0,    0,    2,           0, 32783 ),
	EF_FRAME(    0,    0,    2,           0, 32784 ),
};

const Sint32 pl2c_grp27_07_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32785 ),
	CG_NEXT( pl2c_grp27_07_loop ),
};

/* group 27 8 */
const Sint32 pl2c_grp27_08[] = {
	EF_FRAME(    0,    0,    2,           0, 32768 ),
	EF_FRAME(    0,    0,    2,           0, 32769 ),
	EF_FRAME(    0,    0,    2,           0, 32770 ),
	EF_FRAME(    0,    0,    2,           0, 32771 ),
	EF_FRAME(    0,    0,    2,           0, 32772 ),
	EF_FRAME(    0,    0,    2,           0, 32773 ),
	EF_FRAME(    0,    0,    2,           0, 32774 ),
	EF_FRAME(    0,    0,    2,           0, 32775 ),
	EF_FRAME(    0,    0,    2,           0, 32776 ),
	EF_FRAME(    0,    0,    2,           0, 32777 ),
	EF_FRAME(    0,    0,    2,           0, 32786 ),
	EF_FRAME(    0,    0,    2,           0, 32787 ),
	EF_FRAME(    0,    0,    2,           0, 32788 ),
};

const Sint32 pl2c_grp27_08_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32789 ),
	CG_NEXT( pl2c_grp27_08_loop ),
};

/* group 27 9 */
const Sint32 pl2c_grp27_09[] = {
	EF_FRAME(    0,    0,    2,           0, 32768 ),
	EF_FRAME(    0,    0,    2,           0, 32769 ),
	EF_FRAME(    0,    0,    2,           0, 32770 ),
	EF_FRAME(    0,    0,    2,           0, 32771 ),
	EF_FRAME(    0,    0,    2,           0, 32772 ),
	EF_FRAME(    0,    0,    2,           0, 32773 ),
	EF_FRAME(    0,    0,    2,           0, 32774 ),
	EF_FRAME(    0,    0,    2,           0, 32775 ),
	EF_FRAME(    0,    0,    2,           0, 32776 ),
	EF_FRAME(    0,    0,    2,           0, 32777 ),
	EF_FRAME(    0,    0,    2,           0, 32790 ),
	EF_FRAME(    0,    0,    2,           0, 32791 ),
	EF_FRAME(    0,    0,    2,           0, 32792 ),
};

const Sint32 pl2c_grp27_09_loop[] = {
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32793 ),
	CG_NEXT( pl2c_grp27_09_loop ),
};

/* group 27 10
   group 27 14 */
const Sint32 pl2c_grp27_10[] = {
	EF_FRAME(    0,    0,    3,           0, 32794 ),
	EF_FRAME(    0,    0,    3,           0, 32795 ),
	EF_FRAME(    0,    0,    3,           0, 32796 ),
	EF_FRAME(    0,    0,    3,           0, 32797 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32798 ),
	CG_NEXT( pl2c_grp27_10 ),
};

/* group 27 11
   group 27 15 */
const Sint32 pl2c_grp27_11[] = {
	EF_FRAME(    0,    0,    3,           0, 32799 ),
	EF_FRAME(    0,    0,    3,           0, 32800 ),
	EF_FRAME(    0,    0,    3,           0, 32801 ),
	EF_FRAME(    0,    0,    3,           0, 32802 ),
	EF_FRAME(    0,    0,    3,           0, 32803 ),
	EF_FRAME(    0,    0,    3,           0, 32804 ),
	EF_FRAME(    0,    0,    3,           0, 32805 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32806 ),
	CG_NEXT( pl2c_grp27_11 ),
};

/* group 27 12
   group 27 16 */
const Sint32 pl2c_grp27_12[] = {
	EF_FRAME(    0,    0,    3,           0, 32807 ),
	EF_FRAME(    0,    0,    3,           0, 32808 ),
	EF_FRAME(    0,    0,    3,           0, 32809 ),
	EF_FRAME(    0,    0,    3,           0, 32810 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32811 ),
	CG_NEXT( pl2c_grp27_12 ),
};

/* group 27 13
   group 27 17 */
const Sint32 pl2c_grp27_13[] = {
	EF_FRAME(    0,    0,    3,           0, 32812 ),
	EF_FRAME(    0,    0,    3,           0, 32813 ),
	EF_FRAME(    0,    0,    3,           0, 32814 ),
	EF_FRAME(    0,    0,    3,           0, 32815 ),
	EF_FRAME(    0,    0,    3,           0, 32816 ),
	EF_FRAME(    0,    0,    3,           0, 32817 ),
	EF_FRAME(    0,    0,    3,           0, 32818 ),
	EF_FRAME(    0,    0,    4, END_OF_DATA, 32819 ),
	CG_NEXT( pl2c_grp27_10 ),
};

/* group 27 18: pls2c_01_frame */
const Sint32 pl2c_grp27_18[] = {
	EF_FRAME(    0,    0,    8,           0, 32820 ),
	EF_FRAME(    0,    0,    8,           0, 32821 ),
	EF_FRAME(    0,    0,    8,           0, 32822 ),
	EF_FRAME(    0,    0,    8,           0, 32823 ),
	EF_FRAME(    0,    0,    8,           0, 32824 ),
	EF_FRAME(    0,    0,    8,           0, 32825 ),
	EF_FRAME(    0,    0,    8, END_OF_DATA, 32826 ),
	CG_NEXT( pl2c_grp27_18 ),
};
